#include "catch.hpp"
#include <out/out.hpp>
#include "test_utils.hpp"

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

struct repeat_value
{
  int n;

  repeat_value(int n) : n(n) { }

  template <class T>
  std::vector<T> operator()(T value) const
  {
    return std::vector<T>(n, value);
  }
};


template <class Res, class In, class Pipe>
Res run(const In& in, Pipe pipe)
{
  Res res;
  out::make_range(in) >>= pipe >>= out::push_back(res);
  return res;
}

template <class T, class In, class Pipe>
std::vector<T> run_to_vec(const In& in, Pipe pipe)
{
  return run< std::vector<T> >(in, pipe);
}

TEST_CASE("transform", "")
{
  const std::vector<std::string> res = run_to_vec<std::string>(
    iota(0, 5),
    out::transform(to_string()));
  REQUIRE(res == vec(s("0"), s("1"), s("2"), s("3"), s("4")));
}

TEST_CASE("transform_join", "")
{
  const std::string res = run<std::string>(
    iota(100, 105), 
    out::transform_join(to_string()));
  REQUIRE(res == "100101102103104");
}

TEST_CASE("transform_maybe", "")
{
  const std::vector<std::string> res = run_to_vec<std::string>(
    vec(s("Mercury"), s("Venus"), s("Earth"), s("Mars"), s("Jupiter"), s("Saturn"), s("Uranus"), s("Neptune")),
    out::transform_maybe(TransformMaybeFunctor()));

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
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 10), 
    out::filter(&is_prime));
  REQUIRE(res == vec(2, 3, 5, 7));
}

TEST_CASE("filter, transform", "")
{
  const std::vector<std::string> res = run_to_vec<std::string>(
    iota(0, 10), 
    out::filter(&is_prime) >>= out::transform(to_string()));
  REQUIRE(res == vec(s("2"), s("3"), s("5"), s("7")));
}

TEST_CASE("transform, filter", "")
{
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 5), 
    out::transform(&cube) >>= out::filter(&is_even));
  REQUIRE(res == vec(0, 8, 64));
}

TEST_CASE("take", "")
{
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 10), 
    out::take(4));
  REQUIRE(res == vec(0, 1, 2, 3));
}

TEST_CASE("drop", "")
{
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 10),
    out::drop(4));
  REQUIRE(res == vec(4, 5, 6, 7, 8, 9));
}

TEST_CASE("slice", "")
{
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 20),
    out::slice(5, 10));
  REQUIRE(res == vec(5, 6, 7, 8, 9));
}

TEST_CASE("sub", "")
{
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 20),
    out::sub(5, 2));
  REQUIRE(res == vec(5, 6));
}

TEST_CASE("take_while", "")
{
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 10), 
    out::take_while(out::less(5))); 
  REQUIRE(res == vec(0, 1, 2, 3, 4));
}

TEST_CASE("drop_while", "")
{
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 10), 
    out::drop_while(out::less(5)));
  REQUIRE(res == vec(5, 6, 7, 8, 9));
}

TEST_CASE("stride", "")
{
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 10), 
    out::stride(4));
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
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 5), 
    out::intersperse(-1));
  REQUIRE(res == vec(0, -1, 1, -1, 2, -1, 3, -1, 4));
}

TEST_CASE("intersperse empty", "")
{
  const std::vector<int> res = run_to_vec<int>(std::vector<int>(), out::intersperse(1));
  REQUIRE(res.empty());
}

TEST_CASE("join_with", "")
{
  const std::string res = run<std::string>(
    vec(s("alpha"), s("beta"), s("gamma")),
    out::join_with(','));
  REQUIRE(res == "alpha,beta,gamma");
}

TEST_CASE("join", "")
{
  const std::vector<int> res = run_to_vec<int>(
    vec(vec(0, 1), vec(2, 3, 4), vec(5, 6, 7), vec(8, 9)),
    out::join());
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
  std::stringstream ss;
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 5), 
    out::tap(add_to_buffer(ss, ",")) >>= out::transform(&cube));
  REQUIRE(res == vec(0, 1, 8, 27, 64));
  REQUIRE(ss.str() == "0,1,2,3,4,");
}

TEST_CASE("tee", "")
{
  std::stringstream ss;
  const std::vector<int> res = run_to_vec<int>(
    iota(0, 5), 
    out::tee(out::ostream(ss, ",")) >>= out::transform(&cube));
  REQUIRE(res == vec(0, 1, 8, 27, 64));
  REQUIRE(ss.str() == "0,1,2,3,4,");
}

TEST_CASE("complex", "")
{
  const std::string res = run<std::string>(
    iota(0, 1000),
    out::drop(10) 
    >>= out::stride(3) 
    >>= out::take(4) 
    >>= out::transform_join(repeat_value(2)) 
    >>= out::transform(to_string()) 
    >>= out::intersperse(s(", ")) 
    >>= out::join()
    >>= out::take_while(out::not_equal_to('6')));

  REQUIRE(res == "10, 10, 13, 13, 1");
}

