# `-ffunction-sections` proof and section-ordering-file removal test (w3561)

Status: research note, 2026-11-01. Worktree `wt/w3561`, branch `wave87-w3561`, based on
master `113737ee`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b clang 7.0.2 /
gold 1.12. The worktree was not modified; the only source edit was in a scratch copy
under `/tmp/wt-nofs` (see §1.3).

## TL;DR

1. **The original was compiled with `-ffunction-sections`.** Direct binary fingerprint: the
   original `.ARM.exidx` has **193 CANTUNWIND entries that share their target address with a
   real (non-CANT) entry**. That is exactly what gold's `fix_exidx_coverage()` inserts for a
   text input section with no `.ARM.exidx` companion, and `-ffunction-sections` is what leaves
   such a section (the empty plain `.text`) in every object. A controlled full-tree rebuild
   with the flag removed produces only **52** such pairs (and a different `.text` size); the
   current tree produces **193**, matching the original address-for-address.
2. **The section-ordering file is load-bearing for the current tree, and is not an original
   build flag.** Removing it (omitting the flag, as ndk-build would) drops `byte_exact`
   **4496 -> 1527** (`linked_exact` stays 4496); only **293 / 4524** exported functions keep
   their original address. The original itself was linked without one: its `.text` is
   perfectly grouped by original TU (2231 mapped functions, 105 TUs, **104 transitions,
   0 interleaving**). The file is needed only because our TU membership diverges: **144 of
   195** objects have their exported functions interleaved with other objects in the original
   `.text`. It can be removed only by converging TU membership (the AGENTS policy), not by a
   linker trick.

Baseline (unchanged): `byte_exact 4496`, `linked_exact 4496`, `.text 0x1337c8`,
`.ARM.exidx 0x6f70`, lint `CLEAN`, drift `0`, `RATCHET PASS`.

## 1. Why `-ffunction-sections` leaves an observable in the linked `.so`

### 1.1 The mechanism (controlled tiny compile)

With clang 7.0.2 at `-Oz`, `-ffunction-sections` emits, per object:

* one **empty** plain `.text` (`size 0`),
* one `.text.<mangled>` per function,
* one `.ARM.exidx.text.<mangled>` per function.

Without the flag, the same TU has one non-empty `.text` and one `.ARM.exidx` covering it.

```
$ cat > /tmp/fs/t2.cpp <<'X'
void a() {}
int b(int x) { return x * 3 + 1; }
void c() {}
int d(int x) { return x + 2; }
X
$ CC=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
$ RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$ $CC -target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
    -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
    -c /tmp/fs/t2.cpp -o /tmp/fs/t2_no.o
$ $CC ...same... -ffunction-sections -funwind-tables -c /tmp/fs/t2.cpp -o /tmp/fs/t2_yes.o
$ $RE -SW /tmp/fs/t2_no.o  | grep -E '\.text|exidx'
  [ 2] .text             PROGBITS  00000000 000034 000010 00 AX 0 0 4
  [ 3] .ARM.exidx        ARM_EXIDX 00000000 000044 000020 00 AL 2 0 4
$ $RE -SW /tmp/fs/t2_yes.o | grep -E '\.text|exidx'
  [ 2] .text             PROGBITS  00000000 000034 000000 00 AX 0 0 4
  [ 3] .text._Z1av       PROGBITS  00000000 000034 000002 00 AX 0 0 2
  [ 4] .ARM.exidx.text._Z1av ARM_EXIDX ...
  ... (one .text.* + one .ARM.exidx.text.* per function)
```

The empty `.text` has no `.ARM.exidx` companion. Gold 1.12's `fix_exidx_coverage()`
therefore inserts a CANTUNWIND entry for it. Gold places that empty `.text` **after the
object's own `.text.*` sections** (gold's default `.text` ordering), i.e. at the object's
end = the next object's first-function address. This is visible with two objects:

```
$ cat > /tmp/fs/a.cpp <<'X'
extern int ext(int); int fa(int x){ return ext(x)+1; }
X
$ cat > /tmp/fs/b.cpp <<'X'
extern int ext(int); int fb(int x){ return ext(x)+2; }
X
$ $CC ... -ffunction-sections -funwind-tables -c /tmp/fs/a.cpp -o /tmp/fs/a.o
$ $CC ... -ffunction-sections -funwind-tables -c /tmp/fs/b.cpp -o /tmp/fs/b.o
$ $GOLD -shared -o /tmp/fs/ab.so /tmp/fs/a.o /tmp/fs/b.o
$ $OD -d -j .text /tmp/fs/ab.so
  0000020c <_Z2fai>: ...        # object a
  00000218 <_Z2fbi>: ...        # object b
$ $OD -s -j .ARM.exidx /tmp/fs/ab.so
  0224 e8ffff7f 08849780   # real entry @ 0x20c (fa)
  022c ecffff7f 01000000   # CANT sentinel @ 0x218 (end of fa = start of fb)
  0234 e4ffff7f 08849780   # real entry @ 0x218 (fb)
  023c e8ffff7f 01000000   # CANT sentinel @ 0x224 (end of fb)
```

