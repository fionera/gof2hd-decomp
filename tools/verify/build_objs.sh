#!/usr/bin/env bash
# Compile every src/*.cpp with the matching NDK r18b toolchain (via OrbStack) into
# <build-dir>/base/<rel>.o, resiliently: a TU that fails to compile is skipped and
# reported, never aborting the run (not every recovered TU compiles yet). Objects
# are the "base" side fed to the verify differ.
#
# Usage: build_objs.sh [build-dir]   (default: cmake-build-match/verify)
set -uo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$HERE/../.." && pwd)"
BUILD="${1:-$REPO/cmake-build-match/verify}"
JOBS="${GOF2_VERIFY_JOBS:-8}"
cd "$REPO"
. "$HERE/match_flags.sh"

BASE="$BUILD/base"
LOGS="$BUILD/logs"
mkdir -p "$BASE" "$LOGS"
# Only touch .flags when the flag set actually changes, so unchanged flags don't
# force a full rebuild (objects are compared against this file's mtime below).
if [ ! -f "$BUILD/.flags" ] || [ "$(cat "$BUILD/.flags")" != "$GOF2_MATCH_CXXFLAGS" ]; then
  echo "$GOF2_MATCH_CXXFLAGS" > "$BUILD/.flags"
fi

# Recompile a TU only when its object is missing or older than the source or the
# flag set (so flag tuning forces a rebuild).
compile_one() {
  local src="$1"
  local rel="${src#src/}"; rel="${rel%.cpp}"
  local obj="$BASE/$rel.o"
  local log="$LOGS/$rel.log"
  mkdir -p "$(dirname "$obj")" "$(dirname "$log")"
  if [ -f "$obj" ] && [ "$obj" -nt "$src" ] && [ "$obj" -nt "$BUILD/.flags" ]; then
    return 0
  fi
  if "$HERE/orbcc" $GOF2_MATCH_CXXFLAGS -c "$src" -o "$obj" >"$log" 2>&1; then
    rm -f "$log"
    return 0
  fi
  rm -f "$obj"
  return 1
}
export -f compile_one
export HERE BASE LOGS BUILD GOF2_MATCH_CXXFLAGS

# (portable: macOS ships bash 3.2 which lacks `mapfile`)
total=$(find src -name '*.cpp' | wc -l | tr -d ' ')
echo "[build] $total TUs -> $BASE  (jobs=$JOBS, opt=${GOF2_MATCH_OPT:-?})"
find src -name '*.cpp' | sort | xargs -P "$JOBS" -I{} bash -c 'compile_one "$@"' _ {}

ok=$(find "$BASE" -name '*.o' | wc -l | tr -d ' ')
fail=$(find "$LOGS" -name '*.log' | wc -l | tr -d ' ')
echo "[build] compiled $ok, failed $fail"
if [ "$fail" -gt 0 ]; then
  echo "[build] top failure reasons:"
  grep -rhoE 'error: .*' "$LOGS" 2>/dev/null | sed -E 's/0x[0-9a-f]+/0x_/g; s/[0-9]+/N/g' \
    | sort | uniq -c | sort -rn | head -8 | sed 's/^/    /'
  echo "[build] (per-file logs under $LOGS)"
fi
exit 0
