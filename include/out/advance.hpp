#ifndef OUT_ADVANCE_HPP
#define OUT_ADVANCE_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

struct take_policy
{
  template <class Next, class T>
  void operator()(int count, int& n, Next& next, const T& item) const
  {
    if (n < count)
    {
      yield(next, item);
      ++n;
    }
  }
};

struct drop_policy
{
  template <class Next, class T>
  void operator()(int count, int& n, Next& next, const T& item) const
  {
    if (n == count)
    {
      yield(next, item);
    }
    else
    {
      ++n;
    }
  }
};

struct stride_policy
{
  template <class Next, class T>
  void operator()(int count, int& n, Next& next, const T& item) const
  {
    if (n == 0)
    {
      yield(next, item);
    }
    n = (n + 1) % count;
  }
};

template <class Policy>
struct advance_proxy
{
  int count;

  advance_proxy(int count)
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
      Policy()(count, n, next, item);
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(count, next));
  }
};

inline advance_proxy<drop_policy> drop(int count)
{
  return advance_proxy<drop_policy>(count);
}

inline advance_proxy<take_policy> take(int count)
{
  return advance_proxy<take_policy>(count);
}

inline advance_proxy<stride_policy> stride(int count)
{
  return advance_proxy<stride_policy>(count);
}

} // namespace detail

using detail::drop;
using detail::take;
using detail::stride;

} // namespace out

#endif // OUT_ADVANCE_HPP

