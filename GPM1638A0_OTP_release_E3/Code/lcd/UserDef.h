/*******************************************************************************
*	Filename:																   *
*   Company:   	RAiO Technology Inc.										   *
*******************************************************************************/
//------------------------------------------------------------------------------
// Hardware Setting
//------------------------------------------------------------------------------
/*==== [HW_(1)]   MCU   =====*/
//#define STM_32

/*==== [HW_(2)]   I/F   =====*/
//#define Parallel_8080
//#define Parallel_6800
//#define Serial_3
//#define Serial_4
//#define Serial_I2C

//#define true		1
//#define false		0

#define color256_black   0x00
#define color256_white   0xff
#define color256_red     0xe0
#define color256_green   0x1c
#define color256_blue    0x03
#define color256_yellow  color256_red|color256_green
#define color256_cyan    color256_green|color256_blue
#define color256_purple  color256_red|color256_blue
 
#define color65k_black   0x0000
#define color65k_white   0xffff
#define color65k_red     0xf800
#define color65k_green   0x07e0
#define color65k_blue    0x001f
#define color65k_yellow  color65k_red|color65k_green
#define color65k_cyan    color65k_green|color65k_blue
#define color65k_purple  color65k_red|color65k_blue

#define color65k_grayscale1    2113
#define color65k_grayscale2    2113*2
#define color65k_grayscale3    2113*3
#define color65k_grayscale4    2113*4
#define color65k_grayscale5    2113*5
#define color65k_grayscale6    2113*6
#define color65k_grayscale7    2113*7
#define color65k_grayscale8    2113*8
#define color65k_grayscale9    2113*9
#define color65k_grayscale10   2113*10
#define color65k_grayscale11   2113*11
#define color65k_grayscale12   2113*12
#define color65k_grayscale13   2113*13
#define color65k_grayscale14   2113*14
#define color65k_grayscale15   2113*15
#define color65k_grayscale16   2113*16
#define color65k_grayscale17   2113*17
#define color65k_grayscale18   2113*18
#define color65k_grayscale19   2113*19
#define color65k_grayscale20   2113*20
#define color65k_grayscale21   2113*21
#define color65k_grayscale22   2113*22
#define color65k_grayscale23   2113*23
#define color65k_grayscale24   2113*24
#define color65k_grayscale25   2113*25
#define color65k_grayscale26   2113*26
#define color65k_grayscale27   2113*27
#define color65k_grayscale28   2113*28
#define color65k_grayscale29   2113*29
#define color65k_grayscale30   2113*30

 
#define color16M_black   0x00000000
#define color16M_white   0x00ffffff
#define color16M_red     0x00ff0000
#define color16M_green   0x0000ff00
#define color16M_blue    0x000000ff
#define color16M_yellow  color16M_red|color16M_green
#define color16M_cyan    color16M_green|color16M_blue
#define color16M_purple  color16M_red|color16M_blue


#define	cSetb0		0x01
#define	cSetb1		0x02
#define	cSetb2		0x04
#define	cSetb3		0x08
#define	cSetb4		0x10
#define	cSetb5		0x20
#define	cSetb6		0x40
#define	cSetb7		0x80

#define	cClrb0		0xfe
#define	cClrb1		0xfd
#define	cClrb2		0xfb
#define	cClrb3		0xf7
#define	cClrb4		0xef
#define	cClrb5		0xdf
#define	cClrb6		0xbf
#define	cClrb7		0x7f

//--

#define B0000_0000 	0x00
#define B0000_0001 	0x01
#define B0000_0010 	0x02
#define B0000_0011 	0x03
#define B0000_0100 	0x04
#define B0000_0101 	0x05
#define B0000_0110 	0x06
#define B0000_0111 	0x07
#define B0000_1000 	0x08
#define B0000_1001 	0x09
#define B0000_1010 	0x0A
#define B0000_1011 	0x0B
#define B0000_1100 	0x0C
#define B0000_1101 	0x0D
#define B0000_1110 	0x0E
#define B0000_1111 	0x0F
                   	
#define B0001_0000 	0x10
#define B0001_0001 	0x11
#define B0001_0010 	0x12
#define B0001_0011 	0x13
#define B0001_0100 	0x14
#define B0001_0101 	0x15
#define B0001_0110 	0x16
#define B0001_0111 	0x17
#define B0001_1000 	0x18
#define B0001_1001 	0x19
#define B0001_1010 	0x1A
#define B0001_1011 	0x1B
#define B0001_1100 	0x1C
#define B0001_1101 	0x1D
#define B0001_1110 	0x1E
#define B0001_1111 	0x1F
                   	
