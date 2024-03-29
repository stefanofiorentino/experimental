#include <gmock/gmock.h>

#include <concepts>
#include <limits>
#include <memory>

template<typename T>
class shared_ptr_or_default
{
  std::shared_ptr<T> sp;

  T* get_raw_ptr_or_default() const
  {
    if (!sp) {
      static T t{};
      return &t;
    }
    return sp.get();
  }

public:
  explicit shared_ptr_or_default(std::shared_ptr<T> const& sp)
    : sp(sp)
  {
  }
  T* get() const noexcept { return get_raw_ptr_or_default(); }

  T& operator*() const noexcept { return *get_raw_ptr_or_default(); }

  T* operator->() const noexcept { return get_raw_ptr_or_default(); }
};

TEST(shared_ptr_or_default, pointer_to_one)
{
  auto sp = std::make_shared<int>(1);
  shared_ptr_or_default<int> usp{ sp };
  EXPECT_EQ(1, *usp.get());
  EXPECT_EQ(1, usp.operator*());
  EXPECT_EQ(1, *usp.operator->());
}

TEST(shared_ptr_or_default, return_default_if_nullptr)
{
  std::shared_ptr<int> sp = nullptr;
  shared_ptr_or_default<int> usp{ sp };
  EXPECT_EQ(0, *usp.get());
  EXPECT_EQ(0, usp.operator*());
  EXPECT_EQ(0, *usp.operator->());
}

TEST(shared_ptr_or_default, pointer_to_char)
{
  auto sp = std::make_shared<char>('c');
  shared_ptr_or_default<char> usp{ sp };
  EXPECT_EQ('c', *usp.get());
  EXPECT_EQ('c', usp.operator*());
  EXPECT_EQ('c', *usp.operator->());
}

TEST(shared_ptr_or_default, pointer_to_valid_std__string)
{
  constexpr auto expected = "Hello, World!";
  auto sp = std::make_shared<std::string>(expected);
  shared_ptr_or_default<std::string> usp{ sp };
  EXPECT_EQ(expected, *usp.get());
  EXPECT_EQ(expected, usp.operator*());
  EXPECT_EQ(expected, *usp.operator->());
  EXPECT_EQ(13, usp->size());
}

TEST(shared_ptr_or_default, pointer_to_unvalid_std__string)
{
  std::shared_ptr<std::string> sp;
  shared_ptr_or_default<std::string> usp{ sp };
  EXPECT_EQ("", *usp.get());
  EXPECT_EQ("", usp.operator*());
  EXPECT_EQ("", *usp.operator->());
  EXPECT_EQ(0, usp->size());
}
