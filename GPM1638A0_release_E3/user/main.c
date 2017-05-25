/******************** COPYRIGHT  ********************
* File Name          :main.c
* Author             :RAIO 
* Version            :V1.0 
* Date               :09/28/16 
* Description        : 	
****************************************************/

#include "fsmc_sram.h"
#include "stm32f10x_it.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_gpio.h"


#include "usart.h"
#include "delay.h"
#include "lcd.h"
#include "R5421.h"
#include "G190_demo.h"
#include "R5421_demo.h"
#include "test_code.h"
#include "stdio.h"
#include <string.h>
#include "sdcard.h"
#include "integer.h"
#include "RTC_Time.h"
#include "SPI_Flash.h"

#include "FAT.h"
#include "disp.h"
#include <TSLIIC_REV01.h>
#include "otp.h"



#include "config_hard.h"
#include "lcd1602.h"
#include <GT9271.h>
#include "iic_current.h"
#include "pattern.h"


////////timing  setting
extern u16 TIMING_SET[];  

#define XDP		TIMING_SET[1]
#define YDP		TIMING_SET[2]

#define KEY_BOARD_USE			//////若使用按键盒则启用此定义


time_t current_time;
struct tm time_now;

/* Private typedef -----------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* Private define ------------------------------------------------------------*/
#define BlockSize            512 /* Block Size in Bytes */
#define BufferWordsSize      (BlockSize >> 2)
#define NumberOfBlocks       2  /* For Multi Blocks operation (Read/Write) */
#define MultiBufferWordsSize ((BlockSize * NumberOfBlocks) >> 2)

#define Pic_800_480_start  948224

u8 B1_event_flag = 0;	//B1 按键触发事件
u8 B2_event_flag = 0;	//B2 按键触发事件

u8 B6_event_flag = 0;	//B6 按键触发事件


SD_CardInfo SDCardInfo;
uint32_t Buffer_Block_Tx[BufferWordsSize], Buffer_Block_Rx[BufferWordsSize];
uint32_t Buffer_MultiBlock_Tx[MultiBufferWordsSize], Buffer_MultiBlock_Rx[MultiBufferWordsSize];
volatile TestStatus EraseStatus = FAILED, TransferStatus1 = FAILED, TransferStatus2 = FAILED;
SD_Error Status = SD_OK;

//unsigned short buffer[1600];  //16bit for bin low byte first
unsigned char buffer[5000];  //

unsigned char buffer_data[2]; 

u8 Brightness_level;

char tmp[20];



/* Private function prototypes -----------------------------------------------*/

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void EXTI_Configuration(void);
void KEY_GPIO_Configuration(void);

u32  sd_Capacity;
extern u16 image_count ;
extern u16 image_all ;
/////////////////FOR  SD  use  only
u8   SDConfigFlag = 0;
u8   SDShowFlag=1; 
u8   SDShowTimes = 1;
u8   SD_PIC_NUM_MAX = 0;
void SDPicShow(u8 pic);

void NVIC_Configuration_only_key(void);
void EXTI_Configuration_only_key(void);
void EXTI_Configuration_only_key_DISABLE(void);

extern void Initial_LQ043Y1DX07(void);

void sd_BTE_showpic_256_bin(unsigned short w,unsigned short h,char * fname);
void sd_showpic_256_bin(unsigned short w,unsigned short h,char * fname);
void sd_BTE_showpic_65k_bin(unsigned short w,unsigned short h,char * fname);
void sd_showpic_MPU8_to_65k_bin(unsigned short w,unsigned short h,char * fname);
void sd_showpic_65k_bin(unsigned short w,unsigned short h,char * fname);
void sd_BTE_showpic_16M_mode1_bin(unsigned short w,unsigned short h,char * fname);
void sd_showpic_16M_mode1_bin(unsigned short w,unsigned short h,char * fname);
void sd_showpic_16M_mode2_bin(unsigned short w,unsigned short h,char * fname);

void sd_showpic(unsigned short x,unsigned short y,char * fname);

void detect_button_B1(void);
void detect_button_B2(void);

void D3_timer3_ini_PWM_enable(void);
void D3_timer3_ini_PWM_disable(void);
void D4_timer3_ini_PWM_enable(void);
void D4_timer3_ini_PWM_disable(void);

u8 Program_bin2flash(char * fname);
u8 PageProgram_bin2flash(char * fname);
u8 verify_flash_data(char * fname);
int SYS_BOARD_INITION(void);


