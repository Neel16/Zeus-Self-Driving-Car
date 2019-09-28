/*
 * c_io.cpp
 *
 *  Created on: Mar 3, 2019
 *      Author: NamdevPrabhugaonkar
 */

#include "io.hpp"
#include "c_io.h"
#include "LED.hpp"


void toggle(uint8_t LED_no)
{
    LED::getInstance().toggle(LED_no);
}

void c_LED_display_set_number(char number)
{
    LED_Display::getInstance().setNumber(number);
}
