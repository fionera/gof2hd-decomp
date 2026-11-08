#!/usr/bin/env python3
"""Raw same-address byte diff + instruction-index alignment diff for one or more symbols.

Usage:
  python3 tools/fn_diff.py --ours <libgof2hdaa.so> <mangled> [<mangled> ...]

Metric 1 ("raw")     : bytes that differ at the same offset in the two function bodies
                       (plus the size delta).  Same metric as /dev/shm/fdiff.py.
Metric 2 ("aligned") : non-matching instructions after normalising branch targets to the
                       target instruction index and aligning the two streams with difflib
                       (autojunk off).  Same metric as _work/reconstructions/align_diff.py.

The original is the checked-in ground truth; --ours is required because the sweep builds
live in scratch directories.
"""
import subprocess, sys, re, collections, difflib, argparse, json, os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
_orig_candidates = [
    os.path.normpath(os.path.join(ROOT, '..', '_work', 'bins',
                                  'android_2.0.16_libgof2hdaa.so')),
    '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so',
]
DEFAULT_ORIG = next((p for p in _orig_candidates if os.path.exists(p)), _orig_candidates[0])
DEFAULT_OBJDUMP = os.path.join(ROOT, '.cache', 'ndk', 'android-ndk-r18b', 'toolchains',
                               'arm-linux-androideabi-4.9', 'prebuilt', 'linux-x86_64',
                               'bin', 'arm-linux-androideabi-objdump')

def sym(obj, path, name):
    out = subprocess.check_output([obj, '-T', path], text=True)
    for l in out.splitlines():
        p = l.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    return None

def textsec(path):
    out = subprocess.check_output([OBJ, '-h', path], text=True)
    for l in out.splitlines():
        p = l.split()
        if len(p) > 4 and p[1] == '.text':
            return int(p[3], 16), int(p[4], 16), int(p[5], 16)
    raise SystemExit('no .text')

def data(path, addr, size):
    f = open(path, 'rb').read()
    _, vma, off = textsec(path)
    return f[off + (addr - vma):off + (addr - vma) + size]

def disasm(path, addr, size):
    out = subprocess.check_output([OBJ, '-d', '--start-address=%d' % addr,
                                   '--stop-address=%d' % (addr + size), path], text=True)
    ins = []
    for l in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$', l)
        if not m:
            continue
        a = int(m.group(1), 16); t = m.group(2).strip()
        if t:
            ins.append((a, t))
    return ins

def opcode(t):
    return t.split()[0] if t else ''

def classify(o, u):
    if o is None: return 'extra-ours:' + opcode(u)
    if u is None: return 'extra-orig:' + opcode(o)
    om, um = opcode(o), opcode(u)
    ob = om.replace('.w', '').replace('.n', ''); ub = um.replace('.w', '').replace('.n', '')
    if om == um:
        oo = o[len(om):].strip(); uo = u[len(um):].strip()
        if re.sub(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b', 'R', oo) == \
           re.sub(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b', 'R', uo):
            return 'regalloc:' + om
        if re.sub(r'#-?0x[0-9a-f]+|#-?\d+', '#I', oo) == re.sub(r'#-?0x[0-9a-f]+|#-?\d+', '#I', uo):
            return 'imm:' + om
        if re.sub(r'\[[^\]]*\]', '[M]', oo) == re.sub(r'\[[^\]]*\]', '[M]', uo):
            return 'memaddr:' + om
        return 'operand:' + om
    if ob == ub: return 'width:' + om + '/' + um
    fam = {'ldr': 'load', 'ldrb': 'load', 'ldrh': 'load', 'str': 'store', 'strb': 'store',
           'strh': 'store', 'mov': 'mov', 'movs': 'mov', 'movw': 'mov', 'movt': 'mov',
           'mvn': 'mov', 'mvns': 'mov', 'add': 'add', 'adds': 'add', 'sub': 'sub',
           'subs': 'sub', 'cmp': 'cmp', 'cmn': 'cmp', 'tst': 'cmp', 'teq': 'cmp'}
    if fam.get(ob) == fam.get(ub) and fam.get(ob):
        return 'family:' + om + '/' + um
    return 'opcode:' + om + '/' + um

def score_one(name):
    a = sym(OBJ, ORIG, name); b = sym(OBJ, OURS, name)
    if not a or not b:
        return {'name': name, 'missing': True}
    x = data(ORIG, a[0], a[1]); y = data(OURS, b[0], b[1])
    n = min(len(x), len(y))
    raw = sum(p != q for p, q in zip(x[:n], y[:n])) + abs(len(x) - len(y))
    oi = disasm(ORIG, *a); ui = disasm(OURS, *b)
    oidx = {ad: i for i, (ad, t) in enumerate(oi)}
    uidx = {ad: i for i, (ad, t) in enumerate(ui)}
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
    cats = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            continue
        if tag == 'replace':
            for k in range(max(i2 - i1, j2 - j1)):
                cats.append(classify(ot[i1 + k] if i1 + k < i2 else None,
                                     ut[j1 + k] if j1 + k < j2 else None))
        elif tag == 'delete':
            for i in range(i1, i2):
                cats.append('extra-orig:' + opcode(ot[i]))
        else:
            for j in range(j1, j2):
                cats.append('extra-ours:' + opcode(ut[j]))
    c = collections.Counter(cats)
    return {'name': name, 'raw': raw, 'aligned': sum(c.values()),
            'osize': a[1], 'usize': b[1], 'oinsn': len(oi), 'uinsn': len(ui),
            'cats': dict(c.most_common(8))}

if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('--ours', required=True)
    ap.add_argument('--orig', default=DEFAULT_ORIG)
    ap.add_argument('--objdump', default=DEFAULT_OBJDUMP)
    ap.add_argument('--json', action='store_true')
    ap.add_argument('names', nargs='+')
    a = ap.parse_args()
    OBJ, ORIG, OURS = a.objdump, a.orig, a.ours
    res = [score_one(n) for n in a.names]
    if a.json:
        print(json.dumps(res, indent=1))
    else:
        for r in res:
            if r.get('missing'):
                print('MISSING', r['name']); continue
            print('%-70s raw=%6d aligned=%5d size=%d/%d insn=%d/%d' %
                  (r['name'], r['raw'], r['aligned'], r['usize'], r['osize'],
                   r['uinsn'], r['oinsn']))
