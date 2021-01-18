#include <cinttypes>
#include <tuple>
#include <type_traits>
#include <cassert>


namespace ez {
	namespace intern {
		template<typename Iter, typename = int>
		class enumerate_iterator {};

		template<typename Iter>
		class enumerate_iterator<Iter, std::enable_if_t<std::is_same_v<typename Iter::iterator_category, std::input_iterator_tag> || std::is_same_v<typename Iter::iterator_category, std::forward_iterator_tag>, int>> {
		public:
			using utype = typename Iter::value_type;
			using utype_ref = typename Iter::reference;
			using utype_ptr = typename Iter::pointer;

			using size_type = std::size_t;

			struct value_type {
				utype_ref value;
				size_type index;
			};

			using difference_type = std::ptrdiff_t;

			using reference = value_type&;
			using pointer = value_type*;

			using iterator_category = typename Iter::iterator_category;

			enumerate_iterator()
				: iter()
				, index(0)
			{}
			enumerate_iterator(const Iter& _iter, size_type _index)
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


			bool operator==(const enumerate_iterator& other) const {
				return iter == other.iter;
			}
			bool operator!=(const enumerate_iterator& other) const {
				return iter != other.iter;
			}
		private:
			Iter iter;
			size_type index;
		};

		template<typename Iter>
		class enumerate_iterator<Iter, std::enable_if_t<std::is_same_v<typename Iter::iterator_category, std::bidirectional_iterator_tag>, int>> {
		public:
			using utype = typename Iter::value_type;
			using utype_ref = typename Iter::reference;
			using utype_ptr = typename Iter::pointer;

			using size_type = std::size_t;

			struct value_type {
				utype_ref value;
				size_type index;
			};

			using difference_type = std::ptrdiff_t;

			using reference = value_type&;
			using pointer = value_type*;

			using iterator_category = std::bidirectional_iterator_tag;

			enumerate_iterator()
				: iter()
				, index(0)
			{}
			enumerate_iterator(const Iter& _iter, size_type _index)
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

			enumerate_iterator& operator--() {
				--iter;
				--index;
				return *this;
			}
			enumerate_iterator operator--(int) {
				enumerate_iterator copy = *this;
				--(*this);
				return copy;
			}

			bool operator==(const enumerate_iterator& other) const {
				return iter == other.iter;
			}
			bool operator!=(const enumerate_iterator& other) const {
				return iter != other.iter;
			}
		private:
			Iter iter;
			size_type index;
		};

		template<typename Iter>
		class enumerate_iterator<Iter, std::enable_if_t<std::is_same_v<typename Iter::iterator_category, std::random_access_iterator_tag>, int>> {
		public:
			using utype = typename Iter::value_type;
			using utype_ref = typename Iter::reference;
			using utype_ptr = typename Iter::pointer;

			using size_type = std::size_t;

			struct value_type {
				utype_ref value;
				size_type index;
			};

			using difference_type = std::ptrdiff_t;

			using reference = value_type&;
			using pointer = value_type*;

			using iterator_category = std::random_access_iterator_tag;

			enumerate_iterator()
				: iter()
				, index(0)
			{}
			enumerate_iterator(const Iter& _iter, size_type _index)
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

			enumerate_iterator& operator--() {
				--iter;
				--index;
				return *this;
			}
			enumerate_iterator operator--(int) {
				enumerate_iterator copy = *this;
				--(*this);
				return copy;
			}

			enumerate_iterator& operator+=(difference_type offset) {
				index += offset;
				iter += offset;
				return *this;
			}
			enumerate_iterator& operator-=(difference_type offset) {
				index -= offset;
				iter -= offset;
				return *this;
			}
			difference_type operator-(const enumerate_iterator& other) const {
				return iter - other.iter;
			}
			enumerate_iterator operator+(difference_type offset) const {
				enumerate_iterator copy = *this;
				copy.iter += offset;
				copy.index += offset;
				return copy;
			}
			enumerate_iterator operator-(difference_type offset) const {
				enumerate_iterator copy = *this;
				copy.iter -= offset;
				copy.index -= offset;
				return copy;
			}

