#!/usr/bin/env python3
"""flag_fingerprint.py -- differential build-flag matrix and reproducibility harness.

This tool re-derives the original libgof2hdaa.so compile-flag set from the shipped artifact by
A/B building the tree with candidate flags and comparing, for every variant:

  * the official per-function verifier counts (byte_exact / linked_exact / size-exact),
  * every ELF section's size and SHA-256 against the original .so,
  * the exact set of functions whose code changed relative to the canonical -Oz build,
  * the effect on the 24 functions that are not yet byte-exact.

It also has a per-TU mode that recompiles representative translation units (the TUs containing the
24 non-exact functions, plus the vendored zlib/libzip/libcrypto TUs) at -O0/-O1/-O2/-O3/-Os and
compares the object-file function bytes with the canonical -Oz object.  A TU whose exact functions
change at another -O level is positive proof that that TU was compiled at -Oz.

Usage
-----
  # whole-.so differential matrix (the default)
  python3 tools/flag_fingerprint.py --mode variants --jobs 8

  # per-TU optimization-level spot check
  python3 tools/flag_fingerprint.py --mode per-tu --jobs 8

  # both
  python3 tools/flag_fingerprint.py --mode all --jobs 8

The canonical build (cmake-build-match) is used as the baseline and is never modified.  All
scratch builds live under --workdir (default /dev/shm/w5020/flag_fingerprint).  Results are written
to <workdir>/results.json; a markdown table is printed to stdout.
"""
import argparse
import concurrent.futures
import hashlib
import json
import os
import re
import shlex
import struct
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
DEFAULT_WORKDIR = Path("/dev/shm/w5020/flag_fingerprint")
sys.path.insert(0, str(ROOT / "tools/verify"))
from elf import Elf32  # noqa: E402
ORIGINAL = Path("/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so")
FMOD_DIR = Path("/home/debian/GalaxyOnFire2/_work/bins/apk_libs")

# ---------------------------------------------------------------------------------------------
# ELF helpers (ELF32 little-endian, both ET_REL objects and the linked .so)
# ---------------------------------------------------------------------------------------------
SHT_SYMTAB, SHT_NOBITS, SHT_DYNSYM = 2, 8, 11
STT_FUNC, SHN_UNDEF = 2, 0


def _sections(data):
    e_shoff = struct.unpack_from("<I", data, 0x20)[0]
    e_shentsize = struct.unpack_from("<H", data, 0x2E)[0]
    e_shnum = struct.unpack_from("<H", data, 0x30)[0]
    e_shstrndx = struct.unpack_from("<H", data, 0x32)[0]
    shstr_off = struct.unpack_from("<I", data, e_shoff + e_shstrndx * e_shentsize + 0x10)[0]
    out = []
    for i in range(e_shnum):
        o = e_shoff + i * e_shentsize
        nm, typ = struct.unpack_from("<II", data, o)
        addr, off, size = struct.unpack_from("<III", data, o + 0x0C)
        link, = struct.unpack_from("<I", data, o + 0x18)
        ent, = struct.unpack_from("<I", data, o + 0x24)
        end = data.find(b"\0", shstr_off + nm)
        name = data[shstr_off + nm:end].decode("ascii", "replace")
        out.append(dict(name=name, type=typ, addr=addr, offset=off, size=size,
                        link=link, entsize=ent))
    return out


def section_digest(path):
    """name -> {size, sha256, type}; NOBITS sections hash the empty string."""
    data = Path(path).read_bytes()
    out = {}
    for s in _sections(data):
        if s["type"] == SHT_NOBITS:
            blob = b""
        else:
            blob = data[s["offset"]:s["offset"] + s["size"]]
        out[s["name"]] = dict(size=s["size"], sha256=hashlib.sha256(blob).hexdigest(),
                              type=s["type"])
    return out


