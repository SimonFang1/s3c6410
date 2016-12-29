/*************************************************************************************************************
 * 文件名:	TFT_LCD.c
 * 功能:		S3C6410 TFT_LCD底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年3月11日15:46
 * 最后修改时间:2012年5月6日
 * 详细:		TFT_LCD初始化以及相关绘图函数
*************************************************************************************************************/
#include"system.h"
#include"tft_lcd.h"
#include "ASCII.H"

//定义显存
volatile u16 LCD_BUFFER0[LCD_YSIZE][LCD_XSIZE];	//窗口0,缓冲区0
volatile u16 LCD_BUFFER1[LCD_YSIZE][LCD_XSIZE];	//窗口1,缓冲区1
volatile u16 (*LCD_BUFFER)[LCD_XSIZE] = LCD_BUFFER0;	//当前缓冲区选择指针

const u8 FONT_H[6] = {16,16,13,16,13,16};//对应的字体宽和高


#if TFT_LCD_VER == TFT50
//液晶相关参数 5'TFT 800*480
//时序控制
//括号里面的为LCD时序要求的值,但是实际需要调整
//只要调整LCD_VSPW和LCD_HSPW就可以了
#define LCD_VBPD	30		//VSBP-VSPW(32-3)   	在帧开始处，垂直后沿是不活动行的数量，在垂直同步过程之后
#define LCD_VFPD	14		//VSFP(13)				在帧末尾处，垂直前沿是不活动行的数量，在垂直同步过程之前。
#define LCD_VSPW	5		//VSPW(3)				通过计算不活动行的数量，垂直同步脉冲宽度决定VSYNC 脉冲的高点品宽度。
#define LCD_HBPD	41		//HSBP-HSPW(88-48)		水平后沿是HSYNC 下降沿和有效数据开始之间的VCLK 周期的数量。
#define LCD_HFPD	41		//HSFP(40)				水平前沿是有效数据末端和HSYNC 上升沿之间的VCLK 周期的数量。
#define LCD_HSPW	175		//HSPW(48)				通过计算不VCLK 的数量，水平同步脉冲宽度决定HSYNC 脉冲的高点品宽度
#define LCD_LINEVAL 480		//垂直分辨率
#define LCD_HOZVAL	800		//水平分辨率
#define	LCD_BUF_SIZE	(LCD_XSIZE*LCD_YSIZE)		//缓冲区大小
#elif TFT_LCD_VER == TFT43
//液晶相关参数 4.3'TFT 800*480
//时序控制
//括号里面的为LCD时序要求的值,但是实际需要调整
//只要调整LCD_VSPW和LCD_HSPW就可以了
#define LCD_VBPD	1		//VSBP-VSPW(32-3)   	在帧开始处，垂直后沿是不活动行的数量，在垂直同步过程之后
#define LCD_VFPD	2		//VSFP(13)				在帧末尾处，垂直前沿是不活动行的数量，在垂直同步过程之前。
#define LCD_VSPW	11		//VSPW(3)				通过计算不活动行的数量，垂直同步脉冲宽度决定VSYNC 脉冲的高点品宽度。
#define LCD_HBPD	3		//HSBP-HSPW(88-48)		水平后沿是HSYNC 下降沿和有效数据开始之间的VCLK 周期的数量。
#define LCD_HFPD	2		//HSFP(40)				水平前沿是有效数据末端和HSYNC 上升沿之间的VCLK 周期的数量。
#define LCD_HSPW	40		//HSPW(48)				通过计算不VCLK 的数量，水平同步脉冲宽度决定HSYNC 脉冲的高点品宽度
#define LCD_LINEVAL 272		//垂直分辨率
#define LCD_HOZVAL	480		//水平分辨率
#define	LCD_BUF_SIZE	(LCD_XSIZE*LCD_YSIZE)
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//配置
//视频主控制器0寄存器VIDCON0
#define VIDCON0_INTERLACE_F		(0 << 29)	//渐行
#define VIDCON0_VIDOUT			(0 << 26)	//RGB接口
#define VIDCON0_L1_DATA16		(0 << 23)	//I80 (LDI1)输出16bit模式
#define VIDCON0_L0_DATA16		(0 << 20)	//I80 (LDI0)输出16bit模式	
#define VIDCON0_PNRMODE			(0 << 17)	//显示格式 RGB格式
#define VIDCON0_CLKVALUP		(1 << 16)	//选择CLKVAL_F 更新时序控制 ,帧开始时控制
#define VIDCON0_CLKVAL_F		(3 << 6)	//时钟分频,最大66MHz	VCLK=视频时钟源/（CLKVAL+1） CLKVAL>=1
#define VIDCON0_VCLKFREE		(1 << 5)	//VCLK 自由运行控制（只有在RGB IF 模式下有效）:自由运行模式
#define VIDCON0_CLKDIR			(1 << 4)	//用CLKVAL_F 寄存器划分时钟源
#define VIDCON0_CLKSEL_F		(0 << 2)	//选择时钟源,HCLK = 133MHz
#define VIDCON0_ENVID			(0 << 1)	//视频输出及显示控制信号禁止
#define VIDCON0_ENVID_F			(0 << 0)	//视频输出及显示控制信号禁止
//视频主控制器1寄存器VIDCON1
#if TFT_LCD_VER == TFT50
#define VIDCON1_IVCLK			(1 << 7)	//时钟上升沿有效
#define VIDCON1_IHSYNC			(1 << 6)	//HSYNC反向
#define VIDCON1_IVSYNC			(1 << 5)	//VSYNC反向
#define VIDCON1_IVDEN			(0 << 4)	//DE信号不反向
#elif TFT_LCD_VER == TFT43
#define VIDCON1_IVCLK			(0 << 7)	//时钟下降沿有效
#define VIDCON1_IHSYNC			(1 << 6)	//HSYNC反向
#define VIDCON1_IVSYNC			(1 << 5)	//VSYNC反向
#define VIDCON1_IVDEN			(0 << 4)	//DE信号不反向
#endif
//视频时序控制0寄存器VIDTCON0
#define VIDTCON0_VBPDE			(0 << 24)			//在帧开始处，垂直后沿是不活动行的数量，在垂直同步过程之后（只对YVU 接口）
#define VIDTCON0_VBPD			((LCD_VBPD - 1) << 16)
#define VIDTCON0_VFPD			((LCD_VFPD - 1) << 8)	
#define VIDTCON0_VSPW			((LCD_VSPW - 1) << 0)		
//视频时序控制1寄存器VIDTCON1
#define VIDTCON1_VFPDE			(0 << 24)			//在帧末尾处，垂直前沿是不活动行的数量，在垂直同步过程之前（只对YVU 接口）
#define VIDTCON1_HBPD			((LCD_HBPD - 1) << 16)
#define VIDTCON1_HFPD			((LCD_HFPD - 1) << 8)
#define VIDTCON1_HSPW			((LCD_HSPW - 1) << 0)
//视频时序控制2寄存器VIDTCON2
#define VIDTCON2_LINEVAL		((LCD_LINEVAL - 1) << 11)
#define VIDTCON2_HOZVAL			((LCD_HOZVAL - 1) << 0)
//窗口0控制寄存器WINCON0
#define WINCON0_nWide_Narrow	(0 << 16)		//根据输入值范围选择从YCbCr 到RGB 的颜色空间转换等式。YCbCr 宽范围是2’00, YCbCr 窄范围是2’11,宽范围：Y/Cb/Cr:2514-0窄范围:Y:2314-16,Cb/Cr:240-16
#define WINCON0_ENLOCAL			(0 << 22)		//选择数据访问方式:专用DMA
#define WINCON0_BUFSEL			(0 << 20)		//缓冲区设置为0
#define WINCON0_BUFAUTOEN		(0 << 19) 		//双缓冲区自动控制位,由BUFSEL控制
#define WINCON0_BITSWP			(0 << 18)		//位交换禁止
#define WINCON0_BYTSWP			(0 << 17)		//字节交换禁止
#define WINCON0_HAWSWP			(1 << 16)		//半子交换使能
#define WINCON0_InRGB			(0 << 13)		//此位指明源图像的输入颜色空间（只对ENLOCALI 有效）:RGB
#define WINCON0_BURSTLEN		(0 << 9)		//DMA 的突发最大长度选择:16BYTE
#define WINCON0_BPPMODE_F		(5 << 2)		//16bit RGB565
//窗口0位置控制A寄存器VIDOSD0A
#define VIDOSD0A_OSD_LeftTopX_F	(0 << 11)		//左上角的OSD 图像像素横向屏幕坐标
#define VIDOSD0A_OSD_LeftTopY_F (0 << 0)		//左上角的OSD 图像像素纵向屏幕坐标（对于接口TV输出而言，这个值必须设置在屏幕y 坐标值的一半位置。Y 坐标值的初始位置必须为偶数）
//窗口0位置控制B寄存器VIDOSD0B
#define VIDOSD0B_OSD_RightBotX_F	((LCD_HOZVAL - 1) << 11)	//右下角的OSD 图像像素横向屏幕坐标	
#define VIDOSD0B_OSD_RightBotY_F	((LCD_LINEVAL - 1) << 0) 	//右下角的OSD 图像像素纵向屏幕坐标（对于接口TV输出而言，这个值必须设置在屏幕y 坐标值的一半位置。Y 坐标值的初始位置必须为奇数）
//窗口0位置控制C寄存器VIDOSD0C
#define VIDOSD0C_OSDSIZE		(LCD_HOZVAL * LCD_LINEVAL)		//窗口尺寸=高*宽（字数)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////
//加入以下代码,支持printf函数
#if 0                  
//重定义fputc函数 
int fputc(int ch, FILE *f)
{    
	static u16 X;
	static u16 X1;
	static u16 Y;
	static u8 flag;
 	
 	if(flag == 0)
 	{
	 	if(Y > LCD_YSIZE-1-16)
		{
			
			Y = 0;
			X = X1 = LCD_XSIZE/2;
			flag = 1;
			LCD_Fill(X1,LCD_XSIZE-1,0,LCD_YSIZE-1,0xffff);	//清除右半屏
		}
 	}
 	else
 	{
	 	if(Y > LCD_YSIZE-1-16)
		{
			Y = 0;
			X = X1 = 0;
			flag = 0;
			LCD_Fill(0,LCD_XSIZE/2-1,0,LCD_YSIZE-1,0xffff);	//清除左半屏
		}
 	}
	
	
	if((u8)ch == '\n')
	{
	 	X = X1;
		Y += 12;
	}
	else if((u8)ch > 0x80) //汉字
	{
		return ch;
	}
	else
	{
		LCD_Char(X,Y,(u8 *)&ch,0x0000,0xffff,0x80);
		
		if(flag == 0)
		{
			if(X > LCD_XSIZE/2-1-8)
			{
			 	X = X1;
				Y += 12;
			}
			else
				X += 8;
		}
		else
		{
			if(X > LCD_XSIZE-1-8)
			{
			 	X = X1;
				Y += 12;
			}
			else
				X += 8;
		}
	}     

	return ch;
}
#endif 

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////







