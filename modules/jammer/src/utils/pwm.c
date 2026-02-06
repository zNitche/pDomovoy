#include "../../includes/pwm.h"

#include "hardware/pwm.h"

void post_pwm_irq(uint gpio_pin, uint16_t pwm_level) {
  pwm_clear_irq(pwm_gpio_to_slice_num(gpio_pin));

  pwm_set_gpio_level(gpio_pin, pwm_level);
}

void set_pwm_irq_callback(uint gpio_pin, buzzer_callback irq_cb) {
  gpio_set_function(gpio_pin, GPIO_FUNC_PWM);
  uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

  pwm_clear_irq(slice_num);
  irq_set_exclusive_handler(PWM_DEFAULT_IRQ_NUM(), irq_cb);

  pwm_config config = pwm_get_default_config();

  pwm_config_set_clkdiv(&config, 1);
  pwm_init(slice_num, &config, true);
}

void set_pwm_irq_state(uint slice_num, bool state) {
  irq_set_enabled(PWM_DEFAULT_IRQ_NUM(), state);
  pwm_set_irq_enabled(slice_num, state);
}

void enable_pwm_irq_on_pin(uint gpio_pin) {
  uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

  set_pwm_irq_state(slice_num, true);
}

void disable_pwm_irq_on_pin(uint gpio_pin) {
  uint slice_num = pwm_gpio_to_slice_num(gpio_pin);

  set_pwm_irq_state(slice_num, false);

  pwm_set_gpio_level(gpio_pin, 0);
}