def object_functions(path):
    """name -> raw bytes for every defined STT_FUNC symbol in an ET_REL object."""
    data = Path(path).read_bytes()
    secs = _sections(data)
    sec = next((s for s in secs if s["type"] == SHT_SYMTAB), None)
    if sec is None:
        return {}
    strtab = secs[sec["link"]]
    sb, _ss = strtab["offset"], strtab["size"]

    def cstr(i):
        e = data.find(b"\0", sb + i)
        return data[sb + i:e].decode("utf-8", "replace")

    out = {}
    entsize = sec["entsize"] or 16
    for o in range(sec["offset"], sec["offset"] + sec["size"], entsize):
        nm, val, sz = struct.unpack_from("<III", data, o)
        info = data[o + 12]
        shndx, = struct.unpack_from("<H", data, o + 14)
        if not nm or shndx == SHN_UNDEF or (info & 0xF) != STT_FUNC:
            continue
        s = secs[shndx]
        if s["type"] == SHT_NOBITS:
            continue
        out[cstr(nm)] = data[s["offset"] + val:s["offset"] + val + sz]
    return out


def function_blobs(path):
    """name -> (size, raw bytes) for every exported function in a linked .so."""
    e = Elf32(path)
    f = e.functions()
    return {n: (f[n]["size"], e.bytes_at(f[n]["value"], f[n]["size"])) for n in f}


def differing_bytes(a, b):
    if len(a) != len(b):
        return None
    return sum(1 for x, y in zip(a, b) if x != y)


def section_diff(ours, orig):
    names = sorted(set(ours) | set(orig))
    rows = []
    for n in names:
        o, r = ours.get(n), orig.get(n)
        if o is None or r is None:
            rows.append(dict(name=n, size_ours=(o or {}).get("size"),
                             size_orig=(r or {}).get("size"), same=False,
                             diff_bytes=None, present=bool(o and r)))
            continue
        same = o["sha256"] == r["sha256"] and o["size"] == r["size"]
        db = None
        if o["size"] == r["size"] and o["type"] != SHT_NOBITS:
            # re-read both blobs for a byte count; cheap (sections are small except .text)
            db = None
        rows.append(dict(name=n, size_ours=o["size"], size_orig=r["size"],
                         same=same, diff_bytes=db, present=True))
    return rows


def section_diff_bytes(ours_path, orig_path, rows):
    """Fill diff_bytes for same-size PROGBITS sections."""
    od = Path(ours_path).read_bytes()
    rd = Path(orig_path).read_bytes()
    so = {s["name"]: s for s in _sections(od)}
    sr = {s["name"]: s for s in _sections(rd)}
    for row in rows:
        n = row["name"]
        if not row["present"] or row["size_ours"] != row["size_orig"]:
            continue
        a, b = so[n], sr[n]
        if a["type"] == SHT_NOBITS:
            row["diff_bytes"] = 0
            continue
        x = od[a["offset"]:a["offset"] + a["size"]]
        y = rd[b["offset"]:b["offset"] + b["size"]]
        row["diff_bytes"] = differing_bytes(x, y)
    return rows


# ---------------------------------------------------------------------------------------------
# Build helpers
# ---------------------------------------------------------------------------------------------
def run(cmd, cwd=None, log=None, check=True):
    p = subprocess.run([str(c) for c in cmd], cwd=cwd, capture_output=True, text=True)
    if log:
        Path(log).write_text((p.stdout or "") + (p.stderr or ""))
    if check and p.returncode != 0:
        raise RuntimeError(f"command failed ({p.returncode}): {' '.join(map(str, cmd))}\n"
                           f"{(p.stderr or '')[-4000:]}")
    return p


def cache_var(build_dir, name):
    text = (Path(build_dir) / "CMakeCache.txt").read_text()
    m = re.search(rf"^{re.escape(name)}:\w+=(.*)$", text, re.M)
    return m.group(1).strip() if m else None


def write_wrapper(workdir):
    wrapper = workdir / "toolchain_extra.cmake"
    wrapper.write_text(f'''include("{ROOT}/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${{CMAKE_CXX_FLAGS}} ${{GOF2_EXTRA_FLAGS}}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${{CMAKE_C_FLAGS}}   ${{GOF2_EXTRA_FLAGS}}" CACHE STRING "" FORCE)
endif()
''')
    return wrapper


def configure_variant(workdir, wrapper, build_dir, extra_flags, cmake_args=()):
    run(["cmake", "-S", ROOT, "-B", build_dir, "-G", "Ninja",
         f"-DCMAKE_TOOLCHAIN_FILE={wrapper}",
         f"-DGOF2_ORIGINAL_SO={ORIGINAL}",
         f"-DGOF2_FMOD_LIBDIR={FMOD_DIR}",
         f"-DGOF2_EXTRA_FLAGS={extra_flags}", *cmake_args],
        log=build_dir / "configure.log")


