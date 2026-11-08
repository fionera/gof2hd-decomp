#!/usr/bin/env python3
"""Data/layout compile-flag fingerprint sweep (wave 155, w10119).

Re-runs the source-level flag matrix that changes data/layout rather than
codegen, at the wave-154 tree state (gate 4507/4524).  For every cell it:

  * reconfigures a scratch build with the flag appended AFTER the canonical
    CMAKE_{C,CXX}_FLAGS (a wrapper toolchain, so last-wins flags win),
  * deletes the 192 app objects and rebuilds `link_so` (vendored archives are
    reused; they are built with their own flags),
  * runs the official tools/verify/verify.py and records byte_exact /
    linked_exact / size-exact,
  * compares every allocatable section against the original and sums the
    same-address differing bytes over the 17 residual functions.

Two flag-ordering groups are needed:

  * `wrapper` cells append the flag to CMAKE_*_FLAGS.  This is enough for
    every flag except the ones the target overrides: CMakeLists.txt has
    `target_compile_options(gof2 PRIVATE -ffunction-sections -funwind-tables)`,
    which lands AFTER CMAKE_*_FLAGS, so `-fno-unwind-tables` in the wrapper is
    silently overridden.
  * `launcher` cells use CMAKE_*_COMPILER_LAUNCHER to append the flag at the
    very end of the compile command, after the target options.  This is the
    only way to observe `-fno-unwind-tables` / `-fno-asynchronous-unwind-tables`.

Usage:
    python3 _work/reconstructions/build_data_flag_fingerprint_w10119.py \
        --root /home/debian/GalaxyOnFire2/wt/w10119 \
        --scratch /dev/shm/w10119 --jobs 24

The script writes results.json (per-cell gate/section/raw data) and prints a
fingerprint table.  It does not touch the repo's cmake-build-match.
"""
import argparse, json, os, shutil, subprocess, sys, time

CELLS = [
    # (name, flags, mode)
    ('control', '', 'wrapper'),
    ('fmerge-all-constants', '-fmerge-all-constants', 'wrapper'),
    ('fno-merge-all-constants', '-fno-merge-all-constants', 'wrapper'),
    ('fno-merge-constants', '-fno-merge-constants', 'wrapper'),
    ('fno-common', '-fno-common', 'wrapper'),
    ('fshort-enums', '-fshort-enums', 'wrapper'),
    ('fno-exceptions', '-fno-exceptions', 'wrapper'),
    ('fno-rtti', '-fno-rtti', 'wrapper'),
    ('fvisibility-hidden', '-fvisibility=hidden', 'wrapper'),
    ('fvisibility-inlines-hidden', '-fvisibility-inlines-hidden', 'wrapper'),
    ('fstack-protector', '-fstack-protector', 'wrapper'),
    ('fstack-protector-all', '-fstack-protector-all', 'wrapper'),
    ('fno-omit-frame-pointer', '-fno-omit-frame-pointer', 'wrapper'),
    ('mfloat-abi-hard', '-mfloat-abi=hard', 'wrapper'),
    ('fno-unwind-tables', '-fno-unwind-tables', 'wrapper'),
    ('fno-asynchronous-unwind-tables', '-fno-asynchronous-unwind-tables', 'wrapper'),
    ('fno-threadsafe-statics', '-fno-threadsafe-statics', 'wrapper'),
    ('fno-use-cxa-atexit', '-fno-use-cxa-atexit', 'wrapper'),
    ('fno-sized-deallocation', '-fno-sized-deallocation', 'wrapper'),
    ('fsized-deallocation', '-fsized-deallocation', 'wrapper'),
    ('fno-stack-protector', '-fno-stack-protector', 'wrapper'),
    ('launch-control', '', 'launcher'),
    ('launch-fno-unwind-tables', '-fno-unwind-tables', 'launcher'),
    ('launch-fno-asynchronous-unwind-tables', '-fno-asynchronous-unwind-tables', 'launcher'),
]
SECS = ['.text', '.ARM.exidx', '.ARM.extab', '.rodata', '.data.rel.ro', '.data',
        '.bss', '.got', '.rel.plt', '.dynsym', '.dynstr', '.hash', '.rel.dyn',
        '.note.gnu.build-id', '.ARM.attributes', '.comment', '.shstrtab']

WRAPPER = '''include("{root}/cmake/toolchain.cmake")
set(GOF2_EXTRA_FLAGS "" CACHE STRING "extra matching flags")
if(GOF2_EXTRA_FLAGS)
  set(CMAKE_CXX_FLAGS "${{CMAKE_CXX_FLAGS}} ${{GOF2_EXTRA_FLAGS}}" CACHE STRING "" FORCE)
  set(CMAKE_C_FLAGS   "${{CMAKE_C_FLAGS}}   ${{GOF2_EXTRA_FLAGS}}" CACHE STRING "" FORCE)
endif()
'''
LAUNCHER = '''include("{root}/cmake/toolchain.cmake")
set(CMAKE_CXX_COMPILER_LAUNCHER "{scratch}/launch.sh" CACHE STRING "" FORCE)
set(CMAKE_C_COMPILER_LAUNCHER   "{scratch}/launch.sh" CACHE STRING "" FORCE)
'''
LAUNCH_SH = '#!/bin/sh\nexec "$@" $GOF2_EXTRA_FLAGS\n'


