/*
 * c_io.h
 *
 *  Created on: Mar 3, 2019
 *      Author: NamdevPrabhugaonkar
 */

/**
 * @file
 * This is a C header file that other C code can se to access UART2
 * driver that is written in C++
 *
 * Note that we have a C header file, but its implementation in C++
 * such that we can invoke C++ Uart driver
 *
 * The C Unit-test framework can use this header file to "Mock" the
 * UART2 API and carry out the tests.
 */
#ifdef __cplusplus
extern "C" {
#endif

void toggle(uint8_t ledNum);
void LED_ON(uint8_t LED_no);
void LED_OFF(uint8_t LED_no);
void GPIO_setHigh();
void GPIO_setAsOutput();

#ifdef __cplusplus
}
#endif