/**************************************************************
480*272 TFT LCD关闭输出
**************************************************************/
void LCD_Disable(void)
{
	rVIDCON0 &= (~(BIT1 | BIT0));	//视频输出和逻辑瞬态使能禁止+当前帧末尾视频输出和逻辑瞬态使能禁止
	rWINCON0 &= ~BIT0;				//禁止视频输出和VIDEO 控制信号
}

/**************************************************************
480*272 TFT LCD使能
**************************************************************/
void LCD_Enable(void)
{
	rVIDCON0 |= (BIT1 | BIT0);	//视频输出和逻辑瞬态使能+当前帧末尾视频输出和逻辑瞬态使能
	rWINCON0 |= BIT0;			//使能视频输出和VIDEO 控制信号
}

/*************************************************************************************************************************
*函数        :	void LCD_Init(void)
*功能        :	LCD初始化
*参数        :	无
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120311
*最后修改时间:	20120311
*说明        :	初始化TFT LCD
*************************************************************************************************************************/
void LCD_Init(void)
{
	u32 i;
   
   	//IO以及硬件初始化
	rGPICON	=	0XAAAAAAAA;
	rGPJCON	=	0XAAAAAAAA;
	rSPCON &= ~0x3;		// 将SPCON寄存器中，最低两位设为01，即RGB I/F style
	rSPCON |= 0x1;
	
	LCD_Disable();		//显示控制器失能
	S3C6410_SFR32(0x7410800C) &= ~(1 << 3);	 		// (1)MOFPCON:SEL_BYPASS[3] value@0x7410800C  必须设置为0	
	
	//视频主控制器0寄存器配置
	rVIDCON0 = VIDCON0_INTERLACE_F + VIDCON0_VIDOUT + VIDCON0_L1_DATA16 + VIDCON0_L0_DATA16 + VIDCON0_PNRMODE+\
		      VIDCON0_CLKVALUP + VIDCON0_CLKVAL_F + VIDCON0_VCLKFREE + VIDCON0_CLKDIR + VIDCON0_CLKSEL_F + \
			  VIDCON0_ENVID + VIDCON0_ENVID_F;
	//视频主控制器1寄存器配置
	rVIDCON1 = VIDCON1_IVCLK + VIDCON1_IHSYNC + VIDCON1_IVSYNC + VIDCON1_IVDEN;		  
	//视频时序控制0寄存器配置
	rVIDTCON0 = VIDTCON0_VBPDE + VIDTCON0_VBPD + VIDTCON0_VFPD + VIDTCON0_VSPW;
	//视频时序控制1寄存器配置
	rVIDTCON1 = VIDTCON1_VFPDE + VIDTCON1_HBPD + VIDTCON1_HFPD + VIDTCON1_HSPW;
	//视频时序控制2寄存器配置
	rVIDTCON2 = VIDTCON2_LINEVAL + VIDTCON2_HOZVAL;
	//窗口0控制寄存器配置
	rWINCON0 = WINCON0_nWide_Narrow + WINCON0_ENLOCAL + WINCON0_BUFSEL + WINCON0_BUFAUTOEN + WINCON0_BITSWP + WINCON0_BYTSWP + WINCON0_HAWSWP+\
			  WINCON0_InRGB + WINCON0_BURSTLEN + WINCON0_BPPMODE_F;
	//窗口0位置控制A寄存器配置
	rVIDOSD0A = VIDOSD0A_OSD_LeftTopX_F + VIDOSD0A_OSD_LeftTopY_F;
	//窗口0位置控制B寄存器配置
	rVIDOSD0B = VIDOSD0B_OSD_RightBotX_F + VIDOSD0B_OSD_RightBotY_F;
	//窗口0位置控制C寄存器配置
	rVIDOSD0C = VIDOSD0C_OSDSIZE;
	//窗口0缓冲0地址开始
	rVIDW00ADD0B0 = (u32)LCD_BUFFER0;
	//窗口0缓冲0地址结束
	rVIDW00ADD1B0 = (u32)LCD_BUFFER0 + LCD_BUF_SIZE + 1;
	//窗口0缓冲1地址开始
	rVIDW00ADD0B1 = (u32)LCD_BUFFER1;
	//窗口0缓冲1地址结束
	rVIDW00ADD1B1 = (u32)LCD_BUFFER1 + LCD_BUF_SIZE + 1;
	//帧缓冲地址2寄存器
	rVIDW00ADD2 = (0 << 13) | (LCD_HOZVAL * 2);//虚拟平面的页面宽度此值定义了帧内可可视端口的宽度。PAGEWIDTH 的值必须比突发尺寸大且在字边界范围内。

	LCD_Enable();	//显示使能
	LCD_SetGRAMBuff(0);	//默认使用第0个显存
	LCD_SetWindows(0);	//默认使用窗口0
	
	for(i = 0;i < 60000;i ++);		//加一个延时,否则可能会出错
	LCD_ClearScreen(0xffff);		//清屏
}


