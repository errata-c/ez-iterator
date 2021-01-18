#pragma once
#include <cinttypes>
#include <type_traits>

namespace ez {
	namespace intern {
		template<typename T, std::enable_if_t<std::is_integral<T>::value, int> = 0>
		class range_iterator {
		public:
			using size_type = typename std::make_unsigned<T>::type;
			using difference_type = typename std::make_signed<T>::type;

			using value_type = T;

			using reference = value_type&;
			using pointer = value_type*;

			using iterator_category = std::random_access_iterator_tag;

			constexpr range_iterator(T _index)
				: index(_index)
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
				index += 1;
				return *this;
			};
			range_iterator operator++(int) {
				range_iterator copy = *this;
				++(*this);
				return copy;
			};

			range_iterator& operator--() {
				index -= 1;
				return *this;
			};
			range_iterator operator--(int) {
				range_iterator copy = *this;
				--(*this);
				return copy;
			};

			constexpr range_iterator operator+(difference_type val) const {
				return range_iterator(index + val);
			};
			constexpr range_iterator operator-(difference_type val) const {
				return range_iterator(index - val);
			};
			constexpr difference_type operator-(range_iterator other) const {
				return index - other.index;
			};

			range_iterator& operator+=(difference_type val) {
				index += val;
				return *this;
			};
			range_iterator& operator-=(difference_type val) {
				index -= val;
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
				return index + offset;
			};
		private:
			value_type index;
		};

		template<typename T>
		constexpr range_iterator<T> operator+(T val, const range_iterator<T>& iter) {
			return range_iterator<T>(*iter + val);
		}


	};
};