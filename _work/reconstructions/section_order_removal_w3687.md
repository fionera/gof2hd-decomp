# Section-ordering-file removal test (w3687)

Status: research note (2026-11-02). Worktree `wt/w3687`, branch `wave87-w3687`,
based on master `88aaf59f`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with this tree's NDK r18b
clang 7.0.2 / gold 1.12. This is a fresh measurement of the question already
posed in `_work/reconstructions/ffunction_sections_and_order_file.md` §2: the
original was linked without a `--section-ordering-file`, but our tree currently
needs one. The new result is the **root cause**: the removal cost is dominated by
**intra-TU function order**, not by TU membership/interleaving.

## TL;DR

* Baseline (unchanged): **byte_exact 4496 / linked_exact 4496**, `.text`
  `0x1337c8 @ 0x69440`, lint `CLEAN ✓`, drift `0`.
* Omitting `--section-ordering-file` (the faithful ndk-build link) drops
  **byte_exact 4496 -> 1527**; `linked_exact` stays **4496** and `.text` size
  stays exactly `0x1337c8`. Only **307 of 4524** exported functions remain at
  their original address, and the exact-address prefix is just **47**
  functions. The first mismatch is
  `keyEventReleased(Engine*, char*)` at `0x6a41c`.
* The original `.text` is grouped by original TU, and our object link order is
  already the original TU order. Strong inter-TU interleaving is only
  **1 of 191** objects (`NdkHooks.cpp` contains `NdkAutosave.cpp`'s
  `pConstToNonConst`). The removal blocker is that **94 of 191** objects emit
  their strong exported functions in an order different from the original's
  address order (3764 inverted pairs); counting weak copies, **156 of 193**
  objects differ (16061 inverted pairs).
* A targeted source reorder of the first divergent TU (`VirtualInput.cpp`)
  extends the exact-address prefix **47 -> 134**, but the next mismatch is
  `glShadeModel` (`GLES1Compat.cpp`), and with the ordering file re-enabled the
  same reorder **regresses** byte_exact **4496 -> 4491** because it changes the
  object's local-section placement. Source reordering is therefore not a free
  removal path; it must be done per TU under the full gate.
* A full `GOF2_MATCH_LAYOUT=OFF` rebuild (which also drops
  `-ffunction-sections`) is **1345 / 4355**, `.text 0x133178`; an empty ordering
  file is **1408**; a minimal file containing only `.text.startup`,
  `.text.crtbegin` and `*` is **1516**. None is close.
* **Conclusion: keep the ordering file for now.** Removing it requires
  converging the per-TU source/emission order (and the last strong interleaving),
  not a linker change.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section fingerprint (original == baseline except the known `.ARM.extab` -8):

| section | original | baseline |
|---|---:|---:|
| `.text` | `0x1337c8 @ 0x69440` | `0x1337c8 @ 0x69440` |
| `.ARM.exidx` | `0x6f70 @ 0x19cc08` | `0x6f70 @ 0x19cc08` |
| `.ARM.extab` | `0x26298 @ 0x1a3b78` | `0x26290 @ 0x1a3b78` |
| `.rodata` | `0x41b67 @ 0x1c9e10` | `0x41b67 @ 0x1c9e10` |
| `.data.rel.ro` | `0x2fe8 @ 0x20cbb0` | `0x2fe8 @ 0x20cbb0` |
| `.data` | `0x296c @ 0x214000` | `0x296c @ 0x214000` |
| `.bss` | `0x69ac @ 0x216970` | `0x69ac @ 0x216970` |

Exported functions at their original address: **4524 / 4524**.

## 2. Removal experiments

The relink helper takes the exact `libgof2hdaa.so` command from
`ninja -C cmake-build-match -t commands libgof2hdaa.so`, substitutes the output
path, and optionally removes or replaces the ordering-file argument. The
unchanged command reproduces `cmake-build-match/libgof2hdaa.so` byte-for-byte
(`cmp` identical).

