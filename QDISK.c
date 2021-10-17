#include "QDISK.h"

QERR_T QDiskOpen(QDISK_T* disk, const char* name, QDISK_DRIVER_T* driver, VOID* initData) {
	disk->name = name;
	disk->driver = driver;
	return driver->Open(disk, initData);
}

QERR_T QDiskClose(QDISK_T* disk) {
	return disk->driver->Close(disk);
}

static QERR_T __QDiskGetExtendPartCount(QDISK_T* disk, U32 startSector, U32* extCount) {
	// TODO.
	return ERR_OK;
}

QERR_T QDiskGetPartCount(QDISK_T* disk, U32* count) {
	/* 暂定一个扇区512字节 */
	char diskBuf[512] = { 0 };
	QERR_T err = QDiskReadSector(disk, diskBuf, 0, 1);
	if (err) return err;

	U8 extend_part_flag = 0;
	U32 startSectors[4] = { 0 };
	U32 c = 0;

	QMBR_PART_T* part = ((QMBR_T*)diskBuf)->partTable;
	for (int i = 0; i < MBR_PRI_PART_NR; i++, part++) {
		if (part->systemId == FS_INVALID) {
			continue;
		}
		else if (part->systemId == FS_EXTEND) {
			// 如果是扩展分区
			startSectors[i] = part->relativeSectors;
			extend_part_flag |= 1 << i;
		}
		else {
			c++;
		}
	}

	if (extend_part_flag) {
		for (int i = 0; i < MBR_PRI_PART_NR; i++) {
			if (extend_part_flag & (1 << i)) {
				U32 extCount = 0;
				if (err = __QDiskGetExtendPartCount(disk, startSectors[i], &extCount)) {
					return err;
				}
				c += extCount;
			}
		}
	}

	*count = c;
	return ERR_OK;
}

QERR_T QDiskReadSector(QDISK_T* disk, U8* buffer, U32 startSector, U32 count) {
	if (startSector + count > disk->totalSector) {
		return ERR_PARAM;
	}
	return disk->driver->ReadSector(disk, buffer, startSector, count);
}

QERR_T QDiskWriteSector(QDISK_T* disk, U8* buffer, U32 startSector, U32 count) {
	if (startSector + count > disk->totalSector) {
		return ERR_PARAM;
	}
	return disk->driver->WriteSector(disk, buffer, startSector, count);
}