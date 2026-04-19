#include "../includes/defines.h"
#include "pdomovoy_common/debug_print.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico_ssd1306/ssd1306.h"
#include <stdio.h>

int main() {
    stdio_init_all();

    int rc = cyw43_arch_init();
    hard_assert(rc == PICO_OK);

    while (true) {
        debug_print("trumpet\n");
        sleep_ms(1000);
    }
}