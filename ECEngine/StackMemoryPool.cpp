#include "stdafx.h"
#include "StackMemoryPool.h"
#include <stdio.h>
#include <assert.h>
#include <ostream>

//IF THE MEMORY POOL EXISTS ONLY IN ALLOCATORS, THE BLOCK HEADER IS NOT NEEDED
struct MemoryBlockHeader {
	uint32_t size;
};

StackMemoryPool::StackMemoryPool(size_t size_, uint32_t alignmentBits_) : memsize(size_), alignmentBits(alignmentBits_)
{
	assert(memsize > 0);
	memoryChunk = (uint8_t*)::operator new(memsize);

	if (memoryChunk != nullptr) {
		top = memoryChunk;
	}
	else {
		std::cout << "malloc() faild" << std::endl;
		//throw empty for now but you would normally want more debugging info
		throw;
	}
}

StackMemoryPool::~StackMemoryPool()
{
	if (memoryChunk != nullptr) {
		::operator delete(memoryChunk);
	}
}


StackMemoryPool& StackMemoryPool::operator=(StackMemoryPool&& other) {
	//this a a move, not an assignment
	if (memoryChunk != nullptr) {
		::operator delete(memoryChunk);
	}

	memoryChunk = other.memoryChunk;
	memsize = other.memsize;
	//load is here because top is atomic
	top.store(other.top.load());
	alignmentBits = other.alignmentBits;

	other.memoryChunk = nullptr;
	other.memsize = 0;
	other.top = nullptr;

	return *this;
}

size_t StackMemoryPool::getAllocatedSize() const {
	return top.load() - memoryChunk;
}

void* StackMemoryPool::allocate(size_t size_) throw() {

	assert(memoryChunk != nullptr);

	size_t size = calculateAlignmentBits(size_ + sizeof(MemoryBlockHeader));

	assert(size < std::numeric_limits<uint32_t>::max());

	//atomically replaces the current value with the result of the addition of the value and arg
	//get the top and add size
	uint8_t* out = top.fetch_add(size);

	if (out + size <= memoryChunk + memsize) {
		//Write the block header
		((MemoryBlockHeader*)out)->size = size;

		//Set the correct output
		out += sizeof(MemoryBlockHeader);
	}
	else {
		//error
		out = nullptr;
	}
	return out;
}

bool StackMemoryPool::free(void* ptr) throw() {

	assert(memoryChunk != nullptr);

	//Correct the P
	uint8_t* realPtr = (uint8_t*)ptr - sizeof(MemoryBlockHeader);

	//realptr should be inside the pool's preallocated memory
	assert(realPtr >= memoryChunk && realPtr < memoryChunk + memsize);

	//Get block size
	uint32_t size = ((MemoryBlockHeader*)realPtr)->size;

	//Atomic stuff
	uint8_t* expected = realPtr + size;
	uint8_t* desired = realPtr;

	/* This is the code for the whats going on below
	if (top == expected) {
		top = desired;
		exchange = true
	} else {
		expected = top;
		exchange = false
	}
	
	*/

	bool exchange = top.compare_exchange_strong(expected, desired);

	return exchange;
}


void StackMemoryPool::reset() {
	assert(memoryChunk != nullptr);
	top = memoryChunk;
}

size_t StackMemoryPool::calculateAlignmentBits(size_t size) const {
	return size + (size % (alignmentBits / 8));
}