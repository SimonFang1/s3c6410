/*************************************************************************************************************
 * 文件名:		uart.h
 * 功能:		S3C6410 UART底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年3月4日16:33
 * 最后修改时间:2012年3月4日
 * 详细:		串口初始化,发送,接收,配置等
*************************************************************************************************************/

#ifndef	_UART_H_
#define _UART_H_

#include "sys_types.h"

extern u8 New_UART0_Rx;				//BIT0代表有新数据,BIT7代表数据帧接收完毕
#define UART0_NewData	URXH0	//UART0接收到的新数据

//UART0
void UART0_Init(FunctionalState EnableIntnt,vu32 UART_Speed);//初始化串口0
void UART0_SendByte(vu8 data);	//UART0发送一个字节


#endif

