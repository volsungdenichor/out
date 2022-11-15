#ifndef OUT_FILTER_HPP
#define OUT_FILTER_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

template <class Pred>
struct filter_proxy
{
  Pred pred;

  filter_proxy(Pred pred)
    : pred(pred)
  {
  }

  template <class Next>
  struct impl
  {
    Pred pred;
    Next next;

    impl(Pred pred, Next next)
      : pred(pred)
      , next(next)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      if (pred(item))
      {
        yield(next, item);
      }
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(pred, next));
  }
};

template <class Pred>
filter_proxy<Pred> filter(Pred pred)
{
  return filter_proxy<Pred>(pred);
}

} // namespace detail

using detail::filter;

} // namespace out

#endif // OUT_FILTER_HPP