```
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -2 > /tmp/w3687_linkcmd.txt
python3 /tmp/w3687/relink.py /tmp/w3687/base.so
cmp /tmp/w3687/base.so cmake-build-match/libgof2hdaa.so          # identical

python3 /tmp/w3687/relink.py /tmp/w3687/noorder.so --no-order    # omit the flag
python3 /tmp/w3687/relink.py /tmp/w3687/nullorder.so --null-order
python3 /tmp/w3687/relink_plain.py /tmp/w3687/plain_noorder.so   # plain objects, no markers
printf '.text.startup\n.text.crtbegin\n*\n' > /tmp/w3687/min_order.txt
python3 /tmp/w3687/relink_custom.py /tmp/w3687/minorder.so /tmp/w3687/min_order.txt
```

Verify each relink with:

```
python3 tools/verify/verify.py \
  --build-dir /tmp/w3687/verify_<variant> \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3687/<variant>.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
```

Measured (same 4524 compared functions):

| variant | byte_exact | linked_exact | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata` | exported funcs at orig addr |
|---|---:|---:|---:|---:|---:|---:|---:|
| baseline (ordering file) | **4496** | **4496** | `0x1337c8 @ 0x69440` | `0x6f70 @ 0x19cc08` | `0x26290 @ 0x1a3b78` | `0x41b67 @ 0x1c9e10` | **4524 / 4524** |
| omit `--section-ordering-file` | **1527** | 4496 | `0x1337c8 @ 0x69440` | `0x6f30 @ 0x19cc08` | `0x26290 @ 0x1a3b38` | `0x41b67 @ 0x1c9dd0` | **307 / 4524** |
| plain objects, omit flag | 1527 | 4496 | `0x1337c8 @ 0x69440` | `0x6f30 @ 0x19cc08` | `0x26290 @ 0x1a3b38` | `0x41b67 @ 0x1c9dd0` | 307 / 4524 |
| `--section-ordering-file=/dev/null` | 1408 | 4496 | `0x1337c8 @ 0x69440` | `0x6f30 @ 0x19cc08` | `0x26290 @ 0x1a3b38` | `0x41b67 @ 0x1c9dd0` | 114 / 4524 |
| minimal file (startup, crtbegin, `*`) | 1516 | 4496 | `0x1337c8 @ 0x69440` | `0x6f30 @ 0x19cc08` | `0x26290 @ 0x1a3b38` | `0x41b77 @ 0x1c9dd0` | 307 / 4524 |
| full rebuild, `GOF2_MATCH_LAYOUT=OFF` | 1345 | 4355 | `0x133178 @ 0x69240` | `0x64b8 @ 0x19c3b8` | `0x25570 @ 0x1a2870` | `0x41b67 @ 0x1c7de0` | 0 / 4524 |

Notes:

* Omitting the flag is the faithful ndk-build case. Passing an empty file is a
  different experiment: the file itself disables gold's default
  `.text.startup`-first sort, which costs another 119 byte_exact (1527 -> 1408).
* The minimal file shows that startup/crtbegin placement alone is worth almost
  nothing; the function list is the value. It also changes the `.rodata` group
  order (`.rodata` size `+0x10`), so it is not a clean isolation.
* The `GOF2_MATCH_LAYOUT=OFF` row is not a clean removal test: it also removes
  `-ffunction-sections` and the exidx-marker renaming. It is included only to
  show that the full no-layout path is far worse (`linked_exact` also drops,
  4496 -> 4355, because function sizes/aliases change).
* `linked_exact` is insensitive to the ordering file in every relink row
  (4496). The flag changes placement, not function bodies or call targets.

The older w3561 note recorded slightly different no-order `.ARM.exidx`
sizes (`0x6f38`/`0x6f60`). Those were measured on the pre-w3613 tree (195
objects, different empty-`.text` marker set); at this HEAD the marker and plain
variants both give `0x6f30`.

## 3. What actually breaks when the file is removed

### 3.1 Only the first 47 exported functions keep their address

```
python3 /tmp/w3687/addr_match.py ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3687/noorder.so
# 307/4524 common exported funcs at original address
```

The 307 is not a contiguous prefix. The exact-address prefix is **47**
functions; the first mismatch is:

```
0x6a41c  _Z16keyEventReleasedPN11AbyssEngine6EngineEPc
```

The original's emitted order in `VirtualInput.cpp` is:

```
0x6a41c  keyEventReleased
0x6a458  keyReleased
0x6a5e4  SendStoredKeyUpEvents
```

Our object order (plain link) is:

```
0x6a41c  SendStoredKeyUpEvents
0x6a458  keyReleased
0x6a5e4  keyEventReleased
```

The source file defines `SendStoredKeyUpEvents` at line 246 and
`keyEventReleased`/`keyReleased` at lines 365/372, i.e. the original TU's emission
order (and therefore its source order at `-Oz`) was different. This is an **intra-TU** ordering difference, not a TU-membership
difference.

### 3.2 Section effects

The `.text` size is unchanged because the ordering file only permutes function
sections. The `.ARM.exidx` shrinks by `0x40` (`0x6f70 -> 0x6f30`) and the
`.ARM.extab`/`.rodata` bases shift by `-0x40`. `.rodata` **content** is still
byte-identical (0 differing bytes); only its base moves. `.plt`/`.got` are
byte-identical. `.dynsym` symbol order is unchanged (same 5578 names in the same
order); its bytes differ only in `st_value` fields. `.data.rel.ro` (188 bytes)
and `.data` (75 bytes) differ only in relocated function pointers.

```
python3 /tmp/w3687/dynsym_names.py ../_work/bins/android_2.0.16_libgof2hdaa.so \
  /tmp/w3687/base.so /tmp/w3687/noorder.so
