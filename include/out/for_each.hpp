#ifndef OUT_FOR_EACH_HPP
#define OUT_FOR_EACH_HPP
namespace out
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
  return make_out_iterator(for_each_impl<Func>(func));
}

} // namespace out

#endif // OUT_FOR_EACH_HPP
