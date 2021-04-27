#include <ez/meta.hpp>
#include <cinttypes>
#include <cassert>

namespace ez {
	namespace intern {
		/*
		Wraps an iterator type, and keeps track of an index value.
		*/
		template<typename Iter, bool reversed = false>
		class enumerate_iterator {
		public:
			static constexpr bool
				at_least_bidirectional = ez::is_bidirectional_iterator_v<Iter>,
				at_least_random = ez::is_random_iterator_v<Iter>;

			static_assert(!reversed || (reversed && at_least_bidirectional), "Reversed enumeration requires at least a bidirectional iterator!");

			using utype = ez::iterator_value_t<Iter>;
			using utype_reference = utype&;
			using utype_pointer = utype*;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			struct value_type {
				utype_reference value;
				difference_type index;
			};
			using iterator_category = ez::extract_iterator_category_t<Iter>;
			using reference = value_type&;
			using pointer = value_type*;

			enumerate_iterator()
				: iter()
				, index(0)
			{}
			enumerate_iterator(const Iter& _iter, difference_type _index)
				: iter(_iter)
				, index(_index)
			{}
			enumerate_iterator(const enumerate_iterator& other) = default;
			~enumerate_iterator() = default;

			enumerate_iterator& operator=(const enumerate_iterator&) = default;

			value_type operator->() {
				if constexpr (reversed) {
					Iter copy = iter;
					return { *--copy, index };
				}
				else {
					return { *iter, index };
				}
			}
			value_type operator*() {
				if constexpr (reversed) {
					Iter copy = iter;
					return { *--copy, index };
				}
				else {
					return { *iter, index };
				}
			}

			enumerate_iterator& operator++() {
				if constexpr (reversed) {
					--iter;
					--index;
				}
				else {
					++iter;
					++index;
				}
				
				return *this;
			}
			enumerate_iterator operator++(int) {
				enumerate_iterator copy = *this;
				if constexpr (reversed) {
					--(*this);
				}
				else {
					++(*this);
				}
				
				return copy;
			}

			template<typename = std::enable_if_t<at_least_bidirectional>>
			enumerate_iterator& operator--() {
				if constexpr (reversed) {
					++iter;
					++index;
				}
				else {
					--iter;
					--index;
				}
				
				return *this;
			}
			template<typename = std::enable_if_t<at_least_bidirectional>>
			enumerate_iterator& operator--(int) {
				enumerate_iterator copy = *this;
				if constexpr (reversed) {
					++(*this);
				}
				else {
					--(*this);
				}
				
				return copy;
			}

			bool operator==(const enumerate_iterator& other) const noexcept {
				return index == other.index;
			}
			bool operator!=(const enumerate_iterator& other) const noexcept {
				return index != other.index;
			}

			template<typename = std::enable_if_t<at_least_random>>
			bool operator<(const enumerate_iterator& other) const noexcept {
				return index < other.index;
			}
			template<typename = std::enable_if_t<at_least_random>>
			bool operator<=(const enumerate_iterator& other) const noexcept {
				return index <= other.index;
			}
			template<typename = std::enable_if_t<at_least_random>>
			bool operator>(const enumerate_iterator& other) const noexcept {
				return index > other.index;
			}
			template<typename = std::enable_if_t<at_least_random>>
			bool operator>=(const enumerate_iterator& other) const noexcept {
				return index >= other.index;
			}

			template<typename = std::enable_if_t<at_least_random>>
			difference_type operator-(enumerate_iterator other) const {
				return index - other.index;
			}

			template<typename = std::enable_if_t<at_least_random>>
			enumerate_iterator operator+(difference_type offset) const {
				if constexpr (reversed) {
					offset = -offset;
				}

				return enumerate_iterator{iter + offset, index + offset};
			}
			template<typename = std::enable_if_t<at_least_random>>
			enumerate_iterator operator-(difference_type offset) const {
				if constexpr (reversed) {
					offset = -offset;
				}

				return enumerate_iterator{ iter - offset, index - offset };
			}
			template<typename = std::enable_if_t<at_least_random>>
			enumerate_iterator& operator+=(difference_type offset) {
				if constexpr (reversed) {
					offset = -offset;
				}

				iter += offset;
				index += offset;

				return *this;
			}
			template<typename = std::enable_if_t<at_least_random>>
			enumerate_iterator& operator-=(difference_type offset) {
				if constexpr (reversed) {
					offset = -offset;
				}

				iter -= offset;
				index -= offset;

				return *this;
			}
		private:
			Iter iter;
			difference_type index;
		};
	};

	// Enumerate a container, run through the elements in the container and provide an index value along the way.
	// The type returned when dereferencing the iterators in the range is a simple struct containing two data members, 'value' and 'index'
	// You can use structured bindings to make the enumeration more intuitive, see the examples for how.
	template<typename Container>
	auto enumerate(Container&& container) {
		using container_t = std::remove_reference_t<Container>;
		using container_iterator_t = decltype(container.begin());

		static_assert(ez::is_forward_iterator_v<container_iterator_t>, "ez::enumerate requires at least a forward iterator!");

		using enumerator_t = intern::enumerate_iterator<container_iterator_t>;
		
		return intern::simple_range<enumerator_t>{
			enumerator_t{container.begin(), 0},
			enumerator_t{container.end(), static_cast<std::ptrdiff_t>(container.size())}
		};
	}

	// Reverse enumerate, does the exact same thing as ez::enumerate but in the opposite direction.
	template<typename Container>
	auto renumerate(Container&& container) {
		using container_t = std::remove_reference_t<Container>;
		using container_iterator_t = decltype(container.begin());
		
		static_assert(ez::is_bidirectional_iterator_v<container_iterator_t>, "ez::renumerate requires at least a bidirectional iterator!");

		using enumerator_t = intern::enumerate_iterator<container_iterator_t, true>;

		return intern::simple_range<enumerator_t>{
			enumerator_t{ container.end(), static_cast<std::ptrdiff_t>(container.size()) - 1 },
				enumerator_t{ container.begin(), -1 },
		};
	}
};