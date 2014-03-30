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
	Description	:	Define Interface ID to replace RTTI usages
	History		:	2012, Initial implementation.
*/
#ifndef ___Naga_InterfaceID_h__
#define ___Naga_InterfaceID_h__

NAMESPACE_NAGA_BEGIN

/// class forwards
class UTxUniqueString;

/// <description>
/// base class for interface id definition
/// </description>
class NAGAAPI UTxInterfaceID
{
public:
    /// <description>
    /// Destructor.
    /// </description>
    virtual ~UTxInterfaceID(){}

    /// <description>
    /// checks to see if the interface ID is corresponding to the given id
    /// </description>
    virtual void* GetInterface(UTxUniqueString* interfaceID) = 0;
};

/// <description>
/// in track of count of this class
/// </description>
template <class _Ty>
class ClsObjectCount 
{
public:
	ClsObjectCount()
	{
		_Ty::IncreaseObjectCount();
	}

	ClsObjectCount(const ClsObjectCount&)
	{
		_Ty::IncreaseObjectCount();
	}

	virtual~ClsObjectCount()
	{
		_Ty::DecreaseObjectCount();
	}
};

NAMESPACE_NAGA_END

/// <description>
/// declare InterfaceID methods 
/// </description>
#define DECLARE_INTERFACE_ID()\
public:\
	static NAGA UTxUniqueString* InterfaceID();\
    virtual void* GetInterface(NAGA UTxUniqueString*) override

/// <description>
/// define InterfaceID methods 
/// </description>
#define DEFINE_INTERFACE_ID(parentCls,cls,tag)\
	NAGA UTxUniqueString* cls::InterfaceID()\
	{\
		static NAGA UTxUniqueString* pInterfaceID = NAGA Intern(tag);\
		return pInterfaceID;\
	}\
	void* cls::GetInterface(NAGA UTxUniqueString* t)\
	{\
		if (InterfaceID() == t)\
			return this;\
		return parentCls::GetInterface(t); \
	}

/// <description>
/// declare ClsObjectCount 
/// </description>
#define USE_CLSOBJECTCOUNT(cls) public NAGA ClsObjectCount<cls>

#define DECLARE_CLSOBJECTCOUNT()\
	private: \
		static int mClsObjectCount;\
		static int mClsObjectID;\
	public: \
		static INLINE int NetObjectCount()\
		{\
			return mClsObjectCount;\
		}\
		static INLINE int NetObjectID()\
		{\
			return mClsObjectID;\
		}\
		static INLINE void IncreaseObjectCount()\
		{\
			++ mClsObjectID; \
			++ mClsObjectCount;\
		}\
		static INLINE void DecreaseObjectCount()\
		{\
			-- mClsObjectCount;\
		}

/// <description>
/// define ClsObjectCount
/// </description>
#define DEFINE_CLSOBJECTCOUNT(cls)\
	 int cls::mClsObjectCount = 0;\
	 int cls::mClsObjectID=0;

#endif // !defined ___Naga_InterfaceID_h__
