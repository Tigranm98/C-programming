/* ******************************************************************************
 * @file    spi.c
 * @author  Instigate Robotics CJSC
 * @version V0.1.0
 * @date    10-September-2019
 * @brief   This is the main file for the Phase Chaanger project.
 */

/* Standard libraries headers */
#include <STM32F10x_StdPeriph_Driver/misc.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x_gpio.h> 
#include <STM32F10x_StdPeriph_Driver/stm32f10x_spi.h>

SPI_InitTypeDef SPI_InitStructure;

/*
 *
 * @brief  Clears the ADCx's interrupt pending bits.
 * @param  None
 * @retval None
 */
void SPI1_Init()
{
    GPIO_InitTypeDef GPIO_InitDef;
    SPI_InitTypeDef SPI_InitDef;
    // initialize init structs
    GPIO_StructInit(&GPIO_InitDef);
    SPI_StructInit(&SPI_InitDef);
    // initialize clocks
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 | RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA, ENABLE);
    // initialize A5/SCK alternate function push-pull (50 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
    // initialize A6/MISO input pull-up (50MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
    // initialize A7/MOSI alternate function push-pull (50 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitDef);
    // initialize SPI master
    // for slave, no need to define SPI_BaudRatePrescaler
    SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitDef.SPI_Mode = SPI_Mode_Master;
    SPI_InitDef.SPI_DataSize = SPI_DataSize_8b;     // 8-bit transactions
    SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
    SPI_InitDef.SPI_CPOL = SPI_CPOL_Low;            // CPOL = 0, clock idle low
    SPI_InitDef.SPI_CPHA = SPI_CPHA_2Edge;          // CPHA = 1
    SPI_InitDef.SPI_NSS = SPI_NSS_Hard;             // use hardware SS
    //SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64; // APB2 72/64 = 1.125 MHz
    // SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256; // APB2 72/256 = 0.28 MHz
    SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; // APB2 72/16 = 4.5 MHz
    SPI_InitDef.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitDef);
    SPI_Cmd(SPI1, ENABLE);
}

/*
 *
 * @brief  Clears the ADCx's interrupt pending bits.
 * @param  None
 * @retval None
 */
void spi3_init()
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitTypeDef GPIO_InitDef;
    SPI_InitTypeDef SPI_InitDef;
    // initialize init structs
    GPIO_StructInit(&GPIO_InitDef);
    SPI_StructInit(&SPI_InitDef);
    // initialize clocks
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE); // this is on a different bus
    GPIO_PinRemapConfig (GPIO_Remap_SWJ_JTAGDisable, ENABLE );
    // Init SS pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    // Init SS pin
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    // initialize B13/SCK alternate functio push-pull (10 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitDef);
    // initialize B4/MISO input pull-up (10 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_4;
    //GPIO_InitDef.GPIO_Mode_IPD = 0x28,
    //GPIO_InitDef.GPIO_Mode_IPU = 0x48,
    //GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitDef);
    // initialize B5/MOSI alternate function push-pull (10 MHz)
    GPIO_InitDef.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitDef.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitDef.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_Init(GPIOB, &GPIO_InitDef);
    //  initialize SPI master
    SPI_InitDef.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitDef.SPI_Mode = SPI_Mode_Master;
    SPI_InitDef.SPI_DataSize = SPI_DataSize_16b;     // 8-bit transactions
    SPI_InitDef.SPI_FirstBit = SPI_FirstBit_MSB;    // MSB first
    SPI_InitDef.SPI_CPOL = SPI_CPOL_Low;            // CPOL = 0, clock idle low
    SPI_InitDef.SPI_CPHA = SPI_CPHA_1Edge;              // CPHA = 0
    SPI_InitDef.SPI_NSS = SPI_NSS_Soft;             // use hardware SS
    //SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32; // APB1 36/32 = 1.125 MHz
    // SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; // APB1 36/128 = 0.28 MHz
    SPI_InitDef.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; // APB1 36/4 = 9 MHz
    SPI_InitDef.SPI_CRCPolynomial = 7;
    SPI_Init(SPI3, &SPI_InitDef);
    SPI_Cmd(SPI3, ENABLE);
}

/**
 * @brief 
 * @param  None
 * @retval None
 */
uint16_t spi1_read(void)
{
    SPI1->DR = 0; 
    while(!(SPI1->SR & SPI_SR_RXNE));
    return SPI1->DR;
}

/**
 * @brief
 * @param  None
 * @retval None
 */
void spi1_send(uint16_t data)
{
    SPI_I2S_SendData(SPI1, data);
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET); 
}

/**
 * @brief  Clears the ADCx's interrupt pending bits.
 * @param  None
 * @retval None
 */
void spi3_send(uint16_t data)
{
    SPI_I2S_SendData(SPI3, data);
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET); 
}

//TODO
uint16_t spi_send16(uint16_t outByte)
{
    while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE));
    SPI_I2S_SendData(SPI3, outByte); //send
    while(!SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE));
    return SPI_I2S_ReceiveData(SPI3); //read recieived
}

uint8_t SPIx_Transfer(uint8_t data)
{
    // Write data to be transmitted to the SPI data register
    SPI3->DR = data;
    // Wait until transmit complete
    while (!(SPI3->SR & (SPI_I2S_FLAG_TXE)));
    // Wait until receive complete
    while (!(SPI3->SR & (SPI_I2S_FLAG_RXNE)));
    // Wait until SPI is not busy anymore
    while (SPI3->SR & (SPI_I2S_FLAG_BSY));
    // Return received data from SPI data register
    return SPI3->DR;
}

