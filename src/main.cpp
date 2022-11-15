#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <set>
#include <functional>

#include "out/out.hpp"
#include "out/in_iterator.hpp"
#include <boost/optional.hpp>

template <class Iter, class T>
void iota(Iter b, Iter e, T init)
{
  for (; b != e; ++b, ++init)
  {
    *b = init;
  }
}

struct sqr
{
  int operator()(int v) const
  {
    return v * v;
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

template <class R>
void print_range(const char* name, const R& r)
{
  std::cout << name << " = [";
  std::copy(out::begin(r), out::end(r), std::ostream_iterator<typename R::value_type>(std::cout, ", "));
  std::cout << "]" << std::endl;
}

struct Person
{
  std::string first_name;
  std::string last_name;

  Person(const std::string& first_name, const std::string& last_name)
    : first_name(first_name)
    , last_name(last_name)
  {
  }

  friend std::ostream& operator<<(std::ostream& os, const Person& item)
  {
    return os << item.first_name << " " << item.last_name;
  }
};

struct get_prefix
{
    int n;

    get_prefix(int n)
        : n(n)
    {
    }

    std::string operator()(const std::string& text) const
    {
        return text.substr(0, std::min(text.size(), static_cast<std::size_t>(n)));
    }
};

template <class T>
std::vector<T> create_vector(T lo, T up)
{
  std::vector<T> res(up - lo);
  iota(res.begin(), res.end(), lo);
  return res;
}

int main()
{
  Person adam("Adam", "Mickiewicz");
  Person juliusz("Juliusz", "Slowacki");
  Person zygmunt("Zygmunt", "Krasinski");

  std::vector<const Person*> persons;
  persons.push_back(&adam);
  persons.push_back(NULL);
  persons.push_back(&adam);
  persons.push_back(&juliusz);
  persons.push_back(&adam);
  persons.push_back(NULL);
  persons.push_back(&juliusz);
  persons.push_back(&zygmunt);
  persons.push_back(NULL);
  persons.push_back(&juliusz);

  std::vector<std::string> dest;

  out::make_range(std::make_pair(persons.begin(), persons.end()))
    >>= out::output()
    >>= out::transform_maybe(out::identity())
    >>= out::transform(out::mem_fn(&Person::first_name))
    >>= out::tee(out::insert(dest))
    >>= out::intersperse(std::string(", "))
    >>= out::enumerate()
    >>= out::cout("\n");

  print_range("dest", dest);
}


