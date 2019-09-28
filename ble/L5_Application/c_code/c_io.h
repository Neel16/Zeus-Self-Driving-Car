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

#include <stdint.h>
#include <stdbool.h>
 // neel: adding LED display code
void toggle(uint8_t ledNum);

void c_LED_display_set_number(char number);

#ifdef __cplusplus
}
#endif

