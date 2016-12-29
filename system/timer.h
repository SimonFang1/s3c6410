/*************************************************************************************************************
 * 文件名:	timer.h
 * 功能:		S3C6410 timer底层驱动函数
 * 作者:		cp1300@139.com
 * 创建时间:	2012年9月17日20:32
 * 最后修改时间:2012年9月17日
 * 详细:		定时器的初始化以及中断服务程序
*************************************************************************************************************/
#ifndef TIMER_H_
#define TIMER_H_


void Timer0_Init(u32 RTime,FunctionalState EnInt,void (*TimerIsr)(void));	//定时器0初始化函数
void Timer1_Init(u32 RTime,FunctionalState EnInt,void (*TimerIsr)(void));	//定时器1初始化函数



#endif /*TIMER_H_*/
