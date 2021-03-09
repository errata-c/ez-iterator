#pragma once
// For std::size_t and std::ptrdiff_t
#include <cstddef>
// various static inspection functions
#include <type_traits>
// For the iterator categories
#include <iterator>
// For std::addressof
#include <memory>

#include "intern/helpers.hpp"

namespace ez {
	// Do we still need this?
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
		using parent_deref_type = decltype(std::declval<parent_t>().operator*());

		static_assert(std::is_default_constructible_v<functor_t>, "ez::functor_adaptor requires a default constructible functor!");
		static_assert(std::is_nothrow_copy_constructible_v<parent_t>, "ez::functor_adaptor requires a nothrow copy constructible iterator type!");
		static_assert(std::is_invocable_v<functor_t, parent_deref_type>, "ez::functor_adaptor requires an invokable object type!");

		using ret_type = decltype(functor_t{}(std::declval<parent_deref_type>()));
		static_assert(!std::is_rvalue_reference_v<ret_type>, "Returning rvalue references from an iterator adaptor is not supported!");
		static constexpr bool is_reference = std::is_lvalue_reference_v<ret_type>;

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
			// Only return an actual pointer type if the return type has an actual address
			if constexpr (is_reference) {
				return std::addressof(functor_t{}(parent_t::operator*()));
			}
			else {
				return functor_t{}(parent_t::operator*());
			}
		}
	};

	template<typename Iter, typename Functor>
	class lambda_adaptor : public Iter {
	public:
		using functor_t = Functor;
		using parent_t = Iter;

		static_assert(ez::is_iterator_v<parent_t>, "ez::lambda_adaptor requires an iterator type!");
		using parent_value_type = ez::iterator_value_t<parent_t>;
		using parent_pointer = parent_value_type*;
		using parent_reference = parent_value_type&;

		// We can only support forward iteration at best, because we have to take ownership of the lambda.
		// This will hopefully prevent people from trying to reverse the range.
		using iterator_category = 
			std::conditional_t<std::is_same_v<ez::extract_iterator_category_t<parent_t>, std::input_iterator_tag>,
			std::input_iterator_tag, std::forward_iterator_tag>;

		// We have to get the exact type resulting from dereferencing the iterator, to make sure the later invocation check works
		using parent_deref_type = decltype(std::declval<parent_t>().operator*());

		// Not all lambdas are trivially movable, it depends on the captured parameters.
		static_assert(std::is_trivially_move_constructible_v<Functor>, 
			"ez::lambda_adaptor requires a trivially movable lambda! This means that all captured variables must also be trivially movable.");
		
		// Make sure that the lambda can actually be called with the iterators return type.
		static_assert(std::is_invocable_v<functor_t, parent_deref_type>, "ez::lambda_adaptor requires a lambda invokable with the type returned from the iterator!");

		using ret_type = decltype(std::declval<functor_t>()(std::declval<parent_deref_type>()));
		
		// I don't know what kind of use case rvalues would even have for returning from an adaptor.
		// If you truely want that, just return lvalue and call std::move
		static_assert(!std::is_rvalue_reference_v<ret_type>, "Returning rvalue references from an iterator adaptor is not supported!");
		static constexpr bool is_reference = std::is_lvalue_reference_v<ret_type>;

		using value_type = std::remove_reference_t<ret_type>;
		using pointer = std::conditional_t<is_reference, value_type*, value_type>;
		using reference = std::conditional_t<is_reference, value_type&, value_type>;
		using difference_type = typename std::ptrdiff_t;

		lambda_adaptor(const parent_t& source, Functor&& _func)
			: parent_t(source)
			, func(std::move(_func))
		{}

		reference operator*() {
			return func(Iter::operator*());
		}
		pointer operator->() {
			// Only return an actual pointer type if the return type has an actual address
			if constexpr (is_reference) {
				return std::addressof(func(parent_t::operator*()));
			}
			else {
				return func(parent_t::operator*());
			}
		}
	private:
		functor_t func;
	};

	// Adapt an iterator using a functor type directly (instead of passing a functor object into the function)
	template<typename Functor, typename T>
	auto adapt(T& obj) {
		if constexpr (ez::is_iterator_v<T>) {
			return functor_adaptor<T, Functor>(obj);
		}
		else {
			// Check that this exists?
			using Iter = decltype(obj.begin());
			return typename intern::simple_range<functor_adaptor<Iter, Functor>>{
				functor_adaptor<Iter, Functor>(obj.begin()),
				functor_adaptor<Iter, Functor>(obj.end())
			};
		}
	};

	// Adapt using a lambda or an actual functor object instance. This form of adapt takes ownership of the functor passed in.
	template<typename T, typename Functor>
	auto adapt(T& obj, Functor&& func) {
		if constexpr (ez::is_iterator_v<T>) {
			return lambda_adaptor<T, Functor>(obj, std::move(func));
		}
		else {
			// Check that this exists?
			using Iter = decltype(obj.begin());
			return typename intern::simple_range<lambda_adaptor<Iter, Functor>, Iter>{
				lambda_adaptor<Iter, Functor>(obj.begin(), std::move(func)),

				// lambda can only be owned by one object, so we return a normal iterator here:
				obj.end()
			};
		}
	};

	template<typename iterator>
	using deref_adaptor = functor_adaptor<iterator, typename intern::deref_functor<iterator_value_t<iterator>>>;
};