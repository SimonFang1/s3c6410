/*************************************************************************************************************
 * �ļ���:	s3c6410_map.h
 * ����:		S3C6410��ؼĴ����궨��
 * ����:		����
 * ����ʱ��:	2012��3��4��11:22
 * ����޸�ʱ��:2012��3��4��
 * ��ϸ:		��ؼĴ����궨��
*************************************************************************************************************/


#ifndef _S3C6410_MAP_H_
#define _S3C6410_MAP_H_

#include "sys_types.h"
 
//ʹ�ܱ���ѡ��
//UART
#define _UART0
#define _UART1
#define _UART2
#define _UART3
//VIC
#define _VIC0
#define _VIC1
//MODEM
#define _MODEM
//NAND FLASH
#define _NAND
//RTC
#define _RTC
//ADC
#define _ADC
//SDIO
#define _SDIO0
#define _SDIO1
#define _SDIO2
//AC97
#define _AC97
//2D
#define _G2D
//SPI
#define _SPI



//�Ĵ�����ַӳ��
//ӳ���Ϊ����
#define	S3C6410_SFR32(add)	(*((volatile unsigned long int*)(add)))	//ֱ��ӳ��
#define	S3C6410_SFR16(add)	(*((volatile unsigned short*)(add)))	//ֱ��ӳ��
#define	S3C6410_SFR8(add)	(*((volatile unsigned char *)(add)))	//ֱ��ӳ��
//ӳ���Ϊָ��
#define S3C6410_SFR32_P(add)	((volatile unsigned long int*)(add))
#define S3C6410_SFR16_P(add)	((volatile unsigned short*)(add))
#define S3C6410_SFR8_P(add)		((volatile unsigned char*)(add))




//**********************************************************************************//
//����Ĵ����ṹ����

//VICx�жϿ�����
typedef struct
{	
/*      �Ĵ���				������	����	����	��λֵ*/
vu32	IRQSTATUS; 		//	0x000 	�� 		IRQ ״̬�Ĵ��� 0x00000000
vu32	FIQSTATUS; 		//	0x004 	�� 		FIQ ״̬�Ĵ��� 0x00000000
vu32	IRAWINTR; 		//	0x008 	�� 		ԭʼ�ж�״̬�Ĵ��� 0x00000000
vu32	INTSELECT; 		//	0x00C 	��д 	�ж�ѡ��Ĵ��� 0x00000000
vu32	INTENABLE; 		//	0x010 	��д 	�ж�ʹ�ܼĴ��� 0x00000000
vu32	INTENCLEAR; 	//	0x014 	д 		�ж�ʹ������Ĵ��� -
vu32	SOFTINT; 		//	0x018 	��д 	����жϼĴ��� 0x00000000
vu32	SOFTINTCLEAR; 	//	0x01C 	д 		����ж�����Ĵ��� -
vu32	PROTECTION; 	//	0x020 	��д 	����ʹ�ܼĴ��� 0x0
vu32	SWPRIORITYMASK; //	0x024 	��д 	����������μĴ��� 0x0FFFF
vu32	PRIORITYDAISY;	//	0x028 	��д 	�ջ�����ʸ�����ȼĴ��� 0xF 
uc32	Reserved0[53];	//����
vu32	VECTADDR[32];	//			��д	ʸ����ַ�Ĵ���   0x00000000
uc32	Reserved[32];	//����
vu32	VECTRPRIORITY[32];//			��д	ʸ�����ȼ��Ĵ���	0xf
uc32	Reserved2[800];	//����
vu32	ADDRESS;		//			��д	��ǰʸ����ַ�Ĵ���	0x00000000
}VICx_TypeDef;


//NAND FLASH������
typedef struct
{
	vu32	CONF;		//���üĴ���
	vu32	CONT;		//���ƼĴ���
	vu8		CMMD;		//����Ĵ���
	u8		Reserved0[3];
	vu8		ADDR;		//��ַ�Ĵ���
	u8		Reserved1[3];
	vu32	DATA;		//���ݼĴ���
	vu32	MECCD0;		//��һ���͵ڶ�����ECC ���ݼĴ���
	vu32	MECCD1;		//�������͵��ĸ���ECC ���ݼĴ���
	vu32	SECCD;		//����ECC ���Ĵ���
	vu32	SBLK;		//�ɱ�̿�ʼ���ַ�Ĵ���
	vu32	EBLK;		//�ɱ�̽������ַ�Ĵ���
	vu32	STAT;		//NAND ״̬�Ĵ���
	vuc32	ECCERR0;	//ECC ����״̬0 �Ĵ���
	vuc32	ECCERR1;	//ECC ����״̬1 �Ĵ���
	vuc32	MECC0;		//����ECC ״̬0 �Ĵ���
	vuc32	MECC1;		//����ECC ״̬1 �Ĵ���
	vuc32	SECC;		//���ɱ�����ECC ״̬�Ĵ���
	vuc32	MLCBITPT;	//4 λECC ����λģʽ�Ĵ���
}NAND_TypeDef;


//RTC ʵʱʱ�ӼĴ���
typedef struct
{
	//�Ĵ��� ��ַ ��/д ���� ��λֵ
	vu32	INTP; 			//   0x7E005030 ��/д �жϵȺ�Ĵ��� 0x0
	uc32	Reserved0[3];
	vu32	CON; 			//	 0x7E005040 ��/д ʵʱʱ�ӿ��ƼĴ��� 0x0
	vu32	TICNT; 			//	 0x7E005044 ��/д ���ʱ������Ĵ��� 0x0
	uc32	Reserved1[2];
	vu32	ALM; 			//	 0x7E005050 ��/д ʵʱʱ�ӱ������ƼĴ��� 0x0
	vu32	ALMSEC; 		//	 0x7E005054 ��/д ���������ݼĴ��� 0x0
	vu32	ALMMIN; 		//	 0x7E005058 ��/д �����������ݼĴ��� 0x00
	vu32	ALMHOUR; 		//	 0x7E00505C ��/д ����Сʱ���ݼĴ��� 0x0
	vu32	ALMDATE; 		//	 0x7E005060 ��/д ���������ݼĴ��� 0x01
	vu32	ALMMON; 		//	 0x7E005064 ��/д ���������ݼĴ��� 0x01
	vu32	ALMYEAR; 		//	 0x7E005068 ��/д ���������ݼĴ��� 0x0
	uc32	Reserved2;
	vu32	BCDSEC; 		//	 0x7E005070 ��/д BCD ��Ĵ��� δ����
	vu32	BCDMIN; 		//	 0x7E005074 ��/д BCD ���ӼĴ��� δ����
	vu32	BCDHOUR; 		//	 0x7E005078 ��/д BCD Сʱ�Ĵ��� δ����
	vu32	BCDDATE; 		//	 0x7E00507C ��/д BCD ���ڼĴ��� δ����
	vu32	BCDDAY; 		//	 0x7E005080 ��/д BCD ��Ĵ��� δ����
	vu32	BCDMON; 		//	 0x7E005084 ��/д BCD �¼Ĵ��� δ����
	vu32	BCDYEAR; 		//	 0x7E005088 ��/д BCD ��Ĵ����� δ����
	uc32	Reserved3;
	vu32	CURTICCNT; 		//	 0x7E005090 �� ��ǰ���ʱ������Ĵ����� 0x0
}RTC_TypeDef;


//ADC �Ĵ���
typedef struct
{
	vu32	CON;			//ADC���ƼĴ��� 
	vu32	TSC;			//���������ƼĴ���
	vu32	DLY;			//ADC��ʼ�ӳټĴ���
	vu32	DAT0;			//ADC���ݼĴ���0
	vu32	DAT1;			//ADC���ݼĴ���1
	vu32	UPDN;			//������UP-DOWN�Ĵ���
	vu32	CLRINT;			//ADC�ж�����Ĵ���
	u32		Reserved;
	vu32	CLRINTPNDNUP;	//���������жϼĴ���
}ADC_TypeDef;


//SDIO �Ĵ���
typedef struct
{
	vu32 	SDMASYSAD;		//0x7C200000	SDMAϵͳ���üĴ���
	vu16 	BLKSIZE;		//0x7C200004	���С���üĴ���
	vu16	BLKCNT;			//0x7C200006	������Ĵ���
	vu32	ARGUMENT;		//0x7C200008	���������Ĵ���
	vu16	TRNMOD;			//0x7C20000C	����ģʽ�Ĵ���
	vu16	CMDREG;			//0x7C20000E	����Ĵ���
	vu32	RSPREG[4];		//0x7C200010-0x7C20001C	Ӧ��Ĵ���
	vu32	BDATA;			//0x7C200020	���������ݶ˿ڼĴ���
	vu32	PRNSTS;			//0x7C200024	��ǰ״̬�Ĵ���
	vu8		HOSTCTL;		//0x7C200028	�������ƼĴ���
	vu8		PWRCON;			//0x7C200029	��Դ���ƼĴ���
	vu8		BLKGAP;			//0x7C20002A	�������ƼĴ���
	vu8		WAKCON;			//0x7C20002B	���ѿ��ƼĴ���
	vu16	CLKCON;			//0x7C20002C	ʱ�ӿ��ƼĴ���
	vu8		TIMEOUTCON;		//0x7C20002E	��ʱ���ƼĴ���
	vu8		SWRST;			//0x7C20002F	�����λ�Ĵ���
	
//	vu16	NORINTSTS;		//0x7C200030	�����ж�״̬�Ĵ���
//	vu16	ERRINTSTS;		//0x7C200032	�����ж�״̬�Ĵ���
	//������״̬������״̬�Ĵ����ϲ�,�������,��λΪ����״̬�Ĵ���,��λΪ����״̬��������
	vu32	INTSTS;			//�ж�״̬�Ĵ���
	vu32	INTSTSEN;		//�ж�״̬�ź�ʹ��
	vu32	INTSEGEN;		//�жϴ���ʹ��		
	
//	vu16	NORINTSTSEN;	//0x7C200034	�����ж�״̬ʹ�ܼĴ���
//	vu16	ERRINTSTSEN;	//0x7C200036	�����ж�״̬ʹ�ܼĴ���
//	vu16	NORINTSIGEN;	//0x7C200038	�����ж�״̬�ж�ʹ�ܼĴ���
//	vu16	ERRINTSIGEN;	//0x7C20003A	�����ж�״̬�ж�ʹ�ܼĴ���
	vu16	ACMD12ERRSTS;	//0x7C20003C	�Զ�CMD12����״̬�Ĵ���
	u16		Reserved0;
	vu32	CAPAREG;		//0x7C200040	��ѹ���޼Ĵ���
	u32		Reserved1;
	vu32	MAXCURR;		//0x7C200048	���������޼Ĵ���
	u32		Reserved2;
	vu16	FEAER;			//0x7C200050	FORCE EVENT REGISTER FOR AUTO CMD12 ERROR STATUS
	vu16	FEERR;			//0x7C200052	FORCE EVENT REGISTER FOR ERROR INTERRUPT STATUS
	vu32	ADMAERR;		//0x7C200054	ADMA����״̬�Ĵ���
	vu32	ADMASYSADDR;	//0x7C200058	ADMA��ַ�Ĵ���	
	u32		Reserved3[9];
	vu32	CONTROL2;		//0x7C200080	�������ƼĴ���2
	vu32	CONTROL3;		//0x7C200084	�������ƼĴ���3
	u32		Reserved4;
	vu32	CONTROL4;		//0x7C20008C	�������ƼĴ���4
	u16		Reserved5[55];
	vu16	HCVER;			//0x7C2000FE	�����������汾�Ĵ���
}SDIO_TypeDef;



//AC97������ �Ĵ���
typedef struct
{
	vu32	GLBCTRL;	 	 // 0x7F001000 ��/д 	AC97 ͨ�ÿ��ƼĴ����� 					0x00000000
	vu32	GLBSTAT;	 	 // 0x7F001004 �� 		AC97 ȫ��״̬�Ĵ����� 					0x00000001
	vu32	CODEC_CMD;	 	 // 0x7F001008 ��/д 	AC97 �����������Ĵ����� 				0x00000000
	vu32	CODEC_STAT;	 	 // 0x7F00100C ��		AC97 �������״̬�Ĵ����� 				0x00000000
	vu32	PCMADDR;	 	 // 0x7F001010 �� 		AC97 ��PCM ���/���� ͨ��FIFO ��ַ		0x00000000
	vu32	MICADDR;	 	 // 0x7F001014 �� 		AC97 ��MIC ����ͨ��FIFO ��ַ�Ĵ�����	0x00000000
	vu32	PCMDATA;	 	 // 0x7F001018 ��/д 	AC97 ��PCM ���/���� ͨ��FIFO ����		0x00000000
	vu32	MICDATA;	 	 // 0x7F00101C ��/д 	AC97 ��MIC ����ͨ��FIFO ���ݼĴ����� 	0x00000000
}AC97_TypeDef;


