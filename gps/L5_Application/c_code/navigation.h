/*
 * navigation.h
 *
 *  Created on: May 2, 2019
 *      Author: AmanChandan
 */

#ifndef NAVIGATION_H_
#define NAVIGATION_H_
#include <stdbool.h>


typedef struct {
    float latitude;
    float longitude;
} checkpoint;

void command_car(void);

void plan_path(void);

int get_checkpoint(checkpoint current_coordinate);

void receive_coordinates_ble(void);

#endif /* NAVIGATION_H_ */
