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

bool perform_action(abstract_remote_interface &ri)
{
    if (!ri.is_available())
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

TEST(mocking_workshop, e2e_di)
{
    remote_interface ri;
    ASSERT_FALSE(perform_action(ri));
}

struct remote_interface_mock : abstract_remote_interface
{
    MOCK_METHOD(bool, is_available, (), (const));
};

struct mocking_workshop_fixture : ::testing::Test
{
    mocking_workshop_fixture() : rim() {}

    void SetUp() override
    {
        // setup your fixture
        std::puts(__FUNCSIG__);
    }

    void TearDown() override
    {
        // tear down fixture
        std::puts(__FUNCSIG__);
    }

    remote_interface_mock rim;
};

TEST_F(mocking_workshop_fixture, default_mock)
{
    std::puts(__FUNCSIG__);
    EXPECT_CALL(rim, is_available);
    ASSERT_FALSE(perform_action(rim));
}

TEST_F(mocking_workshop_fixture, true_mock)
{
    std::puts(__FUNCSIG__);
    EXPECT_CALL(rim, is_available).WillOnce(Return(true));
    ASSERT_TRUE(perform_action(rim));
}
