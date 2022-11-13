#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

#include "out/out.hpp"
#include <boost/range/algorithm/copy.hpp>

template <class Iter, class T>
void iota(Iter b, Iter e, T init)
{
  for (; b != e; ++b, ++init)
  {
    *b = init;
  }
}

struct inc
{
  int operator()(int v) const
  {
    return v + 1;
  }
};

struct sqr
{
  int operator()(int v) const
  {
    return v * v;
  }
};

struct neg
{
  int operator()(int v) const
  {
    return -v;
  }
};

struct is_even
{
  bool operator()(int v) const
  {
    return v % 2 == 0;
  }
};

struct is_vowel
{
  bool operator()(char ch) const
  {
    static const std::string vowels = "aeiouy";
    return vowels.find(std::tolower(ch)) != std::string::npos;
  }
};

struct print
{
  const char* prefix;

  print(const char* prefix)
    : prefix(prefix)
  {
  }

  template <class T>
  void operator()(const T& item) const
  {
    std::cout << prefix << item << std::endl;
  }
};

template <class T>
void print_vect(const char* name, const std::vector<T>& v)
{
  std::cout << name << " = [";
  std::copy(v.begin(), v.end(), std::ostream_iterator<int>(std::cout, " "));
  std::cout << "]" << std::endl;
}

int main()
{
  std::vector<int> a, b, c;

  a.resize(50);
  iota(a.begin(), a.end(), 0);

  boost::range::copy(
    a,
    out::drop(3) >>= out::take(3)
    >>= out::cout("\n"));

  print_vect("a", a);
  print_vect("b", b);
  print_vect("c", c);
}


