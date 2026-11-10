#!/usr/bin/env python3
"""Reproduce the round-6 build-provenance measurements.

Usage:
  python3 _work/reconstructions/build_provenance_round6.py \
      --repo /home/debian/GalaxyOnFire2/wt/w11119 \
      --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
      --build cmake-build-match

The script prints:
  * the 192-TU original link order with per-TU exactness;
  * the 17 residual exported functions;
  * the per-TU -g sweep (-g0/-g1/-g2/-g3/-gline-tables-only/-gmlt);
  * the per-TU -O sweep (-O1/-O2/-Os/-O3);
  * per-TU call-site and frame-pointer fingerprints;
  * the byte-identity of the metadata sections.

It needs the NDK r18b tree in <repo>/.cache/ndk/android-ndk-r18b (the normal
cmake --preset default cache) and a fresh cmake-build-match build.  The sweeps
compile every TU several times; use --no-sweeps to skip them.
"""

import argparse
import collections
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
import threading
from concurrent.futures import ThreadPoolExecutor

ROOT = None
OBJCOPY = None
OBJDUMP = None


def run(cmd, **kw):
    return subprocess.run(cmd, check=True, capture_output=True, text=True, **kw)


def readelf_syms(path):
    out = run(["readelf", "-sW", path]).stdout
    funcs = {}
    objects = set()
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 8 and p[3] in ("FUNC", "OBJECT"):
            objects.add(p[7])
            if p[3] == "FUNC":
                try:
                    funcs[p[7]] = (int(p[1], 16), int(p[2]))
                except ValueError:
                    pass
    return funcs, objects


def section_bytes(path, name):
    out = run(["readelf", "-SW", path]).stdout
    for line in out.splitlines():
        m = re.match(
            r"\s*\[\s*\d+\]\s+(\S+)\s+\S+\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)",
            line,
        )
        if m and m.group(1) == name:
            off = int(m.group(3), 16)
            size = int(m.group(4), 16)
            with open(path, "rb") as f:
                f.seek(off)
                return f.read(size)
    return b""


def text_dump(obj, tmpdir, tag):
    """Concatenated .text.* bytes of a relocatable object, as a SHA-256."""
    out = os.path.join(tmpdir, "text_%s.bin" % tag)
    run([OBJCOPY, "-O", "binary", "--only-section=.text*", obj, out])
    with open(out, "rb") as f:
        h = hashlib.sha256(f.read()).hexdigest()
    os.unlink(out)
    return h


def object_map(build, ofuncs):
    """mangled symbol -> repo-relative TU path."""
    sym2tu = {}
    tu_syms = collections.defaultdict(list)
    for dirpath, _dirnames, filenames in os.walk(os.path.join(build, "CMakeFiles", "gof2.dir")):
        for fn in filenames:
            if not fn.endswith(".obj"):
                continue
            obj = os.path.join(dirpath, fn)
            rel = os.path.relpath(obj, os.path.join(build, "CMakeFiles", "gof2.dir"))[:-4]
            out = run(["nm", "-A", "--defined-only", obj]).stdout
            for line in out.splitlines():
                try:
                    _left, typ, name = line.rsplit(" ", 2)
                except ValueError:
                    continue
                name = name.strip()
                if not name:
                    continue
                sym2tu.setdefault(name, rel)
                if typ in ("T", "t") and name in ofuncs:
                    tu_syms[rel].append(name)
    return sym2tu, tu_syms


def print_tu_table(repo, build, original):
    report = json.load(open(os.path.join(repo, "report.json")))
    ofuncs, _ = readelf_syms(original)
    sym2tu, tu_syms = object_map(build, ofuncs)
    tu_rows = collections.defaultdict(lambda: {"n": 0, "exact": 0, "res": []})
    for row in report["rows"]:
        tu = sym2tu.get(row["name"])
        if not tu:
            continue
        tu_rows[tu]["n"] += 1
        if row["byte"]:
            tu_rows[tu]["exact"] += 1
        else:
            tu_rows[tu]["res"].append(row["name"])

    order = [
        l.strip()
        for l in open(os.path.join(repo, "tools", "source_order.txt"))
        if l.strip().startswith("src/")
    ]
    print("## per-TU link order")
    print("| # | TU | strong funcs | strong bytes | exported | exact | residual | first strong addr |")
    print("|---:|---|---:|---:|---:|---:|---|---:|")
    for i, tu in enumerate(order):
        ss = tu_syms.get(tu, [])
        addrs = [ofuncs[s][0] for s in ss if s in ofuncs]
        first = min(addrs) if addrs else None
        rr = tu_rows.get(tu, {"n": 0, "exact": 0, "res": []})
        res = ", ".join("`%s`" % x for x in rr["res"])
        print(
            "| %d | `%s` | %d | %d | %d | %d | %s | %s |"
            % (
                i,
                tu,
                len(ss),
                sum(ofuncs[s][1] for s in ss if s in ofuncs),
                rr["n"],
                rr["exact"],
                res,
                ("0x%x" % first) if first is not None else "-",
            )
        )

    print("\n## residual exported functions")
    print("| TU | function | pct | size | mangled |")
    print("|---|---|---:|---:|---|")
    for row in report["rows"]:
        if row["byte"]:
            continue
        dem = run(["c++filt", row["name"]]).stdout.strip()
        print(
            "| `%s` | `%s` | %s | %d | `%s` |"
            % (sym2tu.get(row["name"], "?"), dem, row["pct"], row["osize"], row["name"])
        )


