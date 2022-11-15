#ifndef OUT_PUSH_BACK_HPP
#define OUT_PUSH_BACK_HPP

#include <iterator>

namespace out
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
  return insert(container, container.end());
}

} // namespace out

#endif // OUT_PUSH_BACK_HPP