/*************************************************************************************************************************
*函数        :	void LCD_ClearScreen(u16 Color)
*功能        :	LCD清屏函数
*参数        :	Color:清屏颜色
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120311
*最后修改时间:	20120311
*说明        :	需要先初始化LCD,颜色值为RGB565
*************************************************************************************************************************/
void LCD_ClearScreen(u16 Color)
{
	u16 x,y ;

	for( x = 0 ; x <  LCD_XSIZE; x++ )
    { 
    	for( y = 0 ; y < LCD_YSIZE ; y++ )
    	{
    		LCD_BUFFER[y][x] = Color;
    	}
    }
}




/*************************************************************************************************************************
* 函数	:	void LCD_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 MODE)
* 功能	:	在指定位置显示一个ASCII字符 
* 参数	:	开始坐标,字符,字体颜色,背景颜色,显示模式以及字体,
* 返回	:	无
* 依赖	:	画点函数
* 作者	:	陈鹏
* 时间	:	20110920
* 最后修改时间 : 20110920
* 说明	: 	Font_MODE最高位为1表示叠加显示,其它位用来选择字体
*************************************************************************************************************************/
void LCD_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE)
{
 	const u8 *font_add;//用来选择不同的字体对应字库地址
	u8 i,j;
	u8 fh;//字高
	switch (FONT_H[Font_MODE & 0x7f])
	{
	 	case 16:fh = 16;font_add = Song_16[*p - 32];break;//选择字体高度为16,字宽为8,宋体正常字体;
		case 13:fh = 13;font_add = Song_13[*p - 32];break;//选择字体高度为13,字宽为8,宋体正常字体;
		default : fh = 16;font_add = Song_16[*p - 32];break;//默认选择字体高度为16,字宽为8,宋体正常字体;
	}

 	for(i = 0;i < fh;i ++)
	{
	 	for(j = 0;j < 8;j ++)
		{
		 	if(font_add[i] & 0x80 >> j)
				LCD_DrawPoint(x + j,y + i,Font_color);//字的颜色
			else 
			{
				if(!(Font_MODE & 0x80))//非叠加显示
					LCD_DrawPoint(x + j,y + i,Back_color);//背景颜色
			}
		}
	}
}

