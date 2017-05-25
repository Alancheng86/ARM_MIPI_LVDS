#ifndef	__TSLIIC_REV01_h__
#define	__TSLIIC_REV01_h__

#include "stm32f10x.h"

#define lvtimes_F 100
 //============================
#define PIN_SCL  GPIO_Pin_2
#define PIN_SDA  GPIO_Pin_0
//#define GPIO_Sensor                               GPIOC		//////SDA
#define GPIO_Sensor_SDA                               GPIOC
#define GPIO_Sensor_SCL                               GPIOC
#define RCC_GPIO_TSLIIC_SDA 					  RCC_APB2Periph_GPIOC
#define RCC_GPIO_TSLIIC_SCL						  RCC_APB2Periph_GPIOC
//------------------------------------------------------------
#define TzSCL_L GPIO_ResetBits(GPIO_Sensor_SCL, PIN_SCL)  //SCL
#define TzSCL_H GPIO_SetBits(GPIO_Sensor_SCL, PIN_SCL)
#define TzSDA_L GPIO_ResetBits(GPIO_Sensor_SDA, PIN_SDA)  //SDA
#define TzSDA_H GPIO_SetBits(GPIO_Sensor_SDA, PIN_SDA)

#define S1_addr 0X92
#define S2_addr 0X72
#define S3_addr 0X52 

void TSL_GPIO_Config(void);
void delay_10ns (u32 k);
unsigned char TSL_WriteReg(unsigned char Device_N,unsigned char addr,unsigned char regdata);
unsigned char  Sensor_Check(unsigned char Device_N);
void TSL_Init(unsigned char Device_N);
unsigned int Get_Bright(unsigned char Device_N);
void Reset_Senser(void);
void Measure_Brightness_Init(void);

//--------------------------------------------------------------
#endif
