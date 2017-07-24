#pragma once

#include <cstdlib>


size_t gAllocatedSize;

//create a template class
template<typename T>
class CustomAllocator {
public:
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T value_type;

	CustomAllocator() throw() {
		// construct default allocator (do nothing)
	};
	CustomAllocator(const CustomAllocator& other) throw() {
		// construct by copying (do nothing)
	}

	//copy constructor with another type
	template<typename U>
	CustomAllocator(const CustomAllocator<U>& other) throw() {
		// construct from a related allocator (do nothing)
	}

	//assignment
	CustomAllocator<T>& operator=(const CustomAllocator& other) {
		return *this;
	}

	template<typename U>
	CustomAllocator<U>& operator=(const CustomAllocator& other) {
		// assign from a related allocator (do nothing)
		return *this;
	}

	//pointer to whatever type we are right now
	//get the address of a reference
	pointer address(reference x) const {
		return &x;
	}

	//get the const address of a const reference
	const_pointer address(const_reference x) const {
		return &x;
	}

	//size_type is the memory size, const void* is due to the fact that memory does not have a type
	//allocate n elements of type T 
	pointer allocate(size_type n, const void* hint = 0) {		
		//this allocates based on the size of the stuff you have multiplied by the datatype
		size_type size = n * sizeof(value_type);
		gAllocatedSize += size;		
		return (pointer)::operator new(size);
	}

	//free memory of pointer p
	void deallocate(void* p, size_type n) {
		size_type size = n * sizeof(T);
		gAllocatedSize -= size;
		::operator delete(p);
	}

	//call the constructor of p
	void construct(pointer p, const T& val) {
		//cast T as a pointer casted as T*
		new ((T*)p) T(val);
	}

	//Call the constructor of p with many arguements
	template<typename U, typename... Args>
	void construct(U* p, Args&&... args) {
		::new((void*)p) U(std::forward<Args>(args)...);
	}

	//call the destructor of p
	void destroy(pointer p) {
		// destroy object at _Ptr
		p->~T();
	}

	template<typename U>
	void destroy(U* p) {
		p->~U();
	}



	//get the max allocation size
	size_type maxSize() const {
		return size_type(-1);
	}

	~CustomAllocator() {}
	//struct to rebind an allocator to another allocator of type U
	template<typename U>
	struct rebind {
		typedef CustomAllocator<U> other;
	};
};


void scopeTest() {
	//the Allocator type in this template is used to aquire/release (new/delete) memory to construct/destroy the elements in memory
	std::vector<int, CustomAllocator<int>> vec;
	int something = 50;
	vec.resize(10, something);
	std::cout << "Allocated Size: " << gAllocatedSize << std::endl;
}

void testAddingElements() {
	std::vector<int, CustomAllocator<int>> vec;
	int first = 1;
	int second = 2;
	int third = 3;
	int fourth = 4;

	//These do infect allocate a new 4 bits of memory each time
	//This is a good way to see the fragmented memory

	vec.push_back(first);
	std::cout << "Allocated Size First: " << gAllocatedSize << std::endl;
	std::cout << "First Address: " << &vec[0] << std::endl;
	vec.push_back(second);
	std::cout << "Allocated Size Second: " << gAllocatedSize << std::endl;
	std::cout << "Second Address: " << &vec[1] << std::endl;
	vec.push_back(third);
	std::cout << "Allocated Size Third: " << gAllocatedSize << std::endl;
	std::cout << "Third Address: " << &vec[2] << std::endl;
	vec.push_back(fourth);
	std::cout << "Allocated Size Fourth: " << gAllocatedSize << std::endl;
	std::cout << "Fourth Adress:" << &vec[3] << std::endl;
}