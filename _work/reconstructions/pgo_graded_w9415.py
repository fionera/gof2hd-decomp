#!/usr/bin/env python3
"""Per-TU `-fprofile-instr-use` experiment for Status::missionCompleted (w9415).

Builds a synthetic .profdata whose per-case counters encode a graded switch
weight vector (the w9305 winning family), compiles Status.cpp with
`-fprofile-instr-use`, links with the checked-in gold command, and reports:
  * the switch `!prof` branch_weights actually emitted,
  * the function size and the TBH case-block order,
  * the raw same-address diff and the instruction-index aligned diff,
  * whether the rest of the TU's function sections are byte-identical.

The case -> profile-counter map is derived from a fresh
`-fprofile-instr-generate -emit-llvm` build of the same TU, so it does not
depend on hard-coded counter numbers.

Usage:
  python3 pgo_graded_w9415.py                 # the w9214 vector, all variants
  python3 pgo_graded_w9415.py --vector 1,3,3,4,4,4,4,4,3x12,2x10
"""
import argparse, hashlib, os, re, struct, subprocess, sys, zlib

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BUILD = os.path.join(REPO, 'cmake-build-match')
NDK = os.path.join(REPO, '.cache/ndk/android-ndk-r18b')
PROFDATA = os.path.join(NDK, 'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
OBJDUMP = os.path.join(NDK, 'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump')
ORIG = os.environ.get('GOF2_ORIGINAL_SO',
                      '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
MANGLE = '_ZN6Status16missionCompletedEbbx'
SCRATCH = os.environ.get('W9415_SCRATCH', '/dev/shm/w9415')


def compile_cmd():
    out = subprocess.run(['ninja', '-C', BUILD, '-t', 'commands'],
                         capture_output=True, text=True).stdout
    for c in out.split('\n'):
        if 'mission/Status.cpp' in c and ' -c ' in c:
            return c
    raise SystemExit('no Status.cpp compile command')


def canon_obj(cmd):
    m = re.search(r'-o (\S+)', cmd)
    p = m.group(1)
    return p if os.path.isabs(p) else os.path.join(BUILD, p)


def link_cmd():
    out = subprocess.run(['ninja', '-C', BUILD, '-t', 'commands', 'libgof2hdaa.so'],
                         capture_output=True, text=True).stdout.split('\n')
    for c in out:
        if 'ld.gold' in c and '-shared' in c:
            return c
    raise SystemExit('no link command')


def instrument_ir(cmd, path):
    m = re.search(r'-o (\S+)', cmd)
    c = cmd.replace(m.group(1), path).replace(' -c ', ' -fprofile-instr-generate -emit-llvm -S -c ')
    subprocess.run(c, shell=True, check=True, cwd=BUILD, capture_output=True)


def parse_switch(ir):
    """Return (func_hash, ncounters, {case_value: counter_index}, ir_case_order)."""
    txt = open(ir).read()
    m = re.search(r'define %%class\.Mission\* @%s.*?\n\}' % MANGLE, txt, re.S)
    if not m:
        raise SystemExit('missionCompleted not found in IR')
    fn = m.group(0)
    pl = re.search(r'^@__profd_%s = .*$' % MANGLE, txt, re.M).group(0)
    ints = re.findall(r'i64 (-?\d+)', pl)
    fhash = int(ints[1])
    ncounters = int(re.search(r'i32 (\d+), \[2 x i16\]', pl).group(1))
    cur = None
    label_ctr = {}
    for ln in fn.split('\n'):
        lm = re.match(r'^; <label>:(\d+):', ln)
        if lm:
            cur = int(lm.group(1)); continue
        cm = re.search(r'@__profc_%s, i32 0, i32 (\d+)\)' % MANGLE, ln)
        if cm and cur is not None and cur not in label_ctr and 'load i64' in ln:
            label_ctr[cur] = int(cm.group(1))
    cases = []
    for ln in fn.split('\n'):
        cm = re.match(r'^\s*i32 (\d+), label %(\d+)$', ln)
        if cm:
            cases.append((int(cm.group(1)), int(cm.group(2))))
    return fhash, ncounters, {v: label_ctr[l] for v, l in cases}, [v for v, _ in cases]


def write_profile(fhash, ncounters, values, path):
    with open(path, 'w') as f:
        f.write('%s\n# Func Hash:\n%d\n# Num Counters:\n%d\n# Counter Values:\n'
                % (MANGLE, fhash, ncounters))
        for v in values:
            f.write('%d\n' % v)


def merge(text, data):
    subprocess.run([PROFDATA, 'merge', text, '-o', data], check=True,
                   capture_output=True)


def compile_use(cmd, profdata, obj):
    m = re.search(r'-o (\S+)', cmd)
    c = cmd.replace(m.group(1), obj).replace(' -c ', ' -fprofile-instr-use=%s -c ' % profdata)
    subprocess.run(c, shell=True, check=True, cwd=BUILD, capture_output=True)


def link_with(obj, so):
    lc = link_cmd()
    lc = lc.replace('-o %s/libgof2hdaa.unstripped.so' % BUILD, '-o %s' % so)
    lc = lc.replace('%s/CMakeFiles/gof2.dir/./src/game/mission/Status.cpp.obj' % BUILD, obj)
    i = lc.find(' && ' + os.path.join(NDK, 'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-strip'))
    if i >= 0:
        lc = lc[:i]
    subprocess.run(lc, shell=True, check=True, cwd=BUILD, capture_output=True)


def sym(path, name):
    out = subprocess.run([OBJDUMP, '-T', path], capture_output=True, text=True).stdout
    for l in out.split('\n'):
        p = l.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    return None


def textsec(path):
    out = subprocess.run([OBJDUMP, '-h', path], capture_output=True, text=True).stdout
    for l in out.split('\n'):
        p = l.split()
        if len(p) > 5 and p[1] == '.text':
            return int(p[3], 16), int(p[4], 16), int(p[5], 16)
    raise SystemExit('no .text')


def text_bytes(path, addr, size):
    d = open(path, 'rb').read()
    _, vma, off = textsec(path)
    return d[off + addr - vma: off + addr - vma + size]


def raw_diff(so):
    a = sym(ORIG, MANGLE); b = sym(so, MANGLE)
    if not a or not b:
        return None
    x = text_bytes(ORIG, *a); y = text_bytes(so, *b)
    n = min(len(x), len(y))
    return sum(p != q for p, q in zip(x[:n], y[:n])), len(x), len(y)


def table_order(so):
    out = subprocess.run(['python3', os.path.join(REPO, '_work/reconstructions/jumptable_w9319.py'), so],
                         capture_output=True, text=True).stdout
    m = re.search(r'base=150 N=39.*?\n\s*phys: \[([^\]]*)\]', out, re.S)
    if not m:
        return None
    return [int(x) for x in m.group(1).split(',')]


SRC_ORDER = [167, 174, 150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184,
             189, 156, 157, 160, 163, 164, 165, 168, 169, 171, 172]


def parse_vector(s):
    out = []
    for part in s.split(','):
        if 'x' in part:
            v, n = part.split('x'); out += [int(v)] * int(n)
        else:
            out.append(int(part))
    return out


def build_vector(weights, ir_order):
    """weights: [default, case weights in ir_order] -> counter values."""
    return weights


def make_counters(weights, ir_order, case_ctr, ncounters, base, body):
    """base: value for non-case counters; body: True -> whole case body gets the
    case weight, False -> only the case-entry block."""
    vals = [base] * ncounters
    seen = set()
    for i, c in enumerate(ir_order):
        ctr = case_ctr[c]
        if ctr in seen:
            continue
        seen.add(ctr)
        vals[ctr] = weights[i + 1] - 1
        if body:
            # counters between this case entry and the next case entry belong
            # to this case body (frontend counter order is source order).
            nxt = ncounters
            for c2 in ir_order[i + 1:]:
                if case_ctr[c2] > ctr:
                    nxt = case_ctr[c2]; break
            for k in range(ctr + 1, nxt):
                vals[k] = weights[i + 1] - 1
    return vals


def obj_sections(path):
    d = open(path, 'rb').read()
    e_shoff, = struct.unpack_from('<I', d, 0x20)
    e_shentsize, e_shnum, e_shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    secs = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        name, typ, flags, addr, offset, size = struct.unpack_from('<IIIIII', d, off)
        secs.append(dict(name_off=name, typ=typ, offset=offset, size=size))
    shstr = secs[e_shstrndx]
    st = d[shstr['offset']:shstr['offset'] + shstr['size']]
    out = {}
    for s in secs:
        end = st.find(b'\0', s['name_off'])
        n = st[s['name_off']:end].decode('latin1')
        if s['typ'] != 8 and n:
            out[n] = d[s['offset']:s['offset'] + s['size']]
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--vector', default='1,3,3,4,4,4,4,4,3x12,2x10')
    ap.add_argument('--base', type=int, default=None, help='sweep base values')
    ap.add_argument('--body', action='store_true', help='also weight the whole case body')
    a = ap.parse_args()

    os.makedirs(SCRATCH, exist_ok=True)
    cmd = compile_cmd()
    ir = os.path.join(SCRATCH, 'status_instr.ll')
    instrument_ir(cmd, ir)
    fhash, ncounters, case_ctr, ir_order = parse_switch(ir)
    print('FuncHash %d  NumCounters %d' % (fhash, ncounters))
    print('IR case order:', ir_order)

    weights = parse_vector(a.vector)
    assert len(weights) == len(ir_order) + 1, (len(weights), len(ir_order) + 1)

    canon = canon_obj(cmd)
    canon_secs = obj_sections(canon)
    bases = [a.base] if a.base is not None else [0, 1]
    for base in bases:
        tag = 'base%d_%s' % (base, 'body' if a.body else 'entry')
        vals = make_counters(weights, ir_order, case_ctr, ncounters, base, a.body)
        txt = os.path.join(SCRATCH, tag + '.proftext')
        dat = os.path.join(SCRATCH, tag + '.profdata')
        write_profile(fhash, ncounters, vals, txt)
        merge(txt, dat)
        obj = os.path.join(SCRATCH, tag + '.obj')
        compile_use(cmd, dat, obj)
        so = os.path.join(SCRATCH, tag + '.so')
        link_with(obj, so)
        rd = raw_diff(so)
        order = table_order(so)
        secs = obj_sections(obj)
        others = [n for n in canon_secs if n.startswith('.text.') and MANGLE not in n]
        same = sum(1 for n in others if n in secs and secs[n] == canon_secs[n])
        print('--- %s' % tag)
        print('  raw diff / orig size / ours size:', rd)
        print('  case order source?', order is not None and
              [v for v in order if v in SRC_ORDER] == SRC_ORDER)
        print('  phys:', order)
        print('  other .text sections identical: %d/%d' % (same, len(others)))
        print('  counters:', ','.join(str(v) for v in vals))


if __name__ == '__main__':
    main()