So the linked `.so` carries **one CANTUNWIND sentinel per object** at the object boundary,
in addition to the real per-function entries. This is the fingerprint we look for in the
original.

### 1.2 The original's fingerprint

Probe (full script in Appendix A) parses `.ARM.exidx`, decodes the `prel31` target of every
entry, and counts entries whose second word is `1` (CANTUNWIND) and whose target is also the
target of a non-CANT entry:

```
$ python3 /tmp/fs/exidx_probe.py ../_work/bins/android_2.0.16_libgof2hdaa.so
../_work/bins/android_2.0.16_libgof2hdaa.so
  .ARM.exidx 0x19cc08 size 0x6f70 entries 3566
  CANTUNWIND 272
  duplicate-target addresses 194  (extra entries 194)
  CANT+real pairs 193
  real+real pairs 1
```

The 193 CANT+real pairs are the sentinels. 74 of the 193 targets are exported function
starts; all 74 are the **first function of their original TU** (checked against
`_work/original_layout/original_layout.json`, HD dump). The other 119 targets are local
functions; they sit at the same TU boundaries as the current build's markers (the addresses
match exactly). The 79 CANT entries without a duplicate are either genuine cantunwind
functions (their own entry) or sentinels at a boundary whose neighbour's entry was merged
away; either way they are not part of the sentinel fingerprint. Only the 193 CANT+real
pairs are used as evidence.

The same probe on the current build gives **exactly the same 272 CANT targets and the same
193 CANT+real pairs, address for address**:

```
$ python3 /tmp/fs/exidx_probe.py cmake-build-match/libgof2hdaa.so
  .ARM.exidx 0x19cc08 size 0x6f70 entries 3566
  CANTUNWIND 272
  duplicate-target addresses 194  (extra entries 194)
  CANT+real pairs 193
```

The 194th duplicate is a real+real pair at `0x6db20` in both files.

### 1.3 Controlled full-tree A/B: remove `-ffunction-sections`

Scratch copy of the tree (so the worktree stays clean), same NDK, same link order, same
link flags; only `-ffunction-sections` removed from `target_compile_options`:

```
$ rsync -a --exclude=.git --exclude=cmake-build-match --exclude=report.json \
    /home/debian/GalaxyOnFire2/wt/w3561/ /tmp/wt-nofs/
$ ln -sfn /home/debian/GalaxyOnFire2/gof2-decomp/.cache /tmp/wt-nofs/.cache
$ cd /tmp/wt-nofs
$ sed -i 's/-ffunction-sections -funwind-tables/-funwind-tables/' CMakeLists.txt
$ cmake -S . -B build-nofs -G Ninja \
    -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/wt/_work/bins/android_2.0.16_libgof2hdaa.so \
    -DGOF2_MATCH_OPT=-Oz
$ cmake --build build-nofs --target link_so -j8
$ python3 /tmp/fs/exidx_probe.py build-nofs/libgof2hdaa.so
  .ARM.exidx 0x19c394 size 0x67d0 entries 3322
  CANTUNWIND 83
  duplicate-target addresses 53  (extra entries 53)
  CANT+real pairs 52
```

Measured table (same 4524 compared functions):

| build | `-ffunction-sections` | `.text` | `.ARM.exidx` | CANT | CANT+real pairs | byte_exact | linked_exact |
|---|---:|---:|---:|---:|---:|---:|---:|
| original | — | `0x1337c8` | `0x6f70` | 272 | 193 | — | — |
| current tree | yes | `0x1337c8` | `0x6f70` | 272 | **193** | **4496** | **4496** |
| scratch tree | no | `0x133154` | `0x67d0` | 83 | **52** | 1345 | 4355 |

Caveat: without `-ffunction-sections` the section-ordering file can only order whole
per-TU `.text` sections, not individual functions, so the `byte_exact`/`linked_exact` drop
in the last row is not a clean measure of the flag alone. The **exidx fingerprint is the
clean observable**: 193 sentinels vs 52, and the 272 CANT targets of the current build are
identical to the original's while the no-flag build's 83 targets are a different set.

**Conclusion (proven):** the original `libgof2hdaa.so` was compiled with
`-ffunction-sections`. The 193 empty-`.text` sentinels in its `.ARM.exidx` cannot arise from
a single non-empty `.text` + `.ARM.exidx` per TU; they match the current `-ffunction-sections`
build exactly and the no-flag build does not.

## 2. Section-ordering-file removal test

