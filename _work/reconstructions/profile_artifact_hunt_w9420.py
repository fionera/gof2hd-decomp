#!/usr/bin/env python3
"""Profile-artifact hunt for the GoF2 HD original build (wave 148, w9420).

Modes:
  scan           search every APK / sibling binary / the original .so for
                 .profraw/.profdata/LLVM/PGO artifacts and profile sections.
  sample-replay  reconstruct a sample profile for Status::missionCompleted,
                 build Status.cpp with -fprofile-sample-use, link the whole
                 .so, and print the raw/aligned/size numbers.

The scan is read-only.  sample-replay writes only under /dev/shm/w9420.
"""
import os, re, subprocess, sys, zipfile

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
NDK = os.path.join(REPO, '.cache/ndk/android-ndk-r18b')
OBJDUMP = os.path.join(NDK, 'toolchains/arm-linux-androideabi-4.9/prebuilt/'
                       'linux-x86_64/bin/arm-linux-androideabi-objdump')
SCRATCH = '/dev/shm/w9420'
APKS = [
    '/dev/shm/gof2flags/GOF2_2.0.16_Original_Price.apk',
    '/tmp/gof2re2/stock/net.fishlabs.gof2hdallandroid2012.apk',
    '/tmp/gof2mod/apk/GOF2FHD_Original.apk',
    '/tmp/gof2mod/apk/GOF2FHD_MOD.apk',
    '/home/debian/GalaxyOnFire2/Original APK/Galaxy on Fire 2™ HD_2.0.16_APKPure.apk',
]
SIBLINGS = [
    '/home/debian/GalaxyOnFire2/_work/bins/ios_1.1.4_HD',
    '/home/debian/GalaxyOnFire2/_work/bins/ios_1.0.5_armv7',
    '/home/debian/GalaxyOnFire2/_work/bins/win_GOF2.exe',
]
PAT = re.compile(r'prof|llvm|pgo|\.bc$|\.o$|\.a$|\.d$|build|ninja|cmake|make|'
                 r'\.json$|\.txt$|\.log$', re.I)
PGO_STRINGS = ['__llvm_prf', '__profc', '__profd', '__profn', 'profraw',
               'profdata', 'fprofile', 'PGO', '.pgd', '.pgc', '__PGO']


def scan():
    print('=== APK entries matching profile/build artifacts ===')
    for apk in APKS:
        print('---', apk)
        try:
            with zipfile.ZipFile(apk) as z:
                hits = [n for n in z.namelist() if PAT.search(n)]
        except Exception as e:
            print('  ERROR', e)
            continue
        print('\n'.join('  ' + h for h in hits) if hits else '  (none)')

    print('\n=== original .so sections ===')
    out = subprocess.run(['readelf', '-SW', ORIG], capture_output=True, text=True).stdout
    for ln in out.splitlines():
        if re.match(r'\s*\[\s*\d+\]', ln):
            print(ln.rstrip())

    print('\n=== original .so PGO strings ===')
    data = open(ORIG, 'rb').read()
    for p in PGO_STRINGS:
        print('  %-12s %d' % (p, data.count(p.encode())))

    print('\n=== sibling binaries PGO strings ===')
    for b in SIBLINGS:
        d = open(b, 'rb').read()
        print('---', b)
        for p in PGO_STRINGS:
            print('  %-12s %d' % (p, d.count(p.encode())))

    print('\n=== profile files under /home/debian/GalaxyOnFire2 and /dev/shm ===')
    for root in ['/home/debian/GalaxyOnFire2', '/dev/shm']:
        for dirpath, _, files in os.walk(root):
            for f in files:
                if re.search(r'\.(profraw|profdata|proftext|pgd|pgc|gcda|gcno)$', f, re.I):
                    print(' ', os.path.join(dirpath, f))


