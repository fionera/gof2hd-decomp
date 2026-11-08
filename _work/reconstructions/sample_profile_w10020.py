#!/usr/bin/env python3
"""Sample-profile replay for Status::missionCompleted (wave 154, w10020)."""
import json, os, re, subprocess, sys

ROOT = '/home/debian/GalaxyOnFire2/wt/w10020'
BUILD = os.path.join(ROOT, 'cmake-build-match')
OBJDUMP = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/'
                       'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
                       'arm-linux-androideabi-objdump')
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = os.path.join(BUILD, 'libgof2hdaa.so')
MANGLE = '_ZN6Status16missionCompletedEbbx'
SCRATCH = '/dev/shm/w10020'
# offsets from function start line 1314
# line offsets (line - 1314) for every line of each case body
CASE_OFF = {
 'a':[23,24,25], 'b':[29,30,31,33,39,40],
 'c':[50,51,55,56,60,61,62],
 'd':[67,68,73,74,80,81,89,90,95,96,101,102,108,109,110,111,112,
      125,126,130,131,143,144,150,156,157,162,163,164,165,171,172,173],
 'e':[181,182,187,188,196,199,206,218,223,224,228,229,235,236,240,241,245,246,249],
}

def compile_command():
    with open(os.path.join(BUILD, 'compile_commands.json')) as f:
        for e in json.load(f):
            if e['file'].endswith('src/game/mission/Status.cpp'):
                return e['command'].replace(
                    '-o CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj -c', '')
    raise SystemExit('no cc')

def link_command():
    with open(os.path.join(BUILD, 'build.ninja')) as f:
        text = f.read()
    m = re.search(r'\n  COMMAND = (cd .*?libgof2hdaa\.so[^\n]*)\n', text)
    return m.group(1).split(' && ')

def sym(path, name):
    out = subprocess.check_output([OBJDUMP, '-T', path], text=True)
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    raise SystemExit('symbol not found')

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

def textsec(path):
    out = subprocess.check_output([OBJDUMP, '-h', path], text=True)
    for line in out.splitlines():
        p = line.split()
        if p[1:2] == ['.text']:
            return int(p[3], 16), int(p[4], 16), int(p[5], 16)
    raise SystemExit('no .text')

def raw_diff():
    a = sym(ORIG, MANGLE); b = sym(OURS, MANGLE)
    def data(path, addr, size):
        sz, vma, off = textsec(path)
        return open(path, 'rb').read()[off + (addr - vma):off + (addr - vma) + size]
    xa = data(ORIG, a[0], a[1]); ya = data(OURS, b[0], b[1])
    n = min(len(xa), len(ya))
    return sum(p != q for p, q in zip(xa[:n], ya[:n])), len(xa), len(ya)

def aligned_diff():
    import difflib
    a = sym(ORIG, MANGLE); b = sym(OURS, MANGLE)
    oi = disasm(ORIG, *a); ui = disasm(OURS, *b)
    oidx = {ad: i for i, (ad, _) in enumerate(oi)}
    uidx = {ad: i for i, (ad, _) in enumerate(ui)}
    def norm(t, idx):
        def rep(m):
            ad = int(m.group(1), 16)
            return 'I%d' % idx[ad] if ad in idx else 'X'
        t = re.sub(r'\b([0-9a-f]{4,8})\b', rep, t)
        t = re.sub(r'<[^>]*>', '', t)
        return re.sub(r'\s+', ' ', t).strip()
    ot = [norm(t, oidx) for _, t in oi]; ut = [norm(t, uidx) for _, t in ui]
    sm = difflib.SequenceMatcher(None, ot, ut, autojunk=False)
    n = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'replace': n += max(i2 - i1, j2 - j1)
        elif tag == 'delete': n += i2 - i1
        elif tag == 'insert': n += j2 - j1
    return n

def run(a, b, c, d, e, extra=()):
    prof = os.path.join(SCRATCH, 'status.prof')
    out = ['%s:1000:0' % MANGLE]
    for g, w in (('a', a), ('b', b), ('c', c), ('d', d), ('e', e)):
        for l in CASE_OFF[g]:
            out.append(' %d: %d' % (l, w))
    open(prof, 'w').write('\n'.join(out) + '\n')
    cc = compile_command()
    obj = os.path.join(BUILD, 'CMakeFiles/gof2.dir/src/game/mission/Status.cpp.obj')
    cmd = [x for x in cc.split() if not x.endswith('src/game/mission/Status.cpp')]
    cmd += ['-fprofile-sample-use=' + prof, '-c',
            os.path.join(ROOT, 'src/game/mission/Status.cpp'), '-o', obj] + list(extra)
    subprocess.run(cmd, check=True, cwd=ROOT, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    for part in link_command():
        subprocess.run(part, shell=True, check=True, cwd=ROOT,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    raw, osz, msz = raw_diff()
    print('raw %d  aligned %d  size %d (orig %d)' % (raw, aligned_diff(), msz, osz))

if __name__ == '__main__':
    extra = []
    if '--extra' in sys.argv:
        i = sys.argv.index('--extra'); extra = sys.argv[i + 1:]; sys.argv = sys.argv[:i]
    run(*[int(x) for x in sys.argv[1:6]], extra=extra)
