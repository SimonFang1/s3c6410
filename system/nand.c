/*************************************************************************************************************
 * 文件名:	Nand.c
 * 功能:		S3C6410 NandFlash底层驱动函数
 * 作者:		陈鹏
 * 创建时间:	2012年3月31日21:34
 * 最后修改时间:2012年3月31日
 * 详细:		NandFlash底层驱动函数,
 				板载NAND FLASH信息:2GB,MLC K9G4G08(K9GAG08U0D,页大小4KB,4bit纠正)
*************************************************************************************************************/
#include "system.h"
#include "Nand.h"

//配置
//CONF
#define ECCType		1	//ECC类型选择,0:SLC(1位修正);1:MLC(4位修正)
#define TACLS		5	//CLE & ALE持续时间(0-7)(= HCLK * TACLS)
#define TWRPH0		5	//TWRPH0持续时间(0-7)(= HCLK * (TWRPH0 + 1))
#define TWRPH1		5	//TWRPH1持续时间(0-7)(= HCLK * (TWRPH1 + 1))
#define AdvFlash	1	//预先NAND flash 存储器启动;0:支持512字节/页;1:支持2KB/页
#define AddrCycle	1	//NAND Flash存储器地址周期,0: 0:3地址周期,1:4地址周期;1: 0:4地址周期,1:5地址周期
//CONT
#define SoftLock	0	//软件锁配置,0:禁用锁,1:使能锁
#define MainECCLock	1	//锁存主区ECC生成:0:开启主区ECC;1:锁存主区ECC
#define SpareECCLock	1	//锁存备用区ECC生成;0:开启备用区ECC,1:锁存备用区ECC,备用区ECC 状态寄存器是NFSECC(0x7020003C)
#define RegNCE1		1	//NAND Flash 存储器nGCS[3]信号控制：0：强制nGCS[3]为低（使能片选）;1：强制nGCS[3]为高（禁用片选）注：即使Reg_nCE1 和 Reg_nCE0 同时被设置为0，它们之中也只有一个被声明
#define MODE		1	//NAND Flash 控制器操作模式：0：NAND Flash 控制器禁用（不工作）1：NAND Flash 控制器使能

//NAND FLASH操作宏
#define NANDCMD(cmd)	(NAND->CMMD = (cmd))	//向NAND flash写入命令
#define NANDADDR(addr)	(NAND->ADDR = (addr))	//向NAND flash写入地址
#define NANDDATA		(NAND->DATA)			//向NAND flash读写数据
#define NF_nCS3_L		(NAND->CONT&=~(1<<2))
#define NF_nCS3_H		(NAND->CONT|=(1<<2))
#define NF_nCS2_L		(NAND->CONT&=~(1<<1))		//片选
#define NF_nCS2_H		(NAND->CONT|=(1<<1))
#define NF_RnB			(NAND->STAT & BIT0)			//0:存储器忙,1:空闲

//NAND片选
#define NAND_CS_L		NF_nCS2_L	//NAND片选有效
#define NAND_CS_H		NF_nCS2_H	//NAND片选无效

//NAND FLASH信息宏
#define FLASH_MAX_ADDR 		0x80000000	//FLASH最大能够达到的地址，是2GB
#define FLASH_BLOCK_SIZE 	0x20000		//FLASH块大小，为512KB
#define FLASH_PAGE_SIZE 	0x1000		//FLASH页大小，为4KB
//ECC 8BIT 512B


//NAND FLASH	命令定义
#define NAND_READ_1th			0x00				//读数据区，第一个访问周期
#define NAND_READ_2th			0x30				//读数据区，第二个访问周期	
#define NAND_READ_ID			0x90				//读NAND ID
#define NAND_READ_STATUS1		0x70				//读状态1
#define NAND_READ_STATUS2		0xf1				//读状态2
#define NAND_RESET				0xff				//复位


