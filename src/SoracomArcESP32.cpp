#include "SoracomArcESP32.h"

bool SoracomArc::activate(SoracomAPI soracomAPIClient, std::string simID) {
  WireGuardConfig wireGuardConfig =
      soracomAPIClient.reinitializeArcCredentials(simID);
  return this->activate(wireGuardConfig);
}

bool SoracomArc::activate(WireGuardConfig conf) {
  bool isWireGuardBegun = this->wg.begin(
      conf.interfaceIPAddress, conf.interfacePrivateKey.c_str(),
      conf.peerAddress.c_str(), conf.peerPublicKey.c_str(), conf.peerPort);
  if (!isWireGuardBegun) {
    // TODO
    return false;
  }

  return true;
}
