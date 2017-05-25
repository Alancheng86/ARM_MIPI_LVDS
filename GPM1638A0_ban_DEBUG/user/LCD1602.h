/******************** (C) COPYRIGHT 2016 KSGIANTLUS ********************
* File Name          : lcd1602.h
* Author             : MCD Application Team
* Version            : V1.1
* Date               : 04/10/2016
* Description        : This file contains all the functions prototypes for the
*                      lcd firmware driver.
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD1602_H
#define __LCD1602_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

#define LCD1602_H 1
#define LCD1602_L 0
#define LCD1602_DAT 1//数据
#define LCD1602_COM 0//命令
#define LCD_SHOWS0 0X0C//开显示，不显示光标不显示闪烁
#define LCD_SHOWS1 0X0E//开显示，显示光标，光标不闪烁
#define LCD_SHOWS2 0X0F//开显示，显示光标，光标闪烁
#define LCD_SHOWS3 0X08//关显示，不显示光标，光标不闪烁
//#define LCD_Clear 0x01//清屏指令			/////////与LCD.H中重复，此处屏蔽，独用。


/////////-----------------LCD端口定义---------------- 
#define	LCD1602RS	(1<<4)  //数据/命令选择端口	
#define	LCD1602RW	(1<<10)  //LCD片选端口  	      
#define	LCD1602EN	(1<<11)  //硬复位	


#define LCD1602_RS(x)   GPIOC->ODR=(GPIOC->ODR&~LCD1602RS)|(x ? LCD1602RS:0)
#define LCD1602_RW(x)   GPIOB->ODR=(GPIOB->ODR&~LCD1602RW)|(x ? LCD1602RW:0)													    
#define LCD1602_EN(x)   GPIOB->ODR=(GPIOB->ODR&~LCD1602EN)|(x ? LCD1602EN:0)	


void delayxms(u32 kk);
void LCD1602_PIN_Configuration(void);
void LCD1602_Init(void);
void Clear_LCD1602(void);
void LCD1602_Write(unsigned char COD,unsigned char data);
void LCD1602_Write_L4BIT(unsigned char COD,unsigned char data);
void OUTPUT_DATA(unsigned char Ddata);
void LCD1602_Pos(unsigned char pos);
void LCD1602_display_Line(unsigned char posx,unsigned char *p);
void LCD1602_display_Line_A(unsigned char posx,unsigned char sx,unsigned char *p);
void LCD1602_display_Word(unsigned char posx,unsigned char data);


#define Line1602_0          0
#define Line1602_1          1


void LCD1602_display_hex(unsigned short x,unsigned short y,u16 dat);
void LCD1602_display_DATA(unsigned short x,unsigned short y,unsigned short data);






void LCD1602_LED_OFF(void);
void LCD1602_LED_ON(void);
void LCD1602_LED_RUN(void);
void LED_RUN_BIBI(void);
void BEEP_BIBI(void);
void LED_RUN(void);

#endif /* __LCD1602_H */



/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


