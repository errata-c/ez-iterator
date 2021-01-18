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

	struct deref {
		int& operator()(int* ptr) {
			return *ptr;
		}
	};

	using adapted = ez::functor_adaptor<std::vector<int*>::iterator, deref>;

	adapted begin = ez::adapt<deref>(values.begin());
	adapted end = ez::adapt<deref>(values.end());

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