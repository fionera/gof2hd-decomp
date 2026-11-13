# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13567

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13567`, base master `c899221ef`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13545.md` (tail `rnd` live-range
hypothesis) and `...w13538.md` (case-183 convoy switch order).

```text
                                     size            raw    aligned  insn
base c899221ef                       17172/17172     2628    844     6206/6197
this pass (no new win)               17172/17172     2628    844     6206/6197
```

No size-exact raw win was found; the tree is left at `c899221ef`
(`git status --porcelain` empty).

## 1. CORRECTION: `[sp,#100]` at `0xb3234` is `&Globals::rnd`, not `total`

w13545 wrote that the dead load at `0xb3234` is the case-2 local `total`.
It is **not**: it is the spilled *address* of the global pointer
`Globals::rnd`.

Evidence from the PEI MIR dump (`-print-after=pei`), block `bb.226`
(`%ir-block.1751`, the `if (alienGuns != 0)` test):

```text
bb.226:
  renamable $r6 = t2LDRi12 $sp, 52   ; alienGuns
  t2CMPri killed renamable $r6, 0
  renamable $r8 = t2LDRi12 $sp, 124  ; this
  renamable $r4 = t2LDRi12 $sp, 100  ; &Globals::rnd
  t2Bcc %bb.251, 0                   ; -> if (pirateFortress)
  t2B   %bb.247                      ; -> alienGuns body
```

and the use, `bb.260` (inside `if (raid)`):

```text
bb.260:
  renamable $r0 = t2LDRi12 killed renamable $r4, 0 \
        :: (dereferenceable load 4 from @_ZN7Globals3rndE)
  tBL ... @_ZN11AbyssEngine8AERandom7nextIntEi
```

`[sp,#100]` is a *reused* stack slot (`%stack.79`): `total` (line 2290),
`clientRace` (line 2370) and `&Globals::rnd` all land there in disjoint
live ranges.  The `!"total"` DBG_VALUE at line 2290 is what misled w13545.

## 2. The real live range

`r4 = &Globals::rnd` is live from `bb.226` (`0xb322e`) to `bb.260`
(`0xb3788`) — straight through `bb.251` (`if (pirateFortress)`), `bb.534`,
`bb.256`, `bb.258` (`getRandomEnemyFighter` / `hardCoreMode` calls) and
`bb.259`.  It is reloaded again at `0xb3398` and `0xb34e6`, so the value is
split into three pieces.  The original reloads it once, inside the raid
block at `0xb3788`:

```text
b3788: ldr r0,[sp,#100]   ; &Globals::rnd
b378a: movs r1,#100
b378c: ldr r0,[r0,#0]     ; Globals::rnd
b378e: blx nextInt
```

Because `r4` is taken, `this` is forced into `r8` at `0xb3232` and the
whole `0xb322e..0xb42ec` tail (2,204 differing bytes) is register-renamed.

## 3. But killing the live range is NOT sufficient

Diagnostic: replacing `Globals::rnd->nextInt(100)` in the raid block with a
constant (removing the only use) gives **raw 4,059 / size 17,156** — the
tail does *not* collapse.  So the live range is one of several tail
differences, not "the" single root cause.  A second diagnostic removing the
`Globals::rnd` uses in the line-2024 pirate loop gives raw 16,665 /
size 17,292.

## 4. Where the residual actually sits

Byte-diff of ours vs the original (whole function = 2,628):

| region | diff bytes |
|---|---:|
| `0xaffd8..0xb178e` (case-183 / case-1 hoist) | 341 |
| `0xb178e..0xb322e` (case-2 loot + misc) | 83 |
| `0xb322e..0xb42ec` (alienGuns/pirateFortress/raid tail) | 2,204 |

The first tail divergence is at `0xb322e` itself (`ldr.w sl,[sp,#52]` vs
`ldr r6,[sp,#52]`).  Normalised (registers/imm/offsets masked) sequence
alignment of `0xb322e..0xb42ec` still shows 128 non-equal hunks at
ratio 0.879, i.e. the tail differs structurally as well as by register
renaming.

## 5. Experiments (all size 17,172 unless noted)