/* Private functions ---------------------------------------------------------*/
//    FATFS fs;            // Work area (file system object) for logical drive
//    FIL fsrc, fdst;      // file objects
//    BYTE bufferpic[500];
//	FRESULT res;         // FatFs function common result code
//    UINT br, bw;         // File R/W count

	 
/* Private function prototypes -----------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;


///////=========================================================================================
//int USART_SWITCH(void);
//int Do_usart_cmd(void);

//void LCD_Display_flow(void);
//void DISP_AREA_SET(void);
void PassKey(void);
void SCAN_KEY_ON(void);
void PIP_SWITCH(u8 pic_area_num);
void GET_PIC_MAX_NUM(void);
void KEY_SCAN_CONTROL_A(void);
u8  HorA=0xFF;                                   //HorA --->  0为手动跑画面    ，  !0为自动跑画面
u8 YUV_RGB=YUV_8BIT;		///////YUV模式：1.。。RGB模式：0.。。。
u8 SD_BMP_BGR=BGR;		//////图片的RGB排列：0->RGB,1->BGR,
u8 LVDS_8BIT_USE=1;		/////////lvds使用8bit标志位，0->标示8bit LVDS MODE，1->表示使用6bit LVDS MODE
u8 pic_num=1;      //显示画面计数用
u8 pic_num_string = 0;
extern u8 LCM_TYPE;
extern u8 PIC_SET_CACHE[][3];
u8 PIP_SWITCH_flag=0;
u8 PIC_NUM=9;
extern BYTE pucData_B[1000];
//extern u8 temp[12];   //????ID?????
u16 DELAY_15 = 150;    /////
void SET2DISPLAY(u8 pic_area_num);

extern u8 MIPI_READ_DATA[];
extern u8  Testpar[128];
extern u16 RESULT_AUTO_CHECK_NUM;
extern u32 stm_time_test;
extern char usart_buff[];
//u8 nosdnodelay=0 ;
extern u16 VCOMDC1;     //自动模式起始地址	
extern u8 VCOMDH;   //手动调整起始地址
extern u8 ID_OK;
extern u8 OTP_VALUE1;
extern u8 MTP_OVER;
extern u16 VCOM_default;
extern u16 OTP_ERROR_MESAGE;

u8 MAN_OTP_FLAG=0;
u8 AUTO_OTP_FLAG=0;

extern u16 CONFIG_SYS_SET[10];
extern u16 MIPI_SET_NOKIA_GPM1461A[3];
extern u16 TIMING_SET_NOKIA_GPM1461A[15];


/*******************************************************************************
* Function Name  : main
* Description    : 
* Input          : 
* Output         : 
* Return         : None
*******************************************************************************/
int main(void)
 {
// 	u32 i,j;
// 	u8 tempx;
    u32  sdcap=1;
    u16 mm_KEYB10 = 1 ;
	u16 mm_KEYB1 = 1 ;
	u16 mm_KEYB11 = 1 ;
//    u8 temp[8];
//     u32 test_times=0;
    
 #ifdef DEBUG
 debug();
 #endif 
   		

	SYS_BOARD_INITION();///////系统启动，   
//////%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
        
LCDTest:           
        MTP_OVER=0;     //////开机即需进行OTP标志位
        sdcap = 1;
        MAN_OTP_FLAG=0;    /////进入手动OTP标志位
        AUTO_OTP_FLAG=0;   /////进入自动OTP标志位
        VDD_OFF(); 		///////MIPI芯片上电
		LCM_VDD_OFF();       ////////产品LCD的VDD和IOVDD上电。
        OTP_ERROR_MESAGE=0;
		BL_OFF();     ////背光控制
        LCD1602_PIN_Configuration();		/////初始化1602显示屏。
        LCD1602_Init();             //delay_ms(40);  
         T226_intion();    ////电流监测，，测机上电电流监测。。
		 VDD_config();		////相应电源以及背光IO配置，便于LCD上电。。
		 delay_ms(1);

#ifdef	KEY_BOARD_USE
		EXTI_Configuration_only_key_DISABLE();
        KEY_GPIO_Configuration();KEY_GPIO_Configuration();
////////////等待在此处，进行选择正确的点亮型号，之后才进行点画面流程。。
        KEY_SCAN_CONTROL_A();      
        LCM_CHANGE();  
		R5421_initial();	
#endif

        B6_event_flag=0;
		
        LCD1602_display_Line_A(Line1602_0,0,"                ");       ////clear  当前行  
        LCD1602_display_Line_A(Line1602_1,0,"                ");       ////clear  当前行  
        LCD1602_display_Line(Line1602_0,"starting....");
//		LCD1602_display_Line(Line1602_1,"M1638A0");

//       //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
///////////////////////////////////以上切勿更改。。。。
//////////////////////////////////////////////////////////////////////////////	  
      
//   //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
/////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
		__nop();
        delay_ms(20);    
 		Display_ON();

#ifdef KEY_BOARD_USE
		///////开始按键中断，将第一个键变更功能 为开关机键，实现开关显示的目的。。
        NVIC_Configuration_only_key();
        EXTI_Configuration_only_key();  // Enable  key  Interrupts   
#endif

		BL_OFF();     ////背光控制
		 DelayMs(5); 
        VDD_ON(); 		///////MIPI芯片上电
		LCM_VDD_ON();       ////////产品LCD的VDD和IOVDD上电。
		VSPN_ON();    		//////VSP/VSN上电。。。。
		MTP_5V_ON();		//开启MTP的5V供电
		sdcap = KEY_RED;
		while( sdcap == 0)
		{	sdcap = KEY_RED;	DelayMs(5); LCD1602_display_Line(Line1602_1,"MTP ON");	}	/////利用按键盒红色按键，进行暂停，调整VDD/OTP电压使用。
		MTP_5V_OFF();		//关闭MTP的5V供电
		DelayMs(5); 
		if((B6_event_flag==2))  { B6_event_flag=0;    goto LCDTest;  }  
	    STM32_Init_SSD2828(); 		//////系统对LCD进行输送初始化
		
		sdcap = KEY_AUTO_MODE;
		OTP_VALUE1 = READ_IC_A(0xD9,0x00);           LCD1602_display_hex(Line1602_1,10,OTP_VALUE1);
//		sdcap = READ_IC(0xDC,0x00);LCD1602_display_hex(Line1602_1,10,MIPI_READ_DATA[0]);
////		 sdcap = READ_IC(0xDB,0x00);LCD1602_display_hex(Line1602_0,10,MIPI_READ_DATA[0]);
//		 sdcap = READ_IC(0xDA,0x00);LCD1602_display_hex(Line1602_0,10,MIPI_READ_DATA[0]);
		if((OTP_VALUE1 == VCOM_default)||(sdcap == 0))       ////////若产品VCOM为默认值则进入OTP流程。
        {       
            MAN_OTP_FLAG=1;    /////进入手动OTP标志位
            AUTO_OTP_FLAG=1;   /////进入自动OTP标志位
        }
        SSD2828_VIDEO_MODE_HS();
		
		LCD1602_display_Line(Line1602_1,"M1638A0_A_OTP    ");
		{
            BL_ON();     ////背光控制
            BL_ON();     ////背光控制
            BL_ON();     ////背光控制
            DelayMs(50);
		}

        if((B6_event_flag==2))  { B6_event_flag=0;    goto LCDTest;  }   		         

        
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////---------------------------开机VSP/N电流以及系统VDD电流检测，数值显示在LCD1602上。
///////////////////////////////////////////////////////////////////////////////////////////////////////////   	
//        LCD1602_display_Line_A(Line1602_1,0,"                ");       ////clear  当前行 
//         LCD1602_display_DATA(Line1602_1,9,Measure_IOIDD(mA));/////////////////电流监测。。 
//         LCD1602_display_Line_A(Line1602_1,14,"mA");
//        
//        LCD1602_display_DATA(Line1602_1,2,Measure_V5V(mA));/////////////////电流监测。。 
//         LCD1602_display_Line_A(Line1602_1,7,"mA"); 
        

        
      GET_PIC_MAX_NUM();	//////读取显示图片信息。。
      pic_num = 1;   /////////设置第一幅显示图片，从1开始计数，
     
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////---------------------------进入自动OTP模式code
///////////////////////////////////////////////////////////////////////////////////////////////////////////        
//    if(AUTO_OTP_FLAG)
//    {
//        sdcap = KEY_AUTO_MODE;
//		while(( sdcap == 0)&&(MTP_OVER == 0))
//            {
//                sdcap = KEY_AUTO_MODE;
//			
//            pic_num = 10;/////SD卡内的flicker画面，画面编号002
////			pic_num_string =  PIC_SET_CACHE[pic_num][0];  	/////读取需要显示的画面顺序，依次进行显示。。
////			nosdnodelay=0;
////			Pattern(pic_num_string);
//			
////			pic_num ++;
//			pic_num_string =  PIC_SET_CACHE[pic_num][0];  	/////读取需要显示的画面顺序，依次进行显示。。
//			nosdnodelay=0;
//			Pattern(pic_num_string);DelayMs(100);  
//			TSL_GPIO_Config();		/////sensor use
//            if((B6_event_flag==2))  {            B6_event_flag=0;    goto LCDTest;               } 
//            
//			{
//				Auto_otp();
////				LCD1602_display_hex(Line1602_1,0,VCOMDC1);
////				LCD1602_display_DATA(Line1602_1,11,GET_FLICKER());	DelayMs(200);
//			}
//            if((B6_event_flag==2))  {            B6_event_flag=0;    goto LCDTest;               } 
//            
//            
//			if((OTP_VALUE1 != VCOMDC1)||(OTP_ERROR_MESAGE>0))
//			{	
//                while(1)
//                {
//                    DelayMs(100); 
//                    LCD1602_display_Line(Line1602_0,"OTP ERROR     NG");
////                    LCD1602_display_hex(Line1602_1,0,VCOMDC1);
//                    pic_num = 10;	ID_OK = 1;
//                    if((B6_event_flag==2))  {            B6_event_flag=0;    goto LCDTest;               } 
//                }
//            }
//            }
//    }
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////---------------------------进入手动OTP模式code
///////////////////////////////////////////////////////////////////////////////////////////////////////////
    if(MAN_OTP_FLAG)
    {
        pic_num = 9;/////SD卡内的flicker画面，画面编号002
        pic_num_string =  PIC_SET_CACHE[pic_num][0];  	/////读取需要显示的画面顺序，依次进行显示。。
//        nosdnodelay=0;
        Pattern(pic_num_string);DelayMs(100); 
        LCD1602_display_Line(Line1602_0,"flicker adjust------");
        LCD1602_display_Line(Line1602_1,"+/- begin          ");
        while(MAN_OTP_FLAG)
        {
			mm_KEYB1  =  	KEY_PAUSE;
            mm_KEYB10 = 	KEY_UP;
			mm_KEYB11  =  	KEY_DOWN;

            
            
			if(mm_KEYB10 == 0)      /////////++
            {
                VCOMDH++;
//                SSD2828_ENTER_LP_mode(); //enter  LP mode	
//                STM32TOSSD2828_W_COM(0xb7);		//LP DCS mode
//                STM32TOSSD2828_W_DATA_16BITS(0x0752);
                
				DelayMs(6);
                VCOM_set(VCOMDH);
                LCD1602_display_hex(Line1602_1,10,VCOMDH);
//                SSD2828_VIDEO_MODE_HS();
                DelayMs(6);                
            }
                
            if(mm_KEYB11 == 0)       /////////--
            {
                VCOMDH--;
//                SSD2828_ENTER_LP_mode(); //enter  LP mode				
				DelayMs(6);
                VCOM_set(VCOMDH);
                LCD1602_display_hex(Line1602_1,10,VCOMDH);
//                SSD2828_VIDEO_MODE_HS();
                DelayMs(6); 
            }
                
            if(mm_KEYB1 == 0)       /////////otp
            {
                LCD1602_display_Line(Line1602_1,"OTP begin          ");
                MAN_OTP_FLAG=0;
                VCOMDC1 = VCOMDH;
                if(VCOMDC1 == VCOM_default){    VCOMDC1=VCOMDC1+1;  }
                MTP(); 
                DelayMs(100);
                MTP_OVER=1;
                LCD1602_display_Line(Line1602_1,"finish          ");
                reset_lcd();				
                DelayMs(100);
                VCOM_READ();					  //回读再复判
                //VCOM_CHECK(VCOMDC1);
                SSD2828_VIDEO_MODE_HS();	 
            }
            
            DelayMs(70);
             if((B6_event_flag==2))  {            B6_event_flag=0;    goto LCDTest;               }    
        }
    }        
////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////---------------------------进入电测画面前判定一下VCOM是否为默认值，不为默认值则进行电测画面循环。
///////////////////////////////////////////////////////////////////////////////////////////////////////////        
        if((MIPI_READ_DATA[1] == VCOM_default)||(OTP_ERROR_MESAGE>0))
        {
            while(1)
            {
                DelayMs(100); 
                LCD1602_display_Line(Line1602_0,"OTP NG           ");
                LCD1602_display_hex(Line1602_1,10,MIPI_READ_DATA[1]);
                if((B6_event_flag==2))  {            B6_event_flag=0;    goto LCDTest;               } 
            }
        }
        
 ////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////---------------------------进行电测画面循环。
///////////////////////////////////////////////////////////////////////////////////////////////////////////        
    pic_num = 1;   /////////设置第一幅显示图片，从1开始计数，
    while(1)
    { 	
        pic_num_string =  PIC_SET_CACHE[pic_num][0];  	/////读取需要显示的画面顺序，依次进行显示。。
//        nosdnodelay=0;
       Pattern(pic_num_string);
		LED_RUN();
		

//		while(1)
//		{
//			Do_usart_cmd();		/////启用上位机操作时打开。。。
//		}

		
       
      //////针对延时参数的处理，便于外部对程式进行延时控制。
      //////仅针对配置文件的参数，使用默认参数时此段代码意义可以忽略。。备忘 
//       if(PIC_SET_CACHE[pic_num][0] < 0x20)
//       {    DELAY_15 = Testpar[2*pic_num+8]*DELAY_20 + DELAY_20;      }
//       else
//       {    DELAY_15 = DELAY_20; }
       

        { PassKey();   }           /////////////真正延时，包含开关屏检测}
        if((B6_event_flag==2))  {            B6_event_flag=0;    goto LCDTest;               } 
       
         
    }    



	}	
 








