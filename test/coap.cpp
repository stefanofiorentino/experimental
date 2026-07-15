#include "gmock/gmock.h"

#include "coap_client.h"

class MockCoapClientProxy : public ICoapClientProxy
{
public:
  MOCK_METHOD(PowerSwitchStatus,
              get,
              (const std::string& uri_path),
              (override));
};

TEST(CoapClientTest, GetPowerSwitchStatusOn)
{
  MockCoapClientProxy mock_proxy;

  EXPECT_CALL(mock_proxy, get("power-switch"))
    .WillOnce(testing::Return(PowerSwitchStatus::ON));

  CoapClient client(mock_proxy);

  auto status = client.get_power_switch_status();

  EXPECT_EQ(status, PowerSwitchStatus::ON);
}

TEST(CoapClientTest, GetPowerSwitchStatusOff)
{
  MockCoapClientProxy mock_proxy;

  EXPECT_CALL(mock_proxy, get("power-switch"))
    .WillOnce(testing::Return(PowerSwitchStatus::OFF));

  CoapClient client(mock_proxy);

  auto status = client.get_power_switch_status();

  EXPECT_EQ(status, PowerSwitchStatus::OFF);
}

TEST(CoapClientTest, GetPowerSwitchStatusUnknownError)
{
  MockCoapClientProxy mock_proxy;

  EXPECT_CALL(mock_proxy, get("power-switch"))
    .WillOnce(testing::Return(PowerSwitchStatus::UNKNOWN_ERROR));

  CoapClient client(mock_proxy);

  auto status = client.get_power_switch_status();

  EXPECT_EQ(status, PowerSwitchStatus::UNKNOWN_ERROR);
}

TEST(CoapClientTest, GetPowerSwitchStatusTimeoutError)
{
  MockCoapClientProxy mock_proxy;

  EXPECT_CALL(mock_proxy, get("power-switch"))
    .WillOnce(testing::Return(PowerSwitchStatus::TIMEOUT_ERROR));

  CoapClient client(mock_proxy);

  auto status = client.get_power_switch_status();

  EXPECT_EQ(status, PowerSwitchStatus::TIMEOUT_ERROR);
}
