#!/usr/bin/env python3
"""Reproduce the wave-147 PGO/!prof experiments (w9319).

--status-only : instrument Status.cpp, build a flat profile for
                _ZN6Status16missionCompletedEbbx, recompile with
                -fprofile-instr-use and print the TBH block order.
--all         : instrument all 188 app TUs, build a flat profile for every
                function, recompile every TU with -fprofile-instr-use and
                report how many objects are byte-identical to the canonical
                build (and, optionally, link+verify).

Needs a configured cmake-build-match (ninja -t commands is the source of the
compile lines) and the NDK r18b llvm-profdata.
"""
import argparse, concurrent.futures, hashlib, os, re, struct, subprocess, sys, zlib

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
BUILD = os.path.join(REPO, 'cmake-build-match')
NDK = os.path.join(REPO, '.cache/ndk/android-ndk-r18b')
PROFDATA = os.path.join(NDK, 'toolchains/llvm/prebuilt/linux-x86_64/bin/llvm-profdata')
SCRATCH = '/dev/shm/w9319'

def compile_cmds():
    out = subprocess.run(['ninja', '-C', BUILD, '-t', 'commands'],
                         capture_output=True, text=True).stdout.split('\n')
    return [c for c in out if 'clang++' in c and ' -c ' in c and c.rstrip().endswith('.cpp')]

def sections(path):
    out = subprocess.run(['readelf', '-SW', path], capture_output=True, text=True).stdout
    secs = {}
    for ln in out.split('\n'):
        m = re.match(r'\s*\[\s*\d+\]\s+(\S+)\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+([0-9a-f]+)', ln)
        if m:
            secs.setdefault(m.group(1), []).append((int(m.group(4), 16), int(m.group(5), 16)))
    return secs

def uleb(b, i):
    v = s = 0
    while True:
        x = b[i]; i += 1
        v |= (x & 0x7f) << s
        if not x & 0x80: return v, i
        s += 7

def parse_obj(path):
    """Return [(name, 64-bit FuncHash, NumCounters)] from __llvm_prf_*."""
    secs = sections(path)
    if '__llvm_prf_names' not in secs or '__llvm_prf_data' not in secs:
        return []
    data = open(path, 'rb').read()
    noff, nsize = secs['__llvm_prf_names'][0]
    blob = data[noff:noff + nsize]
    total, i = uleb(blob, 0)
    plen, i = uleb(blob, i)
    payload = blob[i:i + plen]
    try: payload = zlib.decompress(payload)
    except Exception: pass
    names = payload.split(b'\x01')
    byref = {struct.unpack('<Q', hashlib.md5(n).digest()[:8])[0]: n.decode('latin1')
             for n in names}
    doff, dsize = secs['__llvm_prf_data'][0]
    recs = []
    for off in range(doff, doff + dsize, 40):
        # __llvm_profile_data (LLVM 7, 40-byte record):
        #   +0x00 NameRef (MD5(name)[:8]), +0x08 FuncHash lo32, +0x0c hi32,
        #   +0x10 Counters ptr, +0x14 Name ptr, +0x18 Values ptr,
        #   +0x1c NumCounters, +0x20/+0x24 NumValueSites.
        nr, lo, hi, nc = struct.unpack_from('<QIII', data, off + 0)
        nr = struct.unpack_from('<Q', data, off)[0]
        lo, hi = struct.unpack_from('<II', data, off + 8)
        nc = struct.unpack_from('<I', data, off + 0x1c)[0]
        if nr in byref:
            recs.append((byref[nr], (hi << 32) | lo, nc))
    return recs

def write_profile(recs, path, counters=None):
    with open(path, 'w') as f:
        for name, h, nc in recs:
            f.write('%s\n# Func Hash:\n%d\n# Num Counters:\n%d\n# Counter Values:\n'
                    % (name, h, nc))
            for k in range(nc):
                f.write('%d\n' % (counters(name, k) if counters else 1))

