/*************************************************************************************************************
 * 文件名:	jpeg.c
 * 功能:		S3C6410 JPEG解码底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年9月20日20:59
 * 最后修改时间:2012年9月20日
 * 详细:		JPEG硬解码
*************************************************************************************************************/
#include <string.h>
#include <stdio.h>
#include "system.h"
#include "delay.h"
#include "jpeg.h"
#include "jpeg_tables.h"

// Coefficients for RGB to YUV
#define COEF1_RGB_2_YUV         0x4d971e
#define COEF2_RGB_2_YUV         0x2c5783
#define COEF3_RGB_2_YUV         0x836e13

#define ENABLE_MOTION_ENC       (0x1<<3)		//使能动态编码
#define DISABLE_MOTION_ENC      (0x0<<3)

#define ENABLE_MOTION_DEC       (0x1<<0)		//使能动态解码
#define DISABLE_MOTION_DEC      (0x0<<0)		

#define ENABLE_HW_DEC           (0x1<<2)		//使能硬件控制解码
#define DISABLE_HW_DEC          (0x0<<2)		//软件控制解码模式

#define INCREMENTAL_DEC			(0x1<<3)		//增量解码模式
#define NORMAL_DEC              (0x0<<3)		//正常解码模式




/*************************************************************************************************************************
*函数    :	void JPEG_Init(void)
*功能    :	JPEG解码初始化
*参数    :	无
*返回    :	无
*依赖	: 	底层宏定义
*作者     :	cp1300@139.com
*时间     :	20120920
*最后修改时间:	20120920
*说明     :	无
*************************************************************************************************************************/
void JPEG_Init(void)
{
	Set_GateClk(SCLK_JPEG,ENABLE);
}


/*************************************************************************************************************************
*函数    :	void JPEG_Reset(void)
*功能    :	JPEG解码器软件复位
*参数    :	无
*返回    :	无
*依赖	: 	底层宏定义
*作者     :	cp1300@139.com
*时间     :	20120920
*最后修改时间:	20120920
*说明     :	无
*************************************************************************************************************************/
void JPEG_Reset(void)
{
	RESET_CON = 0;
}


/*************************************************************************************************************************
*函数    :	void JPEG_Wait(void)
*功能    :	JPEG等待操作完成
*参数    :	无
*返回    :	无
*依赖	: 	底层宏定义
*作者     :	cp1300@139.com
*时间     :	20120920
*最后修改时间:	20120920
*说明     :	判断JPGSTS;1:正在运行;0:完成
*************************************************************************************************************************/
void JPEG_Wait(void)
{
	while(JPGSTS);
}


/*************************************************************************************************************************
*函数    :	u8 JPEG_Reset(void)
*功能    :	获取JPEG采样模式
*参数    :	无
*返回    :	JPEG类型,见宏定义
*依赖	: 	底层宏定义
*作者     :	cp1300@139.com
*时间     :	20120920
*最后修改时间:	20120920
*说明     :	编码模式只有0x1,0x2两种模式
*************************************************************************************************************************/
u8 JPEG_GetJpegType(void)
{
	switch (JPGMOD & 0x07)	//通过判断0-2BIT
	{
		case 0	:	return TYPE_JPEG_444;		//色度4:4:4格式
		case 1	: 	return TYPE_JPEG_422;		//色度4:2:2格式
		case 2	: 	return TYPE_JPEG_420;		//色度4:2:0格式
		case 3	: 	return TYPE_JPEG_400;		//灰色格式(单一组成)
		case 6	: 	return TYPE_JPEG_411;		//色度4:1:1格式
		default : 	return TYPE_JPEG_UNKNOWN;	
	}
}


/*************************************************************************************************************************
*函数    :	u32 JPEG_GetStreamLen(void)
*功能    :	获取压缩数据长度
*参数    :	无
*返回    :	压缩数据字节
*依赖	: 	底层宏定义
*作者     :	cp1300@139.com
*时间     :	20120920
*最后修改时间:	20120920
*说明     :	单位字节;0-23BIT有效,共24位
*************************************************************************************************************************/
u32 JPEG_GetStreamLen(void)
{
	return JPGCNT;
}


/*************************************************************************************************************************
*函数    :	void JPEG_GetWidthAndHeight(u16* HSize, u16* VSize)
*功能    :	获取图像大小
*参数    :	HSize:图像宽度缓冲区指针;VSize:图像高度缓冲区指针
*返回    :	无
*依赖	: 	底层宏定义
*作者     :	cp1300@139.com
*时间     :	20120920
*最后修改时间:	20120920
*说明     :	16BIT
*************************************************************************************************************************/
void JPEG_GetWidthAndHeight(u16* HSize, u16* VSize)
{
	*HSize = JPGX;		//在水平方向上定义图像大小的值
	*VSize = JPGY;		//在垂直方向上定义图像大小的值
}



