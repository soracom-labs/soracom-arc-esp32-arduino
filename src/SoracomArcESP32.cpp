#include "SoracomArcESP32.h"

bool SoracomArc::activate(SoracomAPI soracomAPIClient, std::string simID) {
  try {
    WireGuardConfig wireGuardConfig =
        soracomAPIClient.reinitializeArcCredentials(simID);
    return this->activate(wireGuardConfig);
  } catch (const std::exception &e) {
    log_e("error occurrd on Soracom Arc activation; please try retrying");
    return false;
  }
}

bool SoracomArc::activate(WireGuardConfig conf) {
  return this->wg.begin(
      conf.interfaceIPAddress, conf.interfacePrivateKey.c_str(),
      conf.peerAddress.c_str(), conf.peerPublicKey.c_str(), conf.peerPort);
}
