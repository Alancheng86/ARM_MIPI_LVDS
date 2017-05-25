/* Includes ------------------------------------------------------------------*/
#include "fsmc_sram.h"
#include "stdlib.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "UserDef.h"
#include "lcd.h"
#include "fonts.h"
#include "R5421.h"
#include <string.h>

//#include <string.h>
char color_map[3*16];
////////timing  setting
extern u16 TIMING_SET[9];


#define XDP		TIMING_SET[1]
#define YDP		TIMING_SET[2]

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/                              
/* Private variables ---------------------------------------------------------*/
/* Global variables to set the written text color */
static  u16 TextColor = 0x0000, BackColor = 0xFFFF;
extern u8 SDShowTimes;  
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
extern u8 YUV_RGB;
extern u8 SD_BMP_BGR;
extern u8 LVDS_8BIT_USE;
//u32 color=0x1;
//------------------------------------
/**/
void R5421_HW_Reset(void)
{

	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    //delay_ms(1);	//	
	delay_ms(100);				   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
    delay_ms(100);	
}


void System_Check_Temp(void)
{
	unsigned char i=0;
	unsigned char temp=0;
	unsigned char system_ok=0;
	do
	{
		if((LCD_StatusRead()&0x02)==0x00)
		{
			delay_ms(1); //ÈôMCU ËÙ¶ÈÌ«¿ì£¬±ØÒª•rÊ¹ÓÃ
			LCD_CmdWrite(0x01);
			delay_ms(1); //ÈôMCU ËÙ¶ÈÌ«¿ì£¬±ØÒª•rÊ¹ÓÃ
			temp =LCD_DataRead();
			if((temp & 0x80)==0x80)
			{
				system_ok=1;
				i=0;
			}
			else
			{
				delay_ms(1); //ÈôMCU ËÙ¶ÈÌ«¿ì£¬±ØÒª•rÊ¹ÓÃ
				LCD_CmdWrite(0x01);
				delay_ms(1); //ÈôMCU ËÙ¶ÈÌ«¿ì£¬±ØÒª•rÊ¹ÓÃ
				LCD_DataWrite(0x80);
			}
		}
		else
		{
			system_ok=0;
			i++;
		}
		if(system_ok==0 && i==5)
		{
			R5421_HW_Reset(); //note1
			i=0;
		}
	}while(system_ok==0);
}



void lcd_rst(void)
{
	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
	delay_ms(5);					   
    GPIO_SetBits(GPIOE, GPIO_Pin_1 );		 	 
    delay_ms(10);	
}


void LCD_WriteRAM_Prepare(void)
{
  LCD_CmdWrite(0x04); //
}

void LCD_Draw2Point(u32 point1,u32 point2)  //?Display RAM ??1????,????“LCD_WriteRAM_Prepare”????
{ 
    switch(SD_BMP_BGR)
	{
		case RGB:
			switch(LVDS_8BIT_USE)
			{
				case LVDS_8BIT:
				{
					LCD_DataWrite(point1&0x00FFFF);						
					LCD_DataWrite(((point2&0x0000FF)<<8) | ((point1&0xFF0000)>>16));	
					LCD_DataWrite((point2&0xFFFF00)>>8);					
				}
				break;
				case LVDS_6BIT:
				{
					LCD_DataWrite((point1>>2)&0x003F3F);						
					LCD_DataWrite((((point2>>2)&0x00003F)<<8) | (((point1>>2)&0x3F0000)>>16));	
					LCD_DataWrite(((point2>>2)&0x3F3F00)>>8);
				}
				break;
				
				default:
				{
					LCD_DataWrite(point1);						
					LCD_DataWrite((point2<<8) | (point1>>16));	
					LCD_DataWrite(point2>>8);	
				}
					break;
					
			}
			break;
		
		case BGR:
			switch(LVDS_8BIT_USE)
			{	case LVDS_8BIT:
				{
					LCD_DataWrite(((point1)&0X00FF00) | (point1>>16));						
					LCD_DataWrite(((point2&0XFF0000)>>8) | (point1&0XFF));	
					LCD_DataWrite((point2<<8) | ((point2&0XFF00)>>8));					
				}
				break;
				
				case LVDS_6BIT:
				{
					LCD_DataWrite(((point1>>2)&0X003F00) | (((point1>>2)&0x3F0000)>>16));						
					LCD_DataWrite((((point2>>2)&0X3F0000)>>8) | ((point1>>2)&0X00003F));	
					LCD_DataWrite((((point2>>2)&0x00003F)<<8) | (((point2>>2)&0X003F00)>>8));
				}
				break;
			
				default:
				{
					LCD_DataWrite(((point1)&0X00FF00) | (point1>>16));						
					LCD_DataWrite(((point2&0XFF0000)>>8) | (point1&0XFF));	
					LCD_DataWrite((point2<<8) | ((point2&0XFF00)>>8));
				}
				break;
				
			}
			break;
		
		default	:
		switch(LVDS_8BIT_USE)
			{
				case LVDS_8BIT:
				{
					LCD_DataWrite(point1);						
					LCD_DataWrite((point2<<8) | (point1>>16));	
					LCD_DataWrite(point2>>8);					
				}
				break;
				case LVDS_6BIT:
				{
					LCD_DataWrite((point1>>2)&0x003F3F);						
					LCD_DataWrite((((point2>>2)&0x00003F)<<8) | (((point1>>2)&0x3F0000)>>16));	
					LCD_DataWrite(((point2>>2)&0x3F3F00)>>8);
				}
				break;
				
				default:
				{
					LCD_DataWrite(point1);						
					LCD_DataWrite((point2<<8) | (point1>>16));	
					LCD_DataWrite(point2>>8);	
				}
					break;
					
			}
		break;
		
	}
	
//	if(SD_BMP_BGR==RGB)
//	{
//        
//        LCD_DataWrite(point1);						
//        LCD_DataWrite((point2<<8) | (point1>>16));	
//        LCD_DataWrite(point2>>8);					
//      
//    }
//	else if(SD_BMP_BGR==BGR)
//	{
//        
//        LCD_DataWrite(((point1)&0X00FF00) | (point1>>16));						
//        LCD_DataWrite(((point2&0XFF0000)>>8) | (point1&0XFF));	
//        LCD_DataWrite((point2<<8) | ((point2&0XFF00)>>8));					
//      
//    }
//	else
//	{
//		LCD_DataWrite(point1);						
//        LCD_DataWrite((point2<<8) | (point1>>16));	
//        LCD_DataWrite(point2>>8);
//	}
}

void LCD_DrawPoint(u32 point1,u32 point2)  //?Display RAM ??1????,????“LCD_WriteRAM_Prepare”????
{ //???????
//	u8 rr1,gg1,bb1,rr2,gg2,bb2;
 
//  rr1=(point1>>16);  
//  gg1=(point1>>8);  
//  bb1=point1;
//  rr2=(point2>>16);  
//  gg2=(point2>>8);  
//  bb2=point2;
  
    if(SDShowTimes == 1)
    {
        
        LCD_DataWrite(point1);	
        LCD_DataWrite((point2<<8) | (point1>>16));	
        LCD_DataWrite(point2>>8);
        
//    LCD_DataWrite((gg1<<8) | bb1);		
////			Check_Mem_WR_FIFO_not_Full();
//			LCD_DataWrite((bb2<<8) | rr1);		
////			Check_Mem_WR_FIFO_not_Full();
//			LCD_DataWrite((rr2<<8) | gg2);		
////			Check_Mem_WR_FIFO_not_Full();
    }      
//	else
//	{	;	}
}
/*******************************************************************************
* Function Name  : LCD_SetTextColor
* Description    : Sets the Text color.
* Input          : - Color: specifies the Text color code RGB(5-6-5).
* Output         : - TextColor: Text color global variable used by LCD_DrawChar
*                  and LCD_DrawPicture functions.
* Return         : None
*******************************************************************************/
void LCD_SetTextColor(u16 Color)
{
  TextColor = Color;
}

/*******************************************************************************
* Function Name  : LCD_SetBackColor
* Description    : Sets the Background color.
* Input          : - Color: specifies the Background color code RGB(5-6-5).
* Output         : - BackColor: Background color global variable used by 
*                  LCD_DrawChar and LCD_DrawPicture functions.
* Return         : None
*******************************************************************************/
void LCD_SetBackColor(u16 Color)
{
  BackColor = Color;
}


/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : Clears the hole LCD.
* Input          : Color: the color of the background.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Clear(u16 Color)
{
  	u32 index = 0;
  
  	LCD_SetCursor(0,0); 	 //ÉèÖÃ¹â±êÎ»ÖÃ
    
    LCD_Set_DisplayWindow(0,XDP-1,0,YDP-1);
  	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */

//  	for(index = 0; index < 1572864; index++)	//1024*768
    for(index = 0; index < YDP*XDP*3/2; index++)	//480*854
  	{
    	LCD_DataWrite(Color);
        Check_Mem_WR_FIFO_not_Full();
//        while(LCD_StatusRead()&(Color&0xFF))
//        {   LCD_DataWrite(Color);   }
  	}
 	// while(1);  
}


