#include "pdomovoy_common/voltmeter.h"

#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "pico/stdlib.h"

void init_adc_voltmeter(int gpio) { adc_gpio_init(gpio); }

float read_adc_voltage(float voltage_divider_ratio, int adc) {
    const float conversion_factor = 3.3f / (1 << 12);

    adc_select_input(adc);
    uint16_t result = adc_read();

    return result * conversion_factor * voltage_divider_ratio;
};
