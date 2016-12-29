/*************************************************************************************************************
 * 文件名:	ADC.c
 * 功能:		S3C6410 ADC底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年3月12日21:05
 * 最后修改时间:2012年3月12日
 * 详细:		触摸屏驱动以及相关ADC驱动
 * 问题:		一直以来存在一个误区,一直以为使用了启动开始读操作,每次读DAT寄存器后就可以读取到转换后的数据,最终发现使用这个后读到的是上一次的数据,
 * 			还是需要等到转换完成,否则连续转换的时候数据会非常乱.
*************************************************************************************************************/
#include "system.h"
#include "ADC.h"
#include "tft_lcd.h"
#include "math.h"	//标准C库函数,提供数学函数

Pen_Holder Pen_Point;//定义一个笔杆的结构变量

//触摸屏中断服务函数
void __irq Isr_Touch(void);

//ADC通道选择
#define ADC_CH_AIN0		0
#define ADC_CH_AIN1		1
#define ADC_CH_AIN2		2
#define ADC_CH_AIN3		3
#define ADC_CH_YM		4
#define ADC_CH_YP		5
#define ADC_CH_XM		6
#define ADC_CH_XP		7



//ADC的控制寄存器 ADCCON
#define ADCCON_RESSEL_12BIT			(1 << 16)	//12bit模式
#define ADCCON_ECFLG 				(0 << 15)	//A/D转换结束标志只读;
#define ADCCON_PRSCEN				(1 << 14)	//A/D转换器预分频器使能
#define ADCCON_PRSCVL				(32	<< 6)	//预分频值,1-255,分频值+1,至少为PCLK的1/5,此时PCLK = 66MHZ,在2.5MHZ时钟下转换最快
#define ADCCON_SEL_MUX				(0	<< 3)	//默认选择通道0
#define ADCCON_STDBM				(0	<< 2)	//正常模式
#define ADCCON_READ_START			(1	<< 1)	//启动开始读操作
#define ADCCON_ENABLE_START			(0  << 0)	//如果READ_START 启用，这个值是无效的。


//ADCDLY
#define ADCDLY_DELAY				500			//自动采样延时时间,



/*************************************************************************************************************************
*函数        :	void SetADC_Channel(u8 ch)
*功能        :	设置ADC输入通道
*参数        :	ch:通道号,0-7
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120513
*最后修改时间:	20120513
*说明        :	ADC输入通道选择
*************************************************************************************************************************/
void SetADC_Channel(u8 ch)
{
	ADC->CON &= ~(7 << 3);		//清除通道
	ADC->CON |= ch & (0x07);	//设置通道号
}



/*************************************************************************************************************************
*函数        :	void ADC_Init(void)
*功能        :	ADC初始始化
*参数        :	无
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120312
*最后修改时间:	20120313
*说明        :	ADC初始始化
*************************************************************************************************************************/
void ADC_Init(void)
{
	//ADC的控制寄存器配置;12BIT模式
	ADC->CON = ADCCON_RESSEL_12BIT + ADCCON_ECFLG + ADCCON_PRSCEN + ADCCON_PRSCVL + ADCCON_SEL_MUX + ADCCON_STDBM + ADCCON_READ_START + ADCCON_ENABLE_START;
	ADC->DLY = ADCDLY_DELAY;								//设置自动间隔采样时间
}