/*************************************************************************************************************************
* 函数	:	void Show_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 MODE)
* 功能	:	在指定位置显示ASCII字符串 
* 参数	:	开始坐标,字符串,字体颜色,背景颜色,显示模式以及字体,
* 返回	:	无
* 依赖	:	void Show_Char(u16 x,u16 y,char *p,u16 Font_color,u16 Back_color,u8 MODE);//单个字符显示函数
* 作者	:	陈鹏
* 时间	:	20110920
* 最后修改时间 : 20110920
* 说明	: 	Font_最高位为1表示叠加显示,其它位用来选择字体
*************************************************************************************************************************/
void Show_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE)
{
 	while(*p != 0x00)//判断是否到达字符串结尾
	{
		if(x > LCD_XSIZE-1-8)//判断是否需要换行
		{
		 	x = 0;
			y += 16;
		}
		if (*p == '\n')
		{
		 	y += 16;
			x = 0;
			p ++;
		}
	 	LCD_Char(x,y,p,Font_color,Back_color,Font_MODE);
		p ++;		
		x += 8;
	}
}





/*************************************************************************************************************************
* 函数	:	void LCD_China(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE)
* 功能	:	在指定位置显示一个汉字指定大小的汉字 
* 参数	:	开始坐标,汉字点阵数组位置,字体颜色,背景颜色,显示模式以及字体,
* 返回	:	无
* 依赖	:	画点函数
* 作者	:	陈鹏
* 时间	:	20110920
* 最后修改时间 : 20110920
* 说明	: 	Font_最高位为1表示叠加显示,其它位用来选择字体高度
*************************************************************************************************************************/
void LCD_China(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE)
{
	u8 i,j;
	u8 fhl;//字宽高

	fhl = FONT_H[Font_MODE & 0x7f];

	if(Font_MODE & 0x80)//叠加显示
	{
	 	for(i = 0;i < fhl;i ++)
		{
		 	for(j = 0;j < 8;j ++)
			{
			 	if(*p & 0x80 >> j)
					LCD_DrawPoint(x + j,y + i,Font_color);//字的颜色
			}
			p ++;
			for(j = 0;j < fhl - 8;j ++)
			{
				if(*p & 0x80 >> j)
					LCD_DrawPoint(x + j + 8,y + i,Font_color);//字的颜色
			}
			p ++;
		}
	}
	else //非叠加显示
	{
		for(i = 0;i < fhl;i ++)
		{
		 	for(j = 0;j < 8;j ++)
			{
			 	if(*p & 0x80 >> j)
					LCD_DrawPoint(x + j,y + i,Font_color);//字的颜色
				else
					LCD_DrawPoint(x + j,y + i,Back_color);//背景颜色
			}
			p ++;
			for(j = 0;j < fhl - 8;j ++)
			{
				if(*p & 0x80 >> j)
					LCD_DrawPoint(x + j + 8,y + i,Font_color);//字的颜色
				else
					LCD_DrawPoint(x + j + 8,y + i,Back_color);//背景颜色
			}
			p ++;
		}
	}  
}



