#ifndef __JPEG_H__
#define __JPEG_H__


typedef enum
{
	PAL1, PAL2, PAL4, PAL8,
	RGB8, ARGB8, RGB16, ARGB16, RGB18, RGB24, RGB30, ARGB24,
	YC420, YC422, // Non-interleave
	CRYCBY, CBYCRY, YCRYCB, YCBYCR, YUV444, // Interleave
	RGBA16, RGBX24, RGBA24
} CSPACE;



//JPEG类型定义
#define	TYPE_JPEG_UNKNOWN	0	//位置类型
#define	TYPE_JPEG_444	1		//JPEG 444
#define	TYPE_JPEG_422	2		//JPEG 422
#define	TYPE_JPEG_420	3		//JPEG 420
#define	TYPE_JPEG_400	4		//JPEG 400 (Gray)
#define	TYPE_JPEG_411	5		//JPEG 411

//JPEG解码器状态定义
#define JPEG_DONE		0		//完成状态
#define JPEG_RUNNING	1		//正在运行

//JPEG DEC模式
#define DEC_HEADER			0		//
#define DEC_BODY			1
#define DEC_HEADER_N_BODY	2


typedef struct
{
	u32 m_uJpgHSz, m_uJpgVSz;
	u32 m_uJpgConReg;

	u8 m_bIsHeaderParsed;
} JPEG_INFO;


void JPEG_Init(void);
void JPEG_Wait(void);	//JPEG等待操作完成
void JPEG_Reset(void);//JPEG解码器软件复位
u8 JPEG_GetJpegType(void);
void JPEG_GetWidthAndHeight(u16* HSize, u16* VSize);//获取图像大小
void JPEG_ReadAndClearStatus(u8* Status, u8* IrqStatus);//读取并清除JPEG状态
//初始化并开始JPEG解码
void JPEG_InitRegsForDecoding(u32 JpegAddr,u32 ImageAddr,u8 JpegDecMode,u8 Incremental,u8 IsMotion);

void JPEG_StartParsingHeader(u32 JpgAddr);
void JPEG_StartDecodingBody(u32 ImageAddr, u8 IsIncremental);
void JPEG_StartDecodingOneFrame(u32 JpgAddr, u32 ImageAddr, u8 IsIncremental);
	
#endif // __JPEG_H__