| variant | raw | aligned |
|---|---:|---:|
| base | **2,628** | 844 |
| `AERandom *rnd = Globals::rnd;` before `if (raid)` | 4,168 | 923 (size 17,164) |
| `AERandom *rnd` inside `if (raid)` (all 7 uses) | 4,204 | 841 (size 17,152) |
| `AbyssEngine::AERandom *&rnd = Globals::rnd;` in `if (raid)` | 2,628 | 844 |
| `AERandom *&rnd = Globals::rnd;` function scope, all 94 uses | 2,628 | 844 |
| `AERandom *rnd = Globals::rnd;` function scope, all 94 uses | 16,045 | 3,319 (size 17,156) |
| local `rnd` in the line-2024 pirate loop | 2,635 | 874 |
| local `rnd` before the line-2024 loop | 4,204 | 1,372 (size 17,176) |
| local `rnd` in the `stationPirates` escort loop | 10,419 | 1,330 (size 17,156) |
| local `rnd` in `extraAliens` | 15,809 | 1,384 (size 17,164) |
| local `rnd` in `inPirateLootOrbit` | 4,318 | 856 (size 17,188) |
| `(void) Globals::rnd;` in `if (raid)` | 2,628 | 844 |
| `AERandom *volatile rndv = Globals::rnd;` before `if (raid)` | 3,709 | 855 (size 17,184) |
| `((AERandom *) Globals::rnd)->nextInt(100)` | 2,628 | 844 |
| `(*&Globals::rnd)->nextInt(100)` | 2,628 | 844 |
| `Globals::status->getLevel() + (hard ? 40 : 20)` operand swap | 2,628 | 844 |
| `int hard` instead of `bool hard` | 2,628 | 844 |
| `const int raidType` | 2,628 | 844 |
| nested `if (raidRace == 8 && initStreamOutPosition) { if (rnd...) }` | 2,628 | 844 |
| single-line `&&` condition | 2,628 | 844 |
| `bool hard` moved after `int guns` | 2,628 | 844 |
| `int guns` moved after the `if (raidRace == 8 …)` | 4,183 | 848 (size 17,164) |
| `guns` inlined into both loop bounds | 4,183 | 848 (size 17,164) |
| `raidType` moved after `guns` | 4,202 | 847 (size 17,160) |
| `(void) this;` in `inPirateLootOrbit` | 2,628 | 844 |
| `this->enemies` instead of `gun` for `setMaxHitpoints` (alienGuns) | 4,290 | 879 (size 17,180) |
| same in pirateFortress | 3,967 | 849 (size 17,180) |
| swap the alienGuns and pirateFortress blocks | 4,426 | 1,249 (size 17,152) |
| hoist `Matrix rotation; Vector v;` before both blocks | 16,632 | 2,490 (size 17,392) |

Adjacent-statement swap sweeps (each line pair swapped, rebuilt, measured)
over `1615–1760`, `1855–1900`, `1959–2070`, `2160–2280`, `2400–2500`:
**no pair gives size 17,172 with raw < 2,628**.  Best aligned-only results:

| swap | raw | aligned |
|---|---:|---:|
| 2019/2020 (`setPosition(spawnPosition)` before `spawnPosition.z +=`) | 2,629 | **814** |
| 1736/1737 | 3,065 | **769** |
| 2484/2485 | 2,670 | **745** |
| 2042/2043 | 2,634 | 830 |

## 6. `(bigAlienWave + (i < 0) + (i < 0))` size crutch — quantified

The plain `bigAlienWave ? kAlienStationTurrets : kAlienOutpostTurrets`
gives **17,168** (raw 4,381, insn 6203).  Variants:

| condition | size | raw |
|---|---:|---:|
| plain `bigAlienWave` | 17,168 | 4,381 |
| `(bigAlienWave != 0)` / `== 1` / `(bool)` / `(int) != 0` / `+1-1` / `^0` | 17,168 | 4,381 |
| `bigAlienWave + (i < 0)` | 17,168 | 4,074 |
| `bigAlienWave - (i < 0)` | 17,168 | 3,874 |
| **`bigAlienWave + (i < 0) + (i < 0)` (current)** | **17,172** | **2,628** |
| `bigAlienWave + (i < 0) + (i < 0) + (i < 0)` | 17,172 | 2,934 |
| `bigAlienWave + (k < 0)` | 17,168 | 4,381 |
| `bigAlienWave + (k < 0) + (i < 0)` | 17,168 | 4,074 |

So the two-term form is a *raw* crutch as well as a size crutch: it is the
only form tried that both keeps 17,172 and lands the tail at raw 2,628.
The plain ternary flips `this` into `r4` (matching the original's
`0xb3232`) but makes the overall tail diff worse (3,931 vs 2,204), so it is
not the missing 4 bytes.

## 7. Case-2 loot block

The original's semantic order is `G C S R` (`cargo = nullptr`,
`crateCaptured() = 0`, `setSpeed`, `setRotationSpeed`), and it reuses one
loaded `KIPlayer*` for the two stores:

```text
b1930: ldr.w r0,[r5,#248] ; this->enemies
b1934: ldr   r0,[r0,#4]
b1936: add   r0,r6
b1938: ldr.w r1,[r0,r8,lsl#2]
b193c: str.w r9,[r1,#80]  ; cargo
b1940: strb.w r9,[r1,#76] ; crateCaptured
```

Ours (source order `C S G R`) does `strb` first and reloads
`this->enemies` for the `str`.  Measured with the current tree:

| loot order | loot-block diff | raw | aligned |
|---|---:|---:|---:|
| **CSGR (current)** | **31** | **2,628** | **844** |
| GCSR (binary order) | 36 | 2,633 | 840 |

`GCSR` is 5 raw bytes *worse* despite matching the binary's store order,
because it perturbs the surrounding register allocation.  Do not "fix" it.

## 8. Next steps

1. The tail needs a *global* register-allocation change, not a local one:
   `this` must land in `r4` at `0xb3232` **and** the surrounding 128
   structural hunks must close.  A single source edit in the
   `alienGuns`/`pirateFortress`/`raid` region has not been found.
2. Re-derive the original source shape for the `if (raid)` block from the
   binary: the original computes `guns = alienGuns + fortressGuns` at
   `0xb37e6` (inside the loop preheader) while ours hoists it to `0xb377c`
   (before the `raidRace == 8` test).  Moving `int guns` after the test in
   the source does not stop the hoist (compiler reorders), and costs 8 B.
3. The `0xb1608..0xb178e` case-1 hoist (341 raw bytes) is still the other
   independent lever (see w13545 §1/§2).

## 9. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13567
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2628 aligned=844 size=17172/17172`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13567/`
(`Level.cpp.base`, `try.sh`, `sweep_adj.py`, `exp*.py`, MIR dumps).
