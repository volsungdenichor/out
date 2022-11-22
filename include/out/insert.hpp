#ifndef OUT_INSERT_HPP
#define OUT_INSERT_HPP

#include <out/in_iterator.hpp>

namespace out
{

namespace detail
{

template <class Iter>
struct overwrite_impl
{
  Iter b, e;

  overwrite_impl(Iter b, Iter e)
    : b(b)
    , e(e)
  {
  }

  template <class T>
  void operator()(const T& item)
  {
    if (b != e)
    {
      yield(b, item);
    }
  }
};

template <class Container>
std::insert_iterator<Container> insert(Container& container, typename Container::iterator iter)
{
  return std::inserter(container, iter);
}

template <class Container>
std::insert_iterator<Container> insert(Container& container)
{
  return insert(container, end(container));
}

template <class Container>
std::insert_iterator<Container> push_front(Container& container)
{
  return insert(container, begin(container));
}

template <class Container>
std::back_insert_iterator<Container> push_back(Container& container)
{
  return std::back_inserter(container);
}

template <class Container>
out_iterator< overwrite_impl<typename Container::iterator> > overwrite(Container& container)
{
  return overwrite_impl<typename Container::iterator>(begin(container), end(container));
}

} // namespace detail

using detail::push_back;
using detail::push_front;
using detail::insert;
using detail::overwrite;

} // namespace out

#endif // OUT_INSERT_HPP
