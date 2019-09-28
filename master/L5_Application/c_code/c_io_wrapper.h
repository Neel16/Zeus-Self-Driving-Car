/*
 * c_io_wrapper.h
 *
 */

#ifndef C_IO_WRAPPER_H_
#define C_IO_WRAPPER_H_
#ifdef __cplusplus
extern "C" {
#endif
//#include "common_headers.h"
//#include "io.hpp"
#include <stdint.h>
#include <stdbool.h>


void c_LED_on(uint8_t ledNumber);
void c_LED_off(uint8_t ledNumber);
void c_LED_display_set_number(char number);

#ifdef __cplusplus
}
#endif

#endif /* C_IO_WRAPPER_H_ */
