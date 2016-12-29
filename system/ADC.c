/*************************************************************************************************************
 * �ļ���:	ADC.c
 * ����:		S3C6410 ADC�ײ���������
 * ����:		����
 * ����ʱ��:	2012��3��12��21:05
 * ����޸�ʱ��:2012��3��12��
 * ��ϸ:		�����������Լ����ADC����
 * ����:		һֱ��������һ������,һֱ��Ϊʹ����������ʼ������,ÿ�ζ�DAT�Ĵ�����Ϳ��Զ�ȡ��ת���������,���շ���ʹ����������������һ�ε�����,
 * 			������Ҫ�ȵ�ת�����,��������ת����ʱ�����ݻ�ǳ���.
*************************************************************************************************************/
#include "system.h"
#include "ADC.h"
#include "tft_lcd.h"
#include "math.h"	//��׼C�⺯��,�ṩ��ѧ����

Pen_Holder Pen_Point;//����һ���ʸ˵Ľṹ����

//�������жϷ�����
void __irq Isr_Touch(void);

//ADCͨ��ѡ��
#define ADC_CH_AIN0		0
#define ADC_CH_AIN1		1
#define ADC_CH_AIN2		2
#define ADC_CH_AIN3		3
#define ADC_CH_YM		4
#define ADC_CH_YP		5
#define ADC_CH_XM		6
#define ADC_CH_XP		7



//ADC�Ŀ��ƼĴ��� ADCCON
#define ADCCON_RESSEL_12BIT			(1 << 16)	//12bitģʽ
#define ADCCON_ECFLG 				(0 << 15)	//A/Dת��������־ֻ��;
#define ADCCON_PRSCEN				(1 << 14)	//A/Dת����Ԥ��Ƶ��ʹ��
#define ADCCON_PRSCVL				(32	<< 6)	//Ԥ��Ƶֵ,1-255,��Ƶֵ+1,����ΪPCLK��1/5,��ʱPCLK = 66MHZ,��2.5MHZʱ����ת�����
#define ADCCON_SEL_MUX				(0	<< 3)	//Ĭ��ѡ��ͨ��0
#define ADCCON_STDBM				(0	<< 2)	//����ģʽ
#define ADCCON_READ_START			(1	<< 1)	//������ʼ������
#define ADCCON_ENABLE_START			(0  << 0)	//���READ_START ���ã����ֵ����Ч�ġ�


//ADCDLY
#define ADCDLY_DELAY				500			//�Զ�������ʱʱ��,



/*************************************************************************************************************************
*����        :	void SetADC_Channel(u8 ch)
*����        :	����ADC����ͨ��
*����        :	ch:ͨ����,0-7
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120513
*����޸�ʱ��:	20120513
*˵��        :	ADC����ͨ��ѡ��
*************************************************************************************************************************/
void SetADC_Channel(u8 ch)
{
	ADC->CON &= ~(7 << 3);		//���ͨ��
	ADC->CON |= ch & (0x07);	//����ͨ����
}



/*************************************************************************************************************************
*����        :	void ADC_Init(void)
*����        :	ADC��ʼʼ��
*����        :	��
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120312
*����޸�ʱ��:	20120313
*˵��        :	ADC��ʼʼ��
*************************************************************************************************************************/
void ADC_Init(void)
{
	//ADC�Ŀ��ƼĴ�������;12BITģʽ
	ADC->CON = ADCCON_RESSEL_12BIT + ADCCON_ECFLG + ADCCON_PRSCEN + ADCCON_PRSCVL + ADCCON_SEL_MUX + ADCCON_STDBM + ADCCON_READ_START + ADCCON_ENABLE_START;
	ADC->DLY = ADCDLY_DELAY;								//�����Զ��������ʱ��
}


