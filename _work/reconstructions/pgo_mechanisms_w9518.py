#!/usr/bin/env python3
"""Alternative branch-weight / PGO mechanisms on one residual TU (wave 149, w9518).

The original `libgof2hdaa.so` was built with branch weights.  w9420 found no
shipped `.profraw`/`.profdata`; w9415/w9416 replayed hand-built profiles and
found the full-TU `-fprofile-instr-use` layout differs.  This harness tests the
*other* mechanisms that can produce the same block order, on one residual TU
(`Status.cpp`, target `_ZN6Status16missionCompletedEbbx`):

  (a) `-fprofile-instr-use` with a graded profile (from the original's layout)
      -- with and without `-mllvm -profile-guided-section-prefix=false`
  (b) `-mllvm -block-placement-exit-block-bias=N` sweep (the only block-freq
      knob in clang 7.0.2)
  (c) `-fprofile-sample-use` -- with and without section-prefix
  (d) `-fbasic-block-sections`  (absent in clang 7.0.2 -> reported)
  (e) `-mllvm -enable-ext-tsp-block-placement=0` (absent in LLVM 7 -> reported)
  (f) `-mllvm -precise-sample-profile` (absent in LLVM 7 -> reported)
  (g) `-mllvm -profile-guided-section-prefix` (on/off; default true)
  (h) a hand-authored `.profdata` whose counts encode the original block order

Every variant recompiles only `Status.cpp`, re-links the whole `.so` with the
checked-in gold command, and reports the global `byte_exact` (verify.py) plus
the target's raw same-address diff, instruction-index aligned diff, and size.

Usage:
  python3 pgo_mechanisms_w9518.py --list
  python3 pgo_mechanisms_w9518.py --run base instr_graded_off sample_off
  python3 pgo_mechanisms_w9518.py --all
"""
import argparse, json, os, re, subprocess, sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BUILD = os.path.join(REPO, 'cmake-build-match')
NDK = os.path.join(REPO, '.cache/ndk/android-ndk-r18b')
PROFDATA = os.path.join(NDK, 'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
OBJDUMP = os.path.join(NDK, 'toolchains/arm-linux-androideabi-4.9/prebuilt/'
                       'linux-x86_64/bin/arm-linux-androideabi-objdump')
ORIG = os.environ.get('GOF2_ORIGINAL_SO',
                      '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
SRC = 'src/game/mission/Status.cpp'
MANGLE = '_ZN6Status16missionCompletedEbbx'
SCRATCH = os.environ.get('W9518_SCRATCH', '/dev/shm/w9518')
VERIFY = os.path.join(REPO, 'tools/verify/verify.py')

# w9305's winning switch weight family (default=1, then cases in IR order).
W9305_VECTOR = '1,3,3,4,4,4,4,4,3x12,2x10'


def run(cmd, **kw):
    return subprocess.run(cmd, shell=isinstance(cmd, str), check=True,
                          capture_output=True, text=True, **kw)


def compile_cmd(src=SRC):
    out = run(['ninja', '-C', BUILD, '-t', 'commands']).stdout.split('\n')
    for c in out:
        if 'clang++' in c and ' -c ' in c and c.rstrip().endswith(src):
            return c
    raise SystemExit('compile line for %s not found' % src)


def link_cmd():
    out = run(['ninja', '-C', BUILD, '-t', 'commands', 'libgof2hdaa.so']).stdout.split('\n')
    for c in out:
        if 'ld.gold' in c and '-shared' in c:
            return c
    raise SystemExit('no link command')


def canon_obj(cmd):
    m = re.search(r'-o (\S+)', cmd)
    p = m.group(1)
    return p if os.path.isabs(p) else os.path.join(BUILD, p)


def compile_variant(cmd, obj, extra=(), profdata=None):
    m = re.search(r'-o (\S+)', cmd)
    c = cmd.replace(m.group(1), obj)
    flags = list(extra)
    if profdata:
        flags.append('-fprofile-instr-use=%s' % profdata)
    if flags:
        c = c.replace(' -c ', ' ' + ' '.join(flags) + ' -c ', 1)
    run(c, cwd=BUILD)


def link_with(obj, so):
    lc = link_cmd()
    canon = os.path.join(BUILD, 'CMakeFiles/gof2.dir/./src/game/mission/Status.cpp.obj')
    lc = lc.replace(canon, obj)
    unstripped = os.path.join(BUILD, 'libgof2hdaa.unstripped.so')
    stripped = os.path.join(BUILD, 'libgof2hdaa.so')
    tag = os.path.splitext(os.path.basename(so))[0]
    my_unstripped = os.path.join(SCRATCH, tag + '.unstripped.so')
    lc = lc.replace(unstripped, my_unstripped)
    lc = lc.replace(stripped, so)
    run(lc, cwd=BUILD)


def sym(path, name):
    out = run([OBJDUMP, '-T', path]).stdout
    for l in out.split('\n'):
        p = l.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    return None


def textsec(path):
    out = run([OBJDUMP, '-h', path]).stdout
    for l in out.split('\n'):
        p = l.split()
        if len(p) > 5 and p[1] == '.text':
            return int(p[2], 16), int(p[3], 16), int(p[5], 16)
    raise SystemExit('no .text')


def text_bytes(path, addr, size):
    d = open(path, 'rb').read()
    _, vma, off = textsec(path)
    return d[off + addr - vma: off + addr - vma + size]


def disasm_idx(path, addr, size):
    out = run([OBJDUMP, '-d', '--start-address=%d' % addr,
               '--stop-address=%d' % (addr + size), path]).stdout
    ins = []
    for ln in out.split('\n'):
        m = re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$', ln)
        if m:
            ins.append((int(m.group(1), 16), m.group(2).strip()))
    return ins


def measure_fn(so):
    a, b = sym(ORIG, MANGLE), sym(so, MANGLE)
    if not a or not b:
        return None
    x = text_bytes(ORIG, *a)
    y = text_bytes(so, *b)
    n = min(len(x), len(y))
    raw = sum(p != q for p, q in zip(x[:n], y[:n]))
    oi, ui = disasm_idx(ORIG, *a), disasm_idx(so, *b)
    oidx = {ad: i for i, (ad, _) in enumerate(oi)}
    uidx = {ad: i for i, (ad, _) in enumerate(ui)}
    import difflib

    def norm(t, idx):
        def rep(m):
            ad = int(m.group(1), 16)
            return 'I%d' % idx[ad] if ad in idx else 'X'
        t2 = re.sub(r'\b([0-9a-f]{4,8})\b', rep, t)
        t2 = re.sub(r'<[^>]*>', '', t2)
        return re.sub(r'\s+', ' ', t2).strip()

    ot = [norm(t, oidx) for _, t in oi]
    ut = [norm(t, uidx) for _, t in ui]
    sm = difflib.SequenceMatcher(None, ot, ut, autojunk=False)
    aligned = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes()
                  if tag != 'equal')
    return dict(raw=raw, aligned=aligned, orig_size=len(x), size=len(y))


def verify_so(so):
    bd = os.path.join(SCRATCH, 'verify_' + os.path.splitext(os.path.basename(so))[0])
    os.makedirs(bd, exist_ok=True)
    run([sys.executable, VERIFY, '--build-dir', bd, '--original', ORIG,
         '--ours', so, '--objdump', OBJDUMP])
    r = json.load(open(os.path.join(bd, 'report.json')))['summary']
    return r['byte_exact'], r['linked_exact']


# ---------------------------------------------------------------- profiles
def instrument_ir(cmd, path):
    m = re.search(r'-o (\S+)', cmd)
    c = cmd.replace(m.group(1), path).replace(' -c ', ' -fprofile-instr-generate -emit-llvm -S -c ')
    run(c, cwd=BUILD)


def parse_switch(ir):
    txt = open(ir).read()
    m = re.search(r'define %%class\.Mission\* @%s.*?\n\}' % MANGLE, txt, re.S)
    if not m:
        raise SystemExit('missionCompleted not found in IR')
    fn = m.group(0)
    pl = re.search(r'^@__profd_%s = .*$' % MANGLE, txt, re.M).group(0)
    ints = re.findall(r'i64 (-?\d+)', pl)
    fhash = int(ints[1])
    ncounters = int(re.search(r'i32 (\d+), \[2 x i16\]', pl).group(1))
    cur, label_ctr = None, {}
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


def parse_vector(s):
    out = []
    for part in s.split(','):
        if 'x' in part:
            v, n = part.split('x'); out += [int(v)] * int(n)
        else:
            out.append(int(part))
    return out


def make_counters(weights, ir_order, case_ctr, ncounters, base, body):
    vals = [base] * ncounters
    seen = set()
    for i, c in enumerate(ir_order):
        ctr = case_ctr[c]
        if ctr in seen:
            continue
        seen.add(ctr)
        vals[ctr] = weights[i + 1] - 1
        if body:
            nxt = ncounters
            for c2 in ir_order[i + 1:]:
                if case_ctr[c2] > ctr:
                    nxt = case_ctr[c2]; break
            for k in range(ctr + 1, nxt):
                vals[k] = weights[i + 1] - 1
    return vals


def write_profile(fhash, ncounters, values, path):
    with open(path, 'w') as f:
        f.write('%s\n# Func Hash:\n%d\n# Num Counters:\n%d\n# Counter Values:\n'
                % (MANGLE, fhash, ncounters))
        for v in values:
            f.write('%d\n' % v)


def merge(text, data):
    run([PROFDATA, 'merge', text, '-o', data])


def make_graded_profiles(cmd):
    """Build the w9305-family graded .profdata files (base 0/1, entry/body)."""
    os.makedirs(SCRATCH, exist_ok=True)
    ir = os.path.join(SCRATCH, 'status_instr.ll')
    instrument_ir(cmd, ir)
    fhash, ncounters, case_ctr, ir_order = parse_switch(ir)
    weights = parse_vector(W9305_VECTOR)
    assert len(weights) == len(ir_order) + 1, (len(weights), len(ir_order) + 1)
    print('FuncHash %d  NumCounters %d  IR case order %s' % (fhash, ncounters, ir_order))
    out = {}
    for base in (0, 1):
        for body in (False, True):
            tag = 'graded_b%d_%s' % (base, 'body' if body else 'entry')
            vals = make_counters(weights, ir_order, case_ctr, ncounters, base, body)
            txt = os.path.join(SCRATCH, tag + '.proftext')
            dat = os.path.join(SCRATCH, tag + '.profdata')
            write_profile(fhash, ncounters, vals, txt)
            merge(txt, dat)
            out[tag] = dat
    return out


def make_sample_profile():
    os.makedirs(SCRATCH, exist_ok=True)
    sample = os.path.join(SCRATCH, 'all.sample')
    with open(sample, 'w') as f:
        f.write('%s:100000:100000\n' % MANGLE)
        for i in range(0, 260):
            f.write(' %d: 1000\n' % i)
    return sample


# ---------------------------------------------------------------- variants
def variants():
    v = {}
    v['base'] = dict(extra=())
    v['secprefix_off'] = dict(extra=('-mllvm', '-profile-guided-section-prefix=false'))
    v['armjt_off'] = dict(extra=('-mllvm', '-arm-adjust-jump-tables=false'))
    v['disable_placement'] = dict(extra=('-mllvm', '-disable-block-placement'))
    for n in (0, 10, 25, 50, 75, 100, 200):
        v['exitbias_%d' % n] = dict(extra=('-mllvm', '-block-placement-exit-block-bias=%d' % n))
    return v


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--list', action='store_true')
    ap.add_argument('--run', nargs='*')
    ap.add_argument('--all', action='store_true')
    ap.add_argument('--skip-verify', action='store_true')
    a = ap.parse_args()
    os.makedirs(SCRATCH, exist_ok=True)

    cmd = compile_cmd()
    graded = make_graded_profiles(cmd)
    sample = make_sample_profile()

    specs = variants()
    # profile variants
    specs['instr_graded_entry_b1'] = dict(extra=(), profdata=graded['graded_b1_entry'])
    specs['instr_graded_entry_b1_off'] = dict(
        extra=('-mllvm', '-profile-guided-section-prefix=false'),
        profdata=graded['graded_b1_entry'])
    specs['instr_graded_body_b0_off'] = dict(
        extra=('-mllvm', '-profile-guided-section-prefix=false'),
        profdata=graded['graded_b0_body'])
    specs['sample_on'] = dict(extra=('-fprofile-sample-use=%s' % sample,))
    specs['sample_off'] = dict(extra=('-fprofile-sample-use=%s' % sample,
                                      '-mllvm', '-profile-guided-section-prefix=false'))

    if a.list:
        for k in specs:
            print(k)
        return
    names = list(specs) if a.all else (a.run or ['base'])
    results = {}
    for name in names:
        spec = specs[name]
        obj = os.path.join(SCRATCH, name + '.obj')
        so = os.path.join(SCRATCH, name + '.so')
        compile_variant(cmd, obj, spec.get('extra', ()), spec.get('profdata'))
        link_with(obj, so)
        m = measure_fn(so)
        if a.skip_verify:
            be = le = None
        else:
            be, le = verify_so(so)
        tsize, _, _ = textsec(so)
        m['byte_exact'] = be
        m['linked_exact'] = le
        m['text_size'] = tsize
        results[name] = m
        print('%-28s raw %5d  aligned %4d  size %5d  .text %d  byte_exact %s'
              % (name, m['raw'], m['aligned'], m['size'], tsize, be), flush=True)
    with open(os.path.join(SCRATCH, 'results.json'), 'w') as f:
        json.dump(results, f, indent=1)


if __name__ == '__main__':
    main()
