/* ADE9078
Note: Please refer to the Analog Devices ADE9078 datasheet - much of this library was based directly on the statements and methods provided in it!  Their authors get paid, trust them over us!
*/

#include "ADE9078.h"
#define byte uint8_t
//This value tells the ADE9078 that data is to be read from the requested register.
const unsigned int WRITE = 0b00000000; //This value tells the ADE9078 that data is to be written to the requested register.
const unsigned int READ = 0b10000000;  //This value tells the ADE9078 that data is to be read from the requested register.
const long SPI_freq = 1000000;//Communicate with the ADE9078 at 1 MHz frequency.
#define PERIOD_16 0x2
// const int SPI_freq = 12,288,000 // P 6: Input flock frequency
//available from within or outside the class

//Define Constants:
#define dummyWrite 0x00 //Used for SPI Transfer to read out a value const unsigned int READ = 0b10000000;

//*******************************************************
//****************ADE9078 Library Control Functions*****
//****************Object Definition**********************
//*******************************************************
//
struct LastReads lastReads;
struct LastReads
{
    struct InstVoltage instVoltage;
    struct A_Vrms A_vrms;
    struct B_Vrms B_vrms;
    struct InstCurrent instCurrent;
    struct Irms irms;
    struct FilterBasedCurrent filterBasedCurrent;
    struct WattHours wattHours;
    struct InstApparentPower instApparentPower;
    struct InstActivePower instActivePower;
    struct InstReactivePower instReactivePower;
    struct FundamentalReactivePower fundReactPower;
    struct PowerFactor powerFactor;
    struct Period period;
    struct Peak peak;
    struct ResampledWFB resampledData;
};

struct InitializationSettings 
{
  // All gains are 2 bits. Options: 1, 2, 3, 4
  uint8_t vAGain;
  uint8_t vBGain;
  uint8_t vCGain;

  uint8_t iAGain;
  uint8_t iBGain;
  uint8_t iCGain;
  uint8_t iNGain;

  uint32_t powerAGain;
  uint32_t powerBGain;
  uint32_t powerCGain;

  uint8_t vConsel;
  uint8_t iConsel;
}init;

/**
 * @brief  
 * @param  none
 * @retval none
 */
