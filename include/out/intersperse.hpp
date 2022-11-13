#ifndef OUT_INTERSPERSE_HPP
#define OUT_INTERSPERSE_HPP

#include <out/out_iterator.hpp>

namespace out
{

template <class S>
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
      yield(next, item);
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
intersperse_proxy<S> intersperse(const S& separator)
{
  return intersperse_proxy<S>(separator);
}

} // namespace out

#endif // OUT_INTERSPERSE_HPP
