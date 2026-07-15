#include "gtest/gtest.h"
#include <array>

namespace freestanding {
template<typename Tp, std::size_t Nm>
class array final
{
  typedef std::array<Tp, Nm> type;
  typedef typename type::size_type size_type;
  typedef typename type::value_type value_type;
  typedef typename type::pointer pointer;
  typedef typename type::const_pointer const_pointer;
  typedef typename type::reference reference;
  typedef typename type::const_reference const_reference;

  type __arr;

public:
  size_type size() const noexcept { return __arr.size(); }
  // Element access.
  reference operator[](size_type __n) noexcept
  {
    assert(__n >= 0 && __n < Nm);
    return __arr[__n];
  }

  constexpr const_reference operator[](size_type __n) const noexcept
  {
    assert(__n >= 0 && __n < Nm);
    return __arr[__n];
  }

  pointer data() noexcept { return __arr.data(); }
  const_pointer data() const noexcept { return __arr.data(); }

  // Deleted.
  reference at(size_type __n) = delete;
  constexpr const_reference at(size_type __n) const = delete;
};
}

TEST(freestanding, givenCtedArrayThenSizeIsFixed)
{
  freestanding::array<int, 8> arr{};
  ASSERT_EQ(8, arr.size());
}

TEST(freestanding, givenCtedArrayThenFirstElementIsZeroInitialized)
{
  freestanding::array<int, 8> arr{};
  ASSERT_EQ(0, arr[0]);
}

TEST(freestanding, givenCtedArrayWhenSetDataThenExpectedDataIsRead)
{
  freestanding::array<int, 8> arr{};
  arr[4] = 1;
  ASSERT_EQ(0, arr[0]);
  ASSERT_EQ(1, arr[4]);
}

TEST(freestanding, givenCtedArrayWhenSetDataThenExpectedConstDataIsRead)
{
  freestanding::array<int, 8> arr{};
  arr[4] = 1;
  const auto& arr_ref = arr;
  ASSERT_EQ(0, arr_ref[0]);
  ASSERT_EQ(1, arr_ref[4]);
}

TEST(freestanding, givenCtedArrayWhenSetDataThenExpectedRawDataIsRead)
{
  freestanding::array<int, 8> arr{};
  arr[4] = 1;
  auto pointer = arr.data();
  ASSERT_EQ(0, pointer[0]);
  ASSERT_EQ(1, pointer[4]);
}

TEST(freestanding, givenCtedArrayWhenSetDataThenExpectedConstRawDataIsRead)
{
  freestanding::array<int, 8> arr{};
  arr[4] = 1;
  const auto& arr_ref = arr;
  auto pointer = arr_ref.data();
  ASSERT_EQ(0, pointer[0]);
  ASSERT_EQ(1, pointer[4]);
}

TEST(freestanding, not_compiling)
{
  std::array<int, 8> arr{};
  ASSERT_EQ(8, arr.size());
  auto& v = arr.at(0);
  ASSERT_EQ(0, v);
}