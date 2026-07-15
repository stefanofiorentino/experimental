#include <gmock/gmock.h>

#include <memory>

TEST(make_unique, simple)
{
  constexpr std::size_t N = 10;
  auto ptr = std::make_unique<const uint8_t[]>(N);
  ASSERT_EQ(sizeof(ptr), sizeof(uint8_t*));
}
