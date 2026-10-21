#!/usr/bin/env python3
"""Local-code gap report: bytes of .text that lie between consecutive exported functions.

The original is stripped: its local (static / internal-linkage / hidden) functions have no
symbol, but they occupy the gaps between exported functions. Because our .text is laid out in the
original's order (section-ordering file), the same neighbour pair brackets the same local code on
our side. A gap that is larger in the original is local code we have not decompiled yet (or
inlined differently); the neighbouring exported function names tell which TU owns it.

Usage: gapdiff.py <original.so> <ours.so> <llvm-nm> [--top N] [--json out]
"""
import argparse, json, subprocess


def funcs(nm, path):
    out = subprocess.run([nm, "-D", "-S", "-n", "--defined-only", path],
                         capture_output=True, text=True).stdout
    fs = {}
    for ln in out.splitlines():
        p = ln.split()
        if len(p) == 4 and p[2] in "TtWw":
            a, sz = int(p[0], 16) & ~1, int(p[1], 16)
            if sz and p[3] not in fs:
                fs[p[3]] = (a, sz)
    return fs


def text_range(path):
    out = subprocess.run(["readelf", "-SW", path], capture_output=True, text=True).stdout
    for ln in out.splitlines():
        if " .text " in ln:
            p = ln.split("]")[1].split()
            return int(p[2], 16), int(p[2], 16) + int(p[4], 16)
    raise SystemExit("no .text")


def gaps(fs, lo, hi):
    """[(prev_name, next_name, gap_bytes)] over address-sorted exported functions (aliases merged)."""
    by = sorted({a: (a, sz, n) for n, (a, sz) in fs.items()}.values())
    res, end, prev = [], lo, "<.text start>"
    for a, sz, n in by:
        if a > end:
            res.append((prev, n, a - end))
        if a + sz > end:
            end, prev = a + sz, n
    if hi > end:
        res.append((prev, "<.text end>", hi - end))
    return res


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("original"); ap.add_argument("ours"); ap.add_argument("nm")
    ap.add_argument("--top", type=int, default=40); ap.add_argument("--json")
    a = ap.parse_args()
    of, uf = funcs(a.nm, a.original), funcs(a.nm, a.ours)
    common = {n: v for n, v in of.items() if n in uf}
    og = gaps(common, *text_range(a.original))
    ucommon = {n: uf[n] for n in common}
    ug = {(p, n): g for p, n, g in gaps(ucommon, *text_range(a.ours))}
    rows = []
    for p, n, g in og:
        ours = ug.get((p, n), 0)
        rows.append(dict(prev=p, next=n, orig=g, ours=ours, missing=g - ours))
    tot_o = sum(r["orig"] for r in rows); tot_u = sum(ug.values())
    print(f"local-code gaps: orig {tot_o} B in {len(rows)} gaps, ours {tot_u} B "
          f"(bracketed by the same neighbours: {sum(r['ours'] for r in rows)} B)")
    for r in sorted(rows, key=lambda r: -r["missing"])[:a.top]:
        print(f"{r['missing']:>8} {r['orig']:>8} {r['ours']:>8}  {r['prev']}  ..  {r['next']}")
    if a.json:
        json.dump(rows, open(a.json, "w"), indent=0)


if __name__ == "__main__":
    main()