def sweep(repo, build, levels, base_flag):
    """Compile every TU with each level and compare .text.* to the canonical object."""
    cc = json.load(open(os.path.join(build, "compile_commands.json")))
    results = []

    def one(idx_e):
        idx, entry = idx_e
        tid = threading.get_ident()
        cmd = entry["command"]
        m = re.search(r"-o\s+(\S+)", cmd)
        canon = m.group(1)
        if not os.path.isabs(canon):
            canon = os.path.join(build, canon)
        if not os.path.exists(canon):
            return entry["file"], None, {}
        base = re.sub(r"-o\s+\S+", "", cmd)
        base = re.sub(r"-c\s+\S+", "", base)
        base = re.sub(r"(?<!\S)%s(?!\S)" % re.escape(base_flag), "", base)
        src = entry["file"]
        tmpdir = tempfile.mkdtemp(prefix="gof2r6_")
        try:
            canon_hash = text_dump(canon, tmpdir, "canon")
            res = {}
            for lv in levels:
                out = os.path.join(tmpdir, "v.o")
                r = subprocess.run(
                    base + " " + lv + " -c " + src + " -o " + out,
                    shell=True,
                    capture_output=True,
                    text=True,
                )
                if r.returncode:
                    res[lv] = "FAIL"
                    continue
                try:
                    res[lv] = text_dump(out, tmpdir, "v")
                except Exception:
                    res[lv] = "DUMPFAIL"
            return entry["file"], canon_hash, res
        finally:
            for fn in os.listdir(tmpdir):
                os.unlink(os.path.join(tmpdir, fn))
            os.rmdir(tmpdir)

    with ThreadPoolExecutor(max_workers=2) as ex:
        for i, r in enumerate(ex.map(one, enumerate(cc))):
            results.append(r)
            if i % 20 == 0:
                print("  sweep %d/%d" % (i, len(cc)), file=sys.stderr)
    return results


def print_sweep(name, results, levels):
    print("\n## %s" % name)
    print("| variant | TUs identical to canonical | TUs changed |")
    print("|---|---:|---:|")
    for lv in levels:
        same = 0
        non = 0
        for _f, canon_hash, res in results:
            if canon_hash is None:
                continue
            if res.get(lv) == canon_hash:
                same += 1
            else:
                non += 1
        print("| `%s` | %d | %d |" % (lv, same, non))


def call_counts(path, sym):
    out = run([OBJDUMP, "-d", path]).stdout
    cur = None
    counts = collections.Counter()
    for line in out.splitlines():
        m = re.match(r"^([0-9a-f]+) <(.+)>:$", line)
        if m:
            cur = m.group(2).split("@@")[0]
            continue
        if cur is None:
            continue
        if re.search(r"\b(?:bl|blx)\s+[0-9a-f]+ <%s@plt>" % re.escape(sym), line):
            counts[cur] += 1
    return counts


