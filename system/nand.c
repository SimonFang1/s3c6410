/*************************************************************************************************************
 * �ļ���:	Nand.c
 * ����:		S3C6410 NandFlash�ײ���������
 * ����:		����
 * ����ʱ��:	2012��3��31��21:34
 * ����޸�ʱ��:2012��3��31��
 * ��ϸ:		NandFlash�ײ���������,
 				����NAND FLASH��Ϣ:2GB,MLC K9G4G08(K9GAG08U0D,ҳ��С4KB,4bit����)
*************************************************************************************************************/
#include "system.h"
#include "Nand.h"

//����
//CONF
#define ECCType		1	//ECC����ѡ��,0:SLC(1λ����);1:MLC(4λ����)
#define TACLS		5	//CLE & ALE����ʱ��(0-7)(= HCLK * TACLS)
#define TWRPH0		5	//TWRPH0����ʱ��(0-7)(= HCLK * (TWRPH0 + 1))
#define TWRPH1		5	//TWRPH1����ʱ��(0-7)(= HCLK * (TWRPH1 + 1))
#define AdvFlash	1	//Ԥ��NAND flash �洢������;0:֧��512�ֽ�/ҳ;1:֧��2KB/ҳ
#define AddrCycle	1	//NAND Flash�洢����ַ����,0: 0:3��ַ����,1:4��ַ����;1: 0:4��ַ����,1:5��ַ����
//CONT
#define SoftLock	0	//���������,0:������,1:ʹ����
#define MainECCLock	1	//��������ECC����:0:��������ECC;1:��������ECC
#define SpareECCLock	1	//���汸����ECC����;0:����������ECC,1:���汸����ECC,������ECC ״̬�Ĵ�����NFSECC(0x7020003C)
#define RegNCE1		1	//NAND Flash �洢��nGCS[3]�źſ��ƣ�0��ǿ��nGCS[3]Ϊ�ͣ�ʹ��Ƭѡ��;1��ǿ��nGCS[3]Ϊ�ߣ�����Ƭѡ��ע����ʹReg_nCE1 �� Reg_nCE0 ͬʱ������Ϊ0������֮��Ҳֻ��һ��������
#define MODE		1	//NAND Flash ����������ģʽ��0��NAND Flash ���������ã���������1��NAND Flash ������ʹ��

//NAND FLASH������
#define NANDCMD(cmd)	(NAND->CMMD = (cmd))	//��NAND flashд������
#define NANDADDR(addr)	(NAND->ADDR = (addr))	//��NAND flashд���ַ
#define NANDDATA		(NAND->DATA)			//��NAND flash��д����
#define NF_nCS3_L		(NAND->CONT&=~(1<<2))
#define NF_nCS3_H		(NAND->CONT|=(1<<2))
#define NF_nCS2_L		(NAND->CONT&=~(1<<1))		//Ƭѡ
#define NF_nCS2_H		(NAND->CONT|=(1<<1))
#define NF_RnB			(NAND->STAT & BIT0)			//0:�洢��æ,1:����

//NANDƬѡ
#define NAND_CS_L		NF_nCS2_L	//NANDƬѡ��Ч
#define NAND_CS_H		NF_nCS2_H	//NANDƬѡ��Ч

//NAND FLASH��Ϣ��
#define FLASH_MAX_ADDR 		0x80000000	//FLASH����ܹ��ﵽ�ĵ�ַ����2GB
#define FLASH_BLOCK_SIZE 	0x20000		//FLASH���С��Ϊ512KB
#define FLASH_PAGE_SIZE 	0x1000		//FLASHҳ��С��Ϊ4KB
//ECC 8BIT 512B


//NAND FLASH	�����
#define NAND_READ_1th			0x00				//������������һ����������
#define NAND_READ_2th			0x30				//�����������ڶ�����������	
#define NAND_READ_ID			0x90				//��NAND ID
#define NAND_READ_STATUS1		0x70				//��״̬1
#define NAND_READ_STATUS2		0xf1				//��״̬2
#define NAND_RESET				0xff				//��λ


