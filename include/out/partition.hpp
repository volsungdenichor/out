#ifndef OUT_PARTITION_HPP
#define OUT_PARTITION_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

template <class Pred, class OnTrue, class OnFalse>
struct partition_impl
{
  Pred pred;
  OnTrue on_true;
  OnFalse on_false;

  partition_impl(Pred pred, OnTrue on_true, OnFalse on_false)
    : pred(pred)
    , on_true(on_true)
    , on_false(on_false)
  {
  }

  template <class T>
  void operator()(const T& item)
  {
    if (pred(item))
    {
      yield(on_true, item);
    }
    else
    {
      yield(on_false, item);
    }
  }
};

template <class Pred, class OnTrue, class OnFalse>
out_iterator< partition_impl<Pred, OnTrue, OnFalse> > partition(Pred pred, OnTrue on_true, OnFalse on_false)
{
  return make_out_iterator(partition_impl<Pred, OnTrue, OnFalse>(pred, on_true, on_false));
}

} // namespace detail

using detail::partition;

} // namespace out

#endif // OUT_PARTITION_HPP
