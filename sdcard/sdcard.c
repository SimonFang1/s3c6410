/*******************************************************************************
//����:SDCARD ����,SDIO ģʽ
//����:����
//����ʱ��:2012-05-16 10:32
//�޸�ʱ��:2012-05-16 10:32
//�޶�˵��:
//����:Դ���������ⷨSTM32F103X�⺯��
********************************************************************************/
#include "sdcard.h"
#include "sdio.h"
#include "system.h"

u8 SDMMC0_MOV_Flag = 0;		//sdmmc0���Ƴ��ź���

/*
1.3 �Ĵ���
���� ��� ����
CID 128 ����ʶ��
RCA 16 ��Կ���ַ��Relative card address��:����ϵͳ�п��ĵ�
ַ����̬�仯����������ʼ����ʱ��ȷ��
*SPI ģʽ��û��
CSD 128 ����������:������������ص���Ϣ����
SCR 64 SD ���üĴ���:SD ���ض���Ϣ����
OCR 32 ���������Ĵ���*/

//���� = BLOCKNR * BLOCK_LEN = (C_SIZE+1)*2^(C_SIZE_MULT+2)*2^(READ_BL_LEN)


#define SD_BlockSize 			512		//SD�����С
#define SDIO_CMD0TIMEOUT		1000000	//��ʱ��������С
#define SDIO_READTIMEOUT		1000000	//���ȴ���ʱ
#define SDIO_WRITETIMEOUT		2000000	//д�ȴ���ʱ

// SD��ָ���  	   
#define CMD0    0       //����λ
#define CMD1    1
#define CMD9    9       //����9 ����CSD����
#define CMD10   10      //����10����CID����
#define CMD12   12      //����12��ֹͣ���ݴ���
#define CMD16   16      //����16������SectorSize Ӧ����0x00
#define CMD17   17      //����17����sector
#define CMD18   18      //����18����Multi sector
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block
#define CMD24   24      //����24��дsector
#define CMD25   25      //����25��дMulti sector
#define ACMD41  41      //����41��Ӧ����0x00
#define CMD55   55      //����55��Ӧ����0x01
#define CMD58   58      //����58����OCR��Ϣ
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x0

/* Mask for errors Card Status R1 (OCR Register) */
#define SD_OCR_ADDR_OUT_OF_RANGE        ((u32)0x80000000)
#define SD_OCR_ADDR_MISALIGNED          ((u32)0x40000000)
#define SD_OCR_BLOCK_LEN_ERR            ((u32)0x20000000)
#define SD_OCR_ERASE_SEQ_ERR            ((u32)0x10000000)
#define SD_OCR_BAD_ERASE_PARAM          ((u32)0x08000000)
#define SD_OCR_WRITE_PROT_VIOLATION     ((u32)0x04000000)
#define SD_OCR_LOCK_UNLOCK_FAILED       ((u32)0x01000000)
#define SD_OCR_COM_CRC_FAILED           ((u32)0x00800000)
#define SD_OCR_ILLEGAL_CMD              ((u32)0x00400000)
#define SD_OCR_CARD_ECC_FAILED          ((u32)0x00200000)
#define SD_OCR_CC_ERROR                 ((u32)0x00100000)
#define SD_OCR_GENERAL_UNKNOWN_ERROR    ((u32)0x00080000)
#define SD_OCR_STREAM_READ_UNDERRUN     ((u32)0x00040000)
#define SD_OCR_STREAM_WRITE_OVERRUN     ((u32)0x00020000)
#define SD_OCR_CID_CSD_OVERWRIETE       ((u32)0x00010000)
#define SD_OCR_WP_ERASE_SKIP            ((u32)0x00008000)
#define SD_OCR_CARD_ECC_DISABLED        ((u32)0x00004000)
#define SD_OCR_ERASE_RESET              ((u32)0x00002000)
#define SD_OCR_AKE_SEQ_ERROR            ((u32)0x00000008)
#define SD_OCR_ERRORBITS                ((u32)0xFDFFE008)

/* Masks for R6 Response */
#define SD_R6_GENERAL_UNKNOWN_ERROR     ((u32)0x00002000)
#define SD_R6_ILLEGAL_CMD               ((u32)0x00004000)
#define SD_R6_COM_CRC_FAILED            ((u32)0x00008000)

#define SD_VOLTAGE_WINDOW_SD            ((u32)0x80100000)
#define SD_HIGH_CAPACITY                ((u32)0x40000000)
#define SD_STD_CAPACITY                 ((u32)0x00000000)
#define SD_CHECK_PATTERN                ((u32)0x000001AA)

#define SD_MAX_VOLT_TRIAL               ((u32)0x0000FFFF)
#define SD_ALLZERO                      ((u32)0x00000000)

#define SD_WIDE_BUS_SUPPORT             ((u32)0x00040000)
#define SD_SINGLE_BUS_SUPPORT           ((u32)0x00010000)
#define SD_CARD_LOCKED                  ((u32)0x02000000)
#define SD_CARD_PROGRAMMING             ((u32)0x00000007)
#define SD_CARD_RECEIVING               ((u32)0x00000006)
#define SD_DATATIMEOUT                  ((u32)0x000FFFFF)
#define SD_0TO7BITS                     ((u32)0x000000FF)
#define SD_8TO15BITS                    ((u32)0x0000FF00)
#define SD_16TO23BITS                   ((u32)0x00FF0000)
#define SD_24TO31BITS                   ((u32)0xFF000000)
#define SD_MAX_DATA_LENGTH              ((u32)0x01FFFFFF)

#define SD_HALFFIFO                     ((u32)0x00000008)
#define SD_HALFFIFOBYTES                ((u32)0x00000020)

/* Command Class Supported */
#define SD_CCCC_LOCK_UNLOCK             ((u32)0x00000080)
#define SD_CCCC_WRITE_PROT              ((u32)0x00000040)
#define SD_CCCC_ERASE                   ((u32)0x00000020)

/* Following commands are SD Card Specific commands.
   SDIO_APP_CMD should be sent before sending these commands. */
#define SDIO_SEND_IF_COND               ((u32)0x00000008)

//#define SDIO_MULTIMEDIA_CARD               ((u32)0x0)
#define SDIO_SECURE_DIGITAL_CARD           ((u32)0x1)
//#define SDIO_SECURE_DIGITAL_IO_CARD        ((u32)0x2)
//#define SDIO_HIGH_SPEED_MULTIMEDIA_CARD    ((u32)0x3)
//#define SDIO_SECURE_DIGITAL_IO_COMBO_CARD  ((u32)0x4)
//#define SDIO_HIGH_CAPACITY_SD_CARD         ((u32)0x5)
//#define SDIO_HIGH_CAPACITY_MMC_CARD        ((u32)0x6)

#define SDIO_INIT_CLK_DIV                  ((u8)0xB2)
#define SDIO_TRANSFER_CLK_DIV              ((u8)0x01)  //ʱ�ӷ�Ƶ

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static u32 CardType =  SDIO_SECURE_DIGITAL_CARD;
static u32 CSD_Tab[4], CID_Tab[4], RCA = 0;
//static u32 DeviceMode = SD_POLLING_MODE;
//static u32 TotalNumberOfBytes = 0, StopCondition = 0;
u32 *SrcBuffer, *DestBuffer;
volatile SD_Error TransferError = SD_OK;
vu32 TransferEnd = 0;
vu32 NumberOfBytes = 0;
//SD���жϷ������,������⿨�Ĳ������Ƴ���
static void __irq Isr_SDMMC_Card(void);


/* Private function prototypes -----------------------------------------------*/
static SD_Error CmdError(void);
static SD_Error CmdResp1Error(void);
static SD_Error CmdResp7Error(void);
static SD_Error CmdResp3Error(void);
static SD_Error CmdResp2Error(void);
static SD_Error CmdResp6Error(u16 *prca);
static SD_Error SDEnWideBus(u8 EN);
static SD_Error IsCardProgramming(u8 *pstatus);
static SD_Error FindSCR(u16 rca, u32 *pscr);

#define DebugPrintf printf

