/*************************************************************************************************************
 * �ļ���:	system.h
 * ����:		S3C6410���ϵͳ����
 * ����:		����
 * ����ʱ��:	2012��3��4��11:25
 * ����޸�ʱ��:2012��3��4��
 * ��ϸ:		���ϵͳ�����궨��
*************************************************************************************************************/

#ifndef	_SYSTEM_H_
#define	_SYSTEM_H_ 

#include "sys_types.h"
#include "stdio.h"
#include "s3c6410_map.h"

#define Debug printf	//����֧��

#define nop __nop



//����ⲿ��������
void SetEINT0_TriggerMode(vu8 EINT0_N,vu8 Trigger);//�����ⲿ�ж���0����ģʽ
void Set_GateClk(vu8 CLK_DIV,vu8 ENABLE);	//CLKʱ���ſ�����(HCLK,PCLK,SCLK)
void Set_INTtoIRQ(vu8 INT_N);	//����һ���ж�ΪIRQ
void Set_INTtoFIQ(vu8 INT_N);	//����һ���ж�ΪFIQ
void Set_IntEnable(vu8 INT_N,FunctionalState EnInt);	//������ر�һ��VIC�ж�
void Set_SoftInt(vu8 INT_N,vu8 ENABLE);	//������ر�һ������ж�
void Set_IsrAddr(vu8 INT_N,vu32 IsrAdd);	//�����ж�ʸ�����
void Set_VectorPriority(vu8 INT_N,vu8 Priority);	//����ʸ�����ȼ�
u8 Get_IntEnable(vu8 INT_N);	//��ȡһ���ж�����״̬
u32 Get_PCLK(void);			//��ȡPCLKʱ��Ƶ��
u32 Get_FCLK(void);			//��ȡFCLKʱ��Ƶ��
u32 Get_PLLCLK(u8 pllreg);	//��ȡPLLʱ��Ƶ��

//����ⲿȫ�ֱ�������
extern u8 PrintfSet;	//0:����printf������;1:����printf��Һ��

//IOģʽ�궨��
#define	IO_IN_PUT		0	//����ģʽ
#define IO_OUT_PUT		1	//���ģʽ
#define IO_SF_MODE		2	//����ģʽ,���ù���
#define IO_EXT_INT		7	//�ⲿ�ж�����ģʽ
#define IO_NO_UP		0	//��ֹ����,����
#define IO_DROP_DOWM	1	//����
#define IO_PULL_UP		2	//����

//�ⲿ�жϴ���ģʽ����
#define EXT_LowLevel	0	//�͵�ƽ����
#define EXT_HighLevel	1	//�ߵ�ƽ����
#define EXT_NegEdge		2	//�½��ش���
#define EXT_PosEdge		4	//�����ش���
#define EXT_Edge		6	//���ش���

//ʹ��printf���
//0:�ر�printf���;1:ʹ��printf������;2:ʹ��printf��Һ��;3:ͬʱʹ��printf�����ں�Һ��
#define PRINTF_EN_		3



//�ж���0��Ŷ���
//	 �ⲿ�ж���0��IO	ƫ��+��ʾ	�ж���0�еı��
#define EINT0_GPN0		0			//0
#define EINT0_GPN1		0			//1
#define EINT0_GPN2		4			//2
#define EINT0_GPN3		4			//3
#define EINT0_GPN4		8			//4
#define EINT0_GPN5		8			//5
#define EINT0_GPN6		12			//6	
#define EINT0_GPN7		12			//7
#define EINT0_GPN8		16			//8
#define EINT0_GPN9		16			//9
#define EINT0_GPN10		20			//10
#define EINT0_GPN11		20			//11
#define EINT0_GPN12		24			//12
#define EINT0_GPN13		24			//13
#define EINT0_GPN14		28			//14
#define EINT0_GPN15		28			//15
#define EINT0_GPL8		(0x80 + 0)	//16
#define EINT0_GPL9		(0x80 + 0)	//17
#define EINT0_GPL10		(0x80 + 4)	//18
#define EINT0_GPL11		(0x80 + 4)	//19
#define EINT0_GPL12		(0x80 + 8)	//20
#define EINT0_GPL13		(0x80 + 8)	//21
#define EINT0_GPL14		(0x80 + 12)	//22
#define EINT0_GPM0		(0x80 + 12)	//23
#define EINT0_GPM1		(0x80 + 16)	//24
#define EINT0_GPM2		(0x80 + 16)	//25
#define EINT0_GPM3		(0x80 + 20)	//26
#define EINT0_GPM4		(0x80 + 20)	//27