The ordering file is generated by `tools/gen_section_order.py` from the original's function
addresses (plus per-object local placement). The test relinks the **existing objects** with
the same gold command, changing only the `--section-ordering-file` argument. The relink
script is a one-line substitution of the `libgof2hdaa.so` rule in
`cmake-build-match/build.ninja` (Appendix B). A relink with the unchanged command reproduces
`cmake-build-match/libgof2hdaa.so` byte-for-byte (`cmp` identical).

### 2.1 Baseline

```
$ cmake --build cmake-build-match --target check -j8
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`readelf -SW`: `.text 0x1337c8 @ 0x69440`, `.ARM.exidx 0x6f70 @ 0x19cc08`,
`.ARM.extab 0x26290 @ 0x1a3b78`, `.rodata 0x41b67 @ 0x1c9e10`. Exported functions at their
original address: **4524 / 4524**.

### 2.2 Remove the flag entirely (the faithful ndk-build case)

```
$ python3 /tmp/fs/relink.py /tmp/fs/noorder.so --no-order
$ python3 tools/verify/verify.py --build-dir /tmp/fs/verify_noorder \
    --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours /tmp/fs/noorder.so \
    --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
    --fail-on-wrong-type --top 0
compared 4524  avg 99.9%  byte_exact 1527  linked_exact 4496  fuzzy_perfect 4496
```

| variant | byte_exact | linked_exact | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata` base | exported funcs at original address |
|---|---:|---:|---:|---:|---:|---:|---:|
| baseline (ordering file) | 4496 | 4496 | `0x1337c8` | `0x6f70` | `0x26290` | `0x1c9e10` | 4524 / 4524 |
| omit `--section-ordering-file` | **1527** | 4496 | `0x1337c8` | `0x6f38` | `0x26290` | `0x1c9dd0` | **293 / 4524** |
| `--section-ordering-file=/dev/null` | 1408 | 4496 | `0x1337c8` | `0x6f38` | `0x26290` | `0x1c9dd0` | 100 / 4524 |
| plain objects, omit flag | 1527 | 4496 | `0x1337c8` | `0x6f60` | `0x26290` | `0x1c9e00` | 293 / 4524 |

Notes:

* **Omitting** the flag is the faithful test (ndk-build passes no ordering file). Passing
  an **empty** file is a different experiment: the file itself disables gold's default
  `.text.startup`-first sort, so it scores lower (1408, 100 addresses).
* The "plain objects" row links the original object copies (`link_order.rsp`) instead of
  the `exidx_markers/` copies, i.e. it also removes the empty-`.text` marker renaming.
  `.text` size is still exactly `0x1337c8`; only the exidx sentinel placement differs
  (`0x6f60` vs `0x6f38` vs original `0x6f70`).
* `linked_exact` is unchanged in every row: the ordering file only changes **placement**,
  not function sizes or call targets. `byte_exact` is the metric that sees it.

### 2.3 Was the original linked without an ordering file?

Yes, by two independent witnesses:

1. **Per-TU grouping.** Map every exported function in the original to its original TU via
   `_work/original_layout/original_layout.json` (`gof2hd_1.1.19_ios` symbol lists). In
   address order, 2231 functions map to 105 TUs with **104 TU transitions and 0 TUs whose
   address range contains another TU's function**. The original `.text` is perfectly grouped
   by original TU — the signature of a plain input-order link.
2. **Plain no-order link shape.** Relinking our objects with no ordering file gives
   `.text 0x1337c8` (exact) and a sentinel count/fingerprint close to the original
   (277 CANT / 197 CANT+real pairs vs 272 / 193), whereas the ordering-file link is exact.
   The residual 5-CANT / 4-pair difference is our TU-membership divergence, not a link flag.

### 2.4 Why the file is still needed: our TU membership diverges