//SPI
typedef struct
{
	vu32	CHCFG;			//���üĴ���
	vu32	CLKCFG;			//ʱ�����üĴ���
	vu32	MODECFG;		//FIFO���ƼĴ���
	vu32	CS;				//������ѡ��Ĵ���
	vu32	INTEN;			//�ж������Ĵ���
	vu32	STATUS;			//״̬�Ĵ���
	vu32	TXDATA;			//�������ݼĴ���
	vu32	RXDATA;			//�������ݼĴ���
	vu32	CNT;			//�������������յ���������
	vu32	CLR;			//״̬���
	vu32	SWAPCFG	;		//�������üĴ���
	vu32	FBCLK;			//����ʱ��ѡ��Ĵ���	
}SPI_TypeDef;



//2D ͼ�μĴ�������
typedef struct
{
//			�Ĵ��� 				��ַӳ�� 		��/д 	���� 						��λֵ
//ͨ�üĴ���
	vu32	CONTROL;		// 0x76100000	д		����
	vu32	INTEN;			// 0x76100004 	��/д 	�ж���Ч�Ĵ����� 			0x0000_0000
	vu32	FIFO_INTC;		// 0x76100008 	��/д 	�жϿ��ƼĴ����� 			0x0000_0018
	vu32	INTC_PEND;		// 0x7610000C 	��/д 	�жϿ��Ƶȴ��Ĵ����� 		0x0000_0000
	vu32	FIFO_STAT;		// 0x76100010 	�� 		ָ��FIFO״̬�Ĵ�����		0x0000_0600
	vu32	FB_BASE;		// 0x76100020 	��/д 	֡��������ַ�Ĵ����� 		0x0000_0000
	u32		Reserved1[10];
//ָ��Ĵ���
	vu32	CMD0;			// 0x76100100 	д 		������/���ͼ��ָ��Ĵ����� -
	vu32	CMD1;			// 0x76100104	д 		����BitBLT��ָ��Ĵ����� -
	vu32	CMD2;			// 0x76100108 	д 		������������Bitblt���Ϳ�ʼ���ָ��Ĵ�����
	vu32	CMD3;			// 0x7610010C 	д	 	������������Bitblt���ͻָ���ָ��Ĵ�����
	vu32	CMD4;			// 0x76100110 	д 		���ڲ�ɫ��չ��ָ��Ĵ�������-�����������Ϳ�ʼ�㣩��
	vu32	CMD5;			// 0x76100114 	д 		���ڲ�ɫ��չ��ָ��Ĵ������������������ͻָ�����
	u32		Reserved2;
	vu32	CMD7;			// 0x7610011C 	д 		���ڲ�ɫ��չ��ָ��Ĵ������洢����������
//ָ����Դ�Ĵ���
	vu32	COLOR_MODE;		// 0x76100200 	��/д 	ɫ��ģʽ�Ĵ����� 			0x0000_0008
	vu32	HORI;			// 0x76100204 	��/д 	ˮƽ�ֱ��ʼĴ����� 			0x0000_0000
	vu32	SC_WIN;			// 0x76100210 	��/д 	��Ļ�ü����ڼĴ����� 		0x0000_0000
	vu32	SC_WIN_X;		// 0x76100214 	д 		��Ļ�ü����ڵ����X�Ĵ����� 	0x0000_0000
	vu32	SC_WIN_Y;		// 0x76100218 	д 		��Ļ�ü����ڵ����Y�Ĵ����� 	0x0000_0000
	vu32	CW_LT;			// 0x76100220 	��/д 	�ü��������Ϸ����ꡣ 		0x0000_0000
	vu32	CW_LT_X;		// 0x76100224 	д 		�ü�������X���ꡣ 			0x0000_0000-
	vu32	CW_LT_Y;		// 0x76100228 	д 		�ü�������Y���ꡣ 			0x0000_0000-
	vu32	CW_RB;			// 0x76100230 	��/д 	�ü��������·����ꡣ 		0x0000_0000
	vu32	CW_RB_X;		// 0x76100234 	д 		�ü�������X���ꡣ 			0x0000_0000-
	vu32	CW_RB_Y;		// 0x76100238 	д 		�ü�������Y���ꡣ 			0x0000_0000-
	vu32	COORD0;			// 0x76100300 	��/д 	����0�Ĵ����� 				0x0000_0000
	vu32	COORD0_X;		// 0x76100304 	д 		����0��X���ꡣ 				0x0000_0000-
	vu32	COORD0_Y;		// 0x76100308 	д 		����0��Y���ꡣ 				0x0000_0000-
	vu32	COORD1;			// 0x76100310 	��/д 	����1�Ĵ����� 				0x0000_0000
	vu32	COORD1_X;		// 0x76100314 	д 		����1��X���ꡣ 				0x0000_0000-
	vu32	COORD1_Y;		// 0x76100318 	д 		����1��Y���ꡣ 				0x0000_0000-
	vu32	COORD2;			// 0x76100320 	��/д 	����2�Ĵ����� 				0x0000_0000
	vu32	COORD2_X;		// 0x76100324 	д 		����2��X���ꡣ 				0x0000_0000-
	vu32	COORD2_Y;		// 0x76100328 	д 		����2��Y���ꡣ 				0x0000_0000-
	vu32	COORD3;			// 0x76100330 	��/д 	����3�Ĵ����� 				0x0000_0000
	vu32	COORD3_X;		// 0x76100334 	д 		����3��X���ꡣ 				0x0000_0000-
	vu32	COORD3_Y;		// 0x76100338 	д 		����3��Y���ꡣ 				0x0000_0000-
	vu32	ROT_OC;			// 0x76100340 	��/д 	��תԭ�����ꡣ 				0x0000_0000
	vu32	ROT_OC_X;		// 0x76100344 	д 		��תԭ�������X���ꡣ 		0x0000_0000-
	vu32	ROT_OC_Y;		// 0x76100348 	д 		��תԭ�������Y���ꡣ 		0x0000_0000-
	vu32	ROTATE;			// 0x7610034C 	��/д 	��תģʽ�Ĵ����� 			0x0000_0001
	vu32	ENDIA_READSIZE;	// 0x76100350 	��/д 	��˻�С��ѡ�񣬶�ȡ��˳�� 	0x0000_0001
	vu32	X_INCR;			// 0x76100400 	��/д 	X�����Ĵ����� 				0x0000_0000
	vu32	Y_INCR;			// 0x76100404 	��/д 	Y�����Ĵ����� 				0x0000_0000
	vu32	ROP;			// 0x76100410 	��/д 	��դ�����Ĵ����� 			0x0000_0000
	vu32	ALPHA;			// 0x76100420 	��/д 	��ֵ��˥��ƫ������ 			0x0000_0000
	vu32	FG_COLOR;		// 0x76100500 	��/д 	ǰ��ɫ/���Ĵ����� 			0x0000_0000
	vu32	BG_COLOR;		// 0x76100504 	��/д 	����ɫ�Ĵ����� 				0x0000_0000
	vu32	BS_COLOR;		// 0x76100508 	��/д 	������ɫ�Ĵ����� 			0x0000_0000
	vu32	PATTERN;		// 0x76100600��0x7610067C ��/д ͼ���Ĵ�����		 -
	vu32	PATOFF;			// 0x76100700 	��/д 	ͼ��ƫ����XY�Ĵ����� 		0x0000_0000
	vu32	PATOFF_X;		// 0x76100704 	д 		ͼ��ƫ����X�Ĵ����� 			0x0000_0000
	vu32	PATOFF_Y;		// 0x76100708 	д 		ͼ��ƫ����Y�Ĵ����� 			0x0000_0000
}G2D_TypeDef;


 
//**********************************************************************************//
//�����ַ�궨��


//�жϿ�����VIC��ַ 
#define VIC0_BASE	0x71200000
#define VIC1_BASE	0x71300000

//NAND����������ַ
#define NAND_BASE	0x70200000


//RTC ʵʱʱ�ӼĴ���
#define RTC_BASE	0x7E005030

//ADC �Ĵ���
#define ADC_BASE	0x7E00B000

//SDMMC	�Ĵ���
#define SDIO0_BASE 0x7C200000
#define SDIO1_BASE 0x7C300000
#define SDIO2_BASE 0x7C400000

//AC97 ������
#define AC97_BASE	0x7F001000

//G2D	//2Dͼ�μ���
#define G2D_BASE	0x76100000


 
//**********************************************************************************//
//����ṹ��ָ�붨��

//VIC
#ifdef _VIC0	/*VIC0*/
	#define VIC0        ((VICx_TypeDef*)VIC0_BASE)
#endif
#ifdef _VIC1	/*VIC1*/
	#define VIC1        ((VICx_TypeDef*)VIC1_BASE)
#endif

//MODEM
#ifdef _MODEM	/*MODEM*/
	#define MODEM		((MODEM_TypeDef*)MODEM_BASE)
#endif

//NAND
#ifdef _NAND	/*NAND*/
	#define NAND		((NAND_TypeDef*)NAND_BASE)
#endif


//RTC
#ifdef _RTC	/*RTC*/
	#define RTC		((RTC_TypeDef*)RTC_BASE)
#endif


//ADC
#ifdef _ADC	/*ADC*/
	#define ADC		((ADC_TypeDef*)ADC_BASE)
#endif


//SDMMC
#ifdef _SDIO0	/*SDIO0*/
	#define SDIO0	((SDIO_TypeDef*)SDIO0_BASE)
#endif

#ifdef _SDIO1	/*SDIO1*/
	#define SDIO1	((SDIO_TypeDef*)SDIO1_BASE)
#endif

#ifdef _SDIO2	/*SDIO2*/
	#define SDIO2	((SDIO_TypeDef*)SDIO2_BASE)
#endif


//AC97
#ifdef _AC97	/*AC97*/
	#define AC97	((AC97_TypeDef*)AC97_BASE)
#endif

//2D
#ifdef _G2D		/*G2D*/
	#define G2D		((G2D_TypeDef*)G2D_BASE)
#endif	
















//**********************************************************************************//
//ֱ�ӵ�ַӳ��

//UART�ļĴ���
//			�Ĵ��� 			��ַ 						  ��/д 	˵�� 											��ʼֵ
//UART0	
#ifdef	_UART0
	#define	rULCON0 		S3C6410_SFR32(0x7F005000) 		//��/д 	UART ͨ��0 �п��ƼĴ��� 						0x00
	#define	rUCON0 			S3C6410_SFR32(0x7F005004)		//��/д 	UART ͨ��0 ���ƼĴ��� 							0x00
	#define	rUFCON0 		S3C6410_SFR32(0x7F005008)		//��/д 	UART ͨ��0 FIFO ���ƼĴ��� 						0x0
	#define	rUMCON0 		S3C6410_SFR32(0x7F00500C)		//��/д 	UART ͨ��0 ���ƽ������Modem�����ƼĴ��� 		0x0
	#define	rUTRSTAT0 		S3C6410_SFR32(0x7F005010)		//�� 		UART ͨ��0 ����/����״̬�Ĵ��� 					0x6
	#define	rUERSTAT0 		S3C6410_SFR32(0x7F005014)		//�� 		UART ͨ��0 ���մ���״̬�Ĵ��� 					0x0
	#define	rUFSTAT0 		S3C6410_SFR32(0x7F005018)		//�� 		UART ͨ��0 FIFO ״̬�Ĵ��� 						0x00
	#define	rUMSTAT0 		S3C6410_SFR32(0x7F00501C)		//�� 		UART ͨ��0 ���ƽ������Modem��״̬�Ĵ��� 		0x0
	#define	rUTXH0 			S3C6410_SFR32(0x7F005020)		//д 		UART ͨ��0 ���ͻ���Ĵ��� 						-
	#define	rURXH0 			S3C6410_SFR32(0x7F005024)		//�� 		UART ͨ��0 ���ջ���Ĵ��� 						0x00
	#define	rUBRDIV0 		S3C6410_SFR32(0x7F005028)		//��/д 	UART ͨ��0 �����ʷ�Ƶ�Ĵ��� 					0x0000
	#define	rUDIVSLOT0 		S3C6410_SFR32(0x7F00502C)		//��/д 	UART ͨ��0 ��Ƶ��ۼĴ��� 						0x0000
	#define	rUINTP0 		S3C6410_SFR32(0x7F005030)		//��/д 	UART ͨ��0 �жϴ���Ĵ��� 						0x0
	#define	rUINTSP0 		S3C6410_SFR32(0x7F005034)		//��/д 	UART ͨ��0 �ж�Դ����Ĵ��� 					0x0
	#define	rUINTM0 		S3C6410_SFR32(0x7F005038)		//��/д 	UART ͨ��0 �ж����μĴ���						0x0
