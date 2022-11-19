#include "catch.hpp"
#include <out/out.hpp>

struct to_string
{
  template <class T>
  std::string operator()(const T& item) const
  {
    std::stringstream ss;
    ss << item;
    return ss.str();
  }
};

struct add_to_buffer
{
  std::ostream* os;
  const char* separator;

  add_to_buffer(std::ostream& os, const char* separator)
    : os(&os)
    , separator(separator)
  {
  }


  template <class T>
  void operator()(const T& item) const
  {
    *os << item << separator;
  }
};

std::string s(const char* text)
{
  return text;
}

int cube(int x) { return x * x * x; }

bool is_prime(int x)
{
  int divs = 0;
  for (int d = 1; d <= x; ++d)
  {
    if (x % d == 0)
    {
      ++divs;
    }
  }
  return divs == 2;
}

bool is_even(int x)
{
  return x % 2 == 0;
}

struct TransformMaybeFunctor
{
  const std::string* operator()(const std::string& s) const
  {
    if (s.size() <= 5)
      return &s;
    return NULL;
  }
};

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

TEST_CASE("transform", "")
{
  const std::vector<int> in = iota(0, 5);
  std::vector<std::string> res;

  out::make_range(in) >>= out::transform(to_string()) >>= out::push_back(res);
  REQUIRE(res == vec(s("0"), s("1"), s("2"), s("3"), s("4")));
}

TEST_CASE("transform_join", "")
{
  const std::vector<int> in = iota(100, 105);
  std::string res;
  out::make_range(in) >>= out::transform_join(to_string()) >>= out::push_back(res);
  REQUIRE(res == "100101102103104");
}

TEST_CASE("transform_maybe", "")
{
  const std::vector<std::string> in = vec(s("Mercury"), s("Venus"), s("Earth"), s("Mars"), s("Jupiter"), s("Saturn"), s("Uranus"), s("Neptune"));
  std::vector<std::string> res;
  out::make_range(in) >>= out::transform_maybe(TransformMaybeFunctor()) >>= out::push_back(res);
  REQUIRE(res == vec(s("Venus"), s("Earth"), s("Mars")));
}

TEST_CASE("enumerate", "")
{
  const std::vector<std::string> in = vec(s("Mercury"), s("Venus"), s("Earth"), s("Mars"), s("Jupiter"), s("Saturn"), s("Uranus"), s("Neptune"));
  std::stringstream ss;
  out::make_range(in) >>= out::enumerate(1) >>= out::ostream(ss, "\n");
  REQUIRE(ss.str() == "[1] Mercury\n[2] Venus\n[3] Earth\n[4] Mars\n[5] Jupiter\n[6] Saturn\n[7] Uranus\n[8] Neptune\n");
}

TEST_CASE("filter", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<int> res;

  out::make_range(in) >>= out::filter(&is_prime) >>= out::push_back(res);
  REQUIRE(res == vec(2, 3, 5, 7));
}

TEST_CASE("filter, transform", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<std::string> res;

  out::make_range(in) >>= out::filter(&is_prime) >>= out::transform(to_string()) >>= out::push_back(res);
  REQUIRE(res == vec(s("2"), s("3"), s("5"), s("7")));
}

TEST_CASE("transform, filter", "")
{
  const std::vector<int> in = iota(0, 5);
  std::vector<int> res;

  out::make_range(in) >>= out::transform(&cube) >>= out::filter(&is_even) >>= out::push_back(res);
  REQUIRE(res == vec(0, 8, 64));
}

TEST_CASE("take", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<int> res;
  out::make_range(in) >>= out::take(4) >>= out::push_back(res);
  REQUIRE(res == vec(0, 1, 2, 3));
}

TEST_CASE("take_while", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<int> res;
  out::make_range(in) >>= out::take_while(out::less(5)) >>= out::push_back(res);
  REQUIRE(res == vec(0, 1, 2, 3, 4));
}

TEST_CASE("drop", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<int> res;
  out::make_range(in) >>= out::drop(4) >>= out::push_back(res);
  REQUIRE(res == vec(4, 5, 6, 7, 8, 9));
}

TEST_CASE("drop_while", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<int> res;
  out::make_range(in) >>= out::drop_while(out::less(5)) >>= out::push_back(res);
  REQUIRE(res == vec(5, 6, 7, 8, 9));
}

TEST_CASE("stride", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<int> res;
  out::make_range(in)>>= out::stride(4) >>= out::push_back(res);
  REQUIRE(res == vec(0, 4, 8));
}

