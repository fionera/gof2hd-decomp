#!/bin/sh
# Orchestrator: lock gate gains, keep pre-approved genuine original imports, refresh doc numbers.
set -e
cd "$(dirname "$0")/.."
python3 tools/ratchet.py --update-baseline >/dev/null
python3 - <<'P'
import json
p='tools/verify_baseline.json'; d=json.load(open(p))
for n in ['fprintf','printf','sscanf','memalign','__aeabi_l2d']:   # genuine imports of the original .so
    if n not in d['imports']: d['imports'].append(n)
d['imports'].sort(); json.dump(d,open(p,'w'),indent=1)
open(p,'a').write('\n')
P
cp cmake-build-match/verify/report.json report.json
python3 _work/docnums.py >/dev/null
python3 -c "import json;print(json.load(open('tools/verify_baseline.json'))['verify'])"
