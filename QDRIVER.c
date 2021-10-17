#include "QDISK.h"
#include "QFAT.h"

#include <stdio.h>

#define SECTOR_SIZE 512

static QERR_T QDiskHwOpen(QDISK_T* disk, VOID* initData)
{
	const char* path = (const char*)initData;
	FILE* f = fopen(path, "rb+");
	if (f == NULL) {
		return ERR_IO;
	}

	if (fseek(f, 0, SEEK_END)) {
		return ERR_FSEEK;
	}

	disk->Data = f;
	disk->sectorSize = SECTOR_SIZE;
	disk->totalSector = ftell(f) / disk->sectorSize;

	return ERR_OK;
}

static QERR_T QDiskHwClose(QDISK_T* disk)
{
	fclose((FILE*)(disk->Data));
}

static QERR_T QDiskHwReadSector(QDISK_T* disk, U8* buffer, U32 startSector, U32 count)
{
	FILE* file = (FILE*) disk->Data;
	U32 offset = startSector * disk->sectorSize;

	int ret = fseek(file, offset, SEEK_SET);
	if (ret) {
		return ERR_FSEEK;
	}

	ret = fread(buffer, disk->sectorSize, count, file);
	if (ret == -1) {
		return ERR_IO;
	}

	return ERR_OK;
}
static QERR_T QDiskHwWriteSector(QDISK_T* disk, U8* buffer, U32 startSector, U32 count)
{
	FILE* file = (FILE*)disk->Data;
	U32 offset = startSector * disk->sectorSize;

	int ret = fseek(file, offset, SEEK_SET);
	if (ret) {
		return ERR_FSEEK;
	}

	ret = fwrite(buffer, disk->sectorSize, count, file);
	if (ret == -1) {
		return ERR_IO;
	}

	fflush(file);
	return ERR_OK;
}

QDISK_DRIVER_T vdiskDriver = {
	.Open = QDiskHwOpen,
	.Close = QDiskHwClose,
	.ReadSector = QDiskHwReadSector,
	.WriteSector = QDiskHwWriteSector,
};