/*******************************************************************************
* Function Name  : LCD_SetDisplayWindow
* Description    : Sets a display window
* Input          : - Xpos: specifies the X buttom left position.
*                  - Ypos: specifies the Y buttom left position.
*                  - Height: display window height.
*                  - Width: display window width.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Set_DisplayWindow(u16 add_sx, u16 add_ex, u16 add_sy, u16 add_ey)
{            //???????
  u16 sx= add_sx;
  u16 ex= add_ex;
  u16 sy= add_sy;
  u16 ey= add_ey;
  
//	LCD_SetCursor(0,0); 	 //ÉèÖÃ¹â±êÎ»ÖÃ
	
  Canvas_Image_Start_address(0*0);	//Layer 1;»­²¼Í¼ÏñÆðÊ¼µØÖ·
	Canvas_image_width(XDP);		//»­²¼Í¼Ïñ¿í¶È
	Active_Window_XY(sx,sy);
	Active_Window_WH(ex-sx+1,ey-sy+1);	   //ÉèÖÃ¹¤×÷´°¿Ú´óÐ¡

	Goto_Pixel_XY(0,0);

}

void LCD_Set_DisplayWindow_C(u8 pic_area_num,u16 add_sx, u16 add_ex, u16 add_sy, u16 add_ey)
{            //???????
  u16 sx= add_sx;
  u16 ex= add_ex;
  u16 sy= add_sy;
  u16 ey= add_ey;
	

  
//	LCD_SetCursor(0,0); 	 //ÉèÖÃ¹â±êÎ»ÖÃ
    
    Canvas_Image_Start_address((XDP*YDP*3)*pic_area_num);	//Layer 1;»­²¼Í¼ÏñÆðÊ¼µØÖ·
	Canvas_image_width(XDP);		//»­²¼Í¼Ïñ¿í¶È
	Active_Window_XY(sx,sy);
    
	Active_Window_WH(ex-sx+1,ey-sy+1);	   //ÉèÖÃ¹¤×÷´°¿Ú´óÐ¡

    
	Goto_Pixel_XY(0,0);

}

void LCD_Set_DisplayWindow_A(u8 pic_area_num)
{            //???????
  u16 sx= 0;
  u16 ex= XDP-1;
  u16 sy= 0;
  u16 ey= YDP-1;

  
//	LCD_SetCursor(0,0); 	 //ÉèÖÃ¹â±êÎ»ÖÃ
    
    Canvas_Image_Start_address((XDP*YDP*3)*3);	//Layer 1;»­²¼Í¼ÏñÆðÊ¼µØÖ·
	Canvas_image_width(XDP);		//»­²¼Í¼Ïñ¿í¶È
	Active_Window_XY(sx,sy);
    
	Active_Window_WH(ex-sx+1,ey-sy+1);	   //ÉèÖÃ¹¤×÷´°¿Ú´óÐ¡

    
	Goto_Pixel_XY(0,0);

}

void LCD_Set_DisplayWindow_B(u8 pic_area_num)
{            //???????
  u16 sx= 0;
//  u16 ex= XDP-1;
  u16 sy= 0;
  u16 ey= YDP-1;

    u16 XDP1 = 0;
    u8 i=0;
  
    

    
//	LCD_SetCursor(0,0); 	 //ÉèÖÃ¹â±êÎ»ÖÃ
    
//    while((XDP+i)%8 != 0)
//    {   i++;    }
    XDP1 = XDP + i;    /////Îª±ãÓÚ»­²¼¿í¶È·ûºÏ8µÄ±¶Êý£¬¶ÔSDÍ¼Æ¬½øÐÐ´¦Àí¡£
    
    Canvas_Image_Start_address((XDP*YDP*3)*pic_area_num);	//Layer 1;»­²¼Í¼ÏñÆðÊ¼µØÖ·
	Canvas_image_width(XDP1);		//»­²¼Í¼Ïñ¿í¶È
	Active_Window_XY(sx,sy);
    
	Active_Window_WH(XDP1-1-sx+1,ey-sy+1);	   //ÉèÖÃ¹¤×÷´°¿Ú´óÐ¡

    
	Goto_Pixel_XY(0,0);

}

void PIP_Set_DisplayWindow_A(u8 pic_area_num)
{
    PIP_Image_Start_Address((XDP*YDP*3)*pic_area_num);
    PIP_Image_Width(XDP);
    PIP_Window_Width_Height(XDP,YDP);
    PIP_Window_Image_Start_XY(0,0);
    PIP_Display_Start_XY(0,0);
}

void PIP_Set_DisplayWindow_B(u8 pic_area_num)
{
    u16 XDP1 = 0;
    u8 i=0;
    
    while((XDP+i)%8 != 0)
    {   i++;    }
    XDP1 = XDP + i;    /////Îª±ãÓÚ»­²¼¿í¶È·ûºÏ8µÄ±¶Êý£¬¶ÔSDÍ¼Æ¬½øÐÐ´¦Àí¡£
    
    PIP_Image_Start_Address((XDP*YDP*3)*pic_area_num);
    PIP_Image_Width(XDP1);
    PIP_Window_Width_Height(XDP1,YDP);
    PIP_Window_Image_Start_XY(0,0);
    PIP_Display_Start_XY(0,0);
}
/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : Clears the hole LCD.
* Input          : Color: the color of the background.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_All_color(u8 Color_rr,u8 Color_gg,u8 Color_bb)
{
  u32 index = 0;
	u16 color_temp1,color_temp2,color_temp3;
	
	switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			Color_rr = Color_rr;
			Color_gg = Color_gg;
			Color_bb = Color_bb;
			break;
		
		case LVDS_6BIT:
			Color_rr = Color_rr&0x3F;
			Color_gg = Color_gg&0x3F;
			Color_bb = Color_bb&0x3F;
			break;
		
		default :
			Color_rr = Color_rr;
			Color_gg = Color_gg;
			Color_bb = Color_bb;
			break;
	}

//	LCD_Set_DisplayWindow(0,XDP-1,0,YDP-1);
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	
	color_temp1 = (Color_gg<<8) | Color_bb;
	color_temp2 = (Color_bb<<8) | Color_rr;
	color_temp3 = (Color_rr<<8) | Color_gg;
	
  	for(index = 0; index < XDP*YDP/2; index++)	//1024*768   1572864
  	{
    	LCD_DataWrite(color_temp1);		
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite(color_temp2);		
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite(color_temp3);		
			Check_Mem_WR_FIFO_not_Full();
  	}
 	// while(1);  
}

void LCD_All_color_A(u8 Color_rr,u8 Color_gg,u8 Color_bb)
{
  u32 index = 0;
	u16 color_temp1,color_temp2,color_temp3;
	
	switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			Color_rr = Color_rr;
			Color_gg = Color_gg;
			Color_bb = Color_bb;
			break;
		
		case LVDS_6BIT:
			Color_rr = (Color_rr>>0)&0x3F;
			Color_gg = (Color_gg>>0)&0x3F;
			Color_bb = (Color_bb>>0)&0x3F;
			break;
		
		default :
			Color_rr = Color_rr;
			Color_gg = Color_gg;
			Color_bb = Color_bb;
			break;
	}
//	LCD_Set_DisplayWindow(0,XDP-1,0,YDP-1);
//	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	
	color_temp1 = (Color_gg<<8) | Color_bb;
	color_temp2 = (Color_bb<<8) | Color_rr;
	color_temp3 = (Color_rr<<8) | Color_gg;
	
  	for(index = 0; index < XDP*YDP/2; index++)	//1024*768   1572864
  	{
    	LCD_DataWrite(color_temp1);		
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite(color_temp2);		
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite(color_temp3);		
			Check_Mem_WR_FIFO_not_Full();
  	}
 	// while(1);  
}

void write_pixel(char x,char y,char z)
{
	switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			z = z;
			break;
		
		case LVDS_6BIT:
			z = z & 0x3F;
			break;
		
		default :
			z = z;
			break;
	}
	
	switch(y)
  {
    case 0:
          color_map[x+0] = z;
		  color_map[x+1] = 0;
		  color_map[x+2] = z;
          break;
    case 1:
          color_map[x+0] = 0;
		  color_map[x+1] = z;
		  color_map[x+2] = 0;
          break;
    case 2:
	      color_map[x+0] = z;
		  color_map[x+1] = z;
		  color_map[x+2] = 0;
          break;
    case 3:
          color_map[x+0] = 0;
		  color_map[x+1] = z;
		  color_map[x+2] = z;
          break;
    case 4:
          color_map[x+0] = 0;
		  color_map[x+1] = 0;
		  color_map[x+2] = z;
          break;
    case 5:
          color_map[x+0] = z;
		  color_map[x+1] = 0;
		  color_map[x+2] = 0;
          break; 
	case 6:
          color_map[x+0] = z;
		  color_map[x+1] = z;
		  color_map[x+2] = z;
          break; 
	case 7:
          color_map[x+0] = 0;
		  color_map[x+1] = 0;
		  color_map[x+2] = 0;
          break; 
	
  }
}

void Dot_inver_set(u8 inver,u8 g)
{
	u8 i;
	
	switch(inver)
	{
    case 0://Column
          color_map[3*0]= 0;  color_map[3*1]=1; color_map[3*2]= 0; color_map[3*3]= 1;
          color_map[3*4]= 0;  color_map[3*5]=1; color_map[3*6]= 0; color_map[3*7]= 1;
          color_map[3*8]= 0;  color_map[3*9]=1; color_map[3*10]=0; color_map[3*11]=1;
          color_map[3*12]=0; color_map[3*13]=1; color_map[3*14]=0; color_map[3*15]=1;
           for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			}
          break;
    case 1://2V1H
          color_map[3*0]= 0;  color_map[3*1]=1; color_map[3*2]= 0; color_map[3*3]= 1;
          color_map[3*4]= 0;  color_map[3*5]=1; color_map[3*6]= 0; color_map[3*7]= 1;
          color_map[3*8]= 1;  color_map[3*9]=0; color_map[3*10]=1; color_map[3*11]=0;
          color_map[3*12]=1; color_map[3*13]=0; color_map[3*14]=1; color_map[3*15]=0;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			}
          break;
    case 2://1Dot
          color_map[3*0]= 0;  color_map[3*1]=1; color_map[3*2]= 0; color_map[3*3]= 1;
          color_map[3*4]= 1;  color_map[3*5]=0; color_map[3*6]= 1; color_map[3*7]= 0;
          color_map[3*8]= 0;  color_map[3*9]=1; color_map[3*10]=0; color_map[3*11]=1;
          color_map[3*12]=1; color_map[3*13]=0; color_map[3*14]=1; color_map[3*15]=0;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			}
          break;
    case 3://1V2H
          color_map[3*0]= 2;  color_map[3*1]=3; color_map[3*2]= 4; color_map[3*3]= 5;
          color_map[3*4]= 4;  color_map[3*5]=5; color_map[3*6]= 2; color_map[3*7]= 3;
          color_map[3*8]= 2;  color_map[3*9]=3; color_map[3*10]=4; color_map[3*11]=5;
          color_map[3*12]=4; color_map[3*13]=5; color_map[3*14]=2; color_map[3*15]=3;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			} 
          break; 
    case 4://1+2V1H
          color_map[3*0]= 0;  color_map[3*1]=1; color_map[3*2]= 0; color_map[3*3]= 1;
          color_map[3*4]= 1;  color_map[3*5]=0; color_map[3*6]= 1; color_map[3*7]= 0;
          color_map[3*8]= 1;  color_map[3*9]=0; color_map[3*10]=1; color_map[3*11]=0;
          color_map[3*12]=0; color_map[3*13]=1; color_map[3*14]=0; color_map[3*15]=1;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			}
          break;
    case 5://1+1V2H
          color_map[3*0]= 5;  color_map[3*1]=2; color_map[3*2]= 3; color_map[3*3]= 4;
          color_map[3*4]= 3;  color_map[3*5]=4; color_map[3*6]= 5; color_map[3*7]= 2;
          color_map[3*8]= 5;  color_map[3*9]=2; color_map[3*10]=3; color_map[3*11]=4;
          color_map[3*12]=3; color_map[3*13]=4; color_map[3*14]=5; color_map[3*15]=2;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			} 
          break;
    case 6://2Dot
          color_map[3*0]= 2;  color_map[3*1]=3; color_map[3*2]= 4; color_map[3*3]= 5;
          color_map[3*4]= 2;  color_map[3*5]=3; color_map[3*6]= 4; color_map[3*7]= 5;
          color_map[3*8]= 4;  color_map[3*9]=5; color_map[3*10]=2; color_map[3*11]=3;
          color_map[3*12]=4; color_map[3*13]=5; color_map[3*14]=2; color_map[3*15]=3;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			} 
			break;  
	case 7://2V1H---DOT
          color_map[3*0]= 7;  color_map[3*1]=6; color_map[3*2]= 7; color_map[3*3]= 6;
          color_map[3*4]= 7;  color_map[3*5]=6; color_map[3*6]= 7; color_map[3*7]= 6;
          color_map[3*8]= 6;  color_map[3*9]=7; color_map[3*10]=6; color_map[3*11]=7;
          color_map[3*12]=6; color_map[3*13]=7; color_map[3*14]=6; color_map[3*15]=7;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			} 
			break; 
			
	case 8://1V1H---column
          color_map[3*0]= 6;  color_map[3*1]=6; color_map[3*2]= 6; color_map[3*3]= 6;
          color_map[3*4]= 7;  color_map[3*5]=7; color_map[3*6]= 7; color_map[3*7]= 7;
          color_map[3*8]= 6;  color_map[3*9]=6; color_map[3*10]=6; color_map[3*11]=6;
          color_map[3*12]=7; color_map[3*13]=7; color_map[3*14]=7; color_map[3*15]=7;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			} 
			break; 
	
	case 9://1 column
          color_map[3*0]= 6;  color_map[3*1]=7; color_map[3*2]= 6; color_map[3*3]= 7;
          color_map[3*4]= 6;  color_map[3*5]=7; color_map[3*6]= 6; color_map[3*7]= 7;
          color_map[3*8]= 6;  color_map[3*9]=7; color_map[3*10]=6; color_map[3*11]=7;
          color_map[3*12]=6; color_map[3*13]=7; color_map[3*14]=6; color_map[3*15]=7;
			for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			} 
			break;
			
	default :
          color_map[3*0]= 0;  color_map[3*1]=1; color_map[3*2]= 0; color_map[3*3]= 1;
          color_map[3*4]= 0;  color_map[3*5]=1; color_map[3*6]= 0; color_map[3*7]= 1;
          color_map[3*8]= 0;  color_map[3*9]=1; color_map[3*10]=0; color_map[3*11]=1;
          color_map[3*12]=0; color_map[3*13]=1; color_map[3*14]=0; color_map[3*15]=1;
           for(i=0;i<16*3;i+=3)
			{	
				write_pixel(i,color_map[i],g);  
			}
          break;
	  }
}
	
void Dot_Flicker_A(u8 u,u8 g)
{
	u32 index = 0;
	u32 index_xdp = 0;
	u16 color_temp1,color_temp2,color_temp3,color_temp4,color_temp5,color_temp6;
	u8 Color_rr,Color_gg,Color_bb ;
	u8 Color_rr1 ,Color_gg1 ,Color_bb1 ;
	u8 Color_rr2 ,Color_gg2 ,Color_bb2 ;
	u8 Color_rr3 ,Color_gg3 ,Color_bb3 ;
	
	u8 Y1,Cr1,Cb1,Y2;
	
	Dot_inver_set(u,g);
	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	
	
	for(index = 0; index < YDP/4; index++)
	{
			Color_rr = color_map[3*0+0];
			Color_gg = color_map[3*0+1];
			Color_bb = color_map[3*0+2];
			Color_rr1 = color_map[3*1+0];
			Color_gg1 = color_map[3*1+1];
			Color_bb1 = color_map[3*1+2];
			if(YUV_RGB == RGB_mode)
			{
				color_temp1 = (Color_gg<<8) | Color_bb;
				color_temp2 = (Color_bb1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | Color_gg1;
			}
			else if(YUV_RGB == YUV_8BIT)
			{
				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
			Y2=(((((299*Color_rr1)+(587*Color_gg1)+(114*Color_bb1))/1000) )& 0xff);
			
				color_temp1 = (0<<8) | Cb1;
				color_temp2 = (Y1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | 0;
			}
			else
			{
				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
			Y2=(((((299*Color_rr1)+(587*Color_gg1)+(114*Color_bb1))/1000) )& 0xff);
			
				color_temp1 = (Y1<<8) | Cb1;
				color_temp2 = (Cr1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | Y2;
			}
			
			Color_rr2 = color_map[3*2+0];
			Color_gg2 = color_map[3*2+1];
			Color_bb2 = color_map[3*2+2];
			Color_rr3 = color_map[3*3+0];
			Color_gg3 = color_map[3*3+1];
			Color_bb3 = color_map[3*3+2];
			if(YUV_RGB == RGB_mode)
			{
				color_temp4 = (Color_gg2<<8) | Color_bb2;
				color_temp5 = (Color_bb3<<8) | Color_rr2;
				color_temp6 = (Color_rr3<<8) | Color_gg3;
			}
			else if(YUV_RGB == YUV_8BIT)
			{
//				Y1=(((((299*Color_rr2)+(587*Color_gg2)+(114*Color_bb2))/1000) )& 0xff);
//				Cb1=(((128000+(499*Color_bb2)-(331*Color_gg2)-(169*Color_rr2))/1000)& 0xff);
//				Cr1=(((128000+(499*Color_rr2)-(418*Color_gg2)-(81*Color_bb2))/1000)& 0xff);
			Y2=(((((299*Color_rr3)+(587*Color_gg3)+(114*Color_bb3))/1000) )& 0xff);
			
				color_temp4 = (0<<8) | Cr1;
				color_temp5 = (Y2<<8) | Color_rr;
				color_temp6 = (Color_rr1<<8) | 0;
			}
			else
			{
				Y1=(((((299*Color_rr2)+(587*Color_gg2)+(114*Color_bb2))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb2)-(331*Color_gg2)-(169*Color_rr2))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr2)-(418*Color_gg2)-(81*Color_bb2))/1000)& 0xff);
			Y2=(((((299*Color_rr3)+(587*Color_gg3)+(114*Color_bb3))/1000) )& 0xff);
			
				color_temp4 = (Y1<<8) | Cb1;
				color_temp5 = (Cr1<<8) | Color_rr;
				color_temp6 = (Color_rr1<<8) | Y2;
			}
		for(index_xdp = 0; index_xdp < XDP/4; index_xdp++)	//
		{
			
			
				LCD_DataWrite(color_temp1);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp2);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp3);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp4);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp5);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp6);		
				Check_Mem_WR_FIFO_not_Full();
		}
			Color_rr = color_map[3*4+0];
			Color_gg = color_map[3*4+1];
			Color_bb = color_map[3*4+2];
			Color_rr1 = color_map[3*5+0];
			Color_gg1 = color_map[3*5+1];
			Color_bb1 = color_map[3*5+2];
			if(YUV_RGB == RGB_mode)
			{
				color_temp1 = (Color_gg<<8) | Color_bb;
				color_temp2 = (Color_bb1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | Color_gg1;
			}
			else if(YUV_RGB == YUV_8BIT)
			{
				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
			Y2=(((((299*Color_rr1)+(587*Color_gg1)+(114*Color_bb1))/1000) )& 0xff);
			
				color_temp1 = (0<<8) | Cb1;
				color_temp2 = (Y1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | 0;
			}
			else
			{
				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
			Y2=(((((299*Color_rr1)+(587*Color_gg1)+(114*Color_bb1))/1000) )& 0xff);
			
				color_temp1 = (Y1<<8) | Cb1;
				color_temp2 = (Cr1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | Y2;
			}
			
			Color_rr2 = color_map[3*6+0];
			Color_gg2 = color_map[3*6+1];
			Color_bb2 = color_map[3*6+2];
			Color_rr3 = color_map[3*7+0];
			Color_gg3 = color_map[3*7+1];
			Color_bb3 = color_map[3*7+2];
			if(YUV_RGB == RGB_mode)
			{
				color_temp4 = (Color_gg2<<8) | Color_bb2;
				color_temp5 = (Color_bb3<<8) | Color_rr2;
				color_temp6 = (Color_rr3<<8) | Color_gg3;
			}
			else if(YUV_RGB == YUV_8BIT)
			{
//				Y1=(((((299*Color_rr2)+(587*Color_gg2)+(114*Color_bb2))/1000) )& 0xff);
//				Cb1=(((128000+(499*Color_bb2)-(331*Color_gg2)-(169*Color_rr2))/1000)& 0xff);
//				Cr1=(((128000+(499*Color_rr2)-(418*Color_gg2)-(81*Color_bb2))/1000)& 0xff);
			Y2=(((((299*Color_rr3)+(587*Color_gg3)+(114*Color_bb3))/1000) )& 0xff);
			
				color_temp4 = (0<<8) | Cr1;
				color_temp5 = (Y2<<8) | Color_rr;
				color_temp6 = (Color_rr1<<8) | 0;
			}
			else
			{
				Y1=(((((299*Color_rr2)+(587*Color_gg2)+(114*Color_bb2))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb2)-(331*Color_gg2)-(169*Color_rr2))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr2)-(418*Color_gg2)-(81*Color_bb2))/1000)& 0xff);
			Y2=(((((299*Color_rr3)+(587*Color_gg3)+(114*Color_bb3))/1000) )& 0xff);
			
				color_temp4 = (Y1<<8) | Cb1;
				color_temp5 = (Cr1<<8) | Color_rr;
				color_temp6 = (Color_rr1<<8) | Y2;
			}
		for(index_xdp = 0; index_xdp < XDP/4; index_xdp++)	//
		{
			
			
				LCD_DataWrite(color_temp1);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp2);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp3);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp4);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp5);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp6);		
				Check_Mem_WR_FIFO_not_Full();
		}

			Color_rr = color_map[3*8+0];
			Color_gg = color_map[3*8+1];
			Color_bb = color_map[3*8+2];
			Color_rr1 = color_map[3*9+0];
			Color_gg1 = color_map[3*9+1];
			Color_bb1 = color_map[3*9+2];
			if(YUV_RGB == RGB_mode)
			{
				color_temp1 = (Color_gg<<8) | Color_bb;
				color_temp2 = (Color_bb1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | Color_gg1;
			}
			else
			{
				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
			Y2=(((((299*Color_rr1)+(587*Color_gg1)+(114*Color_bb1))/1000) )& 0xff);
			
				color_temp1 = (Y1<<8) | Cb1;
				color_temp2 = (Cr1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | Y2;
			}
			
			Color_rr2 = color_map[3*10+0];
			Color_gg2 = color_map[3*10+1];
			Color_bb2 = color_map[3*10+2];
			Color_rr3 = color_map[3*11+0];
			Color_gg3 = color_map[3*11+1];
			Color_bb3 = color_map[3*11+2];
			if(YUV_RGB == RGB_mode)
			{
				color_temp4 = (Color_gg2<<8) | Color_bb2;
				color_temp5 = (Color_bb3<<8) | Color_rr2;
				color_temp6 = (Color_rr3<<8) | Color_gg3;
			}
			else
			{
				Y1=(((((299*Color_rr2)+(587*Color_gg2)+(114*Color_bb2))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb2)-(331*Color_gg2)-(169*Color_rr2))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr2)-(418*Color_gg2)-(81*Color_bb2))/1000)& 0xff);
			Y2=(((((299*Color_rr3)+(587*Color_gg3)+(114*Color_bb3))/1000) )& 0xff);
			
				color_temp4 = (Y1<<8) | Cb1;
				color_temp5 = (Cr1<<8) | Color_rr;
				color_temp6 = (Color_rr1<<8) | Y2;
			}
		for(index_xdp = 0; index_xdp < XDP/4; index_xdp++)	//
		{
			
				LCD_DataWrite(color_temp1);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp2);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp3);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp4);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp5);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp6);		
				Check_Mem_WR_FIFO_not_Full();
		}
			Color_rr = color_map[3*12+0];
			Color_gg = color_map[3*12+1];
			Color_bb = color_map[3*12+2];
			Color_rr1 = color_map[3*13+0];
			Color_gg1 = color_map[3*13+1];
			Color_bb1 = color_map[3*13+2];
			if(YUV_RGB == RGB_mode)
			{
				color_temp1 = (Color_gg<<8) | Color_bb;
				color_temp2 = (Color_bb1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | Color_gg1;
			}
			else
			{
				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
			Y2=(((((299*Color_rr1)+(587*Color_gg1)+(114*Color_bb1))/1000) )& 0xff);
			
				color_temp1 = (Y1<<8) | Cb1;
				color_temp2 = (Cr1<<8) | Color_rr;
				color_temp3 = (Color_rr1<<8) | Y2;
			}
			
			Color_rr2 = color_map[3*14+0];
			Color_gg2 = color_map[3*14+1];
			Color_bb2 = color_map[3*14+2];
			Color_rr3 = color_map[3*15+0];
			Color_gg3 = color_map[3*15+1];
			Color_bb3 = color_map[3*15+2];
			if(YUV_RGB == RGB_mode)
			{
				color_temp4 = (Color_gg2<<8) | Color_bb2;
				color_temp5 = (Color_bb3<<8) | Color_rr2;
				color_temp6 = (Color_rr3<<8) | Color_gg3;
			}
			else
			{
				Y1=(((((299*Color_rr2)+(587*Color_gg2)+(114*Color_bb2))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb2)-(331*Color_gg2)-(169*Color_rr2))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr2)-(418*Color_gg2)-(81*Color_bb2))/1000)& 0xff);
			Y2=(((((299*Color_rr3)+(587*Color_gg3)+(114*Color_bb3))/1000) )& 0xff);
			
				color_temp4 = (Y1<<8) | Cb1;
				color_temp5 = (Cr1<<8) | Color_rr;
				color_temp6 = (Color_rr1<<8) | Y2;
			}
		for(index_xdp = 0; index_xdp < XDP/4; index_xdp++)	//
		{
			
				LCD_DataWrite(color_temp1);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp2);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp3);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp4);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp5);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp6);		
				Check_Mem_WR_FIFO_not_Full();
		}
	}
}

void Dot_Flicker_B(void)
{
	u32 index = 0;
	u32 index_xdp = 0;
	u16 color_temp1,color_temp2,color_temp3;
	u8 Color_rr,Color_gg,Color_bb ;
	u8 Color_rr1 ,Color_gg1 ,Color_bb1 ;

	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	
	
	for(index = 0; index < YDP/4; index++)
	{
			Color_rr = 128;
			Color_gg =128;
			Color_bb = 128;
			Color_rr1 = 0;
			Color_gg1 =0;
			Color_bb1 = 0;
			color_temp1 = (Color_gg<<8) | Color_bb;
			color_temp2 = (Color_bb1<<8) | Color_rr;
			color_temp3 = (Color_rr1<<8) | Color_gg1;
		for(index_xdp = 0; index_xdp < XDP; index_xdp++)	//
		{
			
			
				LCD_DataWrite(color_temp1);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp2);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp3);		
				Check_Mem_WR_FIFO_not_Full();
		}
//		for(index_xdp = 0; index_xdp < XDP/2; index_xdp++)	//
//		{
//			
//			
//				LCD_DataWrite(color_temp1);		
//				Check_Mem_WR_FIFO_not_Full();
//				LCD_DataWrite(color_temp2);		
//				Check_Mem_WR_FIFO_not_Full();
//				LCD_DataWrite(color_temp3);		
//				Check_Mem_WR_FIFO_not_Full();
//		}
			Color_rr = 0;
			Color_gg =0;
			Color_bb = 0;
			Color_rr1 = 128;
			Color_gg1 =128;
			Color_bb1 = 128;
			color_temp1 = (Color_gg<<8) | Color_bb;
			color_temp2 = (Color_bb1<<8) | Color_rr;
			color_temp3 = (Color_rr1<<8) | Color_gg1;
		for(index_xdp = 0; index_xdp < XDP; index_xdp++)	//
		{
			
				LCD_DataWrite(color_temp1);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp2);		
				Check_Mem_WR_FIFO_not_Full();
				LCD_DataWrite(color_temp3);		
				Check_Mem_WR_FIFO_not_Full();
		}
//		for(index_xdp = 0; index_xdp < XDP/2; index_xdp++)	//
//		{
//			
//				LCD_DataWrite(color_temp1);		
//				Check_Mem_WR_FIFO_not_Full();
//				LCD_DataWrite(color_temp2);		
//				Check_Mem_WR_FIFO_not_Full();
//				LCD_DataWrite(color_temp3);		
//				Check_Mem_WR_FIFO_not_Full();
//		}
	}
 	// while(1);  
}


void Dot_Flicker_C(void)
{
//	u32 index = 0;
//	u32 index_xdp = 0;
//	u16 color_temp1,color_temp2,color_temp3;
//	u8 Color_rr,Color_gg,Color_bb ;
//	u8 Color_rr1 ,Color_gg1 ,Color_bb1 ;
//	u8 Y1,Cb1,Cr1;
//	

//	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
//	
//			Color_rr = 255;
//			Color_gg =0;
//			Color_bb = 0;
//			Color_rr1 = 255;
//			Color_gg1 =0;
//			Color_bb1 = 0;
////		{
//////		
//			{
//				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
//				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
//				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
//			}
//			color_temp1 = (Y1<<8) | Cb1;
//			color_temp2 = (Cr1<<8) | Color_rr;
//			color_temp3 = (Color_rr1<<8) | Y1;
//			
//	for(index = 0; index < YDP/1; index++)
//	{
////			Color_rr = 0X00;
////			Color_gg =0X00;
////			Color_bb = 0X80;
////			Color_rr1 = 0X00;
////			Color_gg1 =0X00;
////			Color_bb1 = 0X80;
//////		{
////////		
////			{
////				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
////				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
////				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
////			}
////			color_temp1 = (Y1<<8) | Cb1;
////			color_temp2 = (Cr1<<8) | Color_rr;
////			color_temp3 = (Color_rr1<<8) | Y1;
////			Color_rr = 0;
//////		Color_rr = (Color_rr&0x80)>>7 
//////		+ (Color_rr&0x40)>>5
//////		+(Color_rr&0x20)>>3
//////		+(Color_rr&0x10)>>1
//////		+(Color_rr&0x01)<<7
//////		+(Color_rr&0x02)<<5
//////		+(Color_rr&0x04)<<3
//////		+(Color_rr&0x08)<<1;
////			Color_gg =0X95;
//////			Color_gg = (Color_gg&0x80)>>7 
//////		+ (Color_gg&0x40)>>5
//////		+(Color_gg&0x20)>>3
//////		+(Color_gg&0x10)>>1
//////		+(Color_gg&0x01)<<7
//////		+(Color_gg&0x02)<<5
//////		+(Color_gg&0x04)<<3
//////		+(Color_gg&0x08)<<1;
////			Color_bb = 0X2B;
//////			Color_bb = (Color_bb&0x80)>>7 
//////		+ (Color_bb&0x40)>>5
//////		+(Color_bb&0x20)>>3
//////		+(Color_bb&0x10)>>1
//////		+(Color_bb&0x01)<<7
//////		+(Color_bb&0x02)<<5
//////		+(Color_bb&0x04)<<3
//////		+(Color_bb&0x08)<<1;
////			Color_rr1 = 0;
//////			Color_rr1 = (Color_rr1&0x80)>>7 
//////		+ (Color_rr1&0x40)>>5
//////		+(Color_rr1&0x20)>>3
//////		+(Color_rr1&0x10)>>1
//////		+(Color_rr1&0x01)<<7
//////		+(Color_rr1&0x02)<<5
//////		+(Color_rr1&0x04)<<3
//////		+(Color_rr1&0x08)<<1;
////			Color_gg1 =0X95;
//////			Color_gg1 = (Color_gg1&0x80)>>7 
//////		+ (Color_gg1&0x40)>>5
//////		+(Color_gg1&0x20)>>3
//////		+(Color_gg1&0x10)>>1
//////		+(Color_gg1&0x01)<<7
//////		+(Color_gg1&0x02)<<5
//////		+(Color_gg1&0x04)<<3
//////		+(Color_gg1&0x08)<<1;
////			Color_bb1 = 0X2B;
//////			Color_bb1 = 0X15;
//////			Color_bb1 = (Color_bb1&0x80)>>7 
//////		+ (Color_bb1&0x40)>>5
//////		+(Color_bb1&0x20)>>3
//////		+(Color_bb1&0x10)>>1
//////		+(Color_bb1&0x01)<<7
//////		+(Color_bb1&0x02)<<5
//////		+(Color_bb1&0x04)<<3
//////		+(Color_bb1&0x08)<<1;
//////		}
////			color_temp1 = (Color_gg<<8) | Color_bb;
////			color_temp2 = (Color_bb1<<8) | Color_rr;
////			color_temp3 = (Color_rr1<<8) | Color_gg1;
////		for(index_xdp = 0; index_xdp < XDP; index_xdp++)	//
////		{
////			
////			
////				LCD_DataWrite(color_temp1);		
////				Check_Mem_WR_FIFO_not_Full();
////				LCD_DataWrite(color_temp2);		
////				Check_Mem_WR_FIFO_not_Full();
////				LCD_DataWrite(color_temp3);		
////				Check_Mem_WR_FIFO_not_Full();
////		}
//		for(index_xdp = 0; index_xdp < XDP/2; index_xdp++)	//
//		{
//			
//			
//				LCD_DataWrite(color_temp1);		
//				Check_Mem_WR_FIFO_not_Full();
//				LCD_DataWrite(color_temp2);		
//				Check_Mem_WR_FIFO_not_Full();
//				LCD_DataWrite(color_temp3);		
//				Check_Mem_WR_FIFO_not_Full();
//		}
////			Color_rr = 0X00;
////			Color_gg =0X00;
////			Color_bb = 0X80;
////			Color_rr1 = 0X00;
////			Color_gg1 =0X00;
////			Color_bb1 = 0X80;
//////		{
////////			{
////////				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
////////				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
////////				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
////////			}
////////			color_temp1 = (Y1<<8) | Cb1;
////////			color_temp2 = (Cr1<<8) | Color_rr;
////////			color_temp3 = (Color_rr1<<8) | Y1;
//////			Color_rr = 0;
////////		Color_rr = (Color_rr&0x80)>>7 
////////		+ (Color_rr&0x40)>>5
////////		+(Color_rr&0x20)>>3
////////		+(Color_rr&0x10)>>1
////////		+(Color_rr&0x01)<<7
////////		+(Color_rr&0x02)<<5
////////		+(Color_rr&0x04)<<3
////////		+(Color_rr&0x08)<<1;
//////			Color_gg =0X95;
////////			Color_gg = (Color_gg&0x80)>>7 
////////		+ (Color_gg&0x40)>>5
////////		+(Color_gg&0x20)>>3
////////		+(Color_gg&0x10)>>1
////////		+(Color_gg&0x01)<<7
////////		+(Color_gg&0x02)<<5
////////		+(Color_gg&0x04)<<3
////////		+(Color_gg&0x08)<<1;
//////			Color_bb = 0X2B;
////////			Color_bb = (Color_bb&0x80)>>7 
////////		+ (Color_bb&0x40)>>5
////////		+(Color_bb&0x20)>>3
////////		+(Color_bb&0x10)>>1
////////		+(Color_bb&0x01)<<7
////////		+(Color_bb&0x02)<<5
////////		+(Color_bb&0x04)<<3
////////		+(Color_bb&0x08)<<1;
//////			Color_rr1 = 0X15;
////////			Color_rr1 = (Color_rr1&0x80)>>7 
////////		+ (Color_rr1&0x40)>>5
////////		+(Color_rr1&0x20)>>3
////////		+(Color_rr1&0x10)>>1
////////		+(Color_rr1&0x01)<<7
////////		+(Color_rr1&0x02)<<5
////////		+(Color_rr1&0x04)<<3
////////		+(Color_rr1&0x08)<<1;
//////			Color_gg1 =0X95;
////////			Color_gg1 = (Color_gg1&0x80)>>7 
////////		+ (Color_gg1&0x40)>>5
////////		+(Color_gg1&0x20)>>3
////////		+(Color_gg1&0x10)>>1
////////		+(Color_gg1&0x01)<<7
////////		+(Color_gg1&0x02)<<5
////////		+(Color_gg1&0x04)<<3
////////		+(Color_gg1&0x08)<<1;
//////			Color_bb1 = 0X15;
////////			Color_bb1 = (Color_bb1&0x80)>>7 
////////		+ (Color_bb1&0x40)>>5
////////		+(Color_bb1&0x20)>>3
////////		+(Color_bb1&0x10)>>1
////////		+(Color_bb1&0x01)<<7
////////		+(Color_bb1&0x02)<<5
////////		+(Color_bb1&0x04)<<3
////////		+(Color_bb1&0x08)<<1;
//////		}
////			color_temp1 = (Color_gg<<8) | Color_bb;
////			color_temp2 = (Color_bb1<<8) | Color_rr;
////			color_temp3 = (Color_rr1<<8) | Color_gg1;
//////		for(index_xdp = 0; index_xdp < XDP; index_xdp++)	//
//////		{
//////			
//////				LCD_DataWrite(color_temp1);		
//////				Check_Mem_WR_FIFO_not_Full();
//////				LCD_DataWrite(color_temp2);		
//////				Check_Mem_WR_FIFO_not_Full();
//////				LCD_DataWrite(color_temp3);		
//////				Check_Mem_WR_FIFO_not_Full();
//////		}
////		for(index_xdp = 0; index_xdp < XDP/2; index_xdp++)	//
////		{
////			
////				LCD_DataWrite(color_temp1);		
////				Check_Mem_WR_FIFO_not_Full();
////				LCD_DataWrite(color_temp2);		
////				Check_Mem_WR_FIFO_not_Full();
////				LCD_DataWrite(color_temp3);		
////				Check_Mem_WR_FIFO_not_Full();
////		}
////	}


//	
////	for(index_xdp=0;index_xdp<color_bit;index_xdp++)
////	{	
////			
////		Color_rr = (color&0x00ff0000)>>16;
////		Color_gg = (color&0x0000ff00)>>8;
////		Color_bb = (color&0x000000ff)>>0;
////		Color_rr1 = (color&0x00ff0000)>>16;
////		Color_gg1 = (color&0x0000ff00)>>8;
////		Color_bb1 = (color&0x000000ff)>>0;
////			color_temp1 = (Color_gg<<8) | Color_bb;
////			color_temp2 = (Color_bb1<<8) | Color_rr;
////			color_temp3 = (Color_rr1<<8) | Color_gg1;
////		for(index = 0; index < (XDP*YDP/color_bit/2); index++)
////		  {
////				LCD_DataWrite(color_temp1);		
////				Check_Mem_WR_FIFO_not_Full();
////				LCD_DataWrite(color_temp2);		
////				Check_Mem_WR_FIFO_not_Full();
////				LCD_DataWrite(color_temp3);		
////				Check_Mem_WR_FIFO_not_Full();
////		  }
////		  color <<= 1 ;
//	}

}

void Dot_Flicker_z(void)
{
//	int i;
//	
//	for(i=0;i<(XDP)/2;i++)
//	{
//		LCD_Draw_Line(i*2+0,0,2*i+0,YDP-1,128,0,128);
//		LCD_Draw_Line(2*i+1,0,2*i+1,YDP-1,0,128,0);
//	}
}

void LCD_Draw_Line(u16 resx1,u16 resy1,u16 resx2,u16 resy2,u8 Color_rr,u8 Color_gg,u8 Color_bb)
{
//	u16 ctp;

//	if ((TIMING_SET[1]>750)|(TIMING_SET[2]>1280))
//		{	ctp=((Color_rr<<8)&0xf800)+((Color_gg<<3)&0x07e0)+((Color_bb>>3)&0x001f);	Foreground_color_65k(ctp);	}
//	else
	switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			Color_rr = Color_rr;
			Color_gg = Color_gg;
			Color_bb = Color_bb;
			break;
		
		case LVDS_6BIT:
			Color_rr = Color_rr&0x3F;
			Color_gg = Color_gg&0x3F;
			Color_bb = Color_bb&0x3F;
			break;
		
		default :
			Color_rr = Color_rr;
			Color_gg = Color_gg;
			Color_bb = Color_bb;
			break;
	}
	
	{		Foreground_color_16M((Color_rr<<16)+(Color_gg<<8)+Color_bb);	}

	Line_Start_XY(resx1,resy1);
	Line_End_XY(resx2,resy2);

	Start_Line();
}
/*******************************************************************************
* Function Name  : void LCD_WriteArea(u16 add_sx, u16 add_ex, u16 add_sy, u16 add_ey, u8 Color_rr,u8 Color_gg,u8 Color_bb)
* Description    : Clears the hole LCD.
* Input          : Color: the color of the background.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteArea(u16 add_sx, u16 add_ex, u16 add_sy, u16 add_ey, u8 Color_rr,u8 Color_gg,u8 Color_bb)
{
//  u32 index = 0;
	u32 i=0;

	u8 Y1,Cr1,Cb1;
//if ((TIMING_SET[1]*TIMING_SET[2]*3)>(16777216-1920*1200*3*2))
//	{
////     ctp=((Color_rr<<8)&0xf800)+((Color_gg<<5)&0x07e0)+(Color_bb&0x001f);
////		   Select_Main_Window_16bpp();Memory_16bpp_Mode();
////        Foreground_color_65k(ctp);
//        ctp=((Color_rr)&0xe0)+((Color_gg>>3)&0x1c)+((Color_bb>>6)&0x03);
//        ctp |=(ctp<<8); 
//        Select_Main_Window_8bpp();Memory_8bpp_Mode();
//        Foreground_color_256(ctp);
//		
//		
//	    Line_Start_XY(add_sx,add_sy);
//		  Line_End_XY(add_ex,add_ey);  
//	    Start_Square_Fill();
//      // Select_Main_Window_24bpp();Memory_24bpp_Mode();
//   }
//	else if((TIMING_SET[1]*TIMING_SET[2]*3)>(16777216-1920*1200*3*2))
	
//	if ((TIMING_SET[1]*TIMING_SET[2]*3)>(16777216-1920*1200*3*1))
//	{
//		ctp=(((Color_rr)&0xf8)<<8)+(((Color_gg>>2))<<5)+((Color_bb>>3));
////        ctp |=(ctp<<8); 
//        Select_Main_Window_16bpp();Select_Main_Window_16bpp();
//        Foreground_color_65k((((Color_rr)>>3)<<8)+(((Color_gg>>2))<<5)+((Color_bb>>3)));
//		
//		
//	    Line_Start_XY(add_sx,add_sy);
//		  Line_End_XY(add_ex,add_ey);  
//	    Start_Square_Fill();
//	}

//else
	{   
    Select_Main_Window_24bpp();Memory_24bpp_Mode();
	
	switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			Color_rr = Color_rr;
			Color_gg = Color_gg;
			Color_bb = Color_bb;
			break;
		
		case LVDS_6BIT:
			Color_rr = (Color_rr>>2)&0x3F;
			Color_gg = (Color_gg>>2)&0x3F;
			Color_bb = (Color_bb>>2)&0x3F;
			break;
		
		default :
			Color_rr = Color_rr;
			Color_gg = Color_gg;
			Color_bb = Color_bb;
			break;
	}
//    Foreground_color_16M((Color_rr<<16)+(Color_gg<<8)+Color_bb);
	
//			Color_rr = 255;
//			Color_gg =0;
//			Color_bb = 0;
//			Color_rr1 = 255;
//			Color_gg1 =0;
//			Color_bb1 = 0;
////		{
//////		
		if(YUV_RGB == YUV_16BIT)
			{	
				{
				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
				}
			
				for(i=0;i<(add_ex-add_sx)/2+1;i++)
				{
					LCD_Draw_Line(i*2+0+add_sx,add_sy,2*i+0+add_sx,add_ey,0,Y1,Cb1);
					LCD_Draw_Line(2*i+1+add_sx,add_sy,2*i+1+add_sx,add_ey,0,Y1,Cr1);
				}
			}
		else if(YUV_RGB == YUV_8BIT)
			{	
				{
				Y1=(((((299*Color_rr)+(587*Color_gg)+(114*Color_bb))/1000) )& 0xff);
				Cb1=(((128000+(499*Color_bb)-(331*Color_gg)-(169*Color_rr))/1000)& 0xff);
				Cr1=(((128000+(499*Color_rr)-(418*Color_gg)-(81*Color_bb))/1000)& 0xff);
				}
			
				for(i=0;i<(add_ex-add_sx+1)/4;i++)
				{
					LCD_Draw_Line(i*4+0+add_sx,add_sy,4*i+0+add_sx,add_ey,0,0,Cb1);
					LCD_Draw_Line(4*i+1+add_sx,add_sy,4*i+1+add_sx,add_ey,0,0,Y1);
					LCD_Draw_Line(4*i+2+add_sx,add_sy,4*i+2+add_sx,add_ey,0,0,Cr1);
					LCD_Draw_Line(4*i+3+add_sx,add_sy,4*i+3+add_sx,add_ey,0,0,Y1);
				}
			}
		else
		{
		Foreground_color_16M((Color_rr<<16)+(Color_gg<<8)+Color_bb);
	    Line_Start_XY(add_sx,add_sy);
		Line_End_XY(add_ex,add_ey);
  
	    Start_Square_Fill();
		}
     }
}



//void colorbar_H(void)
//{
//	char i,j;
//	char XDP_X,YDP_Y;
//	char color_rr=255;
//	
//	YDP_Y = 6;  //////±íÊ¾¶àÉÙÐÐ
//	XDP_X = 6;	//////±íÊ¾¶àÉÙÁÐ
//	
//	for(j=0;j<YDP/4;j++)
//	{
//		color_rr=255;
//		for(i=0;i<XDP;i++)
//		{
//			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255,0,YDP-1,color_rr,0,0); 
//			color_rr = color_rr - 255*i/XDP;
//		}
//	}
//	for(j=0;j<YDP/4;j++)
//	{
//		color_rr=255;
//		for(i=0;i<XDP;i++)
//		{
//			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255,0,YDP-1,0,color_rr,0); 
//			color_rr = color_rr - 255*i/XDP;
//		}
//	}
//	for(j=0;j<YDP/4;j++)
//	{
//		color_rr=255;
//		for(i=0;i<XDP;i++)
//		{
//			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255,0,YDP-1,0,0,color_rr); 
//			color_rr = color_rr - 255*i/XDP;
//		}
//	}
//	for(j=0;j<YDP/4;j++)
//	{
//		color_rr=255;
//		for(i=0;i<XDP;i++)
//		{
//			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255,0,YDP-1,color_rr,color_rr,color_rr); 
//			color_rr = color_rr - 255*i/XDP;
//		}
//	}
//	
//}


void colorbar_H(void)
{
	int i;
	char color_rr=255;
	char color_gray_temp = 0;
	
	switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			color_gray_temp = color_rr;
			break;
		
//		case LVDS_6BIT:
//			color_gray_temp = color_rr&0x3F;
//			break;
		
		default :
			color_gray_temp = color_rr;
			break;
	}
	

		color_rr=color_gray_temp;
		for(i=0;i<color_gray_temp;i++)
		{
			LCD_WriteArea(XDP*i/color_gray_temp,XDP*(i+1)/color_gray_temp-1,0,YDP/4-1,color_rr,0,0); 
			color_rr -= 1;
		}
		color_rr=color_gray_temp;
		for(i=0;i<color_gray_temp;i++)
		{
			LCD_WriteArea(XDP*i/color_gray_temp,XDP*(i+1)/color_gray_temp-1,YDP/4,YDP*2/4-1,0,color_rr,0); 
			color_rr -= 1;
//			color_rr = 255*(XDP-i)/XDP;
		}
		color_rr=color_gray_temp;
		for(i=0;i<color_gray_temp;i++)
		{
			LCD_WriteArea(XDP*i/color_gray_temp,XDP*(i+1)/color_gray_temp-1,YDP*2/4,YDP*3/4-1,0,0,color_rr); 
			color_rr -= 1;
		}

		color_rr=color_gray_temp;
		for(i=0;i<color_gray_temp;i++)
		{
			LCD_WriteArea(XDP*i/color_gray_temp,XDP*(i+1)/color_gray_temp-1,YDP*3/4,YDP*4/4-1,color_rr,color_rr,color_rr); 
			color_rr -= 1;
		}

}

void colorbar_V(void)
{
	int i;
	char color_rr=255;
	char color_gray_temp = 0;
	
	switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			color_gray_temp = color_rr;
			break;
		
//		case LVDS_6BIT:
//			color_gray_temp = color_rr&0x3F;
//			break;
		
		default :
			color_gray_temp = color_rr;
			break;
	}
	color_rr=color_gray_temp;
		for(i=0;i<color_gray_temp;i++)
		{
//			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255-1,0,YDP/4-1,color_rr,0,0); 
			LCD_WriteArea(XDP*0/4,XDP*1/4-1,YDP*i/color_gray_temp,YDP*(i+1)/color_gray_temp-1,color_rr,0,0); 
			color_rr -= color_gray_temp/color_gray_temp;
		}
		
//		color_gray_temp = 0x3F;
//		color_rr=color_gray_temp;
//		for(i=0;i<color_gray_temp;i++)
//		{
////			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255-1,0,YDP/4-1,color_rr,0,0); 
//			LCD_WriteArea(XDP*1/8,XDP*2/8-1,YDP*i/color_gray_temp,YDP*(i+1)/color_grey_num-1,color_rr,0,0); 
//			color_rr -= color_gray_temp/color_gray_temp;
//		}
//	color_rr=color_grey_num;
//		for(i=0;i<color_grey_num;i++)
//		{
////			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255-1,0,YDP/4-1,color_rr,0,0); 
//			LCD_WriteArea(XDP*0/4,XDP*1/4-1,YDP*i/color_grey_num,YDP*(i+1)/color_grey_num-1,color_rr,0,0); 
//			color_rr -= color_grey_num/color_grey_num;
//		}

		color_rr=color_gray_temp;
		for(i=0;i<color_gray_temp;i++)
		{
			LCD_WriteArea(XDP*1/4,XDP*2/4-1,YDP*i/color_gray_temp,YDP*(i+1)/color_gray_temp-1,0,color_rr,0); 
			color_rr -= 1;
		}
		color_rr=color_gray_temp;
		for(i=0;i<color_gray_temp;i++)
		{
			LCD_WriteArea(XDP*2/4,XDP*3/4-1,YDP*i/color_gray_temp,YDP*(i+1)/color_gray_temp-1,0,0,color_rr); 
			color_rr -= 1;
		}
		color_rr=color_gray_temp;
		for(i=0;i<color_gray_temp;i++)
		{
			LCD_WriteArea(XDP*3/4,XDP*4/4-1,YDP*i/color_gray_temp,YDP*(i+1)/color_gray_temp-1,color_rr,color_rr,color_rr); 
			color_rr -= 1;
		}
		
}

void RED_H(void)
{
	int i;
	char color_rr=255;

		color_rr=255;
		for(i=0;i<255;i++)
		{
			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255-1,YDP*0/4,YDP*4/4-1,color_rr,0,0); 
			color_rr -= 1;
		}

}
void RED_V(void)
{
	int i;
	char color_rr=255;
	
		color_rr=255;
		for(i=0;i<255;i++)
		{
			LCD_WriteArea(XDP*0/4,XDP*4/4-1,YDP*i/255,YDP*(i+1)/255-1,color_rr,0,0); 
			color_rr -= 1;
		}

}
void GREEN_H(void)
{
	int i;
	char color_rr=255;

		color_rr=255;
		for(i=0;i<255;i++)
		{
			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255-1,YDP*0/4,YDP*4/4-1,0,color_rr,0); 
			color_rr -= 1;
		}

}
void GREEN_V(void)
{
	int i;
	char color_rr=255;
	
		color_rr=255;
		for(i=0;i<255;i++)
		{
			LCD_WriteArea(XDP*0/4,XDP*4/4-1,YDP*i/255,YDP*(i+1)/255-1,0,color_rr,0); 
			color_rr -= 1;
		}

}

void BLUE_H(void)
{
	int i;
	char color_rr=255;

		color_rr=255;
		for(i=0;i<255;i++)
		{
			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255-1,YDP*0/4,YDP*4/4-1,0,0,color_rr); 
			color_rr -= 1;
		}

}

void BLUE_V(void)
{
	int i;
	char color_rr=255;
	
		color_rr=255;
		for(i=0;i<255;i++)
		{
			LCD_WriteArea(XDP*0/4,XDP*4/4-1,YDP*i/255,YDP*(i+1)/255-1,0,0,color_rr); 
			color_rr -= 1;
		}

}

void GRAY_H(void)
{
	int i;
	char color_rr=255;

		color_rr=255;
		for(i=0;i<255;i++)
		{
			LCD_WriteArea(XDP*i/255,XDP*(i+1)/255-1,YDP*0/4,YDP*4/4-1,color_rr,color_rr,color_rr); 
			color_rr -= 1;
		}

}


void GRAY_V(void)
{
	int i;
	char color_rr=255;
	
		color_rr=255;
		for(i=0;i<255;i++)
		{
			LCD_WriteArea(XDP*0/4,XDP*4/4-1,YDP*i/255,YDP*(i+1)/255-1,color_rr,color_rr,color_rr); 
			color_rr -= 1;
		}
		
}

void checkerboard(void)
{
	char i,j;
	char XDP_X,YDP_Y;
	char color_rr=255;
	
	YDP_Y = 8;  //////±íÊ¾¶àÉÙÐÐ
	XDP_X = 8;	//////±íÊ¾¶àÉÙÁÐ
	
	for(j=0;j<YDP_Y;j++)
	{
		for(i=0;i<XDP_X;i++)
		{
			LCD_WriteArea(XDP*i/XDP_X,XDP*(i+1)/XDP_X-1,YDP*j/YDP_Y,YDP*(j+1)/YDP_Y-1,color_rr,color_rr,color_rr); 
			color_rr = ~color_rr;
		}
		color_rr = ~color_rr;
	}
}

void Flicker_H_column_INVER(void)
{
	int i;
	
	for(i=0;i<(XDP)/2;i++)
	{
		LCD_Draw_Line(i*2+0,0,2*i+0,YDP-1,128,128,128);
		LCD_Draw_Line(2*i+1,0,2*i+1,YDP-1,0,0,0);
	}
}

void Flicker_H_DOT_INVER(void)
{
	int i;
	
	for(i=0;i<(XDP)/2;i++)
	{
		LCD_Draw_Line(i*2+0,0,2*i+0,YDP-1,128,0,128);
		LCD_Draw_Line(2*i+1,0,2*i+1,YDP-1,0,128,0);
	}
}

void Flicker_V_column(void)
{
	int i;
	
	for(i=0;i<(YDP)/2;i++)
	{
		LCD_Draw_Line(0,i*2+0,XDP-1,2*i+0,128,128,128);
		LCD_Draw_Line(0,2*i+1,XDP-1,2*i+1,0,0,0);
	}
}



void Display_bit_color_H(void)
{
    int i;
    u32 Color_bit=1;
    u32 COLOR_TEMP=0;
    char color_bit_set=24;
    
    switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			color_bit_set = 24;
			break;
		
		case LVDS_6BIT:
			color_bit_set = 18;
			break;
		
		default :
			color_bit_set = 24;
			break;
	}
    
    COLOR_TEMP = Color_bit;
		for(i=0;i<color_bit_set;i++)
		{
            switch(LVDS_8BIT_USE)
            {
                case LVDS_8BIT:
                    COLOR_TEMP = Color_bit<<i;
                    break;
                
                case LVDS_6BIT:
                    if(i<=6*1)
                    {
                        COLOR_TEMP = Color_bit<<(i+2);
                    }
                    else if(i<=6*2)
                    {
                        COLOR_TEMP = Color_bit<<(i+4);
                    }
                    else
                    {
                        COLOR_TEMP = Color_bit<<(i+6);
                    }
                    break;
                
                default:
                COLOR_TEMP = Color_bit<<1;
                break;
            }
            LCD_WriteArea(XDP*i/color_bit_set,XDP*(i+1)/color_bit_set,0,YDP-1,((COLOR_TEMP&0xFF0000)>>16),((COLOR_TEMP&0x00FF00)>>8),(COLOR_TEMP&0x0000FF)); 
		}
}

void Display_bit_color_V(void)
{
    int i;
    u32 Color_bit=1;
    u32 COLOR_TEMP=0;
    char color_bit_set=24;
    
    switch(LVDS_8BIT_USE)
	{
		case LVDS_8BIT:
			color_bit_set = 24;
			break;
		
		case LVDS_6BIT:
			color_bit_set = 18;
			break;
		
		default :
			color_bit_set = 24;
			break;
	}
    
    COLOR_TEMP = Color_bit;
		for(i=0;i<color_bit_set;i++)
		{
            switch(LVDS_8BIT_USE)
            {
                case LVDS_8BIT:
                    COLOR_TEMP = Color_bit<<i;
                    break;
                
                case LVDS_6BIT:
                    if(i<=6*1)
                    {
                        COLOR_TEMP = Color_bit<<(i+2);
                    }
                    else if(i<=6*2)
                    {
                        COLOR_TEMP = Color_bit<<(i+4);
                    }
                    else
                    {
                        COLOR_TEMP = Color_bit<<(i+6);
                    }
                    break;
                
                default:
                COLOR_TEMP = Color_bit<<1;
                break;
            }
            LCD_WriteArea(0,XDP,YDP*i/color_bit_set,YDP*(i+1)/color_bit_set,((COLOR_TEMP&0xFF0000)>>16),((COLOR_TEMP&0x00FF00)>>8),(COLOR_TEMP&0x0000FF));
		}
}
/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : Sets the cursor position.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position. 
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
    LCD_CmdWrite(0x5F);
	LCD_DataWrite(Xpos);  
    LCD_CmdWrite(0x60);	   
	LCD_DataWrite(Xpos>>8);
    LCD_CmdWrite(0x61);
	LCD_DataWrite(Ypos);
    LCD_CmdWrite(0x62);	   
	LCD_DataWrite(Ypos>>8);
}	

		 
void LCD_SetPoint_A(u16 x,u16 y,u16 point)
{
    u16 color_temp1,color_temp2,color_temp3;
    
//        if ((TIMING_SET[1]>750)|(TIMING_SET[2]>1280))
//        {
//            LCD_DataWrite(point);		
//            Check_Mem_WR_FIFO_not_Full();
//            LCD_DataWrite(point);		
//            Check_Mem_WR_FIFO_not_Full();
////            LCD_DataWrite(point);		
////            Check_Mem_WR_FIFO_not_Full();
////            LCD_DataWrite(point);		
////            Check_Mem_WR_FIFO_not_Full();
//        }
//        else
        {
            color_temp1 = ((point&0x07E0)<<5) | ((point&0x001F)<<3);
            color_temp2 = (((point&0x001F)<<3)<<8) | ((point&0xF800)>>8);
            color_temp3 = ((point&0xF800)) | ((point&0x07E0)>>3);

            //    LCD_SetCursor(x,y);	  	//ÐÐÁÐµØÖ·
            //    LCD_WriteRAM_Prepare();	//Ð´ÃüÁî
            //  LCD_DataWrite(point);		//Ð´Êý¾Ý

            LCD_DataWrite(color_temp1);		
            Check_Mem_WR_FIFO_not_Full();
            LCD_DataWrite(color_temp2);		
            Check_Mem_WR_FIFO_not_Full();
            LCD_DataWrite(color_temp3);		
            Check_Mem_WR_FIFO_not_Full();
        }
        
        
}

void LCD_SetPoint(u16 x,u16 y,u16 point)
{

    
    LCD_SetCursor(x,y);	  	//ÐÐÁÐµØÖ·
    LCD_WriteRAM_Prepare();	//Ð´ÃüÁî
  LCD_DataWrite(point);		//Ð´Êý¾Ý

}

void LCD_PutChar_A(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u16 tmp_char=0;
    
//    LCD_Set_DisplayWindow(x, x+32*2-1,y, y+24*2-1)	; 
	LCD_Set_DisplayWindow_C(3,x, x+32*2-1,y, y+24*2-1)	; 
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    
  	for (i=0;i<24;i++)
  	{
    	tmp_char=ASCII_Table[((c-0x20)*24)+i];	 //¼õÈ¥32µÄÆ«ÒÆ,ÊÇÒòÎª×Ö·û±í´Ó¿Õ¸ñ¿ªÊ¼µÄ, ²Î¿¼×Ö·û±íÄÇµÄ×¢ÊÍ
    	for (j=0;j<16;j++)
    	{
			//if ( (tmp_char >> 15-j) & 0x01 == 0x01)	//°´ÕÕÉÏÃæµÄÏÔÊ¾,×Ö·ûÊÇµ¹¹ýÀ´µÄ,µ½ÕâÀï¸Ä¹ýÀ´¾ÍÐÐÁË
			if ( (tmp_char ) & 0x0001 )
      		{
        		LCD_SetPoint_A(x+j,y+i,charColor); //×Ö·ûÑÕÉ«
      		}
      		else
      		{
        		LCD_SetPoint_A(x+j,y+i,bkColor); //±³¾°ÑÕÉ«
      		}
            tmp_char >>= 1;
    	}
        
        tmp_char=ASCII_Table[((c-0x20)*24)+i];	 //¼õÈ¥32µÄÆ«ÒÆ,ÊÇÒòÎª×Ö·û±í´Ó¿Õ¸ñ¿ªÊ¼µÄ, ²Î¿¼×Ö·û±íÄÇµÄ×¢ÊÍ
    	for (j=0;j<16;j++)
    	{
			//if ( (tmp_char >> 15-j) & 0x01 == 0x01)	//°´ÕÕÉÏÃæµÄÏÔÊ¾,×Ö·ûÊÇµ¹¹ýÀ´µÄ,µ½ÕâÀï¸Ä¹ýÀ´¾ÍÐÐÁË
			if ( (tmp_char ) & 1 )
      		{
        		LCD_SetPoint_A(x+j,y+i,charColor); //×Ö·ûÑÕÉ«
      		}
      		else
      		{
        		LCD_SetPoint_A(x+j,y+i,bkColor); //±³¾°ÑÕÉ«
      		}
            tmp_char >>= 1;
    	}
  	}
}

void LCD_PutChar_B(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u16 tmp_char=0;

  	for (i=0;i<24;i++)
  	{
    	tmp_char=ASCII_Table[((c-0x20)*24)+i];	 //¼õÈ¥32µÄÆ«ÒÆ,ÊÇÒòÎª×Ö·û±í´Ó¿Õ¸ñ¿ªÊ¼µÄ, ²Î¿¼×Ö·û±íÄÇµÄ×¢ÊÍ
    	for (j=0;j<32;j++)
    	{
			//if ( (tmp_char >> 15-j) & 0x01 == 0x01)	//°´ÕÕÉÏÃæµÄÏÔÊ¾,×Ö·ûÊÇµ¹¹ýÀ´µÄ,µ½ÕâÀï¸Ä¹ýÀ´¾ÍÐÐÁË
			if ( (tmp_char >> j) & 0x01 == 0x01)
      		{
        		LCD_SetPoint(x+j,y+i,charColor); //×Ö·ûÑÕÉ«
                LCD_SetPoint(x+j,y+i,charColor); //×Ö·ûÑÕÉ«
      		}
      		else
      		{
        		LCD_SetPoint(x+j,y+i,bkColor); //±³¾°ÑÕÉ«
                LCD_SetPoint(x+j,y+i,bkColor); //±³¾°ÑÕÉ«
      		}
    	}
  	}
}

void LCD_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor)
{
  u16 i=0;
  u16 j=0;
  
  u16 tmp_char=0;

  	for (i=0;i<24;i++)
  	{
    	tmp_char=ASCII_Table[((c-0x20)*24)+i];	 //¼õÈ¥32µÄÆ«ÒÆ,ÊÇÒòÎª×Ö·û±í´Ó¿Õ¸ñ¿ªÊ¼µÄ, ²Î¿¼×Ö·û±íÄÇµÄ×¢ÊÍ
    	for (j=0;j<16;j++)
    	{
			//if ( (tmp_char >> 15-j) & 0x01 == 0x01)	//°´ÕÕÉÏÃæµÄÏÔÊ¾,×Ö·ûÊÇµ¹¹ýÀ´µÄ,µ½ÕâÀï¸Ä¹ýÀ´¾ÍÐÐÁË
			if ( (tmp_char >> j) & 0x01 == 0x01)
      		{
        		LCD_SetPoint(x+j,y+i,charColor); //×Ö·ûÑÕÉ«
      		}
      		else
      		{
        		LCD_SetPoint(x+j,y+i,bkColor); //±³¾°ÑÕÉ«
      		}
    	}
  	}
}


void LCD_DisplayString(u16 X,u16 Y, char *ptr, u16 charColor, u16 bkColor)
{
  u32 i = 0;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 50))
  {
    /* Display one character on LCD */
	LCD_PutChar(X, Y, *ptr, charColor, bkColor);
    /* Decrement the column position by 16 */
    X += 16;
    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    i++;
  }
}

