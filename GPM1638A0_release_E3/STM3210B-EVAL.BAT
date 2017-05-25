SET PATH=C:\Keil\ARM\BIN40;C:\Program Files\STMicroelectronics\st_toolset\asm;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;D:\PROGRA~1\ULTRAE~1;C:\Program Files\Microsoft SQL Server\80\Tools\BINN;C:\modeltech_10.0c\win32;D:\ModelSimAltera\modelsim_ase\win32aloem
SET ARMCC41INC=C:\Keil\ARM\RV31\Inc
SET ARMCC41LIB=C:\Keil\ARM\RV31\Lib
SET CPU_TYPE=STM32F103VC
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=STM3210B-EVAL
SET CPU_CLOCK=0x007A1200
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\fsmc_sram.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\main.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_it.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\sdcard.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\delay.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\spi_flash.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\usart.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\ra8876_demo.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\misc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_exti.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_fsmc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_gpio.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_rcc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_usart.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_spi.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_dma.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_sdio.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_bkp.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_pwr.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_rtc.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\stm32f10x_tim.__i"
"C:\Keil\ARM\BIN40\ArmAsm" --Via ".\obj\startup_stm32f10x_hd._ia"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\ra8876.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\lcd.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\asc16.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\gb4848.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\ff.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\diskio.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\core_cm3.__i"
"C:\Keil\ARM\BIN40\ArmCC" --Via ".\obj\system_stm32f10x.__i"
"C:\Keil\ARM\BIN40\ArmLink" --Via ".\Obj\STM32_RA8876.lnp"
C:\Keil\ARM\BIN40\fromelf.exe ".\Obj\STM32_RA8876.axf" --i32combined --output ".\Obj\STM32_RA8876.hex"
