#include "diskio.h"//
#include "string.h"//
#include "sdcard.h"//

#include "other.h"	//提供LED闪烁

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */

/*-----------------------------------------------------------------------*/
/* Correspondence between physical drive number and physical drive.      */
/*-----------------------------------------------------------------------*/


/*-----------------------------------------------------------------------*/
/*	存储器初始化程序                                                   */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE drv				/* 磁盘号:0 */
)
{

	switch (drv)
	{
		case 0 : //SD
		{
			if(SD_Init() != SD_OK)
				return RES_ERROR;
			else
			{	
				SDMMC0_MOV_Flag = 0;	//由于卡已经初始化成功了,因此去除标志
				return RES_OK;
			}
				
		}
		default : return RES_ERROR;
	}

}





/*-----------------------------------------------------------------------*/
/* 磁盘状态                                                  */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE drv		/* 磁盘号 */
)
{
	switch (drv)
	{
		case 0: 
		{
			if(SDMMC0_MOV_Flag)//卡移除了
				return  RES_NOTRDY;
			else
				return  RES_OK;
		}
		default : return RES_ERROR;
	}
}



/*-----------------------------------------------------------------------*/
/* 读扇区                                                       */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE drv,		/* Physical drive nmuber (0..) */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address (LBA) */
	BYTE count		/* Number of sectors to read (1..255) */
)
{
	SD_Error Status = SD_OK;
//	u8 i;
	

	switch (drv)
	{
		case 0 :
		{
			if(count > 1)
			{
				LED0_ON();
				Status = SD_ReadMultiBlocks(sector,(u32 *)buff,count);
				LED0_OFF();
			}
			else
			{
				LED0_ON();
				Status = SD_ReadBlock(sector,(u32 *)buff);
				LED0_OFF();
			}
			if(Status != SD_OK)	
				return 	RES_ERROR;		
		}break;
		default : return RES_ERROR;
	}
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* 写扇区                                                      */
/*-----------------------------------------------------------------------*/
/* The FatFs module will issue multiple sector transfer request
/  (count > 1) to the disk I/O layer. The disk function should process
/  the multiple sector transfer properly Do. not translate it into
/  multiple single sector transfers to the media, or the data read/write
/  performance may be drasticaly decreased. */

#if _READONLY == 0
DRESULT disk_write (
	BYTE drv,			/* 磁盘号*/
	const BYTE *buff,	/* 要写入的数据 */
	DWORD sector,		/* 要写入的扇区起始地址 */
	BYTE count			/* 要写的扇区数1-255 */
)
{
	SD_Error Status = SD_OK;
	//	u8 i;
		

		switch (drv)
		{
			case 0 :
			{
				if(count > 1)
				{
					LED0_ON();
					Status = SD_WriteMultiBlocks(sector,(u32 *)buff,count);
					LED0_OFF();
				}
				else
				{
					LED0_ON();
					Status = SD_WriteBlock(sector,(u32 *)buff);
					LED0_OFF();
				}
				if(Status != SD_OK)	
					return 	RES_ERROR;		
			}break;
			default : return RES_ERROR;
		}
		return RES_OK;
}
#endif /* _READONLY */



/*-----------------------------------------------------------------------*/
/* 其它功能                                              */
/*-----------------------------------------------------------------------*/

DRESULT disk_ioctl (
	BYTE drv,		/* 磁盘号，只支持0 */
	BYTE ctrl,		/* 功能选择 */
	void *buff		/* 数据缓存*/
)
{
	/*if(drv == DISK_NAND)
	{
		switch (ctrl)
		{
		 	case CTRL_SYNC:	//功能0：强制写入
				 return  RES_OK;	 //成功
			case GET_SECTOR_COUNT://功能1:磁盘扇区数
				 *(u32*)buff = FLASH_MAX_SECTOR_ADDR/FLASH_SECTOR_SIZE; //NAND_ZONE_SIZE * NAND_BLOCK_SIZE * NAND_MAX_ZONE ;
				 break;
			case GET_SECTOR_SIZE://功能2：磁盘扇区大小
				 *(u32*)buff = 512;
				 break;
			case GET_BLOCK_SIZE://功能3：磁盘簇大小
				 *(u32*)buff = FLASH_SECTOR_SIZE;//NAND_PAGE_SIZE;
				 break;
			default:
				 break;
		}
	}	*/ 

	return RES_OK;
}


/* RTC function */
//为文件系统提供时钟
#if !_FS_READONLY
#include "rtc.h"
DWORD get_fattime (void)
{
	u16 FatTime = 0,FatDate = 0;

/*	FatTime = (u16)( timer.sec/2 );
	FatTime |= (u16)( timer.min << 5 );
	FatTime |= (u16)( timer.hour << 11); 
	FatDate = (u16)( timer.w_date);
	FatDate |= (u16)( timer.w_month <<5);
	FatDate |= (u16)((timer.w_year - 1980) <<9);*/

    return (DWORD)( (FatDate<<16 )+FatTime ); 
}

#endif



//重入相关
#if _FS_REENTRANT
#include "ucos_ii.h"

int ff_cre_syncobj(BYTE vol, _SYNC_t *pID)
{
	//u8 error;
	//Debug("%s\n\r",__FUNCTION__);
	*pID=0;
	//gSD_Mutex=OS_MutexCreate(OS_HIGHEST_PRIORITIES,&error);//kalrno add
	return TRUE;
}

int ff_del_syncobj(_SYNC_t ID)
{
	//Debug("%s\n\r",__FUNCTION__);
	return TRUE;
}

int ff_req_grant(_SYNC_t ID)
{
	//u8 error;

	OSSchedLock();
	//OS_MutexTake(gSD_Mutex,OS_MAX_DELAY,&error);
	//Debug("<%c %d",*(u8 *)OS_TaskGetName(),ret);
	return TRUE;
}

void ff_rel_grant(_SYNC_t ID)
{
	OSSchedUnlock();
	//Debug("|%c>\n\r",*(u8 *)OS_TaskGetName());
	//OS_MutexGive(gSD_Mutex);
}

#endif






