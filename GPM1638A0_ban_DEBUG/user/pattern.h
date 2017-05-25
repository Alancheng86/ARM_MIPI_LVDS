/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PATTERN_H
#define __PATTERN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "lcd.h"
#include "lcd1602.h"
#include "config_hard.h"


void Pattern(char pic_num_string);
void SDRAM_PREPARE(void);


#endif /* __PATTERN_H */
