#!/usr/bin/env python3
"""
gofdiff.py — diff a recovered function's compiled code against the target binary.

Disassembles a target function from the .so (by ELF vaddr, Thumb) and the built object's
function (by symbol substring), normalizes away addresses/relocs, and reports an instruction
match ratio + a unified diff. The core of the per-function matching loop.

Run in the guest (needs arm-linux-gnueabihf-objdump).
Usage:
  gofdiff.py --so <libgof2hdaa.so> --vaddr <elf_hex> --n <bytes> --obj <built.o> --sym <substr>
Example (ArrayAdd; Ghidra 0x805b4 -> ELF 0x705b4):
  gofdiff.py --so ../_work/bins/android_2.0.16_libgof2hdaa.so --vaddr 705b4 --n 40 \
             --obj build/obj/engine/array.o --sym ArrayAdd
"""
import subprocess, sys, re, difflib, argparse, os

OBJDUMP = os.environ.get("ARM_OBJDUMP", "arm-linux-gnueabihf-objdump")

BR = re.compile(r'(b|cbz|cbnz|beq|bne|bcs|bcc|bmi|bpl|bvs|bvc|bhi|bls|bge|blt|bgt|ble|bhs|blo)(\.[wn])?$')
def normalize(ins):
    ins = re.sub(r';.*', '', ins)                 # drop ; comments
    ins = re.sub(r'\s@\s.*$', '', ins)            # drop objdump ARM '@' comments (resolved literal addr etc.)
    ins = re.sub(r'<[^>]*>', '', ins)             # drop <symbol> annotations
    ins = re.sub(r'\s+', ' ', ins.strip())
    mn = ins.split(' ', 1)[0]
    if mn in ('bl', 'blx'):                        # calls: target differs (reloc vs resolved addr)
        return 'call'
    if BR.match(mn) and ins != 'bx lr':            # branches: local target address differs
        return mn + ' @'
    ins = re.sub(r'\bpc, #[0-9]+\b', 'pc, #@', ins)   # pc-relative load offsets (literal pools)
    ins = re.sub(r'0x[0-9a-f]+', '@', ins)            # hex addresses
    ins = re.sub(r'\b[0-9a-f]{5,}\b', '@', ins)       # bare hex addrs
    return re.sub(r'\s+', ' ', ins).strip()

def target_insns(so, vaddr, n):
    end = vaddr + n
    out = subprocess.run([OBJDUMP, "-d", "--no-show-raw-insn", "-M", "force-thumb",
                          "--start-address=0x%x" % vaddr, "--stop-address=0x%x" % end, so],
                         capture_output=True, text=True).stdout
    insns = []
    for line in out.splitlines():
        m = re.match(r'\s*[0-9a-f]+:\s+(\S.*)', line)
        if m:
            ni = normalize(m.group(1))
            insns.append(ni)
            # function ends at the first return; stops the fixed-window overrun into the next func
            if re.search(r'pop \{[^}]*pc\}', ni) or ni == 'bx lr':
                break
    return insns

def obj_insns(obj, sym):
    out = subprocess.run([OBJDUMP, "-d", "-C", "--no-show-raw-insn", obj], capture_output=True, text=True).stdout
    funcs, cur = {}, None     # demangled-name -> instruction list
    for line in out.splitlines():
        h = re.match(r'[0-9a-f]+ <(.+)>:', line)
        if h: cur = h.group(1); funcs[cur] = []; continue
        if cur is None: continue
        if not line.strip(): cur = None; continue
        m = re.match(r'\s*[0-9a-f]+:\s+(\S.*)', line)
        if not m: continue
        ni = normalize(m.group(1))
        if re.match(r'\.(word|short|hword|byte|inst|long)\b', ni): continue   # skip literal-pool data
        funcs[cur].append(ni)
        if re.search(r'pop \{[^}]*pc\}', ni) or ni == 'bx lr':                 # stop at the return
            cur = None
    matched = [v for k, v in funcs.items() if sym in k and v]
    if matched:
        return max(matched, key=len)   # the method itself, not a smaller same-prefix helper
    # sym-fallback: Ghidra short names (e.g. cast operators) don't substring the demangled symbol —
    # if nothing matched, use the single (or largest) defined function in this single-method object.
    nonempty = [v for v in funcs.values() if v]
    return max(nonempty, key=len) if nonempty else []

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--so', required=True); ap.add_argument('--vaddr', required=True)
    ap.add_argument('--n', type=int, default=64); ap.add_argument('--obj', required=True)
    ap.add_argument('--sym', required=True)
    a = ap.parse_args()
    tgt = target_insns(a.so, int(a.vaddr, 16), a.n)
    got = obj_insns(a.obj, a.sym)
    # trim target to the built length window (target dump may overrun into the next function)
    if got and len(tgt) > len(got) + 2: tgt = tgt[:len(got)]
    ratio = difflib.SequenceMatcher(None, tgt, got).ratio()
    same = sum(1 for x, y in zip(tgt, got) if x == y)
    print("MATCH %.1f%%  (%d/%d insns identical; target=%d built=%d)" %
          (ratio * 100, same, max(len(tgt), len(got)), len(tgt), len(got)))
    if tgt != got:
        print("--- target            +++ built")
        for line in difflib.unified_diff(tgt, got, lineterm='', n=1):
            if line[:3] in ('---', '+++', '@@ '): continue
            print(("  " if line[0] == ' ' else line[0] + " ") + line[1:].strip())
    else:
        print(">>> EXACT MATCH")

if __name__ == '__main__': main()