void setup_chip2(void)
{
    //Software reset ADE9078
    spi_write_16(0x481, 0x00000001, GPIOD, GPIO_Pin_2);
    delay(600000);
    init_ADE9078(GPIOD, GPIO_Pin_2);
    //RSTDONE interrrupt disable(IRQ1)
    spi_write_32(STATUS1_32, 0x10000, GPIOD, GPIO_Pin_2);
    delay(1000000);
    //High pass filter enable/disable
    //spi_write_32(CONFIG0_32, 0x40, GPIOD, GPIO_Pin_2);
    //Setup RUN and EP_CFG regitsters
    spi_write_16(0x480, 0xFFFF, GPIOD, GPIO_Pin_2);
    spi_write_16(0x4B0, 0xFFFF, GPIOD, GPIO_Pin_2);
    delay(20000);
    ///////////////////////////////////////////////////
    spi_write_32(0x403, 0x00000000, GPIOD, GPIO_Pin_2);
    //Setup zero-crossing time in ZXTOUT register
    //timeout 0.02s
    spi_write_16(ZXTOUT_16, 0x30, GPIOD, GPIO_Pin_2);
    delay(20000);
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
void setup_chip1(void)
{
    //Software reset ADE9078
    spi_write_16(0x481, 0x00000001, GPIOB, GPIO_Pin_7);
    delay(600000);
    init_ADE9078(GPIOB, GPIO_Pin_7);
    //RSTDONE interrrupt disable(IRQ1)
    spi_write_32(STATUS1_32, 0x10000, GPIOB, GPIO_Pin_7);
    delay(1000000);
    //High pass filter enable/disable
    //spi_write_32(CONFIG0_32, 0x40, GPIOB, GPIO_Pin_7);
    //Setup RUN and EP_CFG regitsters
    spi_write_16(0x480, 0xFFFF, GPIOB, GPIO_Pin_7);
    spi_write_16(0x4B0, 0xFFFF, GPIOB, GPIO_Pin_7);
    delay(20000); ///////////////////////////////////////////////////
    spi_write_32(0x403, 0x00000000, GPIOB, GPIO_Pin_7);
    //Setup zero-crossing time in ZXTOUT register
    //timeout 0.02s
    spi_write_16(ZXTOUT_16, 0x30, GPIOB, GPIO_Pin_7);
    delay(20000);
}
/**
* @brief  
* @param  None
* @retval None
*/
int init_ADE9078(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    spi_write_32(APGAIN_32, init.powerAGain, GPIOx, gpio_pin);
    spi_write_32(BPGAIN_32, init.powerBGain, GPIOx, gpio_pin);
    spi_write_32(CPGAIN_32, init.powerCGain, GPIOx, gpio_pin);
    uint16_t pgaGain = (init.vCGain << 12) + (init.vBGain << 10) + (init.vCGain << 8) + (init.iNGain << 6) + (init.iCGain << 4) + (init.iBGain << 2) + init.iAGain;
    spi_write_16(PGA_GAIN_16, pgaGain, GPIOx, gpio_pin);
    uint32_t vLevelData = 0x117514;
	// #5 : Write VLevel 0x117514
    spi_write_32(VLEVEL_32, vLevelData, GPIOx, gpio_pin);
    spi_write_16(CONFIG0_32, 0x800, GPIOx, gpio_pin);
    spi_write_32(DICOEFF_32, 0xFFFFE000, GPIOx, gpio_pin);
    spi_write_32(DICOEFF_32, 0xFFFFE000, GPIOx, gpio_pin);
	// #7:  If current transformers are used, INTEN and ININTEN in the CONFIG0 register must = 0
    // Table 24 to determine how to configure ICONSEL and VCONSEL in the ACCMODE register
    uint16_t settingsACCMODE = (init.iConsel << 6) + (init.vConsel << 5);
    spi_write_16(ACCMODE_16, settingsACCMODE, GPIOx, gpio_pin); 
    spi_write_16(RUN_16, 1, GPIOx, gpio_pin);  // 8: Write 1 to Run register
    spi_write_16(EP_CFG_16, 1, GPIOx, gpio_pin);  // 9: Write 1 to EP_CFG register
    /*
    Potentially useful registers to configure:
    The following were in the 9078:
      0x49A ZX_LP_SEL : to configure "zero crossing signal"
      0x41F PHNOLOAD : To say if something is "no load".
      Phase calibrations, such as APHCAL1_32
    */
    spi_write_16(CONFIG1_16, 0x0000, GPIOx, gpio_pin);
    spi_write_16(CONFIG2_16, 0x0000, GPIOx, gpio_pin);
    spi_write_16(CONFIG3_16, 0x0000, GPIOx, gpio_pin);
    spi_write_32(DICOEFF_32, 0xFFFFE000, GPIOx, gpio_pin); // Recommended by datasheet
    spi_write_16(EGY_TIME_16, 0x0001, GPIOx, gpio_pin); 
    spi_write_16(EP_CFG_16, 0x0021, GPIOx, gpio_pin); //) RD_EST_EN=1, EGY_LD_ACCUM=0, EGY_TMR_MODE=0, EGY_PWR_EN=1
    return 0;
}

//**************************************************

/**
* @brief  
* @param  None
* @retval None
*/
void configure_wfb(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    int i;
    uint16_t writeValue = spi_read_16(WFB_CFG_16, GPIOx, gpio_pin);
    //stop filling buffer to config buffer
    writeValue &= ~(0b1 << 4);  //set WF_CAP_EN bit to 0 in the WFB_CFG register
    spi_write_16(WFB_CFG_16, writeValue, GPIOx, gpio_pin);
    //enable INeutral: uncomment the following
    writeValue = (writeValue | (0b1 << 12));
    //disable INeutral: uncomment the following
    //writeValue = (writeValue & ~(-0b1<<12));
    //sinc4output
    writeValue = (writeValue & ~(0b1 << 8));//WF_SRC bit to 00
    writeValue = (writeValue & ~(0b1 << 9));
    //mode 0//Stop when buffer is full mode
    writeValue &= ~(0b1<<6); //WF_MODE bits = 00
    writeValue &= ~(0b1<<7);
    //read resampled data
    writeValue &= ~(0b1<<5); //WF_CAP_SEL = 0
    //burstAllChannels
    for (i = 0; i == 3; i++){
        writeValue = writeValue & ~(0b1<<i);
    }
	spi_write_16(WFB_CFG_16, writeValue, GPIOx, gpio_pin);
}

/**
* @this resets the bit in STATUS0_32 called COH_WFB_FULL so we can\
*	 once again be notified if the thing is full
* @param  None
* @retval None
*/
void reset_full_buffer_bit(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t addressContent = spi_read_32(STATUS0_32, GPIOx, gpio_pin);
    addressContent = (addressContent & ~(0b1 << 23));
    spi_write_32(STATUS0_32, addressContent, GPIOx, gpio_pin);
}

//Start the WFB
/**
* @brief  
* @param  None
* @retval None
*/
//TODO added write function argument 
void start_filling_buffer(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
	uint16_t addressContent = spi_read_16(WFB_CFG_16, GPIOx, gpio_pin);
    //set WF_CAP_EN bit to 1 in the WFB_CFG register to start
    //filling the buffer from Address 0x800.
	addressContent = (addressContent | (0b1 << 4));  
	spi_write_16(WFB_CFG_16, addressContent, GPIOx, gpio_pin);
}

/**
* @Stop the WFB
* @param  None
* @retval None
*/
//TODO added write function argument 
void stop_filling_buffer(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
	uint16_t addressContent = spi_read_16(WFB_CFG_16, GPIOx, gpio_pin);
	addressContent = (addressContent & ~(0b1 << 4));
	//set WF_CAP_EN bit to 0 in the WFB_CFG register
	spi_write_16(WFB_CFG_16, addressContent, GPIOx, gpio_pin);
}

/**
* @Stop the WFB
* @param  None
* @retval None
*/
//void burst_av_only(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
//{
//    uint16_t writeValue = spi_read_16(WFB_CFG_16, GPIOx, gpio_pin);
//    //burst Av only
//    writeValue = writeValue |= (0b1<<3);
//    writeValue = writeValue & ~(0b1<<2);
//    writeValue = writeValue & ~(0b1<<1);
//    writeValue = writeValue |= (0b1<<0);
//    spi_write_16(WFB_CFG_16, writeValue, GPIOx, gpio_pin);
//}
//
//TODO added write function argument 
int is_done_sampling(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
   int check = 0;
   uint32_t status = spi_read_32(STATUS0_32, GPIOx, gpio_pin);
   // 23th bit tells you that the buffer is full
   status = (status >> 23);
   check = (status & 0b1);
   return check;
}

//TODO
///* Burst read, resampled waveform */
//void spi_burst_resampled_wfb(uint16_t startingAddress, GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
//{
//    uint16_t commandHeader = ((startingAddress << 4)& 0xFFF0) + 8;
//    spiTransferWord(spy, commandHeader);
//    for(int i=0; i < WFB_RESAMPLE_SEGMENTS; i++)
//    {
//        lastReads.resampledData.Va[i] = spiTransferWord(spy, WRITE);
//        lastReads.resampledData.Ia[i] = spiTransferWord(spy, WRITE);
//        lastReads.resampledData.Vb[i] = spiTransferWord(spy, WRITE);
//        lastReads.resampledData.Ib[i] = spiTransferWord(spy, WRITE);
//        lastReads.resampledData.Vc[i] = spiTransferWord(spy, WRITE);
//        lastReads.resampledData.Ic[i] = spiTransferWord(spy, WRITE);
//        //blank cells are skipped internally
//        lastReads.resampledData.In[i] = spiTransferWord(spy, WRITE);
//    }
//}
//


//*****************************************************************************
//********************User Program Availavle Function**************************
//*****************************************************************************

/**
* @brief  
* @param  None
* @retval None
*/
uint8_t get_version(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    return spi_read_16(VERSION_16, GPIOx, gpio_pin);
}

/**
* @brief  
* @param  None
* @retval None
*/
//TODO
//double get_power_factor_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
//{
//    int16_t value=0;
//    value=spi_read_16(PFA_16, GPIOx, gpio_pin);
//    double decimal = decimalize(value, 1.0, 0.0,0);
//    return (decimal);
//}

/**
* @brief  
* @param  None
* @retval None
*/
uint32_t get_phase_calib_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(APHCAL0_32, GPIOx, gpio_pin);
    return value;
}

