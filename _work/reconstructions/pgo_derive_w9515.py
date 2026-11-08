#!/usr/bin/env python3
"""Per-TU `-fprofile-instr-use` experiment for Status::missionCompleted (w9515).

Extends pgo_graded_w9415.py: instead of a hand-written graded vector, it
DERIVES the per-case weights from the ORIGINAL binary's switch-table block
order (the physical order of the TBH targets), then replays them as a
synthetic `.profdata`.  Non-case counters get a uniform `base`; the three
loop-latch counters (6,7,12, the phi in the backedge block) are forced to 0 so
the latch block is placed last, and counter 28 (the shared `case 0/11` block)
can be overridden to compensate LLVM 7's shared-successor weight quirk.

Usage:
  python3 pgo_derive_w9515.py --base 12 --c28 28     # both tables exact (1428 B)
  python3 pgo_derive_w9515.py --base 2               # 150-table exact (1432 B)
  python3 pgo_derive_w9515.py --dump                 # print the counter vector
"""
import argparse, os, re, struct, subprocess

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BUILD = os.path.join(REPO, 'cmake-build-match')
NDK = os.path.join(REPO, '.cache/ndk/android-ndk-r18b')
PROFDATA = os.path.join(NDK, 'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
OBJDUMP = os.path.join(NDK, 'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump')
ORIG = os.environ.get('GOF2_ORIGINAL_SO',
                      '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
MANGLE = '_ZN6Status16missionCompletedEbbx'
SCRATCH = os.environ.get('W9515_SCRATCH', '/dev/shm/w9515')


def compile_cmd():
    out = subprocess.run(['ninja', '-C', BUILD, '-t', 'commands'],
                         capture_output=True, text=True).stdout
    for c in out.split('\n'):
        if 'mission/Status.cpp' in c and ' -c ' in c:
            return c
    raise SystemExit('no Status.cpp compile command')


def link_cmd():
    out = subprocess.run(['ninja', '-C', BUILD, '-t', 'commands', 'libgof2hdaa.so'],
                         capture_output=True, text=True).stdout.split('\n')
    for c in out:
        if 'ld.gold' in c and '-shared' in c:
            return c
    raise SystemExit('no link command')


def sym(path, name):
    out = subprocess.run([OBJDUMP, '-T', path], capture_output=True, text=True).stdout
    for l in out.split('\n'):
        p = l.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    return None


def text_section(path):
    out = subprocess.run([OBJDUMP, '-h', path], capture_output=True, text=True).stdout
    for l in out.split('\n'):
        p = l.split()
        if len(p) > 5 and p[1] == '.text':
            return int(p[3], 16), int(p[5], 16)
    raise SystemExit('no .text')


def disasm(path, addr, size):
    out = subprocess.run([OBJDUMP, '-d', '--no-show-raw-insn',
                          '--start-address=%d' % addr, '--stop-address=%d' % (addr + size),
                          path], capture_output=True, text=True).stdout
    rows = []
    for ln in out.split('\n'):
        m = re.match(r'^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$', ln)
        if m:
            rows.append((int(m.group(1), 16), m.group(2), m.group(3).strip()))
    return rows


def tbh_orders(path, func_addr):
    """Return {case_base: [values in physical (target-address) order]}."""
    d = open(path, 'rb').read()
    vma, off = text_section(path)

    def rd(v, n):
        return d[off + v - vma: off + v - vma + n]

    rows = disasm(path, func_addr, 0x600)
    out = {}
    for i, (a, mn, ops) in enumerate(rows):
        if mn not in ('tbh', 'tbb'):
            continue
        if mn == 'tbh':
            m = re.search(r'\[pc,\s*(r\d+),\s*lsl\s*#1\]', ops)
        else:
            m = re.search(r'\[pc,\s*(r\d+)\]', ops)
        if not m:
            continue
        reg, N, base = m.group(1), None, None
        for j in range(i - 1, max(-1, i - 9), -1):
            _, mn2, ops2 = rows[j]
            if mn2 == 'cmp':
                mm = re.match(rf'{reg},\s*#(\d+)', ops2)
                if mm:
                    N = int(mm.group(1)); continue
            if mn2 in ('sub', 'sub.w', 'subs', 'subs.w'):
                mm = re.match(rf'{reg},\s*r\d+,\s*#(\d+)', ops2)
                if not mm:
                    mm = re.match(rf'{reg},\s*#(\d+)', ops2)
                if mm:
                    base = int(mm.group(1)); continue
        if N is None or base is None:
            continue
        tbl = a + 4
        if mn == 'tbh':
            ents = struct.unpack_from('<%dh' % (N + 1), rd(tbl, (N + 1) * 2))
            targets = [tbl + 2 * e for e in ents]
        else:
            ents = struct.unpack_from('<%dB' % (N + 1), rd(tbl, N + 1))
            targets = [tbl + e for e in ents]
        order = sorted(range(N + 1), key=lambda k: (targets[k], k))
        out[base] = [base + k for k in order]
    return out


def instrumented_ir(cmd, path):
    m = re.search(r'-o (\S+)', cmd)
    c = cmd.replace(m.group(1), path).replace(' -c ', ' -fprofile-instr-generate -emit-llvm -S -c ')
    subprocess.run(c, shell=True, check=True, cwd=BUILD, capture_output=True)


def parse_profile(cmd, ir):
    txt = open(ir).read()
    m = re.search(r'define %%class\.Mission\* @%s.*?\n\}' % MANGLE, txt, re.S)
    fn = m.group(0)
    pl = re.search(r'^@__profd_%s = .*$' % MANGLE, txt, re.M).group(0)
    ints = re.findall(r'i64 (-?\d+)', pl)
    fhash = int(ints[1])
    ncounters = int(re.search(r'i32 (\d+), \[2 x i16\]', pl).group(1))
    cur, lab_ctr = None, {}
    for ln in fn.split('\n'):
        lm = re.match(r'^; <label>:(\d+):', ln)
        if lm:
            cur = int(lm.group(1)); continue
        cm = re.search(r'@__profc_%s, i32 0, i32 (\d+)\)' % MANGLE, ln)
        if cm and cur is not None and cur not in lab_ctr and 'load i64' in ln:
            lab_ctr[cur] = int(cm.group(1))
    cases = []
    for ln in fn.split('\n'):
        cm = re.match(r'^\s*i32 (\d+), label %(\d+)$', ln)
        if cm:
            cases.append((int(cm.group(1)), int(cm.group(2))))
    case_ctr = {}
    order = []
    for v, l in cases:
        if v not in order:
            order.append(v)
        if l in lab_ctr:
            case_ctr.setdefault(v, lab_ctr[l])
    return fhash, ncounters, case_ctr, order


def write_profile(fhash, ncounters, vals, path):
    with open(path, 'w') as f:
        f.write('%s\n# Func Hash:\n%d\n# Num Counters:\n%d\n# Counter Values:\n'
                % (MANGLE, fhash, ncounters))
        for v in vals:
            f.write('%d\n' % v)


def merge(text, data):
    subprocess.run([PROFDATA, 'merge', text, '-o', data], check=True, capture_output=True)


def compile_use(cmd, profdata, obj):
    m = re.search(r'-o (\S+)', cmd)
    c = cmd.replace(m.group(1), obj).replace(' -c ', ' -fprofile-instr-use=%s -c ' % profdata)
    subprocess.run(c, shell=True, check=True, cwd=BUILD, capture_output=True)


def link_with(obj, so):
    lc = link_cmd()
    lc = lc.replace('%s/libgof2hdaa.unstripped.so' % BUILD, so + '.un')
    lc = lc.replace('%s/libgof2hdaa.so' % BUILD, so)
    lc = lc.replace('%s/CMakeFiles/gof2.dir/./src/game/mission/Status.cpp.obj' % BUILD, obj)
    subprocess.run(lc, shell=True, check=True, cwd=BUILD, capture_output=True)


def raw_diff(so):
    a = sym(ORIG, MANGLE); b = sym(so, MANGLE)
    if not a or not b:
        return None
    va, oa = text_section(ORIG); vb, ob = text_section(so)
    da = open(ORIG, 'rb').read(); db = open(so, 'rb').read()
    x = da[oa + a[0] - va: oa + a[0] - va + a[1]]
    y = db[ob + b[0] - vb: ob + b[0] - vb + b[1]]
    n = min(len(x), len(y))
    return sum(p != q for p, q in zip(x[:n], y[:n])), len(x), len(y)


def obj_text_sections(path):
    d = open(path, 'rb').read()
    e_shoff, = struct.unpack_from('<I', d, 0x20)
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    secs = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        name, typ, flags, addr, offset, size = struct.unpack_from('<IIIIII', d, off)
        secs.append(dict(name_off=name, typ=typ, offset=offset, size=size))
    shstr = secs[e_shstrndx]; st = d[shstr['offset']:shstr['offset'] + shstr['size']]
    out = {}
    for s in secs:
        end = st.find(b'\0', s['name_off'])
        n = st[s['name_off']:end].decode('latin1')
        if s['typ'] != 8 and n:
            out[n] = d[s['offset']:s['offset'] + s['size']]
    return out



def _real_case_order(path, func_addr):
    """Case values in physical block order, excluding the default block."""
    d = open(path, 'rb').read()
    vma, off = text_section(path)

    def rd(v, n):
        return d[off + v - vma: off + v - vma + n]

    rows = disasm(path, func_addr, 0x600)
    groups = []  # (target, [values])
    for i, (a, mn, ops) in enumerate(rows):
        if mn not in ('tbh', 'tbb'):
            continue
        if mn == 'tbh':
            m = re.search(r'\[pc,\s*(r\d+),\s*lsl\s*#1\]', ops)
        else:
            m = re.search(r'\[pc,\s*(r\d+)\]', ops)
        if not m:
            continue
        reg, N, base = m.group(1), None, None
        for j in range(i - 1, max(-1, i - 9), -1):
            _, mn2, ops2 = rows[j]
            if mn2 == 'cmp':
                mm = re.match(rf'{reg},\s*#(\d+)', ops2)
                if mm:
                    N = int(mm.group(1)); continue
            if mn2 in ('sub', 'sub.w', 'subs', 'subs.w'):
                mm = re.match(rf'{reg},\s*r\d+,\s*#(\d+)', ops2)
                if not mm:
                    mm = re.match(rf'{reg},\s*#(\d+)', ops2)
                if mm:
                    base = int(mm.group(1)); continue
        if N is None or base is None:
            continue
        tbl = a + 4
        if mn == 'tbh':
            ents = struct.unpack_from('<%dh' % (N + 1), rd(tbl, (N + 1) * 2))
            targets = [tbl + 2 * e for e in ents]
        else:
            ents = struct.unpack_from('<%dB' % (N + 1), rd(tbl, N + 1))
            targets = [tbl + e for e in ents]
        by_t = {}
        for k, t in enumerate(targets):
            by_t.setdefault(t, []).append(base + k)
        groups.extend(by_t.items())
    # sort all groups globally by target address
    groups.sort(key=lambda g: g[0])
    # the default target is the one shared by the most values
    default_t = max(groups, key=lambda g: len(g[1]))[0]
    out = []
    for t, vals in groups:
        if t == default_t:
            continue
        for v in vals:
            if v not in out:
                out.append(v)
    return out

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--base', type=int, default=12, help='value for every non-case counter')
    ap.add_argument('--c28', type=int, default=28, help='override for the shared case-0/11 counter')
    ap.add_argument('--latch', type=int, default=0, help='value for latch counters 6,7,12')
    ap.add_argument('--dump', action='store_true')
    a = ap.parse_args()
    os.makedirs(SCRATCH, exist_ok=True)

    cmd = compile_cmd()
    ir = os.path.join(SCRATCH, 'derive_instr.ll')
    instrumented_ir(cmd, ir)
    fhash, ncounters, case_ctr, ir_order = parse_profile(cmd, ir)
    print('FuncHash %d  NumCounters %d' % (fhash, ncounters))
    print('IR case order:', ir_order)

    # Derive the case order from the ORIGINAL binary's TBH tables.
    orig = tbh_orders(ORIG, sym(ORIG, MANGLE)[0])
    # The default block is the target shared by most values in the 150..189
    # table; exclude those values so the derived order is the real case order.
    full = []
    for base in sorted(orig):
        vals = orig[base]
        # values that map to the same (default) target are the trailing block
        order = vals
        for v in order:
            if v not in full:
                full.append(v)
    # drop the values that sit at the default target: in the original the
    # default block is the last target, so the real cases are the first
    # (len - default_count) values.  Identify the default target by the
    # largest group of values sharing one address.
    full = _real_case_order(ORIG, sym(ORIG, MANGLE)[0])
    print('original TBH orders:', {hex(k): v for k, v in orig.items()})
    print('derived full case order:', full)

    rank = {v: len(full) - i for i, v in enumerate(full)}
    weights = [1] + [rank.get(v, 1) for v in ir_order]
    vals = [a.base] * ncounters
    seen = set()
    for i, v in enumerate(ir_order):
        ctr = case_ctr.get(v)
        if ctr is None or ctr in seen:
            continue
        seen.add(ctr)
        vals[ctr] = weights[i + 1] - 1
    for k in (6, 7, 12):
        vals[k] = a.latch
    vals[28] = a.c28
    if a.dump:
        print('counter values:', ','.join(str(v) for v in vals))

    txt = os.path.join(SCRATCH, 'derive.proftext')
    dat = os.path.join(SCRATCH, 'derive.profdata')
    write_profile(fhash, ncounters, vals, txt)
    merge(txt, dat)
    obj = os.path.join(SCRATCH, 'derive.obj')
    compile_use(cmd, dat, obj)
    so = os.path.join(SCRATCH, 'derive.so')
    link_with(obj, so)

    rd = raw_diff(so)
    print('raw diff / orig size / ours size:', rd)
    got = tbh_orders(so, sym(so, MANGLE)[0])
    for base in sorted(orig):
        o = [v for v in orig[base] if v in got.get(base, [])]
        g = [v for v in got.get(base, []) if v in o]
        print('  table %s: %s (match=%s)' % (base, got.get(base), g == o))

    canon = obj_text_sections(os.path.join(
        BUILD, 'CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj'))
    secs = obj_text_sections(obj)
    others = [n for n in canon if n.startswith('.text.') and MANGLE not in n]
    same = sum(1 for n in others if n in secs and secs[n] == canon[n])
    print('other .text sections identical: %d/%d' % (same, len(others)))


if __name__ == '__main__':
    main()
