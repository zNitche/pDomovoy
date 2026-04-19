#include <stdbool.h>

#include "../includes/core_0.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "../includes/callbacks.h"
#include "pdomovoy_common/button.h"
#include "pdomovoy_common/debug_print.h"
#include "pdomovoy_common/pwm.h"
#include "pdomovoy_common/version.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

void init_peripherals() {
    // BUTTON
    init_button_irq(PD_ACTION_BUTTON_PIN, action_button_callback);
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

    init_peripherals();

    // core_0 for device handling
    core_0();

    return 0;
}