/*************************************************************************************************************
 * �ļ���:	TFT_LCD.c
 * ����:		S3C6410 TFT_LCD�ײ���������
 * ����:		����
 * ����ʱ��:	2012��3��11��15:46
 * ����޸�ʱ��:2012��5��6��
 * ��ϸ:		TFT_LCD��ʼ���Լ���ػ�ͼ����
*************************************************************************************************************/
#include"system.h"
#include"tft_lcd.h"
#include "ASCII.H"

//�����Դ�
volatile u16 LCD_BUFFER0[LCD_YSIZE][LCD_XSIZE];	//����0,������0
volatile u16 LCD_BUFFER1[LCD_YSIZE][LCD_XSIZE];	//����1,������1
volatile u16 (*LCD_BUFFER)[LCD_XSIZE] = LCD_BUFFER0;	//��ǰ������ѡ��ָ��

const u8 FONT_H[6] = {16,16,13,16,13,16};//��Ӧ�������͸�


#if TFT_LCD_VER == TFT50
//Һ����ز��� 5'TFT 800*480
//ʱ�����
//���������ΪLCDʱ��Ҫ���ֵ,����ʵ����Ҫ����
//ֻҪ����LCD_VSPW��LCD_HSPW�Ϳ�����
#define LCD_VBPD	30		//VSBP-VSPW(32-3)   	��֡��ʼ������ֱ�����ǲ���е��������ڴ�ֱͬ������֮��
#define LCD_VFPD	14		//VSFP(13)				��֡ĩβ������ֱǰ���ǲ���е��������ڴ�ֱͬ������֮ǰ��
#define LCD_VSPW	5		//VSPW(3)				ͨ�����㲻��е���������ֱͬ�������Ⱦ���VSYNC ����ĸߵ�Ʒ��ȡ�
#define LCD_HBPD	41		//HSBP-HSPW(88-48)		ˮƽ������HSYNC �½��غ���Ч���ݿ�ʼ֮���VCLK ���ڵ�������
#define LCD_HFPD	41		//HSFP(40)				ˮƽǰ������Ч����ĩ�˺�HSYNC ������֮���VCLK ���ڵ�������
#define LCD_HSPW	175		//HSPW(48)				ͨ�����㲻VCLK ��������ˮƽͬ�������Ⱦ���HSYNC ����ĸߵ�Ʒ���
#define LCD_LINEVAL 480		//��ֱ�ֱ���
#define LCD_HOZVAL	800		//ˮƽ�ֱ���
#define	LCD_BUF_SIZE	(LCD_XSIZE*LCD_YSIZE)		//��������С
#elif TFT_LCD_VER == TFT43
//Һ����ز��� 4.3'TFT 800*480
//ʱ�����
//���������ΪLCDʱ��Ҫ���ֵ,����ʵ����Ҫ����
//ֻҪ����LCD_VSPW��LCD_HSPW�Ϳ�����
#define LCD_VBPD	1		//VSBP-VSPW(32-3)   	��֡��ʼ������ֱ�����ǲ���е��������ڴ�ֱͬ������֮��
#define LCD_VFPD	2		//VSFP(13)				��֡ĩβ������ֱǰ���ǲ���е��������ڴ�ֱͬ������֮ǰ��
#define LCD_VSPW	11		//VSPW(3)				ͨ�����㲻��е���������ֱͬ�������Ⱦ���VSYNC ����ĸߵ�Ʒ��ȡ�
#define LCD_HBPD	3		//HSBP-HSPW(88-48)		ˮƽ������HSYNC �½��غ���Ч���ݿ�ʼ֮���VCLK ���ڵ�������
#define LCD_HFPD	2		//HSFP(40)				ˮƽǰ������Ч����ĩ�˺�HSYNC ������֮���VCLK ���ڵ�������
#define LCD_HSPW	40		//HSPW(48)				ͨ�����㲻VCLK ��������ˮƽͬ�������Ⱦ���HSYNC ����ĸߵ�Ʒ���
#define LCD_LINEVAL 272		//��ֱ�ֱ���
#define LCD_HOZVAL	480		//ˮƽ�ֱ���
#define	LCD_BUF_SIZE	(LCD_XSIZE*LCD_YSIZE)
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//����
//��Ƶ��������0�Ĵ���VIDCON0
#define VIDCON0_INTERLACE_F		(0 << 29)	//����
#define VIDCON0_VIDOUT			(0 << 26)	//RGB�ӿ�
#define VIDCON0_L1_DATA16		(0 << 23)	//I80 (LDI1)���16bitģʽ
#define VIDCON0_L0_DATA16		(0 << 20)	//I80 (LDI0)���16bitģʽ	
#define VIDCON0_PNRMODE			(0 << 17)	//��ʾ��ʽ RGB��ʽ
#define VIDCON0_CLKVALUP		(1 << 16)	//ѡ��CLKVAL_F ����ʱ����� ,֡��ʼʱ����
#define VIDCON0_CLKVAL_F		(3 << 6)	//ʱ�ӷ�Ƶ,���66MHz	VCLK=��Ƶʱ��Դ/��CLKVAL+1�� CLKVAL>=1
#define VIDCON0_VCLKFREE		(1 << 5)	//VCLK �������п��ƣ�ֻ����RGB IF ģʽ����Ч��:��������ģʽ
#define VIDCON0_CLKDIR			(1 << 4)	//��CLKVAL_F �Ĵ�������ʱ��Դ
#define VIDCON0_CLKSEL_F		(0 << 2)	//ѡ��ʱ��Դ,HCLK = 133MHz
#define VIDCON0_ENVID			(0 << 1)	//��Ƶ�������ʾ�����źŽ�ֹ
#define VIDCON0_ENVID_F			(0 << 0)	//��Ƶ�������ʾ�����źŽ�ֹ
//��Ƶ��������1�Ĵ���VIDCON1
#if TFT_LCD_VER == TFT50
#define VIDCON1_IVCLK			(1 << 7)	//ʱ����������Ч
#define VIDCON1_IHSYNC			(1 << 6)	//HSYNC����
#define VIDCON1_IVSYNC			(1 << 5)	//VSYNC����
#define VIDCON1_IVDEN			(0 << 4)	//DE�źŲ�����
#elif TFT_LCD_VER == TFT43
#define VIDCON1_IVCLK			(0 << 7)	//ʱ���½�����Ч
#define VIDCON1_IHSYNC			(1 << 6)	//HSYNC����
#define VIDCON1_IVSYNC			(1 << 5)	//VSYNC����
#define VIDCON1_IVDEN			(0 << 4)	//DE�źŲ�����
#endif
//��Ƶʱ�����0�Ĵ���VIDTCON0
#define VIDTCON0_VBPDE			(0 << 24)			//��֡��ʼ������ֱ�����ǲ���е��������ڴ�ֱͬ������֮��ֻ��YVU �ӿڣ�
#define VIDTCON0_VBPD			((LCD_VBPD - 1) << 16)
#define VIDTCON0_VFPD			((LCD_VFPD - 1) << 8)	
#define VIDTCON0_VSPW			((LCD_VSPW - 1) << 0)		
//��Ƶʱ�����1�Ĵ���VIDTCON1
#define VIDTCON1_VFPDE			(0 << 24)			//��֡ĩβ������ֱǰ���ǲ���е��������ڴ�ֱͬ������֮ǰ��ֻ��YVU �ӿڣ�
#define VIDTCON1_HBPD			((LCD_HBPD - 1) << 16)
#define VIDTCON1_HFPD			((LCD_HFPD - 1) << 8)
#define VIDTCON1_HSPW			((LCD_HSPW - 1) << 0)
//��Ƶʱ�����2�Ĵ���VIDTCON2
#define VIDTCON2_LINEVAL		((LCD_LINEVAL - 1) << 11)
#define VIDTCON2_HOZVAL			((LCD_HOZVAL - 1) << 0)
//����0���ƼĴ���WINCON0
#define WINCON0_nWide_Narrow	(0 << 16)		//��������ֵ��Χѡ���YCbCr ��RGB ����ɫ�ռ�ת����ʽ��YCbCr ��Χ��2��00, YCbCr խ��Χ��2��11,��Χ��Y/Cb/Cr:2514-0խ��Χ:Y:2314-16,Cb/Cr:240-16
#define WINCON0_ENLOCAL			(0 << 22)		//ѡ�����ݷ��ʷ�ʽ:ר��DMA
#define WINCON0_BUFSEL			(0 << 20)		//����������Ϊ0
#define WINCON0_BUFAUTOEN		(0 << 19) 		//˫�������Զ�����λ,��BUFSEL����
#define WINCON0_BITSWP			(0 << 18)		//λ������ֹ
#define WINCON0_BYTSWP			(0 << 17)		//�ֽڽ�����ֹ
#define WINCON0_HAWSWP			(1 << 16)		//���ӽ���ʹ��
#define WINCON0_InRGB			(0 << 13)		//��λָ��Դͼ���������ɫ�ռ䣨ֻ��ENLOCALI ��Ч��:RGB
#define WINCON0_BURSTLEN		(0 << 9)		//DMA ��ͻ����󳤶�ѡ��:16BYTE
#define WINCON0_BPPMODE_F		(5 << 2)		//16bit RGB565
//����0λ�ÿ���A�Ĵ���VIDOSD0A
#define VIDOSD0A_OSD_LeftTopX_F	(0 << 11)		//���Ͻǵ�OSD ͼ�����غ�����Ļ����
#define VIDOSD0A_OSD_LeftTopY_F (0 << 0)		//���Ͻǵ�OSD ͼ������������Ļ���꣨���ڽӿ�TV������ԣ����ֵ������������Ļy ����ֵ��һ��λ�á�Y ����ֵ�ĳ�ʼλ�ñ���Ϊż����
//����0λ�ÿ���B�Ĵ���VIDOSD0B
#define VIDOSD0B_OSD_RightBotX_F	((LCD_HOZVAL - 1) << 11)	//���½ǵ�OSD ͼ�����غ�����Ļ����	
#define VIDOSD0B_OSD_RightBotY_F	((LCD_LINEVAL - 1) << 0) 	//���½ǵ�OSD ͼ������������Ļ���꣨���ڽӿ�TV������ԣ����ֵ������������Ļy ����ֵ��һ��λ�á�Y ����ֵ�ĳ�ʼλ�ñ���Ϊ������
//����0λ�ÿ���C�Ĵ���VIDOSD0C
#define VIDOSD0C_OSDSIZE		(LCD_HOZVAL * LCD_LINEVAL)		//���ڳߴ�=��*������)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





