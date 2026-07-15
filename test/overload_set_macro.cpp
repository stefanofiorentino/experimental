#include <gmock/gmock.h>
#include <sstream>

// https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1170r0.html

static std::ostringstream probe;

#define FWD(x) static_cast<decltype(x)&&>(x)
#define RETURNS(expr)                                                          \
  noexcept(noexcept(expr))->decltype(expr)                                     \
  {                                                                            \
    return expr;                                                               \
  }
#define OVERLOADS_OF(name) [&](auto&&... args) RETURNS(name(FWD(args)...))

void
f(int)
{
  probe << "Hello, ";
}
void
f(double)
{
  probe << "World!\n";
}

TEST(overload_set_lambda, simple)
{
  auto o = OVERLOADS_OF(f);
  o(1);
  o(1.);
  ASSERT_EQ("Hello, World!\n", probe.str());
}
