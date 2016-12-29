/*-----------------------------------------------------------------------
/  Low level disk interface modlue include file
/-----------------------------------------------------------------------*/

#ifndef _DISKIO

#define _READONLY	0	/* 1: Remove write functions */
#define _USE_IOCTL	1	/* 1: Use disk_ioctl fucntion */

#include "integer.h"


//设备,目前支持SD卡和NAND FLASH
#define DISK_SD  0


/* Status of Disk Functions */
typedef BYTE	DSTATUS;

/* Results of Disk Functions */
typedef enum {
	RES_OK = 0,		/* 0: Successful */			//正常
	RES_ERROR,		/* 1: R/W Error */			//读写错误
	RES_WRPRT,		/* 2: Write Protected */	//写保护
	RES_NOTRDY,		/* 3: Not Ready */			//没准备好
	RES_PARERR		/* 4: Invalid Parameter */	//无效的参数
} DRESULT;


/*---------------------------------------*/
/* Prototypes for disk control functions */

int assign_drives (int, int);
DSTATUS disk_initialize (BYTE);
DSTATUS disk_status (BYTE);
DRESULT disk_read (BYTE, BYTE*, DWORD, BYTE);
#if	_READONLY == 0
DRESULT disk_write (BYTE, const BYTE*, DWORD, BYTE);
DWORD get_fattime (void);//获取时间
#endif
DRESULT disk_ioctl (BYTE, BYTE, void*);


/* Sync functions */
#include "ffconf.h"		/* FatFs configuration options */
#if _FS_REENTRANT  //如果定义了重入,则需要实现以下四个函数
int ff_cre_syncobj (BYTE, _SYNC_t*);/*创建同步对象 Create a sync object */
int ff_req_grant (_SYNC_t);			/*删除同步对象 Lock sync object */
void ff_rel_grant (_SYNC_t);		/*申请同步对象 Unlock sync object */
int ff_del_syncobj (_SYNC_t);		/*释放同步对象 Delete a sync object */
#endif






/* Disk Status Bits (DSTATUS) */

#define STA_NOINIT		0x01	/* Drive not initialized */
#define STA_NODISK		0x02	/* No medium in the drive */
#define STA_PROTECT		0x04	/* Write protected */


/* Command code for disk_ioctrl fucntion */

/* Generic command (defined for FatFs) */
#define CTRL_SYNC			0	/* Flush disk cache (for write functions) */
#define GET_SECTOR_COUNT	1	/* Get media size (for only f_mkfs()) */
#define GET_SECTOR_SIZE		2	/* Get sector size (for multiple sector size (_MAX_SS >= 1024)) */
#define GET_BLOCK_SIZE		3	/* Get erase block size (for only f_mkfs()) */
#define CTRL_ERASE_SECTOR	4	/* Force erased a block of sectors (for only _USE_ERASE) */

/* Generic command */
#define CTRL_POWER			5	/* Get/Set power status */
#define CTRL_LOCK			6	/* Lock/Unlock media removal */
#define CTRL_EJECT			7	/* Eject media */

/* MMC/SDC specific ioctl command */
#define MMC_GET_TYPE		10	/* Get card type */
#define MMC_GET_CSD			11	/* Get CSD */
#define MMC_GET_CID			12	/* Get CID */
#define MMC_GET_OCR			13	/* Get OCR */
#define MMC_GET_SDSTAT		14	/* Get SD status */

/* ATA/CF specific ioctl command */
#define ATA_GET_REV			20	/* Get F/W revision */
#define ATA_GET_MODEL		21	/* Get model name */
#define ATA_GET_SN			22	/* Get serial number */

/* NAND specific ioctl command */
#define NAND_FORMAT			30	/* Create physical format */


#define _DISKIO
#endif
