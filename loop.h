// loop.h - recycle an iterable
#ifndef UMF_LOOP_INCLUDED
#define UMF_LOOP_INCLUDED
#include "concept.h"

namespace umf::iterable {

	template<forward_iterable I>
	class loop {
		I b, i;
	public:
		using value_type = typename I::value_type;
		using reference = typename I::reference;

		loop(const I& i)
			: b(i), i(i)
		{ }
		bool operator==(const loop& a) const
		{
			return b == a.b and i == a.i;
		}
		explicit operator bool() const
		{
			return true;
		}
		loop begin() const
		{
			return *this;
		}
		loop end() const
		{
			return *this;
		}
		value_type operator*() const
		{
			return *i;
		}
		loop& operator++()
		{
			++i;
			if (!i) {
				i = b;
			}

			return *this;
		}
		loop operator++(int)
		{
			auto tmp = *this;

			operator++();

			return tmp;
		}
	};

} // namespace umf::iterable

#ifdef _DEBUG
#include <cassert>
#include "array.h"
#include "iota.h"

inline int test_loop()
{
	using umf::iterable::loop;
	using umf::iterable::array;
	using umf::iterable::iota;

	{
		int i[] = { 1,2,3 };
		auto a = loop(array(i));
		auto a2{ a };
		assert(a2 == a);
		a = a2;
		assert(!(a != a2));

		assert(1 == *a++);
		assert(2 == *a++);
		assert(3 == *a++);
		assert(1 == *a++);
	}

	return 0;
}

#endif // _DEBUG

#endif // UMF_LOOP_INCLUDED
