//
//  CTNumber.h
//  CTObject
//
//  Created by Carlo Tortorella on 24/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTObject.h"
#include <stdint.h>

typedef enum
{
    CTNUMBER_TYPE_ULONG,
    CTNUMBER_TYPE_LONG,
    CTNUMBER_TYPE_DOUBLE
} CTNUMBER_TYPE, * CTNUMBER_TYPERef;

union CTNumberValue
{
    uint64_t ULong;
    int64_t Long;
    long double Double;
};

typedef struct
{
    CTAllocatorRef alloc;
    union CTNumberValue value;
    CTNUMBER_TYPE type;
} CTNumber, * CTNumberRef;

typedef struct
{
    CTAllocatorRef alloc;
    CTNumberRef base;
    CTNumberRef exponent;
} CTLargeNumber, * CTLargeNumberRef;

CTNumberRef CTNumberCreateWithUnsignedInt(CTAllocatorRef restrict alloc, unsigned int integer);
CTNumberRef CTNumberCreateWithInt(CTAllocatorRef restrict alloc, int integer);
CTNumberRef CTNumberCreateWithUnsignedLong(CTAllocatorRef restrict alloc, uint64_t longInteger);
CTNumberRef CTNumberCreateWithLong(CTAllocatorRef restrict alloc, int64_t longInteger);
CTNumberRef CTNumberCreateWithDouble(CTAllocatorRef restrict alloc, long double floatingPoint);

CTNumberRef CTNumberCopy(CTAllocatorRef restrict alloc, CTNumberRef number);

/**
 * Compare two CTNumber objects
 * @param array	A properly initialised CTNumber that was created with CTNumberCreate*.
 * @return		A value indicating equality, 0 = false, 1 = true.
 **/
uint8_t CTNumberCompare(const CTNumber * restrict number1, const CTNumber * restrict number2);
uint8_t CTLargeNumberCompare(const CTLargeNumber * restrict number1, const CTLargeNumber * restrict number2);

void CTNumberRelease(CTNumberRef number);
void CTLargeNumberRelease(CTLargeNumberRef lnumber);

CTLargeNumberRef CTLargeNumberCreate(CTAllocatorRef restrict alloc, CTNumberRef base, CTNumberRef exponent);
CTLargeNumberRef CTLargeNumberCopy(CTAllocatorRef restrict alloc, CTLargeNumberRef number);
CTNumberRef CTLargeNumberBase(const CTLargeNumber * restrict number);
CTNumberRef CTLargeNumberExponent(const CTLargeNumber * restrict number);

void CTNumberSetUnsignedLongValue(CTNumberRef restrict number, uint64_t longInteger);
void CTNumberSetLongValue(CTNumberRef restrict number, int64_t longInteger);
void CTNumberSetDoubleValue(CTNumberRef restrict number, long double floatingPoint);

uint32_t CTNumberUnsignedIntValue(const CTNumber * restrict number);
int32_t CTNumberIntValue(const CTNumber * restrict number);
uint64_t CTNumberUnsignedLongValue(const CTNumber * restrict number);
int64_t CTNumberLongValue(const CTNumber * restrict number);
long double CTNumberDoubleValue(const CTNumber * restrict number);
CTNUMBER_TYPE CTNumberType(const CTNumber * restrict number);

/**
 * Return a CTObject encasing the CTNumber passed.
 * @param n	A properly initialised CTNumber that was created with CTNumberCreate*.
 * @return	The CTNumber wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObjectRef CTObjectWithNumber(CTAllocatorRef alloc, CTNumberRef restrict n);
CTObjectRef CTObjectWithLargeNumber(CTAllocatorRef alloc, CTLargeNumberRef restrict n);