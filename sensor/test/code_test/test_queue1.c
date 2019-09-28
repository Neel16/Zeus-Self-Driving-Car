// @file test_queue1.c
#include "unity.h" // Single Unity Test Framework include
#include "queue1.h"

void test_queue__init(void)
{
    queue_S q;
    TEST_ASSERT_EQUAL(queue__get_count(&q), 0);
}

void test_queue__push(void)
{
    queue_S q;
    queue__init(&q);
    TEST_ASSERT_TRUE(queue__push(&q, 69));
    TEST_ASSERT_EQUAL(queue__get_count(&q), 1);
    TEST_ASSERT_TRUE(queue__push(&q, 70));
    TEST_ASSERT_EQUAL(queue__get_count(&q), 2);
}

void test_queue__push__overflow(void)
{
    queue_S q;
    queue__init(&q);
    for (uint8_t i = 0; i < 100; i++) {
        TEST_ASSERT_TRUE(queue__push(&q, i));
    }
    TEST_ASSERT_EQUAL(queue__get_count(&q), 100);

    TEST_ASSERT_FALSE(queue__push(&q, 69));
    TEST_ASSERT_EQUAL(queue__get_count(&q), 100);
}

void test_queue__pop(void)
{
    queue_S q;
    uint8_t pop;

    queue__init(&q);
    for (uint8_t i = 0; i < 100; i++) {
        TEST_ASSERT_TRUE(queue__push(&q, i));
    }

    TEST_ASSERT_TRUE(queue__pop(&q, &pop));
    TEST_ASSERT_EQUAL(pop, 99);
    TEST_ASSERT_EQUAL(queue__get_count(&q), 99);
    for (uint8_t i = 98; i > 0; i--) {
        TEST_ASSERT_TRUE(queue__pop(&q, &pop));
        TEST_ASSERT_EQUAL(pop, i);
        TEST_ASSERT_EQUAL(queue__get_count(&q), i);
    }
    TEST_ASSERT_TRUE(queue__pop(&q, &pop));
    TEST_ASSERT_EQUAL(pop, 0);
    TEST_ASSERT_EQUAL(queue__get_count(&q), 0);
}

void test_queue__pop__underflow(void)
{
    queue_S q;
    uint8_t pop;

    queue__init(&q);
    for (uint8_t i = 0; i < 100; i++) {
        TEST_ASSERT_TRUE(queue__push(&q, i));
    }
    for (uint8_t i = 99; i > 0; i--) {
        TEST_ASSERT_TRUE(queue__pop(&q, &pop));
        TEST_ASSERT_EQUAL(pop, i);
        TEST_ASSERT_EQUAL(queue__get_count(&q), i);
    }
    TEST_ASSERT_TRUE(queue__pop(&q, &pop));
    TEST_ASSERT_EQUAL(pop, 0);
    TEST_ASSERT_EQUAL(queue__get_count(&q), 0);

    TEST_ASSERT_FALSE(queue__pop(&q, &pop));
    TEST_ASSERT_EQUAL(queue__get_count(&q), 0);
}

void test_queue__get_count(void)
{
    queue_S q;
    uint8_t pop;

    queue__init(&q);
    for (uint8_t i = 0; i < 69; i++) {
        TEST_ASSERT_TRUE(queue__push(&q, i));
    }
    TEST_ASSERT_EQUAL(queue__get_count(&q), 69);
    for (uint8_t i = 0; i < (101-69); i++) {
        queue__push(&q, i);
    }
    TEST_ASSERT_EQUAL(queue__get_count(&q), 100);

    for (uint8_t i = 0; i < 101; i++) {
        queue__pop(&q, &pop);
    }
    TEST_ASSERT_EQUAL(queue__get_count(&q), 0);
}