#endif
//UART1	
#ifdef	_UART1
	#define	rULCON1 		S3C6410_SFR32(0x7F005400)		//��/д 	UART ͨ��1 �п��ƼĴ��� 						0x00
	#define	rUCON1 			S3C6410_SFR32(0x7F005404)		//��/д 	UART ͨ��1 ���ƼĴ����� 						0x00
	#define	rUFCON1 		S3C6410_SFR32(0x7F005408)		//��/д 	UART ͨ��1 FIFO ���ƼĴ��� 						0x0
	#define	rUMCON1 		S3C6410_SFR32(0x7F00540C)		//��/д 	UART ͨ��1 ���ƽ������Modem�����ƼĴ��� 		0x0
	#define	rUTRSTAT1		S3C6410_SFR32(0x7F005410)		//�� 		UART ͨ��1 ����/����״̬�Ĵ��� 					0x6
	#define	rUERSTAT1 		S3C6410_SFR32(0x7F005414)		//�� 		UART ͨ��1 ���մ���״̬�Ĵ��� 					0x0
	#define	rUFSTAT1 		S3C6410_SFR32(0x7F005418)		//�� 		UART ͨ��1 FIFO ״̬�Ĵ��� 						0x00
	#define	rUMSTAT1 		S3C6410_SFR32(0x7F00541C)		//�� 		UART ͨ��1 ���ƽ������Modem��״̬�Ĵ��� 		0x0
	#define	rUTXH1 			S3C6410_SFR32(0x7F005420)		//д 		UART ͨ��1 ���ͻ���Ĵ���	 					-
	#define	rURXH1 			S3C6410_SFR32(0x7F005424)		//�� 		UART ͨ��1 ���ջ���Ĵ����� 					0x00
	#define	rUBRDIV1 		S3C6410_SFR32(0x7F005428)		//��/д 	UART ͨ��1 �����ʷ�Ƶ�Ĵ����� 					0x0000
	#define	rUDIVSLOT1 		S3C6410_SFR32(0x7F00542C)		//��/д 	UART ͨ��1 ��Ƶ��ۼĴ����� 					0x0000
	#define	rUINTP1 		S3C6410_SFR32(0x7F005430)		//��/д 	UART ͨ��1 �жϴ���Ĵ����� 					0x0
	#define	rUINTSP1		S3C6410_SFR32(0x7F005434)		//��/д 	UART ͨ��1 �ж�Դ����Ĵ����� 					0x0
	#define	rUINTM1 		S3C6410_SFR32(0x7F005438)		//��/д 	UART ͨ��1 �ж����μĴ����� 					0x0
#endif
//UART2	
#ifdef	_UART2
	#define	rULCON2 		S3C6410_SFR32(0x7F005800)		//��/д 	UART ͨ��2 �п��ƼĴ����� 						0x00
	#define	rUCON2 			S3C6410_SFR32(0x7F005804)		//��/д 	UART ͨ��2 ���ƼĴ����� 						0x00
	#define	rUFCON2 		S3C6410_SFR32(0x7F005808)		//��/д 	UART ͨ��2 FIFO ���ƼĴ����� 					0x0
	#define	rUTRSTAT2 		S3C6410_SFR32(0x7F005810)		//�� 		UART ͨ��2 ����/����״̬�Ĵ����� 				0x6
	#define	rUERSTAT2 		S3C6410_SFR32(0x7F005814)		//�� 		UART ͨ��2 ���մ���״̬�Ĵ����� 				0x0
	#define	rUFSTAT2 		S3C6410_SFR32(0x7F005818)		//�� 		UART ͨ��2 FIFO ״̬�Ĵ����� 					0x00
	#define	rUTXH2 			S3C6410_SFR32(0x7F005820)		//д 		UART ͨ��2 ���ͻ���Ĵ����� 					-
	#define	rURXH2 			S3C6410_SFR32(0x7F005824)		//�� 		UART ͨ��2 ���ջ���Ĵ����� 					0x00
	#define	rUBRDIV2 		S3C6410_SFR32(0x7F005828)		//��/д 	UART ͨ��2 �����ʷ�Ƶ�Ĵ����� 					0x0000
	#define	rUDIVSLOT2 		S3C6410_SFR32(0x7F00582C)		//��/д 	UART ͨ��2 ��Ƶ��ۼĴ����� 					0x0000
	#define	rINTP2 			S3C6410_SFR32(0x7F005830)		//��/д 	UART ͨ��2 �жϴ���Ĵ����� 					0x0
	#define	rUINTM2 		S3C6410_SFR32(0x7F005838)		//��/д 	UART ͨ��2 �ж����μĴ����� 					0x0
#endif
//UART3
#ifdef	_UART3
	#define	rULCON3 		S3C6410_SFR32(0x7F005C00)		//��/д 	UART ͨ��3 �п��ƼĴ����� 						0x00
	#define	rUCON3 			S3C6410_SFR32(0x7F005C04)		//��/д	UART ͨ��3 ���ƼĴ����� 							0x00
	#define	rUFCON3 		S3C6410_SFR32(0x7F005C08)		//��/д 	UART ͨ��3 FIFO ���ƼĴ����� 					0x0
	#define	rUTRSTAT3 		S3C6410_SFR32(0x7F005C10)		//�� 		UART ͨ��3 ����/����״̬�Ĵ����� 				0x6
	#define	rUERSTAT3 		S3C6410_SFR32(0x7F005C14)		//�� 		UART ͨ��3 ���մ���״̬�Ĵ����� 				0x0
	#define	rUFSTAT3 		S3C6410_SFR32(0x7F005C18)		//�� 		UART ͨ��3 FIFO ״̬�Ĵ����� 					0x00
	#define	rUTXH3 			S3C6410_SFR32(0x7F005C20)		//д 		UART ͨ��3 ���ͻ���Ĵ����� 					-
	#define	rURXH3 			S3C6410_SFR32(0x7F005C24)		//�� 		UART ͨ��3 ���ջ���Ĵ����� 					0x00
	#define	rUBRDIV3 		S3C6410_SFR32(0x7F005C28)		//��/д 	UART ͨ��3 �����ʷ�Ƶ�Ĵ����� 					0x0000
	#define	rUDIVSLOT3 		S3C6410_SFR32(0x7F005C2C)		//��/д 	UART ͨ��3 ��Ƶ��ۼĴ����� 					0x0000
	#define	rINTP3 			S3C6410_SFR32(0x7F005C30)		//��/д 	UART ͨ��3 �жϴ���Ĵ����� 					0x0
	#define	rUINTSP3 		S3C6410_SFR32(0x7F005C34)		//��/д 	UART ͨ��3 �ж�Դ����Ĵ����� 					0x0
	#define	rUINTM3 		S3C6410_SFR32(0x7F005C38)		//��/д 	UART ͨ��3 �ж����μĴ����� 					0x0
#endif


//ϵͳ���ƼĴ���
//			����ͻ��ϵͳ�������ڵ�34 ���Ĵ�����
//		�Ĵ��� 			��ַ 						   ��/д 	���� 						��λֵ
#define	rAPLL_LOCK		S3C6410_SFR32(0x7E00F000)		// ��/д 	����PLL ������APLL�� 		0x0000_FFFF
#define	rMPLL_LOCK 		S3C6410_SFR32(0x7E00F004)		// ��/д 	����PLL ������MPLL��	 	0x0000_FFFF
#define	rEPLL_LOCK 		S3C6410_SFR32(0x7E00F008)		// ��/д 	����PLL ������EPLL�� 		0x0000_FFFF
#define	rAPLL_CON 		S3C6410_SFR32(0x7E00F00C)		// ��/д 	����PLL ���Ƶ�� APLL�� 	0x0190_0302
#define	rMPLL_CON 		S3C6410_SFR32(0x7E00F010)		// ��/д 	����PLL ���Ƶ�� MPLL�� 	0x0214_0603
#define	rEPLL_CON0 		S3C6410_SFR32(0x7E00F014)		// ��/д 	����PLL ���Ƶ�� EPLL�� 	0x0020_0102
#define	rEPLL_CON1 		S3C6410_SFR32(0x7E00F018)		// ��/д 	����PLL ���Ƶ�� EPLL�� 	0x0000_9111
#define	rCLK_SRC 		S3C6410_SFR32(0x7E00F01C)		// ��/д 	ѡ��ʱ��Դ�� 				0x0000_0000
#define	rCLK_DIV0 		S3C6410_SFR32(0x7E00F020)		// ��/д 	����ʱ�ӷ�Ƶ���ı����� 		0x0105_1000
#define	rCLK_DIV1 		S3C6410_SFR32(0x7E00F024)		// ��/д 	����ʱ�ӷ�Ƶ���ı����� 		0x0000_0000
#define	rCLK_DIV2 		S3C6410_SFR32(0x7E00F028)		// ��/д 	����ʱ�ӷ�Ƶ���ı����� 		0x0000_0000
#define	rCLK_OUT 		S3C6410_SFR32(0x7E00F02C)		// ��/д 	ѡ��ʱ������� 				0x0000_0000
#define	rHCLK_GATE 		S3C6410_SFR32(0x7E00F030)		// ��/д 	����HCLK ʱ��ѡͨ�� 		0xFFFF_FFFF
#define	rPCLK_GATE 		S3C6410_SFR32(0x7E00F034)		// ��/д 	����PCLK ʱ��ѡͨ�� 		0xFFFF_FFFF
#define	rSCLK_GATE 		S3C6410_SFR32(0x7E00F038)		// ��/д 	����SCLK ʱ��ѡͨ�� 		0xFFFF_FFFF
#define	rAHB_CON0 		S3C6410_SFR32(0x7E00F100)		// ��/д 	����AHB I/P/X/F ���ߡ� 		0x0400_0000
#define	rAHB_CON1 		S3C6410_SFR32(0x7E00F104)		// ��/д 	����AHB M1/M0/T1/T0 ���ߡ�	0x0000_0000
#define	rAHB_CON2 		S3C6410_SFR32(0x7E00F108)		// ��/д 	����AHB R/S1/S0 ���ߡ� 		0x0000_0000
#define	rSDMA_SEL 		S3C6410_SFR32(0x7E00F110)		// ��/д 	ѡ��ȫDMA ���롣 			0x0000_0000
#define	rSW_RST 		S3C6410_SFR32(0x7E00F114)		// ��/д 	���������λ�� 				0x0000_0000
#define	rSYS_ID 		S3C6410_SFR32(0x7E00F118)		// �� 		ϵͳID �汾�����ͨ���� 	0x0000_0000
#define	rMEM_SYS_CFG 	S3C6410_SFR32(0x7E00F120)		// ��/д 	���ô洢����ϵͳ�� 			0x0000_0080
#define	rQOS_OVERRIDE0 	S3C6410_SFR32(0x7E00F124)		// ��/д 	ȡ��DMC0 QOS�� 				0x0000_0000
#define	rQOS_OVERRIDE1 	S3C6410_SFR32(0x7E00F128)		// ��/д 	ȡ��DMC1 QOS�� 				0x0000_0000
#define	rMEM_CFG_STAT 	S3C6410_SFR32(0x7E00F12C)		// �� ��	������ϵͳ����״̬�� 		0x0000_0000
#define	rPWR_CFG 		S3C6410_SFR32(0x7E00F804)		// ��/д 	���õ�Դ���� 				0x0000_0001
#define	rEINT_MASK 		S3C6410_SFR32(0x7E00F808)		// ��/д 	����EINT���ⲿ�жϣ����Ρ� 	0x0000_0000
#define	rNORMAL_CFG 	S3C6410_SFR32(0x7E00F810)		// ��/д 	������ģʽ�£����õ�Դ����0xFFFF_FF00
#define	rSTOP_CFG 		S3C6410_SFR32(0x7E00F814)		// ��/д 	��ֹͣģʽ�£����õ�Դ����0x2012_0100
#define	rSLEEP_CFG 		S3C6410_SFR32(0x7E00F818)		// ��/д 	��˯��ģʽ�£����õ�Դ����0x0000_0000
#define	rOSC_FREQ 		S3C6410_SFR32(0x7E00F820)		// ��/д 	����Ƶ�ʿ̶ȼ������� 		0x0000_000F
#define	rPWR_STABLE 	S3C6410_SFR32(0x7E00F828)		// ��/д 	��Դ�ȶ��������� 			0x0000_0001
#define	rMTC_STABLE 	S3C6410_SFR32(0x7E00F830)		// ��/д 	MTC �ȶ��������� 			0xFFFF_FFFF
#define	rOTHERS 		S3C6410_SFR32(0x7E00F900)		// ��/д 	�������ƼĴ����� 			0x0000_801E
#define	rRST_STAT 		S3C6410_SFR32(0x7E00F904)		// �� 		��λ״̬�Ĵ����� 			0x0000_0001
#define	rWAKEUP_STAT 	S3C6410_SFR32(0x7E00F908)		// ��/д 	����״̬�Ĵ����� 			0x0000_0000
#define	rBLK_PWR_STAT 	S3C6410_SFR32(0x7E00F90C)		// �� ��	��Դ״̬�Ĵ����� 			0x0000_007F
#define	rINFO_RM0 		S3C6410_SFR32(0x7E00FA00)		// ��/д 	��Ϣ�Ĵ���0�� 				0x0000_0000
#define	rINFO_RM1 		S3C6410_SFR32(0x7E00FA04)		// ��/д 	��Ϣ�Ĵ���1��				0x0000_0000
#define	rINFO_RM2 		S3C6410_SFR32(0x7E00FA08)		// ��/д 	��Ϣ�Ĵ���2�� 				0x0000_0000
#define	rINFO_RM3 		S3C6410_SFR32(0x7E00FA0C)		// ��/д 	��Ϣ�Ĵ���3�� 				0x0000_0000


