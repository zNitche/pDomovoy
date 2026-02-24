#include "pico/cyw43_arch.h"
#include <stdio.h>

int main() {
    stdio_init_all();

    int rc = cyw43_arch_init();
    hard_assert(rc == PICO_OK);

    while (true) {
        printf("trumpet\n");
        sleep_ms(1000);
    }
}