#ifndef OUT_TAKE_WHILE_HPP
#define OUT_TAKE_WHILE_HPP

#include <out/out_iterator.hpp>

namespace out
{

template <class Pred>
struct take_while_proxy
{
  Pred pred;

  take_while_proxy(Pred pred)
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
        if (pred(item))
        {
          yield(next, item);
        }
        else
        {
          done = true;
        }
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
take_while_proxy<Pred> take_while(Pred pred)
{
  return take_while_proxy<Pred>(pred);
}

} // namespace out

#endif // OUT_TAKE_WHILE_HPP

