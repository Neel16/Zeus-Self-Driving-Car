/*
 * gps.h
 *
 *  Created on: Apr 20, 2019
 *      Author: NamdevPrabhugaonkar
 */


#ifndef GPS_H_
#define GPS_H_
#include <stdio.h>
#include <stdbool.h>
#include "common.h"

void fill_gps_buffer();
extern int fix_true;
float convert_to_decimal_coord(float coordinates);
gps_struct parse_gps_data(char *gga_string);

bool send_debug_gps();

#endif /* GPS_H_ */
