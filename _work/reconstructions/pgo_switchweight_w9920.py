#!/usr/bin/env python3
"""Switch-weight replay for Status::missionCompleted (wave 152, w9920).

The original's first jump-table bodies are in *source* order, while the
unprofiled build lays them out in ascending case-value order.  Injecting an
explicit `!prof` weight vector on the switch reproduces the original body
order at exactly 1432 B; the best vector is flat, not geometric:

    default 1, {167,174,8,0,11,13,14} 4,
    {150,151,152,153,154,155,158,162,166,170,184,189} 3,
    {156,157,160,163,164,165,168,169,171,172} 2

=> raw 558, aligned 47, size 1432 (baseline 722 / 320 / 1432).

The script also measures the source-level producers tried this wave:
`switch (__builtin_expect(t, K))` (one-hot) and a four-level nested switch
chain (geometric weights).  Neither reaches 1432 B with the flat layout.

Usage (from the worktree root, after `cmake --preset default ...`):

  python3 _work/reconstructions/pgo_switchweight_w9920.py groups 4 4 4 3 2
  python3 _work/reconstructions/pgo_switchweight_w9920.py onehot 167
  python3 _work/reconstructions/pgo_switchweight_w9920.py nest
  python3 _work/reconstructions/pgo_switchweight_w9920.py score <30 ints>

`groups` takes (a,b,c,d,e): a=167/174, b=8, c=0/11/13/14, d=group2,
e=group3; default is 1.  `score` takes the raw 30-entry vector in the
switch's case order (default, 167,174,8,0,11,13,14,150..189,156..172).

Scratch is /dev/shm/w9920 (override with W9920_SCRATCH).
"""
import json, os, re, struct, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BUILD = os.path.join(ROOT, 'cmake-build-match')
OBJDUMP = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/'
                       'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
                       'arm-linux-androideabi-objdump')
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = os.path.join(BUILD, 'libgof2hdaa.so')
MANGLE = '_ZN6Status16missionCompletedEbbx'
SCRATCH = os.environ.get('W9920_SCRATCH', '/dev/shm/w9920')

# switch case order in the optimized IR (source order)
CASE_ORDER = [167, 174, 8, 0, 11, 13, 14,
              150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184, 189,
              156, 157, 160, 163, 164, 165, 168, 169, 171, 172]
GROUPS = {
    'a': [167, 174],
    'b': [8],
    'c': [0, 11, 13, 14],
    'd': [150, 151, 152, 153, 154, 155, 158, 162, 166, 170, 184, 189],
    'e': [156, 157, 160, 163, 164, 165, 168, 169, 171, 172],
}


def compile_command():
    with open(os.path.join(BUILD, 'compile_commands.json')) as f:
        for e in json.load(f):
            if e['file'].endswith('src/game/mission/Status.cpp'):
                return e['command'].replace(
                    '-o CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj -c', '')
    raise SystemExit('Status.cpp compile command not found')


def link_command():
    with open(os.path.join(BUILD, 'build.ninja')) as f:
        text = f.read()
    m = re.search(r'\n  COMMAND = (cd .*?libgof2hdaa\.so[^\n]*)\n', text)
    if not m:
        raise SystemExit('link command not found')
    return m.group(1).split(' && ')


