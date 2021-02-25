#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <ez/iterator.hpp>
#include <fmt/printf.h>
#include <vector>

TEST_CASE("enumerations") {
	std::vector<float> vec;
	for (float t = 0.f; t < 1.f; t += 0.1f) {
		vec.push_back(t);
	}

	int i = 0;
	float t = 0.f;
	for (auto && [value, index] : ez::enumerate(vec)) {
		REQUIRE(index == i);
		REQUIRE(std::abs(t - value) < 1e-3f);
		++i;
		t += 0.1f;
	}

	i = vec.size() - 1;
	t = 0.9f;
	for (auto && [value, index]: ez::renumerate(vec)) {
		bool index_result = i == index;
		bool value_result = std::abs(t - value) < 1e-3f;

		//value = 0.f;

		--i;
		t -= 0.1f;

		REQUIRE(index_result == true);
		REQUIRE(value_result == true);
	}

	for (auto && [value, index]: ez::enumerate(vec)) {
		value = 0.f;
		REQUIRE(std::abs(value) < 1e-4f);
	}

	/*
	For ranges we need a few specific things:
	I want to be able to make ranges for any specific arithmetic type.
	I want to be able to have different offsets, and have both dynamic ranges, and compile time ranges.
	*/

	i = 0;
	for (auto k : ez::srange<0, 10, 2>) {
		REQUIRE(i == k);
		i += 2;

		REQUIRE(i <= 10);
	}

	//for (int i: ez::range(0, 10)) {
	//	fmt::print("Dynamic range. {}\n", i);
	//}
}