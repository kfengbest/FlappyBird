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
Description	:	Unqiue Strings
History		:	2012, Initial implementation.
*/
#include "NagaLib.h"

NAMESPACE_NAGA_BEGIN

/// <description>
/// static function to compute the hash code & length of a string
/// </description>
static unsigned int hash_value( const char* theString, int* pOutLength )
{
	if( theString == nullptr )
	{
		if( pOutLength != nullptr ) *pOutLength = 0;
		return 0;
	}

	int count = 0;
	int hash  = 0;
	while( theString[count] != L'\0' )
		hash = hash * 31 + theString[ count++ ];

	if( pOutLength != nullptr ) 
		*pOutLength = count;

	return hash;
}

/// <description>
/// constructor
/// </description>
UTxUniqueString::UTxUniqueString(const char* str)
	: mpBuffer(str),mHashCode(0)
{
	mLength = strlen(str);	
}

/// <description>
/// constructor
/// </description>
UTxUniqueString::UTxUniqueString(
	const char* str, int len, unsigned int hashcode/* = 0*/)
	: mpBuffer(str), mLength(len), mHashCode(hashcode)
{
}

/// <description>
/// destructor
/// </description>
UTxUniqueString::~UTxUniqueString()
{
	mpBuffer  = nullptr;
	mHashCode = 0;
	mLength   = 0;
}

/// <description>
/// hash code for this string
/// </description>
unsigned int UTxUniqueString::HashCode() const
{
	if( mHashCode == 0 ) {
		mHashCode = hash_value( mpBuffer, nullptr);
	}

	return mHashCode;
}

template <typename _Ty>
    struct array_delete 
    {
        INLINE void operator()(_Ty* p) 
        {
            delete []p;
        }
    };

/// <description>
/// Unique String Manager
/// </description>
class UTxUniqueStringManager 
	: public Singleton<UTxUniqueStringManager>
{
protected:
	typedef unsigned int key_type;
	typedef SmartPointer<UTxUniqueString> value_type;    
	typedef std::vector<value_type>	StringList;
	typedef std::unordered_map<key_type,StringList> UniqueStringTable;
	enum {
		DEFAULT_STRING_BUFFER_COUNT = 1024
	};

    typedef std::shared_ptr<char>   BufferPtr; 
    typedef std::vector<BufferPtr>  BufferList;
    BufferList mBuffers;

    /// <description> 
    /// allocate new string buffer for the unique strings
    /// </description>
    INLINE void Alloc()  
    {
        BufferPtr buff(new char[DEFAULT_STRING_BUFFER_COUNT], array_delete<char>());
        mBuffers.push_back( buff);
        mStringBuf = buff.get();
        mBufLength = DEFAULT_STRING_BUFFER_COUNT;
        mBufNext = 0;
    }

protected:
	UniqueStringTable mStringTable;
	char*			mStringBuf;   
	unsigned int	mBufLength;   
	int				mBufNext;
	int				mCount;

public:
	/// <description>
	/// constructor
	/// </description>
	UTxUniqueStringManager()
        : mBufLength(DEFAULT_STRING_BUFFER_COUNT)
        , mBufNext(0)
        , mStringBuf(nullptr)
        , mCount(0)
    {
        Alloc();
    }

    /// <description>
	/// destructor
	/// </description>
	~UTxUniqueStringManager()
    {
        mStringBuf = nullptr;
        mBufLength = 0;
        mBufNext = -1;
    }

public:
	/// <description>
	/// Gets a pointer to the UTxUniqueString associated with the string
	/// </description>
	UTxUniqueString* Intern( const char* str);

	/// <description>
	/// Gets a pointer to the UTxUniqueString associated with the desired KString.
	/// This method is thread-safe.
	/// </description>
	INLINE UTxUniqueString* Intern( const UTxString& str) 
	{
		return Intern( str.c_str() );
	}

	/// <description>
	/// Gets a pointer to the UTxUniqueString associated with the key, Returns nullptr if not found.
	/// </description>
	UTxUniqueString* Find( const char* theString ) const;

	/// <description>
	/// Gets a pointer to the element associated with the key
	/// </description>
	INLINE UTxUniqueString* operator[]( const char* key ) const
	{
		return Find( key );
	}

	/// <description>
	/// Returns the count of unique strings
	/// </description>
	INLINE int Count() const {
        return mStringTable.size();
    }

private:
	/// <description>
	/// Adds an element to the hash map
	/// </description>
	UTxUniqueString* AddString(const char* pStr,int len, unsigned int hashCode);
}; 

/// <description>
/// get a pointer to the UTxUniqueString associated with the key, or nullptr if not found
/// </description>
UTxUniqueString* UTxUniqueStringManager::Find( const char* pString ) const
{
	int len = 0;
	unsigned int hashCode = hash_value( pString, &len );
	
	auto itr = mStringTable.find(hashCode);
	if (itr != mStringTable.end())
	{
		const StringList& strings = itr->second;
		for (auto i = strings.begin(); i!=strings.end();++i)
		{
			const value_type& rhs = *i;
 			if ((rhs->HashCode() == hashCode) && (rhs->Length() == len) && !strcmp(rhs->Buffer(), pString))
 				return rhs;
		}	
	}

	return nullptr; 
} 

/// <description>
/// get a pointer to the UTxUniqueString associated with the string, or add it
/// </description>
UTxUniqueString* UTxUniqueStringManager::Intern( const char* pString )
{
	if(!pString) 
		return nullptr;

	int len = 0;
	unsigned int hashCode = hash_value( pString, &len );

	auto itr = mStringTable.find(hashCode);
	if (itr != mStringTable.end())
	{
		const StringList& strings = itr->second;
		for (auto i = strings.begin(); i!=strings.end();++i)
		{
			const value_type& rhs = *i;
			if ((rhs->HashCode() == hashCode) && (rhs->Length() == len) && !strcmp(rhs->Buffer(), pString))
				return rhs;
		}	
	}

	// add a new entry
	return AddString( pString,len,hashCode);
} 

/// <description>
/// add an element to the data, grow the array if necessary
/// </description>
UTxUniqueString* UTxUniqueStringManager::AddString(const char* pStr,int len, unsigned int hashCode)
{	
	if( (unsigned int)(mBufNext + len + 1) >= mBufLength )
	{		
        Alloc();
		NASSERT(mBufLength == DEFAULT_STRING_BUFFER_COUNT);
        NASSERT(mBufNext == 0);
        NASSERT(mStringBuf != nullptr);
	}

	// copy into the string buffer, include the trailing '\0'
	char* pNewStr = &mStringBuf[ mBufNext ];
	for( int i = 0; i < len + 1; ++i )
		mStringBuf[ mBufNext++ ] = pStr[i];

	// add the new string to the hash
	SmartPointer<UTxUniqueString> newStr = new UTxUniqueString( pNewStr, len, hashCode );
	StringList& list = mStringTable[ hashCode ];
	list.push_back( newStr );

	++mCount;
	return newStr;
}

/// <description>
/// Generate a Global Unique string
/// </description>
UTxUniqueString* Intern(const char* str)
{
	UTxUniqueStringManager* pInst = UTxUniqueStringManager::InstancePtr();
	return pInst->Intern(str);
}

NAMESPACE_NAGA_END
