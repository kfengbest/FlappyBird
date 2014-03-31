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
#ifndef __Naga_Object_h__
#define __Naga_Object_h__

NAMESPACE_NAGA_BEGIN

/// <description>
/// Abstracted Layer for objects management
/// </description>
class NAGAAPI Object 
    : public NAGA UTxRefCounted, public NAGA UTxInterfaceID
{    
public:
    /// <description>
    /// constructor
    /// </description>
    Object(int count=0);

    /// <description>
    /// destructor
    /// </description>
    virtual~Object();

public:
    /// <description>
    /// query interface using interface ID
    /// </description>
    virtual void* GetInterface(UTxUniqueString* strInterfaceID) override;

    /// <description>
    /// release reference
    /// </description>
    virtual void ReleaseRef(void) override;

    /// <description>
    /// Add reference
    /// </description>
    virtual void AddRef(void) override;

    /// <description>
    /// Add reference
    /// </description>
    int RefCount(void) const override;

protected:
    /// <description>
    /// the ref count/ not thread safe currently
    /// </description>
    int mCount;

    /// <description>
    /// Disposes the object immediately and release resources.
    /// </description>
    virtual void Delete();
};


/// <description>
/// cast one object from const S type to const T type
/// </description>
template <typename T, typename S>
inline const T* CastInterface(const S* object)
{
    static_assert(STD is_convertible<T*, NAGA Object*>::value == true,
                  "type must be derived from Object");
    static_assert(STD is_convertible<S*, NAGA Object*>::value == true,
                  "type must be derived from Object");
    if (!object)
        return nullptr;
    return (const T*)const_cast<S*>(object)->GetInterface(T::InterfaceID());
}

/// <description>
/// cast one object from S type to T type
/// </description>
template <typename T, typename S>
inline T* CastInterface(S* object)
{
    static_assert(STD is_convertible<T*, Object*>::value == true,
                  "type must be derived from Object");
    static_assert(STD is_convertible<S*, Object*>::value == true,
                  "type must be derived from Object");
    if (!object)
        return nullptr;
    return (T*)object->GetInterface(T::InterfaceID());
}

/// <description>
/// utility functions for tower
/// </description>
template <typename T,typename S>
inline bool IsType(S* itm) {
    static_assert(STD is_convertible<T*, Object*>::value == true,
                  "type must be derived from Object");
    static_assert(STD is_convertible<S*, Object*>::value == true,
                  "type must be derived from Object");
    return itm->GetInterface(T::InterfaceID()) != nullptr;
}

template <typename _Ty>
inline bool IsType(NAGA UTxUniqueString* clsTag, _Ty* itm) {
    static_assert(STD is_convertible<_Ty*, NAGA Object*>::value == true,
                  "type must be derived from Object");
    return itm->GetInterface(clsTag) != nullptr;
}

template <typename _Ty>
inline int ObjectID()
{
    static_assert(STD is_convertible<_Ty*, Object*>::value == true,
                  "type must be derived from Object");
    static_assert(STD is_convertible<_Ty*, ClsObjectCount<_Ty>*>::value == true,
                  "type must be derived from ClsObjectCount");
    return _Ty::NetObjectID();
}


NAMESPACE_NAGA_END

#endif // __Naga_ObjectLayer_h__