def emit_ll(cc, path, extra=()):
    cmd = cc.split() + ['-S', '-emit-llvm', '-o', path] + list(extra)
    subprocess.run(cmd, check=True, cwd=ROOT,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def inject_weights(ll_in, ll_out, weights):
    s = open(ll_in).read()
    m = re.search(r'define[^\n]*' + re.escape(MANGLE) + r'[^\n]*\n', s)
    j = s.index('switch i32 %52', m.start())
    k = s.index('], !dbg !9298', j)
    ids = [int(x) for x in re.findall(r'^!(\d+) =', s, re.M)]
    mid = max(ids) + 1
    s = s[:k] + '], !dbg !9298, !prof !%d' % mid + s[k + len('], !dbg !9298'):]
    s = s.rstrip() + '\n!%d = !{!"branch_weights", %s}\n' % (
        mid, ', '.join('i32 %d' % w for w in weights))
    open(ll_out, 'w').write(s)


def build(ll, cc, extra=()):
    obj = os.path.join(BUILD, 'CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj')
    cmd = [x for x in cc.split() if not x.endswith('src/game/mission/Status.cpp')]
    cmd += ['-c', ll, '-o', obj] + list(extra)
    subprocess.run(cmd, check=True, cwd=ROOT,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    for part in link_command():
        subprocess.run(part, shell=True, check=True, cwd=ROOT,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def sym(path, name):
    out = subprocess.check_output([OBJDUMP, '-T', path], text=True)
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    raise SystemExit('symbol %s not found in %s' % (name, path))


def disasm(path, addr, size):
    out = subprocess.check_output(
        [OBJDUMP, '-d', '--start-address=%d' % addr,
         '--stop-address=%d' % (addr + size), path], text=True)
    ins = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$', line)
        if m:
            ins.append((int(m.group(1), 16), m.group(2).strip()))
    return ins


def raw_diff():
    import difflib  # noqa: F401  (kept for parity with align_diff)
    a = sym(ORIG, MANGLE)
    b = sym(OURS, MANGLE)
    x = open(ORIG, 'rb').read()
    y = open(OURS, 'rb').read()

    def textsec(path):
        out = subprocess.check_output([OBJDUMP, '-h', path], text=True)
        for line in out.splitlines():
            p = line.split()
            if p[1:2] == ['.text']:
                return int(p[3], 16), int(p[4], 16), int(p[5], 16)
        raise SystemExit('no .text')

    def data(path, addr, size):
        sz, vma, off = textsec(path)
        return open(path, 'rb').read()[off + (addr - vma):off + (addr - vma) + size]
    xa = data(ORIG, a[0], a[1])
    ya = data(OURS, b[0], b[1])
    n = min(len(xa), len(ya))
    return sum(p != q for p, q in zip(xa[:n], ya[:n])), len(xa), len(ya)


def aligned_diff():
    """Same normalisation as _work/reconstructions/align_diff.py."""
    import difflib
    a = sym(ORIG, MANGLE)
    b = sym(OURS, MANGLE)
    oi = disasm(ORIG, *a)
    ui = disasm(OURS, *b)
    oidx = {ad: i for i, (ad, _) in enumerate(oi)}
    uidx = {ad: i for i, (ad, _) in enumerate(ui)}

    def norm(t, idx):
        def rep(m):
            ad = int(m.group(1), 16)
            return 'I%d' % idx[ad] if ad in idx else 'X'
        t = re.sub(r'\b([0-9a-f]{4,8})\b', rep, t)
        t = re.sub(r'<[^>]*>', '', t)
        return re.sub(r'\s+', ' ', t).strip()
    ot = [norm(t, oidx) for _, t in oi]
    ut = [norm(t, uidx) for _, t in ui]
    sm = difflib.SequenceMatcher(None, ot, ut, autojunk=False)
    n = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'replace':
            n += max(i2 - i1, j2 - j1)
        elif tag == 'delete':
            n += i2 - i1
        elif tag == 'insert':
            n += j2 - j1
    return n


def score(weights, extra=()):
    os.makedirs(SCRATCH, exist_ok=True)
    cc = compile_command()
    ll0 = os.path.join(SCRATCH, 'status.ll')
    emit_ll(cc, ll0, extra)
    ll1 = os.path.join(SCRATCH, 'status_inj.ll')
    inject_weights(ll0, ll1, weights)
    build(ll1, cc, extra)
    raw, osz, msz = raw_diff()
    print('raw %d  aligned %d  size %d (orig %d)' % (raw, aligned_diff(), msz, osz))


def groups_vector(a, b, c, d, e):
    w = [1]
    for k in CASE_ORDER:
        if k in GROUPS['a']:
            w.append(a)
        elif k in GROUPS['b']:
            w.append(b)
        elif k in GROUPS['c']:
            w.append(c)
        elif k in GROUPS['d']:
            w.append(d)
        elif k in GROUPS['e']:
            w.append(e)
        else:
            raise SystemExit('unmapped case %d' % k)
    return w


def main():
    if len(sys.argv) < 2:
        raise SystemExit(__doc__)
    mode = sys.argv[1]
    extra = []
    if '--extra' in sys.argv:
        i = sys.argv.index('--extra')
        extra = sys.argv[i + 1:]
        sys.argv = sys.argv[:i]
    if mode == 'groups':
        score(groups_vector(*[int(x) for x in sys.argv[2:7]]), extra)
    elif mode == 'onehot':
        k = int(sys.argv[2])
        score([1] + [2000 if x == k else 1 for x in CASE_ORDER], extra)
    elif mode == 'nest':
        # Four-level chain with the default 2000:1 expect weights.
        vals = {167: 2000 ** 4, 174: 2000 ** 3, 8: 2000 ** 3,
                0: 2000 ** 2, 11: 2000 ** 2, 13: 2000 ** 2, 14: 2000 ** 2}
        for k in GROUPS['d']:
            vals[k] = 2000
        for k in GROUPS['e']:
            vals[k] = 1
        score([1] + [vals[k] for k in CASE_ORDER], extra)
    elif mode == 'score':
        score([int(x) for x in sys.argv[2:]], extra)
    else:
        raise SystemExit(__doc__)


if __name__ == '__main__':
    main()
