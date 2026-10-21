#!/usr/bin/env python3
"""apply_wave_results.py — apply a fix-wave's structured results to the tree, serially.

The parallel agents only edit their own component's files; anything cross-cutting they REPORT
instead of editing (so the wave stays race-free). This script, run once after the wave, applies:
  * caller_rewrites  [{file, find, replace, why}]  — exact edits in caller/other files (serial).
  * missing          ["MISSING FIELD Class.field_0xNN (type)" | "MISSING DECL ..."]  -> reconcile.py
                      for FIELD lines; DECL/other lines are appended to docs/DEFERRED.md.
  * deferred         [{symbol, reason, label}]      -> docs/DEFERRED.md (re-queued next wave).

Reads the results JSON from stdin (the object returned by tools/wave_workflow.js) or a file arg.
Prints a summary. Idempotent for caller_rewrites (a find already absent is reported, not failed).
"""
import json
import os
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
REPO = os.path.dirname(HERE)
DEFERRED = os.path.join(REPO, "docs", "DEFERRED.md")


# Independent parallel agents must NEVER apply cross-hierarchy edits piecemeal — a base-virtual
# rename or a dtor-virtuality flip applied in isolation breaks every derived TU (the setPosition3
# cascade that cost a whole wave). So only auto-apply LOCAL call-site edits in .cpp files; anything
# that edits a header or touches the type system (virtual/override/class/struct/base decls) is
# REFUSED and deferred for a dedicated coordinated change.
RISKY = ("virtual", "override", "class ", "struct ", " : public", " : private", " : protected")


def _risky(r):
    if not r["file"].endswith(".cpp"):
        return f"edits a header ({r['file']}) — needs coordinated change"
    blob = (r.get("find", "") + "\n" + (r.get("replace") or ""))
    repl = r.get("replace") or ""
    # reinterpret_cast<T*>(nullptr) is ill-formed C++ — agents sometimes emit it to fill a new
    # ref/pointer param; refuse so the owning TU handles the new arg correctly (use static_cast).
    if "reinterpret_cast" in repl and "nullptr" in repl:
        return "ill-formed reinterpret_cast<...>(nullptr) — owning TU must pass the real arg"
    for kw in RISKY:
        if kw in blob:
            return f"touches the type system ('{kw.strip()}') — needs coordinated change"
    return None


def apply_caller_rewrites(rewrites):
    applied, skipped, refused = 0, [], []
    for r in rewrites:
        why = _risky(r)
        if why:
            refused.append(f"{r['file']}: REFUSED — {why}")
            continue
        path = os.path.join(REPO, r["file"])
        if not os.path.exists(path):
            skipped.append(f"{r['file']}: no such file")
            continue
        txt = open(path).read()
        if r["find"] not in txt:
            skipped.append(f"{r['file']}: find-string absent (already applied?)")
            continue
        if r.get("replace") is None:
            skipped.append(f"{r['file']}: no replacement given")
            continue
        open(path, "w").write(txt.replace(r["find"], r["replace"]))
        applied += 1
    return applied, skipped + refused, refused


def route_missing(missing):
    field_lines = [m for m in missing if m.strip().startswith("MISSING FIELD")]
    other = [m for m in missing if not m.strip().startswith("MISSING FIELD")]
    reconciled = ""
    if field_lines:
        p = subprocess.run([sys.executable, os.path.join(HERE, "reconcile.py")],
                           input="\n".join(field_lines), text=True, capture_output=True)
        reconciled = p.stdout.strip()
    return reconciled, other


def append_deferred(deferred, other_missing):
    if not deferred and not other_missing:
        return
    os.makedirs(os.path.dirname(DEFERRED), exist_ok=True)
    with open(DEFERRED, "a") as f:
        for d in deferred:
            f.write(f"- [ ] `{d.get('symbol','?')}` ({d.get('label','')}): {d.get('reason','')}\n")
        for m in other_missing:
            f.write(f"- [ ] {m}\n")


def main():
    data = json.load(open(sys.argv[1])) if len(sys.argv) > 1 else json.load(sys.stdin)
    rewrites = data.get("caller_rewrites", [])
    missing = data.get("missing", [])
    deferred = data.get("deferred", [])
    fixed = data.get("fixed", [])

    applied, skipped, refused = apply_caller_rewrites(rewrites)
    reconciled, other_missing = route_missing(missing)
    # refused cross-hierarchy rewrites are real work that needs a coordinated pass -> ledger.
    append_deferred(deferred, other_missing + [f"REFUSED caller_rewrite: {r}" for r in refused])

    print(f"fixed reported: {len(fixed)}")
    print(f"caller_rewrites: applied {applied}, skipped/refused {len(skipped)}")
    for s in skipped:
        print(f"    - {s}")
    if reconciled:
        print("reconcile (fields):")
        print("    " + reconciled.replace("\n", "\n    "))
    print(f"deferred logged: {len(deferred)} + {len(other_missing)} non-field missing "
          f"-> docs/DEFERRED.md")
    return 0


if __name__ == "__main__":
    sys.exit(main())
