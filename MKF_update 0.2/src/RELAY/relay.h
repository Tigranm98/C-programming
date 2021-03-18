/* ******************************************************************************
 * @file    relay.h
 * @author  Instigate Robotics CJSC
 * @version V0.1.0
 * @date    15.06.2020
 * @brief   This is the main file for the Phase Chaanger project.
 */
#ifndef ADE9078_h 
#define ADE9078_h

#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>
#include <STM32F10x_StdPeriph_Driver/misc.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x_gpio.h> 
#include <debug/assert.h>
#include <debug/delay.h>
#include <stdbool.h>
//#include <main/definitions.h>

void triac_gpio_setup(void);
void starter_gpio_setup(void);
void configure_relay_gpio(uint16_t gpio_pin, GPIO_TypeDef* gpio_x);
void configure_all_relays(void);
void relay_turn_on_off(bool f, uint16_t gpio_pin, GPIO_TypeDef* gpio_x);
void configure_current_status_pins(uint16_t gpio_pin, GPIO_TypeDef* gpio_x);
bool check_generator_line_status();
void configure_generator_status_pins();
#endif
