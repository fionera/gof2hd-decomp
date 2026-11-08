#!/usr/bin/env python3
"""Targeted clang -mllvm codegen sweep for the wave-154 residual set.

Builds every variant in a scratch directory (default /dev/shm/w10116), links
libgof2hdaa.so with the normal gold pipeline, runs the official verifier for
byte/linked/size-exact, and scores the 17 residual functions with both metrics:

  raw     same-address byte diff (fdiff.py)
  aligned instruction-index alignment (align_diff.py)

Usage:
  python3 _work/reconstructions/mllvm_sweep_w10116.py [--scratch DIR] [--jobs N]

The repo build (cmake-build-match) and tools/verify_baseline.json are not
touched.  Results are written to <scratch>/results.json.
"""
import argparse, json, os, re, subprocess, sys, time, collections, difflib

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
FMOD = '/home/debian/GalaxyOnFire2/_work/bins/apk_libs'
OBJ = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/'
                   'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
                   'arm-linux-androideabi-objdump')

RESIDUALS = [
    '_ZN6LayoutC1Ev', '_ZN6LayoutC2Ev', '_ZN11LevelScript7processEi',
    '_ZN5Level10createShipEiiiP8Waypointbb', '_ZN5Radar4drawEP6PlayerP3Hudi',
    '_ZN6Status16missionCompletedEbbx', '_ZN5MGame10OnTouchEndEiiPv',
    '_ZN5Level13createMissionEv', '_ZN3Gun22calcCharacterCollisionEv',
    '_ZN12HangarWindow6renderEv', '_ZN11SpaceLounge10drawLoungeEv',
    '_ZN3Hud4drawExxP9PlayerEgobjj',
    '_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE',
    '_ZN15MenuTouchWindow12OnTouchBeginEiiPv', '_ZN10ModStationC1Ev',
    '_ZN10ModStationC2Ev', '_ZN5MGame8OnUpdateEv',
]

# (id, extra flags).  Invalid clang-7.0.2 spellings are kept so the sweep
# records the failure; the nearest valid spellings follow the 'v-' prefix.
VARIANTS = [
    ('base', ''),
    ('slp-threshold=0', '-mllvm -slp-threshold=0'),
    ('slp-threshold=1', '-mllvm -slp-threshold=1'),
    ('slp-threshold=2', '-mllvm -slp-threshold=2'),
    ('slp-threshold=4', '-mllvm -slp-threshold=4'),
    ('slp-threshold=8', '-mllvm -slp-threshold=8'),
    ('slp-max-vf=1', '-mllvm -slp-max-vf=1'),
    ('slp-max-vf=2', '-mllvm -slp-max-vf=2'),
    ('slp-max-vf=4', '-mllvm -slp-max-vf=4'),
    ('slp-max-vf=8', '-mllvm -slp-max-vf=8'),
    ('v-slp-max-reg-size=1', '-mllvm -slp-max-reg-size=1'),
    ('v-slp-max-reg-size=2', '-mllvm -slp-max-reg-size=2'),
    ('v-slp-max-reg-size=4', '-mllvm -slp-max-reg-size=4'),
    ('v-slp-max-reg-size=8', '-mllvm -slp-max-reg-size=8'),
    ('loop-sink=false', '-mllvm -enable-loop-sink=false'),
    ('loop-sink=true', '-mllvm -enable-loop-sink=true'),
    ('v-loop-sink=false', '-mllvm -loop-sink=false'),
    ('v-loop-sink=true', '-mllvm -loop-sink=true'),
    ('tail-merge=0', '-mllvm -tail-merge-threshold=0'),
    ('tail-merge=1', '-mllvm -tail-merge-threshold=1'),
    ('tail-merge=2', '-mllvm -tail-merge-threshold=2'),
    ('tail-merge=4', '-mllvm -tail-merge-threshold=4'),
    ('v-enable-tail-merge=false', '-mllvm -enable-tail-merge=false'),
    ('inline-threshold=0', '-mllvm -inline-threshold=0'),
    ('inline-threshold=1', '-mllvm -inline-threshold=1'),
    ('inline-threshold=5', '-mllvm -inline-threshold=5'),
    ('inline-threshold=25', '-mllvm -inline-threshold=25'),
    ('inline-threshold=50', '-mllvm -inline-threshold=50'),
    ('unroll-threshold=0', '-mllvm -unroll-threshold=0'),
    ('unroll-threshold=50', '-mllvm -unroll-threshold=50'),
    ('unroll-threshold=150', '-mllvm -unroll-threshold=150'),
    ('unroll-threshold=300', '-mllvm -unroll-threshold=300'),
    ('gvn-hoist=false', '-mllvm -enable-gvn-hoist=false'),
    ('load-pre=false', '-mllvm -enable-load-pre=false'),
    ('early-taildup=false', '-mllvm -enable-early-taildup=false'),
    ('v-disable-early-taildup', '-mllvm -disable-early-taildup=true'),
    ('merge-functions', '-mllvm -enable-merge-functions'),
    ('v-fmerge-functions', '-fmerge-functions'),
    ('machine-outliner', '-mllvm -enable-machine-outliner'),
    ('constant-islands=false', '-mllvm -enable-constant-islands=false'),
    ('v-arm-cp-islands=false', '-mllvm -arm-cp-islands=false'),
    ('pgs-prefix=false', '-mllvm -profile-guided-section-prefix=false'),
]

