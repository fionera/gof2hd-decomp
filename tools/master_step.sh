#!/usr/bin/env bash
# master_step.sh RESULTS_JSON [wave_prepare args...] — one master-loop iteration AFTER a fix wave.
#
#   1. apply the wave's caller_rewrites / missing-fields / deferred (serial, race-free)
#   2. re-run verify (regenerate report.json + missing/extra/wrong_type)
#   3. scope_filter -> in-scope counts (the "done" gate)
#   4. dispatch_worklist -> fresh worklist.json
#   5. commit the wave's progress
#   6. prepare the next wave script (passthrough args after RESULTS_JSON, default: absent)
#
# Prints the in-scope counts so the driver can decide whether to stop (all zero) or loop.
set -uo pipefail
HERE="$(cd "$(dirname "$0")" && pwd)"
REPO="$(cd "$HERE/.." && pwd)"
cd "$REPO"
RESULTS="${1:?usage: master_step.sh RESULTS_JSON [wave_prepare args...]}"; shift
PREP_ARGS=("$@"); [ ${#PREP_ARGS[@]} -eq 0 ] && PREP_ARGS=(--kinds absent --limit 24 --max-entries 8)

echo "== 1. apply wave results =="
python3 tools/apply_wave_results.py "$RESULTS"

echo "== 2. verify (rebuild changed TUs + diff) =="
python3 tools/verify/verify.py --build-dir cmake-build-match/verify >cmake-build-match/verify/_verify_run.log 2>&1
tail -3 cmake-build-match/verify/_verify_run.log

echo "== 3. scope_filter =="
python3 tools/scope_filter.py

echo "== 4. dispatch =="
python3 tools/dispatch_worklist.py | sed 's/^/   /'

echo "== 5. commit =="
git add -A
if git diff --cached --quiet; then
  echo "   (nothing to commit)"
else
  N=$(python3 -c "import json;c=json.load(open('cmake-build-match/verify/scope_counts.json'));print(f\"absent {c['absent']} wrong_type {c['wrong_type']} extra {c['extra']}\")")
  git commit -q -m "fix wave: in-scope $N

Co-Authored-By: Claude Opus 4.8 (1M context) <noreply@anthropic.com>
Claude-Session: https://claude.ai/code/session_01Vq3g9jxkb38oUx9XXFf4Ht" && echo "   committed ($N)"
fi

echo "== 6. prepare next wave =="
python3 tools/wave_prepare.py "${PREP_ARGS[@]}"

echo "== DONE-CHECK =="
python3 -c "
import json,sys
c=json.load(open('cmake-build-match/verify/scope_counts.json'))
done = all(v==0 for v in c.values())
print('SCOPE_COUNTS', json.dumps(c))
print('ALL_ZERO' if done else 'CONTINUE')
"
