#include <gmock/gmock.h>

class OriginalClass
{
public:
  void nonVirtualMethod()
  {
    // nothing to do, here
  }
};

class IWrapper
{
public:
  virtual ~IWrapper() = default;
  virtual void nonVirtualMethod() = 0;
};

// In your tests, mock IWrapper instead of OriginalClass
class MockWrapper : public IWrapper
{
public:
  MOCK_METHOD(void, nonVirtualMethod, (), (override));
};

void
execute(IWrapper& w)
{
  w.nonVirtualMethod();
}

TEST(mockNonVirtualMethod, simple)
{
  // Arrange
  MockWrapper wrapper;
  EXPECT_CALL(wrapper, nonVirtualMethod());

  // Act
  execute(wrapper);

  // Assert
  // googlemock internals will do the asserts for us.
}
