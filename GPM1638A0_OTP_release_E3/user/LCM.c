#include "stdio.h"
#include <stdlib.h>
#include <string.h>             // 包含memcpy函数


#include "stm32f10x_tim.h"
#include "UserDef.h"
#include "delay.h"
#include "config_hard.h"



////////timing  setting
extern u16 TIMING_SET[];  
 ///////////HS_speed,LP speed,lane parameter
extern u16 MIPI_SET[3];
extern u16 VCOMDC1;

//#define KGM1238A0_LVDS_1024X600
//#define DEMO_107_LVDS_1280X720
#define GPM16538A0_MIPI
//#define KGM1197C0_MIPI
//#define V480x800_HX8369
//#define KGM1680A0_LVDS_1024X600
//#define GPM1651A0_YUV16BIT
//#define GPM1651A0_YUV8BIT



#ifdef	GPM16538A0_MIPI


u16 CONFIG_SYS_SET[10]={MIPI_ON,RGB_mode,RGB,0};
////{      ////KGM1197C0

u16 MIPI_SET_NOKIA_GPM1461A[3] = {450,9,4};
///////////HS_speed,LP,lane

//u16 TIMING_SET_NOKIA_GPM1461A[15] = {60,720,1280,60,60,26,20,10,4,1,0,0,1};     ///////,MAX PCLK =160MHZ
///////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
//////}


u16 TIMING_SET_NOKIA_GPM1461A[15] = {60,720,1280,25,25,12,12,8,2,1,0,0,1};     ///////,MAX PCLK =160MHZ
/////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
////} 

u8 PIC_SET_CACHE_nokia[PIC_SAVE_NUM_MAX][3]=
{
{0,0,0},

{PIC_RED,1,0},      /////纯红画面			//////OK
{PIC_GREEN,0,0},     /////纯绿				/////OK
{PIC_BLUE,1,0},     /////纯蓝				//////OK
{PIC_WHITE,1,0},	/////纯白				//////OK
{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////OK
{PIC_BLACK,0,0},	//////纯黑				//////OK
//{PIC_BORDER,1,0},	/////边框画面
{PIC_colorbar_V,1,0},	//////垂直彩条		//////OK
{PIC_colorbar_H,1,0},	/////水平彩条
{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
{PIC_FROM_SD,2,2},			////SD卡图片，第二幅



////{PIC_RED,1,0},      /////纯红画面
////{PIC_GREEN,0,0},     /////纯绿
////{PIC_BLUE,1,0},     /////纯蓝
////{PIC_GRAY,0,128},     /////纯灰---灰阶128
////{PIC_GRAY,0,90},     /////纯灰---灰阶90
////{PIC_GRAY,0,60},     /////纯灰---灰阶60
////{PIC_GRAY,0,30},     /////纯灰---灰阶30
////{PIC_BORDER,1,0},	/////边框画面
////{PIC_BLACK,0,0},	//////纯黑
////{PIC_WHITE,1,0},	/////纯白
////{PIC_CT,1,0},		////crosstalk
////{PIC_colorbar_V,1,0},	//////垂直彩条
////{PIC_colorbar_H,1,0},	/////水平彩条
////{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_GRAY_H,1,0},			//////灰阶水平渐变
//{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
//{PIC_Flicker_H,1,0},		/////flicker  column
//{PIC_Flicker_V,1,0},		/////flicker   column
//{PIC_Flicker_H_DOT,1,0},	////flicker   点列翻转
//{PIC_RED_H,1,0},			/////红色水平渐变
//{PIC_RED_V,1,0},			////红色垂直渐变
//{PIC_GREEN_H,1,0},			/////绿色水平渐变
//{PIC_GREEN_V,1,0},			/////绿色垂直渐变
//{PIC_BLUE_H,1,0},			/////蓝色水平渐变
//{PIC_BLUE_V,1,0},			/////蓝色垂直渐变

//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式Column
//{PIC_Dot_Flicker_A,1,127},	////flicker	-----127灰，模式2V1H
//{PIC_Dot_Flicker_A,2,127},	////flicker	-----127灰，模式1Dot
//{PIC_Dot_Flicker_A,3,127},	////flicker	-----127灰，模式1V2H
//{PIC_Dot_Flicker_A,4,127},	////flicker	----127灰，模式1+2V1H
//{PIC_Dot_Flicker_A,5,127},	////flicker	-----127灰，模式1+1V2H
//{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2Dot
//{PIC_Dot_Flicker_A,7,127},	////flicker	-----127灰，模式2V1H---DOT
//{PIC_Dot_Flicker_B,1,0},	////两点flicker	
//{PIC_Dot_Flicker_C,1,0},	////两点flicker	

//{PIC_CT_center,1,0},			////////CT 	中心点黑块，
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅
};

//vcom阶次设定方式
void VCOM_set(u8 vcom)
{
			MIPI_SPI_Write(  03-1,0x00,0x00  );        
			MIPI_SPI_Write(  05-1,0xff,0x12,0x89,0x01  );        	////#EXTC=1

			MIPI_SPI_Write(  03-1,0x00,0x80    );      	        ////#Orise mode enable
			MIPI_SPI_Write(  04-1,0xff,0x12,0x89  );  
			Delay(2);
	
			MIPI_SPI_Write(  03-1,0x00,0x00 );                      ////#VCOM=-1.075V
			MIPI_SPI_Write(  04,0xd9,0x00,vcom,vcom);
//	Delay(2);
//	MIPI_SPI_Write(2,0x00,0x00);
//	MIPI_SPI_Write(2,0xD9,vcom);
} 


//////MIPI CODE USE
void MIPI_LCD_INITION(void)
{
	{
    u16 VHPW,VHBP,VHFP;
	u16 data=0; 

    
	VHPW=((TIMING_SET[8]+0)<<8)+(TIMING_SET[7]+0);
	VHBP=((TIMING_SET[5]+2)<<8)+(TIMING_SET[3]+0);	
	VHFP=((TIMING_SET[6]+2)<<8)+(TIMING_SET[4]+0);
        
       //////////MIPI设定
    STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);
        
        STM32TOSSD2828_W_COM(0xB1);
		STM32TOSSD2828_W_DATA_16BITS(VHPW);		//Vertical sync and horizontal sync active period 
        
		STM32TOSSD2828_W_COM(0xB2);
		STM32TOSSD2828_W_DATA_16BITS(VHBP);		//Vertical and horizontal back porch period 

		STM32TOSSD2828_W_COM(0xB3);
		STM32TOSSD2828_W_DATA_16BITS(VHFP);		//Vertical and horizontal front porch period 
        
		STM32TOSSD2828_W_COM(0xB4);
		STM32TOSSD2828_W_DATA_16BITS(TIMING_SET[1]);		//Horizontal active period 
//		X = READ_SSD2828_onebyte(0xB4);

		STM32TOSSD2828_W_COM(0xB5);
		STM32TOSSD2828_W_DATA_16BITS(TIMING_SET[2]);		//Vertical active period 
//		Y = READ_SSD2828_onebyte(0xB5);

		STM32TOSSD2828_W_COM(0xB6);		//Video mode and video pixel format
		STM32TOSSD2828_W_DATA_16BITS(0x000B);		//24bit 			   
        
		STM32TOSSD2828_W_COM(0xDE);		
		STM32TOSSD2828_W_DATA_16BITS(MIPI_SET[2]-1);		//MIPI lane select  
        
		STM32TOSSD2828_W_COM(0xd6);	
		STM32TOSSD2828_W_DATA_16BITS(0x00F5);		//Color order and endianess --------------20161111修改为0x00F4，大于0x0004即可
        
		STM32TOSSD2828_W_COM(0xb8);		//VC register 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);
        
		{
//            STM32TOSSD2828_W_COM(0xC9);		//add  -2016-3-3
//            STM32TOSSD2828_W_DATA_16BITS(0x2302);      ///////default  = 0x1402
//  
//            STM32TOSSD2828_W_COM(0xCA);		//add  -2016-3-3
//            STM32TOSSD2828_W_DATA_16BITS(0x2301);      ///////default  = 0x2803
//            
//            STM32TOSSD2828_W_COM(0xCB);		//add  -2016-3-3
//            STM32TOSSD2828_W_DATA_16BITS(0x0510);      ///////default  = 0x0416
//            
//            STM32TOSSD2828_W_COM(0xCC);		//add  -2016-3-3
//            STM32TOSSD2828_W_DATA_16BITS(0x1005);      ///////default  = 0x0a0a
//            
//            STM32TOSSD2828_W_COM(0xD6);		//add  -2016-3-3
//            STM32TOSSD2828_W_DATA_16BITS(0x0006);      ///////default  = 
//            
//            STM32TOSSD2828_W_COM(0xC4);		//add  -2016-3-3
//            STM32TOSSD2828_W_DATA_16BITS(0x0001);
//            
//            STM32TOSSD2828_W_COM(0xEB);		//add  -2016-3-3
//            STM32TOSSD2828_W_DATA_16BITS(0x8000);
//            
//            STM32TOSSD2828_W_COM(0xDD);		//add  -2016-3-3
//            STM32TOSSD2828_W_DATA_16BITS(0x0000);      ///////default  = 0x0000

        }
      
		STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);
        DelayMs(5);

		data = MIPI_SET[0]/FREQ_SSD2828;

		if(MIPI_SET[0] < 125 )
		{
			data =  data;	
		}
		else if(MIPI_SET[0] < 250 )
		{
			data = 0x4000 + data;	
		}
		else  if(MIPI_SET[0] < 500 )
		{
			data = 0x8000 + data;	
		}
		else
		{
			data = 0xc000 + data;	
		}
			
		STM32TOSSD2828_W_COM(0xba);		//PLL setting 
		STM32TOSSD2828_W_DATA_16BITS(data);
        
		STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
		STM32TOSSD2828_W_DATA_16BITS(MIPI_SET[0]/8/MIPI_SET[1]-1);			//////8.8MHZ		
        DelayMs(10);

		STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
		STM32TOSSD2828_W_DATA_16BITS(0x4001);		//8??	,SYS_CLK??24/8=3MHZ
        DelayMs(5);
    }
     


        /////////屏 CODE
    {
        STM32TOSSD2828_W_COM(0xb7);		//LP DCS mode
        STM32TOSSD2828_W_DATA_16BITS(0x0752);
//        reg_send_check(0xb7,0x0752);
        DelayMs(10);  
        
		
        {
          
        }
        DelayMs(10);       
        MIPI_SPI_Write(0x01,0x11);
        DelayMs(180);
     


        MIPI_SPI_Write(0x01,0x29);
        DelayMs(50);

		
        STM32TOSSD2828_W_COM(0xb7);		//LP DCS mode
        STM32TOSSD2828_W_DATA_16BITS(0x070b);
        DelayMs(10); 
    }
}

