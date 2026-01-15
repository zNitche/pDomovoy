#include <stdio.h>
#include "pico/stdlib.h"  

int main() {
    stdio_init_all();

    while (true) {
        printf("Hello Pico Door Alarm!\n");
        sleep_ms(1000);
    }
}