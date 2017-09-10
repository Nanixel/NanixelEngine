#pragma once
#include <atomic>

//THIS CLASS IS NON COPYABLE -> MEANING WE MUST DELETE DEFAULT COPYING STUFF

/*
Memory Pool is basically a class that holds:
1. preallocated memory
2. a pointer to the top of the stack
3. handles alignment
*/

class StackMemoryPool
{
public:

	StackMemoryPool(size_t size, uint32_t alignmentBits = 16);

	//Move
	StackMemoryPool(StackMemoryPool&& other) {
		*this = std::move(other);
	}
	//Destroy
	~StackMemoryPool();
	//Move
	StackMemoryPool& operator=(StackMemoryPool&& other);
	//total size of our memory chunk
	size_t getSize() const {
		return memsize;
	}
	//allocated size
	size_t getAllocatedSize() const;

	//this allocates memory or returns nullptr on failure
	void* allocate(size_t size) throw();

	/*
	LAST MEANING THE LAST THING ADDED -> remember this is a stack
	Free the memory in stackmemorypool, if the ptr is not the last allocation then nothing happens and the method returns false
	return true if the deallocation succeeded and false otherwise
	*/
	bool free(void* ptr) throw();

	void reset();

private:
	//WHERE the memory chunk is
	//pre-allocated memory chunk
	uint8_t* memoryChunk = nullptr;

	size_t memsize = 0;
	
	//TOP of the memory chunk
	//this is atomic meaning that when it is read or writen to, the operation fully completes before it is read or writen too again
	std::atomic<uint8_t*> top = { nullptr };

	//alignment of our allocations
	uint32_t alignmentBits;
	//calculate the aligned size of an allocation
	size_t calculateAlignmentBits(size_t size) const;
};