def instrument(cmds, outdir, extra=''):
    os.makedirs(outdir, exist_ok=True)
    def run(ic):
        i, c = ic
        m = re.search(r'-o (\S+)', c)
        src = re.search(r'-c (\S+)$', c).group(1)
        base = os.path.basename(src).replace('.cpp', '') + '_%d.o' % i
        out = os.path.join(outdir, base)
        c2 = c.replace(m.group(1), out)
        c2 = c2.replace(' -c ', ' ' + extra + ' -c ')
        r = subprocess.run(c2, shell=True, capture_output=True, text=True)
        return (src, m.group(1), out, r.returncode)
    with concurrent.futures.ThreadPoolExecutor(max_workers=6) as ex:
        return list(ex.map(run, enumerate(cmds)))

def status_only():
    cmds = compile_cmds()
    c = [x for x in cmds if 'mission/Status.cpp' in x][0]
    m = re.search(r'-o (\S+)', c)
    canon = m.group(1)
    if not canon.startswith('/'): canon = os.path.join(BUILD, canon)
    src = re.search(r'-c (\S+)$', c).group(1)
    instr = os.path.join(SCRATCH, 'status_instr.o')
    c2 = c.replace(m.group(1), instr).replace(' -c ', ' -fprofile-instr-generate -c ')
    subprocess.run(c2, shell=True, check=True, capture_output=True)
    recs = [r for r in parse_obj(instr) if 'missionCompleted' in r[0]]
    print('record:', recs)
    write_profile(recs, os.path.join(SCRATCH, 'flat.proftext'))
    subprocess.run([PROFDATA, 'merge', os.path.join(SCRATCH, 'flat.proftext'),
                    '-o', os.path.join(SCRATCH, 'flat.profdata')], check=True)
    out = os.path.join(SCRATCH, 'status_flat.o')
    c3 = c.replace(m.group(1), out).replace(
        ' -c ', ' -fprofile-instr-use=' + os.path.join(SCRATCH, 'flat.profdata') + ' -c ')
    subprocess.run(c3, shell=True, check=True, capture_output=True)
    import hashlib as H
    same = H.sha256(open(canon, 'rb').read()).hexdigest() == H.sha256(open(out, 'rb').read()).hexdigest()
    print('status flat object identical to canonical:', same)

def all_tus():
    cmds = compile_cmds()
    res = instrument(cmds, os.path.join(SCRATCH, 'instr'), '-fprofile-instr-generate')
    recs = []
    for src, canon, out, rc in res:
        if rc == 0: recs.extend(parse_obj(out))
    seen = {}
    for name, h, nc in recs:
        if nc and h: seen[name] = (h, nc)
    recs = [(n, h, nc) for n, (h, nc) in seen.items()]
    print('functions in flat profile:', len(recs))
    write_profile(recs, os.path.join(SCRATCH, 'flat_all.proftext'))
    subprocess.run([PROFDATA, 'merge', os.path.join(SCRATCH, 'flat_all.proftext'),
                    '-o', os.path.join(SCRATCH, 'flat_all.profdata')], check=True)
    res2 = instrument(cmds, os.path.join(SCRATCH, 'use'),
                      '-fprofile-instr-use=' + os.path.join(SCRATCH, 'flat_all.profdata'))
    same = 0
    for src, canon, out, rc in res2:
        if not canon.startswith('/'): canon = os.path.join(BUILD, canon)
        if rc == 0 and os.path.exists(canon):
            if hashlib.sha256(open(canon, 'rb').read()).hexdigest() == \
               hashlib.sha256(open(out, 'rb').read()).hexdigest():
                same += 1
    print('TUs:', len(res2), 'byte-identical objects:', same)

if __name__ == '__main__':
    ap = argparse.ArgumentParser()
    ap.add_argument('--status-only', action='store_true')
    ap.add_argument('--all', action='store_true')
    a = ap.parse_args()
    if a.status_only: status_only()
    if a.all: all_tus()
    if not (a.status_only or a.all): ap.print_help()