// TODO : USER_PERIOD_32
//// No period register?
//double get_period(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
//{
//  uint16_t value=0;
//  value=spi_read_16((functionBitVal(Period_16,1)),(functionBitVal(Period_16,0)), GPIOx, gpio_pin);
//    //convert to double with calibration factors specified
//  double decimal = decimalize(value, 1, 0,0);  return decimal;
//}

/**
* @brief  
* @param  None
* @retval None
*/
uint32_t get_phnoload(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
	//use signed int32_t for signed registers, and unsigned uint32_t for unsigned registers
    uint32_t value=0;
	//use signed uint32_t for signed registers, and unsigned uint32_t for unsigned registers
    value=spi_read_32(PHNOLOAD_32, GPIOx, gpio_pin);
	//Call MSB and LSB from the register constant (template for how all functions should be called)
    return value;
}

/**
* @brief  
* @param  None
* @retval None
*/
uint32_t get_inst_voltage_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(AV_PCF_32, GPIOx, gpio_pin);
    spi_read_32(PHNOLOAD_32, GPIOx, gpio_pin);
    return value;
}

/**
* @brief  
* @param  None
* @retval None
*/
//TODO added write function argument 
uint32_t get_inst_voltage_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(BV_PCF_32, GPIOx, gpio_pin);
    return value;
}

