#pragma once

#include <ostream>
#include <cstring>

#include "string.hpp"

// String builder class
// This will allow appending and prepending strings without
// the performance cost of copying or moving data

// @IMPROVE: strings are created then moved instead of simply moving
//           check if this can be done in a better way
class string_builder {
public:
    // Append substring
    string_builder& append(const string& substr) {
    	if (!first) {
	    last = first = new substring(substr);
	} else {
	    last->next   = new substring(substr);
	    last         = last->next;
	}

	total_length += last->_string.size();
	return *this;
    }
    
    string_builder& append(string&& substr) {
	if (!first) {
	    last = first = new substring(std::move(substr));
	} else {
	    last->next   = new substring(std::move(substr));
	    last         = last->next;
	}

	total_length += last->_string.size();       
	return *this;
    }

    // Prepend String
    string_builder& prepend(const string& substr) {
    	if (!first) {
	    last = first = new substring(substr);
	} else {
	    substring* newsubstring = new substring(substr);
	    newsubstring->next      = first;
	    first                   = newsubstring;
	}

	total_length += first->_string.size();	
	return *this;
    }
    
    string_builder& prepend(string&& substr) {
	if (!first) {
	    last = first = new substring(std::move(substr));
	} else {
	    substring* newsubstring = new substring(std::move(substr));
	    newsubstring->next      = first;
	    first                   = newsubstring;
	}

	total_length += first->_string.size();	
	return *this;
    }

    // Remove all substrings
    void clear() {
	substring* it;
	while (first) {
	    it = first;
	    first = first->next;
	    delete it;
	}

	first = last = nullptr;
	total_length = 0;
    }
    
    // C-tor
    string_builder()
	: total_length(0), first(nullptr), last(nullptr) {}

    // D-tor
    ~string_builder() {
	substring* it;
	while (first) {
	    it = first;
	    first = first->next;
	    delete it;
	}
    }
    
private:
    friend string build_string(const string_builder& sb);

    friend std::ostream& operator<<(std::ostream& stream, const string_builder& sb) {
	stream << "string_builder { total_length: " << sb.total_length << " }";
	return stream;
    }
    
private:
    // Substring struct
    // String builder is made up of multiple substrings appended together
    struct substring {
	string     _string;
	substring*  next;

	// Substring C-tors
	substring(const char* str)
	    : _string(str), next(nullptr) {}

	substring(const string& str)
	    : _string(str), next(nullptr) {}

	substring(string&& str)
	    : _string(std::move(str)), next(nullptr) {}
    };

    size_t total_length     = 0;         // This doesn't include null character
    substring* first, *last = nullptr;
};

// Returns a string made from the substrings in the given string builder
string build_string(const string_builder& sb) {
    char* result = new char[sb.total_length + 1];

    string_builder::substring* it = sb.first;
    char* index = result;
    while (it) {
	strcpy(index, it->_string.data());
	index += it->_string.size();
	it     = it->next;
    }
    
    *index = '\0';
    return std::move(string(result));
}
