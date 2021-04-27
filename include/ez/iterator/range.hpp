#pragma once
#include <cinttypes>
#include <type_traits>
#include <cassert>
#include <limits>
#include <stdexcept>
#include "intern/helpers.hpp"

namespace ez {
	namespace intern {
		template<typename T, bool = std::is_integral_v<T>>
		class range_iterator {
		public:
			using size_type = typename std::make_unsigned_t<T>;
			using difference_type = typename std::make_signed_t<T>;

			using value_type = T;

			using reference = value_type&;
			using pointer = value_type*;

			using iterator_category = std::random_access_iterator_tag;

			constexpr range_iterator(value_type _index, difference_type _inc)
				: index(_index)
				, increment(_inc)
			{}
			constexpr range_iterator(const range_iterator& other) = default;
			~range_iterator() = default;

			constexpr range_iterator& operator=(const range_iterator&) = default;

			constexpr value_type operator->() {
				return index;
			};
			constexpr value_type operator*() {
				return index;
			};

			range_iterator& operator++() {
				index += increment;
				return *this;
			};
			range_iterator operator++(int) {
				range_iterator copy = *this;
				++(*this);
				return copy;
			};

			range_iterator& operator--() {
				index -= increment;
				return *this;
			};
			range_iterator operator--(int) {
				range_iterator copy = *this;
				--(*this);
				return copy;
			};

			constexpr range_iterator operator+(difference_type val) const {
				return range_iterator(index + val * increment);
			};
			constexpr range_iterator operator-(difference_type val) const {
				return range_iterator(index - val * increment);
			};
			constexpr difference_type operator-(range_iterator other) const {
				return index - other.index;
			};

			range_iterator& operator+=(difference_type val) {
				index += val * increment;
				return *this;
			};
			range_iterator& operator-=(difference_type val) {
				index -= val * increment;
				return *this;
			};

			constexpr bool operator==(const range_iterator& other) const {
				return index == other.index;
			};
			constexpr bool operator!=(const range_iterator& other) const {
				return index != other.index;
			};

			constexpr bool operator<(const range_iterator& other) const {
				return index < other.index;
			};
			constexpr bool operator>(const range_iterator& other) const {
				return index > other.index;
			};

			constexpr bool operator<=(const range_iterator& other) const {
				return index <= other.index;
			};
			constexpr bool operator>=(const range_iterator& other) const {
				return index >= other.index;
			};

			constexpr value_type operator[](difference_type offset) const {
				return index + offset * increment;
			};
		private:
			value_type index;
			difference_type increment;
		};

		template<typename T>
		class range_iterator<T, false> {
		public:
			static_assert(std::is_floating_point_v<T>, "ez::range requires an arithmetic type!");

			using size_type = T;
			using difference_type = T;
			using value_type = T;

			using reference = value_type&;
			using pointer = value_type*;

			using iterator_category = std::random_access_iterator_tag;

			constexpr range_iterator(T _index, T _inc)
				: index(_index)
				, increment(_inc)
			{}
			constexpr range_iterator(const range_iterator& other) = default;
			~range_iterator() = default;

			constexpr range_iterator& operator=(const range_iterator&) = default;

			constexpr value_type operator->() {
				return index;
			};
			constexpr value_type operator*() {
				return index;
			};

			range_iterator& operator++() {
				index += increment;
				return *this;
			};
			range_iterator operator++(int) {
				range_iterator copy = *this;
				++(*this);
				return copy;
			};

			range_iterator& operator--() {
				index -= increment;
				return *this;
			};
			range_iterator operator--(int) {
				range_iterator copy = *this;
				--(*this);
				return copy;
			};

			constexpr range_iterator operator+(difference_type val) const {
				return range_iterator(index + val * increment);
			};
			constexpr range_iterator operator-(difference_type val) const {
				return range_iterator(index - val * increment);
			};
			constexpr difference_type operator-(range_iterator other) const {
				return index - other.index;
			};

			range_iterator& operator+=(difference_type val) {
				index += val * increment;
				return *this;
			};
			range_iterator& operator-=(difference_type val) {
				index -= val * increment;
				return *this;
			};

			constexpr bool operator==(const range_iterator& other) const {
				return index == other.index;
			};
			constexpr bool operator!=(const range_iterator& other) const {
				return index != other.index;
			};

			constexpr bool operator<(const range_iterator& other) const {
				return index < other.index;
			};
			constexpr bool operator>(const range_iterator& other) const {
				return index > other.index;
			};

			constexpr bool operator<=(const range_iterator& other) const {
				return index <= other.index;
			};
			constexpr bool operator>=(const range_iterator& other) const {
				return index >= other.index;
			};

			constexpr value_type operator[](difference_type offset) const {
				return index + offset * increment;
			};
		private:
			value_type index, increment;
		};
	};

	template<typename T>
	constexpr intern::simple_range<intern::range_iterator<T>> range(T end) noexcept {
		using stype = std::make_signed_t<T>;

		if (end < T(0)) {
			return intern::simple_range<intern::range_iterator<T>>{
				intern::range_iterator<T>{end, stype(-1)},
				intern::range_iterator<T>{T(0), stype(-1)}
			};
		}
		else {
			return intern::simple_range<intern::range_iterator<T>>{
				intern::range_iterator<T>{T(0), stype(1)},
				intern::range_iterator<T>{end, stype(1)}
			};
		}
	}

	template<typename T>
	constexpr intern::simple_range<intern::range_iterator<T>> range(T start, T end) noexcept {
		using stype = std::make_signed_t<T>;

		if (end < start) {
			return intern::simple_range<intern::range_iterator<T>>{
				intern::range_iterator<T>{start, stype(-1)},
				intern::range_iterator<T>{end, stype(-1)}
			};
		}
		else {
			return intern::simple_range<intern::range_iterator<T>>{
				intern::range_iterator<T>{start, stype(1)},
				intern::range_iterator<T>{end, stype(1)}
			};
		}
	}

	template<typename T, typename T1>
	constexpr intern::simple_range<intern::range_iterator<T>> range(T start, T end, T1 inc) noexcept {
		if (
			(inc == T1(0)) || // Zero increment range makes no sense
			((end < start) && (inc > 0)) // Increment must actually move in the correct direction.
			) {
			throw std::invalid_argument("Call to ez::range has invalid increment!\nMost likely this means the increment had an incorrect sign.\n");
		}

		return intern::simple_range<intern::range_iterator<T>>{
			intern::range_iterator<T>{start, inc},
			intern::range_iterator<T>{end, inc}
		};
	}
};