#ifndef OUT_FORK_HPP
#define OUT_FORK_HPP

#include <out/dev_null.hpp>

namespace out
{

template <class Next0, class Next1, class Next2, class Next3>
struct fork_impl
{
  Next0 next0;
  Next1 next1;
  Next2 next2;
  Next3 next3;

  fork_impl(Next0 next0, Next1 next1, Next2 next2, Next3 next3)
    : next0(next0)
    , next1(next1)
    , next2(next2)
    , next3(next3)
  {
  }

  template <class T>
  void operator()(const T& item)
  {
    yield(next0, item);
    yield(next1, item);
    yield(next2, item);
    yield(next3, item);
  }
};

template <class Next0, class Next1, class Next2, class Next3>
out_iterator< fork_impl<Next0, Next1, Next2, Next3> > fork(Next0 next0, Next1 next1, Next2 next2, Next3 next3)
{
  return make_out_iterator(fork_impl<Next0, Next1, Next2, Next3>(next0, next1, next2, next3));
}

template <class Next0, class Next1, class Next2>
out_iterator< fork_impl<Next0, Next1, Next2, dev_null_t> > fork(Next0 next0, Next1 next1, Next2 next2)
{
  return fork(next0, next1, next2, dev_null());
}

template <class Next0, class Next1>
out_iterator< fork_impl<Next0, Next1, dev_null_t, dev_null_t> > fork(Next0 next0, Next1 next1)
{
  return fork(next0, next1, dev_null());
}

template <class Next0>
out_iterator< fork_impl<Next0, dev_null_t, dev_null_t, dev_null_t> > fork(Next0 next0)
{
  return fork(next0, dev_null());
}

} // namespace out

#endif // OUT_FORK_HPP
