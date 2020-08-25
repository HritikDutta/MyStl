#include <iostream>

#include "./include/array.hpp"
#include "./include/string.hpp"
#include "./include/builder.hpp"

struct vec3 {
    float x, y, z;

    vec3()
	: x(0.0f), y(0.0f), z(0.0f) {}
    vec3(float scalar)
	: x(scalar), y(scalar), z(scalar) {}
    vec3(float x, float y, float z)
	: x(x), y(y), z(z) {}

    vec3(const vec3& other)
	: x(other.x), y(other.y), z(other.z) {
	std::cout << "copied" << std::endl;
    }

    vec3(vec3&& other)
	: x(other.x), y(other.y), z(other.z) {
	std::cout << "moved" << std::endl;
    }
    
    ~vec3() {
	std::cout << "destroyed" << std::endl;
    }

    vec3& operator=(const vec3& other) {
	std::cout << "copied" << std::endl;

	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
    }

    vec3& operator=(vec3&& other) {
	std::cout << "moved" << std::endl;

	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
    }
};

template<>
void print_array(const darray<vec3>& arr) {
    for (size_t i = 0; i < arr.size(); i++)
	std::cout << arr[i].x << ", " << arr[i].y << ", " << arr[i].z << std::endl;

    std::cout << std::endl;
}

int main() {
    darray<int> arr = { 1, 2, 3 };
    std::cout << arr[-5] << std::endl;
}
