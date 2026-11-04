# Layout::Layout C1/C2 — float-typed arms restore the `vcvt` round-trip
# (w5502, wave 107)

Worker **w5502**, branch `wave107-w5502`, base `f857b5171`.
Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` at ELF VMA `0xd1028`, original size
**7072 B**. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

Read alongside the wave-106 notes
`layout-layout-c1-c2-wave106-w5401.md` and
`layout-layout-c1-c2-wave106-w5402.md` (the mid-block order `0x9c, 0xac,
0x98, 0xa8` is the unique 7072 B permutation, and the original's `0xa8`
store is an int-phi round trip).

## Result

**Landed a real, size-exact fuzzy improvement.** The ctor moves from
**7072 B / 47.848605577689 %** to **7072 B / 48.000000000000 %**
(+0.1514 pp, unrounded `difflib` ratio, `autojunk=False`, same
normalization as `tools/verify/verify.py`). Both `_ZN6LayoutC1Ev` and
`_ZN6LayoutC2Ev` measure **48.000000000000 %**.

Gate before == after (`cmake --build cmake-build-match --target check -j6`):
`RATCHET PASS`, `byte_exact` **4500/4524**, `linked_exact` **4500/4524**,
`size_exact` 4524/4524, `data_exact` 843/843, imports 207, lint `CLEAN ✓`,
drift 0, symbol parity unchanged. `.text` size and every section base are
unchanged; the ELF still has 28 sections and `.ARM.extab` is still
`0x26294` vs the original `0x26298` (the pre-existing wave-106 delta). No
extab or section change is introduced by this pass.

## The five edits (all in `src/game/ui/Layout.cpp`, ctor only)

The improvement is five expression-level edits. They make clang 7.0.2 `-Oz`
materialize the affected constants through the float pipeline, which shifts
the literal pool and the surrounding register allocation toward the
original's `vldr/vmov → vcvt.s32.f32 → vstr` shape.

| # | field | before | after |
|---|---|---|---|
| 1 | `field_0xa8` | `iPadHD ? 61 * 1.40625f : (iPadLarge ? 122 : (n9 ? 61 : (iPad ? 61 : (retina ? 82 : 41))))` | every int arm is a float literal (`61.0f * 1.40625f`, `122.0f`, `61.0f`, `61.0f`, `82.0f`, `41.0f`) |
| 2 | `field_0x150` | `(int)(float)(double)(…)` | `(int)(float)(…)` (drop the `double`) |
| 3 | `field_0x160` | `49 * 1.40625f` / int arms | all arms float literals (`49.0f * 1.40625f`, `98.0f`, `49.0f`, `13.0f`, `26.0f`, `13.0f`) |
| 4 | `field_0x1a4` | `(int)(float)((int)(float)(22 : 28 : 32 : 16))` | inner arms float literals (`22.0f`, `28.0f`, `32.0f`, `16.0f`) |
| 5 | `field_0x188` | `23 : 30 : 34 : 17` | all arms float literals (`23.0f`, `30.0f`, `34.0f`, `17.0f`) |

All five are ordinary C++ value expressions — no casts beyond the ones
already in the source, no attributes, no globals, no padding, no asm.

## Measured table (real `link_so` builds, unrounded linked score)

Each row is a full `cmake --build cmake-build-match --target link_so` followed
by the unrounded `difflib` score of `_ZN6LayoutC1Ev`.

| variant | size | % |
|---|---:|---:|
| baseline (`Layout.base.cpp`) | 7072 | 47.848605577689 |
| `0xa8` float arms only | 7048 | 48.019112084412 |
| `0xa8`+`0x160` | 7064 | 47.969745222930 |
| `0xa8`+`0x160`+`0x166` | 7064 | 47.850318471338 |
| `0xa8`+`0x160`+`0x166`+`0x1a4` | 7072 | 47.969745222930 |
| **`0xa8`+`0x160`+`0x166`+`0x1a4`+`0x150`-nodouble (landed)** | **7072** | **48.000000000000** |

Single-edit sizes (all real builds): `0x160` alone 7096 / 47.7214,
`0x166` alone 7080 / 47.8200, `0x1a4` alone 7080 / 47.7992,
`0x150`-nodouble alone 7072 / 47.8486 (score-neutral), `0x177` alone
7080 / 47.7101, `0x70_rowHeight` alone 7088 / 47.5318. Because most single
edits change the size, the 7072 B landing point is a combination effect:
the four float edits plus the `double` removal happen to balance back to
exactly 7072 B.

Exhaustive subset search over the nine candidate lines
(`a8, 160, 166, 151, 177, 283, 125, 138, 28`; 511 combinations) found no
size-exact variant above 47.9697 % before the `0x150`-nodouble edit; adding
that edit is what crosses 48.0 %. A further single-edit scan from the landed
body (every ctor assignment, `floatify` / `nocast` / `nodouble` / `hdint`)
found no size-exact variant above 48.0 %.

## Proven vs hypothesis

**Proven (binary / measured):**

* The landed body is **size-exact at 7072 B** and both C1/C2 score
  **48.000000000000 %**; the full gate is `RATCHET PASS` with
  `byte_exact = linked_exact = 4500`.
* The original's `0xa8` store is the int-phi round trip
  `vmov s0,rN; str rN,[r0,#0xac]; vcvt.f32.s32; vcvt.s32.f32;
  vstr s0,[r0,#0xa8]` (Thumb `0xd19ca..0xd19de`), i.e. the original source
  shares one int between `0xac` and `0xa8`.
* Ghidra/REA confirms the non-HD expression is
  `iPadLarge ? 122 : ((n9 || iPad) ? 61 : (retina ? 82 : 41))` and the HD
  arm is `0x55` (85); the current flat ternary is semantically the same.
* The improvement is entirely from expression typing / literal-pool
  placement: no control-flow, field-offset, or call-order change.

**Hypothesis (not proven):**

* The float-literal form is a *metric* match, not necessarily the original
  spelling. The true original spelling is the shared-int form, which in the
  current flat body collapses the score to **18.28125 % / 7100 B**
  (`int v = …; field_0xac = v; field_0x98 = …; field_0xa8 = (int)(float)v;`)
  because clang hoists the shared computation and reshuffles the HD block.
  The flat body is a reconstruction choice inherited from wave 89/106; the
  original is a block tree (REA shows `if (iPadHD) { … } else { … }`).
* The remaining gap is dominated by vector-register allocation: the original
  keeps the `[0xc0,0xc4,0xc8,0xcc]` constant group in `d0-d1` (loaded first,
  stored later) and uses `d2-d3` for the nine 4-field store groups; ours uses
  `d0-d1` for the store groups. Matching that would require reproducing the
  original's HD-block statement order, which the wave-106 from-scratch block
  tree (6560 B / 30.42 %) does not achieve on this metric.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5502
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS
# unrounded per-function score (difflib, autojunk=False):
python3 /dev/shm/w5502/score1u.py _ZN6LayoutC1Ev            # 7072 B, 48.000000000000
python3 /dev/shm/w5502/score1u.py _ZN6LayoutC2Ev            # 7072 B, 48.000000000000
```

`score1u.py` is a thin wrapper over `tools/verify/verify.py`'s `Elf32`,
`Resolver`, `disassemble` and `reconcile` that prints the unrounded
`difflib.SequenceMatcher(None, orig, ours, autojunk=False).ratio() * 100`
instead of `verify.py`'s 1-decimal `score()`. It is scratch tooling; the
landed source and this note are self-contained.

## Appendix — unrounded scorer (`score1u.py`)

```python
import sys
sys.path.insert(0, '/home/debian/GalaxyOnFire2/wt/w5502/tools/verify')
import verify as V
ORIG = '/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so'
OURS = '/home/debian/GalaxyOnFire2/wt/w5502/cmake-build-match/libgof2hdaa.so'
OBJ = ('/home/debian/GalaxyOnFire2/wt/w5502/.cache/ndk/android-ndk-r18b/toolchains/'
       'arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump')
name = sys.argv[1] if len(sys.argv) > 1 else '_ZN6LayoutC1Ev'
of = V.Elf32(ORIG); oe = V.Elf32(OURS)
orig = of.functions(); ours = oe.functions()
common = set(orig) & set(ours)
def addr_range(elf):
    mapped = [s for s in elf.sections if s['addr']]
    lo = next((s['addr'] for s in mapped if s['name'] == '.plt'),
              min(s['addr'] for s in mapped))
    return lo, max(s['addr'] + s['size'] for s in mapped)
orig_res = V.Resolver(orig, common, of.plt_map(), addr_range(of))
ours_res = V.Resolver(ours, common, oe.plt_map(), addr_range(oe))
common_objects = {o[2] for o in of.objects()} & {o[2] for o in oe.objects()}
for res, e in ((orig_res, of), (ours_res, oe)):
    res.attach_data(e, common_objects)
imports = ({v for k, v in of.got_map().values() if k == 'sym'}
           & {v for k, v in oe.got_map().values() if k == 'sym'})
orig_res.imports = ours_res.imports = imports
o, u = orig[name], ours[name]
ob = of.bytes_at(o['value'], o['size']); ub = oe.bytes_at(u['value'], u['size'])
_, on = V.disassemble(OBJ, ob, o['thumb'], o['value'], orig_res)
_, un = V.disassemble(OBJ, ub, u['thumb'], u['value'], ours_res)
on, un = V.reconcile(on, un)
import difflib
pct = 100.0 if ob == ub else difflib.SequenceMatcher(None, on, un, autojunk=False).ratio() * 100
print(f"{name}: orig {o['size']}B ours {u['size']}B pct={pct:.12f} ninsn {len(on)}/{len(un)}")
```
