# Radar::draw wave 191b (w13660) — gas-blip store order landed (raw tie, aligned −3)

**Status:** complete, 2026-11-13. Worktree `wt/w13660`, branch `wave191b-w13660`,
base `master` `6c02aaeb4`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13647.md` (target brackets), `-w13644.md`
(blip/store order), `-w13636.md` (lock-image landed), `-w13597.md`
(r10/r11 swap), `-w13632.md` (structural divergences).

**Landed:** one semantically faithful store-order reconstruction in the
gas-cloud blip block.  `raw` is unchanged (6,013), `aligned` improves
1,961 → 1,958, size stays 9,908/9,908, `.text` differing bytes 71,210
(unchanged), `.ARM.extab` 577 → 577 (unchanged), `.ARM.exidx` / `.rodata`
0 / 0.  Full `check` gate is green (byte_exact 4508, linked_exact 4508,
size-exact 4524, data 843/843, imports 207, lint CLEAN, parity 5370/5370,
drift 0).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,961 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,210 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Landed change: gas-cloud `field_0x76` before `field_0x73`

The original gas blip block stores `cloud->field_0x76 = 1` at `0x12f61a`
(before `Player::getPosition`) and the unconditional `cloud->field_0x73 = 0`
at `0x12f6ca` (after the `DrawImage2D` at `0x12f6b2`):

```
12f618 movs r0,#1
12f61a strb.w r0,[sl,#118]   ; cloud->field_0x76 = 1
...
12f6b2 ... DrawImage2D
12f6ca movs r0,#0
12f6cc strb.w r0,[sl,#115]   ; cloud->field_0x73 = 0
```

Our source had the two stores in the opposite source order
(`Radar.cpp:998-999`).  Swapping them to put `field_0x76` first is the
w13644 §1.5b divergence.  Size-exact, raw-neutral, and on this base it
improves the aligned metric by 3.

```cpp
cloud->field_0x76 = 1;
cloud->field_0x73 = 0;
```

Note the compiler still schedules the emitted pair as `0x73` then `0x76`
(see §3), so this is a source-order fidelity fix, not a raw byte win; the
aligned metric improves because the local hunk length now matches the
original's 3-instruction hunk instead of 3-vs-1+2.

## 2. Fresh angles measured this pass

All cells are size 9,908 unless noted.  Base = 6,013 / 1,961 / 9,908.

### 2.1 Literal-68 vreg source spelling (angle c) — all IR-identical

All 14 `DrawImage2D(..., 17, 68)` sites were re-spelled.  Every spelling
produced the byte-identical function (6,013 / 1,961 / 9,908):

| spelling | raw | aligned | size |
|---|---:|---:|---:|
| `(unsigned char) 68` | 6,013 | 1,961 | 9,908 |
| `0x44` | 6,013 | 1,961 | 9,908 |
| `68u` | 6,013 | 1,961 | 9,908 |
| `(unsigned char) 0x44` | 6,013 | 1,961 | 9,908 |
| file-scope `static const unsigned char kPlaceFlags = 68` | 6,013 | 1,961 | 9,908 |

`68` is an `unsigned char` literal argument; clang canonicalises every
spelling to the same i8 constant, so the `%900`/r11-vs-r10 home cannot be
changed from the literal's spelling.

### 2.2 Loop variable types (angle b) — all IR-identical

`uint32_t`/`int`/`unsigned`/`size_t` for the landmark, planet, enemy, gas,
spark and asteroid loop induction variables all produce the byte-identical
function (6,013 / 1,961 / 9,908).  The loop bound is `Array::size()` (an
unsigned comparison), so the induction variable is already known
non-negative and the signedness is folded away.

`(i & 0x7fffffff) == 3` → `i == 3` (and `== 1`) is likewise byte-identical.

### 2.3 `__builtin_expect` polarity / removal sweep

All 29 `__builtin_expect` sites were flipped (0↔1) and removed one at a
time.  None gives a raw win.  Raw-tie cells (6,013) that leave the
colouring unchanged: flip14, rm4, rm10, rm12, rm14 (all aligned 1,961).
Best aligned values seen are raw-worse:

| cell | raw | aligned | size |
|---|---:|---:|---:|
| `rm8` (drop expect on the campaign-125 `getStatusValue` test) | 6,019 | **1,927** | 9,908 |
| `rm19` (drop expect on the `enemyLockTimer > ...` test) | 6,728 | **1,897** | 9,908 |
| `flip9` (`!lockConfirmed` 0→1) | 6,178 | 1,938 | **9,904** |
| `flip22` (`field_0x1bc >= 1` 1→0) | 6,177 | 1,947 | **9,912** |
| `flip11` (nested `isDockingToPlanet` outer 0→1) | 6,015 | 1,962 | 9,908 |

### 2.4 Campaign / `missionBlocksPlanets` type sweep

`campaign` as `short`/`char`/`unsigned char`/`int8_t`/`int16_t` is
size-fatal (9,912) and keeps the wrong colouring; `long` is byte-identical
to `int`; `unsigned` is 6,014.  `missionBlocksPlanets` as
`short`/`unsigned short`/`long`/`unsigned long`/`unsigned`/`size_t`/
`intptr_t` is byte-identical; `bool` is 6,172 / **9,912**; `int64_t` is
9,924.

### 2.5 Target-bracket / blip-geometry blocks on this base (angle a)

Re-measured with the landed lock-image change and the new gas order:

| cell | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| base | **6,013** | 1,961 | 9,908 | 71,210 | 577 |
| gas `field_0x76` first (landed) | **6,013** | **1,958** | 9,908 | 71,210 | 577 |
| planet bracket X-first (`P`, w13647 §1.1) | 6,014 | 1,950 | 9,908 | 71,211 | 577 |
| landmark bracket X-first (`L`) | 6,020 | 1,994 | 9,908 | 71,217 | 577 |
| asteroid slot `slotCount`/`field_0x1b0` (`A`) | 6,029 | 1,966 | 9,908 | 71,226 | 577 |
| `P+L` | 6,021 | 1,983 | 9,908 | 71,218 | 577 |
| `P+L+A` | 6,037 | 1,986 | 9,908 | 71,234 | 577 |
| enemy blip `setFrame,setRefPixelPosition,draw` (w13644 §1.4) | 6,017 | **1,941** | 9,908 | 71,214 | 577 |
| gas + `P` | 6,014 | 1,947 | 9,908 | 71,211 | 577 |
| gas + enemy blip | 6,017 | 1,938 | 9,908 | 71,214 | 577 |
| gas + enemy blip + `P` | 6,018 | **1,933** | 9,908 | 71,215 | 577 |

`P` costs only +1 raw on this base (it cost +6 on the w13647 base before
the lock-image landing), and the enemy-blip call order still gives the best
alignment (−20 aligned for +4 raw).  Both were left unlanded because the
task gate is a raw metric and the landed gas swap is the only raw-tie win.

## 3. Why no raw win

The residual is still the single greedy-RA colouring: the shared
zero/`missionBlocksPlanets` range takes `sl` (original `fp`) and `campaign`
is spilled to `sp+0x90` (original `sl`).  The first differing byte is the
`mode` spill slot (`sp+0x68` vs `sp+0x64`); the first length difference is
`cmp.w sl,#2` vs `ldr r0,[sp,#0x90]; cmp r0,#2` at `0x12e38a`.  Every
source spelling tried here leaves the pre-RA IR byte-identical or changes
the loop body (the original reloads `this->planetTargets` from
`[r4,#320]` every iteration, so hoisting it is structurally wrong).

The landed gas swap is visible in the emitted code as the pair
`strb 0x73; strb 0x76` (ours) vs `strb 0x76` at `0x12f61a` (original), but
the aligned metric still improves because the local hunk is 3-vs-3 instead
of 3-vs-1+2.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1958 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -j2   # DRIFT: 0
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13660/`: `cell.py`
(compile+link+strip+`fn_diff`+section-diff cell harness), `Radar.base.cpp`
(pristine source), `adiff.py` (address-annotated aligned diff),
`cells/` (the measured cells), `mir_base.txt` (`-print-machineinstrs`),
`orig.dis` / `ours.dis`.