//�ⲿ�жϷ��鶨��
//		����			//���			//��Χ
#define EINT_Group0    	0				//GPN0--->GPN15        GPL8--->GPL14         GPM0--->GPM4
#define EINT_Group01    1				//GPA0--->GPA7         GPB0--->GPB6
#define EINT_Group02    1				//GPC0--->GPC7
#define EINT_Group03    2				//GPD0--->GPD5
#define EINT_Group04    2				//GPF0--->GPF14
#define EINT_Group05   	3				//GPG0--->GPG7
#define EINT_Group06    3				//GPH0--->GPH9
#define EINT_Group07    4				//GPO0--->GPO15
#define EINT_Group08    4				//GPP0--->GPP14
#define EINT_Group09    5				//GPQ0--->GPQ9


//HCLK�ſ�ʱ�Ӷ���
/*HCLK_GATE��������Ips��HCLK���������Ϊ��1������HCLK���ṩ������HCLK�����Ρ���S3C6410
ת���ɵ���ģʽʱ��ϵͳ���������һЩģ�飨IROM��MEM0��MEM1��MFCģ�飩��״̬����ˣ�λ25��22��
21��0����Ϊ��1�����Է��ϵ����Ҫ��
		HCLK_GATE 		λ 		���� 																��ʼ״̬*/
#define HCLK_UHOST		29		//ΪUHOST ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_SECUR		28		//Ϊ��ȫ��ϵͳѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_SDMA1		27		//ΪSDMA1 ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_SDMA0		26		//ΪSDMA0 ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_IROM		25		//ΪIROM ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_DDR1		24		//ΪDDR1 ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_DDR0		23		//ΪDDR0 ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_MEM1		22		//ΪDMC1 ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_MEM0		21		//ΪDMC0��SROM��OneNAND��NFCON ��CFCON ѡͨHCLK(0�����Σ�1��ͨ��)��	1
#define HCLK_USB		20		//ΪUSB OTG ѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_HSMMC2		19		//ΪHSMMC2 ѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_HSMMC1		18		//ΪHSMMC1 ѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_HSMMC0		17		//ΪHSMMC0 ѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_MDP		16		//ΪMDP ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_DHOST		15		//Ϊֱ��HOST �ӿ�ѡͨHCLK(0�����Σ�1��ͨ��)�� 						1
#define HCLK_IHOST		14		//Ϊ���HOST �ӿ�ѡͨHCLK(0�����Σ�1��ͨ��)�� 						1
#define HCLK_DMA1		13		//ΪDMA1 ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_DMA0		12		//ΪDMA0 ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_JPEG		11		//ΪJPEG ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_CAMIF		10		//Ϊ����ӿ�ѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_SCALER		9		//Ϊ������ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_2D			8		//Ϊ2D ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_TV			7		//ΪTV ������ѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_POST0		5		//ΪPOST0 ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_ROT		4		//Ϊ��ת��ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1
#define HCLK_LCD		3		//ΪLCD ������ѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_TZIC		2		//Ϊ�жϿ�����ѡͨHCLK(0�����Σ�1��ͨ��)�� 							1
#define HCLK_INTC		1		//Ϊ�����жϿ�����ѡͨHCLK(0�����Σ�1��ͨ��)�� 						1
#define HCLK_MFC		0		//ΪMFC ѡͨHCLK(0�����Σ�1��ͨ��)�� 								1


