// integer stack LIFO 

#include <gmock/gmock.h>

struct my_stack_t {
    my_stack_t() : is_empty(true){}
    int stack[100];
    bool is_empty;
    bool empty() const
    {
        return is_empty;
    }

    int pop() 
    {
        return -1;
    }

    bool push(int value)
    {
        stack[0] = value;
        is_empty = false;
        return true;
    }
};

TEST(my_stack_t, empty_true)
{
    my_stack_t s;
    ASSERT_TRUE(s.empty());
}

TEST(my_stack_t, empty_pop)
{
    my_stack_t s;
    ASSERT_TRUE(s.empty());
    ASSERT_EQ(-1, s.pop());
}

TEST(my_stack_t, empty_push)
{
    my_stack_t s;
    ASSERT_TRUE(s.empty());
    ASSERT_TRUE(s.push(42));
    ASSERT_FALSE(s.empty());
}
