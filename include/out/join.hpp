#ifndef OUT_JOIN_HPP
#define OUT_JOIN_HPP

#include <algorithm>
#include <out/out_iterator.hpp>

namespace out
{

template <class T, class Next>
void join_impl(const T& item, Next& next)
{
  std::for_each(item.begin(), item.end(), yield(next));
}

struct join_proxy
{
  template <class Next>
  struct impl
  {
    Next next;

    impl(Next next)
      : next(next)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      join_impl(item, next);
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(next));
  }
};

inline join_proxy join()
{
  return join_proxy();
}

} // namespace out

#endif // OUT_JOIN_HPP
