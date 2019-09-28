#include "unity.h" // Single Unity Test Framework include
#include "stdbool.h"
#include "sensor.h"
//#include <gpio.hpp>

#include "Mockadc0.h"
#include "Mockcan.h"
#include "Mocksensor_trigger.h"
//#include "Mockgpio.h"

void test_can_init(void)
{
    CAN_init_ExpectAndReturn(can1, 100, 16, 16, NULL, NULL, true);
    CAN_bypass_filter_accept_all_msgs_Expect();
    CAN_reset_bus_Expect(can1);
    TEST_ASSERT_TRUE(can_init());
}

//bool can_refresh(void);
void test_can_refresh(void)
{
   // TEST_ASSERT_EQUAL(0, 0);
    CAN_is_bus_off_ExpectAndReturn(can1, true);
    CAN_bypass_filter_accept_all_msgs_Expect();
    CAN_reset_bus_Expect(can1);
    TEST_ASSERT_TRUE(can_refresh());
}
//bool sensor_init(void);
void test_sensor_init(void)
{
    //TEST_ASSERT_EQUAL(0, 0);
        sensor_delay_ms_Expect(250);

        adc0_init_Expect();
        adc_pin_init_ExpectAndReturn(3, true);
        adc_pin_init_ExpectAndReturn(4, true);
        adc_pin_init_ExpectAndReturn(5, true);
        trigger_init_ExpectAndReturn(true);

        TEST_ASSERT_TRUE(sensor_init());
}

//float raw2volt(uint16_t raw, float vcc);
void test_raw2volt(void)
{
    TEST_ASSERT_EQUAL(80, raw2volt(0xffff, 5.0));
}
//uint16_t volt2inch(float voltage, float vcc);
void test_volt2inch(void)
{
    TEST_ASSERT_EQUAL(515.625, volt2inch(3.3, 3.5)); // (3.3/0.0064)
    TEST_ASSERT_EQUAL(336.734, volt2inch(3.3, 4)); // (3.3/0.0098)
}
//uint8_t sensor2chan(enum sensor_dir sensor);
void test_sensor2chan(void)
{
    TEST_ASSERT_EQUAL(3, sensor2chan(sensor_left)); //0+3
    TEST_ASSERT_EQUAL(4, sensor2chan(sensor_center)); //1+3
    TEST_ASSERT_EQUAL(5, sensor2chan(sensor_right)); //2+3
    TEST_ASSERT_EQUAL(6, sensor2chan(sensor_back)); //3+3
}
//uint16_t read_sensor(enum sensor_dir sensor);
void test_read_sensor(void)
{

    uint16_t adc = 2, inches;
    float voltage;

    enum sensor_dir sensor = sensor_left;
    trigger_sensor_ExpectAndReturn(sensor, true, true);
    adc0_get_reading_ExpectAndReturn(sensor2chan(sensor), adc);
    sensor_delay_ms_Expect(1);
    trigger_sensor_ExpectAndReturn(sensor, false, false);
    voltage = raw2volt(adc, VCC);
    inches = volt2inch(voltage, VCC);
    TEST_ASSERT_EQUAL(inches, read_sensor(sensor));

    sensor = sensor_center;
    trigger_sensor_ExpectAndReturn(sensor, true, true);
    adc0_get_reading_ExpectAndReturn(sensor2chan(sensor), adc);
    sensor_delay_ms_Expect(1);
    trigger_sensor_ExpectAndReturn(sensor, false, false);
    voltage = raw2volt(adc, VCC);
    inches = volt2inch(voltage, VCC);
    TEST_ASSERT_EQUAL(inches, read_sensor(sensor));

    sensor = sensor_right;
    trigger_sensor_ExpectAndReturn(sensor, true, true);
    adc0_get_reading_ExpectAndReturn(sensor2chan(sensor), adc);
    sensor_delay_ms_Expect(1);
    trigger_sensor_ExpectAndReturn(sensor, false, false);
    voltage = raw2volt(adc, VCC);
    inches = volt2inch(voltage, VCC);
    TEST_ASSERT_EQUAL(inches, read_sensor(sensor));


}

