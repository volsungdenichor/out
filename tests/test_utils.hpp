#ifndef OUT_TEST_UTILS
#define OUT_TEST_UTILS

template <class T>
std::vector<T> concat(std::vector<T> lhs, const std::vector<T>& rhs)
{
  lhs.insert(lhs.end(), rhs.begin(), rhs.end());
  return lhs;
}

template <class T>
std::vector<T> append(std::vector<T> v, const T& item)
{
  v.push_back(item);
  return v;
}

template <class T>
std::vector<T> vec(T v0)
{
  std::vector<T> res;
  res.push_back(v0);
  return res;
}

template <class T>
std::vector<T> vec(T v0, T v1)
{
  return append(vec(v0), v1);
}

template <class T>
std::vector<T> vec(T v0, T v1, T v2)
{
  return append(vec(v0, v1), v2);
}

template <class T>
std::vector<T> vec(T v0, T v1, T v2, T v3)
{
  return append(vec(v0, v1, v2), v3);
}

template <class T>
std::vector<T> vec(T v0, T v1, T v2, T v3, T v4)
{
  return append(vec(v0, v1, v2, v3), v4);
}

template <class T>
std::vector<T> vec(T v0, T v1, T v2, T v3, T v4, T v5)
{
  return append(vec(v0, v1, v2, v3, v4), v5);
}

template <class T>
std::vector<T> vec(T v0, T v1, T v2, T v3, T v4, T v5, T v6)
{
  return append(vec(v0, v1, v2, v3, v4, v5), v6);
}

template <class T>
std::vector<T> vec(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7)
{
  return append(vec(v0, v1, v2, v3, v4, v5, v6), v7);
}

template <class T>
std::vector<T> vec(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8)
{
  return append(vec(v0, v1, v2, v3, v4, v5, v6, v7), v8);
}

template <class T>
std::vector<T> vec(T v0, T v1, T v2, T v3, T v4, T v5, T v6, T v7, T v8, T v9)
{
  return append(vec(v0, v1, v2, v3, v4, v5, v6, v7, v8), v9);
}

template <class Iter, class T>
void iota(Iter b, Iter e, T init)
{
  for (; b != e; ++b, ++init)
    *b = init;
}

template <class T>
std::vector<T> iota(T lo, T up)
{
  std::vector<T> result(up - lo);
  iota(result.begin(), result.end(), lo);
  return result;
}

#endif // OUT_TEST_UTILS

