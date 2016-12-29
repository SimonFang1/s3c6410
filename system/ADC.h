/*************************************************************************************************************
 * 文件名:	ADC.h
 * 功能:		S3C6410 触摸屏ADC底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年3月12日21:05
 * 最后修改时间:2012年3月12日
 * 详细:		触摸屏驱动以及相关ADC驱动
*************************************************************************************************************/

#ifndef	_ADC_H_
#define _ADC_H_

//按键状态	 
#define TOUCH_DOWN 0
#define TOUCH_UP   1
#define TOUCH_Posedge 0x01//笔抬起事件
#define TOUCH_Negedge 0x02//笔按下事件

//ADC采样模式
#define COMMON_AD_MODER		0	//普通的AD转换模式
#define ASUNDER_X_MODER		1	//分离的X采样模式
#define ASUNDER_Y_MODER		2	//分离的Y采样模式
#define AUTO_XY_MODER		3	//自动的X,Y采样模式//不知道为何不准,只能用分离的X,Y扫描模式
#define INT_AD_MODER		4	//等待中断的采样模式
#define STANDBY_AD_MODER	5	//掉电模式

//ADC通道选择
#define ADC_CH_TOUCHX	0	//X通道
#define ADC_CH_TOUCHY	1	//Y通道


//笔杆结构体
typedef struct 
{
	u16 X0;//有效坐标
	u16 Y0;
	u16 X1; //暂存坐标
	u16 Y1;						   	    
	u8 Touch_Calibration;//触摸屏校准,为1表示已经进行过校准,为0需要重新校准		  
//触摸屏校准参数
	float xfac;
	float yfac;
	short xoff;
	short yoff;
}Pen_Holder;	   
extern Pen_Holder Pen_Point;//定义一个笔杆的结构变量

void TOUCH_Init(FunctionalState EnableInt);	//触摸屏初始化
void ADC_SetMode(u8 Mode);	//设置模式
u16 TOUCH_ReadADC(u8 TouchCH);	//读取坐标,平均,第1层读取
bool TOUCH_Read2ADC(u16 *X,u16 *Y);//读取坐标,防止跳变,第2层读取
void Touch_Adjust(void);	//触摸屏校准
bool Touch_Convert_Pos(void);	//转换坐标
u8 Get_TouchState(void);	//获取按键状态






//使能笔抬起中断
__inline void ADCTSC_UD_SEN (u8 EN)
{
	ADC->TSC &= ~BIT8;			//笔向下中断
	if(EN == Enable)
		ADC->TSC |= BIT8;
}


//YM接GMD无效,接HZ
__inline void ADCTSC_YM_HZ(void)
{
	ADC->TSC &= ~BIT7;		
}

//YM接GMD有效,接GND
__inline void ADCTSC_YM_GND(void)
{
	ADC->TSC |= BIT7;		
}


//YP接VDD无效,接HZ
__inline void ADCTSC_YP_HZ(void)
{
	ADC->TSC |= BIT6;		
}

//YP接VDD有效,接VDDA
__inline void ADCTSC_YP_VDD(void)
{
	ADC->TSC &= ~BIT6;		
}


//XM接GMD无效,接HZ
__inline void ADCTSC_XM_HZ(void)
{
	ADC->TSC &= ~BIT5;		
}

//XM接GMD有效,接GND
__inline void ADCTSC_XM_GND(void)
{
	ADC->TSC |= BIT5;		
}


//XP接VDD无效,接HZ
__inline void ADCTSC_XP_HZ(void)
{
	ADC->TSC |= BIT4;		
}

//XP接VDD有效,接VDDA
__inline void ADCTSC_XP_VDD(void)
{
	ADC->TSC &= ~BIT4;		
}


//正常ADC转换模式
__inline void Normal_ADC_Mode(void)
{
	ADC->TSC &= ~BIT2;	//正常的ADC转换
}

//待机模式
__inline void StandbyMode(void)
{
	ADC->CON |= BIT2;	
}


//自动X,Y转换
__inline void AUTO_XYPosition(void)
{
	ADC->TSC &= ~(BIT0+BIT1);
	ADC->TSC |= BIT2;	//自动X,Y转换
}

//X,Y手动测量模式,没有运行模式
__inline void NO_OpeMode(void)
{
	ADC->TSC &= ~(BIT0+BIT1);
}

//X,Y手动测量模式,X坐标转换模式
__inline void X_PosMode(void)
{
	NO_OpeMode();			//清除设置
	ADC->TSC |= 1;
}

//X,Y手动测量模式,Y坐标转换模式
__inline void Y_PosMode(void)
{
	NO_OpeMode();			//清除设置
	ADC->TSC |= 2;
}

//X,Y手动测量模式,等待中断模式
__inline void INT_WaitingMode(void)
{
	NO_OpeMode();			//清除设置
	ADC->TSC |= 3;
}


//XP上拉启动
__inline void XP_UP_ENABLE(void)
{
	ADC->TSC &= ~BIT3;
}


//XP上拉禁止
__inline void XP_UP_DISABLE(void)
{
	ADC->TSC |= BIT3;
}


//清除ADC唤醒中断
__inline void ADC_ClearInt(void)
{
	ADC->CLRINT = 0xffffffff;		//写入任何值清除中断标志
}


//清除触摸屏中断中断
__inline void TOUCH_ClearInt(void)
{
	ADC->UPDN = 0;						//清除ADC的触摸屏UP-DOWN寄存器
	ADC->CLRINTPNDNUP = 0xffffffff;		//写入任何值清除中断标志
}


//开始一次ADC转换
__inline void ADC_Start(void)
{
	ADC->CON |= BIT0;	//开始ADC转换
}


//等待ADC转换完成
__inline void ADC_Wait(void)
{
	while(!(ADC->CON & BIT15));
}


#endif
