#include "../../includes/core_0.h"

#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "../../includes/i2c_devices.h"
#include "../../includes/pages.h"
#include "pdomovoy_common/debug_print.h"
#include "pico_aht20/aht20.h"
#include "pico_ssd1306/ssd1306.h"
#include <stdio.h>

void process_current_page(SSD1306_Frame* frame) {
    const PageData* current_page = get_current_page(); 

    ssd1306_render_string(frame, 0, 22, current_page->title, 2, true);
}

void core_0() {
    debug_print("[core_0] started, waiting\n");

    g_aht20_i2c = init_aht20();
    g_ssd1306_i2c = init_ssd1306();

    while (true) {
        SSD1306_Frame frame;
        // float readings[2] = {0.0};

        // aht20_get_measurements(aht20_i2c, readings);

        ssd1306_prepare_frame(&frame);

        // char temp_str[9];
        // char humidity_str[9];

        // snprintf(temp_str, sizeof(temp_str), "temp= %d", (int)readings[0]);
        // ssd1306_render_string(&frame, 0, 13, temp_str, 2, true);

        // snprintf(temp_str, sizeof(temp_str), "humi= %d", (int)readings[1]);
        // ssd1306_render_string(&frame, 0, 22, temp_str, 2, true);
        
        process_current_page(&frame);
        ssd1306_render(ssd1306_i2c, &frame);

        sleep_ms(250);
    }

    return;
}
