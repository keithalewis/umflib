// buffer.h - buffer an iterable
#ifndef UMF_BUFFER_INCLUDED
#define UMF_BUFFER_INCLUDED
#include <stdexcept>
#include "concept.h"
#include "counted.h"

namespace umf::iterable {

	template<forward_iterable I>
	class buffer {
		size_t n;
		I i;
		counted<I> in;
	public:
		using value_type = typename counted<I>;
		using reference = typename counted<I>&;

		buffer()
		{ }
		buffer(size_t _n, const I& _i)
			: n(_n), i(_i), in(counted(_n, _i))
		{
			if (n != length(in)) {
				const char* err = __FUNCTION__ /*": too few elements to buffer"*/;
				throw std::runtime_error(err);
			}
		}
		auto operator<=>(const buffer&) const = default;
		explicit operator bool() const
		{
			return n == length(in);
		}
		buffer begin() const
		{
			return *this;
		}
		buffer end() const
		{
			return last(*this);
		}
		value_type operator*() const
		{
			return in;
		}
		reference operator*()
		{
			return in;
		}
		buffer& operator++()
		{
			if (operator bool()) {
				in = take(n, ++i);
			}

			return *this;
		}
		buffer operator++(int)
		{
			auto tmp = *this;

			operator++();

			return tmp;
		}
	};

} // namespace umf::iterable

#ifdef _DEBUG
#include <cassert>
#include <numeric>
#include "iota.h"

inline int test_buffer()
{
	using umf::iterable::buffer;
	using umf::iterable::iota;

	{
		auto i = iota<int>{};
		auto ii = buffer(2, i);
		assert(ii);
		auto ii2{ ii };
		assert(ii == ii2);
		ii2 = ii;
		assert(!(ii2 != ii));
		for (int i = 0; i < 3; ++i) {
			assert(equal(take(2, iota<int>(i)), *ii));
			++ii;
		}
	}

	return 0;
}

#endif // _DEBUG

#endif // UMF_BUFFER_INCLUDED