/*************************************************************************************************************************
* ����	:	SD_Error SD_Init(void)
* ����	:	SD����ʼ��
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120524
* ˵��	: 	��ʼ��SD��
*************************************************************************************************************************/
SD_Error SD_Init(void)
{
	SD_Error errorstatus = SD_OK;
	
	SDIO_SoftwareReset(SDIO_RSTALL);	//�����λ���мĴ���
	SDIO_DeInit();	//��ʼ��SDIOӲ��
	SDIO_FlagITConfig(SDIO_FLAG_CARDREMOVAL | SDIO_FLAG_CARDINSERTION,Enable);//ʹ�ܿ�����γ��ж�
	Set_IsrAddr(INT_HSMMC0,(vu32)Isr_SDMMC_Card);	//�����ж�ʸ�����
	Set_IntEnable(INT_HSMMC0,ENABLE);	//����SDMMC0�ж�
	
	errorstatus = SD_SetIdleSta();	//SD���ϵ�
	if (errorstatus != SD_OK)		//���ϵ緢�ʹ���
	{
		DebugPrintf("SD power up error:(%d)!\n",errorstatus);	//����,��ӡ����
		return(errorstatus);	//���ش���
	}

	errorstatus = SD_InitializeCards();
	if (errorstatus != SD_OK)
	{
		DebugPrintf("SD initialize error(%d)!\n",errorstatus);	//����,��ӡ����
		return(errorstatus);
	}
	
	return(errorstatus);
}



