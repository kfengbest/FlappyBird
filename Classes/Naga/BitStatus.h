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
	Description	:	Bit Status 
	History		:	2012, Initial implementation.
*/
#ifndef ___Naga_BitStatus_h__
#define ___Naga_BitStatus_h__

NAMESPACE_NAGA_BEGIN

/// useful status definition
enum 
{
	/// not used 
	kNone 			= 0,
	/// not initialized 
	kUninitialized  = 0,
	/// initialized already
	kInitialized	= 0x1 << 0,
	/// realized  
	kRealized		= kInitialized,
	/// the object is idle
	kIdle			= kRealized << 1,
	/// the object is alive
	kAlive			= kIdle << 1,
	/// user defined status begins
	kUserDefined    = kAlive << 1
};

/// <description>
/// class BitStatus
/// use a unsigned integer type for internal status set/get
/// </description>
template <typename ValueType = unsigned int>
class BitStatus
{
	static_assert(STD is_unsigned<ValueType>::value == true, 
			"Must be unsigned integral type");

public:
	/// <description>
	/// Constructor
	/// </description>
	BitStatus(ValueType status = 0) : mStatus(status) {}
	/// <description>
	/// Copy Constructor
	/// </description>
	BitStatus(const BitStatus& rhs) : mStatus(rhs.mStatus) {}
	/// <description>
	/// Destructor
	/// </description>
	~BitStatus() { mStatus = 0; }

public:
	/// <description>
	/// Assignment Operator
	/// </description>
	INLINE BitStatus & operator = (const BitStatus& rhs) {
		mStatus = rhs.mStatus;
		return *this;
	}

	/// <description>
	/// Assignment Operator
	/// </description>
	INLINE BitStatus & operator = (ValueType status) {
		mStatus = status;
		return *this;
	}

	/// <description>
	/// Equality Operator
	/// </description>
	INLINE bool operator == (const BitStatus& rhs) const
	{
		return mStatus == rhs.mStatus;
	}

	/// <description>
	/// Equality Operator
	/// </description>
	INLINE bool operator == (unsigned int rhs) const
	{
		return mStatus == rhs;
	}

	/// <description>
	/// Bitwise & Operator
	/// </description>
	INLINE ValueType operator & (ValueType rhs) const
	{
		return mStatus & rhs;
	}

	/// <description>
	/// Bitwise & Operator
	/// </description>
	INLINE ValueType operator | (ValueType rhs) const
	{
		return mStatus | rhs;
	}

	/// <description>
	/// Bitwise & Operator
	/// </description>
	INLINE BitStatus& operator &= (ValueType rhs) 
	{
		mStatus &= rhs;
		return *this;
	}

	/// <description>
	/// Bitwise & Operator
	/// </description>
	INLINE BitStatus& operator |= (ValueType rhs)
	{
		mStatus |= rhs;
		return *this;
	}

	/// <description>
	/// Inequality Operator
	/// </description>
	INLINE bool operator != (const BitStatus& rhs) const
	{
		return mStatus != rhs.mStatus;
	}

	/// <description>
	/// Inequality Operator
	/// </description>
 	INLINE bool operator != (ValueType rhs) const
 	{
 		return mStatus != rhs;
 	}

	/// <description>
	/// operator 
	/// </description>
	INLINE operator ValueType() const { return mStatus; }

public:
	/// <description>
	/// Is Dirty 
	/// </description>
	INLINE bool IsDirty() const { return mStatus != 0; }

	/// <description>
	/// status 
	/// </description>
	INLINE ValueType Status() { return mStatus; }

	/// <description>
	///	set Properties for this texture
	/// </description>
	INLINE void Set(ValueType prop) 
	{
		mStatus |= prop;
	}

	/// <description>
	///	Set/Unset the property according to the input
	/// </description>
	INLINE void Set(ValueType prop, bool set) 
	{
		set ? Set(prop) : Unset(prop);
	}

	/// <description>
	///	Set the property to Zero
	/// </description>
	INLINE void Clear() 
	{
		mStatus = 0;
	}

	/// <description>
	///	Unset Properties for this texture
	/// </description>
	INLINE void Unset(ValueType prop)
	{
		mStatus &= ~prop;
	}

	/// <description>
	///	Is the property set 
	/// </description>
	INLINE bool IsSet(ValueType prop) const
	{
		return (mStatus & prop) == prop;
	}

	/// <description>
	///	Is any of the properties set 
	/// </description>
	INLINE bool IsAnySet(ValueType prop) const
	{
		return (mStatus & prop) != 0;
	}

protected:
	/// <description>
	///	the bitset status 
	/// </description>	
	ValueType mStatus;
};

/// useful 
typedef BitStatus<unsigned short>		UTxBitStatus16;
typedef BitStatus<unsigned int>			UTxBitStatus32;
typedef BitStatus<unsigned long long>	UTxBitStatus64;

NAMESPACE_NAGA_END

#endif //___Naga_BitStatus_h__