/*************************************************************************************************************************
*����        :	void ADC_SetMode(u8 Mode)
*����        :	����ADCģʽ
*����        :	��
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120313
*����޸�ʱ��:	20120313
*˵��        :	����ADCģʽ
*************************************************************************************************************************/
void ADC_SetMode(u8 Mode)
{
	ADC->TSC &= (1 << 8);		//���ԭ������
	ADC->CON &= ~BIT2;			//�˳�����ģʽ
	XP_UP_DISABLE();			//XP������ֹ
	Normal_ADC_Mode();			//��ͨADCģʽ
	switch(Mode)
	{
		case COMMON_AD_MODER:	//��ͨADCģʽ
			Normal_ADC_Mode();break;
		case ASUNDER_X_MODER:	//�����Xɨ��ģʽ
		{
			ADCTSC_XP_VDD();
			ADCTSC_XM_GND();
			ADCTSC_YP_HZ();
			ADCTSC_YM_HZ();
			X_PosMode();
		}break;		//XP=�ⲿ��Դ,XM=GND,YP=AIN5,YM=����
		case ASUNDER_Y_MODER:	//�����Yɨ��ģʽ
		{
			ADCTSC_XP_HZ();
			ADCTSC_XM_HZ();
			ADCTSC_YP_VDD();
			ADCTSC_YM_GND();
			Y_PosMode();
		}break;		//XP=AIN7,XM=����,YP=�ⲿ��Դ,YM=GND
		case AUTO_XY_MODER:		//�Զ�XYɨ��ģʽ
		{
			AUTO_XYPosition();
		}break;
		case INT_AD_MODER:		//�ȴ��ж�ģʽ
		{
			ADCTSC_XP_HZ();
			ADCTSC_XM_HZ();
			ADCTSC_YP_HZ();
			ADCTSC_YM_GND();
			XP_UP_ENABLE();		//XP����ʹ��			
			INT_WaitingMode();
		}break;//XP����,XM=����,YP=AIN5,YM=GND
		case STANDBY_AD_MODER:	//����ģʽ
			StandbyMode();break;
		default:break;
	}
}




/*************************************************************************************************************************
*����        :	void TOUCH_ReadOneADC(u16 *X,u16 *Y)
*����        :	��ȡһ������ֵ
*����        :	X:x���껺����ָ��,Y:y���껺����ָ��
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120313
*����޸�ʱ��:	20120313
*˵��        :	��
*************************************************************************************************************************/
void TOUCH_ReadOneADC(u16 *X,u16 *Y)
{
	ADC_SetMode(AUTO_XY_MODER);		//�Զ�XYɨ��ģʽ
	*X = ADC->DAT0 & 0xFFF;			//��ȡX����
	*Y = ADC->DAT1 & 0xFFF;			//��ȡY����
	ADC_SetMode(INT_AD_MODER);		//�ȴ��ж�ģʽ
}


/*************************************************************************************************************************
*����        :	u8 Get_TouchState(void)
*����        :	��ȡ����״̬
*����        :	��
*����        :	1:��̧��;0:�ʰ���
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120315
*����޸�ʱ��:	20120315
*˵��        :	ͨ����ȡADCDAT1��BIT15��ȷ��״̬
*************************************************************************************************************************/
u8 Get_TouchState(void)
{
	return((ADC->DAT1 & BIT15) ? TOUCH_UP : TOUCH_DOWN);
}


//�������жϷ�����
void __irq Isr_Touch(void)
{
	if(ADC->UPDN & BIT0)	//�����¼�
	{
		lcd_printf("\n DOWM ");
		/*
		ADC_SetMode(AUTO_XY_MODER);
		ADC->DAT0;
		ADC->DAT1;
		lcd_printf("X : %d; Y : %d \n",ADC->DAT0 & 0xFFF,ADC->DAT1 & 0xFFF);*/
		ADCTSC_UD_SEN(Enable);		//����Ϊ̧���ж�
	}
	else if(ADC->UPDN & BIT1)	//̧���¼�
	{
		lcd_printf(" UP");
		ADCTSC_UD_SEN(Disable);		//����Ϊ�����ж�
	}
	TOUCH_ClearInt();			//����������жϱ�־
	ADC_SetMode(INT_AD_MODER);		//�ȴ��ж�ģʽ
	VICInterruptEnd();			//�жϽ���
}





/*************************************************************************************************************************
*����        :	void TOUCH_Init(void)
*����        :	��������ʼʼ��
*����        :	EnableInt:���ж�ʹ��
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120313
*����޸�ʱ��:	20120729
*˵��        :	��������ʼʼ��
*************************************************************************************************************************/
void TOUCH_Init(FunctionalState EnableInt)
{
	ADC_Init();					//��ʼ��ADC
	TOUCH_ClearInt();			//����������жϱ�־
	ADC_ClearInt();				//���ADC�ж�
	ADC_SetMode(INT_AD_MODER);	//�ȴ��ж�ģʽ
	ADCTSC_UD_SEN(Disable);		//����Ϊ�����ж�
	if(EnableInt == ENABLE)	//�������ж�
	{
		Set_IsrAddr(INT_PENDNUP,(u32)Isr_Touch);	//�����ж�ʸ�����
		Set_IntEnable(INT_PENDNUP,ENABLE);			//��������������
	}
}




