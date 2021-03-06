//
//  CTString.c
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include "CTString.h"
#include "CTFunctions.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

CTStringRef CTStringCreate(CTAllocatorRef restrict alloc, const char * restrict characters)
{
    CTStringRef string = CTAllocatorAllocate(alloc, sizeof(CTString));
    string->alloc = alloc;
	
    string->characters = stringDuplicate(alloc, characters ? characters : "");
	if (characters)
	{
		CTStringSetLength(string, strlen(characters));
	}
	string->modified = 1;
    return string;
}

CTStringRef CTStringCopy(CTAllocatorRef restrict alloc, const CTString * string)
{
	return CTStringCreate(alloc, string->characters);
}

void CTStringRelease(CTStringRef string)
{
    CTAllocatorDeallocate(string->alloc, string->characters);
    CTAllocatorDeallocate(string->alloc, string);
}

inline const char * CTStringUTF8String(const CTString * restrict string)
{
	return string->characters;
}

inline uint64_t CTStringLength(const CTString * restrict string)
{
	return string->length;
}

void CTStringSetLength(CTStringRef restrict string, uint64_t length)
{
	string->length = length;
}

hash_t CTStringHash(CTString * restrict string)
{
	if (!string->modified)
	{
		return string->hash;
	}
	string->modified = 0;
	return string->hash = CTStringCharHash(CTStringUTF8String(string));
}

hash_t CTStringCharHash(const char * restrict string)
{
	uint64_t hash = 0;
	size_t len = strlen(string);
	for(uint64_t count = 0; count < len; ++count)
	{
		hash += (hash << 5) + string[count];
	}
	return hash;
}

void CTStringPrependCharacters(CTStringRef restrict string, const char * restrict characters, int64_t limit)
{
	const uint64_t length = limit < 0 ? strlen(characters) : limit;
	string->characters = CTAllocatorReallocate(string->alloc, string->characters, string->length + length + 1);
	memmove(string->characters + length, string->characters, string->length + 1);
	memcpy(string->characters, characters, length);
	string->length += length;
	string->modified = 1;
}

void CTStringPrependCharacter(CTStringRef restrict string, char character)
{
	string->characters = CTAllocatorReallocate(string->alloc, string->characters, string->length + 2);
	memmove(string->characters + 1, string->characters, string->length + 1);
	string->characters[0] = character;
	++string->length;
	string->modified = 1;
}

void CTStringAppendCharacters(CTStringRef restrict string, const char * restrict characters, int64_t limit)
{
	const uint64_t length = limit < 0 ? strlen(characters) : limit;
	string->characters = CTAllocatorReallocate(string->alloc, string->characters, CTStringLength(string) + length + 1);
	memcpy(string->characters + string->length, characters, length);
	string->length += length;
	string->characters[string->length] = 0;
	for (uint64_t i = 0; i < length; ++i)
	{
		string->hash += (string->hash << 5) + characters[i];
	}
}

void CTStringAppendCharacter(CTStringRef restrict string, char character)
{
	string->characters = CTAllocatorReallocate(string->alloc, string->characters, string->length + 2);
	string->characters[string->length] = character;
	++string->length;
	string->characters[string->length] = 0;
	string->hash += (string->hash << 5) + character;
}

void CTStringSet(CTStringRef restrict string, const char * restrict characters)
{
    CTAllocatorDeallocate(string->alloc, string->characters);
    string->characters = stringDuplicate(string->alloc, characters);
    CTStringSetLength(string, strlen(characters));
	string->modified = 1;
}

void CTStringRemoveCharactersFromStart(CTStringRef restrict string, unsigned long count)
{
    if (count < CTStringLength(string))
    {
		memmove(string->characters, string->characters + count, string->length - count + 1);
		CTAllocatorReallocate(string->alloc, string->characters, string->length - count + 1);
		string->length -= count;
    }
    else
    {
        string->characters = CTAllocatorReallocate(string->alloc, string->characters, 1);
		string->characters[0] = 0;
		string->length = 0;
    }
	string->modified = 1;
}

