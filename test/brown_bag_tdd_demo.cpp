// integer stack LIFO 

#include <gmock/gmock.h>
#include <limits>
#include <mutex>

struct my_stack_t
{
    my_stack_t() noexcept : size_(0) {
        for (int i = 0; i<N; i++)
        {
            v_[i] = std::numeric_limits<int>::min();
        }
    }
    bool isEmpty() const noexcept
    {
        return size_ == 0;
    }

    int getSize() const noexcept
    {
        return size_;
    }

    int pop() noexcept
    {
        if (!isEmpty())
        {
            return v_[--size_];
        }
        return std::numeric_limits<int>::min();
    }

    void push(int v) noexcept
    {
        v_[size_++] = v;
    }
    static const size_t N = 100;
    int v_[N];
    int size_;
};

struct stack_fixture_t : ::testing::Test
{
    my_stack_t s;
};

TEST_F(stack_fixture_t, isEmpty)
{
    ASSERT_TRUE(s.isEmpty());
}

TEST_F(stack_fixture_t, getSize)
{
    ASSERT_EQ(0, s.getSize());
}

TEST_F(stack_fixture_t, pop)
{
    ASSERT_EQ(std::numeric_limits<int>::min(), s.pop());
}

TEST_F(stack_fixture_t, push)
{
    s.push(42);
    ASSERT_EQ(1, s.getSize());
    ASSERT_EQ(42, s.pop());
    ASSERT_EQ(0, s.getSize());
}

TEST_F(stack_fixture_t, double_push)
{
    s.push(24);
    ASSERT_EQ(1, s.getSize());
    s.push(42);
    ASSERT_EQ(2, s.getSize());
    ASSERT_EQ(42, s.pop());
    ASSERT_EQ(1, s.getSize());
    ASSERT_EQ(24, s.pop());
    ASSERT_EQ(0, s.getSize());
}

TEST_F(stack_fixture_t, triple_push)
{
    s.push(24);
    ASSERT_EQ(1, s.getSize());
    s.push(42);
    ASSERT_EQ(2, s.getSize());
    s.push(422);
    ASSERT_EQ(3, s.getSize());
    ASSERT_EQ(422, s.pop());
    ASSERT_EQ(2, s.getSize());
    ASSERT_EQ(42, s.pop());
    ASSERT_EQ(1, s.getSize());
    ASSERT_EQ(24, s.pop());
    ASSERT_EQ(0, s.getSize());
}