/*************************************************************************************************************************
*����        :	u16 TOUCH_ReadADC(u8 TouchCH)
*����        :	��ȡ����������,��ԭʼ������
*����        :	TouchCH:Xͨ��,Yͨ��
*����        :	����ֵ
*����     : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120729
*����޸�ʱ��:	20120729
*˵��        :	ȥ������͹�С��ֵ,����ȡƽ��
*************************************************************************************************************************/
#define TOUCH_READ_NUM		(u8)13		//��ȡ����
#define LOST_VAL  			(u8)5
#define MIN_ADC_DAT			300			//ת������Сֵ
u16 TOUCH_ReadADC(u8 TouchCH)
{
	vu32 *p;
	u16 i,j;
	u16 temp;
	u16 buff[TOUCH_READ_NUM];	//������
	u32 data = 0;
	
	switch(TouchCH)
	{
		case ADC_CH_TOUCHX ://Xͨ��
			{
				p = &(ADC->DAT0);
				ADC_SetMode(ASUNDER_X_MODER);
			}break;
		case ADC_CH_TOUCHY ://Yͨ��
			{
				p = &(ADC->DAT1);
				ADC_SetMode(ASUNDER_Y_MODER);	
			}break;
		default : return 0;
	}
	for(i = 0;i < TOUCH_READ_NUM;)
	{
		temp = *p & 0xfff;	//��ȡת������
		//uart_printf("temp=%d\r\n",temp);
		//for(j = 0;j < 100;j ++);
		ADC_Wait();			//�ȴ�ת�����,������ת���ǳ���׼
		if(temp > MIN_ADC_DAT)
		{
			buff[i ++] = temp;
		}	
	}
	
	for(i = 0;i < TOUCH_READ_NUM - 1;i ++)//?��?Y??D��
	{
	 	for(j = i + 1;j < TOUCH_READ_NUM;j ++) //������?��?o��??��?��y����??
		{
		 	if(buff[i] > buff[j]) //��?1??��??��?��????�̡䨮����o��??��??��??DD��???
			{
			 	data = buff[i];
				buff[i] = buff[j];
				buff[j] = data;
			}
		}
	}
	data = 0;
	for(i = LOST_VAL;i < TOUCH_READ_NUM - LOST_VAL;i ++)//?�����ꨮ����y��?o��
		data += buff[i];
	data /= (TOUCH_READ_NUM - 2 * LOST_VAL);//?��??��y???����y
	
	ADC_SetMode(INT_AD_MODER);	//�ȴ��ж�ģʽ
	
	return (u16)data;
}