////--------------------------------------------------------------------------------------------------------







////////////////////?????,?????????????????
void PassKey(void)
{
	u32 coun=0;
	u16 mm1,mm2,mm3;
  
	mm1=mm2=mm3=1;    //??????

			
	 mm1=KEY_UP;   //??
	 mm2=KEY_DOWN;   //??
	 mm3=KEY_MAN_MODE;    //??????
	 DelayMs(1);
/*******?????********/	 
	coco:	
   if(HorA==0)	
   {	
		 while((mm1!=0)&&(mm2!=0))
		 {
			mm1=KEY_DOWN;	
			mm2=KEY_UP;
			DelayMs(1);  

            if((mm1==0)&&(mm2==0))      //??+/-?????? ,?????????
				{
					HorA=~HorA;
					LED_RUN_BIBI();
                    
                    mm1=KEY_DOWN;
                        mm2=KEY_UP;
                        DelayMs(1); 
					while((mm1==0)||(mm2==0))   //??????
					{
                        mm1=KEY_DOWN;
                        mm2=KEY_UP;
                        DelayMs(1); 
					}
				  goto coco;
				}	
		 }
         
         mm1=KEY_DOWN;
			mm2=KEY_UP;
		 if((mm1==0)&&(mm2!=0))
		 {			 
			pic_num++;	 
			if(pic_num>PIC_NUM+1)
			{
				pic_num=1;
			}
		}
		else if((mm2==0)&&(mm1 != 0))
		{	
			pic_num--;	   
			if(pic_num==0)
			{
				pic_num=PIC_NUM;
			}
		}
        else if((mm1==0)&&(mm2==0))      //??+/-?????? ,?????????
				{
					HorA=~HorA;
					LED_RUN_BIBI();
                    
                    mm1=KEY_DOWN;
                        mm2=KEY_UP;
                        DelayMs(1); 
					while((mm1==0)||(mm2==0))   //??????
					{
                        mm1=KEY_DOWN;
                        mm2=KEY_UP;
                        DelayMs(1); 
					}
				  goto coco;
				}	


        DelayMs(5);  
   }
/********?????********/	 
	 else
	 {
		 do
		 {
            mm1=KEY_UP;   //??
            mm2=KEY_DOWN;   //??
            mm3=KEY_MAN_MODE;    //??????		
             
			
             
			if((mm1==0)&&(mm2==0))      //??+/-?????? ,?????????
            {
					HorA=~HorA;
					LED_RUN_BIBI();
					while((mm1==0)||(mm2==0))   //??????
					{
						mm1=KEY_DOWN;
						mm2=KEY_UP;
						DelayMs(5);
                    }
					goto coco;
            }
			coun++; 
			DelayMs(5);
            
            if((mm1==0))      //??pause???,????,?????????
			{	   
					while((mm1==0)&&(mm2!=0))   //??????
					{mm1=KEY_UP;mm1=KEY_DOWN;}
					DelayMs(10);
                    
                    
                    
					mm1=1;					
					while((mm1!=0))   //?????????,????????
					{mm1=KEY_UP;          
       
                    }
				
					while((mm1==0))   //??????
					{mm1=KEY_UP;     

                    }				
			}
            

            
		 }while(coun<DELAY_15);
		 
		
		 pic_num++;
		 if(pic_num>PIC_NUM)
		 {
			 pic_num=1;
		 }	 
	 }

 	
} 





