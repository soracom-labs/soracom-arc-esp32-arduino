# Endpoint lifetime regression test

This test compiles the real `SoracomAPI.cpp` with ArduinoJson and minimal HTTP,
Wi-Fi and TLS stubs. It supplies synthetic responses without network access.
AddressSanitizer catches the original use-after-free on the successful path;
the test also exercises malformed endpoints and later exception cleanup.
The Wi-Fi stub only selects success/error paths; it does not validate IP syntax.

```sh
ARDUINOJSON_DIR=/path/to/ArduinoJson bash tests/host/run.sh
```

Requires Clang with AddressSanitizer and UndefinedBehaviorSanitizer. C++14 is
used so this regression can run independently of the Arduino C++11 build fix.
