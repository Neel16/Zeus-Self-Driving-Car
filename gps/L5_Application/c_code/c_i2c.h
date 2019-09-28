/*
 * c_i2c.h
 *
 *  Created on: Apr 19, 2019
 *      Author: AmanChandan
 */

#ifndef C_I2C_H_
#define C_I2C_H_
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

bool i2c_init(unsigned int baudRate);
uint16_t i2c_readreg(uint8_t deviceAddress, uint8_t registerAddress);
bool i2c_writereg(uint8_t deviceAddress, uint8_t registerAddress, uint8_t value);

#ifdef __cplusplus
}
#endif
#endif /* C_I2C_H_ */
