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

TEST(pure_virtual_call, simple)
{
  std::set_terminate(onTerminate);
  base* b;
  {
    derived d;
    b = &d;
  }
  (void)b; // cppcheck-suppress invalidLifetime
  // b->foo();
}