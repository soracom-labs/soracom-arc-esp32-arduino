#pragma once
#include "WiFi.h"
#include "WiFiClientSecure.h"
#include <deque>
#include <cassert>
extern std::deque<String> responses;
class HTTPClient {
public:
  void begin(WiFiClientSecure &, const char *) {}
  void setConnectTimeout(int) {}
  void setTimeout(int) {}
  void addHeader(const char *, const char *) {}
  int PUT(const char *) { return 200; }
  int POST(const String &) { return 200; }
  int GET() { return 200; }
  String getString() {
    assert(!responses.empty());
    String result = responses.front();
    responses.pop_front();
    return result;
  }
  void end() {}
};
