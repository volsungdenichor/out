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
  out_iterator< impl<Next> > operator >>=(Next next) const
  {
    return make_out_iterator(impl<Next>(it, next));
  }
};

template <class It>
tee_proxy<It> tee(It it)
{
  return tee_proxy<It>(it);
}

} // namespace detail

using detail::tee;

} // namespace out

#endif // OUT_TEE_HPP