/*************************************************************************************************************************
*函数        :	void ADC_SetMode(u8 Mode)
*功能        :	设置ADC模式
*参数        :	无
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120313
*最后修改时间:	20120313
*说明        :	设置ADC模式
*************************************************************************************************************************/
void ADC_SetMode(u8 Mode)
{
	ADC->TSC &= (1 << 8);		//清除原先设置
	ADC->CON &= ~BIT2;			//退出待机模式
	XP_UP_DISABLE();			//XP上拉禁止
	Normal_ADC_Mode();			//普通ADC模式
	switch(Mode)
	{
		case COMMON_AD_MODER:	//普通ADC模式
			Normal_ADC_Mode();break;
		case ASUNDER_X_MODER:	//分离的X扫描模式
		{
			ADCTSC_XP_VDD();
			ADCTSC_XM_GND();
			ADCTSC_YP_HZ();
			ADCTSC_YM_HZ();
			X_PosMode();
		}break;		//XP=外部电源,XM=GND,YP=AIN5,YM=高阻
		case ASUNDER_Y_MODER:	//分离的Y扫描模式
		{
			ADCTSC_XP_HZ();
			ADCTSC_XM_HZ();
			ADCTSC_YP_VDD();
			ADCTSC_YM_GND();
			Y_PosMode();
		}break;		//XP=AIN7,XM=高阻,YP=外部电源,YM=GND
		case AUTO_XY_MODER:		//自动XY扫描模式
		{
			AUTO_XYPosition();
		}break;
		case INT_AD_MODER:		//等待中断模式
		{
			ADCTSC_XP_HZ();
			ADCTSC_XM_HZ();
			ADCTSC_YP_HZ();
			ADCTSC_YM_GND();
			XP_UP_ENABLE();		//XP上拉使能			
			INT_WaitingMode();
		}break;//XP上拉,XM=高阻,YP=AIN5,YM=GND
		case STANDBY_AD_MODER:	//掉电模式
			StandbyMode();break;
		default:break;
	}
}




/*************************************************************************************************************************
*函数        :	void TOUCH_ReadOneADC(u16 *X,u16 *Y)
*功能        :	读取一次坐标值
*参数        :	X:x坐标缓冲区指针,Y:y坐标缓冲区指针
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120313
*最后修改时间:	20120313
*说明        :	无
*************************************************************************************************************************/
void TOUCH_ReadOneADC(u16 *X,u16 *Y)
{
	ADC_SetMode(AUTO_XY_MODER);		//自动XY扫描模式
	*X = ADC->DAT0 & 0xFFF;			//读取X坐标
	*Y = ADC->DAT1 & 0xFFF;			//读取Y坐标
	ADC_SetMode(INT_AD_MODER);		//等待中断模式
}


/*************************************************************************************************************************
*函数        :	u8 Get_TouchState(void)
*功能        :	获取触摸状态
*参数        :	无
*返回        :	1:笔抬起;0:笔按下
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120315
*最后修改时间:	20120315
*说明        :	通过读取ADCDAT1的BIT15来确定状态
*************************************************************************************************************************/
u8 Get_TouchState(void)
{
	return((ADC->DAT1 & BIT15) ? TOUCH_UP : TOUCH_DOWN);
}


//触摸屏中断服务函数
void __irq Isr_Touch(void)
{
	if(ADC->UPDN & BIT0)	//按下事件
	{
		lcd_printf("\n DOWM ");
		/*
		ADC_SetMode(AUTO_XY_MODER);
		ADC->DAT0;
		ADC->DAT1;
		lcd_printf("X : %d; Y : %d \n",ADC->DAT0 & 0xFFF,ADC->DAT1 & 0xFFF);*/
		ADCTSC_UD_SEN(Enable);		//设置为抬起中断
	}
	else if(ADC->UPDN & BIT1)	//抬起事件
	{
		lcd_printf(" UP");
		ADCTSC_UD_SEN(Disable);		//设置为按下中断
	}
	TOUCH_ClearInt();			//清除触摸屏中断标志
	ADC_SetMode(INT_AD_MODER);		//等待中断模式
	VICInterruptEnd();			//中断结束
}





/*************************************************************************************************************************
*函数        :	void TOUCH_Init(void)
*功能        :	触摸屏初始始化
*参数        :	EnableInt:笔中断使能
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120313
*最后修改时间:	20120729
*说明        :	触摸屏初始始化
*************************************************************************************************************************/
void TOUCH_Init(FunctionalState EnableInt)
{
	ADC_Init();					//初始化ADC
	TOUCH_ClearInt();			//清除触摸屏中断标志
	ADC_ClearInt();				//清除ADC中断
	ADC_SetMode(INT_AD_MODER);	//等待中断模式
	ADCTSC_UD_SEN(Disable);		//设置为按下中断
	if(EnableInt == ENABLE)	//开启笔中断
	{
		Set_IsrAddr(INT_PENDNUP,(u32)Isr_Touch);	//设置中断矢量入口
		Set_IntEnable(INT_PENDNUP,ENABLE);			//开启触摸屏按下
	}
}




