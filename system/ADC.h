/*************************************************************************************************************
 * �ļ���:	ADC.h
 * ����:		S3C6410 ������ADC�ײ���������
 * ����:		����
 * ����ʱ��:	2012��3��12��21:05
 * ����޸�ʱ��:2012��3��12��
 * ��ϸ:		�����������Լ����ADC����
*************************************************************************************************************/

#ifndef	_ADC_H_
#define _ADC_H_

//����״̬	 
#define TOUCH_DOWN 0
#define TOUCH_UP   1
#define TOUCH_Posedge 0x01//��̧���¼�
#define TOUCH_Negedge 0x02//�ʰ����¼�

//ADC����ģʽ
#define COMMON_AD_MODER		0	//��ͨ��ADת��ģʽ
#define ASUNDER_X_MODER		1	//�����X����ģʽ
#define ASUNDER_Y_MODER		2	//�����Y����ģʽ
#define AUTO_XY_MODER		3	//�Զ���X,Y����ģʽ//��֪��Ϊ�β�׼,ֻ���÷����X,Yɨ��ģʽ
#define INT_AD_MODER		4	//�ȴ��жϵĲ���ģʽ
#define STANDBY_AD_MODER	5	//����ģʽ

//ADCͨ��ѡ��
#define ADC_CH_TOUCHX	0	//Xͨ��
#define ADC_CH_TOUCHY	1	//Yͨ��


//�ʸ˽ṹ��
typedef struct 
{
	u16 X0;//��Ч����
	u16 Y0;
	u16 X1; //�ݴ�����
	u16 Y1;						   	    
	u8 Touch_Calibration;//������У׼,Ϊ1��ʾ�Ѿ����й�У׼,Ϊ0��Ҫ����У׼		  
//������У׼����
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;	   
extern Pen_Holder Pen_Point;//����һ���ʸ˵Ľṹ����

void TOUCH_Init(FunctionalState EnableInt);	//��������ʼ��
void ADC_SetMode(u8 Mode);	//����ģʽ
u16 TOUCH_ReadADC(u8 TouchCH);	//��ȡ����,ƽ��,��1���ȡ
bool TOUCH_Read2ADC(u16 *X,u16 *Y);//��ȡ����,��ֹ����,��2���ȡ
void Touch_Adjust(void);	//������У׼
bool Touch_Convert_Pos(void);	//ת������
u8 Get_TouchState(void);	//��ȡ����״̬






//ʹ�ܱ�̧���ж�
__inline void ADCTSC_UD_SEN (u8 EN)
{
	ADC->TSC &= ~BIT8;			//�������ж�
	if(EN == Enable)
		ADC->TSC |= BIT8;
}


//YM��GMD��Ч,��HZ
__inline void ADCTSC_YM_HZ(void)
{
	ADC->TSC &= ~BIT7;		
}

//YM��GMD��Ч,��GND
__inline void ADCTSC_YM_GND(void)
{
	ADC->TSC |= BIT7;		
}


//YP��VDD��Ч,��HZ
__inline void ADCTSC_YP_HZ(void)
{
	ADC->TSC |= BIT6;		
}

//YP��VDD��Ч,��VDDA
__inline void ADCTSC_YP_VDD(void)
{
	ADC->TSC &= ~BIT6;		
}


//XM��GMD��Ч,��HZ
__inline void ADCTSC_XM_HZ(void)
{
	ADC->TSC &= ~BIT5;		
}

//XM��GMD��Ч,��GND
__inline void ADCTSC_XM_GND(void)
{
	ADC->TSC |= BIT5;		
}


//XP��VDD��Ч,��HZ
__inline void ADCTSC_XP_HZ(void)
{
	ADC->TSC |= BIT4;		
}

//XP��VDD��Ч,��VDDA
__inline void ADCTSC_XP_VDD(void)
{
	ADC->TSC &= ~BIT4;		
}


//����ADCת��ģʽ
__inline void Normal_ADC_Mode(void)
{
	ADC->TSC &= ~BIT2;	//������ADCת��
}

//����ģʽ
__inline void StandbyMode(void)
{
	ADC->CON |= BIT2;	
}


//�Զ�X,Yת��
__inline void AUTO_XYPosition(void)
{
	ADC->TSC &= ~(BIT0+BIT1);
	ADC->TSC |= BIT2;	//�Զ�X,Yת��
}

//X,Y�ֶ�����ģʽ,û������ģʽ
__inline void NO_OpeMode(void)
{
	ADC->TSC &= ~(BIT0+BIT1);
}

//X,Y�ֶ�����ģʽ,X����ת��ģʽ
__inline void X_PosMode(void)
{
	NO_OpeMode();			//�������
	ADC->TSC |= 1;
}

//X,Y�ֶ�����ģʽ,Y����ת��ģʽ
__inline void Y_PosMode(void)
{
	NO_OpeMode();			//�������
	ADC->TSC |= 2;
}

//X,Y�ֶ�����ģʽ,�ȴ��ж�ģʽ
__inline void INT_WaitingMode(void)
{
	NO_OpeMode();			//�������
	ADC->TSC |= 3;
}


//XP��������
__inline void XP_UP_ENABLE(void)
{
	ADC->TSC &= ~BIT3;
}


//XP������ֹ
__inline void XP_UP_DISABLE(void)
{
	ADC->TSC |= BIT3;
}


//���ADC�����ж�
__inline void ADC_ClearInt(void)
{
	ADC->CLRINT = 0xffffffff;		//д���κ�ֵ����жϱ�־
}


//����������ж��ж�
__inline void TOUCH_ClearInt(void)
{
	ADC->UPDN = 0;						//���ADC�Ĵ�����UP-DOWN�Ĵ���
	ADC->CLRINTPNDNUP = 0xffffffff;		//д���κ�ֵ����жϱ�־
}


//��ʼһ��ADCת��
__inline void ADC_Start(void)
{
	ADC->CON |= BIT0;	//��ʼADCת��
}


//�ȴ�ADCת�����
__inline void ADC_Wait(void)
{
	while(!(ADC->CON & BIT15));
}


#endif
