#ifndef OUT_FOR_EACH_HPP
#define OUT_FOR_EACH_HPP

namespace out
{

namespace detail
{

template <class Func>
struct for_each_impl
{
  Func func;

  for_each_impl(Func func)
    : func(func)
  {
  }

  template <class T>
  void operator()(const T& item)
  {
    func(item);
  }
};

template <class Func>
out_iterator< for_each_impl<Func> > for_each(Func func)
{
  return for_each_impl<Func>(func);
}

} // namespace detail

using detail::for_each;

} // namespace out

#endif // OUT_FOR_EACH_HPP
