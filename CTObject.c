//
//  CTArray.c
//  CTObject
//
//  Created by Carlo Tortorella on 22/10/13.
//  Copyright (c) 2013 Carlo Tortorella. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "CTObject.h"
#include "CTDictionary.h"
#include "CTArray.h"
#include "CTNumber.h"
#include "CTString.h"
#include "CTNull.h"

CTObject * CTObjectCreate(CTAllocator * restrict alloc, void * ptr, CTOBJECT_TYPE type)
{
    CTObject * object = CTAllocatorAllocate(alloc, sizeof(CTObject));
    object->alloc = alloc;
    object->ptr = ptr;
    object->type = type;
    return object;
}

CTObject * CTObjectCopy(CTAllocator * restrict alloc, const CTObject * restrict object)
{
	switch(object->type)
	{
		case CTOBJECT_TYPE_DICTIONARY:
			return CTObjectWithDictionary(alloc, CTDictionaryCopy(alloc, object->ptr));
		case CTOBJECT_TYPE_ARRAY:
			return CTObjectWithArray(alloc, CTArrayCopy(alloc, object->ptr));
		case CTOBJECT_TYPE_NUMBER:
			return CTObjectWithNumber(alloc, CTNumberCopy(alloc, object->ptr));
		case CTOBJECT_TYPE_LARGE_NUMBER:
			return CTObjectWithLargeNumber(alloc, CTLargeNumberCopy(alloc, object->ptr));
		case CTOBJECT_TYPE_STRING:
			return CTObjectWithString(alloc, CTStringCopy(alloc, object->ptr));
		default:
			return CTObjectWithNull(alloc, CTNullCreate(alloc));
	}
}

uint8_t CTObjectCompare(const CTObject * restrict object1, const CTObject * restrict object2)
{
	if (object1->type == object2->type)
	{
		switch(object1->type)
		{
			case CTOBJECT_TYPE_DICTIONARY:
				return CTDictionaryCompare(object1->ptr, object2->ptr);
			case CTOBJECT_TYPE_ARRAY:
				return CTArrayCompare(object1->ptr, object2->ptr);
			case CTOBJECT_TYPE_NUMBER:
				return CTNumberCompare(object1->ptr, object2->ptr);
			case CTOBJECT_TYPE_LARGE_NUMBER:
				return CTLargeNumberCompare(object1->ptr, object2->ptr);
			case CTOBJECT_TYPE_STRING:
				return CTStringCompare(object1->ptr, object2->ptr) == 0;
			case CTOBJECT_TYPE_NULL:
				return 1;
			case CTOBJECT_NOT_AN_OBJECT:
				return 0;
		}
	}
	return 0;
}

inline void * CTObjectValue(const CTObject * restrict object)
{
	assert(object);
	return object->ptr;
}

inline CTOBJECT_TYPE CTObjectType(const CTObject * restrict object)
{
	assert(object);
	return object->type;
}

inline uint64_t CTObjectSize(const CTObject * restrict object)
{
	assert(object);
	return object->size;
}

void CTObjectRelease(CTObject * object)
{
    switch (object->type)
    {
        case CTOBJECT_TYPE_DICTIONARY:
            CTDictionaryRelease(object->ptr);
            break;
            
        case CTOBJECT_TYPE_ARRAY:
            CTArrayRelease(object->ptr);
            break;
            
        case CTOBJECT_TYPE_NUMBER:
            CTNumberRelease(object->ptr);
			break;
			
		case CTOBJECT_TYPE_LARGE_NUMBER:
			break;
            
        case CTOBJECT_TYPE_STRING:
            CTStringRelease(object->ptr);
            break;
			
		case CTOBJECT_TYPE_NULL:
			CTNullRelease(object->ptr);
			break;
			
		case CTOBJECT_NOT_AN_OBJECT:
			break;
    }
    CTAllocatorDeallocate(object->alloc, object);
}