/*************************************************************************************************************************
*����        :	bool TOUCH_Read2ADC(u8 TouchCH,u16 *data)
*����        :	��ȡ���εͼ��˲��������,���Ҳ�ֵ�ڹ涨��Χ��
*����        :	TouchCH:Xͨ��,Yͨ��,data:��Ž����ָ��
*����        :	FALSE:������Ч,��ֵ����,TRUE:������Ч
*����     : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120729
*����޸�ʱ��:	20120729
*˵��        :	��ֹ��������,��ֵ:ERR_RANGE
*************************************************************************************************************************/
#define ERR_RANGE 80
#define MAX_RETRY_NUMBER	3	//ת��ʧ��,������Դ���
bool TOUCH_Read2ADC(u16 *X,u16 *Y)
{
	u16 x1,x2,y1,y2;
	u16 cnt;
	
	for(cnt = MAX_RETRY_NUMBER;cnt != 0;cnt --)
	{
		x1 = TOUCH_ReadADC(ADC_CH_TOUCHX);	
		y1 = TOUCH_ReadADC(ADC_CH_TOUCHY);
		x2 = TOUCH_ReadADC(ADC_CH_TOUCHX);
		y2 = TOUCH_ReadADC(ADC_CH_TOUCHY);
		//uart_printf("x1:%d x2:%d \r\n",x1,x2);
		//uart_printf("y1:%d y2:%d \r\n",y1,y2);
		//��������
		if(((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) && ((y2 <= y1 && y1 < y2 + ERR_RANGE) ||(y1 <= y2 && y2 <y1 + ERR_RANGE))) //?��o����?��?2��?��?��+-50?��
		{
		 	*X = (x1 + x2) / 2;
			*Y = (y1 + y2) / 2;
			//uart_printf("OK!\n");
			return TRUE;
		}
	}
	return FALSE;
}

/*************************************************************************************************************************
*����        :	bool TOUCH_Read2ADC(u8 TouchCH,u16 *data)
*����        :	��ȡһ������ֵ,ֱ������̧���ŷ���
*����        :	��
*����        :	FALSE:������Ч,��ֵ����,TRUE:������Ч
*����     : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120730
*����޸�ʱ��:	20120730
*˵��        :	��
*************************************************************************************************************************/
bool TOUCH_READ_Once(void)
{
	bool flag = TRUE;
	
	while(Get_TouchState() == TOUCH_UP);//�ȴ��������� 
	flag = TOUCH_Read2ADC(&(Pen_Point.X1),&(Pen_Point.Y1));//��ȡ����
	DEBUG("X:%d, Y:%d, flag=%d\r\n",Pen_Point.X1,Pen_Point.Y1,flag);
	while((Get_TouchState() == TOUCH_DOWN));//�ȴ�����̧��
	return flag;	
}

/**************************************************************************************************************/
/*	��һ��������	*/
//����У׼�õ�
void Drow_Touch_Point(u16 x,u16 y)
{
	Draw_Circle(x,y,10,0xf0f0);//��Բ
	Draw_Circle(x,y,3,0xf0f0);//��Բ
	LCD_DrawLine(x - 15, y, x + 15, y,0xf0f0);//����
	LCD_DrawLine(x, y - 15, x, y + 15,0xf0f0);//����	
	LCD_DrawRectangle(x - 10,y - 10,x + 10,y + 10,0xf0f0);//������
}



/*************************************************************************************************************************
*����        :	void Touch_Adjust(void)
*����        :	������У׼����	
*����        :	��
*����        :	��
*����     : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120730
*����޸�ʱ��:	20120730
*˵��        :	�õ��ĸ�У׼����
*************************************************************************************************************************/
void Touch_Adjust(void)
{
	u16 buff[3][2];//���津��������ֵ
	u8 n = 0;
	u32 temp1,temp2;
	u16 d1,d2;
	float fac;
	//Show_Char(2,140,"The first time, please click on the screen for touch-screen calibration.",0x000f,0xffff,0x80);
	//while(Pen_Point.Key_Sta != Key_Down);//�ȴ����´���
	//LCD_Fill(2,239,140,140 + 17 + 17,0xffff);//�������
	//while(Pen_Point.Key_Sta == Key_Down);//�ȴ�����̧��
	//800 * 480
	/*
		(25,30)				
		
		
					(LCD_XSIZE/2,LCD_YSIZE/2)
		
		
		
							(LCD_XSIZE-25,LCD_YSIZE-25)
	
	
	*/

	Pen_Point.Touch_Calibration = 0;//û�н��д�����У׼

	LCD_ClearScreen(0xffff);//����


	Drow_Touch_Point(25,30);	  //���Ͻ�
	//LCD_Font_Show_CN(1,60,"������Ļ ���� �Ǽ�ͷ����.",0);
	while(1)
	{
	 	if(Get_TouchState() == TOUCH_DOWN)//����а�������
		{
			if(TOUCH_READ_Once() == TRUE)//��ȡ��������
			{
			 	buff[n][0] = Pen_Point.X1;
				buff[n][1] = Pen_Point.Y1;
				//DEBUG("Pen_Point.X1 = %d, Pen_Point.Y1 = %d\n\r",Pen_Point.X1,Pen_Point.Y1);
				n ++;
			}
			switch (n)
			{
			 	case 1:	//�м�
					{
						LCD_ClearScreen(0xffff);//����
						Drow_Touch_Point(LCD_XSIZE/2,LCD_YSIZE/2);	  //�м�
					//	LCD_Font_Show_CN(8,185,"������Ļ �м� �Ǽ�ͷ����.",0);
					}break;
				case 2:	 //���½� 
					{
					 	LCD_ClearScreen(0xffff);//����
						Drow_Touch_Point(LCD_XSIZE-25,LCD_YSIZE-30);	  //���½�
					//	LCD_Font_Show_CN(15,200,"������Ļ ���� �Ǽ�ͷ����.",0);
					}break;
				case 3:
					{
						///////////////////���ΰ�������,��ʼ����/////////////////////////////////////////////////////
					//�ж�(�ϱ�+�±�)/2=�����Ƿ����
						//x�᷽������
						temp1 = (buff[0][0] + buff[2][0]) / 2;//�����ϵ׼��µ׳�2
						fac = (float) temp1 / buff[1][0];//������λ��
						if(fac < 0.95 || fac > 1.05 || fac == 0)//���ϸ�
						{
							n = 0;
							LCD_ClearScreen(0xffff);//����
					 		Drow_Touch_Point(25,30);	  //���Ͻ�
						//	LCD_Font_Show_CN(1,60,"������Ļ ���� �Ǽ�ͷ����.",0);
							continue;
						}//x����У׼�ɹ�

						//y�᷽������
						temp1 = (buff[0][1] + buff[2][1]) / 2;//�����ϵ׼��µ׳�2
						fac = (float) temp1 / buff[1][1];//������λ��
						if(fac < 0.95 || fac > 1.05 || fac == 0)//���ϸ�
						{
							n = 0;
							LCD_ClearScreen(0xffff);//����
					 		Drow_Touch_Point(25,30);	  //���Ͻ�
						//	LCD_Font_Show_CN(1,60,"������Ļ ���� �Ǽ�ͷ����.",0);
							continue;
						}//y����У׼�ɹ�

					//�ж�1��2��3��2�ľ����Ƿ����
						temp1 = buff[0][0] - buff[1][0];//x2-x1
						temp2 = buff[0][1] - buff[1][1];//y2-y1
						temp1 *= temp1;
						temp2 *= temp2;
						d1 = sqrt(temp1 + temp2);//�õ�1,2�ľ���

						temp1 = buff[1][0] - buff[2][0];//x3-x2
						temp2 = buff[1][1] - buff[2][1];//y3-y2
						temp1 *= temp1;
						temp2 *= temp2;
						d2 = sqrt(temp1 + temp2);//�õ�2,3�ľ���

						fac = (float) d1 / d2;
						if(fac<0.95||fac>1.05||d1==0||d2==0)//���ϸ�
						{
							n = 0;
							LCD_ClearScreen(0xffff);//����
					 		Drow_Touch_Point(25,30);	  //���Ͻ�
						//	LCD_Font_Show_CN(1,60,"������Ļ ���� �Ǽ�ͷ����.",0);
							continue;
						} //�������,У׼�ɹ�

						LCD_ClearScreen(0xffff);//����

						Pen_Point.Touch_Calibration = 1;//����˴�����У׼

					   ////////////////У׼���////////////////////////////////////////////////////////////
					   //����У׼���
						Pen_Point.xfac = (float) (LCD_XSIZE-50) * 1.0 / (buff[0][0] - buff[2][0]);//((750) / (buff[2][0] - buff[0][0]) + (240 - 25) / (buff[1][0] - buff[0][0])) / 2;
						Pen_Point.xoff = buff[1][0] * Pen_Point.xfac - LCD_XSIZE / 2;
						
						Pen_Point.yfac = (float) (LCD_YSIZE-60) * 1.0 / (buff[0][1] - buff[2][1]);
						Pen_Point.yoff = buff[1][1] * Pen_Point.yfac - LCD_YSIZE / 2;

					//	Wire_Touch();//�洢У׼���
																			  
						//LCD_Font_Show_CN(20,140,"������У׼���!",0);
					//	Delay_MS(1000);
					//	LCD_Fill(0,239,140,160,0xffff);//�������
						return;
					} 
			}
		}
	}	
}



/*************************************************************************************************************************
*����        :	void Touch_Convert_Pos(void)
*����        :	ת������
*����        :	��
*����        :	FALSE:ת����������Ч;TRUE:ת����������Ч
*����     : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120730
*����޸�ʱ��:	20120805
*˵��        :	���ݴ�������У׼����������ת����Ľ��,������X0,Y0��
*************************************************************************************************************************/
bool Touch_Convert_Pos(void)
{	
	u16 x,y;

	if(TOUCH_Read2ADC(&Pen_Point.X1,&Pen_Point.Y1) == TRUE)//��ȡ����,��Ч
	{
		x = LCD_XSIZE - (Pen_Point.xfac * Pen_Point.X1 - Pen_Point.xoff);
		y = LCD_YSIZE - (Pen_Point.yfac * Pen_Point.Y1 - Pen_Point.yoff);
		if(x >= LCD_XSIZE)
			x = LCD_XSIZE - 1;
		if(y >= LCD_YSIZE)
			y = LCD_YSIZE - 1;
		Pen_Point.X0 = x;
		Pen_Point.Y0 = y;
		return TRUE;
	}
	return FALSE;	
}





