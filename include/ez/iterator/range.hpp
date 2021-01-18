#pragma once
#include <cinttypes>
#include <type_traits>
#include <cassert>
#include <limits>
#include "intern/static_range.hpp"

namespace ez {
	namespace intern {
		template<typename T, T ... Ts>
		struct range_helper {};

		template<typename T, T End>
		struct range_helper<T, End> {
			using range_t = static_range<T, static_cast<T>(0), End, End >= 0 ? static_cast<T>(1) : static_cast<T>(-1)>;

			static constexpr range_t value = range_t();
		};

		template<typename T, T Start, T End>
		struct range_helper<T, Start, End> {
			using range_t = static_range<T, Start, End, (End >= Start) ? static_cast<T>(1) : static_cast<T>(-1)>;

			static constexpr range_t value = range_t();
		};

		template<typename T, T Start, T End, T Inc>
		struct range_helper<T, Start, End, Inc> {
			using range_t = static_range<T, Start, End, Inc>;

			static constexpr range_t value = range_t();
		};
	};

	// Static Range, compile time bounds
	template<std::ptrdiff_t ... Ts>
	inline constexpr auto srange = intern::range_helper<std::ptrdiff_t, Ts...>::value;

	// Templated Static Range, compile time bounds, custom underlying type.
	template<typename T, T ... Ts>
	inline constexpr auto tsrange = intern::range_helper<T, Ts...>::value;
};