//PCLK�ſ�ʱ�Ӷ���
//		PCLK_GATE 		λ 				���� 											��ʼ״̬
#define PCLK_SKEY		(0x40 | 24)		// Ϊ��ȫ��ѡͨPCLK(0�����Σ�1��ͨ��)�� 		1
#define PCLK_CHIPID		(0x40 | 23)		// ΪƬ��ID ѡͨPCLK(0�����Σ�1��ͨ��)�� 		1
#define PCLK_SPI1		(0x40 | 22)		// ΪSPI1 ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_SPI0		(0x40 | 21)		// ΪSPI0 ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_HSIRX		(0x40 | 20)		// ΪHSI ������ѡͨPCLK(0�����Σ�1��ͨ��)�� 	1
#define PCLK_HSITX		(0x40 | 19)		// ΪHIS ������ѡͨPCLK(0�����Σ�1��ͨ��)�� 	1
#define PCLK_GPIO		(0x40 | 18)		// ΪGPIO ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_IIC		(0x40 | 17)		// ΪIIC ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_IIS1		(0x40 | 16)		// ΪIIS1 ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_IIS0		(0x40 | 15)		// ΪIIS0 ѡͨPCLK(0�����Σ�1��ͨ��)��			1
#define PCLK_AC97		(0x40 | 14)		// ΪAC97 ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_TZPC		(0x40 | 13)		// ΪTZPC ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_TSADC		(0x40 | 12)		// Ϊ������ADC ѡͨPCLK(0�����Σ�1��ͨ���� 		1
#define PCLK_KEYPAD		(0x40 | 11)		// ΪKey PAD ѡͨPCLK(0�����Σ�1��ͨ��)��		1
#define PCLK_IRDA		(0x40 | 10)		// ΪIRDA ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_PCM1		(0x40 | 9)		// ΪPCM1 ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_PCM0		(0x40 | 8)		// ΪPCM0 ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_PWM		(0x40 | 7)		// ΪPWM ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_RTC		(0x40 | 6)		// ΪRTC ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1
#define PCLK_WDT		(0x40 | 5)		// Ϊ���Ź���ʱ��ѡͨPCLK(0�����Σ�1��ͨ��)�� 	1
#define PCLK_UART3		(0x40 | 4)		// ΪUART3 ѡͨPCLK(0�����Σ�1��ͨ��)�� 		1
#define PCLK_UART2		(0x40 | 3)		// ΪUART2 ѡͨPCLK(0�����Σ�1��ͨ��)�� 		1
#define PCLK_UART1		(0x40 | 2)		// ΪUART1 ѡͨPCLK(0�����Σ�1��ͨ��)�� 		1
#define PCLK_UART0		(0x40 | 1)		// ΪUART0 ѡͨPCLK(0�����Σ�1��ͨ��)�� 		1
#define PCLK_MFC		(0x40 | 0)		// ΪMFC ѡͨPCLK(0�����Σ�1��ͨ��)�� 			1


