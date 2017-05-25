#ifndef _DISP_H_
#define _DISP_H_

#include "../CMSIS/stm32f10x.h"
#include "FAT.h"


//#define u8  alt_u8 
//#define u16 alt_u16
//#define u32 alt_u32

//void Disp_BMP(void);
int Disp_BMP(u16 Xdp,u16 Ydp,u8 n);
void Get_testpar(void);
void Disp_TXT(void);
    

#endif //_DISP_H_

