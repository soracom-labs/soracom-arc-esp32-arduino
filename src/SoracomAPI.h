#define HTTP_TIMEOUT 3000
#define USER_AGENT "soracom-arc-esp32-arduino/v0.0.1"
#define EMPTY_JSON "{}"

#pragma once
#include "Strings.h"
#include "WireGuardConfig.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <string>
#include <stdexcept>

class SoracomAPI {
  class APICredentials {
  public:
    APICredentials(std::string apiKey, std::string token)
        : apiKey(apiKey), token(token){};
    std::string apiKey;
    std::string token;
  };

  class ArcSessionStatus {
  public:
    ArcSessionStatus(IPAddress arcClientPeerIpAddress,
                     std::string arcServerAddress, const int arcServerPort,
                     std::string arcServerPeerPublicKey)
        : arcClientPeerIpAddress(arcClientPeerIpAddress),
          arcServerAddress(arcServerAddress), arcServerPort(arcServerPort),
          arcServerPeerPublicKey(arcServerPeerPublicKey){};
    IPAddress arcClientPeerIpAddress;
    std::string arcServerAddress;
    const int arcServerPort;
    std::string arcServerPeerPublicKey;
  };

  class ArcCredentials {
  public:
    ArcCredentials(std::string arcClientPeerPrivateKey,
                   std::string arcClientPeerPublicKey)
        : arcClientPeerPrivateKey(arcClientPeerPrivateKey),
          arcClientPeerPublicKey(arcClientPeerPublicKey){};

    std::string getArcClientPeerPrivateKey() {
      return this->arcClientPeerPrivateKey;
    };
    std::string getArcClientPeerPublicKey() {
      return this->arcClientPeerPublicKey;
    };

  private:
    std::string arcClientPeerPrivateKey;
    std::string arcClientPeerPublicKey;
  };

public:
  SoracomAPI(std::string authKeyID, std::string authKey, std::string caCert)
      : authKeyID(authKeyID), authKey(authKey), caCert(caCert){};
  WireGuardConfig reinitializeArcCredentials(std::string simID);

private:
  std::string authKeyID;
  std::string authKey;
  std::string caCert;
  APICredentials authenticate();
  ArcSessionStatus fetchArcSessionStatus(std::string simID, std::string apiKey,
                                         std::string apiToken);
};
