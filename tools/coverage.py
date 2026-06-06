#!/usr/bin/env python3
"""
coverage.py — decompilation coverage report.

Reads matches.tsv (recovered functions) against the target's function universe and reports
overall + per-subsystem coverage. The "identified" denominator is the named-function count
(what we know the identity of); the "total" is all discovered functions.

Usage:  python3 tools/coverage.py
"""
import os, collections, re

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# Target universe (Android libgof2hdaa.so): discovered functions / named (demangled) functions.
TOTAL_FUNCS = 7450
NAMED_FUNCS = 5054

def subsystem(name):
    # group by leading C++ namespace/class
    m = re.match(r'([A-Za-z_][\w]*)(::|<)', name)
    if m: return m.group(1)
    if name.startswith('Array'): return 'Array<T>'
    return '(free)'

def main():
    reg = os.path.join(ROOT, 'matches.tsv')
    rows = []
    for line in open(reg):
        if line.startswith('#') or not line.strip(): continue
        p = line.rstrip('\n').split('\t')
        if len(p) >= 6: rows.append((p[0], p[5]))
    exact = sum(1 for _, s in rows if s == 'exact')
    near = sum(1 for _, s in rows if s == 'near')
    by = collections.Counter(subsystem(n) for n, _ in rows)
    print("GOF2 decomp coverage")
    print("  recovered: %d functions  (%d exact, %d near)" % (len(rows), exact, near))
    print("  of %d named / %d total target functions  =>  %.3f%% of named, %.3f%% of all"
          % (NAMED_FUNCS, TOTAL_FUNCS, 100.0*len(rows)/NAMED_FUNCS, 100.0*len(rows)/TOTAL_FUNCS))
    print("  by subsystem:")
    for k, v in by.most_common():
        print("    %-16s %d" % (k, v))
    print("\n  (update matches.tsv as functions land; `make verify` gates regressions.)")

if __name__ == '__main__': main()
