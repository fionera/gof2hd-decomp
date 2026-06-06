#!/usr/bin/env bash
# integrity.sh — anti-cheat guard. The verification tooling and the target binary are the trust
# anchors; a worker with filesystem access must not alter them to fake a match. We hash them and
# verify they're unchanged, and confirm git sees no changes to tools/ or the Makefile.
#   tools/integrity.sh --bless   # record current hashes as the baseline (do this once, clean)
#   tools/integrity.sh           # verify; nonzero exit if tooling/target/gate changed
cd "$(dirname "$0")/.."
TARGET="../_work/bins/android_2.0.16_libgof2hdaa.so"
WATCH=(tools/gofdiff.py tools/try.sh tools/verify.sh tools/coverage.py work/collect.sh work/collect_class.sh Makefile "$TARGET")
REF="tools/SHA256SUMS"
SHA="shasum -a 256"
if [ "$1" = "--bless" ]; then $SHA "${WATCH[@]}" > "$REF"; echo "blessed $(wc -l < "$REF") files"; exit 0; fi
[ -f "$REF" ] || { echo "no baseline ($REF); run: tools/integrity.sh --bless"; exit 1; }
if $SHA -c "$REF" --status; then
  # also: git must report tools/ and Makefile clean (catches edits even if hashes were re-blessed)
  if git rev-parse --git-dir >/dev/null 2>&1 && ! git diff --quiet -- tools Makefile 2>/dev/null; then
    echo "INTEGRITY FAIL: uncommitted changes to tools/ or Makefile"; git --no-pager diff --stat -- tools Makefile; exit 1
  fi
  echo "integrity OK (tooling + target unchanged)"
else
  echo "INTEGRITY FAIL: a watched file's hash changed!"; $SHA -c "$REF" 2>/dev/null | grep -v OK; exit 1
fi
