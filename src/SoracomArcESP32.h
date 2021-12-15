#pragma once
#include "soracom-arc-esp32-arduino/SoracomAPI.h"
#include "soracom-arc-esp32-arduino/WireGuardConfig.h"
#include <WireGuard-ESP32.h>

class SoracomArc {
public:
  bool activate(SoracomAPI soracomAPIClient,
                std::string simID); // TODO support "allowedIPs"
  bool activate(WireGuardConfig conf);

private:
  WireGuard wg;
};
