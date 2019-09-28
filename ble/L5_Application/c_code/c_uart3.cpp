#include "c_uart3.h"
#include "uart3.hpp"
#include "io.hpp"

// Assuming UART3 is already initialized
// You may want to also add uart2_init() in C

bool c_uart3_init(unsigned int baudRate, int rxQSize=64, int txQSize=64)
{
	Uart3::getInstance().init(baudRate,rxQSize,txQSize);
}

bool c_uart3_getChar(char *byte, uint32_t timeout_ms) {
    Uart3::getInstance().getChar(byte, timeout_ms);
}

void c_setNumber(char num) {
	LED_Display::getInstance().setNumber(num);
}

void getstring(char* pBuff, int maxLen, unsigned int timeout){

    Uart3::getInstance().gets( pBuff, maxLen, timeout);
    pBuff[maxLen+1]='\0';
}