/////////RGB code use
void M1651_OTA5601_INIT(void)
{
}

/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //此處需根據不同型號進行OTP流程修改
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{	
    
    STM32TOSSD2828_W_COM(0xb7);		//LP DCS mode
    STM32TOSSD2828_W_DATA_16BITS(0x0752);
    DelayMs(10);  

	MIPI_SPI_Write(1,0x10);
	Delay(150);
	
    

    MIPI_SPI_Write(  03-1,0x00,0x00  );        
    MIPI_SPI_Write(  05-1,0xff,0x12,0x89,0x01  );        	////#EXTC=1

    MIPI_SPI_Write(  03-1,0x00,0x80    );      	        ////#Orise mode enable
    MIPI_SPI_Write(  04-1,0xff,0x12,0x89  ); 
		
	MIPI_SPI_Write(  2,0x00,0x84    );      	        ////##-OTP Program Setting
	MIPI_SPI_Write(  2,0xf6,0x5A  ); 
	Delay(150);
	

    MIPI_SPI_Write(2,0x00,0x00);
	MIPI_SPI_Write(1,0x11);
	Delay(10);
	MIPI_SPI_Write(1,0x28);
	Delay(150);
////////////////////==============set vcom======================		
	MIPI_SPI_Write(2,0x00,0x00);
    MIPI_SPI_Write(4,0xD9,0,VCOMDC1,VCOMDC1);
//	MIPI_SPI_Write(2,0xD9,VCOMDC1);
	Delay(10);


	
	//##-Start NVM Program
	MIPI_SPI_Write(2,0x00,0x00);
	MIPI_SPI_Write(4,0xEB,0x01,0x5A,0xA5);
	DelayMs(950);
	
	//#END NVM Program
	MIPI_SPI_Write(2,0x00,0x00);
	MIPI_SPI_Write(4,0xEB,0x00,0x00,0x00);

//	MIPI_SPI_Write(1,0x10);
	Delay(25);	
    

}

#endif






#ifdef	KGM1197C0_MIPI


u16 CONFIG_SYS_SET[10]={MIPI_ON,RGB_mode,BGR,0};
////{      ////KGM1197C0

u16 MIPI_SET_NOKIA_GPM1461A[3] = {480,8,4};
///////////HS_speed,LP,lane

u16 TIMING_SET_NOKIA_GPM1461A[15] = {60,720,1280,60,60,19,20,10,4,1,0,0,1};     ///////,MAX PCLK =160MHZ
/////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
////} 

u8 PIC_SET_CACHE_nokia[PIC_SAVE_NUM_MAX][3]=
{
{0,0,0},

//{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_RED,1,0},      /////纯红画面			//////YUV16BIT-OK,YUV8BIT-OK
{PIC_GREEN,0,0},     /////纯绿				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_BLUE,1,0},     /////纯蓝				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_WHITE,1,0},	/////纯白				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_BLACK,0,0},	//////纯黑				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_CT_center,0,255},			////////CT 	中心点块颜色255			//////YUV16BIT-OK,YUV8BIT-OK
{PIC_checkerboard,1,0},		//////6x6棋盘格		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2V1H---DOT  //////YUV16BIT-OK,YUV8BIT-NG
{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅

//{PIC_BORDER,1,0},	/////边框画面
//{PIC_CT_center,1,0},			////////CT 	中心点黑块，


//{PIC_RED,1,0},      /////纯红画面
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_BLUE,1,0},     /////纯蓝
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_GRAY,0,90},     /////纯灰---灰阶90
//{PIC_GRAY,0,60},     /////纯灰---灰阶60
//{PIC_GRAY,0,30},     /////纯灰---灰阶30
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_BLACK,0,0},	//////纯黑
//{PIC_WHITE,1,0},	/////纯白
//{PIC_CT,1,0},		////crosstalk
//{PIC_colorbar_V,1,0},	//////垂直彩条
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_GRAY_H,1,0},			//////灰阶水平渐变
//{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
//{PIC_Flicker_H,1,0},		/////flicker  column
//{PIC_Flicker_V,1,0},		/////flicker   column
//{PIC_Flicker_H_DOT,1,0},	////flicker   点列翻转
//{PIC_RED_H,1,0},			/////红色水平渐变
//{PIC_RED_V,1,0},			////红色垂直渐变
//{PIC_GREEN_H,1,0},			/////绿色水平渐变
//{PIC_GREEN_V,1,0},			/////绿色垂直渐变
//{PIC_BLUE_H,1,0},			/////蓝色水平渐变
//{PIC_BLUE_V,1,0},			/////蓝色垂直渐变

//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式Column
//{PIC_Dot_Flicker_A,1,127},	////flicker	-----127灰，模式2V1H
//{PIC_Dot_Flicker_A,2,127},	////flicker	-----127灰，模式1Dot
//{PIC_Dot_Flicker_A,3,127},	////flicker	-----127灰，模式1V2H
//{PIC_Dot_Flicker_A,4,127},	////flicker	----127灰，模式1+2V1H
//{PIC_Dot_Flicker_A,5,127},	////flicker	-----127灰，模式1+1V2H
//{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2Dot
//{PIC_Dot_Flicker_A,7,127},	////flicker	-----127灰，模式2V1H---DOT
//{PIC_Dot_Flicker_B,1,0},	////两点flicker	
//{PIC_Dot_Flicker_C,1,0},	////两点flicker	

//{PIC_CT_center,1,0},			////////CT 	中心点黑块，
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅
};

