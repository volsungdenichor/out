#ifndef OUT_TAP_HPP
#define OUT_TAP_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

template <class Func>
struct tap_proxy
{
  Func func;

  tap_proxy(Func func)
    : func(func)
  {
  }

  template <class Next>
  struct impl
  {
    Func func;
    Next next;

    impl(Func func, Next next)
      : func(func)
      , next(next)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      func(item);
      yield(next, item);
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
    return impl<Next>(func, next);
  }
};

template <class Func>
proxy_base< tap_proxy<Func> > tap(Func func)
{
  return tap_proxy<Func>(func);
}

} // namespace detail

using detail::tap;

} // namespace out

#endif // OUT_TAP_HPP
