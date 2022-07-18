// filter.h - Filter an iterable based on a predicate
#ifndef UMF_FILTER_INCLUDED
#define UMF_FILTER_INCLUDED
#include "concept.h"

namespace umf::iterable {

	template<class P, forward_iterable I>
	class filter {
		P p;
		I i;
		void incr()
		{
			while (i and !p(*i)) {
				++i;
			}
		}
	public:
		using value_type = typename I::value_type;
		using reference = typename I::reference;

		filter(const P& p, const I& i)
			: p(p), i(i)
		{
			incr();
		}
		filter& operator=(const filter& a)
		{
			if (this != &a) {
				i = a.i;
			}

			return *this;
		}
		auto operator==(const filter& a) const
		{
			return i == a.i; // same p object
		}
		explicit operator bool() const
		{
			return !!i;
		}
		filter begin() const
		{
			return *this;
		}
		filter end() const
		{
			return filter(p, i.end());
		}
		value_type operator*() const
		{
			return *i;
		}
		// no reference
		filter& operator++()
		{
			if (operator bool()) {
				++i;
				incr();
			}

			return *this;
		}
		filter operator++(int)
		{
			auto tmp = *this;

			operator++();

			return tmp;
		}
	};

} // namespace umf::iterable

template<umf::iterable::forward_iterable I, class P>
inline auto operator|(const I& i, const P& p)
{
	return umf::iterable::filter(p, i);
}
#define FILTER_OP(op) \
template<umf::iterable::forward_iterable I, typename T = typename I::value_type> \
inline auto operator op(const I& i, T t) \
{ return umf::iterable::filter([t](T s) { return s op t; }, i); }

FILTER_OP(==)
FILTER_OP(!=)
FILTER_OP(< )
FILTER_OP(<=)
FILTER_OP(> )
FILTER_OP(>=)
#undef FILTER_OP

#ifdef _DEBUG
#include <cassert>
#include <functional>
#include "array.h"
#include "iota.h"

inline int test_filter()
{
	using umf::iterable::filter;
	using umf::iterable::array;

	{
		int i[] = { 1,2,3 };
		auto a = filter([](auto i) { return i%2;  }, array(i));
		assert(a);
		auto a2{ a };
		assert(a2 == a);
		a = a2;
		assert(!(a != a2));
		assert(a);
		assert(1 == *a);
		++a;
		assert(a);
		assert(3 == *a);
		++a;
		assert(!a);
		++a;
		assert(!a);
	}
	{
		int i[] = { 1,2,3 };
		auto a = filter([](auto i) { return i % 2;  }, array(i));
		int i2[] = { 1,3 };
		assert(equal(array(i2), a));
	}
	{
		int i[] = { 1,2,3 };
		auto odd = [](auto i) { return i % 2;  };
		int i2[] = { 1,3 };
		assert(equal(array(i2), array(i)|odd));
	}
	{
		int i[] = { 1,2,3 };
		int i2[] = { 1,2 };
		assert(equal(array(i2), array(i) < 3));
		assert(equal(array(i2), array(i) <= 2));
		int i3[] = { 1,3 };
		assert(equal(array(i3), array(i) != 2));
	}

	return 0;
}

#endif // _DEBUG

#endif // UMF_FILTER_INCLUDED