#define B0010_0000 	0x20
#define B0010_0001 	0x21
#define B0010_0010 	0x22
#define B0010_0011 	0x23
#define B0010_0100 	0x24
#define B0010_0101 	0x25
#define B0010_0110 	0x26
#define B0010_0111 	0x27
#define B0010_1000 	0x28
#define B0010_1001 	0x29
#define B0010_1010 	0x2A
#define B0010_1011 	0x2B
#define B0010_1100 	0x2C
#define B0010_1101 	0x2D
#define B0010_1110 	0x2E
#define B0010_1111 	0x2F
                   	
#define B0011_0000 	0x30
#define B0011_0001 	0x31
#define B0011_0010 	0x32
#define B0011_0011 	0x33
#define B0011_0100 	0x34
#define B0011_0101 	0x35
#define B0011_0110 	0x36
#define B0011_0111 	0x37
#define B0011_1000 	0x38
#define B0011_1001 	0x39
#define B0011_1010 	0x3A
#define B0011_1011 	0x3B
#define B0011_1100 	0x3C
#define B0011_1101 	0x3D
#define B0011_1110 	0x3E
#define B0011_1111 	0x3F
                   	
#define B0100_0000 	0x40
#define B0100_0001 	0x41
#define B0100_0010 	0x42
#define B0100_0011 	0x43
#define B0100_0100 	0x44
#define B0100_0101 	0x45
#define B0100_0110 	0x46
#define B0100_0111 	0x47
#define B0100_1000 	0x48
#define B0100_1001 	0x49
#define B0100_1010 	0x4A
#define B0100_1011 	0x4B
#define B0100_1100 	0x4C
#define B0100_1101 	0x4D
#define B0100_1110 	0x4E
#define B0100_1111 	0x4F
                   	
#define B0101_0000 	0x50
#define B0101_0001 	0x51
#define B0101_0010 	0x52
#define B0101_0011 	0x53
#define B0101_0100 	0x54
#define B0101_0101 	0x55
#define B0101_0110 	0x56
#define B0101_0111 	0x57
#define B0101_1000 	0x58
#define B0101_1001 	0x59
#define B0101_1010 	0x5A
#define B0101_1011 	0x5B
#define B0101_1100 	0x5C
#define B0101_1101 	0x5D
#define B0101_1110 	0x5E
#define B0101_1111 	0x5F
                   	
#define B0110_0000 	0x60
#define B0110_0001 	0x61
#define B0110_0010 	0x62
#define B0110_0011 	0x63
#define B0110_0100 	0x64
#define B0110_0101 	0x65
#define B0110_0110 	0x66
#define B0110_0111 	0x67
#define B0110_1000 	0x68
#define B0110_1001 	0x69
#define B0110_1010 	0x6A
#define B0110_1011 	0x6B
#define B0110_1100 	0x6C
#define B0110_1101 	0x6D
#define B0110_1110 	0x6E
#define B0110_1111 	0x6F
                   	
#define B0111_0000 	0x70
#define B0111_0001 	0x71
#define B0111_0010 	0x72
#define B0111_0011 	0x73
#define B0111_0100 	0x74
#define B0111_0101 	0x75
#define B0111_0110 	0x76
#define B0111_0111 	0x77
#define B0111_1000 	0x78
#define B0111_1001 	0x79
#define B0111_1010 	0x7A
#define B0111_1011 	0x7B
#define B0111_1100 	0x7C
#define B0111_1101 	0x7D
#define B0111_1110 	0x7E
#define B0111_1111 	0x7F
                   	
#define B1000_0000 	0x80
#define B1000_0001 	0x81
#define B1000_0010 	0x82
#define B1000_0011 	0x83
#define B1000_0100 	0x84
#define B1000_0101 	0x85
#define B1000_0110 	0x86
#define B1000_0111 	0x87
#define B1000_1000 	0x88
#define B1000_1001 	0x89
#define B1000_1010 	0x8A
#define B1000_1011 	0x8B
#define B1000_1100 	0x8C
#define B1000_1101 	0x8D
#define B1000_1110 	0x8E
#define B1000_1111 	0x8F
                   	
