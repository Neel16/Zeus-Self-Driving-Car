#include "unity.h" // Single Unity Test Framework include
#include "trip_detector.h"

static trip_detector_S trip_detector;

void setUp(void) {
  trip_detector = trip_detector_init(3);
}
void tearDown(void) {
  puts("\ntearDown() is called after each test_* function (unless test is ignored)");
}

void test_trip_detector_init(void) {
  trip_detector = trip_detector_init(1);

  TEST_ASSERT_EQUAL(1, trip_detector.threshold);
  TEST_ASSERT_EQUAL(0, trip_detector.counter);
}

void test_trip_detector_add_sample__threshold(void) {
  trip_detector_add_sample(&trip_detector, true);
  trip_detector_add_sample(&trip_detector, true);
  TEST_ASSERT_EQUAL(2, trip_detector.counter);
  
  trip_detector_add_sample(&trip_detector, true);
  TEST_ASSERT_EQUAL(3, trip_detector.counter);
  
  trip_detector_add_sample(&trip_detector, true);
  TEST_ASSERT_EQUAL(4, trip_detector.counter);
}

void test_trip_detector_add_sample__reset(void) {
  trip_detector_add_sample(&trip_detector, true);
  trip_detector_add_sample(&trip_detector, true);
  TEST_ASSERT_EQUAL(2, trip_detector.counter);
  
  trip_detector_add_sample(&trip_detector, false);
  TEST_ASSERT_EQUAL(0, trip_detector.counter);
}

void test_trip_detector_is_tripped(void) {
  trip_detector_add_sample(&trip_detector, true);
  trip_detector_add_sample(&trip_detector, true);
  
  TEST_ASSERT_FALSE(trip_detector_is_tripped(&trip_detector));
  trip_detector_add_sample(&trip_detector, true);
  TEST_ASSERT_TRUE(trip_detector_is_tripped(&trip_detector));
}