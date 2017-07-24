#pragma once
#include "StackMemoryPool.h"
#include <assert.h>
#include <string>
#include <vector>
//Stack based allocator
#include <iostream>

template<typename T, bool deallocationFlag = false, uint32_t alignmentBits = 16>
class StackAllocator
{
	template<typename U, bool deallocationFlag_, uint32_t alignmentBits_>
	friend class StackAllocator;

public:
	//typedefs
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	StackAllocator() throw() {}

	StackAllocator(const StackAllocator& other) throw() {
		*this = other;
	}

	template<typename U>
	StackAllocator(const StackAllocator<U, deallocationFlag, alignmentBits>& other) throw() {
		*this = other;
	}

	StackAllocator(size_type size) throw() {
		memoryPool.reset(new StackMemoryPool(size, alignmentBits));
	}

	~StackAllocator() {}

	StackAllocator& operator=(const StackAllocator& other) {
		memoryPool = other.memoryPool;
		return *this;
	}

	template<typename U>
	StackAllocator& operator=(const StackAllocator<U, deallocationFlag, alignmentBits>& other) {
		memoryPool = other.memoryPool;
		return *this;
	}

	//get the address of a reference
	pointer address(reference x) const {
		return &x;
	}

	const_pointer address(const_reference x) const {
		return &x;
	}

	pointer allocate(size_type n, const void* hint = 0) {
		assert(memoryPool != nullptr);

		//this is just used to surpress compiler warnings
		//void(hint);

		//calculate the size that we want to allocate
		size_type size = n * sizeof(value_type);

		void* out = memoryPool->allocate(size);

		if (out != nullptr) {
			//Everything is ok
		}
		else {
			throw;
		}
		return (pointer)out;
	}

	void deallocate(void* p, size_type n) {
		assert(memoryPool != nullptr);

		(void)p;
		(void)n;

		if (deallocationFlag) {
			bool ok = memoryPool->free(p);
			if (!ok) {
				throw;
			}
		}
	}

	//call constructor of type
	void construct(pointer p, const T& value) {
		new ((T*)p) T(value);
	}

	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		::new((void*)p) U(std::forward<Args>(args)...);
	}

	void destroy(pointer p) {
		p->~T();
	}

	template<typename U>
	void destroy(U* p) {
		p->~U();
	}

	size_type max_size() const {
		assert(memoryPool != nullptr);
		return memoryPool->getSize();
	}

	template<typename U>
	struct rebind {
		typedef StackAllocator<U, deallocationFlag, alignmentBits> other;
	};

	void reset() {
		assert(memoryPool != nullptr);
		memoryPool->reset();
	}

	const StackMemoryPool& getMemoryPool() const {
		assert(memoryPool != nullptr);
		return *memoryPool;
	}


private:
	std::shared_ptr<StackMemoryPool> memoryPool;
};

//another allocator of the same type can deallocate from this one
template<typename T1, typename T2, bool deallocationFlag, uint32_t alignmentBits>
inline bool operator==(const StackAllocator<T1, deallocationFlag, alignmentBits>&,
	const StackAllocator<T2, deallocationFlag, alignmentBits>&) {
	return true;
}

//another allocator of another type cannot deallocate from this one!!
template<typename T1, typename AnotherAllocator, bool deallocationFlag, uint32_t alignmentBits>
inline bool operator==(const StackAllocator<T1, deallocationFlag, alignmentBits>&, const AnotherAllocator&) {
	return false;
}

template<typename T1, typename T2, bool deallocationFlag, uint32_t alignmentBits>
inline bool operator!=(const StackAllocator<T1, deallocationFlag, alignmentBits>&,
	const StackAllocator<T2, deallocationFlag, alignmentBits>&) {
	return false;
}

template<typename T1, typename AnotherAllocator, bool deallocationFlag, uint32_t alignmentBits>
inline bool operator!=(const StackAllocator<T1, deallocationFlag, alignmentBits>&, const AnotherAllocator&) {
	return true;
}


void testStackAllocator() {
	//if the allocator is used in the proper LIFO way, we can set the deallocation flag to one
	//if it is set to zero the allocator ignores deallocations
	//since memoryPool is a sharedptr it makes things thread safe

	//Test
	//StackAllocator<char, false> alloc(128); 
	//typedef std::basic_string<char, std::char_traits<char>, StackAllocator<char, false>> Str;

	//Str str(alloc);

	//str = "lalala";
	//str = "lalalalo";

	StackAllocator<int> intAlloc(128);

	std::vector<int, StackAllocator<int>> vec(intAlloc);

	for (int i{ 0 }; i < 10; i++) {
		vec.push_back(i);
	}

	const StackMemoryPool& myPool = intAlloc.getMemoryPool();
	std::cout << "Size: " << myPool.getAllocatedSize() << std::endl;

}