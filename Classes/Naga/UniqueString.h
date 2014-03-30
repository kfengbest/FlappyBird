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
#ifndef ___Naga_UniqueString_h__
#define ___Naga_UniqueString_h__

NAMESPACE_NAGA_BEGIN

/// <description>
/// unique string is a kind of immutable string 
/// which has exactly one instance for each string during run-time
/// </description>
class NAGAAPI UTxUniqueString 
    : public NAGA Object
{
public:
	/// <description>
	/// constructor
	/// </description>
	UTxUniqueString(const char* s);
	UTxUniqueString(const char* s, int len, unsigned int hashcode = 0);
	~UTxUniqueString(void);

public:
	/// <description>
	///	function length and size have the same meaning
	/// </description>
	INLINE unsigned int Length() const {return mLength;	}
	INLINE unsigned int Size() const {	return Length();}
	INLINE bool Empty() const { return mLength == 0;	}

	/// <description>
	///	convert to raw string
	/// </description>
	INLINE operator const char*() const { return mpBuffer; }
	INLINE const char* Buffer() const   { return mpBuffer; }

	/// <description>
	/// Access by copy.
	/// </description>
	INLINE char operator[](int index) const 
	{ 
		return mpBuffer[ index ];
	}

	/// <description>
	/// Gets the hash code for this string.
	/// </description>
	unsigned int HashCode() const;

public:
	/// <description>
	/// String Comparison, case-sensitive.
	/// </description>
	INLINE int Compare( const char* pStr ) const
	{
		return strcmp( mpBuffer==nullptr ? "":mpBuffer, pStr==nullptr ? "":pStr );
	}

	/// <description>
	/// String Comparison, case-sensitive.
	/// </description>
	INLINE int Compare( const UTxString& str ) const
	{
		return Compare(str.c_str());
	}

	/// <description>
	/// String Comparison, case non-sensitive.
	/// </description>
	INLINE int CompareNoCase( const char* pStr ) const
	{
		UTxString str1Cpy(mpBuffer==nullptr ? "" : mpBuffer), str2Cpy( pStr==nullptr ? "" : pStr);
		std::transform( str1Cpy.begin(), str1Cpy.end(), str1Cpy.begin(), ::tolower );
		std::transform( str2Cpy.begin(), str2Cpy.end(), str2Cpy.begin(), ::tolower );
		return strcmp( str1Cpy.c_str(), str2Cpy.c_str());
	}

	/// <description>
	/// String Comparison, case non-sensitive.
	/// </description>
	INLINE int CompareNoCase( const UTxString& str ) const
	{
		return CompareNoCase(str.c_str());
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator == ( UTxUniqueString& str )const
	{
		return ((void*)mpBuffer) == ((void*)(str.mpBuffer));
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator != ( UTxUniqueString& str )const
	{
		return ((void*)mpBuffer) != ((void*)(str.mpBuffer));
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator <  ( UTxUniqueString& str )const
	{
		return Compare( str.mpBuffer ) < 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator <= ( UTxUniqueString &str )const
	{
		if( ((void*)mpBuffer) == ((void*)(str.mpBuffer)) )
			return true;

		return Compare( str.mpBuffer ) <= 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator >= ( UTxUniqueString &str )const
	{
		if( ((void*)mpBuffer) == ((void*)(str.mpBuffer)) )
			return true;

		return Compare( str.mpBuffer ) >= 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator > ( UTxUniqueString &str ) const
	{
		return Compare( str.mpBuffer ) > 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator == ( const UTxString& str )const
	{
		return Compare( str ) == 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator != ( const UTxString& str )const
	{
		return Compare( str ) != 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator <  ( const UTxString& str )const
	{
		return Compare( str ) < 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator <= ( const UTxString &str )const
	{
		return Compare( str ) <= 0;
	}

	/// <description>
	/// String Comparison
	/// </description>
	INLINE bool operator >= ( const UTxString&str )const
	{
		return Compare( str ) >= 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator >  ( const UTxString &str )const
	{
		return Compare( str ) > 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator == ( const char *pStr )const
	{
		return Compare( pStr ) == 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator != ( const char *pStr )const
	{
		return Compare( pStr ) != 0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator <  ( const char *pStr )const
	{
		return Compare( pStr )  <   0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator <= ( const char *pStr )const
	{
		return Compare( pStr )  <=  0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator >= ( const char *pStr )const
	{
		return Compare( pStr )  >=  0;
	}

	/// <description>
	/// String comparison.
	/// </description>
	INLINE bool operator >  ( const char *pStr )const
	{
		return Compare( pStr )>0;
	}

protected:
	/// <description>
	/// forbidden operators
	/// unique string is immutable the same as constant string 
	/// <description>
	UTxUniqueString& operator =  (const UTxUniqueString& s);
	UTxUniqueString& operator =  (const char* s);
	UTxUniqueString& operator += (const UTxUniqueString& s);
	UTxUniqueString& operator += (const char*  s);

protected:
	/// <description>
	/// memory buffer for the string, ended with 0
	/// </description>
	const char*	mpBuffer;

	/// <description>
	/// length of the string 
	/// </description>
	unsigned int	mLength;

	/// <description>
	/// cached hash code of this string
	/// </description>
	mutable unsigned int mHashCode;
};

/// <description>
/// Generate a Global Unique string
/// </description>
NAGAAPI UTxUniqueString* Intern(const char* str);
INLINE  UTxUniqueString* Intern(const UTxString& str) {
	return Intern(str.c_str());
}

NAMESPACE_NAGA_END

#endif // !defined ___Naga_UniqueString_h__
