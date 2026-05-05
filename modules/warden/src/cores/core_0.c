#include "../../includes/core_0.h"

#include <stdbool.h>
#include <stdio.h>

#include "../../includes/bluetooth/core.h"
#include "../../includes/callbacks.h"
#include "../../includes/defines.h"
#include "../../includes/delegates.h"
#include "../../includes/globals.h"
#include "../../includes/led_blink.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/helpers.h"
#include "pdomovoy_common/pwm.h"
#include "pdomovoy_common/queue.h"
#include "pdomovoy_common/types.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

void _send_details_to_trumpet() {
    static const uint32_t interval_ms = 10000;
    static uint32_t next_runtime = 0;

    if (!should_execute_repeating_function(&next_runtime, interval_ms)) {
        return;
    }

    pd_enqueue(&g_bt_functions_queue, pd_bt_send_alarm_state);
    pd_enqueue(&g_bt_functions_queue, pd_bt_send_battery_voltage);
}

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
            blink_status_untill_start(-2000,
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

    blink_blocking(PD_STATUS_LED_PIN, PD_WAIT_FOR_ALARM_ARM, 1000);
    arm_alarm();

    g_btn_blocked = false;
}

void _process_event(MulticoreEvent* event) {
    switch (event->status) {
    case PD_ADXL345_OK:
        blink_status_untill_stop(&g_status_led_blink_timer);
        debug_print("[core_0] adxl345 ok\n");

        break;
    case PD_ADXL345_ERROR:
        blink_status_untill_start(-50, blink_status_led_for_standby_callback,
                                  &g_status_led_blink_timer, true);

        g_sensor_error = false;
        debug_print("[core_0] adxl345 fail\n");

        break;
    case PD_ACCELERATION_TRIGGER:
        set_alarm_state(ALARM_STATE_TRIGGERED);

        break;
    case PD_STANDBY_PREP:
        blink_status_untill_start(-100, blink_status_led_for_standby_callback,
                                  &g_status_led_blink_timer, true);

        break;
    case PD_STANDBY_READY:
        blink_status_untill_stop(&g_status_led_blink_timer);

        break;
    default:
        break;
    }

    event->status = 0;
}

void core_0() {
    MulticoreEvent event_item;

    debug_print("[core_0] started, waiting\n");

    while (true) {
        const bool got_event =
            queue_try_remove(&g_core0_events_queue, &event_item);

        if (got_event) {
            _process_event(&event_item);
            continue;
        }

        // handle disarm via trumpet
        if (g_alarm_disarm_requested) {
            debug_print("[core_0] alarm disarm requested\n");

            g_alarm_disarm_requested = false;
            disarm_alarm();
        }

        if (g_alarm_state == ALARM_STATE_STANDBY_INIT) {
            _wait_for_alarm_standby();
            continue;
        }

        if (g_alarm_state == ALARM_STATE_NONE) {
            _check_battery_level();
        }

        if (g_alarm_state == ALARM_STATE_TRIGGERED) {
            debug_print("[core_0] alarm triggered\n");

            enable_pwm_irq_on_pin(PD_BUZZER_PIN);

            while (g_alarm_state == ALARM_STATE_TRIGGERED) {
                sleep_ms(500);
            }

            disable_pwm_irq_on_pin(PD_BUZZER_PIN);
        }

        pd_bt_characteristics_discovery_loop();
        _send_details_to_trumpet();

        sleep_ms(250);
    }

    return;
}