TEST_CASE("for_each", "")
{
  const std::vector<int> in = iota(0, 10);
  std::stringstream ss;
  out::make_range(in) >>= out::for_each(add_to_buffer(ss, "|"));
  REQUIRE(ss.str() == "0|1|2|3|4|5|6|7|8|9|");
}

TEST_CASE("fork", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<int> o0, o1, o2;
  out::make_range(in) >>= out::fork(
      out::transform(&cube) >>= out::push_back(o0),
      out::filter(&is_prime) >>= out::push_back(o1),
      out::push_back(o2));
  REQUIRE(o0 == vec(0, 1, 8, 27, 64, 125, 216, 343, 512, 729));
  REQUIRE(o1 == vec(2, 3, 5, 7));
  REQUIRE(o2 == vec(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TEST_CASE("intersperse", "")
{
  const std::vector<int> in = iota(0, 5);
  std::vector<int> res;
  out::make_range(in) >>= out::intersperse(-1) >>= out::push_back(res);
  REQUIRE(res == vec(0, -1, 1, -1, 2, -1, 3, -1, 4));
}

TEST_CASE("intersperse empty", "")
{
  const std::vector<int> in;
  std::vector<int> res;
  out::make_range(in) >>= out::intersperse(-1) >>= out::push_back(res);
  REQUIRE(res.empty());
}

TEST_CASE("join_with", "")
{
  const std::vector<std::string> in = vec(s("alpha"), s("beta"), s("gamma"));
  std::string res;
  out::make_range(in) >>= out::join_with(',') >>= out::push_back(res);
  REQUIRE(res == "alpha,beta,gamma");
}

TEST_CASE("join", "")
{
  const std::vector< std::vector<int> > in = vec(vec(0, 1), vec(2, 3, 4), vec(5, 6, 7), vec(8, 9));
  std::vector<int> res;
  out::make_range(in) >>= out::join() >>= out::push_back(res);
  REQUIRE(res == vec(0, 1, 2, 3, 4, 5, 6, 7, 8, 9));
}

TEST_CASE("ostream", "")
{
  const std::vector<int> in = iota(100, 105);
  std::stringstream ss;
  out::make_range(in) >>= out::ostream(ss, ", ");
  REQUIRE(ss.str() == "100, 101, 102, 103, 104, ");
}

TEST_CASE("partition", "")
{
  const std::vector<int> in = iota(0, 10);
  std::vector<int> primes, non_primes;
  out::make_range(in) >>= out::partition(&is_prime, out::push_back(primes), out::push_back(non_primes));
  REQUIRE(primes == vec(2, 3, 5, 7));
  REQUIRE(non_primes == vec(0, 1, 4, 6, 8, 9));
}

TEST_CASE("push_back", "")
{
  const std::vector<int> in = iota(50, 55);
  std::vector<int> res;
  out::make_range(in) >>= out::push_back(res);
  REQUIRE(res == vec(50, 51, 52, 53, 54));
}

TEST_CASE("insert", "")
{
  const std::vector<int> in = iota(0, 6);
  std::vector<int> res = vec(1, 2, 10);
  out::make_range(in) >>= out::insert(res, res.begin() + 2);
  REQUIRE(res == vec(1, 2, 0, 1, 2, 3, 4, 5, 10));
}

TEST_CASE("insert - default argument", "")
{
  const std::vector<int> in = iota(0, 6);
  std::vector<int> res = vec(1, 2, 10);
  out::make_range(in) >>= out::insert(res);
  REQUIRE(res == vec(1, 2, 10, 0, 1, 2, 3, 4, 5));
}

TEST_CASE("tap", "")
{
  const std::vector<int> in = iota(0, 5);
  std::vector<int> res;
  std::stringstream ss;
  out::make_range(in) >>= out::tap(add_to_buffer(ss, ",")) >>= out::transform(&cube) >>= out::push_back(res);
  REQUIRE(res == vec(0, 1, 8, 27, 64));
  REQUIRE(ss.str() == "0,1,2,3,4,");
}

TEST_CASE("tee", "")
{
  const std::vector<int> in = iota(0, 5);
  std::vector<int> res;
  std::stringstream ss;
  out::make_range(in) >>= out::tee(out::ostream(ss, ",")) >>= out::transform(&cube) >>= out::push_back(res);
  REQUIRE(res == vec(0, 1, 8, 27, 64));
  REQUIRE(ss.str() == "0,1,2,3,4,");
}


