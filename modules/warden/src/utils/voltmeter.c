#include "../../includes/voltmeter.h"

#include "../../includes/globals.h"
#include "pdomovoy_common/voltmeter.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"


bool _voltmeter_cb(__unused repeating_timer_t* t) {
    float v = read_adc_voltage();

    g_battery_voltage = v;

    return true;
}

void start_voltmeter_bg() {
    add_repeating_timer_ms(1000, _voltmeter_cb, NULL, &g_adc_voltmeter_timer);
}

void stop_voltmeter_bg() { cancel_repeating_timer(&g_adc_voltmeter_timer); }
