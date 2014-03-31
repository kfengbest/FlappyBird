#include "NagaLib.h"

NAMESPACE_NAGA_BEGIN

/// <description>
/// Initialize the pool
/// </description>
void MemoryPool::Construct(int size, int count)
{
	mObjectCount = count;
	mObjectSize = size >= sizeof(FreeItem) ? size : sizeof(FreeItem);
	mBlocks = nullptr;
	mFreeBlock = nullptr;
	mFreePos = 0;
	mFreeList = nullptr;

	int lsb = mObjectSize & (~mObjectSize + 1); 
	if (lsb < 4)
		lsb = 4;
	else if (lsb > 16)
		lsb = 16;
	// mBlockLeader is the size we need to get past the block header and keep alignment
	mBlockLeader = ((sizeof(Block) + lsb - 1) / lsb) * lsb;
}

/// <description>
/// Destructor
/// </description>
MemoryPool::~MemoryPool()
{
	mFreeList = nullptr;
	mFreeBlock = nullptr;
	mFreePos = 0;		

	for (Block* block = mBlocks; block != nullptr;)
	{
		void* mem = block;
		block = block->mNextBlock;
        
        /// _aligned_malloc and _aligned_free are Microsoft Specified. Use posix_memalign or memalign on Linux et al. For Mac OS X
        /// you can just use malloc, as it is always 16 byte aligned. For portable SSE code you generally want to implement
        /// wrapper functions
#if PLATFORM == PLATFORM_WIN32
		_aligned_free(mem);
#elif PLATFORM == PLATFORM_IOS
        free(mem);
#endif
	}

	mBlocks = nullptr;
}

/// <description>
/// Alloc an item from the pool
/// </description>
void* MemoryPool::Alloc()
{
	if (mFreeList != nullptr)
	{
		// Get the first free item on the free list, use it
		FreeItem* firstFreeItem = mFreeList;
		mFreeList = mFreeList->mNextFree;
		return firstFreeItem;
	}

	if (mFreePos  == 0)
	{
		// allocate one block
#if PLATFORM == PLATFORM_WIN32
		Block* block = static_cast<Block*>(_aligned_malloc(mBlockLeader + mObjectSize * mObjectCount,16));
#elif PLATFORM == PLATFORM_IOS
        Block* block = static_cast<Block*>(malloc(mBlockLeader + mObjectSize * mObjectCount));
#endif
		if (block == nullptr)
			return nullptr;

		block->mNextBlock = mBlocks;
		mBlocks = block;
		mFreeBlock = reinterpret_cast<unsigned char*>(block) + mBlockLeader;
		mFreePos = mObjectCount * mObjectSize;		
	}
	
	// Got one in the last free block, use it
	mFreePos -= mObjectSize;
	return mFreeBlock + mFreePos;
}

/// <description>
/// Free an item back to the pool
/// </description>
void MemoryPool::Free(void* item)
{
	if (item != nullptr)
	{
		FreeItem* freeItem = static_cast<FreeItem*>(item);
		freeItem->mNextFree = mFreeList;
		mFreeList = freeItem;
	}
}

NAMESPACE_NAGA_END
