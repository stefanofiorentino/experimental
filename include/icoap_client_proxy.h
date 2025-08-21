// icoap_client_proxy.h
#pragma once
#include <string>
#include "power_switch_status.h"

class ICoapClientProxy {
public:
    virtual ~ICoapClientProxy() = default;

    // Sends a GET request to the specified URI path and returns the response payload as a string
    virtual PowerSwitchStatus get(const std::string& uri_path) = 0;
};
