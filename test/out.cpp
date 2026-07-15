#include <gmock/gmock.h>

#include <concepts>
#include <string>
#include <utility>

template<typename T>
concept has_pre_increment_operator = requires(T v)
{
  ++v;
};

template<typename T>
concept has_post_increment_operator = requires(T v)
{
  v++;
};

struct check
{
  check& operator++() = delete;
  check operator++(int) { return {}; }
};

static_assert(!has_pre_increment_operator<check>,
              "The class has defined the pre-increment operator");
static_assert(has_post_increment_operator<check>,
              "The class has not defined the post-increment operator");

namespace {
template<typename T>
struct out final
{
  out() = delete;
  out(out<T> const&) = delete;
  out<T>& operator=(out<T> const&) = delete;
  out(out<T>&&) = default;
  out<T>& operator=(out<T>&&) = default;

  explicit out(T& rhs)
    : t(rhs)
  {
  }

  explicit out(T const& rhs)
    : t(rhs)
  {
  }
  out<T>& operator=(T const& rhs)
  {
    t = rhs;
    return *this;
  }

  explicit out(T&& rhs)
    : t(rhs)
  {
  }
  out<T>& operator=(T&& rhs)
  {
    t = rhs;
    return *this;
  }

  constexpr operator T() const { return t; }
  constexpr T* operator->() { return &t; }
  constexpr T& get() { return t; }

private:
  T& t;
};
}

template<typename T>
void
source(out<T>&& o)
{
  o = 42;
}

TEST(out, builtin_types_int)
{
  // arrange
  int v = -1;

  // act
  source(out<decltype(v)>(v));

  // assert
  ASSERT_EQ(42, v);
}

TEST(out, builtin_types_float)
{
  // arrange
  float v = -1;

  // act
  source(out<decltype(v)>(v));

  // assert
  ASSERT_FLOAT_EQ(42., v);
}

struct wrapper
{
  int x;
  int y;
};

template<>
void
source(out<wrapper>&& o)
{
  o->x = 42;
  o->y = 21;
}

TEST(out, custom_types)
{
  // arrange
  wrapper w{};

  // act
  source(out<decltype(w)>(w));

  // assert
  ASSERT_EQ(42, w.x);
  ASSERT_EQ(21, w.y);
}
