#ifndef __QTYPES_H__
#define __QTYPES_H__

#include <stdint.h>

#ifdef _WIN32
#define _CRT_SECURE_NO_WARNINGS
#endif

typedef uint8_t  U8;
typedef uint16_t U16;
typedef uint32_t U32;
typedef uint64_t U64;

typedef int8_t  I8;
typedef int16_t I16;
typedef int32_t I32;
typedef int64_t I64;

typedef void VOID;

typedef enum QERR_E {
	ERR_OK = 0,
	ERR_IO,
	ERR_FSEEK,
	ERR_PARAM,
	ERR_NOTFOUND,
} QERR_T;


#endif