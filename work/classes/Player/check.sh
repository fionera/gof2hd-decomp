#!/usr/bin/env bash
# check.sh — run try.sh for each "key method vaddr nbytes" line on stdin, print one-line result.
cd /Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp
while read key method vaddr n; do
  [ -z "$key" ] && continue
  out=$(bash tools/try.sh "work/classes/Player/src/$key.cpp" "$method" "$vaddr" "$n" </dev/null 2>&1)
  res=$(printf '%s\n' "$out" | grep -E 'MATCH|EXACT|COMPILE_ERROR' | head -1)
  if printf '%s\n' "$out" | grep -q 'EXACT MATCH'; then
    printf 'OK    %-30s %s\n' "$key" "$res"
  else
    printf 'FAIL  %-30s %s\n' "$key" "$res"
  fi
done
