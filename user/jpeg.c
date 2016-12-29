/*************************************************************************************************************
 * �ļ���:	jpeg.c
 * ����:		S3C6410 JPEG����ײ���������
 * ����:		����
 * ����ʱ��:	2012��9��20��20:59
 * ����޸�ʱ��:2012��9��20��
 * ��ϸ:		JPEGӲ����
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

#define ENABLE_MOTION_ENC       (0x1<<3)		//ʹ�ܶ�̬����
#define DISABLE_MOTION_ENC      (0x0<<3)

#define ENABLE_MOTION_DEC       (0x1<<0)		//ʹ�ܶ�̬����
#define DISABLE_MOTION_DEC      (0x0<<0)		

#define ENABLE_HW_DEC           (0x1<<2)		//ʹ��Ӳ�����ƽ���
#define DISABLE_HW_DEC          (0x0<<2)		//������ƽ���ģʽ

#define INCREMENTAL_DEC			(0x1<<3)		//��������ģʽ
#define NORMAL_DEC              (0x0<<3)		//��������ģʽ




/*************************************************************************************************************************
*����    :	void JPEG_Init(void)
*����    :	JPEG�����ʼ��
*����    :	��
*����    :	��
*����	: 	�ײ�궨��
*����     :	cp1300@139.com
*ʱ��     :	20120920
*����޸�ʱ��:	20120920
*˵��     :	��
*************************************************************************************************************************/
void JPEG_Init(void)
{
	Set_GateClk(SCLK_JPEG,ENABLE);
}


/*************************************************************************************************************************
*����    :	void JPEG_Reset(void)
*����    :	JPEG�����������λ
*����    :	��
*����    :	��
*����	: 	�ײ�궨��
*����     :	cp1300@139.com
*ʱ��     :	20120920
*����޸�ʱ��:	20120920
*˵��     :	��
*************************************************************************************************************************/
void JPEG_Reset(void)
{
	RESET_CON = 0;
}


/*************************************************************************************************************************
*����    :	void JPEG_Wait(void)
*����    :	JPEG�ȴ��������
*����    :	��
*����    :	��
*����	: 	�ײ�궨��
*����     :	cp1300@139.com
*ʱ��     :	20120920
*����޸�ʱ��:	20120920
*˵��     :	�ж�JPGSTS;1:��������;0:���
*************************************************************************************************************************/
void JPEG_Wait(void)
{
	while(JPGSTS);
}


/*************************************************************************************************************************
*����    :	u8 JPEG_Reset(void)
*����    :	��ȡJPEG����ģʽ
*����    :	��
*����    :	JPEG����,���궨��
*����	: 	�ײ�궨��
*����     :	cp1300@139.com
*ʱ��     :	20120920
*����޸�ʱ��:	20120920
*˵��     :	����ģʽֻ��0x1,0x2����ģʽ
*************************************************************************************************************************/
u8 JPEG_GetJpegType(void)
{
	switch (JPGMOD & 0x07)	//ͨ���ж�0-2BIT
	{
		case 0	:	return TYPE_JPEG_444;		//ɫ��4:4:4��ʽ
		case 1	: 	return TYPE_JPEG_422;		//ɫ��4:2:2��ʽ
		case 2	: 	return TYPE_JPEG_420;		//ɫ��4:2:0��ʽ
		case 3	: 	return TYPE_JPEG_400;		//��ɫ��ʽ(��һ���)
		case 6	: 	return TYPE_JPEG_411;		//ɫ��4:1:1��ʽ
		default : 	return TYPE_JPEG_UNKNOWN;	
	}
}


/*************************************************************************************************************************
*����    :	u32 JPEG_GetStreamLen(void)
*����    :	��ȡѹ�����ݳ���
*����    :	��
*����    :	ѹ�������ֽ�
*����	: 	�ײ�궨��
*����     :	cp1300@139.com
*ʱ��     :	20120920
*����޸�ʱ��:	20120920
*˵��     :	��λ�ֽ�;0-23BIT��Ч,��24λ
*************************************************************************************************************************/
u32 JPEG_GetStreamLen(void)
{
	return JPGCNT;
}


