#pragma once
#include <string>
using String = std::string;
class IPAddress {
public:
  bool fromString(const char *value) { return value && std::string(value) != "invalid"; }
};
#define log_e(...) ((void)0)
