#ifndef OUT_FUNCTIONS_HPP
#define OUT_FUNCTIONS_HPP

#include <stdexcept>
#include <functional>

namespace out
{

struct null_ptr_error : public std::runtime_error
{
  null_ptr_error()
    : std::runtime_error("null pointer")
  {
  }
};

struct identity
{
  template <class T>
  T& operator()(T& item) const
  {
    return item;
  }
};

struct has_value
{
  template <class T>
  bool operator()(T* ptr) const
  {
    return static_cast<bool>(ptr);
  }
};

struct dereference
{
  template <class T>
  T& operator()(T* ptr) const
  {
    if (!ptr)
    {
      throw null_ptr_error();
    }
    return *ptr;
  }
};

struct addressof
{
  template <class T>
  T* operator()(T& item) const
  {
    return &item;
  }
};

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

#define OUT_DEFINE_OPERATOR(name)             \
template <class T>                            \
std::binder2nd< std::name<T> > name(T rhs)    \
{                                             \
  return std::bind2nd(std::name<T>(), rhs);   \
}                                             \

OUT_DEFINE_OPERATOR(plus)
OUT_DEFINE_OPERATOR(minus)
OUT_DEFINE_OPERATOR(multiplies)
OUT_DEFINE_OPERATOR(divides)
OUT_DEFINE_OPERATOR(modulus)

OUT_DEFINE_OPERATOR(equal_to)
OUT_DEFINE_OPERATOR(not_equal_to)
OUT_DEFINE_OPERATOR(less)
OUT_DEFINE_OPERATOR(less_equal)
OUT_DEFINE_OPERATOR(greater)
OUT_DEFINE_OPERATOR(greater_equal)

#undef OUT_DEFINE_OPERATOR

struct negate
{
  template <class T>
  T operator()(T v) const
  {
    return -v;
  }
};

using std::mem_fun_ref;

template <class Pred>
struct not_fn_t
{
  Pred pred;

  not_fn_t(Pred pred)
    : pred(pred)
  {
  }

  template <class T>
  bool operator()(const T& item) const
  {
    return !pred(item);
  }
};

template <class Pred>
not_fn_t<Pred> not_fn(Pred pred)
{
  return not_fn_t<Pred>(pred);
}

struct do_nothing
{
  template <class T>
  void operator()(const T&) const
  {
  }
};

} // namespace out

#endif // OUT_FUNCTIONS_HPP
