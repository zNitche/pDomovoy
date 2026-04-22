#include "../../includes/core_0.h"

#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "../../includes/i2c_devices.h"
#include "../../includes/pages.h"
#include "../../includes/types.h"
#include "pdomovoy_common/debug_print.h"
#include <stdio.h>

void process_current_page(SSD1306_Frame* frame) {
    PageData* current_page = get_current_page();

    ssd1306_render_string(frame, 0, 0, "*", 0, false);
    ssd1306_render_string(frame, 16, 0, current_page->title, 2, true);
    ssd1306_render_string(frame, 119, 0, "*", 0, false);

    if (current_page->handler != NULL) {
        current_page->handler(frame);
    }
}

void core_0() {
    debug_print("[core_0] started, waiting\n");

    g_aht20_i2c = init_aht20();
    g_ssd1306_i2c = init_ssd1306();

    while (true) {
        SSD1306_Frame frame;

        ssd1306_prepare_frame(&frame);

        process_current_page(&frame);
        ssd1306_render(g_ssd1306_i2c, &frame);

        sleep_ms(250);
    }

    return;
}
