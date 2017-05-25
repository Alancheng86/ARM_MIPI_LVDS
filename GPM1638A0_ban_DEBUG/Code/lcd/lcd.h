
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __LCD_H
#define __LCD_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


//STM32 
	#define LCD_BASE0        ((u32)0x60000000)//raR5421
	#define LCD_BASE1        ((u32)0x60020000)//raR5421

	#define LCD_CmdWrite(cmd)	  *(vu16*) (LCD_BASE0)= (cmd);
	#define LCD_DataWrite(data)   *(vu16*) (LCD_BASE1)= (data);
	#define	LCD_StatusRead()	 *(vu16*) (LCD_BASE0) //
	#define	LCD_DataRead()   	 *(vu16*) (LCD_BASE1) //

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

//DEFINE line
#define  APP_LINE_0                0
#define  APP_LINE_1               24
#define  APP_LINE_2               48
#define  APP_LINE_3               72
#define  APP_LINE_4               96
#define  APP_LINE_5              120
#define  APP_LINE_6              144
#define  APP_LINE_7              168
#define  APP_LINE_8              192
#define  APP_LINE_9              216

/* LCD color */
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0


#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216
#define Line10         240
#define Line11         264
#define Line12         288
#define Line13         312
#define Line14         336
#define Line15         360
#define Line16         384
#define Line17         408
#define Line18         432
#define Line19         456
#define Line20         480
#define Line21         504
#define Line22         528
#define Line23         552
#define Line24         576
#define Line25         600
#define Line26         624
#define Line27         648
#define Line28         672
#define Line29         696
#define Line30         720
#define Line31         744

#define Horizontal     0x00
#define Vertical       0x01

#define RGB				0
#define BGR				1
#define LVDS_8BIT		0
#define LVDS_6BIT		1

#define RGB_mode			0
#define YUV_16BIT			1
#define YUV_8BIT			2


/* 字体代码 */
enum
{
	FC_ST_16 = 0,		/* 宋体15x16点阵 （宽x高） */
	FC_ST_12 = 1,		/* 宋体12x12点阵 （宽x高） */
	FC_ST_48 = 2		/* 宋体48x48点阵 （宽x高） */
};



/* 字体属性结构, 用于LCD_DispStr() */
typedef struct
{
	uint16_t usFontCode;	/* 字体代码 0 表示16点阵 */
	uint16_t usTextColor;	/* 字体颜色 */
	uint16_t usBackColor;	/* 文字背景颜色，透明 */
	uint16_t usSpace;		/* 文字间距，单位 = 像素 */
}FONT_T;





void R5421_HW_Reset(void);
void System_Check_Temp(void);	//检测复位是否成功

void lcd_rst(void);

/*----- Medium layer function -----*/
void LCD_WriteRAM_Prepare(void);

void LCD_SetTextColor(u16 Color);
void LCD_SetBackColor(u16 Color);

//void LCD_Clear(u16 Color);
void LCD_Clear(u16 Color);
void LCD_SetCursor(u16 Xpos, u16 Ypos);
		 
void LCD_SetPoint(u16 x,u16 y,u16 point);
void LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);

void LCD_DisplayString(u16 X,u16 Y, char *ptr, u16 charColor, u16 bkColor);
void LCD_DisplayString_A(u16 X,u16 Y, char *ptr, u16 charColor, u16 bkColor);

void LCD_PutString48(uint16_t x, uint16_t y, uint8_t *s, uint16_t fColor, uint16_t bColor);  	//
void DispStr48(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont);
void LCD_PutChar2448(uint16_t x, uint16_t y, uint8_t c[1], uint16_t fColor, uint16_t bColor);
void LCD_PutGB4848(uint16_t x, uint16_t y, uint8_t c[2], uint16_t fColor,uint16_t bColor); 	
void LCD_DisplayStringLine(u16 Line, char *ptr, u16 charColor, u16 bkColor);
void LCD_SetDisplayWindow(u16 Xpos, u16 Ypos, u8 Height, u16 Width);

void LCD_DrawLine(u16 Xpos, u16 Ypos, u16 Length, u8 Direction);
void LCD_DrawRect(u16 Xpos, u16 Ypos, u16 Height, u16 Width);
void LCD_DrawCircle(u16 Xpos, u16 Ypos, u16 Radius);
void LCD_DrawMonoPict(uc32 *Pict);
void LCD_WriteBMP(u32 BmpAddress);

void LCD_WriteArea(u16 add_sx, u16 add_ex, u16 add_sy, u16 add_ey, u8 Color_rr,u8 Color_gg,u8 Color_bb);
void LCD_Set_DisplayWindow(u16 add_sx, u16 add_ex, u16 add_sy, u16 add_ey);
void LCD_Set_DisplayWindow_A(u8 pic_area_num);
void LCD_Set_DisplayWindow_B(u8 pic_area_num);

void PIP_Set_DisplayWindow_A(u8 pic_area_num);
void PIP_Set_DisplayWindow_B(u8 pic_area_num);
void LCD_All_color(u8 Color_rr,u8 Color_gg,u8 Color_bb);
void LCD_All_color_A(u8 Color_rr,u8 Color_gg,u8 Color_bb);
void LCD_Draw_Line(u16 resx1,u16 resy1,u16 resx2,u16 resy2,u8 Color_rr,u8 Color_gg,u8 Color_bb);
void LCD_DisplayString_data_A(u16 X,u16 Y, u32 data, u16 charColor, u16 bkColor);
void LCD_Border_A(void);
void checkerboard(void);

#endif /* __LCD_H */


