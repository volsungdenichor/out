#pragma once

#if __cplusplus >= 201103L

#include <tuple>

namespace pipelines_cpp11
{

template <std::size_t N>
struct tag{};

template <class... Pipes>
struct pipeline_t
{
  std::tuple<Pipes...> pipes;
  static constexpr std::size_t N = sizeof...(Pipes);

private:
  template <class T>
  static T to_result(T&& item)
  {
    return std::forward<T>(item);
  }

  template <class... Args>
  auto call(tag<N - 1>, Args&&... args) const
    -> decltype(to_result(std::get<N - 1>(pipes)(std::forward<Args>(args)...)))
  {
    return to_result(std::get<N - 1>(pipes)(std::forward<Args>(args)...));
  }

  template <std::size_t I, class... Args, class = typename std::enable_if<I + 1 < N>::type>
  auto call(tag<I>, Args&&... args) const
    -> decltype(call(tag<I + 1>{}, std::get<I>(pipes)(std::forward<Args>(args)...)))
  {
    return call(tag<I + 1>{}, std::get<I>(pipes)(std::forward<Args>(args)...));
  }

public:
  template <class... Args>
  auto operator()(Args&&... args) const
    -> decltype(call(tag<0>{}, std::forward<Args>(args)...))
  {
    return call(tag<0>{}, std::forward<Args>(args)...);
  }
};

struct make_pipeline_fn
{
private:
  template <class Pipe>
  auto to_tuple(Pipe pipe) const -> std::tuple<Pipe>
  {
    return { std::move(pipe) };
  }

  template <class... Pipes>
  auto to_tuple(pipeline_t<Pipes...> pipe) const -> std::tuple<Pipes...>
  {
    return pipe.pipes;
  }

  template <class... Pipes>
  auto from_tuple(std::tuple<Pipes...> tuple) const -> pipeline_t<Pipes...>
  {
    return { std::move(tuple) };
  }

public:
  template <class... Pipes>
  auto operator()(Pipes... pipes) const
    -> decltype(from_tuple(std::tuple_cat(to_tuple(std::move(pipes))...)))
  {
    return from_tuple(std::tuple_cat(to_tuple(std::move(pipes))...));
  }
};

#define DEFINE_INLINE_VARIABLE(name, type) namespace { type name = type{}; } 

DEFINE_INLINE_VARIABLE(make_pipeline, make_pipeline_fn);
DEFINE_INLINE_VARIABLE(fn, make_pipeline_fn);

template <class... L, class... R>
auto operator >>=(pipeline_t<L...> lhs, pipeline_t<R...> rhs)
  -> decltype(make_pipeline(std::move(lhs), std::move(rhs)))
{
  return make_pipeline(std::move(lhs), std::move(rhs));
}

template <class T, class... P>
auto operator >>=(T&& item, const pipeline_t<P...>& pipeline)
  -> decltype(pipeline(std::forward<T>(item)))
{
  return pipeline(std::forward<T>(item));
}

}  // namespace pipelines_cpp11

#endif  // __cplusplus >= 201103L

