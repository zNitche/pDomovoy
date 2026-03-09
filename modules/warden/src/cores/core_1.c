#include "../../includes/core_1.h"

#include <math.h>
#include <stdbool.h>

#include "../../includes/acceleration_readings.h"
#include "../../includes/debug_print.h"
#include "../../includes/defines.h"
#include "../../includes/globals.h"
#include "../../includes/types.h"
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "pico_adxl345/adxl345.h"

void _send_event_to_core_0(enum DeviceStatus status) {
    mc_event_item event;
    event.status = status;

    queue_add_blocking(&g_core0_events_queue, &event);
}

void _get_initial_accel_mean(ADXL345I2C* adxl345_i2c, float output[3]) {
    const int readings_count = 10;
    float total_accel[readings_count][3] = {};

    get_bunch_of_accel_readings(adxl345_i2c, total_accel, 200);
    get_accel_readings_mean(total_accel, output);
}

bool _check_for_trigger_for_axis(float initial_mean, float mean,
                                 float trigger_factor) {
    const bool are_axis_different =
        (initial_mean < 0 && mean > 0) || (initial_mean > 0 && mean < 0);

    if (are_axis_different) {
        return true;
    }

    return fabs(fabs(initial_mean) - fabs(mean)) > trigger_factor;
}

bool _check_for_alarm_trigger(ADXL345I2C* adxl345_i2c,
                              float initial_accel_mean[3]) {
    const int readings_count = 5;
    const float trigger_factor = 0.2;

    float total_accel[readings_count][3] = {};
    float accel_mean[3] = {0.0};

    get_bunch_of_accel_readings(adxl345_i2c, total_accel, 100);
    get_accel_readings_mean(total_accel, accel_mean);

    for (int i = 0; i < 3; i++) {
        const bool is_triggered = _check_for_trigger_for_axis(
            initial_accel_mean[i], accel_mean[i], trigger_factor);

        if (is_triggered) {
            debug_print("trigger accel mean = x:%f y:%f z:%f\n", accel_mean[0],
                        accel_mean[1], accel_mean[2]);

            return true;
        }
    }

    return false;
}

void core_1() {
    float initial_accel_mean[3] = {0.0};

    ADXL345I2C adxl345_i2c = {i2c0, 0x53, PD_ADXL345_SDA_PIN,
                              PD_ADXL345_SCL_PIN};

    adxl345_setup_i2c(adxl345_i2c);
    bool connected = adxl345_check_connection(adxl345_i2c);

    if (!connected) {
        _send_event_to_core_0(PD_ADXL345_ERROR);

        return;
    } else {
        _send_event_to_core_0(PD_ADXL345_OK);
    }

    _send_event_to_core_0(PD_STANDBY_PREP);

    adxl345_set_measurements_range(adxl345_i2c, ADXL345_RANGE_4G);
    adxl345_start_measurements(adxl345_i2c);

    debug_print("[core_1] adxl345 has been started\n");

    _get_initial_accel_mean(&adxl345_i2c, initial_accel_mean);

    debug_print("intial accel_mean = x:%f y:%f z:%f\n", initial_accel_mean[0],
                initial_accel_mean[1], initial_accel_mean[2]);

    debug_print(
        "[core_1] got initial acceleration readings, running mainloop\n");

    _send_event_to_core_0(PD_STANDBY_READY);

    while (true) {
        if (_check_for_alarm_trigger(&adxl345_i2c, initial_accel_mean)) {
            debug_print("[core_1] alarm trigger\n");
            _send_event_to_core_0(PD_ACCELERATION_TRIGGER);

            break;
        }
    }

    debug_print("[core_1] exiting...\n");

    adxl345_stop_measurements(adxl345_i2c);

    return;
}
