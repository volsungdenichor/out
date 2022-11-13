#ifndef OUT_UTILS_HPP
#define OUT_UTILS_HPP

namespace out
{

template <class Type, class T>
struct mem_fn_t
{
  Type T::*ptr;

  mem_fn_t(Type T::*ptr)
    : ptr(ptr)
  {
  }

  const Type& operator()(const T& item) const
  {
    return item.*ptr;
  }
  
  Type& operator()(T& item) const
  {
    return item.*ptr;
  }
};

template <class Type, class T>
mem_fn_t<Type, T> mem_fn(Type T::*ptr)
{
  return mem_fn_t<Type, T>(ptr);
}

} // namespace out

#endif // OUT_UTILS_HPP