//////MIPI CODE USE
void MIPI_LCD_INITION(void)
{
	{
    u16 VHPW,VHBP,VHFP;
	u16 data=0; 

    
	VHPW=(TIMING_SET[8]<<8)+TIMING_SET[7];
	VHBP=((TIMING_SET[5])<<8)+(TIMING_SET[3]);	
	VHFP=(TIMING_SET[6]<<8)+TIMING_SET[4];
        
       //////////MIPI设定
//    STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
//		STM32TOSSD2828_W_DATA_16BITS(0x0000);
        
        STM32TOSSD2828_W_COM(0xB1);
		STM32TOSSD2828_W_DATA_16BITS(VHPW);		//Vertical sync and horizontal sync active period 
        
		STM32TOSSD2828_W_COM(0xB2);
		STM32TOSSD2828_W_DATA_16BITS(VHBP);		//Vertical and horizontal back porch period 

		STM32TOSSD2828_W_COM(0xB3);
		STM32TOSSD2828_W_DATA_16BITS(VHFP);		//Vertical and horizontal front porch period 
        
		STM32TOSSD2828_W_COM(0xB4);
		STM32TOSSD2828_W_DATA_16BITS(TIMING_SET[1]);		//Horizontal active period 
//		X = READ_SSD2828_onebyte(0xB4);

		STM32TOSSD2828_W_COM(0xB5);
		STM32TOSSD2828_W_DATA_16BITS(TIMING_SET[2]);		//Vertical active period 
//		Y = READ_SSD2828_onebyte(0xB5);

		STM32TOSSD2828_W_COM(0xB6);		//Video mode and video pixel format
		STM32TOSSD2828_W_DATA_16BITS(0x000B);		//24bit 			   
        
		STM32TOSSD2828_W_COM(0xDE);		
		STM32TOSSD2828_W_DATA_16BITS(MIPI_SET[2]-1);		//MIPI lane select  
        
		STM32TOSSD2828_W_COM(0xd6);	
		STM32TOSSD2828_W_DATA_16BITS(0x00F5);		//Color order and endianess --------------20161111修改为0x00F4，大于0x0004即可
        
		STM32TOSSD2828_W_COM(0xb8);		//VC register 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);
        
      
		STM32TOSSD2828_W_COM(0xb9);		//PLL disable 
		STM32TOSSD2828_W_DATA_16BITS(0x0000);
        DelayMs(5);

		data = MIPI_SET[0]/FREQ_SSD2828;

		if(MIPI_SET[0] < 125 )
		{
			data =  data;	
		}
		else if(MIPI_SET[0] < 250 )
		{
			data = 0x4000 + data;	
		}
		else  if(MIPI_SET[0] < 500 )
		{
			data = 0x8000 + data;	
		}
		else
		{
			data = 0xc000 + data;	
		}
			
		STM32TOSSD2828_W_COM(0xba);		//PLL setting 
		STM32TOSSD2828_W_DATA_16BITS(data);
        
		STM32TOSSD2828_W_COM(0xbb);		//LP clock divider 
		STM32TOSSD2828_W_DATA_16BITS(MIPI_SET[0]/8/MIPI_SET[1]-1);			//////8.8MHZ
//		STM32TOSSD2828_W_DATA_16BITS(0x0006);			
        DelayMs(10);

		STM32TOSSD2828_W_COM(0xb9);		//PLL enable 
		STM32TOSSD2828_W_DATA_16BITS(0x4001);		//8??	,SYS_CLK??24/8=3MHZ
        DelayMs(5);
    }
     


        /////////屏 CODE
    {
        STM32TOSSD2828_W_COM(0xb7);		//LP DCS mode
        STM32TOSSD2828_W_DATA_16BITS(0x0752);
//        reg_send_check(0xb7,0x0752);
        DelayMs(10);  
        
		
        {
                //
                MIPI_SPI_Write(0x04, 0xFF, 0x98 ,0x81, 0x03);
                //#gip_1
                MIPI_SPI_Write(0x02,0x01,0x00);
                MIPI_SPI_Write(0x02,0x02,0x00);
                MIPI_SPI_Write(0x02,0x03,0x73);
                MIPI_SPI_Write(0x02,0x04,0x00);
                MIPI_SPI_Write(0x02,0x05,0x00);
                MIPI_SPI_Write(0x02,0x06,0x0C);
                MIPI_SPI_Write(0x02,0x07,0x00);
                MIPI_SPI_Write(0x02,0x08,0x00);
                MIPI_SPI_Write(0x02,0x09,0x19);
                MIPI_SPI_Write(0x02,0x0a,0x01);
                MIPI_SPI_Write(0x02,0x0b,0x01);
                MIPI_SPI_Write(0x02,0x0c,0x0B);
                MIPI_SPI_Write(0x02,0x0d,0x01);
                MIPI_SPI_Write(0x02,0x0e,0x01);
                MIPI_SPI_Write(0x02,0x0f,0x26);
                MIPI_SPI_Write(0x02,0x10,0x26); 
                MIPI_SPI_Write(0x02,0x11,0x00);
                MIPI_SPI_Write(0x02,0x12,0x00);
                MIPI_SPI_Write(0x02,0x13,0x02);
                MIPI_SPI_Write(0x02,0x14,0x00);
                MIPI_SPI_Write(0x02,0x15,0x00);
                MIPI_SPI_Write(0x02,0x16,0x00); 
                MIPI_SPI_Write(0x02,0x17,0x00); 
                MIPI_SPI_Write(0x02,0x18,0x00);
                MIPI_SPI_Write(0x02,0x19,0x00);
                MIPI_SPI_Write(0x02,0x1a,0x00);
                MIPI_SPI_Write(0x02,0x1b,0x00);
                MIPI_SPI_Write(0x02,0x1c,0x00);
                MIPI_SPI_Write(0x02,0x1d,0x00);
                MIPI_SPI_Write(0x02,0x1e,0x44);
                MIPI_SPI_Write(0x02,0x1f,0xC0);  
                MIPI_SPI_Write(0x02,0x20,0x0A);
                MIPI_SPI_Write(0x02,0x21,0x03);
                MIPI_SPI_Write(0x02,0x22,0x0A);  
                MIPI_SPI_Write(0x02,0x23,0x00);
                MIPI_SPI_Write(0x02,0x24,0x8C); 
                MIPI_SPI_Write(0x02,0x25,0x8C);  
                MIPI_SPI_Write(0x02,0x26,0x00);
                MIPI_SPI_Write(0x02,0x27,0x00);
                MIPI_SPI_Write(0x02,0x28,0x3B);  
                MIPI_SPI_Write(0x02,0x29,0x03);
                MIPI_SPI_Write(0x02,0x2a,0x00);
                MIPI_SPI_Write(0x02,0x2b,0x00);
                MIPI_SPI_Write(0x02,0x2c,0x00);
                MIPI_SPI_Write(0x02,0x2d,0x00);
                MIPI_SPI_Write(0x02,0x2e,0x00);
                MIPI_SPI_Write(0x02,0x2f,0x00);
                MIPI_SPI_Write(0x02,0x30,0x00);
                MIPI_SPI_Write(0x02,0x31,0x00);
                MIPI_SPI_Write(0x02,0x32,0x00);
                MIPI_SPI_Write(0x02,0x33,0x00);
                MIPI_SPI_Write(0x02,0x34,0x43);
                MIPI_SPI_Write(0x02,0x35,0x00);
                MIPI_SPI_Write(0x02,0x36,0x03);
                MIPI_SPI_Write(0x02,0x37,0x00);
                MIPI_SPI_Write(0x02,0x38,0x00);
                MIPI_SPI_Write(0x02,0x39,0x00);
                MIPI_SPI_Write(0x02,0x3a,0x00);
                MIPI_SPI_Write(0x02,0x3b,0x00);
                MIPI_SPI_Write(0x02,0x3c,0x00);
                MIPI_SPI_Write(0x02,0x3d,0x00);
                MIPI_SPI_Write(0x02,0x3e,0x00);
                MIPI_SPI_Write(0x02,0x3f,0x00);
                MIPI_SPI_Write(0x02,0x40,0x00);
                MIPI_SPI_Write(0x02,0x41,0x00);
                MIPI_SPI_Write(0x02,0x42,0x00);
                MIPI_SPI_Write(0x02,0x43,0x00);
                MIPI_SPI_Write(0x02,0x44,0x00);


                //#gip_2
                MIPI_SPI_Write(0x02,0x50,0x01);
                MIPI_SPI_Write(0x02,0x51,0x23);
                MIPI_SPI_Write(0x02,0x52,0x45);
                MIPI_SPI_Write(0x02,0x53,0x67);
                MIPI_SPI_Write(0x02,0x54,0x89);
                MIPI_SPI_Write(0x02,0x55,0xab);
                MIPI_SPI_Write(0x02,0x56,0x01);
                MIPI_SPI_Write(0x02,0x57,0x23);
                MIPI_SPI_Write(0x02,0x58,0x45);
                MIPI_SPI_Write(0x02,0x59,0x67);
                MIPI_SPI_Write(0x02,0x5a,0x89);
                MIPI_SPI_Write(0x02,0x5b,0xab);
                MIPI_SPI_Write(0x02,0x5c,0xcd);
                MIPI_SPI_Write(0x02,0x5d,0xef);

                //#gip_3
                MIPI_SPI_Write(0x02,0x5e,0x11);
                MIPI_SPI_Write(0x02,0x5f,0x02);
                MIPI_SPI_Write(0x02,0x60,0x00);
                MIPI_SPI_Write(0x02,0x61,0x0C);
                MIPI_SPI_Write(0x02,0x62,0x0D);
                MIPI_SPI_Write(0x02,0x63,0x0E);
                MIPI_SPI_Write(0x02,0x64,0x0F);
                MIPI_SPI_Write(0x02,0x65,0x02);
                MIPI_SPI_Write(0x02,0x66,0x02);
                MIPI_SPI_Write(0x02,0x67,0x02);
                MIPI_SPI_Write(0x02,0x68,0x02);
                MIPI_SPI_Write(0x02,0x69,0x02);
                MIPI_SPI_Write(0x02,0x6a,0x02);
                MIPI_SPI_Write(0x02,0x6b,0x02);
                MIPI_SPI_Write(0x02,0x6c,0x02);
                MIPI_SPI_Write(0x02,0x6d,0x02);
                MIPI_SPI_Write(0x02,0x6e,0x05);
                MIPI_SPI_Write(0x02,0x6f,0x05);
                MIPI_SPI_Write(0x02,0x70,0x05);
                MIPI_SPI_Write(0x02,0x71,0x05);
                MIPI_SPI_Write(0x02,0x72,0x01);
                MIPI_SPI_Write(0x02,0x73,0x06);
                MIPI_SPI_Write(0x02,0x74,0x07);
                MIPI_SPI_Write(0x02,0x75,0x02);
                MIPI_SPI_Write(0x02,0x76,0x00);
                MIPI_SPI_Write(0x02,0x77,0x0C);
                MIPI_SPI_Write(0x02,0x78,0x0D);
                MIPI_SPI_Write(0x02,0x79,0x0E);
                MIPI_SPI_Write(0x02,0x7a,0x0F);
                MIPI_SPI_Write(0x02,0x7b,0x02);
                MIPI_SPI_Write(0x02,0x7c,0x02);
                MIPI_SPI_Write(0x02,0x7d,0x02);
                MIPI_SPI_Write(0x02,0x7e,0x02);
                MIPI_SPI_Write(0x02,0x7f,0x02);
                MIPI_SPI_Write(0x02,0x80,0x02);
                MIPI_SPI_Write(0x02,0x81,0x02);
                MIPI_SPI_Write(0x02,0x82,0x02);
                MIPI_SPI_Write(0x02,0x83,0x02);
                MIPI_SPI_Write(0x02,0x84,0x05);
                MIPI_SPI_Write(0x02,0x85,0x05);
                MIPI_SPI_Write(0x02,0x86,0x05);
                MIPI_SPI_Write(0x02,0x87,0x05);
                MIPI_SPI_Write(0x02,0x88,0x01);
                MIPI_SPI_Write(0x02,0x89,0x06);
                MIPI_SPI_Write(0x02,0x8a,0x07);


                //#CMD_Page 4
                MIPI_SPI_Write(4, 0xFF, 0x98, 0x81, 0x04);
                MIPI_SPI_Write(0x02, 0x6C, 0x15);
                MIPI_SPI_Write(0x02, 0x6E, 0x25);               //#di_pwr_reg=0 VGH clamp 15V
                MIPI_SPI_Write(0x02, 0x6F, 0x23);              // #reg vcl + VGH pumping ratio 3x VGL=-2x
                MIPI_SPI_Write(0x02, 0x3A, 0xA4);               //#power saving
                MIPI_SPI_Write(0x02, 0x8D, 0x14);              //#VGL clamp -10V  
                MIPI_SPI_Write(0x02, 0x87, 0xBA);              // #ESD
                //
                //MIPI_SPI_Write(0x02, 0x26, 0x76);
                //MIPI_SPI_Write(0x02, 0xb2, 0xd1);



                //#CMD_Page 1
                MIPI_SPI_Write (4, 0xFF, 0x98, 0x81, 0x01);
                MIPI_SPI_Write (2 ,0x22, 0x0A);		///# p238 0A FORWARD ?A09 BACKWARK
                MIPI_SPI_Write (2 ,0x53, 0x7D);		//#VCOM1
                MIPI_SPI_Write (2 ,0x55, 0x8A);		//#VCOM2
                MIPI_SPI_Write (2 ,0x50, 0x95);         	//#VREG1OUT=4.8V
                MIPI_SPI_Write (2 ,0x51, 0x95);         	//#VREG2OUT=-4.8V
                MIPI_SPI_Write (2 ,0x31, 0x00);		//#column inversion
                MIPI_SPI_Write (2 ,0x60, 0x09);		//#SDT
                        //  # VCOM READ REGISTER   01 read register


                MIPI_SPI_Write (2,0xa0,0x08);		//#vp255	gamma p
                MIPI_SPI_Write (2,0xa1,0x27);              // #vp251        
                MIPI_SPI_Write (2,0xa2,0x28);              // #vp247        
                MIPI_SPI_Write (2,0xa3,0x35);              // #vp243        
                MIPI_SPI_Write (2,0xa4,0x11);              // #vp239        
                MIPI_SPI_Write (2,0xa5,0x25);              // #vp231        
                MIPI_SPI_Write (2,0xa6,0x1A);              // #vp219        
                MIPI_SPI_Write (2,0xa7,0x1C);              // #vp203        
                MIPI_SPI_Write (2,0xa8,0x9E);              // #vp175        
                MIPI_SPI_Write (2,0xa9,0x19);              // #vp144        
                MIPI_SPI_Write (2,0xaa,0x2A);              // #vp111        
                MIPI_SPI_Write (2,0xab,0x91);              // #vp80         
                MIPI_SPI_Write (2,0xac,0x1A);              // #vp52         
                MIPI_SPI_Write (2,0xad,0x18);             // #vp36         
                MIPI_SPI_Write (2,0xae,0x4C);             ///  #vp24         
                MIPI_SPI_Write (2,0xaf,0x24);              // #vp16         
                MIPI_SPI_Write (2,0xb0,0x29);              // #vp12         
                MIPI_SPI_Write (2,0xb1,0x5D);              // #vp8          
                MIPI_SPI_Write (2,0xb2,0x6D);              // #vp4          
                MIPI_SPI_Write (2,0xb3,0x39);              // #vp0          
                                                               
                MIPI_SPI_Write (2,0xc0,0x08);		//#vn255 gamma n
                MIPI_SPI_Write (2,0xc1,0x0D);               //#vn251        
                MIPI_SPI_Write (2,0xc2,0x1C);               //#vn247        
                MIPI_SPI_Write (2,0xc3,0x14);               //#vn243        
                MIPI_SPI_Write (2,0xc4,0x16);               //#vn239        
                MIPI_SPI_Write (2,0xc5,0x29);               //#vn231        
                MIPI_SPI_Write (2,0xc6,0x1D);               //#vn219        
                MIPI_SPI_Write (2,0xc7,0x1F);               //#vn203        
                MIPI_SPI_Write (2,0xc8,0x84);               //#vn175        
                MIPI_SPI_Write (2,0xc9,0x1D);               //#vn144        
                MIPI_SPI_Write (2,0xca,0x28);               //#vn111        
                MIPI_SPI_Write (2,0xcb,0x80);               //#vn80         
                MIPI_SPI_Write (2,0xcc,0x1C);               //#vn52         
                MIPI_SPI_Write (2,0xcd,0x1D);               //#vn36         
                MIPI_SPI_Write (2,0xce,0x4F);               //#vn24         
                MIPI_SPI_Write (2,0xcf,0x22);               //#vn16         
                MIPI_SPI_Write (2,0xd0,0x27);               //#vn12         
                MIPI_SPI_Write (2,0xd1,0x58);              //#vn8          
                MIPI_SPI_Write (2,0xd2,0x6A);               //#vn4          
                MIPI_SPI_Write (2,0xd3,0x39);               //#vn0

                //#CMD_Page 0
                MIPI_SPI_Write (4, 0xFF ,0x98, 0x81, 0x00);
                //#MIPI_SPI_Write 0x39 0x35 0x00  # TE on
                MIPI_SPI_Write(0x02, 0x35,0x00);
        }
        DelayMs(10);       
        MIPI_SPI_Write(0x01,0x11);
        DelayMs(180);
     


        MIPI_SPI_Write(0x01,0x29);
        DelayMs(50);
        
//		READ_IC(0x05,0x00);
//		ID1_READ =   MIPI_READ_DATA[0];
//		
//		READ_IC(0x0A,0x00);
//		READ_IC(0x0A,0x00);
//		READ_IC(0x0A,0x00);
//		ID1_READ =   MIPI_READ_DATA[0];
//		
//		//#CMD_Page 1
//		MIPI_SPI_Write (4, 0xFF ,0x98, 0x81, 0x01);
//		READ_IC(0x00,0x00);
//		ID1_READ =   MIPI_READ_DATA[0];
//		
//		READ_IC(0x01,0x00);
//		ID2_READ =   MIPI_READ_DATA[0];
//		
//		READ_IC(0x02,0x00);
//		ID3_READ =   MIPI_READ_DATA[0];
		
//		VSPN_ON();    DelayMs(5); 
		
        STM32TOSSD2828_W_COM(0xb7);		//LP DCS mode
        STM32TOSSD2828_W_DATA_16BITS(0x070b);
        DelayMs(10); 
    }
}

