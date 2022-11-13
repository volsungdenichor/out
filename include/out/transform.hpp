#ifndef OUT_TRANSFORM_HPP
#define OUT_TRANSFORM_HPP

#include <out/out_iterator.hpp>

namespace out
{

template <class Func>
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
      yield(next, func(item));
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(func, next));
  }
};

template <class Func>
transform_proxy<Func> transform(Func func)
{
  return transform_proxy<Func>(func);
}

} // namespace out

#endif // OUT_TRANSFORM_HPP

