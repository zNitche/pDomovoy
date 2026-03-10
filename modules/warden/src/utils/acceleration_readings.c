#include "../../includes/acceleration_readings.h"
#include "../../includes/types.h"

#include "pico_adxl345/adxl345.h"

accelerometer_reading get_accel_readings_mean(accelerometer_reading input[],
                                               int readings_count) {
    accelerometer_reading means = {};
    accelerometer_reading tmp_readings = {};

    for (int reading_id = 0; reading_id < readings_count; reading_id++) {
        tmp_readings.x += input[reading_id].x;
        tmp_readings.y += input[reading_id].y;
        tmp_readings.z += input[reading_id].z;
    }

    means.x = tmp_readings.x / readings_count;
    means.y = tmp_readings.y / readings_count;
    means.z = tmp_readings.z / readings_count;

    return means;
}

void get_bunch_of_accel_readings(ADXL345I2C* adxl345_i2c,
                                 accelerometer_reading total_accel[],
                                 int readings_count, int delay) {
    float current_accel_buff[3] = {0.0};

    for (int i = 0; i < readings_count; i++) {
        adxl345_get_readings(*adxl345_i2c, current_accel_buff);

        total_accel[i].x = current_accel_buff[0];
        total_accel[i].y = current_accel_buff[1];
        total_accel[i].z = current_accel_buff[2];

        sleep_ms(delay);
    }
}