/*************************************************************************************************************************
*函数        :	u16 TOUCH_ReadADC(u8 TouchCH)
*功能        :	读取触摸屏坐标,最原始的坐标
*参数        :	TouchCH:X通道,Y通道
*返回        :	坐标值
*依赖     : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120729
*最后修改时间:	20120729
*说明        :	去掉过大和过小的值,并且取平均
*************************************************************************************************************************/
#define TOUCH_READ_NUM		(u8)13		//读取次数
#define LOST_VAL  			(u8)5
#define MIN_ADC_DAT			300			//转换的最小值
u16 TOUCH_ReadADC(u8 TouchCH)
{
	vu32 *p;
	u16 i,j;
	u16 temp;
	u16 buff[TOUCH_READ_NUM];	//缓冲区
	u32 data = 0;
	
	switch(TouchCH)
	{
		case ADC_CH_TOUCHX ://X通道
			{
				p = &(ADC->DAT0);
				ADC_SetMode(ASUNDER_X_MODER);
			}break;
		case ADC_CH_TOUCHY ://Y通道
			{
				p = &(ADC->DAT1);
				ADC_SetMode(ASUNDER_Y_MODER);	
			}break;
		default : return 0;
	}
	for(i = 0;i < TOUCH_READ_NUM;)
	{
		temp = *p & 0xfff;	//读取转换坐标
		//uart_printf("temp=%d\r\n",temp);
		//for(j = 0;j < 100;j ++);
		ADC_Wait();			//等待转换完毕,不加上转换非常不准
		if(temp > MIN_ADC_DAT)
		{
			buff[i ++] = temp;
		}	
	}
	
	for(i = 0;i < TOUCH_READ_NUM - 1;i ++)//?°?Y??Dò
	{
	 	for(j = i + 1;j < TOUCH_READ_NUM;j ++) //òà′?ó?oó??μ?êy±è??
		{
		 	if(buff[i] > buff[j]) //è?1??°??μ?ò????μ′óóúoó??μ??ò??DDì???
			{
			 	data = buff[i];
				buff[i] = buff[j];
				buff[j] = data;
			}
		}
	}
	data = 0;
	for(i = LOST_VAL;i < TOUCH_READ_NUM - LOST_VAL;i ++)//?óê￡óàêyμ?oí
		data += buff[i];
	data /= (TOUCH_READ_NUM - 2 * LOST_VAL);//?ó??êy???ùêy
	
	ADC_SetMode(INT_AD_MODER);	//等待中断模式
	
	return (u16)data;
}



/*************************************************************************************************************************
*函数        :	bool TOUCH_Read2ADC(u8 TouchCH,u16 *data)
*功能        :	读取两次低级滤波后的坐标,并且差值在规定范围内
*参数        :	TouchCH:X通道,Y通道,data:存放结果的指针
*返回        :	FALSE:数据无效,差值过大,TRUE:数据有效
*依赖     : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120729
*最后修改时间:	20120729
*说明        :	防止数据跳变,差值:ERR_RANGE
*************************************************************************************************************************/
#define ERR_RANGE 80
#define MAX_RETRY_NUMBER	3	//转换失败,最大重试次数
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
		//满足条件
		if(((x2 <= x1 && x1 < x2 + ERR_RANGE) || (x1 <= x2 && x2 < x1 + ERR_RANGE)) && ((y2 <= y1 && y1 < y2 + ERR_RANGE) ||(y1 <= y2 && y2 <y1 + ERR_RANGE))) //?°oóá?′?2é?ù?ú+-50?ú
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
*函数        :	bool TOUCH_Read2ADC(u8 TouchCH,u16 *data)
*功能        :	读取一次坐标值,直到按键抬起后才返回
*参数        :	无
*返回        :	FALSE:数据无效,差值过大,TRUE:数据有效
*依赖     : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120730
*最后修改时间:	20120730
*说明        :	无
*************************************************************************************************************************/
bool TOUCH_READ_Once(void)
{
	bool flag = TRUE;
	
	while(Get_TouchState() == TOUCH_UP);//等待按键按下 
	flag = TOUCH_Read2ADC(&(Pen_Point.X1),&(Pen_Point.Y1));//读取坐标
	DEBUG("X:%d, Y:%d, flag=%d\r\n",Pen_Point.X1,Pen_Point.Y1,flag);
	while((Get_TouchState() == TOUCH_DOWN));//等待按键抬起
	return flag;	
}

