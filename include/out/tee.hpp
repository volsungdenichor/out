#ifndef OUT_TEE_HPP
#define OUT_TEE_HPP

namespace out
{

namespace detail
{

template <class It>
struct tee_proxy
{
  It it;

  tee_proxy(It it)
    : it(it)
  {
  }

  template <class Next>
  struct impl
  {
    It it;
    Next next;

    impl(It it, Next next)
      : it(it)
      , next(next)
    {
    }

    template <class T>
    void operator()(const T& item)
    {
      yield(it, item);
      yield(next, item);
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
    return impl<Next>(it, next);
  }
};

template <class It>
proxy_base< tee_proxy<It> > tee(It it)
{
  return tee_proxy<It>(it);
}

} // namespace detail

using detail::tee;

} // namespace out

#endif // OUT_TEE_HPP
