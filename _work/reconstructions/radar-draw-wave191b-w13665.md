# Radar::draw wave 191b (w13665) — gas-cloud store order landed (raw tie, aligned −3)

**Status:** complete, 2026-11-13. Worktree `wt/w13665`, branch `wave191b-w13665`,
base `master` `ad6d56523`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13656.md` (target-bracket / literal-68 / loop-form
sweeps), `-w13647.md` (target-bracket / blip geometry), `-w13644.md`
(blip-draw / store order), `-w13636.md` (lock-image order landed), `-w13632.md`
(four structural divergences), `-w13597.md` (r10/r11 pinned to the literal-68 vreg).

**Landed:** one semantically faithful store-order reconstruction in the
gas-cloud `onScreen` block.  `raw` is unchanged (6,013), `aligned` improves
1,961 → 1,958, size stays 9,908/9,908, `.text` differing bytes 71,210 →
71,210 (unchanged), `.ARM.extab` 577 → 577 (unchanged).  Full `check` gate
green (byte_exact 4,508, linked_exact 4,508, size 4,524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5,370/5,370).

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

The original stores `cloud->field_0x76 = 1` at `0x12f61a` and the
(unconditional) `cloud->field_0x73 = 0` at `0x12f6ca`, i.e. `field_0x76`
first.  The source had the reverse order.  Swapping the two independent
stores:

```cpp
cloud->field_0x76 = 1;
cloud->field_0x73 = 0;
```

is size-exact, raw-neutral (6,013) and reduces the instruction-index
alignment diff by 3 (1,961 → 1,958).  The fully faithful order (moving the
`field_0x73 = 0` store *after* the `DrawImage2D`) is raw-worse (6,016,
aligned 1,964), so only the store swap is landed.

## 2. Fresh angles measured this pass (all size 9,908 unless noted)

### 2.1 Angle (a): faithful target-bracket / blip-geometry blocks

Re-measured on the current base (the w13647 numbers were on an older base):

| cell | raw | aligned | size | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|
| base | **6,013** | **1,961** | 9,908 | 71,210 | 577 |
| `P` planet bracket X-first | 6,014 | 1,950 | 9,908 | 71,211 | 577 |
| `L` landmark bracket X-first | 6,020 | 1,994 | 9,908 | 71,217 | 577 |
| `A` asteroid `slotCount` before `field_0x1b0` | 6,029 | 1,966 | 9,908 | 71,226 | 577 |
| `os` planet-loop `onScreen`-first | 6,014 | 1,957 | 9,908 | 71,211 | 577 |
| `eblip` enemy blip `setFrame` first | 6,017 | 1,941 | 9,908 | 71,214 | 577 |
| `sblip` station blip `setRefPixelPosition` before draw | 6,019 | 1,956 | 9,908 | 71,216 | 577 |
| `sframe` station frame `(t/field)*(n-1)` | 6,014 | 1,962 | 9,908 | 71,211 | 577 |
| `pframe` planet frame `(t/field)*(n-1)` | 6,018 | 1,963 | 9,908 | 71,215 | 577 |
| `gasd` gas `isDead`-first | 6,014 | 1,961 | 9,908 | 71,211 | 577 |
| `astA` asteroid tractorAuto store swap | **6,013** | 1,965 | 9,908 | 71,210 | 577 |
| `astM` asteroid tractorMode2 store swap | **6,013** | 1,961 | 9,908 | 71,210 | 577 |
| `g76` gas `field_0x76`-first (**landed**) | **6,013** | **1,958** | 9,908 | 71,210 | 577 |

The enemy `field_0x73` inSight block (`0x12ef8a`-`0x12efbc`) and the gas spark
`inSight` block already match the original operand order
(`screenX > cx-size`, `screenX < cx+size`, `screenY > cy-size`,
`screenY < cy+size`); no source change there.

### 2.2 Angle (b): loop/register forms

Every plain loop-form spelling is IR-invariant and byte-identical to the base
(6,013 / 1,961 / 9,908): planet `int i`, `i != size()`, `size() > i`,
`i += 1`, `unsigned i`, `(uint32_t) size()`; landmark `unsigned i`,
`i != size()`.  The size-caching loop forms remain size-fatal as in w13656.

### 2.3 Angle (c): literal-68 vreg spelling

A function-scope `const unsigned char/int/unsigned/uint8_t blipH = 68;`
used at the four enemy-region `DrawImage2D(..., 17, blipH)` sites is
byte-identical to the base — the constant is rematerialised regardless.
The original's `mov.w sl,#68` at `0x12eea2` (live to `0x12f196`) vs ours
`movs r6,#68` at the call is the same greedy-RA cascade, not a source
spelling difference.

### 2.4 Combination search

17-edit pool {`os`,`P`,`L`,`A`,`g76`,`astA`,`astM`,`eblip`,`gasd`,`sblip`,
`sframe`,`pframe`,`mus`,`snd`,`f`,`f2`}: all 17 singles + 136 pairs + 120
random 3-6 subsets.  Best raw is **6,013**; the only raw-tie cells are
`g76`/`g76+astM` (aligned 1,958), `astM` (1,961), `g76+astA` (1,962),
`astA` (1,965), `astA+astM` (1,965).  `g76` is landed (smallest change, best
raw-tie alignment).  Best aligned overall is `g76+P+eblip` (1,927) but at
raw 6,018 (+5); `os+f2+P+astM+eblip+f` reaches aligned 1,921 at raw 6,196.

## 3. Why the RA still does not flip

The head is byte-identical apart from the zero colouring: original
`mov.w fp,#0` at `0x12e07a`/`0x12e0d8` and `mov sl,r0` at `0x12e0f8`; ours
`mov.w sl,#0` and `str r0,[sp,#0x90]`.  The pre-RA MIR is invariant under
every head-region spelling tried (temporary zero locals, `bool`/`char`/
`unsigned` spellings, declaration permutations, `SetColor` moves, split
declaration/assignment).  Moving the `getCurrentCampaignMission()` call
earlier flips the colouring (`zero=fp`) but changes the call order and is
size-fatal (9,900, raw 8,559).  The residual is still the single greedy-RA
cascade pinned in w13597.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1958 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13665/`: `cell.py`
(compile+link+strip+`fn_diff`+section-diff cell harness with zero-reg readout),
`batch.py`, `Radar.base.cpp`, `search.py` / `search.log` (273 cells),
`adiff.py` / `adiff2.py` / `adiff3.py`, `orig_full.dis` / `ours_full.dis`.