//////////////////////////////////////////////////////////////////
//�������´���,֧��printf����
#if 0                  
//�ض���fputc���� 
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
			LCD_Fill(X1,LCD_XSIZE-1,0,LCD_YSIZE-1,0xffff);	//����Ұ���
		}
 	}
 	else
 	{
	 	if(Y > LCD_YSIZE-1-16)
		{
			Y = 0;
			X = X1 = 0;
			flag = 0;
			LCD_Fill(0,LCD_XSIZE/2-1,0,LCD_YSIZE-1,0xffff);	//��������
		}
 	}
	
	
	if((u8)ch == '\n')
	{
	 	X = X1;
		Y += 12;
	}
	else if((u8)ch > 0x80) //����
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
480*272 TFT LCD�ر����
**************************************************************/
void LCD_Disable(void)
{
	rVIDCON0 &= (~(BIT1 | BIT0));	//��Ƶ������߼�˲̬ʹ�ܽ�ֹ+��ǰ֡ĩβ��Ƶ������߼�˲̬ʹ�ܽ�ֹ
	rWINCON0 &= ~BIT0;				//��ֹ��Ƶ�����VIDEO �����ź�
}

/**************************************************************
480*272 TFT LCDʹ��
**************************************************************/
void LCD_Enable(void)
{
	rVIDCON0 |= (BIT1 | BIT0);	//��Ƶ������߼�˲̬ʹ��+��ǰ֡ĩβ��Ƶ������߼�˲̬ʹ��
	rWINCON0 |= BIT0;			//ʹ����Ƶ�����VIDEO �����ź�
}

