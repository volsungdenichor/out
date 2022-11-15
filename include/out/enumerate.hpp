#ifndef OUT_ENUMERATE_HPP
#define OUT_ENUMERATE_HPP

#include <ostream>
#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

template <class T>
struct index_value
{
  T value;
  int index;

  index_value(T value, int index)
    : value(value)
    , index(index)
  {
  }

  template <class U>
  bool operator==(const index_value<U>& other) const
  {
    return index == other.index && value == other.value;
  }

  template <class U>
  bool operator!=(const index_value<U>& other) const
  {
    return !(*this == other);
  }

  friend std::ostream& operator<<(std::ostream& os, const index_value& item)
  {
    return os << "[" << item.index << "] " << item.value;
  }
};

template <class T>
index_value<const T&> make_index_value(const T& value, int index)
{
  return index_value<const T&>(value, index);
}

struct enumerate_proxy
{
  int start;

  enumerate_proxy(int start)
    : start(start)
  {
  }

  template <class Next>
  struct impl
  {
    int start;
    Next next;

    impl(int start, Next next)
      : start(start)
      , next(next)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      yield(next, make_index_value(item, start++));
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(start, next));
  }
};

inline enumerate_proxy enumerate(int start = 0)
{
  return enumerate_proxy(start);
}

} // namespace detail

using detail::index_value;
using detail::enumerate;

} // namespace out

#endif // OUT_ENUMERATE_HPP
