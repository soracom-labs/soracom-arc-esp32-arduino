#pragma once
#include "SoracomAPI.h"
#include "WireGuardConfig.h"
#include <WireGuard-ESP32.h>

class SoracomArc {
public:
  bool activate(SoracomAPI soracomAPIClient,
                std::string simID); // TODO support "allowedIPs"
  bool activate(WireGuardConfig conf);

private:
  WireGuard wg;
};
