/*************************************************************************************************************
 * �ļ���:	Nand.h
 * ����:		S3C6410 NandFlash�ײ���������
 * ����:		����
 * ����ʱ��:	2012��3��31��21:34
 * ����޸�ʱ��:2012��3��31��
 * ��ϸ:		NandFlash�ײ���������
*************************************************************************************************************/
#ifndef _NAND_H_
#define _NAND_H_

#include "system.h"

bool NandFlashInit(void);			//NandFlash��ʼ������
u32 NandFlashReadStatus(void);		//��ȡNandFlash״̬
u32 NandFlashReadStatus1(void);		//��ȡNandFlash״̬
void NandFlashReadID(u8 *pbuff);	//��ȡNandFlash ID
bool NandFlashReadOneSector(u32 Addr,u8 *pBuff);	//��ȡnand flashһҳ


#endif