/**************************************************************************************************************/
/*	画一个触摸点	*/
//用来校准用的
void Drow_Touch_Point(u16 x,u16 y)
{
	Draw_Circle(x,y,10,0xf0f0);//画圆
	Draw_Circle(x,y,3,0xf0f0);//画圆
	LCD_DrawLine(x - 15, y, x + 15, y,0xf0f0);//画线
	LCD_DrawLine(x, y - 15, x, y + 15,0xf0f0);//画线	
	LCD_DrawRectangle(x - 10,y - 10,x + 10,y + 10,0xf0f0);//画矩形
}



/*************************************************************************************************************************
*函数        :	void Touch_Adjust(void)
*功能        :	触摸屏校准函数	
*参数        :	无
*返回        :	无
*依赖     : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120730
*最后修改时间:	20120730
*说明        :	得到四个校准参数
*************************************************************************************************************************/
void Touch_Adjust(void)
{
	u16 buff[3][2];//缓存触摸屏坐标值
	u8 n = 0;
	u32 temp1,temp2;
	u16 d1,d2;
	float fac;
	//Show_Char(2,140,"The first time, please click on the screen for touch-screen calibration.",0x000f,0xffff,0x80);
	//while(Pen_Point.Key_Sta != Key_Down);//等待按下触屏
	//LCD_Fill(2,239,140,140 + 17 + 17,0xffff);//清除文字
	//while(Pen_Point.Key_Sta == Key_Down);//等待触屏抬起
	//800 * 480
	/*
		(25,30)				
		
		
					(LCD_XSIZE/2,LCD_YSIZE/2)
		
		
		
							(LCD_XSIZE-25,LCD_YSIZE-25)
	
	
	*/

	Pen_Point.Touch_Calibration = 0;//没有进行触摸屏校准

	LCD_ClearScreen(0xffff);//清屏


	Drow_Touch_Point(25,30);	  //左上角
	//LCD_Font_Show_CN(1,60,"请点击屏幕 左上 角箭头中心.",0);
	while(1)
	{
	 	if(Get_TouchState() == TOUCH_DOWN)//如果有按键按下
		{
			if(TOUCH_READ_Once() == TRUE)//读取单次坐标
			{
			 	buff[n][0] = Pen_Point.X1;
				buff[n][1] = Pen_Point.Y1;
				//DEBUG("Pen_Point.X1 = %d, Pen_Point.Y1 = %d\n\r",Pen_Point.X1,Pen_Point.Y1);
				n ++;
			}
			switch (n)
			{
			 	case 1:	//中间
					{
						LCD_ClearScreen(0xffff);//清屏
						Drow_Touch_Point(LCD_XSIZE/2,LCD_YSIZE/2);	  //中间
					//	LCD_Font_Show_CN(8,185,"请点击屏幕 中间 角箭头中心.",0);
					}break;
				case 2:	 //右下角 
					{
					 	LCD_ClearScreen(0xffff);//清屏
						Drow_Touch_Point(LCD_XSIZE-25,LCD_YSIZE-30);	  //右下角
					//	LCD_Font_Show_CN(15,200,"请点击屏幕 右下 角箭头中心.",0);
					}break;
				case 3:
					{
						///////////////////三次按键按下,开始计算/////////////////////////////////////////////////////
					//判断(上边+下边)/2=中线是否相等
						//x轴方向梯形
						temp1 = (buff[0][0] + buff[2][0]) / 2;//梯形上底加下底除2
						fac = (float) temp1 / buff[1][0];//除以中位线
						if(fac < 0.95 || fac > 1.05 || fac == 0)//不合格
						{
							n = 0;
							LCD_ClearScreen(0xffff);//清屏
					 		Drow_Touch_Point(25,30);	  //左上角
						//	LCD_Font_Show_CN(1,60,"请点击屏幕 左上 角箭头中心.",0);
							continue;
						}//x方向校准成功

						//y轴方向梯形
						temp1 = (buff[0][1] + buff[2][1]) / 2;//梯形上底加下底除2
						fac = (float) temp1 / buff[1][1];//除以中位线
						if(fac < 0.95 || fac > 1.05 || fac == 0)//不合格
						{
							n = 0;
							LCD_ClearScreen(0xffff);//清屏
					 		Drow_Touch_Point(25,30);	  //左上角
						//	LCD_Font_Show_CN(1,60,"请点击屏幕 左上 角箭头中心.",0);
							continue;
						}//y方向校准成功

					//判断1到2与3到2的距离是否相等
						temp1 = buff[0][0] - buff[1][0];//x2-x1
						temp2 = buff[0][1] - buff[1][1];//y2-y1
						temp1 *= temp1;
						temp2 *= temp2;
						d1 = sqrt(temp1 + temp2);//得到1,2的距离

						temp1 = buff[1][0] - buff[2][0];//x3-x2
						temp2 = buff[1][1] - buff[2][1];//y3-y2
						temp1 *= temp1;
						temp2 *= temp2;
						d2 = sqrt(temp1 + temp2);//得到2,3的距离

						fac = (float) d1 / d2;
						if(fac<0.95||fac>1.05||d1==0||d2==0)//不合格
						{
							n = 0;
							LCD_ClearScreen(0xffff);//清屏
					 		Drow_Touch_Point(25,30);	  //左上角
						//	LCD_Font_Show_CN(1,60,"请点击屏幕 左上 角箭头中心.",0);
							continue;
						} //距离相等,校准成功

						LCD_ClearScreen(0xffff);//清屏

						Pen_Point.Touch_Calibration = 1;//完成了触摸屏校准

					   ////////////////校准完成////////////////////////////////////////////////////////////
					   //计算校准结果
						Pen_Point.xfac = (float) (LCD_XSIZE-50) * 1.0 / (buff[0][0] - buff[2][0]);//((750) / (buff[2][0] - buff[0][0]) + (240 - 25) / (buff[1][0] - buff[0][0])) / 2;
						Pen_Point.xoff = buff[1][0] * Pen_Point.xfac - LCD_XSIZE / 2;
						
						Pen_Point.yfac = (float) (LCD_YSIZE-60) * 1.0 / (buff[0][1] - buff[2][1]);
						Pen_Point.yoff = buff[1][1] * Pen_Point.yfac - LCD_YSIZE / 2;

					//	Wire_Touch();//存储校准结果
																			  
						//LCD_Font_Show_CN(20,140,"触摸屏校准完成!",0);
					//	Delay_MS(1000);
					//	LCD_Fill(0,239,140,160,0xffff);//清除文字
						return;
					} 
			}
		}
	}	
}



/*************************************************************************************************************************
*函数        :	void Touch_Convert_Pos(void)
*功能        :	转换坐标
*参数        :	无
*返回        :	FALSE:转换的坐标无效;TRUE:转换的坐标有效
*依赖     : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120730
*最后修改时间:	20120805
*说明        :	根据触摸屏的校准参数来决定转换后的结果,保存在X0,Y0中
*************************************************************************************************************************/
bool Touch_Convert_Pos(void)
{	
	u16 x,y;

	if(TOUCH_Read2ADC(&Pen_Point.X1,&Pen_Point.Y1) == TRUE)//读取坐标,有效
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





