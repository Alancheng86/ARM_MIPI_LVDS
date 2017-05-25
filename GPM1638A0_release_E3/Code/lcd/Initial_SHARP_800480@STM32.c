#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "delay.h"

/************************************************************
設定 LQ043Y1DX07_SPI 的 GPIO
define	LCD_SDI		PD_6
define	LCD_SCK		PE_2
define	LCD_CSX		PE_4
define	LCM1_RESX	PE_6
************************************************************/
#define LCD_SDI_L	GPIO_ResetBits(GPIOD, GPIO_Pin_6);
#define LCD_SDI_H	GPIO_SetBits(GPIOD, GPIO_Pin_6);
  
#define LCD_SCK_L	GPIO_ResetBits(GPIOE, GPIO_Pin_2);
#define LCD_SCK_H	GPIO_SetBits(GPIOE, GPIO_Pin_2);

#define LCD_CSX_L	GPIO_ResetBits(GPIOE, GPIO_Pin_4);
#define LCD_CSX_H	GPIO_SetBits(GPIOE, GPIO_Pin_4);

#define LCM1_RESX_L	GPIO_ResetBits(GPIOE, GPIO_Pin_6);
#define LCM1_RESX_H	GPIO_SetBits(GPIOE, GPIO_Pin_6);


/*>>>>>>>>>>>>>>>>>>>>>>>>*/
extern void delay_ms(u16 nms);
extern void delay_us(u32 Nus);

/*------------------------*/
void Reset_LQ043Y1DX07(void);
void LCD_SPI_write(unsigned char DATA);
void LCD_SPI_Set_CMD(void);
void LCD_SPI_Set_DAT(void);
void LCD_SPI_Start(void);
void LCD_SPI_End(void);
/*------------------------*/
void IniGPIO_for_LQ043Y1DX07(void);
void Initial_LQ043Y1DX07(void);
/*------------------------*/

	
/*------------------------*/

void IniGPIO_for_LQ043Y1DX07(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Reset GPIO init structure parameters values */
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_Out_PP;   //通用推挽輸出
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Reset GPIO init structure parameters values */
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2|GPIO_Pin_4|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode =   GPIO_Mode_Out_PP;   //通用推挽輸出
	GPIO_Init(GPIOE, &GPIO_InitStructure);
}

void Initial_LQ043Y1DX07(void)
{

	IniGPIO_for_LQ043Y1DX07();//設置 GPIO 給 LQ043Y1DX07_SPI


	//Hardware Reset >10us
	Reset_LQ043Y1DX07();


	LCD_SPI_Start();
//(1)
	//Sleep Out / write command 0x11
	LCD_SPI_Set_CMD();
	LCD_SPI_write(0x11);
	LCD_SPI_End();
	delay_ms(120);
	
//(2)	//Set_EXTC : Use RGB=888 Setting
	//(2.1)
	LCD_SPI_Start();

		LCD_SPI_Set_CMD();
		LCD_SPI_write(0xB9);
	
		LCD_SPI_Set_DAT();
		LCD_SPI_write(0xFF);

		LCD_SPI_Set_DAT();
		LCD_SPI_write(0x83);
	
		LCD_SPI_Set_DAT();
		LCD_SPI_write(0x63);
	
	LCD_SPI_End();
	
	//(2.2)
	LCD_SPI_Start();
	
		LCD_SPI_Set_CMD();
		LCD_SPI_write(0x3A);
	
		LCD_SPI_Set_DAT();
		LCD_SPI_write(0x70);
	
	LCD_SPI_End();

//(3)//Display On
	LCD_SPI_Start();
		LCD_SPI_Set_CMD();
		LCD_SPI_write(0x29);
	
		LCD_SPI_Set_DAT();
		LCD_SPI_write(0xFF);
	
	LCD_SPI_End();

//(4)//END

	delay_ms(120);
}


/*------------------------*/
void Reset_LQ043Y1DX07(void)
{
	LCM1_RESX_H;
	delay_ms(10);
	LCM1_RESX_L; 
	delay_ms(3);
	LCM1_RESX_H;
	delay_ms(130);	
}

void LCD_SPI_Start(void)
{
	//
	LCD_CSX_H;
	LCD_SCK_H;
	LCD_SDI_H;

	delay_us(1);
	//
	LCD_CSX_L;	

	delay_us(10);	
	
}
void LCD_SPI_Set_CMD(void)
{
	//
	LCD_SDI_L;
	//
	LCD_SCK_L;
		delay_us(1);
	LCD_SCK_H;
		delay_us(1);
}
void LCD_SPI_Set_DAT(void)
{
	//
	LCD_SDI_H;
	//
	LCD_SCK_L;
		delay_us(1);
	LCD_SCK_H;
		delay_us(1);
}
void LCD_SPI_End(void)
{
	LCD_CSX_H;
	LCD_SCK_H;
	LCD_SDI_H;
		delay_us(10);
}
void LCD_SPI_write(unsigned char DATA)
{
    unsigned char i;

	for(i=0 ; i<8 ; i++)
	{
	 	if(DATA & 0x80)
		{	LCD_SDI_H;}
		else
		{	LCD_SDI_L;}

		DATA <<= 1;
		LCD_SCK_L;	
                delay_us(1);
		LCD_SCK_H;
                delay_us(1);
	 }
}