def verify_so(build_dir, so_path, fast=False):
    vd = build_dir / "verify"
    vd.mkdir(parents=True, exist_ok=True)
    (vd / "report.json").unlink(missing_ok=True)
    objdump = cache_var(build_dir, "GOF2_NDK_OBJDUMP")
    if fast:
        # The official verifier's byte_exact / linked_exact / size-exact are independent of the
        # difflib fuzzy percentage.  Short-circuit the fuzzy score for non-identical streams so a
        # variant with thousands of changed functions does not spend minutes in SequenceMatcher.
        wrapper = build_dir / "verify_fast.py"
        wrapper.write_text(f"""import sys
sys.path.insert(0, {str(ROOT / 'tools/verify')!r})
import verify
def _score(a, b):
    if a == b: return 100.0
    if not a and not b: return 100.0
    return 0.0
verify.score = _score
# reconcile() also uses difflib.SequenceMatcher; its only effect is to unify pool words whose
# raw values agree on both sides.  Skipping it cannot turn a non-identical function into a
# byte-exact one; it can only under-count linked_exact for a non-byte-exact function.
verify.reconcile = lambda a, b: (a, b)
sys.argv = ['verify.py'] + sys.argv[1:]
verify.main()
""")
        cmd = [sys.executable, wrapper]
    else:
        cmd = [sys.executable, ROOT / "tools/verify/verify.py"]
    run(cmd + ["--build-dir", vd, "--original", ORIGINAL, "--ours", so_path,
               "--objdump", objdump, "--fail-on-wrong-type", "--top", "0"],
        log=build_dir / "verify.log", check=False)
    return json.loads((vd / "report.json").read_text())


def build_variant(workdir, wrapper, spec, baseline_rows, orig_sections, jobs, fast=False):
    name, flags = spec["name"], spec["flags"]
    bd = workdir / f"build-{name}"
    bd.mkdir(parents=True, exist_ok=True)
    configure_variant(workdir, wrapper, bd, flags, spec.get("cmake_args", ()))
    run(["cmake", "--build", bd, "--target", "link_so", "-j", str(jobs)],
        log=bd / "build.log")
    so = bd / "libgof2hdaa.so"
    report = verify_so(bd, so, fast=fast)
    rows = {r["name"]: r for r in report["rows"]}
    summary = report["summary"]

    base = {r["name"]: r for r in baseline_rows}
    base_blobs = function_blobs(Path(spec["baseline_build"]) / "libgof2hdaa.so")
    var_blobs = function_blobs(so)
    changed, newly_exact, newly_inexact = [], [], []
    for n, r in rows.items():
        b = base.get(n)
        if b is None:
            continue
        # Compare code identity, not the fuzzy percentage: the fast-verify mode short-circuits
        # difflib, so pct is 0 for every non-identical stream and is not comparable to the
        # canonical full-verify report.  Raw bytes catch a change in a function that is
        # non-exact on both sides (the report's boolean alone would miss it).
        same_code = (n in base_blobs and n in var_blobs
                     and base_blobs[n] == var_blobs[n])
        if not same_code:
            changed.append(n)
        if not b["byte"] and r["byte"]:
            newly_exact.append(n)
        if b["byte"] and not r["byte"]:
            newly_inexact.append(n)

    nonexact = []
    for n in spec.get("watch", []):
        b, r = base.get(n), rows.get(n)
        if b is None or r is None:
            continue
        code_changed = not (n in base_blobs and n in var_blobs
                            and base_blobs[n] == var_blobs[n])
        nonexact.append(dict(name=n, pct_before=b["pct"], pct_after=r["pct"],
                             byte_before=b["byte"], byte_after=r["byte"],
                             osize=r["osize"], usize=r["usize"],
                             changed=code_changed))

    ours_sections = section_digest(so)
    srows = section_diff_bytes(so, ORIGINAL, section_diff(ours_sections, orig_sections))
    differing = [r for r in srows if not r["same"]]
    return dict(name=name, flags=flags, cmake_args=list(spec.get("cmake_args", ())),
                verify=dict(byte_exact=summary["byte_exact"],
                            linked_exact=summary["linked_exact"],
                            compared=summary["compared"],
                            avg_match=summary["avg_match"],
                            stub_zero_size=summary["stub_zero_size"],
                            missing=summary["missing"], wrong_type=summary["wrong_type"],
                            extra=summary["extra"]),
                size_exact=sum(1 for r in rows.values() if r["osize"] == r["usize"]),
                changed=changed, newly_exact=newly_exact, newly_inexact=newly_inexact,
                nonexact=nonexact,
                sections=dict(differing=differing,
                              differing_bytes=sum((r["diff_bytes"] or 0) for r in differing),
                              text_size=ours_sections.get(".text", {}).get("size"),
                              rodata_size=ours_sections.get(".rodata", {}).get("size"),
                              exidx_size=ours_sections.get(".ARM.exidx", {}).get("size"),
                              extab_size=ours_sections.get(".ARM.extab", {}).get("size")),
                build_dir=str(bd))


