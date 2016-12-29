/*************************************************************************************************************
 * �ļ���:	sdio.c
 * ����:		S3C6410 SDIO�ײ���������
 * ����:		cp1300@139.com
 * ����ʱ��:	2012��5��16��
 * ����޸�ʱ��:2012��9��17��
 * ��ϸ:		sdio��ز�������
*************************************************************************************************************/
#include "system.h"
#include "sdio.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
//���С�Ĵ���(0ͨ��)BLKSIZE0
#define BLKSIZE0_BLKSIZE			0x0200		//���СΪ512�ֽ�			
//����ģʽ�Ĵ���(0ͨ��)TRNMOD0
#define TRNMOD0_CCSCON				0			//����ģʽ
#define TRNMOD0_MUL1SIN0			(0 << 5)	//Ĭ��Ϊ�����β���ģʽ
#define TRNMOD0_RD1WT0				(1 << 4)	//Ĭ��Ϊ������ģʽ
#define TRNMOD0_ENACMD12			(0 << 2)	//�Զ�CMD12��ֹ,�����β�����ɺ������Զ�����ֹͣ����
#define TRNMOD0_ENBLKCNT			(0 << 1)	//���������Ч
#define TRNMOD0_ENDMA				(0 << 0)	//DMA��Ч
//�������ƼĴ���(0ͨ��)HOSTCTL0
#define HOSTCTL0_WIDE8				(0 << 5)	//���Ϊ��ȱ�λ1ָ�� (���ݴ�����)
#define HOSTCTL0_DMASEL				(0 << 3)	//SDMA
#define HOSTCTL0_OUTEDGEINV			(0 << 2)	//ʱ����������������Ч		//����Ĵ���������,��֪�������Ǹ�ɶ�õ�
#define HOSTCTL0_WIDE4				(0 << 4)	//���ݴ����ȡ�1BITģʽ
//��Դ���ƼĴ���(0ͨ��)PWRCON0
#define PWRCON0_SELPWRLVL			(7 << 1)	//3.3V��Դģʽ
#define PWRCON0_PWRON				(1 << 0)	//��Դ��
//���޼Ĵ���(0 ͨ��)CAPAREG0
#define CAPAREG0_V18				(1 << 26)	//��ѹ֧��1.8V
#define CAPAREG0_V30				(0 << 25)	//��ѹ��֧��3v
#define CAPAREG0_V33				(1 << 24)	//��ѹ֧��3.3V
#define CAPAREG0_SUSRES				(1 << 23)	//֧����ͣ/�ָ�����
#define CAPAREG0_DMA				(1 << 22)	//֧��DMA
#define CAPAREG0_HSPD				(1 << 21)	//֧�ָ���ģʽ
#define CAPAREG0_ADMA2				(0 << 19)	//��֧��DMA2
#define CAPAREG0_MAXBLKLEN			(0 << 16)	//�����СΪ512B
#define CAPAREG0_BASECLK			(25 << 8)	//SD����ʼ��25MHz
#define CAPAREG0_TOUTUNIT			(0 << 7)	//��ʱʱ�ӵ�λKHZ
#define CAPAREG0_TOUTCLK			(10 << 0)	//��ʱʱ��Ƶ��Ϊ10KHZ
//���������޼Ĵ���(0 ͨ��)MAXCURR0
#define MAXCURR0_MAXCURR18			(10 << 16)	//����1.8V,������Ϊ40MA
#define MAXCURR0_MAXCURR30			(10 << 8)	//����3.0V,������Ϊ40MA
#define MAXCURR0_MAXCURR33			(10 << 0)	//����3.3V,������Ϊ40MA
//���ƼĴ���2 CONTROL2_0
#define CONTROL2_0_ENSTAASYNCCLR	(0 << 31)	//��λ����ʹ�����ʹ����жϵ��첽��ȷ����״̬λ
#define CONTROL2_0_ENCMDCNFMSK		(0 << 30)	//������ָ���ͻ
#define CONTROL2_0_CDINVRXD3		(0 << 29)	//������źŵ��ö���RX_DAT[3]����ֹ
#define CONTROL2_0_SELCARDOUT		(0 << 28)	//���Ƴ������ǡ��޿����롱 ״̬��
#define CONTROL2_0_FLTCLKSEL		(8 << 24)	//�˲���ʱ�� (iFLTCLK) ѡ��
#define CONTROL2_0_ENFBCLKTX		(0 << 15)	//����ʱ�ӽ�ֹ,���ڷ�������ʱ��
#define CONTROL2_0_ENFBCLKRX		(0 << 14)	//����ʱ�ӽ�ֹ,���ڽ�������ʱ��
#define CONTROL2_0_SDCDSEL			(0 << 13)	//nSDCD ����SD ������ź�
#define CONTROL2_0_SDSIGPC			(0 << 12)	//ͬ�����������Ч�ź�
#define CONTROL2_0_ENBUSYCHKTXSTART (0 << 11)	//������������״̬ǰæµ״̬��⡣
#define CONTROL2_0_DFCNT			(0 << 9)	//�����˲�������16 iSDCLK
#define CONTROL2_0_ENCLKOUTHOLD		(1 << 8)	//SDCLK ������Ч��
#define CONTROL2_0_RWAITMODE		(0 << 7)	//�����������ͷŶ��ȴ�״̬���Զ���
#define CONTROL2_0_DISBUFRD			(0 << 6)	//����ģʽ����0x20 �Ĵ���ʹ���߿��Զ���������FIFO������
//HCLK = 128MHZ		EPLL = 24MHZ
#define CONTROL2_0_SELBASECLK		(2 << 4)	//����ʱ��Դѡ��00 ��01 = HCLK��10 = EPLL ���ʱ�ӣ�����ϵͳ��11 = �ⲿʱ��Դ��XTI ��XEXTCLk��
#define CONTROL2_0_PWRSYNC			(0 << 3)	//��ͬ��������������Ч�źţ�ָ����ݣ�
#define CONTROL2_0_ENCLKOUTMSKCON	(0 << 1)	//�������뱻���ʱ��SDCLK ���ʱ�����Ρ��������޿�״̬ʱ�����ø�����Ϊ��λ��ֹͣSDCLK��
#define CONTROL2_0_HWINITFIN		(1 << 0)	//SD ����������Ӳ����ʼ����ɡ�
//ʱ�ӿ��ƼĴ���(0 ͨ��)CLKCON0
#define CLKCON0_SELFREQ				(0x80 << 8)	//SDCLKƵ�����
#define CLKCON0_ENSDCLK				(1 << 2)	//SD ʱ��������
#define CLKCON0_ENINTCLK			(1 << 0)	//�ж�ʱ��������
//��ʱ���ƼĴ���(0 ͨ��)TIMEOUTCON0