def mission_numbers(so):
    """Return (raw, aligned, size) for Status::missionCompleted from a .so."""
    name = '_ZN6Status16missionCompletedEbbx'

    def sym(path):
        out = subprocess.run([OBJDUMP, '-T', path], capture_output=True, text=True).stdout
        for ln in out.splitlines():
            p = ln.split()
            if len(p) >= 6 and p[-1] == name:
                return int(p[0], 16), int(p[4], 16)
        return None

    def textsec(path):
        out = subprocess.run([OBJDUMP, '-h', path], capture_output=True, text=True).stdout
        for ln in out.splitlines():
            p = ln.split()
            if p and p[1:2] == ['.text']:
                return int(p[3], 16), int(p[4], 16), int(p[5], 16)
        raise SystemExit('no .text')

    a, b = sym(ORIG), sym(so)
    if not a or not b:
        raise SystemExit('missing symbol')
    sz, vma, off = textsec(ORIG)
    x = open(ORIG, 'rb').read()[off + (a[0] - vma): off + (a[0] - vma) + a[1]]
    sz2, vma2, off2 = textsec(so)
    y = open(so, 'rb').read()[off2 + (b[0] - vma2): off2 + (b[0] - vma2) + b[1]]
    raw = sum(p != q for p, q in zip(x, y))
    # instruction-index alignment (same algorithm as align_diff.py)
    def disasm(path, addr, size):
        out = subprocess.run([OBJDUMP, '-d', '--start-address=%d' % addr,
                              '--stop-address=%d' % (addr + size), path],
                             capture_output=True, text=True).stdout
        ins = []
        for ln in out.splitlines():
            m = re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$', ln)
            if m:
                ins.append(m.group(2).strip())
        return ins
    def disasm_idx(path, addr, size):
        out = subprocess.run([OBJDUMP, '-d', '--start-address=%d' % addr,
                              '--stop-address=%d' % (addr + size), path],
                             capture_output=True, text=True).stdout
        ins = []
        for ln in out.splitlines():
            m = re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$', ln)
            if m:
                ins.append((int(m.group(1), 16), m.group(2).strip()))
        return ins
    oi, ui = disasm_idx(ORIG, a[0], a[1]), disasm_idx(so, b[0], b[1])
    oidx = {ad: i for i, (ad, _) in enumerate(oi)}
    uidx = {ad: i for i, (ad, _) in enumerate(ui)}
    def norm(t, idx):
        def rep(m):
            ad = int(m.group(1), 16)
            return 'I%d' % idx[ad] if ad in idx else 'X'
        t2 = re.sub(r'\b([0-9a-f]{4,8})\b', rep, t)
        t2 = re.sub(r'<[^>]*>', '', t2)
        return re.sub(r'\s+', ' ', t2).strip()
    ot = [norm(t, oidx) for _, t in oi]
    ut = [norm(t, uidx) for _, t in ui]
    import difflib
    sm = difflib.SequenceMatcher(None, ot, ut, autojunk=False)
    aligned = sum(max(i2 - i1, j2 - j1) for tag, i1, i2, j1, j2 in sm.get_opcodes()
                  if tag != 'equal')
    return raw, aligned, len(x), len(y)


def sample_replay():
    os.makedirs(SCRATCH, exist_ok=True)
    sample = os.path.join(SCRATCH, 'all.sample')
    with open(sample, 'w') as f:
        f.write('_ZN6Status16missionCompletedEbbx:100000:100000\n')
        for i in range(0, 260):
            f.write(' %d: 1000\n' % i)
    launch = os.path.join(SCRATCH, 'launch.sh')
    with open(launch, 'w') as f:
        f.write('#!/bin/sh\ncase "$*" in\n'
                '  *mission/Status.cpp*) exec "$@" -fprofile-sample-use=%s ;;\n'
                '  *) exec "$@" ;;\nesac\n' % sample)
    os.chmod(launch, 0o755)
    build = os.path.join(SCRATCH, 'build_sample')
    subprocess.run(['cmake', '-S', REPO, '-B', build, '-G', 'Ninja',
                    '-DGOF2_ORIGINAL_SO=' + ORIG,
                    '-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs',
                    '-DCMAKE_C_COMPILER_LAUNCHER=' + launch,
                    '-DCMAKE_CXX_COMPILER_LAUNCHER=' + launch], check=True)
    subprocess.run(['cmake', '--build', build, '--target', 'link_so', '-j6'], check=True)
    raw, aligned, osz, usz = mission_numbers(os.path.join(build, 'libgof2hdaa.so'))
    print('sample-PGO replay: raw %d  aligned %d  size %d -> %d' % (raw, aligned, osz, usz))


if __name__ == '__main__':
    if len(sys.argv) < 2 or sys.argv[1] not in ('scan', 'sample-replay'):
        print(__doc__)
        sys.exit(1)
    if sys.argv[1] == 'scan':
        scan()
    else:
        sample_replay()