void spi_write_16(uint16_t Address , uint16_t Data, GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint16_t temp_address;
    GPIO_ResetBits(GPIOx, gpio_pin);
    temp_address = ((Address << 4) & 0xFFF0);   //shift address  to align with cmd packet
    spi_send16(temp_address);
    spi_send16(Data);
    GPIO_SetBits(GPIOx, gpio_pin);
}

/* 
Description: Writes 32bit data to a 32 bit register. 
Input: Register address, data
Output:-
*/

void spi_write_32(uint16_t Address , uint32_t Data, GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint16_t temp_address;
    uint16_t temp_highpacket;
    uint16_t temp_lowpacket;
    temp_highpacket= (Data & 0xFFFF0000)>>16;
    temp_lowpacket= (Data & 0x0000FFFF);
    GPIO_ResetBits(GPIOx, gpio_pin);
    temp_address = ((Address << 4) & 0xFFF0);   //shift address  to align with cmd packet
    spi_send16(temp_address);
    spi_send16(temp_highpacket);
    spi_send16(temp_lowpacket);
    GPIO_SetBits(GPIOx, gpio_pin);
 
}

uint16_t spi_read_16(uint16_t Address, GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint16_t temp_address;
    uint16_t return_data;
    GPIO_ResetBits(GPIOx, gpio_pin);
    temp_address = (((Address << 4) & 0xFFF0)+8);
    spi_send16(temp_address);
    return_data = spi_send16(0);
    GPIO_SetBits(GPIOx, gpio_pin);
    return return_data;
}

/* 
Description: Reads 32bit data from register. 
Input: Register address
Output: 32 bit data
*/

uint32_t spi_read_32(uint16_t Address, GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint16_t temp_address;
    uint16_t temp_highpacket;
    uint16_t temp_lowpacket;
    uint32_t return_data;
    GPIO_ResetBits(GPIOx, gpio_pin);
    temp_address = (((Address << 4) & 0xFFF0)+8);
    spi_send16(temp_address);
    temp_highpacket = spi_send16(0);
    temp_lowpacket = spi_send16(0);
    GPIO_SetBits(GPIOx, gpio_pin);
    return_data = temp_highpacket << 16;
    return_data = return_data + temp_lowpacket;
    return return_data;
}

//void init_ADE_regs()
//{
//  //delay(1000);
//  spi_write_16(ADDR_RUN,0x0000); 
//  spi_write_32(ADDR_AVGAIN,0x00000002); 
//  spi_write_32(ADDR_BVGAIN,0x00000002); 
//  spi_write_32(ADDR_CVGAIN,0x00000002); 
//  spi_write_32(ADDR_AIGAIN,0x00000002); 
//  spi_write_32(ADDR_BIGAIN,0x00000002); 
//  spi_write_32(ADDR_CIGAIN,0x00000002);
//  spi_write_32(ADDR_APGAIN,0x00000002); 
//  spi_write_32(ADDR_BPGAIN,0x00000002); 
//  spi_write_32(ADDR_CPGAIN,0x00000002); 
//  //ade9000.spi_write_32(ADDR_AWATTOS,-14831); 
//  spi_write_32(ADDR_ACCMODE,0x0000); 
//  delay(500); //  spi_write_16(ADDR_RUN,VAL_RUN); //}
//}

//void init_chip(void)
//{
//    spi_write_32(APGAIN_32, init.powerAGain);
//    spi_write_32(BPGAIN_32, init.powerBGain);
//    spi_write_32(CPGAIN_32, init.powerCGain);
//    uint16_t pgaGain = (init.vCGain << 12) + (init.vBGain << 10) + (init.vCGain << 8) + (init.iNGain << 6) + (init.iCGain << 4) + (init.iBGain << 2) + init.iAGain;
//    spi_write_16(PGA_GAIN_16, pgaGain);
//    uint32_t vLevelData = 0x117514;
//    // #5 : Write VLevel 0x117514
//    spi_write_32(VLEVEL_32, vLevelData);
//    spi_write_16(CONFIG0_32, 0x00000000);
//    // #7:  If current transformers are used, INTEN and ININTEN in the CONFIG0 register must = 0
//    // Table 24 to determine how to configure ICONSEL and VCONSEL in the ACCMODE register
//    uint16_t settingsACCMODE = (init.iConsel << 6) + (init.vConsel << 5);
//    spi_write_16(ACCMODE_16, settingsACCMODE);
//    spi_write_16(RUN_16, 1);  // 8: Write 1 to Run register
//    spi_write_16(EP_CFG_16, 1);  // 9: Write 1 to EP_CFG register
//    /*
//    Potentially useful registers to configure:
//    The following were in the 9078:
//      0x49A ZX_LP_SEL : to configure "zero crossing signal"
//      0x41F PHNOLOAD : To say if something is "no load".
//      Phase calibrations, such as APHCAL1_32
//    */
//    spi_write_16(CONFIG1_16, 0x0000);
//    spi_write_16(CONFIG2_16, 0x0000);
//    spi_write_16(CONFIG3_16, 0x0000);
//    spi_write_32(DICOEFF_32, 0xFFFFE000); // Recommended by datasheet
//    spi_write_16(EGY_TIME_16, 0x0001);
//    spi_write_16(EP_CFG_16, 0x0021); // RD_EST_EN=1, EGY_LD_ACCUM=0, EGY_TMR_MODE=0, EGY_PWR_EN=1
//}

