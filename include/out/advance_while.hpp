#ifndef OUT_ADVANCE_WHILE_HPP
#define OUT_ADVANCE_WHILE_HPP

#include <out/out_iterator.hpp>

namespace out
{

namespace detail
{

struct take_while_policy
{
  template <class Pred, class Next, class T>
  void operator()(Pred& pred, bool& done, Next& next, const T& item) const
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

struct drop_while_policy
{
  template <class Pred, class Next, class T>
  void operator()(Pred& pred, bool& done, Next& next, const T& item) const
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

template <class Pred, class Policy>
struct advance_while_proxy
{
  Pred pred;

  advance_while_proxy(Pred pred)
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
      static const Policy policy = Policy(); 
      policy(pred, done, next, item);
    }
  };

  template <class Next>
  struct next_iter_info
  {
    typedef impl<Next> type;
  };

  template <class Next>
  typename next_iter_info<Next>::type next_iter(Next next) const
  {
    return impl<Next>(pred, next);
  }
};

template <class Pred>
proxy_base< advance_while_proxy<Pred, take_while_policy> > take_while(Pred pred)
{
  return advance_while_proxy<Pred, take_while_policy>(pred);
}

template <class Pred>
proxy_base< advance_while_proxy<Pred, drop_while_policy> > drop_while(Pred pred)
{
  return advance_while_proxy<Pred, drop_while_policy>(pred);
}

} // namespace detail

using detail::take_while;
using detail::drop_while;

} // namespace out

#endif // OUT_ADVANCE_WHILE_HPP

