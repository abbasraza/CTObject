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

CTObject * CTObjectCreate(CTAllocator * restrict alloc, void * ptr, unsigned long size)
{
    CTObject * object = CTAllocatorAllocate(alloc, sizeof(CTObject));
    object->size = size;
    object->value = ptr;
    object->alloc = alloc;
    return object;
}