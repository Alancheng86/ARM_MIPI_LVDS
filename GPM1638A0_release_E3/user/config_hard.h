#ifndef __CONFIG_HARD_H
#define __CONFIG_HARD_H 	

#include "stdio.h"
#include <stdlib.h>

#include "stm32f10x_tim.h"
#include "UserDef.h"
#include "lcd.h"
#include "delay.h"
#include "stdarg.h"

#include "lcd1602.h"

#include "lcm.h"


#define uint  u16


#define KEY_UP      GPIOB->IDR&GPIO_Pin_6
#define KEY_DOWN      GPIOC->IDR&GPIO_Pin_6
#define KEY_MAN_MODE      GPIOD->IDR&GPIO_Pin_13


/////FOR KEY BOARD USE
#define KEY_AUTO_MODE      GPIOB->IDR&GPIO_Pin_2
#define KEY_RED      	GPIOC->IDR&GPIO_Pin_3
#define KEY_PAUSE		GPIOC->IDR&GPIO_Pin_15	

///////////////设定画面的编号，以便在 config_hard.c 里面 进行画面显示顺序的控制
///////////////             0----->  white-----
///////////////             1----->  red
            ///             2----->  green
            ///             3----->  blue
            ///             4----->  gray(带参数）
            ///             5----->  Border
            ///             6----->  black
            ///             7----->  white
            ///             8----->  corsstalk
            ///             9----->  colorbar
            ///             10---->  TBD
            
            ///          0x20----->  pic FROM SD card
            ///          0x21----->  pic FROM SDRAM card
           
#define PIC_RED                     0x01
#define PIC_GREEN                   0x02
#define PIC_BLUE                    0x03
#define PIC_GRAY                    0x04
#define PIC_BORDER                  0x05
#define PIC_BLACK                   0x06
#define PIC_WHITE                   0x07
#define PIC_CT                      0x08  
#define PIC_colorbar_V              0x09
#define PIC_colorbar_H              0x0A
#define PIC_checkerboard            0x0B
#define PIC_GRAY_H                  0x0C
#define PIC_GRAY_V                  0x0D
#define PIC_RED_H                   0x0E
#define PIC_RED_V                   0x0F
#define PIC_GREEN_H                 0x10
#define PIC_GREEN_V                 0x11
#define PIC_BLUE_H                  0x12
#define PIC_BLUE_V                  0x13
#define PIC_Flicker_H               0x14
#define PIC_Flicker_V               0x15
#define PIC_Flicker_H_DOT           0x16
#define PIC_Dot_Flicker_A			0x17
#define PIC_Dot_Flicker_B			0x18
#define PIC_Dot_Flicker_C			0x19
#define PIC_CT_center				0x1A
#define PIC_BIT_COLOR_H				0x1B
#define PIC_BIT_COLOR_V				0x1C

#define PIC_Dot_Flicker_z			31			/////PIC_NUM   MAX

#define PIC_FROM_SD                 0X20
#define PIC_FROM_SDRAM              0X21
#define PIC_SAVE_NUM_MAX            60


////////define  usart use  ID
#define ID_USART_START					0x55
#define	ID_USART_END					0xAA
#define KEY_HAND_START					0x5A
#define KEY_HAND_BACK					0xA5

#define KEY_HAND_BACK_O					0x4F
#define KEY_HAND_BACK_K					0x4B

#define NUM_USART_PACK					7


#define LCM_TYPE_start                  1

#define NOKIA_GPM1461A                  12

#define LCM_TYPE_end                    10   ////4

//========================================= GPIO CONFIG ===================================================

//----------------- GPIO : STM32 --> SSD2828  ----------------
void SSD2828_RST(bool bit);
void SSD2828_CSX0(bool bit);
void SSD2828_SCL(bool bit);
void SSD2828_SDI(bool bit);


//----------------- GPIO : STM32 --> RGB  ----------------
void RGB_RST(bool bit);
void RGB_CSX0(bool bit);
void RGB_SCL(bool bit);
void RGB_SDI(bool bit);

//----------------- GPIO : STM32 --> LVDS  ----------------
void LVDS_RST(bool bit);		
void LVDS_CSX0(bool bit);		
void LVDS_SCL(bool bit);		
void RLVDS_SDI(bool bit);		




#define FREQ_SSD2828  12
#define LCD_TYPE_ADRESS     0x00000016



#define DELAY_20        1050
#define MIPI_OFF		0
#define MIPI_ON			1




void DelayMs(u16 xxx);
void MDelay(u32 timetemp);
void Delay(u32 ttt);

void STM32TOSSD2828_W_DATA_8BITS(u8 DATA);
void STM32TOSSD2828_W_DATA_16BITS(u16 DATA2);  //16 bit
void STM32TOSSD2828_W_COM(u8 CMD);
void MIPI_SPI_Write(uint i,...);
u16 SSD2825_READ_AnyByte(u8 CMD);
u16 READ_SSD2828_onebyte(u8 CMD);
void STM32_Init_SSD2828(void);
void SSD2828_inition_lcd(void);
void LCM_TYPE_SHOW_ON_1602(void);
void KEY_SCAN_CONTROL(void);

void VDD_config(void);
void HardWare_GPIO_Configuration(void);
void LCM_CHANGE(void);

void BL_OFF(void);
void BL_ON(void);
void VSPN_OFF(void);
void VSPN_ON(void);
void VDD_ON(void);
void VDD_OFF(void);
void LCM_VDD_ON(void);
void MTP_5V_ON(void);
void MTP_5V_OFF(void);
void SSD1963INITIALSPHX8363(void);
void enter_sleep_mode(void);

void SSD2828_ENTER_LP_mode(void);
void SSD2828_ENTER_READ_LCDIC_mode(void);
void SSD2828_VIDEO_MODE_HS(void);

void SSD1963GPIOOUTCOM(u16 com);
void RGB_DATA16(u16 com);
void RGB_DATA9(u16 COM);
void RGB_DATA8(u16 COM);


int USART_SWITCH(void);
int Do_usart_cmd(void);


#endif

//

