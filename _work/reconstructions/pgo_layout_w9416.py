#!/usr/bin/env python3
"""Derive per-block frequencies from a function's (original) block layout and
replay them as an LLVM `-fprofile-instr-use` profile.

Motivation
----------
The original `libgof2hdaa.so` was built with branch weights (PGO-like).  For a
body-byte-exact function such as `Radar::Radar` the *layout* is therefore a
witness for the original's block frequencies.  This tool:

  1. compiles one TU with `-fprofile-instr-generate -emit-llvm` and maps every
     frontend counter index to the IR basic block that increments it;
  2. runs `llc -debug-only=block-freq` on the *canonical* (unprofiled) IR and
     reads the block-frequency vector in RPO order (RPO is identical for the
     instrumented and canonical IR: instrumentation adds no CFG edges);
  3. writes a text profile whose counter K is the frequency of counter K's
     block, merges it with `llvm-profdata`, and recompiles the TU with
     `-fprofile-instr-use`;
  4. optionally gold-links and reports the `.ARM.extab` record diff and the
     raw same-address `.text` diff of the target function.

The RPO/block-frequency parser is deliberately order-based rather than
name-based: LLVM's debug block names embed instruction counts that differ
between the instrumented and canonical IR, but the RPO order is stable.

Usage
-----
  python3 pgo_layout_w9416.py --build-dir cmake-build-match \
      --src src/game/weapons/Radar.cpp --func _ZN5RadarC2EP5Level \
      --addr 0x12d484 --dump
  python3 pgo_layout_w9416.py ... --profile 8=1 --measure
  python3 pgo_layout_w9416.py ... --profile-vector          # block-freq vector

Requires a configured cmake-build-match (ninja -t commands is the source of
the compile line) and the NDK r18b `llvm-profdata`.
"""
import argparse, collections, os, re, struct, subprocess, sys

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
NDK = os.path.join(REPO, '.cache/ndk/android-ndk-r18b')
PROFDATA = os.path.join(NDK, 'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
# The stock NDK clang has no -debug-only; the coordinator's instrumented LLVM 7
# build is used only as a *debug oracle* (never as a compiler for the match).
LLC = '/tmp/llvm_android_702b/build2/bin/llc'
SCRATCH = '/dev/shm/w9416'


def compile_line(build, src):
    out = subprocess.run(['ninja', '-C', build, '-t', 'commands'],
                         capture_output=True, text=True).stdout.split('\n')
    for c in out:
        if 'clang++' in c and ' -c ' in c and c.rstrip().endswith(src):
            return c
    raise SystemExit('compile line for %s not found' % src)


def split_cmd(c):
    """Strip -o/-c and return the prefix that can be extended with new flags."""
    c = re.sub(r' -MD -MT \S+ -MF \S+', '', c)
    c = re.sub(r' -o \S+', '', c)
    c = re.sub(r' -c \S+$', '', c)
    return c


def parse_blocks(text, func):
    lines = text.split('\n')
    start = None
    for i, l in enumerate(lines):
        if l.startswith('define') and '@' + func in l:
            start = i
            break
    if start is None:
        raise SystemExit('function %s not found in IR' % func)
    end = start
    while not lines[end].startswith('}'):
        end += 1
    body = lines[start:end + 1]
    blocks, cur = [], None
    for l in body:
        m = re.match(r'^; <label>:(\d+):', l)
        if m:
            cur = [m.group(1), []]
            blocks.append(cur)
            continue
        m = re.match(r'^([\w.$]+):', l)
        if m:
            cur = [m.group(1), []]
            blocks.append(cur)
            continue
        if cur is not None:
            cur[1].append(l)
    entry = []
    for l in body:
        if re.match(r'^; <label>:', l):
            break
        entry.append(l)
    blocks.insert(0, ['ENTRY', entry])
    return blocks


def is_instr(l):
    s = l.strip()
    return bool(s) and not s.startswith(';')


def opcode(line):
    s = line.strip()
    if not s or s.startswith(';'):
        return None
    m = re.match(r'^%[\w.$]+\s*=\s*(\w+)', s)
    if m:
        return m.group(1)
    m = re.match(r'^(\w+)', s)
    return m.group(1) if m else None


def rpo_order(blocks):
    ins_by = {b[0]: [x for x in b[1] if is_instr(x)] for b in blocks}
    succ = collections.defaultdict(list)
    for lab, ins in blocks:
        for l in ins:
            for m in re.finditer(r'label %([\w.$]+)', l):
                if m.group(1) not in succ[lab]:
                    succ[lab].append(m.group(1))
    seen, order = set(), []

    def dfs(n):
        seen.add(n)
        for s in succ.get(n, []):
            if s not in seen:
                dfs(s)
        order.append(n)

    dfs('ENTRY')
    return list(reversed(order)), ins_by, succ


def block_freqs(ir, func):
    """Return {block label: integer frequency} and the RPO list."""
    blocks = parse_blocks(open(ir).read(), func)
    rpo, ins_by, succ = rpo_order(blocks)
    out = subprocess.run([LLC, '-mtriple=armv7-none-linux-androideabi16',
                          '-mattr=+neon', '-float-abi=soft', '-Oz',
                          '-debug-only=block-freq', ir, '-o', '/dev/null'],
                         capture_output=True, text=True).stderr
    lines = out.split('\n')
    sec, on = [], False
    for l in lines:
        if l.startswith('block-frequency: '):
            if on:
                break
            on = (l.strip() == 'block-frequency: ' + func)
            continue
        if on:
            sec.append(l)
    freq = []
    for l in sec:
        m = re.match(r' - \S+: float = [\d.e+-]+, scaled = [\d.e+-]+, int = (\d+)', l)
        if m:
            freq.append(int(m.group(1)))
    if len(freq) < len(rpo):
        raise SystemExit('block-freq dump shorter than RPO (%d < %d)' % (len(freq), len(rpo)))
    return {lab: freq[i] for i, lab in enumerate(rpo)}, rpo, blocks


def counter_blocks(ir, func):
    blocks = parse_blocks(open(ir).read(), func)
    out = {}
    for lab, ins in blocks:
        for l in ins:
            m = re.search(r'@__profc_%s, i32 0, i32 (\d+)\)' % func, l)
            if m and 'load' in l:
                out[int(m.group(1))] = lab
                break
    return out


def instrumented_ir(cmd, src, out):
    c = cmd + ' -fprofile-instr-generate -emit-llvm -S -o %s -c %s' % (out, src)
    subprocess.run(c, shell=True, check=True, capture_output=True)
    return out


def canonical_ir(cmd, src, out):
    c = cmd + ' -emit-llvm -S -o %s -c %s' % (out, src)
    subprocess.run(c, shell=True, check=True, capture_output=True)
    return out


def parse_prf(path):
    """Return [(name, hash, num_counters)] from an instrumented object."""
    sys.path.insert(0, os.path.join(REPO, '_work/reconstructions'))
    import pgo_flat_w9319 as P
    return P.parse_obj(path)


def write_profile(recs, counters, path):
    with open(path, 'w') as f:
        for name, h, nc in recs:
            f.write('%s\n# Func Hash:\n%d\n# Num Counters:\n%d\n# Counter Values:\n'
                    % (name, h, nc))
            vals = counters.get(name, [1] * nc)
            for c in vals:
                f.write('%d\n' % c)
    pd = path + 'data'
    subprocess.run([PROFDATA, 'merge', path, '-o', pd], check=True, capture_output=True)
    return pd


def extab_record(data, func):
    def sections(d):
        shoff, = struct.unpack_from('<I', d, 0x20)
        shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
        hdrs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize) for i in range(shnum)]
        stroff = hdrs[shstrndx][4]
        out = {}
        for h in hdrs:
            name, typ, flags, addr, off, size, link, info, align, entsize = h
            end = d.index(b'\0', stroff + name)
            out[d[stroff + name:end].decode()] = dict(addr=addr, off=off, size=size)
        return out

    def prel31(d, off, place):
        w, = struct.unpack_from('<I', d, off)
        v = w & 0x7fffffff
        if v & 0x40000000:
            v -= 0x80000000
        return place + v

    s = sections(data)
    ex, et = s['.ARM.exidx'], s['.ARM.extab']
    es = []
    for i in range(ex['size'] // 8):
        e = ex['off'] + i * 8
        ea = ex['addr'] + i * 8
        fn = prel31(data, e, ea)
        w1, = struct.unpack_from('<I', data, e + 4)
        rec = None if (w1 == 1 or (w1 & 0x80000000)) else prel31(data, e + 4, ea + 4)
        es.append((fn, rec))
    ts = sorted(set(r for _, r in es if r is not None))
    end0 = et['addr'] + et['size']
    lens = {t: (ts[i + 1] if i + 1 < len(ts) else end0) - t for i, t in enumerate(ts)}
    for fn, rec in es:
        if fn == func and rec is not None:
            off = et['off'] + (rec - et['addr'])
            return data[off:off + lens[rec]]
    return b''


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build-dir', default=os.path.join(REPO, 'cmake-build-match'))
    ap.add_argument('--src', required=True)
    ap.add_argument('--func', required=True)
    ap.add_argument('--addr', help='function address in the original (hex)')
    ap.add_argument('--orig', default='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
    ap.add_argument('--dump', action='store_true')
    ap.add_argument('--profile', nargs='*', default=None, help='K=V overrides (base=1)')
    ap.add_argument('--profile-vector', action='store_true')
    ap.add_argument('--measure', action='store_true')
    a = ap.parse_args()
    os.makedirs(SCRATCH, exist_ok=True)
    cmd = split_cmd(compile_line(a.build_dir, a.src))
    ins_ir = instrumented_ir(cmd, a.src, os.path.join(SCRATCH, 'instr.ll'))
    ins_obj = os.path.join(SCRATCH, 'instr.o')
    subprocess.run(cmd + ' -fprofile-instr-generate -o %s -c %s' % (ins_obj, a.src),
                   shell=True, check=True, capture_output=True)
    recs = [r for r in parse_prf(ins_obj) if r[0] == a.func]
    if not recs:
        raise SystemExit('no profile record for %s' % a.func)
    name, h, nc = recs[0]
    cb = counter_blocks(ins_ir, a.func)
    can_ir = canonical_ir(cmd, a.src, os.path.join(SCRATCH, 'canon.ll'))
    freqs, rpo, blocks = block_freqs(can_ir, a.func)
    # The instrumented and canonical IR have different SSA/block *labels* but
    # the same CFG, hence the same RPO order.  Map counter -> RPO index in the
    # instrumented IR, then index the canonical frequency vector by position.
    rpo_ins, _, _ = rpo_order(parse_blocks(open(ins_ir).read(), a.func))
    idx_ins = {lab: i for i, lab in enumerate(rpo_ins)}
    freq_by_idx = [freqs[lab] for lab in rpo]
    vec = [freq_by_idx[idx_ins[cb[k]]] if k in cb and cb[k] in idx_ins else 1
           for k in range(nc)]
    if a.dump:
        print('counter -> block -> frequency')
        for k in range(nc):
            print('%2d  %-8s %d' % (k, cb.get(k, '?'), vec[k]))
        print('VECTOR', vec)
    counters = {}
    if a.profile_vector:
        counters[name] = vec
    elif a.profile is not None:
        vals = [1] * nc
        for kv in a.profile:
            k, v = kv.split('=')
            vals[int(k)] = int(v)
        counters[name] = vals
    if counters:
        pd = write_profile(recs, counters, os.path.join(SCRATCH, 'prof.proftext'))
        print('profile', pd)
        if a.measure and a.addr:
            obj = os.path.join(a.build_dir, 'CMakeFiles/gof2.dir', a.src + '.obj')
            subprocess.run(cmd + ' -fprofile-instr-use=%s -o %s -c %s' % (pd, obj, a.src),
                           shell=True, check=True, capture_output=True)
            # relink with the checked-in gold command
            link = [l for l in open(os.path.join(a.build_dir, 'build.ninja'))
                    if 'ld.gold' in l and 'COMMAND' in l]
            if link:
                subprocess.run(link[0].split('COMMAND = ', 1)[1], shell=True, check=True,
                               capture_output=True)
            func = int(a.addr, 16)
            ro = extab_record(open(a.orig, 'rb').read(), func)
            rm = extab_record(open(os.path.join(a.build_dir, 'libgof2hdaa.so'), 'rb').read(), func)
            print('extab orig %d ours %d diff %d' % (len(ro), len(rm),
                  sum(x != y for x, y in zip(ro, rm)) + abs(len(ro) - len(rm))))


if __name__ == '__main__':
    main()
