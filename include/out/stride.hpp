#ifndef OUT_STRIDE_HPP
#define OUT_STRIDE_HPP

#include <out/out_iterator.hpp>

namespace out
{

struct stride_proxy
{
  int count;

  stride_proxy(int count)
    : count(count)
  {
  }

  template <class Next>
  struct impl
  {
    int count;
    int n;
    Next next;

    impl(int count, Next next)
      : count(count)
      , n(0)
      , next(next)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      if (n == 0)
      {
        yield(next, item);
      }
      n = (n + 1) % count;
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(count, next));
  }
};

inline stride_proxy stride(int count)
{
  return stride_proxy(count);
}

} // namespace out

#endif // OUT_STRIDE_HPP

