#pragma once
#include "helpers.hpp"
#include "../adapt.hpp"

namespace ez {
	namespace intern {
		template<typename T, T Start, T SuggestEnd, T Inc, typename = std::enable_if_t<std::is_integral<T>::value, int>>
		struct static_range {};

		template<typename T, T Start, T SuggestEnd, T Inc>
		struct static_range<T, Start, SuggestEnd, Inc, std::enable_if_t<std::is_integral_v<T> && (Inc >= 0), int>> {
			static_assert(Inc != 0, "The increment for the range cannot be zero.");
			static_assert(Start < SuggestEnd, "For a positive increment, the start of the range must be less than the end of the range.");

			// Check that the range won't overflow:
			static_assert((std::numeric_limits<T>::max() - SuggestEnd) >= ((SuggestEnd - Start) % Inc), "This range will overflow!");

			// Adjust the range so that it is a multiple of the increment. (Add instead of subtract so that all values within suggested range are covered)
			static constexpr T End = SuggestEnd + ((SuggestEnd - Start) % Inc);

			// Adapt the range_iterator as necessary.
			using iterator = typename ez::offset_adaptor<range_iterator<T>, static_cast<std::ptrdiff_t>(Inc)>;

			constexpr static_range()
			{}
			~static_range() = default;
			constexpr static_range(const static_range&) = default;
			constexpr static_range& operator=(const static_range&) = default;

			constexpr iterator begin() const {
				return iterator(Start);
			}
			constexpr iterator end() const {
				return iterator(End);
			}
		};

		template<typename T, T Start, T SuggestEnd, T Inc>
		struct static_range<T, Start, SuggestEnd, Inc, std::enable_if_t<std::is_integral_v<T> && (Inc < 0), int>> {
			static_assert(Start > SuggestEnd, "For a negative increment, the start of the range must be greater than the end of the range.");

			// Check that the range won't underflow:
			static_assert((Start - std::numeric_limits<T>::min()) >= ((Start - SuggestEnd) % (-Inc)), "This range will underflow!");

			// Make sure that (End - Start) is a multiple of Inc
			static constexpr T End = SuggestEnd - ((Start - SuggestEnd) % (-Inc));

			using iterator = typename ez::offset_adaptor<range_iterator<T>, static_cast<std::ptrdiff_t>(Inc)>;

			constexpr static_range()
			{};
			~static_range() = default;
			constexpr static_range(const static_range&) = default;
			constexpr static_range& operator=(const static_range&) = default;

			constexpr iterator begin() const {
				return iterator(Start);
			};
			constexpr iterator end() const {
				return iterator(End);
			};
		};
	};
};