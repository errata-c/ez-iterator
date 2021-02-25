#include <ez/meta.hpp>
#include <cinttypes>
#include <cassert>

namespace ez {
	namespace intern {

		template<typename Iter>
		class enumerate_iterator {
		public:
			static constexpr bool
				at_least_bidirectional = ez::is_bidirectional_iterator_v<Iter>,
				at_least_random = ez::is_random_iterator_v<Iter>;

			using utype = ez::iterator_value_t<Iter>;
			using utype_ref = utype&;

			using size_type = std::size_t;
			using difference_type = std::ptrdiff_t;
			struct value_type {
				utype_ref value;
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
				return { *iter, index };
			}
			value_type operator*() {
				return { *iter, index };
			}

			enumerate_iterator& operator++() {
				++iter;
				++index;
				return *this;
			}
			enumerate_iterator operator++(int) {
				enumerate_iterator copy = *this;
				++(*this);
				return copy;
			}

			template<typename = std::enable_if_t<at_least_bidirectional>>
			enumerate_iterator& operator--() {
				--iter;
				--index;
				return *this;
			}
			template<typename = std::enable_if_t<at_least_bidirectional>>
			enumerate_iterator& operator--(int) {
				enumerate_iterator copy = *this;
				--(*this);
				return copy;
			}

			bool operator==(const enumerate_iterator& other) const noexcept {
				return iter == other.iter;
			}
			bool operator!=(const enumerate_iterator& other) const noexcept {
				return iter != other.iter;
			}

			template<typename = std::enable_if_t<at_least_random>>
			bool operator<(const enumerate_iterator& other) const noexcept {
				return iter < other.iter;
			}
			template<typename = std::enable_if_t<at_least_random>>
			bool operator<=(const enumerate_iterator& other) const noexcept {
				return iter <= other.iter;
			}
			template<typename = std::enable_if_t<at_least_random>>
			bool operator>(const enumerate_iterator& other) const noexcept {
				return iter > other.iter;
			}
			template<typename = std::enable_if_t<at_least_random>>
			bool operator>=(const enumerate_iterator& other) const noexcept {
				return iter >= other.iter;
			}

			template<typename = std::enable_if_t<at_least_random>>
			enumerate_iterator operator+(difference_type offset) const {
				return enumerate_iterator{iter + offset, index + offset};
			}
			template<typename = std::enable_if_t<at_least_random>>
			enumerate_iterator operator-(difference_type offset) const {
				return enumerate_iterator{ iter - offset, index - offset };
			}
			template<typename = std::enable_if_t<at_least_random>>
			enumerate_iterator& operator+=(difference_type offset) {
				iter += offset;
				index += offset;
				return *this;
			}
			template<typename = std::enable_if_t<at_least_random>>
			enumerate_iterator& operator-=(difference_type offset) {
				iter -= offset;
				index -= offset;
				return *this;
			}
		private:
			Iter iter;
			difference_type index;
		};

		template<typename Iter, typename = std::enable_if_t<ez::is_random_iterator_v<Iter>>>
		Iter operator+(typename Iter::difference_type offset, const Iter& iter) {
			Iter copy = iter;
			copy += offset;
			return copy;
		};
		template<typename Iter, typename = std::enable_if_t<ez::is_random_iterator_v<Iter>>>
		Iter operator-(typename Iter::difference_type offset, const Iter& iter) {
			Iter copy = iter;
			copy -= offset;
			return copy;
		};
	};

	template<typename Container>
	auto enumerate(Container&& container) {
		using container_t = std::remove_reference_t<Container>;

		static_assert(ez::is_forward_iterator_v<typename container_t::iterator>, "ez::enumerate requires at least a forward iterator!");

		using iterator = intern::enumerate_iterator<decltype(std::declval<container_t>().begin())>;
		
		return intern::simple_range<iterator>{
			iterator{container.begin(), 0},
			iterator{container.end(), static_cast<std::ptrdiff_t>(container.size())}
		};
	}

	template<typename Container>
	auto renumerate(Container&& container) {
		using container_t = std::remove_reference_t<Container>;
		using iterator = intern::enumerate_iterator<decltype(std::declval<container_t>().begin())>;
		
		static_assert(ez::is_bidirectional_iterator_v<typename container_t::iterator>, "ez::renumerate requires at least a bidirectional iterator!");

		using reverse_iterator = std::reverse_iterator<iterator>;

		return intern::simple_range<reverse_iterator>{
			std::make_reverse_iterator(iterator{ container.end(), static_cast<std::ptrdiff_t>(container.size()) }),
			std::make_reverse_iterator(iterator{ container.begin(), 0 }),
		};
	}
};