/*************************************************************************************************************************
*����        :	void LCD_Init(void)
*����        :	LCD��ʼ��
*����        :	��
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120311
*����޸�ʱ��:	20120311
*˵��        :	��ʼ��TFT LCD
*************************************************************************************************************************/
void LCD_Init(void)
{
	u32 i;
   
   	//IO�Լ�Ӳ����ʼ��
	rGPICON	=	0XAAAAAAAA;
	rGPJCON	=	0XAAAAAAAA;
	rSPCON &= ~0x3;		// ��SPCON�Ĵ����У������λ��Ϊ01����RGB I/F style
	rSPCON |= 0x1;
	
	LCD_Disable();		//��ʾ������ʧ��
	S3C6410_SFR32(0x7410800C) &= ~(1 << 3);	 		// (1)MOFPCON:SEL_BYPASS[3] value@0x7410800C  ��������Ϊ0	
	
	//��Ƶ��������0�Ĵ�������
	rVIDCON0 = VIDCON0_INTERLACE_F + VIDCON0_VIDOUT + VIDCON0_L1_DATA16 + VIDCON0_L0_DATA16 + VIDCON0_PNRMODE+\
		      VIDCON0_CLKVALUP + VIDCON0_CLKVAL_F + VIDCON0_VCLKFREE + VIDCON0_CLKDIR + VIDCON0_CLKSEL_F + \
			  VIDCON0_ENVID + VIDCON0_ENVID_F;
	//��Ƶ��������1�Ĵ�������
	rVIDCON1 = VIDCON1_IVCLK + VIDCON1_IHSYNC + VIDCON1_IVSYNC + VIDCON1_IVDEN;		  
	//��Ƶʱ�����0�Ĵ�������
	rVIDTCON0 = VIDTCON0_VBPDE + VIDTCON0_VBPD + VIDTCON0_VFPD + VIDTCON0_VSPW;
	//��Ƶʱ�����1�Ĵ�������
	rVIDTCON1 = VIDTCON1_VFPDE + VIDTCON1_HBPD + VIDTCON1_HFPD + VIDTCON1_HSPW;
	//��Ƶʱ�����2�Ĵ�������
	rVIDTCON2 = VIDTCON2_LINEVAL + VIDTCON2_HOZVAL;
	//����0���ƼĴ�������
	rWINCON0 = WINCON0_nWide_Narrow + WINCON0_ENLOCAL + WINCON0_BUFSEL + WINCON0_BUFAUTOEN + WINCON0_BITSWP + WINCON0_BYTSWP + WINCON0_HAWSWP+\
			  WINCON0_InRGB + WINCON0_BURSTLEN + WINCON0_BPPMODE_F;
	//����0λ�ÿ���A�Ĵ�������
	rVIDOSD0A = VIDOSD0A_OSD_LeftTopX_F + VIDOSD0A_OSD_LeftTopY_F;
	//����0λ�ÿ���B�Ĵ�������
	rVIDOSD0B = VIDOSD0B_OSD_RightBotX_F + VIDOSD0B_OSD_RightBotY_F;
	//����0λ�ÿ���C�Ĵ�������
	rVIDOSD0C = VIDOSD0C_OSDSIZE;
	//����0����0��ַ��ʼ
	rVIDW00ADD0B0 = (u32)LCD_BUFFER0;
	//����0����0��ַ����
	rVIDW00ADD1B0 = (u32)LCD_BUFFER0 + LCD_BUF_SIZE + 1;
	//����0����1��ַ��ʼ
	rVIDW00ADD0B1 = (u32)LCD_BUFFER1;
	//����0����1��ַ����
	rVIDW00ADD1B1 = (u32)LCD_BUFFER1 + LCD_BUF_SIZE + 1;
	//֡�����ַ2�Ĵ���
	rVIDW00ADD2 = (0 << 13) | (LCD_HOZVAL * 2);//����ƽ���ҳ���ȴ�ֵ������֡�ڿɿ��Ӷ˿ڵĿ�ȡ�PAGEWIDTH ��ֵ�����ͻ���ߴ�������ֱ߽緶Χ�ڡ�

	LCD_Enable();	//��ʾʹ��
	LCD_SetGRAMBuff(0);	//Ĭ��ʹ�õ�0���Դ�
	LCD_SetWindows(0);	//Ĭ��ʹ�ô���0
	
	for(i = 0;i < 60000;i ++);		//��һ����ʱ,������ܻ����
	LCD_ClearScreen(0xffff);		//����
}