//GPIO���ƼĴ���
//		�Ĵ��� 			��ַ 								��/д 	���� 						��λֵ
#define	rGPACON 		S3C6410_SFR32(0x7F008000)		// ��/д 	�˿�A ���üĴ��� 			0x0
#define	rGPADAT 		S3C6410_SFR32(0x7F008004)		// ��/д 	�˿�A ���ݼĴ��� 			δ����
#define	rGPAPUD 		S3C6410_SFR32(0x7F008008)		// ��/д 	�˿�A ����/�����Ĵ��� 		0x0000555
#define	rGPACONSLP 		S3C6410_SFR32(0x7F00800C)		// ��/д 	�˿�A ˯��ģʽ���üĴ��� 	0x0
#define	rGPACONSLP 		S3C6410_SFR32(0x7F00800C)		// ��/д 	�˿�A ˯��ģʽ���üĴ��� 	0x0
#define	rGPAPUDSLP 		S3C6410_SFR32(0x7F008010)		// ��/д 	�˿�A ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPBCON 		S3C6410_SFR32(0x7F008020)		// ��/д 	�˿�B ���üĴ��� 			0x40000
#define	rGPBDAT 		S3C6410_SFR32(0x7F008024)		// ��/д 	�˿�B ���ݼĴ��� 			δ����
#define	rGPBPUD 		S3C6410_SFR32(0x7F008028)		// ��/д 	�˿�B ����/�����Ĵ��� 		0x00001555
#define	rGPBCONSLP 		S3C6410_SFR32(0x7F00802C)		// ��/д 	�˿�B ˯��ģʽ���üĴ��� 	0x0
#define	rGPBPUDSLP 		S3C6410_SFR32(0x7F008030)		// ��/д 	�˿�B ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPCCON 		S3C6410_SFR32(0x7F008040)		// ��/д 	�˿�C ���üĴ��� 			0x0
#define	rGPCDAT 		S3C6410_SFR32(0x7F008044)		// ��/д 	�˿�C ���ݼĴ��� 			δ����
#define	rGPCPUD 		S3C6410_SFR32(0x7F008048)		// ��/д 	�˿�C ����/�����Ĵ��� 		0x00005555
#define	rGPCCONSLP 		S3C6410_SFR32(0x7F00804C)		// ��/д 	�˿�C ˯��ģʽ���üĴ��� 	0x0
#define	rGPCPUDSLP 		S3C6410_SFR32(0x7F008050)		// ��/д 	�˿�C ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPDCON 		S3C6410_SFR32(0x7F008060)		// ��/д 	�˿�D ���üĴ��� 			0x0
#define	rGPDDAT 		S3C6410_SFR32(0x7F008064)		// ��/д 	�˿�D ���ݼĴ��� 			δ����
#define	rGPDPUD 		S3C6410_SFR32(0x7F008068)		// ��/д 	�˿�D ����/�����Ĵ��� 		0x00000155
#define	rGPDCONSLP 		S3C6410_SFR32(0x7F00806C)		// ��/д 	�˿�D ˯��ģʽ���üĴ��� 	0x0
#define	rGPDPUDSLP 		S3C6410_SFR32(0x7F008070)		// ��/д 	�˿�D ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPECON 		S3C6410_SFR32(0x7F008080)		// ��/д 	�˿�E ���üĴ��� 			0x0
#define	rGPEDAT 		S3C6410_SFR32(0x7F008084)		// ��/д 	�˿�E ���ݼĴ��� 			δ����
#define	rGPEPUD 		S3C6410_SFR32(0x7F008088)		// ��/д 	�˿�E ����/�����Ĵ��� 		0x00000155
#define	rGPECONSLP 		S3C6410_SFR32(0x7F00808C)		// ��/д 	�˿�E ˯��ģʽ���üĴ��� 	0x0
#define	rGPEPUDSLP 		S3C6410_SFR32(0x7F008090)		// ��/д 	�˿�E ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPFCON 		S3C6410_SFR32(0x7F0080A0)		// ��/д 	�˿�F ���üĴ��� 			0x0
#define	rGPFDAT 		S3C6410_SFR32(0x7F0080A4)		// ��/д 	�˿�F ���ݼĴ��� 			δ����
#define	rGPFPUD 		S3C6410_SFR32(0x7F0080A8)		// ��/д 	�˿�F ����/�����Ĵ��� 		0x55555555
#define	rGPFCONSLP 		S3C6410_SFR32(0x7F0080AC)		// ��/д 	�˿�F ˯��ģʽ���üĴ��� 	0x0
#define	rGPFPUDSLP 		S3C6410_SFR32(0x7F0080B0)		// ��/д 	�˿�F ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPGCON 		S3C6410_SFR32(0x7F0080C0)		// ��/д 	�˿�G ���üĴ��� 			0x0
#define	rGPGDAT 		S3C6410_SFR32(0x7F0080C4)		// ��/д 	�˿�G ���ݼĴ��� 			δ����
#define	rGPGPUD 		S3C6410_SFR32(0x7F0080C8)		// ��/д 	�˿�G ����/�����Ĵ��� 		0x00001555
#define	rGPGCONSLP 		S3C6410_SFR32(0x7F0080CC)		// ��/д 	�˿�G ˯��ģʽ���üĴ��� 	0x0
#define	rGPGPUDSLP 		S3C6410_SFR32(0x7F0080D0)		// ��/д 	�˿�G ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPHCON 		S3C6410_SFR32(0x7F0080E0)		// ��/д 	�˿�H ���üĴ��� 			0x0
#define	rGPHDAT 		S3C6410_SFR32(0x7F0080E4)		// ��/д 	�˿�H ���ݼĴ��� 			δ����
#define	rGPHPUD 		S3C6410_SFR32(0x7F0080E8)		// ��/д 	�˿�H ����/�����Ĵ��� 		0x00055555
#define	rGPHCONSLP 		S3C6410_SFR32(0x7F0080EC)		// ��/д 	�˿�H ˯��ģʽ���üĴ��� 	0x0
#define	rGPHPUDSLP 		S3C6410_SFR32(0x7F0080F0)		// ��/д 	�˿�H ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPICON 		S3C6410_SFR32(0x7F008100)		// ��/д 	�˿�I ���üĴ��� 			0x0
#define	rGPIDAT 		S3C6410_SFR32(0x7F008104)		// ��/д 	�˿�I ���ݼĴ��� 			δ����
#define	rGPIPUD 		S3C6410_SFR32(0x7F008108)		// ��/д 	�˿�I ����/�����Ĵ��� 		0x55555555
#define	rGPICONSLP 		S3C6410_SFR32(0x7F00810C)		// ��/д 	�˿�I ˯�����üĴ��� 		0x0
#define	rGPIPUDSLP 		S3C6410_SFR32(0x7F008110)		// ��/д 	�˿�I ˯����/�����Ĵ��� 	0x0
#define	rGPJCON 		S3C6410_SFR32(0x7F008120)		// ��/д 	�˿�J ���üĴ��� 			0x0
#define	rGPJDAT 		S3C6410_SFR32(0x7F008124)		// ��/д 	�˿�J ���ݼĴ��� 			δ����
#define	rGPJPUD 		S3C6410_SFR32(0x7F008128)		// ��/д 	�˿�J ����/�����Ĵ��� 		0x00555555
#define	rGPJCONSLP 		S3C6410_SFR32(0x7F00812C)		// ��/д 	�˿�J ˯��ģʽ���üĴ��� 	0x0
#define	rGPJPUDSLP 		S3C6410_SFR32(0x7F008130)		// ��/д	�˿�J ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPKCON0 		S3C6410_SFR32(0x7F008800)		// ��/д 	�˿�K ���üĴ���0 			0x22222222
#define	rGPKCON1 		S3C6410_SFR32(0x7F008804)		// ��/д 	�˿�K ���üĴ���1 			0x22222222
#define	rGPKDAT 		S3C6410_SFR32(0x7F008808)		// ��/д 	�˿�K ���ݼĴ��� 			δ����
#define	rGPKPUD 		S3C6410_SFR32(0x7F00880C)		// ��/д	�˿�K ����/�����Ĵ��� 		0x55555555
#define	rGPLCON0 		S3C6410_SFR32(0x7F008810)		// ��/д 	�˿�L ���üĴ���0 			0x22222222
#define	rGPLCON1 		S3C6410_SFR32(0x7F008814)		// ��/д 	�˿�L ���üĴ���1 			0x22222222
#define	rGPLDAT 		S3C6410_SFR32(0x7F008818)		// ��/д 	�˿�L ���ݼĴ��� 			δ����
#define	rGPLPUD 		S3C6410_SFR32(0x7F00881C)		// ��/д 	�˿�L ����/�����Ĵ��� 		0x55555555
#define	rGPMCON 		S3C6410_SFR32(0x7F008820)		// ��/д 	�˿�M ���üĴ��� 			0x00222222
#define	rGPMDAT 		S3C6410_SFR32(0x7F008824)		// ��/д 	�˿�M ���ݼĴ��� 			δ����
#define	rGPMPUD 		S3C6410_SFR32(0x7F008828)		// ��/д 	�˿�M ����/�����Ĵ��� 		0x000002AA
#define	rGPNCON 		S3C6410_SFR32(0x7F008830)		// ��/д 	�˿�N ���üĴ��� 			0x0
#define	rGPNDAT 		S3C6410_SFR32(0x7F008834)		// ��/д 	�˿�N ���ݼĴ��� 			δ����
#define	rGPNPUD 		S3C6410_SFR32(0x7F008838)		// ��/д 	�˿�N ����/�����Ĵ��� 		0x55555555
#define	rGPOCON 		S3C6410_SFR32(0x7F008140)		// ��/д 	�˿�O ���üĴ��� 			0xAAAAAAAA
#define	rGPODAT 		S3C6410_SFR32(0x7F008144)		// ��/д 	�˿�O ���ݼĴ��� 			δ����
#define	rGPOPUD 		S3C6410_SFR32(0x7F008148)		// ��/д 	�˿�O ����/�����Ĵ��� 		0x0
#define	rGPOCONSLP 		S3C6410_SFR32(0x7F00814C)		// ��/д 	�˿�O ˯��ģʽ���üĴ��� 	0x0
#define	rGPOPUDSLP 		S3C6410_SFR32(0x7F008150)		// ��/д 	�˿�O ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPPCON 		S3C6410_SFR32(0x7F008160)		// ��/д 	�˿�P ���üĴ��� 			0x2AAAAAAA
#define	rGPPDAT 		S3C6410_SFR32(0x7F008164)		// ��/д 	�˿�P ���ݼĴ��� 			δ����
#define	rGPPPUD 		S3C6410_SFR32(0x7F008168)		// ��/д 	�˿�P ����/�����Ĵ��� 		0x1011AAA0
#define	rGPPCONSLP 		S3C6410_SFR32(0x7F00816C)		// ��/д 	�˿�P ˯��ģʽ���üĴ��� 	0x0
#define	rGPPPUDSLP 		S3C6410_SFR32(0x7F008170)		// ��/д 	�˿�P ˯��ģʽ��/�����Ĵ��� 0x0
#define	rGPQCON 		S3C6410_SFR32(0x7F008180)		// ��/д 	�˿�Q ���üĴ��� 			0x0002AAAA
#define	rGPQDAT 		S3C6410_SFR32(0x7F008184)		// ��/д 	�˿�Q ���ݼĴ��� 			δ����
#define	rGPQPUD 		S3C6410_SFR32(0x7F008188)		// ��/д 	�˿�Q ����/�����Ĵ��� 		0x00001555
#define	rGPQCONSLP 		S3C6410_SFR32(0x7F00818C)		// ��/д 	�˿�Q ˯��ģʽ���üĴ��� 	0x0
#define	rGPQPUDSLP 		S3C6410_SFR32(0x7F008180)		// ��/д 	�˿�Q ˯��ģʽ��/�����Ĵ��� 0x0
#define	rSPCON 			S3C6410_SFR32(0x7F0081A0)		// ��/д 	����˿����üĴ��� 			0Xbfc11500
#define	rMEM0IOCONSLP0 	S3C6410_SFR32(0x7F0081C0)		// ��/д 	�洢���˿�0 ˯�� ģʽ����0 	0x0
#define	rMEM0IOCONSLP1 	S3C6410_SFR32(0x7F0081C4)		// ��/д 	�洢���˿�0 ˯�� ģʽ����1 	0x0
#define	rMEM0IOCONSLP 	S3C6410_SFR32(0x7F0081C8)		// ��/д 	�洢���˿�0 ˯��ģʽ���� 	0x0
#define	rMEM0IODRVCON 	S3C6410_SFR32(0x7F0081D0)		// ��/д 	�洢���˿�0 �������ƼĴ��� 	0x10555551
#define	rMEM1IODRVCON 	S3C6410_SFR32(0x7F0081D4)		// ��/д 	�洢���˿�0 �������ƼĴ��� 	0x0


//�ⲿ�жϿ��ƼĴ���
/*����ı�����ⲿ�жϿ��ƼĴ������嵥��0 ����ר�õĹܽţ�0 ����жϿ��Ա���������жϿ���
��������ݡ�S3C6410 ��ʾ��0 ��Ķ���Ĵ����Լ�ÿ���ж��ź���ռ��λ��������ͬ���ж���Ĵ�����
ÿ���Ĵ������Կ���2 ��������顣0 �������˲�����������Դ��FIN��������������˲�����������
Դ��PCLK.
		�Ĵ��� 			��ַ 						   ��/д 	���� 								��λֵ*/
