#pragma once

#include "icoap_client_proxy.h"
#include "power_switch_status.h"
#include <string>

class CoapClient
{
public:
  CoapClient(ICoapClientProxy& coap_proxy)
    : coap_proxy_(coap_proxy)
  {
  }

  PowerSwitchStatus get_power_switch_status()
  {
    return coap_proxy_.get("power-switch");
  }

private:
  ICoapClientProxy& coap_proxy_;
};
