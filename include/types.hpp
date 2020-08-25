#pragma once

#include <typeinfo>
#include <cstdio>

template <typename T>
const char* type_name() {
    return typeid(T).name();
}

template<typename T>
const char* type_name(const T& t) {
    return typeid(t).name();
}

template<typename T>
const char* type_name(T&& t) {
    return typeid(t).name();
}

// Overloads for primitive types
#define DECL_TYPE_NAME(t) template<> const char* type_name<t>() { return #t; }

#define DEFINE_ALL_TYPES(t)          \
    DECL_TYPE_NAME(t)                \
    DECL_TYPE_NAME(const t)          \
    DECL_TYPE_NAME(t*)               \
    DECL_TYPE_NAME(const t*)         \
    DECL_TYPE_NAME(t[])              \
    DECL_TYPE_NAME(const t[])


DEFINE_ALL_TYPES(bool)
DEFINE_ALL_TYPES(char)
DEFINE_ALL_TYPES(int)
DEFINE_ALL_TYPES(float)
DEFINE_ALL_TYPES(double)