/*******************************************************************************
* Function Name  : SDIO_DeInit
* Description    : Deinitializes the SDIO peripheral registers to their default
*                  reset values.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SDIO_DeInit(void)
{
	//��ʼ��Ӳ��IO	
	rGPGCON = 0x2222222;	//��ʼ��IOΪSDIOģʽ
	rGPGPUD = 0;			//��ֹ������
		
	//ʱ�ӿ��ƼĴ�������
	SDIO0->CLKCON = CLKCON0_SELFREQ + CLKCON0_ENSDCLK + CLKCON0_ENINTCLK;
	//�������ƼĴ�������
	SDIO0->HOSTCTL = HOSTCTL0_WIDE8 + HOSTCTL0_DMASEL + HOSTCTL0_OUTEDGEINV + HOSTCTL0_WIDE4;
	//���޼Ĵ�������
	SDIO0->CAPAREG = CAPAREG0_V18 + CAPAREG0_V30 + CAPAREG0_V33 + CAPAREG0_SUSRES + CAPAREG0_DMA + CAPAREG0_HSPD + CAPAREG0_ADMA2 +\
					CAPAREG0_MAXBLKLEN + CAPAREG0_BASECLK + CAPAREG0_TOUTUNIT + CAPAREG0_TOUTCLK;
	//���ƼĴ���2����				
	SDIO0->CONTROL2 = CONTROL2_0_ENSTAASYNCCLR + CONTROL2_0_ENCMDCNFMSK + CONTROL2_0_CDINVRXD3 + CONTROL2_0_SELCARDOUT + CONTROL2_0_FLTCLKSEL + \
					CONTROL2_0_ENFBCLKTX + CONTROL2_0_ENFBCLKRX + CONTROL2_0_SDCDSEL + CONTROL2_0_SDSIGPC + CONTROL2_0_ENBUSYCHKTXSTART + \
	 				CONTROL2_0_DFCNT + CONTROL2_0_ENCLKOUTHOLD + CONTROL2_0_RWAITMODE + CONTROL2_0_DISBUFRD + CONTROL2_0_SELBASECLK + \
					CONTROL2_0_PWRSYNC + CONTROL2_0_ENCLKOUTMSKCON + CONTROL2_0_HWINITFIN;	
	//����ģʽ�Ĵ�������
	SDIO0->TRNMOD = TRNMOD0_CCSCON + TRNMOD0_MUL1SIN0 + TRNMOD0_RD1WT0 + TRNMOD0_ENACMD12 + TRNMOD0_ENBLKCNT + TRNMOD0_ENDMA;	
	//��ʱ���ƼĴ���(0 ͨ��)
	SDIO0->TIMEOUTCON	= 0x0e;		//��ʱ�������			
	//��Դ���ƼĴ�������
	SDIO0->PWRCON = PWRCON0_SELPWRLVL + PWRCON0_PWRON;
	//�����Ĵ���
	SDIO0->BLKGAP = 0;
	//����������+���Ƴ�+�������+ָ������ж�״̬+���ݳ�ʱ����+������������+ָ�����λ����+ָ�ʱ����+ָ��CRC + ������������ + д����������
	SDIO_FlagConfig(SDIO_FLAG_CARDREMOVAL | SDIO_FLAG_CARDINSERTION | SDIO_FLAG_TRANSFER | SDIO_FLAG_COMMANDEND | SDIO_FLAG_DATACRC |
					SDIO_FLAG_DATATIMEOUT | SDIO_FLAG_COMMANDINDEX | SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDTIMEOUT | SDIO_FLAG_BUFFREAD | SDIO_FLAG_BUFFWRITE,Enable);
	//ʹ�ܿ�����γ��ж�
	SDIO_FlagITConfig(SDIO_FLAG_CARDREMOVAL | SDIO_FLAG_CARDINSERTION,Enable);	
	SDIO_ClearFlag(SDIO_FLAG_ALL);	//��������жϱ�־			
}



void SDIO_FlagITConfig(u32 SDIO_FLAG, u8 EN)
{
	if(EN)	//�ж�ʹ��
		SDIO0->INTSEGEN |= SDIO_FLAG;
	else
		SDIO0->INTSEGEN &= ~SDIO_FLAG;
		
}


void SDIO_FlagConfig(u32 SDIO_FLAG, u8 EN)
{
	if(EN)	//״̬ʹ��
		SDIO0->INTSTSEN |= SDIO_FLAG;
	else
		SDIO0->INTSTSEN &= ~SDIO_FLAG;
		
}

/*************************************************************************************************************************
*����        :	void SD_SendCommand(vu8 CmdIdx,vu32 CmdArg,vu8 CmdTyp,vu8 Misc)
*����        :	��SD������һ������
*����        :	CmdIdx:ָ������;CmdArg:�������;CmdMisc:������������,���˵��
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120507
*����޸�ʱ��:	20120507
*˵��        :	дSD����Ĵ���;
				ָ������:��Щλ����ΪSD�洢�������淶��ָ���ʽ�ĵ�40��45λ��SDIO���淶��ָ����ָ����(CMD0-63, ACMD0-63)��
				
				����:	SDIO_Type_Default	һ��Ϊ0
						SDIO_Type_Pend      д������ͣ
						SDIO_Type_FS		����ѡ��
						SDIO_Type_IT		�ж�ģʽ
						
						SDIO_CMDIndexEn     SDIOָ������ʹ��
						SDIO_CMDCrcEn       SDIOָ��CRCУ��ʹ��
						
						SDIO_Response_No           ��Ӧ��
						SDIO_Response_Short        ��Ӧ��	48
						SDIO_Response_Long        ��Ӧ��	136
						SDIO_Response_ShortBusy    ��Ӧ��+��ⷱæ���
*************************************************************************************************************************/
void SDIO_SendCommand(vu8 CmdIdx,vu32 CmdArg,vu8 CmdMisc)
{	
	u16 i;	
	u16 temreg = 0;
	
	while(SDIO0->PRNSTS & BIT0);//�ȴ������߿���
	while(SDIO0->PRNSTS & BIT1);//�ȴ������߿���
	SDIO_ClearFlag(SDIO_FLAG_ALL);	//�������״̬�Ĵ�
	
	temreg = CmdIdx;
	temreg <<= 8;
	temreg |= CmdMisc;
	SDIO0->ARGUMENT = CmdArg;	//��д���������
	for(i = 0;i < 400;i ++);	//������ʱ����ָ��ִ���
	SDIO0->CMDREG = temreg;	//��д�����������Լ����͵Ȳ���
}





