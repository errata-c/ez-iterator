#include "helpers.hpp"
#include <ez/iterator.hpp>
#include <vector>
#include <array>
#include <cassert>

void test_ranges() {
	fmt::print("Begin test_ranges() tests\n");

	std::ptrdiff_t t = 0;
	for (std::ptrdiff_t i : ez::range<std::ptrdiff_t>(100)) {
		CHECK(i == t);

		++t;
	}
	CHECK(t == 100);
	fmt::print("Single argument range test passed\n");

	t = 0;
	for (std::ptrdiff_t i : ez::range<std::ptrdiff_t>(0, 100)) {
		CHECK(i == t);

		++t;
	}
	CHECK(t == 100);
	fmt::print("Double argument range test passed\n");

	t = 0;
	for (std::ptrdiff_t i : ez::range<std::ptrdiff_t>(0, 100, 1)) {
		CHECK(i == t);

		++t;
	}
	CHECK(t == 100);
	fmt::print("Triple argument range test passed\n");



	fmt::print("End test_ranges() tests\n");
}