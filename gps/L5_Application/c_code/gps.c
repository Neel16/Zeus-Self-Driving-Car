/*
 * gps.c
 *
 *  Created on: Apr 20, 2019
 *      Author: NamdevPrabhugaonkar
 */
#include "gps.h"
#include <stdbool.h>
#include "c_uart3.h"
#include <string.h>
#include "c_io.h"
#include <stdlib.h>
#include "can.h"
#include "generated_can.h"
#include "GeoSensor.h"
#include "common.h"
#include "navigation.h"

static char gps_string_buffer[200] = { 0 };
static char *gga, *ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6, *ptr7;
static int i = 0;
static char lat[10];
static char longi[11];
static char gps_fix[2];
int fix_true = 0;
gps_struct global_present_coordinates;
gps_struct destination_coordinates;

DEBUG_GPS_t gps_data = { 0 };
float convert_to_decimal_coord(float coordinates)
{
    /* Sample from gps 5153.6605*/
    /* Check limits*/
//    if ((*val == 'm') && (coordinates < 0.0 && coordinates > MAX_LATITUDE))
//    {
//        return 0;
//    }
//    if (*val == 'p' && (coordinates < 0.0 && coordinates > MAX_LONGITUDE))
//    {
//        return 0;
//    }
    int b; //to store the degrees
    float c; //to store the decimal

    b = coordinates / 100; // 51 degrees
    c = (coordinates / 100 - b) * 100; //(51.536605 - 51)* 100 = 53.6605
    c /= 60; // 53.6605 / 60 = 0.8943417
    c += b; // 0.8943417 + 51 = 51.8943417
    return c;
}

gps_struct parse_gps_data(char *gga_string)
{
    gps_struct data;
    data.longitude = 0;
    data.latitude = 0;
    char * temp_gga = strstr(gga_string, "GGA");

    if (temp_gga) {
        temp_gga++;
        ptr1 = strstr(temp_gga, ",");
        ptr1++;
        ptr2 = strstr(ptr1, ",");
        ptr2++;
        strncpy(lat, ptr2, 10);
        global_present_coordinates.latitude = atof(lat);
        global_present_coordinates.latitude = convert_to_decimal_coord(global_present_coordinates.latitude);

        ptr3 = strstr(ptr2, ",");
        ptr3++;
        global_present_coordinates.latitude_dir = ptr3[0];
        if (global_present_coordinates.latitude_dir == 'N') {
            global_present_coordinates.latitude = global_present_coordinates.latitude * 1;
        }
        else if (global_present_coordinates.latitude_dir == 'S') {
            global_present_coordinates.latitude = global_present_coordinates.latitude * (-1);
        }
        ptr4 = strstr(ptr3, ",");
        ptr4++;
        strncpy(longi, ptr4, 11);
        global_present_coordinates.longitude = atof(longi);
        global_present_coordinates.longitude = convert_to_decimal_coord(global_present_coordinates.longitude);


        ptr5 = strstr(ptr4, ",");
        ptr5++;
        global_present_coordinates.longitude_dir = ptr5[0];
        if (global_present_coordinates.longitude_dir == 'E') {
            global_present_coordinates.longitude = global_present_coordinates.longitude * 1;
        }
        else if (global_present_coordinates.longitude_dir == 'W') {
            global_present_coordinates.longitude = global_present_coordinates.longitude * (-1);
        }
        ptr6 = strstr(ptr5, ",");
        ptr6++;
        gps_fix[0] = ptr6[0];
        gps_fix[1] = '\0';
        global_present_coordinates.fix = atoi(gps_fix);
        if (global_present_coordinates.fix) {
            fix_true++;
            LED_ON(1);
        }
        else {
            LED_OFF(1);
        }
        ptr7 = strstr(ptr6, ",");
        ptr7++;
        char satelites[2] = { 0 };
        strncpy(satelites, ptr7, 2);
        global_present_coordinates.number_of_satelites = atoi(satelites);
        gga = NULL;
        gps_data.CURR_LAT = global_present_coordinates.latitude;
        gps_data.CURR_LONG = global_present_coordinates.longitude;
        dbc_encode_and_send_DEBUG_GPS(&gps_data);
    }
    return data;
}

void fill_gps_buffer(void)
{
    char gps = 0;

    if(uart3_getchar(&gps, 0))
    {
        if (i == 150)
        {
            i = 0;
            parse_gps_data(&gps_string_buffer[0]);
            memset(gps_string_buffer , 0 , sizeof(gps_string_buffer));
        }
        else {
            gps_string_buffer[i] = gps;
            i++;
        }
    }
}

bool send_debug_gps(void)
{
    gps_struct gps_data_int = { 0 };
    gps_data.CURR_LAT = global_present_coordinates.latitude;
    gps_data.CURR_LONG = global_present_coordinates.longitude;
    if (gps_data_int.fix) {
        if (gps_data_int.fix) {
            LED_ON(1);
        }
        else {
            LED_OFF(1);
        }
        return dbc_encode_and_send_DEBUG_GPS(&gps_data);
    }
    else {
        return false;
    }
}