/*************************************************************************************************************************
*函数    :	void JPEG_ReadAndClearStatus(u8* Status, u8* IrqStatus)
*功能    :	读取并清除JPEG状态
*参数    :	Status:解码器状态缓冲区指针;IrqStatus:中断状态缓冲区指针
*返回    :	无
*依赖	: 	底层宏定义
*作者     :	cp1300@139.com
*时间     :	20120920
*最后修改时间:	20120920
*说明     :	JPGSTS和JPGIRQ;JPGIRQ读取后自动清除
* 			JPGIRQ:	BIT6结果状态	0：不正常的处理结束; 1：正常的处理完成
* 					BIT4位流的错误状态。只有在解压缩期间有效。0：在被压缩的文件上，没有语法错误。1：在被压缩的文件上，有语法错误。
* 					BIT3标题状态。只有在解压缩期间有效。0：图像大小和取样因素值不可读。1：图像大小和取样因素值可读。	
*************************************************************************************************************************/
void JPEG_ReadAndClearStatus(u8* Status, u8* IrqStatus)
{
	*Status = JPGSTS;
	*IrqStatus = JPGIRQ & ((1<<6)|(1<<4)|(1<<3));
}


/*************************************************************************************************************************
*函数    :	void JPEG_InitRegsForDecoding(u32 JpegAddr,u32 ImageAddr,u8 JpegDecMode,u8 Incremental,u8 IsMotion)
*功能    :	初始化并开始JPEG解码
*参数    :	JpegAddr:	jpeg图像地址
* 			ImageAddr:	解码后图像地址
* 			JpegDecMode:DEC模式,见宏定义
* 			Incremental:增量模式
* 			IsMotion:	动态
*返回    :	无
*依赖	: 	底层宏定义
*作者     :	cp1300@139.com
*时间     :	20120920
*最后修改时间:	20120920
*说明     :	无
*************************************************************************************************************************/
void JPEG_InitRegsForDecoding(u32 JpegAddr,u32 ImageAddr,u8 JpegDecMode,u8 Incremental,u8 IsMotion)
{
	u32 JpgConVal = 0;	
	u32 Misc = 0;

	JPGMOD = 0x8; //设置为解码模式
	if (JpegDecMode == DEC_HEADER || JpegDecMode == DEC_HEADER_N_BODY)
	{
		JHUFADDR0 = JHUFADDR1 = JpegAddr; // Address of compresed input data
		JPGIRQEN = 0xf<<3;// JPGIRQEN[6:3]=For several error conditions @2006.6.8	
		if (JpegDecMode == DEC_HEADER_N_BODY)
		{
			JIMGADDR0 = JIMGADDR1 = ImageAddr;	// Address of decompresed image
		}
		JpgConVal = (JpegDecMode == DEC_HEADER) ? DISABLE_HW_DEC : ENABLE_HW_DEC;
	}
	else // JpegDecMode == DEC_BODY
	{
		JIMGADDR0 = JIMGADDR1 = ImageAddr;	// Address of decompresed image	
	}


	if (JpegDecMode == DEC_BODY || JpegDecMode == DEC_HEADER_N_BODY)
	{
		JpgConVal |= (IsMotion == TRUE) ? ENABLE_MOTION_DEC : DISABLE_MOTION_DEC;
		Misc = (Incremental == TRUE) ? INCREMENTAL_DEC : NORMAL_DEC;
		Misc |= 2 << 5;
	}
	
	JPG_CON = JpgConVal;
	JMISC = Misc;
}



//waitForIRQ


void JPEG_StartParsingHeader(u32 JpgAddr)
{
	JPEG_Reset();
	JPEG_InitRegsForDecoding(JpgAddr, 0, DEC_HEADER, FALSE, FALSE);
	JSTART = 1;	//开始JPEG处理
}

void JPEG_StartDecodingBody(u32 ImageAddr, u8 Incremental)
{
	
	JPEG_InitRegsForDecoding(0, ImageAddr, DEC_BODY, Incremental, FALSE);
	JRSTART = 1;	//开始主解码处理
}


void JPEG_StartDecodingOneFrame(u32 JpgAddr, u32 ImageAddr, u8 Incremental)
{

	JPEG_Reset();
	JPEG_InitRegsForDecoding(JpgAddr, ImageAddr, DEC_HEADER_N_BODY, Incremental, FALSE);
	JSTART = 1;
}
