#ifndef _IIC_CURRENT_H_
#define _IIC_CURRENT_H_

#include "stm32f10x.h"
//#include "config_hard.h"    /////////with something wrong  if you use it,,else  everything ok....so strange...





#define LOW 0
#define HIGH 1
#define ACK LOW
#define NAK HIGH

#define mA 1
#define uA 0

#define DEVICE_ADDRESS_M6	0x82
#define DEVICE_ADDRESS_M3	0x80


#define COMMAND_CONFIG_INDEX	0x00
#define COMMAND_SHUNT_VLOTAGE_SETTING	0x01
#define COMMAND_BUS_VOLTAGE_INDEX	0x02
#define COMMAND_POWER_INDEX	0x03
#define COMMAND_CURRENT_INDEX	0x04		  /////current	   REG
#define COMMAND_CALIBRATION_INDEX	0x05
#define COMMAND_MASK_INDEX	0x06
#define COMMAND_ALERT_LIMIT_INDEX	0x07
#define COMMAND_ID_INDEX	0xFF




#define T226PIN_SCL  GPIO_Pin_5
#define T226PIN_SDA  GPIO_Pin_4						//­×
#define GPIO_TP                                   GPIOE
#define RCC_APB2Periph_GPIO_TPIIC  RCC_APB2Periph_GPIOE
//------------------------------------------------------------
#define T226SCL_L GPIO_ResetBits(GPIO_TP, T226PIN_SCL)  //SCL
#define T226SCL_H GPIO_SetBits  (GPIO_TP, T226PIN_SCL)
#define T226SDA_L GPIO_ResetBits(GPIO_TP, T226PIN_SDA)  //SDA
#define T226SDA_H GPIO_SetBits  (GPIO_TP, T226PIN_SDA)



void T226_intion(void);
void T226_GPIO_Config(void);
bool T226_I2CSend(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength);
bool T226_I2CReceive(BYTE ucDeviceAddr, BYTE ucIndex, BYTE* pucData, unsigned int unDataLength);

u16 Measure_IDD(u8 gears);
u16 Measure_IOIDD(u8 gears);
u16 Measure_V5V(u8 gears) ;

#endif /* __IIC_CURRENT_H */


