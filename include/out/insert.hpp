#ifndef OUT_INSERT_HPP
#define OUT_INSERT_HPP

#include <out/in_iterator.hpp>

namespace out
{

namespace detail
{

template <class Container>
std::back_insert_iterator<Container> push_back(Container& container)
{
  return std::back_inserter(container);
}

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

} // namespace detail

using detail::push_back;
using detail::insert;

} // namespace out

#endif // OUT_INSERT_HPP
