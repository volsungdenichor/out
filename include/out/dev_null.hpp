#ifndef OUT_DEV_NULL_HPP
#define OUT_DEV_NULL_HPP

#include <out/out_iterator.hpp>
#include <out/functions.hpp>

namespace out
{

namespace detail
{

typedef out_iterator< do_nothing > dev_null_t;

inline out_iterator< do_nothing > dev_null()
{
  return do_nothing();
}

} // namespace detail

using detail::dev_null;

} // namespace out

#endif // OUT_DEV_NULL_HPP

