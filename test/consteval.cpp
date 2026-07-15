#include <gmock/gmock.h>

#include <array>
#include <iostream>

// This function might be evaluated at compile-time, if the input
// is known at compile-time. Otherwise, it is executed at run-time.
constexpr unsigned
factorial(unsigned n)
{
  return n < 2 ? 1 : n * factorial(n - 1);
}

// With consteval we enforce that the function will be evaluated at
// compile-time.
consteval unsigned
combination(unsigned m, unsigned n)
{
  auto s = std::array<std::size_t, 128>();
  s.fill(1);
  s[0] = 0;
  s.at(0) = 0;
  return factorial(n) / factorial(m) / factorial(n - m);
}

static_assert(factorial(6) == 720);
static_assert(combination(4, 8) == 70);

TEST(consteval, simple)
{
  constexpr unsigned x{ factorial(4) };
  ASSERT_EQ(24, x);

  //[[maybe_unused]] unsigned y = factorial(static_cast<unsigned>(argc)); // OK
  // unsigned z = combination(argc, 7); // error: 'argc' is not a constant
  // expression
}
