#include "../../includes/core_0.h"

#include <stdbool.h>

#include "../../includes/debug_print.h"
#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "../../includes/led_blink.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

void core_0() {
    mc_event_item event_item;

    debug_print("[core_0] started, waiting\n");

    while (true) {
        const bool got_event =
            queue_try_remove(&g_core0_events_queue, &event_item);

        if (got_event) {
            switch (event_item.status) {
            case PDA_ADXL345_OK:
                debug_print("[core_0] adxl345 ok\n");

                break;
            case PDA_ADXL345_ERROR:
                debug_print("[core_0] adxl345 fail\n");

                break;
            case PDA_ACCELERATION_TRIGGER:
                g_alarm_triggered = true;

                break;
            default:
                break;
            }

            event_item.status = 0;
        }

        if (g_alarm_in_standby & g_alarm_triggered) {
            debug_print("[core_0] alarm triggered\n");

            blink_blocking(PDA_STATUS_LED_PIN, 5, 100);
        }

        sleep_ms(250);
    }

    return;
}