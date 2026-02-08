#include <math.h>
#include <stdbool.h>

#include "../includes/button.h"
#include "../includes/callbacks.h"
#include "../includes/core_0.h"
#include "../includes/core_1.h"
#include "../includes/defines.h"
#include "../includes/globals.h"
#include "../includes/pwm.h"
#include "pico/cyw43_arch.h"
#include "pico/multicore.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"

void init_peripherals() {
    // BUZZER
    set_pwm_irq_callback(PDA_BUZZER_PIN, alarm_buzzer_irq_callback);

    // BUTTON
    init_button_irq(PDA_ACTION_BUTTON_PIN, action_button_callback);

    // LED
    gpio_init(PDA_STATUS_LED_PIN);
    gpio_set_dir(PDA_STATUS_LED_PIN, true);
}

void init_mc_queues() {
    queue_init(&core0_events_queue, sizeof(mc_event_item), 3);
    queue_init(&core1_events_queue, sizeof(mc_event_item), 3);
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
    init_mc_queues();

    // CORES STARTUP
    // core_1 for alarm handling
    multicore_launch_core1(core_1);
    // core_0 for device handling
    core_0();

    return 0;
}