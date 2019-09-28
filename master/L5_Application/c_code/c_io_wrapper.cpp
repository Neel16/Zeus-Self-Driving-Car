/*
 * c_io_wrapper.cpp
 *
 */
#include"c_io_wrapper.h"
#include "io.hpp"


void c_LED_on(uint8_t ledNumber)
{
    LED::getInstance().on(ledNumber);
}

void c_LED_off(uint8_t ledNumber)
{
    LED::getInstance().off(ledNumber);
}
void c_LED_display_set_number(char number)
{
	LED_Display::getInstance().setNumber(number);
}
