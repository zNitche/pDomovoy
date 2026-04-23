#include "pdomovoy_common/voltmeter.h"

#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"

void init_adc_voltmeter(int gpio, int adc) {
    adc_gpio_init(gpio);
    adc_select_input(adc);
}

float read_adc_voltage() {
    const float conversion_factor = 3.3f / (1 << 12);
    uint16_t result = adc_read();

    return result * conversion_factor * 2;
};
