// @file sensor_trigger.cpp
#include <utilities.h>
//#include <uart2.hpp>
#include <lpc_sys.h>
#include <gpio.hpp>
#include <can.h> /* for DBC */
#include "_can_dbc/generated_can.h"
#include "sensor_trigger.h"

// Assuming UART2 is already initialized. You may want to also add uart2_init() in C
//Uart2 *uart2 = NULL;
GPIO trigger_left(P1_19);
GPIO trigger_center(P1_20);
GPIO trigger_right(P1_22);
GPIO trigger_back(P1_23);

GPIO pw_in(P2_2);

typedef struct {
    uint64_t pw_time_up;
    uint64_t pw_time_down;
    float pulse;
} pw_time_t;
pw_time_t pw_timer = { 0 };

uint16_t pulse2inch(float pulse)
{
    float PW_SCALE = 147; /* 147 us/in */
    return (uint16_t)(pulse / PW_SCALE);
}

void sensor_delay_us(uint32_t us)
{
    delay_us(us);
}

void sensor_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

bool adc_pin_init(uint8_t channel)
{
    volatile uint32_t *pinsel;
    uint8_t offset, adc_mask;

    switch (channel) {
    case 2: /* k0_25 */
        pinsel = &LPC_PINCON->PINSEL1;
        offset = 18;
        adc_mask = 0x1;
        break;
    case 3: /* k0_26*/
        pinsel = &LPC_PINCON->PINSEL1;
        offset = 20;
        adc_mask = 0x1;
        break;
    case 4: /* k1_30 */
        pinsel = &LPC_PINCON->PINSEL3;
        offset = 28;
        adc_mask = 0x3;
        break;
    case 5: /* k1_31 */
        pinsel = &LPC_PINCON->PINSEL3;
        offset = 30;
        adc_mask = 0x3;
        break;
    default:
        return false;
    }

    *pinsel &= ~(0x3 << offset);
    *pinsel |= (adc_mask << offset);

    return true;
}

bool trigger_init(void)
{
    trigger_left.setAsOutput();
    trigger_center.setAsOutput();
    trigger_right.setAsOutput();
    trigger_back.setAsOutput();

    pw_in.setAsInput();
    eint3_enable_port2(2, eint_rising_edge, pw_rising);
    eint3_enable_port2(2, eint_falling_edge, pw_falling);

    return true;
}

bool trigger_sensor(sensor_dir dir, bool on)
{
    GPIO *sensor;

    switch (dir) {
        case sensor_left:
            sensor = &trigger_left;
            break;
        case sensor_center:
            sensor = &trigger_center;
            break;
        case sensor_right:
            sensor = &trigger_right;
            break;
        case sensor_back:
            sensor = &trigger_back;
            break;
        default:
            return false;
    }
    sensor->set(on);

    return true;
}

void pw_rising(void)
{
    pw_timer.pw_time_up = sys_get_uptime_us();
}

void pw_falling(void)
{
    pw_timer.pw_time_down = sys_get_uptime_us();
    pw_timer.pulse = (float)(pw_timer.pw_time_down - pw_timer.pw_time_up);
}

uint16_t get_pw_inch(void)
{
    return pulse2inch(pw_timer.pulse);
}

bool dbc_app_send_can_msg(uint32_t mid, uint8_t dlc, uint8_t bytes[8])
{
    can_msg_t can_msg = { 0 };
    can_msg.msg_id                = mid;
    can_msg.frame_fields.data_len = dlc;
    memcpy(can_msg.data.bytes, bytes, dlc);

    return CAN_tx(can1, &can_msg, 0);
}

bool send_sensor_dbc(uint8_t left, uint8_t center, uint8_t right, uint8_t back)
{
    SENSOR_SONARS_t sensor_cmd = { 0 };

    sensor_cmd.SENSOR_SONARS_LEFT = left;
    sensor_cmd.SENSOR_SONARS_MIDDLE = center;
    sensor_cmd.SENSOR_SONARS_RIGHT = right;
    sensor_cmd.SENSOR_SONARS_BACK = back;

    // This function will encode the CAN data bytes, and send the CAN msg using dbc_app_send_can_msg()
    return dbc_encode_and_send_SENSOR_SONARS(&sensor_cmd);
}
