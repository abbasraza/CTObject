//
//  CTString.h
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#pragma once
#include "CTAllocator.h"
#include "CTObject.h"

#define CTSTRING_NO_LIMIT -1

typedef uint64_t hash_t;

typedef struct
{
    CTAllocatorRef alloc;
    uint64_t length;
    char * characters;
	uint64_t hash;
	uint8_t modified;
} CTString, * CTStringRef;

CTStringRef CTStringCreate(CTAllocatorRef restrict alloc, const char * restrict characters);
CTStringRef CTStringCopy(CTAllocatorRef restrict alloc, const CTString * string);
void CTStringRelease(CTStringRef string);

hash_t CTStringCharHash(const char * restrict string);

const char * CTStringUTF8String(const CTString * restrict string);
uint64_t CTStringLength(const CTString * restrict string);
hash_t CTStringHash(CTString * restrict string);
void CTStringSetLength(CTStringRef restrict string, uint64_t length);
void CTStringAppendCharacters(CTStringRef restrict string, const char * restrict characters, int64_t limit);
void CTStringAppendCharacter(CTStringRef restrict string, char character);
void CTStringPrependCharacters(CTStringRef restrict string, const char * restrict characters, int64_t limit);
void CTStringPrependCharacter(CTStringRef restrict string, char character);
void CTStringSet(CTStringRef restrict string, const char * restrict characters);
void CTStringRemoveCharactersFromStart(CTStringRef restrict string, unsigned long count);
void CTStringRemoveCharactersFromEnd(CTStringRef restrict string, unsigned long count);

void CTStringAppendString(CTStringRef restrict string1, const CTString * restrict string2);
void CTStringToUpper(CTStringRef restrict string);
void CTStringToLower(CTStringRef restrict string);

const char * CTStringStringBetween(const CTString * restrict string, const char * restrict search1, const char * restrict search2);
uint8_t CTStringContainsString(const CTString * restrict string, const char * restrict search);
int8_t CTStringCompare(CTString * restrict string1, CTString * restrict string2);
int8_t CTStringCompare2(CTString * restrict string1, const char * restrict string2);
uint8_t CTStringIsEqual(CTString * restrict string1, CTString * restrict string2);
uint8_t CTStringIsEqual2(CTString * restrict string1, const char * restrict string2);
CTStringRef CTStringReplaceCharacterWithCharacters(CTAllocatorRef alloc, const CTString * restrict string, const char * (^repFn)(const char));

/**
 * Return a CTObject encasing the CTString passed.
 * @param str	A properly initialised CTString that was created with CTStringCreate.
 * @return		The CTString wrapped in a CTObject. The result is identical to using CTObjectCreate.
 **/
CTObjectRef CTObjectWithString(CTAllocatorRef alloc, CTString * restrict str);