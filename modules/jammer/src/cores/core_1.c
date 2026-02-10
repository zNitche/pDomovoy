#include "../../includes/core_1.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../includes/acceleration_readings.h"
#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "pico_adxl345/adxl345.h"

void _send_event_to_core_0(enum DeviceStatus status) {
    mc_event_item event;
    event.status = status;

    queue_add_blocking(&g_core0_events_queue, &event);
}

void _get_initial_accel_mean(ADXL345I2C* adxl345_i2c, float output[3]) {
    const int readings_count = 20;
    float total_accel[readings_count][3] = {};

    get_bunch_of_accel_readings(adxl345_i2c, readings_count, total_accel, 20);
    get_accel_readings_mean(total_accel, readings_count, output);
}

bool _is_reading_above_initial_mean(float initial_mean, float mean,
                                    float trigger_factor) {
    return fabs(initial_mean - mean) > trigger_factor;
}

bool _check_for_alarm_trigger(ADXL345I2C* adxl345_i2c,
                              float initial_accel_mean[3]) {
    const int readings_count = 20;
    const float trigger_factor = 0.3;

    float total_accel[readings_count][3] = {};
    float tmp_accel[3] = {0.0};
    float accel_mean[3] = {0.0};

    get_bunch_of_accel_readings(adxl345_i2c, readings_count, total_accel, 20);
    get_accel_readings_mean(total_accel, readings_count, accel_mean);

    for (int i = 0; i < 3; i++) {
        const bool is_above_mean = _is_reading_above_initial_mean(
            initial_accel_mean[i], accel_mean[i], trigger_factor);

        if (is_above_mean) {
            return true;
        }
    }

    return false;
}

void core_1() {
    float initial_accel_mean[3] = {0.0};

    ADXL345I2C adxl345_i2c = {i2c0, 0x53, 0, 1};

    adxl345_setup_i2c(adxl345_i2c);
    bool connected = adxl345_check_connection(adxl345_i2c);

    if (!connected) {
        _send_event_to_core_0(PDA_ADXL345_ERROR);

        return;
    }

    adxl345_set_measurements_range(adxl345_i2c, ADXL345_RANGE_4G);
    adxl345_start_measurements(adxl345_i2c);

    _get_initial_accel_mean(&adxl345_i2c, initial_accel_mean);

    _send_event_to_core_0(PDA_ADXL345_OK);

    while (true) {
        bool is_triggered =
            _check_for_alarm_trigger(&adxl345_i2c, initial_accel_mean);
        // printf("[core_1] is_alarm_trigger %d \n", is_triggered);

        if (is_triggered) {
            _send_event_to_core_0(PDA_ACCELERATION_TRIGGER);
        }

        sleep_ms(100);
    }

    adxl345_stop_measurements(adxl345_i2c);

    return;
}
