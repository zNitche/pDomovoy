#include <math.h>
#include <stdbool.h>

#include "../includes/callbacks.h"
#include "../includes/core_0.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "../includes/led_blink.h"
#include "../includes/voltmeter.h"
#include "hardware/adc.h"
#include "pdomovoy_common/button.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/pwm.h"
#include "pdomovoy_common/version.h"
#include "pdomovoy_common/voltmeter.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

void init_peripherals() {
    // BUZZER
    set_pwm_irq_callback(PD_BUZZER_PIN, alarm_buzzer_irq_callback);

    // BUTTON
    init_button_irq(PD_ACTION_BUTTON_PIN, true);

    gpio_set_irq_callback(&gpio_irq_callback);

    // LED
    gpio_init(PD_STATUS_LED_PIN);
    gpio_set_dir(PD_STATUS_LED_PIN, true);
}

void init_mc_queues() {
    queue_init(&g_core0_events_queue, sizeof(MulticoreEvent), 2);
    queue_init(&g_core1_events_queue, sizeof(MulticoreEvent), 2);
}

void init_voltmeter() {
    init_adc_voltmeter(PD_VOLTMETER_ADC_PIN, PD_VOLTMETER_ADC_ID);
    start_voltmeter_bg();
}

int main() {
    stdio_init_all();
    adc_init();

    int rc = cyw43_arch_init();
    hard_assert(rc == PICO_OK);

    if (DEBUG) {
        // DEBUG waiting for input
        getchar();
    }

    debug_print("[pD - WARDEN] %s\n", WARDEN_VERSION);

    init_peripherals();
    init_mc_queues();

    init_voltmeter();

    cyw34_blink_untill_start(10000, &g_onboard_led_blink_timer, true);

    // core_0 for device handling
    core_0();

    cyw34_blink_untill_stop(&g_onboard_led_blink_timer);
    stop_voltmeter_bg();

    return 0;
}