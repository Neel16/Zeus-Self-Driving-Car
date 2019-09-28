#include "unity.h" // Single Unity Test Framework include
#include "c_period_callbacks.h"
//#include "Mockc_uart_comm.h"

//static period_callbacks_S period_callbacks;

void setUp(void) {
    //trip_detector = trip_detector_init(3);
}
void tearDown(void) {
    //puts("\ntearDown() is called after each test_* function (unless test is ignored)");
}

void test_C_period_init(void)
{
    TEST_ASSERT_TRUE(C_period_init());
}

void test_C_period_reg_tlm(void)
{
    TEST_ASSERT_TRUE(C_period_reg_tlm());
}

void test_C_period_1Hz(void)
{
    TEST_ASSERT_EQUAL(0, 0);
}

void test_C_period_10Hz(void)
{
    TEST_ASSERT_EQUAL(0, 0);
}

void test_C_period_100Hz(void)
{
    TEST_ASSERT_EQUAL(0, 0);
}

void test_C_period_1000Hz(void)
{
    TEST_ASSERT_EQUAL(0, 0);
}