# base.so same order
# noorder.so same order
```

`.text` differing bytes rise from 114,696 (baseline) to 1,130,234 (no order);
`.ARM.exidx` differing bytes rise from 2,707 to 14,438.

## 4. Root cause: intra-TU order, not inter-TU interleaving

The committed w3613 probe still gives the same picture at this HEAD:

```
python3 _work/reconstructions/w3613/tu_interleave.py
objects linked: 193
objects with exported funcs (all): 193; range contains another object's func: 141
objects with strong exported funcs: 191; range contains another object's strong func: 1
  STRONG NdkHooks.cpp.obj 0x93c88-0x94e38 foreign 1
    0x947f0 NdkAutosave.cpp.obj pConstToNonConst
```

So the TU **link order** is essentially right; only one strong interleaving
remains. The problem is that each object's own section order is not the
original's address order. Two independent probes:

1. **Object section order vs original address order** (strong exported functions
   only): **94 of 191** objects differ, **3764 inverted pairs** over 3840 mapped
   functions. Counting all exported functions (including weak inline copies):
   **156 of 193** objects differ, **16061 inverted pairs** over 5031 mapped
   functions.

2. **Object section order vs the ordering file's subsequence for that object**:
   **161 of 193** objects differ (this includes local functions). Every
   `.text.*` section is named in the ordering file (0 unlisted), so the file is
   not compensating for missing sections; it is reordering them.

Representative first divergences (object order -> ordering-file/original order):

```
VirtualInput.cpp:   SendStoredKeyUpEvents ... keyEventReleased ...
                    -> keyEventReleased ... SendStoredKeyUpEvents
GLES1Compat.cpp:    glMatrixMode, glLoadMatrixf, glLoadIdentity, glScalef ...
                    -> glShadeModel, glMatrixMode, ...
FileInterfaceAndroid.cpp: logi, loge, OpenAppend, ctors ...
                    -> logi, loge, ctors, OpenAppend ...
EaseInOutMatrix.cpp: ctor, SetRange, operator-, SetDuration ...
                    -> ctor, SetRange, SetDuration, ...
String.cpp:         Reverse, operator[], ctor, Set ...
                    -> Reverse, operator[], ctor, Set, Set(char) ...
