#ifndef __QDISK_H__
#define __QDISK_H__

#include "QTYPES.h"

#define MBR_PRI_PART_NR 4

typedef enum {
	FS_INVALID       = 0x00,  // ��Ч�ļ�ϵͳ
	FS_FAT32         = 0x01,  // FAT32
	FS_EXTEND        = 0x05,  // ����÷�����1����չ����
	FS_WIN95_FAT32_0 = 0x0B,  // Windows95 FAT32_1
	FS_WIN95_FAT32_1 = 0x0C,  // Windows95 FAT32_2
} QFS_TYPE_T;

#pragma pack(push, 1)
typedef struct QMBR_PART {
	U8            bootActive;          /* ����,ָ���÷����Ƿ����� */
	U8            startHeader;         /* ��ʼ��ͷ */
	U16           startSector : 6;     /* ��ʼ���� */
	U16           startCylinder : 10;  /* �������� */
	U8            systemId;            /* ��������(��FAT32������չ����) */
	U8            endHeader;           /* ������ͷ */
	U16           endSector : 6;       /* �������� */
	U16           endCylinder : 10;    /* �������� */
	U32           relativeSectors;     /* ��������� */
	U32           totalSectors;        /* �÷����������� */
} QMBR_PART_T;

typedef struct QMBR {
	U8            code[446];                  /* �������� */
	QMBR_PART_T   partTable[MBR_PRI_PART_NR]; /* ������   */
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
	U32 sectorSize;              /* ������С     */
	U32 totalSector;             /* ��������     */
	QDISK_DRIVER_T* driver;      /* ��������     */
	VOID* Data;                  /* Data��File* */
	const char* name;            /* ��������     */
} QDISK_T;

QERR_T QDiskOpen(QDISK_T* disk, const char* name, QDISK_DRIVER_T* driver, VOID* initData);
QERR_T QDiskClose(QDISK_T* disk);
QERR_T QDiskGetPartCount(QDISK_T* disk, U32* count);
QERR_T QDiskReadSector(QDISK_T* disk, U8* buffer, U32 startSector, U32 count);
QERR_T QDiskWriteSector(QDISK_T* disk, U8* buffer, U32 startSector, U32 count);

#endif