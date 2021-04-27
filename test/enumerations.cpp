#include "helpers.hpp"
#include <ez/iterator.hpp>
#include <vector>
#include <array>
#include <cassert>

void test_enumerations() {
	fmt::print("Begin test_enumerations()\n");

	// initialization
	std::vector<float> vec;
	float
		delta = 0.1f,
		minValue = 0.1f,
		maxValue = 1.f;
	{
		float t = minValue;
		for (int i = 0; i < 10; ++i) {
			vec.push_back(t);
			t += delta;
		}

		// Make sure the assumptions are correct, or the next sections will be wrong.
		CHECK(approxEq(vec.front(), minValue));
		CHECK(approxEq(vec.back(), maxValue));
		CHECK(vec.size() == 10);
	}
	fmt::print("Assumptions required for test_enumerations() passed.\n");

	{ // normal enumeration
		int i = 0;
		float t = minValue;

		for (auto&& [value, index] : ez::enumerate(vec)) {
			CHECK(approxEq(value, t));
			CHECK(index == i);

			++i;
			t += delta;
		}
	}
	fmt::print("Normal enumeration test passed\n");

	{ // reverse enumeration
		int i = vec.size()-1;
		float t = maxValue;

		for (auto&& [value, index] : ez::renumerate(vec)) {
			CHECK(approxEq(value, t));
			CHECK(index == i);

			--i;
			t -= delta;
		}
	}
	fmt::print("Reverse enumeration test passed\n");

	fmt::print("End test_enumerations()\n");
}