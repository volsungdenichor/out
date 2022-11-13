#ifndef OUT_OUT_ITERATOR_HPP
#define OUT_OUT_ITERATOR_HPP

#include <iterator>

namespace out
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
out_iterator<Impl> make_out_iterator(Impl impl)
{
  return out_iterator<Impl>(impl);
}

} // namespace out

#endif // OUT_OUT_ITERATOR_HPP

