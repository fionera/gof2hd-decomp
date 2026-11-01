#!/usr/bin/env python3
"""Classify the byte_exact losses caused by adding -fdata-sections.

For every function that is byte-exact in the baseline but not in the
-fdata-sections build, disassemble both copies, find the literal-pool words
that differ, resolve each PIC `ldr/add pc` pair to its destination, and count
the destination sections.  A pool word whose destination is the same logical
object in both builds but at a different address is a pure data-layout delta.
"""
import argparse, collections, json, os, re, struct, sys, tempfile

_here = os.path.dirname(os.path.abspath(__file__))
ROOT = os.environ.get('GOF2_ROOT')
if ROOT is None:
    _cand = os.path.dirname(os.path.dirname(_here))
    ROOT = _cand if os.path.isdir(os.path.join(_cand, 'tools', 'verify')) else os.getcwd()
sys.path.insert(0, os.path.join(ROOT, 'tools', 'verify'))
import verify
from elf import Elf32

OBJ = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/'
                   'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
                   'arm-linux-androideabi-objdump')

def analyze(blob, thumb, base):
    with tempfile.NamedTemporaryFile(suffix='.bin', delete=False) as f:
        f.write(blob); tmp = f.name
    try:
        lines = verify._objdump_lines(OBJ, tmp, thumb)
    finally:
        os.unlink(tmp)
    pool, pic = set(), {}
    for i, (addr, insn) in enumerate(lines):
        pm = verify.PCREL_LOAD.match(insn)
        if not pm:
            continue
        mnem, reg, imm = pm.group(1), pm.group(2), int(pm.group(3) or 0)
        tgt = ((addr + (4 if thumb else 8)) & ~3) + imm
        width = 8 if (mnem == 'ldrd' or (mnem == 'vldr' and reg.startswith('d'))) else 4
        pool.update(range(tgt, tgt + width))
        for laddr, later in lines[i + 1:i + 17]:
            if re.match(rf'^add(?:\.[nw])?\s+{reg},\s*pc$', later):
                if tgt + 4 <= len(blob):
                    delta = struct.unpack_from('<I', blob, tgt)[0]
                    pic[tgt] = (base + laddr + (4 if thumb else 8) + delta) & 0xffffffff
                break
            lm = re.match(r'^([a-z]+)\S*\s+(\w+)', later)
            if lm and lm.group(2) == reg and not re.match(
                    r'^(str|cmp|cmn|tst|teq|push|stm|vst|b)', lm.group(1)):
                break
    return pool, pic

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--base', default='cmake-build-match/libgof2hdaa.so')
    ap.add_argument('--fdata', required=True)
    ap.add_argument('--base-report', default='cmake-build-match/verify/report.json')
    ap.add_argument('--fdata-report', required=True)
    args = ap.parse_args()
    base_e, fdata_e = Elf32(args.base), Elf32(args.fdata)
    bf, ff = base_e.functions(), fdata_e.functions()
    br = {r['name']: r for r in json.load(open(args.base_report))['rows']}
    fr = {r['name']: r for r in json.load(open(args.fdata_report))['rows']}
    losses = [n for n in bf if n in ff and br[n]['byte'] and not fr[n]['byte']]
    words = collections.Counter()
    funcs = collections.Counter()
    pct_changed = 0
    for n in sorted(losses):
        if br[n]['pct'] != fr[n]['pct']:
            pct_changed += 1
        o, u = bf[n], ff[n]
        if o['size'] != u['size']:
            continue
        sz = o['size']
        bo = base_e.bytes_at(o['value'], sz)
        bu = fdata_e.bytes_at(u['value'], sz)
        po, pico = analyze(bo, o['thumb'], o['value'])
        pu, picu = analyze(bu, u['thumb'], u['value'])
        seen = set()
        for w in range(0, sz, 4):
            if bo[w:w + 4] == bu[w:w + 4] or w not in po or w not in pu:
                continue
            sec_o = base_e.section_of(pico.get(w)) if w in pico else None
            sec_u = fdata_e.section_of(picu.get(w)) if w in picu else None
            words[(sec_o, sec_u)] += 1
            seen.add((sec_o, sec_u))
        for k in seen:
            funcs[k] += 1
    print('losses:', len(losses), 'pct_changed:', pct_changed)
    print('pool words by destination section:', dict(words))
    print('functions by destination section:', dict(funcs))
    print('total pool words:', sum(words.values()))

if __name__ == '__main__':
    main()