/*************************************************************************************************************************
*����        :	u32 SD_GetResponse(u8 SDIO_RESP)
*����        :	��ȡSD����Ӧ��
*����        :	Rep:Ӧ�����ݴ�ŵ�λ��
				SDIO_RESP1: Response Register 1
*               SDIO_RESP2: Response Register 2
*               SDIO_RESP3: Response Register 3
*               SDIO_RESP4: Response Register 4  
*����        :	����Ӧ��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120507
*����޸�ʱ��:	20120507
*˵��        :	���Ӧ�����ݵĻ�����Ϊ128BIT;4��32bit;
				ָ��Ӧ���±�27-4Ϊÿһ��Ӧ�������˴�SD���ߵ��Ĵ�����ָ��ӳ�䡣��������У��ڱ��� R[]ָ����SD�����ϴ����Ӧ�����ݵķ�Χ��
				REP[]ָ��Ӧ��Ĵ�����λ�ķ�Χ��128λӦ��λ��˳�� {RSPREG3, RSPREG2, RSPREG1, RSPREG0}
*************************************************************************************************************************/
u32 SDIO_GetResponse(u8 SDIO_RESP)
{
	return (SDIO0->RSPREG[SDIO_RESP]);
}


u32 SDIO_ReadData(void)
{ 
  	return SDIO0->BDATA;		//�ӻ�����������
}


