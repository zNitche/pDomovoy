#include "../includes/i2c_devices.h"

#include "../includes/defines.h"
#include "pico_aht20/aht20.h"
#include "pico_aht20/defines.h"
#include "pico_ssd1306/ssd1306.h"

AHT20_I2C init_aht20() {
    AHT20_I2C aht20_i2c = {.i2c = PD_AHT20_I2C,
                           .device_address = PICO_AHT20_I2C_DEFAULT_ADDRESS,
                           .sda_pin = PD_AHT20_SDA_PIN,
                           .scl_pin = PD_AHT20_SCL_PIN};

    aht20_setup_i2c(aht20_i2c);
    sleep_ms(100);

    aht20_reset(aht20_i2c);
    sleep_ms(150);

    aht20_initialize(aht20_i2c);
    sleep_ms(150);

    return aht20_i2c;
}

SSD1306_I2C init_ssd1306() {
    SSD1306_I2C ssd1306_i2c = {.i2c = PD_SSD1306_I2C,
                               .device_address =
                                   PICO_SSD1306_I2C_DEFAULT_ADDRESS,
                               .sda_pin = PD_SSD1306_SDA_PIN,
                               .scl_pin = PD_SSD1306_SCL_PIN};

    ssd1306_setup_i2c(ssd1306_i2c);
    ssd1306_init(ssd1306_i2c);
    ssd1306_clear(ssd1306_i2c);

    return ssd1306_i2c;
}