TOOLCHAIN = '''include("{root}/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${{CMAKE_CXX_FLAGS}} ${{GOF2_EXTRA_FLAGS}}" CACHE STRING "matching ARM compile flags" FORCE)
  set(CMAKE_C_FLAGS   "${{CMAKE_C_FLAGS}}   ${{GOF2_EXTRA_FLAGS}}" CACHE STRING "matching ARM C compile flags" FORCE)
endif()
'''


def sym(path, name):
    out = subprocess.check_output([OBJ, '-T', path], text=True)
    for line in out.splitlines():
        p = line.split()
        if len(p) >= 6 and p[-1] == name:
            return int(p[0], 16), int(p[4], 16)
    return None


def textsec(path):
    out = subprocess.check_output([OBJ, '-h', path], text=True)
    for line in out.splitlines():
        p = line.split()
        if len(p) > 1 and p[1] == '.text':
            return int(p[3], 16), int(p[4], 16), int(p[5], 16)
    raise SystemExit('no .text')


def data(path, addr, size):
    blob = open(path, 'rb').read()
    _sz, vma, off = textsec(path)
    return blob[off + (addr - vma):off + (addr - vma) + size]


def disasm(path, addr, size):
    out = subprocess.check_output(
        [OBJ, '-d', '--start-address=%d' % addr, '--stop-address=%d' % (addr + size), path],
        text=True)
    ins = []
    for line in out.splitlines():
        m = re.match(r'\s*([0-9a-f]+):\s+(?:[0-9a-f]{2,8}(?:\s+[0-9a-f]{2,8})?)\s+(.*)$', line)
        if not m:
            continue
        a, t = int(m.group(1), 16), m.group(2).strip()
        if t:
            ins.append((a, t))
    return ins


def opcode(t):
    return t.split()[0] if t else ''


