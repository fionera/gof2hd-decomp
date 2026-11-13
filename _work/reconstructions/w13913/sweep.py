#!/usr/bin/env python3
"""Per-TU non-optimization flag sweep for the wave-192 residual functions."""
import argparse, concurrent.futures, json, os, subprocess, sys, time

ROOT = '/home/debian/GalaxyOnFire2/wt/w13913'
SCRATCH = '/home/debian/GalaxyOnFire2/scratch/w13913'
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
FMOD = '/home/debian/GalaxyOnFire2/_work/bins/apk_libs'
OBJDUMP = os.path.join(ROOT, '.cache/ndk/android-ndk-r18b/toolchains/'
                       'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
                       'arm-linux-androideabi-objdump')

TUS = {
 'src/game/mission/LevelScript.cpp': ['_ZN11LevelScript7processEi'],
 'src/game/gui/Radar.cpp': ['_ZN5Radar4drawEP6PlayerP3Hudi'],
 'src/game/ui/Layout.cpp': ['_ZN6LayoutC1Ev', '_ZN6LayoutC2Ev'],
 'src/game/world/Level.cpp': ['_ZN5Level10createShipEiiiP8Waypointbb',
                              '_ZN5Level13createMissionEv'],
 'src/game/menu/MGame.cpp': ['_ZN5MGame10OnTouchEndEiiPv', '_ZN5MGame8OnUpdateEv'],
 'src/game/ui/HangarWindow.cpp': ['_ZN12HangarWindow6renderEv'],
 'src/game/mission/Status.cpp': ['_ZN6Status16missionCompletedEbbx'],
 'src/game/gui/Hud.cpp': ['_ZN3Hud4drawExxP9PlayerEgobjj'],
}
FLAGS = ['', '-fno-strict-aliasing', '-fwrapv', '-fno-delete-null-pointer-checks',
         '-fno-sized-deallocation', '-fno-threadsafe-statics', '-fvisibility=hidden',
         '-fomit-frame-pointer', '-fno-omit-frame-pointer', '-mno-unaligned-access',
         '-fno-math-errno', '-fno-exceptions', '-fno-rtti', '-fno-inline-functions',
         '-fno-vectorize', '-fno-slp-vectorize', '-fno-jump-tables', '-fno-common',
         '-fno-builtin', '-ffreestanding']

def slug(tu):
    return tu.replace('src/', '').replace('/', '_').rsplit('.', 1)[0]

def run(cmd, timeout=1800):
    t0 = time.time()
    try:
        r = subprocess.run(cmd, shell=True, capture_output=True, text=True, timeout=timeout)
    except subprocess.TimeoutExpired:
        return 124, 'TIMEOUT', time.time() - t0
    return r.returncode, r.stdout + r.stderr, time.time() - t0

def sect_size(so, name):
    rc, out, _ = run('%s -h %s' % (OBJDUMP, so))
    for l in out.splitlines():
        p = l.split()
        if len(p) > 4 and p[1] == name:
            return int(p[2], 16)
    return None

def cell(tu, flags, tag):
    bd = os.path.join(SCRATCH, 'sweep', slug(tu))
    os.makedirs(bd, exist_ok=True)
    cfg = ('ulimit -v 12000000; cmake -S %s -B %s -G Ninja '
           '-DCMAKE_PROJECT_INCLUDE=%s/tools/per_tu_flags.cmake '
           '-DGOF2_AB_TU=%s -DGOF2_AB_FLAGS=%s '
           '-DGOF2_ORIGINAL_SO=%s -DGOF2_FMOD_LIBDIR=%s' %
           (ROOT, bd, ROOT, tu, "'" + flags + "'", ORIG, FMOD))
    rc, log, dt = run(cfg)
    if rc != 0:
        return {'tu': tu, 'flags': flags, 'error': 'configure', 'log': log[-1500:], 'secs': dt}
    rc, blog, bt = run('ulimit -v 12000000; cmake --build %s --target link_so -j2' % bd)
    if rc != 0:
        return {'tu': tu, 'flags': flags, 'error': 'build', 'log': blog[-1500:], 'secs': dt + bt}
    so = os.path.join(bd, 'libgof2hdaa.so')
    rc, sc, _ = run('python3 %s/tools/fn_diff.py --ours %s %s --json' %
                    (ROOT, so, ' '.join(TUS[tu])))
    try:
        scores = json.loads(sc)
    except Exception:
        scores = {'err': sc[-500:]}
    rc, vlog, vt = run('python3 %s/tools/verify/verify.py --build-dir %s/verify '
                       '--original %s --ours %s --objdump %s --fail-on-wrong-type --top 0' %
                       (ROOT, bd, ORIG, so, OBJDUMP))
    gate = {}
    try:
        rows = json.load(open(os.path.join(bd, 'verify', 'report.json')))['rows']
        gate = {'byte_exact': sum(1 for x in rows if x.get('byte')),
                'linked_exact': sum(1 for x in rows if x.get('linked')),
                'size_exact': sum(1 for x in rows if x.get('osize') == x.get('usize')),
                'missing': sum(1 for x in rows if x.get('kind') == 'missing'),
                'extra': sum(1 for x in rows if x.get('kind') == 'extra'),
                'rows': len(rows)}
    except Exception as e:
        gate = {'err': str(e), 'log': vlog[-800:]}
    res = {'tu': tu, 'flags': flags, 'tag': tag, 'scores': scores, 'gate': gate,
           'text': sect_size(so, '.text'), 'extab': sect_size(so, '.ARM.extab'),
           'secs': dt + bt + vt}
    print('[%s] %-30s byte=%s text=%s extab=%s %.0fs' %
          (slug(tu), flags or '(base)', gate.get('byte_exact'), res['text'],
           res['extab'], res['secs']), flush=True)
    return res

def one_tu(tu, flags_list):
    return [cell(tu, f, '') for f in flags_list]

def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--tus', nargs='*', default=list(TUS))
    ap.add_argument('--flags', nargs='*', default=FLAGS)
    ap.add_argument('--jobs', type=int, default=2)
    ap.add_argument('--out', default=os.path.join(SCRATCH, 'sweep_results.json'))
    a = ap.parse_args()
    allres = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs) as ex:
        futs = {ex.submit(one_tu, tu, a.flags): tu for tu in a.tus}
        for f in concurrent.futures.as_completed(futs):
            allres.extend(f.result())
    old = []
    if os.path.exists(a.out):
        try:
            old = json.load(open(a.out))
        except Exception:
            old = []
    key = lambda r: (r['tu'], r.get('flags'), r.get('tag'))
    merged = {key(r): r for r in old}
    merged.update({key(r): r for r in allres})
    json.dump(list(merged.values()), open(a.out, 'w'), indent=1)
    print('wrote', a.out, len(merged), 'rows')

if __name__ == '__main__':
    main()
