#ifndef OUT_DEV_NULL_HPP
#define OUT_DEV_NULL_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

struct dev_null_impl
{
  template <class T>
  void operator()(const T&)
  {
  }
};

typedef out_iterator< dev_null_impl > dev_null_t;

inline out_iterator< dev_null_impl > dev_null()
{
  return dev_null_impl();
}

} // namespace detail

using detail::dev_null;

} // namespace out

#endif // OUT_DEV_NULL_HPP

