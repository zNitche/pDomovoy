#include "../../includes/core_0.h"

#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"

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