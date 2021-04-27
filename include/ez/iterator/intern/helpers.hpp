#pragma once
#include <ez/meta.hpp>
#include <cinttypes>
#include <type_traits>

namespace ez {
	namespace intern {
		template<typename pointer>
		struct deref_functor {
			deref_functor() noexcept = default;
			~deref_functor() noexcept = default;
			deref_functor(deref_functor&&) noexcept = default;
			deref_functor& operator=(deref_functor&&) noexcept = default;

			using reference = decltype(*std::declval<pointer>());

			reference operator()(pointer obj) const {
				return *obj;
			}
		};

		// Simple range type, just takes two (possibly differently typed) iterators and returns then as begin and end.
		template<typename Iter0, typename Iter1 = Iter0>
		struct simple_range {
			static_assert(ez::is_iterator_v<Iter0>, "ez::intern::simple_range requires an iterator type!");
			static_assert(ez::is_iterator_v<Iter1>, "ez::intern::simple_range requires an iterator type!");
			static_assert(std::is_nothrow_copy_constructible_v<Iter0>, "ez::intern::simple_range requires a nothrow copy constructible iterator type!");
			static_assert(std::is_nothrow_copy_constructible_v<Iter1>, "ez::intern::simple_range requires a nothrow copy constructible iterator type!");

			template<typename = std::enable_if_t<std::is_nothrow_move_constructible_v<Iter0> && std::is_nothrow_move_constructible_v<Iter1>>>
			simple_range(Iter0&& _first, Iter1&& _last) noexcept
				: first(std::move(_first))
				, last(std::move(_last))
			{};

			constexpr simple_range(const Iter0& _first, const Iter1& _last) noexcept
				: first(_first)
				, last(_last)
			{};

			Iter0 first;
			Iter1 last;

			Iter0 begin() noexcept {
				return first;
			}
			Iter1 end() noexcept {
				return last;
			}
		};
	};
};