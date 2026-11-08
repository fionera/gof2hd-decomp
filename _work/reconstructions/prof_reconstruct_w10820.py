#!/usr/bin/env python3
"""Profile-reconstruction harness (wave 162, w10820).

Extracts a function's *original* switch-case body order from the shipped
`libgof2hdaa.so` disassembly (compare chain + TBH jump tables), then feeds a
synthetic instrumented (`-fprofile-instr-use`) or sample
(`-fprofile-sample-use`) profile back into clang 7.0.2 and measures the
residual body.

The extraction side is generic; the profile side reuses the committed
`profgen_w10314.py` helpers (compile_commands.json + the real gold/strip/
reorder_shstrtab link chain).

Usage (from the repo root, after `cmake --preset default ...`):

  python3 _work/reconstructions/prof_reconstruct_w10820.py extract
  python3 _work/reconstructions/prof_reconstruct_w10820.py gradient \
      src/game/world/LevelScript.cpp _ZN11LevelScript7processEi
  python3 _work/reconstructions/prof_reconstruct_w10820.py sample \
      src/game/mission/Status.cpp _ZN6Status16missionCompletedEbbx <prof>

Scratch defaults to /dev/shm/w10820/run (override with W10820_SCRATCH).
"""
import os, re, struct, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
OD = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/'
                  'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
                  'arm-linux-androideabi-objdump')
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'

# LevelScript::process mission switch: the compare chain starts at 0x1387c6
# (after the isCampaignMission/isEmpty guard) and the two contiguous ranges
# (131..145 and 64..73) are TBH tables at 0x138a8e / 0x138c42.
LS_CHAIN = (0x1387c6, 0x138862)
LS_TABLES = [(0x138a8e, 15, 131), (0x138c42, 10, 64)]
LS_DEFAULT = 0x144e36


def disasm(start, stop, path=ORIG):
    out = subprocess.check_output(
        [OD, '-d', '--start-address=%d' % start, '--stop-address=%d' % stop, path],
        text=True)
    rows = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}\s+){1,2}(.*)$', line)
        if m:
            rows.append((int(m.group(1), 16), m.group(2).strip()))
    return rows


def text_vma_off(path=ORIG):
    out = subprocess.check_output([OD, '-h', path], text=True)
    for line in out.splitlines():
        p = line.split()
        if len(p) > 5 and p[1] == '.text':
            return int(p[4], 16), int(p[5], 16)
    raise SystemExit('no .text')


def extract_chain(start, stop):
    """Return [(case_value, target_addr)] for `cmp r0,#N` + `beq/bne target`."""
    last = None
    out = []
    for addr, text in disasm(start, stop):
        m = re.match(r'cmp\s+r0,\s*#(\d+)', text)
        if m:
            last = int(m.group(1))
            continue
        m = re.match(r'(?:beq|bne)(?:\.w)?\s+([0-9a-f]+)', text)
        if m and last is not None:
            out.append((last, int(m.group(1), 16)))
            last = None
    return out


def extract_tables(tables):
    vma, off = text_vma_off()
    data = open(ORIG, 'rb').read()
    out = []
    for base, n, first in tables:
        for i in range(n):
            half = struct.unpack_from('<H', data, off + (base - vma) + 2 * i)[0]
            out.append((first + i, base + 2 * half))
    return out


def orig_mission_order():
    """Case values ordered by the original's physical case-body address."""
    pairs = extract_chain(*LS_CHAIN) + extract_tables(LS_TABLES)
    pairs = [(c, a) for c, a in pairs if a != LS_DEFAULT]
    return [c for c, _ in sorted(pairs, key=lambda x: (x[1], x[0]))]


def _profgen():
    sys.path.insert(0, os.path.join(ROOT, '_work/reconstructions'))
    import profgen_w10314 as P
    P.REPO = ROOT
    P.BUILD = os.path.join(ROOT, 'cmake-build-match')
    P.NDK = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b')
    P.PROFDATA = os.path.join(P.NDK, 'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
    P.OBJDUMP = OD
    P.VERIFY = os.path.join(ROOT, 'tools/verify/verify.py')
    P.SCRATCH = os.environ.get('W10820_SCRATCH', '/dev/shm/w10820/run')
    os.makedirs(P.SCRATCH, exist_ok=True)
    return P


def _switch_map(P, tu, mangle, ncases):
    """Map source case value -> instrumented counter index for the ncases switch."""
    p = P.Prof(tu, mangle)
    ir = os.path.join(P.SCRATCH, os.path.basename(tu) + '.instr.ll')
    text = open(ir).read()
    for m in re.finditer(r'switch i32 %(\d+), label %(\d+) \[(.*?)\n  \]', text, re.S):
        cases = [(int(v), int(l)) for v, l in
                 re.findall(r'i32 (-?\d+), label %(\d+)', m.group(3))]
        if len(cases) == ncases:
            return p, {v: p.meta['lab_ctr'].get(l) for v, l in cases}
    raise SystemExit('switch with %d cases not found' % ncases)


def gradient(tu, mangle, order, ncases=31, base=1, top=1000, extra=()):
    P = _profgen()
    p, val2ctr = _switch_map(P, tu, mangle, ncases)
    vals = [base] * p.meta['ncounters']
    n = len(order)
    for i, val in enumerate(order):
        ctr = val2ctr.get(val)
        if ctr is not None:
            vals[ctr] = top - i * (top // (n + 1))
    m = p.build(vals, 'grad_%d' % ncases, extra)
    print('raw=%d aligned=%d size=%d other_same=%d/%d' %
          (m['raw'], m['aligned'], m['size'], m['other_same'], m['other_total']))
    return m


def sample(tu, mangle, prof, extra=()):
    P = _profgen()
    cmd = P.compile_cmd(tu)
    m = re.search(r'-o (\S+)', cmd)
    obj = os.path.join(P.SCRATCH, 'sample.obj')
    c = cmd.replace(m.group(1), obj)
    flags = ['-fprofile-sample-use=%s' % os.path.abspath(prof)] + list(extra)
    c = c.replace(' -c ', ' ' + ' '.join(flags) + ' -c ', 1)
    P.run(c, cwd=P.BUILD)
    so = os.path.join(P.SCRATCH, 'sample.so')
    P.link_with([(P.obj_path(tu), obj)], so)
    mm = P.measure(so, mangle)
    print('raw=%d aligned=%d size=%d' % (mm['raw'], mm['aligned'], mm['size']))
    return mm


if __name__ == '__main__':
    cmd = sys.argv[1]
    if cmd == 'extract':
        print('original mission-case physical order:')
        print(orig_mission_order())
    elif cmd == 'gradient':
        gradient(sys.argv[2], sys.argv[3], orig_mission_order())
    elif cmd == 'sample':
        sample(sys.argv[2], sys.argv[3], sys.argv[4], sys.argv[5:])
    else:
        raise SystemExit(__doc__)
