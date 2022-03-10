#include <gmock/gmock.h>

#include <iostream>
#include <cstdio>

using ::testing::Return;

struct abstract_remote_interface
{
    virtual bool is_available() const = 0;
    virtual ~abstract_remote_interface() = default;
};

struct remote_interface : abstract_remote_interface
{
    bool is_available() const override
    {
        return false;
    }
};

bool perform_action(abstract_remote_interface &ari)
{
    if (!ari.is_available())
    {
        return false;
    }
    // call other remote interface services
    return true;
}

bool perform_action()
{
    remote_interface ri;
    return perform_action(ri);
}

TEST(mocking_workshop, e2e)
{
    ASSERT_FALSE(perform_action());
}

struct remote_interface_mock : abstract_remote_interface
{
    MOCK_METHOD(bool, is_available, (), (const));
};

TEST(mocking_workshop, mock_true)
{
    remote_interface_mock rim;
    EXPECT_CALL(rim, is_available).WillOnce(Return(false));
    ASSERT_FALSE(perform_action(rim));
}

TEST(mocking_workshop, mock_false)
{
    remote_interface_mock rim;
    EXPECT_CALL(rim, is_available).WillOnce(Return(true));
    ASSERT_TRUE(perform_action(rim));
}

