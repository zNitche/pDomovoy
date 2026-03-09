#include "../../includes/acceleration_readings.h"

#include "pico_adxl345/adxl345.h"

void get_accel_readings_mean(float input[][3], float output[3]) {
    const int input_length = sizeof(*input) / sizeof(*input[0]);

    float means[3] = {0.0};
    float tmp_combined = 0.0;

    for (int axis_id = 0; axis_id < 3; axis_id++) {
        tmp_combined = 0.0;

        for (int reading_id = 0; reading_id < input_length; reading_id++) {
            tmp_combined += input[reading_id][axis_id];
        }

        means[axis_id] = tmp_combined / input_length;
    }

    *output = *means;
}

void get_bunch_of_accel_readings(ADXL345I2C* adxl345_i2c,
                                 float total_accel[][3], int delay) {
    const int readings_count = sizeof(*total_accel) / sizeof(*total_accel[0]);

    float current_accel[3] = {0.0};

    for (int i = 0; i < readings_count; i++) {
        adxl345_get_readings(*adxl345_i2c, current_accel);

        *total_accel[i] = *current_accel;

        sleep_ms(delay);
    }
}
