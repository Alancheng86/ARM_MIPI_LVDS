/******************** (C) COPYRIGHT 2017 KSGIANTLUS ********************
* File Name          : lcm.h
* Author             : MCD Application Team
* Version            : V0.1
* Date               : 01/17/2017
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
#ifndef __LCM_H
#define __LCM_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"



void VCOM_set(u8 vcom);
void MTP(void);

void MIPI_LCD_INITION(void);
void M1651_OTA5601_INIT(void);


#endif /* __LCM_H */



/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/

