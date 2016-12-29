#ifndef _SDIO_H_
#define _SDIO_H_

#include "system.h"



#define FIFO_A0	0	//FIFO�жϵ�ַ0
#define FIFO_A1	1	//FIFO�жϵ�ַ1
#define FIFO_A2	2	//FIFO�жϵ�ַ2
#define FIFO_A3	3	//FIFO�жϵ�ַ3


//SDIO���߿������
#define SDIO_BusWide_1b                     ((u8)0)
#define SDIO_BusWide_4b                     ((u8)1)
#define SDIO_BusWide_8b                     ((u8)2)

//SDIO ������������
//SDIO��Ӧ����
#define SDIO_Response_No                    ((u8)0)	//��Ӧ��
#define SDIO_Response_Long                  ((u8)1)	//��Ӧ��	136
#define SDIO_Response_Short                 ((u8)2)	//��Ӧ��	48
#define SDIO_Response_ShortBusy             ((u8)3)	//��Ӧ��+��ⷱæ���
//��������
#define SDIO_CMDIndexEn                     ((u8)1 << 4)//SDIOָ������ʹ��
#define SDIO_CMDCrcEn                       ((u8)1 << 3)//SDIOָ��CRCУ��ʹ��
#define SDIO_DataSelect						((u8)1 << 5)//SDIO��ǰ����ѡ��
//SDIOָ������CmdTyp
#define SDIO_Type_Default					((u8)0 << 6)//һ��Ϊ0
#define SDIO_Type_Pend                     	((u8)1 << 6)//д������ͣ
#define SDIO_Type_FS						((u8)2 << 6)//����ѡ��
#define SDIO_Type_IT					  	((u8)3 << 6)//�ж�ģʽ



//SDIO��Ӧ�Ĵ���ѡ��
#define SDIO_RESP1                          ((u8)0)
#define SDIO_RESP2                          ((u8)1)
#define SDIO_RESP3                          ((u8)2)
#define SDIO_RESP4                          ((u8)3)





/* SDIO Data Block Size ------------------------------------------------------*/
#define SDIO_DataBlockSize_1b               ((u16)1)
#define SDIO_DataBlockSize_2b               ((u16)2)
#define SDIO_DataBlockSize_4b               ((u16)3)
#define SDIO_DataBlockSize_8b               ((u16)4)
#define SDIO_DataBlockSize_16b              ((u16)16)
#define SDIO_DataBlockSize_32b              ((u16)32)
#define SDIO_DataBlockSize_64b              ((u16)64)
#define SDIO_DataBlockSize_128b             ((u16)128)
#define SDIO_DataBlockSize_256b             ((u16)256)
#define SDIO_DataBlockSize_512b             ((u16)512)
#define SDIO_DataBlockSize_1024b            ((u16)1024)
#define SDIO_DataBlockSize_2048b            ((u16)2048)
#define SDIO_DataBlockSize_4096b            ((u16)4096)
#define SDIO_DataBlockSize_8192b            ((u16)8192)
#define SDIO_DataBlockSize_16384b           ((u16)16384)


