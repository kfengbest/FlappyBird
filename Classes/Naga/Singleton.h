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
	Description	:	Template bases for Singleton objects
	History		:	2012, Initial implementation.
*/
#ifndef __Naga_Singleton_H__
#define __Naga_Singleton_H__

NAMESPACE_NAGA_BEGIN

template <typename _Ty,typename = void>
    struct _CreatePolicy 
    {
        static _Ty * Create() 
        {
            return new _Ty;
        }
    };

/// <description>
/// base class for Lifetime Controlled Singleton Pattern
/// note that the InstancePtr method is _NOT_ thread-safe 
/// </description>
template <typename _Ty, typename _Creator = _CreatePolicy<_Ty> >
class Singleton 
{
    typedef _Ty* PointerType;
    
private:
    /// <description>
	/// the singleton instance
	/// </description>s
	static PointerType  mInstancePtr;

protected:
	/// <description>
	/// constructor
	/// </description>
	Singleton() {
		NASSERT(!mInstancePtr);
	}
    
public:
	/// <description>
	/// destructor
	/// </description>
	virtual ~Singleton() {
		mInstancePtr = nullptr;
	}

public:
	/// <description>
	/// Get the Instance Pointer 
	/// </description>
	static INLINE _Ty* InstancePtr() 
	{	
		if (mInstancePtr == nullptr)
		{
            mInstancePtr = _Creator::Create();
		}

		return mInstancePtr;
	}

	/// <description>
	/// Get the Instance Reference
	/// </description>
	static INLINE _Ty& Instance() {	
		return *InstancePtr();	
	}
};

template <typename _Ty, typename _Cr>
	typename Singleton<_Ty,_Cr>::PointerType  Singleton<_Ty,_Cr>::mInstancePtr; 

NAMESPACE_NAGA_END

#endif // !defined __Naga_Singleton_H__