//等待超时定义
#define NAND_TIME_OUT_CNT	0xffffff


/*************************************************************************************************************************
*函数        :	bool NnadFlashWait(void)
*功能        :	等待操作完成
*参数        :	无
*返回        :	FALSE:超时;TRUE:成功
*依赖	: 	无
*作者        :	陈鹏
*时间        :	20120331
*最后修改时间:	20120331
*说明        :	低电平操作忙
*************************************************************************************************************************/
bool NnadFlashWait(void)
{
	u32 TimeCnt = NAND_TIME_OUT_CNT;
	while(NF_RnB == 0 && TimeCnt != 0)  //等待写完成  为低表示忙
		TimeCnt --;
	if(TimeCnt == 0)
		return FALSE;
	else
		return TRUE;
}


/*************************************************************************************************************************
*函数	:	static void NandFlashWrite5BitAddr(vu32 Addr)
*功能	:	向NAND FLASH写入4字节的地址
*参数	:	地址
*返回	:	无
*依赖	: 	无
*作者	:	陈鹏
*时间		:	20120331
*最后修改时间	:	20120331
*说明	:	写NAND地址,地址共32bit,分5次写入
*************************************************************************************************************************/
static void NandFlashWrite5BitAddr(vu32 Addr)
{
	NANDADDR(Addr & 0xff);			//写入A0-A7;
	NANDADDR((Addr >> 8) & 0x1f);	//写A8-A12,0,0,0
	NANDADDR((Addr >> 13) & 0xff);	//写A13-A20			
	NANDADDR((Addr >> 21) & 0xff);	//写A21-A28
	NANDADDR((Addr >> 29) & 0x07);	//写A29-A31,0,0,0,0,0
}



/*************************************************************************************************************************
*函数        :	bool NandFlashReset(void)
*功能        :	NandFlash复位
*参数        :	无
*返回        :	FALSE:超时;TRUE:成功
*依赖	: 	无
*作者        :	陈鹏
*时间        :	20120331
*最后修改时间:		20120331
*说明        :	写入复位命令等待复位完成
*************************************************************************************************************************/
bool NandFlashReset(void)
{
	NANDADDR(0x00);			//地址复位
	NANDCMD(NAND_RESET);	//写入复位命令
	return NnadFlashWait();		//等待操作完成
}



/*************************************************************************************************************************
*函数        :	bool NandFlashInit(void)
*功能        :	NandFlash初始化函数
*参数        :	无
*返回        :	FALSE:超时;TRUE:成功
*依赖        : 	无
*作者        :	陈鹏
*时间        :	20120331
*最后修改时间:	20120331
*说明        :	用于初始化NAND
*************************************************************************************************************************/
bool NandFlashInit(void)
{
	NAND->CONF = (ECCType << 24) + (TACLS << 12) + (TWRPH0 << 8) + (TWRPH1 << 4) + (AdvFlash << 3) + BIT2 + (AddrCycle << 1);	//配置NAND FLASH
	NAND->CONT = (SoftLock << 16) + (MainECCLock << 7) + (SpareECCLock << 6) + (RegNCE1 << 2) + (MODE << 0);//初始化控制寄存器
	return NandFlashReset();	//复位NAND FLASH
}


/*************************************************************************************************************************
*函数        :	u32 NandFlashReadStatus(void)
*功能        :	读取NandFlash状态
*参数        :	无
*返回        :	状态参数
*依赖        : 	无
*作者        :	陈鹏
*时间        :	20120331
*最后修改时间:	20120331
*说明        :	读取状态
*************************************************************************************************************************/
u32 NandFlashReadStatus(void)
{
	NAND_CS_L;
	NANDCMD(NAND_READ_STATUS1);		//写查询命令
	NnadFlashWait();	//等待操作完成
	NAND_CS_H;
	return NANDDATA;	//返回状态信息
}


