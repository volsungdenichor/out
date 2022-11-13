#ifndef OUT_TAP_HPP
#define OUT_TAP_HPP

#include <out/out_iterator.hpp>

namespace out
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
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(func, next));
  }
};

template <class Func>
tap_proxy<Func> tap(Func func)
{
  return tap_proxy<Func>(func);
}

} // namespace out

#endif // OUT_TAP_HPP
