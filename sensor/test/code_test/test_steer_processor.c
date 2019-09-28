#include "unity.h"
#include "steer_processor.h"

#include "Mocksteering.h"

void test_steer_processor__move_left(void) {
    steer_left_Expect();
    steer_processor(70, 49);
    steer_left_Expect();
    steer_processor(10, 1);
    steer_left_Expect();
    steer_processor(100, 10);
    steer_left_Expect();
    steer_processor(50, 49);
}

void test_steer_processor__move_right(void) {
    steer_right_Expect();
    steer_processor(49, 70);
    steer_right_Expect();
    steer_processor(1, 10);
    steer_right_Expect();
    steer_processor(10, 100);
    steer_right_Expect();
    steer_processor(49, 50);
}

void test_steer_processor__both_sensors_less_than_threshold(void) {
    steer_left_Expect();
    steer_processor(20, 10);
    steer_right_Expect();
    steer_processor(10, 20);

    steer_left_Expect();
    steer_processor(49, 48);
    steer_right_Expect();
    steer_processor(48, 49);

    steer_processor(49, 49);
    steer_processor(0, 0);
}

// Hint: If you do not setup an Expect()
// then this test will only pass none of the steer functions is called
void test_steer_processor__both_sensors_more_than_threshold(void) {
    steer_processor(50, 50);
    steer_processor(50, 1000);
    steer_processor(1000, 50);
    steer_processor(69, 420);
    steer_processor(420, 69);
    steer_processor(50, 51);
    steer_processor(51, 50);
}

// Do not modify this test case
// Modify your implementation of steer_processor() to make it pass
// This tests corner case of both sensors below the threshold
void test_steer_processor(void) {
  steer_right_Expect();
  steer_processor(10, 20);

  steer_left_Expect();
  steer_processor(20, 10);
}