#define	rEINT0CON0		S3C6410_SFR32(0x7F008900)		// ��/д 	�ⲿ�ж�0��0 �飩���üĴ���0 		0x0
#define	rEINT0CON1 		S3C6410_SFR32(0x7F008904)		// ��/д 	�ⲿ�ж�0��0 �飩���üĴ���1 		0x0
#define	rEINT0FLTCON0 	S3C6410_SFR32(0x7F008910)		// ��/д 	�ⲿ�ж�0��0 �飩���˿��ƼĴ���0 	0x0
#define	rEINT0FLTCON1 	S3C6410_SFR32(0x7F008914)		// ��/д 	�ⲿ�ж�0��0 �飩���˿��ƼĴ���1 	0x0
#define	rEINT0FLTCON2 	S3C6410_SFR32(0x7F008918)		// ��/д 	�ⲿ�ж�0��0 �飩���˿��ƼĴ���2 	0x0
#define	rEINT0FLTCON3 	S3C6410_SFR32(0x7F00891C)		// ��/д 	�ⲿ�ж�0��0 �飩���˿��ƼĴ���3 	0x0
#define	rEINT0MASK 		S3C6410_SFR32(0x7F008920)		// ��/д 	�ⲿ�ж�0��0 �飩���μĴ��� 		0x0FFFFFFF
#define	rEINT0PEND 		S3C6410_SFR32(0x7F008924)		// ��/д 	�ⲿ�ж�0��0 �飩���ҼĴ��� 		0x0
#define	rEINT12CON 		S3C6410_SFR32(0x7F008200)		// ��/д 	�ⲿ�ж�1��2��1��2 �飩���üĴ��� 	0x0
#define	rEINT34CON 		S3C6410_SFR32(0x7F008204)		// ��/д 	�ⲿ�ж�3��4��3��4 �飩���üĴ��� 	0x0
#define	rEINT56CON 		S3C6410_SFR32(0x7F008208)		// ��/д 	�ⲿ�ж�5��6��5��6 �飩���üĴ��� 	0x0
#define	rEINT78CON 		S3C6410_SFR32(0x7F00820C)		// ��/д 	�ⲿ�ж�7��8��7��8 �飩���üĴ��� 	0x0
#define	rEINT9CON 		S3C6410_SFR32(0x7F008210)		// ��/д 	�ⲿ�ж�9��9 �飩���üĴ��� 0x0
#define	rEINT12FLTCON 	S3C6410_SFR32(0x7F008220)		// ��/д 	�ⲿ�ж�1��2��1��2 �飩���˿��ƼĴ���0x0
#define	rEINT34FLTCON 	S3C6410_SFR32(0x7F008224)		// ��/д 	�ⲿ�ж�3��4��3��4 �飩���˿��ƼĴ���0x0
#define	rEINT56FLTCON 	S3C6410_SFR32(0x7F008228)		// ��/д 	�ⲿ�ж�5��6��5��6 �飩���˿��ƼĴ���0x0
#define	rEINT78FLTCON 	S3C6410_SFR32(0x7F00822C)		// ��/д 	�ⲿ�ж�7��8��7��8 �飩���˿��ƼĴ���0x0
#define	rEINT9FLTCON 	S3C6410_SFR32(0x7F008230)		// ��/д 	�ⲿ�ж�9��9 �飩���˿��ƼĴ���0 	0x0
#define	rEINT12MASK 	S3C6410_SFR32(0x7F008240)		// ��/д 	�ⲿ�ж�1��2��1��2 �飩���μĴ��� 	0x00FF7FFF
#define	rEINT34MASK 	S3C6410_SFR32(0x7F008244)		// ��/д 	�ⲿ�ж�3��4��3��4 �飩���μĴ��� 	0x3FFF03FF
#define	rEINT56MASK 	S3C6410_SFR32(0x7F008248)		// ��/д 	�ⲿ�ж�5��6��5��6 �飩���μĴ��� 	0x03FF007F
#define	rEINT78MASK 	S3C6410_SFR32(0x7F00824C)		// ��/д 	�ⲿ�ж�7��8��7��8 �飩���μĴ��� 	0x7FFFFFFF
#define	rEINT9MASK 		S3C6410_SFR32(0x7F008250)		// ��/д 	�ⲿ�ж�9��9 �飩���δ��� 			0x000001FF
#define	rEINT12PEND 	S3C6410_SFR32(0x7F008260)		// ��/д 	�ⲿ�ж�1��2��1��2 �飩���ҼĴ��� 	0x0
#define	rEINT34PEND 	S3C6410_SFR32(0x7F008264)		// ��/д 	�ⲿ�ж�3��4��3��4 �飩���ҼĴ��� 	0x0
#define	rEINT56PEND 	S3C6410_SFR32(0x7F008268)		// ��/д 	�ⲿ�ж�5��6��5��6 �飩���ҼĴ��� 	0x0
#define	rEINT78PEND 	S3C6410_SFR32(0x7F00826C)		// ��/д 	�ⲿ�ж�7��8��7��8 �飩���ҼĴ��� 	0x0
#define	rEINT9PEND 		S3C6410_SFR32(0x7F008270)		// ��/д 	�ⲿ�ж�9��9 �飩���ҼĴ��� 		0x0
#define	rPRIORITY 		S3C6410_SFR32(0x7F008280)		// ��/д 	���ȿ��ƼĴ��� 						0x3FF
#define	rSERVICE 		S3C6410_SFR32(0x7F008284)		// �� 		��ǰ����Ĵ��� 						0x0
#define	rSERVICEPEND 	S3C6410_SFR32(0x7F008288)		// ��/д 	��ǰ�������ҼĴ��� 					0x0