//PCLK�ſ�ʱ�Ӷ���
//PCLK_GATE λ ���� ��ʼ״̬
#define SCLK_UHOST		(0x80 | 30) 	// ΪUSB-HOST ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_MMC2_48	(0x80 | 29)		// ΪMMC2 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_MMC1_48	(0x80 | 28)		// ΪMMC1 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_MMC0_48	(0x80 | 27)		// ΪMMC0 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_MMC2		(0x80 | 26)		// ΪMMC2 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_MMC1		(0x80 | 25)		// ΪMMC1 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_MMC0		(0x80 | 24)		// ΪMMC0 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_SPI1_48	(0x80 | 23)		// ΪSPI ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_SPI0_48	(0x80 | 22)		// ΪSPI ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_SPI1		(0x80 | 21)		// ΪSPI ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_SPI0		(0x80 | 20)		// ΪSPI ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_DAC27		(0x80 | 19)		// ΪDAC ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_TV27		(0x80 | 18)		// ΪTV ������ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_SCALER27	(0x80 | 17)		// Ϊscaler27 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_SCALER		(0x80 | 16)		// Ϊ������ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_LCD27		(0x80 | 15)		// ΪLCD ������ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_LCD		(0x80 | 14)		// ΪLCD ������ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_POST0_27	(0x80 | 12)		// ΪPOST0 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_POST0		(0x80 | 10)		// ΪPOST0 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_AUDIO1		(0x80 | 9)		// ΪPCM1��IIS1 ��AC97 1 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)��1
#define SCLK_AUDIO0		(0x80 | 8)		// ΪPCM0��IIS0 ��AC97 0 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)��1
#define SCLK_SECUR		(0x80 | 7)		// Ϊ��ȫģ��ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_IRDA		(0x80 | 6)		// ΪIRDA ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_UART		(0x80 | 5)		// ΪUART0��3 ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_OneNAND	(0x80 | 4)		// ΪOneNAND ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_MFC		(0x80 | 3)		// ΪMFC ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_CAM		(0x80 | 2)		// Ϊ����ӿ�ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1
#define SCLK_JPEG		(0x80 | 1)		// ΪJPEG ѡͨ����ʱ�� (0�����Σ�1��ͨ��)�� 1

//�ж�Դ��Ŷ���
/*		S3C6410X ֧��64 λ�ж�Դ����֧��ARM1176HZF-S �����ж�����.

		 		�ж�Դ 				�жϺ�	���� 									��*/
