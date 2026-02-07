#include "../../includes/core_1.h"

#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#include "../../includes/acceleration_readings.h"
#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico_adxl345/adxl345.h"

void get_initial_accel_mean(ADXL345I2C* adxl345_i2c, float output[3]) {
    float total_accel[20][3] = {0.0};

    get_bunch_of_accel_readings(adxl345_i2c, 20, total_accel, 20);
    get_accel_readings_mean(total_accel, 20, output);
}

bool check_for_alarm_trigger(ADXL345I2C* adxl345_i2c,
                             float initial_accel_mean[3]) {
    const float trigger_factor = 0.3;

    float total_accel[30][3] = {0.0};
    float tmp_accel[3] = {0.0};
    float accel_mean[3] = {0.0};

    get_bunch_of_accel_readings(adxl345_i2c, 30, total_accel, 20);
    get_accel_readings_mean(total_accel, 30, accel_mean);

    bool x_triggered =
        fabs(initial_accel_mean[0] - accel_mean[0]) > trigger_factor;
    bool y_triggered =
        fabs(initial_accel_mean[1] - accel_mean[1]) > trigger_factor;
    bool z_triggered =
        fabs(initial_accel_mean[2] - accel_mean[2]) > trigger_factor;

    return x_triggered || y_triggered || z_triggered;
}

void core_1() {
    float initial_accel_mean[3] = {0.0};

    ADXL345I2C adxl345_i2c = {i2c0, 0x53, 0, 1};

    adxl345_setup_i2c(adxl345_i2c);
    bool connected = adxl345_check_connection(adxl345_i2c);

    if (!connected) {
        multicore_fifo_push_blocking(PDA_MCS_ADXL345_ERROR);
        return;
    }

    enum ADXL345Ranges sensor_range = RANGE_4G;
    adxl345_set_measurements_range(adxl345_i2c, sensor_range);
    adxl345_start_measurements(adxl345_i2c);

    get_initial_accel_mean(&adxl345_i2c, initial_accel_mean);

    multicore_fifo_push_blocking(PDA_MCS_ADXL345_OK);

    while (true) {
        bool is_triggered =
            check_for_alarm_trigger(&adxl345_i2c, initial_accel_mean);
        printf("[core_1] is_alarm_trigger %d \n", is_triggered);

        if (is_triggered) {
            multicore_fifo_push_blocking(PDA_MCS_ALARM_TRIGGERED);
        }

        sleep_ms(100);
    }

    adxl345_stop_measurements(adxl345_i2c);

    return;
}