# ---------------------------------------------------------------------------------------------
# Per-TU optimization spot check
# ---------------------------------------------------------------------------------------------
def compile_command(entry, extra, out):
    cmd = shlex.split(entry["command"])
    cmd[cmd.index("-o") + 1] = str(out)
    return cmd + shlex.split(extra)


def per_tu_one(entry, opt, workdir, baseline_obj, base_report):
    src = Path(entry["file"])
    tag = re.sub(r"[^A-Za-z0-9_.-]", "_", str(src.relative_to(ROOT)))
    out = workdir / "obj" / f"{tag}.{opt.lstrip('-')}.o"
    out.parent.mkdir(parents=True, exist_ok=True)
    p = run(compile_command(entry, opt, out), cwd=entry["directory"], check=False)
    if p.returncode != 0:
        return dict(source=str(src), opt=opt, built=False, error=(p.stderr or "")[-800:])
    a = object_functions(baseline_obj)
    b = object_functions(out)
    changed = sorted(n for n in a if n in b and a[n] != b[n])
    changed_exact = [n for n in changed if base_report.get(n, {}).get("byte")]
    changed_nonexact = [n for n in changed if n in base_report and not base_report[n]["byte"]]
    return dict(source=str(src), opt=opt, built=True, functions=len(a),
                changed=changed, changed_count=len(changed),
                changed_exact=changed_exact, changed_nonexact=changed_nonexact)


def per_tu_mode(workdir, jobs, baseline_build, all_tus=False):
    cc = json.loads((Path(baseline_build) / "compile_commands.json").read_text())
    by_src = {e["file"]: e for e in cc}
    report = json.loads((Path(baseline_build) / "verify" / "report.json").read_text())
    base = {r["name"]: r for r in report["rows"]}
    nonexact = [n for n, r in base.items() if not r["byte"]]

    # Map each non-exact function to its defining TU via the baseline objects' symtabs.
    obj_funcs = {}
    for e in cc:
        obj = Path(baseline_build) / e["output"]
        if obj.exists():
            obj_funcs[e["file"]] = set(object_functions(obj))
    target_srcs = set()
    for n in nonexact:
        for src, funcs in obj_funcs.items():
            if n in funcs:
                target_srcs.add(src)
    # A representative exact TU per directory/area, plus the vendored libs.
    exact_srcs = set()
    for e in cc:
        if e["file"] not in target_srcs and any(
                n in obj_funcs.get(e["file"], ()) and base[n]["byte"] for n in base):
            exact_srcs.add(e["file"])
    if all_tus:
        selected = sorted(by_src)
    else:
        # Keep the per-TU run bounded: all non-exact TUs + one exact TU per directory.
        spread = {}
        for s in sorted(exact_srcs):
            spread.setdefault(str(Path(s).parent), s)
        selected = sorted(target_srcs | set(spread.values()))

    tasks = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        for src in selected:
            e = by_src[src]
            baseline_obj = Path(baseline_build) / e["output"]
            for opt in ("-O0", "-O1", "-O2", "-O3", "-Os"):
                tasks.append(ex.submit(per_tu_one, e, opt, workdir, baseline_obj, base))
        results = [t.result() for t in tasks]

    # Add vendored-library spot checks (zlib/libzip are -Oz clang; libcrypto is GCC -O3).
    vendored = vendored_spot(workdir, jobs)
    return dict(selected=[str(Path(s).relative_to(ROOT)) for s in selected],
                nonexact=nonexact, results=results, vendored=vendored)


