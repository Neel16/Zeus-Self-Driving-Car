#include "unity.h" // Single Unity Test Framework include
#include <stdio.h>
#include <stdbool.h>

void setUp(void) {
  puts("\nsetUp() is called before each test_* function");
}
void tearDown(void) {
  puts("\ntearDown() is called after each test_* function (unless test is ignored)");
}

void test_passing(void) {
  //TEST_ASSERT_EQUAL(1, 1);
}

void test_failing(void) {
  //TEST_FAIL_MESSAGE("Intentional Failure");
}

void test_ignored(void) {
  //TEST_IGNORE();
}
