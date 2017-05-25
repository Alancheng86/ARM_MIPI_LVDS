
/*REG[01h] bit0*/
void Host_8bit_data_bus_test_8bpp(void);
void Host_8bit_data_bus_test_16bpp(void);
void Host_8bit_data_bus_test_24bpp(void);
void Host_16bit_data_bus_test_16bpp(void);
void Host_16bit_data_bus_test_24bpp_mode1(void);
void Host_16bit_data_bus_test_24bpp_mode2(void);

void Memory_Write_Read_Direction_test_LRTD_8bpp_to_8bpp(void);
void Memory_Write_Read_Direction_test_RLTD_8bpp_to_8bpp(void);
void Memory_Write_Read_Direction_test_DTLR_8bpp_to_8bpp(void);
void Memory_Write_Read_Direction_test_TDLR_8bpp_to_8bpp(void);

void Memory_Write_Read_Direction_test_LRTD_16bpp_to_8bpp(void);
void Memory_Write_Read_Direction_test_RLTD_16bpp_to_8bpp(void);
void Memory_Write_Read_Direction_test_DTLR_16bpp_to_8bpp(void);
void Memory_Write_Read_Direction_test_TDLR_16bpp_to_8bpp(void);

void Memory_Write_Read_Direction_test_LRTD_8bpp_to_16bpp(void);
void Memory_Write_Read_Direction_test_RLTD_8bpp_to_16bpp(void);
void Memory_Write_Read_Direction_test_DTLR_8bpp_to_16bpp(void);
void Memory_Write_Read_Direction_test_TDLR_8bpp_to_16bpp(void);

void Memory_Write_Read_Direction_test_LRTD_16bpp_to_16bpp(void);
void Memory_Write_Read_Direction_test_RLTD_16bpp_to_16bpp(void);
void Memory_Write_Read_Direction_test_DTLR_16bpp_to_16bpp(void);
void Memory_Write_Read_Direction_test_TDLR_16bpp_to_16bpp(void);

void Memory_Write_Read_Direction_test_LRTD_8bpp_to_24bpp(void);
void Memory_Write_Read_Direction_test_RLTD_8bpp_to_24bpp(void);
void Memory_Write_Read_Direction_test_DTLR_8bpp_to_24bpp(void);
void Memory_Write_Read_Direction_test_TDLR_8bpp_to_24bpp(void);

void Memory_Write_Read_Direction_test_LRTD_16bpp_to_24bpp(void);
void Memory_Write_Read_Direction_test_RLTD_16bpp_to_24bpp(void);
void Memory_Write_Read_Direction_test_DTLR_16bpp_to_24bpp(void);
void Memory_Write_Read_Direction_test_TDLR_16bpp_to_24bpp(void);

/*REG[03h]ICR*/
void Graphic_Cursor_test_8bpp(void);
void Graphic_Cursor_test_16bpp(void);
void Graphic_Cursor_test_24bpp(void);

/*REG[0B][0C][0D]*/
void Resume_Interrupt_test(void);
void External_Interrupt_Input_test(void);
void I2CM_Interrupt_test(void);
void Vsync_Interrupt_test(void);
void KeyScan_Interrupt_test(void);
void DMA_Draw_BTE_Interrupt_test(void);
void PWM1_Interrupt_test(void);
void PWM0_Interrupt_test(void);

/*REG[11h] */
void PIP_window_test_8bpp(void);
void PIP_window_test_16bpp(void);
void PIP_window_test_24bpp(void);

/*REG[12h] DPCR */
void PCLK_Inversion_test(void);
void Display_Off_test(void);
void Display_Color_Bar_test(void);
void Scan_Direction_test(void);
void Parallel_PDATA_Output_Sequence(void);

/*REG[13h] PCSR*/
void SYNC_Polarity_IdleState_test(void);

/*REG[3Ch][3Dh]*/
void Text_cursor_test_8bpp(void);
void Text_cursor_test_16bpp(void);
void Text_cursor_test_24bpp(void);


