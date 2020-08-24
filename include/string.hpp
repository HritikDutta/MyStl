#pragma once

#include <ostream>
#include <cstring>

// Static strings
// Basically a wrapper struct for c-style strings
// ie. these strings are null terminated
//
// @TODO: Make a string stream version for this
// @TODO: Implement a function like sprintf() to support formatting strings
struct string {
public:
    size_t size() const { return _size; }
    const char* data() const { return buffer; }
    char* data() { return buffer; }
    
    // C-tors
    string()
	: buffer(nullptr), _size(0) {}

    string(const char* str) {
	_size  = strlen(str);
	buffer = new char[_size + 1];
	strcpy(buffer, str);
    }
    
    // Copy C-tor
    string(const string& other)
	: _size(other._size) {
	buffer = new char[_size + 1];
	strcpy(buffer, other.buffer);
    }

    // Move C-tor
    string(string&& other)
	: buffer(other.buffer), _size(other._size) {
	other.buffer = nullptr;
	other._size  = 0;
    }
    
    // D-tor
    ~string() {
	delete[] buffer;
    }

    // Operators

    // = operator for assignment
    string& operator=(const string& other) {
	if (buffer) delete[] buffer;
	
	_size  = other._size;
	buffer = new char[_size + 1];
	strcpy(buffer, other.buffer);

	return *this;
    }

    string& operator=(string&& other) {
	if (buffer != other.buffer) {
	    if (buffer) delete[] buffer;
	
	    _size  = other._size;
	    buffer = other.buffer;

	    other.buffer = nullptr;
	    other._size  = 0;
	}
	
	return *this;
    }

    string& operator=(const char* str) {
	if (buffer) delete[] buffer;

	_size  = strlen(str);
	buffer = new char[_size] + 1;
	strcpy(buffer, str);

	return *this;
    }
    
    // [] operator to get char at given index
    const char& operator[](size_t index) const {
	assert(index < _size);
	return buffer[index];
    }

    char& operator[](size_t index) {
	assert(index < _size);
	return buffer[index];
    }

    //@DECIDE: Should strings have iterators?
    
private:
    // Give string to std::ostream
    // Can be used with std::cout
    friend std::ostream& operator<<(std::ostream& stream, const string& s) {
	stream << s.buffer;
	return stream;
    }    

private:
    size_t _size;
    char*   buffer;
};
