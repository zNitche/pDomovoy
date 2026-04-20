#include <stdbool.h>

#include "../includes/callbacks.h"
#include "../includes/core_0.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "pdomovoy_common/button.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/pwm.h"
#include "pdomovoy_common/version.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

void init_gpio_peripherals() {
    // NAVIGATION BUTTONS
    init_button_irq(PD_PREVIOUS_BUTTON_PIN, true);
    init_button_irq(PD_APPLY_BUTTON_PIN, true);
    init_button_irq(PD_NEXT_BUTTON_PIN, true);

    gpio_set_irq_callback(&gpio_irq_callback);
}

int main() {
    stdio_init_all();

    int rc = cyw43_arch_init();
    hard_assert(rc == PICO_OK);

    if (DEBUG) {
        // DEBUG waiting for input
        getchar();
    }

    debug_print("[pD - TRUMPET] %s\n", TRUMPET_VERSION);

    init_gpio_peripherals();

    // core_0 for device handling
    core_0();

    return 0;
}