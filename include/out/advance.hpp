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
  struct next_iter_info
  {
    typedef impl<Next> type;
  };

  template <class Next>
  typename next_iter_info<Next>::type next_iter(Next next) const
  {
    return impl<Next>(count, next);
  }
};

inline proxy_base< advance_proxy<drop_policy> > drop(int count)
{
  return advance_proxy<drop_policy>(count);
}

inline proxy_base< advance_proxy<take_policy> > take(int count)
{
  return advance_proxy<take_policy>(count);
}

inline proxy_base< advance_proxy<stride_policy> > stride(int count)
{
  return advance_proxy<stride_policy>(count);
}

inline proxy_base< chain_proxy< advance_proxy<drop_policy>, advance_proxy<take_policy> > >  sub(int begin, int count)
{
  return drop(begin) >>= take(count);
}

inline proxy_base< chain_proxy< advance_proxy<drop_policy>, advance_proxy<take_policy> > >  slice(int begin, int end)
{
  return drop(begin) >>= take(std::max(0, end - begin));
}

} // namespace detail

using detail::drop;
using detail::slice;
using detail::stride;
using detail::sub;
using detail::take;

} // namespace out

#endif // OUT_ADVANCE_HPP

