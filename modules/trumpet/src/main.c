#include <stdbool.h>

#include "../includes/callbacks.h"
#include "../includes/core_0.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "../includes/voltmeter.h"
#include "../includes/bluetooth.h"
#include "hardware/adc.h"
#include "pdomovoy_common/button.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/pwm.h"
#include "pdomovoy_common/version.h"
#include "pdomovoy_common/voltmeter.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

void init_gpio_peripherals() {
    // NAVIGATION BUTTONS
    init_button_irq(PD_PREVIOUS_BUTTON_PIN, true);
    init_button_irq(PD_APPLY_BUTTON_PIN, true);
    init_button_irq(PD_NEXT_BUTTON_PIN, true);

    gpio_set_irq_callback(&gpio_irq_callback);
}

void init_voltmeter() {
    init_adc_voltmeter(PD_VOLTMETER_ADC_PIN);
    start_voltmeter_bg();
}

int main() {
    stdio_init_all();

    if (DEBUG) {
        // DEBUG waiting for input
        getchar();
    }

    if (cyw43_arch_init()) {
        printf("failed to initialise cyw43\n");
        return 1;
    }

    debug_print("[pD - TRUMPET] %s\n", TRUMPET_VERSION);

    adc_init();

    init_gpio_peripherals();
    init_voltmeter();

    init_ble();
    turn_ble_on();

    // core_0 for device handling
    core_0();

    return 0;
}