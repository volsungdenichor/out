#ifndef OUT_DROP_WHILE_HPP
#define OUT_DROP_WHILE_HPP

#include <out/out_iterator.hpp>

namespace out
{

template <class Pred>
struct drop_while_proxy
{
  Pred pred;

  drop_while_proxy(Pred pred)
    : pred(pred)
  {
  }

  template <class Next>
  struct impl
  {
    Pred pred;
    Next next;
    bool done;

    impl(Pred pred, Next next)
      : pred(pred)
      , next(next)
      , done(false)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      if (!done)
      {
        done = !pred(item);
      }
      if (done)
      {
        yield(next, item);
      }
    }
  };

  template <class Next>
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(pred, next));
  }
};

template <class Pred>
drop_while_proxy<Pred> drop_while(Pred pred)
{
  return drop_while_proxy<Pred>(pred);
}

} // namespace out

#endif // OUT_DROP_WHILE_HPP

