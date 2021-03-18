/* Standard libraries headers */
#include <debug/delay.h>
#include <debug/led_control.h>
#include "definitions.h"

void usart_init(void)
{
    /* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
 
    /* NVIC Configuration */
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
    /* Configure the GPIOs */
    GPIO_InitTypeDef GPIO_InitStructure;
 
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
 
    /* Configure the USART1 */
    USART_InitTypeDef USART_InitStructure;
 
    /* USART1 configuration ------------------------------------------------------*/
    /* USART1 configured as follow:
        - BaudRate = 115200 baud
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
        - USART Clock disabled
        - USART CPOL: Clock is active low
        - USART CPHA: Data is captured on the middle
        - USART LastBit: The clock pulse of the last data bit is not output to
            the SCLK pin
     */
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
 
    USART_Init(USART1, &USART_InitStructure);
 
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
 
    /* Enable the USART1 Receive interrupt: this interrupt is generated when the
        USART1 receive data register is not empty */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
}


uint16_t receive_uart(USART_TypeDef* USARTx)  // Receive 
{
	while(USART_GetFlagStatus(USARTx,USART_FLAG_RXNE) == RESET){}
	return USART_ReceiveData(USARTx);
}

void check_response()
{
    while (!flag_response);
    flag_response = false;
    led_signal();
}

void check_connect()
{
    while (!connect_flag);
    connect_flag = false;
    led_signal();
}

void check_error()
{
    if (flag_error){
        led_signal_assert();
    }
}

int initialization_gprs(USART_TypeDef* USARTx)
{
    send_command(USARTx, " AT\n");
    check_response();
    send_command(USARTx, "AT+CGATT=1\n");
    check_error();
    check_response();
    send_command(USARTx, "AT+CIPSTART=TCP,185.44.230.253,8071\n");
    check_connect();
    return true; 
}

void send_message_gprs(USART_TypeDef* USARTx, const char *data) 
{
    send_command(USARTx, "at+cipsend\n");
    delay(10000000);
    send_command(USARTx, data);
    USART_SendData(USARTx,  '\n');
    USART_SendData(USARTx,  26);
    check_response();
	GPIO_ResetBits(GPIOA, GPIO_Pin_6);
}

void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
    {
        RXc = USART_ReceiveData(USART1);
        if (flag_k) {
            if (RXc == 'K') {
                flag_response = true;
            }
            flag_k = false;
        }
        if (RXc == 'O') {
            flag_k = true;
        }
        if (flag_n){
            if(RXc == 'E'){
                connect_flag = true;
            }
            flag_n = false;
        }
        if (RXc == 'N'){
            flag_n = true;
        }
        if (flag_e){
            if (RXc == 'R'){
                flag_error = true;
            }
        }
        if (RXc == 'E'){
            flag_e = true;
        }
    }
}

void send_command(USART_TypeDef* USARTx, const char *pucBuffer)
{
    while (*pucBuffer)
    {
        USART_SendData(USARTx, *pucBuffer++);
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET)
        { }
    }
}
