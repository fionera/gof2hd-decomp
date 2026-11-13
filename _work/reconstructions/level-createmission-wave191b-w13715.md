# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13715

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13715`, base master `df4e658df`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13691.md` (note-only, raw 2,626),
`...w13685.md`, `...w13680.md`, `...w13671.md`.  Task: fresh angles
(a) alienGuns loop rotation/cargo interleave, (b) pirateFortress field order,
(c) independent store order / literal pool.  **No size-exact raw win was
found; note-only, `src/` unchanged.**

```text
                                     size            raw    aligned  insn
base df4e658df (checked in)          17172/17172     2626    864     6206/6197
```

`check` on the unchanged tree: byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data_exact 843/843, imports 207, lint CLEAN, drift 0,
parity 5370/5370, `RATCHET PASS`.  `.ARM.extab` diff **577** (size/base exact
at `0x1a3b78`), `.ARM.exidx` 0, `.rodata` 0, `.text` 71,375.

## 1. AlienGuns loop — the whole gap/cargo space re-swept

All 7 insertion gaps for `setMaxHitpoints` × 7 for `cargo` × `gun` before/after
`createStaticObject` (98 builds; `tools/fn_diff.py` + section headers).  The
checked-in shape (gun before store, cargo gap 0 = before rotate1, S1 gap 3 =
after rotate2) is still the unique raw minimum:

| shape | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| **checked in (`gunB_s13_cg0`)** | **2,626** | **864** | 17,172 | 577 |
| gun after store (`gunA_s13_cg0`) | 2,627 | 863 | 17,172 | 577 |
| w13685 cargo c2 (`gunA_s13_cg2`) | 2,635 | 864 | 17,172 | 577 |
| w13650 cargo c3 (`gunB_s10_cg3`) | 2,636 | 841 | 17,172 | 577 |
| faithful binary order (CREATE,GUN,S1,T,ROT1,SETROT,ROT2,SETPOS,SCALE,CARGO,GROUP) | 2,643 | 1,018 | 17,172 | 577 |
| faithful + gun before store | 2,642 | 1,022 | 17,172 | 577 |

The faithful binary statement order is *not* the raw minimum: the original
schedules `setMaxHitpoints` before the table select and `cargo` after
`setScaling`, but reproducing that order costs +17 raw.  The checked-in
reorder is semantically neutral and byte-cheaper.

`t`-select spellings that are algebraically identical to the two-term crutch
(`(bigAlienWave + (i>>31)+(i>>31))`, `!(i>=0)`, `!=0`, `>0`, `+ (k-k)`) all
compile to the **same bytes** (2,626/864/17,172/577).  Three-term crutch is
2,946/864; plain/if-else/copy-bool forms hoist and give 17,168 (4 B short,
raw 4,373, extab 615).  No spelling reaches the binary's in-loop `cbz`
diamond without losing size.

## 2. PirateFortress field order — checked-in order is optimal

All 24 tail permutations of `{setPosition,setScaling,shipGroup,cargo}` × five
`setMaxHitpoints` positions (120 builds).  Best is the checked-in
`SSGC_s0` = `setPosition; setScaling; shipGroup; cargo` with
`setMaxHitpoints` before the first rotation: **2,626 / 864 / 17,172 / 577**.
Next best `SSCG_s0` 2,632/869, `SCSG_s0` 2,641/869.  No win.

## 3. Independent store order / literal pool

### 3a. Case-4 tail (`case4Tail`, lines 2119-2141)

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| checked in | **2,626** | 864 | 17,172 | 577 |
| `hunters_last` (hunterCount before `new Array`) | 2,628 | 866 | 17,172 | **559** |
| `hc_before_new` | 2,627 | 855 | 17,172 | 577 |
| `opt44_after` | 2,629 | 858 | 17,172 | 577 |
| `hunters * (opt44 + 0.5f)` | 2,630 | 873 | 17,172 | **559** |
| no `opt44` local (inline `Globals::options[44]`) | 2,628 | 864 | 17,172 | 577 |

`hunters_last` and the `mulform` spelling each save 18 `.ARM.extab` bytes
(577 -> 559) but cost 2-4 raw bytes, so neither is a raw win.

### 3b. Raid block `volatile` aliases

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| checked in (`rnd` + `globals` volatile refs) | **2,626** | 864 | 17,172 | 577 |
| `globals` non-volatile (rnd stays volatile) | 2,627 | 863 | 17,172 | 577 |
| both direct (`Globals::rnd` / `Globals::globals`) | 2,628 | 863 | 17,172 | 577 |
| both non-volatile | 4,203 | 860 | 17,152 | 634 |

Permuting `{raidType, hard, guns}` is neutral at 2,626 for three orders and
breaks size (17,160) for the other three.  Permuting the three `nextInt`
arguments of the pirates-loop `setPosition` is raw-neutral (2,626) and only
moves aligned (864 best, checked in).  Permuting the `oy/ox/oz` declarations
of the station-escort loop is raw-worse (2,627-2,630).

### 3c. Case-2 loot store order (24 permutations)

Checked-in `CSGR` (`crateCaptured; setSpeed; cargo; setRotationSpeed`) is
still best raw: **2,626 / 864 / 17,172 / 577**.  Next `CGRS` 2,627,
`CSRG`/`GCRS`/`GRCS` 2,629.  Case-2 `coords`/`asteroidPos` zero-init order
(`int coords[6] = {}` / moving `asteroidPos` earlier) is much worse
(3,200-9,324).

## 4. Near miss: removing the case-4-tail aliases

The one large raw win found is **not size-exact**.  Removing
`Globals *globals = Globals::globals;` and
`Array<KIPlayer *> *enemies = this->enemies;` from `case4Tail` and using
`this->enemies->size()` / `Globals::rnd` directly:

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| aliases removed, `Globals::rnd` | **2,569** | 866 | **17,176** | size +32 |
| aliases removed, cast `(AERandom *)Globals::globals` | 2,785 | 857 | 17,168 | 602 |
| keep `enemies` alias, remove `globals` alias, cast | 2,732 | 857 | 17,172 | 602 |
| keep `enemies` alias, remove `globals` alias, `Globals::rnd` | 2,824 | 882 | 17,184 | size +32 |

The 2,569 shape is **+4 B** because the loop now needs a second literal-pool
word (the address of `Globals::rnd`; `Globals::rnd` and `Globals::globals` are
adjacent in `.bss` at `0x2181cc`/`0x2181d0` but the compiler emits separate
GOT slots).  Using the cast (one global) removes the extra literal but is
**-4 B** (17,168) because the exception-cleanup landing pads shrink by one
`b.n`; the cast shape also regresses raw to 2,785.  Loop-bound spellings
(`size()`, `count`, `(*enemies).size()`, `++i`, `i += 1`, `while`, reverse
compare) all reproduce 2,569/+4 or worse.  No 4-byte compensation was found
that keeps raw below 2,626.

## 5. Conclusion

Every size-exact shape measured in the (a)/(b)/(c) space is >= 2,626 raw.
The checked-in shape remains the raw minimum.  The only sub-2,626 candidate
(2,569) is 4 B oversized and cannot be compensated without a larger raw
regression.  `src/` is left unchanged.

## 6. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13715
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2626 aligned=864 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13715/`
(`measure.py`, `bytes.py`, `adiff.py`, `shift.py`, `sweep.py`, `t1.py`..`t29.py`,
`Level.base.cpp`, `Level.noalias.cpp`, `Level.noalias2.cpp`, `dbg.so`).
