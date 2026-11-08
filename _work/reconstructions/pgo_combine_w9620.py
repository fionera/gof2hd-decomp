#!/usr/bin/env python3
"""Combine the w9515 rank-derived profile with section-prefix off (wave 150, w9620).

w9518 found that `-mllvm -profile-guided-section-prefix` is ON by default in
clang 7.0.2 and relocates a profiled function to `.text.hot` at the front of
`.text`.  w9515 found a hand-built per-TU `.profdata` (`base=12`, `c28=28`,
latch counters 6/7/12 = 0) that reproduces BOTH original switch tables exactly
but loses the eager `shortMission`/`longMission` computation at the function
entry.  This harness replays the w9515 profile with section-prefix OFF and
searches the non-case counters for a vector that keeps the entry eager at
exactly 1432 B.

Usage:
  python3 pgo_combine_w9620.py meta
  python3 pgo_combine_w9620.py run 12 28 0 [k=v ...] [--extra FLAG ...]
  python3 pgo_combine_w9620.py sweep-base 0 80
  python3 pgo_combine_w9620.py sweep-single
  python3 pgo_combine_w9620.py verify <so>

Scratch: /dev/shm/w9620/run (override with W9620_SCRATCH).
"""
import json, os, re, subprocess, sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BUILD = os.path.join(REPO, 'cmake-build-match')
NDK = os.path.join(REPO, '.cache/ndk/android-ndk-r18b')
PROFDATA = os.path.join(NDK, 'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
OBJDUMP = os.path.join(NDK, 'toolchains/arm-linux-androideabi-4.9/prebuilt/'
                       'linux-x86_64/bin/arm-linux-androideabi-objdump')
VERIFY = os.path.join(REPO, 'tools/verify/verify.py')
ORIG = os.environ.get('GOF2_ORIGINAL_SO',
                      '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
MANGLE = '_ZN6Status16missionCompletedEbbx'
SCRATCH = os.environ.get('W9620_SCRATCH', '/dev/shm/w9620/run')
PREFIX_OFF = ['-mllvm', '-profile-guided-section-prefix=false']
os.makedirs(SCRATCH, exist_ok=True)


def run(cmd, **kw):
    return subprocess.run(cmd, shell=isinstance(cmd, str), check=True,
                          capture_output=True, text=True, **kw)


def compile_cmd():
    out = run(['ninja', '-C', BUILD, '-t', 'commands']).stdout.split('\n')
    for c in out:
        if 'clang++' in c and ' -c ' in c and c.rstrip().endswith('src/game/mission/Status.cpp'):
            return c
    raise SystemExit('compile line not found')


def link_cmd():
    out = run(['ninja', '-C', BUILD, '-t', 'commands', 'libgof2hdaa.so']).stdout.split('\n')
    for c in out:
        if 'ld.gold' in c and '-shared' in c:
            return c
    raise SystemExit('link command not found')


def sym(path, name):
    for l in run([OBJDUMP, '-T', path]).stdout.split('\n'):
        p = l.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    return None


def text_section(path):
    for l in run([OBJDUMP, '-h', path]).stdout.split('\n'):
        p = l.split()
        if len(p) > 5 and p[1] == '.text':
            return int(p[3], 16), int(p[5], 16)
    raise SystemExit('no .text')


def disasm(path, addr, size):
    out = run([OBJDUMP, '-d', '--no-show-raw-insn',
               '--start-address=%d' % addr, '--stop-address=%d' % (addr + size),
               path]).stdout
    rows = []
    for ln in out.split('\n'):
        m = re.match(r'^\s*([0-9a-f]+):\s+(\S+)\s*(.*)$', ln)
        if m:
            rows.append((int(m.group(1), 16), m.group(2), m.group(3).strip()))
    return rows


def text_bytes(path, addr, size):
    d = open(path, 'rb').read()
    vma, off = text_section(path)
    return d[off + addr - vma: off + addr - vma + size]


def tbh_orders(path, func_addr):
    d = open(path, 'rb').read()
    vma, off = text_section(path)

    def rd(v, n):
        return d[off + v - vma: off + v - vma + n]

    rows = disasm(path, func_addr, 0x600)
    out = {}
    for i, (a, mn, ops) in enumerate(rows):
        if mn not in ('tbh', 'tbb'):
            continue
        m = re.search(r'\[pc,\s*(r\d+),\s*lsl\s*#1\]', ops) if mn == 'tbh' \
            else re.search(r'\[pc,\s*(r\d+)\]', ops)
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
                mm = re.match(rf'{reg},\s*r\d+,\s*#(\d+)', ops2) or \
                     re.match(rf'{reg},\s*#(\d+)', ops2)
                if mm:
                    base = int(mm.group(1)); continue
        if N is None or base is None:
            continue
        tbl = a + 4
        if mn == 'tbh':
            ents = __import__('struct').unpack_from('<%dh' % (N + 1), rd(tbl, (N + 1) * 2))
            targets = [tbl + 2 * e for e in ents]
        else:
            ents = __import__('struct').unpack_from('<%dB' % (N + 1), rd(tbl, N + 1))
            targets = [tbl + e for e in ents]
        out[base] = [base + k for k in sorted(range(N + 1), key=lambda k: (targets[k], k))]
    return out


def real_case_order(path, func_addr):
    orders = tbh_orders(path, func_addr)
    groups = {}
    for base, vals in orders.items():
        pass
    # Re-derive groups with targets to drop the default block.
    d = open(path, 'rb').read()
    vma, off = text_section(path)

    def rd(v, n):
        return d[off + v - vma: off + v - vma + n]

    rows = disasm(path, func_addr, 0x600)
    groups = []
    for i, (a, mn, ops) in enumerate(rows):
        if mn not in ('tbh', 'tbb'):
            continue
        m = re.search(r'\[pc,\s*(r\d+),\s*lsl\s*#1\]', ops) if mn == 'tbh' \
            else re.search(r'\[pc,\s*(r\d+)\]', ops)
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
                mm = re.match(rf'{reg},\s*r\d+,\s*#(\d+)', ops2) or \
                     re.match(rf'{reg},\s*#(\d+)', ops2)
                if mm:
                    base = int(mm.group(1)); continue
        if N is None or base is None:
            continue
        tbl = a + 4
        if mn == 'tbh':
            ents = __import__('struct').unpack_from('<%dh' % (N + 1), rd(tbl, (N + 1) * 2))
            targets = [tbl + 2 * e for e in ents]
        else:
            ents = __import__('struct').unpack_from('<%dB' % (N + 1), rd(tbl, N + 1))
            targets = [tbl + e for e in ents]
        by_t = {}
        for k, t in enumerate(targets):
            by_t.setdefault(t, []).append(base + k)
        groups.extend(by_t.items())
    groups.sort(key=lambda g: g[0])
    default_t = max(groups, key=lambda g: len(g[1]))[0]
    out = []
    for t, vals in groups:
        if t == default_t:
            continue
        for v in vals:
            if v not in out:
                out.append(v)
    return out


def instrument_ir(cmd, path):
    m = re.search(r'-o (\S+)', cmd)
    c = cmd.replace(m.group(1), path).replace(' -c ', ' -fprofile-instr-generate -emit-llvm -S -c ')
    run(c, cwd=BUILD)


def parse_profile(ir):
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
    case_ctr, order = {}, []
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
    run([PROFDATA, 'merge', text, '-o', data])


def compile_use(cmd, profdata, obj, extra=()):
    m = re.search(r'-o (\S+)', cmd)
    c = cmd.replace(m.group(1), obj)
    flags = list(extra) + ['-fprofile-instr-use=%s' % profdata]
    c = c.replace(' -c ', ' ' + ' '.join(flags) + ' -c ', 1)
    run(c, cwd=BUILD)


def link_with(obj, so):
    lc = link_cmd()
    canon = os.path.join(BUILD, 'CMakeFiles/gof2.dir/./src/game/mission/Status.cpp.obj')
    lc = lc.replace(canon, obj)
    lc = lc.replace(os.path.join(BUILD, 'libgof2hdaa.unstripped.so'), so + '.un')
    lc = lc.replace(os.path.join(BUILD, 'libgof2hdaa.so'), so)
    run(lc, cwd=BUILD)


def obj_text_sections(path):
    d = open(path, 'rb').read()
    e_shoff, = __import__('struct').unpack_from('<I', d, 0x20)
    e_shentsize, e_shnum, e_shstrndx = __import__('struct').unpack_from('<HHH', d, 0x2e)
    secs = []
    for i in range(e_shnum):
        off = e_shoff + i * e_shentsize
        name, typ, flags, addr, offset, size = __import__('struct').unpack_from('<IIIIII', d, off)
        secs.append((name, typ, offset, size))
    shstr = secs[e_shstrndx]
    st = d[shstr[2]:shstr[2] + shstr[3]]
    out = {}
    for s in secs:
        end = st.find(b'\0', s[0])
        n = st[s[0]:end].decode('latin1')
        if s[1] != 8 and n:
            out[n] = d[s[2]:s[2] + s[3]]
    return out


def measure(so):
    a, b = sym(ORIG, MANGLE), sym(so, MANGLE)
    if not a or not b:
        return None
    x, y = text_bytes(ORIG, *a), text_bytes(so, *b)
    n = min(len(x), len(y))
    raw = sum(p != q for p, q in zip(x[:n], y[:n]))
    oi = [(r[0], r[1] + ' ' + r[2]) for r in disasm(ORIG, *a)]
    ui = [(r[0], r[1] + ' ' + r[2]) for r in disasm(so, *b)]
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
    import difflib
    sm = difflib.SequenceMatcher(None, ot, ut, autojunk=False)
    aligned = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes()
                  if tag != 'equal')
    mn = [m for (_, m, _) in disasm(so, *b)][:16]
    eager = ('rsbs' in mn) and ('sbcs.w' in mn or 'sbcs' in mn)
    return dict(raw=raw, aligned=aligned, size=len(y), orig_size=len(x), eager=eager)


def verify_so(so, tag):
    bd = os.path.join(SCRATCH, 'verify_' + tag)
    os.makedirs(bd, exist_ok=True)
    run([sys.executable, VERIFY, '--build-dir', bd, '--original', ORIG,
         '--ours', so, '--objdump', OBJDUMP])
    r = json.load(open(os.path.join(bd, 'report.json')))['summary']
    return r['byte_exact'], r['linked_exact']


def get_meta(cmd=None):
    cmd = cmd or compile_cmd()
    ir = os.path.join(SCRATCH, 'instr.ll')
    instrument_ir(cmd, ir)
    fhash, ncounters, case_ctr, ir_order = parse_profile(ir)
    full = real_case_order(ORIG, sym(ORIG, MANGLE)[0])
    rank = {v: len(full) - i for i, v in enumerate(full)}
    return dict(cmd=cmd, fhash=fhash, ncounters=ncounters, case_ctr=case_ctr,
                ir_order=ir_order, rank=rank)


def base_vector(base, c28, latch, meta, overrides=None):
    vals = [base] * meta['ncounters']
    seen = set()
    for v in meta['ir_order']:
        ctr = meta['case_ctr'].get(v)
        if ctr is None or ctr in seen:
            continue
        seen.add(ctr)
        vals[ctr] = meta['rank'].get(v, 1) - 1
    for k in (6, 7, 12):
        vals[k] = latch
    vals[28] = c28
    if overrides:
        for k, v in overrides.items():
            vals[k] = v
    return vals


def build(meta, vals, tag, extra=()):
    txt = os.path.join(SCRATCH, tag + '.proftext')
    dat = os.path.join(SCRATCH, tag + '.profdata')
    write_profile(meta['fhash'], meta['ncounters'], vals, txt)
    merge(txt, dat)
    obj = os.path.join(SCRATCH, tag + '.obj')
    compile_use(meta['cmd'], dat, obj, PREFIX_OFF + list(extra))
    so = os.path.join(SCRATCH, tag + '.so')
    link_with(obj, so)
    m = measure(so)
    canon = os.path.join(BUILD, 'CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj')
    a, b = obj_text_sections(canon), obj_text_sections(obj)
    others = [n for n in a if n.startswith('.text.') and MANGLE not in n]
    m['other_same'] = sum(1 for n in others if n in b and b[n] == a[n])
    m['other_total'] = len(others)
    m['so'] = so
    return m


def main():
    if len(sys.argv) < 2:
        print(__doc__); return
    cmd = sys.argv[1]
    meta = get_meta()
    if cmd == 'meta':
        print('fhash %d ncounters %d' % (meta['fhash'], meta['ncounters']))
        print('ir_order', meta['ir_order'])
        print('rank', meta['rank'])
        json.dump({k: v for k, v in meta.items() if k != 'cmd'},
                  open(os.path.join(SCRATCH, 'meta.json'), 'w'))
    elif cmd == 'run':
        base, c28, latch = int(sys.argv[2]), int(sys.argv[3]), int(sys.argv[4])
        over, extra = {}, []
        i = 5
        while i < len(sys.argv):
            if sys.argv[i] == '--extra':
                extra = sys.argv[i + 1:]; break
            k, v = sys.argv[i].split('='); over[int(k)] = int(v); i += 1
        vals = base_vector(base, c28, latch, meta, over)
        tag = 'run_%d_%d_%d' % (base, c28, latch)
        m = build(meta, vals, tag, extra)
        print(tag, 'raw', m['raw'], 'aligned', m['aligned'], 'size', m['size'],
              'eager', m['eager'], 'other', '%d/%d' % (m['other_same'], m['other_total']))
    elif cmd == 'sweep-base':
        lo, hi = int(sys.argv[2]), int(sys.argv[3])
        for base in range(lo, hi + 1):
            vals = base_vector(base, 28, 0, meta)
            m = build(meta, vals, 'sb_%d' % base)
            print('%d raw=%d aligned=%d size=%d eager=%s other=%d/%d'
                  % (base, m['raw'], m['aligned'], m['size'], m['eager'],
                     m['other_same'], m['other_total']), flush=True)
    elif cmd == 'sweep-single':
        case = set(meta['case_ctr'].values())
        for k in range(meta['ncounters']):
            if k in case:
                continue
            for v in (0, 64):
                vals = base_vector(12, 28, 0, meta, {k: v})
                m = build(meta, vals, 'ks_%d_%d' % (k, v))
                print('k=%d v=%d raw=%d aligned=%d size=%d eager=%s'
                      % (k, v, m['raw'], m['aligned'], m['size'], m['eager']), flush=True)
    elif cmd == 'verify':
        print(verify_so(sys.argv[2], os.path.basename(sys.argv[2])))


if __name__ == '__main__':
    main()
