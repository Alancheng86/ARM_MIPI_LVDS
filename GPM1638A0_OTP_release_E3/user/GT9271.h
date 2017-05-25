#ifndef	__GT9271_H__

#define	__GT9271_H__

#include "config_hard.h"
#include "stm32f10x.h"
//#include <IIC.h>


//////////设置容值偏移的比例，，，，5%----对应设置为 5
#define GTP_MODLE_PERSENT_A         5
#define GTP_MODLE_PERSENT_B         10
#define GTP_MODLE_PERSENT_C         15
#define GTP_MODLE_PERSENT_D         20


//#define GTP_CONFIG_MIN_LENGTH       186
//#define GTP_CONFIG_MAX_LENGTH       240
//#define GTP_CONFIG_tes_LENGTH       288    ///////---for GTM70DG03 use

#define GTP_CONFIG_MIN_LENGTH       186
#define GTP_CONFIG_MAX_LENGTH       240
#define GTP_CONFIG_tes_LENGTH       870         ///////---for GTM9271 use

#define FLAG_OK                     0xCC

#define FLAG_EEPROM_SAVE_START          0x000262
#define FLAG_EEPROM_SAVE_OK             0x0007F0
#define FLAG_SAMPLE_SAVE_NUM            0x0007F1
#define FLAG_SAMPLE_NAME_ADDRE          0x0007F2




#define GT9271_PIN_CS GPIO_Pin_4
#define GPIO_CS                                             GPIOB

#define GT9271_PIN_SCL  GPIO_Pin_3
#define GPIO_GT9271_SCL                                   GPIOE		
#define GT9271_PIN_SDA  GPIO_Pin_4						
#define GPIO_GT9271_SDA                                   GPIOE	 

////------------------------------------------------------------
#define GT9271_TPSCL_L GPIO_ResetBits(GPIO_GT9271_SCL, GT9271_PIN_SCL)  //SCL
#define GT9271_TPSCL_H GPIO_SetBits  (GPIO_GT9271_SCL, GT9271_PIN_SCL)
#define GT9271_TPSDA_L GPIO_ResetBits(GPIO_GT9271_SDA, GT9271_PIN_SDA)  //SDA
#define GT9271_TPSDA_H GPIO_SetBits  (GPIO_GT9271_SDA, GT9271_PIN_SDA)



//////if  TDDI  pannel ,use SPI mode;
#define	TP_SPI__LCDPin_CSX0	(1<<4)    //PB4
#define	TP_SPI__Pin_RST	(1<<0)        //PE0      
#define	TP_SPI__LCDPin_SCL	(1<<9)   ////PB9	       
#define	TP_SPI__LCDPin_SDI	(1<<3)    ////PB3


#define TP_SPI__RST(x)    GPIOE->ODR=(GPIOE->ODR&~TP_SPI__Pin_RST)|(x ? TP_SPI__Pin_RST:0)
#define TP_SPI__CSX0(x)   GPIOB->ODR=(GPIOB->ODR&~TP_SPI__LCDPin_CSX0)|(x ? TP_SPI__LCDPin_CSX0:0)													    
#define TP_SPI__SCL(x)    GPIOB->ODR=(GPIOB->ODR&~TP_SPI__LCDPin_SCL) |(x ? TP_SPI__LCDPin_SCL:0)
#define TP_SPI__SDI(x)    GPIOB->ODR=(GPIOB->ODR&~TP_SPI__LCDPin_SDI)|(x ? TP_SPI__LCDPin_SDI:0)


#define LOW 0
#define HIGH 1
#define ACK LOW
#define NAK HIGH

////-===============================================================================================================================
////// Psudo functions
void GT9271_I2CSDASetInput(void); // Set I2C SDA signal as input
void GT9271_I2CSDASetOutput(void); // Set I2C SDA signal as output

void GT9271_GPIO_Config_release(void);
void GT9271_GPIO_Config(void);
void TOUCH_POINT_CLEAR(void);
void TOUCH_inition(void);
void CTP_GT9271_TEST(void);



void SHOW_TP_PROJECT_NAME(u16 addx,u16 addy);
u8 SET_TP_PROJECT_NAME(u8 TP_NAME);
u8 GET_TP_PROJECT_NAME(void);


//-----------------------------------------------------------------------
#endif

