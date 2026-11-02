#!/usr/bin/env python3
"""w3692 object-level instruction-stream oracle.

Compile one TU with the exact command from cmake-build-match/compile_commands.json
(optionally appending flags), extract one function's .text.<mangled> section, and
compare its normalized instruction stream against the original libgof2hdaa.so.

This is a screening tool, not the gate. It masks external-call relocation targets
(comparing them by symbol name), literal-pool words and tbb/tbh jump tables, and
compares intra-function branches as function-relative offsets. Its `exact` result is
equivalent to the gate's linked_exact for the screened function; confirm every hit
with `cmake --build cmake-build-match --target check`.

Usage (from the repo root, after a baseline build):
  python3 _work/reconstructions/w3692_oracle.py \
      --src src/game/world/StarMap.cpp --fn _ZN7StarMap4drawEv
  python3 _work/reconstructions/w3692_oracle.py \
      --src src/game/core/String.cpp --fn _ZN11AbyssEngine6String7CompareEPKc \
      --flags='-D__builtin_assume(x)='

Validated against tools/verify/verify.py at base 88aaf59f:
  StarMap::update        verify 100.0  oracle 100.0  (byte-exact control)
  StarMap::draw          verify  94.8  oracle  94.6
  MGame::OnUpdate        verify  99.9  oracle  99.8
  Layout::Layout         verify  46.8  oracle  46.6
  LevelScript::process   verify  54.3  oracle  55.5
"""
import argparse
import difflib
import json
import os
import re
import shlex
import subprocess
import sys
import tempfile
from pathlib import Path

ROOT = Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT / "tools" / "verify"))
import verify  # noqa: E402
from elf import Elf32  # noqa: E402

NDK = ROOT / ".cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin"
OD = str(NDK / "arm-linux-androideabi-objdump")
OC = str(NDK / "arm-linux-androideabi-objcopy")
NM = str(NDK / "arm-linux-androideabi-nm")
ORIG = str(ROOT / "../_work/bins/android_2.0.16_libgof2hdaa.so")
CC = json.load(open(ROOT / "cmake-build-match/compile_commands.json"))

_OF = Elf32(ORIG)
_FNS = _OF.functions()


def _addr_range(elf):
    mapped = [s for s in elf.sections if s["addr"]]
    lo = next((s["addr"] for s in mapped if s["name"] == ".plt"),
              min(s["addr"] for s in mapped))
    return lo, max(s["addr"] + s["size"] for s in mapped)


_RES = verify.Resolver(_FNS, set(_FNS), _OF.plt_map(), _addr_range(_OF))
_RES.attach_data(_OF, {o[2] for o in _OF.objects()})
_RES.imports = {v for k, v in _OF.got_map().values() if k == "sym"}


def compile_tu(src, extra, out):
    e = next(x for x in CC if x["file"] == str(ROOT / src))
    cmd = shlex.split(e["command"])
    i = cmd.index("-o")
    cmd[i + 1] = out
    cmd += extra
    r = subprocess.run(cmd, cwd=e["directory"], capture_output=True, text=True)
    if r.returncode:
        raise RuntimeError(r.stderr[-3000:])


def objdump_section(obj, sec):
    out = subprocess.check_output([OD, "-dr", "-j", sec, obj], text=True, errors="replace")
    lines, relocs = [], {}
    for ln in out.splitlines():
        m = verify.ADDR_COL.match(ln)
        if m:
            lines.append((int(m.group(1), 16), verify.ANNOT.sub("", m.group(2)).strip()))
            continue
        m = re.match(r"^\s*([0-9a-f]+):\s+R_ARM_(\S+)\s+(\S+)", ln)
        if m:
            relocs[int(m.group(1), 16)] = (m.group(2), m.group(3))
    return lines, relocs


def section_bytes(obj, sec):
    fd, path = tempfile.mkstemp(suffix=".bin")
    os.close(fd)
    subprocess.check_call([OC, "--dump-section", f"{sec}={path}", obj])
    b = open(path, "rb").read()
    os.unlink(path)
    return b


