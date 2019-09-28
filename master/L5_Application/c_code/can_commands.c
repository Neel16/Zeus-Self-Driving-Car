#include "can_commands.h"
#define DISTANCE 60
#define MID_DISTANCE 50
#include "utilities.h"

extern DIRECTION_GPS_t gps_data;
extern SENSOR_SONARS_t sensor_data ;
extern APP_DATA_t app_start;

/**************************************************/
bool front_obstacle = 0;
bool left_obstacle = 0;
bool right_obstacle = 0;
bool start_flag=0;
//0 : no obstacle ; 1 : obstacle
/**************************************************/

void detect_obstacle_100hz()
{
    receive_CAN();
    DEBUG_OBSTACLE_FLAGS_t obstacle_flags = {0};
    uint8_t middle  =  sensor_data.SENSOR_SONARS_MIDDLE;
    uint8_t left    =  sensor_data.SENSOR_SONARS_LEFT;
    uint8_t right   =  sensor_data.SENSOR_SONARS_RIGHT;

    if(middle > MID_DISTANCE && left > DISTANCE && right > DISTANCE )   // no obstacle
    {
        front_obstacle  = 0;
        left_obstacle   = 0;
        right_obstacle  = 0;
    }
    else if(middle < MID_DISTANCE && left < DISTANCE && right < DISTANCE)   // wall in front
    {
        front_obstacle  = 1;
        left_obstacle   = 1;
        right_obstacle  = 1;
        c_LED_on(1);
        c_LED_on(2);
        c_LED_on(3);
    }
    else if(middle > MID_DISTANCE && (left < DISTANCE || right < DISTANCE))
    {
        if(left < right)
        {
            left_obstacle   = 1;
            right_obstacle  = 0;
            front_obstacle  = 0;
            c_LED_on(1);
            c_LED_off(2);
            c_LED_off(3);
        }
        else if(right < left)
        {
            left_obstacle   = 0;
            right_obstacle  = 1;
            front_obstacle  = 0;
            c_LED_off(1);
            c_LED_off(2);
            c_LED_on(3);
        }
    }
    else if(middle < MID_DISTANCE && (left > DISTANCE && right > DISTANCE))
    {
        front_obstacle  = 1;
        left_obstacle   = 0;
        right_obstacle  = 0;
        c_LED_off(1);
        c_LED_on(2);
        c_LED_off(3);
    }

    //Debug messages
    obstacle_flags.FRONT = front_obstacle;
    obstacle_flags.LEFT = left_obstacle;
    obstacle_flags.RIGHT = right_obstacle;
    dbc_encode_and_send_DEBUG_OBSTACLE_FLAGS(&obstacle_flags);

}

void navigation_10hz(uint32_t count)
{
    if(start_flag == 1)
    {
        if(front_obstacle)                              //front
        {
            stop();
            delay_ms(10);
            go_back();
        }
        else if(gps_data.DESTINATION_REACHED)
        {
            stop();
            stop();
        }
        else
        {
            switch(gps_data.DIRECTION_VAL)
            {
                case 0:
                    if(!right_obstacle && !left_obstacle)       //no obstacles
                        go_straight(1.7);

                    else if(!left_obstacle && right_obstacle)   // right
                        go_hard_left();

                    else if(left_obstacle && !right_obstacle)   //left
                        go_hard_right();

                    else if(right_obstacle && left_obstacle)    //only on left & right both
                    {
                        stop();
                        delay_ms(10);
                        go_back();
                    }
                    break;

                case -2:
                    if(!left_obstacle)
                        go_hard_left();
                    else
                        go_hard_right();
                    break;

                case -1:
                    if(!left_obstacle)
                        go_slight_left();
                    else
                        go_hard_right();
                    break;

                case 1:
                    if(!right_obstacle)
                        go_slight_right();
                    else
                        go_hard_left();
                    break;

                case 2:
                    if(!right_obstacle)
                        go_hard_right();
                    else
                        go_hard_left();
                    break;

                default:
                    printf("Default case!! Check GPS\n");
            }
        }

    }
    else if(!start_flag)
    {
        stop();
        stop();
    }
}


void start_cmd()
{
    if(app_start.START_CAR==1)
    {
        start_flag=1;
        printf("\nstart recieved");
    }
    else if(app_start.START_CAR==2)
    {
        start_flag=0;
        printf("STOP");
    }
}
