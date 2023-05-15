# soracom-arc-esp32-arduino

A utility library to use/bootstrap [Soracom Arc](https://www.soracom.io/products/arc/) easily on ESP32 Arduino boards.

## Synopsis

```ino
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
```

[examples](./examples/) have more various code-tips.

## Description

This library support two ways to use Soracom Arc easily.

### 1. Initialize a session of Soracom Arc with Soracom API dynamically

This way supports to bootstrap a session of Soracom Arc dynamically. This implies it doesn't need to store the WireGuard credentials on a board permanently.

Please see also [./examples/boot-dynamic-wg-credentials/README.md](./examples/boot-dynamic-wg-credentials/README.md).

### 2. Initialize a session of Soracom Arc with WireGuard configuration statically

This way is quite simple; it uses given static WireGuard credentials to make a session of Soracom Arc.

Please see also [./examples/boot-static-wg-credentials/README.md](./examples/boot-static-wg-credentials/README.md).

## Installation

This library is available on Arduino Library Registry. You can find this in Arduino Library Manager by searching `SoracomArcESP32`.

## Dependencies

Please refer to the `lib_deps` section in [platformio.ini](./platformio.ini). If you use Arduino IDE, please install the packages from Arduino Library Manager.

## Tips

### SORACOM API

#### Permissions

When using Soracom API, the [SAM user](https://developers.soracom.io/en/docs/security/users-and-roles/) (i.e. `apiKeyId` and `apiKey` pair) has to have the following permissions:

- `Sim:getSim`
- `Sim:renewArcSimCredentials`

And we highly recommend you not use the root user. It prefers to use a SAM user for this purpose.

#### CA certifications

It needs a CA certification when it accesses `api.soracom.io` over TLS. Please fill the pem style CA certification for `api.soracom.io` in code.

## Known issues

- This library doesn't support `allowedIPs` WireGuard configuration for now
- There is a timing issue around the WireGuard handshake; when it tries to access somewhere before the WireGuard handshake hasn't finished, it cannot reach there. We recommend setting the timeout for the remote access and introducing the retrying mechanism.

## For library developers

### Requirements

- [PlatformIO Core (CLI)](https://platformio.org/install/cli)
- [clang-format](https://clang.llvm.org/docs/ClangFormat.html)

[./devtools/bootstrap.sh](./devtools/bootstrap.sh) installs the platformio-cli simply.

### How to build

```shell
$ make build
```

### How to upload a sketch to a board

```shell
$ make upload
```

When you faced the error message like `serial.serialutil.SerialException: [Errno 2] could not open port /dev/XXX`, please adjust the `upload_port` parameter in `platformio.ini`.

NOTE: This needs a sketch application in `src/` directory.

### How to apply the code formatter

```shell
$ make fmt
```

## Maintainer

moznion (<moznion@mail.moznion.net>)

