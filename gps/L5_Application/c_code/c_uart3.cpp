#include <c_uart3.h>
#include "uart3.hpp"
#include "io.hpp"

// Assuming UART3 is already initialized
// You may want to also add uart2_init() in C
bool uart3_init(unsigned int baudRate, int rxQSize, int txQSize)
{
    Uart3::getInstance().init(baudRate,  rxQSize,  txQSize);
    return true;
}


bool uart3_getchar(char *byte, uint32_t timeout_ms) {
    Uart3::getInstance().getChar(byte, timeout_ms);
    return true;
}

bool uart3_putline(char *string_val , uint32_t timeout_ms)
{
    Uart3::getInstance().putline(string_val , 0);
    return true;
}
bool uart3_putchar(char string_val , uint32_t timeout_ms)
{
    Uart3::getInstance().putChar(string_val , 0);
    return true;
}

void uart3_LED_L(char c)
{
    LED_Display::getInstance().setLeftDigit(c);
}

void uart3_LED_R(char c)
{
    LED_Display::getInstance().setRightDigit(c);
}
