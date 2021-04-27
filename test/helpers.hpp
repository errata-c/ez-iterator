#pragma once
#include <fmt/core.h>


#define CHECK(EXPR) \
do {if(!(EXPR)){fmt::print(stderr, "Check at line {} failed!\n" #EXPR "\n", __LINE__); return; }} while(0)

inline bool approxEq(float lh, float rh) {
	return std::abs(lh - rh) < 1e-4f;
}