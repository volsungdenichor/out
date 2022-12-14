#ifndef OUT_INTERSPERSE_HPP
#define OUT_INTERSPERSE_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

struct intersperse_policy
{
  template <class Next, class T>
  void operator()(Next& next, const T& item) const
  {
    yield(next, item);
  }
};

struct join_with_policy
{
  template <class Next, class T>
  void operator()(Next& next, const T& item) const
  {
    yield_range(next, item);
  }
};

template <class S, class Policy>
struct intersperse_proxy
{
  S separator;

  intersperse_proxy(const S& separator)
    : separator(separator)
  {
  }

  template <class Next>
  struct impl
  {
    S separator;
    Next next;
    bool init;

    impl(const S& separator, Next next)
      : separator(separator)
      , next(next)
      , init(false)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      static const Policy policy = Policy();
      if (init)
      {
        yield(next, separator);
      }
      policy(next, item);
      init = true;
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
    return impl<Next>(separator, next);
  }
};

template <class S>
proxy_base< intersperse_proxy<S, intersperse_policy> > intersperse(const S& separator)
{
  return intersperse_proxy<S, intersperse_policy>(separator);
}

template <class S>
proxy_base< intersperse_proxy<S, join_with_policy> > join_with(const S& separator)
{
  return intersperse_proxy<S, join_with_policy>(separator);
}

} // namespace detail

using detail::intersperse;
using detail::join_with;

} // namespace out

#endif // OUT_INTERSPERSE_HPP