def norm_obj(ob, lines, relocs, size):
    pool = verify._pool_bytes(lines, True)
    jtab = verify._jump_tables(lines, ob, pool)
    skip = pool | jtab
    norm = []
    for addr, insn in lines:
        if addr in skip or insn.startswith((".word", ".short", ".byte")):
            continue
        parts = insn.split(None, 1)
        if len(parts) == 2 and verify.BRANCH.match(parts[0]):
            mnem, ops = parts
            m = None
            # object objdump prints branch targets as bare hex (no 0x)
            for m in re.finditer(r"(?<![\w#])(?:0x)?[0-9a-fA-F]+(?![\w])", ops):
                pass
            regs = {"r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9",
                    "r10", "r11", "r12", "sp", "lr", "pc", "ip", "fp", "sl", "sb"}
            if m and m.group(0).lower() not in regs:
                t = int(m.group(0), 16)
                rel = relocs.get(addr)
                if rel:
                    sym = rel[1]
                    sub = ("sym:" + sym) if sym in _FNS else "#X"
                elif t < size:
                    sub = f"loc_{t:x}"
                else:
                    sub = "#X"
                if mnem in ("bl", "blx"):
                    mnem = "call"
                insn = f"{mnem}\t{ops[:m.start()]}{sub}{ops[m.end():]}"
        insn = re.sub(r"^(?:bl|blx)\b", "call", insn)
        norm.append(insn)
    return norm


def norm_orig(name):
    f = _FNS[name]
    blob = _OF.bytes_at(f["value"], f["size"])
    raw, norm = verify.disassemble(OD, blob, True, f["value"], _RES)
    out = []
    for r, n in zip(raw, norm):
        if r.startswith((".word", ".short", ".byte")):
            continue
        if n.startswith(("blx\t", "bl\t")):
            n = "call\t" + n.split("\t", 1)[1]
        out.append(n)
    return f, blob, out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--src", required=True, help="source path relative to the repo root")
    ap.add_argument("--fn", required=True, help="mangled function name")
    ap.add_argument("--flags", default="", help="extra compiler flags (shell-quoted string)")
    args = ap.parse_args()
    extra = shlex.split(args.flags)
    out = "/tmp/w3692_oracle.obj"
    compile_tu(args.src, extra, out)
    nm = subprocess.check_output([NM, "-S", out], text=True, errors="replace")
    hdr = subprocess.check_output([OD, "-h", out], text=True, errors="replace")
    actual = {ln.split()[1] for ln in hdr.splitlines()
              if ln.split() and ln.split()[0].isdigit()}
    syms = {ln.split()[3] for ln in nm.splitlines() if len(ln.split()) >= 4}
    sec = None
    if args.fn in syms and (".text." + args.fn) in actual:
        sec = ".text." + args.fn
    elif "C1" in args.fn:
        alt = args.fn.replace("C1", "C2")
        if alt in syms and (".text." + alt) in actual:
            sec = ".text." + alt
    if sec is None:
        print(f"{args.fn}: symbol/section not found", file=sys.stderr)
        return 2
    ob = section_bytes(out, sec)
    _sl, relocs = objdump_section(out, sec)
    fd, path = tempfile.mkstemp(suffix=".bin")
    os.write(fd, ob)
    os.close(fd)
    try:
        lines = verify._objdump_lines(OD, path, True)
    finally:
        os.unlink(path)
    f, blob, onorm = norm_orig(args.fn)
    unorm = norm_obj(ob, lines, relocs, len(ob))
    exact = len(ob) == len(blob) and onorm == unorm
    pct = 100.0 if exact else round(
        difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio() * 100, 1)
    print(json.dumps(dict(name=args.fn, osize=len(blob), usize=len(ob),
                          exact=exact, pct=pct)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
