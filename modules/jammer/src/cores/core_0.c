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

void _wait_for_alarm_standby() {
    g_btn_blocked = true;

    debug_print("[core_0] alarm standby init...\n");

    // wait 30s
    blink_blocking(PDA_STATUS_LED_PIN, 30, 1000);
    enable_alarm_standby();

    g_alarm_standby_init = false;
    g_btn_blocked = false;
}

void _process_event(mc_event_item* event) {
    switch (event->status) {
    case PDA_ADXL345_OK:
        debug_print("[core_0] adxl345 ok\n");

        break;
    case PDA_ADXL345_ERROR:
        debug_print("[core_0] adxl345 fail\n");

        break;
    case PDA_ACCELERATION_TRIGGER:
        g_alarm_triggered = true;

        break;
    case PDA_STANDBY_PREP:
        blink_untill_start(100, blink_status_led_for_standby_callback,
                           &g_status_led_blink_timer);

        break;
    case PDA_STANDBY_READY:
        blink_untill_stop(PDA_STATUS_LED_PIN, &g_status_led_blink_timer);

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

        if (g_alarm_standby_init) {
            _wait_for_alarm_standby();
            continue;
        }

        printf("voltage: %f\n", g_battery_voltage);

        if (g_alarm_in_standby & g_alarm_triggered) {
            debug_print("[core_0] alarm triggered\n");

            enable_pwm_irq_on_pin(PDA_BUZZER_PIN);

            while (g_alarm_triggered) {
                sleep_ms(250);
            }

            disable_pwm_irq_on_pin(PDA_BUZZER_PIN);
        }

        sleep_ms(250);
    }

    return;
}