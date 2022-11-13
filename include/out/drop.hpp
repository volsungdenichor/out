#ifndef OUT_DROP_HPP
#define OUT_DROP_HPP

#include <out/out_iterator.hpp>

namespace out
{

struct drop_proxy
{
  int count;

  drop_proxy(int count)
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
      if (n == count)
      {
        yield(next, item);
      }
      else
      {
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

inline drop_proxy drop(int count)
{
  return drop_proxy(count);
}

} // namespace out

#endif // OUT_DROP_HPP

