/*
 * navigation.c
 *
 *  Created on: May 2, 2019
 *      Author: AmanChandan
 */
#include "navigation.h"
#include "common.h"
#include "GeoSensor.h"
#include "compass.h"
#include <stdio.h>
#include <string.h>
#include "generated_can.h"
#include "c_io.h"
#include "gps.h"
#include "can.h"
#include "c_io.h"

//#define CEASER_CHAVEZ

static checkpoint checkpoint_path[30];
static int checkpoints_size = 0;
static bool received_coordinates = false;
int checkpoints_array_size = 30;
checkpoint checkpoints_array[30] = {
        {37.339900, -121.880731},
        {37.339649, -121.880539},
        {37.339405, -121.880357},
        {37.339234, -121.880222},
        {37.339015, -121.880073},
        {37.338969, -121.880186},
        {37.339178, -121.880336},
        {37.339494, -121.880565},
        {37.339865, -121.880841},
        {37.339804, -121.880964},
        {37.339564, -121.880790},
        {37.339260, -121.880562},
        {37.339094, -121.880439},
        {37.338898, -121.880302},
        {37.338840, -121.880423},
        {37.339104, -121.880619},
        {37.339520, -121.880919},
        {37.339748, -121.881088},
        {37.339690, -121.881209},
        {37.339513, -121.881083},
        {37.339391, -121.880994},
        {37.339255, -121.880817},
        {37.338946, -121.880656},
        {37.338779, -121.880554},
        {37.338733, -121.880667},
        {37.338908, -121.880798},
        {37.339015, -121.880860},
        {37.339341, -121.881109},
        {37.339492, -121.881230},
        {37.339635, -121.881324}
};
#ifdef CEASER_CHAVEZ
checkpoint checkpoints_array[55]={
        {37.334650,-121.880693},
        {37.334710,-121.880779},
        {37.334763,-121.880854},
        {37.334777,-121.880859},
        {37.334851,-121.880922},
        {37.334969,-121.881018},
        {37.334984,-121.881032},
        {37.335091,-121.881110},
        {37.335251,-121.881247},
        {37.335342,-121.881304},
        {37.335480,-121.881419},
        {37.335591,-121.881511},
        {37.335907,-121.881745},
        {37.336061,-121.881841},
        {37.336254,-121.881943},
        {37.336292,-121.881896},
        {37.336370,-121.881717},
        {37.336471,-121.881499},
        {37.336563,-121.881299},
        {37.336613,-121.881200},
        {37.336665,-121.881111},
        {37.336734,-121.880979},
        {37.336783,-121.880869},
        {37.336852,-121.880723},
        {37.336932,-121.880565},
        {37.336991,-121.880451},
        {37.337057,-121.880310},
        {37.337117,-121.880185},
        {37.337182,-121.880062},
        {37.337253,-121.879938},
        {37.337283,-121.879703},
        //Rightside
        {37.334563,-121.880637},
        {37.334687,-121.880388},
        {37.334816,-121.880145},
        {37.334959,-121.879838},
        {37.335069,-121.879624},
        {37.335207,-121.879346},
        {37.335332,-121.879103},
        {37.335428,-121.878868},
        {37.335557,-121.878688},
        {37.335607,-121.878538},
        {37.335676,-121.878596},
        {37.335800,-121.878705},
        {37.335957,-121.878804},
        {37.335989,-121.878822},
        {37.336151,-121.878926},
        {37.336292,-121.879080},
        {37.336483,-121.879206},
        {37.336666,-121.879348},
        {37.336828,-121.879480},
        {37.336981,-121.879650},
        {37.337143,-121.879804},
        {37.337217,-121.879795},
        {37.337277,-121.879726},
        {37.337328,-121.879825}
};
#endif

bool path_plan_flag = false;
static int current_checkpoint_index = 0;

