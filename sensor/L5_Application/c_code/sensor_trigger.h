// @file sensor_trigger.h
#ifndef UART2_C_H__
#define UART2_C_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <string.h>
#include <eint.h>

enum sensor_dir {
    sensor_left = 0,
    sensor_center,
    sensor_right,
    sensor_back,
};

enum sensor_type {
    sensor_adc = 0,
    sensor_pw,
};

void sensor_delay_us(uint32_t us);
void sensor_delay_ms(uint32_t ms);
bool adc_pin_init(uint8_t channel);
bool trigger_init(void);
bool trigger_sensor(enum sensor_dir dir, bool on);
void pw_rising(void);
void pw_falling(void);
uint16_t get_pw_inch(void);
bool send_sensor_dbc(uint8_t left, uint8_t center, uint8_t right, uint8_t back);

#ifdef __cplusplus
}
#endif
#endif /* UART2_C_H__ */