void GET_PIC_MAX_NUM(void)
{
    u8 i=1;
    
    for(i=1;i<PIC_SAVE_NUM_MAX;i++)
    {
        if((PIC_SET_CACHE[i][0] > PIC_FROM_SDRAM)||(PIC_SET_CACHE[i][0] < 1))
        {
            PIC_NUM = i-1;
            i=200;
        }
    }
    
}


  
/*******************************************************************************
* Function Name  : KEY_SCAN_CONTROL
* Description    : MAIN UI
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/    
void KEY_SCAN_CONTROL_A(void)
{
    u8 OTP_FLAG = 1;
    u16 mm_KEYB10 = 1 ;
	u16 mm_KEYB1 = 1 ;
	u16 mm_KEYB11 = 1 ;
    
    
    LCD1602_display_Line_A(Line1602_0,0,"                ");       ////clear  当前行  
    LCD1602_display_Line_A(Line1602_1,0,"                ");       ////clear  当前行         
    LCD1602_display_Line_A(Line1602_1,0,"Display off");
    LCM_TYPE_SHOW_ON_1602();
    delay_ms(20);
    
    while(OTP_FLAG)
   {
		
		mm_KEYB1  =  	KEY_MAN_MODE;
        delay_ms(10);

        if(mm_KEYB1  != 0)
        {
            mm_KEYB1 = 	KEY_MAN_MODE;
			while(mm_KEYB1 == 0){      mm_KEYB1 = 	KEY_MAN_MODE;delay_ms(10);    }//////////////等待按键释放
            OTP_FLAG = 0;
        }

/////DEBUG use only          
        {         
////            LCM_TYPE = NOKIA_GPM1461A;
//            OTP_FLAG = 0;
//            SAVE_LCM_TYPE(LCM_TYPE);
////            
////            GET_LCM_TYPE();
            LCM_TYPE_SHOW_ON_1602();
        }    

        delay_ms(10);
    }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval : None
  */