```

This is exactly the kind of ordering that `-ffunction-sections` + a plain
input-order link would preserve from the original source; our reconstructed
source files have not yet converged to the original function order.

## 5. Targeted experiment: fix the first TU only

To prove the first mismatch is causal, `SendStoredKeyUpEvents` was moved in
`src/engine/input/VirtualInput.cpp` from before `keyPressed` to after
`keyReleased` (matching the original address order), then the tree was rebuilt
and re-measured. The edit was reverted afterwards.

| variant | byte_exact | linked_exact | exact-address prefix | first mismatch |
|---|---:|---:|---:|---|
| baseline | 4496 | 4496 | 47 | `keyEventReleased @ 0x6a41c` |
| VirtualInput reorder, ordering file on | **4491** | 4496 | — | — |
| VirtualInput reorder, ordering file off | **1525** | 4496 | **134** | `glShadeModel @ 0x6dac0` |

The reorder extends the exact-address prefix 47 -> 134 and moves the first
mismatch to the next divergent TU (`GLES1Compat.cpp`), which is the expected
causal behaviour. But it **regresses** the ordering-file build by 5 byte_exact
(4496 -> 4491): moving the function also changes the object's local-section
placement (the ordering file places locals relative to their object order), so
the current source order is load-bearing for those local sections. A removal
campaign must therefore reorder each TU **and** keep the full gate green; it is
not enough to sort exported functions by address.

## 6. Full no-layout rebuild

For completeness, a scratch build with `GOF2_MATCH_LAYOUT=OFF` (no ordering
file, no `-ffunction-sections`, no exidx-marker renaming):

```
cmake -S . -B /tmp/w3687/build-nolayout -G Ninja -DGOF2_MATCH_LAYOUT=OFF \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/wt/w3687/../_work/bins/android_2.0.16_libgof2hdaa.so
touch /tmp/w3687/build-nolayout/section_order.txt   # OFF path still lists it in DEPENDS
cmake --build /tmp/w3687/build-nolayout --target link_so -j8
```

Result: **byte 1345 / linked 4355**, `.text 0x133178 @ 0x69240`,
`.ARM.exidx 0x64b8`, `.ARM.extab 0x25570`, `.rodata 0x41b67 @ 0x1c7de0`.
This is worse than the relink-only removal (1527 / 4496) because dropping
`-ffunction-sections` also changes function sizes/aliases. It is not the
recommended removal path.

## 7. Proven vs hypothesis

**Proven by measurement:**

* Removing the ordering file at this HEAD costs **2969 byte_exact**
  (4496 -> 1527) with `linked_exact` unchanged at 4496 and `.text` size
  unchanged at `0x1337c8`.
* Only 47 exported functions keep their original address; the first mismatch is
  `keyEventReleased @ 0x6a41c`, caused by `VirtualInput.cpp` source order.
* The original `.text` is grouped by original TU and our object link order is
  already the original TU order; strong inter-TU interleaving is 1/191 objects.
* 94/191 objects have strong exported-function order different from the
  original address order (156/193 counting weak copies; 161/193 including
  local sections).
* Fixing the first TU extends the exact-address prefix 47 -> 134 and moves the
  first mismatch to `GLES1Compat.cpp`, but regresses the ordering-file build by
  5 byte_exact (4496 -> 4491).
* `linked_exact` is insensitive to the ordering file; the flag is purely a
  placement aid.

**Hypothesis (not proven here):**

* If every TU's function emission order is converged to the original (and the
  last `NdkHooks`/`NdkAutosave` interleaving is removed), the ordering file can
  be dropped and a plain input-order link will reproduce the original `.text`.
  This follows from the original's perfect per-TU grouping and the existing
  original-TU link order, but it requires a per-TU source-order campaign under
  the full gate; the VirtualInput experiment shows that a single reorder is not
  gate-neutral by itself.
* The `.ARM.exidx` `-0x40` and `.rodata` base `-0x40` in the no-order link are
  consequences of gold's default empty-`.text`/marker placement, not of the
  function order itself. They would need to be re-checked after source-order
  convergence.

## Appendix A — relink helper

```python
#!/usr/bin/env python3
# usage: relink.py OUT [--no-order|--null-order]
import subprocess, sys, shlex
cmd = open('/tmp/w3687_linkcmd.txt').read().strip().splitlines()[-1]
cd, link, strip = cmd.split(' && ')
cd = cd[3:]
args = shlex.split(link)
out = sys.argv[1]
i = args.index('-o'); args[i+1] = out
extra = sys.argv[2:]
if '--no-order' in extra:
    extra.remove('--no-order')
    args = [a for a in args if not a.startswith('--section-ordering-file=')]
