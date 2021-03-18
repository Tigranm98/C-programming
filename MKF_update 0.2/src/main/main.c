/* ******************************************************************************
 * @file    main.c
 * @author  Instigate Robotics CJSC
 * @version V0.1.0
 * @date    10-September-2019
 * @brief   This is the main file for the Phase Chaanger project.
 */

/* Standard libraries headers */
#include <STM32F10x_StdPeriph_Driver/misc.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x_gpio.h> 
#include <STM32F10x_StdPeriph_Driver/stm32f10x_i2c.h> 
#include <STM32F10x_StdPeriph_Driver/stm32f10x_tim.h> 
/* Standard libraries from this project */
#include "rcc_configuration.h"
#include <debug/assert.h>
#include <debug/delay.h>
#include <display/LCD.h>
#include <servo/servo.h>
#include <MPU6050/MPU6050.h>
#include <MPU6050/HAL_MPU6050.h>
#include <stdlib.h>
#include <math.h>

void lcd_init(){
	LCDI2C_init(0x27, 20, 4);
	LCDI2C_backlight();
	LCDI2C_noCursor();
	LCDI2C_noBlink();
	char text_ax[] = "A.X";
	char text_ay[] = "A.Y";
	char text_az[] = "A.Z";
	char text_gx[] = "G.X";
	char text_gy[] = "G.Y";
	char text_gz[] = "G.Z";
	LCDI2C_setCursor(0, 0);
	LCDI2C_write_String(text_ax);
	LCDI2C_setCursor(0, 1);
	LCDI2C_write_String(text_ay);
	LCDI2C_setCursor(0, 2);
	LCDI2C_write_String(text_az);
	LCDI2C_setCursor(10, 0);
	LCDI2C_write_String(text_gx);
	LCDI2C_setCursor(10, 1);
	LCDI2C_write_String(text_gy);
	LCDI2C_setCursor(10, 2);
	LCDI2C_write_String(text_gz);
}

void lcd_cl(int f, int s, int r)
{
	   LCDI2C_setCursor(f, r);
	   for (int i=0; i<s; i++)
	   {
		   LCDI2C_write(' ');
	   }
}

int main(void)
{	
	servo1_timer_gpio_init();
	servo2_timer_gpio_init();
	lcd_init();
	MPU6050_Initialize();
	char ex[] = "0";
	int16_t AX;
	int16_t AY;
	int16_t AZ;
	int16_t GX;
	int16_t GY;
	int16_t GZ;
	int16_t  AccelGyro[14]={0};
	while(1)
	{
		servo_set_pos(AX);
		lcd_cl(5, 5, 0);
		lcd_cl(15, 5, 0);
		lcd_cl(5, 5, 1);
		lcd_cl(15, 5, 1);
		lcd_cl(5, 5, 2);
		lcd_cl(15, 5, 2);
		lcd_cl(5, 5, 3);
		lcd_cl(15, 5, 3);
		MPU6050_GetRawAccelGyro(AccelGyro);
		AX = floor(AccelGyro[0]/91);
		AY = floor(AccelGyro[1]/91);
		AZ = floor(AccelGyro[2]/91);
		GX = floor(AccelGyro[3]/131);
		GY = floor(AccelGyro[4]/131);
		GZ = floor(AccelGyro[5]/131);
		itoa(AX, ex, 10);
		LCDI2C_setCursor(4, 0);
		LCDI2C_write_String(ex);
		itoa(AY, ex, 10);
		LCDI2C_setCursor(4, 1);
		LCDI2C_write_String(ex);
		itoa(AZ, ex, 10);
		LCDI2C_setCursor(4, 2);
		LCDI2C_write_String(ex);
		itoa(GX, ex, 10);
		LCDI2C_setCursor(14, 0);
		LCDI2C_write_String(ex);
		itoa(GY, ex, 10);
		LCDI2C_setCursor(14, 1);
		LCDI2C_write_String(ex);
		itoa(GZ, ex, 10);
		LCDI2C_setCursor(14, 2);
		LCDI2C_write_String(ex);
	}
}