/*************************************************************************************************************************
* 函数	:	void LCD_Fill(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd,u16 color)
* 功能	:	指定位置填充颜色
* 参数	:	位置坐标,颜色值
* 返回	:	无
* 依赖	:	底层函数
* 作者	:	陈鹏
* 时间	:	20110920
* 最后修改时间 : 20110920
* 说明	: 	无
*************************************************************************************************************************/
void LCD_Fill(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd,u16 color)
{
 	u16 i,j;
 	
	for(i = yStart;i < yEnd + 1; i ++)
	{
		for(j = xStart;j < xEnd + 1;j ++)
		{
			LCD_DrawPoint(j,i,color);
		}
	}
}




								
/*************************************************************************************************************************
* 函数	:	void Draw_Circle(u16 x0,u16 y0,u8 r)
* 功能	:	画圆函数
* 参数	:	圆心坐标,半径
* 返回	:	无
* 依赖	:	画点函数
* 作者	:	陈鹏
* 时间	:	20110920
* 最后修改时间 : 20110920
* 说明	: 	无
*************************************************************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a = 0,b = r;
	int di;
	di = 3 -(r << 1);             //判断下个点位置的标志
//	LCD_DrawPoint(x0,y0,color);             //中心 
	while(a <= b)
	{
		LCD_DrawPoint(x0 - b,y0 - a,color);             //3           
		LCD_DrawPoint(x0 + b,y0 - a,color);             //0           
		LCD_DrawPoint(x0 - a,y0 + b,color);             //1       
		LCD_DrawPoint(x0 - b,y0 - a,color);             //7           
		LCD_DrawPoint(x0 - a,y0 - b,color);             //2             
		LCD_DrawPoint(x0 + b,y0 + a,color);             //4               
		LCD_DrawPoint(x0 + a,y0 - b,color);             //5
		LCD_DrawPoint(x0 + a,y0 + b,color);             //6 
		LCD_DrawPoint(x0 - b,y0 + a,color);             //
		a ++;
		//使用Bresenham算法画圆     
		if(di<0)
			di += 4 * a + 6;	  									   
		else
		{
			di += 10 + 4 * (a - b);   
			b --;
		} 									   	
	}										  
}
																	  
/*************************************************************************************************************************
* 函数	:	void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
* 功能	:	画线函数
* 参数	:	起点终点坐标
* 返回	:	无
* 依赖	:	画点函数
* 作者	:	陈鹏
* 时间	:	20110920
* 最后修改时间 : 20110920
* 说明	: 	无
*************************************************************************************************************************/ 
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol;
	
	//TFT_LCD_SetRamAddr(0,239,0,319);//设置显示窗口 

	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 												
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}
									 					   