def vendored_spot(workdir, jobs):
    """Compile representative vendored TUs at -Oz vs -O2/-Os and compare function bytes."""
    ndk = ROOT / ".cache/ndk/android-ndk-r18b"
    clang = ndk / "toolchains/llvm/prebuilt/linux-x86_64/bin/clang"
    gcc = ROOT / ".cache/ndk/android-ndk-r13b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc"
    sysroot = ndk / "platforms/android-16/arch-arm"
    base = (f"--target=armv7a-linux-androideabi16 -mfpu=neon -mfloat-abi=softfp "
            f"--sysroot={sysroot} -isystem {ndk}/sysroot/usr/include "
            f"-isystem {ndk}/sysroot/usr/include/arm-linux-androideabi "
            f"-fPIC -ffunction-sections -funwind-tables -Wno-everything")
    jobs_list = []
    zlib = ROOT / "third_party/zlib/zlib-1.2.3"
    for src in ("inflate.c", "deflate.c", "crc32.c", "adler32.c", "trees.c", "inftrees.c"):
        jobs_list.append(("zlib", zlib / src, f"{base} -Oz -g -fstack-protector-strong -I. -mthumb"))
    libzip = ROOT / "third_party/libzip/libzip-0.9.3/lib"
    for src in ("zip_open.c", "zip_close.c", "zip_dirent.c", "zip_fopen_index.c"):
        jobs_list.append(("libzip", libzip / src,
                          f"{base} -Oz -g -fstack-protector-strong -I. -mthumb"))
    openssl = ROOT / "third_party/openssl/openssl-1.1.0l"
    ossl = (f"--sysroot={sysroot} -isystem {ndk}/sysroot/usr/include "
            f"-isystem {ndk}/sysroot/usr/include/arm-linux-androideabi "
            f"-D__ANDROID_API__=16 -march=armv7-a -DDSO_DLFCN -DHAVE_DLFCN_H -DNDEBUG "
            f"-DOPENSSL_THREADS -DOPENSSL_NO_DYNAMIC_ENGINE -DOPENSSL_PIC -DSHA256_ASM "
            f"-DOPENSSL_USE_NODELETE -Wall -pthread -mandroid -fPIC -Wa,--noexecstack "
            f"-I. -Icrypto/include -Iinclude -Icrypto -w")
    jobs_list.append(("libcrypto", openssl / "crypto/sha/sha256.c", ossl))

    outdir = workdir / "vendored"
    outdir.mkdir(parents=True, exist_ok=True)
    results = []
    for lib, src, flags in jobs_list:
        if not src.exists():
            results.append(dict(lib=lib, source=str(src), built=False, error="missing source"))
            continue
        tag = re.sub(r"[^A-Za-z0-9_.-]", "_", f"{lib}_{src.name}")
        objs = {}
        base_opt = "-O3" if lib == "libcrypto" else "-Oz"
        for opt in (base_opt, "-O1", "-O2", "-Os"):
            out = outdir / f"{tag}.{opt.lstrip('-')}.o"
            cc = gcc if lib == "libcrypto" else clang
            cmd = [cc] + shlex.split(flags) + [opt, "-c", src, "-o", out]
            cwd = openssl if lib == "libcrypto" else src.parent
            p = run(cmd, cwd=cwd, check=False)
            objs[opt] = out if p.returncode == 0 else None
        if not objs.get(base_opt):
            results.append(dict(lib=lib, source=str(src), built=False, error="baseline compile failed"))
            continue
        base_funcs = object_functions(objs[base_opt])
        row = dict(lib=lib, source=str(src), built=True, functions=len(base_funcs),
                   base_opt=base_opt, opts={})
        for opt in ("-O1", "-O2", "-Os"):
            if not objs.get(opt):
                row["opts"][opt] = dict(built=False)
                continue
            f = object_functions(objs[opt])
            changed = sorted(n for n in base_funcs if n in f and base_funcs[n] != f[n])
            row["opts"][opt] = dict(built=True, changed=changed, changed_count=len(changed),
                                    changed_exact=changed)
        results.append(row)
    return results