/*************************************************************************************************************************
*����    :	void JPEG_GetWidthAndHeight(u16* HSize, u16* VSize)
*����    :	��ȡͼ���С
*����    :	HSize:ͼ���Ȼ�����ָ��;VSize:ͼ��߶Ȼ�����ָ��
*����    :	��
*����	: 	�ײ�궨��
*����     :	cp1300@139.com
*ʱ��     :	20120920
*����޸�ʱ��:	20120920
*˵��     :	16BIT
*************************************************************************************************************************/
void JPEG_GetWidthAndHeight(u16* HSize, u16* VSize)
{
	*HSize = JPGX;		//��ˮƽ�����϶���ͼ���С��ֵ
	*VSize = JPGY;		//�ڴ�ֱ�����϶���ͼ���С��ֵ
}



/*************************************************************************************************************************
*����    :	void JPEG_ReadAndClearStatus(u8* Status, u8* IrqStatus)
*����    :	��ȡ�����JPEG״̬
*����    :	Status:������״̬������ָ��;IrqStatus:�ж�״̬������ָ��
*����    :	��
*����	: 	�ײ�궨��
*����     :	cp1300@139.com
*ʱ��     :	20120920
*����޸�ʱ��:	20120920
*˵��     :	JPGSTS��JPGIRQ;JPGIRQ��ȡ���Զ����
* 			JPGIRQ:	BIT6���״̬	0���������Ĵ������; 1�������Ĵ������
* 					BIT4λ���Ĵ���״̬��ֻ���ڽ�ѹ���ڼ���Ч��0���ڱ�ѹ�����ļ��ϣ�û���﷨����1���ڱ�ѹ�����ļ��ϣ����﷨����
* 					BIT3����״̬��ֻ���ڽ�ѹ���ڼ���Ч��0��ͼ���С��ȡ������ֵ���ɶ���1��ͼ���С��ȡ������ֵ�ɶ���	
*************************************************************************************************************************/
void JPEG_ReadAndClearStatus(u8* Status, u8* IrqStatus)
{
	*Status = JPGSTS;
	*IrqStatus = JPGIRQ & ((1<<6)|(1<<4)|(1<<3));
}


/*************************************************************************************************************************
*����    :	void JPEG_InitRegsForDecoding(u32 JpegAddr,u32 ImageAddr,u8 JpegDecMode,u8 Incremental,u8 IsMotion)
*����    :	��ʼ������ʼJPEG����
*����    :	JpegAddr:	jpegͼ���ַ
* 			ImageAddr:	�����ͼ���ַ
* 			JpegDecMode:DECģʽ,���궨��
* 			Incremental:����ģʽ
* 			IsMotion:	��̬
*����    :	��
*����	: 	�ײ�궨��
*����     :	cp1300@139.com
*ʱ��     :	20120920
*����޸�ʱ��:	20120920
*˵��     :	��
*************************************************************************************************************************/
void JPEG_InitRegsForDecoding(u32 JpegAddr,u32 ImageAddr,u8 JpegDecMode,u8 Incremental,u8 IsMotion)
{
	u32 JpgConVal = 0;	
	u32 Misc = 0;

	JPGMOD = 0x8; //����Ϊ����ģʽ
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
	JSTART = 1;	//��ʼJPEG����
}

void JPEG_StartDecodingBody(u32 ImageAddr, u8 Incremental)
{
	
	JPEG_InitRegsForDecoding(0, ImageAddr, DEC_BODY, Incremental, FALSE);
	JRSTART = 1;	//��ʼ�����봦��
}


void JPEG_StartDecodingOneFrame(u32 JpgAddr, u32 ImageAddr, u8 Incremental)
{

	JPEG_Reset();
	JPEG_InitRegsForDecoding(JpgAddr, ImageAddr, DEC_HEADER_N_BODY, Incremental, FALSE);
	JSTART = 1;
}