//��ʱ����ؼĴ���(0~4)
//ֻ�ж�ʱ��0,1���ⲿPWM���ģʽ,��ʱ��2,3,4���ⲿ����,�����ڲ���ʱ��
//		�Ĵ��� 		ƫ���� 							��/д 		���� 													��λֵ
#define	rTCFG0  	S3C6410_SFR32(0x7F006000)		//  ��/д 		��ʱ�����üĴ���0 ʱ��������8 λԤ���������������ȡ�	0x0000_0101
#define	rTCFG1  	S3C6410_SFR32(0x7F006004)		//  ��/д 		��ʱ�����üĴ���1 ʱ��5-MUX ��DMA ģʽѡ��Ĵ�����		0x0000_0000
#define	rTCON  		S3C6410_SFR32(0x7F006008)		//  ��/д 		��ʱ�����ƼĴ����� 										0x0000_0000
#define	rTCNTB0  	S3C6410_SFR32(0x7F00600C)		//  ��/д 		��ʱ��0 ������������ 									0x0000_0000
#define	rTCMPB0  	S3C6410_SFR32(0x7F006010)		//  ��/д 		��ʱ��0 �Ƚϻ���Ĵ����� 								0x0000_0000
#define	rTCNTO0  	S3C6410_SFR32(0x7F006014)		//  �� 			��ʱ��0 �����۲�Ĵ����� 								0x0000_0000
#define	rTCNTB1  	S3C6410_SFR32(0x7F006018)		//  ��/д 		��ʱ��1 ������������ 									0x0000_0000
#define	rTCMPB1  	S3C6410_SFR32(0x7F00601c)		//  ��/д 		��ʱ��1 �Ƚϻ���Ĵ����� 								0x0000_0000
#define	rTCNTO1  	S3C6410_SFR32(0x7F006020)		//  �� 			��ʱ��1 �����۲�Ĵ����� 								0x0000_0000
#define	rTCNTB2  	S3C6410_SFR32(0x7F006024)		//  ��/д 		��ʱ��2 ������������ 									0x0000_0000
#define	rTCMPB2  	S3C6410_SFR32(0x7F006028)		//  ��/д 		��ʱ��2 �Ƚϻ���Ĵ����� 								0x0000_0000
#define	rTCNTO2  	S3C6410_SFR32(0x7F00602c)		//  �� 			��ʱ��2 �����۲�Ĵ����� 								0x0000_0000
#define	rTCNTB3  	S3C6410_SFR32(0x7F006030)		//  ��/д 		��ʱ��3 ������������ 									0x0000_0000
#define	rTCMPB3  	S3C6410_SFR32(0x7F006034)		//  ��/д 		��ʱ��3 �Ƚϻ���Ĵ����� 								0x0000_0000
#define	rTCNTO3  	S3C6410_SFR32(0x7F006038)		//  �� 			��ʱ��3 �����۲�Ĵ����� 								0x0000_0000
#define	rTCNTB4  	S3C6410_SFR32(0x7F00603c)		//  ��/д 		��ʱ��4 ������������ 									0x0000_0000
#define	rTCNTO4  	S3C6410_SFR32(0x7F006040)		//  �� 			��ʱ��4 �����۲�Ĵ����� 								0x0000_0000
#define	rTINT_CSTAT S3C6410_SFR32(0x7F006044)		//  ��/д 		��ʱ���жϿ��ƺ�״̬�Ĵ����� 							0x0000_0000




	 
//��ʾ������(TFT_LCD������) 
//SFR �ڴ�ӳ��
//		�Ĵ��� 			��ַ 							 ��/д 			���� 									��λֵ
#define	rVIDCON0   		S3C6410_SFR32(0x77100000)		//   ��/д 		��Ƶ����0 �Ĵ����� 						0x0000_0000
#define	rVIDCON1   		S3C6410_SFR32(0x77100004)		//   ��/д 		��Ƶ����1 �Ĵ��� 						0x0000_0000
#define	rVIDCON2   		S3C6410_SFR32(0x77100008)		//   ��/д 		��Ƶ����2 �Ĵ��� 						0x0000_0000
#define	rVIDTCON0   	S3C6410_SFR32(0x77100010)		//   ��/д 		��Ƶʱ�����0 �Ĵ��� 					0x0000_0000
#define	rVIDTCON1   	S3C6410_SFR32(0x77100014)		//   ��/д 		��Ƶʱ�����1 �Ĵ��� 					0x0000_0000
#define	rVIDTCON2   	S3C6410_SFR32(0x77100018)		//   ��/д 		��Ƶʱ�����2 �Ĵ��� 					0x0000_0000
#define	rWINCON0   		S3C6410_SFR32(0x77100020)		//   ��/д 		���ڿ���0 �Ĵ��� 						0x0000_0000
#define	rWINCON1   		S3C6410_SFR32(0x77100024)		//   ��/д 		���ڿ���1 �Ĵ��� 						0x0000_0000
#define	rWINCON2   		S3C6410_SFR32(0x77100028)		//   ��/д 		���ڿ���2 �Ĵ��� 						0x0000_0000
#define	rWINCON3   		S3C6410_SFR32(0x7710002C)		//   ��/д 		���ڿ���3 �Ĵ��� 						0x0000_0000
#define	rWINCON4   		S3C6410_SFR32(0x77100030)		//   ��/д 		���ڿ���4 �Ĵ��� 						0x0000_0000
#define	rVIDOSD0A   	S3C6410_SFR32(0x77100040)		//   ��/д 		��Ƶ����0 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD0B   	S3C6410_SFR32(0x77100044)		//   ��/д 		��Ƶ����0 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD0C   	S3C6410_SFR32(0x77100048)		//   ��/д 		��Ƶ����0 �ĳߴ���ƼĴ��� 				0x0000_0000
#define	rVIDOSD1A   	S3C6410_SFR32(0x77100050)		//   ��/д 		��Ƶ����1 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD1B   	S3C6410_SFR32(0x77100054)		//   ��/д 		��Ƶ����1 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD1C   	S3C6410_SFR32(0x77100058)		//   ��/д 		��Ƶ����1 ��alpha ���ƼĴ��� 			0x0000_0000
#define	rVIDOSD1D   	S3C6410_SFR32(0x7710005C)		//   ��/д 		��Ƶ����1 �ĳߴ���ƼĴ��� 				0x0000_0000
#define	rVIDOSD2A   	S3C6410_SFR32(0x77100060)		//   ��/д 		��Ƶ����2 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD2B   	S3C6410_SFR32(0x77100064)		//   ��/д 		��Ƶ����2 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD2C   	S3C6410_SFR32(0x77100068)		//   ��/д 		��Ƶ����2 ��alpha ���ƼĴ���			0x0000_0000
#define	rVIDOSD2D   	S3C6410_SFR32(0x7710006C)		//   ��/д 		��Ƶ����2 �ĳߴ���ƼĴ��� 				0x0000_0000
#define	rVIDOSD3A   	S3C6410_SFR32(0x77100070)		//   ��/д 		��Ƶ����3 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD3B   	S3C6410_SFR32(0x77100074)		//   ��/д 		��Ƶ����3 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD3C   	S3C6410_SFR32(0x77100078)		//   ��/д 		��Ƶ����3 ��alpha ���ƼĴ��� 			0x0000_0000
#define	rVIDOSD4A   	S3C6410_SFR32(0x77100080)		//   ��/д 		��Ƶ����4 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD4B   	S3C6410_SFR32(0x77100084)		//   ��/д 		��Ƶ����4 ��λ�ÿ��ƼĴ��� 				0x0000_0000
#define	rVIDOSD4C   	S3C6410_SFR32(0x77100088)		//   ��/д 		��Ƶ����4 ��alpha ���ƼĴ��� 			0x0000_0000
#define	rVIDW00ADD0B0  	S3C6410_SFR32(0x771000A0)		//   ��/д 		����0 �Ļ�������ʼ��ַ�Ĵ�����������0	0x0000_0000
#define	rVIDW00ADD0B1  	S3C6410_SFR32(0x771000A4)		//   ��/д 		����0 �Ļ�������ʼ��ַ�Ĵ�����������1	0x0000_0000
#define	rVIDW01ADD0B0  	S3C6410_SFR32(0x771000A8)		//   ��/д 		����1 �Ļ�������ʼ��ַ�Ĵ�����������0	0x0000_0000
#define	rVIDW01ADD0B1  	S3C6410_SFR32(0x771000AC)		//   ��/д 		����1 �Ļ�������ʼ��ַ�Ĵ�����������1	0x0000_0000
#define	rVIDW02ADD0   	S3C6410_SFR32(0x771000B0)		//   ��/д 		����2 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW03ADD0   	S3C6410_SFR32(0x771000B8)		//   ��/д 		����3 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW04ADD0   	S3C6410_SFR32(0x771000C0)		//   ��/д 		����4 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW00ADD1B0  	S3C6410_SFR32(0x771000D0)		//   ��/д 		����0 �Ļ�������ʼ��ַ�Ĵ�����������0	0x0000_0000
#define	rVIDW00ADD1B1  	S3C6410_SFR32(0x771000D4)		//   ��/д 		����0 �Ļ�������ʼ��ַ�Ĵ�����������1	0x0000_0000
#define	rVIDW01ADD1B0  	S3C6410_SFR32(0x771000D8)		//   ��/д 		����1 �Ļ�������ʼ��ַ�Ĵ���,������0	0x0000_0000
#define	rVIDW01ADD1B1  	S3C6410_SFR32(0x771000DC)		//   ��/д 		����1 �Ļ�������ʼ��ַ�Ĵ�����������1	0x0000_0000
#define	rVIDW02ADD1   	S3C6410_SFR32(0x771000E0)		//   ��/д 		����2 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW03ADD1   	S3C6410_SFR32(0x771000E8)		//   ��/д 		����3 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW04ADD1   	S3C6410_SFR32(0x771000F0)		//   ��/д 		����4 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW00ADD2   	S3C6410_SFR32(0x77100100)		//   ��/д 		����0 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW01ADD2   	S3C6410_SFR32(0x77100104)		//   ��/д 		����1 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW02ADD2   	S3C6410_SFR32(0x77100108)		//   ��/д 		����2 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW03ADD2   	S3C6410_SFR32(0x7710010C)		//   ��/д 		����3 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDW04ADD2   	S3C6410_SFR32(0x77100110)		//   ��/д 		����4 �Ļ�������ʼ��ַ�Ĵ��� 			0x0000_0000
#define	rVIDINTCON0   	S3C6410_SFR32(0x77100130)		//   ��/д 		ָ�������жϿ��ƼĴ��� 					0x03F0_0000
#define	rVIDINTCON1   	S3C6410_SFR32(0x77100134)		//   ��/д 		��Ƶ�ж����ҿ��ƼĴ��� 					0x0000_0000
#define	rW1KEYCON0   	S3C6410_SFR32(0x77100140)		//   ��/д 		Color key ���ƼĴ��� 					0x0000_0000
#define	rW1KEYCON1   	S3C6410_SFR32(0x77100144)		//   ��/д 		Color key ֵ�Ĵ��� 						0x0000_0000
#define	rW2KEYCON0   	S3C6410_SFR32(0x77100148)		//   ��/д 		Color key ���ƼĴ��� 					0x0000_0000
#define	rW2KEYCON1   	S3C6410_SFR32(0x7710014C)		//   ��/д 		Color key ֵ�Ĵ��� 						0x0000_0000
#define	rW3KEYCON0   	S3C6410_SFR32(0x77100150)		//   ��/д 		Color key ���ƼĴ��� 					0x0000_0000
#define	rW3KEYCON1   	S3C6410_SFR32(0x77100154)		//   ��/д 		Color key ֵ�Ĵ��� 						0x0000_0000
#define	rW4KEYCON0   	S3C6410_SFR32(0x77100158)		//   ��/д 		Color key ���ƼĴ��� 					0x0000_0000
#define	rW4KEYCON1   	S3C6410_SFR32(0x7710015C)		//   ��/д 		Color key ֵ�Ĵ��� 						0x0000_0000
#define	rDITHMODE   	S3C6410_SFR32(0x77100170)		//   ��/д 		����ģʽ�Ĵ��� 							0x0000_0000
#define	rWIN0MAP   		S3C6410_SFR32(0x77100180)		//   ��/д 		������ɫ���� 							0x0000_0000
#define	rWIN1MAP   		S3C6410_SFR32(0x77100184)		//   ��/д 		������ɫ���� 							0x0000_0000
#define	rWIN2MAP   		S3C6410_SFR32(0x77100188)		//   ��/д 		������ɫ���� 							0x0000_0000
#define	rWIN3MAP   		S3C6410_SFR32(0x7710018C)		//   ��/д 		������ɫ���� 							0x0000_0000
#define	rWIN4MAP   		S3C6410_SFR32(0x77100190)		//   ��/д 		������ɫ���� 							0x0000_0000
#define	rWPALCON   		S3C6410_SFR32(0x771001A0)		//   ��/д 		���ڵ�ɫ����ƼĴ��� 					0x0000_0000
#define	rTRIGCON   		S3C6410_SFR32(0x771001A4)		//   ��/д 		I80/RGB �������ƼĴ��� 					0x0000_0000
#define	rITUIFCON0   	S3C6410_SFR32(0x771001A8)		//   ��/д 		ITU(BT.601)�ӿڿ��� 					0x0000_0000
#define	rI80IFCONA0   	S3C6410_SFR32(0x771001B0)		//   ��/д 		��LDI ��I80 �ӿڿ���0 					0x0000_0000
#define	rI80IFCONA1   	S3C6410_SFR32(0x771001B4)		//   ��/д 		��LDI ��I80 �ӿڿ���0 					0x0000_0000
#define	rI80IFCONB0   	S3C6410_SFR32(0x771001B8)		//   ��/д 		��LDI ��I80 �ӿڿ���1 					0x0000_0000
#define	rI80IFCONB1   	S3C6410_SFR32(0x771001BC)		//   ��/д 		��LDI ��I80 �ӿڿ���1 					0x0000_0000
#define	rLDI_CMDCON0   	S3C6410_SFR32(0x771001D0)		//   ��/д 		I80 �ӿ�LDI �������0 					0x0000_0000
#define	rLDI_CMDCON1   	S3C6410_SFR32(0x771001D4)		//   ��/д 		I80 �ӿ�LDI �������1 					0x0000_0000
#define	rSIFCCON0   	S3C6410_SFR32(0x771001E0)		//   ��/д 		LCD I80 ϵͳ�ӿ��ֲ�������� 			0x0000_0000
#define	rSIFCCON1   	S3C6410_SFR32(0x771001E4)		//   ��/д 		LCD I80 ϵͳ�ӿ��ֲ���������д����		0x0000_0000
#define	rSIFCCON2   	S3C6410_SFR32(0x771001E8)		//   ��/д 		LCD I80 ϵͳ�ӿ��ֲ��������ݶ�����2		δ����
#define	rLDI_CMD0   	S3C6410_SFR32(0x77100280)		//   ��/д 		I80 �ӿ�LDI ����0 						0x0000_0000
#define	rLDI_CMD1   	S3C6410_SFR32(0x77100284)		//   ��/д 		I80 �ӿ�LDI ����1 						0x0000_0000
#define	rLDI_CMD2   	S3C6410_SFR32(0x77100288)		//   ��/д 		I80 �ӿ�LDI ����2 						0x0000_0000
#define	rLDI_CMD3   	S3C6410_SFR32(0x7710028C)		//   ��/д 		I80 �ӿ�LDI ����3 						0x0000_0000
#define	rLDI_CMD4   	S3C6410_SFR32(0x77100290)		//   ��/д 		I80 �ӿ�LDI ����4 						0x0000_0000
#define	rLDI_CMD5   	S3C6410_SFR32(0x77100294)		//   ��/д 		I80 �ӿ�LDI ����5 						0x0000_0000
#define	rLDI_CMD6   	S3C6410_SFR32(0x77100298)		//   ��/д 		I80 �ӿ�LDI ����6 						0x0000_0000
#define	rLDI_CMD7   	S3C6410_SFR32(0x7710029C)		//   ��/д 		I80 �ӿ�LDI ����7 						0x0000_0000
#define	rLDI_CMD8   	S3C6410_SFR32(0x771002A0)		//   ��/д 		I80 �ӿ�LDI ����8 						0x0000_0000
#define	rLDI_CMD9   	S3C6410_SFR32(0x771002A4)		//   ��/д 		I80 �ӿ�LDI ����9 						0x0000_0000
#define	rLDI_CMD10   	S3C6410_SFR32(0x771002A8)		//   ��/д 		I80 �ӿ�LDI ����10 						0x0000_0000
#define	rLDI_CMD11   	S3C6410_SFR32(0x771002AC)		//   ��/д 		I80 �ӿ�LDI ����11 						0x0000_0000
#define	rW2PDATA01   	S3C6410_SFR32(0x77100300)		//   ��/д 		����0��1 �Ĵ���2 ��ɫ������ 			0x0000_0000
#define	rW2PDATA23   	S3C6410_SFR32(0x77100304)		//   ��/д 		����2��3 �Ĵ���2 ��ɫ������ 			0x0000_0000
#define	rW2PDATA45   	S3C6410_SFR32(0x77100308)		//   ��/д 		����4��5 �Ĵ���2 ��ɫ������ 			0x0000_0000
#define	rW2PDATA67   	S3C6410_SFR32(0x7710030C)		//   ��/д 		����6��7 �Ĵ���2 ��ɫ������ 			0x0000_0000
#define	rW2PDATA89   	S3C6410_SFR32(0x77100310)		//   ��/д 		����8��9 �Ĵ���2 ��ɫ������ 			0x0000_0000
#define	rW2PDATAAB   	S3C6410_SFR32(0x77100314)		//   ��/д 		����A��B �Ĵ���2 ��ɫ������ 			0x0000_0000
#define	rW2PDATACD   	S3C6410_SFR32(0x77100318)		//   ��/д 		����C��D �Ĵ���2 ��ɫ������ 			0x0000_0000
#define	rW2PDATAEF   	S3C6410_SFR32(0x7710031C)		//   ��/д 		����E��F �Ĵ���2 ��ɫ������ 			0x0000_0000
#define	rW3PDATA01   	S3C6410_SFR32(0x77100320)		//   ��/д 		����0��1 �Ĵ���3 ��ɫ������ 			0x0000_0000
#define	rW3PDATA23   	S3C6410_SFR32(0x77100324)		//   ��/д 		����2��3 �Ĵ���3 ��ɫ������ 			0x0000_0000
#define	rW3PDATA45   	S3C6410_SFR32(0x77100328)		//   ��/д 		����4��5 �Ĵ���3 ��ɫ������ 			0x0000_0000
#define	rW3PDATA67   	S3C6410_SFR32(0x7710032C)		//   ��/д 		����6��7 �Ĵ���3 ��ɫ������ 			0x0000_0000
#define	rW3PDATA89   	S3C6410_SFR32(0x77100330)		//   ��/д 		����8��9 �Ĵ���3 ��ɫ������ 			0x0000_0000
#define	rW3PDATAAB   	S3C6410_SFR32(0x77100334)		//   ��/д 		����A��B �Ĵ���3 ��ɫ������ 			0x0000_0000
#define	rW3PDATACD   	S3C6410_SFR32(0x77100338)		//   ��/д 		����C��D �Ĵ���3 ��ɫ������ 			0x0000_0000
#define	rW3PDATAEF   	S3C6410_SFR32(0x7710033C)		//   ��/д 		����E��F �Ĵ���3 ��ɫ������ 			0x0000_0000
#define	rW4PDATA01   	S3C6410_SFR32(0x77100340)		//   ��/д 		����0��1 �Ĵ���4 ��ɫ������ 			0x0000_0000
#define	rW4PDATA23   	S3C6410_SFR32(0x77100344)		//   ��/д 		����2��3 �Ĵ���4 ��ɫ������ 			0x0000_0000 