#define 		INT_ADC 			63		//ADC EOC �ж� 							VIC1
#define 		INT_PENDNUP 		62		//ADC ������/�����ж� �ж� 				VIC1
#define 		INT_SEC 			61		//��ȫ�ж� 								VIC1
#define 		INT_RTC_ALARM 		60		//RTC �����ж� 							VIC1
#define 		INT_IrDA 			59		//IrDA �ж� 							VIC1
#define 		INT_OTG 			58		//USB OTG �ж� 							VIC1
#define 		INT_HSMMC1 			57		//HSMMC1 �ж� 							VIC1
#define 		INT_HSMMC0 			56		//HSMMC0 �ж� 							VIC1
#define 		INT_HOSTIF 			55		//�����ӿ��ж� 							VIC1
#define 		INT_MSM 			54		//MSM ���ƽ���� I/F �ж� 				VIC1
#define 		INT_EINT4 			53		//�ⲿ�ж���1~��9 						VIC1
#define 		INT_HSIrx 			52		//HS Rx �ж� 							VIC1
#define 		INT_HSItx 			51		//HS Tx �ж� 							VIC1
#define 		INT_I2C0 			50		//I2C 0 �ж� 							VIC1
#define 		INT_SPI_INT_HSMMC2 	49		//SPI �жϻ�HSMMC2 �ж� 				VIC1
#define 		INT_SPI0 			48		//SPI0 �ж� 							VIC1
#define 		INT_UHOST 			47		//USB �����ж� 							VIC1
#define 		INT_CFC 			46		//CFCON �ж� 							VIC1
#define	 		INT_NFC 			45		//NFCON �ж� 							VIC1
#define 		INT_ONENAND1 		44		//���1 ��ONENANE �ж� 					VIC1
#define 		INT_ONENAND0 		43		//���0 ��ONENAND �ж� 					VIC1
#define 		INT_DMA1 			42		//DMA1 �ж� 							VIC1
#define 		INT_DMA0 			41		//DMA0 �ж� 							VIC1
#define 		INT_UART3 			40		//UART3 �ж� 							VIC1
#define 		INT_UART2 			39		//UART2 �ж� 							VIC1
#define 		INT_UART1 			38		//UART1 �ж� 							VIC1
#define 		INT_UART0 			37		//UART0 �ж� 							VIC1
#define 		INT_AC97 			36		//AC �ж� 								VIC1
#define 		INT_PCM1 			35		//PCM1 �ж� 							VIC1
#define 		INT_PCM0 			34		//PCM0 �ж� 							VIC1
#define 		INT_EINT3 			33		//�ⲿ�ж�20~27 						VIC1
#define 		INT_EINT2 			32		//�ⲿ�ж�12~19 						VIC1
#define 		INT_LCD_2 			31		//LCD �ж�.ϵͳI/F ��� 				VIC0
#define 		INT_LCD_1 			30		//LCD �ж�.VSYNC �ж� 					VIC0
#define 		INT_LCD_0 			29		//LCD �ж�.FIFO ���� 					VIC0
#define 		INT_TIMER4 			28		//��ʱ��4 �ж�. 						VIC0
#define 		INT_TIMER3 			27		//��ʱ��3 �ж�. 						VIC0
#define 		INT_WDT 			26		//���Ź���ʱ���ж�. 					VIC0
#define 		INT_TIMER2 			25		//��ʱ��2 �ж�. 						VIC0
#define 		INT_TIMER1 			24		//��ʱ��1 �ж�. 						VIC0
#define 		INT_TIMER0 			23		//��ʱ��0 �ж�. 						VIC0
#define 		INT_KEYPAD 			22		//�����ж�. 							VIC0
#define 		INT_ARM_DMAS 		21		//ARM DMAS �ж�. 						VIC0
#define 		INT_ARM_DMA 		20		//ARM DMA �ж�. 						VIC0
#define 		INT_ARM_DMAERR 		19		//ARM DMA �����ж�. 					VIC0
#define 		INT_SDMA1 			18		//��ȫ DMA1 �ж�. 						VIC0
#define 		INT_SDMA0 			17		//��ȫ DMA0 �ж�. 						VIC0
#define 		INT_MFC 			16		//MFC �ж�. 							VIC0
#define 		INT_JPEG 			15		//JPEG �ж�. 							VIC0
#define 		INT_BATF 			14		//��ع����ж�. 						VIC0
#define 		INT_SCALER 			13		//TV ת�����ж�. 						VIC0
#define 		INT_TVENC 			12		//TV �������ж�. 						VIC0
#define 		INT_2D 				11		//2D �ж�. 								VIC0
#define 		INT_ROTATOR 		10		//��ת���ж�. 							VIC0
#define 		INT_POSTO 			9		//�������ж�. 						VIC0
#define 		INT_3D 				8		//3D ͼ��������ж�. 					VIC0
//#define 		Reserved 			7		//���� 									VIC0
#define 		INT_I2S				6		//I2S0/I2S1/INT_I2SV40/I2SV40�ж�		VIC0				
#define 		INT_I2C1 			5		//I2C1 �ж�			 					VIC0
#define 		INT_CAMIF_P 		4		//������ӿ��ж� 						VIC0
#define 		INT_CAMIF_C 		3		//������ӿ��ж� 						VIC0
#define 		INT_RTC_TIC 		2		//RTC TIC �ж� 							VIC0
#define 		INT_EINT1 			1		//�ⲿ�ж�4~11 							VIC0
#define 		INT_EINT0 			0		//�ⲿ�ж�0~3 							VIC0	







