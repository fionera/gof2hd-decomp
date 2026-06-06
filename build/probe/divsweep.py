#!/usr/bin/env python3
# Divergent-set opt sweep: the UNBIASED test. Compile every authored class method that NEVER
# reached exact at -O2 (i.e. not in matches.tsv) at -O2/-Os/-Oz and see which level matches.
# A flip (fail@O2 -> exact@Os/Oz) is direct evidence of the target's real optimization level.
import subprocess, os, sys, re, glob

REPO = "/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"
os.chdir(REPO)
SO = "../_work/bins/android_2.0.16_libgof2hdaa.so"
TC = "/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64"
CXX = TC + "/bin/clang++"
BASE = "-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -fpic -frtti -Iinclude".split()

matched = set()
for ln in open("matches.tsv"):
    if ln.startswith("#") or not ln.strip():
        continue
    matched.add(ln.split("\t")[0])

# build divergent worklist from each class's methods.tsv
work = []
for mt in glob.glob("work/classes/*/methods.tsv"):
    cls_dir = os.path.dirname(mt)
    for ln in open(mt):
        parts = ln.rstrip("\n").split("\t")
        if len(parts) < 4:
            continue
        key, method, vaddr, n = parts[0], parts[1], parts[2], parts[3]
        if key in matched:
            continue  # already exact at -O2 (collected) -- skip
        src = os.path.join(cls_dir, "src", key + ".cpp")
        if not os.path.exists(src):
            continue
        try:
            nn = int(n)
        except ValueError:
            continue
        txt = open(src).read()
        handtuned = bool(re.search(r"neon|vdupq|vst1q|vld1|volatile|disable_tail_calls|minsize|__asm|naked", txt))
        work.append(dict(key=key, method=method, vaddr=vaddr, n=nn, src=src, handtuned=handtuned))

def pct(src, sym, vaddr, n, optflag):
    o = "/tmp/dv.o"
    r = subprocess.run([CXX] + BASE + [optflag, "-c", src, "-o", o],
                       capture_output=True, text=True)
    if r.returncode != 0:
        return None
    d = subprocess.run([sys.executable, "tools/gofdiff.py", "--so", SO, "--vaddr", vaddr,
                        "--n", str(n), "--obj", o, "--sym", sym], capture_output=True, text=True)
    m = re.search(r"MATCH ([0-9.]+)%", d.stdout)
    return float(m.group(1)) if m else None

print("divergent authored functions (not exact at -O2): %d" % len(work))
rows = []
for w in work:
    p2 = pct(w["src"], w["method"], w["vaddr"], w["n"], "-O2")
    ps = pct(w["src"], w["method"], w["vaddr"], w["n"], "-Os")
    pz = pct(w["src"], w["method"], w["vaddr"], w["n"], "-Oz")
    rows.append((w, p2, ps, pz))

def fmt(x): return "  err" if x is None else "%5.1f" % x
flips_os = flips_oz = 0
print("\n%-30s  O2    Os    Oz   tuned" % "function")
for w, p2, ps, pz in sorted(rows, key=lambda r: -(max((r[2] or 0), (r[3] or 0)) - (r[1] or 0))):
    flip = ""
    if (ps is not None and ps >= 100.0 and (p2 is None or p2 < 100.0)): flips_os += 1; flip += " <FLIP-Os"
    if (pz is not None and pz >= 100.0 and (p2 is None or p2 < 100.0)): flips_oz += 1; flip += " <FLIP-Oz"
    print("%-30s %s %s %s   %s%s" % (w["key"][:30], fmt(p2), fmt(ps), fmt(pz),
                                     "Y" if w["handtuned"] else ".", flip))

import statistics
def meanof(i):
    vals = [r[i] for r in rows if r[i] is not None]
    return statistics.mean(vals) if vals else 0
print("\n--- summary over %d divergent functions ---" % len(rows))
print("mean%%:  O2=%.1f  Os=%.1f  Oz=%.1f" % (meanof(1), meanof(2), meanof(3)))
print("exact@O2=%d  exact@Os=%d  exact@Oz=%d" % (
    sum(1 for r in rows if r[1] and r[1] >= 100),
    sum(1 for r in rows if r[2] and r[2] >= 100),
    sum(1 for r in rows if r[3] and r[3] >= 100)))
print("NEW flips to exact:  via -Os=%d   via -Oz=%d" % (flips_os, flips_oz))
# natural-source-only view (exclude hand-tuned, the cleanest signal)
nat = [r for r in rows if not r[0]["handtuned"]]
print("natural-source only (%d): exact@O2=%d exact@Os=%d exact@Oz=%d" % (
    len(nat),
    sum(1 for r in nat if r[1] and r[1] >= 100),
    sum(1 for r in nat if r[2] and r[2] >= 100),
    sum(1 for r in nat if r[3] and r[3] >= 100)))
