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
	Description	:	Memory Pool for small object/fixed type management
	History		:	2012, Initial implementation.
*/
#ifndef __Naga_MemoryPool_H__
#define __Naga_MemoryPool_H__

NAMESPACE_NAGA_BEGIN

/// <description>
/// Template class describing a simple pool of items.
/// Note that the Pool doesn't control the Item lifetime 
/// </description>
template <typename T>
class Pool
{
protected:
    typedef typename STD list<T> ItemList;
    typedef typename ItemList::value_type value_type;
    ItemList mItems;

public:
    /// <description>
    /// constructor
    /// </description>
    Pool() {} 
    /// <description>
    /// destructor
    /// </description>
    virtual ~Pool() {}

public:
    /// <description>
    /// current count of the items
    /// </description>
    virtual size_t Count() const 
    {
        return mItems.size();
    }

    /// <description>
    /// if the pool is empty
    /// </description>
    virtual bool Empty() const 
    {
        return mItems.size() == 0;
    }

    /// <description>
    /// Get the next item from the pool.
    /// return pair indicating whether there was a free item, and the item if so
    /// </description>
    virtual value_type RemoveItem()
    {               
        value_type ret = mItems.front();
        mItems.pop_front();        
        return ret;
    }

    /// <description>
    /// Add a new item to the pool. 
    /// </description>
    virtual void AddItem(const T& i)
    {        
        mItems.push_front(i);
    }

    /// <description>
    /// Clear the pool
    /// </description>
    virtual void Clear()
    {        
        mItems.clear();
    }
};

/// <description>
/// General Memory Pool allocates chunks of memory of a fixed size. 
/// The pool allocates blocks that contain several memory chunks and then
/// returns the chunks from each block.
/// </description>
class NAGAAPI MemoryPool 
{
public:
	/// <description>
	/// Constructor
	/// </description>
	MemoryPool(int size, int count)
	{
		Construct(size, count);
	}

	/// <description>
	/// Frees all of the allocated blocks.
	/// </description>
	~MemoryPool();

public:
	/// <description>
	/// Allocate an item from the pool
	/// </description>
	void* Alloc();

	/// <description>
	/// Free an item back to the pool
	/// </description>
	void Free(void* item);

private:
	/// <description>
	/// the object blocks are chained together 
	/// </description>	
	struct Block
	{
		Block* mNextBlock;
	};

	/// <description>
	/// Keep items returned to the pool as a list of FreeItems
	/// </description>		
	struct FreeItem
	{
		FreeItem* mNextFree;
	};

	int mObjectSize;        // Size of objects in the pool
	int mObjectCount;       // Count of object in a single block

	Block* mBlocks;         // List of blocks, only used to free in the destructor
	int mBlockLeader;       // Size of the block header - used to align items in the block

	unsigned char* mFreeBlock; // Address of object in last block allocated
	int mFreePos;           // Current position in last block allocated
	FreeItem* mFreeList;    // Free list of item freed with Free

protected:
	///<description>
	/// initialize the memory pool
	///</description>
	void Construct(int size, int count);
};

/// <description>
/// Template class for a memory pool that allocated memory for
/// a specific type and block count. 
/// </description>
template<class T, int count> 
	class UTxMemoryPool : private MemoryPool
	{
	public:
		/// <description>
		/// Allocate a block of memory that is used to construct T
		/// </description>
		static inline void* Allocate() {
			return mPool.Alloc();
		}

		/// <description>
		/// Allocate a block of memory after T has been destroyed
		/// </description>
		static inline void Delete(void* item) {
			mPool.Free(item);
		}

	private:
		static UTxMemoryPool mPool;

		/// <description>
		/// initialize the memory pool 
		/// </description>
		UTxMemoryPool()
			: MemoryPool(sizeof(T), count)
		{
		}
	};

template<class T, int count> 
	UTxMemoryPool<T, count> UTxMemoryPool<T,count>::mPool;

/// <description>
/// This macro declares the new and delete operator for a class that uses the memory pool
/// </description>
#define DECLARE_MEMORY_POOL() \
	public: \
		void* operator new(size_t size); \
		void operator delete(void* item, size_t size);

#define DEFINE_MEMORY_POOL(cls, count) \
	void* cls::operator new(size_t size) \
	{ \
		if (size != sizeof(cls)) \
			return ::operator new(size); \
		return UTxMemoryPool<cls, count>::Allocate(); \
	} \
	void cls::operator delete(void* item, size_t size) \
	{ \
		if (size != sizeof(cls)) \
			return ::operator delete(item); \
		else \
			UTxMemoryPool<cls, count>::Delete(item); \
	}

/// <description>
///  This macro is used for aligned memory allocations
///  structured embedding Bullet structs/classes need to be 16-byte aligned)
///  
///  If you are not embedded Bullet data in your classes, or make sure that you align those classes on 16-byte boundaries
/// </description>
#define DECLARE_ALIGNED_ALLOC() \
    public:  \
        INLINE void* operator new(size_t size) \
        { \
            return _aligned_malloc(size,16); \
        } \
        INLINE void operator delete(void* item, size_t size) \
        { \
             _aligned_free(item); \
        }

NAMESPACE_NAGA_END

#endif // __Naga_MemoryPool_H__
