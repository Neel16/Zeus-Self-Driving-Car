/*
 * c_io.cpp
 *
 *  Created on: Mar 3, 2019
 *      Author: NamdevPrabhugaonkar
 */

#include "io.hpp"
#include "c_io.h"
#include "LED.hpp"
#include "gpio.hpp"

GPIO buzzer(P1_19);

void toggle(uint8_t LED_no)
{
    LED::getInstance().toggle(LED_no);
}

void LED_ON(uint8_t LED_no)
{
    LED::getInstance().on(LED_no);
}

void LED_OFF(uint8_t LED_NO)
{
    LED::getInstance().off(LED_NO);
}
void GPIO_setAsOutput()
{
    buzzer.setAsOutput();
}

void GPIO_setHigh()
{
    buzzer.setHigh();
}
