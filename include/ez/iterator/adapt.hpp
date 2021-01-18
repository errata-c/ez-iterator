#pragma once
#include <cinttypes>
#include <type_traits>
#include <iterator>

namespace ez {
	namespace intern {
		template<typename T, typename = int>
		struct is_iterator : std::false_type {};

		template<typename T>
		struct is_iterator < T, decltype((typename T::iterator_category*)0, 0) > : std::true_type {};

		template<typename Iter>
		struct simple_range {
			Iter first, last;

			Iter begin() {
				return first;
			}
			Iter end() {
				return last;
			}
		};

		template<typename T>
		struct Deref {
			T& operator()(T* obj) {
				return *obj;
			}
			const T& operator()(const T* obj) const {
				return *obj;
			}
		};
	}

	template<typename Iter, std::ptrdiff_t Inc>
	struct offset_adaptor: public Iter {
		static_assert(std::is_same_v<typename Iter::iterator_category, std::random_access_iterator_tag>, "Offset adaptor requires that the Iterator type passed in be a random access iterator!");

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
		functor_adaptor(const Iter& source)
			: Iter(source)
		{}

		using iterator_category = typename Iter::iterator_category;
		
		using reference = decltype(Functor{}(*reinterpret_cast<typename Iter::pointer>(0)));
		using pointer = std::remove_reference_t<reference>*;
		using value_type = std::remove_reference_t<reference>;
		
		using difference_type = std::ptrdiff_t;

		reference operator*() {
			return Functor{}(Iter::operator*());
		}
		pointer operator->() {
			return Functor{}(Iter::operator->());
		}
	};

	template<typename Iter, typename Functor, typename T = decltype((*reinterpret_cast<Functor*>(0))(*Iter{})) >
	class lambda_adaptor : public Iter {
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
		if constexpr (intern::is_iterator<T>::value) {
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
		if constexpr (intern::is_iterator<T>::value) {
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
};