#!/usr/bin/env python3
"""Rewrite the current-state verify numbers in docs/*.md from tools/verify_baseline.json."""
import json, re
d = json.load(open('tools/verify_baseline.json'))['verify']
new = f"**byte_exact {d['byte_exact']}, linked_exact {d['linked_exact']}, avg {d['avg_match']}%**"
for p in ('docs/VALIDATION.md', 'docs/IDENTICALITY.md'):
    s = open(p).read()
    s2 = re.sub(r"\*\*byte_exact \d+, linked_exact \d+, avg [\d.]+%\*\*", new, s)
    open(p, 'w').write(s2)
    print(p, 'updated' if s2 != s else 'unchanged')