# ---------------------------------------------------------------------------------------------
# Branch-weight default sweep (object-level, layout-free)
# ---------------------------------------------------------------------------------------------
def branch_weight_mode(workdir, jobs, baseline_build):
    """Compile every TU except StarMap/Radar with global branch-weight variants and compare the
    object-file function bytes with the canonical -Oz object.  StarMap/Radar are excluded because
    they carry per-file overrides; this isolates the *global* default.  Exact functions that
    change disprove the variant; a variant that changes only non-exact functions is unobservable
    on every matched function."""
    cc = json.loads((Path(baseline_build) / "compile_commands.json").read_text())
    report = json.loads((Path(baseline_build) / "verify" / "report.json").read_text())
    base = {r["name"]: r for r in report["rows"]}
    variants = {
        "default": [],
        "likely64": ["-mllvm", "-likely-branch-weight=64"],
        "likely500": ["-mllvm", "-likely-branch-weight=500"],
        "likely1000": ["-mllvm", "-likely-branch-weight=1000"],
        "unlikely0": ["-mllvm", "-unlikely-branch-weight=0"],
        "unlikely2": ["-mllvm", "-unlikely-branch-weight=2"],
    }
    outdir = workdir / "branchweights"
    outdir.mkdir(parents=True, exist_ok=True)

    def one(e, name, extra):
        if e["file"].endswith(("StarMap.cpp", "Radar.cpp")):
            return None
        cmd = shlex.split(e["command"])
        tag = re.sub(r"[^A-Za-z0-9_.-]", "_", e["file"].replace(str(ROOT) + "/", ""))
        out = outdir / f"{tag}.{name}.o"
        cmd[cmd.index("-o") + 1] = str(out)
        cmd += extra
        p = run(cmd, cwd=e["directory"], check=False)
        if p.returncode:
            return dict(source=e["file"], variant=name, built=False)
        a = object_functions(Path(baseline_build) / e["output"])
        b = object_functions(out)
        changed = [n for n in a if n in b and a[n] != b[n]]
        return dict(source=e["file"], variant=name, built=True,
                    changed=changed,
                    changed_exact=[n for n in changed if base.get(n, {}).get("byte")],
                    changed_nonexact=[n for n in changed
                                      if n in base and not base[n]["byte"]])

    tasks = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=jobs) as ex:
        for e in cc:
            for name, extra in variants.items():
                tasks.append(ex.submit(one, e, name, extra))
        rows = [t.result() for t in tasks if t.result() is not None]
    summary = {}
    for name in variants:
        vr = [r for r in rows if r["variant"] == name and r["built"]]
        summary[name] = dict(
            tus=len(vr),
            changed_tus=sum(1 for r in vr if r["changed"]),
            changed_exact=sum(len(r["changed_exact"]) for r in vr),
            changed_nonexact=sum(len(r["changed_nonexact"]) for r in vr),
            exact_change_sources=[str(Path(r["source"]).relative_to(ROOT))
                                  for r in vr if r["changed_exact"]],
            nonexact_change_sources=[str(Path(r["source"]).relative_to(ROOT))
                                     for r in vr if r["changed_nonexact"]])
    return dict(variants=summary, rows=rows)


