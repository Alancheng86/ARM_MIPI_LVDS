#ifndef _SPI_FLASH_H_
#define _SPI_FLASH_H_  

#include "stm32f10x.h"

//#include "stm32f10x_lib.h"

//#define SPI1_USE


//#ifdef  SPI1_USE
#define W25X_FLASH_SPI SPI1
#define W25X_FLASH_SPI_CLK RCC_APB1Periph_SPI1
#define W25X_FLASH_SPI_SCK_PIN GPIO_Pin_4 /* PA.4 */
#define W25X_FLASH_SPI_SCK_GPIO_PORT GPIOA /* GPIOA */
#define W25X_FLASH_SPI_SCK_GPIO_CLK RCC_APB2Periph_GPIOA
#define W25X_FLASH_SPI_MISO_PIN GPIO_Pin_6 /* PA.6 */
#define W25X_FLASH_SPI_MISO_GPIO_PORT GPIOA /* GPIOA */
#define W25X_FLASH_SPI_MISO_GPIO_CLK RCC_APB2Periph_GPIOA
#define W25X_FLASH_SPI_MOSI_PIN GPIO_Pin_7 /* PA.7 */
#define W25X_FLASH_SPI_MOSI_GPIO_PORT GPIOA /* GPIOA */
#define W25X_FLASH_SPI_MOSI_GPIO_CLK RCC_APB2Periph_GPIOA
#define W25X_FLASH_CS_PIN GPIO_Pin_4 /* PA.4 */
#define W25X_FLASH_CS_GPIO_PORT GPIOA /* GPIOA */
#define W25X_FLASH_CS_GPIO_CLK RCC_APB2Periph_GPIOA
//#endif

/*
Chip		Uniform Sector Erase
		20/21	52	D8/DC
		-----	--	-----
W25Q64CV	4	32	64
W25Q128FV	4	32	64
S25FL127S			64
N25Q512A	4		64
N25Q00AA	4		64
S25FL512S			256
SST26VF032	4
*/



//			size	sector			busy	pgm/erase	chip
// Part			Mbyte	kbyte	ID bytes	cmd	suspend		erase
// ----			----	-----	--------	---	-------		-----
// Winbond W25Q64CV	8	64	EF 40 17
// Winbond W25Q128FV	16	64	EF 40 18	05	single		60 & C7
// Winbond W25Q256FV	32	64	EF 40 19	
// Spansion S25FL064A	8	?	01 02 16
// Spansion S25FL127S	16	64	01 20 18	05
// Spansion S25FL128P	16	64	01 20 18
// Spansion S25FL256S	32	64	01 02 19	05			60 & C7
// Spansion S25FL512S	64	256	01 02 20
// Macronix MX25L12805D 16	?	C2 20 18
// Macronix MX66L51235F	64		C2 20 1A
// Numonyx M25P128	16	?	20 20 18
// Micron M25P80	1	?	20 20 14
// Micron N25Q128A	16	64	20 BA 18
// Micron N25Q512A	64	?	20 BA 20	70	single		C4 x2
// Micron N25Q00AA	128	64	20 BA 21		single		C4 x4
// Micron MT25QL02GC	256	64	20 BA 22	70			C4 x2
// SST SST25WF010	1/8	?	BF 25 02
// SST SST25WF020	1/4	?	BF 25 03
// SST SST25WF040	1/2	?	BF 25 04
// SST SST25VF016B	1	?	BF 25 41
// SST26VF016			?	BF 26 01
// SST26VF032			?	BF 26 02
// SST25VF032		4	64	BF 25 4A
// SST26VF064		8	?	BF 26 43
// LE25U40CMC		1/2	64	62 06 13
 
 

//W25Q64BV
#define W25Q64_DeviceID 0xEF16
 
#define W25X_CMD_WriteEnable 0x06 /*Write enable instruction */
#define W25X_CMD_WriteDisable 0x04 /*! Write to Memory Disable */
#define W25X_CMD_WriteStatusReg 0x01 /* Write Status Register instruction */
 
#define W25X_CMD_PageProgram 0x02 /* Write enable instruction */
#define W25X_CMD_QuadPageProgram 0x32 /* Write enable instruction */
 
#define W25X_CMD_BlockErase64 0xD8 /* Block 64k Erase instruction */
#define W25X_CMD_BlockErase32 0x52 /* Block 32k Erase instruction */
#define W25X_CMD_ChipErase 0xC7 /* Bulk Erase instruction */
#define W25X_CMD_SectorErase 0x20 /* Sector 4k Erase instruction */
#define W25X_CMD_EraseSuspend 0x75 /* Sector 4k Erase instruction */
#define W25X_CMD_EraseResume 0x7a /* Sector 4k Erase instruction */
 
