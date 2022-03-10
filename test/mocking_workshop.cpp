#include <gmock/gmock.h>

#include <iostream>
#include <cstdio>

struct remote_interface 
{
    bool is_available() const 
    {
        return false;
    }
};

bool perform_action()
{
    remote_interface ri;
    if (!ri.is_available())
    {
        return false;
    }
    // call other remote interface services
    return true;
}

TEST(mocking_workshop, e2e)
{
    ASSERT_FALSE(perform_action());
}