/*************************************************************************************************/
/*			��Ӧλ����������λ����		*/
#define BIT0	(0x0001 << 0)
#define BIT1	(0x0001 << 1)
#define BIT2	(0x0001 << 2)
#define BIT3	(0x0001 << 3)
#define BIT4	(0x0001 << 4)
#define BIT5	(0x0001 << 5)
#define BIT6	(0x0001 << 6)
#define BIT7	(0x0001 << 7)
#define BIT8	(0x0001 << 8)
#define BIT9	(0x0001 << 9)
#define BIT10	(0x0001 << 10)
#define BIT11	(0x0001 << 11)
#define BIT12	(0x0001 << 12)
#define BIT13	(0x0001 << 13)
#define BIT14	(0x0001 << 14)
#define BIT15	(0x0001 << 15)
#define BIT16	(0x00000001 << 16)
#define BIT17	(0x00000001 << 17)
#define BIT18	(0x00000001 << 18)
#define BIT19	(0x00000001 << 19)
#define BIT20	(0x00000001 << 20)
#define BIT21	(0x00000001 << 21)
#define BIT22	(0x00000001 << 22)
#define BIT23	(0x00000001 << 23)
#define BIT24	(0x00000001 << 24)
#define BIT25	(0x00000001 << 25)
#define BIT26	(0x00000001 << 26)
#define BIT27	(0x00000001 << 27)
#define BIT28	(0x00000001 << 28)
#define BIT29	(0x00000001 << 29)
#define BIT30	(0x00000001 << 30)
#define BIT31	(0x00000001 << 31)

//ʹ����ʧ�ܺ궨��
#ifndef	Enable
	#define Enable	1	//ʹ��
#endif	//Enable
#ifndef	Disable
	#define Disable	0	//ʧ��
#endif	//Enable


//PLLѡ��
#define	APLL 	0	//ARM�ں�ʱ��PLL
#define MPLL 	1	//��ʱ��PLL
#define EPLL 	2	//����ʱ��PLL

//��ʱ������
#define SYSTEM_MAIN_CLK_IN	12000000		//12MHZ

/*************************************************************************************************************************
*����        :	__inline void VICInterruptEnd(void)
*����        :  ���жϿ�Ҫ����ʱ����ж�
*����        :  ��
*����        :  ��
*����        :	�ײ�궨��
*����        :  ����
*ʱ��        :	20120305
*����޸�ʱ��: 	20120305
*˵��        :  д���κ���������ж�,ֻ�����жϷ�������вſɶ�,����ʱ��д
				����Ҫһ�����,������ܵ����޷��ٴν����ж�,����䷢�ֵ�
*************************************************************************************************************************/
__inline void VICInterruptEnd(void)
{
	VIC0->ADDRESS = 0xffffffff;	//д���κ�ֵ�����������ǰ�ж�
	VIC1->ADDRESS = 0xffffffff;	//д���κ�ֵ�����������ǰ�ж�
}


//ͨ��ѡ��
#define ch0		0
#define ch1		1
#define ch2		2



//printf�������
#if (PRINTF_EN_ == 1)	//ʹ�ܵ�����
#define uart_printf(format,...)	(printf(format, ##__VA_ARGS__))	//���ڴ�ӡ
#define DEBUG(format,...) 		(printf("<DebugFile: "__FILE__", Line: %d> "format, __LINE__, ##__VA_ARGS__))	//DEBUG���
#endif


//printf�������
#if (PRINTF_EN_ == 2)	//ʹ�ܵ�Һ��
#define lcd_printf(format,...)	(printf(format, ##__VA_ARGS__))	//LCD��ӡ
#define DEBUG(format,...) 		(printf("<DebugFile: "__FILE__", Line: %d> "format, __LINE__, ##__VA_ARGS__))	//DEBUG���
#endif


//printf�������
#if (PRINTF_EN_ == 3)	//ͬʱʹ�ܵ�Һ���ʹ���
#define uart_printf(format,...)	PrintfSet=0;printf(format, ##__VA_ARGS__)	//���ڴ�ӡ
#define lcd_printf(format,...)	PrintfSet=1;printf(format, ##__VA_ARGS__)	//LCD��ӡ
#define DEBUG(format,...)		PrintfSet=0;printf("<DebugFile: "__FILE__", Line: %d> "format, __LINE__, ##__VA_ARGS__)	//DEBUG���
#endif

#endif






