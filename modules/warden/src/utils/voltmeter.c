#include "../../includes/voltmeter.h"

#include "../../includes/globals.h"
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

bool _voltmeter_cb(__unused repeating_timer_t* t) {
    float v = read_adc_voltage();

    g_battery_voltage = v;

    return true;
}

void start_voltmeter_bg() {
    add_repeating_timer_ms(1000, _voltmeter_cb, NULL, &g_adc_voltmeter_timer);
}

void stop_voltmeter_bg() { cancel_repeating_timer(&g_adc_voltmeter_timer); }
