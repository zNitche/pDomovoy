#include "../../includes/buzzer.h"

#include "../../includes/callbacks.h"
#include "hardware/pwm.h"

void buzzer_pwm_call_irq(uint gpio_pin) {
  pwm_clear_irq(pwm_gpio_to_slice_num(gpio_pin));

  pwm_set_gpio_level(gpio_pin, 2000);
}

void init_buzzer_pwm(uint gpio_pin) {
  gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

  pwm_clear_irq(slice_num);
  irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), alarm_buzzer_irq_callback);

  pwm_config config = pwm_get_default_config();

  pwm_config_set_clkdiv(&config, 1);
  pwm_init(slice_num, &config, true);
}

void enable_buzzer_pwm(uint gpio_pin) {
  uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

  irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), true);
  pwm_set_irq_enabled(slice_num, true);
}

void disable_buzzer_pwm(uint gpio_pin) {
  uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

  irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), false);
  pwm_set_irq_enabled(slice_num, false);
  pwm_set_gpio_level(gpio_pin, 0);
}
