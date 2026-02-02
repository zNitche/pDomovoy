#include <stdio.h>

#include "../../libs/pico-c-adxl345/lib/pico_adxl345.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

const int STATUS_LED_PIN = 13;
const int BUZZER_PIN = 14;
const int ACTION_BUTTON_PIN = 12;

bool alarm_on = false;

void buzzer_pwm_call_irq() {
  static int fade = 0;
  static bool going_up = true;

  pwm_clear_irq(pwm_gpio_to_slice_num(BUZZER_PIN));

  if (going_up) {
    ++fade;
    if (fade > 255) {
      fade = 255;
      going_up = false;
    }
  } else {
    --fade;
    if (fade < 0) {
      fade = 0;
      going_up = true;
    }
  }

  pwm_set_gpio_level(BUZZER_PIN, fade * fade);
}

void init_buzzer_pwm() {
  gpio_set_function(BUZZER_PIN, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

  pwm_clear_irq(slice_num);
  irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), buzzer_pwm_call_irq);

  pwm_config config = pwm_get_default_config();
  pwm_config_set_clkdiv(&config, 1);
  pwm_init(slice_num, &config, true);
}

void enable_buzzer_pwm() {
  uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

  irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);
  pwm_set_irq_enabled(slice_num, true);
}

void disable_buzzer_pwm() {
  uint slice_num = pwm_gpio_to_slice_num(BUZZER_PIN);

  irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), false);
  pwm_set_irq_enabled(slice_num, false);
  pwm_set_gpio_level(BUZZER_PIN, 0);
}

void button_callback(uint gpio, uint32_t events) {
  if (gpio == ACTION_BUTTON_PIN) {
    if (events == 8) {
      printf("alarm trigger\n");

      if (alarm_on) {
        disable_buzzer_pwm();
        alarm_on = false;
      } else {
        enable_buzzer_pwm();
        alarm_on = true;
      }
    }
  }
}

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
  init_buzzer_pwm();

  // BUTTON
  gpio_init(ACTION_BUTTON_PIN);
  gpio_set_dir(ACTION_BUTTON_PIN, GPIO_IN);
  gpio_pull_down(ACTION_BUTTON_PIN);
  gpio_set_irq_enabled_with_callback(ACTION_BUTTON_PIN,
                                     GPIO_IRQ_EDGE_RISE,
                                     true, &button_callback);

  // LED
  gpio_init(STATUS_LED_PIN);
  gpio_set_dir(STATUS_LED_PIN, true);

  while (true) {
    gpio_put(STATUS_LED_PIN, true);
    sleep_ms(250);
    gpio_put(STATUS_LED_PIN, false);
    sleep_ms(250);
  }

  // ADXL345I2C adxl345_i2c = {i2c0, 0x53, 0, 1};

  // adxl345_setup_i2c(adxl345_i2c);
  // sleep_ms(250);

  // bool connected = adxl345_check_connection(adxl345_i2c);

  // printf("connected: %d\n", connected);

  // enum ADXL345Ranges sensor_range = RANGE_4G;
  // adxl345_set_measurements_range(adxl345_i2c, sensor_range);

  // adxl345_start_measurements(adxl345_i2c);

  // if (connected == true) {
  //   printf("processing...\n");
  //   float accel[3] = {0.0};

  //   while (true) {
  //     adxl345_get_readings(adxl345_i2c, accel);

  //     printf("Acc. X = %f, Y = %f, Z = %f\n", accel[0], accel[1], accel[2]);

  //     sleep_ms(200);
  //   }

  //   adxl345_stop_measurements(adxl345_i2c);
  // }

  return 0;
}