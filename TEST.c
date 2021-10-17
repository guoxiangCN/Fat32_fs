#include "QDISK.h"
#include "QTYPES.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern QDISK_DRIVER_T vdiskDriver;

#define DISKPATH "D:\\DISK_TEST.img"

static U8 ReadBuffer[512];
static U8 WriteBuffer[512];

VOID BUF_ZERO() {
	memset(ReadBuffer, 0, sizeof(ReadBuffer));
	memset(WriteBuffer, 0, sizeof(WriteBuffer));
}

I32 QDISK_DRIVER_TEST() {
	BUF_ZERO();

	QERR_T err = ERR_OK;
	QDISK_T disk;
	// disk.driver = &vdiskDriver;
	err = QDiskOpen(&disk, DISKPATH, &vdiskDriver, DISKPATH);
	if (err) {
		printf("Open error\n");
		exit(1);
	}

	printf("sectorSize: %d\n", disk.sectorSize);
	printf("totalSector: %d\n", disk.totalSector);
	U32 partCnt = 0;
	err = QDiskGetPartCount(&disk, &partCnt);
	if (err) {
		printf("QDIskGetPartCount error");
		exit(1);
	}
	printf("partCount:%d\n", partCnt);

	// Init Write buff
	for (int i = 0; i < sizeof(WriteBuffer); i++) {
		WriteBuffer[i] = 'a' + (i % 26);
	}

	// Write
	for (int i = 0; i < disk.totalSector; i++) {
		err = QDiskWriteSector(&disk, WriteBuffer, i, 1);
		if (err) {
			printf("WriterSector error, writingSector = %d\n", i);
			exit(1);
		}
		printf("sectorNum: %d, write done\n", i);
	}

	// Read
	for (int i = 0; i < disk.totalSector; i++) {
		err = QDiskReadSector(&disk, ReadBuffer, i, 1);
		if (err) {
			printf("ReadSector error, readingSector = %d\n", i);
			exit(1);
		}
		printf("sectorNum: %d, data: %s\n", i, (const char*)ReadBuffer);
	}

	err = QDiskClose(&disk);
	if (err) {
		printf("Close error\n");
		exit(1);
	}
}

int main(int argc, char** argv)
{
	QDISK_DRIVER_TEST();
	return 0;
}