/*5Eh*/ 
void Read_modify_write_8bpp(void);
void Read_modify_write_16bpp(void);
void Read_modify_write_24bpp(void);
void linear_mode_test_8bit_8bpp(void);
void linear_mode_test_8bit_16bpp(void);
void linear_mode_test_8bit_24bpp(void);


 /*REG[67h~7Eh]*/
//=======================================================================
void Draw_line_test_8bpp(void);
void Draw_line_test_16bpp(void);
void Draw_line_test_24bpp(void);
void Draw_square_test_8bpp(void);
void Draw_square_test_16bpp(void);
void Draw_square_test_24bpp(void);
void Draw_circle_square_test_8bpp(void);
void Draw_circle_square_test_16bpp(void);
void Draw_circle_square_test_24bpp(void);

void Draw_triangle_test_8bpp(void);
void Draw_triangle_test_16bpp(void);
void Draw_triangle_test_24bpp(void);

void Draw_circle_test_8bpp(void);
void Draw_circle_test_16bpp(void);
void Draw_circle_test_24bpp(void);
void Draw_Ellipse_test_8bpp(void);
void Draw_Ellipse_test_16bpp(void);
void Draw_Ellipse_test_24bpp(void);

/*REG[85~8F]*/
void PWM0_test(void);
void PWM1_test(void);
void PWM_test(void);

void DMA_Init_pattern_8bpp_8x8(void);
void DMA_Init_pattern_8bpp_16x16(void);
void DMA_Init_pattern_16bpp_8x8(void);
void DMA_Init_pattern_16bpp_16x16(void);
void DMA_Init_pattern_24bpp_8x8(void);
void DMA_Init_pattern_24bpp_16x16(void);





/*REG[90h]*/
void BTE_write_with_ROP_8bpp_test_Destination(void);
void BTE_write_with_ROP_8bpp_test_with_S1_to_Destination(void);
void BTE_Move_with_ROP_8bpp_test_positive_move(void);
void BTE_Transparent_Write_test_8bpp(void);
void BTE_Transparent_Move_test_8bpp(void);
void BTE_pattern_Fill_test_8bpp_8x8(unsigned char t);
void BTE_pattern_Fill_test_8bpp_16x16(unsigned char t);
void BTE_Color_Expansion_test_8bpp(unsigned char t);
void BTE_Alpha_blending_8bpp_to_8bpp(void);
void BTE_Alpha_blending_8bpp_to_16bpp(void);
void BTE_MPU_Write_Alpha_blending_8bpp_to_8bpp(void);
void BTE_MPU_Write_Alpha_blending_8bpp_to_16bpp(void);
void BTE_Solid_Fill_test_8bpp(void);

void BTE_move_Alpha_blending_by_picture_8bpp(void);
void BTE_write_Alpha_blending_by_picture_8bpp(void);

void BTE_write_with_ROP_16bpp_test_Destination(void);
void BTE_write_with_ROP_16bpp_test_S1_to_Destination(void);
void BTE_Move_with_ROP_16bpp_test_positive_move(void);
void BTE_Transparent_Write_test_16bpp(void);
void BTE_Transparent_Move_test_16bpp(void);
void BTE_pattern_Fill_test_16bpp_8x8(unsigned char t);
void BTE_pattern_Fill_test_16bpp_16x16(unsigned char t);
void BTE_Color_Expansion_test_16bpp(unsigned char t);
void BTE_Alpha_blending_16bpp_to_16bpp(void);
void BTE_Alpha_blending_16bpp_to_8bpp(void);
void BTE_MPU_Write_Alpha_blending_16bpp_to_8bpp(void);
void BTE_MPU_Write_Alpha_blending_16bpp_to_16bpp(void);
void BTE_Solid_Fill_test_16bpp(void);
	
void BTE_move_Alpha_blending_by_picture_16bpp(void);
void BTE_write_Alpha_blending_by_picture_16bpp(void);


