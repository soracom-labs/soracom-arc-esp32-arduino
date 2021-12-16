#include <SoracomArcESP32.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define BAUDRATE 115200

static SoracomArc soracomArc;

void setup() {
  Serial.begin(BAUDRATE);
  while (!Serial)
    ;

  WiFi.begin("__YOUR_AP_SSID__", "__YOUR_AP_PASSWORD__");
  while (!WiFi.isConnected()) {
    delay(1000);
  }

  configTime(0, 0, "ntp.nict.jp", "time.google.com", "pool.ntp.org");

  // NOTE: usually "client peer" means your device.
  IPAddress clientPeerIPAddress(192, 0, 2, 123);
  std::string clientPeerPrivateKey = "__YOUR_CLIENT_PEER_PRIVATE_KEY__";
  std::string serverPeerPublicKey = "__ARC_SERVER_PEER_PUBLIC_KEY__";
  std::string serverAddress =
      "__ARC_SERVER_ADDRESS__"; // e.g. example.arc.soracom.io
  int serverPort = 11010;

  WireGuardConfig conf(clientPeerPrivateKey, clientPeerIPAddress,
                       serverPeerPublicKey, serverAddress, serverPort);

  while (!soracomArc.activate(conf)) {
    delay(5000);
  }

  log_i("setup finished");
}

void loop() {
  HTTPClient http;

  http.begin("http://metadata.soracom.io/");
  http.setConnectTimeout(1000);
  http.setTimeout(1000);
  int statusCode = http.GET();

  String payload = http.getString();
  log_i("http status: %d", statusCode);
  log_i("%s", payload.c_str());

  http.end();

  delay(30000);
}