//�ȴ���ʱ����
#define NAND_TIME_OUT_CNT	0xffffff


/*************************************************************************************************************************
*����        :	bool NnadFlashWait(void)
*����        :	�ȴ��������
*����        :	��
*����        :	FALSE:��ʱ;TRUE:�ɹ�
*����	: 	��
*����        :	����
*ʱ��        :	20120331
*����޸�ʱ��:	20120331
*˵��        :	�͵�ƽ����æ
*************************************************************************************************************************/
bool NnadFlashWait(void)
{
	u32 TimeCnt = NAND_TIME_OUT_CNT;
	while(NF_RnB == 0 && TimeCnt != 0)  //�ȴ�д���  Ϊ�ͱ�ʾæ
		TimeCnt --;
	if(TimeCnt == 0)
		return FALSE;
	else
		return TRUE;
}


/*************************************************************************************************************************
*����	:	static void NandFlashWrite5BitAddr(vu32 Addr)
*����	:	��NAND FLASHд��4�ֽڵĵ�ַ
*����	:	��ַ
*����	:	��
*����	: 	��
*����	:	����
*ʱ��		:	20120331
*����޸�ʱ��	:	20120331
*˵��	:	дNAND��ַ,��ַ��32bit,��5��д��
*************************************************************************************************************************/
static void NandFlashWrite5BitAddr(vu32 Addr)
{
	NANDADDR(Addr & 0xff);			//д��A0-A7;
	NANDADDR((Addr >> 8) & 0x1f);	//дA8-A12,0,0,0
	NANDADDR((Addr >> 13) & 0xff);	//дA13-A20			
	NANDADDR((Addr >> 21) & 0xff);	//дA21-A28
	NANDADDR((Addr >> 29) & 0x07);	//дA29-A31,0,0,0,0,0
}



/*************************************************************************************************************************
*����        :	bool NandFlashReset(void)
*����        :	NandFlash��λ
*����        :	��
*����        :	FALSE:��ʱ;TRUE:�ɹ�
*����	: 	��
*����        :	����
*ʱ��        :	20120331
*����޸�ʱ��:		20120331
*˵��        :	д�븴λ����ȴ���λ���
*************************************************************************************************************************/
bool NandFlashReset(void)
{
	NANDADDR(0x00);			//��ַ��λ
	NANDCMD(NAND_RESET);	//д�븴λ����
	return NnadFlashWait();		//�ȴ��������
}



/*************************************************************************************************************************
*����        :	bool NandFlashInit(void)
*����        :	NandFlash��ʼ������
*����        :	��
*����        :	FALSE:��ʱ;TRUE:�ɹ�
*����        : 	��
*����        :	����
*ʱ��        :	20120331
*����޸�ʱ��:	20120331
*˵��        :	���ڳ�ʼ��NAND
*************************************************************************************************************************/
bool NandFlashInit(void)
{
	NAND->CONF = (ECCType << 24) + (TACLS << 12) + (TWRPH0 << 8) + (TWRPH1 << 4) + (AdvFlash << 3) + BIT2 + (AddrCycle << 1);	//����NAND FLASH
	NAND->CONT = (SoftLock << 16) + (MainECCLock << 7) + (SpareECCLock << 6) + (RegNCE1 << 2) + (MODE << 0);//��ʼ�����ƼĴ���
	return NandFlashReset();	//��λNAND FLASH
}


/*************************************************************************************************************************
*����        :	u32 NandFlashReadStatus(void)
*����        :	��ȡNandFlash״̬
*����        :	��
*����        :	״̬����
*����        : 	��
*����        :	����
*ʱ��        :	20120331
*����޸�ʱ��:	20120331
*˵��        :	��ȡ״̬
*************************************************************************************************************************/
u32 NandFlashReadStatus(void)
{
	NAND_CS_L;
	NANDCMD(NAND_READ_STATUS1);		//д��ѯ����
	NnadFlashWait();	//�ȴ��������
	NAND_CS_H;
	return NANDDATA;	//����״̬��Ϣ
}


