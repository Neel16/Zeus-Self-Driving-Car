/*
 * GeoSensor.h
 *
 *  Created on: Apr 29, 2019
 *      Author: AmanChandan
 */

#ifndef GEOSENSOR_H_
#define GEOSENSOR_H_
#include <stdbool.h>

bool geo_sensors_init();
float cal_distance(float curr_long , float curr_lat , float dst_long , float dst_lat);
float cal_bearing(float curr_long , float curr_lat , float dst_long , float dst_lat);
float convert_to_radians(float degree);
float convert_to_degree(float radian);




#endif /* GEOSENSOR_H_ */
