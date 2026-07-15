#include <gmock/gmock.h>

struct CoverageFailure
{
  virtual bool inCall() const = 0;
  bool status() const { return false; }
  virtual ~CoverageFailure() = default;
};

struct InPort
{
  virtual bool inCall() const = 0;
  virtual ~InPort() = default;
};

struct OutPort
{
  virtual bool outCall() const = 0;
  virtual ~OutPort() = default;
};

class Hexagon : public InPort
{
  OutPort& outPort;

public:
  explicit Hexagon(OutPort& _outPort)
    : outPort(_outPort)
  {
  }
  bool inCall() const override { return outPort.outCall(); }
};

class OutPortMock : public OutPort
{
public:
  MOCK_METHOD(bool, outCall, (), (const override));
};

TEST(hexagonal, simple)
{
  OutPortMock outPortMock;
  EXPECT_CALL(outPortMock, outCall).WillOnce([] { return true; });
  InPort const& inPort = Hexagon(outPortMock);
  ASSERT_TRUE(inPort.inCall());
}
