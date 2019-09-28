/*
 * io.cpp

 *
 *  Created on: Feb 19, 2019
 *      Author: AmanChandan
 */
#include "io.h"
#include "temperature_sensor.hpp"
#include "switches.hpp"

int c_getcelsius()
{
    return TemperatureSensor::getInstance().getCelsius();
}

bool c_read_switch(int num)
{
    return Switches::getInstance().getSwitch(num);
}
