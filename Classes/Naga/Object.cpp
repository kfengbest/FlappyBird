/*
Copyright 2012 NAGA.  All Rights Reserved.

The source code contained or described herein and all documents related
to the source code ("Material") are owned by NAGA or its suppliers or
licensors.  Title to the Material remains with NAGA or its suppliers and
licensors.  The Material is protected by worldwide copyright laws and
treaty provisions.  No part of the Material may be used, copied, reproduced,
modified, published, uploaded, posted, transmitted, distributed, or
disclosed in any way without NAGA's prior express written permission.

No license under any patent, copyright, trade secret or other
intellectual property right is granted to or conferred upon you by
disclosure or delivery of the Materials, either expressly, by
implication, inducement, estoppel or otherwise.  Any license under such
intellectual property rights must be express and approved by NAGA in
writing.
*/
/*
Author         :	Yu Li
Description	:	base object of Naga System
History		:	2013, Initial implementation.
*/
#include "NagaLib.h"

NAMESPACE_NAGA_BEGIN

/// <description>
/// constructor
/// </description>
Object::Object(int count/*=0*/)    
    : mCount(count)
{
    
}

/// <description>
/// destructor
/// </description>
Object::~Object()
{
}

/// <description>
/// query interface using interface ID
/// </description>
void* Object::GetInterface(UTxUniqueString* strInterfaceID)
{
    static UTxUniqueString* InterfaceID = NAGA Intern("NAGA.Object");
    if (InterfaceID == strInterfaceID)
        return this;
    return nullptr;
}

/// <description>
/// Disposes the object immediately and release resources.
/// </description>
void Object::Delete()  
{ 
    delete this; 
}

/// <description>
/// release reference
/// </description>
void Object::ReleaseRef(void)
{
    if(--mCount <= 0)
        Delete(); 
}

/// <description>
/// Add reference
/// </description>
void Object::AddRef(void)
{
    mCount++; 
}

/// <description>
/// Add reference
/// </description>
int Object::RefCount(void) const
{
    return mCount;
}

NAMESPACE_NAGA_END
