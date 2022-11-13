#ifndef OUT_TRANSFORM_MAYBE_HPP
#define OUT_TRANSFORM_MAYBE_HPP

#include <out/out_iterator.hpp>

namespace out
{

template <class Func>
struct transform_maybe_proxy
{
  Func func;

  transform_maybe_proxy(Func func)
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

    template <class Opt>
    void handle(const Opt& opt)
    {
      if (static_cast<bool>(opt))
      {
        yield(next, *opt);
      }
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(func, next));
  }
};

template <class Func>
transform_maybe_proxy<Func> transform_maybe(Func func)
{
  return transform_maybe_proxy<Func>(func);
}

} // namespace out

#endif // OUT_TRANSFORM_MAYBE_HPP

