#ifndef OUT_IN_ITERATOR_HPP
#define OUT_IN_ITERATOR_HPP

#include <utility>
#include <iterator>
#include <algorithm>

namespace out
{

namespace detail
{

template <class R>
struct range_traits;

template <class R>
struct range_traits
{
  typedef typename R::iterator iter;

  static iter begin(R& r) { return r.begin(); }
  static iter end(R& r) { return r.end(); }
};

template <class R>
struct range_traits<const R>
{
  typedef typename R::const_iterator iter;

  static iter begin(const R& r) { return r.begin(); }
  static iter end(const R& r) { return r.end(); }
};

template <class T, std::size_t N>
struct range_traits<T[N]>
{
  typedef T* iter;

  static iter begin(T(&a)[N]) { return a; }
  static iter end(T(&a)[N]) { return begin(a) + N; }
};

template <class T, std::size_t N>
struct range_traits<const T[N]>
{
  typedef const T* iter;

  static iter begin(const T(&a)[N]) { return a; }
  static iter end(const T(&a)[N]) { return begin(a) + N; }
};

template <class Iter>
struct range_traits< std::pair<Iter, Iter> >
{
  typedef Iter iter;

  static iter begin(const std::pair<Iter, Iter>& p) { return p.first; }
  static iter end(const std::pair<Iter, Iter>& p) { return p.second; }
};

template <class Iter>
struct range_traits< const std::pair<Iter, Iter> >
{
  typedef Iter iter;

  static iter begin(const std::pair<Iter, Iter>& p) { return p.first; }
  static iter end(const std::pair<Iter, Iter>& p) { return p.second; }
};

} // namespace detail

template <class R>
typename detail::range_traits<R>::iter begin(R& r)
{
  return detail::range_traits<R>::begin(r);
}

template <class R>
typename detail::range_traits<R>::iter end(R& r)
{
  return detail::range_traits<R>::end(r);
}

template <class Iter>
class range
{
private:
  typedef std::iterator_traits<Iter> _traits;
public:
  typedef Iter iterator;
  typedef iterator const_iterator;
  typedef typename _traits::value_type value_type;
  typedef typename _traits::reference reference;
  typedef typename _traits::iterator_category iterator_category;
  typedef typename _traits::difference_type difference_type;

  range()
    : b()
    , e()
  {
  }

  range(iterator b, iterator e)
    : b(b)
    , e(e)
  {
  }

  range(const range& other)
    : b(other.b)
    , e(other.e)
  {
  }

  template <class R>
  range(const R& r)
    : b(::out::begin(r))
    , e(::out::end(r))
  {
  }
  
  template <class R>
  range(R& r)
    : b(::out::begin(r))
    , e(::out::end(r))
  {
  }

  range& operator=(range other)
  {
    std::swap(b, other.b);
    std::swap(e, other.e);
    return *this;
  }

  iterator begin() const { return b; }
  iterator end() const { return e; }

private:
  iterator b, e;
};

template <class Iter>
range<Iter> make_range(Iter b, Iter e)
{
  return range<Iter>(b, e);
}

template <class R>
range< typename detail::range_traits<const R>::iter > make_range(const R& r)
{
  return make_range(begin(r), end(r));
}

template <class R>
range< typename detail::range_traits<R>::iter > make_range(R& r)
{
  return make_range(begin(r), end(r));
}

template <class Iter, class Output>
void operator >>=(range<Iter> r, Output output)
{
  std::copy(r.begin(), r.end(), output);
}

} // namespace out

#endif // OUT_IN_ITERATOR_HPP