/*************************************************************************************************************************
* ����	:	SD_Error SD_SetIdleSta(void)
* ����	:	SD���ϵ�������ģʽ,��ʶ��
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	:   SD���ϵ�������ģʽ
*************************************************************************************************************************/
SD_Error SD_SetIdleSta(void)
{
  SD_Error errorstatus = SD_OK;
  u32 response = 0, count = 0;
  bool validvoltage = FALSE;
  u32 SDType = SD_STD_CAPACITY;
  u16 i;

	SDIO_CLKFrequencySelect(SDCLK_1_64);		//����ʱ��400KHZ
	SDIO_SetTimeOutClock(TIME_OUT_UNIT_KHZ,1);	//���ó�ʱʱ��Ƶ�����
	//��������74��ʱ��,�ȴ�SD���ϵ�ɹ���ͬ��
	for(response = 0;response < 1000;response ++);
  
  	
  	//ѭ����������CMD0,����Ӧ,�޷���,��SD���������ģʽ
  	for(i = 0;i < 50;i ++)
  	{
	  	SDIO_SendCommand(CMD0,0,0);		
		errorstatus = CmdError();		//�ж������Ƿ�ִ�гɹ�,������ֻҪ��ʼ����SDIO�ͻ�ִ�гɹ�
		if(errorstatus == SD_OK)
			break;
  	}
  	if(errorstatus != SD_OK)
  	{
  		DEBUG("error!(%d)\n",errorstatus);	//����,��ӡ��Ϣ	
  		return(errorstatus);
  	}
	
  	//����CMD8:SEND_IF_COND;����Ӧ,�������:SD_CHECK_PATTERN;������ӦR7
  	//ʶ�𿨰汾
	SDIO_SendCommand(SDIO_SEND_IF_COND,SD_CHECK_PATTERN,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp7Error();
	if (errorstatus == SD_OK)		//���سɹ�;˵����ΪSD Card 2.0 V2.0
	{
		CardType = SDIO_STD_CAPACITY_SD_CARD_V2_0; /* SD Card 2.0 */
		SDType = SD_HIGH_CAPACITY;	
		DEBUG("SDIO_STD_CAPACITY_SD_CARD_V2_0!\n");	//����,��ӡ������Ϣ
	}
	else	//V1.0 V1.1
	{
		DEBUG("SD Card V1.1!\n");	//����,��ӡ��Ϣ
		CardType = SDIO_STD_CAPACITY_SD_CARD_V1_1; 		//V1.0 V1.1
		SDIO_SendCommand(CMD55,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		CmdResp1Error();
	}

	do
	{
		//����CMD55 SDIO_APP_CMD;�������:0;������ӦR1,����RCAΪ0,����Ӧ
      	SDIO_SendCommand(SDIO_APP_CMD,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp1Error();
		if (errorstatus != SD_OK)
		{
			DEBUG("CMD55 error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
			return(errorstatus);
		}
		//����ACM41����;�������:SD_APP_OP_COND(0x80100000);����Ӧ.��ӦΪR3,���ز��������Ĵ���RCA
		SDIO_SendCommand(SDIO_SD_APP_OP_COND,SD_VOLTAGE_WINDOW_SD | SDType,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp3Error();
		if (errorstatus != SD_OK)
		{
			DEBUG("ACM41 error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
			return(errorstatus);
		}
		response = SDIO_GetResponse(SDIO_RESP1);	//��ȡ��Ӧ,RESE1
		validvoltage = (bool) (((response >> 31) == 1) ? 1 : 0);
		count++;
	}
	while((!validvoltage) && (count < SD_MAX_VOLT_TRIAL));//ѭ����ʼ��,ֱ�����سɹ����߳�ʱ

    if (count >= SD_MAX_VOLT_TRIAL)	//���Դ�������
    {
		errorstatus = SD_INVALID_VOLTRANGE;
		return(errorstatus);
    }

    if (response &= SD_HIGH_CAPACITY)
    {
		CardType = SDIO_HIGH_CAPACITY_SD_CARD;
    }

	return(errorstatus);
}

/*************************************************************************************************************************
* ����	:	SD_Error SD_PowerOFF(void)
* ����	:	SD������
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	SD������
*************************************************************************************************************************/
SD_Error SD_PowerOFF(void)
{
	SD_Error errorstatus = SD_OK;
	
	SDIO_BusPowerOFF();	//�ر����ߵ�Դ
	
	return(errorstatus);
}


/*************************************************************************************************************************
* ����	:	SD_Error SD_PowerON(void)
* ����	:	SD����Դ����
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120524
* ˵��	: 	SD����Դ����
*************************************************************************************************************************/
SD_Error SD_PowerON(void)
{
	SD_Error errorstatus = SD_OK;
	
	SDIO_BusPowerON();	//�����ߵ�Դ
	
	return(errorstatus);
}



/*************************************************************************************************************************
* ����	:	SD_Error SD_InitializeCards(void)
* ����	:	�����еĿ����г�ʼ������
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	�����еĿ����г�ʼ������
*************************************************************************************************************************/
SD_Error SD_InitializeCards(void)
{
	SD_Error errorstatus = SD_OK;
	u16 rca = 0x01;

	//����CMD2 SDIO_ALL_SEND_CID����,�������:0;���ظ�,R2
	//����CMD������ȡCID��Ϣ��
	SDIO_SendCommand(SDIO_ALL_SEND_CID,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Long);
	errorstatus = CmdResp2Error();				//��ȡ��ӦR2
	if (errorstatus != SD_OK)
	{
		DEBUG("error!(%d)\n",errorstatus);					//����,��ӡ������Ϣ
		return(errorstatus);
	}
	//��ÿ�����Ի�ȡÿ������Ψһ��ʶCID
	CID_Tab[0] = SDIO_GetResponse(SDIO_RESP1);		
	CID_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
	CID_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
	CID_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
	
	//����CMD3,SET_REL_ADDR����,����0,��Ӧ,����Ӧ,R6
	//������ȡ����ַ
	//��������CMD3��SEND_RELATIVE_ADDR��Ҫ�󿨷���һ���µ���Կ���ַRCA����ַ��CID�̣����Ժ�����ݴ���ģʽ������Ѱַ����һ�����RCA�󣬿�״̬��ɾ���״̬��Stand-by state��
	SDIO_SendCommand(SDIO_SET_REL_ADDR,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
    errorstatus = CmdResp6Error(&rca);	//��ȡ����ַ
    if (errorstatus != SD_OK)
    {
		DEBUG("error!(%d)!\n",errorstatus);					//����,��ӡ������Ϣ
		return(errorstatus);
    }
		
	RCA = rca;//�洢����ַ
	
	//����CMD9 SDIO_SEND_CSD����,����:rca��ַ;����Ӧ,R2;
	//��������һ���µ�RCA,��Ҫ���������ÿ���ַ��
    SDIO_SendCommand(SDIO_SEND_CSD,(u32)rca << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Long);
    errorstatus = CmdResp2Error();
    if (errorstatus != SD_OK)
    {
		DEBUG("error!(%d)!\n",errorstatus);					//����,��ӡ������Ϣ
		return(errorstatus);
    }
    
    //��ȡCSD
    CSD_Tab[0] = SDIO_GetResponse(SDIO_RESP1);
    CSD_Tab[1] = SDIO_GetResponse(SDIO_RESP2);
    CSD_Tab[2] = SDIO_GetResponse(SDIO_RESP3);
    CSD_Tab[3] = SDIO_GetResponse(SDIO_RESP4);
    
    //ѡ�п�,������
 	errorstatus = SD_SelectDeselect((u32)RCA << 16);
 	if(errorstatus != SD_OK)
 	{
 		DEBUG("SelectDeselect error!(%d)!\n",errorstatus);					//����,��ӡ������Ϣ
 		return(errorstatus);
 	}

	errorstatus = SD_OK; /* All cards get intialized */
 	
 	SDIO_CLKFrequencySelect(SDCLK_1_2);	//����ʱ��
 	SDIO_SetTimeOutClock(TIME_OUT_UNIT_KHZ,63);	//���ó�ʱʱ��Ƶ�����
	errorstatus = SD_EnableWideBusMode(Enable);
	if(errorstatus == SD_OK)	//����SD��Ϊ4��ģʽ
	{
		DEBUG("SD SDIO 4BIT OK\n");
	}
	else
	{
		errorstatus = SD_EnableWideBusMode(Disable);
		DEBUG("SD SDIO 4BIT ERROR (%d)\n",errorstatus);
	}
	
	SDIO_SetTransferBlockSize(SD_BlockSize);//���ô�����СΪ512�ֽ�
	errorstatus = SD_SetBlockSize(SD_BlockSize);//����SD�����С	
	if (errorstatus != SD_OK)
	{
		DEBUG("SD SetBlockSize error(%d)!\n",errorstatus);
		return(errorstatus);
	}
	
	return(errorstatus);
}


/*************************************************************************************************************************
* ����	:	SD_Error SD_SelectDeselect(u32 addr)
* ����	:	ѡ��һ����,�����ڴ���״̬
* ����	:	addr:����ַ
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	ѡ��һ�������������ڴ���״̬��Transfer state)
*************************************************************************************************************************/
SD_Error SD_SelectDeselect(u32 addr)
{
	SD_Error errorstatus = SD_OK;

	//CMD7����ѡ��һ�������������ڴ���״̬��Transfer state�������κ�ʱ��ֻ����һ�������ڴ���״̬
	SDIO_SendCommand(SDIO_SEL_DESEL_CARD,addr,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();

	return(errorstatus);
}

/*************************************************************************************************************************
* ����	:	SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
* ����	:	��ȡ����ϸ����Ϣ
* ����	:	cardinfo:����Ϣ�ṹָ��,ָ����Ϣ��Ż�������ַ
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	��ȡ������Ϣ,ͨ��CSD��Ϣ�õ�
*************************************************************************************************************************/
SD_Error SD_GetCardInfo(SD_CardInfo *cardinfo)
{
  SD_Error errorstatus = SD_OK;
  u8 tmp = 0;

  cardinfo->CardType = (u8)CardType;
  cardinfo->RCA = (u16)RCA;

  /* Byte 0 */
  tmp = (u8)((CSD_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CSDStruct = (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.SysSpecVersion = (tmp & 0x3C) >> 2;
  cardinfo->SD_csd.Reserved1 = tmp & 0x03;

  /* Byte 1 */
  tmp = (u8)((CSD_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.TAAC = tmp;

  /* Byte 2 */
  tmp = (u8)((CSD_Tab[0] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.NSAC = tmp;

  /* Byte 3 */
  tmp = (u8)(CSD_Tab[0] & 0x000000FF);
  cardinfo->SD_csd.MaxBusClkFrec = tmp;

  /* Byte 4 */
  tmp = (u8)((CSD_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_csd.CardComdClasses = tmp << 4;

  /* Byte 5 */
  tmp = (u8)((CSD_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.CardComdClasses |= (tmp & 0xF0) >> 4;
  cardinfo->SD_csd.RdBlockLen = tmp & 0x0F;

  /* Byte 6 */
  tmp = (u8)((CSD_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.PartBlockRead = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrBlockMisalign = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.RdBlockMisalign = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.DSRImpl = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.Reserved2 = 0; /* Reserved */
  cardinfo->SD_csd.DeviceSize = (tmp & 0x03) << 10;

  /* Byte 7 */
  tmp = (u8)(CSD_Tab[1] & 0x000000FF);
  cardinfo->SD_csd.DeviceSize |= (tmp) << 2;

  /* Byte 8 */
  tmp = (u8)((CSD_Tab[2] & 0xFF000000) >> 24);

  cardinfo->SD_csd.DeviceSize |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.MaxRdCurrentVDDMin = (tmp & 0x38) >> 3;
  cardinfo->SD_csd.MaxRdCurrentVDDMax = (tmp & 0x07);

  /* Byte 9 */
  tmp = (u8)((CSD_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrCurrentVDDMin = (tmp & 0xE0) >> 5;
  cardinfo->SD_csd.MaxWrCurrentVDDMax = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.DeviceSizeMul = (tmp & 0x03) << 1;

  /* Byte 10 */
  tmp = (u8)((CSD_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.DeviceSizeMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.EraseGrSize = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.EraseGrMul = (tmp & 0x3F) << 1;

  /* Byte 11 */
  tmp = (u8)(CSD_Tab[2] & 0x000000FF);
  cardinfo->SD_csd.EraseGrMul |= (tmp & 0x80) >> 7;
  cardinfo->SD_csd.WrProtectGrSize = (tmp & 0x7F);

  /* Byte 12 */
  tmp = (u8)((CSD_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_csd.WrProtectGrEnable = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.ManDeflECC = (tmp & 0x60) >> 5;
  cardinfo->SD_csd.WrSpeedFact = (tmp & 0x1C) >> 2;
  cardinfo->SD_csd.MaxWrBlockLen = (tmp & 0x03) << 2;

  /* Byte 13 */
  tmp = (u8)((CSD_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_csd.MaxWrBlockLen |= (tmp & 0xC0) >> 6;
  cardinfo->SD_csd.WriteBlockPaPartial = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.Reserved3 = 0;
  cardinfo->SD_csd.ContentProtectAppli = (tmp & 0x01);

  /* Byte 14 */
  tmp = (u8)((CSD_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_csd.FileFormatGrouop = (tmp & 0x80) >> 7;
  cardinfo->SD_csd.CopyFlag = (tmp & 0x40) >> 6;
  cardinfo->SD_csd.PermWrProtect = (tmp & 0x20) >> 5;
  cardinfo->SD_csd.TempWrProtect = (tmp & 0x10) >> 4;
  cardinfo->SD_csd.FileFormat = (tmp & 0x0C) >> 2;
  cardinfo->SD_csd.ECC = (tmp & 0x03);

  /* Byte 15 */
  tmp = (u8)(CSD_Tab[3] & 0x000000FF);
  cardinfo->SD_csd.CSD_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_csd.Reserved4 = 1;


  /* Byte 0 */
  tmp = (u8)((CID_Tab[0] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ManufacturerID = tmp;

  /* Byte 1 */
  tmp = (u8)((CID_Tab[0] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.OEM_AppliID = tmp << 8;

  /* Byte 2 */
  tmp = (u8)((CID_Tab[0] & 0x000000FF00) >> 8);
  cardinfo->SD_cid.OEM_AppliID |= tmp;

  /* Byte 3 */
  tmp = (u8)(CID_Tab[0] & 0x000000FF);
  cardinfo->SD_cid.ProdName1 = tmp << 24;

  /* Byte 4 */
  tmp = (u8)((CID_Tab[1] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdName1 |= tmp << 16;

  /* Byte 5 */
  tmp = (u8)((CID_Tab[1] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdName1 |= tmp << 8;

  /* Byte 6 */
  tmp = (u8)((CID_Tab[1] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdName1 |= tmp;

  /* Byte 7 */
  tmp = (u8)(CID_Tab[1] & 0x000000FF);
  cardinfo->SD_cid.ProdName2 = tmp;

  /* Byte 8 */
  tmp = (u8)((CID_Tab[2] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdRev = tmp;

  /* Byte 9 */
  tmp = (u8)((CID_Tab[2] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.ProdSN = tmp << 24;

  /* Byte 10 */
  tmp = (u8)((CID_Tab[2] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ProdSN |= tmp << 16;

  /* Byte 11 */
  tmp = (u8)(CID_Tab[2] & 0x000000FF);
  cardinfo->SD_cid.ProdSN |= tmp << 8;

  /* Byte 12 */
  tmp = (u8)((CID_Tab[3] & 0xFF000000) >> 24);
  cardinfo->SD_cid.ProdSN |= tmp;

  /* Byte 13 */
  tmp = (u8)((CID_Tab[3] & 0x00FF0000) >> 16);
  cardinfo->SD_cid.Reserved1 |= (tmp & 0xF0) >> 4;
  cardinfo->SD_cid.ManufactDate = (tmp & 0x0F) << 8;

  /* Byte 14 */
  tmp = (u8)((CID_Tab[3] & 0x0000FF00) >> 8);
  cardinfo->SD_cid.ManufactDate |= tmp;

  /* Byte 15 */
  tmp = (u8)(CID_Tab[3] & 0x000000FF);
  cardinfo->SD_cid.CID_CRC = (tmp & 0xFE) >> 1;
  cardinfo->SD_cid.Reserved2 = 1;

  return(errorstatus);
}


/*************************************************************************************************************************
* ����	:	SD_Error SD_EnableWideBusMode(u8 EN)
* ����	:	ʹ��4bit DAT��ģʽ,���ʧ�ܽ�����ԭ��ģʽ
* ����	:	Enable:4bitģʽ;Disable:1bitģʽ
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120524
* ˵��	: 	���ÿ��Ϳ�����Ϊ4bit����ģʽ
*************************************************************************************************************************/
SD_Error SD_EnableWideBusMode(u8 EN)
{
	SD_Error errorstatus = SD_OK;
	u8 IntFlag;

	//SD����֧������
	if (CardType == SDIO_MULTIMEDIA_CARD)
	{
		errorstatus = SD_UNSUPPORTED_FEATURE;
		return(errorstatus);
	}
	else if ((CardType == SDIO_SECURE_DIGITAL_CARD) || (CardType == SDIO_HIGH_CAPACITY_SD_CARD))	//SD��
	{
		if (EN)	//4BITģʽ
		{
    		errorstatus = SDEnWideBus(Enable);	//����SD��Ϊ4bit����ģʽ
    		if (errorstatus == SD_OK)	//����SD���ɹ�
			{
				IntFlag = Get_IntEnable(INT_HSMMC0);		
				if(IntFlag)	//��������˿�ȫ���ж�
				{
					Set_IntEnable(INT_HSMMC0,DISABLE);	//�ȹر��ж�,��ֹ�󴥷�
				}
				SDIO_SetDataDataWidth_4b();	//����SDIOΪ4bitģʽ
    			SDIO_ClearFlag(SDIO_FLAG_ALL);	//������б�־
				if(IntFlag)	//��������˿�ȫ���ж�
				{
					Set_IntEnable(INT_HSMMC0,ENABLE);	//����
				}
			}	
  		}
  		else
  		{
  			errorstatus = SDEnWideBus(Disable);	//����SD��Ϊ1bit����ģʽ
    		if (errorstatus == SD_OK)	//����SD���ɹ�
			{
				IntFlag = Get_IntEnable(INT_HSMMC0);		
				if(IntFlag)	//��������˿�ȫ���ж�
				{
					Set_IntEnable(INT_HSMMC0,DISABLE);	//�ȹر��ж�,��ֹ�󴥷�
				}
				SDIO_SetDataDataWidth_1b();	//����SDIOΪ1bitģʽ
    			SDIO_ClearFlag(SDIO_FLAG_ALL);	//������б�־
				if(IntFlag)	//��������˿�ȫ���ж�
				{
					Set_IntEnable(INT_HSMMC0,ENABLE);	//����
				}
			}
  		}
  	}	
	return(errorstatus);
}


/*************************************************************************************************************************
* ����	:	static SD_Error SDEnWideBus(u8 EN)
* ����	:	����SD��������ģʽ
* ����	:	Enable:4bitģʽ;Disable:1bitģʽ
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120524
* ˵��	: 	����SD��������ģʽ
*************************************************************************************************************************/
static SD_Error SDEnWideBus(u8 EN)
{
	SD_Error errorstatus = SD_OK;
	u32 scr[2] = {0, 0};

	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
	{
		errorstatus = SD_LOCK_UNLOCK_FAILED;
		return(errorstatus);
	}

	//��ȡRCA
	errorstatus = FindSCR(RCA, scr);

	if (errorstatus != SD_OK)
	{
		DEBUG("Get SCR error(%d)!\n",errorstatus);
		return(errorstatus);
	}
  
	if (EN)	//ʹ��4bit dat
	{
		//�������Ŀ�֧�ֿ�������ģʽ
	    if ((scr[1] & SD_WIDE_BUS_SUPPORT) != SD_ALLZERO)
	    {
			//����CMD55,SDIO_APP_CMD,���
			//����CMD55 SDIO_APP_CMD;�������:RCA;������ӦR1,����RCAΪ0,����Ӧ
		  	SDIO_SendCommand(SDIO_APP_CMD,(u32)RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				DEBUG("CMD55 error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
				return(errorstatus);
			}
			//����ACMD6,SDIO_APP_SD_SET_BUSWIDTH,���ÿ�����ģʽ,����0x2,����Ӧ,R1
			SDIO_SendCommand(SDIO_APP_SD_SET_BUSWIDTH,0x2,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				DEBUG("ACMD6 error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
				return(errorstatus);
			}
			return(errorstatus);
	    }
	    else	//����Ŀ���֧�ֿ�����ģʽ
	    {
	    	errorstatus = SD_REQUEST_NOT_APPLICABLE;
	    	return(errorstatus);
		}
	}   
	else	//ʧ�ܿ�����ģʽ
  	{
    	//�������Ŀ�֧��1bit����ģʽ
		if ((scr[1] & SD_SINGLE_BUS_SUPPORT) != SD_ALLZERO)
		{
			//����CMD55 SDIO_APP_CMD;�������:RCA;������ӦR1,����RCAΪ0,����Ӧ
		  	SDIO_SendCommand(SDIO_APP_CMD,(u32)RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				DEBUG("CMD55 error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
				return(errorstatus);
			}
			//����ACMD6,SDIO_APP_SD_SET_BUSWIDTH,���ÿ�����ģʽ,����0x0,����Ӧ,R1
			SDIO_SendCommand(SDIO_APP_SD_SET_BUSWIDTH,0x0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				DEBUG("ACMD6 error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
				return(errorstatus);
			}
			return(errorstatus);
		}
		else	//��֧��1bit����ģʽ
		{
			errorstatus = SD_REQUEST_NOT_APPLICABLE;
			return(errorstatus);
		}
	}
}



/*************************************************************************************************************************
* ����	:	SD_Error SD_SetDeviceMode(u32 Mode)
* ����	:	����SDIO����ģʽ
* ����	:	SD_DMA_MODE: DMAģʽ
*           SD_INTERRUPT_MODE: �ж�ģʽ
*           SD_POLLING_MODE: ��ѯģʽ(��ͨģʽ)
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	����SDIO����ģʽ
*************************************************************************************************************************/
SD_Error SD_SetDeviceMode(u32 Mode)
{
  SD_Error errorstatus = SD_OK;

 /* if ((Mode == SD_DMA_MODE) || (Mode == SD_INTERRUPT_MODE) || (Mode == SD_POLLING_MODE))
  {
    DeviceMode = Mode;
  }
  else
  {
    errorstatus = SD_INVALID_PARAMETER;	//ģʽ���ô���
  }*/
  return(errorstatus);

}



/*************************************************************************************************************************
* ����	:	SD_Error SD_SetBlockSize(u16 BlockSize)
* ����	:	����SD�����С
* ����	:	���С,512,1024,�ȵ�
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	ͨ��ȡ512B
*************************************************************************************************************************/
SD_Error SD_SetBlockSize(u16 BlockSize)
{
	SD_Error errorstatus = SD_OK;

	//������������򷵻�
	if (SDIO_GetResponse(SDIO_RESP1) & SD_CARD_LOCKED)
	{
		errorstatus = SD_LOCK_UNLOCK_FAILED;
		DEBUG("%d\r\n",errorstatus);
		return(errorstatus);
	}

	//�жϿ��С�Ƿ����
	if ((BlockSize > 0) && (BlockSize <= 2048) && ((BlockSize & (BlockSize - 1)) == 0))
	{
		//Set Block Size for Card 
		SDIO_SendCommand(SDIO_SET_BLOCKLEN,BlockSize,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp1Error();
		if (errorstatus != SD_OK)
		{
			DEBUG("%d\r\n",errorstatus);
			return(errorstatus);
		}
	}
	else	//���С���ô���
	{
		errorstatus = SD_INVALID_PARAMETER;
		return(errorstatus);
	}
	return(errorstatus);
}



/*************************************************************************************************************************
* ����	:	SD_Error SD_ReadBlock(u32 BlockAddr, u32 *BlockBuff)
* ����	:	��SD��һ����
* ����	:	BlockAddr:���ַ;
			BlockBuff:�黺������ַ
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120728
* ˵��	: 	��SD��һ������
*************************************************************************************************************************/
SD_Error SD_ReadBlock(u32 BlockAddr, u32 *BlockBuff)
{
	SD_Error errorstatus = SD_OK;
	u32 timeout = SDIO_READTIMEOUT;		//���ȴ���ʱ������
	u16 cnt = 0;
 
	if (BlockBuff == NULL)			//û�з�����ջ�����,����
	{
		errorstatus = SD_INVALID_PARAMETER;
		return(errorstatus);
	}
	
	SDIO_SoftwareReset(SDIO_RSTDAT);	//�����λDAT,��Ҫ�������FIFO
	SDIO_SoftwareReset(SDIO_RSTCMD);	//�����λDAT,��Ҫ�������״̬�Լ�����
	SDIO_ReadMode();					//��������������Ϊ��ģʽ
	SDIO_SetTransferBlockSize(SD_BlockSize);	//���õ��δ���Ŀ��СΪSD_BlockSize
	SDIO_SingleBlockMode();				//����Ϊ���鴫��ģʽ
	
	//����CMD17 READ_SINGLE_BLOCK,���ȡָ��,����:���ַ,�̻ظ�,R1
	//�˴������Ҽ����ʱ�����,���շ��־�Ȼ������һ������:SDIO_DataSelect;��ǰ����ѡ��,������������DAT����û�����ݻ���
	SDIO_SendCommand(SDIO_READ_SINGLE_BLOCK,(BlockAddr << 9),SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();	//��ȡ�ظ�
	if (errorstatus != SD_OK)		//����ʹ���,����
	{
		DEBUG("CMD17 error (%d)!\n",errorstatus);
		return(errorstatus);
	}
	
	while(((SDIO0->PRNSTS & BIT11) == 0) && timeout && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0))	//�ȴ�����������Ч,���ߵȴ���ʱ
	{
		timeout --;
	}

	if (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) || (timeout == 0))	//��ʱ
    {
		SDIO_ClearFlag(SDIO_FLAG_DATATIMEOUT);	//�����־
 		errorstatus = SD_DATA_TIMEOUT;
 		DEBUG("%d\r\n",errorstatus);
		return(errorstatus);
    }
	
	//��������СSD_BlockSize�ֽ�,��SD_BlockSize/4��
	for(cnt = 0;cnt < SD_BlockSize / 4;cnt ++)
  	{
  		*(BlockBuff ++) = SDIO_ReadData();
	}
    //������б�־
    SDIO_ClearFlag(SDIO_FLAG_ALL);
 
	return(errorstatus);
}



/*************************************************************************************************************************
* ����	:	SD_Error SD_ReadMultiBlocks(u32 BlockAddr, u32 *BlockBuff, u32 NumberOfBlocks)
* ����	:	��SD�������
* ����	:	BlockAddr:���ַ;
			BlockBuff:�黺������ַ
			NumberOfBlocks:������
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120728
* ˵��	: 	��SD�����(����1��)����
*************************************************************************************************************************/
SD_Error SD_ReadMultiBlocks(u32 BlockAddr, u32 *BlockBuff, u16 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	u32 timeout = SDIO_READTIMEOUT;//���ȴ���ʱ������
	u16 BlockCnt = NumberOfBlocks;	//��Ҫ����Ŀ����
	u16 cnt = 0;

	if (BlockBuff == NULL)	//û�з�����ջ�����,����
	{
		errorstatus = SD_INVALID_PARAMETER;
		DEBUG("%d\r\n",errorstatus);
		return(errorstatus);
	}

	if (NumberOfBlocks > 1)	
	{
		if (NumberOfBlocks * SD_BlockSize > SD_MAX_DATA_LENGTH)//�ж���ַ�Ƿ񳬳���Χ
		{
			errorstatus = SD_INVALID_PARAMETER;
			DEBUG("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		SDIO_SoftwareReset(SDIO_RSTDAT);			//�����λDAT,��Ҫ�������FIFO
		SDIO_SoftwareReset(SDIO_RSTCMD);			//�����λDAT,��Ҫ�������״̬�Լ�����
		SDIO_MultipleBlockMode();					//����Ϊ�����δ���ģʽ
		SDIO_SetTransferBlockCnt(NumberOfBlocks);	//���ô��������
		SDIO_SetTransferBlockSize(SD_BlockSize);	//���õ��δ���Ŀ��СΪSD_BlockSize
		SDIO_ReadMode();							//����Ϊ������ģʽ
		SDIO_BlockCountEnable();					//���������

	    //����CMD18 SDIO_READ_MULT_BLOCK;�����ζ�����,����:��ʼ��ַ,�̷���,R1
	    SDIO_SendCommand(SDIO_READ_MULT_BLOCK,(BlockAddr << 9),SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	    errorstatus = CmdResp1Error();
	    if (errorstatus != SD_OK)
	    {
	    	DEBUG("%d\r\n",errorstatus);
	    	return(errorstatus);
	    }
	    
	    do
	    {
	    	if(SDIO0->PRNSTS & BIT11)	//����������Ч
	    	{
	    		SDIO_ClearFlag(SDIO_FLAG_BUFFREAD);		//�������������Ч��־
	    		//��������СSD_BlockSize�ֽ�,��SD_BlockSize/4��
	    		for(cnt = 0;cnt < SD_BlockSize / 4;cnt ++)
			  	{
			  		*(BlockBuff ++) = SDIO_ReadData();
				}
			  	BlockCnt --;	//������һ��
			  	timeout = SDIO_READTIMEOUT;	//������һ��,��ʱ���������¿�ʼ����
	    	}
	    	timeout --;
	    }while(BlockCnt && (timeout != 0) && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0));	//ʣ���������Ϊ0,û��ʱ
		
		//����CMD12 SDIO_STOP_TRANSMISSION����,��ֹ��ȡ;����:0,����Ӧ,R1
		SDIO_SendCommand(SDIO_STOP_TRANSMISSION,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp1Error();
		if (errorstatus != SD_OK)
		{
			DEBUG("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		if(timeout == 0)	//��ʱ
		{
			errorstatus = SD_DATA_TIMEOUT;
			DEBUG("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		SDIO_ClearFlag(SDIO_FLAG_ALL);	//��������жϱ�־
		
	}
	return(errorstatus);
}





/*************************************************************************************************************************
* ����	:	SD_Error SD_WriteBlock(u32 BlockAddr, u32 *BlockBuff)
* ����	:	дSD��һ����
* ����	:	BlockAddr:���ַ;
			writebuff:д��������ַ
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120525
* ����޸�ʱ�� : 20120728
* ˵��	: 	дSD��һ������
* 			20120728:�޸���д�뺯��,���ڿ��Գɹ�д������
*************************************************************************************************************************/
SD_Error SD_WriteBlock(u32 BlockAddr, u32 *BlockBuff)
{
	SD_Error errorstatus = SD_OK;
	u32 timeout = SDIO_WRITETIMEOUT;		//д�ȴ���ʱ������
	u32 cardstatus = 0;
	u8 cardstate = 0;
	u16 cnt = 0;

	if (BlockBuff == NULL)
	{
		errorstatus = SD_INVALID_PARAMETER;
		DEBUG("%d\r\n",errorstatus);
		return(errorstatus);
	}

	SDIO_SoftwareReset(SDIO_RSTDAT);	//�����λDAT,��Ҫ�������FIFO
	SDIO_SoftwareReset(SDIO_RSTCMD);	//�����λDAT,��Ҫ�������״̬�Լ�����
	SDIO_WriteMode();					//��������������Ϊдģʽ
	SDIO_SetTransferBlockSize(SD_BlockSize);	//���õ��δ���Ŀ��СΪSD_BlockSize
	SDIO_SingleBlockMode();				//����Ϊ���鴫��ģʽ
	
	do
	{
		timeout --;
		//����CMD13,SDIO_SEND_STATUS,�� Card_Status �Ĵ���,����,RCA��ַ,�̷���,R1;
		SDIO_SendCommand(SDIO_SEND_STATUS,(u32) RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp1Error();
		if (errorstatus != SD_OK)
		{
			DEBUG("%d\r\n",errorstatus);
			return(errorstatus);
		}
		cardstatus = SDIO_GetResponse(SDIO_RESP1);
	}
	while (((cardstatus & 0x00000100) == 0) && (timeout > 0));
	
	if (timeout == 0)
	{
		DEBUG("%d\r\n",errorstatus);
		return(SD_ERROR);
	}
	
	//����CMD24,SDIO_WRITE_SINGLE_BLOCK,д����,����:��ַ,����Ӧ,R1
	SDIO_SendCommand(SDIO_WRITE_SINGLE_BLOCK,(BlockAddr << 9),SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();
	if (errorstatus != SD_OK)
	{
		DEBUG("%d\r\n",errorstatus);
		return(errorstatus);
	}
	timeout = SDIO_WRITETIMEOUT;		//д�ȴ���ʱ������
	//д�������Ƿ���ЧӦ���ж�PRNSTS����Ĵ���
	while(((SDIO0->PRNSTS & BIT10) == 0) && (SDIO_GetFlagStatus(SDIO_FLAG_BUFFWRITE) == 0) && timeout && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0))	//�ȴ�д��������Ч,���ߵȴ���ʱ
	{
		timeout --;
	}

	if (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) || (timeout == 0))	//��ʱ
    {
		SDIO_ClearFlag(SDIO_FLAG_DATATIMEOUT);	//�����־
 		errorstatus = SD_DATA_TIMEOUT;
 		DEBUG("%d\r\n",errorstatus);
		return(errorstatus);
    }
	
	//��������СSD_BlockSize�ֽ�,��SD_BlockSize / 4��
	for(cnt = 0;cnt < SD_BlockSize / 4;cnt ++)
  	{
  		SDIO_WriteData(BlockBuff[cnt]);
	}
    //������б�־
    SDIO_ClearFlag(SDIO_FLAG_ALL);

    //��ȡ��״̬,�ȴ�д�����
	do
	{
		errorstatus = IsCardProgramming(&cardstate);
	}while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)));
	
	return(errorstatus);
}





/*************************************************************************************************************************
* ����	:	SD_Error SD_WriteMultiBlocks(u32 BlockAddr, u32 *BlockBuff, u16 NumberOfBlocks)
* ����	:	дSD�������
* ����	:	BlockAddr:���ַ;
			BlockBuff:�黺������ַ
			NumberOfBlocks:������
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120525
* ����޸�ʱ�� : 20120728
* ˵��	: 	дSD�����(����1��)����
* 			20120728:�޸���д�뺯��,���ڿ��Գɹ�д������
*************************************************************************************************************************/
SD_Error SD_WriteMultiBlocks(u32 BlockAddr, u32 *BlockBuff, u16 NumberOfBlocks)
{
	SD_Error errorstatus = SD_OK;
	u32 timeout = SDIO_WRITETIMEOUT;		//д�ȴ���ʱ������
	u16 BlockCnt = NumberOfBlocks;			//����Ŀ���������
	u8 cardstate = 0;
	u32 cardstatus = 0;
	u16 cnt = 0;

	if (BlockBuff == NULL)
	{
		errorstatus = SD_INVALID_PARAMETER;
		DEBUG("%d\r\n",errorstatus);
		return(errorstatus);
	}
 
	if (NumberOfBlocks > 1)
	{
		if (NumberOfBlocks * SD_BlockSize > SD_MAX_DATA_LENGTH)	//�жϵ�ַ
		{
			errorstatus = SD_INVALID_PARAMETER;
			DEBUG("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		SDIO_SoftwareReset(SDIO_RSTDAT);			//�����λDAT,��Ҫ�������FIFO
		SDIO_SoftwareReset(SDIO_RSTCMD);	//�����λDAT,��Ҫ�������״̬�Լ�����
		SDIO_MultipleBlockMode();					//����Ϊ�����δ���ģʽ
		SDIO_SetTransferBlockCnt(NumberOfBlocks);	//���ô��������
		SDIO_SetTransferBlockSize(SD_BlockSize);	//���õ��δ���Ŀ��СΪSD_BlockSize
		SDIO_WriteMode();							//����Ϊд����ģʽ
		SDIO_BlockCountEnable();					//���������
		
		do
		{
			timeout --;
			//����CMD13,SDIO_SEND_STATUS,�� Card_Status �Ĵ���,����,RCA��ַ,�̷���,R1;
			SDIO_SendCommand(SDIO_SEND_STATUS,(u32) RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
			errorstatus = CmdResp1Error();
			if (errorstatus != SD_OK)
			{
				DEBUG("%d\r\n",errorstatus);
				return(errorstatus);
			}
			cardstatus = SDIO_GetResponse(SDIO_RESP1);
		}
		while (((cardstatus & 0x00000100) == 0) && (timeout > 0));
		
		if (timeout == 0)
		{
			DEBUG("%d\r\n",errorstatus);
			return(SD_ERROR);
		}
		
		timeout = SDIO_WRITETIMEOUT;
    	//����CMD25,SDIO_WRITE_MULT_BLOCK,����:�ֽڵ�ַ,�̷���,R1
	    SDIO_SendCommand(SDIO_WRITE_MULT_BLOCK,(BlockAddr << 9),SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	    errorstatus = CmdResp1Error();
	    if (errorstatus != SD_OK)
	    {
	    	DEBUG("%d\r\n",errorstatus);
	    	return(errorstatus);
	    }
	    
	    do
	    {
	    	if(SDIO0->PRNSTS & BIT10)	//д��������Ч
	    	{
	    		SDIO_ClearFlag(SDIO_FLAG_BUFFWRITE);		//���д��������Ч��־
	    		//��������СSD_BlockSize�ֽ�,��SD_BlockSize / 4��
			  	for(cnt = 0;cnt < SD_BlockSize / 4;cnt ++)
			  	{
			  		SDIO_WriteData(*BlockBuff ++);
				}
			  	BlockCnt --;	//������һ��
			  	timeout = SDIO_READTIMEOUT;	//������һ��,��ʱ���������¿�ʼ����
	    	}
	    	timeout --;
	    }while(BlockCnt && (timeout != 0) && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0));	//ʣ���������Ϊ0,û��ʱ
	    
		//����CMD12 SDIO_STOP_TRANSMISSION����,��ֹ��ȡ;����:0,����Ӧ,R1
		SDIO_SendCommand(SDIO_STOP_TRANSMISSION,0,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
		errorstatus = CmdResp1Error();
		if (errorstatus != SD_OK)
		{
			DEBUG("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		if(timeout == 0)	//��ʱ
		{
			errorstatus = SD_DATA_TIMEOUT;
			DEBUG("%d\r\n",errorstatus);
			return(errorstatus);
		}
		
		SDIO_ClearFlag(SDIO_FLAG_ALL);	//��������жϱ�־
	}
	
	//��ȡ��״̬,�ȴ�д�����
	do
	{
		errorstatus = IsCardProgramming(&cardstate);
	}while ((errorstatus == SD_OK) && ((cardstate == SD_CARD_PROGRAMMING) || (cardstate == SD_CARD_RECEIVING)));
	
	return(errorstatus);
}





/*************************************************************************************************************************
* ����	:	static SD_Error CmdError(void)
* ����	:	ָ��ִ��״̬(����Ӧ)
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	����ָ��ִ�н��(�ڲ�����)
*************************************************************************************************************************/
static SD_Error CmdError(void)
{
  SD_Error errorstatus = SD_OK;
  u32 timeout;

  timeout = SDIO_CMD0TIMEOUT;

  while ((timeout > 0) && (SDIO_GetFlagStatus(SDIO_FLAG_COMMANDEND) == 0))
  {
    timeout--;
  }

  if (timeout == 0)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    return(errorstatus);
  }

  //������б�־
  SDIO_ClearFlag(SDIO_FLAG_ALL);

  return(errorstatus);
}


/*************************************************************************************************************************
* ����	:	static SD_Error CmdResp7Error(void)
* ����	:	ָ��ִ��״̬(R7)
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	����ָ��ִ�н��(�ڲ�����)
*************************************************************************************************************************/
static SD_Error CmdResp7Error(void)
{
  SD_Error errorstatus = SD_OK;
 // u32 status;
  u32 timeout = SDIO_CMD0TIMEOUT;

  do
  {
		timeout--;
  }
	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT)) && (timeout > 0));
	
  if ((timeout == 0) || (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT))
  {
    /* Card is not V2.0 complient or card does not support the set voltage range */
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }

  if (SDIO_STATUS & SDIO_FLAG_COMMANDEND)
  {
    /* Card is SD V2.0 compliant */
    errorstatus = SD_OK;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDEND);
    return(errorstatus);
  }
  //������б�־
  SDIO_ClearFlag(SDIO_FLAG_ALL);
  
  return(errorstatus);

}


/*************************************************************************************************************************
* ����	:	static SD_Error CmdResp1Error(void)
* ����	:	ָ��ִ��״̬(R1)
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	����ָ��ִ�н��(�ڲ�����)
*************************************************************************************************************************/
static SD_Error CmdResp1Error(void)
{
  SD_Error errorstatus = SD_OK;
  u32 response_r1;
  
	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT | SDIO_FLAG_COMMANDINDEX)));
	if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
	{
	    errorstatus = SD_CMD_RSP_TIMEOUT;
	    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
	    return(errorstatus);
	}
	else if (SDIO_STATUS & SDIO_FLAG_COMMANDCRC)
 	{
	    errorstatus = SD_CMD_CRC_FAIL;
	    SDIO_ClearFlag(SDIO_FLAG_COMMANDCRC);
	    return(errorstatus);
  	}
	else if (SDIO_STATUS & SDIO_FLAG_COMMANDINDEX)
	{
	    errorstatus = SD_ILLEGAL_CMD;
	    SDIO_ClearFlag(SDIO_FLAG_COMMANDINDEX);
	    return(errorstatus);
	}

  //������б�־
  SDIO_ClearFlag(SDIO_FLAG_ALL);

  /* We have received response, retrieve it for analysis  */
  response_r1 = SDIO_GetResponse(SDIO_RESP1);
  if ((response_r1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
  {
    return(errorstatus);
  }

  if (response_r1 & SD_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SD_ADDR_OUT_OF_RANGE);
  }

  if (response_r1 & SD_OCR_ADDR_MISALIGNED)
  {
    return(SD_ADDR_MISALIGNED);
  }

  if (response_r1 & SD_OCR_BLOCK_LEN_ERR)
  {
    return(SD_BLOCK_LEN_ERR);
  }

  if (response_r1 & SD_OCR_ERASE_SEQ_ERR)
  {
    return(SD_ERASE_SEQ_ERR);
  }

  if (response_r1 & SD_OCR_BAD_ERASE_PARAM)
  {
    return(SD_BAD_ERASE_PARAM);
  }

  if (response_r1 & SD_OCR_WRITE_PROT_VIOLATION)
  {
    return(SD_WRITE_PROT_VIOLATION);
  }

  if (response_r1 & SD_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SD_LOCK_UNLOCK_FAILED);
  }

  if (response_r1 & SD_OCR_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  if (response_r1 & SD_OCR_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (response_r1 & SD_OCR_CARD_ECC_FAILED)
  {
    return(SD_CARD_ECC_FAILED);
  }

  if (response_r1 & SD_OCR_CC_ERROR)
  {
    return(SD_CC_ERROR);
  }

  if (response_r1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SD_OCR_STREAM_READ_UNDERRUN)
  {
    return(SD_STREAM_READ_UNDERRUN);
  }

  if (response_r1 & SD_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SD_STREAM_WRITE_OVERRUN);
  }

  if (response_r1 & SD_OCR_CID_CSD_OVERWRIETE)
  {
    return(SD_CID_CSD_OVERWRITE);
  }

  if (response_r1 & SD_OCR_WP_ERASE_SKIP)
  {
    return(SD_WP_ERASE_SKIP);
  }

  if (response_r1 & SD_OCR_CARD_ECC_DISABLED)
  {
    return(SD_CARD_ECC_DISABLED);
  }

  if (response_r1 & SD_OCR_ERASE_RESET)
  {
    return(SD_ERASE_RESET);
  }

  if (response_r1 & SD_OCR_AKE_SEQ_ERROR)
  {
    return(SD_AKE_SEQ_ERROR);
  }
  return(errorstatus);
}


/*************************************************************************************************************************
* ����	:	static SD_Error CmdResp3Error(void)
* ����	:	ָ��ִ��״̬(R3,OCR)
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	����ָ��ִ�н��(�ڲ�����)
*************************************************************************************************************************/
static SD_Error CmdResp3Error(void)
{
  SD_Error errorstatus = SD_OK;
//  u32 status;

	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT)));
  if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }
  //������б�־
  SDIO_ClearFlag(SDIO_FLAG_ALL);
  
  return(errorstatus);
}



/*************************************************************************************************************************
* ����	:	static SD_Error CmdResp2Error(void)
* ����	:	ָ��ִ��״̬(R2,CID or CSD)
* ����	:	��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	����ָ��ִ�н��(�ڲ�����)
*************************************************************************************************************************/
static SD_Error CmdResp2Error(void)
{
  SD_Error errorstatus = SD_OK;
//  u32 status;

	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT)));

  if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }
  else if (SDIO_STATUS & SDIO_FLAG_COMMANDCRC)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDCRC);
    return(errorstatus);
  }

  //������б�־
  SDIO_ClearFlag(SDIO_FLAG_ALL);

  return(errorstatus);
}


/*************************************************************************************************************************
* ����	:	static SD_Error CmdResp6Error(u16 *prca)
* ����	:	ָ��ִ��״̬(R6,RCA)
* ����	:	prca:RCA������ָ��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	����ָ��ִ�н��(�ڲ�����)
*************************************************************************************************************************/
static SD_Error CmdResp6Error(u16 *prca)
{
  SD_Error errorstatus = SD_OK;
 // u32 status;
  u32 response_r1;
  
  	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT | SDIO_FLAG_COMMANDINDEX)));
  if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }
  else if (SDIO_STATUS & SDIO_FLAG_COMMANDCRC)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDCRC);
    return(errorstatus);
  }
  else if (SDIO_STATUS & SDIO_FLAG_COMMANDINDEX)
  {
    errorstatus = SD_ILLEGAL_CMD;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDINDEX);
    return(errorstatus);
  }
  
  //������б�־
  SDIO_ClearFlag(SDIO_FLAG_ALL);

  /* We have received response, retrieve it.  */
  response_r1 = SDIO_GetResponse(SDIO_RESP1);

  if (SD_ALLZERO == (response_r1 & (SD_R6_GENERAL_UNKNOWN_ERROR | SD_R6_ILLEGAL_CMD | SD_R6_COM_CRC_FAILED)))
  {
    *prca = (u16) (response_r1 >> 16);
    return(errorstatus);
  }

  if (response_r1 & SD_R6_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (response_r1 & SD_R6_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (response_r1 & SD_R6_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  return(errorstatus);
}


/*************************************************************************************************************************
* ����	:	static SD_Error FindSCR(u16 rca, u32 *pscr)
* ����	:	��ȡSCR��Ϣ
* ����	:	rca:����ַ,prca:RCA������ָ��
* ����	:	SD_OK:�ɹ�,������SD Card Error code.
* ����	:	�ײ�Ĵ�����������
* ����	:	����
* ʱ��	:	20120516
* ����޸�ʱ�� : 20120516
* ˵��	: 	����ָ��ִ�н��(�ڲ�����)
*************************************************************************************************************************/
static SD_Error FindSCR(u16 rca, u32 *pscr)
{
	SD_Error errorstatus = SD_OK;
	u32 tempscr[2] = {0, 0};
	u32 timeout = SDIO_READTIMEOUT;//���ȴ���ʱ������

	//���ÿ��СΪ8�ֽ�
	//����SDIO_SET_BLOCKLEN,����8,����Ӧ
	SDIO_SendCommand(SDIO_SET_BLOCKLEN,8,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();

	if (errorstatus != SD_OK)
	{
		DEBUG("SDIO_SET_BLOCKLEN error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
		goto _error;
	}
	
	//����CMD55 SDIO_APP_CMD;�������:RCA;������ӦR1,����RCAΪ0,����Ӧ
  	SDIO_SendCommand(SDIO_APP_CMD,(u32)rca << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();
	if (errorstatus != SD_OK)
	{
		DEBUG("CMD55 error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
		goto _error;
	}
	
	SDIO_SoftwareReset(SDIO_RSTDAT);	//�����λDAT,��Ҫ�������FIFO
	SDIO_ReadMode();					//��������������Ϊ��ģʽ
	SDIO_SetTransferBlockSize(8);		//���õ��δ���Ŀ��СΪ8b
	SDIO_SingleBlockMode();				//����Ϊ���鴫��ģʽ

	//����ACMD51 SD_APP_SEND_SCR,����0,����Ӧ,R1
	SDIO_SendCommand(SDIO_SD_APP_SEND_SCR,0,SDIO_DataSelect | SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	errorstatus = CmdResp1Error();
	if (errorstatus != SD_OK)
	{
		DEBUG("ACMD51 error(%d)!\n",errorstatus);	//����,��ӡ������Ϣ
		goto _error;
	}
	
	while((SDIO_GetFlagStatus(SDIO_FLAG_BUFFREAD) == 0) && timeout && (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) == 0))	//�ȴ�����������Ч,���ߵȴ���ʱ
	{
		timeout --;
	}
	
	if (SDIO_GetFlagStatus(SDIO_FLAG_DATATIMEOUT) || (timeout == 0))	//��ʱ
    {
		SDIO_ClearFlag(SDIO_FLAG_DATATIMEOUT);	//�����־
 		errorstatus = SD_DATA_TIMEOUT;
		goto _error;
    }
	
	//��������С8�ֽ�,��2��
	tempscr[0] = SDIO_ReadData();
	tempscr[1] = SDIO_ReadData();


	//������б�־
	SDIO_ClearFlag(SDIO_FLAG_ALL);

	*(pscr + 1) = ((tempscr[0] & SD_0TO7BITS) << 24) | ((tempscr[0] & SD_8TO15BITS) << 8) | ((tempscr[0] & SD_16TO23BITS) >> 8) | ((tempscr[0] & SD_24TO31BITS) >> 24);

	*(pscr) = ((tempscr[1] & SD_0TO7BITS) << 24) | ((tempscr[1] & SD_8TO15BITS) << 8) | ((tempscr[1] & SD_16TO23BITS) >> 8) | ((tempscr[1] & SD_24TO31BITS) >> 24);

_error:
	//�ָ����СΪ SD_BlockSize
	SDIO_SetTransferBlockSize(SD_BlockSize);//���ô�����СΪ512�ֽ�
	errorstatus = SD_SetBlockSize(SD_BlockSize);//����SD�����С	
	if (errorstatus != SD_OK)
	{
		DEBUG("SD SetBlockSize error(%d)!\n",errorstatus);
		return(errorstatus);
	}
	
	return(errorstatus);
}





/*******************************************************************************
* Function Name  : IsCardProgramming
* Description    : Checks if the SD card is in programming state.
* Input          : pstatus: pointer to the variable that will contain the SD
*                  card state.
* Output         : None
* Return         : SD_Error: SD Card Error code.
*******************************************************************************/
static SD_Error IsCardProgramming(u8 *pstatus)
{
  SD_Error errorstatus = SD_OK;
  vu32 respR1 = 0, status = 0;


  //����SDIO_SEND_STATUS,����״̬
  SDIO_SendCommand(SDIO_SEND_STATUS,(u32) RCA << 16,SDIO_CMDIndexEn | SDIO_CMDCrcEn | SDIO_Response_Short);
	
	while (!(SDIO_STATUS & (SDIO_FLAG_COMMANDCRC | SDIO_FLAG_COMMANDEND | SDIO_FLAG_COMMANDTIMEOUT | SDIO_FLAG_COMMANDINDEX)));
  if (SDIO_STATUS & SDIO_FLAG_COMMANDTIMEOUT)
  {
    errorstatus = SD_CMD_RSP_TIMEOUT;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDTIMEOUT);
    return(errorstatus);
  }
  else if (SDIO_STATUS & SDIO_FLAG_COMMANDCRC)
  {
    errorstatus = SD_CMD_CRC_FAIL;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDCRC);
    return(errorstatus);
  }
  else if (SDIO_STATUS & SDIO_FLAG_COMMANDINDEX)
  {
    errorstatus = SD_ILLEGAL_CMD;
    SDIO_ClearFlag(SDIO_FLAG_COMMANDINDEX);
    return(errorstatus);
  }

	//������б�־
  SDIO_ClearFlag(SDIO_FLAG_ALL);


  /* We have received response, retrieve it for analysis  */
  respR1 = SDIO_GetResponse(SDIO_RESP1);

  /* Find out card status */
  *pstatus = (u8) ((respR1 >> 9) & 0x0000000F);

  if ((respR1 & SD_OCR_ERRORBITS) == SD_ALLZERO)
  {
    return(errorstatus);
  }

  if (respR1 & SD_OCR_ADDR_OUT_OF_RANGE)
  {
    return(SD_ADDR_OUT_OF_RANGE);
  }

  if (respR1 & SD_OCR_ADDR_MISALIGNED)
  {
    return(SD_ADDR_MISALIGNED);
  }

  if (respR1 & SD_OCR_BLOCK_LEN_ERR)
  {
    return(SD_BLOCK_LEN_ERR);
  }

  if (respR1 & SD_OCR_ERASE_SEQ_ERR)
  {
    return(SD_ERASE_SEQ_ERR);
  }

  if (respR1 & SD_OCR_BAD_ERASE_PARAM)
  {
    return(SD_BAD_ERASE_PARAM);
  }

  if (respR1 & SD_OCR_WRITE_PROT_VIOLATION)
  {
    return(SD_WRITE_PROT_VIOLATION);
  }

  if (respR1 & SD_OCR_LOCK_UNLOCK_FAILED)
  {
    return(SD_LOCK_UNLOCK_FAILED);
  }

  if (respR1 & SD_OCR_COM_CRC_FAILED)
  {
    return(SD_COM_CRC_FAILED);
  }

  if (respR1 & SD_OCR_ILLEGAL_CMD)
  {
    return(SD_ILLEGAL_CMD);
  }

  if (respR1 & SD_OCR_CARD_ECC_FAILED)
  {
    return(SD_CARD_ECC_FAILED);
  }

  if (respR1 & SD_OCR_CC_ERROR)
  {
    return(SD_CC_ERROR);
  }

  if (respR1 & SD_OCR_GENERAL_UNKNOWN_ERROR)
  {
    return(SD_GENERAL_UNKNOWN_ERROR);
  }

  if (respR1 & SD_OCR_STREAM_READ_UNDERRUN)
  {
    return(SD_STREAM_READ_UNDERRUN);
  }

  if (respR1 & SD_OCR_STREAM_WRITE_OVERRUN)
  {
    return(SD_STREAM_WRITE_OVERRUN);
  }

  if (respR1 & SD_OCR_CID_CSD_OVERWRIETE)
  {
    return(SD_CID_CSD_OVERWRITE);
  }

  if (respR1 & SD_OCR_WP_ERASE_SKIP)
  {
    return(SD_WP_ERASE_SKIP);
  }

  if (respR1 & SD_OCR_CARD_ECC_DISABLED)
  {
    return(SD_CARD_ECC_DISABLED);
  }

  if (respR1 & SD_OCR_ERASE_RESET)
  {
    return(SD_ERASE_RESET);
  }

  if (respR1 & SD_OCR_AKE_SEQ_ERROR)
  {
    return(SD_AKE_SEQ_ERROR);
  }

  return(errorstatus);
}



//SD���жϷ������,������⿨�Ĳ������Ƴ���
static void __irq Isr_SDMMC_Card(void)
{
	if(SDIO_GetFlagStatus(SDIO_FLAG_CARDREMOVAL))	//���Ƴ���
	{
	//	DEBUG("SDMMC Card MOVAL!\n");			//����,��ӡ������Ϣ
		SDIO_ClearFlag(SDIO_FLAG_CARDREMOVAL);		//������Ƴ��ж�
		SDMMC0_MOV_Flag = 1;	//���Ƴ���
	}
	if(SDIO_GetFlagStatus(SDIO_FLAG_CARDINSERTION))	//��������
	{
	//	DEBUG("SDMMC Card IN!\n");			//����,��ӡ������Ϣ
		SDIO_ClearFlag(SDIO_FLAG_CARDINSERTION);	//����������ж�
	}
	VICInterruptEnd();	//�жϽ���
}








