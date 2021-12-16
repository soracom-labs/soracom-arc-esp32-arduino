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

  std::string soracomAPIAuthKeyID = "keyId-__YOUR_SORACOM_API_KEY_ID__";
  std::string soracomAPIAuthKey = "secret-__YOUR_SORACOM_API_AUTH_KEY__";
  std::string caCert = "-----BEGIN CERTIFICATE-----\n"
                       "...\n"
                       "__CA_CERTIFICATION_STRING__\n"
                       "...\n"
                       "-----END CERTIFICATE-----\n";

  SoracomAPI soracomAPIClient(soracomAPIAuthKeyID, soracomAPIAuthKey, caCert);

  std::string simID = "__YOUR_SIM_ID__";
  while (!soracomArc.activate(soracomAPIClient, simID)) {
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
