#include <gmock/gmock.h>
#include <type_traits>

template<typename T>
struct A
{
  virtual ~A()
  {
    static_assert(std::is_integral_v<typename T::Value>,
                  "`T::Value` must be an integral type.");
  }
};

struct B : A<B>
{
  using Value = int;
};

TEST(integral_type, simple)
{
  ASSERT_TRUE(std::is_integral_v<typename B::Value>);
}
