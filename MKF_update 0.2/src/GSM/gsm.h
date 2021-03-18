#ifndef GSM_h 
#define GSM_h

int i,j;
char name[11];
void init_settings();
void gsm_init(void);
void NVIC_Configuration(void);
void GPIO_Configuration(void);
void USART_Configuration(void);
void USART1_IRQHandler(void);
void UARTSend(const unsigned char *pucBuffer, unsigned long ulCount);
void send_data(const char str[256]);
void send_num(int num);

void send_data_to_server();

void generate_json();

//char json[] = "{";

//const char main_phase_1_voltage[256];
char main_phase_1_voltage[256];
char main_phase_2_voltage[256];
char main_phase_3_voltage[256];

char second_phase_1_voltage[256];
char second_phase_2_voltage[256];
char second_phase_3_voltage[256];

char main_phase_1_consumption[256];
char main_phase_2_consumption[256];
char main_phase_3_consumption[256];

char second_phase_1_consumption[256];
char second_phase_2_consumption[256];
char second_phase_3_consumption[256];

void main_line_voltage(const char main_p_1[256], const char main_p_2[256], const char main_p_3[256]);
void second_line_voltage(const char second_p_1[256], const char second_p_2[256], const char second_p_3[256]);
void main_line_consumption(const char main_p_1[256], const char main_p_2[256], const char main_p_3[256]);
void second_line_consumption(const char second_p_1[256], const char second_p_2[256], const char second_p_3[256]);


#endif
