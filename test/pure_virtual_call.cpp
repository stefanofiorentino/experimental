#include "gtest/gtest.h"
#include <gmock/gmock.h>

struct base
{
  virtual ~base() = default;
  virtual void foo() const = 0;
};

struct derived final : base
{
  void foo() const override {}
};

TEST(pure_virtual_call, simple)
{
  base* b;
  {
    derived d;
    b = &d;
  }
  (void)b; // cppcheck-suppress invalidLifetime
  // b->foo();
}
