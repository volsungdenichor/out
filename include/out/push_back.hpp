#ifndef OUT_PUSH_BACK_HPP
#define OUT_PUSH_BACK_HPP

namespace out
{

template <class Container>
struct push_back_impl
{
  Container* container;

  push_back_impl(Container& container)
    : container(&container)
  {
  }

  template <class T>
  void operator()(const T& item) const
  {
    container->push_back(item);
  }
};

template <class Container>
out_iterator< push_back_impl<Container> > push_back(Container& container)
{
  return make_out_iterator(push_back_impl<Container>(container));
}

} // namespace out
#endif // OUT_PUSH_BACK_HPP
