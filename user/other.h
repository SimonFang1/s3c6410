/*************************************************************************************************************
 * �ļ���:		OTHER.h
 * ����:		S3C6410 ��غ���,���綨ʱ��,LED��
 * ����:		����
 * ����ʱ��:	2012��5��20��19:19
 * ����޸�ʱ��:2012��5��26��
 * ��ϸ:		��:
 				��ʼ��LED
 				4��LED GPION 0 - 4
*************************************************************************************************************/
#ifndef _OTHER_H_
#define _OTHER_H_

#include "system.h"

void LED_Init(void);	//��ʼ��LED


//LED0 PN0
#define LED0_ON()		(rGPMDAT &= ~(1 << 0))	//LED0��
#define LED0_OFF()		(rGPMDAT |= (1 << 0))	//LED0��
#define LED0_FLASH()	(rGPMDAT ^= (1 << 0))	//LED0��˸
//LED1 PN1
#define LED1_ON()		(rGPMDAT &= ~(1 << 1))	//LED0��
#define LED1_OFF()		(rGPMDAT |= (1 << 1))	//LED0��
#define LED1_FLASH()	(rGPMDAT ^= (1 << 1))	//LED0��˸
//LED2 PN2
#define LED2_ON()		(rGPMDAT &= ~(1 << 2))	//LED0��
#define LED2_OFF()		(rGPMDAT |= (1 << 2))	//LED0��
#define LED2_FLASH()	(rGPMDAT ^= (1 << 2))	//LED0��˸
//LED3 PN3
#define LED3_ON()		(rGPMDAT &= ~(1 << 3))	//LED0��
#define LED3_OFF()		(rGPMDAT |= (1 << 3))	//LED0��
#define LED3_FLASH()	(rGPMDAT ^= (1 << 3))	//LED0��˸


#endif