/*************************************************************************************************************************
*����        :	void LCD_ClearScreen(u16 Color)
*����        :	LCD��������
*����        :	Color:������ɫ
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120311
*����޸�ʱ��:	20120311
*˵��        :	��Ҫ�ȳ�ʼ��LCD,��ɫֵΪRGB565
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
* ����	:	void LCD_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 MODE)
* ����	:	��ָ��λ����ʾһ��ASCII�ַ� 
* ����	:	��ʼ����,�ַ�,������ɫ,������ɫ,��ʾģʽ�Լ�����,
* ����	:	��
* ����	:	���㺯��
* ����	:	����
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	Font_MODE���λΪ1��ʾ������ʾ,����λ����ѡ������
*************************************************************************************************************************/
void LCD_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE)
{
 	const u8 *font_add;//����ѡ��ͬ�������Ӧ�ֿ��ַ
	u8 i,j;
	u8 fh;//�ָ�
	switch (FONT_H[Font_MODE & 0x7f])
	{
	 	case 16:fh = 16;font_add = Song_16[*p - 32];break;//ѡ������߶�Ϊ16,�ֿ�Ϊ8,������������;
		case 13:fh = 13;font_add = Song_13[*p - 32];break;//ѡ������߶�Ϊ13,�ֿ�Ϊ8,������������;
		default : fh = 16;font_add = Song_16[*p - 32];break;//Ĭ��ѡ������߶�Ϊ16,�ֿ�Ϊ8,������������;
	}

 	for(i = 0;i < fh;i ++)
	{
	 	for(j = 0;j < 8;j ++)
		{
		 	if(font_add[i] & 0x80 >> j)
				LCD_DrawPoint(x + j,y + i,Font_color);//�ֵ���ɫ
			else 
			{
				if(!(Font_MODE & 0x80))//�ǵ�����ʾ
					LCD_DrawPoint(x + j,y + i,Back_color);//������ɫ
			}
		}
	}
}

