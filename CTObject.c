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
#include "CTObject.h"
#include "CTDictionary.h"
#include "CTArray.h"
#include "CTNumber.h"
#include "CTString.h"
#include "CTNull.h"

CTObject * CTObjectCreate(CTAllocator * restrict alloc, void * ptr, int8_t type)
{
    CTObject * object = CTAllocatorAllocate(alloc, sizeof(CTObject));
    object->alloc = alloc;
    object->ptr = ptr;
    object->type = type;
    return object;
}

CTObject * CTObjectCreate2(CTAllocator * restrict alloc, void * ptr, int8_t type, uint64_t size)
{
    CTObject * object = CTObjectCreate(alloc, ptr, type);
    object->size = size;
    return object;
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
		}
	}
	return 0;
}

void * CTObjectValue(const CTObject * restrict object)
{
	return object->ptr;
}

int8_t CTObjectType(const CTObject * restrict object)
{
	return object->type;
}

uint64_t CTObjectSize(const CTObject * restrict object)
{
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
            
        case CTOBJECT_TYPE_STRING:
            CTStringRelease(object->ptr);
            break;
			
		case CTOBJECT_TYPE_NULL:
			CTNullRelease(object->ptr);
			break;
    }
    
    CTAllocatorDeallocate(object->alloc, object);
}