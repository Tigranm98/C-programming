/* Copyright Instigate Robotics CJSC 2019 */
#ifndef UART_H 
#define UART_H

void send_command(USART_TypeDef* USARTx, const char *pucBuffer);
void usart_init(void);
void USART1_IRQHandler(void);
void send_message_gprs(USART_TypeDef* USARTx, const char *data);
int initialization_gprs(USART_TypeDef* USARTx);
void send_uart(USART_TypeDef* USARTx, unsigned char c); 
uint16_t receive_uart(USART_TypeDef* USARTx); 
void check_response();
void check_connect();
void check_error();
#endif 

