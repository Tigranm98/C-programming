/* ADE9078
 ADE9078.h - Simple library for operating the ADE9078 Single-Phase AC Line measurement IC over SPI for Arduino Uno
  example library for operating the ADE9078 3-Phase AC Line measurement IC over SPI interfacing

ADE9078 board and library created by: David Brady, Jeff Baez, and Jacky Wan 11/2018

Code major development by David Brady

PCB design done by Esteban Granizo and Peter Nguyen (12/2018)

University of California, Irvine - California Plug Load Research Center (CalPlug)

December 2018 - Library First Release (December 2018) - Library Latest Release for ADE9078

Copyright (C) The Regents of the University of California, 2019

  Note: Please refer to the Analog Devices ADE9078 datasheet - much of this library was based directly on the statements and methods provided in it!  Their authors get paid, trust them over us!

  Released into the public domain.
*/

//Architecture Control:
//Select the architecture in use, one but not both!  Do this in your main program file, here are examples for the defines:
//#define AVRESP8266  //this architecture is for AVR/Arduino boards and the ESP8266

#ifndef ADE9078_h 
#define ADE9078_h

//Debug Control:
//#define ADE9078_VERBOSE_DEBUG //This line turns on verbose debug via serial monitor (Normally off or //'ed).  Use sparingly and in a test program to debug operation!  Turning this on can take a lot of memory and the delay from USB printing out every statement is taxing temporally!  This is non-specific and for all functions, beware, it's a lot of output that can challenge device memory!  Reported bytes are typically in HEX
//#define ADE9078_Calibration //Shows the output of the type cast functions to allow the raw values to be seen - very helpful in calibration

//#define ADE9078_CRC_Output //Used to output debug information for the CRC check functions

/* Standard libraries headers */
#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>
#include <STM32F10x_StdPeriph_Driver/misc.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x_gpio.h> 
#include <STM32F10x_StdPeriph_Driver/stm32f10x_rcc.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x_spi.h>
#include <debug/delay.h>

/* Standard libraries from this project */
#include <stdlib.h>
#include "ADE9078registers.h"
#include "ADE9078Calibrations.h"
#include "LastReads.h" 
#include "SPI/spi.h"

#define byte uint8_t
typedef uint8_t bool;


//INIT
void setup_chip1(void);
void setup_chip2(void);
int init_ADE9078(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_period(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint8_t get_version(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_power_factor_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t get_phase_calib_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t get_inst_voltage_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t get_inst_voltage_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t get_inst_voltage_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_voltage(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_A_avrms();
double get_A_bvrms();
double get_A_cvrms();
double get_B_avrms();
double get_B_bvrms();
double get_B_cvrms();
void read_B_vrms();
void read_A_vrms();

double get_A_airms();
double get_A_birms();
double get_A_cirms();
double get_B_airms();
double get_B_birms();
double get_B_cirms();
void read_A_irms();
void read_B_irms();

uint32_t get_inst_current_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t get_inst_current_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t get_inst_current_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t get_inst_current(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);

void readFilterBasedCurrent();

uint32_t get_vpeak(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t get_ipeak(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_peak(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double read_32bit_and_scale(uint16_t readRegister, GPIO_TypeDef* GPIOx, uint16_t gpio_pin);

double read_watt_hours_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double read_watt_hours_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double read_watt_hours_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_watt_hours(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);

void  read_fundamental_reactive_power(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_filter_based_current(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_power_factor(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_period(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);

double get_inst_apparent_power_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_inst_apparent_power_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_inst_apparent_power_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_inst_apparent_power(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);

double get_inst_active_power_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_inst_active_power_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_inst_active_power_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_inst_active_power(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);

double get_inst_reactive_power_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_inst_reactive_power_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
double get_inst_reactive_power_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void read_inst_reactive_power(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);

uint32_t get_phnoload(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);


//void isPageFull(int);
//void whichPageIsFull();
void spiBurstResampledWFB(uint16_t);
void configure_wfb(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void reset_full_buffer_bit(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void stop_filling_buffer(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void spiBurstResampledWFB_Avonly(uint16_t);
void burst_av_only(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
int is_done_sampling(GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint8_t spiRead8(uint16_t address);   // Read, inbound function: address, NOTE: This is an example function, 8 Bit registers for returned values are not used in the ADE9078
void spi_x_enable_slave(int16_t gpio_pin, GPIO_TypeDef* gpio_x);
void spi_x_disable_slave(int16_t gpio_pin, GPIO_TypeDef* gpio_x);

bool checkBit(int data, int i);
double decimalize(uint32_t input, double factor, double offset, bool absolutevalue);
double decimalizeSigned(int32_t input, double factor, double offset, bool absolutevalue);
uint16_t crc16(char* pData, int length);
byte functionBitVal(uint16_t addr, uint8_t byteVal);


#endif
