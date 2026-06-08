#!/usr/bin/env bash
# CI regression gate: rebuild, then re-diff every function in matches.tsv against the target.
# Fails if any 'exact' function drops below 100% or any 'near' drops below 90%.
# Run in the guest:  make verify   (or  tools/verify.sh)
set -u
here="$(cd "$(dirname "$0")/.." && pwd)"; cd "$here"
SO="${SO:-../_work/bins/android_2.0.16_libgof2hdaa.so}"
# Fail loud, not silent: without the ARM objdump every diff returns empty and scores 0%, which
# masquerades as "all regressed" (and can drown out a real regression). This gate MUST run in the
# guest, e.g. `orb run make verify`.
OBJDUMP="${ARM_OBJDUMP:-arm-linux-gnueabihf-objdump}"
command -v "$OBJDUMP" >/dev/null 2>&1 || { echo "ABORT: '$OBJDUMP' not found — run this in the OrbStack guest (orb run make verify), not the host."; exit 1; }
# Anti-cheat: the differ/wrapper/flags/target must be untampered (workers have fs access).
bash tools/integrity.sh || { echo "ABORT: integrity check failed"; exit 1; }
# Quality gate: recovered source must be genuine C/C++, not inline-asm/naked reproductions.
if grep -rlnE '\b(__asm__|asm[[:space:]]*\(|asm[[:space:]]*volatile|__attribute__\(\(naked)' src/ 2>/dev/null; then
  echo "REJECTED: inline asm / naked found in src/ (not a real decompilation)"; exit 1
fi
# Always build CLEAN: objects don't depend on the Makefile, so a CXXFLAGS change (e.g. opt level)
# would otherwise leave stale objects and the gate would diff the wrong build. The trust anchor
# must reflect the current flags+source exactly.
NPROC="$(nproc 2>/dev/null || echo 4)"
# Build + diff in PARALLEL across all cores (the gate was the cadence bottleneck at >1600 fns).
make clean >/dev/null 2>&1
make -j"$NPROC" NDK="${NDK:-/opt/android-ndk-r18b}" >/dev/null 2>&1 || { echo "BUILD FAILED"; exit 1; }
# Per-entry check (one matches.tsv line -> PASS/FAIL row). Run via xargs -P across all cores;
# each gofdiff is independent (own .o + read-only target), so there are no races.
check_one() {
  IFS=$'\t' read -r name vaddr nbytes obj sym status <<< "$1"
  case "$name" in \#*|"") return;; esac
  local pct need
  pct=$(python3 tools/gofdiff.py --so "$SO" --vaddr "$vaddr" --n "$nbytes" --obj "$obj" --sym "$sym" 2>/dev/null | grep -oE '[0-9]+\.[0-9]+%' | tr -d '%' | head -1)
  pct=${pct:-0}; need=100; [ "$status" = near ] && need=90
  if awk "BEGIN{exit !($pct+0 >= $need)}"; then printf 'PASS\t%s\t%s\t%s\n' "$name" "$pct" "$status"
  else printf 'FAIL\t%s\t%s\t%s\n' "$name" "$pct" "$need"; fi
}
export -f check_one; export SO
results=$(grep -vE '^#|^$' matches.tsv | xargs -P"$NPROC" -d '\n' -I LINE bash -c 'check_one "$@"' _ LINE)
echo "$results" | sort | awk -F'\t' '$1=="PASS"{printf "  PASS  %-24s %6s%%  (%s)\n",$2,$3,$4} $1=="FAIL"{printf "  FAIL  %-24s %6s%%  (need >=%s%%)\n",$2,$3,$4}'
n=$(printf '%s\n' "$results" | grep -c .); fail=$(printf '%s\n' "$results" | grep -c '^FAIL')
echo "---"; echo "$n checked, $fail regressed"
exit $((fail>0))