void RCC_Configuration(void)
{   
  /* Setup the microcontroller system. Initialize the Embedded Flash Interface,  
     initialize the PLL and update the SystemFrequency variable. */
  SystemInit();

  /* Enable Key Button GPIO Port, GPIO_LED and AFIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);


  /* 使能USART1&使能GPIO port clock */
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 |RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB |
                         RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD |
                         RCC_APB2Periph_GPIOE, ENABLE);
}											

//////////////////////////////////////
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
   #ifdef   VECT_TAB_RAM    //如果C/C++ Compiler\Preprocessor\Defined symbols中的定义了VECT_TAB_RAM(见程序库更改内容的表格)
		   /*Set the Vector Table base location at 0x20000000*/
           NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); //在RAM调试

   #else                                              //如果没有定义VECT_TAB_RAM
		   /*Set the Vector Table base location at 0x08000000*/
           NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//在Flash调试

    #endif                                                                  

  /* Configure one bit for preemption priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn  ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


//  /* Enable the EXTI9_5 Interrupt */
// 
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn  ;

//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);
 

  NVIC_InitStructure.NVIC_IRQChannel = SDIO_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  
}

//////////////////////////////////////
void NVIC_Configuration_only_key(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the EXTI9_5 Interrupt */
 
//  NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn  ;          ///////PA6~PG6

     /* Enable the EXTI15_10 Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn  ;          ///////PA13~PG13
    
    
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
 


  
}


void KEY_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO, ENABLE);  
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);				 
	

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_2; //key 3   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//输入悬空
	GPIO_Init(GPIOB, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_3 | GPIO_Pin_15; //key2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//输入悬空
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; //key1   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//输入悬空
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	
	///////IIC  use
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_0; //iic
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	//输入悬空
	GPIO_Init(GPIOC, &GPIO_InitStructure);
    
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	//LED2 PA8 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);					 
	
	GPIO_ResetBits(GPIOA, GPIO_Pin_8);	 //SET LED2 ON					 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1  ; 	 //MCU_RST (TO raR5421)
	GPIO_Init(GPIOE, &GPIO_InitStructure);  	
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0  ; 	 //SSD2828_RST (TO SSD2828)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOE, &GPIO_InitStructure);  
    
    
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 ; 	 //LCD背光开关
  	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
////	GPIO_SetBits(GPIOC, GPIO_Pin_7);	 //LCD背光开
//    GPIO_ResetBits(GPIOC, GPIO_Pin_7);	 //LCD背光off

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6; //PD6 MCU_WAIT
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//输入悬空
	GPIO_Init(GPIOD, &GPIO_InitStructure);
		
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0 | GPIO_Pin_1; 		//key1,key2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 	//上拉式输入
	GPIO_Init(GPIOB, &GPIO_InitStructure);
 
/*-- GPIO Configuration ------------------------------------------------------*/  

	//PD0(D2),PD1(D3),PD4(RD),PD5(WR),PD8(D13),PD9(D14),PD10(D15),PD14(D0),PD15(D1)
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 |
	                            GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽复用
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Set PE.07(D4), PE.08(D5), PE.09(D6), PE.10(D7), PE.11(D8), PE.12(D9), PE.13(D10),
	 PE.14(D11), PE.15(D12) as alternate function push pull */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
	                            GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
	                            GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure); 
	
	
	
	/* NE1 configuration */	  //CS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* RS */					  //RS
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ; 
	GPIO_Init(GPIOD, &GPIO_InitStructure); 
	
	delay_ms(5);
	
	GPIO_SetBits(GPIOD, GPIO_Pin_7);			//CS=1 
	GPIO_SetBits(GPIOD, GPIO_Pin_14| GPIO_Pin_15 |GPIO_Pin_0 | GPIO_Pin_1);  	 
	GPIO_SetBits(GPIOE, GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10);   
	//GPIO_ResetBits(GPIOE, GPIO_Pin_0);		//I2C1_RESET
	//GPIO_ResetBits(GPIOE, GPIO_Pin_1);		//RESET=0
	GPIO_SetBits(GPIOD, GPIO_Pin_4);		    //RD=1
	GPIO_SetBits(GPIOD, GPIO_Pin_5);			//WR=1
	
	GPIO_SetBits(GPIOA, GPIO_Pin_4);			//SPI NSS
	
	//	GPIO_SetBits(GPIOC, GPIO_Pin_7);	 //LCD背光开
    GPIO_ResetBits(GPIOC, GPIO_Pin_7);	 //LCD背光off
	 
}