void BTE_write_with_ROP_24bpp_test_Destination(void);
void BTE_write_with_ROP_24bpp_test_S1_to_Destination(void);
void BTE_Move_with_ROP_24bpp_test_positive_move(void);
void BTE_Transparent_Write_test_24bpp(void);
void BTE_Transparent_Move_test_24bpp(void);
void BTE_pattern_Fill_test_24bpp_8x8(unsigned char t);
void BTE_pattern_Fill_test_24bpp_16x16(unsigned char t);
void BTE_Color_Expansion_test_24bpp(unsigned char t);
void BTE_Alpha_blending_24bpp_to_16bpp(void);
 void BTE_Alpha_blending_24bpp_to_8bpp(void);
 void BTE_MPU_Write_Alpha_blending_24bpp_to_8bpp(void);
 void BTE_MPU_Write_Alpha_blending_24bpp_to_16bpp(void);
 void BTE_Solid_Fill_test_24bpp(void);
 void BTE_move_Alpha_blending_by_picture_24bpp(void);
 void BTE_write_Alpha_blending_by_picture_24bpp(void);


/*REG[B8h]*/
void SPIM_WREN(void);
void SPIM_WRDI(void);
void SPIM_WRSR(unsigned char UpdataValue);

unsigned char SPIM_WIP(void);
unsigned char SPIM_WEL(void);
unsigned char SPIM_RDSR(void);

void SPIM_WRITE_DATA_BYTE(unsigned long address,unsigned char UpdataValue);
void SPIM_WRITE_DATA_PAGE(unsigned long address,unsigned char *UpdataValue);

unsigned char SPIM_READ_DATA_BYTE(unsigned long address);
void SPIM_CHIP_ERASE(void);
void SPIM_RDID(void);
unsigned char SPIM_FLASH_ERASE(void);
unsigned char SPIM_READ_DATA_BYTE(unsigned long address);
void SPI_Master_test(void);
void SPIM_test_OVFIRQEN_EMTIRQEN(void);

/*REG[CCh][DBh][DEh]*/
void CGRAM_test_8bpp_HalfFont_8x16(void);
void CGRAM_test_8bpp_FullFont_16x16(void);
void CGRAM_test_8bpp_HalfFont_12x24(void);
void CGRAM_test_8bpp_FullFont_24x24(void);
void CGRAM_test_8bpp_HalfFont_16x32(void); 
void CGRAM_test_8bpp_FullFont_32x32(void);

void CGRAM_test_16bpp_HalfFont_8x16(void);
void CGRAM_test_16bpp_FullFont_16x16(void);
void CGRAM_test_16bpp_HalfFont_12x24(void);
void CGRAM_test_16bpp_FullFont_24x24(void);
void CGRAM_test_16bpp_HalfFont_16x32(void);
void CGRAM_test_16bpp_FullFont_32x32(void);

void CGRAM_test_24bpp_mode1_HalfFont_8x16(void);
void CGRAM_test_24bpp_mode1_FullFont_16x16(void);
void CGRAM_test_24bpp_mode1_HalfFont_12x24(void);
void CGRAM_test_24bpp_mode1_FullFont_24x24(void);
void CGRAM_test_24bpp_mode1_HalfFont_16x32(void);
void CGRAM_test_24bpp_mode1_FullFont_32x32(void);


void DMA_Init_CGRAM_test_8bpp_HalfFont_8x16(void);
void DMA_Init_CGRAM_test_8bpp_FullFont_16x16(void);
void DMA_Init_CGRAM_test_8bpp_HalfFont_12x24(void);
void DMA_Init_CGRAM_test_8bpp_FullFont_24x24(void);
void DMA_Init_CGRAM_test_8bpp_HalfFont_16x32(void); 
void DMA_Init_CGRAM_test_8bpp_FullFont_32x32(void);