The link order (`link_order.rsp`) is already the original TU order (derived from the
original's `.dynsym`), but our objects do not correspond 1:1 to original TUs. For each of
our 195 objects with exported functions, take the min/max original address of its exported
functions and check whether another object's function lies inside that range:

```
objects with exported functions: 195
objects whose original address range contains another object: 144
objects with no foreign function inside their range: 51
```

144 / 195 objects are split across / interleaved with original TUs (e.g. our
`Transform.cpp`, `PaintCanvas.cpp`, `Matrix.cpp`, `ApplicationManager.cpp` each span dozens
of original TUs). A plain input-order link therefore cannot reproduce the original `.text`
with the current TU layout. The ordering file is the reconstruction aid that compensates.

**Conclusion (proven):** removing the ordering file costs **2969 byte_exact**
(4496 -> 1527) with `linked_exact` unchanged and `.text` size unchanged. It is not an
original build flag; it is required only because our TU membership is not yet the original's.
The removal path is the AGENTS TU-convergence work (dismantle `AbyssEngine.cpp`, fold the
split/merged TUs), not a linker change.

## 3. Proven vs hypothesis

**Proven by measurement:**

* The original was compiled with `-ffunction-sections` (193 empty-`.text` CANTUNWIND
  sentinels at TU boundaries; current build matches all 272 CANT targets address-for-address;
  no-flag build produces 52 sentinels and a different set).
* The original `.text` is grouped per original TU (105 TUs, 104 transitions, 0 interleaving).
* The current ordering file is worth +2969 `byte_exact`; removing it leaves 293/4524
  exported functions at their original address.
* `linked_exact` is insensitive to the ordering file (4496 in all variants).

**Hypothesis (not proven here):**

* With exact original TU membership, the ordering file could be dropped and a plain
  input-order link would reproduce the original `.text`. This follows from the perfect
  per-TU grouping plus the fact that the link order is already the original TU order, but it
  cannot be tested until the TU merges/splits are done.
* The 5 extra CANT entries / 4 extra CANT+real pairs in the plain no-order relink
  (277/197 vs 272/193) are due to our extra / invented TUs and archive-member placement;
  this is consistent with the TU divergence above but was not individually attributed.

## Appendix A — `.ARM.exidx` probe

```python
#!/usr/bin/env python3
# usage: exidx_probe.py FILE.so [...]
import struct, sys
from collections import defaultdict

def sections(path):
    d = open(path, 'rb').read()
    shoff, = struct.unpack_from('<I', d, 0x20)
    shentsize, shnum, shstrndx = struct.unpack_from('<HHH', d, 0x2e)
    hdrs = [struct.unpack_from('<IIIIIIIIII', d, shoff + i * shentsize)
            for i in range(shnum)]
    stroff = hdrs[shstrndx][4]
    out = {}
    for h in hdrs:
        name, typ, flags, addr, off, size, link, info, align, ent = h
        end = d.index(b'\0', stroff + name)
        out[d[stroff + name:end].decode()] = (addr, off, size)
    return d, out

def prel31(w, addr):
    v = w & 0x7fffffff
    if v & 0x40000000:
        v -= 0x80000000
    return (addr + v) & 0xffffffff

for path in sys.argv[1:]:
    d, sec = sections(path)
    addr, off, size = sec['.ARM.exidx']
    ents = []
    for i in range(size // 8):
        ea = addr + i * 8
        w0, w1 = struct.unpack_from('<II', d, off + i * 8)
        ents.append((prel31(w0, ea), w1))
    byt = defaultdict(list)
    for t, w1 in ents:
        byt[t].append(w1)
    cant = [t for t, w1 in ents if w1 == 1]
    dup = [t for t, n in byt.items() if len(n) > 1]
    cant_real = [t for t in dup if 1 in byt[t] and any(w != 1 for w in byt[t])]
    real_real = [t for t in dup if 1 not in byt[t]]
    print(path)
    print(f'  .ARM.exidx {addr:#x} size {size:#x} entries {len(ents)}')
    print(f'  CANTUNWIND {len(cant)}')
    print(f'  duplicate-target addresses {len(dup)}  (extra entries {sum(len(v)-1 for v in byt.values())})')
    print(f'  CANT+real pairs {len(cant_real)}')
    print(f'  real+real pairs {len(real_real)}')
```

## Appendix B — relink helper

`/tmp/fs/relink.py` takes the exact `libgof2hdaa.so` `COMMAND` from
`ninja -C cmake-build-match -t commands libgof2hdaa.so`, substitutes the output path and
optionally the ordering-file argument, runs gold, then strips with the same `strip` command.
The unchanged command reproduces `cmake-build-match/libgof2hdaa.so` byte-for-byte.

```python
import subprocess, sys, shlex
cmd = open('/tmp/fs/linkcmd.txt').read().strip()   # last line of ninja -t commands
cd, link, strip = cmd.split(' && ')
cd = cd[3:]
args = shlex.split(link)
out = sys.argv[1]; extra = sys.argv[2:]
i = args.index('-o'); args[i + 1] = out
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

Function-address comparison (used for the "exported funcs at original address" column):

```python
# nm -nD --defined-only on both files; compare st_value & ~1 by symbol name.
```

## Appendix C — TU grouping / object-contiguity probe

```python
# Map exported functions to original TUs:
#   sym2tu = {dump_symbol[1:]: tu for tu, info in
#             json.load(open('_work/original_layout/original_layout.json'))
#                 ['tu_symbols']['gof2hd_1.1.19_ios'].items()
#             for dump_symbol in info['symbols']}
# nm -nDC --defined-only on the original gives (addr, demangled_name).
# Group by TU; count address-order TU transitions and range interleaving.
#
# Object contiguity: parse each .obj's .symtab STT_FUNC names, look up their original
# addresses, and test whether another object's function lies inside the object's [min,max].
```
