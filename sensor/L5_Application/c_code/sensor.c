// @file sensor.c
#include "sensor.h"

const float VCC = 5.0;
const uint16_t SENSOR_THRESHOLD = 236; /* 236 in = 600 cm */
const uint16_t SENSOR_MAX = 255;

bool can_init(void)
{
    if (CAN_init(can1, 100, 16, 16, NULL, NULL)) {
        CAN_bypass_filter_accept_all_msgs();
        CAN_reset_bus(can1);
        return true;
    }
    return false;
}

bool can_refresh(void)
{
    if (CAN_is_bus_off(can1)) {
        CAN_bypass_filter_accept_all_msgs();
        CAN_reset_bus(can1);
    }
    return true;
}

bool sensor_init(void)
{
    sensor_delay_ms(250);

    adc0_init();
    adc_pin_init(3);
    adc_pin_init(4);
    adc_pin_init(5);
    trigger_init();
    return true;
}

float raw2volt(uint16_t raw, float vcc)
{
    float voltage;

    voltage = ((float)raw / (float)0xfff) * vcc;
    return voltage;
}

uint16_t volt2inch(float voltage, float vcc)
{
    uint16_t inches;
    float ratio;
    if (vcc < 4.0) {
        ratio = 0.0064;
    } else {
        ratio = 0.0098;
    }

    inches = (uint16_t)(voltage / ratio);
    return inches;
}

uint8_t sensor2chan(enum sensor_dir sensor)
{
    /* TODO handle back sensor */
    const uint8_t channel_offset = 3;
    return (uint8_t)sensor + channel_offset;
}

enum sensor_type sensor2type(enum sensor_dir sensor)
{
    if (sensor == sensor_back) {
        return sensor_pw;
    } else {
        return sensor_adc;
    }
}

uint16_t read_sensor(enum sensor_dir sensor)
{
    uint16_t adc, inches;
    float voltage;

    trigger_sensor(sensor, true);
    sensor_delay_us(25);
    trigger_sensor(sensor, false);

    switch (sensor2type(sensor)) {
        case sensor_adc:
            adc = adc0_get_reading(sensor2chan(sensor));
            voltage = raw2volt(adc, VCC);
            inches = volt2inch(voltage, VCC);
            break;
        case sensor_pw:
            inches = get_pw_inch();
            break;
        default:
            inches = SENSOR_MAX;
            break;
    }

    return inches;
}

bool read_sensor_left(sensor_t *sensors)
{
    sensors->left = read_sensor(sensor_left);
    return true;
}

bool read_sensor_center(sensor_t *sensors)
{
    sensors->center = read_sensor(sensor_center);
    return true;
}

bool read_sensor_right(sensor_t *sensors)
{
    sensors->right = read_sensor(sensor_right);
    return true;
}

uint8_t filter_sensor(uint16_t inches, uint16_t prev)
{
    uint8_t filtered = (uint8_t)inches;

    /**
     * Filter out surges (hold to previous value)
     * If sensor holding at Vcc (broken/misconfigured), return SENSOR_MAX
     */
    if (inches >= SENSOR_MAX) {
        if (prev <= SENSOR_THRESHOLD) {
            filtered = prev;
        } else {
            filtered = SENSOR_MAX;
        }
    }

    return filtered;
}

bool filter_sensors(sensor_t *sensors, sensor_t *sensors_prev)
{
    uint16_t left = sensors->left, center = sensors->center, right = sensors->right, back = sensors->back;

    sensors->left = filter_sensor(left, sensors_prev->left);
    sensors_prev->left = left;
    sensors->center = filter_sensor(center, sensors_prev->center);
    sensors_prev->center = center;
    sensors->right = filter_sensor(right, sensors_prev->right);
    sensors_prev->right = right;
    sensors->back = filter_sensor(back, sensors_prev->back);
    sensors_prev->back = back;

    return true;
}

bool send_sensor(sensor_t *sensors)
{
    can_refresh();

    return send_sensor_dbc((uint8_t)sensors->left, (uint8_t)sensors->center, (uint8_t)sensors->right, (uint8_t)sensors->back);
}