/**
* @brief  
* @param  None
* @retval None
*/
uint32_t get_inst_voltage_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(CV_PCF_32, GPIOx, gpio_pin);
    return value;
}

/**
* @brief  
* @param  None
* @retval None
*/
void read_voltage(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.instVoltage.a = spi_read_32(AV_PCF_32, GPIOx, gpio_pin);
    lastReads.instVoltage.b = spi_read_32(BV_PCF_32, GPIOx, gpio_pin);
    lastReads.instVoltage.c = spi_read_32(CV_PCF_32, GPIOx, gpio_pin);
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_A_avrms()
{
    uint32_t value=0;
    value=spi_read_32(AVRMS_32, GPIOB, GPIO_Pin_7);
    //convert to double with calibration factors specified, no abs value
    double decimal = decimalize(value, A_AVrmsGain, A_AVrmsOffset,0); 
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_A_bvrms()
{
    uint32_t value=0;
    value=spi_read_32(BVRMS_32, GPIOB, GPIO_Pin_7);
    //convert to double with calibration factors specified, no abs value
    double decimal = decimalize(value, A_BVrmsGain, A_BVrmsOffset,0);
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_A_cvrms()
{
    uint32_t value=0;
    value=spi_read_32(CVRMS_32, GPIOB, GPIO_Pin_7);
    //convert to double with calibration factors specified,  no abs value
    double decimal = decimalize(value, A_CVrmsGain, A_CVrmsOffset,0); 
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_B_avrms()
{
    uint32_t value=0;
    value=spi_read_32(AVRMS_32, GPIOD, GPIO_Pin_2);
    //convert to double with calibration factors specified, no abs value
    double decimal = decimalize(value, B_AVrmsGain, B_AVrmsOffset,0); 
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_B_bvrms()
{
    uint32_t value=0;
    value=spi_read_32(BVRMS_32, GPIOD, GPIO_Pin_2);
    //convert to double with calibration factors specified, no abs value
    double decimal = decimalize(value, B_BVrmsGain, B_BVrmsOffset,0);
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_B_cvrms()
{
    uint32_t value=0;
    value=spi_read_32(CVRMS_32, GPIOD, GPIO_Pin_2);
    //convert to double with calibration factors specified,  no abs value
    double decimal = decimalize(value, B_CVrmsGain, B_CVrmsOffset,0); 
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
void read_A_vrms()
{
    lastReads.A_vrms.a = (int32_t)(decimalize(spi_read_32(AVRMS_32, GPIOB, GPIO_Pin_7), A_AVrmsGain, A_AVrmsOffset, 0));
    lastReads.A_vrms.b = (int32_t)(decimalize(spi_read_32(BVRMS_32, GPIOB, GPIO_Pin_7), A_BVrmsGain, A_BVrmsOffset, 0));
    lastReads.A_vrms.c = (int32_t)(decimalize(spi_read_32(CVRMS_32, GPIOB, GPIO_Pin_7), A_CVrmsGain, A_CVrmsOffset, 0));
}

/**
* @brief  
* @param  None
* @retval None
*/
void read_B_vrms()
{
    lastReads.B_vrms.a = (int32_t)(decimalize(spi_read_32(AVRMS_32, GPIOB, GPIO_Pin_2), B_AVrmsGain, B_AVrmsOffset, 0));
    lastReads.B_vrms.b = (int32_t)(decimalize(spi_read_32(BVRMS_32, GPIOB, GPIO_Pin_2), B_BVrmsGain, B_BVrmsOffset, 0));
    lastReads.B_vrms.c = (int32_t)(decimalize(spi_read_32(CVRMS_32, GPIOB, GPIO_Pin_2), B_CVrmsGain, B_CVrmsOffset, 0));
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_A_airms()
{
    uint32_t value=0;
    value=spi_read_32(AIRMS_32, GPIOB, GPIO_Pin_7);
    //convert to double with calibration factors specified
    double decimal = decimalize(value, A_AIrmsGain, A_AIrmsOffset,0);
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_A_birms()
{
    uint32_t value=0;
    value=spi_read_32(BIRMS_32, GPIOB, GPIO_Pin_7);
    //convert to double with calibration factors specified
    double decimal = decimalize(value, A_BIrmsGain, A_BIrmsOffset,0); 
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_A_cirms()
{
    uint32_t value=0;
    value=spi_read_32(CIRMS_32, GPIOB, GPIO_Pin_7);
    //convert to double with calibration factors specified
    double decimal = decimalize(value, A_CIrmsGain, A_CIrmsOffset, 0); 
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
void read_A_irms()
{
    lastReads.irms.a = (int32_t)(decimalize(spi_read_32(AIRMS_32, GPIOB, GPIO_Pin_7), A_AIrmsGain, A_AIrmsOffset, 0));
    lastReads.irms.b = (int32_t)(decimalize(spi_read_32(BIRMS_32, GPIOB, GPIO_Pin_7), A_BIrmsGain, A_BIrmsOffset, 0));
    lastReads.irms.c = (int32_t)(decimalize(spi_read_32(CIRMS_32, GPIOB, GPIO_Pin_7), A_CIrmsGain, A_CIrmsOffset, 0));
}
/**
* @brief  
* @param  None
* @retval None
*/
double get_B_airms()
{
    uint32_t value=0;
    value=spi_read_32(AIRMS_32, GPIOD, GPIO_Pin_2);
    //convert to double with calibration factors specified
    double decimal = decimalize(value, B_AIrmsGain, B_AIrmsOffset,0);
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_B_birms()
{
    uint32_t value=0;
    value=spi_read_32(BIRMS_32, GPIOD, GPIO_Pin_2);
    //convert to double with calibration factors specified
    double decimal = decimalize(value, B_BIrmsGain, B_BIrmsOffset,0); 
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
double get_B_cirms()
{
    uint32_t value=0;
    value=spi_read_32(CIRMS_32, GPIOD, GPIO_Pin_2);
    //convert to double with calibration factors specified
    double decimal = decimalize(value, B_CIrmsGain, B_CIrmsOffset, 0); 
    return decimal;
}

/**
* @brief  
* @param  None
* @retval None
*/
void read_B_irms()
{
    lastReads.irms.a = (int32_t)(decimalize(spi_read_32(AIRMS_32, GPIOD, GPIO_Pin_2), B_AIrmsGain, B_AIrmsOffset, 0));
    lastReads.irms.b = (int32_t)(decimalize(spi_read_32(BIRMS_32, GPIOD, GPIO_Pin_2), B_BIrmsGain, B_BIrmsOffset, 0));
    lastReads.irms.c = (int32_t)(decimalize(spi_read_32(CIRMS_32, GPIOD, GPIO_Pin_2), B_CIrmsGain, B_CIrmsOffset, 0));
}

/**
* @brief  
* @param  None
* @retval None
*/
uint32_t get_inst_current_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(AI_PCF_32, GPIOx, gpio_pin);
    return value;
}

/**
* @brief  
* @param  None
* @retval None
*/
uint32_t get_inst_current_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(BI_PCF_32, GPIOx, gpio_pin);
    return value;
}

/**
* @brief  
* @param  None
* @retval None
*/
uint32_t get_inst_current_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(CI_PCF_32, GPIOx, gpio_pin);
    return value;
}

/**
* @brief  
* @param  None
* @retval None
*/
void read_inst_current(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.instCurrent.a = spi_read_32(AI_PCF_32, GPIOx, gpio_pin);
    lastReads.instCurrent.b = spi_read_32(BI_PCF_32, GPIOx, gpio_pin);
    lastReads.instCurrent.c = spi_read_32(CI_PCF_32, GPIOx, gpio_pin);
}

/**
* @brief  
* @param  None
* @retval None
*/
double read_watt_hours_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t data = spi_read_32(AWATTHR_HI_32, GPIOx, gpio_pin);
    double decimal = decimalize(data, AWattHrGain, AWattHrOffset,0);
    return (decimal);
}

/**
* @brief  
* @param  None
* @retval None
*/
double read_watt_hours_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t data = spi_read_32(BWATTHR_HI_32, GPIOx, gpio_pin);
    double decimal = decimalize(data, AWattHrGain, AWattHrOffset,0);
    return (decimal);
}

/**
* @brief  
* @param  None
* @retval None
*/
double read_watt_hours_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t data = spi_read_32(CWATTHR_HI_32, GPIOx, gpio_pin);
    double decimal = decimalize(data, AWattHrGain, AWattHrOffset,0);
    return (decimal);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void read_watt_hours(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.wattHours.a = decimalize(spi_read_32(AWATTHR_HI_32, GPIOx, gpio_pin), AWattHrGain, AWattHrOffset, 0);
    lastReads.wattHours.b = decimalize(spi_read_32(BWATTHR_HI_32, GPIOx, gpio_pin), BWattHrGain, BWattHrOffset, 0);
    lastReads.wattHours.c = decimalize(spi_read_32(CWATTHR_HI_32, GPIOx, gpio_pin), CWattHrGain, CWattHrOffset, 0);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void  read_fundamental_reactive_power(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.fundReactPower.a = (int32_t) (spi_read_32(AFVAR_32, GPIOx, gpio_pin));
    lastReads.fundReactPower.b = (int32_t) (spi_read_32(AFVAR_32, GPIOx, gpio_pin));
    lastReads.fundReactPower.c = (int32_t) (spi_read_32(AFVAR_32, GPIOx, gpio_pin));
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void read_filter_based_current(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.filterBasedCurrent.a = (int32_t) (spi_read_32(AIRMS_32, GPIOx, gpio_pin));
    lastReads.filterBasedCurrent.b = (int32_t) (spi_read_32(BIRMS_32, GPIOx, gpio_pin));
    lastReads.filterBasedCurrent.c = (int32_t) (spi_read_32(CIRMS_32, GPIOx, gpio_pin));
    lastReads.filterBasedCurrent.n = (int32_t) (spi_read_32(NIRMS_32, GPIOx, gpio_pin));
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void read_power_factor(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    // not sure what 9000 code is doing with the values
    lastReads.powerFactor.regA = (int32_t) (spi_read_32(APF_32, GPIOx, gpio_pin));
    // lastReads.powerFactor.valA(int32_t)
    lastReads.powerFactor.regB = (int32_t) (spi_read_32(BPF_32, GPIOx, gpio_pin));
    // lastReads.powerFactor.valB
    lastReads.powerFactor.regC = (int32_t) (spi_read_32(CPF_32, GPIOx, gpio_pin));
    // lastReads.powerFactor.valC =
}

/**
 * @brief  
 * @param  None
 * @retval None
*/
void read_period(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.period.a = (int32_t) (spi_read_32(APERIOD_32, GPIOx, gpio_pin));
    lastReads.period.b = (int32_t) (spi_read_32(APERIOD_32, GPIOx, gpio_pin));
    lastReads.period.c = (int32_t) (spi_read_32(APERIOD_32, GPIOx, gpio_pin));
    // 9000 code also calculates some values here?
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_apparent_power_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(AVA_32, GPIOx, gpio_pin);
    double decimal = decimalize(value, AAppPowerGain, AAppPowerOffset,0);
	//convert to double with calibration factors specified
    return (decimal);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_apparent_power_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(BVA_32, GPIOx, gpio_pin);
    double decimal = decimalize(value, BAppPowerGain, BAppPowerOffset,0);
	//convert to double with calibration factors specified
    return (decimal);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_apparent_power_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{  	//type conversion approach used for the ADE9000
    int32_t value = (int32_t)spi_read_32(CVA_32, GPIOx, gpio_pin);
    double decimal = decimalizeSigned(value, CAppPowerGain, CAppPowerOffset,0); 
	//convert to double with calibration factors specified
    return (decimal);
}

/**
 * @brief  
 * @param  None
 * @retval None
*/
void get_inst_apparent_power(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.instApparentPower.a = (int32_t) (decimalizeSigned(spi_read_32(AVA_32, GPIOx, gpio_pin), AAppPowerGain, AAppPowerOffset, 0));
    lastReads.instApparentPower.b = (int32_t) (decimalizeSigned(spi_read_32(BVA_32, GPIOx, gpio_pin), BAppPowerGain, BAppPowerOffset, 0));
    lastReads.instApparentPower.c = (int32_t) (decimalizeSigned(spi_read_32(CVA_32, GPIOx, gpio_pin), CAppPowerGain, CAppPowerOffset, 0));
}


/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_active_power_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{ 
	//type conversion approach used for the ADE9000
    int32_t value = (int32_t)spi_read_32(AWATT_32, GPIOx, gpio_pin);
    double decimal = decimalizeSigned(value, AInstPowerGain, AInstPowerOffset,0); //convert to double with calibration factors specified
    return (decimal);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_active_power_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{ 
	//type conversion approach used for the ADE9000
    int32_t value = (int32_t)spi_read_32(BWATT_32, GPIOx, gpio_pin);
    double decimal = decimalizeSigned(value, BInstPowerGain, BInstPowerOffset,0); //convert to double with calibration factors specified
    return (decimal);
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_active_power_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
	//type conversion approach used for the ADE9000
    int32_t value = (int32_t)spi_read_32(CWATT_32, GPIOx, gpio_pin);
    double decimal = decimalizeSigned(value, CInstPowerGain, CInstPowerOffset,0); //convert to double with calibration factors specified
    return (decimal);
}

/**
 * @brief  
 * @param  None
 * @retval None
*/
void read_inst_active_power(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.instActivePower.a = (int32_t)(decimalizeSigned(spi_read_32(AWATT_32, GPIOx, gpio_pin), AInstPowerGain, AInstPowerOffset, 0));
    lastReads.instActivePower.b = (int32_t)(decimalizeSigned(spi_read_32(BWATT_32, GPIOx, gpio_pin), BInstPowerGain, BInstPowerOffset, 0));
    lastReads.instActivePower.c = (int32_t)(decimalizeSigned(spi_read_32(CWATT_32, GPIOx, gpio_pin), CInstPowerGain, CInstPowerOffset, 0));
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_reactive_power_a(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
	//type conversion approach used for the ADE9000
    int32_t value = (int32_t)spi_read_32(AVAR_32, GPIOx, gpio_pin);
    double decimal = decimalizeSigned(value, AInstReactivePowerGain, AInstReactivePowerOffset,0); //convert to double with calibration factors specified
    return decimal;
  }

/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_reactive_power_b(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{ 
	//type conversion approach used for the ADE9000
    int32_t value = (int32_t)spi_read_32(BVAR_32, GPIOx, gpio_pin);
    double decimal = decimalizeSigned(value, BInstReactivePowerGain, BInstReactivePowerOffset,0); //convert to double with calibration factors specified
    return decimal;
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
double get_inst_reactive_power_c(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
 	//type conversion approach used for the ADE9000
    int32_t value = (int32_t)spi_read_32(CVAR_32, GPIOx, gpio_pin);
    double decimal = decimalizeSigned(value, CInstReactivePowerGain, CInstReactivePowerOffset,0); //convert to double with calibration factors specified
    return decimal;
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void read_inst_reactive_power(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.instReactivePower.a = (int32_t)(decimalizeSigned(spi_read_32(AVAR_32, GPIOx, gpio_pin), AInstReactivePowerGain, AInstReactivePowerOffset, 0));
    lastReads.instReactivePower.b = (int32_t)(decimalizeSigned(spi_read_32(BVAR_32, GPIOx, gpio_pin), BInstReactivePowerGain, BInstReactivePowerOffset, 0));
    lastReads.instReactivePower.c = (int32_t)(decimalizeSigned(spi_read_32(CVAR_32, GPIOx, gpio_pin), CInstReactivePowerGain, CInstReactivePowerOffset, 0));
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
uint32_t get_vpeak(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(VPEAK_32, GPIOx, gpio_pin);
    return value;
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
uint32_t get_ipeak(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t value=0;
    value=spi_read_32(IPEAK_32, GPIOx, gpio_pin);
    return value;
}

/**
 * @brief  
 * @param  None
 * @retval None
 */
void read_peak(GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    lastReads.peak.Vpeak = spi_read_32(VPEAK_32, GPIOx, gpio_pin);
    lastReads.peak.Ipeak = spi_read_32(IPEAK_32, GPIOx, gpio_pin);
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
double read_32bit_and_scale(uint16_t readRegister, GPIO_TypeDef* GPIOx, uint16_t gpio_pin)
{
    uint32_t data = spi_read_32(readRegister, GPIOx, gpio_pin);
    double decimal = decimalize(data, 1.0, 0.0,0);
    return (decimal);
}

//**************** Helper Functions *****************

/** * @brief  
 * @param  none
 * @retval none
 */
bool checkBit(int data, int i) // return true if i'th bit is set, false otherwise
{
     //datasheet counts bits starting at 0. so the 2nd bit would be the 1 in 0010;
    /* example: assume input.. data=5, i=2
    data = 5 ----> 00101
    00101 & 00100 ---> 00100 --> 4
    4 > 0, return true, the bit is set.
    */
    return ((data) & (1 << i)) > 0;
}

/**
 * @This function converts to floating point with an optional linear calibration (y=mx+b)\
 * by providing input in the following way as arguments (rawinput, gain, offset)
 * @param  uint32_t input, double factor, double offset, bool absolutevalue
 * @retval None
 */
double decimalize(uint32_t input, double factor, double offset, bool absolutevalue)
{
    if(absolutevalue == 0){
    return (((double)input*factor)+offset); //standard y=mx+b calibration function applied to return
    }
    else{
        return ((double)input*factor)+offset;
    }
}

/**
* @This function converts to floating point with an optional linear calibration\
 *   (y=mx+b) by providing input in the following way as arguments (rawinput, gain, offset)
 * @param  none
 * @retval none
 */
double decimalizeSigned(int32_t input, double factor, double offset, bool absolutevalue)
{
    if(absolutevalue == 0) {
        return (((double)input*factor)+offset); //standard y=mx+b calibration function applied to return
    }
    else{
        return ((double)input*factor)+offset; 
    }
}

/**
 * @brief  
 * @param  none
 * @retval none
 */
uint16_t crc16(char* pData, int length)
{
    uint8_t i;
    uint16_t wCrc = 0xffff;
    while (length--) {
        wCrc ^= *(unsigned char *)pData++ << 8;
        for (i=0; i < 8; i++)
            wCrc = wCrc & 0x8000 ? (wCrc << 1) ^ 0x1021 : wCrc << 1;
    }
    return wCrc & 0xffff;
}

/**
 * @Returns as integer an address of a specified byte - basically a byte\
 *  controlled shift register with "byteVal" controlling the byte that is read and returned
 * @param  None
 * @retval None
*/
uint8_t functionBitVal(uint16_t addr, uint8_t byteVal)
{
  uint16_t x = ((addr >> (8*byteVal)) & 0xff);
  return x;
    //convert to double with calibration factors specified
}
