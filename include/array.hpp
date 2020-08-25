#pragma once

#include <ostream>
#include <initializer_list>
#include <cassert>

#include "types.hpp"

// Static Array
// Data allocated on the stack
template <typename T, const size_t _size>
struct sarray {
public:
    inline constexpr const size_t size() const { return _size; }
    inline const T* data() const { return buffer; }
    inline T* data() { return buffer; }

    // C-tors
    sarray() {}

    // This C-tor allows us to use an initializer list as an input
    // Example:
    //     sarray<int, 4> arr = { 1, 2, 3, 4 };
    //
    // @TODO: Create my own intitializer_list
    sarray(const std::initializer_list<T>& values) {
	assert(values.size() == _size);      // @TEMP: Number of elements in initlializer list must be equal to _size of sarray

	// @DECIDE: Maybe it just sets the values according to whichever is shorter
	//          instead of using an assert?
	
	int i = 0;
	for (auto val : values)
	    buffer[i++] = std::move(val);
    }
    
    // No Copy C-tor
    // @DECIDE: Can you implement a copy c-tor?
    sarray(const sarray&) = delete;

    // No Move C-tor
    // @DECIDE: Can you implement a move c-tor?
    sarray(sarray&& other) = delete;

    // Operators

    // @DECIDE: Should you have an assignment operator?

    // [] operator to get element at given index
    const T& operator[](size_t index) const {
	assert(index >= 0 && index < _size);
	return buffer[index];
    }

    T& operator[](size_t index) {
	assert(index >= 0 && index < _size);
	return buffer[index];
    }
    
    // @TODO: Implement iterators

private:
    // Give data about a Static Array to an std::ostream
    // Can be used with std::cout
    friend std::ostream& operator<<(std::ostream& stream, const sarray& sa) {
	stream << "sarray <" << type_name<T>() << "> { size: " << _size << " }";
	return stream;
    }
    
private:
    T buffer[_size];
};


// Dynamic Array
// Data allocated on the heap
template <typename T>
struct darray {
private:
    #define growth_rate 2.0;
    
public:
    inline size_t size() const { return _size; }
    inline size_t capacity() const { return _capacity; }

    // Push element to the end of the array
    T& push_back(const T& value) {
	// If the vector has filled up, resize it
	if (_size >= _capacity) {
	    size_t cap = 2;
	    if (_capacity > 1) cap = _capacity * growth_rate;
	    reallocate(cap);
	}
	
	buffer[_size] = value;
	return buffer[_size++];
    }

    T& push_back(T&& value) {
	if (_size >= _capacity) {
	    size_t cap = 2;
	    if (_capacity != 0) cap = _capacity * growth_rate;
	    reallocate(cap);
	}
	
	buffer[_size] = std::move(value);
	return buffer[_size++];
    }

    // Emplace elements
    // Constructs element at already allocated space
    // prevents redundant construction
    template<typename... Args>
    T& emplace_back(Args&&... args) {
	if (_size >= _capacity) {
	    size_t cap = 2;
	    if (_capacity > 1) cap = _capacity * growth_rate;
	    reallocate(cap);
	}

	// Create object in already allocated space
	new(&buffer[_size]) T(std::forward<Args>(args)...);
	return buffer[_size++];
    }

    // Remove element from the back of the array
    void pop_back() {
	if (_size > 0) {
	    _size--;
	    buffer[_size].~T();    // Call D-tor of element explicitly
	}
    }

    // Remove all elements in the array
    // Calls D-tors of all elements in the array
    void clear() {
	for (int i = 0; i < _size; i++)
	    buffer[i].~T();

	_size = 0;
    }

    // Trim function that deallocates any unused memory
    void trim() {
	if (_size < _capacity) {
	    T* ptr = buffer + _size;
	    deallocate_from(ptr);
	}
    }
    
    // C-tor
    darray()
	: _size(0) {
	reallocate(2);
    }

    // This C-tor allows us to use an initializer list as an input
    // Example:
    //     darray<int> arr = { 1, 2, 3, 4 };
    //
    // @TODO: Create my own intitializer_list
    darray(const std::initializer_list<T>& values)
	: _size(0), _capacity(values.size()) {
	reallocate(_capacity);

        for (auto&& val : values)
	    buffer[_size++] = std::move(val);
    }
    
    // Copy C-tor
    darray(const darray& other)
	: _size(0), _capacity(other._capacity) {
	reallocate(_capacity);

	for (; _size < other._size; _size++)
	    buffer[_size] = std::move(other.buffer[_size]);
    }

    // Move C-tor
    darray(darray&& other)
	: _size(other._size), _capacity(other._capacity), buffer(other.buffer) {
	other._size  = other._capacity = 0;
	other.buffer = nullptr;
    }
    
    // D-tor
    ~darray() {
	clear();
	deallocate_buffer();
    }

    // Operators

    // [] operator to get element at given index
    const T& operator[](size_t index) const {
	assert(index >= 0 && index < _size);
	return buffer[index];
    }

    T& operator[](size_t index) {
	assert(index >= 0 && index < _size);
	return buffer[index];
    }

    // @TODO: Implement Iterators
    
private:
    // Allocates a new buffer and moves old data into new buffer
    // and deallocates old buffer
    void reallocate(size_t new_cap) {
	T* new_buffer = allocate_without_initializing(new_cap);

	// If array needs to be shrinked
	size_t size = _size;
	if (new_cap < _size) size = new_cap;

	// Move elements if possible to avoid performance cost of copying
	for (size_t i = 0; i < size; i++)
	    new_buffer[i] = std::move(buffer[i]);

	// Call d-tor of all elements
	//
	// This is done explicitly to prevent conflicts when an
	// object has already been destroyed by pop_back()
	// or the clear() functions
	for (size_t i = 0; i < _size; i++)
	    buffer[i].~T();

	// Deallocate old buffer
        deallocate_buffer();

	buffer = new_buffer;
	_capacity = new_cap;
	_size = size;
    }

    // Allocate memory block without initializing
    T* allocate_without_initializing(size_t cap) {
	return (T*)::operator new(cap * sizeof(T));
    }

    // Deallocate memory block without calling the D-tor
    void deallocate_buffer() {
	::operator delete(buffer, _capacity * sizeof(T));
    }

    // Deallocate memory from given address to the end, without calling the D-tor
    void deallocate_from(T* start) {
	assert(start >= buffer);     // Don't deallocate memory outside the buffer
	
	size_t remaining = _capacity - (start - buffer);
	::operator delete(start, remaining * sizeof(T));
	_capacity -= remaining;
    }

    // Give data about a Dynamic Array to an std::ostream
    // Can be used with std::cout
    friend std::ostream& operator<<(std::ostream& stream, const darray& da) {
	stream << "darray <" << type_name<T>() << "> { size: " << da._size
	       << ", capacity: " << da._capacity << " }";
	return stream;
    }
    
private:
    size_t _size;
    size_t _capacity;
    T* buffer = nullptr;
};


// Print a Static Array without newline
template <typename T, const size_t _size>
inline void print_array(const sarray<T, _size>& arr) {
    for (size_t i = 0; i < _size; i++)
	std::cout << arr[i] << " ";
}

// Print a Dynamic Array without newline
template <typename T>
inline void print_array(const darray<T>& arr) {
    for (size_t i = 0; i < arr.size(); i++)
	std::cout << arr[i] << " ";
}