#define B1001_0000 	0x90
#define B1001_0001 	0x91
#define B1001_0010 	0x92
#define B1001_0011 	0x93
#define B1001_0100 	0x94
#define B1001_0101 	0x95
#define B1001_0110 	0x96
#define B1001_0111 	0x97
#define B1001_1000 	0x98
#define B1001_1001 	0x99
#define B1001_1010 	0x9A
#define B1001_1011 	0x9B
#define B1001_1100 	0x9C
#define B1001_1101 	0x9D
#define B1001_1110 	0x9E
#define B1001_1111 	0x9F
                   	
#define B1010_0000 	0xA0
#define B1010_0001 	0xA1
#define B1010_0010 	0xA2
#define B1010_0011 	0xA3
#define B1010_0100 	0xA4
#define B1010_0101 	0xA5
#define B1010_0110 	0xA6
#define B1010_0111 	0xA7
#define B1010_1000 	0xA8
#define B1010_1001 	0xA9
#define B1010_1010 	0xAA
#define B1010_1011 	0xAB
#define B1010_1100 	0xAC
#define B1010_1101 	0xAD
#define B1010_1110 	0xAE
#define B1010_1111 	0xAF
                   	
#define B1011_0000 	0xB0
#define B1011_0001 	0xB1
#define B1011_0010 	0xB2
#define B1011_0011 	0xB3
#define B1011_0100 	0xB4
#define B1011_0101 	0xB5
#define B1011_0110 	0xB6
#define B1011_0111 	0xB7
#define B1011_1000 	0xB8
#define B1011_1001 	0xB9
#define B1011_1010 	0xBA
#define B1011_1011 	0xBB
#define B1011_1100 	0xBC
#define B1011_1101 	0xBD
#define B1011_1110 	0xBE
#define B1011_1111 	0xBF
                   	
#define B1100_0000 	0xC0
#define B1100_0001 	0xC1
#define B1100_0010 	0xC2
#define B1100_0011 	0xC3
#define B1100_0100 	0xC4
#define B1100_0101 	0xC5
#define B1100_0110 	0xC6
#define B1100_0111 	0xC7
#define B1100_1000 	0xC8
#define B1100_1001 	0xC9
#define B1100_1010 	0xCA
#define B1100_1011 	0xCB
#define B1100_1100 	0xCC
#define B1100_1101 	0xCD
#define B1100_1110 	0xCE
#define B1100_1111 	0xCF
                   	
#define B1101_0000 	0xD0
#define B1101_0001 	0xD1
#define B1101_0010 	0xD2
#define B1101_0011 	0xD3
#define B1101_0100 	0xD4
#define B1101_0101 	0xD5
#define B1101_0110 	0xD6
#define B1101_0111 	0xD7
#define B1101_1000 	0xD8
#define B1101_1001 	0xD9
#define B1101_1010 	0xDA
#define B1101_1011 	0xDB
#define B1101_1100 	0xDC
#define B1101_1101 	0xDD
#define B1101_1110 	0xDE
#define B1101_1111 	0xDF
                   	
#define B1110_0000 	0xE0
#define B1110_0001 	0xE1
#define B1110_0010 	0xE2
#define B1110_0011 	0xE3
#define B1110_0100 	0xE4
#define B1110_0101 	0xE5
#define B1110_0110 	0xE6
#define B1110_0111 	0xE7
#define B1110_1000 	0xE8
#define B1110_1001 	0xE9
#define B1110_1010 	0xEA
#define B1110_1011 	0xEB
#define B1110_1100 	0xEC
#define B1110_1101 	0xED
#define B1110_1110 	0xEE
#define B1110_1111 	0xEF
                   	
#define B1111_0000 	0xF0
#define B1111_0001 	0xF1
#define B1111_0010 	0xF2
#define B1111_0011 	0xF3
#define B1111_0100 	0xF4
#define B1111_0101 	0xF5
#define B1111_0110 	0xF6
#define B1111_0111 	0xF7
#define B1111_1000 	0xF8
#define B1111_1001 	0xF9
#define B1111_1010 	0xFA
#define B1111_1011 	0xFB
#define B1111_1100 	0xFC
#define B1111_1101 	0xFD
#define B1111_1110 	0xFE
#define B1111_1111 	0xFF 





