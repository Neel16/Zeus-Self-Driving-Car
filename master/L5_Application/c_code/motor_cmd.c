#include "motor_cmd.h"
//MOTOR_COMMAND_t motor_cmd = {0};	//Need to reset every time a fn is called



void go_straight(float speed)
{
	//printf("go_straight\n");
	MOTOR_COMMAND_t motor_cmd = {0};
	motor_cmd.SPEED_cmd = speed;
	motor_cmd.STEER_cmd = 0;
	motor_cmd.DIRECTION_cmd = 1;
	dbc_encode_and_send_MOTOR_COMMAND(&motor_cmd);
}

void go_hard_left()
{
	//printf("go_left\n");
	MOTOR_COMMAND_t motor_cmd = {0};
	motor_cmd.SPEED_cmd = 1.7;
	motor_cmd.STEER_cmd = -2;
	motor_cmd.DIRECTION_cmd = 1;
	dbc_encode_and_send_MOTOR_COMMAND(&motor_cmd);
}
void go_hard_right()
{
	//printf("go_right\n");
	MOTOR_COMMAND_t motor_cmd = {0};
	motor_cmd.SPEED_cmd = 1.7;
	motor_cmd.STEER_cmd = 2;
	motor_cmd.DIRECTION_cmd = 1;
	dbc_encode_and_send_MOTOR_COMMAND(&motor_cmd);
}

void go_slight_left()
{
    //printf("go_slight left\n");
    MOTOR_COMMAND_t motor_cmd = {0};
    motor_cmd.SPEED_cmd = 1.7;
    motor_cmd.STEER_cmd = -1;
    motor_cmd.DIRECTION_cmd = 1;
    dbc_encode_and_send_MOTOR_COMMAND(&motor_cmd);
}

void go_slight_right()
{
    //printf("go_slight right\n");
    MOTOR_COMMAND_t motor_cmd = {0};
    motor_cmd.SPEED_cmd = 1.7;
    motor_cmd.STEER_cmd = 1;
    motor_cmd.DIRECTION_cmd = 1;
    dbc_encode_and_send_MOTOR_COMMAND(&motor_cmd);
}

void stop()
{
    //printf("STOPPP!!\n");
    MOTOR_COMMAND_t motor_cmd = {0};
    motor_cmd.SPEED_cmd = 0;
    motor_cmd.STEER_cmd = 0;
    motor_cmd.DIRECTION_cmd = 0;
    dbc_encode_and_send_MOTOR_COMMAND(&motor_cmd);
}
void go_back()
{
    //printf("GOBACK!!\n");
	MOTOR_COMMAND_t motor_cmd = {0};
	motor_cmd.SPEED_cmd = 1.3;
	motor_cmd.STEER_cmd = 2;
	motor_cmd.DIRECTION_cmd = 0;
	//printf("\nmotor_cmd.DIRECTION_cmd: %f",motor_cmd.DIRECTION_cmd);
	dbc_encode_and_send_MOTOR_COMMAND(&motor_cmd);
}
