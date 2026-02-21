#include "../../includes/core_0.h"

#include <stdbool.h>
#include <stdio.h>

#include "../../includes/callbacks.h"
#include "../../includes/debug_print.h"
#include "../../includes/defines.h"
#include "../../includes/delegates.h"
#include "../../includes/globals.h"
#include "../../includes/led_blink.h"
#include "../../includes/pwm.h"
#include "../../includes/types.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

int _is_battery_voltage_low() {
    // 0 - undefined
    // 1 - false
    // 2 - true

    static float voltages[BATTER_VOLTAGE_READINGS] = {0.0};
    static int readings_count = 0;

    voltages[readings_count + 1] = g_battery_voltage;
    readings_count += 1;

    if (readings_count >= BATTER_VOLTAGE_READINGS) {
        float readings_mean = 0.0;

        for (int i = 0; i < BATTER_VOLTAGE_READINGS; i++) {
            readings_mean += voltages[i];
            voltages[i] = 0.0;
        }

        // reset array
        // memset(voltages, 0.0, sizeof(voltages));

        readings_mean = readings_mean / readings_count;
        readings_count = 0;

        return readings_mean <= MIN_BATTERY_VOLTAGE ? 2 : 1;
    }

    return 0;
}

void _check_battery_level() {
    const int battery_status = _is_battery_voltage_low();

    if (battery_status == 2) {
        debug_print("[core_0] low battery\n");

        if (!g_detected_low_battery_voltage) {
            blink_status_untill_start(2000,
                                      blink_status_led_for_standby_callback,
                                      &g_status_led_blink_timer, true);

            g_detected_low_battery_voltage = true;
        }
    } else if (battery_status == 1) {
        debug_print("[core_0] high battery\n");

        blink_status_untill_stop(&g_status_led_blink_timer);
        g_detected_low_battery_voltage = false;
    }
}

void _wait_for_alarm_standby() {
    g_btn_blocked = true;

    debug_print("[core_0] alarm standby init...\n");

    // wait 30s
    blink_blocking(PDA_STATUS_LED_PIN, 30, 1000);
    enable_alarm_standby();

    g_btn_blocked = false;
}

void _process_event(mc_event_item* event) {
    switch (event->status) {
    case PDA_ADXL345_OK:
        blink_status_untill_stop(&g_status_led_blink_timer);
        debug_print("[core_0] adxl345 ok\n");

        break;
    case PDA_ADXL345_ERROR:
        blink_status_untill_start(50, blink_status_led_for_standby_callback,
                                  &g_status_led_blink_timer, true);

        g_sensor_error = false;
        debug_print("[core_0] adxl345 fail\n");

        break;
    case PDA_ACCELERATION_TRIGGER:
        g_alarm_state = ALARM_STATE_TRIGGERED;

        break;
    case PDA_STANDBY_PREP:
        blink_status_untill_start(100, blink_status_led_for_standby_callback,
                                  &g_status_led_blink_timer, true);

        break;
    case PDA_STANDBY_READY:
        blink_status_untill_stop(&g_status_led_blink_timer);

        break;
    default:
        break;
    }

    event->status = 0;
}

void core_0() {
    mc_event_item event_item;

    debug_print("[core_0] started, waiting\n");

    while (true) {
        const bool got_event =
            queue_try_remove(&g_core0_events_queue, &event_item);

        if (got_event) {
            _process_event(&event_item);
            continue;
        }

        if (g_alarm_state == ALARM_STATE_STANDBY_INIT) {
            _wait_for_alarm_standby();
            continue;
        }

        if (!g_sensor_error && g_alarm_state == ALARM_STATE_NONE) {
            _check_battery_level();
        }

        if (g_alarm_state == ALARM_STATE_TRIGGERED) {
            debug_print("[core_0] alarm triggered\n");

            enable_pwm_irq_on_pin(PDA_BUZZER_PIN);

            while (g_alarm_state == ALARM_STATE_TRIGGERED) {
                sleep_ms(250);
            }

            disable_pwm_irq_on_pin(PDA_BUZZER_PIN);
        }

        sleep_ms(250);
    }

    return;
}