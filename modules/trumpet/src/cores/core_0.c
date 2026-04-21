#include "../../includes/core_0.h"

#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "../../includes/i2c_devices.h"
#include "pdomovoy_common/debug_print.h"
#include "pico_aht20/aht20.h"
#include "pico_ssd1306/ssd1306.h"

void core_0() {
    debug_print("[core_0] started, waiting\n");

    AHT20_I2C aht20_i2c = init_aht20();
    SSD1306_I2C ssd1306_i2c = init_ssd1306();

    while (true) {
        SSD1306_Frame frame;
        float readings[2] = {0.0};

        aht20_get_measurements(aht20_i2c, readings);

        ssd1306_prepare_frame(&frame);

        char temp_str[12];
        char humidity_str[12];

        snprintf(temp_str, sizeof(temp_str), "temp= %.2f", readings[0]);
        ssd1306_render_string(&frame, 0, 13, temp_str, 2, true);

        snprintf(temp_str, sizeof(temp_str), "humi= %.2f", readings[1]);
        ssd1306_render_string(&frame, 0, 22, temp_str, 2, true);

        ssd1306_render(ssd1306_i2c, &frame);

        sleep_ms(250);
    }

    return;
}
