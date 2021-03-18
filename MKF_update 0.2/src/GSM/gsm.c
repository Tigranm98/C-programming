/* Standard libraries headers */
//#include <STM32F10x_StdPeriph_Driver/misc.h>
//#include <STM32F10x_StdPeriph_Driver/stm32f10x.h>
#include <STM32F10x_StdPeriph_Driver/stm32f10x_usart.h>
/* Heasers from this project */
#include "gsm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <debug/delay.h>
//#include <debug/assert.h>
//#include <debug/led_control.h>

GPIO_InitTypeDef GPIO_InitStructure;
ErrorStatus HSEStartUpStatus;
char rx_data[11] = {'\0'}; 


char json___[] = "{";

/**
 * @brief
 * @param  none
 * @retval none
 */
void gsm_init(void)
{
    /* Enable USART1 and GPIOA clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
    /* NVIC Configuration */
    NVIC_Configuration();
    /* Configure the GPIOs */
    GPIO_Configuration();
    /* Configure the USART1 */
    USART_Configuration();
    /* Enable the USART1 Receive interrupt: this interrupt is generated when the
         USART1 receive data register is not empty */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
 //   init_settings();
}

/**
 * @brief
 * @param  none
 * @retval none
 */
void USART_Configuration(void)
{
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
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    /* Enable USART1 */
    USART_Cmd(USART1, ENABLE);
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    /* Enable the USARTx Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    #ifdef  VECT_TAB_RAM  
        /* Set the Vector Table base location at 0x20000000 */
        NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
    #else  /* VECT_TAB_FLASH  */
        /* Set the Vector Table base location at 0x08000000 */
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    #endif
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)           
    {          
        i = USART_ReceiveData(USART1);
        if(j == 10){
            rx_data[j] = i;
            j = 0;
        }
        else{
            rx_data[j++] = i;
        }
        rx_data[j] = '\0';
    }
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void send_at(const char data[256])
{   
    int timeout_cnt = 0;
    send_data(data);
    while(1){
        timeout_cnt++;
        if(timeout_cnt >= 1000){
            //send_data("TIMEOUT\r\n");
            break;
        }
        if(strstr(rx_data, "OK") != NULL){
            break;
        }
        if(strstr(rx_data, "DOWNLOAD") != NULL){
            break;
        }
        if(strstr(rx_data, "ERROR") != NULL){
            //send_data("Error\r\n");
            break;
        }
        if(strstr(rx_data, "+HTTPACTION:") != NULL){
            //send_data("Error\r\n");
            break;
        }
        delay(10);
    }
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void send_http(const char data[256])
{   
    int timeout_cnt = 0;
    send_data(data);
    while(1){
        timeout_cnt++;
        if(timeout_cnt >= 1000){
            //send_data("TIMEOUT\r\n");
            break;
        }
        if(strstr(rx_data, "ERROR") != NULL){
            //send_data("Error\r\n");
            break;
        }
        if(strstr(rx_data, "+HTTPACTION:") != NULL){
            send_data("~~~~~~~~~~\r\n");
            break;
        }
        delay_ms(10);
    }
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void init_settings() 
{
    for(int at_cnt= 0; at_cnt<=5;at_cnt++){
        //print("ATE0\r\n");
        send_data("AT\r\n");
        //delay(100000);
    }
    send_at("ATE0\r\n");
    delay_ms(200);
    send_at("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
    delay_ms(200);
    send_at("AT+SAPBR=3,1,\"APN\",\"inet.ucom.am\"\r\n");
    delay_ms(200);
    send_at("AT+SAPBR=1,1\r\n");
    delay_ms(200);
    send_at("AT+SAPBR=2,1\r\n");
    //delay_ms(2000);
    for(int at_cnt= 0; at_cnt<=5;at_cnt++){
        //print("ATE0\r\n");
        send_at("AT+SAPBR=2,1\r\n");
        delay_ms(200);
        //delay(100000);
    }
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void GPIO_Configuration(void)
{
    /* Configure USART1 Tx (PA.09) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* Configure USART1 Rx (PA.10) as input floating */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount)
{
    while(ulCount--)
    {
        // Last Version USART_SendData(USART1,(uint16_t) *pucBuffer++);
        USART_SendData(USART1, *pucBuffer++);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
}


/*
main_phase_1_consumption[256];
main_phase_2_consumption[256];
main_phase_3_consumption[256];

second_phase_1_consumption[256];
second_phase_2_consumption[256];
second_phase_3_consumption[256];
*/


void main_line_voltage(const char main_p_1[256], const char main_p_2[256], const char main_p_3[256]){   
    strncat(main_phase_1_voltage, main_p_1, 256); 
    strncat(main_phase_2_voltage, main_p_2, 256); 
    strncat(main_phase_3_voltage, main_p_3, 256); 
}


void second_line_voltage(const char second_p_1[256], const char second_p_2[256], const char second_p_3[256]){   
    
    strncat(second_phase_1_voltage, second_p_1, 256); 
    strncat(second_phase_2_voltage, second_p_2, 256); 
    strncat(second_phase_3_voltage, second_p_3, 256); 
}


void main_line_consumption(const char main_p_1[256], const char main_p_2[256], const char main_p_3[256]){   
    
    strncat(main_phase_1_consumption, main_p_1, 256); 
    strncat(main_phase_2_consumption, main_p_2, 256); 
    strncat(main_phase_3_consumption, main_p_3, 256); 
}


void second_line_consumption(const char second_p_1[256], const char second_p_2[256], const char second_p_3[256]){   
    
    strncat(second_phase_1_consumption, second_p_1, 256); 
    strncat(second_phase_2_consumption, second_p_2, 256); 
    strncat(second_phase_3_consumption, second_p_3, 256); 

}

// (Main Line)           (Second Line)
// (phase1,phase2,phase3 ,phase1,phase2,phase3  )
//
void generate_json()
{
    //char json[] = "{"; 
    char end_char[] = "\",";
    char end[] = "},";
    char end_[] = "}";
    char end__[] = "}}}";
    char s_char[2] = "\"";

    char main_line[] = "\"main_line\":{"; 
    
    /// PHASE 1
    char phase_1[256] = "\"phase1\":{"; 
    char phase_1_power_c[256] = "\"power_consumption\":\""; 
    strncat(phase_1_power_c, main_phase_1_consumption, 256); 
    strncat(phase_1_power_c, end_char, 256); 
    char phase_1_voltage[256] = "\"voltage\":\"";
    strncat(phase_1_voltage, main_phase_1_voltage, 256); 
    strncat(phase_1_voltage, s_char, 256); 
    strncat(phase_1_voltage, end, 256); 
    /// PHASE 2
    char phase_2[256] = "\"phase2\":{"; 
    char phase_2_power_c[256] = "\"power_consumption\":\""; 
    strncat(phase_2_power_c, main_phase_2_consumption, 256); 
    strncat(phase_2_power_c, end_char, 256); 
    char phase_2_voltage[256] = "\"voltage\":\"";
    strncat(phase_2_voltage, main_phase_2_voltage, 256); 
    strncat(phase_2_voltage, s_char, 256); 
    strncat(phase_2_voltage, end, 256); 
    /// PHASE 3
    char phase_3[256] = "\"phase3\":{"; 
    char phase_3_power_c[256] = "\"power_consumption\":\""; 
    strncat(phase_3_power_c, main_phase_3_consumption, 256); 
    strncat(phase_3_power_c, end_char, 256); 
    char phase_3_voltage[256] = "\"voltage\":\"";
    strncat(phase_3_voltage, main_phase_3_voltage, 256); 
    strncat(phase_3_voltage, s_char, 256); 
    strncat(phase_3_voltage, end_, 256); 
    /// PHASE 3
    //Backup line 
    char second_line[] = "\"backup_line\":{"; 
    /// PHASE 1
    char backup_phase_1[256] = "\"phase1\":{"; 
    char backup_phase_1_power_c[256] = "\"power_consumption\":\""; 
    strncat(backup_phase_1_power_c, second_phase_1_consumption, 256); 
    strncat(backup_phase_1_power_c, end_char, 256); 
    char backup_phase_1_voltage[256] = "\"voltage\":\"";
    strncat(backup_phase_1_voltage, second_phase_1_voltage, 256); 
    strncat(backup_phase_1_voltage, s_char, 256); 
    strncat(backup_phase_1_voltage, end, 256); 
    /// PHASE 2
    char backup_phase_2[256] = "\"phase2\":{"; 
    char backup_phase_2_power_c[256] = "\"power_consumption\":\""; 
    strncat(backup_phase_2_power_c, second_phase_2_consumption, 256); 
    strncat(backup_phase_2_power_c, end_char, 256); 
    char backup_phase_2_voltage[256] = "\"voltage\":\"";
    strncat(backup_phase_2_voltage, second_phase_2_voltage, 256); 
    strncat(backup_phase_2_voltage, s_char, 256); 
    strncat(backup_phase_2_voltage, end, 256); 
    /// PHASE 3
    char backup_phase_3[256] = "\"phase3\":{"; 
    char backup_phase_3_power_c[256] = "\"power_consumption\":\""; 
    strncat(backup_phase_3_power_c, second_phase_3_consumption, 256); 
    strncat(backup_phase_3_power_c, end_char, 256); 
    char backup_phase_3_voltage[256] = "\"voltage\":\"";
    strncat(backup_phase_3_voltage, second_phase_3_voltage, 256); 
    strncat(backup_phase_3_voltage, s_char, 256); 
    strncat(phase_3_voltage, end, 256); 
    /// PHASE 3
    /* 
    char phase_2[] = "\"main_line\""; 
    char phase_3[] = "\"main_line\""; 
    */
    // declare and initialize char 
    // append ch to str 
    strncat(json___, main_line, 256); 
    send_data("1\r\n");
    strncat(json___, phase_1, 256); 
    strncat(json___, phase_1_power_c, 256); 
    strncat(json___, phase_1_voltage, 256); 
    strncat(json___, phase_2, 256); 
    strncat(json___, phase_2_power_c, 256); 
    strncat(json___, phase_2_voltage, 256); 
    strncat(json___, phase_3, 256); 
    strncat(json___, phase_3_power_c, 256); 
    strncat(json___, phase_3_voltage, 256); 
    //strncat(json___, end, 256); 
    strncat(json___, second_line, 256); 
    strncat(json___, backup_phase_1, 256); 
    strncat(json___, backup_phase_1_power_c, 256); 
    strncat(json___, backup_phase_1_voltage, 256); 
    strncat(json___, backup_phase_2, 256); 
    strncat(json___, backup_phase_2_power_c, 256); 
    strncat(json___, backup_phase_2_voltage, 256); 
    strncat(json___, backup_phase_3, 256); 
    strncat(json___, backup_phase_3_power_c, 256); 
    strncat(json___, backup_phase_3_voltage, 256); 
    strncat(json___, end__, 256);
    
    send_data(json___);
    //strncat(json___, json, 256); 
    ///json___
}

void send_data_to_server()
{
    ///////////////////////////
    send_at("AT+HTTPTERM\r\n");
    delay_ms(2000);
    //////////////////////////

    send_at("AT+HTTPINIT\r\n");
    delay_ms(6000);
    send_at("AT+HTTPSSL=1\r\n");
    delay_ms(5000);
    //send_at("AT+HTTPPARA=\"CID\",\"1\"\r\n");
    //delay_ms(6000);
    send_at("AT+HTTPPARA=\"URL\",\"ehayq.am:444/db/save/node?enc=0&id=b19bca19-2af1-43d4-b3e8-ddf1a254c424\" \r\n");
    delay_ms(4000);
    send_at("AT+HTTPPARA=\"CONTENT\",\"application/json\"\r\n");
    delay_ms(2000);
    send_at("AT+HTTPDATA=2000,2000\r\n");
    delay_ms(200);
    generate_json();
    send_data(json___);
    send_data("\r\n");
    //send_at("aHELLasdfasfdassasdfO1\r\n");
    delay_ms(4000);
    send_http("AT+HTTPACTION=1\r\n");
    delay_ms(5000);
    send_at("AT+HTTPREAD\r\n");
    delay_ms(5000);
    send_at("AT+HTTPTERM\r\n");
    delay_ms(2000);
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void send_data(const char str[256])
{
    for(unsigned int cc=0; cc<=strlen(str);cc++){
        USART_SendData(USART1, str[cc]);
        /* Loop until the end of transmission */
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
        {
        }
    }
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void send_num(int num) 
{
    char snum[5];
    // convert 123 to string [buf]
    itoa(num, snum, 10);
    send_data(snum);
}
