/* Copyright Instigate Robotics CJSC 2019 */
#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>
#include <CMSIS/core_cm3.h>

// For store tick counts in us
static __IO uint32_t usTicks;

// SysTick_Handler function will be called every 1 us
void SysTick_Handler()
{
    if (usTicks != 0)
    {
        usTicks--;
    }
}

void delay_init()
{
    // Update SystemCoreClock value
    SystemCoreClockUpdate();
    // Configure the SysTick timer to overflow every 1 us
 //   SysTick_Config(SystemCoreClock / 1000000);
}

void delay_us(uint32_t us)
{
    // Reload us value
    usTicks = us;
    // Wait until usTick reach zero
    while (usTicks);
}


void delay(__IO uint32_t nCount)
{
  while(nCount--);
}


void delay_ms(uint32_t ms)
{
    ms = (ms * 2500) + 62200;
    delay(ms);
}