def run(cmd, log, env=None):
    with open(log, 'w') as f:
        return subprocess.run(cmd, stdout=f, stderr=subprocess.STDOUT, env=env).returncode


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('--root', default=os.path.dirname(os.path.dirname(os.path.dirname(
        os.path.abspath(__file__)))))
    ap.add_argument('--scratch', default='/dev/shm/w10119')
    ap.add_argument('--jobs', default='24')
    ap.add_argument('--original', default='/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so')
    ap.add_argument('--fmod', default='/home/debian/GalaxyOnFire2/_work/bins/apk_libs')
    args = ap.parse_args()
    root, scratch = os.path.abspath(args.root), os.path.abspath(args.scratch)
    os.makedirs(scratch + '/so', exist_ok=True)
    os.makedirs(scratch + '/logs', exist_ok=True)
    objdump = root + '/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump'

    # residual function list = the non-byte-exact rows of the canonical report
    report = json.load(open(root + '/cmake-build-match/verify/report.json'))
    residual = [r['name'] for r in report['rows'] if not r['byte']]
    json.dump(residual, open(scratch + '/residual.json', 'w'))

    open(scratch + '/launch.sh', 'w').write(LAUNCH_SH)
    os.chmod(scratch + '/launch.sh', 0o755)
    open(scratch + '/toolchain_wrapper.cmake', 'w').write(WRAPPER.format(root=root))
    open(scratch + '/toolchain_launcher.cmake', 'w').write(
        LAUNCHER.format(root=root, scratch=scratch))

    sys.path.insert(0, root + '/tools/verify')
    from elf import Elf32
    orig = Elf32(args.original)

    def score(so):
        u = Elf32(so)
        om = {s['name']: s for s in orig.sections}
        um = {s['name']: s for s in u.sections}
        sec = {}
        for n in SECS:
            if n not in om or n not in um:
                continue
            a, b = om[n], um[n]
            if a['type'] == 8:
                d = 'n/a'
            else:
                x = orig.data[a['offset']:a['offset'] + a['size']]
                y = u.data[b['offset']:b['offset'] + b['size']]
                d = sum(p != q for p, q in zip(x, y))
            sec[n] = {'osize': a['size'], 'usize': b['size'], 'diff': d}
        of, uf = orig.functions(), u.functions()
        raw = 0
        for nm in residual:
            if nm in of and nm in uf:
                x = orig.bytes_at(of[nm]['value'], of[nm]['size'])
                y = u.bytes_at(uf[nm]['value'], uf[nm]['size'])
                raw += sum(p != q for p, q in zip(x, y))
        return sec, raw

    results = {}
    for name, flags, mode in CELLS:
        bdir = scratch + '/build-' + mode
        toolchain = scratch + '/toolchain_' + mode + '.cmake'
        env = dict(os.environ)
        if mode == 'launcher':
            env['GOF2_EXTRA_FLAGS'] = flags
            cfg_flags = ''
        else:
            cfg_flags = flags
        rc = run(['cmake', '-S', root, '-B', bdir, '-G', 'Ninja',
                  '-DCMAKE_TOOLCHAIN_FILE=' + toolchain, '-DGOF2_MATCH_OPT=-Oz',
                  '-DGOF2_EXTRA_FLAGS=' + cfg_flags, '-DGOF2_ORIGINAL_SO=' + args.original,
                  '-DGOF2_FMOD_LIBDIR=' + args.fmod], scratch + '/logs/configure_%s.log' % name)
        subprocess.run(['find', bdir + '/CMakeFiles/gof2.dir', '-name', '*.obj', '-delete'], check=True)
        rc = run(['cmake', '--build', bdir, '--target', 'link_so', '-j' + args.jobs],
                 scratch + '/logs/build_%s.log' % name, env=env)
        rec = {'flags': flags, 'mode': mode, 'build_rc': rc}
        if rc == 0:
            so = scratch + '/so/' + name + '.so'
            shutil.copy(bdir + '/libgof2hdaa.so', so)
            vrc = run(['python3', root + '/tools/verify/verify.py',
                       '--build-dir', bdir + '/verify', '--original', args.original,
                       '--ours', so, '--objdump', objdump,
                       '--fail-on-wrong-type', '--top', '0'],
                      scratch + '/logs/verify_%s.log' % name)
            rec['verify_rc'] = vrc
            rep = json.load(open(bdir + '/verify/report.json'))
            rec.update(rep['summary'])
            rec['sec'], rec['raw'] = score(so)
        results[name] = rec
        print('%-36s rc=%s byte=%s linked=%s raw=%s' % (
            name, rec.get('build_rc'), rec.get('byte_exact'), rec.get('linked_exact'),
            rec.get('raw')), flush=True)
        json.dump(results, open(scratch + '/results.json', 'w'), indent=1)
    print('DONE ->', scratch + '/results.json')


if __name__ == '__main__':
    main()