void LCD_DisplayString_A(u16 X,u16 Y, char *ptr, u16 charColor, u16 bkColor)
{
  u32 i = 0;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 50))
  {
    /* Display one character on LCD */
      if ((TIMING_SET[1]>750)|(TIMING_SET[2]>1280)){    LCD_PutChar_B(X, Y, *ptr, charColor, bkColor);  }
      else{     
      LCD_PutChar_A(X, Y, *ptr, charColor, bkColor);     
      }
    /* Decrement the column position by 16 */
    X += 16*2;
    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    i++;
  }
}

void LCD_DisplayString_data_A(u16 X,u16 Y, u32 data, u16 charColor, u16 bkColor)
{
//  u32 i = 0;


    unsigned char valtable[4]={0};
		unsigned short temp,demay=1000;
		unsigned char i;
    
		temp=data;
		if(temp==0)
		{	
			 for(i=0;i<3;i++)
			 {
			  valtable[i]=32; 
			 }
			 valtable[i]=0x30;       			
		}
		else
		{			
			for(i=0;i<4;i++)
			{
				valtable[i]=temp/demay+0x30;
				temp=temp%demay;
				demay/=10;
			}
			valtable[i]='\0';
			for(i=0;i<4;i++)
			{
				if(valtable[i]==0x30)
				{
					valtable[i]=32;
				}
				else
				{
					break;
				}
			}
		}
//		LCD1602_display_Line_A(x,y,valtable);
        LCD_DisplayString_A(X,Y,valtable,charColor,bkColor);
}

