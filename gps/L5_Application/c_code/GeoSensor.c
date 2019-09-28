/*
 * GeoSensor.c
 *
 *  Created on: Apr 29, 2019
 *      Author: Namdev Prabhugaonkar
 */
#include "GeoSensor.h"
#include <math.h>

#define Earth_radius 6371000 //radius in meters

float distance;
float current_long; //current logitude in radians
float current_lat; //current latitude in radians
float destination_lat; //destination latitude
float longitude_difference;
float latitude_difference;
float bearing;

float a;
float c;

float convert_to_radians(float degree)
{
    return degree * (M_PI / 180.0);
}

float convert_to_degree(float radian)
{
    return radian * (180.0 / M_PI);
}

float cal_distance(float curr_long, float curr_lat, float dst_long, float dst_lat)
{

//formula to calculate distance
//    a = sin^2(latDiff_rad/2) + cos(lat1_rad) * cos(lat2_rad) * sin^2(longDiff_rad/2)
//       formula: c = 2 *atan2(sqrt(a), sqrt(1-a))
//       d = Radius of Earth * c

    current_lat = convert_to_radians(curr_lat);
    destination_lat = convert_to_radians(dst_lat);
    current_long = convert_to_radians(curr_long);

    latitude_difference = convert_to_radians(dst_lat - curr_lat);
    longitude_difference = convert_to_radians(dst_long - curr_long);

    a = (sin(latitude_difference / 2) * sin(latitude_difference / 2))
            + cos(current_lat) * cos(destination_lat) * sin(longitude_difference / 2) * sin(longitude_difference / 2);
    c = 2 * atan2(sqrt(a), sqrt(1 - a));
    distance = Earth_radius * c;

    return distance;
}

float cal_bearing(float curr_long, float curr_lat, float dst_long, float dst_lat)
{
    //formula to calculate bearing
    // bearing = atan2( sin(longDiff_rad)*cos(lat2_rad),
    //cos(lat1_rad)*sin(lat2_rad)-sin(lat1_rad)*cos(lat2_rad)*cos(longDiff_rad))

    current_lat = convert_to_radians(curr_lat);
    destination_lat = convert_to_radians(dst_lat);
    longitude_difference = convert_to_radians(dst_long - curr_long);

    float x = sin(longitude_difference) * cos(destination_lat);
    float y = cos(current_lat) * sin(destination_lat) - sin(current_lat) * cos(destination_lat) * cos(longitude_difference);

    bearing = fmodf(convert_to_degree(atan2(x, y)) + 360, 360);

    return bearing;
}
