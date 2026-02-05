#include "../includes/button.h"
#include "../includes/buzzer.h"
#include "../includes/callbacks.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "pico_adxl345/adxl345.h"

int main() {
  stdio_init_all();

  int rc = cyw43_arch_init();
  hard_assert(rc == PICO_OK);

#ifdef DEBUG
  // waiting for input
  getchar();
#endif

  printf("jammer...\n");

  // BUZZER
  init_buzzer_pwm(PDA_BUZZER_PIN, alarm_buzzer_irq_callback);

  // BUTTON
  init_button_irq(PDA_ACTION_BUTTON_PIN, action_button_callback);

  // LED
  gpio_init(PDA_STATUS_LED_PIN);
  gpio_set_dir(PDA_STATUS_LED_PIN, true);

  // ADXL345
  ADXL345I2C adxl345_i2c = {i2c0, 0x53, 0, 1};

  adxl345_setup_i2c(adxl345_i2c);
  sleep_ms(250);

  bool connected = adxl345_check_connection(adxl345_i2c);

  printf("connected: %d\n", connected);

  enum ADXL345Ranges sensor_range = RANGE_4G;
  adxl345_set_measurements_range(adxl345_i2c, sensor_range);

  adxl345_start_measurements(adxl345_i2c);

  while (true) {
    if (alarm_on) {
      gpio_put(PDA_STATUS_LED_PIN, true);
      sleep_ms(250);
      gpio_put(PDA_STATUS_LED_PIN, false);
      sleep_ms(250);
    }

    if (connected) {
      float accel[3] = {0.0};
      adxl345_get_readings(adxl345_i2c, accel);

      printf("Acc. X = %f, Y = %f, Z = %f\n", accel[0], accel[1], accel[2]);

      sleep_ms(200);
    }
  }

  adxl345_stop_measurements(adxl345_i2c);

  return 0;
}