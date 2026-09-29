#!/usr/bin/env bash
set -euo pipefail
: "${ARDUINOJSON_DIR:?Set ARDUINOJSON_DIR to the ArduinoJson library directory}"
repo_dir=$(cd "$(dirname "$0")/../.." && pwd)
build_dir=$(mktemp -d)
trap 'rm -rf "$build_dir"' EXIT
"${CXX:-clang++}" -std=c++14 -g -fsanitize=address,undefined \
  -fno-omit-frame-pointer -iquote "$repo_dir/src" \
  -I "$repo_dir/tests/host/stubs" -I "$ARDUINOJSON_DIR/src" \
  "$repo_dir/src/SoracomAPI.cpp" "$repo_dir/tests/host/endpoint_lifetime.cpp" \
  -o "$build_dir/endpoint_lifetime"
"$build_dir/endpoint_lifetime"
