#include "c_uart2.h"
#include "uart2.hpp"
#include "io.hpp"

// Assuming UART2 is already initialized
// You may want to also add uart2_init() in C

bool c_uart2_init(unsigned int baudRate, int rxQSize=32, int txQSize=64)
{
	Uart2::getInstance().init(baudRate,rxQSize,txQSize);
}

bool c_uart2_putline(const char *out, uint32_t timeout_ms)
{
	Uart2::getInstance().putline(out, timeout_ms);
}

int c_getRawValue(void)
{
	return Light_Sensor::getInstance().getRawValue();
}