//SDI����Ĵ���
//ϵͳ��ַ�Ĵ���,�üĴ�����������DMA ���������洢��ַ��
//		�Ĵ��� 			��ַ 							   ��/д 		���� 						��λֵ
#define	rSYSAD0 		S3C6410_SFR32(0x7C200000)			// ��/д 	ϵͳ��ַ�Ĵ���(0ͨ��) 		0x0
#define	rSYSAD1 		S3C6410_SFR32(0x7C300000)			// ��/д 	ϵͳ��ַ�Ĵ���(1ͨ��) 		0x0
#define	rSYSAD2 		S3C6410_SFR32(0x7C400000)			// ��/д 	ϵͳ��ַ�Ĵ���(2ͨ��) 		0x0
//���С�Ĵ���,�üĴ�����������һ�����ݿ���ֽ�����
#define	rBLKSIZE0  		S3C6410_SFR16(0x7C200004)			// ��/д 	����DMA�������߽�ʹ�����С�Ĵ���(0ͨ��) 0x0
#define	rBLKSIZE1  		S3C6410_SFR16(0x7C300004)			// ��/д 	����DMA�������߽�ʹ�����С�Ĵ���(1ͨ��) 0x0
#define	rBLKSIZE2  		S3C6410_SFR16(0x7C400004)			// ��/д 	����DMA�������߽�ʹ�����С�Ĵ���(2ͨ��) 0x0
//������Ĵ���,�üĴ��������������ݿ��������
#define	rBLKCNT0   		S3C6410_SFR16(0x7C200006)			// ��/д 	Ϊ��ǰ����Ŀ����( 0ͨ��) 0x0
#define	rBLKCNT1   		S3C6410_SFR16(0x7C300006)			// ��/д 	Ϊ��ǰ����Ŀ����( 1ͨ��) 0x0
#define	rBLKCNT2   		S3C6410_SFR16(0x7C400006)			// ��/д 	Ϊ��ǰ����Ŀ����( 2ͨ��) 0x0
//��Ԫ�Ĵ���,�üĴ�������SDָ���Ԫ��
#define	rARGUMENT0  	S3C6410_SFR32(0x7C200008)			// ��/д 	ָ���Ԫ�Ĵ���(0ͨ��) 0x0
#define	rARGUMENT1  	S3C6410_SFR32(0x7C300008)			// ��/д 	ָ���Ԫ�Ĵ���(1ͨ��) 0x0
#define	rARGUMENT2  	S3C6410_SFR32(0x7C400008)			// ��/д 	ָ���Ԫ�Ĵ���(2ͨ��) 0x0
/*����ģʽ�Ĵ���
�üĴ������ڿ������ݴ���Ĳ������ڷ������ڴ������ݵ�ָ��ǰ���������ڷ���һ���ָ�ָ��ǰ��
������������������������Ĵ����������ݴ�����ͣ��ʹ�ûָ�ָ��ָ���֮ǰ���������������������
�Ĵ�����ֵ��Ϊ�˷�ֹ���ݶ�ʧ�������ݴ����ڼ䣬������������Ϊ�Ĵ���ִ��д��������ָ���ֹ(DAT)
�ڵ�ǰ״̬�Ĵ���������Ϊ1ʱ��д������Ĵ�����������*/
#define	rTRNMOD0    	S3C6410_SFR16(0x7C20000C)			// ��/д 	����ģʽ���üĴ���(0ͨ��) 0x0
#define	rTRNMOD1    	S3C6410_SFR16(0x7C30000C)			// ��/д 	����ģʽ���üĴ���(1ͨ��) 0x0
#define	rTRNMOD2    	S3C6410_SFR16(0x7C40000C)			// ��/д 	����ģʽ���üĴ���(2ͨ��) 0x0
/*����Ĵ���
����Ĵ�������SDָ���Ԫ����д������Ĵ���ǰ���������������ڵ�ǰ״̬�Ĵ������ָ���ֹ
(DAT)λ��ָ���ֹ(CMD)λ��д������Ĵ����ĸ�λ�ֽ�����SDָ�����������������������д�������
��������Ϊ��ָ���ֹ(CMD)�����ã��������������ṩд������*/
#define	rCMDREG0     	S3C6410_SFR16(0x7C20000E)			// ��/д 	����Ĵ���(ͨ��0)�� 0x0
#define	rCMDREG1    	S3C6410_SFR16(0x7C30000E)			// ��/д 	����Ĵ���(ͨ��1)�� 0x0
#define	rCMDREG2    	S3C6410_SFR16(0x7C40000E)			// ��/д 	����Ĵ���(ͨ��2)�� 0x0
//Ӧ��Ĵ���,�üĴ��������洢����SD����Ӧ��
#define	rRSPREG0_0    	S3C6410_SFR32(0x7C200010)			// ROC 		Ӧ��Ĵ���0(0ͨ��)�� 0x0
#define	rRSPREG1_0    	S3C6410_SFR32(0x7C200014)			// ROC 		Ӧ��Ĵ���1(0ͨ��)�� 0x0
#define	rRSPREG2_0    	S3C6410_SFR32(0x7C200018)			// ROC 		Ӧ��Ĵ���2(0ͨ��)�� 0x0
#define	rRSPREG3_0    	S3C6410_SFR32(0x7C20001C)			// ROC 		Ӧ��Ĵ���3(0ͨ��)�� 0x0
#define	rRSPREG0_1    	S3C6410_SFR32(0x7C300010)			// ROC	 	Ӧ��Ĵ���0(1ͨ��)�� 0x0
#define	rRSPREG1_1    	S3C6410_SFR32(0x7C300014)			// ROC 		Ӧ��Ĵ���1(1ͨ��)�� 0x0
#define	rRSPREG2_1    	S3C6410_SFR32(0x7C300018)			// ROC 		Ӧ��Ĵ���2(1ͨ��)�� 0x0
#define	rRSPREG3_1    	S3C6410_SFR32(0x7C30001C)			// ROC 		Ӧ��Ĵ���3(1ͨ��)�� 0x0
#define	rRSPREG0_2    	S3C6410_SFR32(0x7C400010)			// ROC 		Ӧ��Ĵ���0(2ͨ��)�� 0x0
#define	rRSPREG1_2    	S3C6410_SFR32(0x7C400014)			// ROC 		Ӧ��Ĵ���1(2ͨ��)�� 0x0
#define	rRSPREG2_2    	S3C6410_SFR32(0x7C400018)			// ROC 		Ӧ��Ĵ���2(2ͨ��)�� 0x0
#define	rRSPREG3_2    	S3C6410_SFR32(0x7C40001C)			// ROC		Ӧ��Ĵ���3(2ͨ��)�� 0x0
//���������ݶ˿ڼĴ���,32λ���ݶ˿ڼĴ����������ڲ���������
#define	rBDATA0   		S3C6410_SFR32(0x7C200020)			//  ��/д 	���������� �Ĵ���(0ͨ��)�� 0x0
#define	rBDATA1   		S3C6410_SFR32(0x7C300020)			//  ��/д 	���������� �Ĵ���(1ͨ��)�� 0x0
#define	rBDATA2   		S3C6410_SFR32(0x7C400020)			//  ��/д 	���������� �Ĵ���(2ͨ��)�� 0x0
//��ǰ״̬�Ĵ���,�üĴ�������SDָ���Ԫ��
#define	rPRNSTS0   		S3C6410_SFR32(0x7C200024)			//  RO/ROC 	��ǰ״̬�Ĵ���(ͨ��0)�� 0x000A0000
#define	rPRNSTS1   		S3C6410_SFR32(0x7C300024)			//  RO/ROC 	��ǰ״̬�Ĵ���(ͨ��1)�� 0x000A0000
#define	rPRNSTS2   		S3C6410_SFR32(0x7C400024)			//  RO/ROC 	��ǰ״̬�Ĵ���(ͨ��2)�� 0x000A0000
//�������ƼĴ���,�üĴ�������SDָ���Ԫ��
#define	rHOSTCTL0   	S3C6410_SFR8(0x7C200028)			//  ��/д 	�������ƼĴ���(0ͨ��)�� 0x0
#define	rHOSTCTL1   	S3C6410_SFR8(0x7C300028)			//  ��/д 	�������ƼĴ���(1ͨ��)�� 0x0
#define	rHOSTCTL2   	S3C6410_SFR8(0x7C400028)			//  ��/д 	�������ƼĴ���(2ͨ��)�� 0x0
//��Դ���ƼĴ���,�üĴ�������SDָ���Ԫ��
#define	rPWRCON0   		S3C6410_SFR8(0x7C200029)			//  ��/д 	��Դ���ƼĴ���(0ͨ��)�� 0x0
#define	rPWRCON1   		S3C6410_SFR8(0x7C300029)			//  ��/д 	��Դ���ƼĴ���(1ͨ��)�� 0x0
#define	rPWRCON2   		S3C6410_SFR8(0x7C400029)			//  ��/д 	��Դ���ƼĴ���(2ͨ��)�� 0x0
//�������ƼĴ���,�üĴ�������SDָ���Ԫ��
#define	rBLKGAP0   		S3C6410_SFR8(0x7C20002A)			//  ��/д 	�������ƼĴ���(0 ͨ��)�� 0x0
#define	rBLKGAP1   		S3C6410_SFR8(0x7C30002A)			//  ��/д 	�������ƼĴ���(1 ͨ��)�� 0x0
#define	rBLKGAP2   		S3C6410_SFR8(0x7C40002A)			//  ��/д 	�������ƼĴ���(2 ͨ��)�� 0x0
/*���ѿ��ƼĴ���
����Ĵ�����ǿ���Եģ����ǻ��ѹ�����������������ϵͳӲ�������������������ά�������ϵĵ�
ѹ��ͨ���ڵ�Դ������������SD ���ߵ�ԴΪ1������ʱ���ɿ��жϵõ���*/
#define	rWAKCON0   		S3C6410_SFR8(0x7C20002B)			//  ��/д 	���ѿ��ƼĴ���(0 ͨ��)�� 0x0
#define	rWAKCON1   		S3C6410_SFR8(0x7C30002B)			//  ��/д 	���ѿ��ƼĴ���(1 ͨ��)�� 0x0
#define	rWAKCON2   		S3C6410_SFR8(0x7C40002B)			//  ��/д 	���ѿ��ƼĴ���(2 ͨ��)�� 0x0
//ʱ�ӿ��ƼĴ���,��������������ʼ��ʱ�������������������޼Ĵ�������SDCLK Ƶ��ѡ��
#define	rCLKCON0   		S3C6410_SFR16(0x7C20002C)			//  ��/д 	ʱ�ӿ��ƼĴ���(0 ͨ��)�� 0x0
#define	rCLKCON1   		S3C6410_SFR16(0x7C30002C)			//  ��/д 	ʱ�ӿ��ƼĴ���(1 ͨ��)�� 0x0
#define	rCLKCON2   		S3C6410_SFR16(0x7C40002C)			//  ��/д 	ʱ�ӿ��ƼĴ���(2 ͨ��)�� 0x0
//��ʱ���ƼĴ���,������������ʼ��ʱ�������������ܸ������޼Ĵ����������ݳ�ʱ������ֵ��
#define	rTIMEOUTCON0   	S3C6410_SFR8(0x7C20002E)			//  ��/д 	��ʱ���ƼĴ���(0 ͨ��)�� 0x0
#define	rTIMEOUTCON1   	S3C6410_SFR8(0x7C30002E)			//  ��/д 	��ʱ���ƼĴ���(1 ͨ��)�� 0x0
#define	rTIMEOUTCON2   	S3C6410_SFR8(0x7C40002E)			//  ��/д 	��ʱ���ƼĴ���(2 ͨ��)�� 0x0
/*�����λ�Ĵ���
д1 ������Ĵ�����ÿһλ���������λ���塣��λ��ɺ��������������ÿһλ������������
��λҪ��һЩʱ�䣬SD ������������ȷ����Щλ��0��*/
#define	rSWRST0   		S3C6410_SFR8(0x7C20002F)			//  ��/д 	�����λ�Ĵ���(0 ͨ��)�� 0x0
#define	rSWRST1   		S3C6410_SFR8(0x7C30002F)			//  ��/д 	�����λ�Ĵ���(1 ͨ��)�� 0x0
#define	rSWRST2   		S3C6410_SFR8(0x7C40002F)			//  ��/д 	�����λ�Ĵ���(2 ͨ��)�� 0x0
/*�����ж�״̬�Ĵ���
�����ж�״̬����Ӱ������Ĵ����Ķ������������ж��ź�������û�����Ӱ�졣�������ж��ź���
�������������һ��״̬λ����Ϊ1��һ���жϲ��������˿��жϺʹ����ж�λ�⣬�������е�λд1
��һ��λ���������д0 ����Щλ���ֲ��䡣ͨ��д��һ���źżĴ���������һ��״̬�ܱ����������ͣ
ֹ�ж���Чʱ�����жϱ������Ҳ���ǣ���������Ϊ�ж��ṩ�����������*/
#define	rNORINTSTS0   	S3C6410_SFR16(0x7C200030)			//  ROC/RW1C �����ж�״̬�Ĵ���(0 ͨ��)�� 0x0
#define	rNORINTSTS1   	S3C6410_SFR16(0x7C300030)			//  ROC/RW1C �����ж�״̬�Ĵ���(1 ͨ��)�� 0x0
#define	rNORINTSTS2   	S3C6410_SFR16(0x7C400030)			//  ROC/RW1C �����ж�״̬�Ĵ���(2 ͨ��)�� 0x0
/*�����ж�״̬�Ĵ���
ͨ�������ж�״̬�����Ĵ�������������Ĵ����е��źŶ��壬���ǲ���ͨ�������ж��ź������Ĵ�
�����������ж��ź����������������һ��״̬����Ϊ1���жϲ�����д1 �����λ��д0 ������λ��
�䡣��һ���Ĵ���д������һ��״̬�ܱ������*/
#define	rERRINTSTS0   	S3C6410_SFR16(0x7C200032)			//  ROC/RW1C �����ж�״̬�Ĵ���(0 ͨ��)�� 0x0
#define	rERRINTSTS1   	S3C6410_SFR16(0x7C300032)			//  ROC/RW1C �����ж�״̬�Ĵ���(1 ͨ��)�� 0x0
#define	rERRINTSTS2   	S3C6410_SFR16(0x7C400032)			//  ROC/RW1C �����ж�״̬�Ĵ���(2 ͨ��)�� 0x0
//�����ж�״̬�����Ĵ���,����1 ��ʹ�ж�״̬������
#define	rNORINTSTSEN0   S3C6410_SFR16(0x7C200034)			//  ��/д 	�����ж�״̬�����Ĵ���(0 ͨ��)�� 0x0
#define	rNORINTSTSEN1   S3C6410_SFR16(0x7C300034)			//  ��/д 	�����ж�״̬�����Ĵ���(1 ͨ��)�� 0x0
#define	rNORINTSTSEN2   S3C6410_SFR16(0x7C400034)			//  ��/д 	�����ж�״̬�����Ĵ���(2 ͨ��)�� 0x0
//�����ж�״̬�����Ĵ���,����1 ����������ж�״̬��
#define	rERRINTSTSEN0   S3C6410_SFR16(0x7C200036)			//  ��/д 	�����ж�״̬�����Ĵ���(0 ͨ��)�� 0x0
#define	rERRINTSTSEN1   S3C6410_SFR16(0x7C300036)			//  ��/д 	�����ж�״̬�����Ĵ���(1 ͨ��)�� 0x0
#define	rERRINTSTSEN2   S3C6410_SFR16(0x7C400036)			//  ��/д 	�����ж�״̬�����Ĵ���(2 ͨ��)�� 0x0
//�����ж��ź������Ĵ���,�üĴ�������ѡ������ϵͳ��һ���ж�״̬��Ϊ�жϡ���Щ�ж�״̬��������ͬ��1 λ�ж��ߡ�
#define	rNORINTSIGEN0   S3C6410_SFR16(0x7C200038)			//  ��/д 	�����ж��ź������Ĵ���(0 ͨ��)�� 0x0
#define	rNORINTSIGEN1   S3C6410_SFR16(0x7C300038)			//  ��/д 	�����ж��ź������Ĵ���(1 ͨ��)�� 0x0
#define	rNORINTSIGEN2   S3C6410_SFR16(0x7C400038)			//  ��/д 	�����ж��ź������Ĵ���(2 ͨ��)�� 0x0
//�����ж��ź������Ĵ���,�üĴ�������ѡ������ϵͳ��һ���ж�״̬��Ϊ�жϡ����е�״̬λ������ͬ��1 λ�ж��ߡ�
#define	rERRINTSIGEN0   S3C6410_SFR16(0x7C20003A)			//  ��/д 	�����ж��ź������Ĵ���(0 ͨ��)�� 0x0
#define	rERRINTSIGEN1   S3C6410_SFR16(0x7C30003A)			//  ��/д 	�����ж��ź������Ĵ���(1 ͨ��)�� 0x0
#define	rERRINTSIGEN2   S3C6410_SFR16(0x7C40003A)			//  ��/д 	�����ж��ź������Ĵ���(2 ͨ��)�� 0x0
//�Զ�CMD12 ����״̬�Ĵ���
//���Զ�CMD12 ����״̬�����ã������������������Ĵ����������Զ�CMD12 ��ʾ����ʲô���ʹ���
//����Ĵ���ֻ�е��Զ�CMD12 ��������ʱ��Ч��
#define	rACMD12ERRSTS0  S3C6410_SFR16(0x7C20003C)			//  ROC 	�Զ�CMD12 ����״̬�Ĵ���(0 ͨ��)�� 0x0
#define	rACMD12ERRSTS1  S3C6410_SFR16(0x7C30003C)			//  ROC 	�Զ�CMD12 ����״̬�Ĵ���(1 ͨ��)�� 0x0
#define	rACMD12ERRSTS2  S3C6410_SFR16(0x7C40003C)			//  ROC 	�Զ�CMD12 ����״̬�Ĵ���(2 ͨ��)�� 0x0
//���޼Ĵ���
//��������������ִ�У��üĴ���Ϊ�����������ṩ����ϸ��Ϣ���ϵ��ʼ���ڼ䣬����������ִ����Щֵ��Ϊ�̶��Ļ��ߴ��������롣
#define	rCAPAREG0   	S3C6410_SFR32(0x7C200040)			//  ��/д 	���޼Ĵ���(0 ͨ��)�� 0x05E00080
#define	rCAPAREG1   	S3C6410_SFR32(0x7C300040)			//  ��/д 	���޼Ĵ���(1 ͨ��)�� 0x05E00080
#define	rCAPAREG2   	S3C6410_SFR32(0x7C400040)			//  ��/д 	���޼Ĵ���(2 ͨ��)�� 0x05E00080
/*���������޼Ĵ���
����Ĵ���ָʾ���ĵ������ޡ������ѹ֧�������޼Ĵ����б����ã���ô�������������ֵ��
��������ġ��������ϵͳͨ����һ�ַ����ṩ�����Ϣ�����е����������޼Ĵ�������0��*/
#define	rMAXCURR0   	S3C6410_SFR32(0x7C200048)			//  HWInit 	���������޼Ĵ���(0 ͨ��)�� 0x0
#define	rMAXCURR1   	S3C6410_SFR32(0x7C300048)			//  HWInit 	���������޼Ĵ���(1 ͨ��)�� 0x0
#define	rMAXCURR2   	S3C6410_SFR32(0x7C400048)			//  HWInit 	���������޼Ĵ���(2 ͨ��)�� 0x0
//���ƼĴ���2,�üĴ�������SD ָ���Ԫ��
#define	rCONTROL2_0   	S3C6410_SFR32(0x7C200080)			//  ��/д	���ƼĴ���2 (0 ͨ��)�� 0x0
#define	rCONTROL2_1   	S3C6410_SFR32(0x7C300080)			//  ��/д 	���ƼĴ���2 (1 ͨ��)�� 0x0
#define	rCONTROL2_2   	S3C6410_SFR32(0x7C400080)			//  ��/д 	���ƼĴ���2 (2 ͨ��)�� 0x0
//���ƼĴ���3�Ĵ���
#define	rCONTROL3_0   	S3C6410_SFR32(0x7C200084)			//  ��/д 	FIFO �жϿ���(���ƼĴ���3) (0 ͨ��)�� 0x7F5F3F1F
#define	rCONTROL3_1   	S3C6410_SFR32(0x7C300084)			//  ��/д 	FIFO �жϿ���(���ƼĴ���3) (1 ͨ��)�� 0x7F5F3F1F
#define	rCONTROL3_2   	S3C6410_SFR32(0x7C400084)			//  ��/д 	FIFO �жϿ���(���ƼĴ���3) (2 ͨ��)�� 0x7F5F3F1F
//�����������汾�Ĵ���,�üĴ�������SD ָ���Ԫ��
#define	rHCVER0   		S3C6410_SFR16(0x7C2000FE)			//  HWInit 	�������ư汾�Ĵ�����0 ͨ������ 0x1300
#define	rHCVER1   		S3C6410_SFR16(0x7C3000FE)			//  HWInit 	�������ư汾�Ĵ�����1 ͨ������ 0x1300
#define	rHCVER2   		S3C6410_SFR16(0x7C4000FE)			//  HWInit 	�������ư汾�Ĵ�����2 ͨ������ 0x1300