			value_type operator[](difference_type offset) {
				return { *(iter + offset), index + offset };
			}

			bool operator==(const enumerate_iterator& other) const {
				return iter == other.iter;
			}
			bool operator!=(const enumerate_iterator& other) const {
				return iter != other.iter;
			}

			bool operator<(const enumerate_iterator& other) const {
				return iter < other.iter;
			}
			bool operator>(const enumerate_iterator& other) const {
				return iter > other.iter;
			}
			bool operator<=(const enumerate_iterator& other) const {
				return iter <= other.iter;
			}
			bool operator>=(const enumerate_iterator& other) const {
				return iter >= other.iter;
			}
		private:
			Iter iter;
			size_type index;
		};

		template<typename Iter, std::enable_if_t<std::is_same_v<typename Iter::iterator_category, std::random_access_iterator_tag>, int> = 0>
		Iter operator+(typename Iter::difference_type offset, const Iter& iter) {
			Iter copy = iter;
			copy += offset;
			return copy;
		};
		template<typename Iter, std::enable_if_t<std::is_same_v<typename Iter::iterator_category, std::random_access_iterator_tag>, int> = 0>
		Iter operator-(typename Iter::difference_type offset, const Iter& iter) {
			Iter copy = iter;
			copy -= offset;
			return copy;
		};

		template<typename Container, bool = false, typename = int>
		struct enumerate_range {};

		template<typename Container>
		struct enumerate_range<Container, false, decltype(((Container*)0)->begin(), ((Container*)0)->end(), ((Container*)0)->size(), 0)> {
			using iterator = enumerate_iterator<typename Container::iterator>;

			enumerate_range(Container& _container)
				: container(&_container)
			{}

			Container* container;

			iterator begin() {
				return iterator(container->begin(), 0);
			};
			iterator end() {
				return iterator(container->end(), container->size());
			};
		};

		template<typename Container>
		struct enumerate_range<Container, true, decltype(((Container*)0)->begin(), ((Container*)0)->end(), ((Container*)0)->size(), 0)> {
			using iterator = enumerate_iterator<typename Container::const_iterator>;

			enumerate_range(const Container& _container)
				: container(&_container)
			{}

			const Container* container;

			iterator begin() {
				return iterator(container->begin(), 0);
			};
			iterator end() {
				return iterator(container->end(), container->size());
			};
		};

		template<typename Container, bool = false, typename = int>
		struct renumerate_range {};

		template<typename Container>
		struct renumerate_range<Container, false, decltype(((Container*)0)->begin(), ((Container*)0)->end(), ((Container*)0)->size(), 0)> {
			using iterator = enumerate_iterator<std::reverse_iterator<typename Container::iterator>>;

			renumerate_range(Container& _container)
				: container(&_container)
			{}

			Container* container;

			iterator begin() {
				return iterator(std::make_reverse_iterator(container->end()), 0);
			};
			iterator end() {
				return iterator(std::make_reverse_iterator(container->begin()), container->size());
			};
		};

		template<typename Container>
		struct renumerate_range<Container, true, decltype(((Container*)0)->begin(), ((Container*)0)->end(), ((Container*)0)->size(), 0)> {
			using iterator = enumerate_iterator<std::reverse_iterator<typename Container::iterator>>;

			renumerate_range(const Container& _container)
				: container(&_container)
			{}

			const Container* container;

			iterator begin() {
				return iterator(std::make_reverse_iterator(container->end()), 0);
			};
			iterator end() {
				return iterator(std::make_reverse_iterator(container->begin()), container->size());
			};
		};
	};

	template<typename Container>
	auto enumerate(Container&& container) {
		return intern::enumerate_range<std::remove_const_t<std::remove_reference_t<Container>>, std::is_const_v<decltype(container)>, int>{ std::forward<Container>(container) };
	}

	template<typename Container>
	auto renumerate(Container&& container) {
		return intern::renumerate_range<std::remove_const_t<std::remove_reference_t<Container>>, std::is_const_v<decltype(container)>, int>{ std::forward<Container>(container) };
	}
};