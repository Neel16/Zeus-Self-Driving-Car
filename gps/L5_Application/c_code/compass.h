/*
 * compass.h
 *
 *  Created on: Apr 19, 2019
 *      Author: AmanChandan
 */
#include <stdint.h>
#include <stdbool.h>
#ifndef COMPASS_H_
#define COMPASS_H_
typedef enum
{
    first_command_0xf0,
    second_command_0xf5,
    third_command_0xf6,
    end_calibration_0xf8,
    idle
}calibration_states;

void compass_init();
float get_heading();
uint8_t get_calibration();
bool calibrate(bool start);
bool send_heading();
#endif /* COMPASS_H_ */
