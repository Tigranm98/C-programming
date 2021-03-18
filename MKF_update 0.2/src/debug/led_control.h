/* ******************************************************************************
 * @file    led_control.h
 * @author  Instigate Robotics CJSC
 * @version V0.1.0
 * @date    10-September-2019
 * @brief   This is the main file for the Phase Chaanger project.
 */

#ifndef LED_CONTROL_H 
#define LED_CONTROL_H

#define on 1
#define off 0

typedef uint8_t bool;

void configure_led_gpio();
void led_turn_on_off(bool f);
void led_signal_assert();
void led_blink_signal();

#endif // LED_CONTROL_H
