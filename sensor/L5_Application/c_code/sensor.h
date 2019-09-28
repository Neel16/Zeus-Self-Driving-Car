// @file sensor.h
#include <stdint.h>
#include <stdbool.h>

#include <adc0.h>
#include <can.h>

#include "sensor_trigger.h"

extern const float VCC;

typedef struct {
    uint16_t left;
    uint16_t center;
    uint16_t right;
    uint16_t back;
} sensor_t;

bool can_init(void);
bool can_refresh(void);
bool sensor_init(void);

float raw2volt(uint16_t raw, float vcc);
uint16_t volt2inch(float voltage, float vcc);
uint8_t sensor2chan(enum sensor_dir sensor);
enum sensor_type sensor2type(enum sensor_dir sensor);
uint16_t read_sensor(enum sensor_dir sensor);

bool read_sensor_left(sensor_t *sensors);
bool read_sensor_center(sensor_t *sensors);
bool read_sensor_right(sensor_t *sensors);
uint8_t filter_sensor(uint16_t inches, uint16_t prev);
bool filter_sensors(sensor_t *sensors, sensor_t *sensors_prev);
bool send_sensor(sensor_t *sensors);