/*
*********************************************************************************************************
*	º¯ Êý Ãû: LCD_DispStr
*	¹¦ÄÜËµÃ÷: ÔÚLCDÖ¸¶¨×ø±ê£¨×óÉÏ½Ç£©ÏÔÊ¾Ò»¸ö×Ö·û´®
*	ÐÎ    ²Î£º
*		_usX : X×ø±ê£¬¶ÔÓÚ3.0´ç¿íÆÁ£¬·¶Î§Îª¡¾0 - 399¡¿
*		_usY : Y×ø±ê£¬¶ÔÓÚ3.0´ç¿íÆÁ£¬·¶Î§Îª ¡¾0 - 239¡¿
*		_ptr  : ×Ö·û´®Ö¸Õë
*		_tFont : ×ÖÌå½á¹¹Ìå£¬°üº¬ÑÕÉ«¡¢±³¾°É«(Ö§³ÖÍ¸Ã÷)¡¢×ÖÌå´úÂë¡¢ÎÄ×Ö¼ä¾àµÈ²ÎÊý
*	·µ »Ø Öµ: ÎÞ
*********************************************************************************************************
*/
void DispStr48(uint16_t _usX, uint16_t _usY, char *_ptr, FONT_T *_tFont)
{
	uint32_t i,k;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint8_t buf[288];	 	//48µãÕóºº×Ö
	uint8_t m, width;
	uint16_t font_width,font_height, font_bytes;
	uint16_t x, y;
	const uint8_t *pAscDot;
	const uint8_t *pHzDot;

	
	font_height = 48;	 //ºº×Ö¸ß¶È
	font_width = 48;	   //ºº×Ö¿í¶È
	font_bytes = 288;	   //×Ö½ÚÊý
	pAscDot = code24x48;   
	pHzDot = codeGB_48;


	/* ¿ªÊ¼Ñ­»·´¦Àí×Ö·û */
	while (*_ptr != 0)
	{
		code1 = *_ptr;	/* ¶ÁÈ¡×Ö·û´®Êý¾Ý£¬ ¸ÃÊý¾Ý¿ÉÄÜÊÇascii´úÂë£¬Ò²¿ÉÄÜºº×Ö´úÂëµÄ¸ß×Ö½Ú */
		if (code1 < 0x80)
		{
			/* ½«ascii×Ö·ûµãÕó¸´ÖÆµ½buf */
			//memcpy(buf, &pAscDot[code1 * (font_bytes / 2)], (font_bytes / 2));
			width = font_width / 2;

			m = 0;
			for (k=0;k<11;k++)  //64±êÊ¾×Ô½¨ºº×Ö¿âÖÐµÄ¸öÊý£¬Ñ­»·²éÑ¯ÄÚÂë
			{
				address = m * (font_bytes/2 + 1);
				m++;
				if (code1 == pAscDot[address])
				{
					address++;
					memcpy(buf, &pAscDot[address], font_bytes/2);
					break;
				}
			}
		}
		else
		{
			code2 = *++_ptr;
			if (code2 == 0)
			{
				break;
			}

			/* ¼ÆËã16µãÕóºº×ÖµãÕóµØÖ·
				ADDRESS = [(code1-0xa1) * 94 + (code2-0xa1)] * 32
				;
			*/
			#ifdef USE_SMALL_FONT
				m = 0;
				while(1)
				{
					address = m * (font_bytes + 2);
					m++;
					if ((code1 == pHzDot[address + 0]) && (code2 == pHzDot[address + 1]))
					{
						address += 2;
						memcpy(buf, &pHzDot[address], font_bytes);
						break;
					}
					else if ((pHzDot[address + 0] == 0xFF) && (pHzDot[address + 1] == 0xFF))
					{
						/* ×Ö¿âËÑË÷Íê±Ï£¬Î´ÕÒµ½£¬ÔòÌî³äÈ«FF */
						memset(buf, 0xFF, font_bytes);
						break;
					}
				}
			#else	/* ÓÃÈ«×Ö¿â */
				/* ´Ë´¦ÐèÒª¸ù¾Ý×Ö¿âÎÄ¼þ´æ·ÅÎ»ÖÃ½øÐÐÐÞ¸Ä */
				address = ((code1-0xa1) * 94 + (code2-0xa1)) * font_bytes + HZK16_ADDR;
				memcpy(buf, (const uint8_t *)address, font_bytes);
			#endif

				width = font_width;
		}

		y = _usY;
		/* ¿ªÊ¼Ë¢LCD */
		for (m = 0; m < font_height; m++)	/* ×Ö·û¸ß¶È */
		{
			x = _usX;
			for (i = 0; i < width; i++)	/* ×Ö·û¿í¶È */
			{
				if ((buf[m * ((6 * width) / font_width) + i / 8] & (0x80 >> (i % 8 ))) != 0x00)
				{
					LCD_SetPoint(x, y, _tFont->usTextColor); //×Ö·ûÑÕÉ«
				}
				else
				{
					LCD_SetPoint(x, y, _tFont->usBackColor); //×Ö·ûÑÕÉ«
				}

				x++;
			}
			y++;
		}

		if (_tFont->usSpace > 0)
		{
			/* Èç¹ûÎÄ×Öµ×É«°´_tFont->usBackColor£¬²¢ÇÒ×Ö¼ä¾à´óÓÚµãÕóµÄ¿í¶È£¬ÄÇÃ´ÐèÒªÔÚÎÄ×ÖÖ®¼äÌî³ä(ÔÝÊ±Î´ÊµÏÖ) */
		}
		_usX += width + _tFont->usSpace;	/* ÁÐµØÖ·µÝÔö */
		_ptr++;			/* Ö¸ÏòÏÂÒ»¸ö×Ö·û */
	}
}