void EXTI_Configuration(void)
{

	EXTI_InitTypeDef EXTI_InitStructure;


	    /*connect PB5 to EXTI*/
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
    /*Config EXTI8*/
    EXTI_InitStructure.EXTI_Line = EXTI_Line5;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);

//    /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
//    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);

//   

    //EXTI_GenerateSWInterrupt(EXTI_Line8);
} 

void EXTI_Configuration_only_key(void)
{

	EXTI_InitTypeDef EXTI_InitStructure;


//////////////////PB6
//    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource6);

//    /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
//    EXTI_InitStructure.EXTI_Line = EXTI_Line6;
//    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
//    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
//    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//    EXTI_Init(&EXTI_InitStructure);

    
    
//////////////////PD13    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource13);

    /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);   

    //EXTI_GenerateSWInterrupt(EXTI_Line8);
} 

void EXTI_Configuration_only_key_DISABLE(void)
{

	EXTI_InitTypeDef EXTI_InitStructure;

//////////////////PD13    
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource13);

    /* Configure Key Button EXTI Line to generate an interrupt on falling edge */  
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);   

    //EXTI_GenerateSWInterrupt(EXTI_Line8);
}

void SDPicShow(u8 pic)
{

}


int SYS_BOARD_INITION(void)
{
	u32  sdcap=0;
	
  	/* System Clocks Configuration */
  	RCC_Configuration(); 
  
  	delay_init(72);  

  	GPIO_Configuration();
  
  	NVIC_Configuration();

  	EXTI_Configuration();

  	SPI_Peripheral_Init();

  	USART_Configuration();//set BaudRate 115200

    /* Enable the FSMC Clock */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
    
  	
    KEY_GPIO_Configuration();
    LCD1602_PIN_Configuration();
    LCD1602_Init();             //delay_ms(40);  
	delay_ms(1);
    LCD1602_display_Line_A(Line1602_0,0,"                ");       ////clear  当前行  
    LCD1602_display_Line_A(Line1602_1,0,"                ");       ////clear  当前行  
    LCD1602_display_Line_A(Line1602_1,0,"Starting.....");
//    T226_intion();
    
    Status = SD_Init();
    if (Status == SD_OK)
    {Status = SD_GetCardInfo(&SDCardInfo);}
     	
    if (Status == SD_OK)
  	{Status = SD_SelectDeselect((u32) (SDCardInfo.RCA << 16));}
    
    if (Status == SD_OK)
    {
    /*----------------- Set BusWidth ------------------------------*/
    Status = SD_EnableWideBusOperation(SDIO_BusWide_4b);
    }
    
    if (Status == SD_OK)
    {   
        Status = SD_SetDeviceMode(SD_DMA_MODE);  
    }

    sdcap=sd_Capacity>>20; //(sdcap=sd_Capacity/1024/1024)
    if(sdcap != 0){     SDConfigFlag = 1;   }
    
    
	//tied test1 to high if test on system program mode 
	//or set RESET pin = low
//	GPIO_ResetBits(GPIOE, GPIO_Pin_1);
    GPIO_SetBits(GPIOE, GPIO_Pin_1);     /////RGB CORE IC  RST
    
    GPIO_ResetBits(GPIOE, GPIO_Pin_0);    //////ssd2828 RST
    GPIO_ResetBits(GPIOB, GPIO_Pin_4);    //////GT9271 RST
    
//    GPIO_ResetBits(GPIOA, GPIO_Pin_8);    ////work LED ON
    GPIO_SetBits(GPIOA, GPIO_Pin_8);        //////work LED OFF
    
      {   
            LCD1602_display_Line_A(Line1602_0,0,"SD =                      ");
             LCD1602_display_DATA(Line1602_0,5,sdcap); //delay_seconds(1);//

                                           
            LCD_Set_DisplayWindow(0,XDP-1,0,YDP-1);
            LCD_WriteRAM_Prepare(); /* Prepare to write GRAM */	

            if (FAT_Init())	//???FAT????
                  {	
//                        LCD_DisplayString(0,Line0,"SD ERROR!",White,Blue2);
                        LCD1602_display_Line_A(Line1602_0,0,"FAT      non");//delay_seconds(1);//
                      Testpar[127] = 0xFF;
//                       SDShowFlag=0; 
                      goto NEXT_STEP;

                  }

            SearchInit();
                  SDShowTimes = 0;

                while (SDShowFlag) //??BMP????
                {	  
                    LCD_Set_DisplayWindow(0,XDP-1,0,YDP-1);
                    LCD_WriteRAM_Prepare();
//                    Disp_BMP() ;
                    SDShowFlag = 0;	
                }
                Get_testpar();
                SDShowTimes = 1;
            }

	 
     NEXT_STEP:
//    GPIO_ResetBits(GPIOA, GPIO_Pin_8);    /////work  LED on
    GPIO_SetBits(GPIOA, GPIO_Pin_8);    ///////work  LED OFF
	printf("\r\n Test mode\r\n");
    BL_OFF();
    
    
//	while(1)	
	{
		//	Initial_LQ043Y1DX07();/*Initial SHARP LQ043Y1DX07*/   	
		//	GPIO_Configuration();//Initial LCD 之后　重置 GPIO	
		//------------------------------------
		//**[00h]**//

		/* Enable the FSMC Clock */
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);
		/* Configure FSMC Bank1 NOR/PSRAM */


		FSMC_LCD_Init_H();	//FSMC 加速
		delay_ms(100);//delay for R5421 power on

		R5421_HW_Reset();
		
		System_Check_Temp();	//检测复位是否成功

		delay_ms(100);
		while(LCD_StatusRead()&0x02);	 //Initial_Display_test	and  set SW2 pin2 = 1

/////////////////////flash  check
        if(SPI_TEST() != TRUE)
        {   
            LCD1602_display_Line_A(Line1602_0,0,"                ");       ////clear  当前行  
            LCD1602_display_Line(Line1602_0,"flash error");
        }
		
		LCM_CHANGE(); 
        R5421_initial();
		SDRAM_PREPARE();
	}
	return  TRUE;
}


