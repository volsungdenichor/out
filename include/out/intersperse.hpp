#ifndef OUT_INTERSPERSE_HPP
#define OUT_INTERSPERSE_HPP

#include <out/out_iterator.hpp>

namespace out
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
      if (init)
      {
        yield(next, separator);
      }
      Policy()(next, item);
      init = true;
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(separator, next));
  }
};

template <class S>
intersperse_proxy<S, intersperse_policy> intersperse(const S& separator)
{
  return intersperse_proxy<S, intersperse_policy>(separator);
}

template <class S>
intersperse_proxy<S, join_with_policy> join_with(const S& separator)
{
  return intersperse_proxy<S, join_with_policy>(separator);
}

} // namespace out

#endif // OUT_INTERSPERSE_HPP
