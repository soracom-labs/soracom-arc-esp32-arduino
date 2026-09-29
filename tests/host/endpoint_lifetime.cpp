#include "SoracomAPI.h"
#include <cassert>
#include <deque>
#include <iostream>

std::deque<String> responses;

static WireGuardConfig fetch(const std::string &endpointJSON,
                             const char *interfaceAddress = "test-address") {
  responses = {
      R"({"apiKey":"example-api-key","token":"example-token"})",
      R"({"arcClientPeerPrivateKey":"example-private-key"})",
      std::string(R"({"arcSessionStatus":{"arcServerEndpoint":)") + endpointJSON +
          R"(,"arcClientPeerIpAddress":")" + interfaceAddress +
          R"(","arcServerPeerPublicKey":"example-public-key"}})"};
  SoracomAPI api("example-id", "example-auth", "example-ca");
  return api.reinitializeArcCredentials("example-sim");
}

static void expectError(const std::string &endpoint, const char *message,
                        const char *interfaceAddress = "test-address") {
  try {
    fetch(endpoint, interfaceAddress);
    assert(false && "expected an exception");
  } catch (const std::runtime_error &error) {
    assert(std::string(error.what()).find(message) != std::string::npos);
  }
}

int main() {
  const auto config = fetch(R"("vpn.example.test:51820")");
  assert(config.peerAddress == "vpn.example.test");
  assert(config.peerPort == 51820);
  assert(config.peerPublicKey == "example-public-key");
  assert(config.interfacePrivateKey == "example-private-key");
  expectError(R"("")", "no arcServerAddress");
  expectError(R"("vpn.example.test")", "no arcServerPort");
  expectError(R"("vpn.example.test:")", "no arcServerPort");
  expectError("null", "missing Arc server endpoint");
  expectError(R"("vpn.example.test:51820")", "invalid interface IP", "invalid");
  std::cout << "Endpoint lifetime tests passed\n";
}