#define W25X_CMD_ReadStatusReg1 0x05 /* Read Status Register instruction */
#define W25X_CMD_ReadStatusReg2 0x35 /* Read Status Register instruction */
 
#define W25X_CMD_High_Perform_Mode 0xa3
#define W25X_CMD_Conti_Read_Mode_Ret 0xff
 
#define W25X_WakeUp 0xAB
#define W25X_JedecDeviveID 0x9F /*Read identification */
#define W25X_ManufactDeviveID 0x90 /* Read identification */
#define W25X_ReadUniqueID 0x4B
 
#define W25X_Power_Down 0xB9 /*Sector 4k Erase instruction */
 
#define W25X_CMD_ReadData 0x03 /* Read from Memory instruction */
#define W25X_CMD_FastRead 0x0b /* Read from Memory instruction */
#define W25X_CMD_FastReadDualOut 0x3b /*Read from Memory instruction */
#define W25X_CMD_FastReadDualIO 0xBB /* Read from Memory instruction */
#define W25X_CMD_FastReadQuadOut 0x6b /* Read from Memory instruction */
#define W25X_CMD_FastReadQuadIO 0xeb /* Read from Memory instruction */
#define W25X_CMD_OctalWordRead 0xe3 /* Read from Memory instruction */
 
#define W25X_DUMMY_BYTE 0xff //0xA5
#define W25X_SPI_PAGESIZE 0x100

#define Dummy_Byte 0xff //0xA5 0xA5

///////////////////////-------------add  2016/05/03  alan
#define W25Qxx_Read_Status_Register			0x05
#define W25Qxx_Erase_Block64Kb				0xd8	//擦除一个块，64Kb
#define W25Qxx_Erase_Block32Kb				0x52	//擦除一个块，32Kb
#define W25Qxx_Erase_Sector4Kb				0x20	//擦除一个扇区，4Kb


/* Select SPI FLASH: ChipSelect pin low  */
#define Flash_cs_low()     GPIO_ResetBits(GPIOA, GPIO_Pin_4)
/* Deselect SPI FLASH: ChipSelect pin high */
#define Flash_cs_high()    GPIO_SetBits(GPIOA, GPIO_Pin_4)

//#define FLASH_CHIP MX25L8
//#define FLASH_CHIP MX25L16
//#define FLASH_CHIP MX25L32
//#define FLASH_CHIP MX25L64
//#define FLASH_CHIP MX25L128

#define MX25L8		1048576 //memory size
#define MX25L16		2097152 //memory size
#define MX25L32		4194304 //memory size
#define MX25L64 	8388608 //memory size
#define MX25L128   16777216 //memory size

#define mem_size 1024

void SPI_Peripheral_Init(void);	        //SPI﹍て
u8 SPI_Peripheral_ReadByte(void);	    //FLASH巨膀セㄧ计弄竊
u8 SPI_Peripheral_SendByte(u8 byte);	//FLASH巨膀セㄧ计祇癳竊


void WREN(void);
void WRDI(void);
void WRSR(unsigned char UpdataValue);

u8 WIP(void);
u8 WEL(void);
u8 RDSR(void);

void WRITE_DATA_BYTE(u32 address,u8 UpdataValue);
void WRITE_DATA_PAGE(u32 address,u8 *UpdataValue);

u8 READ_DATA_BYTE(u32 address);
void CHIP_ERASE(void);
void RDID(void);
u8 FLASH_ERASE(void);
int SPI_TEST(void);


u16 W25Qxx_Read_ID(void);	//读取W25Qxx序列号（ID）

void W25Qxx_Enter_Power_Down(void);	//W25Qxx进入掉电模式

void W25Qxx_Exit_Power_Down(void);	//唤醒W25Qxx

u8 W25Qxx_Read_Register_Status(u8 REG_CMD);	//W25Qxx读取状态寄存器

void W25Qxx_Wait_Busy(void);	//等待W25Qxx忙完

void W25Qxx_Write_Register_Status(u8 RS_dat);	//W25Qxx写状态寄存器

void W25Qxx_EraseSector(u32 Sectoradd,u8 ORD);	//W25Qxx擦除一个扇区4K，或者擦除一个块

void W25Qxx_EraseChip(void);	//W25Qxx擦除整个芯片

void W25Qxx_Write_Sector(u32 writeadd,u8* wbuffer,u16 sum);	//W25Qxx写扇区数据


void W25Qxx_Write_Page(u32 startadd,u8* wbuffer,u16 num);	//W25Qxx在一个页内写入数据

void W25Qxx_Flash_ReadData(u8* rbuffer,u32 startadd,u16 num);	//W25Qxx读数据
void W25Qxx_Flash_WriteData(u8* pdbuffer,u32 writeadd,u16 sum);	//W25Qxx写数据



#endif


