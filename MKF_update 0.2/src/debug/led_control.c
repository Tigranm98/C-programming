/* ******************************************************************************
 * @file    led_control.c
 * @author  Instigate Robotics CJSC
 * @version V0.1.0
 * @date    10-September-2019
 * @brief   This is the main file for the Phase Chaanger project.
 */

/* Standard libraries headers */
#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>

/* Headers from this project */
#include "led_control.h"
#include <debug/delay.h>

/**
 * @brief  
 * @param  None
 * @retval None
 */
void configure_led_gpio()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void led_turn_on_off(bool f)
{
    if (f) {
        GPIO_ResetBits(GPIOC, GPIO_Pin_13);
    } else {
        GPIO_SetBits(GPIOC, GPIO_Pin_13);
    }
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void led_signal_assert()
{
    led_turn_on_off(on);
    delay(100000);
    led_turn_on_off(off);
    delay(100000);
    led_turn_on_off(on);
    delay(100000);
    led_turn_on_off(off);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void led_blink_signal()
{
    led_turn_on_off(on);
    delay(1000000);
    led_turn_on_off(off);
    delay(1000000);
    led_turn_on_off(on);
    delay(1000000);
    led_turn_on_off(off);
}
