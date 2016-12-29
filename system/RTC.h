/*************************************************************************************************************
 * 文件名:	RTC.H
 * 功能:		S3C6410 RTC底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年3月28日21:52
 * 最后修改时间:2012年3月28日
 * 详细:		RTC相关底层驱动函数
*************************************************************************************************************/
#ifndef _RTC_H_
#define _RTC_H_


 //时间结构体
typedef struct 
{
	u8 hour; 	//小时
	u8 min;	 	//分钟
	u8 sec;		//秒			
	//公历日月年周
	u16 w_year;	 	//年
	u8  w_month;	//月
	u8  w_date;		//日
	u8  week;		//星期
#if _Lunar_calendar
	//农历日月年节气
	u8 l_year;		//年
	u8 l_month;		//月
	u8 l_data;		//日
	u8 SolarTerms;	//农历节气 
#endif
}Time_TypeDef;

extern Time_TypeDef Timer;//存放全局时间

void UpdateTimer(void);	//更新全部时间
void RTC_Init(FunctionalState EnableInt);	//初始化RTC
void InstallData(u16 Year,u8 Month,u8 Date,u8 Week);	//设置日期
void InstallTime(u8 Hour,u8 Min,u8 Sec);				//设置时间
bool RTC_SecEnd(void);							//判断秒是否到达



#endif
