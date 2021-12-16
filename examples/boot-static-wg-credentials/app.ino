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

  IPAddress device_ip_address(192, 0, 2, 123);
  WireGuardConfig conf("__YOUR_DEVICE_PRIVATE_KEY__", device_ip_address,
                       "__SERVER_PUBLIC_KEY__", "example.arc.soracom.io",
                       11010);

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
