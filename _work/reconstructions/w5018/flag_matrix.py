#!/usr/bin/env python3
"""A/B the link flags against the shipped original (w5018).

Reads the exact gold+strip+reorder command from ninja, then relinks with one
mutation at a time and reports which sections/NEEDED/exports/imports change.
A mutation that produces byte-identical output is UNOBSERVABLE; one that
changes output is a candidate original flag (compare against the original).

Usage: python3 flag_matrix.py <repo-root> <build-dir> <original.so>
"""
import hashlib
import os
import shlex
import struct
import subprocess
import sys

sys.path.insert(0, os.path.join(sys.argv[1], "tools", "verify"))
from elf import Elf32  # noqa: E402

SECTIONS = [".text", ".rodata", ".data", ".data.rel.ro", ".bss", ".dynsym", ".dynstr",
            ".hash", ".rel.dyn", ".rel.plt", ".plt", ".got", ".dynamic",
            ".ARM.exidx", ".ARM.extab", ".shstrtab", ".note.gnu.build-id"]


def sec(e, n):
    return next((s for s in e.sections if s["name"] == n), None)


def diffbytes(ea, eb, n):
    a, b = sec(ea, n), sec(eb, n)
    if not a or not b:
        return None
    if a["size"] != b["size"]:
        return f"size {a['size']:x}/{b['size']:x}"
    da = ea.data[a["offset"]:a["offset"] + a["size"]]
    db = eb.data[b["offset"]:b["offset"] + b["size"]]
    return sum(x != y for x, y in zip(da, db))


def needed(e):
    s = sec(e, ".dynamic")
    st = sec(e, ".dynstr")
    out = []
    for off in range(s["offset"], s["offset"] + s["size"], 8):
        t, v = struct.unpack_from("<II", e.data, off)
        if t == 1:
            p = st["offset"] + v
            q = e.data.find(b"\0", p)
            out.append(e.data[p:q].decode())
    return out


def exports(e):
    return {s["name"] for s in e._symbols(11)
            if s["name"] and s["shndx"] != 0 and (s["info"] >> 4) != 0}


def imports(e):
    return {s["name"] for s in e._symbols(11) if s["name"] and s["shndx"] == 0}


def exidx(e):
    s = sec(e, ".ARM.exidx")
    n = s["size"] // 8
    cant = inl = ext = 0
    for i in range(n):
        w0, w1 = struct.unpack_from("<II", e.data, s["offset"] + i * 8)
        if w1 == 1:
            cant += 1
        elif w1 & 0x80000000:
            inl += 1
        else:
            ext += 1
    return n, cant, inl, ext


