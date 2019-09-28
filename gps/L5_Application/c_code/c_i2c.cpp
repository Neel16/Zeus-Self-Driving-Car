/*
 * c_i2c.cpp
 *
 *  Created on: Apr 19, 2019
 *      Author: AmanChandan
 */
#include "c_i2c.h"
#include "i2c2.hpp"

bool i2c_init(unsigned int baudRate)
{
    return I2C2::getInstance().init(baudRate);
}
uint16_t i2c_readreg(uint8_t deviceAddress, uint8_t registerAddress)
{
    uint8_t heading_arr[2];
    uint16_t heading;
    I2C2::getInstance().readRegisters(deviceAddress , registerAddress , &heading_arr[0] , 2);
    heading = heading_arr[0] << 8 | heading_arr[1];
    return heading;
}
bool i2c_writereg(uint8_t deviceAddress, uint8_t registerAddress, uint8_t value)
{
    return I2C2::getInstance().writeReg(deviceAddress , registerAddress , value);
}
