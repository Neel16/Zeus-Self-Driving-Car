#ifndef L5_APPLICATION_C_CODE_MOTOR_CMD_H_
#define L5_APPLICATION_C_CODE_MOTOR_CMD_H_

#include "common_headers.h"

void receive_and_update_MOTORSENSOR_10hz();
void go_straight(float speed);
void go_hard_left();
void go_hard_right();
void go_slight_left();
void go_slight_right();
void stop();
void go_back();


#endif /* L5_APPLICATION_C_CODE_MOTOR_CMD_H_ */
