// epsilon.h - truncate iterables
#ifndef UMF_EPSILON_INCLUDED
#define UMF_EPSILON_INCLUDED
#include <limits>
#include "concept.h"
#include "counted.h"
#include "iota.h"

namespace umf::iterable {

	// stop when abs(*i) <= epsilon n times in a row
	template<forward_iterable I, class T = typename I::value_type>
	class epsilon {
		I i;
		T eps;
		std::size_t n;
		std::size_t _n; // current number of consecutive epsilons
	public:
		using value_type = T;
		using reference = T&;

		epsilon(const I& i, T eps = std::numeric_limits<T>::epsilon(), std::size_t n = 1)
			: i(i), eps(eps), n(n), _n(0)
		{ }
		auto operator<=>(const epsilon&) const = default;
		explicit operator bool() const
		{
			return _n != n or std::abs(*i) > eps;
		}
		epsilon begin() const
		{
			return *this;
		}
		epsilon end() const
		{
			auto e = *this;

			while (e) {
				++e;
			}

			return e;
		}
		value_type operator*() const
		{
			return *i;
		}
		reference operator*()
		{
			return *i;
		}
		epsilon& operator++()
		{
			if (*i > eps) {
				++i;
				_n = 0;
			}
			else {
				++_n;
				if (_n < n) {
					++i;
				}
			}

			return *this;
		}
		epsilon operator++(int)
		{
			auto tmp = *this;

			operator++();

			return tmp;
		}
	};

} // umf::iterable

#ifdef _DEBUG
#include <cassert>
#include "array.h"
#include "counted.h"
#include "iota.h"

inline int test_epsilon()
{
	using umf::iterable::array;
	using umf::iterable::epsilon;
	using umf::iterable::power;

	{
		int i[] = { 1,1,0 };
		auto e = epsilon(array(i), 0);
		assert(e);
		++e;
		assert(e);
		++e;
//		assert(!e);

	}

	return 0;
}

#endif // _DEBUG

#endif // UMF_EPSILON_INCLUDED
