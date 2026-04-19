#include "../includes/callbacks.h"

#include "../includes/defines.h"
#include "../includes/globals.h"
#include "pdomovoy_common/button.h"
#include "pdomovoy_common/debug_print.h"

void action_button_callback(uint gpio, uint32_t event) {
    static uint32_t last_click_time = 0;
    static int clicks_in_row = 0;

    if (!debounce_push_button(event, CLICK_DEBOUNCE_TIME)) {
        return;
    }

    debug_print("action button click\n");

    if (g_btn_blocked) {
        return;
    }
}
