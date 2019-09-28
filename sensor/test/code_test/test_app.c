#include "unity.h"
#include "app.h"
#include "Mockbit_count.h"

void setUp(void) {
}

void tearDown(void) {
}

void test_app(void) {
	bit_count_get_uint8_ExpectAndReturn(100, 0);
	app_test(100);
}
