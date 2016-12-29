/*************************************************************************************************************
 * 文件名:		TFT_LCD.c
 * 功能:		S3C6410 TFT_LCD底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年3月11日15:46
 * 最后修改时间:2012年5月6日
 * 详细:		TFT_LCD初始化以及相关绘图函数
*************************************************************************************************************/
#ifndef _TFT_LCD_H_
#define _TFT_LCD_H_
#include "system.h"

#define TFT50		1
#define TFT43		2

#define TFT_LCD_VER	TFT50

//显存分辨率
#if TFT_LCD_VER	== TFT50
#define LCD_XSIZE	800		//水平分辨率
#define LCD_YSIZE  	480		//垂直分辨率
#elif TFT_LCD_VER == TFT43
#define LCD_XSIZE	480		//水平分辨率
#define LCD_YSIZE  	272		//垂直分辨率
#endif

//定义显存
extern volatile u16 LCD_BUFFER0[LCD_YSIZE][LCD_XSIZE];	//窗口0,缓冲区0
extern volatile u16 LCD_BUFFER1[LCD_YSIZE][LCD_XSIZE];	//窗口1,缓冲区1
extern volatile u16 (*LCD_BUFFER)[LCD_XSIZE];	//当前缓冲区选择指针

//常见颜色
#define	BLACK	0X000000
#define	BLUE	0X0000FF
#define	GREEN	0X008000
#define	CYAN	0X00FFFF
#define	PURPLE	0X800080
#define	SKYBLUE	0X87CEEB
#define	BLOWN	0XA52A2A
#define	SILVER	0XC0C0C0
#define	RED		0XFF0000
#define	PINK	0XFFC0CB
#define	GOLD	0XFFD700
#define	YELLOW	0XFFFF00
#define	WHITE	0XFFFFFF

//RGB888,转RGB565
#define	RGB565(x) ((((x >> 16) & 0xff) >> 3) << 11) | ((((x >> 8) & 0xff) >> 2) << 5) | ((x & 0xff) >> 3)



void LCD_Init(void);
void LCD_ClearScreen(u16 Color);
//void LCD_DrawPoint(u16 x,u16 y,u16 Color);
void LCD_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE);
void Show_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE);
void Draw_Big_Point(u16 x,u16 y,u16 color);//画一个2*2的大点
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//在指定位置画一个矩形
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//画线函数
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//画圆函数
void LCD_Fill(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd,u16 color);//指定位置填充颜色
void LCD_China(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE);//在指定位置显示一个汉字指定大小的汉字 
void LCD_SetGRAMBuff(u8 BuffNum);	//当前显存选择
void LCD_SetWindows(u8 WinNum);		//选择当前显示窗口

#define SetLCD_Windows0()	(rWINCON0 &= ~BIT20)	//显示窗口0		
#define SetLCD_Windows1()	(rWINCON0 |= BIT20)		//显示窗口1

#define GetLCD_Windows()	((rWINCON0 & BIT21) ? 1 : 0)	//获取当前的显示窗口号

/*************************************************************************************************************************
*函数        :	__inline void  LCD_DrawPoint(u16 x,u16 y,u16 Color)
*功能        :	在指定位置画一个指定颜色的点
*参数        :	x,y:坐标;Color:颜色
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	陈鹏
*时间        :	20120311
*最后修改时间:	20120311
*说明        :	需要先初始化LCD,颜色值为RGB565,注意不要超出范围
*************************************************************************************************************************/
__inline void LCD_DrawPoint(u16 x,u16 y,u16 Color)
{
    if ( ( x < LCD_XSIZE) && ( y < LCD_YSIZE) )
    {
    		LCD_BUFFER[y][x] = Color;
    }
		
}

#endif