/////////RGB code use
void M1651_OTA5601_INIT(void)
{
}

#endif



#ifdef	V480x800_HX8369

u16 CONFIG_SYS_SET[10]={MIPI_ON,RGB_mode,BGR,0};
////{      ////KGM1197C0
u16 MIPI_SET_NOKIA_GPM1461A[3] = {480,11,4};
///////////HS_speed,LP,lane
u16 TIMING_SET_NOKIA_GPM1461A[15] = {60,720,1280,60,60,19,20,10,4,1,0,0,1};     ///////,MAX PCLK =160MHZ
/////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
////} 


u8 PIC_SET_CACHE_nokia[PIC_SAVE_NUM_MAX][3]=
{
{0,0,0},

//{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
{PIC_RED,1,0},      /////纯红画面			//////YUV16BIT-OK,YUV8BIT-OK
{PIC_GREEN,0,0},     /////纯绿				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_BLUE,1,0},     /////纯蓝				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_WHITE,1,0},	/////纯白				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_BLACK,0,0},	//////纯黑				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_CT_center,0,255},			////////CT 	中心点块颜色255			//////YUV16BIT-OK,YUV8BIT-OK
{PIC_checkerboard,1,0},		//////6x6棋盘格		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2V1H---DOT  //////YUV16BIT-OK,YUV8BIT-NG
{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅

//{PIC_BORDER,1,0},	/////边框画面
//{PIC_CT_center,1,0},			////////CT 	中心点黑块，


//{PIC_RED,1,0},      /////纯红画面
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_BLUE,1,0},     /////纯蓝
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_GRAY,0,90},     /////纯灰---灰阶90
//{PIC_GRAY,0,60},     /////纯灰---灰阶60
//{PIC_GRAY,0,30},     /////纯灰---灰阶30
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_BLACK,0,0},	//////纯黑
//{PIC_WHITE,1,0},	/////纯白
//{PIC_CT,1,0},		////crosstalk
//{PIC_colorbar_V,1,0},	//////垂直彩条
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_GRAY_H,1,0},			//////灰阶水平渐变
//{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
//{PIC_Flicker_H,1,0},		/////flicker  column
//{PIC_Flicker_V,1,0},		/////flicker   column
//{PIC_Flicker_H_DOT,1,0},	////flicker   点列翻转
//{PIC_RED_H,1,0},			/////红色水平渐变
//{PIC_RED_V,1,0},			////红色垂直渐变
//{PIC_GREEN_H,1,0},			/////绿色水平渐变
//{PIC_GREEN_V,1,0},			/////绿色垂直渐变
//{PIC_BLUE_H,1,0},			/////蓝色水平渐变
//{PIC_BLUE_V,1,0},			/////蓝色垂直渐变

//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式Column
//{PIC_Dot_Flicker_A,1,127},	////flicker	-----127灰，模式2V1H
//{PIC_Dot_Flicker_A,2,127},	////flicker	-----127灰，模式1Dot
//{PIC_Dot_Flicker_A,3,127},	////flicker	-----127灰，模式1V2H
//{PIC_Dot_Flicker_A,4,127},	////flicker	----127灰，模式1+2V1H
//{PIC_Dot_Flicker_A,5,127},	////flicker	-----127灰，模式1+1V2H
//{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2Dot
//{PIC_Dot_Flicker_A,7,127},	////flicker	-----127灰，模式2V1H---DOT
//{PIC_Dot_Flicker_B,1,0},	////两点flicker	
//{PIC_Dot_Flicker_C,1,0},	////两点flicker	

//{PIC_CT_center,1,0},			////////CT 	中心点黑块，
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅
};

//////MIPI CODE USE
void MIPI_LCD_INITION(void)
{

}

/////////RGB code use
void M1651_OTA5601_INIT(void)
{
}

#endif

#ifdef KGM1680A0_LVDS_1024X600


u16 CONFIG_SYS_SET[10]={MIPI_OFF,RGB_mode,BGR,0};
////{      ////KGM1680A0
u16 MIPI_SET_NOKIA_GPM1461A[3] = {480,8,4};
///////////HS_speed,LP,lane
u16 TIMING_SET_NOKIA_GPM1461A[15] = {60,1024,600,128,88,25,25,20,4,1,0,0,1};     ///////,MAX PCLK =160MHZ
/////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
////} 

u8 PIC_SET_CACHE_nokia[PIC_SAVE_NUM_MAX][3]=
{
{0,0,0},

//////----------CT  use------------
//{PIC_CT_center,0,0},			////////CT 	中心点块颜色0
//{PIC_GRAY,0,128},     /////纯灰---灰阶128	
//{PIC_WHITE,1,0},	/////纯白
///////-------------------------


//////----------RELEASE  use------------
//{PIC_BLUE,1,0},
//{PIC_GREEN,0,0},     /////纯绿				
//{PIC_RED,1,0},
//{PIC_WHITE,1,0},	/////纯白				
//{PIC_GRAY,0,128},     /////纯灰---灰阶128		
//{PIC_BLACK,0,0},	//////纯黑				
//{PIC_colorbar_V,1,0},	//////垂直彩条		
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_checkerboard,1,0},		//////6x6棋盘格		
//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式2V1H---DOT 
//{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅	
///////-------------------------


//////----------GAMMA  use------------
//{PIC_colorbar_H,1,0},	/////水平彩条
///////-------------------------


//{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_BLUE,1,0},
////{PIC_RED,1,0},      /////纯红画面			//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_GREEN,0,0},     /////纯绿				//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_RED,1,0},
//{PIC_BLUE,1,0},     /////纯蓝				//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_WHITE,1,0},	/////纯白				//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_BLACK,0,0},	//////纯黑				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_BORDER,1,0},	/////边框画面
{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_CT_center,0,255},			////////CT 	中心点块颜色255			//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_checkerboard,1,0},		//////6x6棋盘格		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式2V1H---DOT  //////YUV16BIT-OK,YUV8BIT-NG
//{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_checkerboard,1,0},		//////6x6棋盘格
{PIC_FROM_SD,2,2},			////SD卡图片，第二幅

//{PIC_BORDER,1,0},	/////边框画面
//{PIC_CT_center,1,0},			////////CT 	中心点黑块，


//{PIC_RED,1,0},      /////纯红画面
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_BLUE,1,0},     /////纯蓝
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_GRAY,0,90},     /////纯灰---灰阶90
//{PIC_GRAY,0,60},     /////纯灰---灰阶60
//{PIC_GRAY,0,30},     /////纯灰---灰阶30
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_BLACK,0,0},	//////纯黑
//{PIC_WHITE,1,0},	/////纯白
//{PIC_CT,1,0},		////crosstalk
//{PIC_colorbar_V,1,0},	//////垂直彩条
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_GRAY_H,1,0},			//////灰阶水平渐变
//{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
//{PIC_Flicker_H,1,0},		/////flicker  column
//{PIC_Flicker_V,1,0},		/////flicker   column
//{PIC_Flicker_H_DOT,1,0},	////flicker   点列翻转
//{PIC_RED_H,1,0},			/////红色水平渐变
//{PIC_RED_V,1,0},			////红色垂直渐变
//{PIC_GREEN_H,1,0},			/////绿色水平渐变
//{PIC_GREEN_V,1,0},			/////绿色垂直渐变
//{PIC_BLUE_H,1,0},			/////蓝色水平渐变
//{PIC_BLUE_V,1,0},			/////蓝色垂直渐变

