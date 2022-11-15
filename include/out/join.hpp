#ifndef OUT_JOIN_HPP
#define OUT_JOIN_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

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
      yield_range(next, item);
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

} // namespace detail

using detail::join;

} // namespace out

#endif // OUT_JOIN_HPP