void CTStringRemoveCharactersFromEnd(CTStringRef restrict string, unsigned long count)
{
    if (count < CTStringLength(string))
    {
		CTAllocatorReallocate(string->alloc, string->characters, string->length - count + 1);
		string->characters[string->length - count] = 0;
		string->length -= count;
    }
    else
    {
        string->characters = CTAllocatorReallocate(string->alloc, string->characters, 1);
		string->characters[0] = 0;
		string->length = 0;
    }
	string->modified = 1;
}

void CTStringAppendString(CTStringRef restrict string1, const CTString * restrict string2)
{
	CTStringAppendCharacters(string1, CTStringUTF8String(string2), CTStringLength(string2));
}

void CTStringToUpper(CTStringRef restrict string)
{
	for (uint64_t i = 0; i < CTStringLength(string); ++i)
	{
		string->characters[i] = toupper(string->characters[i]);
	}
	string->modified = 1;
}

void CTStringToLower(CTStringRef restrict string)
{
	for (uint64_t i = 0; i < CTStringLength(string); ++i)
	{
		string->characters[i] = tolower(string->characters[i]);
	}
	string->modified = 1;
}

const char * CTStringStringBetween(const CTString * restrict string, const char * restrict search1, const char * restrict search2)
{
	uint64_t index = 0;
	char * ret1 = NULL, * ret2 = NULL;
	
	while ((!ret1 && !ret2) || ret1 >= ret2)
	{
		if (index + strlen(search1) >= CTStringLength(string) || !(ret1 = strstr(CTStringUTF8String(string) + index, search1)) || !(ret2 = strstr(CTStringUTF8String(string) + index + strlen(search1), search2)))
		{
			return NULL;
		}
		if (ret1 < ret2)
		{
			char * retVal = CTAllocatorAllocate(string->alloc, ret2 - (ret1 + strlen(search1)));
			strncpy(retVal, ret1 + strlen(search1), ret2 - (ret1 + strlen(search1)));
			return retVal;
		}
		index = ret1 - CTStringUTF8String(string);
		if (index >= CTStringLength(string)) return NULL;
	}
	return NULL;
}

uint8_t CTStringContainsString(const CTString * restrict string, const char * restrict search)
{
	return strstr(CTStringUTF8String(string), search) != NULL;
}

int8_t CTStringCompare(CTString * restrict string1, CTStringRef restrict string2)
{
	return CTStringHash(string1) != CTStringHash(string2);
}

int8_t CTStringCompare2(CTString * restrict string1, const char * restrict string2)
{
	uint64_t ret = 0;
	uint64_t length = strlen(string2);
	for (uint64_t count = 0; count < length; ++count)
	{
		ret += (ret << 5) + string2[count];
	}
	return CTStringHash(string1) != ret;
}

uint8_t CTStringIsEqual(CTString * restrict string1, CTString * restrict string2)
{
	return CTStringHash(string1) == CTStringHash(string2);
}

uint8_t CTStringIsEqual2(CTString * restrict string1, const char * restrict string2)
{
	return CTStringHash(string1) == CTStringCharHash(string2);
}

CTObjectRef CTObjectWithString(CTAllocatorRef alloc, CTString * restrict str)
{
	return CTObjectCreate(alloc, str, CTOBJECT_TYPE_STRING);
}

CTStringRef CTStringReplaceCharacterWithCharacters(CTAllocatorRef alloc, const CTString * restrict string, const char * (^repFn)(const char))
{
	CTStringRef ret_val = CTStringCreate(alloc, "");
	for (uint64_t index = 0; index < string->length; ++index)
	{
		const char * characters = repFn(string->characters[index]);
		if (characters)
		{
			CTStringAppendCharacters(ret_val, characters, CTSTRING_NO_LIMIT);
		}
		else
		{
			CTStringAppendCharacter(ret_val, string->characters[index]);
		}
	}
	return ret_val;
}