def main():
    root, build, orig_path = sys.argv[1], sys.argv[2], sys.argv[3]
    cmd = subprocess.run(["ninja", "-t", "commands", "libgof2hdaa.so"],
                         cwd=build, capture_output=True, text=True).stdout.strip().splitlines()[-1]
    parts = cmd.split(" && ")
    gold = shlex.split(parts[1])
    strip = shlex.split(parts[2])
    reorder = shlex.split(parts[3])
    ri = next(i for i, x in enumerate(reorder) if x.endswith("reorder_shstrtab.py"))
    reorder = reorder[:ri + 1]
    # locate the output
    oi = gold.index("-o")
    out = gold[oi + 1]
    scratch = "/dev/shm/w5018/flagmatrix"
    os.makedirs(scratch, exist_ok=True)
    O = Elf32(orig_path)
    base_so = os.path.join(scratch, "base.so")
    base_un = os.path.join(scratch, "base.unstripped.so")
    t = list(gold)
    t[oi + 1] = base_un
    subprocess.run(t, check=True, capture_output=True)
    subprocess.run([strip[0], "--strip-all", "-o", base_so, base_un], check=True)
    subprocess.run(reorder + [base_un, base_so], check=True)
    base = Elf32(base_so)
    base_sha = hashlib.sha256(open(base_so, "rb").read()).hexdigest()[:16]
    print(f"base sha {base_sha}  orig sha {hashlib.sha256(open(orig_path,'rb').read()).hexdigest()[:16]}")
    print(f"orig needed {needed(O)}")
    print(f"orig exidx {exidx(O)}")

    def run(tag, tokens):
        so = os.path.join(scratch, tag + ".so")
        un = os.path.join(scratch, tag + ".unstripped.so")
        tt = list(tokens)
        oi2 = tt.index("-o")
        tt[oi2 + 1] = un
        r = subprocess.run(tt, capture_output=True, text=True)
        if r.returncode:
            print(f"{tag:24} LINK FAIL {r.stderr.strip()[:100]}")
            return
        subprocess.run([strip[0], "--strip-all", "-o", so, un], check=True)
        subprocess.run(reorder + [un, so], check=True)
        e = Elf32(so)
        sha = hashlib.sha256(open(so, "rb").read()).hexdigest()[:16]
        d = {n: diffbytes(O, e, n) for n in SECTIONS}
        changed = " ".join(f"{k}:{v}" for k, v in d.items() if v not in (0, None))
        need = "" if needed(e) == needed(O) else f" NEEDED={needed(e)}"
        ex = "" if exidx(e) == exidx(O) else f" exidx={exidx(e)}"
        exn = "" if len(exports(e)) == len(exports(O)) else f" exports={len(exports(e))}/{len(exports(O))}"
        imn = "" if len(imports(e)) == len(imports(O)) else f" imports={len(imports(e))}/{len(imports(O))}"
        print(f"{tag:24} sha {sha}{' ==base' if sha == base_sha else ''}{need}{ex}{exn}{imn}")
        if changed:
            print("   ", changed)

    def rm(tokens, *vals):
        return [x for x in tokens if x not in vals]

    def rep(tokens, a, b):
        i = tokens.index(a)
        t = list(tokens)
        t[i] = b
        return t

    V = []
    # build mutations explicitly
    def mut(name, fn):
        V.append((name, fn(list(gold))))
    mut("z_lazy", lambda t: rep(t, "now", "lazy") if "now" in t else t)
    mut("z_norelro", lambda t: t + ["-z", "norelro"])
    mut("z_execstack", lambda t: t + ["-z", "execstack"])
    mut("z_noexecstack", lambda t: t + ["-z", "noexecstack"])
    mut("z_nocombreloc", lambda t: t + ["-z", "nocombreloc"])
    mut("no_exclude_libs", lambda t: rm(t, "--exclude-libs=libgcc.a"))
    mut("exclude_all", lambda t: rep(t, "--exclude-libs=libgcc.a", "--exclude-libs=ALL"))
    mut("exclude_cxxabi", lambda t: rep(t, "--exclude-libs=libgcc.a", "--exclude-libs=libc++abi.a"))
    mut("no_whole_archive", lambda t: rm(t, "--whole-archive", "--no-whole-archive"))
    def whole_zlib_only(t):
        t = list(t)
        z = next(x for x in t if x.endswith("libzip_gof2.a"))
        t.remove(z)
        i = t.index("--no-whole-archive") + 1
        return t[:i] + [z] + t[i:]
    mut("whole_zlib_only", whole_zlib_only)
    mut("no_zlib", lambda t: [x for x in t if not x.endswith("libz_gof2.a")])
    mut("no_libzip", lambda t: [x for x in t if not x.endswith("libzip_gof2.a")])
    def whole_crypto(t):
        t = list(t)
        c = next(x for x in t if x.endswith("libcrypto_gof2.a"))
        i = t.index(c)
        return t[:i] + ["--whole-archive", c, "--no-whole-archive"] + t[i + 1:]
    mut("whole_archive_libcrypto", whole_crypto)
    mut("no_crypto", lambda t: [x for x in t if not x.endswith("libcrypto_gof2.a")])
    mut("no_aos", lambda t: [x for x in t if not x.endswith("libandroid_support.a")])
    mut("no_libgcc", lambda t: [x for x in t if not x.endswith("libgcc.a")])
    mut("hash_gnu", lambda t: t + ["--hash-style=gnu"])
    mut("hash_both", lambda t: t + ["--hash-style=both"])
    mut("hash_sysv", lambda t: t + ["--hash-style=sysv"])
    mut("sort_name", lambda t: t + ["--sort-section=name"])
    mut("gc_sections", lambda t: t + ["--gc-sections"])
    mut("rosegment", lambda t: t + ["--rosegment"])
    mut("O1", lambda t: t + ["-O1"])
    mut("O2", lambda t: t + ["-O2"])
    mut("buildid_md5", lambda t: rep(t, "--build-id", "--build-id=md5"))
    mut("buildid_uuid", lambda t: rep(t, "--build-id", "--build-id=uuid"))
    mut("buildid_none", lambda t: rep(t, "--build-id", "--build-id=none"))
    mut("buildid_sha1", lambda t: rep(t, "--build-id", "--build-id=sha1"))
    mut("no_soname", lambda t: rm(t, "-soname", "libgof2hdaa.so"))
    def before_libs(t, flag):
        i = t.index("--exclude-libs=libgcc.a") + 1
        return t[:i] + [flag] + t[i:]
    mut("as_needed", lambda t: before_libs(t, "--as-needed"))
    mut("no_as_needed", lambda t: before_libs(t, "--no-as-needed"))
    mut("no_undefined", lambda t: t + ["--no-undefined"])
    mut("allow_multiple", lambda t: t + ["--allow-multiple-definition"])
    mut("unresolved_ignore", lambda t: t + ["--unresolved-symbols=ignore-all"])
    mut("fix_cortex", lambda t: t + ["--fix-cortex-a8"])
    mut("warn_textrel", lambda t: t + ["--warn-shared-textrel"])
    mut("z_text", lambda t: t + ["-z", "text"])
    mut("z_notext", lambda t: t + ["-z", "notext"])
    mut("z_muldefs", lambda t: t + ["-z", "muldefs"])
    mut("no_copy_dt_needed", lambda t: t + ["--no-copy-dt-needed-entries"])
    mut("no_add_needed", lambda t: t + ["--no-add-needed"])
    mut("no_allow_shlib_undef", lambda t: t + ["--no-allow-shlib-undefined"])
    mut("no_keep_memory", lambda t: t + ["--no-keep-memory"])
    def move_after(t, moving, anchor):
        t = list(t)
        items = [x for x in moving if x in t]
        for x in items:
            t.remove(x)
        i = t.index(anchor) + 1
        return t[:i] + items + t[i:]
    def move_before(t, moving, anchor):
        t = list(t)
        items = [x for x in moving if x in t]
        for x in items:
            t.remove(x)
        i = t.index(anchor)
        return t[:i] + items + t[i:]
    def find(t, suffix):
        return next(x for x in t if x.endswith(suffix))
    mut("swap_fmod_cxxabi", lambda t: move_after(t, [find(t, "libfmodex.so")], find(t, "libc++abi.a")))
    mut("fmod_after_libgcc", lambda t: move_after(t, [find(t, "libfmodex.so")], find(t, "libgcc.a")))
    mut("libgcc_before_cxxabi", lambda t: move_before(t, [find(t, "libgcc.a")], find(t, "libc++abi.a")))
    mut("aos_before_cxxabi", lambda t: move_before(t, [find(t, "libandroid_support.a")], find(t, "libc++abi.a")))
    def swap_libs(t):
        t = list(t)
        a = t.index(find(t, "libz_gof2.a"))
        b = t.index(find(t, "libzip_gof2.a"))
        t[a], t[b] = t[b], t[a]
        return t
    mut("swap_zlib_libzip", swap_libs)
    mut("emit_relocs", lambda t: t + ["--emit-relocs"])
    mut("sort_alignment", lambda t: t + ["--sort-section=alignment"])
    for name, tokens in V:
        run(name, tokens)


if __name__ == "__main__":
    main()