/*************************************************************************************************************************
* 函数	:	void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
* 功能	:	在指定位置画一个矩形
* 参数	:	多边形的两个坐标
* 返回	:	无
* 依赖	:	画线函数
* 作者	:	陈鹏
* 时间	:	20110920
* 最后修改时间 : 20110920
* 说明	: 	无
*************************************************************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}								  	 

/*************************************************************************************************************************
* 函数	:	void Draw_Big_Point(u16 x,u16 y,u16 color)
* 功能	:	画一个2*2的大点
* 参数	:	点的中心坐标
* 返回	:	无
* 依赖	:	画点函数
* 作者	:	陈鹏
* 时间	:	20110920
* 最后修改时间 : 20110920
* 说明	: 	无
*************************************************************************************************************************/	   
void Draw_Big_Point(u16 x,u16 y,u16 color)
{	
	//TFT_LCD_SetRamAddr(0,239,0,319);     
	LCD_DrawPoint(x,y,color);//中心点 
	LCD_DrawPoint(x+1,y,color);
	LCD_DrawPoint(x,y+1,color);
	LCD_DrawPoint(x+1,y+1,color);	 	  	
}


/*************************************************************************************************************************
*函数        :	void LCD_SetGRAMBuff(u8 BuffNum)
*功能        :	选择当前显存
*参数        :	BuffNum:0:显存0;1:显存1
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120729
*最后修改时间:	20120729
*说明        :	初始化后一定要选择显存,否则会出现非法访问等问题
*************************************************************************************************************************/
void LCD_SetGRAMBuff(u8 BuffNum)
{
	switch(BuffNum)
	{
		case 0:
			LCD_BUFFER = LCD_BUFFER0;	//选择显存0
			break;
		case 1:
			LCD_BUFFER = LCD_BUFFER1;	//选择显存1
			break;
		default :
			break;	//超出范围
	}	
}


/*************************************************************************************************************************
*函数        :	void LCD_SetWindows(u8 WinNum)
*功能        :	选择当前显示窗口
*参数        :	WinNum:0:窗口0;1:窗口1
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120729
*最后修改时间:	20120729
*说明        :	初始化后一定要选择显存,否则会出现非法访问等问题
*************************************************************************************************************************/
void LCD_SetWindows(u8 WinNum)
{
	switch(WinNum)
	{
		case 0:
			SetLCD_Windows0();	//选择显示窗口0
			break;
		case 1:
			SetLCD_Windows1();	//选择显示窗口1
			break;
		default :
			break;	//超出范围
	}	
}	
