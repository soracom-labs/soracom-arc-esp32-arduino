#!/bin/bash

set -ue

BIN_PATH="$(cd "$(dirname "$0")" || exit; pwd)"

CLANG_FORMAT_OPT=""
if [ $# -ge 1 ]; then
  if [ "$1" = "check" ]; then
    CLANG_FORMAT_OPT="--dry-run -Werror"
  elif [ "$1" = "apply" ]; then
    CLANG_FORMAT_OPT="-i"
  fi
fi

FIND_CMD="find"
if type "gfind" > /dev/null 2>&1; then
  FIND_CMD="gfind"
fi

# shellcheck disable=SC2046,2086
clang-format $CLANG_FORMAT_OPT $($FIND_CMD "$BIN_PATH/../" -regextype egrep -regex ".+[.][ch](pp)?")
clang-format $CLANG_FORMAT_OPT $($FIND_CMD "$BIN_PATH/../" -regextype egrep -regex ".+[.]ino")

