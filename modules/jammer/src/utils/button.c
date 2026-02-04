#include "../../includes/button.h"

#include <stdio.h>

#include "pico/time.h"

void init_button_irq(uint gpio, button_callback callback) {
  gpio_init(gpio);
  gpio_set_dir(gpio, GPIO_IN);
  gpio_pull_down(gpio);
  gpio_set_irq_enabled_with_callback(
      gpio, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, callback);
}

void toggle_button_callback(uint gpio, uint32_t event) {
  static bool is_pressed = false;
  static uint32_t last_call = 0;

  const uint32_t current_time = to_ms_since_boot(get_absolute_time());

  if (current_time - last_call < 300) {
    return;
  }

  if (event == GPIO_IRQ_EDGE_FALL) {
    is_pressed = false;
    return;
  }

  if (event != GPIO_IRQ_EDGE_RISE || is_pressed) {
    return;
  }

  printf("callback\n");

  is_pressed = true;
  last_call = current_time;
}