/* SDIO Flags ----------------------------------------------------------------*/
//SDIO�ж�״̬
#define SDIO_STATUS							(SDIO0->INTSTS)
#define SDIO_FLAG_FIFOADDERR3				((u32)1 << 14)		//FIFO SD ��ַָʾ���ж�3 ״̬ (RW1C)
#define SDIO_FLAG_FIFOADDERR2				((u32)1 << 13)		//FIFO SD ��ַָʾ���ж�2 ״̬ (RW1C)
#define SDIO_FLAG_FIFOADDERR1				((u32)1 << 12)		//FIFO SD ��ַָʾ���ж�1 ״̬ (RW1C)
#define SDIO_FLAG_FIFOADDERR0				((u32)1 << 11)		//FIFO SD ��ַָʾ���ж�0 ״̬ (RW1C)
#define SDIO_FLAG_READWAIT					((u32)1 << 10)		//���ȴ��ж�״̬ (RW1C)��
#define SDIO_FLAG_CCS						((u32)1 << 9)		//CCS �ж�״̬ (RW1C)��
#define SDIO_FLAG_CARD						((u32)1 << 8)		//�� �жϡ�
#define SDIO_FLAG_CARDREMOVAL				((u32)1 << 7)		//���Ƴ�
#define SDIO_FLAG_CARDINSERTION				((u32)1 << 6)		//������
#define SDIO_FLAG_BUFFREAD					((u32)1 << 5)		//������������
#define SDIO_FLAG_BUFFWRITE					((u32)1 << 4)		//д����������
#define SDIO_FLAG_DMA						((u32)1 << 3)		//DMA
#define SDIO_FLAG_BLOCKGAP					((u32)1 << 2)		//�����¼���
#define SDIO_FLAG_TRANSFER					((u32)1 << 1)		//������ɡ���ʱ��������
#define SDIO_FLAG_COMMANDEND				((u32)1 << 0)		//ָ����ɡ���ʱ��������
#define SDIO_FLAG_ADMA						((u32)1 << 25)		//DMA����
#define SDIO_FLAG_AUTOCMD12					((u32)1 << 24)		//�Զ�CMD12����
#define SDIO_FLAG_DATAENDBIT				((u32)1 << 22)		//�������λ����
#define SDIO_FLAG_DATACRC					((u32)1 << 21)		//����CRC����
#define SDIO_FLAG_DATATIMEOUT				((u32)1 << 20)		//���ݳ�ʱ����
#define SDIO_FLAG_COMMANDINDEX				((u32)1 << 19)		//������������
#define SDIO_FLAG_COMMANDENDBIT				((u32)1 << 18)		//ָ�����λ����
#define SDIO_FLAG_COMMANDCRC				((u32)1 << 17)		//ָ��CRC����
#define SDIO_FLAG_COMMANDTIMEOUT			((u32)1 << 16)		//ָ�ʱ����
#define SDIO_FLAG_ALL						((u32)0xffffffff)	//���б�־







void SDIO_DeInit(void);
void SDIO_ClockCmd(u8 EN);
void SDIO_SetPowerState(u32 SDIO_PowerState);
void SDIO_FlagITConfig(u32 SDIO_FLAG, u8 EN);
void SDIO_FlagConfig(u32 SDIO_FLAG, u8 EN);
void SDIO_SendCommand(vu8 CmdIdx,vu32 CmdArg,vu8 CmdMisc);
u32 SDIO_GetResponse(u8 SDIO_RESP);
u32 SDIO_ReadData(void);
void SDIO_WriteData(u32 Data);
u8 SDIO_GetFlagStatus(u32 SDIO_FLAG);
void SDIO_ClearFlag(u32 SDIO_FLAG);
void SDIO_SetFIFOInterruptAddress(u8 FIFOxAdd,u8 WordAdd);
void SDIO_SetTimeOutClock(u8 Unit,u8 Frequency);
void SDIO_CLKFrequencySelect(u8 SDCLK);
void SDIO_SoftwareReset(u32 SDIO_RST);//�����λ


