/* ******************************************************************************
 * @file    relay.c
 * @author  Instigate Robotics CJSC
 * @version V0.1.0
 * @date    10-September-2019
 * @brief   This is the main file for the Phase Chaanger project.
 */

/* Standard libraries headers */
#include "relay.h"

GPIO_InitTypeDef GPIO_InitStructure;

#define on 1
#define off 0
#define true 1
#define false 0

/**
 * @brief  
 * PB(14,15), PC(3, 5-7), PA(0-4)
 * @param  None
 * @retval None
 */
void configure_relay_gpio(uint16_t gpio_pin, GPIO_TypeDef* gpio_x)
{
    GPIO_InitStructure.GPIO_Pin = gpio_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(gpio_x, &GPIO_InitStructure);
}

/**
 * @brief  
 * PB(12,15), PC(6), PA(0-4)
 * @param  None
 * @retval None
 */ 
void configure_all_relays(void)
{
    configure_relay_gpio(GPIO_Pin_6, GPIOC);	
    configure_relay_gpio(GPIO_Pin_1, GPIOA);	
    configure_relay_gpio(GPIO_Pin_2, GPIOA);	
    configure_relay_gpio(GPIO_Pin_3, GPIOA);	
    configure_relay_gpio(GPIO_Pin_4, GPIOA);	
    configure_relay_gpio(GPIO_Pin_12, GPIOB);	
    configure_relay_gpio(GPIO_Pin_13, GPIOB);
    configure_relay_gpio(GPIO_Pin_14, GPIOB);
    configure_relay_gpio(GPIO_Pin_15, GPIOB);	
}


/**
 * @brief  
 * @param  None
 * @retval None
 */
void relay_turn_on_off(bool f, uint16_t gpio_pin, GPIO_TypeDef* gpio_x)
{
    if (f) {
        GPIO_SetBits(gpio_x, gpio_pin);
    } else {
        GPIO_ResetBits(gpio_x, gpio_pin);
    }
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void triac_gpio_setup(void)
{
    // Cofigure PA1 as input with internal pull-up resistor
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void starter_gpio_setup(void)
{
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}
