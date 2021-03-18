#ifndef I2C_h
#define I2C_h
#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>
#include <STM32F10x_StdPeriph_Driver/misc.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x_gpio.h> 
#include <STM32F10x_StdPeriph_Driver/stm32f10x_rcc.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x_i2c.h>

void init_I2C2(void);
void I2C_StartTransmission(I2C_TypeDef* I2Cx, uint8_t transmissionDirection,  uint8_t slaveAddress);
void I2C_WriteData(I2C_TypeDef* I2Cx, uint8_t data);
uint8_t I2C_ReadData(I2C_TypeDef* I2Cx);

#endif