/*******************************************************************************
* Function Name  : LCD_DisplayStringLine
* Description    : Displays a maximum of 20 char on the LCD.
* Input          : - Line: the Line where to display the character shape .
*                    This parameter can be one of the following values:
*                       - Linex: where x can be 0..9
*                  - *ptr: pointer to string to display on LCD.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DisplayStringLine(u16 Line, char *ptr, u16 charColor, u16 bkColor)
{
  u32 i = 0;
  u16 refcolumn = 0;

  /* Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 50))
  {
    /* Display one character on LCD */
//    LCD_PutChar(refcolumn, Line, *ptr, charColor, bkColor);
	LCD_PutChar(refcolumn, Line, *ptr, charColor, bkColor);
    /* Decrement the column position by 16 */
    refcolumn += 16;
    /* Point on the next character */
    ptr++;
    /* Increment the character counter */
    i++;
  }
}

/*******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : Displays a line.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Length: line length.
*                  - Direction: line direction.
*                    This parameter can be one of the following values: Vertical 
*                    or Horizontal.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawLine(u16 Xpos, u16 Ypos, u16 Length, u8 Direction)
{
  u32 i = 0;
  
  LCD_SetCursor(Xpos,319 - Ypos);

  if(Direction == Horizontal)
  {
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    for(i = 0; i < Length; i++)
    {
      LCD_DataWrite(TextColor);
    }
  }
  else
  {
    for(i = 0; i < Length; i++)
    {
      LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
      LCD_DataWrite(TextColor);
      Xpos++;
      LCD_SetCursor(Xpos, 479 - Ypos);
    }
  }
}

/*******************************************************************************
* Function Name  : LCD_DrawRect
* Description    : Displays a rectangle.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Height: display rectangle height.
*                  - Width: display rectangle width.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawRect(u16 Xpos, u16 Ypos, u16 Height, u16 Width)
{
  LCD_DrawLine(Xpos, Ypos, Width, Horizontal);
  LCD_DrawLine((Xpos + Height), Ypos, Width, Horizontal);
  
  LCD_DrawLine(Xpos, Ypos, Height, Vertical);
  LCD_DrawLine(Xpos, (Ypos - Width + 1), Height, Vertical);
}

/*******************************************************************************
* Function Name  : LCD_DrawCircle
* Description    : Displays a circle.
* Input          : - Xpos: specifies the X position.
*                  - Ypos: specifies the Y position.
*                  - Height: display rectangle height.
*                  - Width: display rectangle width.
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawCircle(u16 Xpos, u16 Ypos, u16 Radius)
{
  s32  D;/* Decision Variable */ 
  u32  CurX;/* Current X Value */
  u32  CurY;/* Current Y Value */ 
  
  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  
  while (CurX <= CurY)
  {
    LCD_SetCursor(Xpos + CurX, Ypos + CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_DataWrite(TextColor);

    LCD_SetCursor(Xpos + CurX, Ypos - CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_DataWrite(TextColor);

    LCD_SetCursor(Xpos - CurX, Ypos + CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_DataWrite(TextColor);

    LCD_SetCursor(Xpos - CurX, Ypos - CurY);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_DataWrite(TextColor);

    LCD_SetCursor(Xpos + CurY, Ypos + CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_DataWrite(TextColor);

    LCD_SetCursor(Xpos + CurY, Ypos - CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_DataWrite(TextColor);

    LCD_SetCursor(Xpos - CurY, Ypos + CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_DataWrite(TextColor);

    LCD_SetCursor(Xpos - CurY, Ypos - CurX);
    LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
    LCD_DataWrite(TextColor);

    if (D < 0)
    { 
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}




void LCD_Border_A(void)
{
  u32 m,n;
	u16 color_temp1,color_temp2,color_temp3;
    u8 Color1_gg,Color1_rr,Color1_bb,Color2_rr,Color2_gg,Color2_bb;

//	LCD_Set_DisplayWindow(0,XDP-1,0,YDP-1);
//	LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
//	rr1=(point1>>16);  
//  gg1=(point1>>8);  
//  bb1=point1;
//  rr2=(point2>>16);  
//  gg2=(point2>>8);  
//  bb2=point2;
    
//    LCD_DataWrite((gg1<<8) | bb1);		
////			Check_Mem_WR_FIFO_not_Full();
//			LCD_DataWrite((bb2<<8) | rr1);		
////			Check_Mem_WR_FIFO_not_Full();
//			LCD_DataWrite((rr2<<8) | gg2);		
////			Check_Mem_WR_FIFO_not_Full();
    
    
//	color_temp1 = (Color1_gg<<8) | Color1_bb;
//	color_temp2 = (Color2_bb<<8) | Color1_rr;
//	color_temp3 = (Color2_rr<<8) | Color2_gg;
	
//  	u32 index = 0;
//	u16 m,n;
//	u16 color_temp1,color_temp2,color_temp3;
//  
//	LCD_Set_DisplayWindow(add_sx,add_ex,add_sy,add_ey);
//  LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */
	
//	color_temp1 = (Color_gg<<8) | Color_bb;
//	color_temp2 = (Color_bb<<8) | Color_rr;
//	color_temp3 = (Color_rr<<8) | Color_gg;
	
	for(m = 0; m <YDP;m++)
	{
		for(n = 0; n <XDP;n+=2)
		{
			if((m==0)||(m==YDP-1))    
            {   
                Color1_gg = 255;Color1_rr = 255;Color1_bb = 255;       //////white
                Color2_gg = 255;Color2_rr = 255;Color2_bb = 255;       //////white              
            }     //////white
            else if(n == 0)
            {
                Color1_gg = 255;Color1_rr = 255;Color1_bb = 255;       //////white 
                Color2_gg = 0;Color2_rr = 0;Color2_bb = 0;              /////black
            }
            else if(n == (XDP-2))
            {
                Color1_gg = 0;Color1_rr = 0;Color1_bb = 0;              /////black
                Color2_gg = 255;Color2_rr = 255;Color2_bb = 255;        //////white
            }
            else
            {   
                Color1_gg = 0;Color1_rr = 0;Color1_bb = 0;      /////black
                Color2_gg = 0;Color2_rr = 0;Color2_bb = 0;      /////black
            }     /////black

            color_temp1 = (Color1_gg<<8) | Color1_bb;
            color_temp2 = (Color2_bb<<8) | Color1_rr;
            color_temp3 = (Color2_rr<<8) | Color2_gg;
            
            LCD_DataWrite(color_temp1);		
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite(color_temp2);		
			Check_Mem_WR_FIFO_not_Full();
			LCD_DataWrite(color_temp3);		
			Check_Mem_WR_FIFO_not_Full();
		}
	}
}



/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/


