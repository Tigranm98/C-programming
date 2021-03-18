/* ******************************************************************************
 * @file    rcc_configuration.h
 * @author  Instigate Robotics CJSC
 * @version V0.1.0
 * @date    10-September-2019
 * @brief   This is the main file for the Phase Chaanger project.
 */

#ifndef RCC_CONFIGURATION_H 
#define RCC_CONFIGURATION_H

void clock_configuration();
void rcc_configuration(void);
void nvic_configuration(void);
void mco_gpio_conf(void);

#endif // RCC_CONFIGURATION_H 
