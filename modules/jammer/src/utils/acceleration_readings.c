#include "../../includes/acceleration_readings.h"

#include "pico_adxl345/adxl345.h"

void get_accel_readings_mean(float input[][3], int input_length,
                             float output[3]) {
  float means[3] = {0.0};
  float tmp_combined = 0.0;

  for (int i = 0; i < 3; i++) {
    tmp_combined = 0.0;

    for (int j = 0; j < input_length; j++) {
      tmp_combined += input[j][i];
    }

    means[i] = tmp_combined / input_length;
  }

  output[0] = means[0];
  output[1] = means[1];
  output[2] = means[2];
}

void get_bunch_of_accel_readings(ADXL345I2C* adxl345_i2c, int readings_count,
                                 float total_accel[][3], int delay) {
  float current_accel[3] = {0.0};

  for (int i = 0; i < readings_count; i++) {
    adxl345_get_readings(*adxl345_i2c, current_accel);

    total_accel[i][0] = current_accel[0];
    total_accel[i][1] = current_accel[1];
    total_accel[i][2] = current_accel[2];

    sleep_ms(delay);
  }
}
