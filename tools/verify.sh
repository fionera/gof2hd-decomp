#!/usr/bin/env bash
# CI regression gate: rebuild, then re-diff every function in matches.tsv against the target.
# Fails if any 'exact' function drops below 100% or any 'near' drops below 90%.
# Run in the guest:  make verify   (or  tools/verify.sh)
set -u
here="$(cd "$(dirname "$0")/.." && pwd)"; cd "$here"
SO="${SO:-../_work/bins/android_2.0.16_libgof2hdaa.so}"
# Anti-cheat: the differ/wrapper/flags/target must be untampered (workers have fs access).
bash tools/integrity.sh || { echo "ABORT: integrity check failed"; exit 1; }
# Quality gate: recovered source must be genuine C/C++, not inline-asm/naked reproductions.
if grep -rlnE '\b(__asm__|asm[[:space:]]*\(|asm[[:space:]]*volatile|__attribute__\(\(naked)' src/ 2>/dev/null; then
  echo "REJECTED: inline asm / naked found in src/ (not a real decompilation)"; exit 1
fi
make NDK="${NDK:-/opt/android-ndk-r18b}" >/dev/null 2>&1 || { echo "BUILD FAILED"; exit 1; }
fail=0; n=0
while IFS=$'\t' read -r name vaddr nbytes obj sym status; do
  [[ "$name" =~ ^#|^$ ]] && continue
  n=$((n+1))
  out=$(python3 tools/gofdiff.py --so "$SO" --vaddr "$vaddr" --n "$nbytes" --obj "$obj" --sym "$sym" 2>/dev/null | head -1)
  pct=$(echo "$out" | grep -oE '[0-9]+\.[0-9]+%' | tr -d '%')
  pct=${pct:-0}
  need=100; [[ "$status" == near ]] && need=90
  if awk "BEGIN{exit !($pct+0 >= $need)}"; then
    printf "  PASS  %-22s %6s%%  (%s)\n" "$name" "$pct" "$status"
  else
    printf "  FAIL  %-22s %6s%%  (need >=%s%%)\n" "$name" "$pct" "$need"; fail=$((fail+1))
  fi
done < matches.tsv
echo "---"; echo "$n checked, $fail regressed"
exit $((fail>0))
