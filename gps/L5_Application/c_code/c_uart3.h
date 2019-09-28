/**
 * @file
 * This is a C header file that other C code can use to access UART2
 * driver that is written in C++
 *
 * Note that we have a C header file, but its implementation in C++
 * such that we can invoke C++ Uart driver
 *
 * The C Unit-test framework can use this header file to "Mock" the
 * UART2 API and carry out the tests.
 */
#ifndef UART2_C_H__
#define UART2_C_H__
#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

bool uart3_init(unsigned int baudRate, int rxQSize, int txQSize);

bool uart3_getchar(char *byte, uint32_t timeout_ms);

bool uart3_putchar(char string_val , uint32_t timeout_ms);

void uart3_LED_L(char c);

void uart3_LED_R(char c);

bool uart3_putline(char *string_val , uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif
#endif /* UART2_C_H__ */
