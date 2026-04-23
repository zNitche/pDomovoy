#include "../../includes/voltmeter.h"

#include "../../includes/globals.h"
#include "../../includes/defines.h"
#include "pdomovoy_common/voltmeter.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"


bool _voltmeter_cb(repeating_timer_t* t) {
    float v = read_adc_voltage(1.3, PD_VOLTMETER_ADC_ID);

    g_battery_voltage = v;

    return true;
}

void start_voltmeter_bg() {
    add_repeating_timer_ms(3000, _voltmeter_cb, NULL, &g_adc_voltmeter_timer);
}

void stop_voltmeter_bg() { cancel_repeating_timer(&g_adc_voltmeter_timer); }
