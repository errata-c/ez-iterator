#include <ez/iterator.hpp>
#include <fmt/printf.h>
#include <vector>

int main(int argc, char ** argv) {

	fmt::print("This is a test!\n");
	
	std::vector<float> vec;
	for (float t = 0.f; t < 1.f; t += 0.1f) {
		vec.push_back(t);
	}

	for (auto && [value, index] : ez::enumerate(vec)) {
		fmt::print("Is this working? {}, {}.\n", value, index);
	}

	for (auto && [value, index]: ez::renumerate(vec)) {
		fmt::print("How bout this? {}, {}\n", value, index);
		value = 0.f;
	}

	for (auto && [value, index]: ez::enumerate(vec)) {
		fmt::print("Were the values changed to 0.0? {}, {}\n", value, index);
	}

	/*
	For ranges we need a few specific things:
	I want to be able to make ranges for any specific arithmetic type.
	I want to be able to have different offsets, and have both dynamic ranges, and compile time ranges.
	*/

	for (auto i : ez::srange<0, 10, 2>) {
		fmt::print("{}\n", i);
	}

	//for (int i: ez::range(0, 10)) {
	//	fmt::print("Dynamic range. {}\n", i);
	//}

	return 0;
}