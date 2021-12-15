#include "SoracomAPI.h"

WireGuardConfig SoracomAPI::reinitializeArcCredentials(std::string simID) {
  SoracomAPI::APICredentials apiCredentials = this->authenticate();

  std::string url = Strings::format(
      "https://api.soracom.io/v1/sims/%s/credentials/arc",
      simID.c_str()); // TODO base URL TODO safe URL construction

  WiFiClientSecure secureWifiClient;
  secureWifiClient.setCACert(this->caCert.c_str());

  std::string apiKey = apiCredentials.apiKey;
  std::string apiToken = apiCredentials.token;

  HTTPClient http;
  http.begin(secureWifiClient, url.c_str());
  http.setConnectTimeout(HTTP_TIMEOUT);
  http.setTimeout(HTTP_TIMEOUT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("User-Agent", USER_AGENT);
  http.addHeader("X-Soracom-Api-Key", apiKey.c_str());
  http.addHeader("X-Soracom-Token", apiToken.c_str());

  int statusCode = http.PUT(EMPTY_JSON);
  if (statusCode < 200 || statusCode >= 300) {
    http.end();
    secureWifiClient.stop();
    throw std::runtime_error(
        "failed reinitializing the Arc credentials with statusCode: " +
        statusCode); // TODO either type would be better?
  }

  String payload = http.getString();
  http.end();
  secureWifiClient.stop();

  DynamicJsonDocument responseBody(1024);
  deserializeJson(responseBody, payload);

  SoracomAPI::ArcSessionStatus arcSessionStatus =
      this->fetchArcSessionStatus(simID, apiKey, apiToken);

  return WireGuardConfig(responseBody["arcClientPeerPrivateKey"],
                         arcSessionStatus.arcClientPeerIpAddress,
                         arcSessionStatus.arcServerPeerPublicKey,
                         arcSessionStatus.arcServerAddress,
                         arcSessionStatus.arcServerPort);
}

SoracomAPI::APICredentials SoracomAPI::authenticate() {
  WiFiClientSecure secureWifiClient;
  secureWifiClient.setCACert(this->caCert.c_str());

  HTTPClient http;
  http.begin(secureWifiClient,
             "https://api.soracom.io/v1/auth"); // TODO base URL
  http.setConnectTimeout(HTTP_TIMEOUT);
  http.setTimeout(HTTP_TIMEOUT);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("User-Agent", USER_AGENT);

  DynamicJsonDocument requestBody(1024);
  if (!this->email.empty() && !this->password.empty()) {
    requestBody["email"] = this->email;
    requestBody["password"] = this->password;
  }
  if (!this->authKeyID.empty() && !this->authKey.empty()) {
    requestBody["authKeyId"] = this->authKeyID;
    requestBody["authKey"] = this->authKey;
  }
  String requestBodyJSON;
  serializeJson(requestBody, requestBodyJSON);

  int statusCode = http.POST(requestBodyJSON);
  if (statusCode < 200 || statusCode >= 300) {
    http.end();
    secureWifiClient.stop();
    delay(5000);
    throw std::runtime_error(
        "failed the Soracom API authentication with statusCode: " +
        statusCode); // TODO either type would be better?
  }

  String payload = http.getString();
  http.end();
  secureWifiClient.stop();

  DynamicJsonDocument responseBody(4096);
  deserializeJson(responseBody, payload);

  return SoracomAPI::APICredentials(responseBody["apiKey"],
                                    responseBody["token"]);
}

SoracomAPI::ArcSessionStatus
SoracomAPI::fetchArcSessionStatus(std::string simID, std::string apiKey,
                                  std::string apiToken) {
  std::string url = Strings::format(
      "https://api.soracom.io/v1/sims/%s",
      simID.c_str()); // TODO base URL TODO safe URL construction

  WiFiClientSecure secureWifiClient;
  secureWifiClient.setCACert(this->caCert.c_str());

  HTTPClient http;
  http.begin(secureWifiClient, url.c_str());
  http.setConnectTimeout(HTTP_TIMEOUT);
  http.setTimeout(HTTP_TIMEOUT);
  http.addHeader("User-Agent", USER_AGENT);
  http.addHeader("X-Soracom-Api-Key", apiKey.c_str());
  http.addHeader("X-Soracom-Token", apiToken.c_str());

  int statusCode = http.GET();
  if (statusCode < 200 || statusCode >= 300) {
    http.end();
    secureWifiClient.stop();
    throw std::runtime_error(
        "failed fetch an Arc session status with statusCode: " +
        statusCode); // TODO either type would be better?
  }

  String payload = http.getString();
  http.end();
  secureWifiClient.stop();

  DynamicJsonDocument responseBody(8192);
  deserializeJson(responseBody, payload);

  char *arcServerEndpoint =
      strdup(responseBody["arcSessionStatus"]["arcServerEndpoint"]);
  const char *delimiter = ":";

  char *arcServerAddress = strtok(arcServerEndpoint, delimiter);
  if (arcServerAddress == NULL) {
    free(arcServerEndpoint);
    throw std::runtime_error(
        "unexpectedly, there is no arcServerAddress in " +
        std::string(arcServerEndpoint)); // TODO either type would be better?
  }
  char *arcServerPortString = strtok(NULL, delimiter);
  if (arcServerPortString == NULL) {
    free(arcServerEndpoint);
    throw std::runtime_error(
        "unexpectedly, there is no arcServerPort in " +
        std::string(arcServerEndpoint)); // TODO either type would be better?
  }
  free(arcServerEndpoint);
  int arcServerPort = strtol(arcServerPortString, NULL, 10);

  const char *interfaceIPAddressStr =
      responseBody["arcSessionStatus"]["arcClientPeerIpAddress"];
  IPAddress interfaceIPAddress;
  if (!interfaceIPAddress.fromString(
          interfaceIPAddressStr)) { // try to parse into the IPAddress
    throw std::runtime_error(
        "invalid interface IP address: " +
        std::string(
            interfaceIPAddressStr)); // TODO either type would be better?
  }

  return SoracomAPI::ArcSessionStatus(
      interfaceIPAddress, arcServerAddress, arcServerPort,
      responseBody["arcSessionStatus"]["arcServerPeerPublicKey"]);
}