void detect_button_B1(void)
{
 
  if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))//B1 按键
   {
    delay_ms(1);
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13))//B1 按键
	 B1_event_flag = 1;
   }
 
}

void detect_button_B2(void)
{
 
  if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))//B2 按键
   {
    delay_ms(1);
	if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12))//B2 按键
	B2_event_flag = ~B2_event_flag;
	 //B2_event_flag = 1;
   }
 
}


uint16_t CCR3_Val = 500;   

void D3_timer3_ini_PWM_enable(void)
{

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  

  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //设定Timer为内定值
  TIM_DeInit(TIM3);
  //采用内部时间提供给TIM3
  TIM_InternalClockConfig(TIM3);

  TIM_TimeBaseStructure.TIM_Prescaler = 0x1c1f;        //100us 牧箇だ繵计  牧繵瞯 = 72/(牧箇だ繵+1) 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //家Α  璸计
  TIM_TimeBaseStructure.TIM_Period = 999;       //笆杆更盚竟秅戳	  PWM よ猧秅戳

  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; //丁だ澄
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  
  TIM_OCStructInit(&TIM_OCInitStructure);
 //PWM皌竚 皌竚ゑ㎝硄笵  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //PWM家Α
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//块﹍
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;  //竚ゑ ゑ=CCR3_Val/(TIM_Period +1)=50%

  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //ゑ耕块伐┦
  TIM_OC3Init(TIM3, &TIM_OCInitStructure);  //皌竚硄笵3

  TIM_Cmd(TIM3, ENABLE);     //ゴ秨﹚竟竟3
  TIM_CtrlPWMOutputs(TIM3,ENABLE);
}