elif '--null-order' in extra:
    extra.remove('--null-order')
    args = [a for a in args if not a.startswith('--section-ordering-file=')]
    extra.append('--section-ordering-file=/dev/null')
i = args.index('-o'); args[i:i] = extra
subprocess.run(args, cwd=cd, check=True)
sargs = shlex.split(strip); sargs[-1] = out
subprocess.run(sargs, check=True)
```

`/tmp/w3687_linkcmd.txt` is produced by:

```
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -2 > /tmp/w3687_linkcmd.txt
```

`relink_plain.py` is the same script with the marker inputs replaced by the
plain object list and the original archives:

```python
repl = {
  '.../cmake-build-match/exidx_markers/link_order.rsp': '.../cmake-build-match/link_order.rsp',
  '.../cmake-build-match/exidx_markers/archives/libz_gof2.a': '.../third_party/zlib/libz_gof2.a',
  '.../cmake-build-match/exidx_markers/archives/libzip_gof2.a': '.../third_party/libzip/libzip_gof2.a',
  '.../cmake-build-match/exidx_markers/archives/libcrypto_gof2.a': '.../third_party/openssl/libcrypto_gof2.a',
  '.../cmake-build-match/exidx_markers/archives/libc++abi.a': '.../.cache/ndk/android-ndk-r18b/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libc++abi.a',
  '.../cmake-build-match/exidx_markers/archives/libandroid_support.a': '.../.cache/ndk/android-ndk-r18b/sources/cxx-stl/llvm-libc++/libs/armeabi-v7a/libandroid_support.a',
  '.../cmake-build-match/exidx_markers/archives/libgcc.a': '.../.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/lib/gcc/arm-linux-androideabi/4.9.x/armv7-a/thumb/libgcc.a',
}
```

`relink_custom.py` is the same script but removes the existing
`--section-ordering-file=` argument and inserts
`--section-ordering-file=<path>` instead.

## Appendix B — address-match probe

```python
#!/usr/bin/env python3
# usage: addr_match.py ORIG.so OURS.so [...]
import subprocess, sys
NM = ('/home/debian/GalaxyOnFire2/wt/w3687/.cache/ndk/android-ndk-r18b/'
      'toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/'
      'arm-linux-androideabi-nm')
def syms(path):
    out = {}
    r = subprocess.run([NM, '-nD', '--defined-only', path],
                       capture_output=True, text=True)
    for ln in r.stdout.splitlines():
        p = ln.split(maxsplit=2)
        if len(p) == 3 and p[1] in 'tTwW':
            out[p[2]] = int(p[0], 16) & ~1
    return out
orig = syms(sys.argv[1])
for path in sys.argv[2:]:
    ours = syms(path)
    common = set(orig) & set(ours)
    same = sum(1 for n in common if orig[n] == ours[n])
    print(f'{path}: {same}/{len(common)} common exported funcs at original address')
```

## Appendix C — intra-TU order probe

```python
#!/usr/bin/env python3
# Counts objects whose object-section order differs from the original address
# order. Run in the repo root after a build.
import struct, subprocess, os
ROOT = '/home/debian/GalaxyOnFire2/wt/w3687'
NM = (ROOT + '/.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/'
      'prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm')