void SDIO_WriteData(u32 Data)
{ 
	SDIO0->BDATA = Data;		//д���ݵ�������
}


u8 SDIO_GetFlagStatus(u32 SDIO_FLAG)	//��ȡ״̬�жϱ�־
{ 
	return ((SDIO_STATUS & SDIO_FLAG) ? 1 : 0);
}


void SDIO_ClearFlag(u32 SDIO_FLAG)	//���״̬�жϱ�־
{ 
	SDIO_STATUS |= SDIO_FLAG;
}




/*************************************************************************************************************************
* ����	:	void SDIO_SetFIFOInterruptAddress(u8 FIFOxAdd,u8 WordAdd)
* ����	:	����FIFO�жϴ���λ��
* ����	:	FIFOxAdd:FIFO��ַѡ��
			WordAdd:����λ��ѡ��,��λΪ��,��512B,Ҳ����1-128��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	��
*************************************************************************************************************************/
void SDIO_SetFIFOInterruptAddress(u8 FIFOxAdd,u8 WordAdd)
{
	if(WordAdd > 128)
		WordAdd = 128;
	switch (FIFOxAdd)
	{
		case FIFO_A0:
		{
			SDIO0->CONTROL3 &= ~(0x1f);
			SDIO0->CONTROL3 |= WordAdd;
		}break;
		case FIFO_A1:
		{
			SDIO0->CONTROL3 &= ~(0x1f << 8);
			SDIO0->CONTROL3 |= WordAdd << 8;
		}break;
		case FIFO_A2:
		{
			SDIO0->CONTROL3 &= ~(0x1f << 16);
			SDIO0->CONTROL3 |= WordAdd << 16;
		}break;
		case FIFO_A3:
		{
			SDIO0->CONTROL3 &= ~(0x1f << 24);
			SDIO0->CONTROL3 |= WordAdd << 24;
		}break;
		default : break;
	}
}


