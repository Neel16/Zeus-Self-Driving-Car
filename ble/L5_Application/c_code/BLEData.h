/*
 * BLEData.h
 *
 *  Created on: Apr 23, 2019
 *      Author: himan
 */

#ifndef L5_APPLICATION_C_CODE_BLEDATA_H_
#define L5_APPLICATION_C_CODE_BLEDATA_H_

#include <stdint.h>
#include <stdbool.h>
#include "c_uart2.h"
#include "c_uart3.h"
#include "stdio.h"
#include "string.h"
#include "can.h"
#include "c_period_callbacks.h"
#include "_can_dbc/generated_can.h"

void receive_and_update_BLE_1HZ();
void get_BLE_data();


#endif /* L5_APPLICATION_C_CODE_BLEDATA_H_ */