def orig_syms():
    out = {}
    r = subprocess.run([NM, '-nD', '--defined-only',
                        ROOT + '/../_work/bins/android_2.0.16_libgof2hdaa.so'],
                       capture_output=True, text=True)
    for ln in r.stdout.splitlines():
        p = ln.split(maxsplit=2)
        if len(p) == 3 and p[1] in 'tTwW':
            out[p[2]] = (int(p[0], 16) & ~1, p[1])
    return out

def obj_funcs(path, strong_only):
    data = open(path, 'rb').read()
    if data[:4] != b'\x7fELF':
        return []
    shoff, = struct.unpack_from('<I', data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', data, 0x2e)
    hdrs = [struct.unpack_from('<IIIIIIIIII', data, shoff + i * shentsize)
            for i in range(shnum)]
    funcs = []
    for h in hdrs:
        name, typ, flags, addr, off, size, link, info, align, entsize = h
        if typ != 2:
            continue
        entsize = entsize or 16
        strtab_off = hdrs[link][4]
        for o in range(off, off + size, entsize):
            st_name, _val, _sz, st_info, _other, shndx = struct.unpack_from(
                '<IIIBBH', data, o)
            if shndx == 0 or st_name == 0:
                continue
            if (st_info & 0xf) != 2:          # STT_FUNC
                continue
            if strong_only and (st_info >> 4) != 1:   # STB_GLOBAL
                continue
            end = data.index(b'\0', strtab_off + st_name)
            funcs.append((shndx, data[strtab_off + st_name:end].decode()))
    funcs.sort(key=lambda x: x[0])
    return funcs

orig = orig_syms()
objs = [ln.strip() for ln in open(ROOT + '/cmake-build-match/link_order.rsp')
        if ln.strip()]
for strong_only in (False, True):
    tot = inv_obj = inv_pairs = mapped = 0
    for path in objs:
        seq = []
        for shndx, nm in obj_funcs(path, strong_only):
            if nm in orig:
                a, t = orig[nm]
                if strong_only and t not in 'T':
                    continue
                seq.append((a, nm))
        if not seq:
            continue
        tot += 1
        mapped += len(seq)
        inv = sum(1 for i in range(len(seq)) for j in range(i + 1, len(seq))
                  if seq[i][0] > seq[j][0])
        if inv:
            inv_obj += 1
            inv_pairs += inv
    print(f'strong_only={strong_only}: objects {tot}, '
          f'objects with order mismatch {inv_obj}, '
          f'inverted pairs {inv_pairs}, mapped {mapped}')
```

## Appendix D — ordering-file subsequence probe

```python
#!/usr/bin/env python3
# Compares each object's .text.* section order with the order in which the
# generated section_order.txt lists those same sections (markers excluded).
import struct, os
ROOT = '/home/debian/GalaxyOnFire2/wt/w3687'

def text_sections(path):
    data = open(path, 'rb').read()
    if data[:4] != b'\x7fELF':
        return []
    shoff, = struct.unpack_from('<I', data, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', data, 0x2e)
    hdrs = [struct.unpack_from('<IIIIIIIIII', data, shoff + i * shentsize)
            for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    out = []
    for h in hdrs:
        name, typ, flags, addr, off, size, link, info, align, entsize = h
        end = data.index(b'\0', stroff + name)
        n = data[stroff + name:end].decode()
        if typ == 1 and (flags & 0x4) and n.startswith('.text.') \
                and not n.startswith('.text.tuend.'):
            out.append(n)
    return out

order = [ln.strip() for ln in open(ROOT + '/cmake-build-match/section_order.txt')
         if ln.strip()]
pos = {n: i for i, n in enumerate(order)}
objs = [ln.strip() for ln in
        open(ROOT + '/cmake-build-match/exidx_markers/link_order.rsp')
        if ln.strip()]
tot = diff = 0
for path in objs:
    secs = text_sections(path)
    if not secs:
        continue
    tot += 1
    listed = sorted((pos[s], s) for s in secs if s in pos)
    objseq = [s for s in secs if s in pos]
    fileseq = [s for _, s in listed]
    if objseq != fileseq:
        diff += 1
print(f'objects {tot}, objects whose section_order subsequence differs {diff}')
```