void DMA_Init_CGRAM_test_16bpp_HalfFont_8x16(void);
void DMA_Init_CGRAM_test_16bpp_FullFont_16x16(void);
void DMA_Init_CGRAM_test_16bpp_HalfFont_12x24(void);
void DMA_Init_CGRAM_test_16bpp_FullFont_24x24(void);
void DMA_Init_CGRAM_test_16bpp_HalfFont_16x32(void); 
void DMA_Init_CGRAM_test_16bpp_FullFont_32x32(void);


/* REG[DFh] with display off function test*/ 
void Power_saving_test_standby_mode_16bpp(void);
void Power_saving_test_standby_mode_24bpp(void);
void Power_saving_test_suspend_mode_16bpp(void);
void Power_saving_test_suspend_mode_24bpp(void);
void Power_saving_test_sleep_mode_16bpp(void);
void Power_saving_test_sleep_mode_24bpp(void);

/**/
void RA8876_I2CM_Delay(void);
void RA8876_I2CM_Init(void);

unsigned char RA8876_I2CM_Puts(
				       unsigned char SlaveAddr,
				       unsigned char SubAddr,
				       unsigned char size,
				       unsigned char *dat
			          );

unsigned char RA8876_I2CM_Gets(
				       unsigned char SlaveAddr,
				       unsigned char SubAddr,
				       unsigned char size,
				       unsigned char *dat
			          );

 /*REG[E5h~EAh]*/
void RA8876_I2C_Master_test(void);



/*key scan test for digital panel package*/
void key_scan_test_5x5(void);
void Key_Scan_Wakeup_test(void);



void GT_Font_test_8bpp(void);

void Font_Funcion_8bpp(void);
void Font_Funcion_16bpp(void);
void Font_Funcion_24bpp(void);

void DMA_Flash_8bpp_32bit_address(void);
void DMA_Flash_8bpp(void);
void DMA_Flash_8bpp_partial(void);
void DMA_Flash_8bpp_partial_in_partial(void);
void DMA_Flash_8bpp_2048(void);
void DMA_Flash_8bpp_8188x8188(void);
void DMA_Flash_8bpp_multiple_canvas(void);
void DMA_8bpp_linear_mode_test(void);

void DMA_Flash_16bpp_32bit_address(void);
void DMA_Flash_16bpp(void);
void DMA_Flash_16bpp_partial(void);
void DMA_Flash_16bpp_partial_in_partial(void);
void DMA_Flash_16bpp_2048(void);
void DMA_Flash_16bpp_8188x8188(void);
void DMA_Flash_16bpp_multiple_canvas(void);
void DMA_16bpp_linear_mode_test(void);

void DMA_Flash_24bpp_32bit_address(void);
void DMA_Flash_24bpp(void);
void DMA_Flash_24bpp_partial(void);
void DMA_Flash_24bpp_partial_in_partial(void);
void DMA_Flash_24bpp_2048(void);
void DMA_Flash_24bpp_8188x8188(void);
void DMA_Flash_24bpp_multiple_canvas(void);
void DMA_24bpp_linear_mode_test(void);


/*REG[F0h~FAh]*/
void GPIO_A_test(void);
void GPIO_B_test(void);
void GPIO_C_test(void);
void GPIO_D_test(void);
void GPIO_E_test(void);
void GPIO_F_test(void);

void Print_Hex(unsigned char buf);
void Print_Hex_4(unsigned short buf);
void Print_Decimal(unsigned int buffer);
void Print_Decimal_5(unsigned int buffer);
void Print_Decimal_6(unsigned int buffer);


void Graphic_cursor_initial(void);
void Text_cursor_initial(void);

void PIP_window_with_DMA_8bpp(void);
void PIP_window_with_DMA_16bpp(void);
void PIP_window_with_DMA_24bpp(void);

void PIP_window_with_DMA_Draw_8bpp(void);
void PIP_window_with_DMA_Draw_16bpp(void);
void PIP_window_with_DMA_Draw_24bpp(void);

void Tom_test_Read_Write_16bit_16bpp (void);

