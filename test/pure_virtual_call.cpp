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

[[noreturn]] void
onTerminate() noexcept
{
  std::_Exit(EXIT_SUCCESS);
}

const auto installed{ std::set_terminate(onTerminate) };

TEST(pure_virtual_call, simple)
{
  base* b;
  {
    derived d;
    b = &d;
  }
  // cppcheck-suppress invalidLifetime
  b->foo();
  ASSERT_TRUE(false);
}