def classify(o, u):
    if o is None:
        return 'extra-ours:' + opcode(u)
    if u is None:
        return 'extra-orig:' + opcode(o)
    om, um = opcode(o), opcode(u)
    ob, ub = om.replace('.w', '').replace('.n', ''), um.replace('.w', '').replace('.n', '')
    if om == um:
        oo, uo = o[len(om):].strip(), u[len(um):].strip()
        if re.sub(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b', 'R', oo) == re.sub(r'\b(r\d+|sb|sl|fp|ip|sp|lr|pc)\b', 'R', uo):
            return 'regalloc:' + om
        if re.sub(r'#-?0x[0-9a-f]+|#-?\d+', '#I', oo) == re.sub(r'#-?0x[0-9a-f]+|#-?\d+', '#I', uo):
            return 'imm:' + om
        if re.sub(r'\[[^\]]*\]', '[M]', oo) == re.sub(r'\[[^\]]*\]', '[M]', uo):
            return 'memaddr:' + om
        return 'operand:' + om
    if ob == ub:
        return 'width:' + om + '/' + um
    fam = {'ldr': 'load', 'ldrb': 'load', 'ldrh': 'load', 'str': 'store', 'strb': 'store',
           'strh': 'store', 'mov': 'mov', 'movs': 'mov', 'movw': 'mov', 'movt': 'mov',
           'mvn': 'mov', 'mvns': 'mov', 'add': 'add', 'adds': 'add', 'sub': 'sub',
           'subs': 'sub', 'cmp': 'cmp', 'cmn': 'cmp', 'tst': 'cmp', 'teq': 'cmp'}
    if fam.get(ob) == fam.get(ub) and fam.get(ob):
        return 'family:' + om + '/' + um
    return 'opcode:' + om + '/' + um


def aligned(name, a, b):
    oi, ui = disasm(ORIG, *a), disasm(OURS, *b)
    oidx = {ad: i for i, (ad, _t) in enumerate(oi)}
    uidx = {ad: i for i, (ad, _t) in enumerate(ui)}

    def norm(t, idx):
        def rep(m):
            ad = int(m.group(1), 16)
            return 'I%d' % idx[ad] if ad in idx else 'X'
        t2 = re.sub(r'\b([0-9a-f]{4,8})\b', rep, t)
        t2 = re.sub(r'<[^>]*>', '', t2)
        return re.sub(r'\s+', ' ', t2).strip()

    ot = [norm(t, oidx) for _a, t in oi]
    ut = [norm(t, uidx) for _a, t in ui]
    sm = difflib.SequenceMatcher(None, ot, ut, autojunk=False)
    n = 0
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == 'equal':
            continue
        n += max(i2 - i1, j2 - j1) if tag == 'replace' else (i2 - i1 if tag == 'delete' else j2 - j1)
    return n


def score(so):
    global OURS
    OURS = so
    out = {}
    for name in RESIDUALS:
        a, b = sym(ORIG, name), sym(so, name)
        if not a or not b:
            out[name] = {'raw': None, 'aligned': None}
            continue
        x, y = data(ORIG, a[0], a[1]), data(so, b[0], b[1])
        m = min(len(x), len(y))
        out[name] = {'raw': sum(p != q for p, q in zip(x[:m], y[:m])),
                     'aligned': aligned(name, a, b), 'size': len(x)}
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--scratch', default='/dev/shm/w10116')
    ap.add_argument('--jobs', type=int, default=6)
    args = ap.parse_args()
    scratch = args.scratch
    os.makedirs(scratch, exist_ok=True)
    bd = os.path.join(scratch, 'build-sweep')
    tc = os.path.join(scratch, 'toolchain_extra.cmake')
    open(tc, 'w').write(TOOLCHAIN.format(root=ROOT))
    results = {}
    for vid, flags in VARIANTS:
        t0 = time.time()
        cfg = ['cmake', '-S', ROOT, '-B', bd, '-G', 'Ninja',
               '-DCMAKE_TOOLCHAIN_FILE=' + tc, '-DGOF2_MATCH_OPT=-Oz',
               '-DGOF2_EXTRA_FLAGS=' + flags,
               '-DGOF2_ORIGINAL_SO=' + ORIG, '-DGOF2_FMOD_LIBDIR=' + FMOD]
        r = subprocess.run(cfg, capture_output=True, text=True)
        if r.returncode != 0:
            results[vid] = {'status': 'cfg-fail', 'err': (r.stderr or r.stdout)[-800:]}
            print(vid, 'CFGFAIL', flush=True)
            continue
        r = subprocess.run(['cmake', '--build', bd, '--target', 'link_so', '-j', str(args.jobs)],
                           capture_output=True, text=True)
        if r.returncode != 0:
            results[vid] = {'status': 'build-fail', 'err': (r.stderr or r.stdout)[-1500:]}
            print(vid, 'BUILDFAIL', flush=True)
            continue
        so = os.path.join(bd, 'libgof2hdaa.so')
        vd = os.path.join(scratch, 'verify', vid)
        os.makedirs(vd, exist_ok=True)
        r = subprocess.run(['python3', os.path.join(ROOT, 'tools/verify/verify.py'),
                            '--build-dir', vd, '--original', ORIG, '--ours', so,
                            '--objdump', OBJ, '--fail-on-wrong-type', '--top', '0'],
                           capture_output=True, text=True)
        if r.returncode != 0:
            results[vid] = {'status': 'verify-fail', 'err': (r.stderr or r.stdout)[-1500:]}
            print(vid, 'VERIFYFAIL', flush=True)
            continue
        rows = json.load(open(os.path.join(vd, 'report.json')))['rows']
        results[vid] = {
            'status': 'ok',
            'byte': sum(1 for x in rows if x.get('byte')),
            'linked': sum(1 for x in rows if x.get('linked')),
            'size': sum(1 for x in rows if x.get('osize') == x.get('usize')),
            'score': score(so),
            'secs': round(time.time() - t0, 1),
        }
        print(vid, 'byte', results[vid]['byte'], 'linked', results[vid]['linked'],
              'size', results[vid]['size'], 't', results[vid]['secs'], flush=True)
        json.dump(results, open(os.path.join(scratch, 'results.json'), 'w'), indent=1)
    print('DONE')


if __name__ == '__main__':
    main()
