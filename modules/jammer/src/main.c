#include <math.h>
#include <stdbool.h>

#include "../includes/button.h"
#include "../includes/callbacks.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "../includes/pwm.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
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

  //   printf("accel means x: %f y: %f z: %f\n", accel_mean[0], accel_mean[1],
  //          accel_mean[2]);

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
    printf("core_1 is_alarm_trigger %d \n", is_triggered);

    if (is_triggered) {
      multicore_fifo_push_blocking(PDA_MCS_ALARM_TRIGGERED);
    }

    sleep_ms(100);
  }

  adxl345_stop_measurements(adxl345_i2c);

  return;
}

void core_0() {
  bool is_alarm_triggered = false;
  uint32_t mc_fifo_data = 0;

  while (true) {
    multicore_fifo_pop_timeout_us(100, &mc_fifo_data);

    switch (mc_fifo_data) {
      case PDA_MCS_ADXL345_OK:
        printf("adxl345 ok\n");

        break;
      case PDA_MCS_ADXL345_ERROR:
        printf("adxl345 fail\n");

        break;
      case PDA_MCS_ALARM_TRIGGERED:
        is_alarm_triggered = true;

        break;
      default:
        break;
    }

    if (alarm_on & is_alarm_triggered) {
      printf("[core_0] is_alarm_triggered: %d\n", is_alarm_triggered);

      // todo implement with timers
      gpio_put(PDA_STATUS_LED_PIN, true);
      sleep_ms(100);
      gpio_put(PDA_STATUS_LED_PIN, false);
      sleep_ms(100);
    }

    mc_fifo_data = 0;
    sleep_ms(250);
  }

  return;
}

void init_peripherals() {
  // BUZZER
  set_pwm_irq_callback(PDA_BUZZER_PIN, alarm_buzzer_irq_callback);

  // BUTTON
  init_button_irq(PDA_ACTION_BUTTON_PIN, action_button_callback);

  // LED
  gpio_init(PDA_STATUS_LED_PIN);
  gpio_set_dir(PDA_STATUS_LED_PIN, true);
}

int main() {
  stdio_init_all();

  int rc = cyw43_arch_init();
  hard_assert(rc == PICO_OK);

#ifdef DEBUG
  // DEBUG waiting for input
  getchar();
#endif

  printf("jammer...\n");

  init_peripherals();

  // CORES STARTUP
  // core_1 for alarm handling
  multicore_launch_core1(core_1);
  // core_0 for device handling
  core_0();

  return 0;
}