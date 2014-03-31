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
	Author		:	Yu Li
	Description	:	Base class for Reference Counted Objects
	History		:	2012, Initial implementation.
*/
#ifndef __Naga_RefCount_H__
#define __Naga_RefCount_H__

NAMESPACE_NAGA_BEGIN

/// <description>
/// class UTxRefCounted provides base class for Using with SmartPointer
/// </description>
class NAGAAPI UTxRefCounted 
{	
public:
	/// <description>
	/// Constructors for reference counted objects. Default gives count of 0.
	/// </description>
	virtual~UTxRefCounted() {}

	/// <description>
	/// Adds 1 to the reference count. A newly created object has reference count of 0.
	/// </description>
	virtual void AddRef() = 0;

	/// <description>
	/// Decrements the reference count by one. Delayed dispose must be called explicitly.
	/// </description>
	virtual void ReleaseRef() = 0;

	/// <description>
	/// Returns the current ref count.
	/// </description>
	virtual int RefCount() const= 0;
}; // ref counted

NAMESPACE_NAGA_END

#endif // __Naga_RefCount_H__