/*************************************************************************************************************************
*����        :	u32 NandFlashReadStatus1(void)
*����        :	��ȡNandFlash״̬1
*����        :	��
*����        :	״̬����
*����        : 	��
*����        :	����
*ʱ��        :	20120331
*����޸�ʱ��:	20120331
*˵��        :	��ȡ״̬1
*************************************************************************************************************************/
u32 NandFlashReadStatus1(void)
{
	NAND_CS_L;
	NANDCMD(NAND_READ_STATUS2);		//д��ѯ����1
	NnadFlashWait();	//�ȴ��������
	NAND_CS_H;
	return NANDDATA;	//����״̬��Ϣ
}


/*************************************************************************************************************************
*����        :	void NandFlashReadID(u8 *pbuff)
*����        :	��ȡNandFlash ID
*����        :	������ָ��
*����        :	��
*����        : 	��
*����        :	����
*ʱ��        :	20120331
*����޸�ʱ��:	20120720
*˵��        :	��ȡID,ע��IDΪ6�ֽ�,ECH,DeviceCode,3rd cyc,4th cyc,5th cyc,6th cyc,��һ����32BIT,4	�ֽ�
*************************************************************************************************************************/
void NandFlashReadID(u8 *pbuff)
{
	u8 i;
	u32 buff[2];
	u8 *p = (u8 *)buff;
	
	NAND_CS_L;
	NANDCMD(NAND_READ_ID);	//д��ȡID����	
	NANDADDR(0x00);			//��ַ��λ
	NnadFlashWait();		//�ȴ��������
	//��ȡ2��,��8�ֽ�
	for(i = 0;i < 2;i ++)	
		buff[i] = NANDDATA;
	NAND_CS_H;
	
	//����ǰ���6�ֽ�ID
	for(i = 0;i < 6;i ++)
		pbuff[i] = p[i];
}


/*************************************************************************************************************************
*����        :	bool NandFlashReadOneSector(u32 Addr,u8 *pBuff)
*����        :	��ȡnand flashһҳ
*����        :	Addr:ҳ��ַ,pBuff:������ָ��;
*����        :	FALSE:����;TRUE:�ɹ�
*����     : �ײ��������
*����        :	����
*ʱ��        :	20120401
*����޸�ʱ��:	20120401
*˵��        :	��ȡһҳ,FLASH_PAGE_SIZE,ע�⻺������ҪԽ��,��һ����32BIT,4	�ֽ�
*************************************************************************************************************************/
bool NandFlashReadOneSector(u32 Addr,u8 *pBuff)
{
	u32 i;
	u32 *p = (u32 *)pBuff;
	u32 cnt = FLASH_PAGE_SIZE / 4;	//��ȡ����,ÿ�ζ�ȡ4�ֽ�
	
	if(Addr > FLASH_MAX_ADDR)	//��ַ������Χ��,���ش������1,��ַԽ��
		return FALSE;
	NAND_CS_L;
	//�����ҳ�ģ���д����������
	NANDCMD(NAND_READ_1th);
	NandFlashWrite5BitAddr(Addr);
	NANDCMD(NAND_READ_2th);
	if(NnadFlashWait() == FALSE) 	//�ȴ����ݶ���
	{	
		NAND_CS_H;			//��ȡʧ����,�ر�Ƭѡ
		NandFlashReset();	//��λNAND	
		return FALSE;		//�ȴ���ʱ
	}
	for(i = 0;i < cnt;i++)
	{
		p[i] = NANDDATA;  //��һ�ֽ�����
	}
	NAND_CS_H;
	return TRUE;
}


/*************************************************************************************************************************
*����	:	u8 NandFlashReadOneSector(u32 Addr,u8 *pBuff)
*����	:	��ȡnand flash������չ��
*����	:	Addr:ҳ��ַ,pBuff:������ָ��;
*����	:	״̬
*����	: 	�ײ��������
*����	:	����
*ʱ��        :	20120401
*����޸�ʱ��:	20120401
*˵��        :	��ȡһҳ,һҳ4KB,ע�⻺������ҪԽ��,��һ����32BIT,4	�ֽ�
*************************************************************************************************************************/
//bool NandFlashReadOne()
//{
	
//}








