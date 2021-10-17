#ifndef __QDISK_H__
#define __QDISK_H__

#include "QTYPES.h"

#define MBR_PRI_PART_NR 4

typedef enum {
	FS_INVALID       = 0x00,  // 无效文件系统
	FS_FAT32         = 0x01,  // FAT32
	FS_EXTEND        = 0x05,  // 代表该分区是1个扩展分区
	FS_WIN95_FAT32_0 = 0x0B,  // Windows95 FAT32_1
	FS_WIN95_FAT32_1 = 0x0C,  // Windows95 FAT32_2
} QFS_TYPE_T;

#pragma pack(push, 1)
typedef struct QMBR_PART {
	U8            bootActive;          /* 活动标记,指明该分区是否活动分区 */
	U8            startHeader;         /* 开始磁头 */
	U16           startSector : 6;     /* 开始扇区 */
	U16           startCylinder : 10;  /* 结束柱面 */
	U8            systemId;            /* 分区类型(如FAT32或者扩展分区) */
	U8            endHeader;           /* 结束磁头 */
	U16           endSector : 6;       /* 结束扇区 */
	U16           endCylinder : 10;    /* 结束柱面 */
	U32           relativeSectors;     /* 相对扇区数 */
	U32           totalSectors;        /* 该分区总扇区数 */
} QMBR_PART_T;

typedef struct QMBR {
	U8            code[446];                  /* 引导代码 */
	QMBR_PART_T   partTable[MBR_PRI_PART_NR]; /* 分区表   */
	U8            bootSig[2];                 /* 0x55AA   */
} QMBR_T;
#pragma pack(pop)

typedef struct QDISK QDISK_T;

typedef struct QDISK_DRIVER_S {
	QERR_T(*Open)(QDISK_T* disk, VOID* initData);
	QERR_T(*Close)(QDISK_T* disk);
	QERR_T(*ReadSector)(QDISK_T* disk, U8* buffer, U32 startSector, U32 count);
	QERR_T(*WriteSector)(QDISK_T* disk, U8* buffer, U32 startSector, U32 count);
} QDISK_DRIVER_T;

typedef struct QDISK {
	U32 sectorSize;              /* 扇区大小     */
	U32 totalSector;             /* 扇区数量     */
	QDISK_DRIVER_T* driver;      /* 磁盘驱动     */
	VOID* Data;                  /* Data存File* */
	const char* name;            /* 磁盘名称     */
} QDISK_T;

QERR_T QDiskOpen(QDISK_T* disk, const char* name, QDISK_DRIVER_T* driver, VOID* initData);
QERR_T QDiskClose(QDISK_T* disk);
QERR_T QDiskGetPartCount(QDISK_T* disk, U32* count);
QERR_T QDiskReadSector(QDISK_T* disk, U8* buffer, U32 startSector, U32 count);
QERR_T QDiskWriteSector(QDISK_T* disk, U8* buffer, U32 startSector, U32 count);

#endif