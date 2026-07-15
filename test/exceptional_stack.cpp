#include <array>
#include <gmock/gmock.h>
#include <stdexcept>

// struct stack
// {
//   bool empty() const;
//   bool full() const;
//   size_t pop();
//   void push(size_t);
// ...
// };

struct exceptional_stack_t
{
  exceptional_stack_t()
    : k(0)
  {
  }
  bool empty() const { return k == 0; }

  size_t pop()
  {
    if (empty())
      throw std::logic_error("stack is empty");
    return arr[--k];
  }
  bool full() const { return k == MAX; }
  void push(size_t el)
  {
    if (full()) {
      throw std::logic_error("stack is full");
    }
    arr[k++] = el;
  }

  static const size_t MAX = 100;

private:
  size_t k;
  std::array<size_t, MAX> arr;
};

struct fixture : ::testing::Test
{
  exceptional_stack_t s;
};

TEST_F(fixture, empty)
{
  ASSERT_TRUE(s.empty());
}

TEST_F(fixture, push_and_pop)
{
  s.push(3);
  ASSERT_EQ(3, s.pop());
}

TEST_F(fixture, pop_thows_if_empty)
{
  ASSERT_TRUE(s.empty());
  ASSERT_THROW(s.pop(), std::logic_error);
}

TEST_F(fixture, full)
{
  for (auto&& el : std::vector<size_t>(100)) {
    s.push(el);
  }
  ASSERT_TRUE(s.full());
}

TEST_F(fixture, push_throws_if_full)
{
  for (auto&& el : std::vector<size_t>(100)) {
    s.push(el);
  }
  ASSERT_THROW(s.push(42), std::logic_error);
}