# ---------------------------------------------------------------------------------------------
# Variant table
# ---------------------------------------------------------------------------------------------
def default_variants(watch):
    return [
        dict(name="fno_vectorize", flags="-fno-vectorize"),
        dict(name="fno_slp_vectorize", flags="-fno-slp-vectorize"),
        dict(name="fno_unroll_loops", flags="-fno-unroll-loops"),
        dict(name="inline_threshold_0", flags="-mllvm -inline-threshold=0"),
        dict(name="inline_threshold_1", flags="-mllvm -inline-threshold=1"),
        dict(name="inline_threshold_5", flags="-mllvm -inline-threshold=5"),
        dict(name="inline_threshold_10", flags="-mllvm -inline-threshold=10"),
        dict(name="inline_threshold_25", flags="-mllvm -inline-threshold=25"),
        dict(name="inline_threshold_50", flags="-mllvm -inline-threshold=50"),
        dict(name="inline_threshold_100", flags="-mllvm -inline-threshold=100"),
        dict(name="unsafe_fp_math", flags="-mllvm -enable-unsafe-fp-math"),
        dict(name="menable_unsafe_fp_math", flags="-Xclang -menable-unsafe-fp-math"),
        dict(name="ffast_math", flags="-ffast-math"),
        dict(name="fno_math_errno", flags="-fno-math-errno"),
        dict(name="fno_trapping_math", flags="-fno-trapping-math"),
        dict(name="freciprocal_math", flags="-freciprocal-math"),
        dict(name="fno_delete_null_pointer_checks", flags="-fno-delete-null-pointer-checks"),
        dict(name="fwrapv", flags="-fwrapv"),
        dict(name="fno_strict_overflow", flags="-fno-strict-overflow"),
        dict(name="fno_builtin", flags="-fno-builtin"),
        dict(name="no_per_file_weights", flags="",
             cmake_args=("-DGOF2_NO_PER_FILE_WEIGHTS=ON",)),
        dict(name="branch_likely_64", flags="-mllvm -likely-branch-weight=64 -mllvm -unlikely-branch-weight=1",
             cmake_args=("-DGOF2_NO_PER_FILE_WEIGHTS=ON",)),
        dict(name="branch_likely_1000", flags="-mllvm -likely-branch-weight=1000 -mllvm -unlikely-branch-weight=1",
             cmake_args=("-DGOF2_NO_PER_FILE_WEIGHTS=ON",)),
        dict(name="branch_unlikely_0", flags="-mllvm -likely-branch-weight=2000 -mllvm -unlikely-branch-weight=0",
             cmake_args=("-DGOF2_NO_PER_FILE_WEIGHTS=ON",)),
        dict(name="O1", flags="-O1"),
        dict(name="O2", flags="-O2"),
        dict(name="O3", flags="-O3"),
        dict(name="Os", flags="-Os"),
    ]


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--mode", choices=("variants", "per-tu", "per-tu-all", "branch-weights",
                                       "all"), default="variants")
    ap.add_argument("--workdir", default=str(DEFAULT_WORKDIR))
    ap.add_argument("--baseline-build", default=str(ROOT / "cmake-build-match"))
    ap.add_argument("--jobs", type=int, default=8)
    ap.add_argument("--variants", default=None,
                    help="comma-separated subset of variant names (default: all)")
    ap.add_argument("--json", default=None, help="output JSON path (default <workdir>/results.json)")
    ap.add_argument("--full-verify", action="store_true",
                    help="run the full difflib fuzzy score (slow on variants with many changes)")
    args = ap.parse_args()

    workdir = Path(args.workdir)
    workdir.mkdir(parents=True, exist_ok=True)
    wrapper = write_wrapper(workdir)

    baseline_build = Path(args.baseline_build)
    baseline_report = json.loads((baseline_build / "verify" / "report.json").read_text())
    baseline_rows = baseline_report["rows"]
    base = {r["name"]: r for r in baseline_rows}
    watch = [n for n, r in base.items() if not r["byte"]]
    orig_sections = section_digest(ORIGINAL)

    out = dict(baseline=dict(build_dir=str(baseline_build),
                             verify=baseline_report["summary"],
                             size_exact=sum(1 for r in baseline_rows if r["osize"] == r["usize"]),
                             nonexact=watch),
               variants=[], per_tu=None)

    if args.mode in ("variants", "all"):
        specs = default_variants(watch)
        if args.variants:
            wanted = {x.strip() for x in args.variants.split(",") if x.strip()}
            specs = [s for s in specs if s["name"] in wanted]
        for s in specs:
            s["watch"] = watch
            s["baseline_build"] = str(baseline_build)
        results = []
        with concurrent.futures.ThreadPoolExecutor(max_workers=args.jobs) as ex:
            futs = {ex.submit(build_variant, workdir, wrapper, s, baseline_rows, orig_sections,
                              max(1, 128 // args.jobs), not args.full_verify): s for s in specs}
            for f in concurrent.futures.as_completed(futs):
                s = futs[f]
                try:
                    results.append(f.result())
                    print(f"[done] {s['name']}", file=sys.stderr)
                except Exception as e:  # noqa: BLE001
                    print(f"[FAIL] {s['name']}: {e}", file=sys.stderr)
                    results.append(dict(name=s["name"], flags=s["flags"], error=str(e)))
        results.sort(key=lambda r: [x["name"] for x in specs].index(r["name"]))
        out["variants"] = results

    if args.mode in ("per-tu", "per-tu-all", "all"):
        out["per_tu"] = per_tu_mode(workdir, args.jobs, baseline_build,
                                    all_tus=args.mode in ("per-tu-all", "all"))
    if args.mode in ("branch-weights", "all"):
        out["branch_weights"] = branch_weight_mode(workdir, args.jobs, baseline_build)

    jpath = Path(args.json) if args.json else workdir / "results.json"
    jpath.write_text(json.dumps(out, indent=1) + "\n")
    print(f"\nwrote {jpath}")
    print_markdown(out)


def print_markdown(out):
    base = out["baseline"]
    print("\n## Baseline\n")
    print(f"- byte_exact {base['verify']['byte_exact']} / linked_exact "
          f"{base['verify']['linked_exact']} / size-exact {base['size_exact']} / "
          f"avg {base['verify']['avg_match']}")
    print(f"- non-exact ({len(base['nonexact'])}): " + ", ".join(base["nonexact"]))
    if out["variants"]:
        print("\n## Whole-.so variants\n")
        print("| variant | flags | byte | linked | size | .text | .rodata | exidx | extab | "
              "diff bytes | changed | new exact | new inexact |")
        print("|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|")
        for r in out["variants"]:
            if "error" in r:
                print(f"| {r['name']} | `{r['flags']}` | BUILD FAIL | | | | | | | | | | |")
                continue
            v, s = r["verify"], r["sections"]
            print(f"| {r['name']} | `{r['flags']}` | {v['byte_exact']} | {v['linked_exact']} | "
                  f"{r['size_exact']}/{v['compared']} | 0x{s['text_size']:x} | "
                  f"0x{s['rodata_size']:x} | 0x{s['exidx_size']:x} | 0x{s['extab_size']:x} | "
                  f"{s['differing_bytes']} | {len(r['changed'])} | "
                  f"{len(r['newly_exact'])} | {len(r['newly_inexact'])} |")
    if out.get("per_tu"):
        print("\n## Per-TU optimization spot check\n")
        print("| TU | -O0 exact/nonexact | -O1 | -O2 | -O3 | -Os |")
        print("|---|---:|---:|---:|---:|---:|")
        by = {}
        for r in out["per_tu"]["results"]:
            by.setdefault(r["source"], {})[r["opt"]] = r
        for src, opts in sorted(by.items()):
            cells = []
            for o in ("-O0", "-O1", "-O2", "-O3", "-Os"):
                r = opts.get(o)
                if not r or not r.get("built"):
                    cells.append("FAIL")
                else:
                    cells.append(f"{len(r['changed_exact'])}/{len(r['changed_nonexact'])}")
            print(f"| {Path(src).relative_to(ROOT)} | " + " | ".join(cells) + " |")
        print("\n| vendored TU | -O1 changed | -O2 changed | -Os changed |")
        print("|---|---:|---:|---:|")
        for r in out["per_tu"]["vendored"]:
            if not r.get("built"):
                print(f"| {r['lib']}/{Path(r['source']).name} | FAIL | FAIL | FAIL |")
                continue
            print(f"| {r['lib']}/{Path(r['source']).name} | "
                  f"{r['opts'].get('-O1', {}).get('changed_count', 'FAIL')} | "
                  f"{r['opts'].get('-O2', {}).get('changed_count', 'FAIL')} | "
                  f"{r['opts'].get('-Os', {}).get('changed_count', 'FAIL')} |")
    if out.get("branch_weights"):
        print("\n## Global branch-weight defaults (per-TU object A/B, StarMap/Radar excluded)\n")
        print("| variant | TUs | changed TUs | changed exact | changed non-exact |")
        print("|---|---:|---:|---:|---:|")
        for name, r in out["branch_weights"]["variants"].items():
            print(f"| {name} | {r['tus']} | {r['changed_tus']} | {r['changed_exact']} | "
                  f"{r['changed_nonexact']} |")


if __name__ == "__main__":
    main()
