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
	Description	:	Smart Pointer
	History		:	2012, Initial implementation.
*/
#ifndef __Naga_SmartPointer_H__
#define __Naga_SmartPointer_H__

NAMESPACE_NAGA_BEGIN

/// <description>
/// template class SmartPointer 
/// general usages to hold class instances derived from Object
/// 
/// class SmartPointer is dedicated for Cocos2DX usages
/// especially useful for such scenarios, 
/// 1. static class::create functions 
///		 SmartPointer<Tower> tower = new Tower(type, pt);
///			if (tower && tower->init())
///			{
///				tower->autorelease();
///				return tower;
///			}
///		 
///      tower = nullptr;
///      return nullptr;
/// 2. created for class members 
/// 
/// </description>

template <typename T, typename = void>
    class PointerTrait {};

template <typename T>
    class PointerTrait<T, typename STD enable_if<STD is_base_of<NAGA UTxRefCounted, T>::value>::type>
    {
       //static_assert(STD is_base_of<NAGA UTxRefCounted, T>::value, "Must be derived from UTxRefCounted");

    public:
       static INLINE void AddRef(T* pointer) {
            
           if (pointer) {
                pointer->AddRef();
           }
       }

       static INLINE void ReleaseRef(T* pointer) {
           if (pointer) {
               pointer->ReleaseRef();
           }           
       }
    };

template <class T, class S = PointerTrait<T>>
    class SmartPointer
    {
        typedef S Traits;

    public:
	    /// <description>
	    /// constructor
	    /// </description>
	    SmartPointer() 
		    : m_pkObject(nullptr)
	    {
	    }

	    /// <description>
	    /// constructors
	    /// </description>
	    SmartPointer (T* pkObject)
		    : m_pkObject(pkObject) 
	    {        
            Traits::AddRef(pkObject);
	    }

	    /// <description>
	    /// copy constructor 
	    /// </description>
	    SmartPointer (const SmartPointer& rkSmartPointer)
		    : m_pkObject(rkSmartPointer.m_pkObject)
	    {		
		    Traits::AddRef(m_pkObject);
	    }

	    /// <description>
	    /// copy constructor 
	    /// pointers should be convertible, e.g. pointers to derived instance is assigned to pointers to base class
	    /// </description>
	    template <typename _Ty>
	    SmartPointer (const SmartPointer<_Ty>& rkSmartPointer, 
		    typename STD enable_if<STD is_convertible<_Ty*, T*>::value,void>::type ** = 0)
		    : m_pkObject(rkSmartPointer.m_pkObject)
	    {		
		    Traits::AddRef(m_pkObject);
	    }

	    /// <description>
	    /// move constructor 
	    /// pointers should be convertible, e.g. pointers to derived instance is assigned to pointers to base class
	    /// </description>
	    template <typename _Ty>
	    SmartPointer (SmartPointer<_Ty>&& rkSmartPointer, 
		    typename STD enable_if<STD is_convertible<_Ty*, T*>::value,void>::type ** = 0)
		    : m_pkObject(rkSmartPointer.m_pkObject)
	    {	
		    rkSmartPointer.m_pkObject = nullptr;		
	    }

	    /// <description>
	    /// destruction
	    /// </description>
	    ~SmartPointer () 
	    {
		    Traits::ReleaseRef(m_pkObject);
	    }

    public:
	    /// <description>
	    /// implicit conversions
	    /// </description>	
	    INLINE operator T* () const {	return m_pkObject;	}

	    /// <description>
	    /// dereference 
	    /// </description>
	    INLINE T& operator* () const {	return *m_pkObject;	}

	    /// <description>
	    /// dereference 
	    /// </description>
	    INLINE T* operator-> () const {	return m_pkObject;	}
    
	    /// <description>
	    /// assignment
	    /// </description>
	    template <typename _Ty>
	    INLINE SmartPointer& operator= (_Ty* pkObject)
	    {
		    static_assert(STD is_convertible<_Ty*, T*>::value == true, 
			    "2 types are not convertible");
		    if (m_pkObject != pkObject)
		    {
                Traits::AddRef(pkObject);
			    Traits::ReleaseRef(m_pkObject);

			    m_pkObject = pkObject;
		    }
		    return *this;
	    }

	    /// <description>
	    /// assignment operator
	    /// </description>	
	    INLINE SmartPointer& operator= (T* pkObject)
	    {
		    if (m_pkObject != pkObject)
		    {
                Traits::AddRef(pkObject);
                Traits::ReleaseRef(m_pkObject);

			    m_pkObject = pkObject;
		    }

		    return *this;
	    }

        /// <description>
        /// assignment operator
        /// </description>	
        INLINE SmartPointer& operator= (SmartPointer& rkReference)
        {        
            if (m_pkObject != rkReference.m_pkObject)
            {
                Traits::AddRef(rkReference.m_pkObject);
                Traits::ReleaseRef(m_pkObject);

                m_pkObject = rkReference.m_pkObject;
            }

            return *this;
        }

	    /// <description>
	    /// assignment operator
	    /// </description>	
	    template <typename _Ty>
	    INLINE SmartPointer& operator= (SmartPointer<_Ty>& rkReference)
	    {
		    static_assert(STD is_convertible<_Ty*, T*>::value == true, 
			    "2 types are not convertible");
		    if (m_pkObject != rkReference.m_pkObject)
		    {
                Traits::AddRef(rkReference.m_pkObject);
                Traits::ReleaseRef(m_pkObject);

			    m_pkObject = rkReference.m_pkObject;
		    }
		    return *this;
	    }

    public:
	    /// <description>
	    /// if the shared object is Null
	    /// </description>
	    INLINE bool IsNull() const {	return m_pkObject == nullptr;	}

	    /// <description>
	    /// return the object directly
	    /// </description>
	    INLINE T* Pointer() {	return m_pkObject;	}

	    /// <description>
	    /// assign a newly allocated resource to smart pointer
	    /// Object already initialize its reference to 1, so no need to retain it
	    /// </description>
	    INLINE SmartPointer& Attach(T* pointer) 
	    {
		    m_pkObject = pointer;		
		    return *this;
	    }

	    /// <description>
	    /// Detach the object from the smart pointer
	    /// NOTE, the object is still retained
	    /// </description>
	    INLINE T* Detach()  
	    { 
		    T* pkObject = m_pkObject; 
		    m_pkObject  = nullptr;
		    return pkObject;
	    }

	    /// <description>
	    /// comparisons
	    /// </description>	
	    INLINE bool operator== (T* pkObject) const 
	    {
		    return m_pkObject == pkObject;
	    }

	    /// <description>
	    /// inequality test
	    /// </description>	
	    INLINE bool operator!= (T* pkObject) const 
	    {
		    return m_pkObject != pkObject;
	    }

	    /// <description>
	    /// equality test which only tests its pointer
	    /// </description>	
	    INLINE bool operator== (const SmartPointer& rkReference) const
	    {
		    return m_pkObject == rkReference.m_pkObject;
	    }

	    /// <description>
	    /// inequality test 
	    /// </description>	
	    INLINE bool operator!= (const SmartPointer& rkReference) const 
	    {
		    return m_pkObject != rkReference.m_pkObject;
	    }

    protected:
	    /// <description>
	    /// managed object
	    /// </description>
	    T* m_pkObject;
    };

NAMESPACE_NAGA_END

#endif // __Naga_SmartSmartPointer_H__
