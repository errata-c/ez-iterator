#include <ez/iterator.hpp>
#include <ez/iterator/adapt.hpp>
#include <fmt/printf.h>
#include <vector>

int main() {
	fmt::print("adaptor test\n");

	int one = 1;
	int two = 2;
	int three = 3;

	std::vector<int*> values;
	values.push_back(&one);
	values.push_back(&two);
	values.push_back(&three);

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
	assert(b == a);

	adapted begin(values.begin());
	adapted end(values.end());

	for (adapted iter = begin; iter != end; ++iter) {
		int& val = *iter;
		fmt::print("Value: {}\n", val);
	}

	for (int & val : ez::adapt<deref>(values)) {
		fmt::print("Value: {}\n", val);
	}

	for (int& val : ez::adapt(values, [](int* ptr) -> int& { return *ptr; })) {
		fmt::print("Value: {}\n", val);
	}

	return 0;
}