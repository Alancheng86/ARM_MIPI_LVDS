#ifndef __DELAY_H
#define __DELAY_H 			   
//�ϥ�SysTick�����q�p�ƼҦ��冀��i��޲z
//�]�Adelay_us,delay_ms  

//2008/12/14
//V1.2
//�ץ��F���_���եΥX�{���`�������~
//����ɤ�����,����do while����!
	 
static u8  fac_us=0;//us���ɭ�����
static u16 fac_ms=0;//ms���ɭ�����
//��l�Ʃ�����

void delay_init(u8 SYSCLK);
void delay_ms(u16 nms);
void delay_us(u32 Nus);

void delay_seconds(u32 ns);
#endif

//