//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式Column
//{PIC_Dot_Flicker_A,1,127},	////flicker	-----127灰，模式2V1H
//{PIC_Dot_Flicker_A,2,127},	////flicker	-----127灰，模式1Dot
//{PIC_Dot_Flicker_A,3,127},	////flicker	-----127灰，模式1V2H
//{PIC_Dot_Flicker_A,4,127},	////flicker	----127灰，模式1+2V1H
//{PIC_Dot_Flicker_A,5,127},	////flicker	-----127灰，模式1+1V2H
//{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2Dot
//{PIC_Dot_Flicker_A,7,127},	////flicker	-----127灰，模式2V1H---DOT
//{PIC_Dot_Flicker_B,1,0},	////两点flicker	
//{PIC_Dot_Flicker_C,1,0},	////两点flicker	

//{PIC_CT_center,1,0},			////////CT 	中心点黑块，
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅
};

//////MIPI CODE USE
void MIPI_LCD_INITION(void)
{
	
}

/////////RGB code use
void M1651_OTA5601_INIT(void)
{
}

void VCOM_set(u8 vcom)
{

}
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //此處需根據不同型號進行OTP流程修改
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{

}
#endif

#ifdef DEMO_107_LVDS_1280X720


u16 CONFIG_SYS_SET[10]={MIPI_OFF,RGB_mode,BGR,LVDS_8BIT};
////{      ////KGM1680A0
u16 MIPI_SET_NOKIA_GPM1461A[3] = {480,8,4};
///////////HS_speed,LP,lane
u16 TIMING_SET_NOKIA_GPM1461A[15] = {60,1280,720,128,88,25,25,20,4,1,0,0,1};     ///////,MAX PCLK =160MHZ
/////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
////} 

u8 PIC_SET_CACHE_nokia[PIC_SAVE_NUM_MAX][3]=
{
{0,0,0},

//////----------CT  use------------
//{PIC_CT_center,0,0},			////////CT 	中心点块颜色0
//{PIC_GRAY,0,128},     /////纯灰---灰阶128	
//{PIC_WHITE,1,0},	/////纯白
///////-------------------------


//////----------RELEASE  use------------
{PIC_BLUE,1,0},
//{PIC_GREEN,0,0},     /////纯绿				
//{PIC_RED,1,0},
{PIC_WHITE,1,0},	/////纯白				
//{PIC_GRAY,0,128},     /////纯灰---灰阶128		
//{PIC_BLACK,0,0},	//////纯黑				
//{PIC_colorbar_V,1,0},	//////垂直彩条		
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_checkerboard,1,0},		//////6x6棋盘格		
//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式2V1H---DOT 
//{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅	
///////-------------------------


//////----------GAMMA  use------------
//{PIC_colorbar_H,1,0},	/////水平彩条
///////-------------------------


//{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_BLUE,1,0},
////{PIC_RED,1,0},      /////纯红画面			//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_GREEN,0,0},     /////纯绿				//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_RED,1,0},
//{PIC_BLUE,1,0},     /////纯蓝				//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_WHITE,1,0},	/////纯白				//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_BLACK,0,0},	//////纯黑				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_CT_center,0,255},			////////CT 	中心点块颜色255			//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_checkerboard,1,0},		//////6x6棋盘格		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式2V1H---DOT  //////YUV16BIT-OK,YUV8BIT-NG
//{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_checkerboard,1,0},		//////6x6棋盘格
{PIC_FROM_SD,2,2},			////SD卡图片，第二幅

//{PIC_BORDER,1,0},	/////边框画面
//{PIC_CT_center,1,0},			////////CT 	中心点黑块，


//{PIC_RED,1,0},      /////纯红画面
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_BLUE,1,0},     /////纯蓝
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_GRAY,0,90},     /////纯灰---灰阶90
//{PIC_GRAY,0,60},     /////纯灰---灰阶60
//{PIC_GRAY,0,30},     /////纯灰---灰阶30
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_BLACK,0,0},	//////纯黑
//{PIC_WHITE,1,0},	/////纯白
//{PIC_CT,1,0},		////crosstalk
//{PIC_colorbar_V,1,0},	//////垂直彩条
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_GRAY_H,1,0},			//////灰阶水平渐变
//{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
//{PIC_Flicker_H,1,0},		/////flicker  column
//{PIC_Flicker_V,1,0},		/////flicker   column
//{PIC_Flicker_H_DOT,1,0},	////flicker   点列翻转
//{PIC_RED_H,1,0},			/////红色水平渐变
//{PIC_RED_V,1,0},			////红色垂直渐变
//{PIC_GREEN_H,1,0},			/////绿色水平渐变
//{PIC_GREEN_V,1,0},			/////绿色垂直渐变
//{PIC_BLUE_H,1,0},			/////蓝色水平渐变
//{PIC_BLUE_V,1,0},			/////蓝色垂直渐变

//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式Column
//{PIC_Dot_Flicker_A,1,127},	////flicker	-----127灰，模式2V1H
//{PIC_Dot_Flicker_A,2,127},	////flicker	-----127灰，模式1Dot
//{PIC_Dot_Flicker_A,3,127},	////flicker	-----127灰，模式1V2H
//{PIC_Dot_Flicker_A,4,127},	////flicker	----127灰，模式1+2V1H
//{PIC_Dot_Flicker_A,5,127},	////flicker	-----127灰，模式1+1V2H
//{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2Dot
//{PIC_Dot_Flicker_A,7,127},	////flicker	-----127灰，模式2V1H---DOT
//{PIC_Dot_Flicker_B,1,0},	////两点flicker	
//{PIC_Dot_Flicker_C,1,0},	////两点flicker	

//{PIC_CT_center,1,0},			////////CT 	中心点黑块，
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅
};

//////MIPI CODE USE
void MIPI_LCD_INITION(void)
{
	
}


	
/////////RGB code use
void M1651_OTA5601_INIT(void)
{
}
void VCOM_set(u8 vcom)
{

}
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //此處需根據不同型號進行OTP流程修改
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{

}

#endif


#ifdef KGM1238A0_LVDS_1024X600


u16 CONFIG_SYS_SET[10]={MIPI_OFF,RGB_mode,BGR,LVDS_6BIT};
////{      ////KGM1680A0
u16 MIPI_SET_NOKIA_GPM1461A[3] = {480,8,4};
///////////HS_speed,LP,lane
u16 TIMING_SET_NOKIA_GPM1461A[15] = {60,1024,600,128,88,25,25,20,4,1,0,0,1};     ///////,MAX PCLK =160MHZ
/////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
////} 

u8 PIC_SET_CACHE_nokia[PIC_SAVE_NUM_MAX][3]=
{
{0,0,0},

//////----------CT  use------------
//{PIC_CT_center,0,0},			////////CT 	中心点块颜色0
//{PIC_GRAY,0,128},     /////纯灰---灰阶128	
//{PIC_WHITE,1,0},	/////纯白
///////-------------------------


//////----------RELEASE  use------------
{PIC_FROM_SD,1,2},			////SD卡图片，第一幅	
{PIC_BORDER,1,0},	/////边框画面
{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
{PIC_BLUE,1,0},
{PIC_GREEN,0,0},     /////纯绿				
{PIC_RED,1,0},
{PIC_WHITE,1,0},	/////纯白	
{PIC_FROM_SD,2,1},			////SD卡图片，第一幅

////{PIC_GRAY,0,128},     /////纯灰---灰阶128		
//{PIC_BLACK,0,0},	//////纯黑				
//{PIC_colorbar_V,1,0},	//////垂直彩条		
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_BIT_COLOR_H,1,0},    //////H侧颜色条。。进行bit位颜色检测
//{PIC_BIT_COLOR_V,1,0},    //////V侧颜色条。。进行bit位颜色检测
////{PIC_BORDER,1,0},	/////边框画面
////{PIC_checkerboard,1,0},		//////6x6棋盘格		
//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式2V1H---DOT 
//{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅	
///////-------------------------


////////----------GAMMA  use------------
////{PIC_colorbar_H,1,0},	/////水平彩条
/////////-------------------------


////{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-NG,YUV8BIT-NG
////{PIC_GREEN,0,0},     /////纯绿
////{PIC_GRAY,0,128},     /////纯灰---灰阶128
////{PIC_BLUE,1,0},
//////{PIC_RED,1,0},      /////纯红画面			//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_GREEN,0,0},     /////纯绿				//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_RED,1,0},
////{PIC_BLUE,1,0},     /////纯蓝				//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_WHITE,1,0},	/////纯白				//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_BLACK,0,0},	//////纯黑				//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_BORDER,1,0},	/////边框画面
////{PIC_colorbar_H,1,0},	/////水平彩条
////{PIC_CT_center,0,255},			////////CT 	中心点块颜色255			//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_checkerboard,1,0},		//////6x6棋盘格		//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式2V1H---DOT  //////YUV16BIT-OK,YUV8BIT-NG
////{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
////{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅

////{PIC_BORDER,1,0},	/////边框画面
////{PIC_CT_center,1,0},			////////CT 	中心点黑块，


//{PIC_RED,1,0},      /////纯红画面
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_BLUE,1,0},     /////纯蓝
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_GRAY,0,90},     /////纯灰---灰阶90
//{PIC_GRAY,0,60},     /////纯灰---灰阶60
//{PIC_GRAY,0,30},     /////纯灰---灰阶30
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_BLACK,0,0},	//////纯黑
//{PIC_WHITE,1,0},	/////纯白
//{PIC_CT,1,0},		////crosstalk
//{PIC_colorbar_V,1,0},	//////垂直彩条
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_GRAY_H,1,0},			//////灰阶水平渐变
//{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
//{PIC_Flicker_H,1,0},		/////flicker  column
//{PIC_Flicker_V,1,0},		/////flicker   column
//{PIC_Flicker_H_DOT,1,0},	////flicker   点列翻转
//{PIC_RED_H,1,0},			/////红色水平渐变
//{PIC_RED_V,1,0},			////红色垂直渐变
//{PIC_GREEN_H,1,0},			/////绿色水平渐变
//{PIC_GREEN_V,1,0},			/////绿色垂直渐变
//{PIC_BLUE_H,1,0},			/////蓝色水平渐变
//{PIC_BLUE_V,1,0},			/////蓝色垂直渐变

//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式Column
//{PIC_Dot_Flicker_A,1,127},	////flicker	-----127灰，模式2V1H
//{PIC_Dot_Flicker_A,2,127},	////flicker	-----127灰，模式1Dot
//{PIC_Dot_Flicker_A,3,127},	////flicker	-----127灰，模式1V2H
//{PIC_Dot_Flicker_A,4,127},	////flicker	----127灰，模式1+2V1H
//{PIC_Dot_Flicker_A,5,127},	////flicker	-----127灰，模式1+1V2H
//{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2Dot
//{PIC_Dot_Flicker_A,7,127},	////flicker	-----127灰，模式2V1H---DOT
//{PIC_Dot_Flicker_B,1,0},	////两点flicker	
//{PIC_Dot_Flicker_C,1,0},	////两点flicker	