void D3_timer3_ini_PWM_disable(void)
{
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,DISABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//崩块
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  TIM_Cmd(TIM3, DISABLE); 
  TIM_CtrlPWMOutputs(TIM3,DISABLE);
}

void D4_timer3_ini_PWM_enable(void)
{

  TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
  TIM_OCInitTypeDef TIM_OCInitStructure;
  

  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //砞﹚Timerず﹚
  TIM_DeInit(TIM3);
  //蹦ノず场丁矗ㄑ倒TIM3
  TIM_InternalClockConfig(TIM3);

  TIM_TimeBaseStructure.TIM_Prescaler = 0x1c1f;        //100us 牧箇だ繵计  牧繵瞯 = 72/(牧箇だ繵+1) 
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //家Α  璸计
  TIM_TimeBaseStructure.TIM_Period = 999;       //笆杆更盚竟秅戳	  PWM よ猧秅戳

  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0; //丁だ澄
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

  
  TIM_OCStructInit(&TIM_OCInitStructure);
 //PWM皌竚 皌竚ゑ㎝硄笵  
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;   //PWM家Α
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//块﹍
  TIM_OCInitStructure.TIM_Pulse = CCR3_Val;  //竚ゑ ゑ=CCR3_Val/(TIM_Period +1)=50%

  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;   //ゑ耕块伐┦
  TIM_OC4Init(TIM3, &TIM_OCInitStructure);  //皌竚硄笵4

  TIM_Cmd(TIM3, ENABLE);     //ゴ秨﹚竟竟3
  TIM_CtrlPWMOutputs(TIM3,ENABLE);
}

void D4_timer3_ini_PWM_disable(void)
{
  GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,DISABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_1;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//崩块
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  TIM_Cmd(TIM3, DISABLE); 
  TIM_CtrlPWMOutputs(TIM3,DISABLE);
}



/**
   * @brief  Fill the global buffer
  * @param pBuffer: pointer on the Buffer to fill
  * @param BufferSize: size of the buffer to fill
  * @param Offset: first value to fill on the Buffer
  */

 

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *   where the assert_param error has occurred.
  * @param file: pointer to the source file name
  * @param line: assert_param error line source number
  * @retval : None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}






#endif


void EXTI13_IRQHandler(void)
{
    delay_ms(20);
    {
        if((GPIOD->IDR&GPIO_Pin_13) == 0)
        {
            B6_event_flag++;
        }
    }


    LCD1602_LED_RUN();
    
    if((B6_event_flag%2) ==1)
	{
            BL_OFF();
            enter_sleep_mode();
			VSPN_OFF();
            delay_ms(20);
            VDD_OFF(); 		///////MIPI芯片上电
		LCM_VDD_OFF();       ////////产品LCD的VDD和IOVDD上电。
            B6_event_flag = 2;
    }
    else
    {

    }
    
    EXTI_ClearFlag(EXTI_Line13);
    EXTI_ClearITPendingBit(EXTI_Line13);
  
}

