#include "system.h"
#include "uart.h"
#include "tft_lcd.h"
#include "other.h"
#include "delay.h"
#include "timer.h"
#include "jpeg.h"
#include "jpg.h"

vu8 bJpegDone = 1;

//LED1闪烁程序,在定时器0中断服务程序中闪烁,周期400MS
void LED1_flash(void)
{
	LED1_FLASH();
}

static void __irq Isr_Jpeg(void)
{
	u8 eJpgIrq,eJpgSts;
	
	Set_IntEnable(INT_JPEG,DISABLE);
	
	lcd_printf("JPGIRQ = 0x%X\n",JPGIRQ);
	JPEG_ReadAndClearStatus(&eJpgSts, &eJpgIrq);
	bJpegDone = 0;
	
	
	Set_IntEnable(INT_JPEG,ENABLE);
	VICInterruptEnd();	
}











void YUV422ToRGB565(u8 *YUVBuff,u16 width,u16 height)
{
	u16 x,y;
	u32 RGBdata;
	u32 cnt = 0;
	u8 Y,U = 0,V = 0;
	int R,G,B;

	for(y = 0;y < height;y ++)
	{
	 	for(x = 0;x < width;x ++ )
		{
		 	Y = YUVBuff[cnt * 2];
			if((cnt % 2) == 0)
			{
				U = YUVBuff[cnt * 2 + 1];
				V = YUVBuff[cnt * 2 + 3];
			} 


			R = Y + (1.370705 * (V-128));
	        G = Y - (0.698001 * (V-128)) - (0.337633 * (U-128));
	        B = Y + (1.732446 * (U-128));


			if(R > 255) R = 255;
			if(G > 255) G = 255;
			if(B > 255) B = 255;
			if(R < 0) R = 0;
			if(G < 0) G = 0;
			if(B < 0) B = 0;


			RGBdata = R;
			RGBdata <<= 8;
			RGBdata |= G;
			RGBdata <<= 8;
			RGBdata |= B;

			LCD_DrawPoint(x,y,RGB565(RGBdata));
			cnt ++;
		}
	}
}


static u8 buff[4*800*480];
u16 fbuff[800*480];

int main(void)
{		
	u16 x,y;
	u16 i;
	
	LCD_Init();					//初始化LCD
	UART0_Init(DISABLE,115200);	//初始化串口,失能中断接收,波特率115200
	LED_Init();					//初始化LED
	Timer1_Init(400000-1,ENABLE,LED1_flash);	//初始化定时器0,周期400ms
	
	lcd_printf("Get_FCLK : %d Hz\n",Get_FCLK());	
	lcd_printf("Get_PCLK : %d Hz\n",Get_PCLK());


	JPEG_Init();
	Set_IsrAddr(INT_JPEG,(vu32)Isr_Jpeg);
	//Set_IntEnable(INT_JPEG,ENABLE);
	bJpegDone = 1;
	//JPEG_StartDecodingOneFrame((vu32)Array,(vu32)LCD_BUFFER0,FALSE);
	
LOOP:	
	Delay_MS(500);
	JPEG_StartParsingHeader((vu32)Array);
	while(!(JPGIRQ & BIT3));	//等待Header_status为高电平
	JPEG_GetWidthAndHeight(&x,&y);	//获取图像宽度,高度
	lcd_printf("x = %d,y = %d\n",x,y);
	lcd_printf("MOD = %d\n",JPEG_GetJpegType());	
	lcd_printf("JPGSTS = 0x%X\n",JPGSTS);
	lcd_printf("JPGIRQ = 0x%X\n",JPGIRQ);
	JPEG_StartDecodingBody((vu32)buff,FALSE);
	i = 1000;
	lcd_printf("JPGSTS = 0x%X\n",JPGSTS);
	while(!(JPGIRQ))
	{
		x=JPGSTS;
		Delay_MS(1);
		i --;
		if(i == 0)
			goto LOOP;
	}
	lcd_printf("JPGSTS = 0x%X\n",JPGSTS);
	lcd_printf("JPGIRQ = 0x%X\n",JPGIRQ);
	JPGIRQ;
	JPGSTS;
	
	YUV422ToRGB565(buff,800,480);
	

	lcd_printf("buff = 0x%X\n",(u32)buff);
	while(1)
	{
		LED2_FLASH();		//LED2闪烁
		Delay_US(600000);
	}
}
