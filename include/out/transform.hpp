#ifndef OUT_TRANSFORM_HPP
#define OUT_TRANSFORM_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

struct transform_policy
{
  template <class Next, class R>
  void operator()(Next& next, const R& result) const
  {
    yield(next, result);
  }
};

struct transform_maybe_policy
{
  template <class Next, class R>
  void operator()(Next& next, const R& result) const
  {
    if (static_cast<bool>(result))
    {
      yield(next, *result);
    }
  }
};

struct transform_join_policy
{
  template <class Next, class R>
  void operator()(Next& next, const R& result) const
  {
    yield_range(next, result);
  }
};

template <class Func, class Policy>
struct transform_proxy
{
  Func func;

  transform_proxy(Func func)
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
      handle(func(item));
    }

    template <class R>
    void handle(const R& result)
    {
      Policy()(next, result);
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
proxy_base< transform_proxy<Func, transform_policy> > transform(Func func)
{
  return transform_proxy<Func, transform_policy>(func);
}

template <class Func>
proxy_base< transform_proxy<Func, transform_join_policy> > transform_join(Func func)
{
  return transform_proxy<Func, transform_join_policy>(func);
}

template <class Func>
proxy_base< transform_proxy<Func, transform_maybe_policy> > transform_maybe(Func func)
{
  return transform_proxy<Func, transform_maybe_policy>(func);
}

} // namespace detail

using detail::transform;
using detail::transform_join;
using detail::transform_maybe;

} // namespace out

#endif // OUT_TRANSFORM_HPP

