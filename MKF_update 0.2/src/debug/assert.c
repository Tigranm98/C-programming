/* Copyright Instigate Robotics CJSC 2019 */

#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>
#include "led_control.h"

void assert_failed(const uint8_t* file, uint32_t line)
{
        (void)file;
        (void)line;
        /* User can add his own implementation (file name and line number) */
        /* Infinite loop */
        while (1) {
            led_signal_assert();
        }
}
