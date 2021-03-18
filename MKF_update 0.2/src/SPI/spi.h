/* Copyright Instigate Robotics CJSC 2019 */
#ifndef SPI_H 
#define SPI_H 

void SPI1_Init();
void spi3_init();
void spi1_send(uint16_t data);
void spi3_send(uint16_t data);
int16_t spi1_read(void);
uint8_t SPIx_Transfer(uint8_t data);
uint16_t spi_send16(uint16_t outByte);
void spi_write_16(uint16_t Address , uint16_t Data, GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
void spi_write_32(uint16_t Address , uint32_t Data, GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint16_t spi_read_16(uint16_t Address, GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
uint32_t spi_read_32(uint16_t Address, GPIO_TypeDef* GPIOx, uint16_t gpio_pin);
#endif 

