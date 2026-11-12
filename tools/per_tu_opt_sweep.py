#!/usr/bin/env python3
"""Per-TU optimization-level sweep for the residual non-byte-exact functions.

Hypothesis tested: some TUs were compiled at a level other than -Oz (engine at -O2,
game at -Oz, a few files at -Os, ...).  For each TU that owns a residual function (and
for a control set of fully exact TUs) this rebuilds *only that TU* at -O1/-O2/-Os/-O3
(with -Oz as the control) and records:

  * the residual function's raw same-address byte diff and instruction-index aligned diff
    (tools/fn_diff.py),
  * the full per-function gate (byte_exact / linked_exact / size_exact) from
    tools/verify/verify.py,
  * the linked .text size.

Each (TU, level) cell uses its own scratch build directory, so the cells are independent
and can run in parallel.  The canonical tree is never modified.

Usage:
  python3 tools/per_tu_opt_sweep.py --build-root /dev/shm/per_tu_opt_sweep --jobs 16
  python3 tools/per_tu_opt_sweep.py --build-root ... --controls src/engine/render/PaintCanvas.cpp
"""
import argparse, concurrent.futures, json, os, subprocess, sys

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
OBJDUMP = os.path.join(ROOT, '.cache', 'ndk', 'android-ndk-r18b', 'toolchains',
                       'arm-linux-androideabi-4.9', 'prebuilt', 'linux-x86_64', 'bin',
                       'arm-linux-androideabi-objdump')
_orig = [os.path.normpath(os.path.join(ROOT, '..', '_work', 'bins',
                                       'android_2.0.16_libgof2hdaa.so')),
         '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so']
ORIG = next((p for p in _orig if os.path.exists(p)), _orig[0])
FMOD = os.path.join(os.path.dirname(ORIG), 'apk_libs')

# TU -> residual functions it owns (the wave-154 residual set).
RESIDUAL = {
    'src/game/ui/Layout.cpp': ['_ZN6LayoutC1Ev', '_ZN6LayoutC2Ev'],
    'src/game/world/LevelScript.cpp': ['_ZN11LevelScript7processEi'],
    'src/game/world/Level.cpp': ['_ZN5Level10createShipEiiiP8Waypointbb',
                                 '_ZN5Level13createMissionEv'],
    'src/game/mission/Status.cpp': ['_ZN6Status16missionCompletedEbbx'],
    'src/game/weapons/Radar.cpp': ['_ZN5Radar4drawEP6PlayerP3Hudi'],
    'src/game/menu/MGame.cpp': ['_ZN5MGame10OnTouchEndEiiPv', '_ZN5MGame8OnUpdateEv'],
    'src/game/weapons/Gun.cpp': ['_ZN3Gun22calcCharacterCollisionEv'],
    'src/game/ui/HangarWindow.cpp': ['_ZN12HangarWindow6renderEv'],
    'src/game/menu/SpaceLounge.cpp': ['_ZN11SpaceLounge10drawLoungeEv'],
    'src/game/ui/Hud.cpp': ['_ZN3Hud4drawExxP9PlayerEgobjj'],
    'src/game/mission/Generator.cpp':
        ['_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE'],
    'src/game/ui/MenuTouchWindow.cpp': ['_ZN15MenuTouchWindow12OnTouchBeginEiiPv'],
    'src/game/menu/ModStation.cpp': ['_ZN10ModStationC1Ev', '_ZN10ModStationC2Ev'],
}
LEVELS = ['-Oz', '-O1', '-O2', '-Os', '-O3']


def slug(tu):
    return tu.replace('src/', '').replace('/', '_').rsplit('.', 1)[0]


def run(cmd):
    return subprocess.run(cmd, shell=True, capture_output=True, text=True)


def text_size(so):
    out = run('%s -h %s' % (OBJDUMP, so)).stdout
    for l in out.splitlines():
        p = l.split()
        if len(p) > 4 and p[1] == '.text':
            return int(p[3], 16)
    return None


def one_tu(tu, targets, levels, build_root):
    bd = os.path.join(build_root, slug(tu))
    os.makedirs(bd, exist_ok=True)
    results = []
    for lvl in levels:
        cfg = ('cmake -S %s -B %s -G Ninja -DCMAKE_PROJECT_INCLUDE=%s '
               '-DGOF2_AB_TU=%s -DGOF2_AB_FLAGS=%s '
               '-DGOF2_ORIGINAL_SO=%s -DGOF2_FMOD_LIBDIR=%s' %
               (ROOT, bd, os.path.join(ROOT, 'tools', 'per_tu_flags.cmake'),
                tu, lvl, ORIG, FMOD))
        r = run(cfg)
        if r.returncode != 0:
            results.append({'tu': tu, 'level': lvl, 'error': 'configure',
                            'log': (r.stdout + r.stderr)[-1200:]})
            continue
        r = run('cmake --build %s --target link_so -j2' % bd)
        if r.returncode != 0:
            results.append({'tu': tu, 'level': lvl, 'error': 'build',
                            'log': (r.stdout + r.stderr)[-1200:]})
            continue
        so = os.path.join(bd, 'libgof2hdaa.so')
        if targets:
            sc = run('python3 %s --ours %s %s --json' %
                     (os.path.join(ROOT, 'tools', 'fn_diff.py'), so, ' '.join(targets)))
            try:
                scores = json.loads(sc.stdout)
            except Exception:
                scores = {'err': (sc.stdout + sc.stderr)[-500:]}
        else:
            scores = []
        ver = run('python3 %s --build-dir %s/verify --original %s --ours %s '
                  '--objdump %s --fail-on-wrong-type --top 0' %
                  (os.path.join(ROOT, 'tools', 'verify', 'verify.py'),
                   bd, ORIG, so, OBJDUMP))
        gate = {}
        try:
            rows = json.load(open(os.path.join(bd, 'verify', 'report.json')))['rows']
            gate = {'byte_exact': sum(1 for x in rows if x.get('byte')),
                    'linked_exact': sum(1 for x in rows if x.get('linked')),
                    'size_exact': sum(1 for x in rows if x.get('osize') == x.get('usize')),
                    'rows': len(rows)}
        except Exception as e:
            gate = {'err': str(e), 'log': (ver.stdout + ver.stderr)[-800:]}
        results.append({'tu': tu, 'level': lvl, 'scores': scores, 'gate': gate,
                        'text_size': text_size(so), 'so': so})
        print('[%s] %s gate=%s/%s size=%s text=%s' %
              (slug(tu), lvl, gate.get('byte_exact'), gate.get('linked_exact'),
               gate.get('size_exact'), text_size(so)), flush=True)
    return results


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--build-root', default='/dev/shm/per_tu_opt_sweep')
    ap.add_argument('--jobs', type=int, default=4)
    ap.add_argument('--levels', default=','.join(LEVELS))
    ap.add_argument('--controls', nargs='*', default=[])
    ap.add_argument('--out', default=None)
    a = ap.parse_args()
    tus = dict(RESIDUAL)
    for c in a.controls:
        tus[c] = []
    levels = a.levels.split(',')
    out = []
    with concurrent.futures.ThreadPoolExecutor(max_workers=a.jobs) as ex:
        futs = {ex.submit(one_tu, tu, tg, levels, a.build_root): tu
                for tu, tg in tus.items()}
        for f in concurrent.futures.as_completed(futs):
            out.extend(f.result())
    path = a.out or os.path.join(a.build_root, 'results.json')
    json.dump(out, open(path, 'w'), indent=1)
    print('WROTE ' + path)


if __name__ == '__main__':
    main()
