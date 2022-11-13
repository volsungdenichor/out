#ifndef OUT_TAKE_HPP
#define OUT_TAKE_HPP

#include <out/out_iterator.hpp>

namespace out
{

struct take_proxy
{
  int count;

  take_proxy(int count)
    : count(count)
  {
  }

  template <class Next>
  struct impl
  {
    int count;
    int n;
    Next next;

    impl(int count, Next next)
      : count(count)
      , n(0)
      , next(next)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      if (n < count)
      {
        yield(next, item);
        ++n;
      }
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(count, next));
  }
};

inline take_proxy take(int count)
{
  return take_proxy(count);
}

} // namespace out

#endif // OUT_TAKE_HPP