def fingerprints(repo, build, original):
    ofuncs, _ = readelf_syms(original)
    sym2tu, _ = object_map(build, ofuncs)
    syms = [
        "__stack_chk_fail",
        "_Unwind_Resume",
        "__cxa_throw",
        "__cxa_guard_acquire",
        "__cxa_atexit",
        "_ZdlPv",
        "_ZdaPv",
        "__assert2",
    ]
    print("\n## per-TU call-site fingerprints")
    print("| symbol | original calls | canonical calls | TUs differing |")
    print("|---|---:|---:|---:|")
    for sym in syms:
        co = call_counts(original, sym)
        cu = call_counts(os.path.join(build, "libgof2hdaa.so"), sym)
        tu_o = collections.Counter()
        tu_u = collections.Counter()
        for name in set(co) | set(cu):
            tu = sym2tu.get(name)
            if not tu:
                continue
            tu_o[tu] += co.get(name, 0)
            tu_u[tu] += cu.get(name, 0)
        ndiff = sum(1 for tu in set(tu_o) | set(tu_u) if tu_o[tu] != tu_u[tu])
        print(
            "| `%s` | %d | %d | %d |"
            % (sym, sum(co.values()), sum(cu.values()), ndiff)
        )

    print("\n## per-TU frame-pointer fingerprints")
    print("| pattern | original | canonical | TUs differing |")
    print("|---|---:|---:|---:|")
    for pat in ("mov r7, sp", "add r7, sp, #", "push"):
        co = collections.Counter()
        cu = collections.Counter()
        for path, dest in (
            (original, co),
            (os.path.join(build, "libgof2hdaa.so"), cu),
        ):
            out = run([OBJDUMP, "-d", path]).stdout
            cur = None
            for line in out.splitlines():
                m = re.match(r"^([0-9a-f]+) <(.+)>:$", line)
                if m:
                    cur = m.group(2).split("@@")[0]
                    continue
                if cur is None:
                    continue
                if pat == "push":
                    if re.search(r"\bpush\s*\{[^}]*\br7\b", line):
                        dest[cur] += 1
                elif pat == "mov r7, sp":
                    if re.search(r"\bmov\s+r7,\s*sp\b", line):
                        dest[cur] += 1
                elif pat == "add r7, sp, #":
                    if re.search(r"\badd(?:\.w)?\s+r7,\s*sp,\s*#", line):
                        dest[cur] += 1
        tu_o = collections.Counter()
        tu_u = collections.Counter()
        for name in set(co) | set(cu):
            tu = sym2tu.get(name)
            if not tu:
                continue
            tu_o[tu] += co.get(name, 0)
            tu_u[tu] += cu.get(name, 0)
        ndiff = sum(1 for tu in set(tu_o) | set(tu_u) if tu_o[tu] != tu_u[tu])
        print("| `%s` | %d | %d | %d |" % (pat, sum(co.values()), sum(cu.values()), ndiff))


def metadata(repo, original, build):
    print("\n## metadata sections (original vs canonical)")
    print("| section | size | original sha256 | canonical sha256 | identical |")
    print("|---|---:|---|---|---|")
    for name in (
        ".comment",
        ".note.android.ident",
        ".note.gnu.gold-version",
        ".ARM.attributes",
        ".shstrtab",
        ".rodata",
        ".data",
        ".bss",
        ".rel.plt",
    ):
        a = section_bytes(original, name)
        b = section_bytes(os.path.join(build, "libgof2hdaa.so"), name)
        print(
            "| `%s` | %d | `%s` | `%s` | %s |"
            % (
                name,
                len(a),
                hashlib.sha256(a).hexdigest()[:16],
                hashlib.sha256(b).hexdigest()[:16],
                "yes" if a == b else "no",
            )
        )


def main():
    global ROOT, OBJCOPY, OBJDUMP
    ap = argparse.ArgumentParser()
    ap.add_argument("--repo", default=os.getcwd())
    ap.add_argument("--original", required=True)
    ap.add_argument("--build", default="cmake-build-match")
    ap.add_argument("--ndk", default=None)
    ap.add_argument("--no-sweeps", action="store_true")
    args = ap.parse_args()

    ROOT = os.path.abspath(args.repo)
    build = args.build if os.path.isabs(args.build) else os.path.join(ROOT, args.build)
    ndk = args.ndk or os.path.join(ROOT, ".cache", "ndk", "android-ndk-r18b")
    gcc = os.path.join(
        ndk, "toolchains", "arm-linux-androideabi-4.9", "prebuilt", "linux-x86_64", "bin"
    )
    OBJCOPY = os.path.join(gcc, "arm-linux-androideabi-objcopy")
    OBJDUMP = os.path.join(gcc, "arm-linux-androideabi-objdump")
    for p in (OBJCOPY, OBJDUMP, args.original, os.path.join(build, "libgof2hdaa.so")):
        if not os.path.exists(p):
            sys.exit("missing: %s" % p)

    print_tu_table(ROOT, build, args.original)
    metadata(ROOT, args.original, build)
    fingerprints(ROOT, build, args.original)

    if not args.no_sweeps:
        g_levels = ["-g0", "-g1", "-g2", "-g3", "-gline-tables-only", "-gmlt"]
        print("\n## -g sweep", file=sys.stderr)
        g_res = sweep(ROOT, build, g_levels, "-g")
        print_sweep("-g sweep", g_res, g_levels)

        o_levels = ["-O1", "-O2", "-Os", "-O3"]
        print("\n## -O sweep", file=sys.stderr)
        o_res = sweep(ROOT, build, o_levels, "-Oz")
        print_sweep("-O sweep", o_res, o_levels)


if __name__ == "__main__":
    main()
