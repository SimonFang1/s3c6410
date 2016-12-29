/*************************************************************************************************************
 * �ļ���:		TFT_LCD.c
 * ����:		S3C6410 TFT_LCD�ײ���������
 * ����:		����
 * ����ʱ��:	2012��3��11��15:46
 * ����޸�ʱ��:2012��5��6��
 * ��ϸ:		TFT_LCD��ʼ���Լ���ػ�ͼ����
*************************************************************************************************************/
#ifndef _TFT_LCD_H_
#define _TFT_LCD_H_
#include "system.h"

#define TFT50		1
#define TFT43		2

#define TFT_LCD_VER	TFT50

//�Դ�ֱ���
#if TFT_LCD_VER	== TFT50
#define LCD_XSIZE	800		//ˮƽ�ֱ���
#define LCD_YSIZE  	480		//��ֱ�ֱ���
#elif TFT_LCD_VER == TFT43
#define LCD_XSIZE	480		//ˮƽ�ֱ���
#define LCD_YSIZE  	272		//��ֱ�ֱ���
#endif

//�����Դ�
extern volatile u16 LCD_BUFFER0[LCD_YSIZE][LCD_XSIZE];	//����0,������0
extern volatile u16 LCD_BUFFER1[LCD_YSIZE][LCD_XSIZE];	//����1,������1
extern volatile u16 (*LCD_BUFFER)[LCD_XSIZE];	//��ǰ������ѡ��ָ��

//������ɫ
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

//RGB888,תRGB565
#define	RGB565(x) ((((x >> 16) & 0xff) >> 3) << 11) | ((((x >> 8) & 0xff) >> 2) << 5) | ((x & 0xff) >> 3)



void LCD_Init(void);
void LCD_ClearScreen(u16 Color);
//void LCD_DrawPoint(u16 x,u16 y,u16 Color);
void LCD_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE);
void Show_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE);
void Draw_Big_Point(u16 x,u16 y,u16 color);//��һ��2*2�Ĵ��
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//��ָ��λ�û�һ������
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);//���ߺ���
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color);//��Բ����
void LCD_Fill(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd,u16 color);//ָ��λ�������ɫ
void LCD_China(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE);//��ָ��λ����ʾһ������ָ����С�ĺ��� 
void LCD_SetGRAMBuff(u8 BuffNum);	//��ǰ�Դ�ѡ��
void LCD_SetWindows(u8 WinNum);		//ѡ��ǰ��ʾ����

#define SetLCD_Windows0()	(rWINCON0 &= ~BIT20)	//��ʾ����0		
#define SetLCD_Windows1()	(rWINCON0 |= BIT20)		//��ʾ����1

#define GetLCD_Windows()	((rWINCON0 & BIT21) ? 1 : 0)	//��ȡ��ǰ����ʾ���ں�

/*************************************************************************************************************************
*����        :	__inline void  LCD_DrawPoint(u16 x,u16 y,u16 Color)
*����        :	��ָ��λ�û�һ��ָ����ɫ�ĵ�
*����        :	x,y:����;Color:��ɫ
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120311
*����޸�ʱ��:	20120311
*˵��        :	��Ҫ�ȳ�ʼ��LCD,��ɫֵΪRGB565,ע�ⲻҪ������Χ
*************************************************************************************************************************/
__inline void LCD_DrawPoint(u16 x,u16 y,u16 Color)
{
    if ( ( x < LCD_XSIZE) && ( y < LCD_YSIZE) )
    {
    		LCD_BUFFER[y][x] = Color;
    }
		
}

#endif
