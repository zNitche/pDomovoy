#include <stdio.h>
#include "../includes/callbacks.h"
#include "../includes/button.h"


void action_button_callback(uint gpio, uint32_t event) {
    if (!debounce_push_button(event)) {
        return;
    }

    printf("callback\n");
}
