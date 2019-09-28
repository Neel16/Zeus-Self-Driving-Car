/**
 * @file
 *
 * The purpose of this "C" callbacks is to provide the code to be able
 * to call pure C functions and unit-test it in C test framework
 */
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "can.h"
#include "c_period_callbacks.h"
#include "can_commands.h"
//#include "sensor_cmd.h"
#include "CAN_receive.h"

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}
/*******************************************************/
bool C_period_init(void) {
    CAN_init(can1, 100, 25, 25 , NULL, NULL);
    CAN_reset_bus(can1);
    CAN_bypass_filter_accept_all_msgs();

    return true;
}

bool C_period_reg_tlm(void) {
	return true;
}

void C_period_1Hz(uint32_t count) {
    (void) count;

    if(CAN_is_bus_off(can1))
       CAN_reset_bus(can1);

    start_cmd();
}

void C_period_10Hz(uint32_t count) {
    (void) count;

    navigation_10hz(count);
}

void C_period_100Hz(uint32_t count) {
    (void) count;

    detect_obstacle_100hz();
}

void C_period_1000Hz(uint32_t count) {
    (void) count;
}
