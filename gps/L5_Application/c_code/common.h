/*
 * common.h
 *
 *  Created on: May 2, 2019
 *      Author: AmanChandan
 */

#ifndef COMMON_H_
#define COMMON_H_

typedef struct
{
    float longitude;
    char longitude_dir;
    float latitude;
    char latitude_dir;
    int number_of_satelites;
    int fix;
}gps_struct;

typedef enum
{
    steer_hard_left = -2,
    steer_soft_left = -1,
    steer_straight = 0 ,
    steer_soft_right = 1 ,
    steer_hard_right = 2,
    stop = 10
}car_command_states;

extern gps_struct global_present_coordinates;

extern gps_struct destination_coordinates;

#endif /* COMMON_H_ */
