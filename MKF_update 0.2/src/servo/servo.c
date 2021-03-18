#include <STM32F10x_StdPeriph_Driver/stm32f10x.h> 
#include <STM32F10x_StdPeriph_Driver/stm32f10x_gpio.h> 
#include <STM32F10x_StdPeriph_Driver/stm32f10x_rcc.h>
#include <stdlib.h>
#include "servo.h"
#include <display/LCD.h>

#define SERVO_MIN 500
char servo[] = "0";
int pos1;
int pos2;

GPIO_InitTypeDef GPIO_Servo1;
GPIO_InitTypeDef GPIO_Servo2;
GPIO_InitTypeDef GPIO_Blink;

void servo1_timer_gpio_init()
{
	// Config GPIOC For Blink 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_Blink.GPIO_Pin = GPIO_Pin_13;
	GPIO_Blink.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Blink.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_Blink);
	// Config GPIO_A and TIM2 For Servo SHIM 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	GPIO_Servo1.GPIO_Pin = GPIO_Pin_1;
	GPIO_Servo1.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Servo1.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_Servo1);
	TIM2->CCER |= (TIM_CCER_CC2E);
	TIM2->CCMR1|= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
	TIM2->PSC = 5;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void servo2_timer_gpio_init()
{
	// Config GPIO_A and TIM2 For Servo SHIM 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
	GPIO_Servo2.GPIO_Pin = GPIO_Pin_2;
	GPIO_Servo2.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Servo2.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_Servo2);
	TIM2->CCER |= (TIM_CCER_CC3E);
	TIM2->CCMR2|= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);
	TIM2->PSC = 5;
	TIM2->CR1 |= TIM_CR1_CEN;
}

void servo_set_pos(int pos) {
	pos1=180-pos;
	if (pos<0){
		pos=0;
		TIM2->CCR2 = SERVO_MIN + (pos * 56.5);
		TIM2->CCR3 = SERVO_MIN + (pos1 * 56.5);
	}else if(pos>180){
		pos=180;
		TIM2->CCR2 = SERVO_MIN + (pos * 56.5);
		TIM2->CCR3 = SERVO_MIN + (pos1 * 56.5);
	}
	TIM2->CCR2 = SERVO_MIN + (pos * 56.5);
	TIM2->CCR3 = SERVO_MIN + (pos1 * 56.5);
	delay(500);
}

void servo_delay() {
	volatile uint32_t i;
	for (i=1; i != 0xFFF; i++);
}

void servo_180() {
	for (int i=0; i<180; i++){
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
                servo_set_pos(i);
                delay(5000);
		//itoa(i, servo, 10);
		//LCDI2C_setCursor(col, row);
		//LCDI2C_write_String(servo);
        }
        for (int j=180; j>0; j--){
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
                servo_set_pos(j);
                delay(5000);
		//itoa(j, servo, 10);
		//LCDI2C_setCursor(col, row);
		//LCDI2C_write_String(servo);
		//if (j==99) {
		//	LCDI2C_setCursor(col+2, row);
		//	LCDI2C_write(' ');
		//}
		//if (j==9) {
		//	LCDI2C_setCursor(col+1, row);
		//	LCDI2C_write(' ');
		//}
        }
}

void servo_pos(int pos){
		servo_set_pos(pos);
		LCDI2C_setCursor(11, 3);
		itoa(pos, servo, 10);
		LCDI2C_write_String(servo);
		if (pos<90) {
			LCDI2C_setCursor(13, 3);
			LCDI2C_write(' ');
		}
		if (pos<10) {
			LCDI2C_setCursor(12, 3);
			LCDI2C_write(' ');
		}
}

void servo_set_rand(){
	        int r=rand() % 60;
                servo_pos(r);
                delay(10000000);
}
