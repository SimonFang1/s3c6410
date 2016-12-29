/*************************************************************************************************************
 * 文件名:	Nand.h
 * 功能:		S3C6410 NandFlash底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年3月31日21:34
 * 最后修改时间:2012年3月31日
 * 详细:		NandFlash底层驱动函数
*************************************************************************************************************/
#ifndef _NAND_H_
#define _NAND_H_

#include "system.h"

bool NandFlashInit(void);			//NandFlash初始化函数
u32 NandFlashReadStatus(void);		//读取NandFlash状态
u32 NandFlashReadStatus1(void);		//读取NandFlash状态
void NandFlashReadID(u8 *pbuff);	//读取NandFlash ID
bool NandFlashReadOneSector(u32 Addr,u8 *pBuff);	//读取nand flash一页


#endif



