/*
 * compass.c
 *
 *  Created on: Apr 19, 2019
 *      Author: AmanChandan
 */
#include "compass.h"
#include "c_i2c.h"
#include "generated_can.h"
#include <stdbool.h>
#include <stdio.h>
#include "utilities.h"
//#include "tasks.hpp"

void compass_init()
{
    i2c_init(100);
}


float get_heading()
{
    return ((i2c_readreg(0xc0 , 0x02)) * 0.1);
}

uint8_t get_calibration()
{
    return i2c_readreg(0xc0 , 0x1e);
}

bool calibrate(bool start)
{
    static calibration_states next_state = first_command_0xf0;
    switch(next_state)
    {
        case idle:
            if(start)
            {
                next_state = first_command_0xf0;
            }
            else
            {
                next_state = idle;
            }
            break;
        case first_command_0xf0:
            i2c_writereg(0xc0 , 0 , 0xe0);
            printf("Sent 1\n");
            next_state = second_command_0xf5;
            delay_ms(20);
            break;
        case second_command_0xf5:
            i2c_writereg(0xc0 , 0 , 0xe5);
            printf("Sent 2\n");
            next_state = third_command_0xf6;
            delay_ms(20);
            break;
        case third_command_0xf6:
            i2c_writereg(0xc0 , 0 , 0xe2);
            printf("Sent 3\n");
            next_state = idle;
            delay_ms(20);
            break;
        case end_calibration_0xf8:
            printf("Sent end\n");
            i2c_writereg(0xc0 , 0 , 0xf8);
            next_state = idle;
            break;
        default:
            next_state = idle;
            break;
    }
    return true;
}

bool send_heading()
{
    HEADING_t can_heading = {0};
    can_heading.HEADING_VAL = get_heading();
    //printf("Heading:%lu\n" , can_heading.HEADING_VAL);
    dbc_encode_and_send_HEADING(&can_heading);
}
