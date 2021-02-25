#pragma once
#include <cinttypes>
#include <type_traits>
#include <iterator>
#include "intern/helpers.hpp"

namespace ez {

	template<typename Iter, std::ptrdiff_t Inc>
	struct offset_adaptor: public Iter {
		static_assert(ez::is_random_iterator_v<Iter>, "ez::offset_adaptor requires a random access iterator!");

		using Iter::Iter;

		offset_adaptor& operator++() {
			(*this) += Inc;
			return *this;
		};
		offset_adaptor operator++(int) {
			offset_adaptor copy = *this;
			++copy;
			return copy;
		};

		offset_adaptor& operator--() {
			(*this) -= Inc;
			return *this;
		};
		offset_adaptor operator--(int) {
			offset_adaptor copy = *this;
			--copy;
			return copy;
		};
	};

	template<typename Iter, typename Functor>
	class functor_adaptor : public Iter {
	public:
		using functor_t = Functor;
		using parent_t = Iter;

		static_assert(ez::is_iterator_v<parent_t>, "ez::functor_adaptor requires an iterator type!");
		using parent_value_type = ez::iterator_value_t<parent_t>;
		using parent_pointer = parent_value_type*;
		using parent_reference = parent_value_type&;
		using iterator_category = ez::extract_iterator_category_t<parent_t>;

		static_assert(std::is_default_constructible_v<functor_t>, "ez::functor_adaptor requires a default constructible functor!");
		static_assert(std::is_nothrow_copy_constructible_v<parent_t>, "ez::functor_adaptor requires a nothrow copy constructible iterator type!");
		static_assert(std::is_invocable_v<functor_t, parent_reference>, "ez::functor_adaptor requires an invokable object type!");

		functor_adaptor(const parent_t& source)
			: parent_t(source)
		{}
		functor_adaptor(parent_t&& source) noexcept
			: parent_t(std::move(source))
		{}

		functor_adaptor(const functor_adaptor&) = default;
		functor_adaptor(functor_adaptor&&) noexcept = default;
		functor_adaptor& operator=(const functor_adaptor&) = default;
		functor_adaptor& operator=(functor_adaptor&&) noexcept = default;
		
		using ret_type = decltype(functor_t{}(std::declval<parent_value_type>()));

		static constexpr bool is_reference = std::is_lvalue_reference_v<ret_type> || std::is_rvalue_reference_v<ret_type>;

		// if its a reference type then remove reference
		// if its a pointer, treat as value_type
		
		using value_type = std::remove_reference_t<ret_type>;
		using pointer = std::conditional_t<is_reference, value_type*, value_type>;
		using reference = std::conditional_t<is_reference, value_type&, value_type>;
		using difference_type = typename std::ptrdiff_t;

		reference operator*() {
			return functor_t{}(parent_t::operator*());
		}
		pointer operator->() {
			if constexpr (is_reference) {
				return &functor_t{}(parent_t::operator*());
			}
			else {
				return functor_t{}(parent_t::operator*());
			}
		}
	};

	template<typename Iter, typename Functor, typename T = decltype(std::declval<Functor>()(*std::declval<Iter>()))>
	class lambda_adaptor: public Iter {
	public:
		using iterator_category = typename Iter::iterator_category;
		using value_type = T;
		using reference_type = value_type&;
		using pointer_type = value_type*;
		using difference_type = std::ptrdiff_t;

		lambda_adaptor(const Iter& source, Functor&& _func)
			: Iter(source)
			, func(&_func)
		{}
		lambda_adaptor(const Iter& source)
			: Iter(source)
			, func(nullptr)
		{}

		value_type operator*() {
			return (*func)(Iter::operator*());
		}
		value_type operator->() {
			return (*func)(Iter::operator*());
		}
	private:
		Functor* func;
	};

	template<typename Iter, typename Functor, typename T>
	class lambda_adaptor<Iter, Functor, T&> : public Iter {
	public:
		using iterator_category = typename Iter::iterator_category;
		using value_type = T;
		using reference_type = value_type&;
		using pointer_type = value_type*;
		using difference_type = std::ptrdiff_t;

		lambda_adaptor(const Iter& source, Functor&& _func)
			: Iter(source)
			, func(&_func)
		{}
		lambda_adaptor(const Iter& source)
			: Iter(source)
			, func(nullptr)
		{}

		reference_type operator*() {
			return (*func)(Iter::operator*());
		}
		pointer_type operator->() {
			return &(*func)(Iter::operator*());
		}
	private:
		Functor* func;
	};

	template<typename Functor, typename T>
	auto adapt(T& obj) {
		if constexpr (ez::is_iterator_v<T>) {
			return functor_adaptor<T, Functor>(obj);
		}
		else {
			using Iter = decltype(obj.begin());
			return typename intern::simple_range<functor_adaptor<Iter, Functor>>{
				functor_adaptor<Iter, Functor>(obj.begin()),
				functor_adaptor<Iter, Functor>(obj.end())
			};
		}
	};

	template<typename T, typename Functor>
	auto adapt(T obj, Functor&& func) {
		if constexpr (ez::is_iterator_v<T>) {
			return lambda_adaptor<T, Functor>(obj, std::move(func));
		}
		else {
			using Iter = decltype(obj.begin());
			return typename intern::simple_range<lambda_adaptor<Iter, Functor>>{
				lambda_adaptor<Iter, Functor>(obj.begin(), std::move(func)),
				lambda_adaptor<Iter, Functor>(obj.end())
			};
		}
	};

	template<typename iterator>
	using deref_adaptor = functor_adaptor<iterator, typename intern::deref_functor<iterator_value_t<iterator>>>;
};