/*************************************************************************************************************************
* ����	:	void SDIO_CLKFrequencySelect(u8 SDCLK)	
* ����	:	����SDIOʱ�ӷ�Ƶϵ��
* ����	:	SDCLK:����SDIOʱ�ӷ�Ƶϵ��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	���ٻ���ʱ��Ϊ50MHz
			���ٻ���ʱ��Ϊ25MHz
*************************************************************************************************************************/
void SDIO_CLKFrequencySelect(u8 SDCLK)	
{
	SDIO_SDClkDisable();			//ʱ��ֹͣ
	SDIO0->CAPAREG &= ~(0x3f << 8);	//�������
	SDIO0->CAPAREG |= (50 << 8);	//��һλ������ʵû����
	SDIO0->CLKCON &= ~(0xff << 8);	//���
	SDIO0->CLKCON |= (SDCLK << 8);	//���û���ʱ�ӷ�Ƶϵ��
	SDIO_SDClkEnable();				//ʱ��ʹ��
	while(!(SDIO0->CLKCON & BIT0));	//�ȴ�ʱ���ȶ�
}



/*************************************************************************************************************************
* ����	:	void SDIO_SoftwareReset(u32 SDIO_RST)
* ����	:	����SDIO�����λ
* ����	:	SDIO_RST:
					SDIO_RSTDAT	//��λDAT��
					SDIO_RSTCMD	//��λCMD��
					SDIO_RSTALL	//��λ����
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	��
*************************************************************************************************************************/
void SDIO_SoftwareReset(u32 SDIO_RST)
{
	SDIO0->SWRST |= SDIO_RST;			//��λ
	while(SDIO0->SWRST & SDIO_RST);		//�ȴ���λ���
}



/*************************************************************************************************************************
*����        :	void SDIO_SetTimeOutClock(u8 Unit,u8 Frequency)
*����        :	��ʱʱ������
*����        :	Unit:��ʱʱ�ӵ�λѡ��
							TIME_OUT_UNIT_KHZ(0):��ʱʱ�ӵ�λΪKHZ
							TIME_OUT_UNIT_MHZ(1):��ʱʱ�ӵ�λΪMHZ
				Frequency:ʱ��Ƶ��:1~63
*����        :	����Ӧ��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120521
*����޸�ʱ��:	20120521
*˵��        :	����ָ�ʱʱ��
*************************************************************************************************************************/
void SDIO_SetTimeOutClock(u8 Unit,u8 Frequency)
{
	SDIO0->CAPAREG &= ~(0xff);	//���ԭ������
	if(Unit)					//���ó�ʱʱ�ӵ�λ:MHZ
		SDIO0->CAPAREG |= (1 << 7);
	if(Frequency > 63)			//���ֻ������Ϊ63
		Frequency = 63;
	SDIO0->CAPAREG |= Frequency;
}


