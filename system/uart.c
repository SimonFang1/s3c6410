/*************************************************************************************************************
 * 文件名:	uart.c
 * 功能:		S3C6410 UART底层驱动函数
 * 作者:		cp1300@139.com
 * 创建时间:	2012年3月4日16:33
 * 最后修改时间:2012年3月4日
 * 详细:		串口初始化,发送,接收,配置等
*************************************************************************************************************/
#include "system.h"
#include "uart.h"
 
 
u8 New_UART0_Rx = 0;			//BIT0代表有新数据,BIT7代表数据帧接收完毕
void __irq Isr_UART0_Rx(void);	//中断接收服务函数
 

/*************************************************************************************************************************
*函数        :	void BaudRateCalculation(vu32 *UBRDIV,vu32 *UDIVSLOT,vu32 PCLK,vu32 Uart_Speed) 
*功能        :	波特率计算
*参数        :	UBRDIV:波特率分频整数部分,UDIVSLOT:波特率分频小数部分,PCLK:时钟,单位MHz,通常取66Mhz,Uart_Speed:串口波特率,如9600,115200等
*返回        :	无
*依赖        : 	浮点运算
*作者        :	cp1300@139.com
*时间        :	20120311
*最后修改时间:	20120311
*说明        :	用于计算波特率分频寄存器的值,需要进行浮点运算
*************************************************************************************************************************/
void BaudRateCalculation(vu32 *UBRDIV,vu32 *UDIVSLOT,vu32 PCLK,vu32 Uart_Speed) 
{
	float temp;
	u8 i,j;
	
	temp = PCLK*1.0 / (Uart_Speed * 16.0) - 1;	//计算波特率分频值
	*UBRDIV = (u32)temp;		//整数部分
	temp -= *UBRDIV;			//取出小数部分
	temp *=	16;
	j = (u8)temp;				//计算1的个数
	*UDIVSLOT = 0;
	for(i = 0;i < j;i ++)
		*UDIVSLOT |= 1 << i;
}
 
/*************************************************************************************************************************
*函数        :	void UART0_Init(FunctionalState EnableIntnt,vu32 UART_Speed)
*功能        :	串口0初始化
*参数        :	EnableIntnt:1:使能接收中断,UART_Speed:串口波特率,如9600,115200等
*返回        :	无
*依赖        : 	底层宏定义
*作者        :	cp1300@139.com
*时间        :	20120220
*最后修改时间:	20120719
*说明        :	初始化串口0
*************************************************************************************************************************/
void UART0_Init(FunctionalState EnableIntnt,vu32 UART_Speed)
{
	rPCLK_GATE |= 1 << 1;			//UART0选通PCLK时钟,66mhz
	rGPACON &= ~(0xff);        	//清除之前的设置,GPIOA0,GPIOA1
	rGPACON |= (IO_SF_MODE | (IO_SF_MODE <<4));      //设置GPIOA0,GPIOA1为uart模式
	rULCON0 = 0x3;       			//每帧8位,清除设置,普通模式,无校验,1个停止位
	rUCON0= (0x1 << 2) | 0x1 | (2 <<10);         		//清除控制寄存器//设置(PCLK=66MHz)为波特率时钟
	rUFCON0 = 0;                		//禁止FIFO
	rUMCON0 = 0;                		//禁止AFC,nRTS高电平无效
	BaudRateCalculation(&rUBRDIV0,&rUDIVSLOT0,Get_PCLK(),UART_Speed);//计算并设置波特率分频寄存器
	rUINTP0 = 0;						//关闭UART0中断
	rUINTM0 = 0xf;					//屏蔽一切中断
	Set_IntEnable(INT_UART0,DISABLE);//关闭UART0全局中断使能
	if(EnableIntnt == ENABLE)						//使能中断接收
	{
		rUCON0 |= BIT0;				//中断或者轮询方式读取接收缓冲器
		rUERSTAT0 = rUERSTAT0;		//清除错误状态寄存器
		rUINTP0 |= BIT0;				//允许产生接收中断
		rUINTM0 = 0;				//关闭中断屏蔽
		rUINTSP0 = 0;				//清除中断标志
		Set_IsrAddr(INT_UART0,(u32)Isr_UART0_Rx);		//写入中断服务程序入口地址
		Set_IntEnable(INT_UART0,ENABLE);				//使能UART0全局中断
	}
}

 
/*************************************************************************************************************************
*函数        :	void UART0_SendByte(vu8 data)
*功能        :	串口0发送一个字节
*参数        :	发送的数据
*返回        :	无
*依赖        :	底层宏定义
*作者        :	cp1300@139.com
*时间        :	20120220
*最后修改时间:	20120220
*说明        :	使用串口0发送一个字节
*************************************************************************************************************************/
void UART0_SendByte(vu8 data)
{
	while(!(rUTRSTAT0 & (1 <<2)));   //等待发送缓冲器为空
	rUTXH0 = data;        			//将数据写入发送缓冲器
}

//中断接收服务函数
void __irq Isr_UART0_Rx(void)
{
	New_UART0_Rx |= BIT0;	//有新数据
	rUINTSP0 = 0;
	rUINTP0 = 1;				//清除中断标志
	VICInterruptEnd();	//中断结束
}