void command_car()
{
    DIRECTION_GPS_t command = {0};
    DEBUG_NAVIGATION_t debug_val = {0};
    DEBUG_BEARING_t debug_bearing = {0};
    NXT_COORD_t next_coordinates = {0};
    float heading_difference;
    float bearing;
    float diff_dist = cal_distance(global_present_coordinates.longitude , global_present_coordinates.latitude\
            ,checkpoint_path[current_checkpoint_index].longitude , checkpoint_path[current_checkpoint_index].latitude);

    bearing = cal_bearing(global_present_coordinates.longitude , global_present_coordinates.latitude\
            ,checkpoint_path[current_checkpoint_index].longitude , checkpoint_path[current_checkpoint_index].latitude);
    debug_bearing.BEARING = bearing;
    next_coordinates.LAT = checkpoint_path[current_checkpoint_index].latitude;
    next_coordinates.LONG = checkpoint_path[current_checkpoint_index].longitude;
    dbc_encode_and_send_NXT_COORD(&next_coordinates);
    dbc_encode_and_send_DEBUG_BEARING(&debug_bearing);
    heading_difference = (bearing - get_heading());
    debug_val.HEAD_DIFF = abs(heading_difference);
    debug_val.DIST_DIFF = diff_dist;
    dbc_encode_and_send_DEBUG_NAVIGATION(&debug_val);

    if(current_checkpoint_index != checkpoints_size)
    {
        if( diff_dist > 5)
        {
            if(heading_difference < -5 && heading_difference >= -90)
            {
                LED_ON(4);
                LED_OFF(2);
                LED_OFF(3);
                command.DIRECTION_VAL = steer_soft_left;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
            else if(heading_difference < -90)
            {
                LED_ON(4);
                LED_OFF(2);
                LED_OFF(3);
                command.DIRECTION_VAL = steer_hard_left;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
            else if(heading_difference > 5 && heading_difference <= 90)
            {
                LED_ON(2);
                LED_OFF(3);
                LED_OFF(4);
                command.DIRECTION_VAL = steer_soft_right;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
            else if (heading_difference > 90)
            {
                LED_ON(2);
                LED_OFF(3);
                LED_OFF(4);
                command.DIRECTION_VAL = steer_hard_right;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
            else if (heading_difference > -5 && heading_difference < 5)
            {
                LED_ON(2);
                LED_ON(3);
                LED_ON(4);
                command.DIRECTION_VAL = steer_straight;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
        }
        else if(diff_dist <= 5 && diff_dist > 0)
        {
            DEBUG_CHECKPOINT_t index = {0};
            current_checkpoint_index++;
            index.INDEX = current_checkpoint_index;
            dbc_encode_and_send_DEBUG_CHECKPOINT(&index);
            printf("Checkpoint reached\n");
        }
    }
    else
    {
        diff_dist = cal_distance(global_present_coordinates.longitude , global_present_coordinates.latitude\
                ,destination_coordinates.longitude , destination_coordinates.latitude);

        bearing = cal_bearing(global_present_coordinates.longitude , global_present_coordinates.latitude\
                ,destination_coordinates.longitude , destination_coordinates.latitude);

        dbc_encode_and_send_DEBUG_BEARING(&debug_bearing);
        heading_difference = (bearing - get_heading());
        debug_val.HEAD_DIFF = abs(heading_difference);
        debug_val.DIST_DIFF = diff_dist;
        dbc_encode_and_send_DEBUG_NAVIGATION(&debug_val);
        if( diff_dist > 5)
        {
            if(heading_difference < -5 && heading_difference >= -90)
            {
                LED_ON(4);
                LED_OFF(2);
                LED_OFF(3);
                command.DIRECTION_VAL = steer_soft_left;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
            else if(heading_difference < -90)
            {
                LED_ON(4);
                LED_OFF(2);
                LED_OFF(3);
                command.DIRECTION_VAL = steer_hard_left;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
            else if(heading_difference > 5 && heading_difference <= 90)
            {
                LED_ON(2);
                LED_OFF(3);
                LED_OFF(4);
                command.DIRECTION_VAL = steer_soft_right;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
            else if (heading_difference > 90)
            {
                LED_ON(2);
                LED_OFF(3);
                LED_OFF(4);
                command.DIRECTION_VAL = steer_hard_right;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
            else if (heading_difference > -5 && heading_difference < 5)
            {
                LED_ON(2);
                LED_ON(3);
                LED_ON(4);
                command.DIRECTION_VAL = steer_straight;
                dbc_encode_and_send_DIRECTION_GPS(&command);
            }
        }
        else if(diff_dist <= 5 && diff_dist > 0)
        {
            command.DIRECTION_VAL = 0;
            command.DESTINATION_REACHED = 1;
            current_checkpoint_index = 0;
            checkpoints_size = 0;
            dbc_encode_and_send_DIRECTION_GPS(&command);
        }
    }
}

int get_checkpoint(checkpoint current_coordinate)
{
    int min = 0;
    bool flag = false;
    float distance[30];
    checkpoint destination;
    for(int i = 0 ; i < checkpoints_array_size ; i++)
    {
        distance[i] = cal_distance(current_coordinate.longitude , current_coordinate.latitude\
                ,checkpoints_array[i].longitude , checkpoints_array[i].latitude);
        //printf("Dist[%d]: %f\n" , i , distance[i]);
    }
    for(int i = 0 ; i < checkpoints_array_size ; i++)
    {
        if(distance[min] > distance[i]  && \
                cal_distance(destination_coordinates.longitude , destination_coordinates.latitude , current_coordinate.longitude , current_coordinate.latitude) >\
                cal_distance(destination_coordinates.longitude , destination_coordinates.latitude , checkpoints_array[i].longitude , checkpoints_array[i].latitude))
        {
            min = i;
            //printf("Min:%d\n" , min);
            flag = true;
        }
    }
    memset(distance , 0 , sizeof(distance));
    if(flag == false)
    {
        destination.longitude = destination_coordinates.longitude;
        destination.latitude = destination_coordinates.latitude;
        return 100; //TODO: figure out a way to return destination
    }
    else
    {
        destination.longitude = checkpoints_array[min].longitude;
        destination.latitude = checkpoints_array[min].latitude;
        return min;
    }
}

void plan_path()
{
    if(path_plan_flag)
    {
        int index = 0 , result = 0;
        checkpoint current_checkpoint;
        current_checkpoint.longitude = global_present_coordinates.longitude;
        current_checkpoint.latitude = global_present_coordinates.latitude;
        result = get_checkpoint(current_checkpoint);
        checkpoint_path[index].latitude = checkpoints_array[result].latitude;
        checkpoint_path[index].longitude = checkpoints_array[result].longitude;
        checkpoints_size++;
        while(result != 100)
        {
            index++;
            result = get_checkpoint(checkpoint_path[index - 1]);
            checkpoint_path[index].latitude = checkpoints_array[result].latitude;
            checkpoint_path[index].longitude = checkpoints_array[result].longitude;
            checkpoints_size++;
        }
        checkpoints_size--;
        received_coordinates = true;
        CHECKPOINTS_t total_checkpoints = {0};
        total_checkpoints.COUNT = checkpoints_size;
        dbc_encode_and_send_CHECKPOINTS(&total_checkpoints);
        printf("Path planned\n");
        path_plan_flag = false;
    }
}

void receive_coordinates_ble(void)
{
    APP_DATA_LAT_LONG_t dst_coord = {0};
    can_msg_t can_msg;
    while (CAN_rx(can1, &can_msg, 0))
    {
        dbc_msg_hdr_t can_msg_hdr;
        can_msg_hdr.dlc = can_msg.frame_fields.data_len;
        can_msg_hdr.mid = can_msg.msg_id;
        switch(can_msg_hdr.mid)
        {
            case 182:
                dbc_decode_APP_DATA_LAT_LONG(&dst_coord, can_msg.data.bytes, &can_msg_hdr);
                destination_coordinates.latitude = dst_coord.DEST_LAT;
                destination_coordinates.latitude = destination_coordinates.latitude / 1000000.0;
                destination_coordinates.longitude = dst_coord.DEST_LONG;
                destination_coordinates.longitude = destination_coordinates.longitude / 1000000.0;
                destination_coordinates.longitude = destination_coordinates.longitude * -1;
                memset(checkpoint_path , 0 , sizeof(checkpoint_path));
                printf("dst coord: %f           %f" , destination_coordinates.latitude , destination_coordinates.longitude);
                //plan_path();
                checkpoints_size = 0;
                current_checkpoint_index = 0;
                path_plan_flag = true;
                break;
        }

    }
}
