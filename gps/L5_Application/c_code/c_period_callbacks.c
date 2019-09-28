/**
 * @file
 *
 * The purpose of this "C" callbacks is to provide the code to be able
 * to call pure C functions and unit-test it in C test framework
 */
#include <stdint.h>
#include <stdbool.h>
#include "stdio.h"
#include "c_io.h"
#include "can.h"
#include "generated_can.h"
#include "c_uart3.h"
#include <string.h>
#include <stdlib.h>
#include "gps.h"
#include "c_i2c.h"
#include "compass.h"
#include "GeoSensor.h"
#include "common.h"
#include "navigation.h"

#define NMEA_RMC_ONLY "$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"
#define GPS_ANTENNA_STATUS "$PGCMD,33,1*6C"
#define SET_GPS_BAUD "$PMTK251,57600*2C"

//#define COMPASS_CALIBRATE

//char *gga , *ptr1 , *ptr2 ,*ptr3 , *ptr4 , *ptr5 , *ptr6 , *ptr7 , *ptr8;
//gps_struct gps_data;
//can_msg_t can_heading = {0};
//HEADING_t heading_val = {0};
//
//can_msg_t can_lat={0};
//DEBUG_GPS_t gps_lat={0};
//
//can_msg_t can_long={0};
//DEBUG_GPS_t gps_long={0};

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}

bool C_period_init(void)
{
    if (CAN_init(can1, 100, 25, 25, NULL, NULL)) {
        CAN_reset_bus(can1);
        CAN_bypass_filter_accept_all_msgs();
    }
    LED_OFF(1);
    LED_OFF(2);
    LED_OFF(3);
    LED_OFF(4);
    uart3_init(9600, 100, 100);
    compass_init();
    return true;
}

bool C_period_reg_tlm(void)
{
    return true;
}

void C_period_1Hz(uint32_t count)
{
    (void) count;
    plan_path();
    send_heading();
#ifdef COMPASS_CALIBRATE
    if(count == 1)
    {
        calibrate(true);
        calibrate(true);
        calibrate(true);
        //calibrate(true);
        int calibration = get_calibration();
    }
#endif
}

void C_period_10Hz(uint32_t count)
{
    command_car();
    receive_coordinates_ble();
    (void) count;
}

void C_period_100Hz(uint32_t count)
{
    (void) count;
    fill_gps_buffer();
}

void C_period_1000Hz(uint32_t count)
{
    (void) count;
}
