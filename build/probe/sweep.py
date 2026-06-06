#!/usr/bin/env python3
# Opt-level sweep (experiment, not committed tooling): recompile every matches.tsv source at
# several -O levels and count how many functions stay byte-exact. Tells us which optimization
# level the target was actually built with. Run in the guest.
import subprocess, os, sys, re

REPO = "/Users/fionera/Downloads/GalaxyOnFire2/gof2-decomp"
os.chdir(REPO)
SO = "../_work/bins/android_2.0.16_libgof2hdaa.so"
TC = "/opt/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64"
CXX = TC + "/bin/clang++"
BASE = "-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -fpic -frtti -Iinclude".split()

# load registry
entries = []
for ln in open("matches.tsv"):
    if ln.startswith("#") or not ln.strip():
        continue
    name, vaddr, n, obj, sym, status = ln.rstrip("\n").split("\t")
    src = "src/" + obj[len("build/obj/"):].rsplit(".o", 1)[0] + ".cpp"
    entries.append(dict(name=name, vaddr=vaddr, n=int(n), obj=obj, sym=sym, status=status, src=src))

# unique sources
srcs = sorted({e["src"] for e in entries})

def compile_all(optflags):
    ok, errs = 0, []
    for s in srcs:
        o = "build/obj/" + s[len("src/"):].rsplit(".cpp", 1)[0] + ".o"
        os.makedirs(os.path.dirname(o), exist_ok=True)
        r = subprocess.run([CXX] + BASE + optflags + ["-c", s, "-o", o],
                           capture_output=True, text=True)
        if r.returncode == 0:
            ok += 1
        else:
            errs.append((s, r.stderr.splitlines()[-1] if r.stderr else "?"))
    return ok, errs

def pct_of(e):
    r = subprocess.run([sys.executable, "tools/gofdiff.py", "--so", SO, "--vaddr", e["vaddr"],
                        "--n", str(e["n"]), "--obj", e["obj"], "--sym", e["sym"]],
                       capture_output=True, text=True)
    m = re.search(r"MATCH ([0-9.]+)%", r.stdout)
    return float(m.group(1)) if m else 0.0

LEVELS = [("-O2", ["-O2"]), ("-Os", ["-Os"]), ("-Oz", ["-Oz"]), ("-O1", ["-O1"]), ("-O3", ["-O3"])]
print("level  compiled  exact(100pct)  >=90pct   mean   (of %d functions)" % len(entries))
results = {}
for label, flags in LEVELS:
    ok, errs = compile_all(flags)
    pcts = [pct_of(e) for e in entries]
    exact = sum(1 for p in pcts if p >= 100.0)
    near = sum(1 for p in pcts if p >= 90.0)
    mean = sum(pcts) / len(pcts)
    results[label] = pcts
    print("%-5s  %8d  %11d  %5d   %5.1f" % (label, ok, exact, near, mean))
    if errs:
        print("        (%d compile errors, e.g. %s: %s)" % (len(errs), errs[0][0], errs[0][1][:80]))

# Which functions FLIP from non-exact@O2 to exact at some other level? (the unlock signal)
o2 = results["-O2"]
print("\nFunctions NOT exact at -O2 that become exact at another level:")
flips = 0
for i, e in enumerate(entries):
    if o2[i] < 100.0:
        better = [(lab, results[lab][i]) for lab, _ in LEVELS if results[lab][i] > o2[i] + 0.05]
        if any(p >= 100.0 for _, p in better):
            flips += 1
            print("  %-26s O2=%.1f -> %s" % (e["name"], o2[i],
                  ", ".join("%s=%.1f" % (l, p) for l, p in better)))
print("total flips to exact: %d" % flips)
