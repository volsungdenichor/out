#ifndef OUT_OUT_ITERATOR_HPP
#define OUT_OUT_ITERATOR_HPP

#include <algorithm>
#include <out/in_iterator.hpp>

namespace out
{

namespace detail
{

template <class It, class T>
void yield(It& it, const T& item)
{
  *it = item;
  ++it;
}

template <class It>
struct yield_t
{
  It& it;

  yield_t(It& it)
    : it(it)
  {
  }

  template <class T>
  void operator()(const T& item)
  {
    yield(it, item);
  }
};

template <class It>
yield_t<It> yield(It& it)
{
  return yield_t<It>(it);
}

template <class It, class R>
void yield_range(It& it, const R& r)
{
  std::for_each(begin(r), end(r), yield(it));
}

template <class First, class Second>
struct chain
{
  First first;
  Second second;

  chain(const First& first, const Second& second)
    : first(first)
    , second(second)
  {
  }

  template <class T>
  void operator()(const T& item) const
  {
    first(item);
    second(item);
  }
};

template <class Impl>
struct out_iterator
{
  typedef std::output_iterator_tag iterator_category;
  typedef void value_type;
  typedef void reference;
  typedef void pointer;
  typedef void difference_type;

  struct proxy
  {
    Impl& impl;

    proxy(Impl& impl)
      : impl(impl)
    {
    }

    template <class T>
    proxy& operator=(const T& item)
    {
      impl(item);
      return *this;
    }
  };

  out_iterator(const Impl& impl)
      : impl(impl)
  {
  }

  out_iterator(const out_iterator& other)
    : impl(other.impl)
  {
  }

  out_iterator& operator=(out_iterator other)
  {
    std::swap(impl, other.impl);
    return *this;
  }

  proxy operator*() { return proxy(impl); }
      
  out_iterator& operator++() { return *this; }
        
  out_iterator operator++(int) { return *this; }

  Impl impl;
};

template <class Impl>
struct proxy_base
{
  Impl impl;

  proxy_base(const Impl& impl)
    : impl(impl)
  {
  }

  template <class Next>
  proxy_base< chain<Impl, Next> > operator >>=(const proxy_base<Next>& next) const
  {
    return proxy_base< chain<Impl, Next> >(chain<Impl, Next>(impl, next.impl));
  }

  template <class Iter>
  out_iterator< typename Impl::template next_iter_info<Iter>::type > operator >>=(Iter iter) const
  {
    return impl.next_iter(iter);
  }
};

} // namespace detail

} // namespace out

#endif // OUT_OUT_ITERATOR_HPP