/*************************************************************************************************************************
* ����	:	void Show_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 MODE)
* ����	:	��ָ��λ����ʾASCII�ַ��� 
* ����	:	��ʼ����,�ַ���,������ɫ,������ɫ,��ʾģʽ�Լ�����,
* ����	:	��
* ����	:	void Show_Char(u16 x,u16 y,char *p,u16 Font_color,u16 Back_color,u8 MODE);//�����ַ���ʾ����
* ����	:	����
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	Font_���λΪ1��ʾ������ʾ,����λ����ѡ������
*************************************************************************************************************************/
void Show_Char(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE)
{
 	while(*p != 0x00)//�ж��Ƿ񵽴��ַ�����β
	{
		if(x > LCD_XSIZE-1-8)//�ж��Ƿ���Ҫ����
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
* ����	:	void LCD_China(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE)
* ����	:	��ָ��λ����ʾһ������ָ����С�ĺ��� 
* ����	:	��ʼ����,���ֵ�������λ��,������ɫ,������ɫ,��ʾģʽ�Լ�����,
* ����	:	��
* ����	:	���㺯��
* ����	:	����
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	Font_���λΪ1��ʾ������ʾ,����λ����ѡ������߶�
*************************************************************************************************************************/
void LCD_China(u16 x,u16 y,const u8 *p,u16 Font_color,u16 Back_color,u8 Font_MODE)
{
	u8 i,j;
	u8 fhl;//�ֿ��

	fhl = FONT_H[Font_MODE & 0x7f];

	if(Font_MODE & 0x80)//������ʾ
	{
	 	for(i = 0;i < fhl;i ++)
		{
		 	for(j = 0;j < 8;j ++)
			{
			 	if(*p & 0x80 >> j)
					LCD_DrawPoint(x + j,y + i,Font_color);//�ֵ���ɫ
			}
			p ++;
			for(j = 0;j < fhl - 8;j ++)
			{
				if(*p & 0x80 >> j)
					LCD_DrawPoint(x + j + 8,y + i,Font_color);//�ֵ���ɫ
			}
			p ++;
		}
	}
	else //�ǵ�����ʾ
	{
		for(i = 0;i < fhl;i ++)
		{
		 	for(j = 0;j < 8;j ++)
			{
			 	if(*p & 0x80 >> j)
					LCD_DrawPoint(x + j,y + i,Font_color);//�ֵ���ɫ
				else
					LCD_DrawPoint(x + j,y + i,Back_color);//������ɫ
			}
			p ++;
			for(j = 0;j < fhl - 8;j ++)
			{
				if(*p & 0x80 >> j)
					LCD_DrawPoint(x + j + 8,y + i,Font_color);//�ֵ���ɫ
				else
					LCD_DrawPoint(x + j + 8,y + i,Back_color);//������ɫ
			}
			p ++;
		}
	}  
}



/*************************************************************************************************************************
* ����	:	void LCD_Fill(u16 xStart, u16 xEnd, u16 yStart, u16 yEnd,u16 color)
* ����	:	ָ��λ�������ɫ
* ����	:	λ������,��ɫֵ
* ����	:	��
* ����	:	�ײ㺯��
* ����	:	����
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	��
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
* ����	:	void Draw_Circle(u16 x0,u16 y0,u8 r)
* ����	:	��Բ����
* ����	:	Բ������,�뾶
* ����	:	��
* ����	:	���㺯��
* ����	:	����
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	��
*************************************************************************************************************************/
void Draw_Circle(u16 x0,u16 y0,u8 r,u16 color)
{
	int a = 0,b = r;
	int di;
	di = 3 -(r << 1);             //�ж��¸���λ�õı�־
//	LCD_DrawPoint(x0,y0,color);             //���� 
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
		//ʹ��Bresenham�㷨��Բ     
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
* ����	:	void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2)
* ����	:	���ߺ���
* ����	:	����յ�����
* ����	:	��
* ����	:	���㺯��
* ����	:	����
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	��
*************************************************************************************************************************/ 
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol;
	
	//TFT_LCD_SetRamAddr(0,239,0,319);//������ʾ���� 

	delta_x=x2-x1; //������������ 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		LCD_DrawPoint(uRow,uCol,color);//���� 
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
* ����	:	void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2)
* ����	:	��ָ��λ�û�һ������
* ����	:	����ε���������
* ����	:	��
* ����	:	���ߺ���
* ����	:	����
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	��
*************************************************************************************************************************/
void LCD_DrawRectangle(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	LCD_DrawLine(x1,y1,x2,y1,color);
	LCD_DrawLine(x1,y1,x1,y2,color);
	LCD_DrawLine(x1,y2,x2,y2,color);
	LCD_DrawLine(x2,y1,x2,y2,color);
}								  	 

/*************************************************************************************************************************
* ����	:	void Draw_Big_Point(u16 x,u16 y,u16 color)
* ����	:	��һ��2*2�Ĵ��
* ����	:	�����������
* ����	:	��
* ����	:	���㺯��
* ����	:	����
* ʱ��	:	20110920
* ����޸�ʱ�� : 20110920
* ˵��	: 	��
*************************************************************************************************************************/	   
void Draw_Big_Point(u16 x,u16 y,u16 color)
{	
	//TFT_LCD_SetRamAddr(0,239,0,319);     
	LCD_DrawPoint(x,y,color);//���ĵ� 
	LCD_DrawPoint(x+1,y,color);
	LCD_DrawPoint(x,y+1,color);
	LCD_DrawPoint(x+1,y+1,color);	 	  	
}


/*************************************************************************************************************************
*����        :	void LCD_SetGRAMBuff(u8 BuffNum)
*����        :	ѡ��ǰ�Դ�
*����        :	BuffNum:0:�Դ�0;1:�Դ�1
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120729
*����޸�ʱ��:	20120729
*˵��        :	��ʼ����һ��Ҫѡ���Դ�,�������ַǷ����ʵ�����
*************************************************************************************************************************/
void LCD_SetGRAMBuff(u8 BuffNum)
{
	switch(BuffNum)
	{
		case 0:
			LCD_BUFFER = LCD_BUFFER0;	//ѡ���Դ�0
			break;
		case 1:
			LCD_BUFFER = LCD_BUFFER1;	//ѡ���Դ�1
			break;
		default :
			break;	//������Χ
	}	
}


/*************************************************************************************************************************
*����        :	void LCD_SetWindows(u8 WinNum)
*����        :	ѡ��ǰ��ʾ����
*����        :	WinNum:0:����0;1:����1
*����        :	��
*����        : 	�ײ�궨��
*����        :	����
*ʱ��        :	20120729
*����޸�ʱ��:	20120729
*˵��        :	��ʼ����һ��Ҫѡ���Դ�,�������ַǷ����ʵ�����
*************************************************************************************************************************/
void LCD_SetWindows(u8 WinNum)
{
	switch(WinNum)
	{
		case 0:
			SetLCD_Windows0();	//ѡ����ʾ����0
			break;
		case 1:
			SetLCD_Windows1();	//ѡ����ʾ����1
			break;
		default :
			break;	//������Χ
	}	
}	
