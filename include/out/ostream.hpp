#ifndef OUT_OSTREAM_HPP
#define OUT_OSTREAM_HPP

#include <iostream>
#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

struct ostream_impl
{
  std::ostream* os;
  const char* separator;

  ostream_impl(std::ostream& os, const char* separator)
    : os(&os)
    , separator(separator)
  {
  }

  template <class T>
  void operator()(const T& item) const
  {
    *os << item << separator;
  }
};

inline out_iterator<ostream_impl> ostream(std::ostream& os, const char* separator)
{
  return ostream_impl(os, separator);
}

inline out_iterator<ostream_impl> cout(const char* separator)
{
  return ostream(std::cout, separator);
}

} // namespace detail

using detail::ostream;
using detail::cout;

} // namespace out

#endif // OUT_OSTREAM_HPP