//������Ҫ����Ŀ�����
#define SDIO_SetTransferBlockCnt(x)					(SDIO0->BLKCNT = x)
//��ȡʣ�ഫ��Ŀ������
#define SDIO_GetTransferBlockCnt()					(SDIO0->BLKCNT)
//���õ��δ���Ŀ�Ĵ�С
#define SDIO_SetTransferBlockSize(x)				(SDIO0->BLKSIZE = (x & 0xfff))
//����Ϊ���鴫��ģʽ
#define SDIO_SingleBlockMode()						(SDIO0->TRNMOD &= ~(BIT5))
//����Ϊ�����δ���ģʽ
#define SDIO_MultipleBlockMode()					(SDIO0->TRNMOD |= BIT5)
//�Զ�CMD12��������
#define SDIO_AutoCMD12Enable()						(SDIO0->TRNMOD |= BIT2)
//�Զ�CMD12�����ֹ
#define SDIO_AuotCMD12Disable()						(SDIO0->TRNMOD &= ~(BIT2))
//����SDIOΪд����ģʽ
#define SDIO_WriteMode()							(SDIO0->TRNMOD &= ~(BIT4))
//����SDIOΪ������ģʽ
#define SDIO_ReadMode()								(SDIO0->TRNMOD |= BIT4)
//�����������
#define SDIO_BlockCountEnable()						(SDIO0->TRNMOD |= BIT1)
//��ֹ�����������
#define SDIO_BlockCountDisable()					(SDIO0->TRNMOD &= ~(BIT1))
//DMA����
#define SDIO_DMAEnable()							(SDIO0->TRNMOD |= BIT0)
//DMA��ֹ	
#define SDIO_DMADisable()							(SDIO0->TRNMOD &= ~(BIT0))
//������æ
#define SDIO_DATLineActive()						(SDIO0->PRNSTS & BIT2)
//ָ���ֹ
#define SDIO_CommandInhibit()						(SDIO0->PRNSTS & BIT0)
//����ʱ��ģʽʹ��
#define SDIO_HighSpeedEnable()						(SDIO0->CAPAREG |= BIT21)
//ȡ������ʱ��ģʽ
#define SDIO_HighSpeedDisable()						(SDIO0->CAPAREG &= ~BIT21)
//�����߿������Ϊ4bit
#define SDIO_SetDataDataWidth_4b()					(SDIO0->HOSTCTL |= BIT1)
//�����߿�����Ϊ1bit
#define SDIO_SetDataDataWidth_1b()					(SDIO0->HOSTCTL &= ~BIT1)
//���ߵ�ѹѡ��3.3V
#define SDIO_SetBusVoltage_3_3V()					(SDIO0->PWRCON &= ~(0x7 << 1); SDIO0->PWRCON |= (7 << 1))
//���ߵ�Դ����
#define SDIO_BusPowerON()							(SDIO0->PWRCON |= BIT0)
//���ߵ�ѹ�ر�
#define SDIO_BusPowerOFF()							(SDIO0->PWRCON &= ~BIT0)
//���ѷ�����SD������
#define SDIO_WakeupOnSDInsertion()					(SDIO0->WAKCON |= BIT1
//ʹ��SDʱ��
#define SDIO_SDClkEnable()							(SDIO0->CLKCON |= BIT2)
//SDʱ��ֹͣ	
#define SDIO_SDClkDisable()							(SDIO0->CLKCON &= ~BIT2)				

//��ʱʱ�ӵ�λ����
#define TIME_OUT_UNIT_KHZ	0	//��ʱʱ�ӵ�λΪKHZ
#define TIME_OUT_UNIT_MHZ	1	//��ʱʱ�ӵ�λΪMHZ


//ʱ�ӷ�Ƶϵ��
#define SDCLK_1_256 	((u8)0x80)	//����ʱ��256��Ƶ
#define SDCLK_1_128 	((u8)0x40)	//����ʱ��128��Ƶ
#define SDCLK_1_64 	((u8)0x20)	//����ʱ��64��Ƶ
#define SDCLK_1_32	((u8)0x10)	//����ʱ��32��Ƶ
#define SDCLK_1_16 	((u8)0x08)	//����ʱ��16��Ƶ
#define SDCLK_1_8 	((u8)0x04)	//����ʱ��8��Ƶ
#define SDCLK_1_4 	((u8)0x02)	//����ʱ��4��Ƶ
#define SDCLK_1_2 	((u8)0x01)	//����ʱ��2��Ƶ
#define SDCLK_1_1		((u8)0x00)	//����ʱ��1��Ƶ

//�����λѡ��
#define SDIO_RSTDAT		BIT2	//��λDAT��
#define SDIO_RSTCMD		BIT1	//��λCMD��
#define SDIO_RSTALL		BIT0	//��λ����


#endif
