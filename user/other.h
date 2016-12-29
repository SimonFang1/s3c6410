/*************************************************************************************************************
 * 文件名:		OTHER.h
 * 功能:		S3C6410 相关函数,比如定时器,LED等
 * 作者:		陈鹏
 * 创建时间:	2012年5月20日19:19
 * 最后修改时间:2012年5月26日
 * 详细:		如:
 				初始化LED
 				4个LED GPION 0 - 4
*************************************************************************************************************/
#ifndef _OTHER_H_
#define _OTHER_H_

#include "system.h"

void LED_Init(void);	//初始化LED


//LED0 PN0
#define LED0_ON()		(rGPMDAT &= ~(1 << 0))	//LED0亮
#define LED0_OFF()		(rGPMDAT |= (1 << 0))	//LED0灭
#define LED0_FLASH()	(rGPMDAT ^= (1 << 0))	//LED0闪烁
//LED1 PN1
#define LED1_ON()		(rGPMDAT &= ~(1 << 1))	//LED0亮
#define LED1_OFF()		(rGPMDAT |= (1 << 1))	//LED0灭
#define LED1_FLASH()	(rGPMDAT ^= (1 << 1))	//LED0闪烁
//LED2 PN2
#define LED2_ON()		(rGPMDAT &= ~(1 << 2))	//LED0亮
#define LED2_OFF()		(rGPMDAT |= (1 << 2))	//LED0灭
#define LED2_FLASH()	(rGPMDAT ^= (1 << 2))	//LED0闪烁
//LED3 PN3
#define LED3_ON()		(rGPMDAT &= ~(1 << 3))	//LED0亮
#define LED3_OFF()		(rGPMDAT |= (1 << 3))	//LED0灭
#define LED3_FLASH()	(rGPMDAT ^= (1 << 3))	//LED0闪烁


#endif
