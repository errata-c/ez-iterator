#include <catch2/catch.hpp>

#include <ez/iterator.hpp>
#include <ez/iterator/adapt.hpp>
#include <fmt/printf.h>
#include <vector>
#include <deque>

TEST_CASE("adapt") {
	std::deque<int> data;
	std::vector<int*> values;

	for (int i = 0; i < 10; ++i) {
		data.push_back(i);
		values.push_back(&data.back());
	}

	// Bunch of static checks:

	using deref = ez::intern::deref_functor<std::vector<int*>::value_type>;
	static_assert(std::is_same_v<deref::reference, int&>, "ez::intern::deref_functor is incorrect!");

	static_assert(std::is_same_v<decltype(deref{}(std::declval<int*>())), int& >, "ez::intern::deref_functor is incorrect!");

	using adapted = ez::deref_adaptor<std::vector<int*>::iterator>;
	static_assert(std::is_same_v<adapted::value_type, int>, "ez::functor_adaptor is incorrect!");
	static_assert(std::is_same_v<adapted::pointer, int*>, "ez::functor_adaptor is incorrect!");
	static_assert(std::is_same_v<adapted::reference, int&>, "ez::functor_adaptor is incorrect!");

	static_assert(std::is_default_constructible_v<deref>, "ez::intern::deref_functor is incorrect!");

	static_assert(std::is_same_v<adapted::parent_t, std::vector<int*>::iterator>, "ez::intern::functor_adaptor is incorrect!");
	static_assert(std::is_same_v<adapted::functor_t, deref>, "ez::intern::functor_adaptor is incorrect!");
	//static_assert(std::is_same_v<decltype(deref), std::vector<int*>::iterator>, "ez::intern::functor_adaptor is incorrect!");

	int a = 0;
	int& b = deref{}(&a);
	REQUIRE(b == a);

	adapted begin(values.begin());
	adapted end(values.end());

	REQUIRE((end - begin) == values.size());
	int index = 0;
	for (adapted iter = begin; iter != end; ++iter, ++index) {
		int& val = *iter;
		REQUIRE(data[index] == val);
		REQUIRE(&data[index] == &val);
		REQUIRE(&val == values[index]);
	}

	index = 0;
	for (int& val : ez::adapt(values, [](int* ptr) -> int& { return *ptr; })) {
		REQUIRE(data[index] == val);
		REQUIRE(&data[index] == &val);
		REQUIRE(&val == values[index]);
		++index;
	}
}