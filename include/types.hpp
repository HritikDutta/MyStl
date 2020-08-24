#pragma once

#include <typeinfo>

template <typename T>
const char* type_name() {
    return typeid(T).name();
}