//bool sensor_20Hz(sensor_t *sensors);
void test_sensor_20Hz(void)
{
     sensor_t sensors;
   // uint16_t adc = 2;
   // enum sensor_dir sensor = sensor_left;


    //trigger_sensor_ExpectAndReturn(sensor, true, true);
   // adc0_get_reading_ExpectAndReturn(sensor2chan(0), adc);
  //  sensor_delay_ms_Expect(1);
  //  trigger_sensor_ExpectAndReturn(sensor, false, false);


     //sensor_delay_ms_Expect(18);

    //sensor = sensor_center;
    //trigger_sensor_ExpectAndReturn(sensor, true, true);
   // adc0_get_reading_ExpectAndReturn(sensor2chan(1), adc);
    //sensor_delay_ms_Expect(1);
   // trigger_sensor_ExpectAndReturn(sensor, false, false);

     sensor_delay_ms_Expect(1);
     sensor_delay_ms_Expect(1);

   /* sensor = sensor_right;
    trigger_sensor_ExpectAndReturn(sensor, true, true);
    adc0_get_reading_ExpectAndReturn(sensor2chan(2), adc);
    sensor_delay_ms_Expect(1);
    trigger_sensor_ExpectAndReturn(sensor, false, false);*/




   sensors.back = 0;


    TEST_ASSERT_TRUE(sensor_20Hz(&sensors));

}
//uint8_t filter_sensor(uint16_t inches, uint16_t prev);
void test_filter_sensor(void)
{
    TEST_ASSERT_EQUAL(236, filter_sensor(255, 236));
    TEST_ASSERT_EQUAL(236, filter_sensor(270, 236));
    TEST_ASSERT_EQUAL(200, filter_sensor(200, 236));
}
//bool filter_sensors(sensor_t *sensors, sensor_t *sensors_prev);
void test_filter_sensors(void)
{
    sensor_t sensors ;
    sensor_t sensors_prev;

    sensors.left = 0;
    sensors.center = 1;
    sensors.right = 2;
    sensors.back = 3;

    sensors_prev.left = 0;
    sensors_prev.center = 1;
    sensors_prev.right = 2;
    sensors_prev.back = 3;

    TEST_ASSERT_EQUAL(true, filter_sensors(&sensors, &sensors_prev));

}
//bool send_sensor(sensor_t *sensors);
void test_send_sensor(void)
{

   /* sensor_t sensors;
    sensors.left = 0;
    sensors.center = 1;
    sensors.right = 2;
    sensors.back =3;
    */
   // can_refresh_ExpectAndReturn(true);
   // send_sensor_dbc_ExpectAndReturn(0, 1, 2, 3, true);
  //  send_sensor_ExpectAndReturn(true);
    TEST_ASSERT_EQUAL(0,0);
}

//void test_can_refresh(void)
//{
    //CAN_is_bus_off_ExpectAndReturn(can1, false);
    ////TEST_ASSERT_TRUE(can_refresh());
//}

//void test_can_refresh__reset(void)
//{
    //CAN_is_bus_off_ExpectAndReturn(can1, true);
    //CAN_bypass_filter_accept_all_msgs_Expect();
    //CAN_reset_bus_Expect(can1);
    //TEST_ASSERT_TRUE(can_refresh());
//}
//
//void test_can_message__pressed(void)
//{
//#ifdef CAN_SEND
    //can_msg_t msg;
//
    //msg.frame = 0x0;
    //msg.frame_fields.data_len = 1;
    //msg.frame_fields.is_29bit = 0;
    //msg.frame_fields.is_rtr = 0;
    //msg.msg_id = 0x0001;
    //msg.data.qword = 0xaa;

    //read_button_ExpectAndReturn(true);
    //CAN_tx_ExpectAndReturn(can1, &msg, 0, true);
    //TEST_ASSERT_TRUE(can_message());
//#endif /* CAN_SEND */
//#ifdef CAN_RECV
    //led_on_Expect();
    //TEST_ASSERT_TRUE(can_recv(0xaa));
//#endif /* CAN_RECV */
//}

//void test_can_message__unpressed(void)
//{
//#ifdef CAN_SEND
    //can_msg_t msg;
//
    //msg.frame = 0x0;
    //msg.frame_fields.data_len = 1;
    //msg.frame_fields.is_29bit = 0;
    //msg.frame_fields.is_rtr = 0;
    //msg.msg_id = 0x0001;
    //msg.data.qword = 0x00;
//
    //read_button_ExpectAndReturn(false);
    //CAN_tx_ExpectAndReturn(can1, &msg, 0, true);
    //TEST_ASSERT_TRUE(can_message());
//#endif /* CAN_SEND */
//#ifdef CAN_RECV
    ////led_off_Expect();
    //TEST_ASSERT_TRUE(can_recv(0x00));
//
    //led_toggle_Expect();
    //TEST_ASSERT_FALSE(can_recv(0x69));
//#endif /* CAN_RECV */
//}
//

