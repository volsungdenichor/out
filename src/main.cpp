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

struct has_value
{
  template <class T>
  bool operator()(T* ptr) const
  {
    return !!ptr;
  }
};

struct dereference
{
  template <class T>
  T& operator()(T* ptr) const
  {
    if (!ptr)
      throw std::runtime_error("Dereferencing null ptr");
    return *ptr;
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

template <class Iter, class Output>
void operator >>=(boost::iterator_range<Iter> range, Output output)
{
  boost::range::copy(range, output);
}

int main()
{
  Person adam("Adam", "Mickiewicz");
  Person juliusz("Juliusz", "Slowacki");
  std::vector<const Person*> persons;
  persons.push_back(&adam);
  persons.push_back(NULL);
  persons.push_back(&adam);
  persons.push_back(&juliusz);
  persons.push_back(&adam);
  persons.push_back(NULL);
  persons.push_back(&juliusz);
  persons.push_back(&juliusz);

  boost::make_iterator_range(persons)
    >>= out::output()
    >>= out::filter(has_value())
    >>= out::transform(dereference())
    >>= out::cout("\n");
}


