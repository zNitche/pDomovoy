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

void send_event_to_core_0(enum DeviceStatus status) {
    mc_event_item event;
    event.status = status;

    queue_add_blocking(&core0_events_queue, &event);
}

void get_initial_accel_mean(ADXL345I2C* adxl345_i2c, float output[3]) {
    const int readings_count = 20;
    float total_accel[readings_count][3] = {};

    get_bunch_of_accel_readings(adxl345_i2c, readings_count, total_accel, 20);
    get_accel_readings_mean(total_accel, readings_count, output);
}

bool is_reading_above_initial_mean(float initial_mean, float mean,
                                   float trigger_factor) {
    return fabs(initial_mean - mean) > trigger_factor;
}

bool check_for_alarm_trigger(ADXL345I2C* adxl345_i2c,
                             float initial_accel_mean[3]) {
    const int readings_count = 20;
    const float trigger_factor = 0.3;

    float total_accel[readings_count][3] = {};
    float tmp_accel[3] = {0.0};
    float accel_mean[3] = {0.0};

    get_bunch_of_accel_readings(adxl345_i2c, readings_count, total_accel, 20);
    get_accel_readings_mean(total_accel, readings_count, accel_mean);

    for (int i = 0; i < 3; i++) {
        const bool is_above_mean = is_reading_above_initial_mean(
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
        send_event_to_core_0(PDA_ADXL345_ERROR);

        return;
    }

    enum ADXL345Ranges sensor_range = RANGE_4G;
    adxl345_set_measurements_range(adxl345_i2c, sensor_range);
    adxl345_start_measurements(adxl345_i2c);

    get_initial_accel_mean(&adxl345_i2c, initial_accel_mean);

    send_event_to_core_0(PDA_ADXL345_OK);

    while (true) {
        bool is_triggered =
            check_for_alarm_trigger(&adxl345_i2c, initial_accel_mean);
        printf("[core_1] is_alarm_trigger %d \n", is_triggered);

        if (is_triggered) {
            send_event_to_core_0(PDA_ALARM_TRIGGERED);
        }

        sleep_ms(100);
    }

    adxl345_stop_measurements(adxl345_i2c);

    return;
}
