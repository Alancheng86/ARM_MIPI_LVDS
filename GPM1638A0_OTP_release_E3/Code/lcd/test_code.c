#include <math.h>
#include "stdio.h"
#include <string.h>

#include "fsmc_sram.h"
#include "stdlib.h"
#include "stm32f10x_tim.h"
#include "delay.h"
#include "UserDef.h"
#include "RA8876.h"
#include "lcd.h"
#include "User_font.h"
#include "p128_1bpp.h"
#include "test_code.h"

#include "pen_il.h"
#include "arrow_il.h"
#include "busy_im.h"
#include "no_im.h"


#include "integer.h"
#include "ff.h"
#include "diskio.h"


/*==== [SW_(2)]  Master I2C  =====*/
//#define Enable_I2CM_Noise_Filter  //測試用Mountain20120523
#define Disable_I2CM_Noise_Filter //測試用Mountain20120523

extern unsigned char B1_event_flag ;//B1 按鍵觸發事件
extern unsigned char B2_event_flag ;//B2 按鍵觸發事件

extern void detect_button_B1(void);
extern void detect_button_B2(void);

extern unsigned char buffer[5000];  //
extern unsigned char buffer_data[2]; 

  char iic_test1[] = "crazy";
  char iic_test2[] = "raio";

  unsigned char tempaa[5];

  extern void sd_BTE_showpic_256_bin(unsigned short w,unsigned short h,char * fname);
  extern void sd_showpic_256_bin(unsigned short w,unsigned short h,char * fname);
  extern void sd_BTE_showpic_65k_bin(unsigned short w,unsigned short h,char * fname);
  extern void sd_showpic_MPU8_to_65k_bin(unsigned short w,unsigned short h,char * fname);
  void sd_showpic_MPU8_to_16M_bin(unsigned short w,unsigned short h,char * fname);
  extern void sd_showpic_65k_bin(unsigned short w,unsigned short h,char * fname);
  extern void sd_BTE_showpic_16M_mode1_bin(unsigned short w,unsigned short h,char * fname);
  extern void sd_showpic_16M_mode1_bin(unsigned short w,unsigned short h,char * fname);
  extern void sd_showpic_16M_mode2_bin(unsigned short w,unsigned short h,char * fname);

  extern unsigned char SPIM_PageProgram_bin2flash(char * fname);
  extern unsigned char SPI_Master_verify_flash_data(char * fname);

/*REG[01h] bit0*/
//使用ARM測試8BIT 點8BPP  (256色)
void Host_8bit_data_bus_test_8bpp(void)
{
  unsigned int i,j;

   Host_Bus_8bit();


	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

	Active_Window_WH(800,600);


    Graphic_cursor_initial(); //initaial and enable graphic cursor 


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		  LCD_DataWrite(color256_blue);
		  Check_Mem_WR_FIFO_not_Full();

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);


     //測試一直寫
  Main_Image_Start_Address(0);
  delay_ms(20);
  Canvas_Image_Start_address(0);

  	Main_Window_Start_XY(0,0);
	delay_ms(20);
	Active_Window_XY(0,0);
	Goto_Pixel_XY(0,0);	
	LCD_CmdWrite(0x04);
	
  while(1)
  {
   sd_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();
   } 
   
   while(1)
   {
     for(i=0;i<2;i++)
     {
	  Main_Image_Start_Address(i*8188*600);
      Canvas_Image_Start_address(i*8188*600);
	  Goto_Pixel_XY(0,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_256_bin(800,600,"p1.bin");
	  //delay_seconds(1);
	  }

	  for(i=0;i<4;i++)
      {
	    Main_Image_Start_Address(i*8188*600);
		 delay_seconds(1);
	  }

   }

	

}



//使用ARM測試8BIT 點16BPP  (65k色)
void Host_8bit_data_bus_test_16bpp(void)
{
  unsigned int i,j;

     Host_Bus_8bit();

	Main_Image_Start_Address(0);					
	Main_Image_Width(2048);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(2048);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_8b_16bpp();
    Memory_16bpp_Mode();

    Graphic_cursor_initial(); //initaial and enable graphic cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);

    //測試一直寫
    Main_Image_Start_Address(0);
    delay_ms(20);
    Canvas_Image_Start_address(0);

  	Main_Window_Start_XY(0,0);
	delay_ms(20);
	Active_Window_XY(0,0);
	Goto_Pixel_XY(0,0);	
	LCD_CmdWrite(0x04);
  /*
  while(1)
  {
   sd_showpic_MPU8_to_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();
  }
   */

  while(1)
  {

	for(i=0; i<10;i++)
	 {
	  Main_Image_Start_Address(i*2048*600*2);
      Canvas_Image_Start_address(i*2048*600*2);
	  Goto_Pixel_XY(0,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_MPU8_to_65k_bin(800,600,"p2.bin");

		/*
	  delay_seconds(1);
  	    for(j=0; j<480000;j++)
		{				
		  LCD_DataWrite(color65k_blue);
		  LCD_DataWrite(color65k_blue>>8);
		  Check_Mem_WR_FIFO_not_Full();
 		}
		delay_seconds(1);  */
	 }

	 for(i=0;i<10;i++)
      {
	    Main_Image_Start_Address(i*2048*600*2);
		delay_seconds(1);
	  }


   }


}


//使用ARM測試16BIT 點16BPP (65k色)
void Host_16bit_data_bus_test_16bpp(void)
{
  unsigned int i,j,k;

     Host_Bus_16bit();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();
   
    //Graphic_cursor_initial(); //initaial and enable graphic cursor 


	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);

  //測試一直寫
  Main_Image_Start_Address(0);
  delay_ms(20);
  Canvas_Image_Start_address(0);

  	Main_Window_Start_XY(0,0);
	delay_ms(20);
	Active_Window_XY(0,0);
	Goto_Pixel_XY(0,0);	
	LCD_CmdWrite(0x04);
  while(1)
  {
     sd_showpic_65k_bin(800,600,"p2.bin");
     Check_Mem_WR_FIFO_Empty();
	 /*
     for(k=0; k<480000;k++)
		{				
		  LCD_DataWrite(color16M_blue);
		  Check_Mem_WR_FIFO_Empty();
 		 } */	

  }


	for(i=0; i<2;i++)
	 {
	  Main_Image_Start_Address(i*8188*600*2);
	  delay_ms(20);
      Canvas_Image_Start_address(i*8188*600*2);


	  for(j=0;j<10;j++)
	  {
		Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    //Goto_Pixel_XY(j*800,0);	
	    LCD_CmdWrite(0x04);
	    sd_showpic_65k_bin(800,600,"p2.bin");

		   Goto_Text_XY(300+j*800,200);
           Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(i*10+j);
           Graphic_Mode(); //back to graphic mode

	    delay_seconds(1);
		 /*
  	     for(k=0; k<480000;k++)
		 {				
		  LCD_DataWrite(color16M_blue);
		  LCD_DataWrite(color16M_blue>>8);
		  LCD_DataWrite(color16M_blue>>16);
		  //Check_Busy_Mem_WR();

 		  } */ 
	   }
	 }

  while(1)
  {


	for(i=0; i<2;i++)
	 {
	  Main_Image_Start_Address(i*8188*600*2);
      Canvas_Image_Start_address(i*8188*600*2);

	  for(j=0;j<10;j++)
	  {
		Main_Window_Start_XY(j*800,0);
		delay_seconds(1);
	  }
	 }

   }

}



//使用ARM測試8BIT 點24BPP (16M色)
void Host_8bit_data_bus_test_24bpp(void)
{
  unsigned int i,j,k;

     Host_Bus_8bit();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);


    Graphic_cursor_initial(); //initaial and enable graphic cursor 


	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_8b_24bpp();
    Memory_24bpp_Mode();



	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);

  //測試一直寫
  Main_Image_Start_Address(0);
  delay_ms(20);
  Canvas_Image_Start_address(0);

  	Main_Window_Start_XY(0,0);
	delay_ms(20);
	Active_Window_XY(0,0);
	Goto_Pixel_XY(0,0);	
	LCD_CmdWrite(0x04);
  while(1)
  {
   sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();
  }


	for(i=0; i<2;i++)
	 {
	  Main_Image_Start_Address(i*8188*600*3);
	  delay_ms(20);
      Canvas_Image_Start_address(i*8188*600*3);


	  for(j=0;j<10;j++)
	  {
		Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    //Goto_Pixel_XY(j*800,0);	
	    LCD_CmdWrite(0x04);
	    sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");

		   Goto_Text_XY(300+j*800,200);
           Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(i*10+j);
           Graphic_Mode(); //back to graphic mode

	    delay_seconds(1);
		 /*
  	     for(k=0; k<480000;k++)
		 {				
		  LCD_DataWrite(color16M_blue);
		  LCD_DataWrite(color16M_blue>>8);
		  LCD_DataWrite(color16M_blue>>16);
		  //Check_Busy_Mem_WR();
 		  } */ 
	   }
	 }

  while(1)
  {


	for(i=0; i<2;i++)
	 {
	  Main_Image_Start_Address(i*8188*600*3);
      Canvas_Image_Start_address(i*8188*600*3);

	  for(j=0;j<10;j++)
	  {
		Main_Window_Start_XY(j*800,0);
		delay_seconds(1);
	  }
	 }

   }

}



//使用ARM測試16BIT 點24BPP_mode1 (16M色)
void Host_16bit_data_bus_test_24bpp_mode1(void)
{
  unsigned int i,j,k,temp;

  Host_Bus_16bit();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 //Graphic_cursor_initial(); //initaial and enable graphic cursor 

	 //PDATA_Set_RGB();  //紅色
     //PDATA_Set_GRB();  //綠色
	 //PDATA_Set_RBG();  //紅色
	 //PDATA_Set_GBR();  //藍色
	 //PDATA_Set_BRG();  //綠色
	 //PDATA_Set_BGR();  //藍色


	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();
    //測試一直寫
	
  Main_Image_Start_Address(0);
  delay_ms(20);
  Canvas_Image_Start_address(0);

  	Main_Window_Start_XY(0,0);
	delay_ms(20);
	Active_Window_XY(0,0);
	Goto_Pixel_XY(0,0);	
	LCD_CmdWrite(0x04);
  while(1)
  {
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

 
   //Check_Mem_WR_FIFO_Empty();

   	 	 
    	for(i=0; i<480000/2;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		  Check_Mem_WR_FIFO_not_Full();
	     temp = color16M_blue>>16;
		 temp|=	color16M_blue<<8;
		 LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>8);
		  Check_Mem_WR_FIFO_not_Full();

 		} 
		  Check_Mem_WR_FIFO_Empty();


	  while(1)
	  {
	   	Active_Window_XY(0,0);
	    Active_Window_WH(128,128);
	    Goto_Pixel_XY(0,0);
	    LCD_CmdWrite(0x04);
       
		sd_showpic_16M_mode1_bin(128,128,"face.bin");
		Check_Mem_WR_FIFO_Empty();
	  }
  }	 

 //===================================
 //測試只讀暫存器
	 /*
	  j=0;

	    LCD_CmdWrite(0x12);
	    LCD_DataWrite(0x0040);

	    while(1)
		{	 			
		  temp = LCD_DataRead();

		  if(temp!=0x0040)
		  {Text_Mode();
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }
		 }
	 */




	for(i=0; i<2;i++)
	 {
	  Main_Image_Start_Address(i*8188*600*3);
	  delay_ms(20);
      Canvas_Image_Start_address(i*8188*600*3);


	  for(j=0;j<10;j++)
	  {
		Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    //Goto_Pixel_XY(0,0);	
	    LCD_CmdWrite(0x04);
	    sd_showpic_16M_mode1_bin(800,600,"p3.bin");

		   Goto_Text_XY(300+j*800,200);
           Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(i*10+j);
           Graphic_Mode(); //back to graphic mode

	    delay_seconds(1);
		 /*
  	     for(k=0; k<480000;k++)
		 {				
		  LCD_DataWrite(color16M_blue);
		  LCD_DataWrite(color16M_blue>>8);
		  LCD_DataWrite(color16M_blue>>16);
		  //Check_Busy_Mem_WR();

 		  } */ 
	   }
	 }

  while(1)
  {


	for(i=0; i<2;i++)
	 {
	  Main_Image_Start_Address(i*8188*600*3);
      Canvas_Image_Start_address(i*8188*600*3);
	  Main_Image_Start_Address(i*8188*600*3);

	  for(j=0;j<10;j++)
	  {
		Main_Window_Start_XY(j*800,0);
		delay_seconds(1);
	  }
	 }

   }


}

//使用ARM測試16BIT 點24BPP_mode2 (16M色)
void Host_16bit_data_bus_test_24bpp_mode2(void)
{
  unsigned int i,j,k,temp;

   temp =  color16M_green;

  Host_Bus_16bit();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 


	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode2();
    Memory_24bpp_Mode();

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);


     //測試一直寫
  Main_Image_Start_Address(0);
  delay_ms(20);
  Canvas_Image_Start_address(0);

  	Main_Window_Start_XY(0,0);
	delay_ms(20);
	Active_Window_XY(0,0);
	Goto_Pixel_XY(0,0);	
	LCD_CmdWrite(0x04);
  while(1)
  {
       sd_showpic_16M_mode2_bin(800,600,"p3.bin");
       Check_Mem_WR_FIFO_Empty();

      	for(i=0; i<(480000);i++) //每次寫入1個pixels
		{		 				
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		  LCD_DataWrite(temp>>16);
		  Check_Mem_WR_FIFO_not_Full();
 		}
		Check_Mem_WR_FIFO_Empty();
  }


	for(i=0; i<2;i++)
	 {
	  Main_Image_Start_Address(i*8188*600*3);
	  delay_ms(20);
      Canvas_Image_Start_address(i*8188*600*3);


	  for(j=0;j<10;j++)
	  {
		Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    //Goto_Pixel_XY(0,0);	
	    LCD_CmdWrite(0x04);
	    sd_showpic_16M_mode2_bin(800,600,"p3.bin");

		   Goto_Text_XY(300+j*800,200);
           Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(i*10+j);
           Graphic_Mode(); //back to graphic mode

	    delay_seconds(1);
		 /*
  	     for(k=0; k<480000;k++)
		 {				
		  LCD_DataWrite(color16M_blue);
		  LCD_DataWrite(color16M_blue>>8);
		  LCD_DataWrite(color16M_blue>>16);
		  //Check_Busy_Mem_WR();
		
 		  } */ 
	   }
	 }

  while(1)
  {


	for(i=0; i<2;i++)
	 {
	  Main_Image_Start_Address(i*8188*600*3);
      Canvas_Image_Start_address(i*8188*600*3);
	  Main_Image_Start_Address(i*8188*600*3);

	  for(j=0;j<10;j++)
	  {
		Main_Window_Start_XY(j*800,0);
		delay_seconds(1);
	  }
	 }

   }

}

//=========================================================================
void Memory_Write_Read_Direction_test_LRTD_8bpp_to_8bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;
 Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();


    Graphic_cursor_initial(); //initaial and enable graphic cursor 



    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);


  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();

  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();





     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  LCD_DataWrite(0xaa);
 		 }
		 //delay_seconds(1);
		}
	  }

 while(1)
 {
	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
           Graphic_Mode(); //back to graphic mode
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		  Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
           Graphic_Mode(); //back to graphic mode
		  while(1);
		  }

 		 }
		   /*
		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		   */
		 //delay_seconds(1);
		}

	  }

     }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_RLTD_8bpp_to_8bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;
 Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);


  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();

  MemRead_Right_Left_Top_Down();
  MemWrite_Right_Left_Top_Down();

  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();

  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();




 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();
	
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		    Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
 
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		    Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

     }
	
}


//=========================================================================
void Memory_Write_Read_Direction_test_DTLR_8bpp_to_8bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;
 Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

    Graphic_cursor_initial(); //initaial and enable graphic cursor 


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		  Check_Mem_WR_FIFO_not_Full();
	
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  MemRead_Down_Top_Left_Right();
  MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();
		
		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();
		
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   GPIO_SetBits(GPIOE, GPIO_Pin_6);//LA detect pin	   
 		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	  
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		    GPIO_ResetBits(GPIOE, GPIO_Pin_6);//
		  while(1);
		  }

		  temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		    GPIO_SetBits(GPIOE, GPIO_Pin_6);//LA detect pin	
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
 		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		    GPIO_ResetBits(GPIOE, GPIO_Pin_6);//
		  while(1);
		  }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

     }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_TDLR_8bpp_to_8bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;
 Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

 Graphic_cursor_initial(); //initaial and enable graphic cursor 


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();

  MemWrite_Top_Down_Left_Right();
  MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		 Check_Mem_WR_FIFO_not_Full();

 		 }
		 Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Text_Mode();
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
           Graphic_Mode(); //back to graphic mode
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		   Text_Mode();
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
           Graphic_Mode(); //back to graphic mode
		  while(1);
		  }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

     }	
}

//=========================================================================

//=========================================================================
void Memory_Write_Read_Direction_test_LRTD_16bpp_to_8bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;
 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   //Font_Width_X2();
  Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   //Font_Height_X2();
   Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

 Graphic_cursor_initial(); //initaial and enable graphic cursor 


 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
	
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);


  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();

  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();




 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
	
		  Check_Mem_WR_FIFO_not_Full();
		  LCD_DataWrite(0xaa);

		  Check_Mem_WR_FIFO_not_Full();
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
           Graphic_Mode(); //back to graphic mode
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		  Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
           Graphic_Mode(); //back to graphic mode
		  while(1);
		  }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

     }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_RLTD_16bpp_to_8bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;
 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);


  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();

  MemRead_Right_Left_Top_Down();
  MemWrite_Right_Left_Top_Down();

  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();

  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();




 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();
		 
		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();
 		 }
		 Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		    Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
 
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		    Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

     }
	
}


//=========================================================================
void Memory_Write_Read_Direction_test_DTLR_16bpp_to_8bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;
 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		  Check_Mem_WR_FIFO_not_Full();

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  MemRead_Down_Top_Left_Right();
  MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
 		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	  
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {

		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
 		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

     }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_TDLR_16bpp_to_8bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;
 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 


 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();

  MemWrite_Top_Down_Left_Right();
  MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		 Check_Mem_WR_FIFO_not_Full();

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Text_Mode();
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
           Graphic_Mode(); //back to graphic mode
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		   Text_Mode();
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
           Graphic_Mode(); //back to graphic mode
		  while(1);
		  }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

     }	
}


//=========================================================================
void Memory_Write_Read_Direction_test_LRTD_8bpp_to_16bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;

  Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  //Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  //Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_8b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);

		 LCD_DataWrite(color65k_blue>>8);
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_MPU8_to_65k_bin(800,600,"p2.bin");
	delay_seconds(1);


 
  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }


	   ////////////////////////////////////////////
	    for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }


    }
	
}


//=========================================================================
void Memory_Write_Read_Direction_test_RLTD_8bpp_to_16bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;

  Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_8b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);

		 LCD_DataWrite(color65k_blue>>8);

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_65k_bin(800,600,"p2.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down();
  //MemRead_Left_Right_Top_Down();
  MemRead_Right_Left_Top_Down();
  MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		  
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_DTLR_8bpp_to_16bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;

  Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	  Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_8b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);

		 LCD_DataWrite(color65k_blue>>8);

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_65k_bin(800,600,"p2.bin");
	delay_seconds(1);


 
  //MemWrite_Left_Right_Top_Down();
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
   MemRead_Down_Top_Left_Right();
   MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
  
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}


//=========================================================================
void Memory_Write_Read_Direction_test_TDLR_8bpp_to_16bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;

  Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_8b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);

		 LCD_DataWrite(color65k_blue>>8);

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_65k_bin(800,600,"p2.bin");
	delay_seconds(1);


 
  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
   //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  MemWrite_Top_Down_Left_Right();
  MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x55);
		  Check_Mem_WR_FIFO_not_Full();
		 
		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0xaa);
		  Check_Mem_WR_FIFO_not_Full();

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		  	Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {
		  Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaa)
		  {	
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}
//=========================================================================
//=========================================================================
void Memory_Write_Read_Direction_test_LRTD_16bpp_to_16bpp(void)
{
     unsigned int i,j,k;
	 unsigned int temp;

  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

    //Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<800*600;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		  Check_Mem_WR_FIFO_not_Full();

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_65k_bin(800,600,"p2.bin");
	delay_seconds(1);


 
  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

  while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x5555);
		  Check_Mem_WR_FIFO_not_Full();
		
		  LCD_DataWrite(0xaaaa);
		  Check_Mem_WR_FIFO_not_Full();
		 
	
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x5555)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaaaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

 		 }
		   
		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode	
		 //delay_seconds(1);
		}

	  }
	  ///////////////////////////////////////////////



    }
	
}


//=========================================================================
void Memory_Write_Read_Direction_test_RLTD_16bpp_to_16bpp(void)
{
     unsigned int i,j,k;
	 unsigned int temp;

  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 


 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);


 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_65k_bin(800,600,"p2.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down();
  //MemRead_Left_Right_Top_Down();
  MemRead_Right_Left_Top_Down();
  MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x5555);
		  Check_Mem_WR_FIFO_not_Full();
		 

		  LCD_DataWrite(0xaaaa);
		  Check_Mem_WR_FIFO_not_Full();
		
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x5555)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaaaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		  
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);

		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_DTLR_16bpp_to_16bpp(void)
{
     unsigned int i,j,k;
	 unsigned int temp;

  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	 // Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);


 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_65k_bin(800,600,"p2.bin");
	delay_seconds(1);


 
  //MemWrite_Left_Right_Top_Down();
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
   MemRead_Down_Top_Left_Right();
   MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x5555);
		  Check_Mem_WR_FIFO_not_Full();
	

		  LCD_DataWrite(0xaaaa);
		  Check_Mem_WR_FIFO_not_Full();
	

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x5555)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

	

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaaaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
  
		   while(1);
		   }
 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}


//=========================================================================
void Memory_Write_Read_Direction_test_TDLR_16bpp_to_16bpp(void)
{
     unsigned int i,j,k;
	 unsigned int temp;

  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 


 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_65k_bin(800,600,"p2.bin");
	delay_seconds(1);


 
  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
   //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  MemWrite_Top_Down_Left_Right();
  MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x5555);
		  Check_Mem_WR_FIFO_not_Full();
		 

		  LCD_DataWrite(0xaaaa);
		  Check_Mem_WR_FIFO_not_Full();
	

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		Check_Mem_RD_FIFO_not_Empty();

	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x5555)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }



		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0xaaaa)
		  {
		  Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }
 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_LRTD_8bpp_to_24bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;

 Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_8b_24bpp();
    Memory_24bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 LCD_DataWrite(color16M_blue>>8);
		 LCD_DataWrite(color16M_blue>>16);
		 //Check_Busy_Mem_WR();
		 Check_Mem_WR_FIFO_not_Full();
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");
	delay_seconds(1);


  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x00005555);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x00005555>>8);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00005555>>16);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x00aaaa00);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x00aaaa00>>8);
		  Check_Mem_WR_FIFO_not_Full();
		
		  LCD_DataWrite(0x00aaaa00>>16);
		  Check_Mem_WR_FIFO_not_Full();
		 
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {Text_Mode();
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {	
		  Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x00)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x00);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0x00)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x00);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		  
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
  
		   while(1);
		   }

		   temp = LCD_DataRead();
		   Check_Mem_RD_FIFO_not_Empty();
		  if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		   while(1);
		   }

		   temp = LCD_DataRead();
		   Check_Mem_RD_FIFO_not_Empty();
		  if(temp!=0xaa)
		  {	
		  	Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}


//=========================================================================
void Memory_Write_Read_Direction_test_RLTD_8bpp_to_24bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;

 Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_8b_24bpp();
    Memory_24bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 LCD_DataWrite(color16M_blue>>8);
		 LCD_DataWrite(color16M_blue>>16);

		Check_Mem_WR_FIFO_not_Full();
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  MemRead_Right_Left_Top_Down();
  MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x00005555);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00005555>>8);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00005555>>16);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00>>8);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00>>16);
		  Check_Mem_WR_FIFO_not_Full();
	
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x00)
		  {	
		  	Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x00);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
   
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0x00)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x00);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);  
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
   
		   while(1);
		   }

		   temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();
		  if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
 
		   while(1);
		   }

		   temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();
		  if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
    
		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}
//=======================================================================
//=========================================================================
void Memory_Write_Read_Direction_test_DTLR_8bpp_to_24bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;

 Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_8b_24bpp();
    Memory_24bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 LCD_DataWrite(color16M_blue>>8);
		 LCD_DataWrite(color16M_blue>>16);

		 Check_Mem_WR_FIFO_not_Full();
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  MemRead_Down_Top_Left_Right();
  MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x00005555);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x00005555>>8);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x00005555>>16);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x00aaaa00>>8);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x00aaaa00>>16);
		  Check_Mem_WR_FIFO_not_Full();

 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x00)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x00);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0x00)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x00);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150); 
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		   while(1);
		   }

		   temp = LCD_DataRead();
		   Check_Mem_RD_FIFO_not_Empty();
		  if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		   while(1);
		   }

		   temp = LCD_DataRead();
		   Check_Mem_RD_FIFO_not_Empty();
		  if(temp!=0xaa)
		  {	
		  	Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_TDLR_8bpp_to_24bpp(void)
{
     unsigned int i,j,k;
	 unsigned char temp;

 Host_Bus_8bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_8b_24bpp();
    Memory_24bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 LCD_DataWrite(color16M_blue>>8);
		 LCD_DataWrite(color16M_blue>>16);
		Check_Mem_WR_FIFO_not_Full();
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  MemWrite_Top_Down_Left_Right();
  MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x00005555);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00005555>>8);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00005555>>16);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00);
		  Check_Mem_WR_FIFO_not_Full();
	
		  LCD_DataWrite(0x00aaaa00>>8);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00>>16);
		  Check_Mem_WR_FIFO_not_Full();
	
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x55)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x55);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x00)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x00);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

          if(temp!=0x00)
		  {
		  	Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0x00);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		   temp = LCD_DataRead();
		   Check_Mem_RD_FIFO_not_Empty();
		  if(temp!=0xaa)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

		   temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();
		  if(temp!=0xaa)
		  {
		  Goto_Text_XY(300+(j*800),50);
		   Print_Hex(0xaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   while(1);
		   }

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}
//=======================================================================

//=========================================================================
void Memory_Write_Read_Direction_test_LRTD_16bpp_to_24bpp(void)
{
     unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 //Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000/2;i++)
		{				
		 LCD_DataWrite(color16M_blue);
	     temp = color16M_blue>>16;
		 temp|=	color16M_blue<<8;
		 LCD_DataWrite(temp);
		 LCD_DataWrite(color16M_blue>>8);
		 Check_Mem_WR_FIFO_not_Full();
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	delay_seconds(1);


  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

   while(1)
   {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<1;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/4;k++)
		 {				
		  LCD_DataWrite(0x00005555);
		  Check_Mem_WR_FIFO_not_Full();

		  temp = 0x00005555>>16;
		  temp|=0x00aaaa00<<8;
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00>>8);
		  Check_Mem_WR_FIFO_not_Full();


		  LCD_DataWrite(0x00001111);
		  Check_Mem_WR_FIFO_not_Full();
	
		  temp = 0x00001111>>16;
		  temp|=0x00cccc00<<8;
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00cccc00>>8);
		  Check_Mem_WR_FIFO_not_Full();

 		 }

		 Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<1;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/4;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x5555)
		  {
		   GPIO_ResetBits(GPIOE, GPIO_Pin_4);//test pin 輸出low
	       delay_us(1);
	       GPIO_SetBits(GPIOE, GPIO_Pin_4);//test pin

		  Text_Mode();
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		   printf("\r\n Read left read_data=0x%x, expect_data=0x%x, i=0x%x, j=0x%x, k=0x%x error\r\n", temp, 0x5555, i, j, k);

		   printf("\r\n Error \r\n");


		  while(1);
		  }

		  temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x0000)
		  {
		  		   GPIO_ResetBits(GPIOE, GPIO_Pin_4);//test pin 輸出low
	       delay_us(1);
	       GPIO_SetBits(GPIOE, GPIO_Pin_4);//test pin	
		  Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x0000);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   printf("\r\n Error \r\n");

		   printf("\r\n Read left read_data=0x%x, expect_data=0x%x, i=0x%x, j=0x%x, k=0x%x error\r\n", temp, 0x0000, i, j, k);
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0xaaaa)
		  {
		  		   GPIO_ResetBits(GPIOE, GPIO_Pin_4);//test pin 輸出low
	       delay_us(1);
	       GPIO_SetBits(GPIOE, GPIO_Pin_4);//test pin
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   printf("\r\n Error \r\n");
		   printf("\r\n Read left read_data=0x%x, expect_data=0x%x, i=0x%x, j=0x%x, k=0x%x error\r\n", temp, 0xaaaa, i, j, k);
		  while(1);
		  }


		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x1111)
		  {
		    		   GPIO_ResetBits(GPIOE, GPIO_Pin_4);//test pin 輸出low
	       delay_us(1);
	       GPIO_SetBits(GPIOE, GPIO_Pin_4);//test pin

		  Text_Mode();
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		   printf("\r\n Read left read_data=0x%x, expect_data=0x%x, i=0x%x, j=0x%x, k=0x%x error\r\n", temp, 0x5555, i, j, k);

		   printf("\r\n Error \r\n");

		  while(1);
		  }

		  temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x0000)
		  {	
		   		   GPIO_ResetBits(GPIOE, GPIO_Pin_4);//test pin 輸出low
	       delay_us(1);
	       GPIO_SetBits(GPIOE, GPIO_Pin_4);//test pin

		  Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x0000);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   printf("\r\n Error \r\n");

		   printf("\r\n Read left read_data=0x%x, expect_data=0x%x, i=0x%x, j=0x%x, k=0x%x error\r\n", temp, 0x0000, i, j, k);
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0xcccc)
		  {
		  		   GPIO_ResetBits(GPIOE, GPIO_Pin_4);//test pin 輸出low
	       delay_us(1);
	       GPIO_SetBits(GPIOE, GPIO_Pin_4);//test pin

		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);	   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);	   
	       Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		   printf("\r\n Error \r\n");
		   printf("\r\n Read left read_data=0x%x, expect_data=0x%x, i=0x%x, j=0x%x, k=0x%x error\r\n", temp, 0xaaaa, i, j, k);
		  while(1);
		  }


 		 }
		  
		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		   
		 //delay_seconds(1);
		}

	  }

    }
	
}


//=========================================================================
void Memory_Write_Read_Direction_test_RLTD_16bpp_to_24bpp(void)
{
     unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000/2;i++)
		{				
		 LCD_DataWrite(color16M_blue);
	     temp = color16M_blue>>16;
		 temp|=	color16M_blue<<8;
		 LCD_DataWrite(temp);
		 LCD_DataWrite(color16M_blue>>8);
		 Check_Mem_WR_FIFO_not_Full();
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
		sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  MemRead_Right_Left_Top_Down();
  MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	   for(k=0; k< 480000/2;k++)
		 {				
	      LCD_DataWrite(0x00005555);
		  Check_Mem_WR_FIFO_not_Full();

		  temp = 0x00005555>>16;
		  temp|=0x00aaaa00<<8;
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		  LCD_DataWrite(0x00aaaa00>>8);
		  Check_Mem_WR_FIFO_not_Full();

 		 }
		 Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800+799,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x5555)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x0000)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x0000);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0xaaaa)
		  {	
		  	Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
   
		  while(1);
		  }	

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}
//=======================================================================
//=========================================================================
void Memory_Write_Read_Direction_test_DTLR_16bpp_to_24bpp(void)
{
     unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 


 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();


      Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000/2;i++)
		{				
		 LCD_DataWrite(color16M_blue);
	     temp = color16M_blue>>16;
		 temp|=	color16M_blue<<8;
		 LCD_DataWrite(temp);
		 LCD_DataWrite(color16M_blue>>8);
		 Check_Mem_WR_FIFO_not_Full();
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
		sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  MemRead_Down_Top_Left_Right();
  MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,599);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		 LCD_DataWrite(0x00005555);
		  Check_Mem_WR_FIFO_not_Full();

		  temp = 0x00005555>>16;
		  temp|=0x00aaaa00<<8;
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00>>8);
		  Check_Mem_WR_FIFO_not_Full();
 		 }
		 Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,599);
	   LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x5555)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x0000)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x0000);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0xaaaa)
		  {	
		  	Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
   
		  while(1);
		  }	

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}

//=========================================================================
void Memory_Write_Read_Direction_test_TDLR_16bpp_to_24bpp(void)
{
     unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 


 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

      Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000/2;i++)
		{				
		 LCD_DataWrite(color16M_blue);
	     temp = color16M_blue>>16;
		 temp|=	color16M_blue<<8;
		 LCD_DataWrite(temp);
		 LCD_DataWrite(color16M_blue>>8);

		 Check_Mem_WR_FIFO_not_Full();
 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
		sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	delay_seconds(1);

  //MemWrite_Left_Right_Top_Down()
  //MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  MemWrite_Top_Down_Left_Right();
  MemRead_Top_Down_Left_Right();

 while(1)
 {
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/2;k++)
		 {				
		  LCD_DataWrite(0x00005555);
		  Check_Mem_WR_FIFO_not_Full();

		  temp = 0x00005555>>16;
		  temp|=0x00aaaa00<<8;
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();

		  LCD_DataWrite(0x00aaaa00>>8);
		  Check_Mem_WR_FIFO_not_Full();
 		 }
		 Check_Mem_WR_FIFO_Empty();
		 //delay_seconds(1);
		}
	  }


	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	   Goto_Pixel_XY(j*800,0);
	   LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
	    for(k=0; k<480000/2;k++)
		 {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x5555)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x5555);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");

		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x0000)
		  {
		   Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0x0000);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
		  while(1);
		  }

		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0xaaaa)
		  {	
		  	Goto_Text_XY(300+(j*800),50);
		   Print_Hex_4(0xaaaa);
		   Goto_Text_XY(300+(j*800),100);
		   Print_Hex_4(temp);
		   Goto_Text_XY(300+(j*800),150);		   
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
		   Print_Decimal_6(k);
		   Goto_Text_XY(300+(j*800),250);
 		   Show_String("Read error");
   
		  while(1);
		  }	

 		 }

		   Goto_Text_XY(300+(j*800),150);
		   Text_Mode();
		   LCD_CmdWrite(0x04);
		   Print_Decimal(j);
		   Goto_Text_XY(300+(j*800),200);
 		   Show_String("Read Pass");
           Graphic_Mode(); //back to graphic mode
		 //delay_seconds(1);
		}

	  }

    }
	
}
//=======================================================================

/*REG[03h]ICR*/

void Graphic_Cursor_test_8bpp(void)
{
    unsigned int i,j,k;
	unsigned int temp;
  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();


		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");
	   }


  //資料寫入graphic cursor
  Memory_Select_Graphic_Cursor_RAM(); 
  Graphic_Mode();

    Select_Graphic_Cursor_1();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_pen_il[i]);
  	  Check_Mem_WR_FIFO_not_Full();
    }

    Select_Graphic_Cursor_2();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_arrow_il[i]);
  	   Check_Mem_WR_FIFO_not_Full();
    }

     Select_Graphic_Cursor_3();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_busy_im[i]);
	  Check_Mem_WR_FIFO_not_Full();
     }

     Select_Graphic_Cursor_4();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_no_im[i]);
 	  Check_Mem_WR_FIFO_not_Full();
     }

    Memory_Select_SDRAM();//寫完後切回SDRAM


    Set_Graphic_Cursor_Color_1(0xff);
    Set_Graphic_Cursor_Color_2(0x00);
    Graphic_Cursor_XY(100,100);
    Enable_Graphic_Cursor();

	/*底下測試在cursor底下一直寫入*/
	/*
	Main_Window_Start_XY(0,0);
	Active_Window_XY(50,50);
	Active_Window_WH(200,200);
    while(1)		
    {         
	   Goto_Pixel_XY(50,50);
	   LCD_CmdWrite(0x04);
	  for(j=0;j<256;j++)
	  {
   	   for(i=0; i<200*200;i++)
		{				
		 LCD_DataWrite(j);

		 Check_Mem_WR_FIFO_not_Full();
 		}
		delay_ms(500);
       }  
	}
	*/

   /*測試cursor位置變更*/
  while(1)
  {

	
	for(k=0;k<10;k++)
	{
	  Main_Window_Start_XY(k*800,0);
      delay_ms(20);
	  Active_Window_WH(800,600);
	  Active_Window_XY(k*800,0);

     Select_Graphic_Cursor_1();
     for(i=0;i<600;i+=6)
     {
      for(j=0;j<800;j+=10)
	  {
	   Graphic_Cursor_XY(j,i);
	   delay_ms(5);
	  }
      }

	}

	Select_Graphic_Cursor_1();
    //delay_seconds(1);
	delay_ms(500);
   Select_Graphic_Cursor_2();
	delay_ms(500);
   //delay_seconds(1);
   Select_Graphic_Cursor_3();
   delay_ms(500);
   //delay_seconds(1);
   Select_Graphic_Cursor_4();
	delay_ms(500);
  // delay_seconds(1); 
  }


}


void Graphic_Cursor_test_16bpp(void)
{
    unsigned int i,j,k;
	unsigned int temp;
  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    sd_showpic_65k_bin(800,600,"p2.bin");
	   }

	delay_seconds(1);

  //資料寫入graphic cursor
  Memory_Select_Graphic_Cursor_RAM(); 
  Graphic_Mode();

    Select_Graphic_Cursor_1();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_pen_il[i]);
      Check_Mem_WR_FIFO_not_Full();
    }

    Select_Graphic_Cursor_2();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_arrow_il[i]);
      Check_Mem_WR_FIFO_not_Full();
    }

     Select_Graphic_Cursor_3();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_busy_im[i]);
      Check_Mem_WR_FIFO_not_Full();
     }

     Select_Graphic_Cursor_4();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_no_im[i]);
      Check_Mem_WR_FIFO_not_Full();
     }

    Memory_Select_SDRAM();//寫完後切回SDRAM


    Set_Graphic_Cursor_Color_1(0xff);
    Set_Graphic_Cursor_Color_2(0x00);
    Graphic_Cursor_XY(100,100);
    Enable_Graphic_Cursor();
   


	//test change cursor
	/*
   Graphic_Cursor_XY(100,100);
	 while(1)
   {
	 
	 Select_Graphic_Cursor_1();
	 delay_seconds(1);
	 Select_Graphic_Cursor_2();
	 delay_seconds(1);
	 Select_Graphic_Cursor_3();
	 delay_seconds(1);
	 Select_Graphic_Cursor_4();
	 delay_seconds(1);
   }   */

   	/*底下測試在cursor底下一直寫入*/
	/*
	Main_Window_Start_XY(0,0);
	Active_Window_XY(50,50);
	Active_Window_WH(200,200);
    while(1)		
    {         
	   Goto_Pixel_XY(50,50);
	   LCD_CmdWrite(0x04);
	  for(j=0;j<65536;j++)
	  {
   	   for(i=0; i<200*200;i++)
		{				
		 LCD_DataWrite(j);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
 		}
		delay_ms(100);
       }  
	}  */

  /*測試cursor位置變更*/
  while(1)
  {	
  	for(k=0;k<10;k++)
	{

	Select_Graphic_Cursor_1();
	delay_ms(100);
	  Main_Window_Start_XY(k*800,0);
      delay_ms(20);
	  Active_Window_WH(800,600);
	  Active_Window_XY(k*800,0);
   

     for(i=0;i<600;i+=6)
     {
        for(j=0;j<800;j+=8)
	    {
	    Graphic_Cursor_XY(j,i);
	    delay_ms(1);
	    }
      }
     }

   delay_seconds(1);
   
   Select_Graphic_Cursor_2();
    for(k=0;k<10;k++)
	{
	  Main_Window_Start_XY(k*800,0);
      delay_ms(20);
	  Active_Window_WH(800,600);
	  Active_Window_XY(k*800,0);
   

     for(i=0;i<600;i+=6)
     {
        for(j=0;j<800;j+=8)
	    {
	    Graphic_Cursor_XY(j,i);
	    delay_ms(1);
	    }
      }
     }
   delay_seconds(1);
   Select_Graphic_Cursor_3();
	  	for(k=0;k<1;k++)
	{
	  Main_Window_Start_XY(k*800,0);
      delay_ms(20);
	  Active_Window_WH(800,600);
	  Active_Window_XY(k*800,0);
   

     for(i=0;i<600;i+=6)
     {
        for(j=0;j<800;j+=8)
	    {
	    Graphic_Cursor_XY(j,i);
	    delay_ms(1);
	    }
      }
     }
   delay_seconds(1);
   Select_Graphic_Cursor_4();
     for(k=0;k<1;k++)
	{
	  Main_Window_Start_XY(k*800,0);
      delay_ms(20);
	  Active_Window_WH(800,600);
	  Active_Window_XY(k*800,0);

     for(i=0;i<600;i+=6)
     {
        for(j=0;j<800;j+=8)
	    {
	    Graphic_Cursor_XY(j,i);
	    delay_ms(1);
	    }
      }
     }
   delay_seconds(1); 
  }


}


void Graphic_Cursor_test_24bpp(void)
{
    unsigned int i,j,k;
	unsigned int temp;
  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
    Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();
	
	for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	   }

	delay_seconds(1);





	/*測試顏色*/
	/*
	while(1)
	{
	 Set_Graphic_Cursor_Color_1(color256_red);
	 Set_Graphic_Cursor_Color_2(color256_black);
	 delay_seconds(1);

	 Set_Graphic_Cursor_Color_1(color256_green);
	 Set_Graphic_Cursor_Color_2(color256_black);
	 delay_seconds(1);

	 Set_Graphic_Cursor_Color_1(color256_blue);
	 Set_Graphic_Cursor_Color_2(color256_black);
	 delay_seconds(1);

     Set_Graphic_Cursor_Color_1(color256_black);
	 Set_Graphic_Cursor_Color_2(color256_red);
	 delay_seconds(1);

	 Set_Graphic_Cursor_Color_1(color256_black);
	 Set_Graphic_Cursor_Color_2(color256_green);
	 delay_seconds(1);

 	 Set_Graphic_Cursor_Color_1(color256_black);
	 Set_Graphic_Cursor_Color_2(color256_blue);
	 delay_seconds(1);
	}
	 */

    /*底下測試在cursor底下一直寫入*/
	/*
	Main_Window_Start_XY(0,0);
	Active_Window_XY(50,50);
	Active_Window_WH(200,200);
    while(1)		
    {         
	  Goto_Pixel_XY(50,50);
	   
	  for(j=0;j<16000000;j++)
	  {
	   //Enable_Graphic_Cursor();
	   LCD_CmdWrite(0x04);
   	   for(i=0; i<(200*200)/2;i++)
		{				
		 LCD_DataWrite(j);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(j);
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(j);
		 Check_Mem_WR_FIFO_not_Full();
 		}
		//Disable_Graphic_Cursor();
		delay_ms(100);
       }  
	}  
   	 */
  while(1)
  {
    for(k=0;k<10;k++)
	{
	  Main_Window_Start_XY(k*800,0);
      delay_ms(20);
	  Active_Window_WH(800,600);
	  Active_Window_XY(k*800,0);
     Select_Graphic_Cursor_1();
    for(i=0;i<3;i++)
     {
      for(j=0;j<800;j++)
	  {
	   Graphic_Cursor_XY(j,i);
	   delay_ms(1);
	  }
     }

    }

   delay_seconds(1);
   Main_Window_Start_XY(0,0);
   Graphic_Cursor_XY(200,200);
   Select_Graphic_Cursor_2();
   delay_seconds(1);
   Select_Graphic_Cursor_3();
   delay_seconds(1);
   Select_Graphic_Cursor_4();
   delay_seconds(1); 
  }

}

//=======================================================================
/*REG[0B][0C][0D]*/

/*0Bh bit7*/
void Resume_Interrupt_test(void)
{
   #define TFT_16bpp
   //#define TFT_24bpp  


   Interrupt_Active_Low();

   /*mix with key scan*/
   Long_Key_Timing_Adjustment(0x0c);
   Long_Key_enable();

   Key_Scan_Freg(0x07);
   Key_Scan_Wakeup_Function_Enable();
    
    Enable_KeyScan_Interrupt();
   Key_Scan_Enable();

   	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

 #ifdef TFT_24bpp
    
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();
	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();
 #endif

 #ifdef TFT_16bpp

    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();
	Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04); //
     sd_showpic_65k_bin(800,600,"p2.bin"); //
 #endif

   Display_ON();
   Graphic_cursor_initial(); //initaial and enable graphic cursor 

    Enable_Resume_Interrupt();
    //Disable_Resume_Interrupt();

    //Mask_Resume_Interrupt_Flag();
  
	 delay_seconds(3);
 while(1)
 {
   Clear_KeyScan_Interrupt_Flag();

	Display_OFF();				//display off 
    delay_ms(20);
    Power_Saving_Standby_Mode(); //
	//Power_Saving_Suspend_Mode(); //
	//Power_Saving_Sleep_Mode();
    delay_seconds(5);

    Power_Normal_Mode();
	//delay_ms(1);

	do{
	 delay_ms(1);
	}
	while(Power_Saving_Status());

	 if((Read_Interrupt_status()&0x80)==0x80)//read 0ch bit7 如果為1表示外部中斷
	 {
	   GPIO_SetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考
	   //delay_us(100);
	   delay_ms(1);
	   GPIO_ResetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考
	   Clear_Resume_Interrupt_Flag();
	 }

	 // delay_ms(200);
	 while((Read_Interrupt_status()&0x08)==0x08)
	 {
	   GPIO_SetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考
	   //delay_us(100);
	   delay_ms(1);
	   GPIO_ResetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考
	   Clear_KeyScan_Interrupt_Flag();
	  }

	delay_ms(1);
	Display_ON();			 

	delay_seconds(5);
  }


}

/*0Bh bit6*/ 


void External_Interrupt_Input_test(void)
{


   #define TFT_16bpp
   //#define TFT_24bpp  

	  #ifdef TFT_24bpp
    
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();
	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();
 #endif

 #ifdef TFT_16bpp

    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();
	Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04); //
     sd_showpic_65k_bin(800,600,"p2.bin"); //
 #endif

   Display_ON();

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 

   //ExtInterrupt_Debounce();	 //debounce 1024t osc clock
   ExtInterrupt_Nodebounce();

  
	Enable_ExtInterrupt_Input();
    //Disable_ExtInterrupt_Input();

	//Mask_ExtInterrupt_Input_Flag();


 //檢測low level trigger
   	 /*
   ExtInterrupt_Input_Low_Level_Trigger(); //設定外部	 
  while(1)
    {	
	 GPIO_ResetBits(GPIOC, GPIO_Pin_3);//test pin 輸出low
	 //delay_us(200);//with debounce
	 delay_us(1);//with no debounce

	  while((Read_Interrupt_status()&0x40)==0x40)//read 0ch bit6 如果為1表示外部中斷
	 {
	   GPIO_SetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考
	   GPIO_SetBits(GPIOC, GPIO_Pin_3);//test pin
	   Clear_ExtInterrupt_Input_Flag();
	   delay_us(1);
	 }
	  if((Read_Interrupt_status()&0x40)==0x00)
	  {GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	  delay_us(1);
	  }
	 }
	 */
	/*
  //檢測high level trigger

    ExtInterrupt_Input_High_Level_Trigger();
	while(1)
    {	
	 GPIO_SetBits(GPIOC, GPIO_Pin_3);//test pin 輸出high
	 delay_us(3);

	 while((Read_Interrupt_status()&0x40)==0x40)//read 0ch bit6 如果為1表示外部中斷
	 {
	   GPIO_SetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考
	   GPIO_ResetBits(GPIOC, GPIO_Pin_3);//test pin output low
	   Clear_ExtInterrupt_Input_Flag();
	   delay_us(3);
	 }
	  
	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	  delay_us(3);
	 

	 }
	 */

    //檢測Falling Edge trigger
	/*
    ExtInterrupt_Input_Falling_Edge_Trigger();
	while(1)
    {	
	  GPIO_ResetBits(GPIOC, GPIO_Pin_3);//test pin 輸出low
	  delay_us(300); //for with debounce
	  
      while((Read_Interrupt_status()&0x40)==0x40)//read 0ch bit6 如果為1表示外部中斷
	   {
	   GPIO_SetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考
	   GPIO_SetBits(GPIOC, GPIO_Pin_3);//test pin
	   Clear_ExtInterrupt_Input_Flag();
	   delay_us(300);	 //for with debounce
	   }

	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	  delay_us(300);   //for with debounce
	
	 }
	  */
	 
	
	 //檢測Risiing Edge trigger
	 ExtInterrupt_Input_Rising_Edge_Trigger();
	while(1)
    {	
	  GPIO_SetBits(GPIOC, GPIO_Pin_3);//test pin 輸出low
	  delay_us(500);
	  GPIO_ResetBits(GPIOC, GPIO_Pin_3);//test pin

	  if((Read_Interrupt_status()&0x40)==0x40)//read 0ch bit6 如果為1表示外部中斷
	 {
	   GPIO_SetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考

	   Clear_ExtInterrupt_Input_Flag();
	   delay_us(500);
	 }

	  if((Read_Interrupt_status()&0x40)==0x00)//read 0ch bit6 如果為1表示外部中斷
	  {	 
	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	  delay_us(500);
	   }

	 }
	  

 
}



/*0Bh bit5*/ 
void I2CM_Interrupt_test(void)
{
Graphic_cursor_initial(); //initaial and enable graphic cursor 

 RA8876_I2C_Master_test();
}



/*0Bh bit4,0ch bit4, 0dh bit4*/
void Vsync_Interrupt_test(void)
{
   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   //VSYNC_High_Active();
   VSYNC_Low_Active();

    Interrupt_Active_Low();
  //Interrupt_Active_High();

   Enable_Vsync_Interrupt();
  // Disable_Vsync_Interrupt();
   // Mask_Vsync_Interrupt_Flag();

 while(1)
 {
  	  if((Read_Interrupt_status()&0x10)==0x10)//read 0ch bit4 如果為1表示外部中斷
	 {
	   GPIO_SetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考

	   Clear_Vsync_Interrupt_Flag();
	   //delay_us(1);
	 }


	 if((Read_Interrupt_status()&0x10)==0x00)//read 0ch bit6 如果為1表示外部中斷
	 {
	 
	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	  //delay_us(1);
	 }

  } 
}

/*0Bh bit3*/
void KeyScan_Interrupt_test(void)
{
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
 //直接跳至底下副程式
    key_scan_test_5x5();

}

/*0Bh bit2*/
void DMA_Draw_BTE_Interrupt_test(void)
{
   unsigned int i;

 //**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();

Graphic_cursor_initial(); //initaial and enable graphic cursor 


Enable_SFlash_SPI();



Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(0);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12441600);	//32bit address

Main_Window_Start_XY(0,0);


  Enable_DMA_Draw_BTE_Interrupt();
  //Disable_DMA_Draw_BTE_Interrupt();

   while(1)
   {	  
	  Start_SFI_DMA();
	  Check_Busy_SFI_DMA();
	 
  	  while((Read_Interrupt_status()&0x04)==0x04)//read 0ch bit4 如果為1表示外部中斷
	 {
	   GPIO_SetBits(GPIOC, GPIO_Pin_1); //檢測到外部中斷後 提供一個外部信號參考
	   Clear_DMA_Draw_BTE_Interrupt_Flag();  
	 }
	  if((Read_Interrupt_status()&0x04)==0x00)
	  {
	   GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	  }

		 /*
	     Goto_Pixel_XY(0,0);
	    LCD_CmdWrite(0x04);
  	     for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00f0);
		 //Check_Busy_Mem_WR();
 		}
		 */
       }


}

/*0Bh bit1*/
void PWM1_Interrupt_test(void)
{
unsigned char temp;

  Graphic_cursor_initial(); //initaial and enable graphic cursor 

  Interrupt_Active_Low();
  //Interrupt_Active_High();

   PWM0_test();
   PWM1_test();	 //測試同時開啟用

   	LCD_CmdWrite(0x86);
	temp = LCD_DataRead();
	temp |= 0x11;
	LCD_DataWrite(temp);


   Enable_PWM1_Interrupt();
  //Disable_PWM1_Interrupt();
   //Mask_PWM1_Interrupt_Flag();

  while(1)
  {  
   
   while((Read_Interrupt_status()&0x02)==0x02)
    {
   	  

	  GPIO_SetBits(GPIOC, GPIO_Pin_1);
	  delay_us(1);
	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	  Clear_PWM1_Interrupt_Flag();
    }
   	 
	
//    if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))//check INT PIN
//    {
//    	LCD_CmdWrite(0x04);	
//		LCD_DataWrite(0x001f);
//
//        Clear_PWM1_Interrupt();
//    }
	
	 
		/*check status*/
//	 if((LCD_StatusRead()&0x01)==0x01)
//	 {
//	  Clear_PWM0_Interrupt_Flag();
//	  GPIO_SetBits(GPIOC, GPIO_Pin_1);
//	  delay_us(1);
//	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
//	 }

	
  }
}

/*0Bh bit0*/
void PWM0_Interrupt_test(void)
{ 
  Graphic_cursor_initial(); //initaial and enable graphic cursor 

   Interrupt_Active_Low();
  //Interrupt_Active_High();


  PWM0_test();
  //PWM1_test();  //測試同時開啟用

  Enable_PWM0_Interrupt();
  //Disable_PWM0_Interrupt();
  //Mask_PWM0_Interrupt_Flag();
 while(1)
 {
   
   while((Read_Interrupt_status()&0x01)==0x01)
    {
    	//LCD_CmdWrite(0x04);	
		//LCD_DataWrite(0x001f);
       Clear_PWM0_Interrupt_Flag();
	  GPIO_SetBits(GPIOC, GPIO_Pin_1);
	  delay_us(1);
	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
    }
	

	/*
	 if(!GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5))//check INT PIN
    {
    	LCD_CmdWrite(0x04);	
		LCD_DataWrite(0x001f);

        Clear_PWM0_Interrupt_Flag();
    }*/


	/*check status*/
//	 if((LCD_StatusRead()&0x01)==0x01)
//	 {
//	  Clear_PWM0_Interrupt_Flag();
//	  GPIO_SetBits(GPIOC, GPIO_Pin_1);
//	  delay_us(10);
//	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
//	 }



  }
}




/*REG[11h] */
void PIP_window_test_8bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	//Canvas_image_width(8188);//
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);


    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
   while(1)
   {
     for(i=0;i<1;i++)
     {
	  Main_Window_Start_XY(i*800,0);
	  delay_ms(20);
	  Active_Window_XY(i*800,0);
	  Goto_Pixel_XY(i*800,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_256_bin(800,600,"p1.bin");
	  Goto_Text_XY(300+(i*800),150);		   
	  Print_Decimal(i);
	  delay_ms(100);
	 }

	 Main_Window_Start_XY(0,0);
  	 delay_ms(100);

	//寫入PIP1圖
	Memory_8bpp_Mode();
	//Main_Image_Start_Address(800*600);
	//Main_Image_Width(800);
	delay_ms(20);
	Canvas_Image_Start_address(800*600);
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240;i++)
	{
	 LCD_DataWrite(0x00);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address(800*600*2);
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240;i++)
	{
	 LCD_DataWrite(0xFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_8bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(800*600);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(120,120);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_8bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address(800*600*2);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(300,300);

	Enable_PIP2();
	delay_seconds(1);


	Canvas_Image_Start_address(800*600);
	Active_Window_XY(0,0);
	Active_Window_WH(320,240);

    Foreground_color_256(color256_yellow);
    Background_color_256(color256_purple);
	Goto_Text_XY(0,0);

	for(i=0;i<1000;i++)
	{
	Show_String("PIP1");
	}
	delay_seconds(1);
	Canvas_Image_Start_address(800*600*2);

	Foreground_color_256(color256_red);
    Background_color_256(color256_blue);
	Goto_Text_XY(0,0);

	for(i=0;i<1000;i++)
	{
	Show_String("PIP2");
	}

	while(1)
	{
	 Enable_PIP1();
	 Enable_PIP2();
     delay_seconds(2);
	 Disable_PIP1();
	 Disable_PIP2();
     delay_seconds(2);

	}


		   
   }	
}


/*REG[11h] */
void PIP_window_test_16bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	//Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
   while(1)
   {
     for(i=0;i<10;i++)
     {
	  Main_Window_Start_XY(i*800,0);
	  delay_ms(20);
	  Active_Window_XY(i*800,0);
	  Goto_Pixel_XY(i*800,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_65k_bin(800,600,"p2.bin");
	  Goto_Text_XY(300+(i*800),150);		   
	  Print_Decimal(i);
	  delay_ms(100);
	 }

	 Main_Window_Start_XY(800,0);
  	 delay_ms(100);

	//寫入PIP1圖
	Memory_16bpp_Mode();
	delay_ms(20);
	Canvas_Image_Start_address(8188*600*2);
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240;i++)
	{
	 LCD_DataWrite(0x0000);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address(8188*600*2+(800*600*2));
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240;i++)
	{
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_16bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(8188*600*2);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(120,120);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_16bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address(8188*600*2+(800*600*2));
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(10,10);

	Enable_PIP2();
	delay_seconds(1);

	Canvas_Image_Start_address(8188*600*2);
	Active_Window_XY(0,0);
	Active_Window_WH(320,240);

    Foreground_color_65k(color65k_yellow);
    Background_color_65k(color65k_purple);
	Goto_Text_XY(0,0);

	for(i=0;i<1000;i++)
	{
	Show_String("PIP1");
	}
	delay_seconds(2);

	Canvas_Image_Start_address(8188*600*2+(800*600*2));
	Active_Window_XY(0,0);
	Active_Window_WH(320,240);

	Foreground_color_65k(color65k_red);
    Background_color_65k(color65k_blue);
	Goto_Text_XY(0,0);

	for(i=0;i<1000;i++)
	{
	Show_String("PIP2");
	}
   	delay_seconds(2);

	while(1)
	{
	 Enable_PIP1();
	 Enable_PIP2();
     delay_seconds(2);
	 Disable_PIP1();
	 Disable_PIP2();
     delay_seconds(2);

	}


		   
   }	
}


/*REG[11h] */
void PIP_window_test_24bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 

      //Disable_Text_Cursor();
    Enable_Text_Cursor_Blinking();
    //Disable_Text_Cursor_Blinking();
    Blinking_Time_Frames(10);
    //**[3E][3Fh]**//
    Text_Cursor_H_V(3,15);
	Enable_Text_Cursor();


 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	//Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);



   while(1)
   {
     for(i=0;i<1;i++)
     {
	  Main_Window_Start_XY(i*800,0);
	  delay_ms(20);
	  Active_Window_XY(i*800,0);
	  Goto_Pixel_XY(i*800,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	  Goto_Text_XY(300+(i*800),150);		   
	  Print_Decimal(i);
	  delay_ms(100);
	 }

	 Main_Window_Start_XY(0,0);
  	 delay_ms(100);

	//寫入PIP1圖
	Memory_24bpp_Mode();
	delay_ms(20);
	Canvas_Image_Start_address(8188*600*3);
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240/2;i++)
	{
	 LCD_DataWrite(0x0000);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0x0000);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0x0000);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address((8188*600*3)+(800*600*3));
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240/2;i++)
	{
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_24bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(8188*600*3);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(120,120);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_24bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address((8188*600*3)+(800*600*3));
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(240,240);

	Enable_PIP2();
	delay_seconds(1);

	Canvas_Image_Start_address(8188*600*3);
	Active_Window_XY(0,0);
	Active_Window_WH(320,240);

    Foreground_color_16M(color16M_yellow);
    Background_color_16M(color16M_purple);
	Goto_Text_XY(0,0);

	for(i=0;i<1000;i++)
	{
	Show_String("PIP1");
	}
	delay_seconds(2);



	Foreground_color_16M(color16M_red);
    Background_color_16M(color16M_blue);
	Goto_Text_XY(0,0);

   while(1)
   {
	 for(i=0;i<65536;i++)
	 {
	  Canvas_Image_Start_address(8188*600*3);
	  Foreground_color_16M(i);
	  Show_String("PIP1");
	  delay_ms(100);
	  Canvas_Image_Start_address((8188*600*3)+(800*600*3));
	  Foreground_color_16M(i);
	  Show_String("PIP2");
	  delay_ms(100);
	 }
	}

   	delay_seconds(2);

	//for(i=0;i<2;i++)
	while(1)
	{
	 Enable_PIP1();
	 delay_seconds(1);
	 Enable_PIP2();
     delay_seconds(1);
	 Disable_PIP1();
	 delay_seconds(1);
	 Disable_PIP2();
     delay_seconds(1);
	}
		   
   }	
}
//=======================================================================

/*REG[12h] DPCR*/
 
void PCLK_Inversion_test(void)	//與DE信號一起測量
{
	 unsigned int i,j;


    Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

  Graphic_cursor_initial(); //initaial and enable graphic cursor 


 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	//Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);

		Goto_Pixel_XY(0,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	   

   //第一列白色 其他黑色
	  /*
  	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
	{	
	   
	   if(i%800==0)
        {	 			
		 LCD_DataWrite(0xffff);
		  
		 }
		 else
		 {
		 LCD_DataWrite(0x0000);
		   
		 }
 	}  */



  while(1)
  {
    DE_PCLK_Falling();
	delay_seconds(5);
    DE_PCLK_Rising();
	delay_seconds(5);
   }
}

void Display_Off_test(void)
{
Graphic_cursor_initial(); //initaial and enable graphic cursor 

 Power_saving_test_suspend_mode_16bpp();
}


void Display_Color_Bar_test(void)
{
  unsigned int i;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);	

	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//

	

  while(1)
  {

   Color_Bar_ON();
	for(i=0;i<7387;i++)
	{
	Main_Window_Start_XY(i,0);
	delay_ms(1);
	//delay_seconds(1);
	}

   delay_seconds(3);
   Color_Bar_OFF();
   Main_Window_Start_XY(0,0);
   delay_seconds(3);
  }
}

void Scan_Direction_test(void)
{
  //必需確認是否會影響寫入

   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   while(1)
  {
   VSCAN_T_to_B();
   HSCAN_L_to_R();
   delay_seconds(5);
   HSCAN_R_to_L();
   VSCAN_B_to_T();
   delay_seconds(5);

   }
}


void Parallel_PDATA_Output_Sequence(void)
{	
  unsigned int i;

    Graphic_cursor_initial(); //initaial and enable graphic cursor 
	/*
   	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
	{				
	  LCD_DataWrite(0x07ff); 
	 
    }

	 //PDATA_Set_RGB();  //淡藍色
     //PDATA_Set_GRB();  //紫色
	 //PDATA_Set_RBG();  //淡藍色
	 //PDATA_Set_GBR();  //黃色
	  //PDATA_Set_BRG(); //紫色
	 // PDATA_Set_BGR();	 //黃色
	while(1);*/
	
	 /*
	 TFT_16bit();

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
	{				
	  LCD_DataWrite(0xf801); 
	 
    }

	 PDATA_Set_RGB();  //紅色
	 delay_seconds(3);
     PDATA_Set_GRB();  //綠色
	 delay_seconds(3);
	 PDATA_Set_RBG();  //紅色
	 delay_seconds(3);
	 PDATA_Set_GBR();  //藍色
	 delay_seconds(3);
	 PDATA_Set_BRG(); //綠色
	 delay_seconds(3);
	 PDATA_Set_BGR();//藍色
	   while(1);
	  */
	  /*
	 TFT_18bit();
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
	{				
	  LCD_DataWrite(0x07e0); 
	
    }

	 PDATA_Set_RGB();  //綠色
	 delay_seconds(3);
     PDATA_Set_GRB();  //紅色
	 delay_seconds(3);
	 PDATA_Set_RBG();  //藍色
	 delay_seconds(3);
	 PDATA_Set_GBR();  //紅色
	 delay_seconds(3);
	 PDATA_Set_BRG(); //藍色
	 delay_seconds(3);
	 PDATA_Set_BGR();//綠色
	 while(1);
	 */

	TFT_24bit();
	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
	{				
	  LCD_DataWrite(0x07e0); 
	  
    }

	 PDATA_Set_RGB();  //綠色
	 delay_seconds(3);
     PDATA_Set_GRB();  //紅色
	 delay_seconds(3);
	 PDATA_Set_RBG();  //藍色
	 delay_seconds(3);
	 PDATA_Set_GBR();  //紅色
	 delay_seconds(3);
	 PDATA_Set_BRG(); //藍色
	 delay_seconds(3);
	 PDATA_Set_BGR();//綠色
	while(1);
   
  	/*
  	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
	{				
	  LCD_DataWrite(0x001f); 
	 
    }

	 //PDATA_Set_RGB();  //
     //PDATA_Set_GRB();  //
	 //PDATA_Set_RBG();  //
	 //PDATA_Set_GBR();  //
	  //PDATA_Set_BRG(); //
	   PDATA_Set_BGR();//
	while(1);
	 */

	  //LCD_CmdWrite(0x12);
	  //LCD_DataWrite(0xC6); 
	 //while(1);

	//PDATA_Set_RGB();  //
    //PDATA_Set_GRB();  //
	//PDATA_Set_RBG();  //
	 //PDATA_Set_GBR();  //
	 //PDATA_Set_BRG(); //
	 //PDATA_Set_BGR();//
	//while(1);

  
	//PDATA_IDLE_STATE();
  	while(1);


}



//=========================================================================
/*REG[84h~8Fh]*/
void PWM0_test(void)
{
 unsigned int i;

  // Graphic_cursor_initial(); //initaial and enable graphic cursor 

   Set_PWM_Prescaler_1_to_256(0); //Time base is "Core_Freq / (Prescaler + 1)"

   //Select_PWM0_Clock_Divided_By_1();
   //Select_PWM0_Clock_Divided_By_2();
   //Select_PWM0_Clock_Divided_By_4();
   Select_PWM0_Clock_Divided_By_8();

  //Select_PWM0_is_GPIO_C7();
  //Select_PWM0_is_Core_Clock();
  Select_PWM0();
  //Select_PWM1();  //enable dead zone function needs set 

   //Set_Timer0_Dead_Zone_Length(50);
   //Enable_PWM0_Dead_Zone();
   //Disable_PWM0_Dead_Zone();

   //Enable_PWM0_Inverter();
   //Disable_PWM0_Inverter();

   //Auto_Reload_PWM0(); //一般PWM輸出設定,後續改為default 20120504
   //One_Shot_PWM0();  //Buzzer用設定

	Set_Timer0_Count_Buffer(256);  //設定階數
    Set_Timer0_Compare_Buffer(128);	//DUTY
	
	 
    Start_PWM0();

	//delay_seconds(3);
    //Stop_PWM0();
 
	 /*
    Set_Timer0_Count_Buffer(65535);
    while(1)
    {
    for(i=0;i<65536;i++)
    {
    //delay_seconds(1);
    Set_Timer0_Compare_Buffer(i);
   	delay_ms(1);
    }
    } */
    

}



void PWM1_test(void)
{
  unsigned int i;

  //Graphic_cursor_initial(); //initaial and enable graphic cursor 

  Set_PWM_Prescaler_1_to_256(1); //Time base is "Core_Freq / (Prescaler + 1)"

   //Select_PWM1_Clock_Divided_By_1();
   //Select_PWM1_Clock_Divided_By_2();
   //Select_PWM1_Clock_Divided_By_4();
   Select_PWM1_Clock_Divided_By_8();

  //Select_PWM1_is_ErrorFlag();
  //Select_PWM1_is_Osc_Clock();
  Select_PWM1();
  
  //Enable_PWM1_Inverter();
  //Disable_PWM1_Inverter();
  //Auto_Reload_PWM1(); //一般PWM輸出設定,後續改為default 20120504
  //One_Shot_PWM1();  //Buzzer用設定

   Set_Timer1_Count_Buffer(256); //設定階數
   Set_Timer1_Compare_Buffer(128); //DUTY

   Start_PWM1();
   //Stop_PWM1();

   	/*
   Set_Timer1_Count_Buffer(65535);
  while(1)
  {
   for(i=0;i<65535;i++)
   {
    //delay_seconds(1);
    Set_Timer1_Compare_Buffer(i);
   	delay_ms(5);
    }
  } 
   */

}

void PWM_test(void)
{
 // PWM0_test();
 // PWM1_test();
   
  //底下測試同時打開的情況 
	LCD_CmdWrite(0x84);
	LCD_DataWrite(0x01);

	LCD_CmdWrite(0x85);
	LCD_DataWrite(0x0A);

    LCD_CmdWrite(0x88);
	LCD_DataWrite(0x80);
	LCD_CmdWrite(0x89);
	LCD_DataWrite(0x00);


    LCD_CmdWrite(0x8A);
	LCD_DataWrite(0x00);
	LCD_CmdWrite(0x8B);
	LCD_DataWrite(0x01);

	LCD_CmdWrite(0x8C);
	LCD_DataWrite(0x80);
	LCD_CmdWrite(0x8D);
	LCD_DataWrite(0x00);

	LCD_CmdWrite(0x8E);
	LCD_DataWrite(0x00);
	LCD_CmdWrite(0x8F);
	LCD_DataWrite(0x01);

	//LCD_CmdWrite(0x86);
	//LCD_DataWrite(0x23);

	delay_ms(1);

	LCD_CmdWrite(0x86);
	LCD_DataWrite(0x33);
	
	 /*
  	//LCD_CmdWrite(0x86);
	//LCD_DataWrite(0x33);
	 */
}


//=======================================================================

/*REG[13h] PCSR*/
void SYNC_Polarity_IdleState_test(void)
{
   Graphic_cursor_initial(); //initaial and enable graphic cursor 
  HSYNC_Low_Active();
  //HSYNC_High_Active();
  VSYNC_Low_Active();
  //VSYNC_High_Active();

  //DE_Low_Active();
  DE_High_Active();
  
while(1);

 //底下測試請參考 void Power_saving_test_sleep_mode(void)
 Idle_DE_Low();
 Idle_DE_High();
 Idle_PCLK_Low();
 Idle_PCLK_High();
 Idle_PDAT_Low();
 Idle_PDAT_High();
 Idle_HSYNC_Low();
 Idle_HSYNC_High();
 Idle_VSYNC_Low();
 Idle_VSYNC_High();


}


void Text_cursor_test_8bpp(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;
  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X4();
   Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");
	   }


    //Disable_Text_Cursor();
    Enable_Text_Cursor_Blinking();
    //Disable_Text_Cursor_Blinking();
    Blinking_Time_Frames(10);
    //**[3E][3Fh]**//
    Text_Cursor_H_V(1,15);
	Enable_Text_Cursor();

   /*測試cursor位置變更*/
  while(1)
  {		
  		//底下測試8188範圍
        /*
		Text_Mode();
		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
		for(i=0;i<800/8;i++)
		{
		  Goto_Text_XY(j*800+i*8,0);
		  Show_String("A");
		  delay_ms(200);
		 }
	   } */

		Text_Mode();
		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_WH(800,600);
		Active_Window_XY(j*800,0);
		for(i=0;i<600/64;i++)
		{
		  for(k=0;k<800/32;k++)
		   {
		  Goto_Text_XY(j*800+k*32,i*64);
		  Show_String("A");
		  //delay_ms(100);
	    //while(1)
		 //{
		   Graphic_Mode();
		   Goto_Pixel_XY(300+j*800,200);		      
		   Active_Window_XY(300+j*800,200);
		   Active_Window_WH(100,100);
	       LCD_CmdWrite(0x04);
   	       for(a=0; a<100*100;a++)
		    {				
		      LCD_DataWrite(0x00);
		    // 
		     Check_Mem_WR_FIFO_not_Full();
 		    }
			Check_Mem_WR_FIFO_Empty();
		   // delay_ms(5);
		  //}
   			 Active_Window_WH(800,600);
		     Active_Window_XY(j*800,0);
  		   }
		 }
	   }

  }

}


void Text_cursor_test_24bpp(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;
  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   Font_Height_X1();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);



		//Memory_8bpp_Mode();


    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	    Goto_Text_XY(300+(j*800),150);		   
	    Print_Decimal(j);
	    //Check_2D_Busy();
	   }

 
    //Disable_Text_Cursor();
    //Enable_Text_Cursor_Blinking();
    //Disable_Text_Cursor_Blinking();
    //Blinking_Time_Frames(60);
    //**[3E][3Fh]**//
    Text_Cursor_H_V(2,15);
	Enable_Text_Cursor();

   /*測試cursor位置變更*/
  while(1)
  {		
  		//底下測試8188範圍
        /*
		Text_Mode();
		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
		for(i=0;i<800/8;i++)
		{
		  Goto_Text_XY(j*800+i*8,0);
		  Show_String("A");
		  delay_ms(200);
		 }
	   } */
		Main_Window_Start_XY(0,0);
		delay_ms(20);
		
		Text_Mode();
		for(j=1;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_WH(800,600);
		Active_Window_XY(j*800,0);
	//	Goto_Text_XY(600,200);
		//delay_ms(1000);
		
		for(i=0;i<600/64;i++)
		{
		  for(k=0;k<800/32;k++)
		   {
		  Goto_Text_XY(j*800+k*32,i*64);
		  Show_String("A");
		  delay_ms(10);
		   
		   Graphic_Mode();
		   Goto_Pixel_XY(j*800+300,200);		      
		   Active_Window_XY(j*800+300,200);
		   Active_Window_WH(100,100);
	       LCD_CmdWrite(0x04);
   	       for(a=0; a<100*100/2;a++)
		    {				
		      LCD_DataWrite(0x0000);
		    // 
		     Check_Mem_WR_FIFO_not_Full();
			 LCD_DataWrite(0x0000);
		    // 
		     Check_Mem_WR_FIFO_not_Full();
			  LCD_DataWrite(0x0000);
		    // 
		     Check_Mem_WR_FIFO_not_Full();
 		    }
			Check_Mem_WR_FIFO_Empty();
		    delay_ms(5);  
		
   			 Active_Window_WH(800,600);
		     Active_Window_XY(j*800,0);
			 
  		   }
		 }
		 
	   }

  }

}


void Text_cursor_test_16bpp(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;
  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   Font_Height_X1();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
	    Goto_Pixel_XY(j*800,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_65k_bin(800,600,"p2.bin");
	   }


    //Disable_Text_Cursor();
    Enable_Text_Cursor_Blinking();
    //Disable_Text_Cursor_Blinking();
    Blinking_Time_Frames(10);
    //**[3E][3Fh]**//
    Text_Cursor_H_V(1,15);
	Enable_Text_Cursor();

   /*測試cursor位置變更*/
  while(1)
  {		
  		//底下測試8188範圍
        
		Text_Mode();
		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
		for(i=0;i<800/8;i++)
		{
		  Goto_Text_XY(j*800+i*8,0);
		  Show_String("A");
		  delay_ms(10);
		 }
	   } 

	   		Text_Mode();
		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);
		for(i=0;i<800/8;i++)
		{
		  Goto_Text_XY(j*800+i*8,580);
		  Show_String("A");
		  delay_ms(10);
		 }
	   } 


		Text_Mode();
		for(j=0;j<10;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_WH(800,600);
		Active_Window_XY(j*800,0);
		for(i=0;i<600/64;i++)
		{
		  for(k=0;k<800/32;k++)
		   {
		  Goto_Text_XY(j*800+k*32,i*64);
		  Show_String("A");
		  delay_ms(100);

		   Graphic_Mode();
		   Goto_Pixel_XY(300+j*800,200);		      
		   Active_Window_XY(30+j*800,200);
		   Active_Window_WH(100,100);
	       LCD_CmdWrite(0x04);
   	       for(a=0; a<100*100;a++)
		    {				
		      LCD_DataWrite(0x0000);
		    // 
		     Check_Mem_WR_FIFO_not_Full();
 		    }
			Check_Mem_WR_FIFO_Empty();
		    delay_ms(5);
   			 Active_Window_WH(800,600);
		     Active_Window_XY(j*800,0);
  		   }
		 }
	   }

  }

}



/*5Eh*/ 

void Read_modify_write_8bpp(void)
{
    unsigned int i,j,k;
	 unsigned int temp;

  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

  Graphic_cursor_initial(); //initaial and enable graphic cursor 
  Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<800*600;i++)
		{				
		 LCD_DataWrite(color256_blue);


 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	delay_seconds(1);


 
  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();


     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<1;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
        sd_showpic_256_bin(800,600,"p1.bin");
		//Check_Mem_WR_FIFO_Empty();//寫完後檢查
		 //delay_seconds(1);
		}
	  }

  while(1)
 {
	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*8188*600);
	   delay_ms(20);
       Canvas_Image_Start_address(i*8188*600);

	  for(j=0;j<1;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty(); 
	    for(k=0; k<800*600;k++)
		  {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();
		  //temp |= 0x5500;
		   LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Mem_WR_FIFO_Empty();//寫完後檢查
 		  }
		 delay_ms(5);
           Graphic_Mode(); //back to graphic mode
		 delay_seconds(1);
		}

	  }

    }
}


void Read_modify_write_16bpp(void)
{
    unsigned int i,j,k;
	 unsigned int temp;

  Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

  //Graphic_cursor_initial(); //initaial and enable graphic cursor 
  //Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<800*600;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		  

 		}
	delay_seconds(1);
	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);

	sd_showpic_65k_bin(800,600,"p2.bin");
	
	delay_seconds(1);

  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();


	 //=========================================================

  while(1)
  {
		Main_Image_Start_Address(0);
	   	delay_ms(20);
        Canvas_Image_Start_address(0);
	    Main_Window_Start_XY(0,0);
		delay_ms(20);
		Active_Window_XY(0,0);
		 

        Goto_Pixel_XY(0,0);
	    LCD_CmdWrite(0x04);
	    for(k=0; k< 480000/4;k++)
		 {				
		  LCD_DataWrite(0xff00);
		  Check_Mem_WR_FIFO_not_Full();
		  LCD_DataWrite(0xff00);
		  Check_Mem_WR_FIFO_not_Full();
		  // 
		  LCD_DataWrite(0x001f);
		  Check_Mem_WR_FIFO_not_Full();
		  LCD_DataWrite(0x001f);
		  Check_Mem_WR_FIFO_not_Full();
		  // 
	
 		 }
		  Check_Mem_WR_FIFO_Empty();
		 
 
	   	/*
	    Goto_Pixel_XY(0,0);
	    LCD_CmdWrite(0x04);
        sd_showpic_65k_bin(800,600,"p2.bin");

		Foreground_color_65k(0xfe00);
	    Square_Start_XY(0,0);
		Square_End_XY(799,599);

	    Start_Square();
		 */
		
	 while(1)
	 {
	     Goto_Pixel_XY(0,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();  //dummy

	    for(k=0; k<800*600/4;k++)
		  {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		   
		  if(temp!=0xff00)
		  {
		   Goto_Text_XY(300,50);
		   Print_Hex_4(0xff00);
		   Goto_Text_XY(300,100);
		   Print_Hex_4(temp);

		   Goto_Text_XY(300,250);
 		   Show_String("Read error");
		  while(1);
		  }

		   LCD_DataWrite(temp);
		   Check_Mem_WR_FIFO_not_Full();


		   temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		   
		  if(temp!=0xff00)
		  {
		   Goto_Text_XY(300,50);
		   Print_Hex_4(0xff00);
		   Goto_Text_XY(300,100);
		   Print_Hex_4(temp);

		   Goto_Text_XY(300,250);
 		   Show_String("Read error");
		  while(1);
		  }

		   LCD_DataWrite(temp);
		   Check_Mem_WR_FIFO_not_Full();


			
		   temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x001f)
		  {
		   Goto_Text_XY(300,50);
		   Print_Hex_4(0x001f);
		   Goto_Text_XY(300,100);
		   Print_Hex_4(temp);

		   Goto_Text_XY(300,250);
 		   Show_String("Read error");
		  while(1);
		  }

		   LCD_DataWrite(temp);
		   Check_Mem_WR_FIFO_not_Full();
		   
		      temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  if(temp!=0x001f)
		  {
		   Goto_Text_XY(300,50);
		   Print_Hex_4(0x001f);
		   Goto_Text_XY(300,100);
		   Print_Hex_4(temp);

		   Goto_Text_XY(300,250);
 		   Show_String("Read error");
		  while(1);
		  }

		   LCD_DataWrite(temp);
		   Check_Mem_WR_FIFO_not_Full();

 		  }
		  Check_Mem_WR_FIFO_Empty();//寫完後檢查
	 }

  }


   //=========================================================


     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<9;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
        sd_showpic_65k_bin(800,600,"p2.bin");
		//Check_Mem_WR_FIFO_Empty();//寫完後檢查
		 //delay_seconds(1);
		}
	  }

  while(1)
 {
	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*2*8188*600);
	   delay_ms(20);
       Canvas_Image_Start_address(i*2*8188*600);

	  for(j=0;j<9;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty(); 
	    for(k=0; k<800*600;k++)
		  {				
		  temp = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();
		  //temp |= 0x5500;
		   LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Mem_WR_FIFO_Empty();//寫完後檢查
 		  }
		 delay_ms(5);
           Graphic_Mode(); //back to graphic mode
		 delay_seconds(1);
		}

	  }

    }
}



void Read_modify_write_24bpp(void)
{
    unsigned int i,j,k;
	 unsigned int temp1,temp2,temp3;

 Host_Bus_16bit();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	//Graphic_cursor_initial(); //initaial and enable graphic cursor 
	//Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();


	
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	delay_seconds(1);


 
  MemWrite_Left_Right_Top_Down();
  MemRead_Left_Right_Top_Down();
  //MemRead_Right_Left_Top_Down();
  //MemWrite_Right_Left_Top_Down();
  //MemRead_Down_Top_Left_Right();
  //MemWrite_Down_Top_Left_Right();
  //MemWrite_Top_Down_Left_Right();
  //MemRead_Top_Down_Left_Right();



   //=========================================================

  while(1)
  {
        Main_Image_Start_Address(0);
	   	delay_ms(20);
        Canvas_Image_Start_address(0);
	    Main_Window_Start_XY(0,0);
		delay_ms(20);
		Active_Window_XY(0,0);

	    Goto_Pixel_XY(0,0);
	    LCD_CmdWrite(0x04);
        sd_showpic_16M_mode1_bin(800,600,"p3.bin");

		 /*
		Foreground_color_16M(0x000000);
	    Square_Start_XY(0,0);
		Square_End_XY(799,599);
		Start_Square_Fill();

        Foreground_color_16M(0xff0000);
	    Square_Start_XY(0,0);
		Square_End_XY(799,599);
		Start_Square();	
		 */

	     Goto_Pixel_XY(0,0);
	    LCD_CmdWrite(0x04);
	    temp1 = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty(); 

	 while(1)
	 {

	    for(k=0; k<800*600/2;k++)
		  {			  
		  //while(1)
		  //{			
		  temp1 = LCD_DataRead();
		  //delay_us(1);
		  Check_Mem_RD_FIFO_not_Empty();
		  //delay_us(1);
		  temp2 = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();
		  // delay_us(1);
		  temp3 = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();
		   //}

		   LCD_DataWrite(~temp1);
		  // delay_us(1);
		   Check_Mem_WR_FIFO_not_Full();

		   LCD_DataWrite(~temp2);
		   //delay_us(1);
		   Check_Mem_WR_FIFO_not_Full();

		   LCD_DataWrite(~temp3);
		   //delay_us(1);
		   Check_Mem_WR_FIFO_not_Full();
		 
 		  }
		  //Check_Mem_WR_FIFO_Empty();//寫完後檢查
	 }

  }


   //=========================================================
     for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
	   	delay_ms(20);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<1;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
        sd_showpic_16M_mode1_bin(800,600,"p3.bin");
		//Check_Mem_WR_FIFO_Empty();//寫完後檢查
		 //delay_seconds(1);
		}
	  }

  while(1)
 {
	 for(i=0;i<1;i++)
     {
	   Main_Image_Start_Address(i*3*8188*600);
	   delay_ms(20);
       Canvas_Image_Start_address(i*3*8188*600);

	  for(j=0;j<1;j++)
	   {
	    Main_Window_Start_XY(j*800,0);
		delay_ms(20);
		Active_Window_XY(j*800,0);

	    Goto_Pixel_XY(j*800,0);
	    LCD_CmdWrite(0x04);
	    temp1 = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty(); 
	    for(k=0; k<800*600/2;k++)
		  {				
		  temp1 = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  temp2 = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();

		  temp3 = LCD_DataRead();
		  Check_Mem_RD_FIFO_not_Empty();


		   LCD_DataWrite(temp1);
		  Check_Mem_WR_FIFO_not_Full();

		   LCD_DataWrite(temp2);
		  Check_Mem_WR_FIFO_not_Full();

		   LCD_DataWrite(temp3);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Mem_WR_FIFO_Empty();//寫完後檢查
 		  }
		 delay_ms(5);
           Graphic_Mode(); //back to graphic mode
		 delay_seconds(1);
		}

	  }

    }
}




void linear_mode_test_8bit_8bpp(void)
{
   unsigned int i,j,k;

  Graphic_cursor_initial(); //initaial and enable graphic cursor 

  Memory_Linear_Mode();
  Memory_8bpp_Mode();

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);	//linear mode 與active windows無關
    Active_Window_WH(100,100);


   Select_Main_Window_8bpp();
   RGB_16b_8bpp();
   Memory_8bpp_Mode();

   	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  while(1)
  {


     for(j=0;j<10;j++)
	 {
	 Canvas_Image_Start_address(j*800*600);
	 Main_Image_Start_Address(j*800*600);
	 delay_ms(20);
	
	 Goto_Linear_Addr(j*800*600);
	 LCD_CmdWrite(0x04);

	 sd_showpic_256_bin(800,600,"p1.bin");

	 delay_seconds(2);
	
	 Goto_Linear_Addr(j*800*600);
	 LCD_CmdWrite(0x04);
   	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
     //delay_seconds(2);
	   }
	 }





}


void linear_mode_test_8bit_16bpp(void)
{
   unsigned int i,j,k;

  Graphic_cursor_initial(); //initaial and enable graphic cursor 

  Memory_Linear_Mode();
  Memory_8bpp_Mode();

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);	//linear mode 與active windows無關
    Active_Window_WH(100,100);


   Select_Main_Window_16bpp();
   RGB_16b_8bpp();
   Memory_8bpp_Mode();

   	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 while(1)
 {

     for(j=0;j<10;j++)
	 {

	 Canvas_Image_Start_address(j*800*600*2);
	 Main_Image_Start_Address(j*800*600*2);
	 delay_ms(20);
	
	 Goto_Linear_Addr(j*800*600*2);
	
	 LCD_CmdWrite(0x04);
	 sd_showpic_MPU8_to_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	delay_seconds(1);
 

     Goto_Linear_Addr(j*800*600*2);
	 LCD_CmdWrite(0x04);
   	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color65k_blue>>8);
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
 		}
		Check_Mem_WR_FIFO_Empty();
	//delay_seconds(1);
     }

   }
	
}


void linear_mode_test_8bit_24bpp(void)
{
   unsigned int i,j,k;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 

    Memory_Linear_Mode();
    Memory_8bpp_Mode();

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);	//linear mode 與active windows無關
    Active_Window_WH(100,100);

    Select_Main_Window_24bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 /*
//測試寫入會偏問題
	Main_Image_Start_Address(0);
	Canvas_Image_Start_address(0);
while(1)
{
  
	Goto_Linear_Addr(0);
    Graphic_Mode();
	LCD_CmdWrite(0x04);

  for(j=0;j<600;j++)
  {
    	LCD_DataWrite(color16M_black);
        // 
	    Check_Mem_WR_FIFO_not_Full();
	    LCD_DataWrite(color16M_black>>8);
	    // 
		Check_Mem_WR_FIFO_not_Full();
		LCD_DataWrite(color16M_black>>16);
		// 
	    Check_Mem_WR_FIFO_not_Full();
		 
	 for(i=0;i<798;i++)
	  {				
	    LCD_DataWrite(color16M_black);
        // 
	    Check_Mem_WR_FIFO_not_Full();
	    LCD_DataWrite(color16M_black>>8);
	    // 
		Check_Mem_WR_FIFO_not_Full();
		LCD_DataWrite(color16M_black>>16);
		// 
		Check_Mem_WR_FIFO_not_Full();
 	   }

	    LCD_DataWrite(color16M_black);
        // 
	    Check_Mem_WR_FIFO_not_Full();
	    LCD_DataWrite(color16M_black>>8);
	    // 
		Check_Mem_WR_FIFO_not_Full();
		LCD_DataWrite(color16M_black>>16);
		// 
		Check_Mem_WR_FIFO_not_Full();
	}
	Check_Mem_WR_FIFO_Empty();
}  
 */
 while(1)
 {

	//測試一直寫入下半部1/3會突然閃


   
 while(1)
  {

     for(j=0;j<10;j++)
	 {

	 Canvas_Image_Start_address(j*800*600*3);
	 Main_Image_Start_Address(j*800*600*3);
	 delay_ms(20);
	
	 Goto_Linear_Addr(j*800*600*3);

     Graphic_Mode();
	 LCD_CmdWrite(0x04);
	 sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 delay_seconds(1);


      Goto_Linear_Addr(j*800*600*3);
	  LCD_CmdWrite(0x04);
   	  for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>8);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>16);
		 // 
		 Check_Mem_WR_FIFO_not_Full();

 		} 
		Check_Mem_WR_FIFO_Empty();
	 }
	}

	Main_Image_Start_Address(800*600*3);
	Canvas_Image_Start_address(800*600*3);
	Goto_Linear_Addr(800*600*3);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>8);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>16);
		 // 
		 Check_Mem_WR_FIFO_not_Full();

 		} 
		Check_Mem_WR_FIFO_Empty();
	//delay_seconds(1);
	

	Main_Image_Start_Address(800*600*3*2);
	Canvas_Image_Start_address(800*600*3*2);
	Goto_Linear_Addr(800*600*3*2);
	LCD_CmdWrite(0x04);
	sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");

	Check_Mem_WR_FIFO_Empty();
	//delay_seconds(1);

	Main_Image_Start_Address(800*600*3*3);
	Canvas_Image_Start_address(800*600*3*3);

	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>8);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>16);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
 		}

		Check_Mem_WR_FIFO_Empty();
	 //delay_seconds(1);
   


	Main_Image_Start_Address(800*600*3*4);
	Canvas_Image_Start_address(800*600*3*4);
	Goto_Linear_Addr(800*600*3*4);
	LCD_CmdWrite(0x04);
	sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");
	//delay_seconds(1);

	Main_Image_Start_Address(800*600*3*5);
	Canvas_Image_Start_address(800*600*3*5);
	Goto_Linear_Addr(800*600*3*5);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>8);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>16);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
 		}
	//delay_seconds(1);


	Main_Image_Start_Address(800*600*3*6);
	Canvas_Image_Start_address(800*600*3*6);
	Goto_Linear_Addr(800*600*3*6);
	LCD_CmdWrite(0x04);
	sd_showpic_MPU8_to_16M_bin(800,600,"p3.bin");
	delay_seconds(1);

	Main_Image_Start_Address(800*600*3*7);
	Canvas_Image_Start_address(800*600*3*7);
	Goto_Linear_Addr(800*600*3*7);
	LCD_CmdWrite(0x04);
   	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color16M_blue);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>8);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
		 LCD_DataWrite(color16M_blue>>16);
		 // 
		 Check_Mem_WR_FIFO_not_Full();
 		}
	//delay_seconds(1);


  }
}

 /*REG[67h~7Eh]*/
//=======================================================================
void Draw_line_test_8bpp(void)
{
unsigned long i,j;
unsigned long resx1,resy1,resx2,resy2;

Select_Main_Window_8bpp();

RGB_16b_8bpp();
Memory_8bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);

   	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");

	     Enable_DMA_Draw_BTE_Interrupt();
  //Disable_DMA_Draw_BTE_Interrupt();

	while(1)
	{
		i=rand()%256;
		j=rand()%2;
	    resx1=rand()%700;
        resy1=rand()%500;

		resx2=rand()%700;
        resy2=rand()%500;

        Foreground_color_256(i);
	    Line_Start_XY(resx1,resy1);
		Line_End_XY(resx2,resy2);

	    Start_Line();

		//delay_ms(10);			
		//while(1);
	   ///////////////////////////

		//test Draw INT 
		 while((Read_Interrupt_status()&0x04)==0x00);

  	    if((Read_Interrupt_status()&0x04)==0x04)//read 0ch bit4 如果為1表示外部中斷
	    {
	   GPIO_SetBits(GPIOE, GPIO_Pin_6); //檢測到外部中斷後 提供一個外部信號參考

	   Clear_DMA_Draw_BTE_Interrupt_Flag();
	   delay_us(1);
	   GPIO_ResetBits(GPIOE, GPIO_Pin_6);
	    }

	   ///////////////////////////
 	 }


	while(1)
	{

        Foreground_color_256(color256_yellow);
	   for(i=0;i<600;i++)
	   {
	    Line_Start_XY(0,i);
		Line_End_XY(799,i);
	    Start_Line();
		//delay_ms(1);
	    }
		
			delay_seconds(1);
		Foreground_color_256(color256_white);
	   for(i=0;i<800;i++)
	    {
	    Line_Start_XY(i,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
        Foreground_color_256(color256_black);
	   for(i=0;i<600;i++)
	   {
	    Line_Start_XY(799,i);
		Line_End_XY(0,i);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
		Foreground_color_256(color256_red);
	   for(i=0;i<800;i++)
	    {
	    Line_Start_XY(i,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);	
		Foreground_color_256(color256_blue);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(0,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
	    Foreground_color_256(color256_green);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(0,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
	    Foreground_color_256(color256_purple);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(799,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }		
		 	delay_seconds(1);
	    Foreground_color_256(color256_black);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(799,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);				
		//while(1);
 	 }
}


//=======================================================================
void Draw_line_test_16bpp(void)
{
unsigned long i,j;
unsigned long resx1,resy1,resx2,resy2;

Select_Main_Window_16bpp();

RGB_16b_16bpp();
Memory_16bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_65k(color65k_white);


  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_65k(color65k_black);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_65k_bin(800,600,"p2.bin");

	while(1)
	{
		i=rand()%65536;
		j=rand()%2;
	    resx1=rand()%600;
        resy1=rand()%400;

		resx2=rand()%600;
        resy2=rand()%400;

        Foreground_color_65k(i);
	    Line_Start_XY(resx1,resy1);
		Line_End_XY(resx2,resy2);

	    Start_Line();

		//delay_ms(10);			
		//while(1);
 	 }

	while(1)
	{

        Foreground_color_65k(color65k_yellow);
	   for(i=0;i<600;i++)
	   {
	    Line_Start_XY(0,i);
		Line_End_XY(799,i);
	    Start_Line();
		//delay_ms(1);
	    }
		
			delay_seconds(1);
		Foreground_color_65k(color65k_white);
	   for(i=0;i<800;i++)
	    {
	    Line_Start_XY(i,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
        Foreground_color_65k(color65k_black);
	   for(i=0;i<600;i++)
	   {
	    Line_Start_XY(799,i);
		Line_End_XY(0,i);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
		Foreground_color_65k(color65k_red);
	   for(i=0;i<800;i++)
	    {
	    Line_Start_XY(i,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);	
		Foreground_color_65k(color65k_blue);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(0,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
	    Foreground_color_65k(color65k_green);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(0,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
	    Foreground_color_65k(color65k_purple);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(799,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }		
		 	delay_seconds(1);
	    Foreground_color_65k(color65k_black);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(799,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);				
		//while(1);
 	 }
}


//=======================================================================
void Draw_line_test_24bpp(void)
{
unsigned long i,j;
unsigned long resx1,resy1,resx2,resy2;

Select_Main_Window_24bpp();

RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_16M(color16M_white);


  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_16M(color16M_black);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_16M_mode1_bin(800,600,"p3.bin");
   
	while(1)
	{
		i=rand()%16777216;
		j=rand()%2;
	    resx1=rand()%700;
        resy1=rand()%500;

		resx2=rand()%700;
        resy2=rand()%500;

        Foreground_color_16M(i);
	    Line_Start_XY(resx1,resy1);
		Line_End_XY(resx2,resy2);

        Start_Line();

		//delay_ms(1);			
		//while(1);
 	 }

	while(1)
	{

        Foreground_color_16M(color16M_yellow);
	   for(i=0;i<600;i++)
	   {
	    Line_Start_XY(0,i);
		Line_End_XY(799,i);
	    Start_Line();
		//delay_ms(1);
	    }
		
			delay_seconds(1);
		Foreground_color_16M(color16M_white);
	   for(i=0;i<800;i++)
	    {
	    Line_Start_XY(i,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
        Foreground_color_16M(color16M_black);
	   for(i=0;i<600;i++)
	   {
	    Line_Start_XY(799,i);
		Line_End_XY(0,i);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
		Foreground_color_16M(color16M_red);
	   for(i=0;i<800;i++)
	    {
	    Line_Start_XY(i,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);	
		Foreground_color_16M(color16M_blue);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(0,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
	    Foreground_color_16M(color16M_green);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(0,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);
	    Foreground_color_16M(color16M_purple);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(799,599);
		Line_End_XY(i,0);
	    Start_Line();
		//delay_ms(1);
	    }		
		 	delay_seconds(1);
	    Foreground_color_16M(color16M_black);
	   for(i=0;i<800;i++)
	    {		 
	    Line_Start_XY(799,0);
		Line_End_XY(i,599);
	    Start_Line();
		//delay_ms(1);
	    }
			delay_seconds(1);				
		//while(1);
 	 }
}


void Draw_square_test_8bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;


	Main_Image_Start_Address(0);				
	Main_Image_Width(8188);							
	Main_Window_Start_XY(0,0);	
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);


Select_Main_Window_8bpp();

RGB_16b_8bpp();
Memory_8bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);
  
 	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


		Active_Window_XY(0,0);
        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");

		Active_Window_XY(800,0);
        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(800,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");

	    Active_Window_XY(0,0);
        Active_Window_WH(800,600);
	while(1)
	{
		i=rand()%256;
		j=rand()%2;
	    resx1=rand()%700;
        resy1=rand()%500;

		resx2=rand()%700;
        resy2=rand()%500;

        Foreground_color_256(i);
	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);

	    if(j)
		{
	    Start_Square();
		Main_Image_Start_Address(0);
		delay_ms(100);
		}
		else
		{
		Start_Square_Fill();
		Main_Image_Start_Address(800);
		delay_ms(100);
		}

		//delay_ms(10);			
		//while(1);
 	 }
}


void Draw_square_test_16bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;


 	Main_Image_Start_Address(0);				
	Main_Image_Width(8188);							
	Main_Window_Start_XY(0,0);	
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);


Select_Main_Window_16bpp();

RGB_16b_16bpp();
Memory_16bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

     Active_Window_WH(800,600);
	 Goto_Pixel_XY(0,0);
	 Graphic_Mode();
	 LCD_CmdWrite(0x04);
	 sd_showpic_65k_bin(800,600,"p2.bin");
	Main_Image_Start_Address(8188*600*2);
	Canvas_Image_Start_address(8188*600*2);
	 Goto_Pixel_XY(0,0);
	 Graphic_Mode();
	 LCD_CmdWrite(0x04);
	 sd_showpic_65k_bin(800,600,"p2.bin");

	while(1)
	{
		i=rand()%65536;
		j=rand()%2;
	    resx1=rand()%600;
        resy1=rand()%400;

		resx2=rand()%600;
        resy2=rand()%400;

        Foreground_color_65k(i);
	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);

	    if(j)
		{
	    Start_Square();
		//Main_Image_Start_Address(0);
		//delay_ms(500);
		}
		else
		{
		Start_Square_Fill();
		//Main_Image_Start_Address(8188*600*2);
		//delay_ms(500);
		}
		//delay_ms(10);			
		//while(1);
 	 }
}


void Draw_square_test_24bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;



	Main_Image_Start_Address(0);				
	Main_Image_Width(8188);							
	Main_Window_Start_XY(0,0);	
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(8188,600);


Select_Main_Window_24bpp();

RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

     Active_Window_WH(800,600);
	 Goto_Pixel_XY(0,0);
	 Graphic_Mode();
	 LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin");
/*
	 Main_Image_Start_Address(800*600*3);
	 delay_ms(50);
     Canvas_Image_Start_address(800*600*3);
	 Goto_Pixel_XY(0,0);
	 Graphic_Mode();
	 LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	*/
	 Active_Window_WH(8000,600);
	 Main_Image_Start_Address(0);
	while(1)
	{
		i=rand()%16777216;
		j=rand()%2;
	    resx1=rand()%7200;
        resy1=rand()%500;

		resx2=rand()%7200;
        resy2=rand()%500;


	   Main_Window_Start_XY(resx1,0);
	   delay_ms(20);

        Foreground_color_16M(i);
	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);

	    if(j)
		{
	    Start_Square();
		//Main_Image_Start_Address(0);
		delay_ms(500);
		}	
		else
		{
		Start_Square_Fill();
		//Main_Image_Start_Address(800*600*3);
		delay_ms(500);
		}
				
		//while(1);
 	 }

	

}



void Draw_circle_square_test_8bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;

unsigned long k;

Select_Main_Window_8bpp();

RGB_16b_8bpp();
Memory_8bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");

	while(1)
	{
		
		do
		{
	     resx1=rand()%600;
         resy1=rand()%400;

		 resx2=rand()%600;
         resy2=rand()%400;
		 
        }while((resx1==resx2)||(resy1==resy2));


		i=rand()%256;
        Foreground_color_256(i);


	    if(resx1<resx2)
		  {resx2+=10;
		  i = resx2-resx1;}
		else 	  
		  {	resx1+=10;
		  i = resx1-resx2;}

		if(resy1<resy2)
		  {	resy2+=10;
		  j = resy2-resy1;}
		else 	  
		  {	resy1+=10;
		  j = resy1-resy2;}

		  i/=9;
		  if(i<1)
		  i = 1;

		  j/=9;
		  if(j<1)
		  j = 1;

		  if(i<j)
		  k=i;
		  else if (i>j)
		  k=j;
		  else
		  k=1;

	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);


		  //printf("\r\n Error \r\n");
		  printf("%d\r\n",resx1);
		  printf("%d\r\n",resy1);
		  printf("%d\r\n",resx2);
		  printf("%d\r\n",resy2);

          printf("%d\r\n",i);
		  printf("%d\r\n",j);
		  

		
		Circle_Square_Radius_RxRy (k,k);

		j=rand()%3;
	    if(j)
	    Start_Circle_Square();
		else
		Start_Circle_Square_Fill();

		//delay_ms(500);			
		//while(1);
 	 }
}



void Draw_circle_square_test_16bpp(void)
{
unsigned long i,j,k;

unsigned long resx1,resy1,resx2,resy2;

Select_Main_Window_16bpp();

RGB_16b_16bpp();
Memory_16bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

     Active_Window_WH(800,600);
	 Goto_Pixel_XY(0,0);
	 Graphic_Mode();
	 LCD_CmdWrite(0x04);
	 sd_showpic_65k_bin(800,600,"p2.bin");

	while(1)
	{

		do
		{
	     resx1=rand()%600;
         resy1=rand()%400;

		 resx2=rand()%600;
         resy2=rand()%400;
		 
        }while((resx1==resx2)||(resy1==resy2));


			i=rand()%65536;
        Foreground_color_65k(i);


	    if(resx1<resx2)
		  {resx2+=10;
		  i = resx2-resx1;}
		else 	  
		  {	resx1+=10;
		  i = resx1-resx2;}

		if(resy1<resy2)
		  {	resy2+=10;
		  j = resy2-resy1;}
		else 	  
		  {	resy1+=10;
		  j = resy1-resy2;}

		  i/=9;
		  if(i<1)
		  i = 1;

		  j/=9;
		  if(j<1)
		  j = 1;

		  if(i<j)
		  k=i;
		  else if (i>j)
		  k=j;
		  else
		  k=1;

	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);


		  //printf("\r\n Error \r\n");
		  printf("%d\r\n",resx1);
		  printf("%d\r\n",resy1);
		  printf("%d\r\n",resx2);
		  printf("%d\r\n",resy2);

          printf("%d\r\n",i);
		  printf("%d\r\n",j);
		  

		
		Circle_Square_Radius_RxRy (k,k);

		j=rand()%3;
	    if(j)
	    Start_Circle_Square();
		else
		Start_Circle_Square_Fill();
 	 }
}



void Draw_circle_square_test_24bpp(void)
{
unsigned long i,j,k;

unsigned long resx1,resy1,resx2,resy2;

Select_Main_Window_24bpp();

RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

     Active_Window_WH(800,600);
	 Goto_Pixel_XY(0,0);
	 Graphic_Mode();
	 LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	while(1)
	{
	 	do
		{
	     resx1=rand()%600;
         resy1=rand()%400;

		 resx2=rand()%600;
         resy2=rand()%400;
		 
        }while((resx1==resx2)||(resy1==resy2));


		i=rand()%16777216;
        Foreground_color_16M(i);


	    if(resx1<resx2)
		  {resx2+=10;
		  i = resx2-resx1;}
		else 	  
		  {	resx1+=10;
		  i = resx1-resx2;}

		if(resy1<resy2)
		  {	resy2+=10;
		  j = resy2-resy1;}
		else 	  
		  {	resy1+=10;
		  j = resy1-resy2;}

		  i/=9;
		  if(i<1)
		  i = 1;

		  j/=9;
		  if(j<1)
		  j = 1;

		  if(i<j)
		  k=i;
		  else if (i>j)
		  k=j;
		  else
		  k=1;

	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);


		  //printf("\r\n Error \r\n");
		  printf("%d\r\n",resx1);
		  printf("%d\r\n",resy1);
		  printf("%d\r\n",resx2);
		  printf("%d\r\n",resy2);

          printf("%d\r\n",i);
		  printf("%d\r\n",j);
		  

		
		Circle_Square_Radius_RxRy (k,k);

		j=rand()%3;
	    if(j)
	    Start_Circle_Square();
		else
		Start_Circle_Square_Fill();
 	 }
}



void Draw_triangle_test_8bpp(void)
{
unsigned long i;
unsigned short resx1,resy1,resx2,resy2,resx3,resy3;
unsigned char j;
unsigned int temp;
Select_Main_Window_8bpp();

RGB_16b_8bpp();
Memory_8bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");

		Goto_Pixel_XY(0,0);
		LCD_CmdWrite(0x04);
		for(i=0;i<800*600;i++)
		{
		   LCD_DataWrite(0x00);	
		  // 
		   Check_Mem_WR_FIFO_not_Full();//
		}
		delay_ms(10);	

		 /*
		Foreground_color_256(color256_purple);
		Triangle_Point1_XY(92,424);
		Triangle_Point2_XY(511,303);
		Triangle_Point3_XY(194,394);
	
		Start_Triangle_Fill();

		while(1);
		*/
	while(1)
	{

	//	i=rand()%256;
		j=rand()%3;

	    resx1=rand()%700;
        resy1=rand()%500;

		resx2=rand()%700;
        resy2=rand()%500;

		resx3=rand()%700;
        resy3=rand()%500;

	//	Foreground_color_256(i);	
        Foreground_color_256(color256_purple);

        Triangle_Point1_XY(resx1,resy1);
		Triangle_Point2_XY(resx2,resy2);
		Triangle_Point3_XY(resx3,resy3);
	
		Start_Triangle_Fill();

	   /*
	    if(j)
		{
		 //printf("%d",j);
		 Start_Triangle();
		}		  
		else
		{
		 //printf("%d",j);
		 Start_Triangle_Fill();
		 }	*/
		//delay_ms(10);			
		//while(1);



		 //read and check 
		for(i=0;i<300;i++)
		{
		 Goto_Pixel_XY(700,i);
		 LCD_CmdWrite(0x04);
	     temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();
		 temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();

		 if(temp!=0x00)
		 {
		  printf("\r\n Error \r\n");
		  printf("%d\r\n",resx1);
		  printf("%d\r\n",resy1);
		  printf("%d\r\n",resx2);
		  printf("%d\r\n",resy2);
		  printf("%d\r\n",resx3);
		  printf("%d\r\n",resy3);
		  while(1);
		 }

		}


 	 }
}


void Draw_triangle_test_16bpp(void)
{
unsigned long i;
unsigned short resx1,resy1,resx2,resy2,resx3,resy3;
unsigned char j;
Select_Main_Window_16bpp();

RGB_16b_16bpp();
Memory_16bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);
 
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_65k_bin(800,600,"p2.bin");

	while(1)
	{
		i=rand()%65536;
		j=rand()%3;

	    resx1=rand()%700;
        resy1=rand()%500;
		resx2=rand()%700;
        resy2=rand()%500;
		resx3=rand()%700;
        resy3=rand()%500;
		
        Foreground_color_65k(i);
        Triangle_Point1_XY(resx1,resy1);
		Triangle_Point2_XY(resx2,resy2);
		Triangle_Point3_XY(resx3,resy3);
	
	    if(j)
		{
		 printf("%d",j);
		 Start_Triangle();
		}		  
		else
		{
		 printf("%d",j);
		 Start_Triangle_Fill();
		}

		//delay_ms(1);			
		//while(1);
 	 }
}



void Draw_triangle_test_24bpp(void)
{
unsigned long i;
unsigned short resx1,resy1,resx2,resy2,resx3,resy3;
unsigned char j;


	Main_Image_Start_Address(0);				
	Main_Image_Width(8188);							
	Main_Window_Start_XY(0,0);	
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);


Select_Main_Window_24bpp();
RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);
 	
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 
		
        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	   /*
	   Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 for(i=0; i<800*600;i++)
	  {
	   LCD_DataWrite(0x00e0);
	   		  
		  //Check_Mem_WR_FIFO_not_Full();//
	   LCD_DataWrite(0x00e0);
	   		   
		  //Check_Mem_WR_FIFO_not_Full();//
	  }	*/

		//一直寫會閃屏
		/*
	  	 resx1=rand()%800;
        resy1=rand()%600;
		resx2=rand()%800;
        resy2=rand()%600;
		resx3=rand()%800;
        resy3=rand()%600;
	  while(1)
	  {	
	    i=rand()%16777216;
	    Foreground_color_16M(i);
	    Triangle_Point1_XY(resx1,resy1);
		Triangle_Point2_XY(resx2,resy2);
		Triangle_Point3_XY(resx3,resy3);
        Start_Triangle_Fill();
	  }	 */

	while(1)
	{
		i=rand()%16777216;
		j=rand()%3;

	    resx1=rand()%700;
        resy1=rand()%500;
		resx2=rand()%700;
        resy2=rand()%500;
		resx3=rand()%700;
        resy3=rand()%500;
		
        Foreground_color_16M(i);
        Triangle_Point1_XY(resx1,resy1);
		Triangle_Point2_XY(resx2,resy2);
		Triangle_Point3_XY(resx3,resy3);
	
	    if(j)
		{
		 printf("%d",j);
		 Start_Triangle();
		}		  
		else
		{
		 printf("%d",j);
		 Start_Triangle_Fill();
		}

		//delay_ms(50);			
		//while(1);
 	 }
}



void Draw_circle_test_8bpp(void)
{
unsigned long i,j;
unsigned long resx1,resy1,resx2,resy2;

	
	Main_Image_Start_Address(0);				
	Main_Image_Width(1600);							
	Main_Window_Start_XY(0,0);	
	Canvas_Image_Start_address(0);
	Canvas_image_width(1600);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);
	
Select_Main_Window_8bpp();

RGB_16b_8bpp();
Memory_8bpp_Mode();
 /*
 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();
  */
  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


        Active_Window_WH(800,600);
	    Goto_Pixel_XY(0,0);
	    Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");


	    //Circle_Center_XY(225,433);
		//Circle_Radius_R(480);
		//Start_Circle_or_Ellipse_Fill();
        //while(1);

	while(1)
	{
		i=rand()%256;
        Foreground_color_256(i);

		resx1=rand()%600;
        resy1=rand()%400;
		i=rand()%100;

		Circle_Center_XY(resx1,resy1);
		Circle_Radius_R(i);

         printf("x=%d",resx1);
		 printf(" ");
		 printf("y=%d",resy1);
		 printf(" ");
         printf("i=%d",i);
		 printf(" ");

		j=rand()%3;
	    if(j)
	    Start_Circle_or_Ellipse();
		else
		Start_Circle_or_Ellipse_Fill();

		//delay_ms(500);			
		//while(1);
 	 }
}



void Draw_circle_test_16bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;


	Main_Image_Start_Address(0);				
	Main_Image_Width(8188);							
	Main_Window_Start_XY(0,0);	
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);


Select_Main_Window_16bpp();

RGB_16b_16bpp();
Memory_16bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_65k_bin(800,600,"p2.bin");

		/*
		Circle_Center_XY(478,178);
		//Circle_Radius_R(541);
		Circle_Radius_R(479);
		Start_Circle_or_Ellipse();
		//Start_Circle_or_Ellipse_Fill();
		while(1);
		*/
	while(1)
	{
		i=rand()%65536;
        Foreground_color_65k(i);

		resx1=rand()%600;
        resy1=rand()%400;
		i=rand()%100;

		Circle_Center_XY(resx1,resy1);
		Circle_Radius_R(i);


		 printf("x=%d",resx1);
		 printf(" ");

		 printf("y=%d",resy1);
		 printf(" ");

         printf("i=%d",i);
		 printf(" ");

		j=rand()%3;
	    if(j)
	    Start_Circle_or_Ellipse();
		else
		Start_Circle_or_Ellipse_Fill();

		//delay_ms(1000);			
		//while(1);
 	 }
}


void Draw_circle_test_24bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;

Select_Main_Window_24bpp();

RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_16M_mode1_bin(800,600,"p3.bin");

		//以下條件會錯誤
		
		//Circle_Center_XY(478,178);
		//Circle_Radius_R(541);
		//Circle_Radius_R(479);
		//Start_Circle_or_Ellipse();
		//Start_Circle_or_Ellipse_Fill();
		//while(1); 
	while(1)
	{
		i=rand()%16777216;
        Foreground_color_16M(i);

		resx1=rand()%600;
        resy1=rand()%400;
		i=rand()%100;

		Circle_Center_XY(resx1,resy1);
		Circle_Radius_R(i);


		 printf("x=%d",resx1);
		 printf(" ");

		 printf("y=%d",resy1);
		 printf(" ");

         printf("i=%d",i);
		 printf(" ");

		j=rand()%3;
	    if(j)
	    Start_Circle_or_Ellipse();
		else
		Start_Circle_or_Ellipse_Fill();

		//delay_ms(500);			
		//while(1);
 	 }
}



void Draw_Ellipse_test_8bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;


	Main_Image_Start_Address(0);				
	Main_Image_Width(8188);							
	Main_Window_Start_XY(0,0);	
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);


Select_Main_Window_8bpp();

RGB_16b_8bpp();
Memory_8bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");

	while(1)
	{
		i=rand()%256;
        Foreground_color_256(i);

		resx1=rand()%600;
        resy1=rand()%400;

		Circle_Center_XY(resx1,resy1);

		i=rand()%100;
		j=rand()%100;
		/*
		if(i<4)
		i=3;

		if(j<4)
		j=3;
		*/
		Ellipse_Radius_RxRy(i,j);

        // printf("i=%d",i);
		// printf(" ");

		// printf("i=%d",j);
		// printf(" ");

		j=rand()%3;
	    if(j)
	    Start_Circle_or_Ellipse();
		else
		Start_Circle_or_Ellipse_Fill();

		//delay_ms(500);			
		//while(1);
 	 }
}


void Draw_Ellipse_test_16bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;

Select_Main_Window_16bpp();

RGB_16b_16bpp();
Memory_16bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_65k_bin(800,600,"p2.bin");

	while(1)
	{
		i=rand()%65536;
        Foreground_color_65k(i);

		resx1=rand()%600;
        resy1=rand()%400;

		Circle_Center_XY(resx1,resy1);

		i=rand()%100;
		j=rand()%100;

		Ellipse_Radius_RxRy(i,j);

         printf("i=%d",i);
		 printf(" ");

		 printf("i=%d",j);
		 printf(" ");

		j=rand()%3;
	    if(j)
	    Start_Circle_or_Ellipse();
		else
		Start_Circle_or_Ellipse_Fill();

		//delay_ms(500);			
		//while(1);
 	 }
}


void Draw_Ellipse_test_24bpp(void)
{
unsigned long i,j;

unsigned long resx1,resy1,resx2,resy2;

Select_Main_Window_24bpp();

RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

 Graphic_Mode();
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	 Graphic_cursor_initial(); //initaial and enable graphic cursor 
	 Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	while(1)
	{
		i=rand()%16777216;
        Foreground_color_16M(i);

		resx1=rand()%600;
        resy1=rand()%400;

		Circle_Center_XY(resx1,resy1);

		i=rand()%100;
		j=rand()%100;

		Ellipse_Radius_RxRy(i,j);

         printf("i=%d",i);
		 printf(" ");

		 printf("i=%d",j);
		 printf(" ");

		j=rand()%3;
	    if(j)
	    Start_Circle_or_Ellipse();
		else
		Start_Circle_or_Ellipse_Fill();

		//delay_ms(500);			
		//while(1);
 	 }
}


//==============================================================================
void DMA_Init_pattern_8bpp_8x8(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

  	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);
          
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 
  RGB_16b_8bpp();
  //RGB_16b_16bpp();
  //RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();

  Memory_8bpp_Mode();
  //Memory_16bpp_Mode();
  //Memory_24bpp_Mode();
       
  Select_Main_Window_8bpp();
  //Select_Main_Window_16bpp();
  //Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();
  //Select_SFI_32bit_Address();

  //Select_SFI_Waveform_Mode_0();
  Select_SFI_Waveform_Mode_3();

  Select_SFI_0_DummyRead();	//normal read mode
  //Select_SFI_8_DummyRead(); //1byte dummy cycle
  //Select_SFI_16_DummyRead();
  //Select_SFI_24_DummyRead();

  Select_SFI_Single_Mode();
  //Select_SFI_Dual_Mode0();
  //Select_SFI_Dual_Mode1();

  SPI_Clock_Period(2);
 
    Graphic_Mode();
	//Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	SFI_DMA_Destination_Start_Address(480000);
    SFI_DMA_Transfer_Number(64);//for linear mode
	SFI_DMA_Source_Start_Address(12441960);//24bit address

	Goto_Linear_Addr(480000);

	//Start_SFI_DMA();
    //Check_Busy_SFI_DMA(); 
					 
	for(j=0; j<2000;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(480000+j*64);
	     Start_SFI_DMA();
		 Check_Busy_SFI_DMA(); 
		 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode    
	Pattern_Format_8X8();

while(1){

 for(j=0;j<20;j++)
 {
  for(i=0;i<100;i++)
  { 
    BTE_S0_Color_8bpp();
    BTE_S0_Memory_Start_Address(480000+i*64);
    BTE_S0_Image_Width(8);
 
    BTE_S1_Color_8bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(2048);

    BTE_Destination_Color_8bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);
    BTE_Operation_Code(0x06);
          
    BTE_Destination_Window_Start_XY(i*8,j*8);
    BTE_Window_Size(8,8);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }


     }


  while(1);
}


void DMA_Init_pattern_8bpp_16x16(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 

  RGB_16b_8bpp();
  //RGB_16b_16bpp();
  //RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();

 
  Memory_8bpp_Mode();
  //Memory_16bpp_Mode();
  //Memory_24bpp_Mode();

          
  Select_Main_Window_8bpp();
  //Select_Main_Window_16bpp();
  //Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);

  	//資料寫入graphic cursor
  Memory_Select_Graphic_Cursor_RAM(); 
  Graphic_Mode();

    Select_Graphic_Cursor_1();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_pen_il[i]);
      
    }

    Select_Graphic_Cursor_2();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_arrow_il[i]);
      
    }

     Select_Graphic_Cursor_3();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_busy_im[i]);
       
     }

     Select_Graphic_Cursor_4();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_no_im[i]);
       
     }

    Memory_Select_SDRAM();//寫完後切回SDRAM


    Set_Graphic_Cursor_Color_1(0xff);
    Set_Graphic_Cursor_Color_2(0x00);
    Graphic_Cursor_XY(100,100);
    Enable_Graphic_Cursor();



 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(2);



	 
     Graphic_Mode();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關


	SFI_DMA_Destination_Start_Address(480000);
    SFI_DMA_Transfer_Number(256);//for linear mode
	SFI_DMA_Source_Start_Address(12442024);//24bit address

	Goto_Linear_Addr(480000);

	//Start_SFI_DMA();
    //Check_Busy_SFI_DMA(); 
					 
	for(j=0; j<1000;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(480000+j*256);
	     Start_SFI_DMA();
		 Check_Busy_SFI_DMA(); 
		 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode   
	   
	Pattern_Format_16X16();

while(1){

 for(j=0;j<20;j++)
 {
  for(i=0;i<50;i++)
  { 
    BTE_S0_Color_8bpp();
    BTE_S0_Memory_Start_Address(480000+i*256);
    BTE_S0_Image_Width(16);
 
    BTE_S1_Color_8bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_8bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);
    BTE_Operation_Code(0x06);
          
    BTE_Destination_Window_Start_XY(i*16,j*16);
    BTE_Window_Size(16,16);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }

 delay_ms(500);
     }


  while(1);
}


void DMA_Init_pattern_16bpp_8x8(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

 	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 

  //RGB_16b_8bpp();
  RGB_16b_16bpp();
  //RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();

 
  //Memory_8bpp_Mode();
  Memory_16bpp_Mode();
  //Memory_24bpp_Mode();

          
  //Select_Main_Window_8bpp();
  Select_Main_Window_16bpp();
  //Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);

 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(2);



	 
     Graphic_Mode();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關


	SFI_DMA_Destination_Start_Address(960000);
    SFI_DMA_Transfer_Number(128);//for linear mode
	SFI_DMA_Source_Start_Address(12442280);//24bit address

	Goto_Linear_Addr(960000);

	//Start_SFI_DMA();
    //Check_Busy_SFI_DMA(); 
					 
	for(j=0; j<2000;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(960000+j*128);
	     Start_SFI_DMA();
		 Check_Busy_SFI_DMA(); 
		 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode    
	Pattern_Format_8X8();

while(1){

 for(j=0;j<20;j++)
 {
  for(i=0;i<100;i++)
  { 
    BTE_S0_Color_16bpp();
    BTE_S0_Memory_Start_Address(960000+i*128);
    BTE_S0_Image_Width(8);
 
    BTE_S1_Color_16bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(2048);

    BTE_Destination_Color_16bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);
    BTE_Operation_Code(0x06);
          
    BTE_Destination_Window_Start_XY(i*8,j*8);
    BTE_Window_Size(8,8);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }


     }


  while(1);
}


void DMA_Init_pattern_16bpp_16x16(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 

  //RGB_16b_8bpp();
  RGB_16b_16bpp();
  //RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();

 
 // Memory_8bpp_Mode();
  Memory_16bpp_Mode();
  //Memory_24bpp_Mode();

          
  //elect_Main_Window_8bpp();
  Select_Main_Window_16bpp();
  //Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);

 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(2);



	 
     Graphic_Mode();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關


	SFI_DMA_Destination_Start_Address(960000);
    SFI_DMA_Transfer_Number(512);//for linear mode
	SFI_DMA_Source_Start_Address(12442408);//24bit address

	Goto_Linear_Addr(960000);

	//Start_SFI_DMA();
    //Check_Busy_SFI_DMA(); 
					 
	for(j=0; j<1000;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(960000+j*512);
	     Start_SFI_DMA();
		 Check_Busy_SFI_DMA(); 
		 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode   
	   
	Pattern_Format_16X16();

while(1){

 for(j=0;j<20;j++)
 {
  for(i=0;i<50;i++)
  { 
    BTE_S0_Color_16bpp();
    BTE_S0_Memory_Start_Address(960000+i*512);
    BTE_S0_Image_Width(16);
 
    BTE_S1_Color_16bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(2048);

    BTE_Destination_Color_16bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);
    BTE_Operation_Code(0x06);
          
    BTE_Destination_Window_Start_XY(i*16,j*16);
    BTE_Window_Size(16,16);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }


     }


  while(1);
}



void DMA_Init_pattern_24bpp_8x8(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 


  RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();
  Memory_24bpp_Mode();
  Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);

	tempa = color65k_red;

  	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		}

	 //Active_Window_WH(800,600);
     //Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 


	  Canvas_Image_Start_address(800*600*3);

	tempa = color65k_red;

  	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		}

	  Canvas_Image_Start_address(0);

Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(2);

  RGB_16b_24bpp_mode1();
  Memory_24bpp_Mode();
  	 
     Graphic_Mode();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關



	SFI_DMA_Destination_Start_Address(800*600*3);
    SFI_DMA_Transfer_Number(8*8*3);//for linear mode
	SFI_DMA_Source_Start_Address(12442920);//24bit address

	Goto_Linear_Addr(800*600*3);
	  //Main_Image_Start_Address(800*600*3);
	//Start_SFI_DMA();
    //Check_Busy_SFI_DMA(); 
					 
	for(j=0; j<1000;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address((800*600*3)+(j*192));
	     Start_SFI_DMA();
		 Check_Busy_SFI_DMA(); 	
		 delay_ms(10);	 	
	   }	 

	Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode    
	Pattern_Format_8X8();


  

while(1){

 for(j=0;j<10;j++)
 {
  for(i=0;i<100;i++)
  { 
    BTE_S0_Color_24bpp();
    BTE_S0_Memory_Start_Address((800*600*3)+i*192);
    BTE_S0_Image_Width(8);
 
    BTE_S1_Color_24bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_24bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);
    BTE_Operation_Code(0x06);
          
    BTE_Destination_Window_Start_XY(i*8,j*8);
    BTE_Window_Size(8,8);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }


     }


  while(1);
}


void DMA_Init_pattern_24bpp_16x16(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);

  RGB_16b_24bpp_mode1();
  Memory_24bpp_Mode();
  Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);

   	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		}

	 //Active_Window_WH(800,600);
     //Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 


 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(2);

     Graphic_Mode();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	SFI_DMA_Destination_Start_Address(800*600*3);
    SFI_DMA_Transfer_Number(768);//for linear mode
	SFI_DMA_Source_Start_Address(12443112);//24bit address

	Goto_Linear_Addr(800*600*3);
					 
	for(j=0; j<1000;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address((800*600*3)+j*768);
	     Start_SFI_DMA();
		 Check_Busy_SFI_DMA(); 
		 	
	   }	 

	Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode   	   
	Pattern_Format_16X16();

while(1){

 for(j=0;j<20;j++)
 {
  for(i=0;i<50;i++)
  { 
    BTE_S0_Color_24bpp();
    BTE_S0_Memory_Start_Address((800*600*3)+i*768);
    BTE_S0_Image_Width(16);
 
    BTE_S1_Color_24bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_24bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);
    BTE_Operation_Code(0x06);
          
    BTE_Destination_Window_Start_XY(i*16,j*16);
    BTE_Window_Size(16,16);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }


     }


  while(1);
}





void BTE_pattern_Fill_test_8bpp_8x8(unsigned char t)
{
 short i,j;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 

  RGB_16b_8bpp();
  //RGB_16b_16bpp();
  //RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();

  Memory_8bpp_Mode();
  //Memory_16bpp_Mode();
  //Memory_24bpp_Mode();
       
  Select_Main_Window_8bpp();
  //Select_Main_Window_16bpp();
  //Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);
   
  	//資料寫入graphic cursor
  Memory_Select_Graphic_Cursor_RAM(); 
  Graphic_Mode();

    Select_Graphic_Cursor_1();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_pen_il[i]);
      
    }

    Select_Graphic_Cursor_2();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_arrow_il[i]);
      
    }

     Select_Graphic_Cursor_3();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_busy_im[i]);
       
     }

     Select_Graphic_Cursor_4();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_no_im[i]);
       
     }

    Memory_Select_SDRAM();//寫完後切回SDRAM


    Set_Graphic_Cursor_Color_1(0xff);
    Set_Graphic_Cursor_Color_2(0x00);
    Graphic_Cursor_XY(100,100);
    Enable_Graphic_Cursor();




  LCD_CmdWrite(0x04);
  sd_showpic_256_bin(800,600,"p1.bin");

  Canvas_Image_Start_address(480000);
  Canvas_image_width(8);
  Active_Window_XY(0,0);
  Active_Window_WH(8,8);
  Goto_Pixel_XY(0,0);

  for(i=0;i<5000;i++)
  {
   Canvas_Image_Start_address(480000+i*64);
   Pattern_Format_8X8();
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(8,8,"p8_256.bin");
  }

while(1){

 for(j=0;j<50;j++)
 {
  for(i=0;i<100;i++)
  { 
    BTE_S0_Color_8bpp();
    BTE_S0_Memory_Start_Address(480000+i*64);
    BTE_S0_Image_Width(8);
 
    BTE_S1_Color_8bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_8bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);


    BTE_ROP_Code(0xc);

	if(t==0)
    {BTE_Operation_Code(0x06);}
    else
	{
	BTE_Operation_Code(0x07);	
    } 

	BTE_S1_Window_Start_XY(i*8,j*8); 
    BTE_Destination_Window_Start_XY(i*8,j*8);
    BTE_Window_Size(8,8);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }


     }


  while(1);

}


void BTE_pattern_Fill_test_8bpp_16x16(unsigned char t)
{
 short i,j;

 	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 

  RGB_16b_8bpp();
  //RGB_16b_16bpp();
  //RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();

 
  Memory_8bpp_Mode();
  //Memory_16bpp_Mode();
  //Memory_24bpp_Mode();

          
  Select_Main_Window_8bpp();
  //Select_Main_Window_16bpp();
  //Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);



  LCD_CmdWrite(0x04);
  sd_showpic_256_bin(800,600,"p1.bin");

  Canvas_Image_Start_address(480000);
  Canvas_image_width(16);
  Active_Window_XY(0,0);
  Active_Window_WH(16,16);
  Goto_Pixel_XY(0,0);

  for(i=0;i<1000;i++)
  {
   Canvas_Image_Start_address(480000+i*256);
   Pattern_Format_16X16();
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(16,16,"p16_256.bin");
  }

while(1){

 for(j=0;j<20;j++)
 {
  for(i=0;i<50;i++)
  { 
    BTE_S0_Color_8bpp();
    BTE_S0_Memory_Start_Address(480000+i*256);
    BTE_S0_Image_Width(16);
 
    BTE_S1_Color_8bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_8bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);

    if(t==0)
    {BTE_Operation_Code(0x06);}
    else
	{
	BTE_Operation_Code(0x07);	
    } 
	
	BTE_S1_Window_Start_XY(i*16,j*16);       
    BTE_Destination_Window_Start_XY(i*16,j*16);
    BTE_Window_Size(16,16);

    BTE_Enable();   
    Check_BTE_Busy();
	//delay_ms(100);
    }
  }

	 delay_ms(1000);
     }

  while(1);

}




void BTE_pattern_Fill_test_16bpp_8x8(unsigned char t)
{
 short i,j;

  	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 

  //RGB_16b_8bpp();
  RGB_16b_16bpp();
  //RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();

 
  //Memory_8bpp_Mode();
  Memory_16bpp_Mode();
  //Memory_24bpp_Mode();

          
  //Select_Main_Window_8bpp();
  Select_Main_Window_16bpp();
  //Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);


   LCD_CmdWrite(0x04); //
   sd_showpic_65k_bin(800,600,"p2.bin"); //

  Canvas_Image_Start_address(960000);
  Canvas_image_width(8);
  Active_Window_XY(0,0);
  Active_Window_WH(8,8);
  Goto_Pixel_XY(0,0);

  for(i=0;i<5000;i++)
  {
   Canvas_Image_Start_address(960000+i*128);
   Pattern_Format_8X8();
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(8,8,"p8_65k.bin");
  }

while(1){

 for(j=0;j<50;j++)
 {
  for(i=0;i<100;i++)
  { 
    BTE_S0_Color_16bpp();
    BTE_S0_Memory_Start_Address(960000+i*128);
    BTE_S0_Image_Width(8);
 
    BTE_S1_Color_16bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_16bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);

	if(t==0)
    BTE_Operation_Code(0x06);
	else
	BTE_Operation_Code(0x07);
    
	BTE_S1_Window_Start_XY(i*8,j*8);      
    BTE_Destination_Window_Start_XY(i*8,j*8);
    BTE_Window_Size(8,8);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }


     }


}


void BTE_pattern_Fill_test_16bpp_16x16(unsigned char t)
{
 short i,j;

  	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);

           
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 

  //RGB_16b_8bpp();
  RGB_16b_16bpp();
  //RGB_16b_24bpp_mode1();
  //RGB_16b_24bpp_mode2();

  //Memory_8bpp_Mode();
  Memory_16bpp_Mode();
  //Memory_24bpp_Mode();

          
  //Select_Main_Window_8bpp();
  Select_Main_Window_16bpp();
  //Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);

   LCD_CmdWrite(0x04); //
   sd_showpic_65k_bin(800,600,"p2.bin"); //

  Canvas_Image_Start_address(960000);
  Canvas_image_width(16);
  Active_Window_XY(0,0);
  Active_Window_WH(16,16);
  Goto_Pixel_XY(0,0);

  for(i=0;i<1000;i++)
  {
   Canvas_Image_Start_address(960000+i*512);
   Pattern_Format_16X16();
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(16,16,"p16_65k.bin");
  }

while(1){

 for(j=0;j<20;j++)
 {
  for(i=0;i<50;i++)
  { 
    BTE_S0_Color_16bpp();
    BTE_S0_Memory_Start_Address(960000+i*512);
    BTE_S0_Image_Width(16);
 
    BTE_S1_Color_16bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_16bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);

	if(t==0)
    BTE_Operation_Code(0x06);
    else
	BTE_Operation_Code(0x07);//
	
	BTE_S1_Window_Start_XY(i*16,j*16);      
    BTE_Destination_Window_Start_XY(i*16,j*16);
    BTE_Window_Size(16,16);

    BTE_Enable();   
    Check_BTE_Busy();
	//delay_ms(100);
    }
  }

	 //delay_ms(1000);
     }




}

//===========================================================================================

/*REG[90h]*/
//S0 = MCU  S1=Desti 
void BTE_write_with_ROP_8bpp_test_Destination(void)
{
   unsigned int i;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_256(color256_white);
  Background_color_256(color256_black);


 	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 





	Enable_DMA_Draw_BTE_Interrupt();
  //Disable_DMA_Draw_BTE_Interrupt();


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_8bpp();

   BTE_S1_Color_8bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_8bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);


  /*
  //test BTE INT

     //清除回原圖
    Goto_Pixel_XY(0,0);
    LCD_CmdWrite(0x04);
    sd_showpic_256_bin(800,600,"p1.bin");
  while(1)
  {

    //write with ROP 0 
    BTE_ROP_Code(0);
    BTE_Operation_Code(0); //BTE write
    BTE_Enable();
    LCD_CmdWrite(0x04);
    sd_BTE_showpic_256_bin(800,600,"p1.bin");
    //Check_Mem_WR_FIFO_not_Full();
    Check_Mem_WR_FIFO_Empty();//寫完後檢查
    Check_BTE_Busy();


    while((Read_Interrupt_status()&0x04)==0x00);

  	  if((Read_Interrupt_status()&0x04)==0x04)//read 0ch bit4 如果為1表示外部中斷
	 {
	   GPIO_SetBits(GPIOE, GPIO_Pin_6); //檢測到外部中斷後 提供一個外部信號參考

	   Clear_DMA_Draw_BTE_Interrupt_Flag();
	   delay_us(1);
	   GPIO_ResetBits(GPIOE, GPIO_Pin_6);
	 }

   }
   */

while(1)
{
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 0");
   delay_seconds(1);
	//====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 1");
   delay_seconds(1);
   //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 2");
   delay_seconds(1);
	//====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 3");
   delay_seconds(1);

   //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 4");
   delay_seconds(1);

   //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 5");
   delay_seconds(1);

   //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 6");
   delay_seconds(1);

   //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 7");
   delay_seconds(1);

   //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 8");
   delay_seconds(1);

   //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 9");
   delay_seconds(1);

   //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 10");
   delay_seconds(1);

  //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 11");
   delay_seconds(1);

  //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 12");
   delay_seconds(1);

  //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 13");
   delay_seconds(1);

  //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 14");
   delay_seconds(1);

  //====================================
   //清除回原圖
    Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //write with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 15");
   delay_seconds(1);


 }

  
}


/*REG[90h]*/
//S0 = MCU  S1=Desti 
void BTE_write_with_ROP_8bpp_test_with_S1_to_Destination(void)
{
	   unsigned int i;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_8bpp();
   
   BTE_S1_Color_8bpp();
   BTE_S1_Memory_Start_Address(800*600);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_8bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

   //寫一張底圖在S1
   Canvas_Image_Start_address(800*600);//寫入位置切換到另一張S1畫布
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   Canvas_Image_Start_address(0);//寫入位置Destination畫布
while(1)
{   
   //write with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 0");
   //delay_seconds(5);
	//====================================

   //write with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 1");
   //delay_seconds(5);
   //====================================

   //write with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 2");
   //delay_seconds(5);
	//====================================

   //write with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 3");
  // delay_seconds(5);

   //====================================

   //write with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 4");
   //delay_seconds(5);

   //====================================


   //write with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 5");
   //delay_seconds(5);

   //====================================


   //write with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 6");
   //delay_seconds(5);

   //====================================


   //write with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 7");
   //delay_seconds(5);

   //====================================


   //write with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 8");
   //delay_seconds(5);

   //====================================


   //write with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 9");
   //delay_seconds(5);

   //====================================


   //write with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 10");
   //delay_seconds(5);

  //====================================


   //write with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 11");
   //delay_seconds(5);

  //====================================


   //write with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 12");
   //delay_seconds(5);

  //====================================
 

   //write with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 13");
   //delay_seconds(5);

  //====================================

   //write with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 14");
   //delay_seconds(5);

  //====================================


   //write with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_256_bin(800,600,"p1.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 15");
   //delay_seconds(5);


 }

  
}




void BTE_Move_with_ROP_8bpp_test_positive_move(void)
{
     unsigned int i;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

 
  Foreground_color_256(color256_white);
  Background_color_256(color256_black);


 	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 



   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 
   Active_Window_WH(800,600);

   

   BTE_S0_Color_8bpp();
   BTE_S0_Memory_Start_Address(800*600);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);

   BTE_S1_Color_8bpp();
   BTE_S1_Memory_Start_Address(800*600*2);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_8bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);


   //寫一張底圖在S0
   Canvas_Image_Start_address(800*600);//
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");

   //寫一張底圖在S1
   Canvas_Image_Start_address(800*600*2);
   LCD_CmdWrite(0x04);
   sd_showpic_256_bin(800,600,"p1.bin");   

   Canvas_Image_Start_address(0);//
while(1)
{

   
   //move with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 0");
   delay_seconds(1);
	//====================================

   //move with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 1");
   delay_seconds(1);
   //====================================

   //move with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 2");
   delay_seconds(1);
	//====================================

   //move with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 3");
   delay_seconds(1);

   //====================================

   //move with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 4");
   delay_seconds(1);

   //====================================


   //move with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 5");
   delay_seconds(1);

   //====================================


   //move with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 6");
   delay_seconds(1);

   //====================================


   //move with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 7");
   delay_seconds(1);

   //====================================


   //move with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 8");
   delay_seconds(1);

   //====================================


   //move with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 9");
   delay_seconds(1);

   //====================================


   //move with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 10");
   delay_seconds(1);

  //====================================


   //move with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 11");
   delay_seconds(1);

  //====================================


   //move with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 12");
   delay_seconds(1);

  //====================================
 

   //move with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 13");
  delay_seconds(1);

  //====================================

   //move with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 14");
   delay_seconds(1);

  //====================================


   //move with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 15");
   delay_seconds(1);


 }

  
}



void BTE_Transparent_Write_test_8bpp(void)
{
 unsigned int i,j,k;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_256(color256_white);
  Background_color_256(color256_black);


	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 
   Active_Window_WH(800,600);

  
   BTE_S1_Color_8bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_8bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

   while(1)
   {

    BTE_Destination_Window_Start_XY(0,0); 
    BTE_Window_Size(800,600);
    //清除回原圖
    LCD_CmdWrite(0x04);
    sd_showpic_256_bin(800,600,"p1.bin");
	
	while(1)
	{
	/*
    BTE_Destination_Window_Start_XY(0,0); 
    BTE_Window_Size(800,600);
    //清除回原圖
    LCD_CmdWrite(0x04);
    sd_showpic_256_bin(800,600,"p1.bin");
	*/
	

	Background_color_256(0x1c);	//設定transparent key color


    for(j=0;j<(600/128);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
	 BTE_S1_Window_Start_XY(i*128,j*128);
	 BTE_Destination_Window_Start_XY(i*128,j*128); 
     BTE_Window_Size(128,128);
     BTE_ROP_Code(0);   //transparent write 與ROP cod無關
     BTE_Operation_Code(4); //BTE Transparent write
     BTE_Enable();
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(128,128,"p4_256.bin");
     //Check_Mem_WR_FIFO_not_Full();
     Check_Mem_WR_FIFO_Empty();//寫完後檢查
     Check_BTE_Busy();
	   
	   }
	  }
	  delay_seconds(1);
	 }
	  	  delay_seconds(5);
	  

	   	 //測試所有關鍵色
	  for(i=0;i<256;i++)
	  {
	   Background_color_256(i);	//設定transparent key color
	   BTE_S1_Window_Start_XY(99,99);
	   BTE_Destination_Window_Start_XY(99,99); 
       BTE_Window_Size(50,50);
       BTE_ROP_Code(0);   //transparent write 與ROP cod無關
       BTE_Operation_Code(4); //BTE Transparent write
       BTE_Enable();
       LCD_CmdWrite(0x04);
	    for(j=0;j<50*50;j++)
	    {
	    LCD_DataWrite(i);	//write 16 bits data to lcd 
	    // 
	    Check_Mem_WR_FIFO_not_Full();//BTE 寫入時用
	    }
	   Check_Mem_WR_FIFO_Empty();//寫完後檢查
       Check_BTE_Busy();

	   //清除回原圖
		   Background_color_256(color256_black);
	       Text_Mode();			  //
		   Goto_Text_XY(200,200);
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(i);	 //顯示目前KEY COLOR 
		   delay_ms(10);
           Graphic_Mode(); //back to graphic mode
		   delay_ms(100);
		   //delay_seconds(1);
	    }

    }

 }


//後續將修改成與S1無關20120618
void BTE_Transparent_Move_test_8bpp(void)
{
 unsigned int i,j,k;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_8bpp();
   BTE_S0_Memory_Start_Address(800*600);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);


   BTE_S1_Color_8bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_8bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

    BTE_Destination_Window_Start_XY(0,0); 
    BTE_Window_Size(800,600);
    //清除回原圖
    LCD_CmdWrite(0x04);
    sd_showpic_256_bin(800,600,"p1.bin");

	//先寫入準備Move圖檔到S0
    Canvas_Image_Start_address(800*600);//S0
   
	 for(j=0;j<(600/128);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
  	 Active_Window_XY(i*128,j*128);
     Active_Window_WH(128,128);
	  Goto_Pixel_XY(i*128,j*128);
	 LCD_CmdWrite(0x04);
     sd_showpic_256_bin(128,128,"p4_256.bin");
	 }
	 }

    Background_color_256(0x1c);	//設定transparent key color
   while(1)
   {

     for(j=0;j<(600/128);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
	 //都必須指定再同一個位置
	 BTE_S1_Window_Start_XY(i*128,j*128);	 
	 BTE_S0_Window_Start_XY(i*128,j*128);
	 BTE_Destination_Window_Start_XY(i*128,j*128); 
     BTE_Window_Size(128,128);
     BTE_ROP_Code(12);   //transparent Move S0
     BTE_Operation_Code(5); //BTE Transparent Move
     BTE_Enable();
     Check_BTE_Busy();
	 }
	 } 	  

    }

 }


//==========================================================================


void BTE_Color_Expansion_test_8bpp(unsigned char t)
{
 unsigned int i,j,k;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

 	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);
   
   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_8bpp();
   BTE_S0_Memory_Start_Address(800*600);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);


   BTE_S1_Color_8bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_8bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

    //清除回原圖
    LCD_CmdWrite(0x04);
    sd_showpic_256_bin(800,600,"p1.bin");

   while(1)
   {
	 Foreground_color_256(color256_white);
     Background_color_256(color256_black);
	 BTE_ROP_Code(7);

	 if(t==0)
     BTE_Operation_Code(8); //BTE color expansion
	 else
	 BTE_Operation_Code(9); //BTE color expansion

	 BTE_Enable();
	 delay_us(10);
	 LCD_CmdWrite(0x04);
     for(j=0;j<600;j++)
     {
	  for(i=0;i<(800/8);i++)
	  {
	   LCD_DataWrite(0x70);
	   //delay_ms(1);
	   Check_Mem_WR_FIFO_not_Full();
       }
	  }
	  Check_Mem_WR_FIFO_Empty();	  	  
	 Check_BTE_Busy();
    }

 }

//==========================================================================

 void BTE_Alpha_blending_8bpp_to_8bpp(void)
 {

  unsigned int i,j;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_white);
  Background_color_256(color256_black);


   	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 

   //256色 Palette 寫入
    Memory_Select_Color_Palette_RAM();
	LCD_CmdWrite(0x04);  
	  
	for(i=0;i<64;i++)
	{
	 temp = 0;
	 k=i;	 
	 if((k&0x01)==0x01)
	 {temp |=4;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=8;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=64;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=128;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 1024;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 2048;}

	 LCD_DataWrite(temp);
	 //LCD_DataWrite(0x00);
	 delay_ms(1);
	 LCD_DataWrite(temp>>8);
	 //LCD_DataWrite(0x0f);
	 delay_ms(1);
	 }

	//寫完切回
     Memory_Select_SDRAM();

	 Active_Window_WH(800,600);
     BTE_Destination_Color_8bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

     BTE_S0_Color_8bpp();
     BTE_S0_Memory_Start_Address(800*600);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600);
     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

   while(1)
   { 
  	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(800*600*2);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 Canvas_Image_Start_address(800*600*2);
	 Active_Window_WH(128,128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xcc);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE color expansion
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	   delay_seconds(2);

	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(800*600*2);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 Canvas_Image_Start_address(800*600*2);
	 Active_Window_WH(128,128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x8c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE color expansion
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

     delay_seconds(2);



	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(800*600*2);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 Canvas_Image_Start_address(800*600*2);
	 Active_Window_WH(128,128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x4c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE color expansion
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

     delay_seconds(2);


	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(800*600*2);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 Canvas_Image_Start_address(800*600*2);
	 Active_Window_WH(128,128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x0c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE color expansion
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

     delay_seconds(2);



    }

 }



 //==========================================================================

 void BTE_Alpha_blending_8bpp_to_16bpp(void)
 {

  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_white);
  Background_color_256(color256_black);


	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 

	 Active_Window_WH(800,600);
     BTE_Destination_Color_8bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

	 Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

     BTE_S0_Color_8bpp();
     BTE_S0_Memory_Start_Address(800*600);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600);

	 Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

   while(1)
   { 

  for(b=0;b<(600/128);b++)
   {
	for(a=0;a<(800/128);a++)
    {
	 BTE_Destination_Window_Start_XY(a*128,b*128);  
	 BTE_S0_Window_Start_XY(a*128,b*128);

	 Memory_16bpp_Mode();
  	 BTE_S1_Color_16bit_Alpha(); //當設定為Alpha 4bit  index color 為12bit  
     BTE_S1_Memory_Start_Address(800*600*2);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(a*128,b*128);

	 Canvas_Image_Start_address(800*600*2);
	 
	 Active_Window_XY(a*128,b*128);
	 Active_Window_WH(128,128);
	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x00);
	       
		  LCD_DataWrite(0xff);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		  LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE color expansion
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	 delay_seconds(1);

	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xf0);
	       
		  LCD_DataWrite(0xf0);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x11);
	       
		   LCD_DataWrite(0xE1);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	  delay_seconds(1);


	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x0f);
	       
		 LCD_DataWrite(0xf0);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x22);
	       
		  LCD_DataWrite(0x02);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	  delay_seconds(1);


	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x55);
	       
		  LCD_DataWrite(0xa5);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x11);
	       
		 LCD_DataWrite(0xA1);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	  delay_seconds(1);
 }
 }
    }

 }



//=======================================================================
void BTE_MPU_Write_Alpha_blending_8bpp_to_8bpp(void)
{
  unsigned int i,j;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

 	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 

   //256色 Palette 寫入
    Memory_Select_Color_Palette_RAM();
	LCD_CmdWrite(0x04);  
	  
	for(i=0;i<64;i++)
	{
	 temp = 0;
	 k=i;	 
	 if((k&0x01)==0x01)
	 {temp |=4;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=8;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=64;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=128;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 1024;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 2048;}

	 LCD_DataWrite(temp);
	 //LCD_DataWrite(0x00);
	 delay_ms(1);
	 LCD_DataWrite(temp>>8);
	 //LCD_DataWrite(0x0f);
	 delay_ms(1);
	 }

	//寫完切回
     Memory_Select_SDRAM();

	 Active_Window_WH(800,600);

     BTE_Destination_Color_8bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

	  Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

	 BTE_S0_Color_8bpp();					 //MPU write = S0
     //BTE_S0_Memory_Start_Address(800*600);
     //BTE_S0_Image_Width(800);
     //BTE_S0_Window_Start_XY(0,0);
     //Canvas_Image_Start_address(800*600);


	 	
	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(800*600*2);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);
	 Canvas_Image_Start_address(800*600*2);	//寫入S1位置

	 Active_Window_WH(800,600);	//OSD 寫入大小	
	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<800*600;i++)
	 {
       LCD_DataWrite(0x00);
	    
	 }

	 Active_Window_XY(0,0);
	 Active_Window_WH(128,128);	//OSD 寫入大小
	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xcc);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }


   while(1)
   { 
   	 
	 BTE_Window_Size(800,600);
     BTE_Operation_Code(11); //BTE write with alpha 
	 BTE_Enable();

     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  	  
	 
	 delay_seconds(1);

	 Active_Window_XY(128,128);
	 Goto_Pixel_XY(128,128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x8c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }
	 

	 BTE_Enable();
	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

	  Active_Window_XY(256,256);
	  Goto_Pixel_XY(256,256);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x4c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }
	  

	 BTE_Enable();
	   	  
	 
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  
     delay_seconds(1);

	   Active_Window_XY(384,384);
	  Goto_Pixel_XY(384,384);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x0c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }
	  
	 BTE_Enable();
	   	   	 
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);



    }
}



//=======================================================================
void BTE_MPU_Write_Alpha_blending_8bpp_to_16bpp(void)
{
  unsigned int i,j;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

   	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(2400);

   Main_Image_Start_Address(0);				
   Main_Image_Width(2400);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 


	 Active_Window_WH(800,600);

     BTE_Destination_Color_8bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(2400);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

	 Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

	 BTE_S0_Color_8bpp();					 //MPU write = S0
     //BTE_S0_Memory_Start_Address(800*600);
     //BTE_S0_Image_Width(800);
     //BTE_S0_Window_Start_XY(0,0);
     //Canvas_Image_Start_address(800*600);


	 Memory_16bpp_Mode();	
	 BTE_S1_Color_16bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(2400*600*2);
     BTE_S1_Image_Width(2400);
     BTE_S1_Window_Start_XY(0,0);
	 Canvas_Image_Start_address(2400*600*2);	//寫入S1位置

	 Active_Window_WH(800,600);	//OSD 寫入大小	
	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<800*600;i++)
	 {
       LCD_DataWrite(0x00);
	    
	   LCD_DataWrite(0x00);
	    
	 }

	 Active_Window_WH(128,128);	//OSD 寫入大小
	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x00);
	       
		  LCD_DataWrite(0xff);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		  LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 Memory_8bpp_Mode();

   while(1)
   { 
 
	 BTE_Window_Size(800,600);
     BTE_Operation_Code(11); //BTE write with alpha 
	 BTE_Enable();

     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  	  
	 
	 delay_seconds(1);

	 BTE_Enable();
	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);


	 //BTE_Destination_Window_Start_XY(128,128);  
	 BTE_Enable();
	   	  

     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  
     delay_seconds(1);

	 //BTE_Destination_Window_Start_XY(256,256); 
	 BTE_Enable();
	   	   	 
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

    }
}





//==================================================================
void BTE_Solid_Fill_test_8bpp(void)
{
  unsigned int i;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

   	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 
	 BTE_S0_Color_8bpp();	//與S0色深有關 ,後續會修改與destination 有關20120619

	 Active_Window_WH(800,600);
     BTE_Destination_Color_8bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

	  Graphic_Cursor_XY(50,50);

      Goto_Text_XY(100,100);


	 BTE_ROP_Code(12);
     BTE_Operation_Code(12); //BTE solid fill
 while(1)
 {

	 for(i=0;i<8;i++)
	 {
     Foreground_RGB(i<<5,0x00,0x00);
	 BTE_Destination_Window_Start_XY(i*30,0);
     BTE_Window_Size(30,30);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

	 for(i=0;i<8;i++)
	 {
     Foreground_RGB(0x00,i<<5,0x00);
	 BTE_Destination_Window_Start_XY(i*30,30);
     BTE_Window_Size(30,30);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

	 for(i=0;i<8;i++)
	 {
     Foreground_RGB(0x00,0x00,i<<5);
	 BTE_Destination_Window_Start_XY(i*30,60);
     BTE_Window_Size(30,30);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

 }
}


//=======================================================================
void BTE_move_Alpha_blending_by_picture_8bpp(void)
{
 unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_256(color256_white);
  Background_color_256(color256_black);
 
 	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(0);



   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 

	 Active_Window_WH(800,600);
     BTE_Destination_Color_8bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");


     

	 Main_Image_Start_Address(800*600);
     BTE_S0_Color_8bpp();
     BTE_S0_Memory_Start_Address(800*600);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600);
     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");
	 delay_seconds(1);

     BTE_S1_Color_8bpp();
     BTE_S1_Memory_Start_Address(800*600*2);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 Main_Image_Start_Address(800*600*2);	
     Canvas_Image_Start_address(800*600*2);


	 LCD_CmdWrite(0x04);
     for(i=0;i<800*600;i++)
	 {
	  LCD_DataWrite(0x00);
	   
	 }
	 delay_seconds(1);

	 Main_Image_Start_Address(0);
  	 /*
	 SFI_DMA_Destination_Upper_Left_Corner(0,0);
     SFI_DMA_Transfer_Width_Height(800,600);
     SFI_DMA_Source_Width(1920);
     SFI_DMA_Source_Start_Address(0);//24bit address

	 Start_SFI_DMA();
     Check_Busy_SFI_DMA();
	 */

  while(1)
  {

     for(i=0;i<32;i++)
	 {
	 BTE_Alpha_Blending_Effect(i);
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE move  
	 BTE_Enable();

	 delay_seconds(2);
	 Canvas_Image_Start_address(0);
	 Active_Window_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

	 delay_seconds(1);
	 }
	 /*
	 BTE_Alpha_Blending_Effect(0x1F);
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE move  
	 BTE_Enable();

	 delay_seconds(2);

	 Canvas_Image_Start_address(0);
	 Active_Window_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");

	 delay_seconds(1);*/
   	 }
}


//=======================================================================
void BTE_write_Alpha_blending_by_picture_8bpp(void)
{
 unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_white);
  Background_color_256(color256_black);
 
  	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


  Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();
  //Select_SFI_32bit_Address();

  //Select_SFI_Waveform_Mode_0();
  Select_SFI_Waveform_Mode_3();

  //Select_SFI_0_DummyRead();	//normal read mode
  Select_SFI_8_DummyRead(); //1byte dummy cycle
  //Select_SFI_16_DummyRead();
  //Select_SFI_24_DummyRead();

  //Select_SFI_Single_Mode();
  Select_SFI_Dual_Mode0();
  //Select_SFI_Dual_Mode1();
  SPI_Clock_Period(0);

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_8bpp();
   Memory_8bpp_Mode();	
   Select_Main_Window_8bpp();
 

	 Active_Window_WH(800,600);
     BTE_Destination_Color_8bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_256_bin(800,600,"p1.bin");


     BTE_S1_Color_8bpp();
     BTE_S1_Memory_Start_Address(800*600*2);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);	
     Canvas_Image_Start_address(800*600*2);

	 LCD_CmdWrite(0x04);
     for(i=0;i<800*600;i++)
	 {
	  LCD_DataWrite(0x00);
	   
	 }
	 delay_seconds(2);
	   	 /*
	 SFI_DMA_Destination_Upper_Left_Corner(0,0);
     SFI_DMA_Transfer_Width_Height(800,600);
     SFI_DMA_Source_Width(1920);
     SFI_DMA_Source_Start_Address(0);//24bit address

	 Start_SFI_DMA();
     Check_Busy_SFI_DMA();
	 */

	 //write S0 = MCU
     BTE_S0_Color_8bpp();
     BTE_S0_Memory_Start_Address(800*600);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

	 BTE_Alpha_Blending_Effect(80);
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(11); //BTE move  

  while(1)
  {
	 BTE_Enable();
	 LCD_CmdWrite(0x04);
     sd_BTE_showpic_256_bin(800,600,"p1.bin");
	 Check_Mem_WR_FIFO_Empty();//寫完後檢查
     Check_BTE_Busy();

   }
	 

}


//========================================================================
//S0 = MCU  S1=Desti 
void BTE_write_with_ROP_16bpp_test_Destination(void)
{

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_16bpp();

   BTE_S1_Color_16bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_16bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);
      
   

while(1)
{
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 0");
   delay_seconds(5);
	//====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 1");
   delay_seconds(5);
   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 2");
   delay_seconds(5);
	//====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 3");
   delay_seconds(5);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 4");
   delay_seconds(5);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 5");
   delay_seconds(5);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 6");
   delay_seconds(5);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 7");
   delay_seconds(5);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 8");
   delay_seconds(5);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 9");
   delay_seconds(5);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 10");
   delay_seconds(5);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 11");
   delay_seconds(5);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 12");
   delay_seconds(5);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 13");
   delay_seconds(5);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 14");
   delay_seconds(5);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //write with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 15");
   delay_seconds(5);

 }

  
}


//==========================================================================

//S0 = MCU  S1=Desti 
void BTE_write_with_ROP_16bpp_test_S1_to_Destination(void)
{

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_16bpp();

   BTE_S1_Color_16bpp();
   BTE_S1_Memory_Start_Address(800*600*2);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_16bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);
    
	  
   //寫入一張到S1
   Canvas_Image_Start_address(800*600*2);
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin"); 

   //切換回destination
   Canvas_Image_Start_address(0);
while(1)
{
   //write with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 0");
   //delay_seconds(1);
   //====================================
   //write with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 1");
   //delay_seconds(1);
   //====================================
   //write with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 2");
   //delay_seconds(1);
   //====================================
   //write with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 3");
   //delay_seconds(1);

   //====================================
   //write with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 4");
   //delay_seconds(1);

   //====================================
   //write with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 5");
   //delay_seconds(1);

   //====================================
   //write with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 6");
   //delay_seconds(1);

   //====================================
   //write with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 7");
   //delay_seconds(1);

   //====================================
   //write with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 8");
   //delay_seconds(1);

   //====================================
   //write with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 9");
   //delay_seconds(1);

   //====================================
   //write with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 10");
  // delay_seconds(1);

  //====================================
   //write with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 11");
   //delay_seconds(1);

  //====================================
   //write with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 12");
   //delay_seconds(1);

  //====================================
   //write with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 13");
   //delay_seconds(1);

  //====================================
   //write with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 14");
   //delay_seconds(1);

  //====================================
   //write with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_65k_bin(800,600,"p2.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 15");
   //delay_seconds(1);


 }

  
}




void BTE_Move_with_ROP_16bpp_test_positive_move(void)
{

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
   Active_Window_WH(800,600);

   

   BTE_S0_Color_16bpp();
   BTE_S0_Memory_Start_Address(800*600*2);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);

   BTE_S1_Color_16bpp();
   BTE_S1_Memory_Start_Address(800*600*2*2);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_16bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);


   //寫一張底圖在S0
   Canvas_Image_Start_address(800*600*2);//
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   //寫一張底圖在S1
   Canvas_Image_Start_address(800*600*2*2);
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");   

   Canvas_Image_Start_address(0);//
while(1)
{

   
   //move with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 0");
   //delay_seconds(1);
	//====================================

   //move with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 1");
   //delay_seconds(1);
   //====================================

   //move with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 2");
   //delay_seconds(1);
	//====================================

   //move with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 3");
   //delay_seconds(1);

   //====================================

   //move with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();

   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 4");
   //delay_seconds(1);

   //====================================


   //move with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 5");
   //delay_seconds(1);

   //====================================


   //move with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 6");
   //delay_seconds(1);

   //====================================


   //move with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 7");
   //delay_seconds(1);

   //====================================


   //move with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 8");
   //delay_seconds(1);

   //====================================


   //move with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 9");
   //delay_seconds(1);

   //====================================


   //move with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 10");
   //delay_seconds(1);

  //====================================


   //move with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 11");
   //delay_seconds(1);

  //====================================


   //move with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 12");
   //delay_seconds(1);

  //====================================
 

   //move with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 13");
  // delay_seconds(1);

  //====================================

   //move with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 14");
   //delay_seconds(1);

  //====================================


   //move with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("move with ROP 15");
   //delay_seconds(5);


 }

  
}




void BTE_Transparent_Write_test_16bpp(void)
{
 unsigned int i,j,k;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);


	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_16bpp();

   BTE_S1_Color_16bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_16bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

   while(1)
   {
    BTE_Destination_Window_Start_XY(0,0); 
    BTE_Window_Size(800,600);
    //清除回原圖
    LCD_CmdWrite(0x04);
    sd_showpic_65k_bin(800,600,"p2.bin");

	delay_seconds(1);

	Background_color_65k(0x07e0);	//設定transparent key color

 while(1)
 {	 
    for(j=0;j<(600/128);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
	 BTE_S1_Window_Start_XY(i*128,j*128);
	 BTE_Destination_Window_Start_XY(i*128,j*128); 
     BTE_Window_Size(128,128);
     BTE_ROP_Code(0);   //transparent write 與ROP cod無關
     BTE_Operation_Code(4); //BTE Transparent write
     BTE_Enable();
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(128,128,"p5_65k.bin");
     //Check_Mem_WR_FIFO_not_Full();
     Check_Mem_WR_FIFO_Empty();//寫完後檢查
     Check_BTE_Busy();

	   }
	  }
  }	 
	  	  delay_seconds(1);
	  

	   	 //測試所有關鍵色
	  for(i=0;i<65535;i++)
	  {
	   Background_color_65k(i);	//設定transparent key color
	   BTE_S1_Window_Start_XY(100,100);
	   BTE_Destination_Window_Start_XY(100,100); 
       BTE_Window_Size(50,50);
       BTE_ROP_Code(0);   //transparent write 與ROP cod無關
       BTE_Operation_Code(4); //BTE Transparent write
       BTE_Enable();
       LCD_CmdWrite(0x04);
	    for(j=0;j<50*50;j++)
	    {
	    LCD_DataWrite(i);	//write 16 bits data to lcd 
		 //LCD_DataWrite(0xf800);
	    // 
		 delay_us(3);
	    //Check_Mem_WR_FIFO_not_Full();//BTE 寫入時用
	    }
	   //Check_Mem_WR_FIFO_Empty();//寫完後檢查
       //Check_BTE_Busy();


		   Background_color_65k(color65k_black);
	       Text_Mode();			  //
		   Goto_Text_XY(200,200);
	       LCD_CmdWrite(0x04);	
		   Print_Decimal_5(i);     
	      // Print_Decimal(i);	 //顯示目前KEY COLOR 
           Graphic_Mode(); //back to graphic mode
		   //delay_seconds(1);
	    }

		while(1);

    }

 }


//後續將修改成與S1無關20120618
void BTE_Transparent_Move_test_16bpp(void)
{
 unsigned int i,j,k;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_16bpp();
   BTE_S0_Memory_Start_Address(800*600*2);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);

   BTE_S1_Color_16bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_16bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

    BTE_Destination_Window_Start_XY(0,0); 
    BTE_Window_Size(800,600);
    //清除回原圖
	Goto_Pixel_XY(0,0);
    LCD_CmdWrite(0x04);
    sd_showpic_65k_bin(800,600,"p2.bin");

	//先寫入準備Move圖檔到S0
    Canvas_Image_Start_address(800*600*2);//S0
	Active_Window_XY(100,100);
    Active_Window_WH(128,128);
	 Goto_Pixel_XY(100,100);
	LCD_CmdWrite(0x04);
    sd_showpic_65k_bin(128,128,"p5_65k.bin");




	//先寫入準備Move圖檔到S0
    Canvas_Image_Start_address(800*600*2);//S0
   
	 for(j=0;j<(600/128);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
  	 Active_Window_XY(i*128,j*128);
     Active_Window_WH(128,128);
	 Goto_Pixel_XY(i*128,j*128);
	 LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(128,128,"p5_65k.bin");
	 }
	 }

	 Background_color_65k(0x07e0);	//設定transparent key color

   while(1)
   {

     for(j=0;j<(600/128);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
	 //都必須指定再同一個位置
	 BTE_S1_Window_Start_XY(i*128,j*128);	 
	 BTE_S0_Window_Start_XY(i*128,j*128);
	 BTE_Destination_Window_Start_XY(i*128,j*128); 
     BTE_Window_Size(128,128);
     BTE_ROP_Code(12);   //transparent Move S0
     BTE_Operation_Code(5); //BTE Transparent Move
     BTE_Enable();
     Check_BTE_Busy();
	 }
	 } 	
   }

 }


//==========================================================================


void BTE_Color_Expansion_test_16bpp(unsigned char t)
{
 unsigned int i,j,k;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_16bpp();
   BTE_S0_Memory_Start_Address(800*600*2);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);


   BTE_S1_Color_16bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_16bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_65k_bin(800,600,"p2.bin");

   while(1)
   {
	 Foreground_color_65k(color65k_white);
     Background_color_65k(color65k_black);
	 BTE_ROP_Code(15);

	 if(t==0)
     BTE_Operation_Code(8); //BTE color expansion
	 else
	 BTE_Operation_Code(9); //BTE color expansion

	 BTE_Enable();
	 LCD_CmdWrite(0x04);
     for(j=0;j<600;j++)
     {
	  for(i=0;i<(800/16);i++)
	  {
	   LCD_DataWrite(0xaa00);	   
	   Check_Mem_WR_FIFO_not_Full(); //
       }
	  }	
	  Check_Mem_WR_FIFO_Empty();  	  
	  Check_BTE_Busy();

    }

 }

//======================================================================

 void BTE_Alpha_blending_16bpp_to_8bpp(void)
 {

  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 

   //65k色應用  Palette 寫入
    Memory_Select_Color_Palette_RAM();
	LCD_CmdWrite(0x04);  
	  
	for(i=0;i<64;i++)
	{
	 temp = 0;
	 k=i;	 
	 if((k&0x01)==0x01)
	 {temp |=4;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=8;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=64;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=128;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 1024;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 2048;}

	 LCD_DataWrite(temp);
	 //LCD_DataWrite(0x00);
	 delay_ms(1);

	 }

	//寫完切回
     Memory_Select_SDRAM();

	 Active_Window_WH(800,600);
     BTE_Destination_Color_16bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");

     BTE_S0_Color_16bpp();
     BTE_S0_Memory_Start_Address(800*600*2);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600*2);
     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");

   while(1)
   { 


 for(b=0;b<(600/128);b++)
   {
	for(a=0;a<(800/128);a++)
    {
	   BTE_Destination_Window_Start_XY(a*128,b*128);  
	   BTE_S0_Window_Start_XY(a*128,b*128);

  	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(800*600*4);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 Memory_8bpp_Mode();//OSD LAYER 先切換成8BPP	
	 Canvas_Image_Start_address(800*600*4);
	 Active_Window_WH(128,128);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xcc);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE Alpha
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	   delay_seconds(1);
	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x8c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

     delay_seconds(1);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x4c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();	   	  
	 Check_BTE_Busy();

     delay_seconds(1);


	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x0c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();	   	  
	 Check_BTE_Busy();

     delay_seconds(1);

 }
 }

    }

 }

//=======================================================================
//=======================================================================
void BTE_MPU_Write_Alpha_blending_16bpp_to_8bpp(void)
{
  unsigned int i,j;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);


	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 

   //65k應用 Palette 寫入
    Memory_Select_Color_Palette_RAM();
	LCD_CmdWrite(0x04);  
	  
	for(i=0;i<64;i++)
	{
	 temp = 0;
	 k=i;	 
	 if((k&0x01)==0x01)
	 {temp |=4;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=8;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=64;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=128;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 1024;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 2048;}

	 LCD_DataWrite(temp);
	 //LCD_DataWrite(0x00);
	 delay_ms(1);
	 LCD_DataWrite(temp>>8);
	 //LCD_DataWrite(0x0f);
	 delay_ms(1);
	 }


	 Canvas_Image_Start_address(800*600*2);

     Main_Image_Start_Address(800*600*2);
	//寫完切回
     Memory_Select_SDRAM();
	 Active_Window_WH(800,600);

     BTE_Destination_Color_16bpp();  
     //BTE_Destination_Memory_Start_Address(0);
	 BTE_Destination_Memory_Start_Address(800*600*2);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");




	 BTE_S0_Color_16bpp();					 //MPU write = S0
     //BTE_S0_Memory_Start_Address(800*600*2);
     //BTE_S0_Image_Width(800);
     //BTE_S0_Window_Start_XY(0,0);
     //Canvas_Image_Start_address(800*600*2);

	 	
	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     //BTE_S1_Memory_Start_Address(800*600*4);
	 BTE_S1_Memory_Start_Address(0);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 //Canvas_Image_Start_address(800*600*4);	//寫入S1位置
	  Canvas_Image_Start_address(0);	//寫入S1位置
	 Memory_8bpp_Mode();//OSD LAYER 先切換成8BPP
	 RGB_16b_8bpp();

	 Active_Window_WH(800,600);	//OSD 寫入大小	
	 LCD_CmdWrite(0x04);
	 for(i=0;i<800*600;i++)
	 {
       LCD_DataWrite(0x00);
	    
	 }
	 Active_Window_WH(128,128);	//OSD 寫入大小	

	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xcc);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 Memory_16bpp_Mode();//寫完切回16bit
	 RGB_16b_16bpp();

   while(1)
   {  	 
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(12);
     BTE_Operation_Code(11); //BTE write with alpha 
	 BTE_Enable();

     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  	  
	 
	 delay_seconds(1);

	 BTE_Enable();	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

	 BTE_Enable();	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  
     delay_seconds(1);

	 BTE_Enable();	   	   	 
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

    }
}



//=======================================================================
void BTE_MPU_Write_Alpha_blending_16bpp_to_16bpp(void)
{
  unsigned int i,j;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 

	//寫完切回
     Memory_Select_SDRAM();
	 Active_Window_WH(800,600);

     BTE_Destination_Color_16bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");

	 BTE_S0_Color_16bpp();					 //MPU write = S0
     //BTE_S0_Memory_Start_Address(800*600*2);
     //BTE_S0_Image_Width(800);
     //BTE_S0_Window_Start_XY(0,0);
     //Canvas_Image_Start_address(800*600*2);

	 	
	 BTE_S1_Color_16bit_Alpha(); //
     BTE_S1_Memory_Start_Address(800*600*4);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);
	 Canvas_Image_Start_address(800*600*4);	//寫入S1位置
	 
	 
	 Active_Window_WH(800,600);	//OSD 寫入大小	
	 LCD_CmdWrite(0x04);
	 for(i=0;i<800*600;i++)
	 {
       LCD_DataWrite(0x0000);
	    
	 }


	 Active_Window_WH(128,128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xff00);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x0000);
	       
		 }
		 temp1<<=1;
	   }
	 }


   while(1)
   {  	 
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(12);
     BTE_Operation_Code(11); //BTE write with alpha 
	 BTE_Enable();

     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  	  
	 
	 delay_seconds(1);

	 BTE_Enable();	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

	 BTE_Enable();	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  
     delay_seconds(1);

	 BTE_Enable();	   	   	 
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

    }
}

//==========================================================================

 void BTE_Alpha_blending_16bpp_to_16bpp(void)
 {

  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);


	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 

	 Active_Window_WH(800,600);
     BTE_Destination_Color_16bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");

     BTE_S0_Color_16bpp();
     BTE_S0_Memory_Start_Address(800*600*2);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600*2);
     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");

   while(1)
   { 

  for(b=0;b<(600/128);b++)
   {
	for(a=0;a<(800/128);a++)
    {
	 BTE_Destination_Window_Start_XY(a*128,b*128);  
	 BTE_S0_Window_Start_XY(a*128,b*128);

  	 BTE_S1_Color_16bit_Alpha(); //當設定為Alpha 4bit  index color 為12bit  
     BTE_S1_Memory_Start_Address(800*600*4);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(a*128,b*128);

	 Canvas_Image_Start_address(800*600*4);
	 Active_Window_XY(a*128,b*128);
	 Active_Window_WH(128,128);
	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xff00);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x0000);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE 
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	  //delay_seconds(1);

	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xf0f0);
	       
		 }
		else
		 {
	      LCD_DataWrite(0xE111);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	  //delay_seconds(1);


	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xf00f);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x0222);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	  //delay_seconds(1);


	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xa555);
	       
		 }
		else
		 {
	      LCD_DataWrite(0xA111);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	  //delay_seconds(1);
 }
 }
    }

 }


//==================================================================
void BTE_Solid_Fill_test_16bpp(void)
{
  unsigned int i;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
	 BTE_S0_Color_16bpp();	//與S0色深有關 ,後續會修改與destination 有關20120619

	 Active_Window_WH(800,600);
     BTE_Destination_Color_16bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");

	 BTE_ROP_Code(12);
     BTE_Operation_Code(12); //BTE solid fill
 while(1)
 {

	 for(i=0;i<64;i++)
	 {
     Foreground_RGB(i<<2,0x00,0x00);
	 BTE_Destination_Window_Start_XY(i*10,0);
     BTE_Window_Size(10,100);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

	 for(i=0;i<64;i++)
	 {
     Foreground_RGB(0x00,i<<2,0x00);
	 BTE_Destination_Window_Start_XY(i*10,100);
     BTE_Window_Size(10,100);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

	 for(i=0;i<64;i++)
	 {
     Foreground_RGB(0x00,0x00,i<<2);
	 BTE_Destination_Window_Start_XY(i*10,200);
     BTE_Window_Size(10,100);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

 }
}

	
void BTE_move_Alpha_blending_by_picture_16bpp(void)
{
  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_white);
  Background_color_65k(color65k_black);
 
  	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


  Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();
  //Select_SFI_32bit_Address();

  //Select_SFI_Waveform_Mode_0();
  Select_SFI_Waveform_Mode_3();

  //Select_SFI_0_DummyRead();	//normal read mode
  Select_SFI_8_DummyRead(); //1byte dummy cycle
  //Select_SFI_16_DummyRead();
  //Select_SFI_24_DummyRead();

  //Select_SFI_Single_Mode();
  Select_SFI_Dual_Mode0();
  //Select_SFI_Dual_Mode1();

  SPI_Clock_Period(0);


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
	 Active_Window_WH(800,600);
     BTE_Destination_Color_16bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");


	 Main_Image_Start_Address(800*600*2);
     BTE_S0_Color_16bpp();
     BTE_S0_Memory_Start_Address(800*600*2);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600*2);
     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");
	 delay_seconds(2);

     BTE_S1_Color_16bpp();
     BTE_S1_Memory_Start_Address(800*600*4);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 Main_Image_Start_Address(800*600*4);	
     Canvas_Image_Start_address(800*600*4);


	 LCD_CmdWrite(0x04);
     for(i=0;i<800*600;i++)
	 {
	  LCD_DataWrite(0x0000);
	  // 
	 }
	 delay_seconds(2);

	 Main_Image_Start_Address(0);
  	 /*
	 SFI_DMA_Destination_Upper_Left_Corner(0,0);
     SFI_DMA_Transfer_Width_Height(800,600);
     SFI_DMA_Source_Width(1920);
     SFI_DMA_Source_Start_Address(0);//24bit address

	 Start_SFI_DMA();
     Check_Busy_SFI_DMA();
	 */
	 BTE_Alpha_Blending_Effect(80);
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(15);
     BTE_Operation_Code(10); //BTE move  
	 BTE_Enable();

	 while(1);
}

void BTE_write_Alpha_blending_by_picture_16bpp(void)
{
  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_white);
  Background_color_65k(color65k_black);
 
  Graphic_cursor_initial(); //initaial and enable graphic cursor 


  Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();
  //Select_SFI_32bit_Address();

  //Select_SFI_Waveform_Mode_0();
  Select_SFI_Waveform_Mode_3();

  //Select_SFI_0_DummyRead();	//normal read mode
  Select_SFI_8_DummyRead(); //1byte dummy cycle
  //Select_SFI_16_DummyRead();
  //Select_SFI_24_DummyRead();

  //Select_SFI_Single_Mode();
  Select_SFI_Dual_Mode0();
  //Select_SFI_Dual_Mode1();

  SPI_Clock_Period(0);


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_16bpp();
   Memory_16bpp_Mode();	
   Select_Main_Window_16bpp();
 
	 Active_Window_WH(800,600);
     BTE_Destination_Color_16bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_65k_bin(800,600,"p2.bin");



     BTE_S1_Color_16bpp();
     BTE_S1_Memory_Start_Address(800*600*4);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);	
     Canvas_Image_Start_address(800*600*4);

	 LCD_CmdWrite(0x04);
     for(i=0;i<800*600;i++)
	 {
	  LCD_DataWrite(0x0000);
	   
	 }
	 delay_seconds(2);
	   	 /*
	 SFI_DMA_Destination_Upper_Left_Corner(0,0);
     SFI_DMA_Transfer_Width_Height(800,600);
     SFI_DMA_Source_Width(1920);
     SFI_DMA_Source_Start_Address(0);//24bit address

	 Start_SFI_DMA();
     Check_Busy_SFI_DMA();
	 */

	 //write S0 = MCU
     BTE_S0_Color_16bpp();
     BTE_S0_Memory_Start_Address(800*600*2);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

	 BTE_Alpha_Blending_Effect(80);
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(11); //BTE write 

  while(1)
  {
	 BTE_Enable();
	 LCD_CmdWrite(0x04);
     sd_BTE_showpic_65k_bin(800,600,"p2.bin");
	 Check_Mem_WR_FIFO_Empty();//寫完後檢查
     Check_BTE_Busy();

   }
}

//=========================================================================


//========================================================================
//S0 = MCU  S1=Desti 
void BTE_write_with_ROP_24bpp_test_Destination(void)
{
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_24bpp();

   BTE_S1_Color_24bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_24bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);
      
   

while(1)
{
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 0");
   delay_seconds(2);
	//====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
    sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 1");
   delay_seconds(2);
   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 2");
   delay_seconds(2);
	//====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 3");
   delay_seconds(2);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 4");
   delay_seconds(2);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");
   //write with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 5");
   delay_seconds(2);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 6");
   delay_seconds(2);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 7");
   delay_seconds(2);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 8");
   delay_seconds(2);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 9");
   delay_seconds(2);

   //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 10");
   delay_seconds(2);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 11");
   delay_seconds(2);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 12");
   delay_seconds(2);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 13");
   delay_seconds(2);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
    sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 14");
   delay_seconds(2);

  //====================================
   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //write with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 15");
   delay_seconds(2);

 }

}
//=========================================================================

//S0 = MCU  S1=Desti 
void BTE_write_with_ROP_24bpp_test_S1_to_Destination(void)
{
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
   Active_Window_WH(800,600);

   Main_Image_Start_Address(0);	
   Canvas_Image_Start_address(0);
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 

   BTE_S0_Color_24bpp();				//使用24bpp 會卡住!?
   //BTE_S0_Memory_Start_Address(800*600*6);
   //BTE_S0_Image_Width(800);
   //BTE_S0_Window_Start_XY(0,0);

   BTE_S1_Color_24bpp();
   BTE_S1_Memory_Start_Address(800*600*3);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_24bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);
    	  
   //寫入一張到S1

   Main_Image_Start_Address(800*600*3);	
   Canvas_Image_Start_address(800*600*3);
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 

    Main_Image_Start_Address(0);	
   //切換回destination
   Canvas_Image_Start_address(0);
while(1)
{
   //write with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 0");
   //delay_seconds(1);
   //====================================
   //write with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   Goto_Pixel_XY(0,0);
   LCD_CmdWrite(0x04);
  sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 1");
   //delay_seconds(1);
   //====================================
   //write with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
  sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 2");
   //delay_seconds(1);
   //====================================
   //write with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
  sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 3");
   //delay_seconds(1);

   //====================================
   //write with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
  sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 4");
   //delay_seconds(1);

   //====================================
   //write with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 5");
   //delay_seconds(1);

   //====================================
   //write with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   //Check_Mem_WR_FIFO_not_Full();
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 6");
   //delay_seconds(1);

   //====================================
   //write with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 7");
   //delay_seconds(1);

   //====================================
   //write with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 8");
   //delay_seconds(1);

   //====================================
   //write with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 9");
   //delay_seconds(1);

   //====================================
   //write with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 10");
  // delay_seconds(1);

  //====================================
   //write with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 11");
   //delay_seconds(1);

  //====================================
   //write with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 12");
   //delay_seconds(1);

  //====================================
   //write with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 13");
   //delay_seconds(1);

  //====================================
   //write with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 14");
   //delay_seconds(1);

  //====================================
   //write with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(0); //BTE write
   BTE_Enable();
   LCD_CmdWrite(0x04);
   sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();//寫完後檢查
   Check_BTE_Busy();

   Goto_Text_XY(300,200);
   Show_String("write with ROP 15");
   //delay_seconds(1);

 }
 
}


void BTE_Move_with_ROP_24bpp_test_positive_move(void)
{

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

   Foreground_color_16M(color16M_yellow);
   Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_24bpp();
   BTE_S0_Memory_Start_Address(800*600*3);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);

   BTE_S1_Color_24bpp();
   BTE_S1_Memory_Start_Address(800*600*3*2);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_24bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

   //寫一張底圖在S0
   Canvas_Image_Start_address(800*600*3);//
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   //寫一張底圖在S1
   Canvas_Image_Start_address(800*600*3*2);
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 

   Canvas_Image_Start_address(0);//
while(1)
{  
   //move with ROP 0 
   BTE_ROP_Code(0);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 0");
   //delay_seconds(1);
	//====================================
   //move with ROP 1 
   BTE_ROP_Code(1);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 1");
   //delay_seconds(1);
   //====================================
   //move with ROP 2 
   BTE_ROP_Code(2);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 2");
   //delay_seconds(1);
	//====================================
   //move with ROP 3 
   BTE_ROP_Code(3);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 3");
   //delay_seconds(1);
   //====================================
   //move with ROP 4 
   BTE_ROP_Code(4);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 4");
   //delay_seconds(1);
   //====================================
   //move with ROP 5 
   BTE_ROP_Code(5);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 5");
   //delay_seconds(1);
   //====================================
   //move with ROP 6 
   BTE_ROP_Code(6);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 6");
   //delay_seconds(1);
   //====================================
   //move with ROP 7 
   BTE_ROP_Code(7);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 7");
   //delay_seconds(1);
   //====================================
   //move with ROP 8 
   BTE_ROP_Code(8);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 8");
   //delay_seconds(1);
   //====================================
   //move with ROP 9 
   BTE_ROP_Code(9);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 9");
   //delay_seconds(1);
   //====================================
   //move with ROP 10 
   BTE_ROP_Code(10);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 10");
   //delay_seconds(1);
  //====================================
   //move with ROP 11 
   BTE_ROP_Code(11);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 11");
   //delay_seconds(1);
  //====================================
   //move with ROP 12 
   BTE_ROP_Code(12);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 12");
   //delay_seconds(1);
  //====================================
   //move with ROP 13 
   BTE_ROP_Code(13);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 13");
  // delay_seconds(1);
  //===================================
   //move with ROP 14 
   BTE_ROP_Code(14);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 14");
   //delay_seconds(1);
  //====================================
   //move with ROP 15 
   BTE_ROP_Code(15);
   BTE_Operation_Code(2); //BTE move
   BTE_Enable();
   Check_BTE_Busy();
   Goto_Text_XY(300,200);
   Show_String("move with ROP 15");
   //delay_seconds(5);
 }
 
}


void BTE_Transparent_Write_test_24bpp(void)
{
 unsigned int j,k,temp,tempa;
 unsigned long i;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();


  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	//Graphic_cursor_initial(); //initaial and enable graphic cursor 
	//Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_24bpp();

   BTE_S1_Color_24bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_24bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

   while(1)
   {
    BTE_Destination_Window_Start_XY(0,0); 
    BTE_Window_Size(800,600);
    //清除回原圖
    LCD_CmdWrite(0x04);
    sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	delay_seconds(1);

	Background_color_16M(color16M_green);	//設定transparent key color

	 
    for(j=0;j<(600/128);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
	 BTE_S1_Window_Start_XY(i*128,j*128);
	 BTE_Destination_Window_Start_XY(i*128,j*128); 
     BTE_Window_Size(128,128);
     BTE_ROP_Code(0);   //transparent write 與ROP cod無關
     BTE_Operation_Code(4); //BTE Transparent write
     BTE_Enable();
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(128,128,"p6_16M.bin");
     //Check_Mem_WR_FIFO_not_Full();
     Check_Mem_WR_FIFO_Empty();//寫完後檢查
     Check_BTE_Busy();

	   }
	  }
	 
	  	  delay_seconds(1);
	  

	   	 //測試所有關鍵色
	  for(i=0;i<16777216;i+=1)
	  {
	   Background_color_16M(i);	//設定transparent key color
	   BTE_S1_Window_Start_XY(100,100);
	   BTE_Destination_Window_Start_XY(100,100); 
       BTE_Window_Size(20,20);
       BTE_ROP_Code(0);   //transparent write 與ROP cod無關
       BTE_Operation_Code(4); //BTE Transparent write
       BTE_Enable();
       LCD_CmdWrite(0x04);

		 tempa=i;
		 for(j=0; j<(400/2);j++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp = (tempa>>16);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 

 		}
	   Check_Mem_WR_FIFO_Empty();//寫完後檢查
       Check_BTE_Busy();

		   
		   Background_color_16M(color16M_black);
	       Text_Mode();			  //
		   Goto_Text_XY(200,200);
	       LCD_CmdWrite(0x04);	
		   Print_Decimal_6(i); 
  		   delay_ms(1);
	      // Print_Decimal(i);	 //顯示目前KEY COLOR 
           Graphic_Mode(); //back to graphic mode  
		   //delay_seconds(1);
	    }

		//while(1);

    }

 }



//後續將修改成與S1無關20120618
void BTE_Transparent_Move_test_24bpp(void)
{
 unsigned int i,j,k;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

   Foreground_color_16M(color16M_yellow);
   Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_24bpp();
   BTE_S0_Memory_Start_Address(800*600*3);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);

   BTE_S1_Color_24bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_24bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

    BTE_Destination_Window_Start_XY(0,0); 
    BTE_Window_Size(800,600);
    //清除回原圖
    LCD_CmdWrite(0x04);
    sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	//先寫入準備Move圖檔到S0
    Canvas_Image_Start_address(800*600*3);//S0
	Active_Window_XY(100,100);
    Active_Window_WH(128,128);
	Goto_Pixel_XY(100,100);
	LCD_CmdWrite(0x04);
    sd_showpic_16M_mode1_bin(128,128,"p6_16M.bin");

	//先寫入準備Move圖檔到S0
    Canvas_Image_Start_address(800*600*3);//S0
   
	 for(j=0;j<(600/128);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
  	 Active_Window_XY(i*128,j*128);
     Active_Window_WH(128,128);
	 Goto_Pixel_XY(i*128,j*128);
	 LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(128,128,"p6_16M.bin");
	 }
	 }

	 Background_color_16M(color16M_green);	//設定transparent key color

   while(1)
   {

     for(j=0;j<(600/256);j++)
     {
	 for(i=0;i<(800/128);i++)
	 {
	 //都必須指定再同一個位置
	 BTE_S1_Window_Start_XY(i*128,j*256);	 
	 BTE_S0_Window_Start_XY(i*128,j*256);
	 BTE_Destination_Window_Start_XY(i*128,j*256); 
     BTE_Window_Size(128,128);
     BTE_ROP_Code(12);   //transparent Move S0
     BTE_Operation_Code(5); //BTE Transparent Move
     BTE_Enable();
     Check_BTE_Busy();
	 }
	 } 	
   }

 }


void BTE_pattern_Fill_test_24bpp_8x8(unsigned char t)
{
 short i,j;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);
          
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 
  RGB_16b_24bpp_mode1();
  Memory_24bpp_Mode();
  Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);


   LCD_CmdWrite(0x04); //
   sd_showpic_16M_mode1_bin(800,600,"p3.bin"); //

  Canvas_Image_Start_address(800*600*3);
  Canvas_image_width(8);
  Active_Window_XY(0,0);
  Active_Window_WH(8,8);
  Goto_Pixel_XY(0,0);

  for(i=0;i<5000;i++)
  {
   Canvas_Image_Start_address((800*600*3)+i*192);
   Pattern_Format_8X8();
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(8,8,"p8_16M.bin");
  }

while(1){

 for(j=0;j<50;j++)
 {
  for(i=0;i<100;i++)
  { 
    BTE_S0_Color_24bpp();
    BTE_S0_Memory_Start_Address((800*600*3)+i*192);
    BTE_S0_Image_Width(8);
 
    BTE_S1_Color_24bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_24bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);

	if(t==0)
    BTE_Operation_Code(0x06);
	else
	BTE_Operation_Code(0x07);
    
	BTE_S1_Window_Start_XY(i*8,j*8);      
    BTE_Destination_Window_Start_XY(i*8,j*8);
    BTE_Window_Size(8,8);

    BTE_Enable();   
    Check_BTE_Busy();
    }
  }


     }

}


void BTE_pattern_Fill_test_24bpp_16x16(unsigned char t)
{
 short i,j;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Canvas_Image_Start_address(0);
  Canvas_image_width(800);

  Main_Image_Start_Address(0);                                   
  Main_Image_Width(800);                                                                 
  Main_Window_Start_XY(0,0);
       
  Active_Window_XY(0,0);
  Active_Window_WH(800,600);
 

  RGB_16b_24bpp_mode1();
  Memory_24bpp_Mode();
  Select_Main_Window_24bpp();

  Main_Window_Start_XY(0,0);

   LCD_CmdWrite(0x04); //
   sd_showpic_16M_mode1_bin(800,600,"p3.bin"); //

  Canvas_Image_Start_address(800*600*3);
  Canvas_image_width(16);
  Active_Window_XY(0,0);
  Active_Window_WH(16,16);
  Goto_Pixel_XY(0,0);

  for(i=0;i<1000;i++)
  {
   Canvas_Image_Start_address((800*600*3)+i*768);
   Pattern_Format_16X16();
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(16,16,"p16_16M.bin");
  }

while(1){

 for(j=0;j<20;j++)
 {
  for(i=0;i<50;i++)
  { 
    BTE_S0_Color_24bpp();
    BTE_S0_Memory_Start_Address((800*600*3)+i*768);
    BTE_S0_Image_Width(16);
 
    BTE_S1_Color_24bpp();
    BTE_S1_Memory_Start_Address(0);
    BTE_S1_Image_Width(800);

    BTE_Destination_Color_24bpp();  
    BTE_Destination_Memory_Start_Address(0);
    BTE_Destination_Image_Width(800);

    BTE_ROP_Code(0xc);

	if(t==0)
    BTE_Operation_Code(0x06);
    else
	BTE_Operation_Code(0x07);
	
	BTE_S1_Window_Start_XY(i*16,j*16);      
    BTE_Destination_Window_Start_XY(i*16,j*16);
    BTE_Window_Size(16,16);

    BTE_Enable();   
    Check_BTE_Busy();
	//delay_ms(100);
    }
  }

	 //delay_ms(1000);
     }

}


void BTE_Color_Expansion_test_24bpp(unsigned char t)
{
 unsigned int i,j,k;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
   Active_Window_WH(800,600);

   BTE_S0_Color_24bpp();
   BTE_S0_Memory_Start_Address(800*600*3);
   BTE_S0_Image_Width(800);
   BTE_S0_Window_Start_XY(0,0);

   BTE_S1_Color_24bpp();
   BTE_S1_Memory_Start_Address(0);
   BTE_S1_Image_Width(800);
   BTE_S1_Window_Start_XY(0,0);

   BTE_Destination_Color_24bpp();  
   BTE_Destination_Memory_Start_Address(0);
   BTE_Destination_Image_Width(800);
   BTE_Destination_Window_Start_XY(0,0);  
   BTE_Window_Size(800,600);

   //清除回原圖
   LCD_CmdWrite(0x04);
   sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   while(1)
   {
	 Foreground_color_16M(color16M_white);
     Background_color_16M(color16M_black);
	 BTE_ROP_Code(15);

	 if(t==0)
     BTE_Operation_Code(8); //BTE color expansion
	 else
	 BTE_Operation_Code(9); //BTE color expansion

	 RGB_16b_16bpp();
	 BTE_Enable();
	 LCD_CmdWrite(0x04);
     for(j=0;j<600;j++)
     {
	  for(i=0;i<(800/16);i++)
	  {
	   LCD_DataWrite(0xaa00);
	   
	   Check_Mem_WR_FIFO_not_Full(); //
	   //delay_ms(1);
       }
	  }	
	  Check_Mem_WR_FIFO_Empty();  	  
	  Check_BTE_Busy();
	  //while(1);
    }

 }



 void BTE_Alpha_blending_24bpp_to_16bpp(void)
 {

  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 

	 Active_Window_WH(800,600);
     BTE_Destination_Color_24bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

	 Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");

     BTE_S0_Color_24bpp();
     BTE_S0_Memory_Start_Address(800*600*3);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600*3);
	 Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   while(1)
   { 

 
for(b=0;b<(600/128);b++)
   {
	for(a=0;a<(800/128);a++)
    {
	 BTE_Destination_Window_Start_XY(a*128,b*128);  
	 BTE_S0_Window_Start_XY(a*128,b*128);

  	 BTE_S1_Color_16bit_Alpha(); //當設定為Alpha 4bit  index color 為12bit  
     BTE_S1_Memory_Start_Address(800*600*6);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(a*128,b*128);


	 RGB_16b_16bpp();
	 Memory_16bpp_Mode();
	 Canvas_Image_Start_address(800*600*6);
	 Active_Window_XY(a*128,b*128);
	 Active_Window_WH(128,128);

	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xff00);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x0000);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE 
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	 // delay_seconds(1);

	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xf0f0);
	       
		 }
		else
		 {
	      LCD_DataWrite(0xE111);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	 // delay_seconds(1);


	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xf00f);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x0222);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	  //delay_seconds(1);


	 Goto_Pixel_XY(a*128,b*128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xa555);
	       
		 }
		else
		 {
	      LCD_DataWrite(0xA111);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	 // delay_seconds(1);
 }
 }
    }

 }


 void BTE_Alpha_blending_24bpp_to_8bpp(void)
 {
  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

 
   Foreground_color_16M(color65k_yellow);
   Background_color_16M(color65k_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 

   //16M色應用  Palette 寫入
	RGB_16b_16bpp();
    Memory_Select_Color_Palette_RAM();
	LCD_CmdWrite(0x04);  
	  
	for(i=0;i<64;i++)
	{
	 temp = 0;
	 k=i;	 
	 if((k&0x01)==0x01)
	 {temp |=4;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=8;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=64;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=128;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 1024;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 2048;}

	 LCD_DataWrite(temp);
	 //LCD_DataWrite(0x00);
	 delay_ms(1);

	 }

	//寫完切回
	RGB_16b_24bpp_mode1();
     Memory_Select_SDRAM();

	 Active_Window_WH(800,600);
     BTE_Destination_Color_24bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

	  Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");

     BTE_S0_Color_24bpp();
     BTE_S0_Memory_Start_Address(800*600*3);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600*3);
	  Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");

   while(1)
   { 


 for(b=0;b<(600/128);b++)
   {
	for(a=0;a<(800/128);a++)
    {
	   BTE_Destination_Window_Start_XY(a*128,b*128);  
	   BTE_S0_Window_Start_XY(a*128,b*128);

  	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(800*600*6);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

	 Memory_8bpp_Mode();//OSD LAYER 先切換成8BPP	
	 Canvas_Image_Start_address(800*600*6);

	 Active_Window_WH(128,128);

	   Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xcc);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Window_Size(128,128);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(10); //BTE Alpha
	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

	   //delay_seconds(1);

       Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x8c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 BTE_Enable();
	   	  
	 Check_BTE_Busy();

     //delay_seconds(1);

	    Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x4c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();	   	  
	 Check_BTE_Busy();

     //delay_seconds(1);


	   Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0x0c);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }


	 BTE_Enable();	   	  
	 Check_BTE_Busy();

     //delay_seconds(1);

 }
 }

    }

 }

 //=======================================================================
void BTE_MPU_Write_Alpha_blending_24bpp_to_8bpp(void)
{
  unsigned int i,j;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 

   //16M應用 Palette 寫入
    RGB_16b_8bpp();
    Memory_Select_Color_Palette_RAM();
	LCD_CmdWrite(0x04);  
	  
	for(i=0;i<64;i++)
	{
	 temp = 0;
	 k=i;	 
	 if((k&0x01)==0x01)
	 {temp |=4;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=8;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=64;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |=128;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 1024;}
	 k>>=1;
	 if((k&0x01)==0x01)
	 {temp |= 2048;}

	 LCD_DataWrite(temp);
	 //LCD_DataWrite(0x00);
	 delay_ms(1);
	 LCD_DataWrite(temp>>8);
	 //LCD_DataWrite(0x0f);
	 delay_ms(1);
	 }


	//寫完切回
	RGB_16b_24bpp_mode1();
     Memory_Select_SDRAM();
	 Active_Window_WH(800,600);

     BTE_Destination_Color_24bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	 BTE_S0_Color_24bpp();					 //MPU write = S0
     //BTE_S0_Memory_Start_Address(800*600*3);
     //BTE_S0_Image_Width(800);
     //BTE_S0_Window_Start_XY(0,0);
     //Canvas_Image_Start_address(800*600*3);

	 	
	 BTE_S1_Color_8bit_Alpha(); //當設定為Alpha 8bit 會由palette取值 index color 為6bit  
     BTE_S1_Memory_Start_Address(800*600*3);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);
	 Canvas_Image_Start_address(800*600*3);	//寫入S1位置
	 
	 RGB_16b_8bpp();
	 Memory_8bpp_Mode();//OSD LAYER 先切換成8BPP
	 
	 Active_Window_WH(800,600);	//OSD 寫入大小	
	 LCD_CmdWrite(0x04);
	 for(i=0;i<800*600;i++)
	 {
       LCD_DataWrite(0x00);
	    
	 }
	 Active_Window_WH(128,128);	//OSD 寫入大小	
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xcc);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x00);
	       
		 }
		 temp1<<=1;
	   }
	 }
	 RGB_16b_24bpp_mode1();
	 Memory_24bpp_Mode();//寫完切回24bit


   while(1)
   {  	 
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(12);
     BTE_Operation_Code(11); //BTE write with alpha 
	 BTE_Enable();

     LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  	  
	 
	 delay_seconds(1);

	 BTE_Enable();	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

	 BTE_Enable();	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  
     delay_seconds(1);

	 BTE_Enable();	   	   	 
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

    }
}


//=======================================================================
void BTE_MPU_Write_Alpha_blending_24bpp_to_16bpp(void)
{
  unsigned int i,j;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 

	//寫完切回
     Memory_Select_SDRAM();
	 Active_Window_WH(800,600);

     BTE_Destination_Color_24bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	 BTE_S0_Color_24bpp();					 //MPU write = S0
     //BTE_S0_Memory_Start_Address(800*600*3);
     //BTE_S0_Image_Width(800);
     //BTE_S0_Window_Start_XY(0,0);
     //Canvas_Image_Start_address(800*600*3);

	 	
	 BTE_S1_Color_16bit_Alpha(); //
     BTE_S1_Memory_Start_Address(800*600*6);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);
	 Canvas_Image_Start_address(800*600*6);	//寫入S1位置
	 
	  RGB_16b_16bpp();
     Memory_16bpp_Mode();	

	 Active_Window_WH(800,600);	//OSD 寫入大小	
	 LCD_CmdWrite(0x04);
	 for(i=0;i<800*600;i++)
	 {
       LCD_DataWrite(0x0000);
	    
	 }


	 Active_Window_WH(128,128);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<16*128;i++)
	 {
	   temp1=gImage_p128_1bpp[i];

	   for(j=0;j<8;j++)
	   {
	    if((temp1&0x80)==0x80)
		 {
	      LCD_DataWrite(0xff00);
	       
		 }
		else
		 {
	      LCD_DataWrite(0x0000);
	       
		 }
		 temp1<<=1;
	   }
	 }

	 RGB_16b_24bpp_mode1();
     Memory_24bpp_Mode();

   while(1)
   {  	 
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(12);
     BTE_Operation_Code(11); //BTE write with alpha 
	 BTE_Enable();

     LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  	  
	 
	 delay_seconds(1);

	 BTE_Enable();	   	  
     LCD_CmdWrite(0x04);
      sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

	 BTE_Enable();	   	  
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  
     delay_seconds(1);

	 BTE_Enable();	   	   	 
     LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();
	 Check_BTE_Busy();  

     delay_seconds(1);

    }
}


//==================================================================
void BTE_Solid_Fill_test_24bpp(void)
{
  unsigned int i;

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

   Foreground_color_16M(color16M_yellow);
   Background_color_16M(color16M_purple);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
	 BTE_S0_Color_24bpp();	//與S0色深有關 ,後續會修改與destination 有關20120619

	 Active_Window_WH(800,600);
     BTE_Destination_Color_24bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	 BTE_ROP_Code(12);
     BTE_Operation_Code(12); //BTE solid fill
 while(1)
 {

	 for(i=0;i<256;i++)
	 {
     Foreground_RGB(i,0x00,0x00);
	 BTE_Destination_Window_Start_XY(i*3,0);
     BTE_Window_Size(3,200);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

	 for(i=0;i<256;i++)
	 {
     Foreground_RGB(0x00,i,0x00);
	 BTE_Destination_Window_Start_XY(i*3,200);
     BTE_Window_Size(3,200);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

	 for(i=0;i<256;i++)
	 {
     Foreground_RGB(0x00,0x00,i);
	 BTE_Destination_Window_Start_XY(i*3,400);
     BTE_Window_Size(3,200);
	 BTE_Enable();
	 Check_BTE_Busy();
	 }

 }
}


void BTE_move_Alpha_blending_by_picture_24bpp(void)
{
  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_white);
  Background_color_16M(color16M_black);
 
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

  Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();
  //Select_SFI_32bit_Address();

  //Select_SFI_Waveform_Mode_0();
  Select_SFI_Waveform_Mode_3();

  //Select_SFI_0_DummyRead();	//normal read mode
  Select_SFI_8_DummyRead(); //1byte dummy cycle
  //Select_SFI_16_DummyRead();
  //Select_SFI_24_DummyRead();

  //Select_SFI_Single_Mode();
  Select_SFI_Dual_Mode0();
  //Select_SFI_Dual_Mode1();
  SPI_Clock_Period(0);

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
	 Active_Window_WH(800,600);
     BTE_Destination_Color_24bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");


	 Main_Image_Start_Address(800*600*3);
     BTE_S0_Color_24bpp();
     BTE_S0_Memory_Start_Address(800*600*3);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

     Canvas_Image_Start_address(800*600*3);
     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");
	 delay_seconds(2);

     BTE_S1_Color_24bpp();
     BTE_S1_Memory_Start_Address(800*600*6);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);

  while(1)
  {
	 //Main_Image_Start_Address(800*600*6);	
     Canvas_Image_Start_address(800*600*6);

	 LCD_CmdWrite(0x04);
     for(i=0;i<(800*600)/2;i++)
	 {
	  LCD_DataWrite(0x0000);
	   
	  LCD_DataWrite(0x0000);
	   
	  LCD_DataWrite(0x0000);
	   
	 }
	 delay_seconds(2);

	 Main_Image_Start_Address(0);
  	 /*
	 SFI_DMA_Destination_Upper_Left_Corner(0,0);
     SFI_DMA_Transfer_Width_Height(800,600);
     SFI_DMA_Source_Width(1920);
     SFI_DMA_Source_Start_Address(0);//24bit address

	 Start_SFI_DMA();
     Check_Busy_SFI_DMA();
	 */
	 BTE_Alpha_Blending_Effect(80);
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(15);
     BTE_Operation_Code(10); //BTE move  
	 BTE_Enable();
	 Check_BTE_Busy();

   }
	 
}


void BTE_write_Alpha_blending_by_picture_24bpp(void)
{
  unsigned int i,j,a,b;
  unsigned short temp=0;
  unsigned char temp1,k;
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   //Font_Width_X1();
   Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   //Font_Height_X1();
   Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_16M(color16M_white);
  Background_color_16M(color16M_black);
 
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor  

  Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();
  //Select_SFI_32bit_Address();

  //Select_SFI_Waveform_Mode_0();
  Select_SFI_Waveform_Mode_3();

  //Select_SFI_0_DummyRead();	//normal read mode
  Select_SFI_8_DummyRead(); //1byte dummy cycle
  //Select_SFI_16_DummyRead();
  //Select_SFI_24_DummyRead();

  //Select_SFI_Single_Mode();
  Select_SFI_Dual_Mode0();
  //Select_SFI_Dual_Mode1();

  SPI_Clock_Period(0);


   Canvas_Image_Start_address(0);
   Canvas_image_width(800);

   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);
	
   Active_Window_XY(0,0);
   Active_Window_WH(800,600);

   RGB_16b_24bpp_mode1();
   Memory_24bpp_Mode();	
   Select_Main_Window_24bpp();
 
	 Active_Window_WH(800,600);
     BTE_Destination_Color_24bpp();  
     BTE_Destination_Memory_Start_Address(0);
     BTE_Destination_Image_Width(800);
     BTE_Destination_Window_Start_XY(0,0);  
     BTE_Window_Size(800,600);

     LCD_CmdWrite(0x04);
     sd_showpic_16M_mode1_bin(800,600,"p3.bin");



     BTE_S1_Color_24bpp();
     BTE_S1_Memory_Start_Address(800*600*4);
     BTE_S1_Image_Width(800);
     BTE_S1_Window_Start_XY(0,0);	
     Canvas_Image_Start_address(800*600*4);

	 LCD_CmdWrite(0x04);
     for(i=0;i<(800*600)/2;i++)
	 {
	  LCD_DataWrite(0x0000);
	   
	  	  LCD_DataWrite(0x0000);
	   
	  	  LCD_DataWrite(0x0000);
	   
	 }
	 delay_seconds(2);
	   	 /*
	 SFI_DMA_Destination_Upper_Left_Corner(0,0);
     SFI_DMA_Transfer_Width_Height(800,600);
     SFI_DMA_Source_Width(1920);
     SFI_DMA_Source_Start_Address(0);//24bit address

	 Start_SFI_DMA();
     Check_Busy_SFI_DMA();
	 */

	 //write S0 = MCU
     BTE_S0_Color_24bpp();
     BTE_S0_Memory_Start_Address(800*600*3);
     BTE_S0_Image_Width(800);
     BTE_S0_Window_Start_XY(0,0);

	 BTE_Alpha_Blending_Effect(80);
	 BTE_Window_Size(800,600);
	 BTE_ROP_Code(7);
     BTE_Operation_Code(11); //BTE write 

  while(1)
  {
	 BTE_Enable();
	 LCD_CmdWrite(0x04);
     sd_BTE_showpic_16M_mode1_bin(800,600,"p3.bin");
	 Check_Mem_WR_FIFO_Empty();//寫完後檢查
     Check_BTE_Busy();

   }
}






//write enable
void SPIM_WREN(void)
{
	nSS_Active();
	SPI_Master_FIFO_Data_Put(0x06);
	nSS_Inactive();
}

//write disable
void SPIM_WRDI(void)
{

	nSS_Active();
	SPI_Master_FIFO_Data_Put(0x04); 
	nSS_Inactive();
	
}	

//write ststus register
void SPIM_WRSR(u8 UpdataValue)
{	
    SPIM_WREN();
	nSS_Active();
	SPI_Master_FIFO_Data_Put(0x01);
	SPI_Master_FIFO_Data_Put(UpdataValue);
	nSS_Inactive();
	
}


//check write in progress bit
//1:busy
//0:normal
 unsigned char SPIM_WIP(void)
{
 unsigned char temp;
temp = SPIM_RDSR();

if((temp&0x01)==0x01)
return 1;
else   
return 0;
}

//check write enable latch bit 
//1: write latch
//0: no latch
 unsigned char SPIM_WEL(void)
{
 unsigned char temp;
temp = SPIM_RDSR();

if((temp&0x02)==0x02)
return 1;
else 
return 0;	
}


//read status register
 unsigned char SPIM_RDSR(void)
{
 unsigned char temp;

	nSS_Active();
	SPI_Master_FIFO_Data_Put(0x05);
	temp=SPI_Master_FIFO_Data_Put(0xff);//dummy cycle and read back value
	nSS_Inactive();
	return(temp);	
}
  

//
 void SPIM_WRITE_DATA_BYTE( unsigned long address, unsigned char UpdataValue)
{
    do{
    SPIM_WREN(); 
    }
	while(SPIM_WEL()==0);  	
	nSS_Active();
    SPI_Master_FIFO_Data_Put(0x02);
	SPI_Master_FIFO_Data_Put((address&0xffffff)>>16);
	SPI_Master_FIFO_Data_Put((address&0xffff)>>8);
	SPI_Master_FIFO_Data_Put(address&0xff);

	SPI_Master_FIFO_Data_Put(UpdataValue);	
	nSS_Inactive(); 

	do{}
	while(SPIM_WIP());
} 

 void SPIM_WRITE_DATA_PAGE(unsigned long address,unsigned char *UpdataValue)
{	unsigned short i ;

    do{
    SPIM_WREN(); 
    }
	while(SPIM_WEL()==0); 
	nSS_Active();
    SPI_Master_FIFO_Data_Put(0x02);
	SPI_Master_FIFO_Data_Put((address&0xffffff)>>16);
	SPI_Master_FIFO_Data_Put((address&0xffff)>>8);
	SPI_Master_FIFO_Data_Put(address&0xff);
	for(i=0;i<256;i++)
	{
	SPI_Master_FIFO_Data_Put(UpdataValue[i]);
	}	
	nSS_Inactive(); 

	do{}
	while(SPIM_WIP());
}
//==========================================================================

//SPI_Master application subroutine

 unsigned char SPIM_READ_DATA_BYTE(unsigned long address)
{
	unsigned char temp;

	 nSS_Active();

	 SPI_Master_FIFO_Data_Put(0x03);
	 SPI_Master_FIFO_Data_Put((address&0xffffff)>>16);
	 SPI_Master_FIFO_Data_Put((address&0xffff)>>8);
	 SPI_Master_FIFO_Data_Put(address&0xff);

	 temp=SPI_Master_FIFO_Data_Put(0xff);//dummy cycle
	
	 nSS_Inactive(); 

	return(temp);
	
}

void SPIM_CHIP_ERASE(void)
{
 nSS_Active();
 SPI_Master_FIFO_Data_Put(0x60);
 nSS_Inactive();
}

/****************************
*FLASH 整片擦除
*返回1 異常
*返回0 成功
*****************************/ 
 unsigned char SPIM_FLASH_ERASE(void)
{ 
 unsigned char h;
	 //u8 temp;
	// u32 i;
	 GPIO_ResetBits(GPIOB, GPIO_Pin_0);	//LED D3 off 綠燈

     SPIM_WREN(); 
	 	   
	 if(SPIM_WEL()==0) //如果WEL=1,write enable失敗
	 {return 1 ;}

	 SPIM_CHIP_ERASE();//

	 h=0;
	 do{
	   GPIO_SetBits(GPIOB, GPIO_Pin_1);    //LED D4 on 	紅燈
	   delay_ms(500);
	   GPIO_ResetBits(GPIOB, GPIO_Pin_1);    //LED D4 off 紅燈
	   delay_ms(500);
	   h++;
	  }
	 while(SPIM_WIP()&&(h<401));	 //等待100秒   64Mbit 50秒完成,	128Mbit 80秒完成  256Mbit 160~400S

	 if(h==400)		 //如400秒還未完成返回1 異常
	 {return 1;}
	 
	 //if needs verify 
	 /* 
	//verify erase 	check all value = 0xff

	  D4_timer3_ini_PWM_enable();//

	  for(i=0;i<FLASH_CHIP;i++)
	  {			 
	    temp=SPI_READ_DATA_BYTE(i);	 
      
		if(temp != 0xff)
		{
		 D4_timer3_ini_PWM_disable();
		return 1;}	
		
		//GPIO_SetBits(GPIOB, GPIO_Pin_0);    //LED D3 on 	 
		//GPIO_ResetBits(GPIOB, GPIO_Pin_0);	//LED D3 off	
	  }

    D4_timer3_ini_PWM_disable();
	*/	  	    
   return 0;  //片擦除完成

} 


/****************************
*FLASH 	燒錄
*返回1 異常
*返回0 成功
*****************************/ 
 unsigned char SPIM_FLASH_PROGRAM(void)
{

   if(SPIM_FLASH_ERASE())
    {
	 return 1;
    }
	else
	{
	 //add your code
	}

 	return 0;
 }
//==========================================================================
/*REG[B8h]*/
void SPI_Master_test(void)
{
 //燒錄
 //讀出serial flash 資料比對

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();

Active_Window_WH(800,600);
Goto_Pixel_XY(0,0);
Graphic_Mode();
LCD_CmdWrite(0x04);
sd_showpic_256_bin(800,600,"p1.bin");

Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);
 /*
SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(0);//24bit address

Main_Window_Start_XY(0,0);
Start_SFI_DMA();
delay_seconds(1);
*/
//Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

 
 
while(1){
 
 detect_button_B1();  

 while(B1_event_flag) //當B1按鍵事件觸發,擦除serial flash
 {
	 //LCD_Clear(0x07e0);
     //LCD_DisplayStringLine(Line0,"Erase",Black,Green);
	 printf("\r\n Erase\r\n");
    if(SPIM_FLASH_ERASE())	//1 = 異常
     {
	 GPIO_ResetBits(GPIOB, GPIO_Pin_0);	//LED D3 off 綠燈
     GPIO_SetBits(GPIOB, GPIO_Pin_1);    //LED D4 on  紅燈
	 //LCD_Clear(0xf800);
     //LCD_DisplayStringLine(Line0,"Erase Fail",Black,Green);
	 printf("\r\n Erase Fail\r\n");
     }
    else //0 = 完成
     {	
     GPIO_SetBits(GPIOB, GPIO_Pin_0);    //LED D3 on  綠燈
     GPIO_ResetBits(GPIOB, GPIO_Pin_1);	//LED D4 off 紅燈

	 //LCD_Clear(0x07e0);
     //LCD_DisplayStringLine(Line0,"Erase Done",Black,Green);
  	 printf("\r\n Erase Done\r\n");
	 delay_ms(1000);

	 //LCD_Clear(0x07e0);
     //LCD_DisplayStringLine(Line0,"Programming..",Black,Green);
	 printf("\r\n Programming..\r\n");
     //Program_bin2flash("AllPic.bin");

      SPIM_PageProgram_bin2flash("AllPic.bin");
      //SPIM_PageProgram_bin2flash("INI_32.bin"); //for test initial display
	  //SPIM_PageProgram_bin2flash("INI_24.bin"); //for test initial display		   
	   //LCD_Clear(0x07e0);
       //LCD_DisplayStringLine(Line0,"Verifying..",Black,Green);
	   printf("\r\n Verifying..\r\n");
		//delay_ms(1000);

		delay_seconds(2);
			 
//call compare 
	 //if(SPIM_verify_flash_data("INI_32.bin")) //for test initial display
	 //if(SPIM_verify_flash_data("INI_24.bin")) //for test initial display	
   	   if(SPIM_verify_flash_data("AllPic.bin"))	     
       {
        GPIO_ResetBits(GPIOB, GPIO_Pin_0);	//LED D3 off 綠燈
        GPIO_SetBits(GPIOB, GPIO_Pin_1);    //LED D4 on  紅燈
       // LCD_Clear(0x07e0);
       // LCD_DisplayStringLine(Line0,"Verify Error",Black,Green);
		printf("\r\n Verify Error\r\n");
       }
       else
       {
        GPIO_SetBits(GPIOB, GPIO_Pin_0);    //LED D3 on  綠燈
        GPIO_ResetBits(GPIOB, GPIO_Pin_1);	//LED D4 off 紅燈
        //LCD_Clear(0x07e0);
        //LCD_DisplayStringLine(Line0,"Verify Done..Succeed",Black,Green);
		printf("\r\n Verify Done..Succeed\r\n");
       }

	  }
  
     B1_event_flag = 0;

   }

  }
}


void SPIM_test_OVFIRQEN_EMTIRQEN(void)
{
   unsigned temp;
   unsigned i,j;

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();

Active_Window_WH(800,600);
Goto_Pixel_XY(0,0);
Graphic_Mode();
LCD_CmdWrite(0x04);
sd_showpic_256_bin(800,600,"p1.bin");

Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);
 /*
SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(0);//24bit address

Main_Window_Start_XY(0,0);
Start_SFI_DMA();
delay_seconds(1);
*/
//Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

	 //Mask_SPI_Master_Interrupt_Flag();//如果MASK就測量不到外部INT信號

    //OVFIRQEN_Enable();
	EMTIRQEN_Enable();

while(1)
{
     nSS_Active();

	 SPI_Master_FIFO_Data_Put(0x03);
	 j = EMTI_Flag();
	 printf("j=%d",j);
	 printf(" ");

	 Clear_EMTI_Flag();
	 j = EMTI_Flag();
	 printf("j=%d",j);
	 printf(" ");

	 SPI_Master_FIFO_Data_Put(0x00);

	 SPI_Master_FIFO_Data_Put(0x03);
	 j = EMTI_Flag();
	 printf("j=%d",j);
	 printf(" ");

	 Clear_EMTI_Flag();
	 j = EMTI_Flag();
	 printf("j=%d",j);
	 printf(" ");

	 SPI_Master_FIFO_Data_Put(0x00);
	 SPI_Master_FIFO_Data_Put(0x03);
	 j = EMTI_Flag();
	 printf("j=%d",j);
	 printf(" ");

	 Clear_EMTI_Flag();
	 j = EMTI_Flag();
	 printf("j=%d",j);
	 printf(" ");
	 /*
	 SPI_Master_FIFO_Data_Put(0x00);

	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	 SPI_Master_FIFO_Data_Put(0xff);
	  */
	 nSS_Inactive(); 
	 /*
	 i = OVFI_Flag();
	 printf("i=%d",i);
	 printf(" ");

	 Clear_OVFI_Flag();
	 i = OVFI_Flag();
	 printf("i=%d",i);
	 printf(" ");
	 */
  delay_seconds(1);
  }

  	 while(1);

}




//=======================================================================

/*REG[CCh][DBh][DEh]*/

void CGRAM_test_8bpp_HalfFont_8x16(void)
{
 unsigned int i,j,temp;



//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
   

     //半形
    /* 
   //Write CGRAM in block mode (canvas 必須與active window 設定一樣大才可以在block mode下使用,平常建議使用linear mode, initial CGRAM)
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	
	for(j=0; j<2000;j++)				 //寫入半型字
	 {	 
	 for(i=0; i<16;i++)	 //			 
		{
		 LCD_DataWrite(gImage_Font_8x16[i]);	  
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	}	 


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<2000;i++)				//寫出半型字 字碼由0x0000開始
	  {
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 
  	   }
	  */
  	  

   //半形
   while(1)
   {

     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(240000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);

	 Goto_Linear_Addr(240000);

	 LCD_CmdWrite(0x04);					 
	for(j=0; j<1500;j++)
	 {	 
	 for(i=0; i<16;i++)	 //16*16/8			 
		{
		 LCD_DataWrite(gImage_Font_8x16[i]);  //寫入半型字
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1500;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }

}


void CGRAM_test_8bpp_FullFont_16x16(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
   

     //全形
    /* 
   //Write CGRAM in block mode (canvas 必須與active window 設定一樣大才可以在block mode下使用,平常建議使用linear mode, initial CGRAM)
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	
	for(j=0; j<10;j++)				 //寫入全型字
	 {	 
	 for(i=0; i<32;i++)	 //16*16/8			 
		{
		 LCD_DataWrite(gImage_Font_16x16[i]);	  
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	}	 


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(16,16);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<10;i++)				//寫出全型字 字碼由0x8000
	  {
	   temp=i+32768;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 
  	   }

  	  */

   //全形
   while(1)
   {

     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(240000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);

	 Goto_Linear_Addr(240000);

	 LCD_CmdWrite(0x04);					  //寫入全型字
	for(j=0; j<200;j++)
	 {	 
	 for(i=0; i<32;i++)	 //16*16/8			 
		{
		 LCD_DataWrite(gImage_Font_16x16[i]);
		   Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   	 for(i=0; i<32;i++)	 //16*16/8			 
		{
		 LCD_DataWrite(~gImage_Font_16x16[i]);
		   Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<200*2;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   delay_ms(10);
	  }
	  Check_2D_Busy();
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }


}


void CGRAM_test_8bpp_HalfFont_12x24(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  Check_Mem_WR_FIFO_not_Full();
 		}
   

     //半形
     /*
   //Write CGRAM in block mode (canvas 必須與active window 設定一樣大才可以在block mode下使用,平常建議使用linear mode, initial CGRAM)
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	
	for(j=0; j<1000;j++)				 //寫入半型字
	 {	 
	 for(i=0; i<48;i++)	 //		 
		{
		 LCD_DataWrite(gImage_Font_12x24[i]);	  
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(2);	
		}
	}	 


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1000;i++)				//寫出半型字 字碼由0x0000開始
	  {
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 
  	   }
	 */
  	  

   //半形
   while(1)
   {

     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(480000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);

	 Goto_Linear_Addr(480000);

	 LCD_CmdWrite(0x04);					 
	for(j=0; j<500;j++)
	 {	 
	 for(i=0; i<48;i++)	 //		 
		{
		 LCD_DataWrite(gImage_Font_12x24[i]);  //寫入半型字
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}

	    for(i=0; i<48;i++)	 //		 
		{
		 LCD_DataWrite(~gImage_Font_12x24[i]);  //寫入半型字
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}

	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<500*2;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	    Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	    Check_Mem_WR_FIFO_not_Full();	   
	   //delay_ms(10);
	  }
	  Check_2D_Busy();
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }

}


void CGRAM_test_8bpp_FullFont_24x24(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  Check_Mem_WR_FIFO_not_Full();
 		}
   

     //全形
    /* 
   //Write CGRAM in block mode (canvas 必須與active window 設定一樣大才可以在block mode下使用,平常建議使用linear mode, initial CGRAM)
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	
	for(j=0; j<500;j++)				 //寫入全型字
	 {	 
	 for(i=0; i<72;i++)	 //			 
		{
		 LCD_DataWrite(gImage_Font_24x24[i]);	  
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	}	 


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(16,16);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  LCD_CmdWrite(0x04);

	  for(i=0;i<500;i++)				//寫出全型字 字碼由0x8000
	  {
	   temp=i+32768;

	   
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 
  	   }

  	  */

   //全形
   while(1)
   {

     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(480000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);

	 Goto_Linear_Addr(480000);

	 LCD_CmdWrite(0x04);					  //寫入全型字
	for(j=0; j<500;j++)
	 {	 
	 for(i=0; i<72;i++)	 //			 
		{
		 LCD_DataWrite(gImage_Font_24x24[i]);
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(2);	
		}
	   for(i=0; i<72;i++)	 //			 
		{
		 LCD_DataWrite(~gImage_Font_24x24[i]);
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(2);	
		}
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	    LCD_CmdWrite(0x04);
	  for(i=0;i<500*2;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;

	 
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   delay_ms(10);
	  }	
	  Check_2D_Busy();
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }


}


void CGRAM_test_8bpp_HalfFont_16x32(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
 		}
   

     //半形
     /*
   //Write CGRAM in block mode (canvas 必須與active window 設定一樣大才可以在block mode下使用,平常建議使用linear mode, initial CGRAM)
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	
	for(j=0; j<2000;j++)				 //寫入半型字
	 {	 
	 for(i=0; i<64;i++)	 //		 
		{
		 LCD_DataWrite(gImage_Font_16x32[i]);	  
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	}	 


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<2000;i++)				//寫出半型字 字碼由0x0000開始
	  {
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 
  	   }
	 */
  	  

   //半形
   while(1)
   {

     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(480000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);

	 Goto_Linear_Addr(480000);

	 LCD_CmdWrite(0x04);					 
	for(j=0; j<500;j++)
	 {	 
	 for(i=0; i<64;i++)	 //		 
		{
		 LCD_DataWrite(gImage_Font_16x32[i]);  //寫入半型字
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
			 for(i=0; i<64;i++)	 //		 
		{
		 LCD_DataWrite(~gImage_Font_16x32[i]);  //寫入半型字
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1000;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	  }
	  Check_2D_Busy();
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }

}




void CGRAM_test_8bpp_FullFont_32x32(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

//	Graphic_cursor_initial(); //initaial and enable graphic cursor 
//	Text_cursor_initial();	//initaial and enable text cursor 


	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
 		}
   

     //全形
    
   //Write CGRAM in block mode (canvas 必須與active window 設定一樣大才可以在block mode下使用,平常建議使用linear mode, initial CGRAM)
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	
	for(j=0; j<250;j++)				 //寫入全型字
	 {	 
	 for(i=0; i<128;i++)	 //16*16/8			 
		{
		 LCD_DataWrite(gImage_Font_32x32[i]);
		 Check_Mem_WR_FIFO_not_Full();	  
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(2);	
		}
	 for(i=0; i<128;i++)	 //16*16/8			 
		{
		 LCD_DataWrite(~gImage_Font_32x32[i]);
		 Check_Mem_WR_FIFO_not_Full();	  
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(2);	
		}

	}	 


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<250;i++)				//寫出全型字 字碼由0x8000
	  {
	   temp=i+32768;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	    Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	    Check_Mem_WR_FIFO_not_Full();
	   
	   //Check_2D_Busy();
	   //delay_ms(10);
	  }
	  Check_2D_Busy();
	  Graphic_Mode();
	 
  	   }

  	  

   //全形
   while(1)
   {

     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(240000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);

	 Goto_Linear_Addr(240000);

	 LCD_CmdWrite(0x04);					  //寫入全型字
	for(j=0; j<500;j++)
	 {	 
	 for(i=0; i<128;i++)	 //16*16/8			 
		{
		 LCD_DataWrite(gImage_Font_32x32[i]);
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<500;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	  }
	 Check_2D_Busy();
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }


}



void CGRAM_test_16bpp_HalfFont_8x16(void)
{
  unsigned int i,j;
  unsigned short temp;
 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
 		}

   
    //block mode 下寫入CGRAM
	/*
    while(1)
    {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);


		for(j=0; j<1900;j++)
		{	 
		  for(i=0; i<8;i++)	 //16*16/8/	 mcu16bit
		  {
		  temp = gImage_Font_8x16[i*2+1];	//寫入半型字,一次送16bit
		  temp <<= 8;
		  temp |= gImage_Font_8x16[i*2]; 		
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(1);	
	     }
		}


	  //while(1);
      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);


	  Foreground_color_65k(0xffff);
	  Background_color_65k(0x0000);

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();

	  Goto_Text_XY(0,0);
	  Text_Mode();
	  LCD_CmdWrite(0x04);
	while(1)
	{
	  for(i=0;i<1900;i++)
	  {
	   temp=i;					 //寫出全型字 字碼由0x0000

	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	  }	
	  Check_2D_Busy();
	  Graphic_Mode();
     
	   }
	 }
	
   */

  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<900;j++)
		{	 
		  for(i=0; i<8;i++)	 //	 16bp寫入	User_Font 為16x16
		  {
		  temp = gImage_Font_8x16[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_8x16[i*2]; 
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(2);	
	     }

		 for(i=0; i<8;i++)	 //	 16bp寫入	User_Font 為16x16
		  {
		  temp = gImage_Font_8x16[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_8x16[i*2]; 
		  LCD_DataWrite(~temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(2);	
	     }
		} 
   
	 /*
    Goto_Linear_Addr(960000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<1800;j++)
	 {	 
	 for(i=0; i<16;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(User_Font_Code[i*2]);		 	//寫入半型字 
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	   }
	  	*/ 

	  RGB_16b_16bpp(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_16bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1800;i++)
	   {
	   temp=i;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x0000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	   }
	   Check_2D_Busy();
	   Graphic_Mode();
	 
	 /*
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/
  	   }
   }
	
	    
}




void CGRAM_test_16bpp_FullFont_16x16(void)
{
  unsigned int i,j;
  unsigned short temp;
 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_red);
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
 		}

   
    //block mode 下寫入CGRAM
	
    while(1)
    {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);

	 
		for(j=0; j<750;j++)
		{	 
		  for(i=0; i<16;i++)	 //16*16/8/2	 mcu16bit
		  {
		  temp = gImage_Font_16x16[i*2+1];			//寫入全型字 
		  temp <<= 8;
		  temp |= gImage_Font_16x16[i*2]; 		
		  LCD_DataWrite(temp);
		   Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(1);	
	     }

		 for(i=0; i<16;i++)	 //16*16/8/2	 mcu16bit
		  {
		  temp = gImage_Font_16x16[i*2+1];			//寫入全型字 
		  temp <<= 8;
		  temp |= gImage_Font_16x16[i*2]; 		
		  LCD_DataWrite(~temp);
		   Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(1);	
	     }

		}


	  //while(1);
      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);


	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();

	  Goto_Text_XY(0,0);
	  Text_Mode();
	  LCD_CmdWrite(0x04);
	  for(i=0;i<1500;i++)
	  {
	   temp=i+32768;					 //寫出全型字 字碼由0x8000
	   
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();	   
	   //delay_ms(10);
	  }
	   Check_2D_Busy();
	  Graphic_Mode();
     
	  //while(1);
	 }



  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  /*
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1800;j++)
		{	 
		  for(i=0; i<16;i++)	 //16*16/8/2	 16bp寫入
		  {
		  temp = gImage_Font_16x16[i*2+1];	
		  temp <<= 8;
		  temp |= gImage_Font_16x16[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	     }
		} */
   

    Goto_Linear_Addr(960000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<1800;j++)
	 {	 
	 for(i=0; i<32;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_16x16[i]);		 	//寫入全型字 
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	   }
	  	 

	  RGB_16b_16bpp(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_16bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1800;i++)
	   {
	   temp=i+32768;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x8000
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(20);
	   }
	   Graphic_Mode();
	 

	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_red);
		 //Check_Busy_Mem_WR();
		  
 		}

  	   }
   }
	
	   
  
}

//===============================================================================

void CGRAM_test_16bpp_HalfFont_12x24(void)
{
  unsigned int i,j;
  unsigned short temp;
 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}

   
    //block mode 下寫入CGRAM
	/*
    while(1)
    {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);


		for(j=0; j<1500;j++)
		{	 
		  for(i=0; i<24;i++)	 //16*16/8/	 mcu16bit
		  {
		  temp = gImage_Font_12x24[i*2+1];	//寫入半型字,一次送16bit
		  temp <<= 8;
		  temp |= gImage_Font_12x24[i*2]; 		
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(1);	
	     }
		}


	  //while(1);
      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);


	  Foreground_color_65k(0xffff);
	  Background_color_65k(0x0000);

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();

	  Goto_Text_XY(0,0);
	  Text_Mode();
	  LCD_CmdWrite(0x04);
	while(1)
	{
	  for(i=0;i<1500;i++)
	  {
	   temp=i;					 //寫出全型字 字碼由0x0000

	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
     
	   }
	 }
	 */


  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<750;j++)
		{	 
		  for(i=0; i<24;i++)	 //	 16bp寫入	User_Font 為16x16
		  {
		  temp = gImage_Font_12x24[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_12x24[i*2]; 
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(2);	
	      }
		  for(i=0; i<24;i++)	 //	 16bp寫入	User_Font 為16x16
		  {
		  temp = gImage_Font_12x24[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_12x24[i*2]; 
		  LCD_DataWrite(~temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(2);	
	      }
		 } 
		
   	     

	 /*
    Goto_Linear_Addr(960000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<1800;j++)
	 {	 
	 for(i=0; i<16;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(User_Font_Code[i*2]);		 	//寫入半型字 
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	   }
	  	*/ 

	  RGB_16b_16bpp(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_16bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1500;i++)
	   {
	   temp=i;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x0000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	   }
		Check_2D_Busy();
	   Graphic_Mode();
	 
	 /*
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/
  	   }
  	}	    
}
//===============================================================================


//===============================================================================

void CGRAM_test_16bpp_FullFont_24x24(void)
{
  unsigned int i,j;
  unsigned short temp;
 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_red);
		 //Check_Busy_Mem_WR();
		  
 		}

   
    //block mode 下寫入CGRAM
	/*
    while(1)
    {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);

	 
		for(j=0; j<1500;j++)
		{	 
		  for(i=0; i<36;i++)	 //	 mcu16bit
		  {
		  temp = gImage_Font_24x24[i*2+1];			//寫入全型字 
		  temp <<= 8;
		  temp |= gImage_Font_24x24[i*2]; 		  
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(1);	
	     }
		}


	  //while(1);
      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);


	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);

       //Font_Select_8x16_16x16();
       Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();

	  Goto_Text_XY(0,0);
	  Text_Mode();
	  LCD_CmdWrite(0x04);
	  for(i=0;i<1500;i++)
	  {
	   temp=i+32768;					 //寫出全型字 字碼由0x8000

	   
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
     
	  //while(1);
	 }
	
	*/

  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  /*
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1800;j++)
		{	 
		  for(i=0; i<36;i++)	 //	 16bp寫入
		  {
		  temp = gImage_Font_24x24[i*2+1];		 
		  temp <<= 8;
		  temp |= UgImage_Font_24x24[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	     }
		} */
   

    Goto_Linear_Addr(960000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<300;j++)
	 {	 
	 for(i=0; i<72;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_24x24[i]);		 	//寫入全型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
		for(i=0; i<72;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_24x24[i]);		 	//寫入全型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }
	  	 

	  RGB_16b_16bpp(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_16bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<600;i++)
	   {
	   temp=i+32768;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x8000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(20);
	   }
	   Check_2D_Busy();
	   Graphic_Mode();
	 
	 /*
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_red);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/
  	   }
   }
	
	   
  
}


//===============================================================================

//===============================================================================

void CGRAM_test_16bpp_HalfFont_16x32(void)
{
  unsigned int i,j;
  unsigned short temp;
 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}

   
    //block mode 下寫入CGRAM
	/*
    while(1)
    {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);


		for(j=0; j<1500;j++)
		{	 
		  for(i=0; i<32;i++)	 //16*16/8/	 mcu16bit
		  {
		  temp = gImage_Font_16x32[i*2+1];	//寫入半型字,一次送16bit
		  temp <<= 8;
		  temp |= gImage_Font_16x32[i*2]; 		
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(1);	
	     }
		}


	  //while(1);
      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);


	  Foreground_color_65k(0xffff);
	  Background_color_65k(0x0000);

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();

	  Goto_Text_XY(0,0);
	  Text_Mode();
	  LCD_CmdWrite(0x04);
	while(1)
	{
	  for(i=0;i<1000;i++)
	  {
	   temp=i;					 //寫出全型字 字碼由0x0000

	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	   }
	   Graphic_Mode();
     
	   }
	  }
	 */


  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<500;j++)
		{	 
		  for(i=0; i<32;i++)	 //	 16bp寫入
		  {
		  temp = gImage_Font_16x32[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_16x32[i*2]; 
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(2);	
	      }

		  for(i=0; i<32;i++)	 //	 16bp寫入
		  {
		  temp = gImage_Font_16x32[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_16x32[i*2]; 
		  LCD_DataWrite(~temp);
		  Check_Mem_WR_FIFO_not_Full();
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  //delay_us(2);	
	      }

		 } 
		
   	     

	 /*
    Goto_Linear_Addr(960000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<1800;j++)
	 {	 
	 for(i=0; i<16;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(User_Font_Code[i*2]);		 	//寫入半型字 
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 delay_us(1);	
		}
	   }
	  	*/ 

	  RGB_16b_16bpp(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_16bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1000;i++)
	   {
	   temp=i;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x0000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	   }
		Check_2D_Busy();
	   Graphic_Mode();
	 
	 /*
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/
  	   }
  	}	    
}
//===============================================================================


//===============================================================================
//===============================================================================

void CGRAM_test_16bpp_FullFont_32x32(void)
{
  unsigned int i,j;
  unsigned short temp;
 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	//Text_cursor_initial();	//initaial and enable text cursor 


 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_red);
		 //Check_Busy_Mem_WR();
		  
 		}

   
    //block mode 下寫入CGRAM
	/*
    while(1)
    {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode

	 Canvas_Image_Start_address(960000);//
	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);

	 
		for(j=0; j<1000;j++)
		{	 
		  for(i=0; i<64;i++)	 //	 mcu16bit
		  {
		  temp = gImage_Font_32x32[i*2+1];			//寫入全型字 
		  temp <<= 8;
		  temp |= gImage_Font_32x32[i*2]; 		 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(1);	
	     }
		}


	  //while(1);
      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);


	  Foreground_color_65k(0xf800);
	  Background_color_65k(0x001f);

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();

	  Goto_Text_XY(0,0);
	  Text_Mode();
	  LCD_CmdWrite(0x04);
	  for(i=0;i<1000;i++)
	  {
	   temp=i+32768;					 //寫出全型字 字碼由0x8000

	   
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
     
	  //while(1);
	 }
	*/


  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  /*
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1800;j++)
		{	 
		  for(i=0; i<64;i++)	 //16*16/8/2	 16bp寫入
		  {
		  temp = gImage_Font_32x32[i*2+1];		   
		  temp <<= 8;
		  temp |= gImage_Font_32x32[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	     }
		} */
   

    Goto_Linear_Addr(960000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<700;j++)
	 {	 
	 for(i=0; i<128;i++)	 //		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_32x32[i]);		 	//寫入全型字 
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
			 for(i=0; i<128;i++)	 //		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_32x32[i]);		 	//寫入全型字 
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }
	  	 

	  RGB_16b_16bpp(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_16bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1000;i++)
	   {
	   temp=i+32768;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x8000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(20);
	   }
	   Check_2D_Busy();
	   Graphic_Mode();
	 
	  /*
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_red);
		 //Check_Busy_Mem_WR();
		  
 		} */

  	   }
   }
	
	   
  
}



//===============================================================================



void CGRAM_test_24bpp_mode1_HalfFont_8x16(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

    tempa = color16M_red;

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 

 		}

	 //Active_Window_WH(800,600);
     //Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 


  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(1440000);//	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(1440000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	 /* 
	 Goto_Linear_Addr(1440000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1800;j++)
		{	 
		  for(i=0; i<8;i++)	 //	 16bp寫入	User_Font 為16x16
		  {
		  temp = gImage_Font_8x16[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_8x16[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	     }
		} 
   	  */
	 
    Goto_Linear_Addr(1440000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<900;j++)
	 {	 
	 for(i=0; i<16;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_8x16[i]);		 	//寫入半型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}

		for(i=0; i<16;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_8x16[i]);		 	//寫入半型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }
	  	

	  RGB_16b_24bpp_mode1(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_24bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_16M(color16M_white);
	  Background_color_16M(color16M_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();

	   
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1800;i++)
	   {
	   temp=i;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x0000
	   Check_Mem_WR_FIFO_not_Full();

	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   }
		Check_2D_Busy();

	   Graphic_Mode();
	 
	 /*
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/
  	   }
   }
	
	    
}




void CGRAM_test_24bpp_mode1_FullFont_16x16(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

      tempa = color16M_red;

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();

		 //Check_Busy_Mem_WR();
		 // 

 		}

        LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 
  
  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(1440000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  /*
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1800;j++)
		{	 
		  for(i=0; i<16;i++)	 //16*16/8/2	 16bp寫入
		  {
		  temp = gImage_Font_16x16[i*2+1];	
		  temp <<= 8;
		  temp |= gImage_Font_16x16[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	     }
		} */
   

    Goto_Linear_Addr(1440000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<1000;j++)
	 {	 
	 for(i=0; i<32;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_16x16[i]);		 	//寫入全型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(10);
		 //delay_us(1);	
		}
			 for(i=0; i<32;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_16x16[i]);		 	//寫入全型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(10);
		 //delay_us(1);	
		}
	   }
	  	 

	  RGB_16b_24bpp_mode1(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_24bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_16M(color16M_white);
	  Background_color_16M(color16M_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1800;i++)
	   {
	   temp=i+32768;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x8000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	  
	  // delay_ms(50);
	   }
	    Check_2D_Busy();
	   Graphic_Mode();
	 
		/*
	 Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 
		 //Check_Busy_Mem_WR();
		 // 

 		}
		*/
  	   }
   }
	
	   
  
}

//===============================================================================

void CGRAM_test_24bpp_mode1_HalfFont_12x24(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

	tempa = color16M_red;

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
   	for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);
		 Check_Mem_WR_FIFO_not_Full();

		 //Check_Busy_Mem_WR();
		 // 

 		}

     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 
   

  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(1440000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	 /* 
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1500;j++)
		{	 
		  for(i=0; i<24;i++)	 //	 16bp寫入	User_Font 為16x16
		  {
		  temp = gImage_Font_12x24[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_12x24[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	      }
		 } 
		*/
   	     
    Goto_Linear_Addr(1440000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<900;j++)
	 {	 
	 for(i=0; i<48;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_12x24[i]);		 	//寫入半型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
		 for(i=0; i<48;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_12x24[i]);		 	//寫入半型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }
	   

	  RGB_16b_24bpp_mode1(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_24bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_16M(color16M_white);
	  Background_color_16M(color16M_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1500;i++)
	   {
	   temp=i;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x0000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(50);
	   }
		Check_2D_Busy();
	   Graphic_Mode();
	  /*
 	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
   	 for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		}
		 */

  	   }
  	}	    
}
//===============================================================================


//===============================================================================

void CGRAM_test_24bpp_mode1_FullFont_24x24(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

		tempa = color16M_red;

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


 	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
   	 for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		}


	 LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 

  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(1440000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  /*
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1800;j++)
		{	 
		  for(i=0; i<36;i++)	 //	 16bp寫入
		  {
		  temp = gImage_Font_24x24[i*2+1];		 
		  temp <<= 8;
		  temp |= UgImage_Font_24x24[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	     }
		} */
   

    Goto_Linear_Addr(1440000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<750;j++)
	 {	 
	 for(i=0; i<72;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_24x24[i]);		 	//寫入全型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
			 for(i=0; i<72;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_24x24[i]);		 	//寫入全型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }
	  	 

	  RGB_16b_24bpp_mode1(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_24bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_16M(color16M_white);
	  Background_color_16M(color16M_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1500;i++)
	   {
	   temp=i+32768;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x8000
	    Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	    Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(50);
	   }
	   Check_2D_Busy();
	   Graphic_Mode();
	  /*
 	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
   	 for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		}
		 */

  	   }
   }
	
	   
  
}


//===============================================================================

//===============================================================================

void CGRAM_test_24bpp_mode1_HalfFont_16x32(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

    tempa = color16M_red;

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


  	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
   	 for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		}

     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 

  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(1440000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	 /* 
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1000;j++)
		{	 
		  for(i=0; i<32;i++)	 //	 16bp寫入
		  {
		  temp = gImage_Font_16x32[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_16x32[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	      }
		 } 
		 */
   	     

	 
    Goto_Linear_Addr(1440000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<700;j++)
	 {	 
	 for(i=0; i<64;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_16x32[i]);		 	//寫入半型字 
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}

			 for(i=0; i<64;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_16x32[i]);		 	//寫入半型字 
		  Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}

	   }
	  	 

	  RGB_16b_24bpp_mode1(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_24bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_16M(color16M_white);
	  Background_color_16M(color16M_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1400;i++)
	   {
	   temp=i;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x0000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	   }
	   Check_2D_Busy();
	   Graphic_Mode();
	  /*
 	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
   	 for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		} */


  	   }
  	}	    
}
//===============================================================================


//===============================================================================
//===============================================================================

void CGRAM_test_24bpp_mode1_FullFont_32x32(void)
{
  unsigned int i,j,tempa;
  unsigned short temp;

	tempa = color16M_red;

 	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 

  //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
  while(1)
   {
     CGRAM_Start_address(1440000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 //Canvas_Image_Start_address(960000);//
	 //Active_Window_XY(0,0);
	 //Active_Window_WH(800,600);
	  /*
	 Goto_Linear_Addr(960000);
	   LCD_CmdWrite(0x04);
		for(j=0; j<1800;j++)
		{	 
		  for(i=0; i<64;i++)	 //16*16/8/2	 16bp寫入
		  {
		  temp = gImage_Font_32x32[i*2+1];		   
		  temp <<= 8;
		  temp |= gImage_Font_32x32[i*2]; 
		  LCD_DataWrite(temp);
		  //Check_Busy_Mem_WR();
		  // 
		  //delay_ms(1);
		  delay_us(2);	
	     }
		} */
   

    Goto_Linear_Addr(1440000);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<500;j++)
	 {	 
	 for(i=0; i<128;i++)	 //		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_32x32[i]);		 	//寫入全型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
			 for(i=0; i<128;i++)	 //		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_32x32[i]);		 	//寫入全型字 
		 Check_Mem_WR_FIFO_not_Full();
		 //Check_Busy_Mem_WR();
		 // 
		 //delay_ms(1);
		 //delay_us(1);	
		}
	   }
	  	 

	  RGB_16b_24bpp_mode1(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_24bpp_Mode();//切換為原始的色深	


      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_16M(color16M_white);
	  Background_color_16M(color16M_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	   Goto_Text_XY(0,0);
	   Text_Mode();
	   LCD_CmdWrite(0x04);
	   for(i=0;i<1000;i++)
	   {
	   temp=i+32768;
 
	   LCD_DataWrite(temp>>8);			   //寫出全型字 字碼由0x8000
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(20);
	   }
	   Check_2D_Busy();
	   Graphic_Mode();
	 /*
     Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
   	 for(i=0; i<(480000/2);i++) //每次寫入兩個pixels
		{
		 temp = (tempa);		 				
		 LCD_DataWrite(temp);
		 temp = (tempa>>8);
		 temp &= 0x00ff;
		 temp |= (tempa<<8); 
		 LCD_DataWrite(temp);
		 temp =  (tempa>>8);
		 LCD_DataWrite(temp);

		 //Check_Busy_Mem_WR();
		  

 		}
		 */

  	   }
   }
	
	   
  
}






/*使用DAM FLASH來initial,建議採用linear mode*/
//=======================================================================

/*REG[CCh][DBh][DEh]*/

void DMA_Init_CGRAM_test_8bpp_HalfFont_8x16(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	//Graphic_cursor_initial(); //initaial and enable graphic cursor 
	//Text_cursor_initial();	//initaial and enable text cursor 


	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
  

Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);

	  
   //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
   //半形
   while(1)
   {

     CGRAM_Start_address(240000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關


	SFI_DMA_Destination_Start_Address(240000);
    SFI_DMA_Transfer_Number(16);//for linear mode
	SFI_DMA_Source_Start_Address(12441600);//24bit address

	Goto_Linear_Addr(240000);

	//Start_SFI_DMA();
    //Check_Busy_SFI_DMA(); 
					 
	for(j=0; j<1500;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(240000+j*16);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 
		 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1500;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	  }
	  Check_2D_Busy();
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }

}


void DMA_Init_CGRAM_test_8bpp_FullFont_16x16(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();
    

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
   
Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);


    
	//Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
   //全形
   while(1)
   {

     CGRAM_Start_address(240000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關


	 SFI_DMA_Destination_Start_Address(240000);
     SFI_DMA_Transfer_Number(32);//for linear mode
	 SFI_DMA_Source_Start_Address(12441616);//24bit address

	 Goto_Linear_Addr(240000);

	 LCD_CmdWrite(0x04);					  //寫入全型字
	for(j=0; j<500;j++)
	 {	 

	     SFI_DMA_Destination_Start_Address(240000+j*32);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 

	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<500;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	  }
	  Check_2D_Busy();
	  Graphic_Mode();

  	   }
     }


}


void DMA_Init_CGRAM_test_8bpp_HalfFont_12x24(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}

Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);

    


   //半形
   while(1)
   {
     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
     CGRAM_Start_address(480000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 SFI_DMA_Destination_Start_Address(480000);
     SFI_DMA_Transfer_Number(48);//for linear mode
	 SFI_DMA_Source_Start_Address(12441648);//24bit address

	 Goto_Linear_Addr(480000);
					 
	for(j=0; j<1500;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(480000+j*48);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1500;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	   //delay_ms(10);
	  }//
	  Check_2D_Busy();
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }

}


void DMA_Init_CGRAM_test_8bpp_FullFont_24x24(void)
{
 unsigned int i,j,temp;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
 
 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);

  
    //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)  
   //全形
   while(1)
   {

     CGRAM_Start_address(480000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 SFI_DMA_Destination_Start_Address(480000);
     SFI_DMA_Transfer_Number(72);//for linear mode
	 SFI_DMA_Source_Start_Address(12441696);//24bit address
	 Goto_Linear_Addr(480000);

					  
	for(j=0; j<500;j++)	 //寫入全型字
	 {	 
	     SFI_DMA_Destination_Start_Address(480000+j*72);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();

     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	    LCD_CmdWrite(0x04);
	  for(i=0;i<500;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;
	 
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	  
	   delay_ms(10);
	  }
	   Check_2D_Busy();
	  Graphic_Mode();
	

  	   }
     }


}


void DMA_Init_CGRAM_test_8bpp_HalfFont_16x32(void)
{
 unsigned int i,j,temp;

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}

 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);
    

   //半形
   while(1)
   {

     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(480000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 SFI_DMA_Destination_Start_Address(480000);
     SFI_DMA_Transfer_Number(64);//for linear mode
	 SFI_DMA_Source_Start_Address(12441768);//24bit address

	 Goto_Linear_Addr(480000);
				 
	for(j=0; j<1500;j++)
	 {	 

	     SFI_DMA_Destination_Start_Address(480000+j*64);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 		
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1500;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	   
	  // delay_ms(10);
	  }
	  Check_2D_Busy();
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }

}




void DMA_Init_CGRAM_test_8bpp_FullFont_32x32(void)
{
 unsigned int i,j,temp;

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}

 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);
   
   //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)	  
   //全形
   while(1)
   {

     CGRAM_Start_address(480000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 SFI_DMA_Destination_Start_Address(480000);
     SFI_DMA_Transfer_Number(128);//for linear mode
	 SFI_DMA_Source_Start_Address(12441832);//24bit address

	 Goto_Linear_Addr(480000);

	for(j=0; j<500;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(480000+j*128);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 		
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_256(color256_white);
	  Background_color_256(color256_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<500;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 
  	   }
     }


}

//===============================================================================

void DMA_Init_CGRAM_test_16bpp_HalfFont_8x16(void)
{
 unsigned int i,j,temp;

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
  

Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);

	  
   //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
   //半形
   while(1)
   {

     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關


	SFI_DMA_Destination_Start_Address(960000);
    SFI_DMA_Transfer_Number(16);//for linear mode
	SFI_DMA_Source_Start_Address(12441600);//24bit address

	Goto_Linear_Addr(960000);

	//Start_SFI_DMA();
    //Check_Busy_SFI_DMA(); 
					 
	for(j=0; j<1500;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(960000+j*16);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 
		 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1500;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 
  	   }
     }

}


void DMA_Init_CGRAM_test_16bpp_FullFont_16x16(void)
{
 unsigned int i,j,temp;


	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
   
Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

//SPI_Clock_Period(5);//設定為5的時候沒有clk

SPI_Clock_Period(2);

    
	//Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
   //全形
   while(1)
   {

     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關


	 SFI_DMA_Destination_Start_Address(960000);
     SFI_DMA_Transfer_Number(32);//for linear mode
	 SFI_DMA_Source_Start_Address(12441616);//24bit address

	 Goto_Linear_Addr(960000);

	 LCD_CmdWrite(0x04);					  //寫入全型字
	for(j=0; j<500;j++)
	 {	 

	     SFI_DMA_Destination_Start_Address(960000+j*32);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 

	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<500;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();

  	   }
     }


}


void DMA_Init_CGRAM_test_16bpp_HalfFont_12x24(void)
{
 unsigned int i,j,temp;

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
		 //Check_Busy_Mem_WR();
		  
 		}

Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);

    


   //半形
   while(1)
   {
     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)
     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 SFI_DMA_Destination_Start_Address(960000);
     SFI_DMA_Transfer_Number(48);//for linear mode
	 SFI_DMA_Source_Start_Address(12441648);//24bit address

	 Goto_Linear_Addr(960000);
					 
	for(j=0; j<1500;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(960000+j*48);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1500;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 

	   /*
	   Active_Window_WH(800,600);
       Goto_Pixel_XY(0,0);
	   LCD_CmdWrite(0x04);
  	   for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		 //Check_Busy_Mem_WR();
		  
 		}
		*/

  	   }
     }

}


void DMA_Init_CGRAM_test_16bpp_FullFont_24x24(void)
{
 unsigned int i,j,temp;

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
	
 		}
 
 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);

  
    //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)  
   //全形
   while(1)
   {

     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 SFI_DMA_Destination_Start_Address(960000);
     SFI_DMA_Transfer_Number(72);//for linear mode
	 SFI_DMA_Source_Start_Address(12441696);//24bit address
	 Goto_Linear_Addr(960000);

					  
	for(j=0; j<500;j++)	 //寫入全型字
	 {	 
	     SFI_DMA_Destination_Start_Address(960000+j*72);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 	
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       //Font_Select_8x16_16x16();
      Font_Select_12x24_24x24();
       //Font_Select_16x32_32x32();

     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	    LCD_CmdWrite(0x04);
	  for(i=0;i<500;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;

	 
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	

  	   }
     }


}


void DMA_Init_CGRAM_test_16bpp_HalfFont_16x32(void)
{
 unsigned int i,j,temp;

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color65k_blue);
	
 		}

 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(5);
    

   //半形
   while(1)
   {

     //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 SFI_DMA_Destination_Start_Address(960000);
     SFI_DMA_Transfer_Number(64);//for linear mode
	 SFI_DMA_Source_Start_Address(12441768);//24bit address

	 Goto_Linear_Addr(960000);
				 
	for(j=0; j<1500;j++)
	 {	 

	     SFI_DMA_Destination_Start_Address(960000+j*64);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 		
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       //Font_Select_8x16_16x16();
      //Font_Select_12x24_24x24();
       Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<1500;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 

  	   }
     }

}




void DMA_Init_CGRAM_test_16bpp_FullFont_32x32(void)
{
 unsigned int i,j,temp;

	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(color256_blue);
		
 		}

 Enable_SFlash_SPI();
Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);
   
   //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)	  
   //全形
   while(1)
   {

     CGRAM_Start_address(960000);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 SFI_DMA_Destination_Start_Address(960000);
     SFI_DMA_Transfer_Number(128);//for linear mode
	 SFI_DMA_Source_Start_Address(12441832);//24bit address

	 Goto_Linear_Addr(960000);

	for(j=0; j<500;j++)
	 {	 
	     SFI_DMA_Destination_Start_Address(960000+j*128);
	     Start_SFI_DMA();
         Check_Busy_SFI_DMA(); 		
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode

      Font_Select_UserDefine_Mode(); 
	  Canvas_Image_Start_address(0);
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);
	  Goto_Pixel_XY(0,0);

	  Foreground_color_65k(color65k_white);
	  Background_color_65k(color65k_black);
	  

       Font_Select_16x32_32x32();
     while(1)
     {
	  Goto_Text_XY(0,0);
	  Text_Mode();	
	  for(i=0;i<500;i++)
	  {										 //寫出全型字 字碼由0x8000
	   temp=i+32768;

	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   LCD_DataWrite(temp);
	   //Check_2D_Busy();
	   delay_ms(10);
	  }
	  Graphic_Mode();
	 
  	   }
     }


}


//=======================================================================

/* REG[DFh] with display off function test*/ 
void Power_saving_test_standby_mode_16bpp(void)
{
 unsigned int i,j;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 
	 
    Idle_DE_Low(); 
    Idle_PCLK_Low();  
    Idle_PDAT_Low();
    Idle_HSYNC_Low();   
    Idle_VSYNC_Low();
	/*
    Idle_DE_High();
    Idle_PCLK_High();
    Idle_PDAT_High();
    Idle_HSYNC_High();
    Idle_VSYNC_High();
	*/
	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
    sd_showpic_65k_bin(800,600,"p2.bin");//
    delay_seconds(3);

   while(1)
   {
	 Power_Saving_Standby_Mode();  //
     delay_seconds(3);

		 if((LCD_StatusRead()&0x02)==0x02)
	  { j = LCD_StatusRead();
	    printf("j=%d",j);

		LCD_CmdWrite(0xdf);
		
		 if((LCD_DataRead()&0x80)==0x80)
		 {
		   j=LCD_DataRead();
		   printf("j=%d",j);

	     Power_Normal_Mode();
	     delay_ms(10);
	     Display_ON();
	     delay_seconds(3);
		 }
		 else
		 {
	      printf("error2");
	       while(1);
		 }
	  }
	  else
	  {
	   printf("error1");
	   while(1);
	  }
   	 /*
    Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);
		 //delay_us(1);
		 //Check_Busy_Mem_WR();
		
 		}  */

	//delay_seconds(3);
//	Display_OFF();				//display off 
    //delay_ms(20);
	//delay_seconds(1);			//


	 /*
	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);
	
 		}
	 delay_seconds(3);*/
	 

     }
 }


void Power_saving_test_standby_mode_24bpp(void)
{
 unsigned int i,j;

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 
	 
    Idle_DE_Low(); 
    Idle_PCLK_Low();  
    Idle_PDAT_Low();
    Idle_HSYNC_Low();   
    Idle_VSYNC_Low();
	/*
    Idle_DE_High();
    Idle_PCLK_High();
    Idle_PDAT_High();
    Idle_HSYNC_High();
    Idle_VSYNC_High();
	*/

   	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();

   while(1)
   {
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_16M_mode1_bin(800,600,"p3.bin");
    Check_Mem_WR_FIFO_Empty();

	delay_seconds(3);
    //Display_OFF();			//display off 
    delay_ms(20);
	//delay_seconds(1);			//
    Power_Saving_Standby_Mode();//

    delay_seconds(3);

	 if((LCD_StatusRead()&0x02)==0x02)
	  { j = LCD_StatusRead();
	    printf("j=%d",j);

		LCD_CmdWrite(0xdf);
		
		 if((LCD_DataRead()&0x80)==0x80)
		 {
		   j=LCD_DataRead();
		   printf("j=%d",j);

	     Power_Normal_Mode();
	     delay_ms(10);
	     Display_ON();
	     delay_seconds(3);
		 }
		 else
		 {
	      printf("error2");
	       while(1);
		 }
	  }
	  else
	  {
	   printf("error1");
	   while(1);
	  }
	 
	 
     }
 }


void Power_saving_test_suspend_mode_16bpp(void)
{
    unsigned int i,j;
	 
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

//    Idle_DE_Low(); 
//    Idle_PCLK_Low();  
//    Idle_PDAT_Low();
//    Idle_HSYNC_Low();   
//    Idle_VSYNC_Low();

    Idle_DE_High();
    Idle_PCLK_High();
    Idle_PDAT_High();
    Idle_HSYNC_High();
    Idle_VSYNC_High();

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
    sd_showpic_65k_bin(800,600,"p2.bin");//
    delay_seconds(3);

	//進入suspend mode之前必須調整SDRAM refresh time
	 /* */
    while(1)
    {

//	 Display_OFF();				//display off 
//      delay_ms(1000);	 
//      Power_Saving_Suspend_Mode();
//			delay_ms(1000);	
//	        Power_Normal_Mode();
//	     delay_ms(1000);
//	     Display_ON();
//	     delay_seconds(3);


	  Display_OFF();				//display off 
      delay_ms(20);	 
      Power_Saving_Suspend_Mode();
	  delay_ms(10);
	   
	   //FSMC_LCD_Init_L();

	  if((LCD_StatusRead()&0x02)==0x02)
	  { j = LCD_StatusRead();
	    printf("j=%d",j);

		LCD_CmdWrite(0xdf);
		
		 if((LCD_DataRead()&0x80)==0x80)
		 {
		   j=LCD_DataRead();
		   printf("j=%d",j);

	     Power_Normal_Mode();	  
	     delay_ms(10);
		  //FSMC_LCD_Init_H();
	     Display_ON();
	     delay_seconds(3);
		 }
		 else
		 {
	      printf("error2");
	       while(1);
		 }
	  }
	  else
	  {
	   printf("error1");
	   while(1);
	  }


      }

}


void Power_saving_test_suspend_mode_24bpp(void)
{
    unsigned int i,j;
	 
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

    Idle_DE_Low(); 
    Idle_PCLK_Low();  
    Idle_PDAT_Low();
    Idle_HSYNC_Low();   
    Idle_VSYNC_Low();
	/*
    Idle_DE_High();
    Idle_PCLK_High();
    Idle_PDAT_High();
    Idle_HSYNC_High();
    Idle_VSYNC_High();
	*/
   	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();

	//進入suspend mode之前必須調整SDRAM refresh time
	 /* */
    while(1)
    { 

//     Goto_Pixel_XY(0,0);
//	 LCD_CmdWrite(0x04);
//	 sd_showpic_16M_mode1_bin(800,600,"p3.bin");
//     Check_Mem_WR_FIFO_Empty();
//
//	  //Display_OFF();				//display off 
//      delay_ms(20);	 
//       Power_Saving_Suspend_Mode();
//
//	   delay_seconds(5);
//       Power_Normal_Mode();
//	   delay_ms(10);
//	   //Display_ON();
//	   delay_seconds(5);
//




	  //Display_OFF();				//display off 
      delay_ms(20);	 
      Power_Saving_Suspend_Mode();
	  delay_ms(10);
	   delay_seconds(10);
	  if((LCD_StatusRead()&0x02)==0x02)
	  { j = LCD_StatusRead();
	    printf("j=%d",j);

		LCD_CmdWrite(0xdf);
		
		 if((LCD_DataRead()&0x80)==0x80)
		 {
		   j=LCD_DataRead();
		   printf("j=%d",j);

	     Power_Normal_Mode();	  
	     delay_ms(10);

	     //Display_ON();
	     delay_seconds(3);
		 }
		 else
		 {
	      printf("error2");
	       while(1);
		 }
	  }
	  else
	  {
	   printf("error1");
	   while(1);
	  }


      }

}


void Power_saving_test_sleep_mode_16bpp(void)
{
     unsigned int i,j;
	 
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


    Idle_DE_Low(); 
    Idle_PCLK_Low();  
    Idle_PDAT_Low();
    Idle_HSYNC_Low();   
    Idle_VSYNC_Low();
	/*
    Idle_DE_High();
    Idle_PCLK_High();
    Idle_PDAT_High();
    Idle_HSYNC_High();
    Idle_VSYNC_High();
	*/
	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
    sd_showpic_65k_bin(800,600,"p2.bin");//
    delay_seconds(3);

    while(1)
    {
//   	Display_OFF();				//display off 
//      delay_ms(20);	 	 
//      Power_Saving_Sleep_Mode();
//      delay_seconds(10);
//      Power_Normal_Mode();
//      delay_ms(10);
//	  Display_ON();
//	  delay_seconds(10);


	  delay_ms(20);	 
      Power_Saving_Sleep_Mode();
	  delay_ms(10);
	   delay_seconds(20);
	  if((LCD_StatusRead()&0x02)==0x02)
	  { j = LCD_StatusRead();
	    printf("j=%d",j);

		LCD_CmdWrite(0xdf);
		
		 if((LCD_DataRead()&0x80)==0x80)
		 {
		   j=LCD_DataRead();
		   printf("j=%d",j);

	     Power_Normal_Mode();	  
	     delay_ms(10);

	     //Display_ON();
	     delay_seconds(5);
		 }
		 else
		 {
	      printf("error2");
	       while(1);
		 }
	  }
	  else
	  {
	   printf("error1");
	   while(1);
	  }


    }

 }


void Power_saving_test_sleep_mode_24bpp(void)
{
     unsigned int i;
	 
	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


    Idle_DE_Low(); 
    Idle_PCLK_Low();  
    Idle_PDAT_Low();
    Idle_HSYNC_Low();   
    Idle_VSYNC_Low();
	/*
    Idle_DE_High();
    Idle_PCLK_High();
    Idle_PDAT_High();
    Idle_HSYNC_High();
    Idle_VSYNC_High();
	*/


   	Main_Image_Start_Address(0);					
	Main_Image_Width(8188);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Active_Window_WH(800,600);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_16M_mode1_bin(800,600,"p3.bin");
   Check_Mem_WR_FIFO_Empty();

    while(1)
    {

	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin");
     Check_Mem_WR_FIFO_Empty();

   	  Display_OFF();				//display off 
      delay_ms(20);	 	 
      Power_Saving_Sleep_Mode();
      delay_seconds(5);
      Power_Normal_Mode();
      delay_ms(10);
	  Display_ON();
	  delay_seconds(5);
    }

 }



//=================================================================================
/*FOR RA8876 I2CM 專用函式*/
/******************************************************************************
I2C_Delay()
******************************************************************************/
void RA8876_I2CM_Delay(void)
{
	unsigned char t;
	t = 100;
	while ( --t != 0 );		//延時2*t個機器週期
}


/******************************************************************************
I2C_Init()
******************************************************************************/
void RA8876_I2CM_Init(void)
{
  //Enable_I2CM_Interrupt();
  RA8876_I2CM_Enable();
 //RA8876_I2CM_Clock_Prescale(99);//cclk=50MHZ,I2CM_SCK=50MHZ/(5*(99+1))=100kHZ
  RA8876_I2CM_Clock_Prescale(159);//cclk=80MHZ,I2CM_SCK=80MHZ/(5*(159+1))=kHZ
}



/******************************************************************************
函數：RA8876_I2CM_Puts()
功能：主機通過I2C總線向從機發送多個字節的數據
參數：
	SlaveAddr：從機地址（高7位是從機地址，最低位是讀寫標誌）
	SubAddr：從機的子地址
	size：數據大小（以字節計）
	*dat：要發送的數據
返回：
	0：發送成功
	1：在發送過程中出現異常
******************************************************************************/
unsigned char RA8876_I2CM_Puts(
				       unsigned char SlaveAddr,
				       unsigned char SubAddr,
				       unsigned char size,
				       unsigned char *dat
			          )
{


  
	SlaveAddr &= 0xFE;//確保從機地址最低位是0
	
	RA8876_I2CM_Transmit_Data(SlaveAddr);	//輸入從機地址
	RA8876_I2CM_Write_With_Start();//開始起始發送  
	while(RA8876_I2CM_transmit_Progress());//檢查傳送進度
	
	if (RA8876_I2CM_Check_Slave_ACK()) //檢查從機ACK
	{
		RA8876_I2CM_Stop();
		return 1;
	} 
   	
    RA8876_I2CM_Transmit_Data(SubAddr);	//發送子地址
	RA8876_I2CM_Write();//發送

	while(RA8876_I2CM_transmit_Progress());//檢查傳送進度
		 
	if (RA8876_I2CM_Check_Slave_ACK()) //檢查從機ACK
	{
		RA8876_I2CM_Stop();
		return 1;
	}	
	 //GPIO_SetBits(GPIOE, GPIO_Pin_6);		//test pin 

	//發送數據
	do	  
	{
	    RA8876_I2CM_Transmit_Data(*dat++);
	    RA8876_I2CM_Write();//發送 
	    while(RA8876_I2CM_transmit_Progress());//檢查傳送進度

		 
	    if (RA8876_I2CM_Check_Slave_ACK()) //檢查從機ACK
	     {
		 RA8876_I2CM_Stop();
		 return 1;
	     } 

		 RA8876_I2CM_Delay();

	} while ( --size != 0 );

	RA8876_I2CM_Stop();	 //發送完畢，停止I2C總線，返回
	return 0;
	
}


/******************************************************************************
      RA8876_I2CM_Gets()
      主機通過I2C總線從從機接收個字節的數據

	SlaveAddr：從機地址（高7位是從機地址，最低位是讀寫標誌）
	SubAddr：從機的子地址
	size：數據大小（以字節計）
	*dat：保存接收到的數據
返回：
	0：接收成功
	1：在接收過程中出現異常
******************************************************************************/
unsigned char RA8876_I2CM_Gets(
				       unsigned char SlaveAddr,
				       unsigned char SubAddr,
				       unsigned char size,
				       unsigned char *dat
			          )
{


	SlaveAddr &= 0xFE;   //確保從機地址最低位是0
 	 
	RA8876_I2CM_Transmit_Data(SlaveAddr);	//輸入從機地址
	RA8876_I2CM_Write_With_Start();//開始起始發送

	while(RA8876_I2CM_transmit_Progress());//檢查傳送進度
	
	if (RA8876_I2CM_Check_Slave_ACK()) //檢查從機ACK
	{
		RA8876_I2CM_Stop();
		return 1;
	}  
   	

    RA8876_I2CM_Transmit_Data(SubAddr);	//發送子地址
	RA8876_I2CM_Write();//發送
  
	while(RA8876_I2CM_transmit_Progress());//檢查傳送進度
	
	if (RA8876_I2CM_Check_Slave_ACK()) //檢查從機ACK
	{
		RA8876_I2CM_Stop();
		return 1;
	}  
	
	SlaveAddr |= 0x01;   //從機地址
    RA8876_I2CM_Transmit_Data(SlaveAddr);//
	RA8876_I2CM_Write_With_Start();//發送重複起始條件 //
   
	while(RA8876_I2CM_transmit_Progress());//檢查傳送進度
	
	if (RA8876_I2CM_Check_Slave_ACK()) //檢查從機ACK
	{
		RA8876_I2CM_Stop();
		return 1;
	}  
	 
    //接收數據
    for (;;)
    {

		if ( --size != 0 )
		{
		 RA8876_I2CM_Read_With_Ack();//開始讀,自動產生應答
		 while(RA8876_I2CM_transmit_Progress()); //檢查傳送進度
		 *dat++ = RA8876_I2CM_Receiver_Data();//數據放到指標
		}
		else
		{
		 RA8876_I2CM_Read_With_Nack();	//主機收到最後一個字節產生非應答,接收完畢，停止I2C總線，返回
		 while(RA8876_I2CM_transmit_Progress());//檢查傳送進度
		 *dat++ = RA8876_I2CM_Receiver_Data();//數據放到指標
		 break;
		}  
 
	}
	RA8876_I2CM_Stop();
	return 0;
}

//=================================================================================
/*REG[E5h~EAh]*/
void RA8876_I2C_Master_test(void)
{
  unsigned int i;
  unsigned char temp;


	//Enable_I2CM_Interrupt();
	//Mask_I2CM_Interrupt_Flag();


	 
     RA8876_I2CM_Init();
	 RA8876_I2CM_Stop(); //for MCU reset setting to stop I2C device

     Goto_Text_XY(0,0);
     Show_String("I2C Master test");


	//====write data to 24c02======
	temp = 	RA8876_I2CM_Puts(0xa0,0x00,5,iic_test1);

	 	// while((Read_Interrupt_status()&0x20)==0x00);
	if(temp==1)
	{
	 Goto_Text_XY(0,20);
     Show_String("write ack error");
	 while(1);
	}
   	else if(temp==2)
	 {
	 Goto_Text_XY(0,20);
     Show_String("write over time error");
	 while(1);
	}
		
	delay_ms(100);
    //=============================

   while(1)
 {

    if(RA8876_I2CM_Gets(0xa0,0x00,5,tempaa)==1)
	 {
	  Goto_Text_XY(0,40);
      Show_String("read error");
	  while(1);
	 } 


     //delay_us(5);
	 Goto_Text_XY(0,60);	
	 Text_Mode();
	 LCD_CmdWrite(0x04);
	 for(i=0;i<5;i++)
	 {
	  LCD_DataWrite(tempaa[i]);
	   Check_Mem_WR_FIFO_not_Full();
	   //delay_ms(10);
	  }
	  Check_2D_Busy();
    //delay_ms(1000);


	//for test I2CM Interrupt
     if((Read_Interrupt_status()&0x20)==0x20) 
	 {
	  Clear_I2CM_Interrupt_Flag();
	  GPIO_SetBits(GPIOC, GPIO_Pin_1);
	   delay_us(1);
	  if((Read_Interrupt_status()&0x20)==0x00) 
	  GPIO_ResetBits(GPIOC, GPIO_Pin_1);
	 }
	
	 /*	 
     RA8876_I2CM_Gets(0xa0,0x0a,4,tempaa);

	 Goto_Text_XY(0,80);
	 Text_Mode();
	 LCD_CmdWrite(0x04);
	 for(i=0;i<4;i++)
	 {
	   LCD_DataWrite(tempaa[i]);
	   Check_2D_Busy();
	  }
	 */
	 /*
      delay_ms(1000);

	  Goto_Pixel_XY(0,0);
	  Graphic_Mode(); //back to graphic mode
	  LCD_CmdWrite(0x04);
	  for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x07e0);

		 //delay_us(1);	
		 //Check_Busy_Mem_WR();
	
 		}
	   delay_ms(1000);
	   */
  }	 
  
}



//=======================================================================

/*REG[FAh][FBh][FCh][FDh][FEh][FFh]*/
/*key scan test for digital panel package*/
//需切換LCD driveing 信號到key scan 
void key_scan_test_5x5(void)
{
  unsigned char tempa,tempb;
  
	Graphic_cursor_initial(); //initaial and enable graphic cursor 

    Interrupt_Active_Low();
	//Interrupt_Active_High();
    Enable_KeyScan_Interrupt();
    //Disable_KeyScan_Interrupt();
	//Mask_KeyScan_Interrupt_Flag();


   //TFT_16bit();	//設定LCD driveing 使用16bit 565
    TFT_18bit();
  //TFT_24bit();

   //Display_OFF();
   delay_ms(20);
   /*
   //XPDAT18_Set_GPIO_D7();
   XPDAT18_Set_KOUT4();
   //XPDAT17_Set_GPIO_D5();
   XPDAT17_Set_KOUT2();
   //XPDAT16_Set_GPIO_D4();
   XPDAT16_Set_KOUT1();
   //XPDAT9_Set_GPIO_D3();
   XPDAT9_Set_KOUT3();
   //XPDAT8_Set_GPIO_D2();
   XPDAT8_Set_KIN3();
   //XPDAT2_Set_GPIO_D6();
   XPDAT2_Set_KIN4();
   //XPDAT1_Set_GPIO_D1();
   XPDAT1_Set_KIN2();
   //XPDAT0_Set_GPIO_D0();
   XPDAT0_Set_KIN1();
	*/
   LCD_CmdWrite(0x0F);
   LCD_DataWrite(0xff);

   Long_Key_Timing_Adjustment(0x0c);

   Key_Scan_Freg(0x07);
   //Key_Scan_Wakeup_Function_Enable();
   Key_Scan_Enable();

    LCD_CmdWrite(0x01);
   LCD_DataWrite(0xa1);
	 
//   LCD_CmdWrite(0x0D);
//   LCD_DataWrite(0x00);

  // Text_Mode();

    while(1)
	{  	    
	  Clear_KeyScan_Interrupt_Flag();
	  //delay_ms(1);
	  delay_us(1);
      if((Read_Interrupt_status()&0x08)==0x08)
	   {
		  GPIO_SetBits(GPIOC, GPIO_Pin_1);
	      delay_us(1);
	      GPIO_ResetBits(GPIOC, GPIO_Pin_1);

		 tempa = Numbers_of_Key_Hit();

		  Foreground_color_65k(0xf800);

		 Goto_Text_XY(200,0);
	     LCD_CmdWrite(0x04);	     
	     Print_Hex(tempa);

           switch(tempa)
		   {
		      case 0x00:
		      {
  				 printf("0");
		         break;
		      }

			  case 0x01:
		      {
	     		   printf("1");
	             tempb = Read_Key_Strobe_Data_0();
				 Foreground_color_65k(0xf800);
		         Goto_Text_XY(200,20);
	             LCD_CmdWrite(0x04);	     
	             Print_Hex(tempb);

		         break;
		      }

			  case 0x02:
		      {
			     tempb = Read_Key_Strobe_Data_0();

		         Foreground_color_65k(0xf800);
				 Goto_Text_XY(200,20);
	             LCD_CmdWrite(0x04);	     
	             Print_Hex(tempb);

				 tempb = Read_Key_Strobe_Data_1();

		         Foreground_color_65k(0xf81f);
		          Goto_Text_XY(200,40);
	             LCD_CmdWrite(0x04);	     
	             Print_Hex(tempb);

		         break;
		      }

			  case 0x03:
		      {
			     tempb = Read_Key_Strobe_Data_0();

		         Foreground_color_65k(0xf800);
		          Goto_Text_XY(200,20);
	             LCD_CmdWrite(0x04);	     
	             Print_Hex(tempb);

				 tempb = Read_Key_Strobe_Data_1();

		         Foreground_color_65k(0xf81f);
		          Goto_Text_XY(200,40);
	             LCD_CmdWrite(0x04);	     
	             Print_Hex(tempb);

				 tempb = Read_Key_Strobe_Data_2();

		         Foreground_color_65k(0x07e0);
		          Goto_Text_XY(200,60);
	             LCD_CmdWrite(0x04);	     
	             Print_Hex(tempb);
		

		         break;
		      }

			  default:			  
			    { break; }
	
         }
	   }
	}

}


void Key_Scan_Wakeup_test(void)
{
    Graphic_cursor_initial(); //initaial and enable graphic cursor 

    Interrupt_Active_Low();
    Enable_KeyScan_Interrupt();
    //Disable_KeyScan_Interrupt();
	//Mask_KeyScan_Interrupt_Flag();


   TFT_16bit();	//設定LCD driveing 使用16bit 565
   /*
   //XPDAT18_Set_GPIO_D7();
   XPDAT18_Set_KOUT4();
   //XPDAT17_Set_GPIO_D5();
   XPDAT17_Set_KOUT2();
   //XPDAT16_Set_GPIO_D4();
   XPDAT16_Set_KOUT1();
   //XPDAT9_Set_GPIO_D3();
   XPDAT9_Set_KOUT3();
   //XPDAT8_Set_GPIO_D2();
   XPDAT8_Set_KIN3();
   //XPDAT2_Set_GPIO_D6();
   XPDAT2_Set_KIN4();
   //XPDAT1_Set_GPIO_D1();
   XPDAT1_Set_KIN2();
   //XPDAT0_Set_GPIO_D0();
   XPDAT0_Set_KIN1();
	*/
   LCD_CmdWrite(0x0F);
   LCD_DataWrite(0xff);

   Long_Key_Timing_Adjustment(0x0c);
   Long_Key_enable();

   Key_Scan_Freg(0x07);
   Key_Scan_Wakeup_Function_Enable();
   
   Key_Scan_Enable();

   

   while(1)
   {

	//delay_seconds(3);
	Display_OFF();				//display off 
    delay_ms(20);
	//delay_seconds(1);			//
    //Power_Saving_Standby_Mode();  //
	//Power_Saving_Suspend_Mode(); 
	Power_Saving_Sleep_Mode();
    //delay_seconds(1);

    //Power_Normal_Mode();
	do{
	 delay_ms(100);
	}
	while(Power_Saving_Status());

    GPIO_SetBits(GPIOE, GPIO_Pin_6);
	delay_us(1);
	GPIO_ResetBits(GPIOE, GPIO_Pin_6);

	delay_ms(10);
	Display_ON();

	delay_seconds(5);	 

     }



}




void Font_Funcion_8bpp(void)
{
unsigned long i;

 

Select_Main_Window_8bpp();

RGB_16b_8bpp();
Memory_8bpp_Mode();



 Graphic_Mode();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_256(color256_white);
  Background_color_256(color256_black);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");

	while(1)
	{
	/*
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 for(i=0; i<480000;i++)
	  {LCD_DataWrite(0x00e0);}
   delay_ms(1000);*/
	   for(i=0;i<256;i++)
	   {
	    Foreground_color_256(i);
	    delay_ms(10);	
        //Goto_Text_XY(0,0);
        Show_String("AB");
		}
	   Goto_Text_XY(0,200);
	   for(i=0;i<256;i++)
	   {
	    Foreground_color_256(color256_white);
	    Background_color_256(i);
	    delay_ms(10);	
        //Goto_Text_XY(0,0);
        Show_String("  ");
		}
		//while(1);
 	 }


}

void Font_Funcion_16bpp(void)
{
unsigned short i;
Select_Main_Window_16bpp();
RGB_16b_16bpp();
Memory_16bpp_Mode();

Graphic_Mode();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Foreground_color_65k(color65k_white);
  Background_color_65k(color65k_black);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_65k_bin(800,600,"p2.bin");

  while(1)
  {
		//顯示B color
	   for(i=0;i<32;i++)
	   {
	    Foreground_color_65k(i);
        //Goto_Text_XY(0,0);
        Show_String("B");
		}

		//顯示G color
	   for(i=0;i<64;i++)
	   {
	    Foreground_color_65k(i*32);
        //Goto_Text_XY(0,0);
        Show_String("G");
		}
	    //顯示R color
	   for(i=0;i<32;i++)
	   {
	    Foreground_color_65k(i*2048);
        //Goto_Text_XY(0,0);
        Show_String("R");
		}

	   Goto_Text_XY(0,100);

	    //顯示B color
	   for(i=0;i<32;i++)
	   {
	     Background_color_65k(i);
        //Goto_Text_XY(0,0);
        Show_String(" ");
		}

		//顯示G color
	   for(i=0;i<64;i++)
	   {
	     Background_color_65k(i*32);
        //Goto_Text_XY(0,0);
        Show_String(" ");
		}
	    //顯示R color
	   for(i=0;i<32;i++)
	   {
	     Background_color_65k(i*2048);
        //Goto_Text_XY(0,0);
        Show_String(" ");
		}

		/*
	   for(i=0;i<65536;i++)
	   {
	    Foreground_color_65k(color65k_white);
	    Background_color_65k(i);
	
        //Goto_Text_XY(0,0);
        Show_String(" ");
		}*/

		while(1);
   }
}

void Font_Funcion_24bpp(void)
{
unsigned long i;

	TFT_24bit();

	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

    Display_ON();

Select_Main_Window_24bpp();
RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

Graphic_Mode();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  
  Foreground_color_16M(color16M_white);
  Background_color_16M(color16M_black);




	 RGB_16b_24bpp_mode1();

     Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 for(i=0; i<800*600;i++)
	  {
	   LCD_DataWrite(0x00e0);
	   	
		  //Check_Mem_WR_FIFO_not_Full();//
	   LCD_DataWrite(0x00e0);
	   	
		  //Check_Mem_WR_FIFO_not_Full();//
	  }
 
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 

	  //RGB_16b_24bpp_mode2();
	  //LCD_CmdWrite(0x04);
	  //sd_showpic_16M_mode2_bin(800,600,"p3.bin");


 while(1)
  {
       Goto_Text_XY(0,0);
		//顯示B color
	   for(i=0;i<256;i++)
	   {
	    Foreground_color_16M(i);
		delay_ms(1);
        //Goto_Text_XY(0,0);
        Show_String("B");
		}

		//顯示G color
	   for(i=0;i<256;i++)
	   {
	    Foreground_color_16M(i*256);
		delay_ms(1);
        //Goto_Text_XY(0,0);
        Show_String("G");
		}
	    //顯示R color
	   for(i=0;i<256;i++)
	   {
	    Foreground_color_16M(i*65536);
		delay_ms(1);
        //Goto_Text_XY(0,0);
        Show_String("R");
		}

	   Goto_Text_XY(0,300);

	    //顯示B color
	   for(i=0;i<256;i++)
	   {
	     Background_color_16M(i);
		 delay_ms(1);
        //Goto_Text_XY(0,0);
        Show_String(" ");
		}

		//顯示G color
	   for(i=0;i<256;i++)
	   {
	     Background_color_16M(i*256);
		 delay_ms(1);
        //Goto_Text_XY(0,0);
        Show_String(" ");
		}
	    //顯示R color
	   for(i=0;i<256;i++)
	   {
	     Background_color_16M(i*65536);
		 delay_ms(1);
        //Goto_Text_XY(0,0);
        Show_String(" ");
		}


		while(1);
   }
}




void GT_Font_test_8bpp(void)
{
 unsigned long i;


Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

 Graphic_Mode();


 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 //CGROM_Select_Internal_CGROM();
 CGROM_Select_Genitop_FontROM();

 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  //GTFont_Select_GT21L16TW_GT21H16T1W();
  //GTFont_Select_GT23L16U2W();
  GTFont_Select_GT23L24T3Y_GT23H24T3Y();
  //GTFont_Select_GT23L24M1Z();
  //GTFont_Select_GT23L32S4W_GT23H32S4W();
  //GTFont_Select_GT20L24F6Y();
  //GTFont_Select_GT21L24S1W();
  //GTFont_Select_GT22L16A1Y();

  Set_GTFont_Decoder(0x11);  //BIG5    


  Enable_SFlash_SPI();
  Select_SFI_0();
  //Select_SFI_1();

  Select_SFI_Font_Mode();
  //Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();
  //Select_SFI_32bit_Address();

  Select_SFI_Waveform_Mode_0();
  //Select_SFI_Waveform_Mode_3();

  Select_SFI_0_DummyRead();
  //Select_SFI_1_DummyRead();
  //Select_SFI_2_DummyRead();
  //Select_SFI_4_DummyRead();

  Select_SFI_Single_Mode();
  //Select_SFI_Dual_Mode0();
  //Select_SFI_Dual_Mode1();

  SPI_Clock_Period(3);



  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);

        Active_Window_WH(800,600);
	 	Goto_Pixel_XY(0,0);
		Graphic_Mode();
	    LCD_CmdWrite(0x04);
	    sd_showpic_256_bin(800,600,"p1.bin");

	while(1)
	{
	/*
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 for(i=0; i<480000;i++)
	  {LCD_DataWrite(0x00e0);}
   delay_ms(1000);*/
		/*
	   for(i=0;i<256;i++)
	   {
	    Foreground_color_256(i);
	    delay_ms(10);	
        //Goto_Text_XY(0,0);
        Show_String("AB");
		} */
	   Goto_Text_XY(0,0);
	   for(i=0;i<256;i++)
	   {
	    Foreground_color_256(color256_white);
	    Background_color_256(i);
	    delay_ms(10);	
        //Goto_Text_XY(0,0);
        Show_String("瑞佑科技ABCD");
		}
		//while(1);
 	 }
}





void DMA_Flash_8bpp_32bit_address(void)
{
unsigned int i;



//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();


	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
//Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);


//Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

 //Enter 4-byte mode
  
  nSS_Active();
   SPI_Master_FIFO_Data_Put(0xB7);  //切換到32位址模式
   //while( EMTI_Flag()!=1);
  // Clear_EMTI_Flag();
   delay_ms(1); 
  nSS_Inactive();
   /*
   nSS_Active();
   SPI_Master_FIFO_Data_Put(0xE9);  //切換到24位址模式
   while( EMTI_Flag()!=1);
   Clear_EMTI_Flag();
   nSS_Inactive();
   	*/
 Select_SFI_32bit_Address();

/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
//SFI_DMA_Source_Start_Address(0);//24bit address
SFI_DMA_Source_Start_Address(12443880);	//32bit address


Main_Window_Start_XY(0,0);

//Start_SFI_DMA();
//while(1);

 while(1)
 {
   for(i=0;i<(1920-800+1);i++)
   { 
	SFI_DMA_Source_Start_Address(0+i);//32bit address
    
	Start_SFI_DMA();

    //Check_2D_Busy();
    // while(1);
    //Check_Busy_SFI_DMA();
	delay_ms(10); 
   }
   delay_seconds(2);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00f0);
	
 		}
	  delay_ms(10); 
 }



 while(1);
 

}



void DMA_Flash_8bpp(void)
{
unsigned int i,j;



//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(0);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12443880);	//32bit address


Main_Window_Start_XY(0,0);

//Start_SFI_DMA();
//while(1);
  j=0;
 while(1)
 {
  

   for(i=0;i<(1920-800+1);i++)
   { 
      SPI_Clock_Period(j);
	  j=j+1;
	  delay_ms(30); 

	SFI_DMA_Source_Start_Address(0+i);//24bit address
    
	Start_SFI_DMA();

    //Check_2D_Busy();
    // while(1);
    Check_Busy_SFI_DMA();
	delay_ms(100); 

	Goto_Text_XY(300,150);
	Print_Decimal(j);

   }
   delay_seconds(2);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00f0);
	
 		}
	  delay_ms(1000); 


 }


SFI_DMA_Destination_Upper_Left_Corner(0,0);
//SFI_DMA_Transfer_Number();//for linear mode
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12443880);	//32bit address
Start_SFI_DMA();
delay_ms(1);
Check_Busy_SFI_DMA();
//delay_seconds(5);



 while(1);
 

}


void DMA_Flash_8bpp_partial(void)
{
unsigned int i;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();

	//Graphic_cursor_initial(); //initaial and enable graphic cursor 
	//Text_cursor_initial();	//initaial and enable text cursor 


Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(0);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12443880);	//32bit address

Main_Window_Start_XY(0,0);

//Start_SFI_DMA();
//while(1);
   /*
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00f0);
	
 		}
	  delay_ms(1000);
	 */
 while(1)
 {

   //=================================================
   for(i=1;i<801;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(i,600);

    Start_SFI_DMA();
    Check_Busy_SFI_DMA();
	//delay_ms(50);
    }

   delay_seconds(1);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00f0);
	
 		}
	  delay_ms(1000);

    //=================================================
   for(i=1;i<601;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(800,i);

    Start_SFI_DMA();

   //Check_2D_Busy();
   // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(50);
    }

   delay_seconds(1);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00f0);

 		}
	  delay_ms(1000);

  //=================================================
   for(i=1;i<601;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(i,i);

    Start_SFI_DMA();

   //Check_2D_Busy();
   // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(50);
    }

  // delay_seconds(2);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00f0);

 		}
	  delay_ms(1000);



}


 while(1);
 

}



//DMA搬移部分到顯示區或非顯示區任意位置
void DMA_Flash_8bpp_partial_in_partial(void)
{
unsigned int i,j,k,h;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(8);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12443880);	//32bit address

Main_Window_Start_XY(0,0);

//Start_SFI_DMA();
//while(1);

    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00f0);


 		}
	  delay_ms(1000);



	  /*
	 SFI_DMA_Destination_Upper_Left_Corner(0,360);
	 SFI_DMA_Transfer_Width_Height(320,240);

     Start_SFI_DMA();

     //Check_2D_Busy();
     // while(1);
     Check_Busy_SFI_DMA();

	while(1);
	 */


 //when canvas width =800
/*
	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);
 while(1)
 {

  for(k=0;k<10;k++)
  {  
	Canvas_Image_Start_address(k*800*600);
	Main_Image_Start_Address(k*800*600);
	delay_ms(20);
	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00e0);
		 Check_Mem_WR_FIFO_not_Full();

 		}

	 	   Text_Mode();			  //寫出對應的頁
		   Goto_Text_XY(500,500);
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(k);
		   delay_ms(10);
		   Graphic_Mode();	   
	
	delay_ms(1000);

   //=================================================
   for(j=0;j<(600);j+=60)
   {
    for(i=0;i<(800);i+=80)
    {

     SFI_DMA_Destination_Upper_Left_Corner(i,j);
	 SFI_DMA_Transfer_Width_Height(80,60);

     Start_SFI_DMA();

     //Check_2D_Busy();
     // while(1);
     Check_Busy_SFI_DMA();
	 //delay_ms(50);
     }
    }
    delay_seconds(1);
	


   }
 }
 */
//////////////////////////////////////////////////////////////////////////

//when canvas width =8188

	Main_Image_Start_Address(0);				
	Main_Image_Width(8188);							
	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(8188);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);
 while(1)
 {

  for(k=0;k<2;k++)
  {  
	Canvas_Image_Start_address(k*8188*600);
	Main_Image_Start_Address(k*8188*600);
	delay_ms(20);
	 Goto_Pixel_XY(0,0);
	 Active_Window_WH(8188,600);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<8188*600;i++)
		{				
		 LCD_DataWrite(0x00e0);
		 Check_Mem_WR_FIFO_not_Full();
	
 		}

	 	   Text_Mode();			  //寫出對應的頁
		   Goto_Text_XY(500,500);
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(k);
		   delay_ms(10);
		   Graphic_Mode();	   
	
	delay_ms(1000);

   for(h=0;h<10;h++)
   {
     Main_Image_Start_Address((k*8188*600)+(h*800));
	 delay_ms(1000);
    //=================================================
    for(j=0;j<(600);j+=60)
    {
     for(i=0;i<(800);i+=80)
     {
      SFI_DMA_Destination_Upper_Left_Corner(i+h*800,j);
	  SFI_DMA_Transfer_Width_Height(80,60);
      Start_SFI_DMA();
      //Check_2D_Busy();
      // while(1);
      Check_Busy_SFI_DMA();
	  //delay_ms(50);
     }
    }
    delay_seconds(2);
	
    }

   }
 }



///////////////////////////////////////////////////////////////////////////
   //test DMA border

   //**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);


  for(k=0;k<10;k++)
  {  
	Canvas_Image_Start_address(k*800*600);
	Main_Image_Start_Address(k*800*600);
	delay_ms(20);
	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00);
		 Check_Mem_WR_FIFO_not_Full();
 	  }
	 }
	Canvas_Image_Start_address(0);
	Main_Image_Start_Address(0);
   for(j=0;j<600;j+=60)
   {
    for(i=0;i<800;i+=80)
    {

     SFI_DMA_Destination_Upper_Left_Corner(i,j);
	 SFI_DMA_Transfer_Width_Height(80,60);

     Start_SFI_DMA();

     //Check_2D_Busy();
     // while(1);
     Check_Busy_SFI_DMA();
	 //delay_ms(50);
     }
    }
    delay_seconds(3);

		Main_Image_Start_Address(800*600);
		while(1);




}



void DMA_Flash_8bpp_2048(void)
{
unsigned long i;



//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(2048);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(2048);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_8bpp();
RGB_16b_8bpp();
Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

Enable_SFlash_SPI();



Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(0);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode


SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12443880);	//32bit address

Main_Window_Start_XY(0,0);

//Start_SFI_DMA();
//while(1);
 while(1)
 {

    Main_Window_Start_XY(1248,0); 
   SFI_DMA_Destination_Upper_Left_Corner(1248,0);
   Start_SFI_DMA();
   Check_Busy_SFI_DMA();

  // while(1);

  for(i=0;i<1248;i+=4)
  {
   Main_Window_Start_XY(i,0); 
   delay_ms(16);
   SFI_DMA_Destination_Upper_Left_Corner(i,0);
   Start_SFI_DMA();
   //Check_2D_Busy();
   // while(1);
   Check_Busy_SFI_DMA();
   
   //delay_seconds(1);
   //delay_ms(16);
   }
  
   delay_seconds(3);


 }


SFI_DMA_Destination_Upper_Left_Corner(0,0);
//SFI_DMA_Transfer_Number();//for linear mode
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12441600);	//32bit address
Start_SFI_DMA();
delay_ms(1);
Check_Busy_SFI_DMA();
//delay_seconds(5);



 //while(1);
 

}


//條件:當畫布為8188*8188
//256 color
void DMA_Flash_8bpp_8188x8188(void)
{
unsigned int i,j;


   Main_Image_Start_Address(0);				
   Main_Image_Width(8188);							
   Main_Window_Start_XY(0,0);//Display window start address

   Canvas_Image_Start_address(0);
   Canvas_image_width(8188);//
   Active_Window_XY(0,0);	
   Active_Window_WH(8188,8188);
   Memory_XY_Mode();

   Display_ON();

  Select_Main_Window_8bpp();
  RGB_16b_8bpp();
  Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(1);



/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12443880);	//32bit address
while(1)
{

	   for(i=0;i<3;i++)
	   {

	     for(j=0;j<10;j++)
		 {
	      
	       SFI_DMA_Destination_Upper_Left_Corner(j*800,i*600); 
           Start_SFI_DMA();
           Check_Busy_SFI_DMA();

		   Text_Mode();			  //寫出對應的頁
		   Goto_Text_XY((100+j*800),(100+i*600));
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(i);
		   delay_ms(10);
		   Print_Hex(j);
		   delay_ms(10);
		   Graphic_Mode();

		   Main_Window_Start_XY(j*800,i*600);
		  // delay_seconds(1);
		   delay_ms(100);
  		  }
		}
}


// while(1);
}


//條件:當畫布為800*600多張垂直排列
//256 color
void DMA_Flash_8bpp_multiple_canvas(void)
{
unsigned int i,j;


   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);//Display window start address

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);//
   Active_Window_XY(0,0);	
   Active_Window_WH(800,600);
   Memory_XY_Mode();

   Display_ON();

  Select_Main_Window_8bpp();
  RGB_16b_8bpp();
  Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(1);



/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(0);//24bit address
//SFI_DMA_Source_Start_Address(12443880);	//32bit address
while(1)
{

	   for(i=0;i<32;i++)
	   {
	       Active_Window_XY(0,0);	
           Active_Window_WH(800,600);
	       SFI_DMA_Destination_Upper_Left_Corner(0,0); 
		   Canvas_Image_Start_address(i*800*600);
           Start_SFI_DMA();
           Check_Busy_SFI_DMA();

		   Text_Mode();			  
		   Goto_Text_XY(100,100);
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(i);
		   delay_ms(10);
		   Graphic_Mode();

		   Main_Window_Start_XY(0,0);
		   Main_Image_Start_Address(i*800*600);	
		   
		  // delay_seconds(1);
		   delay_ms(100);
		}
}


// while(1);
}



//原圖為1920*1080,所以把canvas設定成一樣大小方便測試linear mode
void DMA_8bpp_linear_mode_test(void)
{
	unsigned long i,j;


    Memory_Linear_Mode();

 	Main_Image_Start_Address(0);				
	Main_Image_Width(1920);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(1920);//
	Active_Window_XY(0,0);
	Active_Window_WH(1920,1080);

    Display_ON();

  Select_Main_Window_8bpp();
  RGB_16b_8bpp();
  Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

  SPI_Clock_Period(0);


/*insert code*/


SFI_DMA_Transfer_Number(1920*1080);//for linear mode
SFI_DMA_Source_Start_Address(0);//24bit address
SFI_DMA_Destination_Start_Address(0);//linear mode


	Start_SFI_DMA();
    Check_Busy_SFI_DMA();


	//Main_Window_Start_XY(1120,0);
	//while(1);

 while(1)
 {
	for(i=0;i<1920-800+1;i++)
	{
    Main_Window_Start_XY(i,0);
   	delay_ms(5);
	}
	delay_ms(1000);


	
	for(j=0;j<1080-600+1;j++)
	{
    Main_Window_Start_XY(1120,j);
   	delay_ms(5);
	}
	delay_ms(1000);

		
	for(i=1920-800;i>0;i--)
	{
    Main_Window_Start_XY(i,480);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,480);
	delay_ms(1000);
	
    for(j=1080-600;j>0;j--)
	{
    Main_Window_Start_XY(0,j);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,0);
	delay_ms(1000);


 }								  

 }




void DMA_Flash_16bpp_32bit_address(void)
{
unsigned int i;


//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_16bpp();
RGB_16b_16bpp();
Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

//資料寫入graphic cursor
  Memory_Select_Graphic_Cursor_RAM(); 
  Graphic_Mode();

    Select_Graphic_Cursor_1();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_pen_il[i]);
    
    }

    Select_Graphic_Cursor_2();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_arrow_il[i]);
    
    }

     Select_Graphic_Cursor_3();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_busy_im[i]);
     
     }

     Select_Graphic_Cursor_4();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
    LCD_DataWrite(gImage_no_im[i]);
   
     }

    Memory_Select_SDRAM();//寫完後切回SDRAM


    Set_Graphic_Cursor_Color_1(0xff);
    Set_Graphic_Cursor_Color_2(0x00);
    Graphic_Cursor_XY(100,100);
    Enable_Graphic_Cursor();



Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
//Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(0);


//Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

 //Enter 4-byte mode
  
  nSS_Active();
   SPI_Master_FIFO_Data_Put(0xB7);  //切換到32位址模式
   //while( EMTI_Flag()!=1);
  // Clear_EMTI_Flag();
   delay_ms(1); 
  nSS_Inactive();
   /*
   nSS_Active();
   SPI_Master_FIFO_Data_Put(0xE9);  //切換到24位址模式
   while( EMTI_Flag()!=1);
   Clear_EMTI_Flag();
   nSS_Inactive();
   	*/
 Select_SFI_32bit_Address();

/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(2073600);//24bit address
//SFI_DMA_Source_Start_Address(14517480);//32bit address


Main_Window_Start_XY(0,0);



 while(1)
 {
   for(i=0;i<(1920-800+1);i++)
   { 
	SFI_DMA_Source_Start_Address(2073600+i*2);//32bit address
    
	Start_SFI_DMA();

    //Check_2D_Busy();
    // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(100); 
   }
   delay_seconds(1);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x00ff);
	
 		}
	  delay_ms(1000); 
 }



 while(1);
 

}




void DMA_Flash_16bpp(void)
{
unsigned int i;


	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

  Select_Main_Window_16bpp();
  RGB_16b_16bpp();
  Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

  SPI_Clock_Period(0);


/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(2073600);//24bit address
//SFI_DMA_Source_Start_Address(14517480);//32bit address
while(1)
 {
   for(i=0;i<(1920-800+1);i++)
   { 
	SFI_DMA_Source_Start_Address(2073600+i*2);//24bit address   
	Start_SFI_DMA();
    //Check_2D_Busy();
    // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(100); 
   }

   //delay_seconds(2);

    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x001f);
		 

 		}
   }
}





void DMA_Flash_16bpp_partial(void)
{
unsigned int i;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_16bpp();
RGB_16b_16bpp();
Memory_16bpp_Mode();

	//Graphic_cursor_initial(); //initaial and enable graphic cursor 
	//Text_cursor_initial();	//initaial and enable text cursor 


Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(2073600);//24bit address
//SFI_DMA_Source_Start_Address(14517480);//32bit address

Main_Window_Start_XY(0,0);
delay_ms(20);
//Start_SFI_DMA();
//while(1);

 while(1)
 {

   //=================================================
   for(i=1;i<800;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(i,600);

    Start_SFI_DMA();

   //Check_2D_Busy();
   // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(50);
    }

   delay_seconds(1);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x001f);
	
 		}
	  delay_ms(1000);

    //=================================================
   for(i=1;i<600;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(800,i);

    Start_SFI_DMA();

   //Check_2D_Busy();
   // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(50);
    }

   delay_seconds(1);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x001f);
	
 		}
	  delay_ms(1000);

  //=================================================
   for(i=1;i<600;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(i,i);

    Start_SFI_DMA();

   //Check_2D_Busy();
   // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(100);
    }

  // delay_seconds(2);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x001f);
 		}
	  delay_ms(1000);
 }


 while(1);
 

}


//DMA搬移部分到顯示區或非顯示區任意位置
void DMA_Flash_16bpp_partial_in_partial(void)
{
unsigned int i,j,k;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_16bpp();
RGB_16b_16bpp();
Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(2073600);//24bit address
//SFI_DMA_Source_Start_Address(14517480);//32bit address

Main_Window_Start_XY(0,0);

//Start_SFI_DMA();
//while(1);

    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x001f);

 		}
	  delay_ms(1000);


	//test keep dma move 

	 SFI_DMA_Transfer_Width_Height(320,240);
   	while(1)
	{
	 SFI_DMA_Destination_Upper_Left_Corner(1,1);
     Start_SFI_DMA();
     Check_Busy_SFI_DMA();

	 SFI_DMA_Destination_Upper_Left_Corner(479,1);
     Start_SFI_DMA();
     Check_Busy_SFI_DMA();

	 SFI_DMA_Destination_Upper_Left_Corner(1,359);
     Start_SFI_DMA();
     Check_Busy_SFI_DMA();

	 SFI_DMA_Destination_Upper_Left_Corner(479,359);
     Start_SFI_DMA();
     Check_Busy_SFI_DMA();
	 }
	

 while(1)
 {

  for(k=0;k<10;k++)
  {  
	Canvas_Image_Start_address(k*800*600*2);
	Main_Image_Start_Address(k*800*600*2);
	delay_ms(20);
	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0x001f);

 		}

	 	   Text_Mode();			  //寫出對應的頁
		   Goto_Text_XY(500,500);
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(k);
		   delay_ms(10);
		   Graphic_Mode();	   
	
	delay_ms(1000);

   //=================================================
   for(j=0;j<(600);j+=60)
   {
    for(i=0;i<(800);i+=80)
    {

     SFI_DMA_Destination_Upper_Left_Corner(i,j);
	 SFI_DMA_Transfer_Width_Height(80,60);

     Start_SFI_DMA();

     //Check_2D_Busy();
     // while(1);
     Check_Busy_SFI_DMA();
	 //delay_ms(50);
     }
    }
    delay_seconds(1);
	


   }
 }


}






void DMA_Flash_16bpp_2048(void)
{
unsigned long i;


	Main_Image_Start_Address(0);				
	Main_Image_Width(2048);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(2048);//
   Active_Window_XY(0,0);	
   Active_Window_WH(800,600);
   Memory_XY_Mode();

   Display_ON();



  Select_Main_Window_16bpp();
  RGB_16b_16bpp();
  Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

  SPI_Clock_Period(1);


/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(2073600);//24bit address
//SFI_DMA_Source_Start_Address(14517480);//32bit address
while(1)
{

		Main_Window_Start_XY(1248,0);
	    SFI_DMA_Destination_Upper_Left_Corner(1248,0);
	    //delay_seconds(2);
        Start_SFI_DMA();
	    // Check_2D_Busy();
        Check_Busy_SFI_DMA();
		//while(1);


	   for(i=0;i<1244;i+=4)
	   {
	   		
	    SFI_DMA_Destination_Upper_Left_Corner(i,0);
			
	    //delay_seconds(2);
        Start_SFI_DMA();
	    // Check_2D_Busy();
        Check_Busy_SFI_DMA();
  		Main_Window_Start_XY(i,0);
		delay_ms(20);
		}
}
//while(1);

SFI_DMA_Source_Start_Address(2073610);
Start_SFI_DMA();
//Check_Busy_SFI_DMA();
delay_ms(1000);

 
// while(1);
}



//條件:當畫布為8188*8188
//65k color
void DMA_Flash_16bpp_8188x8188(void)
{
unsigned int i,j;


   Main_Image_Start_Address(0);				
   Main_Image_Width(8188);							
   Main_Window_Start_XY(0,0);//Display window start address

   Canvas_Image_Start_address(0);
   Canvas_image_width(8188);//
   Active_Window_XY(0,0);	
   Active_Window_WH(8188,1200);
   Memory_XY_Mode();

   Display_ON();

  Select_Main_Window_16bpp();
  RGB_16b_16bpp();
  Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  //Foreground_color_256(color256_white);
  Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  //Background_color_256(color256_black);
  Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(1);



/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(2073600);//24bit address
//SFI_DMA_Source_Start_Address(14517480);//32bit address
while(1)
{

	   for(i=0;i<2;i++)
	   {

	     for(j=0;j<10;j++)
		 {
	      
	       SFI_DMA_Destination_Upper_Left_Corner(j*800,i*600); 
           Start_SFI_DMA();
           Check_Busy_SFI_DMA();

		   Text_Mode();			  //寫出對應的頁
		   Goto_Text_XY((100+j*800),(100+i*600));
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(i);
		   delay_ms(10);
		   Print_Hex(j);
		   delay_ms(10);
		   Graphic_Mode();

		   Main_Window_Start_XY(j*800,i*600);
		  // delay_seconds(1);
		   //delay_ms(100);
  		  }
		}
}
while(1);

SFI_DMA_Source_Start_Address(2073610);
Start_SFI_DMA();
//Check_Busy_SFI_DMA();
delay_ms(1000);

 
// while(1);
}


void DMA_Flash_16bpp_multiple_canvas(void)
{
unsigned int i,j;


   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);//Display window start address

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);//
   Active_Window_XY(0,0);	
   Active_Window_WH(800,600);
   Memory_XY_Mode();

   Display_ON();

  Select_Main_Window_16bpp();
  RGB_16b_16bpp();
  Memory_16bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  //Foreground_color_256(color256_white);
  Foreground_color_65k(color65k_yellow);
  //Foreground_color_16M();

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  //Background_color_256(color256_black);
  Background_color_65k(color65k_purple);
  //Background_color_16M(unsigned long	temp);


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(0);



/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(2073600);//24bit address
//SFI_DMA_Source_Start_Address(14517480);//32bit address
while(1)
 {

	   for(i=0;i<20;i++)
	   {
		   Main_Window_Start_XY(0,0);
		   Main_Image_Start_Address(i*800*600*2);
		   delay_ms(100);
	       Active_Window_XY(0,0);	
           Active_Window_WH(800,600);
	       SFI_DMA_Destination_Upper_Left_Corner(0,0); 
		   Canvas_Image_Start_address(i*800*600*2);
           Start_SFI_DMA();
           Check_Busy_SFI_DMA();

		   Text_Mode();			  
		   Goto_Text_XY(50,50);
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(i);
		   delay_ms(10);
		   Graphic_Mode();

		  	
		   
		  // delay_seconds(1);
		   
		}
 }


// while(1);
}



//原圖為1920*1080,所以把canvas設定成一樣大小方便測試linear mode
void DMA_16bpp_linear_mode_test(void)
{
	unsigned long i,j;

    
    Memory_Linear_Mode();

 	Main_Image_Start_Address(0);				
	Main_Image_Width(1920);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(1920);//
	//Active_Window_XY(0,0);
	//Active_Window_WH(1920,1080);

    Display_ON();

  Select_Main_Window_16bpp();
  RGB_16b_16bpp();
  Memory_16bpp_Mode();


	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 



  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

  SPI_Clock_Period(1);


/*insert code*/

SFI_DMA_Source_Start_Address(2073600);//24bit address
//SFI_DMA_Source_Start_Address(14517480);//32bit address

SFI_DMA_Transfer_Number(1920*1080*2);//for linear mode
SFI_DMA_Source_Start_Address(2073600);//24bit address
SFI_DMA_Destination_Start_Address(0);//linear mode


	Start_SFI_DMA();
    Check_Busy_SFI_DMA();



 while(1)
 {
	for(i=0;i<1920-800+1;i++)
	{
    Main_Window_Start_XY(i,0);
   	delay_ms(20);
	}
	delay_ms(1000);


	
	for(j=0;j<1080-600+1;j++)
	{
    Main_Window_Start_XY(1120,j);
   	delay_ms(20);
	}
	delay_ms(1000);

		
	for(i=1920-800;i>0;i--)
	{
    Main_Window_Start_XY(i,480);
   	delay_ms(20);
	}
	Main_Window_Start_XY(0,480);
	delay_ms(1000);
	
    for(j=1080-600;j>0;j--)
	{
    Main_Window_Start_XY(0,j);
   	delay_ms(20);
	}
	Main_Window_Start_XY(0,0);
	delay_ms(1000);


 }								  

 }


//=====================================================================

void DMA_Flash_24bpp_32bit_address(void)
{
unsigned int i;


//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(1600);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(1600);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_24bpp();
RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();


Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(3);


 RGB_16b_24bpp_mode1();

     Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 for(i=0; i<800*600;i++)
	  {
	   LCD_DataWrite(0x00e0);
		  //Check_Mem_WR_FIFO_not_Full();//
	   LCD_DataWrite(0x00e0);
		  //Check_Mem_WR_FIFO_not_Full();//
	  }

	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 



//Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

 //Enter 4-byte mode
  
  nSS_Active();
   SPI_Master_FIFO_Data_Put(0xB7);  //切換到32位址模式
   //while( EMTI_Flag()!=1);
  // Clear_EMTI_Flag();
   delay_ms(1); 
  nSS_Inactive();
   /*
   nSS_Active();
   SPI_Master_FIFO_Data_Put(0xE9);  //切換到24位址模式
   while( EMTI_Flag()!=1);
   Clear_EMTI_Flag();
   nSS_Inactive();
   	*/
 Select_SFI_32bit_Address();

/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(6220800);//24bit address
//SFI_DMA_Source_Start_Address(18664680);//32bit address


Main_Window_Start_XY(0,0);

while(1)
{
SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(128,128);

SFI_DMA_Source_Width(128);
 	SFI_DMA_Source_Start_Address(1130);//32bit address   
	Start_SFI_DMA();
	Check_Busy_SFI_DMA();	
}

 while(1)
 {
  
	for(i=0;i<(1920-800+1);i++)
   { 
	SFI_DMA_Source_Start_Address(6220800+i*3);//32bit address   
	Start_SFI_DMA();
	Check_Busy_SFI_DMA();	 
   }

     delay_seconds(2);

	 Active_Window_XY(0,0);
	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 

 }



 while(1);
 

}



void DMA_Flash_24bpp(void)
{
   unsigned int i;
	Main_Image_Start_Address(0);				
	Main_Image_Width(800);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();

    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


    Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);


 RGB_16b_24bpp_mode1();

     Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 for(i=0; i<800*600;i++)
	  {
	   LCD_DataWrite(0x00e0);
		  //Check_Mem_WR_FIFO_not_Full();//
	   LCD_DataWrite(0x00e0);

		  //Check_Mem_WR_FIFO_not_Full();//
	  }

	 Active_Window_WH(800,600);
     Goto_Pixel_XY(0,0);
     LCD_CmdWrite(0x04);
	 sd_showpic_16M_mode1_bin(800,600,"p3.bin"); 


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);//linear
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(100,100);  //block mode

SFI_DMA_Transfer_Width_Height(320,240);
SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(6220800);//24bit address
//SFI_DMA_Source_Start_Address(18664680);//32bit address

while(1)
{ 
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();
}

 while(1)
 {
 
     for(i=0;i<(1920-800+1);i++)
   { 
	SFI_DMA_Source_Start_Address(6220800+i*3);//24bit address   
	Start_SFI_DMA();
    //Check_2D_Busy();
    // while(1);
    Check_Busy_SFI_DMA();
	delay_ms(100); 
   }

 
 delay_ms(1000);
 //while(1);

    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);

		 LCD_DataWrite(0x001f);
	
 		}

 }

}



void DMA_Flash_24bpp_partial(void)
{
unsigned int i;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_24bpp();
RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

	//Graphic_cursor_initial(); //initaial and enable graphic cursor 
	//Text_cursor_initial();	//initaial and enable text cursor 


Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(0);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(6220800);//24bit address
//SFI_DMA_Source_Start_Address(18664680);//32bit address

Main_Window_Start_XY(0,0);

//Start_SFI_DMA();
//while(1);
   /*
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);

		 LCD_DataWrite(0x001f);

 		}
	  delay_ms(1000);
	 */
 while(1)
 {

  //=================================================
   for(i=1;i<801;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(i,600);

    Start_SFI_DMA();

   //Check_2D_Busy();
   // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(50);
    }

   delay_seconds(1);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);
		 LCD_DataWrite(0x001f);

 		}
	  delay_ms(1000);

    //=================================================
   for(i=1;i<601;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(800,i);

    Start_SFI_DMA();

   //Check_2D_Busy();
   // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(50);
    }

   delay_seconds(1);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);

		 LCD_DataWrite(0x001f);

 		}
	  delay_ms(1000);

  //=================================================


   for(i=1;i<600;i++)
   {

    SFI_DMA_Destination_Upper_Left_Corner(0,0);
	SFI_DMA_Transfer_Width_Height(i,i);

    Start_SFI_DMA();

   //Check_2D_Busy();
   // while(1);
    Check_Busy_SFI_DMA();
	//delay_ms(100);
    }

  // delay_seconds(2);
	
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);

		 LCD_DataWrite(0x001f);

 		}
	  delay_ms(1000);
 }


 while(1);
 

}



//DMA搬移部分到顯示區或非顯示區任意位置
void DMA_Flash_24bpp_partial_in_partial(void)
{
unsigned int i,j,k;

//**[20h][21h][22h][23h]**//
	Main_Image_Start_Address(0);
//**[24h][25h]**//					
	Main_Image_Width(800);							
//**[26h][27h][28h][29h]**//
	Main_Window_Start_XY(0,0);//Display window start address
//**[50h][51h][52h][53h]**//
	Canvas_Image_Start_address(0);
//**[54h][55h]**//
	Canvas_image_width(800);//
//**[56h][57h][58h][59h]**//
	Active_Window_XY(0,0);
//**[5Ah][5Bh][5Ch][5Dh]**//
	Active_Window_WH(800,600);

    Display_ON();


Select_Main_Window_24bpp();
RGB_16b_24bpp_mode1();
Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


Enable_SFlash_SPI();

Select_SFI_1();
Select_SFI_DMA_Mode();
Select_SFI_24bit_Address();
//Select_SFI_32bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

//Select_SFI_0_DummyRead();	//normal read mode
Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

//Select_SFI_Single_Mode();
Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

SPI_Clock_Period(1);


/*insert code*/
//SFI_DMA_Destination_Start_Address(0);
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);

SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(6220800);//24bit address
//SFI_DMA_Source_Start_Address(18664680);//32bit address

Main_Window_Start_XY(0,0);

//Start_SFI_DMA();
//while(1);

    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
  	for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);
		 LCD_DataWrite(0x001f);

 		}
	  delay_ms(1000);



	  /*
	 SFI_DMA_Destination_Upper_Left_Corner(0,360);
	 SFI_DMA_Transfer_Width_Height(320,240);

     Start_SFI_DMA();

     //Check_2D_Busy();
     // while(1);
     Check_Busy_SFI_DMA();

	while(1);
	 */

 while(1)
 {

  for(k=0;k<10;k++)
  {  
	Canvas_Image_Start_address(k*800*600*3);

	 Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
  	 for(i=0; i<480000;i++)
		{				
		 LCD_DataWrite(0xffff);

		 LCD_DataWrite(0x001f);

 		}

	 	   Text_Mode();			  //寫出對應的頁
		   Goto_Text_XY(500,500);
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(k);
		   delay_ms(10);
		   Graphic_Mode();	   
	Main_Image_Start_Address(k*800*600*3);
	delay_ms(1000);

   //=================================================
   for(j=0;j<(600);j+=3)
   {
    for(i=0;i<(800);i+=4)
    {

     SFI_DMA_Destination_Upper_Left_Corner(i,j);
	 SFI_DMA_Transfer_Width_Height(4,3);

     Start_SFI_DMA();

     //Check_2D_Busy();
     // while(1);
     Check_Busy_SFI_DMA();
	 //delay_ms(50);
     }
    }
    delay_seconds(1);
	


   }
 }


}



void DMA_Flash_24bpp_2048(void)
{
unsigned long i;


	Main_Image_Start_Address(0);				
	Main_Image_Width(2048);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(2048);//
   Active_Window_XY(0,0);	
   Active_Window_WH(800,600);
   Memory_XY_Mode();

   Display_ON();



  Select_Main_Window_24bpp();
  RGB_16b_24bpp_mode1();
  Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

  SPI_Clock_Period(0);


/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(1920,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(6220800);//24bit address
//SFI_DMA_Source_Start_Address(18664680);//32bit address
while(1)
{

		Main_Window_Start_XY(1248,0);
	    SFI_DMA_Destination_Upper_Left_Corner(1248,0);
	    //delay_seconds(2);
        Start_SFI_DMA();
	    // Check_2D_Busy();
        Check_Busy_SFI_DMA();
		//while(1);


	   for(i=0;i<1244;i+=4)
	   {
	    Main_Window_Start_XY(i,0);
	    SFI_DMA_Destination_Upper_Left_Corner(i,0);
	    //delay_seconds(2);
        Start_SFI_DMA();
	    // Check_2D_Busy();
        Check_Busy_SFI_DMA();
  
		}
}
while(1);

SFI_DMA_Source_Start_Address(2073610);
Start_SFI_DMA();
//Check_Busy_SFI_DMA();
delay_ms(1000);

 
// while(1);
}



//條件:當畫布為8188*1200  with 32Mbytes
//65k color
void DMA_Flash_24bpp_8188x8188(void)
{
unsigned int i,j;


   Main_Image_Start_Address(0);				
   Main_Image_Width(8188);							
   Main_Window_Start_XY(0,0);//Display window start address

   Canvas_Image_Start_address(0);
   Canvas_image_width(8188);//
   Active_Window_XY(0,0);	
   Active_Window_WH(8188,1200);
   Memory_XY_Mode();

   Display_ON();

  Select_Main_Window_24bpp();
  RGB_16b_24bpp_mode1();
  Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


   //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  //Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  Foreground_color_16M(color16M_yellow);

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  //Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  Background_color_16M(color16M_purple);


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(1);



/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(1920,1080);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(6220800);//24bit address
//SFI_DMA_Source_Start_Address(18664680);//32bit address

            Start_SFI_DMA();
           Check_Busy_SFI_DMA();

 while(1)
 {
	for(i=0;i<1920-800+1;i++)
	{
    Main_Window_Start_XY(i,0);
   	delay_ms(5);
	}
	delay_ms(1000);


	
	for(j=0;j<1080-600+1;j++)
	{
    Main_Window_Start_XY(1120,j);
   	delay_ms(5);
	}
	delay_ms(1000);

		
	for(i=1920-800;i>0;i--)
	{
    Main_Window_Start_XY(i,480);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,480);
	delay_ms(1000);
	
    for(j=1080-600;j>0;j--)
	{
    Main_Window_Start_XY(0,j);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,0);
	delay_ms(1000);

  }	


while(1)
{

	   for(i=0;i<2;i++)
	   {

	     for(j=0;j<10;j++) //
		 {
	        Main_Window_Start_XY(j*800,i*600);
			delay_ms(20);
	       SFI_DMA_Destination_Upper_Left_Corner(j*800,i*600); 
           Start_SFI_DMA();
           Check_Busy_SFI_DMA();

		   Text_Mode();			  //寫出對應的頁
		   Goto_Text_XY((100+j*800),(100+i*600));
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(i);
		   Print_Hex(j);

		   Graphic_Mode();

		  
		  // delay_seconds(1);
		   delay_ms(100);
  		  }
		}
}
while(1);


}



void DMA_Flash_24bpp_multiple_canvas(void)
{
unsigned int i,j;


   Main_Image_Start_Address(0);				
   Main_Image_Width(800);							
   Main_Window_Start_XY(0,0);//Display window start address

   Canvas_Image_Start_address(0);
   Canvas_image_width(800);//
   Active_Window_XY(0,0);	
   Active_Window_WH(800,600);
   Memory_XY_Mode();

   Display_ON();

  Select_Main_Window_24bpp();
  RGB_16b_24bpp_mode1();
  Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

   //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  //Foreground_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  //Foreground_color_256(color256_white);
  //Foreground_color_65k(color65k_yellow);
  Foreground_color_16M(color16M_yellow);

  //Background_RGB(unsigned char RED,unsigned char GREEN,unsigned char BLUE);
  //Background_color_256(color256_black);
  //Background_color_65k(color65k_purple);
  Background_color_16M(color16M_purple);


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(1);



/*insert code*/

//SFI_DMA_Destination_Start_Address(0);//linear mode
//SFI_DMA_Transfer_Number();//for linear mode

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);

SFI_DMA_Source_Start_Address(6220800);//24bit address
//SFI_DMA_Source_Start_Address(18664680);//32bit address
while(1)
 {

	   for(i=0;i<20;i++)   //2561616A = 32Mbytes 只能寫20張
	   {
	       Active_Window_XY(0,0);	
           Active_Window_WH(800,600);
	       SFI_DMA_Destination_Upper_Left_Corner(0,0); 
		   Canvas_Image_Start_address(i*800*600*3);
		   Main_Window_Start_XY(0,0);
		   Main_Image_Start_Address(i*800*600*3);

           Start_SFI_DMA();
           Check_Busy_SFI_DMA();

		   Text_Mode();			  
		   Goto_Text_XY(100,100);
	       LCD_CmdWrite(0x04);	     
	       Print_Hex(i);
		   delay_ms(10);
		   Graphic_Mode();

		  	
		   
		  // delay_seconds(1);
		   delay_ms(100);
		}
 }


// while(1);
}



//原圖為1920*1080,所以把canvas設定成一樣大小方便測試linear mode
void DMA_24bpp_linear_mode_test(void)
{
	unsigned long i,j;

    
    Memory_Linear_Mode();

 	Main_Image_Start_Address(0);				
	Main_Image_Width(1920);							
	Main_Window_Start_XY(0,0);//Display window start address

	Canvas_Image_Start_address(0);
	Canvas_image_width(1920);//
	//Active_Window_XY(0,0);
	//Active_Window_WH(1920,1080);

    Display_ON();

  Select_Main_Window_24bpp();
  RGB_16b_24bpp_mode1();
  Memory_24bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();


//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();

Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();

Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();

  SPI_Clock_Period(1);


/*insert code*/

SFI_DMA_Source_Start_Address(6220800);//24bit address
//SFI_DMA_Source_Start_Address(18664680);//32bit address

SFI_DMA_Transfer_Number(1920*1080*3);//for linear mode
SFI_DMA_Source_Start_Address(6220800);//24bit address
SFI_DMA_Destination_Start_Address(0);//linear mode


	Start_SFI_DMA();
    Check_Busy_SFI_DMA();



 while(1)
 {
	for(i=0;i<1920-800+1;i++)
	{
    Main_Window_Start_XY(i,0);
   	delay_ms(5);
	}
	delay_ms(1000);


	
	for(j=0;j<1080-600+1;j++)
	{
    Main_Window_Start_XY(1120,j);
   	delay_ms(5);
	}
	delay_ms(1000);

		
	for(i=1920-800;i>0;i--)
	{
    Main_Window_Start_XY(i,480);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,480);
	delay_ms(1000);
	
    for(j=1080-600;j>0;j--)
	{
    Main_Window_Start_XY(0,j);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,0);
	delay_ms(1000);

  }								  

 }


//==============================================================================
/*REG[F0h~FAh]*/
void GPIO_A_test(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;
  Host_Bus_8bit();	 //切換到8bit 
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
  CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
  Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
  Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   Font_Height_X1();

   Foreground_color_256(color256_yellow);
   Background_color_256(color256_purple);

  	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	//Canvas_image_width(8188);//
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
 
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	  
	  //Goto_Text_XY(300+(i*800),150);  		   
	  //Print_Decimal(i);
	  //delay_ms(100);
	  //GPIO-A default input 

	  
	 //Set_GPIO_A_In_Out(0xff);
	//測試input
	while(1)
	{
	  LCD_CmdWrite(0xAAF1);
	  delay_ms(10);
	  temp=LCD_DataRead();
	  //temp = Read_GPIO_A_7_0();

  	  Goto_Text_XY(300,150);  		   
	  Print_Hex(temp);
	  delay_seconds(3);

	  LCD_CmdWrite(0x55F1);
	  delay_ms(10);
	  temp=LCD_DataRead();
	  //temp = Read_GPIO_A_7_0();

  	  Goto_Text_XY(300,150);  		   
	  Print_Hex(temp);
	  delay_seconds(3);

	  LCD_CmdWrite(0x00F1);
	  delay_ms(10);
	  temp=LCD_DataRead();
	  //temp = Read_GPIO_A_7_0();

  	  Goto_Text_XY(300,150);  		   
	  Print_Hex(temp);
	  delay_seconds(3);

	  LCD_CmdWrite(0xFFF1);
	  delay_ms(10);
	  temp=LCD_DataRead();
	  //temp = Read_GPIO_A_7_0();

  	  Goto_Text_XY(300,150);  		   
	  Print_Hex(temp);
	  delay_seconds(3);
	 } 

	//使用ARM此PORT無法測output
}


void GPIO_B_test(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;
  Host_Bus_8bit();	 //切換到8bit 
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
  CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
  Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
  Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   Font_Height_X1();

   Foreground_color_256(color256_yellow);
   Background_color_256(color256_purple);

  	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	//Canvas_image_width(8188);//
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
 
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	  
	  //Goto_Text_XY(300+(i*800),150);  		   
	  //Print_Decimal(i);
	  //delay_ms(100);
	  //GPIO-A default input 

	 

	//測試GPIO-B output
	while(1)
	{  
	  
	  Write_GPIO_B_7_4(0xAA);
	  delay_ms(1);

      //delay_seconds(1);
	  Write_GPIO_B_7_4(0x55);
	  delay_ms(1);
	  //delay_ms(10);
	  //LCD_CmdWrite(0xF2); //read 只有在serial IF下有效
	  //temp=LCD_DataRead();//


  	  Goto_Text_XY(300,150);  		   
	  Print_Hex(temp);
	  

	 } 


}


void GPIO_C_test(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;
  Host_Bus_8bit();	 //切換到8bit 
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
  CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
  Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
  Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   Font_Height_X1();

   Foreground_color_256(color256_yellow);
   Background_color_256(color256_purple);

  	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	//Canvas_image_width(8188);//
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();


	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
 
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	  
	  //Goto_Text_XY(300+(i*800),150);  		   
	  //Print_Decimal(i);
	  //delay_ms(100);
	  //GPIO-A default input 

	 

	Set_GPIO_C_In_Out(0xff);	//default input

	//測試GPIO-C read
	/*while(1)
	{  
	  delay_ms(10);
	  LCD_CmdWrite(0xF4); //
	  temp=LCD_DataRead();//
	   delay_ms(10);
  	  Goto_Text_XY(300,150);  		   
	  Print_Hex(temp);
	   delay_ms(100);
	   //delay_seconds(1);
	 } */
	 
	Set_GPIO_C_In_Out(0x00);
   //測試GPIO-C write
	while(1)
	{
	  delay_ms(10);
	  Write_GPIO_C_7_0(0x55);
	  delay_ms(10);
  	  Goto_Text_XY(300,150);  		   
	  Print_Hex(0x55);
	   delay_ms(1000);

	   	  delay_ms(10);
	  Write_GPIO_C_7_0(0xaa);
	  delay_ms(10);
  	  Goto_Text_XY(300,150);  		   
	  Print_Hex(0xaa);
	   delay_ms(1000);
	}

}


void GPIO_D_test(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;

  TFT_16bit();
  Host_Bus_8bit();	 //切換到8bit 
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
  CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
  Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
  Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   Font_Height_X1();

   Foreground_color_256(color256_yellow);
   Background_color_256(color256_purple);

  	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	//Canvas_image_width(8188);//
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
 
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	  
	 //Goto_Text_XY(300+(i*800),150);  		   
	 //Print_Decimal(i);
	 //delay_ms(100);
	 //GPIO-A default input 

	//Set_GPIO_D_In_Out(0xff);	//default input
	//測試GPIO-D read
	 /*
	while(1)
	{  
	   temp=Read_GPIO_D_7_0();	  // Port D 為TFT data pin,當設定不為24bpp output時多出來的PIN即為可規劃GPIO  
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   Print_Hex(temp);
	   delay_ms(100);

	 } 
	 */
     Set_GPIO_D_In_Out(0x00);// Port D 為TFT data pin,當設定不為24bpp output時多出來的PIN即為可規劃GPIO  
	//測試GPIO-D Write
	while(1)
	{  

	   Write_GPIO_D_7_0(0x55);
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   Print_Hex(0x55);
	   delay_ms(100);
	   delay_seconds(1);

	   Write_GPIO_D_7_0(0xAA);
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   Print_Hex(0xAA);
	   delay_ms(100);
	   delay_seconds(1);

	 } 


}


void GPIO_E_test(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;

  //TFT_16bit();

  TFT_LVDS(); //for test GPIO-E

  Host_Bus_8bit();	 //切換到8bit 
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
  CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
  Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
  Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   Font_Height_X1();

   Foreground_color_256(color256_yellow);
   Background_color_256(color256_purple);

  	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	//Canvas_image_width(8188);//
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
 
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	  
	 //Goto_Text_XY(300+(i*800),150);  		   
	 //Print_Decimal(i);
	 //delay_ms(100);
	 //GPIO-A default input 

	Set_GPIO_E_In_Out(0xff);	//default input
	//測試GPIO-E read
	
	while(1)
	{  
	   temp=Read_GPIO_E_7_0();	  // Port E  for LVDS package use lvds mode
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   //Print_Hex(temp);
	    printf("%x",temp);
	   delay_ms(1000);

	 } 
	 

     Set_GPIO_E_In_Out(0x00);   // Port E  for LVDS package use lvds mode
	 
	//測試GPIO-D Write
	while(1)
	{  

	   Write_GPIO_E_7_0(0x55);
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   Print_Hex(0x55);
	   delay_ms(100);
	   //delay_seconds(1);

	   Write_GPIO_E_7_0(0xAA);
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   Print_Hex(0xAA);
	   delay_ms(100);
	   //delay_seconds(1);

	 } 
}



void GPIO_F_test(void)
{
    unsigned int i,j,k,a;
	unsigned int temp;

  //TFT_16bit();

  TFT_LVDS(); //for test GPIO-E

  Host_Bus_8bit();	 //切換到8bit 
 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
  CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
  Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
  Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   Font_Height_X1();

   Foreground_color_256(color256_yellow);
   Background_color_256(color256_purple);

  	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	//Canvas_image_width(8188);//
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_8b_8bpp();
    Memory_8bpp_Mode();

	Graphic_cursor_initial(); //initaial and enable graphic cursor 
	Text_cursor_initial();	//initaial and enable text cursor 


	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
 
	Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	sd_showpic_256_bin(800,600,"p1.bin");
	  
	 //Goto_Text_XY(300+(i*800),150);  		   
	 //Print_Decimal(i);
	 //delay_ms(100);
	 //GPIO-A default input 

	Set_GPIO_F_In_Out(0xff);	//default input
	//測試GPIO-F read
	/*
	while(1)
	{  
	   temp=Read_GPIO_F_7_0();	  // Port E  for LVDS package use lvds mode
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   //Print_Hex(temp);
	    printf("%x",temp);
	   delay_ms(1000);

	 } 
	 */

     Set_GPIO_F_In_Out(0x00);   // Port E  for LVDS package use lvds mode
	 
	//測試GPIO-F Write
	while(1)
	{  

	   Write_GPIO_F_7_0(0x55);
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   Print_Hex(0x55);
	   delay_ms(100);
	   //delay_seconds(1);

	   Write_GPIO_F_7_0(0xAA);
	   delay_ms(10);
  	   Goto_Text_XY(300,150);  		   
	   Print_Hex(0xAA);
	   delay_ms(100);
	   //delay_seconds(1);

	 } 


}


//==============================================================================
void Print_Hex(unsigned char buf)
{
	unsigned char temp;

	Text_Mode();
	LCD_CmdWrite(0x04);

	temp=buf;
	temp = (temp >>4) & 0x0F;
	if(temp < 0x0A)
	{temp |= 0x30;
	}
	else temp = temp + 0x37;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();

	temp=buf;
	temp = temp & 0x0F;
	if(temp < 0x0A)
	{temp |= 0x30;
	}
	else temp = temp + 0x37;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
    Check_2D_Busy();//
	Graphic_Mode(); //back to graphic mode
}

void Print_Hex_4(unsigned short buf)
{
	unsigned short temp;

	Text_Mode();
	LCD_CmdWrite(0x04);

	temp=buf;
	temp = (temp >>12) & 0x0F;
	if(temp < 0x0A)
	{temp |= 0x30;
	}
	else temp = temp + 0x37;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();

	temp=buf;
	temp = (temp >>8) & 0x0F;
	if(temp < 0x0A)
	{temp |= 0x30;
	}
	else temp = temp + 0x37;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();

	temp=buf;
	temp = (temp >>4) & 0x0F;
	if(temp < 0x0A)
	{temp |= 0x30;
	}
	else temp = temp + 0x37;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();

	temp=buf;
	temp = temp & 0x0F;
	if(temp < 0x0A)
	{temp |= 0x30;
	}
	else temp = temp + 0x37;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
    Check_2D_Busy();//
	Graphic_Mode(); //back to graphic mode
}

void Print_Decimal(unsigned int buffer)
{
	unsigned int temp,temp1;
     
    temp=buffer/100;
    temp1= buffer%100;

    Text_Mode();
	LCD_CmdWrite(0x04);

    temp|=0x30;
	LCD_DataWrite(temp);
    //delay_ms(10);
	Check_Mem_WR_FIFO_not_Full();

    temp=temp1/10;	
	temp|=0x30;
    LCD_DataWrite(temp);
    //delay_ms(10);
	Check_Mem_WR_FIFO_not_Full();
    temp=temp1%10;	
	temp|=0x30;
    LCD_DataWrite(temp);
    //delay_ms(10);
	Check_Mem_WR_FIFO_not_Full();

    Check_2D_Busy();//
	Graphic_Mode(); //back to graphic mode
}


void Print_Decimal_5(unsigned int buffer)
{
	unsigned int temp,temp1,temp2;
     
    temp=buffer/10000;
    temp1= buffer%10000;

	 Text_Mode();
	 LCD_CmdWrite(0x04);

    temp|=0x30;
	LCD_DataWrite(temp);
    Check_Mem_WR_FIFO_not_Full();
    //delay_ms(10);

	temp=temp1/1000;	
	temp|=0x30;
    LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
    //delay_ms(10);

	temp2 = temp1%1000;

    temp=temp2/100;
    temp1= temp2%100;

    temp|=0x30;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
    //delay_ms(10);
  
    temp=temp1/10;	
	temp|=0x30;
    LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
   //delay_ms(10);

    temp=temp1%10;	
	temp|=0x30;
    LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
    //delay_ms(10);

	Check_2D_Busy();//
	Graphic_Mode(); //back to graphic mode

}

void Print_Decimal_6(unsigned int buffer)
{
	unsigned int temp,temp1,temp2;

     Text_Mode();
	 LCD_CmdWrite(0x04); 

    temp=buffer/100000;
    temp1= buffer%100000;

    temp|=0x30;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
    //delay_ms(5);

	temp = temp1/10000;
    temp|=0x30;
    LCD_DataWrite(temp);
    Check_Mem_WR_FIFO_not_Full();
    //delay_ms(5);

	temp2 = temp1%10000;

	temp=temp2/1000;	
	temp|=0x30;
    LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
    //delay_ms(5);

	temp1 = temp2%1000;

    temp=temp1/100;
    temp2= temp1%100;

    temp|=0x30;
	LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();
  
    temp=temp2/10;	
	temp|=0x30;
    LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();

    temp=temp2%10;	
	temp|=0x30;
    LCD_DataWrite(temp);
	Check_Mem_WR_FIFO_not_Full();;
	Check_2D_Busy();//
	Graphic_Mode(); //back to graphic mode

}





void Graphic_cursor_initial(void)
{
  unsigned int i ;

//資料寫入graphic cursor
  Memory_Select_Graphic_Cursor_RAM(); 
  Graphic_Mode();

    Select_Graphic_Cursor_1();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_pen_il[i]);
    }

    Select_Graphic_Cursor_2();
    LCD_CmdWrite(0x04);
    for(i=0;i<256;i++)
    {					 
     LCD_DataWrite(gImage_arrow_il[i]);
    }

     Select_Graphic_Cursor_3();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
      LCD_DataWrite(gImage_busy_im[i]);

     }

     Select_Graphic_Cursor_4();
     LCD_CmdWrite(0x04);
     for(i=0;i<256;i++)
     {					 
      LCD_DataWrite(gImage_no_im[i]);

     }

    Memory_Select_SDRAM();//寫完後切回SDRAM


    Set_Graphic_Cursor_Color_1(0xff);
    Set_Graphic_Cursor_Color_2(0x00);
    Graphic_Cursor_XY(120,100);
    Enable_Graphic_Cursor();
}


void Text_cursor_initial(void)
{

   //Disable_Text_Cursor();
    Enable_Text_Cursor_Blinking();
    //Disable_Text_Cursor_Blinking();
    Blinking_Time_Frames(10);
    //**[3E][3Fh]**//
    Text_Cursor_H_V(8,15);
	Enable_Text_Cursor();

}

//=============================================//
//=============================================//
//=============================================//
/*Mix test*/

/*REG[11h] */
void PIP_window_with_DMA_8bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
  // Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
  // Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(0);



  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);


   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	//Canvas_image_width(8188);//
	Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();


   SFI_DMA_Destination_Upper_Left_Corner(0,0);
   SFI_DMA_Transfer_Width_Height(800,600);
   SFI_DMA_Source_Width(1920);
   SFI_DMA_Source_Start_Address(0);//24bit address


	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
   while(1)
   {
     for(i=0;i<1;i++)
     {
	  Main_Window_Start_XY(i*800,0);
	  delay_ms(20);
	  Active_Window_XY(i*800,0);
	  Goto_Pixel_XY(i*800,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_256_bin(800,600,"p1.bin");
	  Goto_Text_XY(300+(i*800),150);		   
	  Print_Decimal(i);
	  delay_ms(100);
	 }

	 Main_Window_Start_XY(0,0);
  	 delay_ms(100);

	//寫入PIP1圖
	Memory_8bpp_Mode();
	//Main_Image_Start_Address(800*600);
	//Main_Image_Width(800);
	delay_ms(20);
	Canvas_Image_Start_address(800*600);
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240;i++)
	{
	 LCD_DataWrite(0x00);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address(800*600*2);
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240;i++)
	{
	 LCD_DataWrite(0xFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_8bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(800*600);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(120,120);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_8bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address(800*600*2);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(480,272);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(300,300);

	Enable_PIP2();
	delay_seconds(1);
	Active_Window_XY(0,0);
	Goto_Text_XY(0,0);

 	for(i=0;i<1000;i++)
	{
	Canvas_Image_Start_address(800*600);

	SFI_DMA_Transfer_Width_Height(320,240);
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

	Active_Window_WH(320,240);
    Foreground_color_256(color256_yellow);
    Background_color_256(color256_purple);
			
	Show_String("PIP1");
	//delay_ms(500);
	Canvas_Image_Start_address(800*600*2);

	SFI_DMA_Transfer_Width_Height(480,272);
	Start_SFI_DMA();
    Check_Busy_SFI_DMA();

	Active_Window_WH(480,272);

	Foreground_color_256(color256_red);
    Background_color_256(color256_blue);

	Show_String("PIP2");

	 //delay_ms(500);
	}



	

	  //測試PIP開關
	  /*
	 while(1)
	 {
	  Enable_PIP1();
	  Enable_PIP2();
      delay_seconds(2);
	  Disable_PIP1();
	  Disable_PIP2();
      delay_seconds(2);

	 }	*/


		   
   }	
}



/*REG[11h] */
void PIP_window_with_DMA_16bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();
	//Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 CGROM_Select_Internal_CGROM();
 //CGROM_Select_Genitop_FontROM();
 Font_Select_8x16_16x16();
 //Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

  Enable_SFlash_SPI();
  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(0);

SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);


SFI_DMA_Source_Start_Address(2073600);//24bit address


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 

 	Main_Image_Start_Address(0);					
	Main_Image_Width(2048);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(2048);//
	//Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
   while(1)
   {
	  Main_Window_Start_XY(0,0);
	  delay_ms(20);

	  Active_Window_WH(800,600);
	  Active_Window_XY(0,0);
	  Goto_Pixel_XY(0,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_65k_bin(800,600,"p2.bin");

	//寫入PIP1圖
	Memory_16bpp_Mode();
	delay_ms(20);
	Canvas_Image_Start_address(2048*600*2);
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240;i++)
	{
	 LCD_DataWrite(0x0000);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address(2048*600*2+(800*600*2));
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240;i++)
	{
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_16bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(2048*600*2);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(120,120);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_16bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address(2048*600*2+(800*600*2));
	PIP_Image_Width(800);
	PIP_Window_Width_Height(480,272);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(10,10);

	Enable_PIP2();
	delay_seconds(1);


   while(1)
   {
	 Canvas_Image_Start_address(2048*600*2);

	 SFI_DMA_Transfer_Width_Height(320,240);
	 Start_SFI_DMA();
     Check_Busy_SFI_DMA();

     Active_Window_XY(0,0);
	 Active_Window_WH(320,240);

     Foreground_color_65k(color65k_yellow);
     Background_color_65k(color65k_purple);

	 Show_String("PIP1");

	 Canvas_Image_Start_address(2048*600*2+(800*600*2));

	 SFI_DMA_Transfer_Width_Height(480,272);
	 Start_SFI_DMA();
     Check_Busy_SFI_DMA();

	 Active_Window_XY(0,0);
	 Active_Window_WH(480,272);

	 Foreground_color_65k(color65k_red);
     Background_color_65k(color65k_blue);

	 Show_String("PIP2");
	}



    //測試開關
	/*
	while(1)
	{
	 Enable_PIP1();
	 Enable_PIP2();
     delay_seconds(2);
	 Disable_PIP1();
	 Disable_PIP2();
     delay_seconds(2);

	} */


		   
   }	
}


/*REG[11h] */
void PIP_window_with_DMA_24bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;

 Host_Bus_16bit();

 //Font_Select_CGROM_Mode();
 //Font_Select_UserDefine_Mode();
 //CGROM_Select_Internal_CGROM();
 CGROM_Select_Genitop_FontROM();

 //Font_Select_8x16_16x16();
 Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  //GTFont_Select_GT21L16TW_GT21H16T1W();
  //GTFont_Select_GT23L16U2W();
  GTFont_Select_GT23L24T3Y_GT23H24T3Y();
  //GTFont_Select_GT23L24M1Z();
  //GTFont_Select_GT23L32S4W_GT23H32S4W();
  //GTFont_Select_GT20L24F6Y();
  //GTFont_Select_GT21L24S1W();
  //GTFont_Select_GT22L16A1Y();

  Set_GTFont_Decoder(0x11);  //BIG5  

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

   Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(0);


SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(6220800);//24bit address

//set serial flash to 32bit address mode 

//Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

 //Enter 4-byte mode  
  nSS_Active();
   SPI_Master_FIFO_Data_Put(0xB7);  //切換到32位址模式
   //while( EMTI_Flag()!=1);
  // Clear_EMTI_Flag();
   delay_ms(1); 
  nSS_Inactive();


  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 

      //Disable_Text_Cursor();
    Enable_Text_Cursor_Blinking();
    //Disable_Text_Cursor_Blinking();
    Blinking_Time_Frames(10);
    //**[3E][3Fh]**//
    Text_Cursor_H_V(3,15);
	Enable_Text_Cursor();


 	Main_Image_Start_Address(0);					
	Main_Image_Width(800);							
	//Main_Image_Width(800);

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(800);//
	//Canvas_image_width(800);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
   while(1)
   {
 
	  Main_Window_Start_XY(0,0);
	  delay_ms(20);
	  Active_Window_XY(0,0);
	  Goto_Pixel_XY(0,0);
	  LCD_CmdWrite(0x04);
	  sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	  delay_ms(100);

	 Main_Window_Start_XY(0,0);
  	 delay_ms(100);

	 //寫入PIP1圖
	 Memory_24bpp_Mode();
	 delay_ms(20);
	 Canvas_Image_Start_address(800*600*3);
	 Canvas_image_width(800);//
     Active_Window_XY(0,0);
	 Active_Window_WH(320,240);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<320*240/2;i++)
	 {
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
 	 }
	  Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address((800*600*3)*2);
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240/2;i++)
	{
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_24bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(800*600*3);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(120,120);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_24bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address((800*600*3)*2);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(800,480);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(0,100);

	Enable_PIP2();
	delay_seconds(1);

	Goto_Text_XY(0,0);
   while(1)
   {
  	  Canvas_Image_Start_address(800*600*3);

	  Select_SFI_1();
      Select_SFI_DMA_Mode();
      Select_SFI_32bit_Address();
      Select_SFI_Waveform_Mode_3();
      Select_SFI_0_DummyRead();	//normal read mode
      Select_SFI_Single_Mode();
      SPI_Clock_Period(0);

	  SFI_DMA_Transfer_Width_Height(320,240);
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();

	  Active_Window_XY(0,0);
	  Active_Window_WH(320,240);

      Foreground_color_16M(color16M_yellow);
      Background_color_16M(color16M_purple);


	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
      Select_SFI_0_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(2);

	  Show_String("PIP1 浮動視窗1");


	  Canvas_Image_Start_address((800*600*3)*2);

	  Select_SFI_1();
      Select_SFI_DMA_Mode();
      Select_SFI_32bit_Address();
      Select_SFI_Waveform_Mode_3();
      Select_SFI_0_DummyRead();	//normal read mode
      Select_SFI_Single_Mode();
      SPI_Clock_Period(0);

	  SFI_DMA_Transfer_Width_Height(800,480);
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();

	  Active_Window_XY(0,0);
	  Active_Window_WH(800,480);

	  Foreground_color_16M(color16M_red);
      Background_color_16M(color16M_blue);

	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
      Select_SFI_0_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(2);

	  Show_String("PIP2 浮動視窗2");


	}

	 /*
	while(1)
	{
	 Enable_PIP1();
	 delay_seconds(1);
	 Enable_PIP2();
     delay_seconds(1);
	 Disable_PIP1();
	 delay_seconds(1);
	 Disable_PIP2();
     delay_seconds(1);
	}  */
		   
   }	
}



void PIP_window_with_DMA_Draw_8bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;
   unsigned long resx1,resy1,resx2,resy2,resx3,resy3;


 Host_Bus_16bit();

 //Font_Select_UserDefine_Mode();
 //CGROM_Select_Internal_CGROM();
 CGROM_Select_Genitop_FontROM();

 //Font_Select_8x16_16x16();
 Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  //GTFont_Select_GT21L16TW_GT21H16T1W();
  //GTFont_Select_GT23L16U2W();
  GTFont_Select_GT23L24T3Y_GT23H24T3Y();
  //GTFont_Select_GT23L24M1Z();
  //GTFont_Select_GT23L32S4W_GT23H32S4W();
  //GTFont_Select_GT20L24F6Y();
  //GTFont_Select_GT21L24S1W();
  //GTFont_Select_GT22L16A1Y();

  Set_GTFont_Decoder(0x11);  //BIG5  

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
  // Font_Background_select_Transparency();
   Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

   Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(1);


SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(0);//24bit address


//set serial flash to 32bit address mode 

//Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

 //Enter 4-byte mode  
  nSS_Active();
   SPI_Master_FIFO_Data_Put(0xB7);  //切換到32位址模式
   //while( EMTI_Flag()!=1);
  // Clear_EMTI_Flag();
   delay_ms(1); 
  nSS_Inactive();


  Foreground_color_256(color256_yellow);
  Background_color_256(color256_purple);

	Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

   //Write CGRAM in linear mode initial CGRAM(平常建議在此模式下並設定canvas為8bpp)

     CGRAM_Start_address(3033600);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關


	 Goto_Linear_Addr(3033600);

	 LCD_CmdWrite(0x04);					  //寫入全型字
	for(j=0; j<50;j++)
	 {	 
	 for(i=0; i<128;i++)	 //16*16/8			 
		{
		 LCD_DataWrite(gImage_Font_32x32[i]);
		 Check_Mem_WR_FIFO_not_Full();
		 //delay_us(1);	
		}
	   }	 


	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode


   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 



 	Main_Image_Start_Address(0);					
	Main_Image_Width(1920);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(1920);//

	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_8bpp();
    RGB_16b_8bpp();
    Memory_8bpp_Mode();

	Select_SFI_32bit_Address();
	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(1920,1080);
    SFI_DMA_Source_Width(1920);

    SFI_DMA_Source_Start_Address(0);//24bit address

     Start_SFI_DMA();
     Check_Busy_SFI_DMA();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);
   while(1)
   {
 
	  Main_Window_Start_XY(0,0);
	  delay_ms(20);
	  Active_Window_XY(0,0);
	  Goto_Pixel_XY(0,0);
	  LCD_CmdWrite(0x04);
	  //sd_showpic_256_bin(800,600,"p1.bin");

	  delay_ms(100);

	 Main_Window_Start_XY(0,0);
  	 delay_ms(100);

	 //寫入PIP1圖
	 Memory_8bpp_Mode();
	 delay_ms(20);
	 Canvas_Image_Start_address(1920*1080);
	 Canvas_image_width(800);//
     Active_Window_XY(0,0);
	 Active_Window_WH(320,240);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<320*240/2;i++)
	 {
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
 	 }
	  Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address((1920*1080)+(800*600));
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240/2;i++)
	{
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_8bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(1920*1080);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(400,350);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_8bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address((1920*1080)+(800*600));
	PIP_Image_Width(800);
	PIP_Window_Width_Height(600,480);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(50,50);

	Enable_PIP2();
	delay_seconds(1);

	Goto_Text_XY(0,0);
   while(1)
   {
  	  Canvas_Image_Start_address(1920*1080);

	  Select_SFI_1();
      Select_SFI_DMA_Mode();
      Select_SFI_32bit_Address();
      Select_SFI_Waveform_Mode_3();
      Select_SFI_0_DummyRead();	//normal read mode
      Select_SFI_Single_Mode();
      SPI_Clock_Period(1);

	  SFI_DMA_Transfer_Width_Height(320,240);
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();

	  Active_Window_XY(0,0);
	  Active_Window_WH(320,240);

      Foreground_color_256(color256_yellow);
      Background_color_256(color256_purple);


	  Font_Width_X1();
      Font_Height_X1();
	  CGROM_Select_Genitop_FontROM();
      Font_Select_12x24_24x24();
	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
      Select_SFI_0_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(2);	 // Freq must setting <=20MHZ
	   Goto_Text_XY(0,0);

	   Font_Background_select_Color();
	  Show_String("PIP1浮動視窗1");

	   //Font_Background_select_Transparency();
	   CGROM_Select_Internal_CGROM();
       Font_Select_8x16_16x16();
	   Show_String("PIPwindow1");

	   Font_Select_UserDefine_Mode();
	   Font_Select_16x32_32x32();	    
		//Goto_Text_XY(0,0);
	    Text_Mode();	
	  for(i=0;i<5;i++)
	  {							//寫出半型字 字碼由0x8000開始
	   temp=i|0x8000;
	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	  }
	  Check_2D_Busy();
	  Graphic_Mode();


	  Active_Window_WH(800,600);

	   do
		{
	     resx1=rand()%600;
         resy1=rand()%400;

		 resx2=rand()%600;
         resy2=rand()%400;
		 
        }while((resx1==resx2)||(resy1==resy2));


		i=rand()%256;
        Foreground_color_256(i);


	    if(resx1<resx2)
		  {resx2+=10;
		  i = resx2-resx1;}
		else 	  
		  {	resx1+=10;
		  i = resx1-resx2;}

		if(resy1<resy2)
		  {	resy2+=10;
		  j = resy2-resy1;}
		else 	  
		  {	resy1+=10;
		  j = resy1-resy2;}

		  i/=9;
		  if(i<1)
		  i = 1;

		  j/=9;
		  if(j<1)
		  j = 1;

		  if(i<j)
		  k=i;
		  else if (i>j)
		  k=j;
		  else
		  k=1;

	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);

		  //printf("\r\n Error \r\n");
		  //printf("%d\r\n",resx1);
		  //printf("%d\r\n",resy1);
		  //printf("%d\r\n",resx2);
		  //printf("%d\r\n",resy2);

          //printf("%d\r\n",i);
		  //printf("%d\r\n",j);
		 	
		Circle_Square_Radius_RxRy (k,k);

		j=rand()%3;
	    if(j)
		{
	    Start_Circle_Square();
		delay_ms(100);
		}
		else
		{
		Start_Circle_Square_Fill();
		delay_ms(100);
		}


	  Canvas_Image_Start_address((1920*1080)+(800*600));

	  Select_SFI_1();
      Select_SFI_DMA_Mode();
      Select_SFI_32bit_Address();
      Select_SFI_Waveform_Mode_3();
      Select_SFI_0_DummyRead();	//normal read mode
      Select_SFI_Single_Mode();
      SPI_Clock_Period(1);

	  SFI_DMA_Transfer_Width_Height(800,480);
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();

	  Active_Window_XY(0,0);
	  Active_Window_WH(800,480);

	  Foreground_color_256(color256_red);
      Background_color_256(color256_blue);

	  Font_Width_X2();
      Font_Height_X2();

	  CGROM_Select_Genitop_FontROM();
      Font_Select_12x24_24x24();
	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
      Select_SFI_0_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(2);	  // Freq must setting <=20MHZ

	  Font_Background_select_Transparency();
	  Show_String("PIP2 浮動視窗2");

	  Font_Background_select_Color();
	  CGROM_Select_Internal_CGROM();
       Font_Select_8x16_16x16();
	   Show_String("PIP window2");

		Font_Select_16x32_32x32();
	    Font_Select_UserDefine_Mode(); 
		//Goto_Text_XY(0,0);
	    Text_Mode();	
	  for(i=0;i<5;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i|0x8000;
	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	  }
	  Check_2D_Busy();
	  Graphic_Mode();

	  	Active_Window_WH(800,600);

			i=rand()%256;
        Foreground_color_256(i);

		resx1=rand()%600;
        resy1=rand()%400;
		i=rand()%100;

		Circle_Center_XY(resx1,resy1);
		Circle_Radius_R(i);


		 //printf("x=%d",resx1);
		 //printf(" ");
		 //printf("y=%d",resy1);
		 //printf(" ");
         //printf("i=%d",i);
		 //printf(" ");

		j=rand()%3;
	    if(j)
		{
	    Start_Circle_or_Ellipse();
		delay_ms(100);
		}
		else
		{
		Start_Circle_or_Ellipse_Fill();
		delay_ms(100);
		}


		
	//test main window
	for(i=0;i<1920-800+1;i++)
	{
    Main_Window_Start_XY(i,0);
   	delay_ms(5);
	}
	delay_ms(100);


	
	for(j=0;j<1080-600+1;j++)
	{
    Main_Window_Start_XY(1120,j);
   	delay_ms(5);
	}
	delay_ms(100);

		
	for(i=1920-800;i>0;i--)
	{
    Main_Window_Start_XY(i,480);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,480);
	delay_ms(100);
	
    for(j=1080-600;j>0;j--)
	{
    Main_Window_Start_XY(0,j);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,0);
	delay_ms(100);


	 }
		   
   }	
}




void PIP_window_with_DMA_Draw_16bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;
   unsigned long resx1,resy1,resx2,resy2,resx3,resy3;


 Host_Bus_16bit();


 //Font_Select_UserDefine_Mode();
 //CGROM_Select_Internal_CGROM();
 CGROM_Select_Genitop_FontROM();

 //Font_Select_8x16_16x16();
 Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  //GTFont_Select_GT21L16TW_GT21H16T1W();
  //GTFont_Select_GT23L16U2W();
  GTFont_Select_GT23L24T3Y_GT23H24T3Y();
  //GTFont_Select_GT23L24M1Z();
  //GTFont_Select_GT23L32S4W_GT23H32S4W();
  //GTFont_Select_GT20L24F6Y();
  //GTFont_Select_GT21L24S1W();
  //GTFont_Select_GT22L16A1Y();

  Set_GTFont_Decoder(0x11);  //BIG5  

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
   Font_Background_select_Transparency();
   //Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

   Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();

//Select_SFI_Waveform_Mode_0();
Select_SFI_Waveform_Mode_3();
Select_SFI_0_DummyRead();	//normal read mode
//Select_SFI_8_DummyRead(); //1byte dummy cycle
//Select_SFI_16_DummyRead();
//Select_SFI_24_DummyRead();
Select_SFI_Single_Mode();
//Select_SFI_Dual_Mode0();
//Select_SFI_Dual_Mode1();
SPI_Clock_Period(1);


SFI_DMA_Destination_Upper_Left_Corner(0,0);
SFI_DMA_Transfer_Width_Height(800,600);
SFI_DMA_Source_Width(1920);
SFI_DMA_Source_Start_Address(2073600);//24bit address


//set serial flash to 32bit address mode 

//Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

 //Enter 4-byte mode  
  nSS_Active();
   SPI_Master_FIFO_Data_Put(0xB7);  //切換到32位址模式
   //while( EMTI_Flag()!=1);
  // Clear_EMTI_Flag();
   delay_ms(1); 
  nSS_Inactive();


  Foreground_color_65k(color65k_yellow);
  Background_color_65k(color65k_purple);

   //Graphic_cursor_initial(); //initaial and enable graphic cursor 
   //Text_cursor_initial();	//initaial and enable text cursor 


     CGRAM_Start_address(6067200);//假設每各畫部都是800*600,共有10各畫布,CGRAM起始位置為800*600*10=4800000	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關
 
    Goto_Linear_Addr(6067200);
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

    LCD_CmdWrite(0x04);
		for(j=0; j<10;j++)
		{	 
		  for(i=0; i<24;i++)	 //	 16bp寫入	User_Font 
		  {
		  temp = gImage_Font_12x24[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_12x24[i*2]; 
		  LCD_DataWrite(temp);
		  Check_Mem_WR_FIFO_not_Full();
	
	      }

		  for(i=0; i<24;i++)	 //	 16bp寫入	User_Font 
		  {
		  temp = gImage_Font_12x24[i*2+1];
		  temp <<= 8;
		  temp |=gImage_Font_12x24[i*2]; 
		  LCD_DataWrite(~temp);
		  Check_Mem_WR_FIFO_not_Full();
		
	      }
		 } 
	  	 

	  RGB_16b_16bpp(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_16bpp_Mode();//切換為原始的色深




 	Main_Image_Start_Address(0);					
	Main_Image_Width(1920);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(1920);//
	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_16bpp();
    RGB_16b_16bpp();
    Memory_16bpp_Mode();

    Select_SFI_32bit_Address();
	SFI_DMA_Destination_Upper_Left_Corner(0,0);
    SFI_DMA_Transfer_Width_Height(1920,1080);
    SFI_DMA_Source_Width(1920);

     SFI_DMA_Source_Start_Address(2073600);//24bit address
     //SFI_DMA_Source_Start_Address(14517480);//32bit address

        Start_SFI_DMA();
        Check_Busy_SFI_DMA();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);

	
	//test DMA falling 
    LCD_CmdWrite(0x01);
	temp = LCD_DataRead();
    temp |= cSetb6;
	LCD_DataWrite(temp);
	 

   while(1)
   {
 
	  Main_Window_Start_XY(0,0);
	  delay_ms(20);
	  Active_Window_XY(0,0);
	  Goto_Pixel_XY(0,0);
	  LCD_CmdWrite(0x04);
	 // sd_showpic_65k_bin(800,600,"p2.bin");

	  delay_ms(100);

	 Main_Window_Start_XY(0,0);
  	 delay_ms(100);

	 //寫入PIP1圖
	 Memory_16bpp_Mode();
	 delay_ms(20);
	 Canvas_Image_Start_address(1920*1080*2);
	 Canvas_image_width(800);//
     Active_Window_XY(0,0);
	 Active_Window_WH(320,240);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<320*240/2;i++)
	 {
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
 	 }
	  Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address((1920*1080*2)+(800*600*2));
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(320,240);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<320*240/2;i++)
	{
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_16bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(1920*1080*2);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(120,120);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_16bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address((1920*1080*2)+(800*600*2));
	PIP_Image_Width(800);
	PIP_Window_Width_Height(600,480);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(50,50);

	Enable_PIP2();
	delay_seconds(1);

	Goto_Text_XY(0,0);
   while(1)
   {

  	  Canvas_Image_Start_address(1920*1080*2);

	  Select_SFI_1();
      Select_SFI_DMA_Mode();
      Select_SFI_32bit_Address();
      Select_SFI_Waveform_Mode_3();
      Select_SFI_0_DummyRead();	//normal read mode
      Select_SFI_Single_Mode();
      SPI_Clock_Period(0);

	  SFI_DMA_Transfer_Width_Height(320,240);
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();

	  Active_Window_XY(0,0);
	  Active_Window_WH(320,240);

      Foreground_color_65k(color65k_yellow);
      Background_color_65k(color65k_purple);


	  Font_Width_X3();
      Font_Height_X3();
	  CGROM_Select_Genitop_FontROM();
      Font_Select_12x24_24x24();
	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
      Select_SFI_0_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(3);	 // Freq must setting <=20MHZ
	   Goto_Text_XY(0,0);

	   Font_Background_select_Color();
	  Show_String("PIP1浮動視窗1");

	   Font_Background_select_Transparency();
	   CGROM_Select_Internal_CGROM();
       Font_Select_8x16_16x16();
	   Show_String("PIPwindow1");

	   Font_Select_UserDefine_Mode();
	   Font_Select_12x24_24x24();	    
		//Goto_Text_XY(0,0);
	    Text_Mode();	
	  for(i=0;i<5;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;
	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	  }
	  Check_2D_Busy();
	  Graphic_Mode();



	  Active_Window_WH(800,600);

	  	i=rand()%65536;
		j=rand()%2;
	    resx1=rand()%700;
        resy1=rand()%500;

		resx2=rand()%700;
        resy2=rand()%500;

        Foreground_color_65k(i);
	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);

	    if(j)
		{
	    Start_Square();
		//delay_ms(100);
		//Main_Image_Start_Address(0);
		//delay_ms(500);
		}	
		else
		{
		Start_Square_Fill();
		//delay_ms(100);
		//Main_Image_Start_Address(800*600*3);
		//delay_ms(500);
		}

	  Canvas_Image_Start_address((1920*1080*2)+(800*600*2));

	  Select_SFI_1();
      Select_SFI_DMA_Mode();
      Select_SFI_32bit_Address();
      Select_SFI_Waveform_Mode_3();
      Select_SFI_0_DummyRead();	//normal read mode
      Select_SFI_Single_Mode();
      SPI_Clock_Period(0);

	  SFI_DMA_Transfer_Width_Height(800,480);
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();

	  Active_Window_XY(0,0);
	  Active_Window_WH(800,480);

	  Foreground_color_65k(color65k_red);
      Background_color_65k(color65k_blue);

	  Font_Width_X2();
      Font_Height_X2();

	  CGROM_Select_Genitop_FontROM();
      Font_Select_12x24_24x24();
	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
      Select_SFI_0_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(3);	  // Freq must setting <=20MHZ

	  Font_Background_select_Transparency();
	  Show_String("PIP2 浮動視窗2");

	  Font_Background_select_Color();
	  CGROM_Select_Internal_CGROM();
       Font_Select_8x16_16x16();
	   Show_String("PIP window2");

		Font_Select_12x24_24x24();
	    Font_Select_UserDefine_Mode(); 
		//Goto_Text_XY(0,0);
	    Text_Mode();	
	  for(i=0;i<5;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;
	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	  }
	  Check_2D_Busy();
	  Graphic_Mode();

	  	Active_Window_WH(800,600);

		i=rand()%65536;
        Foreground_color_65k(i);

		resx1=rand()%600;
        resy1=rand()%400;
		i=rand()%100;

		Circle_Center_XY(resx1,resy1);
		Circle_Radius_R(i);


		 //printf("x=%d",resx1);
		 //printf(" ");
		 //printf("y=%d",resy1);
		 //printf(" ");
         //printf("i=%d",i);
		 //printf(" ");

		j=rand()%3;
	    if(j)
		{
	    Start_Circle_or_Ellipse();
		//delay_ms(100);
		}
		else
		{
		Start_Circle_or_Ellipse_Fill();
		//delay_ms(100);
		}


	//test main window
	for(i=0;i<1920-800+1;i++)
	{
    Main_Window_Start_XY(i,0);
   	delay_ms(5);
	}
	delay_ms(100);

	for(i=0;i<100;i++)
	{
	 Select_PIP1_Parameter();
	 PIP_Display_Start_XY(120+i,120);
	 	delay_ms(5);
	}
	for(i=0;i<50;i++)
	{
	 Select_PIP2_Parameter();
	 PIP_Display_Start_XY(50+i,50);
	 	delay_ms(5);
	}

	for(j=0;j<1080-600+1;j++)
	{
    Main_Window_Start_XY(1120,j);
   	delay_ms(5);
	}
	delay_ms(100);

	for(i=0;i<100;i++)
	{
	 Select_PIP1_Parameter();
	 PIP_Display_Start_XY(120+i,120+i);
	 	delay_ms(5);
	}
	for(i=0;i<50;i++)
	{
	 Select_PIP2_Parameter();
	 PIP_Display_Start_XY(50+i,50+i);
	 	delay_ms(5);
	}

		
	for(i=1920-800;i>0;i--)
	{
    Main_Window_Start_XY(i,480);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,480);
	delay_ms(100);
	
	for(i=0;i<100;i++)
	{
	 Select_PIP1_Parameter();
	 PIP_Display_Start_XY(240-i,240-i);
	 	delay_ms(5);
	}
	for(i=0;i<50;i++)
	{
	 Select_PIP2_Parameter();
	 PIP_Display_Start_XY(100-i,100-i);
	 	delay_ms(5);
	}


    for(j=1080-600;j>0;j--)
	{
    Main_Window_Start_XY(0,j);
   	delay_ms(5);
	}
	Main_Window_Start_XY(0,0);
	delay_ms(100);







	 }
		   
   }	
}



void PIP_window_with_DMA_Draw_24bpp(void)
{
   unsigned int i,j,k;
	 unsigned int temp;
   unsigned long resx1,resy1,resx2,resy2,resx3,resy3;


 Host_Bus_16bit();


 //Font_Select_UserDefine_Mode();
 //CGROM_Select_Internal_CGROM();
 CGROM_Select_Genitop_FontROM();

 //Font_Select_8x16_16x16();
 Font_Select_12x24_24x24();
 //Font_Select_16x32_32x32();
 Internal_CGROM_Select_ISOIEC8859_1();
 //Internal_CGROM_Select_ISOIEC8859_2();
 //Internal_CGROM_Select_ISOIEC8859_3();
 //Internal_CGROM_Select_ISOIEC8859_4();

  //GTFont_Select_GT21L16TW_GT21H16T1W();
  //GTFont_Select_GT23L16U2W();
  GTFont_Select_GT23L24T3Y_GT23H24T3Y();
  //GTFont_Select_GT23L24M1Z();
  //GTFont_Select_GT23L32S4W_GT23H32S4W();
  //GTFont_Select_GT20L24F6Y();
  //GTFont_Select_GT21L24S1W();
  //GTFont_Select_GT22L16A1Y();

  Set_GTFont_Decoder(0x11);  //BIG5  

  // Enable_Font_Alignment();
  // Disable_Font_Alignment();
   Font_Background_select_Transparency();
  // Font_Background_select_Color();
  // Font_0_degree();
  // Font_90_degree();
   Font_Width_X1();
   //Font_Width_X2();
  // Font_Width_X3();
  // Font_Width_X4();
   Font_Height_X1();
   //Font_Height_X2();
  // Font_Height_X3();
  // Font_Height_X4();

   Enable_SFlash_SPI();

  Select_SFI_1();
  Select_SFI_DMA_Mode();
  Select_SFI_24bit_Address();

  //Select_SFI_Waveform_Mode_0();
  Select_SFI_Waveform_Mode_3();
  Select_SFI_0_DummyRead();	//normal read mode
  //Select_SFI_8_DummyRead(); //1byte dummy cycle
  //Select_SFI_16_DummyRead();
  //Select_SFI_24_DummyRead();
  Select_SFI_Single_Mode();
  //Select_SFI_Dual_Mode0();
  //Select_SFI_Dual_Mode1();
  SPI_Clock_Period(0);

  //set serial flash to 32bit address mode 

  //Select_nSS_drive_on_xnsfcs0();
  Select_nSS_drive_on_xnsfcs1();

  Reset_CPOL();
  //Set_CPOL();
  Reset_CPHA();
  //Set_CPHA();

  //Enter 4-byte mode  
  nSS_Active();
  SPI_Master_FIFO_Data_Put(0xB7);  //切換到32位址模式
   //while( EMTI_Flag()!=1);
  // Clear_EMTI_Flag();
   delay_ms(1); 
  nSS_Inactive();

  Foreground_color_16M(color16M_yellow);
  Background_color_16M(color16M_purple);

   Graphic_cursor_initial(); //initaial and enable graphic cursor 
   Text_cursor_initial();	//initaial and enable text cursor 

     CGRAM_Start_address(9100800);//	 
     Graphic_Mode();
	 Memory_Select_SDRAM();
	 //Memory_XY_Mode();//=block mode 如寫入CGRAM時active window 與canvas一樣大時就同等於linear mode
	 Memory_Linear_Mode();//linear mode ,當使用linear mode ,寫入與canvas,active windows 無關

	 Goto_Linear_Addr(9100800);
	RGB_16b_8bpp();
	Memory_8bpp_Mode();

	LCD_CmdWrite(0x04);	
	for(j=0; j<900;j++)
	 {	 
	 for(i=0; i<16;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(gImage_Font_8x16[i]);//寫入半型字 
		 Check_Mem_WR_FIFO_not_Full();
		}

		for(i=0; i<16;i++)	 //16*16/8		8bp寫入	 
		{
		 LCD_DataWrite(~gImage_Font_8x16[i]);//寫入半型字 
		 Check_Mem_WR_FIFO_not_Full();
	
		}
	   }
	  	
	  RGB_16b_24bpp_mode1(); //
	  Memory_XY_Mode();//初始化CGRAM完成後,切換為block mode
	  Memory_24bpp_Mode();//切換為原始的色深	


      //Disable_Text_Cursor();
    Enable_Text_Cursor_Blinking();
    //Disable_Text_Cursor_Blinking();
    Blinking_Time_Frames(10);
    //**[3E][3Fh]**//
    Text_Cursor_H_V(3,15);
	Enable_Text_Cursor();


 	Main_Image_Start_Address(0);					
	Main_Image_Width(1920);							

	Main_Window_Start_XY(0,0);//Display window start address
	Canvas_Image_Start_address(0);
	Canvas_image_width(1920);//

	Active_Window_XY(0,0);
	Active_Window_WH(800,600);

    Display_ON();
    Select_Main_Window_24bpp();
    RGB_16b_24bpp_mode1();
    Memory_24bpp_Mode();

		Select_SFI_32bit_Address();
	    SFI_DMA_Destination_Upper_Left_Corner(0,0);
        SFI_DMA_Transfer_Width_Height(1920,1080);
        SFI_DMA_Source_Width(1920);

        SFI_DMA_Source_Start_Address(6220800);//24bit address
        //SFI_DMA_Source_Start_Address(18664680);//32bit address

        Start_SFI_DMA();
        Check_Busy_SFI_DMA();

	Main_Image_Start_Address(0);
    Canvas_Image_Start_address(0);

	
   //test DMA falling 
    LCD_CmdWrite(0xB7);
	temp = LCD_DataRead();
    temp |= cSetb4;
	LCD_DataWrite(temp);
 
   while(1)
   {
 
	  Main_Window_Start_XY(0,0);
	  delay_ms(20);
	  Active_Window_XY(0,0);
	  Goto_Pixel_XY(0,0);
	  LCD_CmdWrite(0x04);
	  //sd_showpic_16M_mode1_bin(800,600,"p3.bin");

	  delay_ms(100);

	 Main_Window_Start_XY(0,0);
  	 delay_ms(100);

	 //寫入PIP1圖
	 Memory_24bpp_Mode();
	 delay_ms(20);
	 Canvas_Image_Start_address(1920*1080*3);
	 Canvas_image_width(800);//
     Active_Window_XY(0,0);
	 Active_Window_WH(320,240);
     Goto_Pixel_XY(0,0);
	 LCD_CmdWrite(0x04);
	 for(i=0;i<320*240/2;i++)
	 {
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
	  LCD_DataWrite(0x0000);
	  Check_Mem_WR_FIFO_not_Full();
 	 }
	  Check_Mem_WR_FIFO_Empty();

	//寫入PIP2圖
	Canvas_Image_Start_address((1920*1080*3)+(800*600*3));
	Canvas_image_width(800);//
    Active_Window_XY(0,0);
	Active_Window_WH(600,480);
    Goto_Pixel_XY(0,0);
	LCD_CmdWrite(0x04);
	for(i=0;i<600*480/2;i++)
	{
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
	 LCD_DataWrite(0xFFFF);
	 Check_Mem_WR_FIFO_not_Full();
 	}
	Check_Mem_WR_FIFO_Empty();

 	//PIP1
 	Select_PIP1_Window_24bpp();//	
	Select_PIP1_Parameter();
	PIP_Image_Start_Address(1920*1080*3);
	PIP_Image_Width(800);
	PIP_Window_Width_Height(320,240);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(60,60);

	Enable_PIP1();
	delay_seconds(1);

    Select_PIP2_Window_24bpp(); //注意		

	//PIP2
	Select_PIP2_Parameter();
	PIP_Image_Start_Address((1920*1080*3)+(800*600*3));
	PIP_Image_Width(800);
	PIP_Window_Width_Height(600,480);
	PIP_Window_Image_Start_XY(0,0);
	PIP_Display_Start_XY(50,50);

	Enable_PIP2();
	delay_seconds(1);

	Goto_Text_XY(0,0);
   while(1)
   {
   	  Color_Bar_ON();
	  delay_ms(1000);
      Display_OFF();
	  //delay_ms(1000);
	  delay_seconds(1);
  	  Canvas_Image_Start_address(1920*1080*3);

	/*
	Select_nSS_drive_on_xnsfcs1();
    Reset_CPOL();
    //Set_CPOL();
    Reset_CPHA();
    //Set_CPHA();

   //Enter 4-byte mode  
   nSS_Active();
   SPI_Master_FIFO_Data_Put(0xB7);  //切換到32位址模式
    //while( EMTI_Flag()!=1);
   // Clear_EMTI_Flag();
    delay_ms(1); 
   nSS_Inactive();

	  Select_SFI_1();
      Select_SFI_DMA_Mode();
      Select_SFI_32bit_Address();
      Select_SFI_Waveform_Mode_3();
      Select_SFI_0_DummyRead();	//normal read mode
      Select_SFI_Single_Mode();
      SPI_Clock_Period(1);

	  SFI_DMA_Transfer_Width_Height(320,240);
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();
	  */
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,600);

      Foreground_color_16M(color16M_yellow);
      Background_color_16M(color16M_purple);


	  Font_Width_X2();
      Font_Height_X2();
	  CGROM_Select_Genitop_FontROM();
      Font_Select_12x24_24x24();
	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
     Select_SFI_0_DummyRead();
	  // Select_SFI_8_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(2);	 // Freq must setting <=20MHZ
	   Goto_Text_XY(0,0);
	  Show_String("PIP1浮動視窗1");

	   CGROM_Select_Internal_CGROM();
       Font_Select_8x16_16x16();
	   Show_String("PIPwindow1");

	    Font_Select_UserDefine_Mode(); 
		//Goto_Text_XY(0,0);
	    Text_Mode();	
	  for(i=0;i<5;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;
	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	  }
	  Check_2D_Busy();
	  Graphic_Mode();


	    Active_Window_WH(800,600);

	  	i=rand()%16777216;
		j=rand()%2;
	    resx1=rand()%700;
        resy1=rand()%500;

		resx2=rand()%700;
        resy2=rand()%500;

        Foreground_color_16M(i);
	    Square_Start_XY(resx1,resy1);
		Square_End_XY(resx2,resy2);

	    if(j)
		{
	    Start_Square();
		delay_ms(100);
		//Main_Image_Start_Address(0);
		//delay_ms(500);
		}	
		else
		{
		Start_Square_Fill();
		delay_ms(100);
		//Main_Image_Start_Address(800*600*3);
		//delay_ms(500);
		}

	  Canvas_Image_Start_address((1920*1080*3)+(800*600*3));

	  /*
	  Select_nSS_drive_on_xnsfcs1();
       Reset_CPOL();
      //Set_CPOL();
      Reset_CPHA();
      //Set_CPHA(); 
      nSS_Active();
      SPI_Master_FIFO_Data_Put(0xE9);  //切換到24位址模式
      while( EMTI_Flag()!=1);
      Clear_EMTI_Flag();
      nSS_Inactive();
   	
	  Select_SFI_1();
      Select_SFI_DMA_Mode();
	  Select_SFI_24bit_Address();
      //Select_SFI_32bit_Address();
      Select_SFI_Waveform_Mode_3();
      Select_SFI_0_DummyRead();	//normal read mode
      Select_SFI_Single_Mode();
      SPI_Clock_Period(1);

	  SFI_DMA_Transfer_Width_Height(800,480);
	  Start_SFI_DMA();
      Check_Busy_SFI_DMA();
	  */
	  Active_Window_XY(0,0);
	  Active_Window_WH(800,480);

	  Foreground_color_16M(color16M_red);
      Background_color_16M(color16M_blue);


	  Font_Width_X2();
      Font_Height_X2();

	  CGROM_Select_Genitop_FontROM();
      Font_Select_12x24_24x24();
	  Select_SFI_0();
      Select_SFI_Font_Mode();
      Select_SFI_24bit_Address();
      Select_SFI_Waveform_Mode_0();
      Select_SFI_0_DummyRead();
	  //Select_SFI_8_DummyRead();
      Select_SFI_Single_Mode();
      SPI_Clock_Period(2);	  // Freq must setting <=20MHZ

	  Show_String("PIP2 浮動視窗2");

	  CGROM_Select_Internal_CGROM();
       Font_Select_8x16_16x16();
	   Show_String("PIP window2");

	    Font_Select_UserDefine_Mode(); 
		//Goto_Text_XY(0,0);
	    Text_Mode();	
	  for(i=0;i<5;i++)
	  {							//寫出半型字 字碼由0x0000開始
	   temp=i;
	   LCD_CmdWrite(0x04);
	   LCD_DataWrite(temp>>8);
	   Check_Mem_WR_FIFO_not_Full();
	   LCD_DataWrite(temp);
	   Check_Mem_WR_FIFO_not_Full();
	  }
	  Check_2D_Busy();
	  Graphic_Mode();


		 /*
	  	  Active_Window_WH(800,600);

		  i=rand()%16777216;
		j=rand()%3;

	    resx1=rand()%700;
        resy1=rand()%500;
		resx2=rand()%700;
        resy2=rand()%500;
		resx3=rand()%700;
        resy3=rand()%500;
		
        Foreground_color_16M(i);
        Triangle_Point1_XY(resx1,resy1);
		Triangle_Point2_XY(resx2,resy2);
		Triangle_Point3_XY(resx3,resy3);
	
	    if(j)
		{
		 //printf("%d",j);
		 Start_Triangle();
		 delay_ms(100);
		}		  
		else
		{
		 //printf("%d",j);
		 Start_Triangle_Fill();
		 delay_ms(100);
		}
		 */
        Active_Window_WH(800,600);

		i=rand()%65536;
        Foreground_color_65k(i);

		resx1=rand()%600;
        resy1=rand()%400;
		i=rand()%100;

		Circle_Center_XY(resx1,resy1);
		Circle_Radius_R(i);


		 //printf("x=%d",resx1);
		 //printf(" ");
		 //printf("y=%d",resy1);
		 //printf(" ");
         //printf("i=%d",i);
		 //printf(" ");

		j=rand()%3;
	    if(j)
		{
	    Start_Circle_or_Ellipse();
		delay_ms(100);
		}
		else
		{
		Start_Circle_or_Ellipse_Fill();
		delay_ms(100);
		}

    Display_ON();
	delay_ms(20);

	Color_Bar_OFF();
	delay_ms(20);

	//delay_seconds(1);

    //test main window
	for(i=0;i<1920-800+1;i++)
	{
    Main_Window_Start_XY(i,0);
   	delay_ms(1);
	}
	delay_ms(100);


	
	for(j=0;j<1080-600+1;j++)
	{
    Main_Window_Start_XY(1120,j);
   	delay_ms(1);
	}
	delay_ms(100);

		
	for(i=1920-800;i>0;i--)
	{
    Main_Window_Start_XY(i,480);
   	delay_ms(1);
	}
	Main_Window_Start_XY(0,480);
	delay_ms(100);
	
    for(j=1080-600;j>0;j--)
	{
    Main_Window_Start_XY(0,j);
   	delay_ms(1);
	}
	Main_Window_Start_XY(0,0);
	delay_ms(100);


	 }
		   
   }	
}





void Tom_test_Read_Write_16bit_16bpp (void)
{
   	unsigned int i,j,k,temp;

   		Memory_16bpp_Mode();
	    Select_Main_Window_16bpp();
	    Host_Bus_16bit();
	    RGB_16b_16bpp();
		MemRead_Left_Right_Top_Down();
        MemWrite_Left_Right_Top_Down();
	    Graphic_Mode();

LCD_CmdWrite(0x85);
LCD_DataWrite(0x03);



        Main_Window_Start_XY(0,0);
	    Active_Window_XY(0,0);
	    Active_Window_WH(400,480); 					//10張後剩下508
		Goto_Pixel_XY(0,0);
	 	LCD_CmdWrite(0x04);

	for(i=0;i<400;i++)
		{
		LCD_DataWrite(0x07e0);
		Check_Mem_WR_FIFO_not_Full();
		}

for(j=0;j<478;j++)
{
	
		LCD_DataWrite(0x07e0);
		Check_Mem_WR_FIFO_not_Full();
    for(i=0;i<398;i++)
		 {

		LCD_DataWrite(0xf800);
		Check_Mem_WR_FIFO_not_Full();
		 }	
		LCD_DataWrite(0x07e0);
		Check_Mem_WR_FIFO_not_Full();
}
	for(i=0;i<400;i++)
		{
		LCD_DataWrite(0x07e0);
		Check_Mem_WR_FIFO_not_Full();
		}
	Check_Mem_WR_FIFO_Empty();


//*************************************************************************


   while(1)
   {
		Graphic_Mode();
        Main_Window_Start_XY(0,0);
	    Active_Window_XY(400,0);
	    Active_Window_WH(400,480); 					//10張後剩下508
		Goto_Pixel_XY(400,0);
	 	LCD_CmdWrite(0x04);

	for(i=0;i<400;i++)
		{
		LCD_DataWrite(0x07e0);
		Check_Mem_WR_FIFO_not_Full();
		}

    for(j=0;j<478;j++)
    {
	
		LCD_DataWrite(0x07e0);
		Check_Mem_WR_FIFO_not_Full();
         for(i=0;i<398;i++)
		 {
		 LCD_DataWrite(0xf800);
		 Check_Mem_WR_FIFO_not_Full();		 
		 }
		LCD_DataWrite(0x07e0);
		Check_Mem_WR_FIFO_not_Full();
    }

	for(i=0;i<400;i++)
		{
		LCD_DataWrite(0x07e0);
		Check_Mem_WR_FIFO_not_Full();
		}

	   Check_Mem_WR_FIFO_Empty();


		printf("\r\n Read test\r\n");
		Graphic_Mode();
	   	Main_Window_Start_XY(0,0);
	    Active_Window_XY(0,0);
	    Active_Window_WH(400,480); 	
		Goto_Pixel_XY(0,0);
		LCD_CmdWrite(0x04);
		temp = LCD_DataRead();

        Check_Mem_RD_FIFO_not_Empty();  //dummy	
		 	
		for(i=0;i<400;i++)
		{
	   	temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0x07e0)
		{
		Text_Mode();
		Goto_Text_XY(0,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0x07e0);
		printf("\r\n Read error\r\n");
		while(1);
		}
		}


		for(j=0;j<478;j++)
		{
	   	temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0x07e0)
		{
		Text_Mode();
		Goto_Text_XY(0,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0x07e0);
		printf("\r\n Read error\r\n");
		while(1);
		}
		
		
		for(i=0;i<398;i++)
		{
		temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0xf800)
		{
		Text_Mode();
		Goto_Text_XY(0,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0xf800);
		printf("\r\n Read error\r\n");
		while(1);
		}
		}

		temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0x07e0)
		{
		Text_Mode();
		Goto_Text_XY(0,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0x07e0);
		printf("\r\n Read error\r\n");
		while(1);
		}
		}
					 
		for(i=0;i<400;i++)
		{
		temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0x07e0)
		{
		Text_Mode();
		Goto_Text_XY(0,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0x07e0);
		printf("\r\n Read error\r\n");
		while(1);
		}
		}
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
//   	 	Text_Mode();
//		Goto_Text_XY(200,200);
//		LCD_CmdWrite(0x04);
//		LCD_DataWrite(0x4b);
//		Check_2D_Busy();



		Graphic_Mode();
		printf("\r\n Write test\r\n");
		Graphic_Mode();
 	   	Main_Window_Start_XY(0,0);
	    Active_Window_XY(400,0);
	    Active_Window_WH(400,480); 	
		Goto_Pixel_XY(400,0);
		LCD_CmdWrite(0x04);
		temp = LCD_DataRead();
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
	 	for(i=0;i<400;i++)
		{
	   	temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0x07e0)
		{
		Text_Mode();
		Goto_Text_XY(400,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0x07e0);
		printf("\r\n Write error\r\n");
		while(1);
		}
		}


		for(j=0;j<478;j++)
		{
	   	temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0x07e0)
		{
		Text_Mode();
		Goto_Text_XY(400,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0x07e0);
		printf("\r\n Write error\r\n");
		while(1);
		}
		
		
		for(i=0;i<398;i++)
		{
		temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0xf800)
		{
		Text_Mode();
		Goto_Text_XY(400,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0xf800);
		printf("\r\n Write error\r\n");
		while(1);
		}
		}
		temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		if(temp!=0x07e0)
		{
		Text_Mode();
		Goto_Text_XY(400,0);
		LCD_CmdWrite(0x04);
		Print_Hex_4(temp);
		Print_Hex_4(0x07e0);
		printf("\r\n Write error\r\n");
		while(1);
		}
		}
		
					 
		for(i=0;i<400;i++)
		{
		temp = LCD_DataRead();		
		 Check_Mem_RD_FIFO_not_Empty();  //dummy	
		 if(temp!=0x07e0)
		 {
		 Text_Mode();
		 Goto_Text_XY(400,0);
		 LCD_CmdWrite(0x04);
		 Print_Hex_4(temp);
		 Print_Hex_4(0x07e0);
		 printf("\r\n Write error\r\n");
		 while(1);
		 }
		}

		Check_Mem_WR_FIFO_Empty();	

   	 	Text_Mode();
		Goto_Text_XY(600,200);
		LCD_CmdWrite(0x04);
		LCD_DataWrite(0x4b);
		Check_2D_Busy();

	 }
}