//JPEG
#define JPEG_BASE		0x78800000
//��ؼĴ�������
#define	JPGMOD       	S3C6410_SFR32(JPEG_BASE+0x000)		//����ģʽ�Ĵ���
#define	JPGSTS       	S3C6410_SFR32(JPEG_BASE+0x004)		//����״̬�Ĵ���
#define	JPGQHNO      	S3C6410_SFR32(JPEG_BASE+0x008)		//��������Ŀ�Ĵ����͹���������Ŀ�Ĵ���
#define	JPGDRI       	S3C6410_SFR32(JPEG_BASE+0x00C)		//΢������Ƕ��RST���
#define	JPGY         	S3C6410_SFR32(JPEG_BASE+0x010)		//��ֱ�ֱ���
#define	JPGX        	S3C6410_SFR32(JPEG_BASE+0x014)		//ˮƽ�ֱ���
#define	JPGCNT      	S3C6410_SFR32(JPEG_BASE+0x018)		//ѹ�����ݵ��ֽ���
#define	JPGIRQEN   		S3C6410_SFR32(JPEG_BASE+0x01C)		//�ж����üĴ���
#define	JPGIRQ        	S3C6410_SFR32(JPEG_BASE+0x020)		//�ж�״̬�Ĵ��� 
#define	JQTBL0        	S3C6410_SFR32_P(JPEG_BASE+0x400)		//8λ������0	64
#define	JQTBL1        	S3C6410_SFR32_P(JPEG_BASE+0x500)		//8λ������1	64
#define	JQTBL2        	S3C6410_SFR32_P(JPEG_BASE+0x600)		//8λ������2	64
#define	JQTBL3        	S3C6410_SFR32_P(JPEG_BASE+0x700)		//8λ������3	64
#define	JHDCTBL0      	S3C6410_SFR32_P(JPEG_BASE+0x800)		//ÿ�����볤�ȵ�����	16
#define	JHDCTBLG0    	S3C6410_SFR32_P(JPEG_BASE+0x840)		//���ڷ����ʵ�Ⱥ����	12
#define	JHACTBL0      	S3C6410_SFR32_P(JPEG_BASE+0x880)		//ÿ�����볤�ȵ�����	16
#define	JHACTBLG0    	S3C6410_SFR32_P(JPEG_BASE+0x8c0)		//���ڷ�����/��������Ⱥ����	162
#define	JHDCTBL1      	S3C6410_SFR32_P(JPEG_BASE+0xc00)		//ÿ�����볤�ȵ�����	16
#define	JHDCTBLG1    	S3C6410_SFR32_P(JPEG_BASE+0xc40)		//���ڷ����ʵ�Ⱥ����	12
#define	JHACTBL1      	S3C6410_SFR32_P(JPEG_BASE+0xc80)		//ÿ�����볤�ȵ�����	16
#define	JHACTBLG1    	S3C6410_SFR32_P(JPEG_BASE+0xcc0)		//���ڷ�����/��������Ⱥ����	162
#define	JIMGADDR0     	S3C6410_SFR32(JPEG_BASE+0x1000)		//Դ��Ŀ��ͼ���ַ1
#define	JIMGADDR1     	S3C6410_SFR32(JPEG_BASE+0x1004)		//Դ��Ŀ��ͼ���ַ2
#define	JHUFADDR0     	S3C6410_SFR32(JPEG_BASE+0x1008)		//Դ��Ŀ��JPEG �ļ���ַ1
#define	JHUFADDR1     	S3C6410_SFR32(JPEG_BASE+0x100c)		//Դ��Ŀ��JPEG �ļ���ַ2
#define	JSTART        	S3C6410_SFR32(JPEG_BASE+0x1010)		//��ʼJPEG����
#define	JRSTART      	S3C6410_SFR32(JPEG_BASE+0x1014)		//���¿�ʼJPEG����
#define	RESET_CON 		S3C6410_SFR32(JPEG_BASE+0x1018)		//�����λ
#define	JPG_CON     	S3C6410_SFR32(JPEG_BASE+0x101c)		//JPEG���ƼĴ���
#define	JCOEF1       	S3C6410_SFR32(JPEG_BASE+0x1020)		//ϵ��ֵRGB ? YcbC ��ת����
#define	JCOEF2       	S3C6410_SFR32(JPEG_BASE+0x1024)		//ϵ��ֵRGB ? YcbC ��ת����
#define	JCOEF3       	S3C6410_SFR32(JPEG_BASE+0x1028)		//ϵ��ֵRGB ? YcbC ��ת����
#define	JMISC       	S3C6410_SFR32(JPEG_BASE+0x102c)		//����
#define	JFRAME_INTV   	S3C6410_SFR32(JPEG_BASE+0x1030)		//֡��ļ��ʱ��ֵ,����ʱ������
////������Щ�Ĵ������ֲ������Ҳ���,���ǹٷ����Գ����г�����
#define	RESUME        	S3C6410_SFR32(JPEG_BASE+0x1038)		//
#define	IMGADDR_END   	S3C6410_SFR32(JPEG_BASE+0x103c)		//	
#define	HUFADDR_END   	S3C6410_SFR32(JPEG_BASE+0x1040)		//	
#define	HUFADDR_MNT   	S3C6410_SFR32(JPEG_BASE+0x1044)		//

 
#endif