//{PIC_CT_center,1,0},			////////CT 	中心点黑块，
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅
};

//////MIPI CODE USE
void MIPI_LCD_INITION(void)
{
	
}


	
/////////RGB code use
void M1651_OTA5601_INIT(void)
{
}
void VCOM_set(u8 vcom)
{

}
/*******************************************************************************
* Function Name  : void MTP(void)
* Description    : otp -> vcom     //此處需根據不同型號進行OTP流程修改
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/  
void MTP(void)
{

    
    
    
    
    
    
    
    
}

#endif





#ifdef GPM1651A0_YUV16BIT


u16 CONFIG_SYS_SET[10]={MIPI_OFF,YUV_16BIT,BGR,0};
//////{      ////M1651A0   YUV16BIT
u16 MIPI_SET_NOKIA_GPM1461A[3] = {480,11,4};
///////////HS_speed,LP,lane
u16 TIMING_SET_NOKIA_GPM1461A[15] = {60,640,480,31,25,20,20,1,1,1,0,0,1};     ///////,MAX PCLK =160MHZ
/////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
////} 

u8 PIC_SET_CACHE_nokia[PIC_SAVE_NUM_MAX][3]=
{
{PIC_WHITE,0,0},

//{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_GREEN,0,0},     /////纯绿
////{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_RED,1,0},      /////纯红画面			//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_GREEN,0,0},     /////纯绿				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_BLUE,1,0},     /////纯蓝				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_WHITE,1,0},	/////纯白				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_BLACK,0,0},	//////纯黑				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_CT_center,255,0},			////////CT 	中心点块颜色255			//////YUV16BIT-OK,YUV8BIT-OK
{PIC_checkerboard,1,0},		//////6x6棋盘格		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2V1H---DOT  //////YUV16BIT-OK,YUV8BIT-NG
{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅



////{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-NG,YUV8BIT-NG
////{PIC_GREEN,0,0},     /////纯绿
////{PIC_GRAY,0,128},     /////纯灰---灰阶128
////{PIC_BLUE,1,0},
//////{PIC_RED,1,0},      /////纯红画面			//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_GREEN,0,0},     /////纯绿				//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_RED,1,0},
////{PIC_BLUE,1,0},     /////纯蓝				//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_WHITE,1,0},	/////纯白				//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_BLACK,0,0},	//////纯黑				//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_BORDER,1,0},	/////边框画面
////{PIC_colorbar_H,1,0},	/////水平彩条
////{PIC_CT_center,0,255},			////////CT 	中心点块颜色255			//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_checkerboard,1,0},		//////6x6棋盘格		//////YUV16BIT-OK,YUV8BIT-OK
////{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式2V1H---DOT  //////YUV16BIT-OK,YUV8BIT-NG
////{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
////{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅

//{PIC_BORDER,1,0},	/////边框画面
//{PIC_CT_center,1,0},			////////CT 	中心点黑块，


//{PIC_RED,1,0},      /////纯红画面
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_BLUE,1,0},     /////纯蓝
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_GRAY,0,90},     /////纯灰---灰阶90
//{PIC_GRAY,0,60},     /////纯灰---灰阶60
//{PIC_GRAY,0,30},     /////纯灰---灰阶30
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_BLACK,0,0},	//////纯黑
//{PIC_WHITE,1,0},	/////纯白
//{PIC_CT,1,0},		////crosstalk
//{PIC_colorbar_V,1,0},	//////垂直彩条
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_GRAY_H,1,0},			//////灰阶水平渐变
//{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
//{PIC_Flicker_H,1,0},		/////flicker  column
//{PIC_Flicker_V,1,0},		/////flicker   column
//{PIC_Flicker_H_DOT,1,0},	////flicker   点列翻转
//{PIC_RED_H,1,0},			/////红色水平渐变
//{PIC_RED_V,1,0},			////红色垂直渐变
//{PIC_GREEN_H,1,0},			/////绿色水平渐变
//{PIC_GREEN_V,1,0},			/////绿色垂直渐变
//{PIC_BLUE_H,1,0},			/////蓝色水平渐变
//{PIC_BLUE_V,1,0},			/////蓝色垂直渐变

//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式Column
//{PIC_Dot_Flicker_A,1,127},	////flicker	-----127灰，模式2V1H
//{PIC_Dot_Flicker_A,2,127},	////flicker	-----127灰，模式1Dot
//{PIC_Dot_Flicker_A,3,127},	////flicker	-----127灰，模式1V2H
//{PIC_Dot_Flicker_A,4,127},	////flicker	----127灰，模式1+2V1H
//{PIC_Dot_Flicker_A,5,127},	////flicker	-----127灰，模式1+1V2H
//{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2Dot
//{PIC_Dot_Flicker_A,7,127},	////flicker	-----127灰，模式2V1H---DOT
//{PIC_Dot_Flicker_B,1,0},	////两点flicker	
//{PIC_Dot_Flicker_C,1,0},	////两点flicker	

//{PIC_CT_center,1,0},			////////CT 	中心点黑块，
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅
};

//////MIPI CODE USE
void MIPI_LCD_INITION(void)
{
	
}

/////////RGB code use
void M1651_OTA5601_INIT(void)
{

	{
////////YUV 16BIT
	SSD1963GPIOOUTCOM(0xFD00); //SW reset
	SSD1963GPIOOUTCOM(0x0200);
	DelayMs(100);
		
	SSD1963GPIOOUTCOM(0xFDFF); //CMD enable
	SSD1963GPIOOUTCOM(0x8156);
	SSD1963GPIOOUTCOM(0x8201);
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFD00);//
	SSD1963GPIOOUTCOM(0x1801);//YUV16 mode
//	SSD1963GPIOOUTCOM(0x40E6);//Vcom  (0xBB)
//	SSD1963GPIOOUTCOM(0x4420);//CMD2
	SSD1963GPIOOUTCOM(0x4530);//6 line dot inversion
	SSD1963GPIOOUTCOM(0x1995);//VBLK
	SSD1963GPIOOUTCOM(0x1A27);//HBLK
	SSD1963GPIOOUTCOM(0x0441);//YUV Format setting  Matrix_A_0x0441/Matrix_B_0x0445
	DelayMs(10);

	SSD1963GPIOOUTCOM(0xFDC5);
	SSD1963GPIOOUTCOM(0x821C);
	SSD1963GPIOOUTCOM(0xA29A);//VGH=15V, VGL=-14V
	SSD1963GPIOOUTCOM(0xD194);
	DelayMs(10);	


	SSD1963GPIOOUTCOM(0xFDD8);
	SSD1963GPIOOUTCOM(0x011F); // GVDDP Voltage Setting   25
	SSD1963GPIOOUTCOM(0x021F); // GVDDN Voltage Setting   25
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDC1);
	SSD1963GPIOOUTCOM(0x9142);
	DelayMs(10);	

	SSD1963GPIOOUTCOM(0xFDC0); //YUV page shift
	SSD1963GPIOOUTCOM(0xA101);
	SSD1963GPIOOUTCOM(0xA21C);
	SSD1963GPIOOUTCOM(0xA30A);
	SSD1963GPIOOUTCOM(0xA434);
	SSD1963GPIOOUTCOM(0xA500);
	SSD1963GPIOOUTCOM(0xA609);
	SSD1963GPIOOUTCOM(0xA734);
	SSD1963GPIOOUTCOM(0xA800);
	SSD1963GPIOOUTCOM(0xA909);
	SSD1963GPIOOUTCOM(0xAA34);
	DelayMs(10);
	
	SSD1963GPIOOUTCOM(0xFDCE); //YUV page shift
	SSD1963GPIOOUTCOM(0x8114);
	SSD1963GPIOOUTCOM(0x823E);
	SSD1963GPIOOUTCOM(0x833E);
	SSD1963GPIOOUTCOM(0x9107);
	SSD1963GPIOOUTCOM(0x9331);
	SSD1963GPIOOUTCOM(0x9402);
	SSD1963GPIOOUTCOM(0x9507);
	SSD1963GPIOOUTCOM(0x9731);
	SSD1963GPIOOUTCOM(0x9802);
	SSD1963GPIOOUTCOM(0x9907);
	SSD1963GPIOOUTCOM(0x9B31);
	SSD1963GPIOOUTCOM(0x9C02);	
	DelayMs(10);
	
	//Gamma 2.2 balance_140618
	SSD1963GPIOOUTCOM(0xFDE1); //R+
	SSD1963GPIOOUTCOM(0x0175); //255  7F
	SSD1963GPIOOUTCOM(0x0214); //254
	SSD1963GPIOOUTCOM(0x0321); //251
	SSD1963GPIOOUTCOM(0x0422); //247
	SSD1963GPIOOUTCOM(0x0510);
	SSD1963GPIOOUTCOM(0x0611);
	SSD1963GPIOOUTCOM(0x0720);
	SSD1963GPIOOUTCOM(0x0808);
	SSD1963GPIOOUTCOM(0x0908);
	SSD1963GPIOOUTCOM(0x0A08);
	SSD1963GPIOOUTCOM(0x0B0A);
	SSD1963GPIOOUTCOM(0x0C15);
	SSD1963GPIOOUTCOM(0x0D0F);
	SSD1963GPIOOUTCOM(0x0E0C); //16
	SSD1963GPIOOUTCOM(0x0F18); //8   18  +_UP
	SSD1963GPIOOUTCOM(0x100C); //4   1E
	SSD1963GPIOOUTCOM(0x1104); //1   04
	SSD1963GPIOOUTCOM(0x1210); //0   04
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDE2); //R-
	SSD1963GPIOOUTCOM(0x0175);
	SSD1963GPIOOUTCOM(0x0214);
	SSD1963GPIOOUTCOM(0x0321);
	SSD1963GPIOOUTCOM(0x0422);
	SSD1963GPIOOUTCOM(0x0510);
	SSD1963GPIOOUTCOM(0x0611);
	SSD1963GPIOOUTCOM(0x0720);
	SSD1963GPIOOUTCOM(0x0808);
	SSD1963GPIOOUTCOM(0x0908);
	SSD1963GPIOOUTCOM(0x0A08);
	SSD1963GPIOOUTCOM(0x0B0A);
	SSD1963GPIOOUTCOM(0x0C15);
	SSD1963GPIOOUTCOM(0x0D0F);
	SSD1963GPIOOUTCOM(0x0E0C);
	SSD1963GPIOOUTCOM(0x0F18);
	SSD1963GPIOOUTCOM(0x100C);
	SSD1963GPIOOUTCOM(0x1104);
	SSD1963GPIOOUTCOM(0x1210);
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDE3); //G+
	SSD1963GPIOOUTCOM(0x0175); //255  7F
	SSD1963GPIOOUTCOM(0x0214); //254
	SSD1963GPIOOUTCOM(0x0321); //251
	SSD1963GPIOOUTCOM(0x0422); //247
	SSD1963GPIOOUTCOM(0x0510);
	SSD1963GPIOOUTCOM(0x0611);
	SSD1963GPIOOUTCOM(0x0720);
	SSD1963GPIOOUTCOM(0x0808);
	SSD1963GPIOOUTCOM(0x0908);
	SSD1963GPIOOUTCOM(0x0A08);
	SSD1963GPIOOUTCOM(0x0B0A);
	SSD1963GPIOOUTCOM(0x0C15);
	SSD1963GPIOOUTCOM(0x0D0F);
	SSD1963GPIOOUTCOM(0x0E0C); //16
	SSD1963GPIOOUTCOM(0x0F18); //8   18  +_UP
	SSD1963GPIOOUTCOM(0x100C); //4   1E
	SSD1963GPIOOUTCOM(0x1104); //1   04
	SSD1963GPIOOUTCOM(0x1210); //0   04
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDE4); //G-
	SSD1963GPIOOUTCOM(0x0175);
	SSD1963GPIOOUTCOM(0x0214);
	SSD1963GPIOOUTCOM(0x0321);
	SSD1963GPIOOUTCOM(0x0422);
	SSD1963GPIOOUTCOM(0x0510);
	SSD1963GPIOOUTCOM(0x0611);
	SSD1963GPIOOUTCOM(0x0720);
	SSD1963GPIOOUTCOM(0x0808);
	SSD1963GPIOOUTCOM(0x0908);
	SSD1963GPIOOUTCOM(0x0A08);
	SSD1963GPIOOUTCOM(0x0B0A);
	SSD1963GPIOOUTCOM(0x0C15);
	SSD1963GPIOOUTCOM(0x0D0F);
	SSD1963GPIOOUTCOM(0x0E0C);
	SSD1963GPIOOUTCOM(0x0F18);
	SSD1963GPIOOUTCOM(0x100C);
	SSD1963GPIOOUTCOM(0x1104);
	SSD1963GPIOOUTCOM(0x1210);
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDE5); //B+
	SSD1963GPIOOUTCOM(0x0175); //255  7F
	SSD1963GPIOOUTCOM(0x0214); //254
	SSD1963GPIOOUTCOM(0x0321); //251
	SSD1963GPIOOUTCOM(0x0422); //247
	SSD1963GPIOOUTCOM(0x0510);
	SSD1963GPIOOUTCOM(0x0611);
	SSD1963GPIOOUTCOM(0x0720);
	SSD1963GPIOOUTCOM(0x0808);
	SSD1963GPIOOUTCOM(0x0908);
	SSD1963GPIOOUTCOM(0x0A08);
	SSD1963GPIOOUTCOM(0x0B0A);
	SSD1963GPIOOUTCOM(0x0C15);
	SSD1963GPIOOUTCOM(0x0D0F);
	SSD1963GPIOOUTCOM(0x0E0C); //16
	SSD1963GPIOOUTCOM(0x0F18); //8   18  +_UP
	SSD1963GPIOOUTCOM(0x100C); //4   1E
	SSD1963GPIOOUTCOM(0x1104); //1   04
	SSD1963GPIOOUTCOM(0x1210); //0   04
	DelayMs(10);	
	
	SSD1963GPIOOUTCOM(0xFDE6); //B-
	SSD1963GPIOOUTCOM(0x0175);
	SSD1963GPIOOUTCOM(0x0214);
	SSD1963GPIOOUTCOM(0x0321);
	SSD1963GPIOOUTCOM(0x0422);
	SSD1963GPIOOUTCOM(0x0510);
	SSD1963GPIOOUTCOM(0x0611);
	SSD1963GPIOOUTCOM(0x0720);
	SSD1963GPIOOUTCOM(0x0808);
	SSD1963GPIOOUTCOM(0x0908);
	SSD1963GPIOOUTCOM(0x0A08);
	SSD1963GPIOOUTCOM(0x0B0A);
	SSD1963GPIOOUTCOM(0x0C15);
	SSD1963GPIOOUTCOM(0x0D0F);
	SSD1963GPIOOUTCOM(0x0E0C);
	SSD1963GPIOOUTCOM(0x0F18);
	SSD1963GPIOOUTCOM(0x100C);
	SSD1963GPIOOUTCOM(0x1104);
	SSD1963GPIOOUTCOM(0x1210);  				
	DelayMs(10);

	SSD1963GPIOOUTCOM(0xFD00);
	SSD1963GPIOOUTCOM(0x4400);
	DelayMs(10);		
	
	SSD1963GPIOOUTCOM(0xFD00);//Sleep out
	SSD1963GPIOOUTCOM(0x0101);
	DelayMs(180);
 

  	}
}
#endif

#ifdef GPM1651A0_YUV8BIT


u16 CONFIG_SYS_SET[10]={MIPI_OFF,YUV_8BIT,BGR,0};

////{      ////M1651A0  YUV8BIT
u16 MIPI_SET_NOKIA_GPM1461A[3] = {480,11,4};
///////////HS_speed,LP,lane
u16 TIMING_SET_NOKIA_GPM1461A[15] = {46,1280,480,32,36,20,24,2,1,1,0,0,1};     ///////,MAX PCLK =160MHZ
/////////Fram_Feq,HDP,VDP,HBP,HFP,VBP,VFP,HPW,VPW
////} 

u8 PIC_SET_CACHE_nokia[PIC_SAVE_NUM_MAX][3]=
{
{0,0,0},


//{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
{PIC_RED,1,0},      /////纯红画面			//////YUV16BIT-OK,YUV8BIT-OK
{PIC_GREEN,0,0},     /////纯绿				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_BLUE,1,0},     /////纯蓝				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_WHITE,1,0},	/////纯白				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_GRAY,0,128},     /////纯灰---灰阶128		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_BLACK,0,0},	//////纯黑				//////YUV16BIT-OK,YUV8BIT-OK
{PIC_colorbar_V,1,0},	//////垂直彩条		//////YUV16BIT-OK,YUV8BIT-OK
//{PIC_CT_center,255,0},			////////CT 	中心点块颜色255			//////YUV16BIT-OK,YUV8BIT-OK
{PIC_checkerboard,1,0},		//////6x6棋盘格		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2V1H---DOT  //////YUV16BIT-OK,YUV8BIT-NG
{PIC_CT_center,0,0},			////////CT 	中心点块颜色0		//////YUV16BIT-OK,YUV8BIT-OK
{PIC_FROM_SD,2,1},			////SD卡图片，第一幅		//////YUV16BIT-NG,YUV8BIT-NG
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅


//{PIC_BORDER,1,0},	/////边框画面
//{PIC_CT_center,1,0},			////////CT 	中心点黑块，


//{PIC_RED,1,0},      /////纯红画面
//{PIC_GREEN,0,0},     /////纯绿
//{PIC_BLUE,1,0},     /////纯蓝
//{PIC_GRAY,0,128},     /////纯灰---灰阶128
//{PIC_GRAY,0,90},     /////纯灰---灰阶90
//{PIC_GRAY,0,60},     /////纯灰---灰阶60
//{PIC_GRAY,0,30},     /////纯灰---灰阶30
//{PIC_BORDER,1,0},	/////边框画面
//{PIC_BLACK,0,0},	//////纯黑
//{PIC_WHITE,1,0},	/////纯白
//{PIC_CT,1,0},		////crosstalk
//{PIC_colorbar_V,1,0},	//////垂直彩条
//{PIC_colorbar_H,1,0},	/////水平彩条
//{PIC_checkerboard,1,0},		//////6x6棋盘格
//{PIC_GRAY_H,1,0},			//////灰阶水平渐变
//{PIC_GRAY_V,1,0},			/////灰阶垂直渐变
//{PIC_Flicker_H,1,0},		/////flicker  column
//{PIC_Flicker_V,1,0},		/////flicker   column
//{PIC_Flicker_H_DOT,1,0},	////flicker   点列翻转
//{PIC_RED_H,1,0},			/////红色水平渐变
//{PIC_RED_V,1,0},			////红色垂直渐变
//{PIC_GREEN_H,1,0},			/////绿色水平渐变
//{PIC_GREEN_V,1,0},			/////绿色垂直渐变
//{PIC_BLUE_H,1,0},			/////蓝色水平渐变
//{PIC_BLUE_V,1,0},			/////蓝色垂直渐变

//{PIC_Dot_Flicker_A,0,127},	////flicker	-----127灰，模式Column
//{PIC_Dot_Flicker_A,1,127},	////flicker	-----127灰，模式2V1H
//{PIC_Dot_Flicker_A,2,127},	////flicker	-----127灰，模式1Dot
//{PIC_Dot_Flicker_A,3,127},	////flicker	-----127灰，模式1V2H
//{PIC_Dot_Flicker_A,4,127},	////flicker	----127灰，模式1+2V1H
//{PIC_Dot_Flicker_A,5,127},	////flicker	-----127灰，模式1+1V2H
//{PIC_Dot_Flicker_A,6,127},	////flicker	-----127灰，模式2Dot
//{PIC_Dot_Flicker_A,7,127},	////flicker	-----127灰，模式2V1H---DOT
//{PIC_Dot_Flicker_B,1,0},	////两点flicker	
//{PIC_Dot_Flicker_C,1,0},	////两点flicker	

//{PIC_CT_center,1,0},			////////CT 	中心点黑块，
//{PIC_FROM_SD,2,1},			////SD卡图片，第一幅
//{PIC_FROM_SD,2,2},			////SD卡图片，第二幅
};

//////MIPI CODE USE		////使用MIPI接口类使用，
void MIPI_LCD_INITION(void)
{
	
}

/////////RGB code use   /////若是使用非MIPI接口，则从此函数入口编写初始化代码，現有16bit送入、8bit送入、9bit送入三种方式函数
void M1651_OTA5601_INIT(void)
{
	{
////////YUV 8BIT
	SSD1963GPIOOUTCOM(0xFD00); //SW reset
	SSD1963GPIOOUTCOM(0x0200);
	DelayMs(100);
		
	SSD1963GPIOOUTCOM(0xFDFF); //CMD enable
	SSD1963GPIOOUTCOM(0x8156);
	SSD1963GPIOOUTCOM(0x8201);
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFD00);//
	SSD1963GPIOOUTCOM(0x1802);//YUV16 mode
//	SSD1963GPIOOUTCOM(0x40E6);//Vcom  (0xBB)
	SSD1963GPIOOUTCOM(0x4420);//CMD2
	SSD1963GPIOOUTCOM(0x4530);//6 line dot inversion
	SSD1963GPIOOUTCOM(0x1995);//VBLK
	SSD1963GPIOOUTCOM(0x1A27);//HBLK
	SSD1963GPIOOUTCOM(0x04E1);//YUV Format setting  Matrix_A_0x0441/Matrix_B_0x0445
	DelayMs(10);

	SSD1963GPIOOUTCOM(0xFDC5);
	SSD1963GPIOOUTCOM(0x821C);
	SSD1963GPIOOUTCOM(0xA29A);//VGH=15V, VGL=-14V
	SSD1963GPIOOUTCOM(0xD194);
	DelayMs(10);	


	SSD1963GPIOOUTCOM(0xFDD8);
	SSD1963GPIOOUTCOM(0x0129); // GVDDP Voltage Setting   25
	SSD1963GPIOOUTCOM(0x0229); // GVDDN Voltage Setting   25
//	SSD1963GPIOOUTCOM(0x011F); // GVDDP Voltage Setting   25
//	SSD1963GPIOOUTCOM(0x021F); // GVDDN Voltage Setting   25
	DelayMs(10);
		
//	SSD1963GPIOOUTCOM(0xFDC1);
//	SSD1963GPIOOUTCOM(0x9142);
	SSD1963GPIOOUTCOM(0xFDC4);
	SSD1963GPIOOUTCOM(0x8205);
	SSD1963GPIOOUTCOM(0xFDC1);
	SSD1963GPIOOUTCOM(0x9102);
	DelayMs(10);	

	SSD1963GPIOOUTCOM(0xFDC0); //YUV page shift
	SSD1963GPIOOUTCOM(0xA101);
	SSD1963GPIOOUTCOM(0xA21C);
	SSD1963GPIOOUTCOM(0xA30A);
	SSD1963GPIOOUTCOM(0xA434);
	SSD1963GPIOOUTCOM(0xA500);
	SSD1963GPIOOUTCOM(0xA609);
	SSD1963GPIOOUTCOM(0xA734);
	SSD1963GPIOOUTCOM(0xA800);
	SSD1963GPIOOUTCOM(0xA909);
	SSD1963GPIOOUTCOM(0xAA34);
	DelayMs(10);
	
	SSD1963GPIOOUTCOM(0xFDCE); //YUV page shift
	SSD1963GPIOOUTCOM(0x8114);
	SSD1963GPIOOUTCOM(0x823E);
	SSD1963GPIOOUTCOM(0x833E);
	SSD1963GPIOOUTCOM(0x9107);
	SSD1963GPIOOUTCOM(0x9331);
	SSD1963GPIOOUTCOM(0x9402);
	SSD1963GPIOOUTCOM(0x9507);
	SSD1963GPIOOUTCOM(0x9731);
	SSD1963GPIOOUTCOM(0x9802);
	SSD1963GPIOOUTCOM(0x9907);
	SSD1963GPIOOUTCOM(0x9B31);
	SSD1963GPIOOUTCOM(0x9C02);	
	DelayMs(10);
	
	//Gamma 2.2 balance_140618
	SSD1963GPIOOUTCOM(0xFDE1); //R+
	SSD1963GPIOOUTCOM(0x0168); //255  7F
	SSD1963GPIOOUTCOM(0x020C); //254
	SSD1963GPIOOUTCOM(0x031A); //251
	SSD1963GPIOOUTCOM(0x0425); //247
	SSD1963GPIOOUTCOM(0x050E);
	SSD1963GPIOOUTCOM(0x0610);
	SSD1963GPIOOUTCOM(0x071D);
	SSD1963GPIOOUTCOM(0x0807);
	SSD1963GPIOOUTCOM(0x0906);
	SSD1963GPIOOUTCOM(0x0A09);
	SSD1963GPIOOUTCOM(0x0B0B);
	SSD1963GPIOOUTCOM(0x0C16);
	SSD1963GPIOOUTCOM(0x0D11);
	SSD1963GPIOOUTCOM(0x0E10); //16
	SSD1963GPIOOUTCOM(0x0F10); //8   18  +_UP
	SSD1963GPIOOUTCOM(0x100C); //4   1E
	SSD1963GPIOOUTCOM(0x1104); //1   04
	SSD1963GPIOOUTCOM(0x1210); //0   04
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDE2); //R-
	SSD1963GPIOOUTCOM(0x0168);
	SSD1963GPIOOUTCOM(0x020C);
	SSD1963GPIOOUTCOM(0x031A);
	SSD1963GPIOOUTCOM(0x0425);
	SSD1963GPIOOUTCOM(0x050E);
	SSD1963GPIOOUTCOM(0x0610);
	SSD1963GPIOOUTCOM(0x071D);
	SSD1963GPIOOUTCOM(0x0807);
	SSD1963GPIOOUTCOM(0x0906);
	SSD1963GPIOOUTCOM(0x0A09);
	SSD1963GPIOOUTCOM(0x0B0B);
	SSD1963GPIOOUTCOM(0x0C16);
	SSD1963GPIOOUTCOM(0x0D11);
	SSD1963GPIOOUTCOM(0x0E10);
	SSD1963GPIOOUTCOM(0x0F10);
	SSD1963GPIOOUTCOM(0x100C);
	SSD1963GPIOOUTCOM(0x1104);
	SSD1963GPIOOUTCOM(0x1210);
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDE3); //G+
	SSD1963GPIOOUTCOM(0x016D); //255  7F
	SSD1963GPIOOUTCOM(0x020C); //254
	SSD1963GPIOOUTCOM(0x031A); //251
	SSD1963GPIOOUTCOM(0x0425); //247
	SSD1963GPIOOUTCOM(0x050E);
	SSD1963GPIOOUTCOM(0x0610);
	SSD1963GPIOOUTCOM(0x071D);
	SSD1963GPIOOUTCOM(0x0807);
	SSD1963GPIOOUTCOM(0x0906);
	SSD1963GPIOOUTCOM(0x0A09);
	SSD1963GPIOOUTCOM(0x0B0B);
	SSD1963GPIOOUTCOM(0x0C16);
	SSD1963GPIOOUTCOM(0x0D11);
	SSD1963GPIOOUTCOM(0x0E10); //16
	SSD1963GPIOOUTCOM(0x0F10); //8   18  +_UP
	SSD1963GPIOOUTCOM(0x100C); //4   1E
	SSD1963GPIOOUTCOM(0x1104); //1   04
	SSD1963GPIOOUTCOM(0x1210); //0   04
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDE4); //G-
	SSD1963GPIOOUTCOM(0x016D);
	SSD1963GPIOOUTCOM(0x020C);
	SSD1963GPIOOUTCOM(0x031A);
	SSD1963GPIOOUTCOM(0x0425);
	SSD1963GPIOOUTCOM(0x050E);
	SSD1963GPIOOUTCOM(0x0610);
	SSD1963GPIOOUTCOM(0x071D);
	SSD1963GPIOOUTCOM(0x0807);
	SSD1963GPIOOUTCOM(0x0906);
	SSD1963GPIOOUTCOM(0x0A09);
	SSD1963GPIOOUTCOM(0x0B0B);
	SSD1963GPIOOUTCOM(0x0C16);
	SSD1963GPIOOUTCOM(0x0D11);
	SSD1963GPIOOUTCOM(0x0E10);
	SSD1963GPIOOUTCOM(0x0F10);
	SSD1963GPIOOUTCOM(0x100C);
	SSD1963GPIOOUTCOM(0x1104);
	SSD1963GPIOOUTCOM(0x1210);
	DelayMs(10);
		
	SSD1963GPIOOUTCOM(0xFDE5); //B+
	SSD1963GPIOOUTCOM(0x0170); //255  7F
	SSD1963GPIOOUTCOM(0x020C); //254
	SSD1963GPIOOUTCOM(0x031A); //251
	SSD1963GPIOOUTCOM(0x0425); //247
	SSD1963GPIOOUTCOM(0x050E);
	SSD1963GPIOOUTCOM(0x0610);
	SSD1963GPIOOUTCOM(0x071D);
	SSD1963GPIOOUTCOM(0x0807);
	SSD1963GPIOOUTCOM(0x0906);
	SSD1963GPIOOUTCOM(0x0A09);
	SSD1963GPIOOUTCOM(0x0B0B);
	SSD1963GPIOOUTCOM(0x0C16);
	SSD1963GPIOOUTCOM(0x0D11);
	SSD1963GPIOOUTCOM(0x0E10); //16
	SSD1963GPIOOUTCOM(0x0F10); //8   18  +_UP
	SSD1963GPIOOUTCOM(0x100C); //4   1E
	SSD1963GPIOOUTCOM(0x1104); //1   04
	SSD1963GPIOOUTCOM(0x1210); //0   04
	DelayMs(10);	
	
	SSD1963GPIOOUTCOM(0xFDE6); //B-
	SSD1963GPIOOUTCOM(0x0170);
	SSD1963GPIOOUTCOM(0x020C);
	SSD1963GPIOOUTCOM(0x031A);
	SSD1963GPIOOUTCOM(0x0425);
	SSD1963GPIOOUTCOM(0x050E);
	SSD1963GPIOOUTCOM(0x0610);
	SSD1963GPIOOUTCOM(0x071D);
	SSD1963GPIOOUTCOM(0x0807);
	SSD1963GPIOOUTCOM(0x0906);
	SSD1963GPIOOUTCOM(0x0A09);
	SSD1963GPIOOUTCOM(0x0B0B);
	SSD1963GPIOOUTCOM(0x0C16);
	SSD1963GPIOOUTCOM(0x0D11);
	SSD1963GPIOOUTCOM(0x0E10);
	SSD1963GPIOOUTCOM(0x0F10);
	SSD1963GPIOOUTCOM(0x100C);
	SSD1963GPIOOUTCOM(0x1104);
	SSD1963GPIOOUTCOM(0x1210);  				
	DelayMs(10);

//	SSD1963GPIOOUTCOM(0xFD00);
//	SSD1963GPIOOUTCOM(0x4400);
//	DelayMs(10);		
	
	SSD1963GPIOOUTCOM(0xFD00);//Sleep out
	SSD1963GPIOOUTCOM(0x0101);
	DelayMs(180);
 

  	}
}


#endif


////////end;


