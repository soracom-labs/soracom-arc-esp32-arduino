#pragma once
#include <WiFi.h>

class WireGuardConfig {
public:
  WireGuardConfig(std::string interfacePrivateKey, IPAddress interfaceIPAddress,
                  std::string peerPublicKey, std::string peerAddress,
                  int peerPort)
      : interfacePrivateKey(interfacePrivateKey),
        interfaceIPAddress(interfaceIPAddress), peerPublicKey(peerPublicKey),
        peerAddress(peerAddress), peerPort(peerPort){};

  std::string interfacePrivateKey;
  IPAddress interfaceIPAddress;
  std::string peerPublicKey;
  std::string peerAddress;
  int peerPort;
  // TODO support "allowedIPs"
};
