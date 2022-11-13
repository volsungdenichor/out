#ifndef OUT_DEV_NULL_HPP
#define OUT_DEV_NULL_HPP

#include <out/out_iterator.hpp>

namespace out
{

struct dev_null_impl
{
  template <class T>
  void operator()(const T&)
  {
  }
};

typedef out_iterator<dev_null_impl> dev_null_t;

inline out_iterator<dev_null_impl> dev_null()
{
  return make_out_iterator(dev_null_impl());
}

} // namespace out

#endif // OUT_DEV_NULL_HPP

