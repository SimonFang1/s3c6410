/*************************************************************************************************************
 * �ļ���:	RTC.H
 * ����:		S3C6410 RTC�ײ���������
 * ����:		����
 * ����ʱ��:	2012��3��28��21:52
 * ����޸�ʱ��:2012��3��28��
 * ��ϸ:		RTC��صײ���������
*************************************************************************************************************/
#ifndef _RTC_H_
#define _RTC_H_


 //ʱ��ṹ��
typedef struct 
{
	u8 hour; 	//Сʱ
	u8 min;	 	//����
	u8 sec;		//��			
	//������������
	u16 w_year;	 	//��
	u8  w_month;	//��
	u8  w_date;		//��
	u8  week;		//����
#if _Lunar_calendar
	//ũ�����������
	u8 l_year;		//��
	u8 l_month;		//��
	u8 l_data;		//��
	u8 SolarTerms;	//ũ������ 
#endif
}Time_TypeDef;

extern Time_TypeDef Timer;//���ȫ��ʱ��

void UpdateTimer(void);	//����ȫ��ʱ��
void RTC_Init(FunctionalState EnableInt);	//��ʼ��RTC
void InstallData(u16 Year,u8 Month,u8 Date,u8 Week);	//��������
void InstallTime(u8 Hour,u8 Min,u8 Sec);				//����ʱ��
bool RTC_SecEnd(void);							//�ж����Ƿ񵽴�



#endif