/*************************************************************************************************************************
*函数        :	u32 NandFlashReadStatus1(void)
*功能        :	读取NandFlash状态1
*参数        :	无
*返回        :	状态参数
*依赖        : 	无
*作者        :	陈鹏
*时间        :	20120331
*最后修改时间:	20120331
*说明        :	读取状态1
*************************************************************************************************************************/
u32 NandFlashReadStatus1(void)
{
	NAND_CS_L;
	NANDCMD(NAND_READ_STATUS2);		//写查询命令1
	NnadFlashWait();	//等待操作完成
	NAND_CS_H;
	return NANDDATA;	//返回状态信息
}


/*************************************************************************************************************************
*函数        :	void NandFlashReadID(u8 *pbuff)
*功能        :	读取NandFlash ID
*参数        :	缓冲区指针
*返回        :	无
*依赖        : 	无
*作者        :	陈鹏
*时间        :	20120331
*最后修改时间:	20120720
*说明        :	读取ID,注意ID为6字节,ECH,DeviceCode,3rd cyc,4th cyc,5th cyc,6th cyc,读一次是32BIT,4	字节
*************************************************************************************************************************/
void NandFlashReadID(u8 *pbuff)
{
	u8 i;
	u32 buff[2];
	u8 *p = (u8 *)buff;
	
	NAND_CS_L;
	NANDCMD(NAND_READ_ID);	//写读取ID命令	
	NANDADDR(0x00);			//地址复位
	NnadFlashWait();		//等待操作完成
	//读取2次,共8字节
	for(i = 0;i < 2;i ++)	
		buff[i] = NANDDATA;
	NAND_CS_H;
	
	//复制前面的6字节ID
	for(i = 0;i < 6;i ++)
		pbuff[i] = p[i];
}


/*************************************************************************************************************************
*函数        :	bool NandFlashReadOneSector(u32 Addr,u8 *pBuff)
*功能        :	读取nand flash一页
*参数        :	Addr:页地址,pBuff:缓冲区指针;
*返回        :	FALSE:错误;TRUE:成功
*依赖     : 底层操作函数
*作者        :	陈鹏
*时间        :	20120401
*最后修改时间:	20120401
*说明        :	读取一页,FLASH_PAGE_SIZE,注意缓冲区不要越界,读一次是32BIT,4	字节
*************************************************************************************************************************/
bool NandFlashReadOneSector(u32 Addr,u8 *pBuff)
{
	u32 i;
	u32 *p = (u32 *)pBuff;
	u32 cnt = FLASH_PAGE_SIZE / 4;	//读取次数,每次读取4字节
	
	if(Addr > FLASH_MAX_ADDR)	//地址超出范围返,返回错误代码1,地址越界
		return FALSE;
	NAND_CS_L;
	//如果跨页的，则写读数据命令
	NANDCMD(NAND_READ_1th);
	NandFlashWrite5BitAddr(Addr);
	NANDCMD(NAND_READ_2th);
	if(NnadFlashWait() == FALSE) 	//等待数据读回
	{	
		NAND_CS_H;			//读取失败了,关闭片选
		NandFlashReset();	//复位NAND	
		return FALSE;		//等待超时
	}
	for(i = 0;i < cnt;i++)
	{
		p[i] = NANDDATA;  //读一字节数据
	}
	NAND_CS_H;
	return TRUE;
}


/*************************************************************************************************************************
*函数	:	u8 NandFlashReadOneSector(u32 Addr,u8 *pBuff)
*功能	:	读取nand flash数据扩展区
*参数	:	Addr:页地址,pBuff:缓冲区指针;
*返回	:	状态
*依赖	: 	底层操作函数
*作者	:	陈鹏
*时间        :	20120401
*最后修改时间:	20120401
*说明        :	读取一页,一页4KB,注意缓冲区不要越界,读一次是32BIT,4	字节
*************************************************************************************************************************/
//bool NandFlashReadOne()
//{
	
//}








