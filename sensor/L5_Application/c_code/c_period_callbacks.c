/**
 * @file
 *
 * The purpose of this "C" callbacks is to provide the code to be able
 * to call pure C functions and unit-test it in C test framework
 */
#include "sensor.h"

bool C_period_init(void) {
    can_init();
    sensor_init();

    return true;
}

bool C_period_reg_tlm(void) {
    return true;
}

void C_period_1Hz(uint32_t count) {
    (void) count;
}

void C_period_10Hz(uint32_t count) {
    (void) count;
}

void C_period_100Hz(uint32_t count) {
    (void) count;
}

void C_period_1000Hz(uint32_t count) {
    (void) count;
    const uint32_t period_20Hz = 50;

    static sensor_t sensors_prev = { 0, 0, 0, 0 };
    static sensor_t sensors = { 0, 0, 0, 0 };

    if (count % period_20Hz == 0) {
        read_sensor_left(&sensors);
    } else if (count % period_20Hz == 16) {
        read_sensor_center(&sensors);
    } else if (count % period_20Hz == 32) {
        read_sensor_right(&sensors);
        filter_sensors(&sensors, &sensors_prev);
        send_sensor(&sensors);
    }
}
