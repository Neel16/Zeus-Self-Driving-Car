#include "unity.h" // Single Unity Test Framework include
#include "uart_lab.h"

#include "Mockc_uart_comm.h"

//static period_callbacks_S period_callbacks;

void setUp(void) {
    //trip_detector = trip_detector_init(3);
}
void tearDown(void) {
    //puts("\ntearDown() is called after each test_* function (unless test is ignored)");
}

void test_digits2chars(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    digits2chars(0, &a, &b);
    TEST_ASSERT_EQUAL('0', a);
    TEST_ASSERT_EQUAL('0', b);

    digits2chars(1, &a, &b);
    TEST_ASSERT_EQUAL('0', a);
    TEST_ASSERT_EQUAL('1', b);

    digits2chars(69, &a, &b);
    TEST_ASSERT_EQUAL('6', a);
    TEST_ASSERT_EQUAL('9', b);

    digits2chars(169, &a, &b);
    TEST_ASSERT_EQUAL('6', a);
    TEST_ASSERT_EQUAL('9', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_orientation__left(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    as_getX_ExpectAndReturn(2);
    as_getY_ExpectAndReturn(1);
    as_getZ_ExpectAndReturn(-1);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('L', a);
    TEST_ASSERT_EQUAL('E', b);

    as_getX_ExpectAndReturn(65535);
    as_getY_ExpectAndReturn(65535);
    as_getZ_ExpectAndReturn(0);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('L', a);
    TEST_ASSERT_EQUAL('E', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_orientation__right(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    as_getX_ExpectAndReturn(-2);
    as_getY_ExpectAndReturn(1);
    as_getZ_ExpectAndReturn(-1);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('R', a);
    TEST_ASSERT_EQUAL('I', b);

    as_getX_ExpectAndReturn(-65535);
    as_getY_ExpectAndReturn(65535);
    as_getZ_ExpectAndReturn(0);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('R', a);
    TEST_ASSERT_EQUAL('I', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_orientation__forward(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    as_getX_ExpectAndReturn(1);
    as_getY_ExpectAndReturn(2);
    as_getZ_ExpectAndReturn(-1);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('F', a);
    TEST_ASSERT_EQUAL('O', b);

    as_getX_ExpectAndReturn(65534);
    as_getY_ExpectAndReturn(65535);
    as_getZ_ExpectAndReturn(0);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('F', a);
    TEST_ASSERT_EQUAL('O', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_orientation__backward(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    as_getX_ExpectAndReturn(1);
    as_getY_ExpectAndReturn(-2);
    as_getZ_ExpectAndReturn(-1);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('B', a);
    TEST_ASSERT_EQUAL('A', b);

    as_getX_ExpectAndReturn(65534);
    as_getY_ExpectAndReturn(-65535);
    as_getZ_ExpectAndReturn(0);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('B', a);
    TEST_ASSERT_EQUAL('A', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_orientation__up(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    as_getX_ExpectAndReturn(1);
    as_getY_ExpectAndReturn(-1);
    as_getZ_ExpectAndReturn(2);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('U', a);
    TEST_ASSERT_EQUAL('P', b);

    as_getX_ExpectAndReturn(65534);
    as_getY_ExpectAndReturn(0);
    as_getZ_ExpectAndReturn(65535);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('U', a);
    TEST_ASSERT_EQUAL('P', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_orientation__down(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    as_getX_ExpectAndReturn(1);
    as_getY_ExpectAndReturn(-1);
    as_getZ_ExpectAndReturn(-2);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('D', a);
    TEST_ASSERT_EQUAL('O', b);

    as_getX_ExpectAndReturn(65534);
    as_getY_ExpectAndReturn(0);
    as_getZ_ExpectAndReturn(-65535);
    get_orientation(&a, &b);
    TEST_ASSERT_EQUAL('D', a);
    TEST_ASSERT_EQUAL('O', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_temperature(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    ts_getCelsius_ExpectAndReturn(0.1);
    get_temperature(&a, &b);
    TEST_ASSERT_EQUAL('0', a);
    TEST_ASSERT_EQUAL('0', b);

    ts_getCelsius_ExpectAndReturn(69.420);
    get_temperature(&a, &b);
    TEST_ASSERT_EQUAL('6', a);
    TEST_ASSERT_EQUAL('9', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_light(void)
{
#ifdef UART_SEND
    char a = 'a', b = 'b';

    ls_getPercentValue_ExpectAndReturn(0.1);
    get_light(&a, &b);
    TEST_ASSERT_EQUAL('0', a);
    TEST_ASSERT_EQUAL('0', b);

    ls_getPercentValue_ExpectAndReturn(69.420);
    get_light(&a, &b);
    TEST_ASSERT_EQUAL('6', a);
    TEST_ASSERT_EQUAL('9', b);

    ls_getPercentValue_ExpectAndReturn(100.0);
    get_light(&a, &b);
    TEST_ASSERT_EQUAL('0', a);
    TEST_ASSERT_EQUAL('0', b);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_get_sense(void)
{
#ifdef UART_SEND
    TEST_ASSERT_TRUE(true);
#else
    TEST_IGNORE();
#endif /* UART_SEND */
}

void test_uart_init(void)
{
#ifdef UART_SEND
    uart2_init_ExpectAndReturn(true);
#endif /* UART_SEND */
#ifdef UART_RECV
    uart3_init_ExpectAndReturn(true);
#endif /* UART_RECV */
    TEST_ASSERT_TRUE(uart_init());
}

void test_uart_1Hz(void)
{
#ifdef UART_SEND
    test_get_sense();
    //uart2_putChar_ExpectAnyArgsAndReturn(true);
    //uart2_putChar_ExpectAnyArgsAndReturn(true);
#endif /* UART_SEND */
#ifdef UART_RECV
    uart3_getChar_ExpectAnyArgsAndReturn(true);
    uart3_getChar_ExpectAnyArgsAndReturn(true);
    ld_setLeftDigit_ExpectAnyArgs();
    ld_setRightDigit_ExpectAnyArgs();
    TEST_ASSERT_TRUE(uart_1Hz());
#endif /* UART_RECV */
}
