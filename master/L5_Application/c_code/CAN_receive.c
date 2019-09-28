/*
 * CAN_receive.c
 *
 */
#include"CAN_receive.h"

SENSOR_SONARS_t sensor_data= {0};
DIRECTION_GPS_t gps_data = {0};
SPEED_MOTOR_t motor_sensor_data ={0};
APP_DATA_t app_start ={0};

/********FOR MIA***********************************/
const uint32_t SENSOR_SONARS__MIA_MS = 50;
const SENSOR_SONARS_t SENSOR_SONARS__MIA_MSG = {0};

const uint32_t DIRECTION_GPS__MIA_MS = 0;
const DIRECTION_GPS_t DIRECTION_GPS__MIA_MSG = {0};

const uint32_t SPEED_MOTOR__MIA_MS = 0;
const SPEED_MOTOR_t SPEED_MOTOR__MIA_MSG = {0};
/**************************************************/

void handle_sensor_mia()
{
    dbc_handle_mia_SENSOR_SONARS(&sensor_data,10);
    c_LED_display_set_number(3);
}

void handle_GPS_mia()
{
    dbc_handle_mia_DIRECTION_GPS(&gps_data, 10);
    //c_LED_display_set_number(2);
}

void handle_motor_mia()
{
    dbc_handle_mia_SPEED_MOTOR(&motor_sensor_data,10);
    c_LED_display_set_number(1);
}

void receive_CAN()
{
    can_msg_t can_msg;
    while (CAN_rx(can1, &can_msg, 0))
    {
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;

        switch(can_msg_hdr.mid)
        {
            case 121: dbc_decode_SENSOR_SONARS(&sensor_data, can_msg.data.bytes, &can_msg_hdr);
            handle_sensor_mia();
            break;

            case 141: dbc_decode_DIRECTION_GPS(&gps_data, can_msg.data.bytes, &can_msg_hdr);
            handle_GPS_mia();
            break;

            case 161: dbc_decode_SPEED_MOTOR(&motor_sensor_data, can_msg.data.bytes, &can_msg_hdr);
            handle_motor_mia();
            break;

            case 181: dbc_decode_APP_DATA(&app_start,can_msg.data.bytes, &can_msg_hdr);
            break;
        }
        debug_sensor_data();
    }
}

void debug_sensor_data()
{
            DEBUG_MASTER_t debug_sensor_data = {0};
            debug_sensor_data.MASTER_LEFT_cmd = sensor_data.SENSOR_SONARS_LEFT;
            debug_sensor_data.MASTER_RIGHT_cmd = sensor_data.SENSOR_SONARS_RIGHT ;
            debug_sensor_data.MASTER_MIDDLE_cmd = sensor_data.SENSOR_SONARS_MIDDLE;

            dbc_encode_and_send_DEBUG_MASTER(&debug_sensor_data);
}
