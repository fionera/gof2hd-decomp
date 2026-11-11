# Radar::draw wave 180 (w12409) — two non-original branch hints dropped (raw 6,407 -> 6,400)

**Status:** landed, 2026-11-11. Worktree `wt/w12409`, branch `wave180-w12409`,
base `master` `99e341e9b`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave179-w12307.md` (the landed 6,458 -> 6,418 base),
`radar-draw-wave179-w12309.md` and `radar-draw-wave174-w11907.md` (the
`campaign`/`sl` allocation residual, re-confirmed below).

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,407 | **6,400** (-7) |
| instruction-index alignment | 1,806 | **1,723** (-83) |
| instruction count | 3,542 ours / 3,532 orig | 3,543 / 3,532 |
| whole-`.so` `.text` differing bytes | 76,870 | **76,863** (-7) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 709 (size/base exact at `0x1a3b78`, 156,312 B) | **709** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` / `.data.rel.ro` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| file size | 2,187,328 | 2,187,328 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change

Two `__builtin_expect` branch hints that the original source shape does not
carry are removed. Both are pure branch-weight hints; the conditions and their
evaluation order are unchanged, so semantics and call counts are identical.

```diff
@@ station-lock blip frame (line ~508)
-                    if (__builtin_expect(this->field_0x28 != 0, 1) && this->field_0x28 != this->lockedStation) {  // lint: arm_builtin
+                    if (this->field_0x28 != 0 && this->field_0x28 != this->lockedStation) {

@@ planet-lock timer (line ~630)
-                if (__builtin_expect((this->planetLockTimer > this->field_0x1b8), 1)) { // lint: arm_builtin
+                if (this->planetLockTimer > this->field_0x1b8) {
```

The stale `// lint: arm_builtin` waivers on those two lines are removed with
the hints (no `__builtin_expect` remains on either line).

## 2. Why these two hints

They are the only edit pair in the whole 31-site hint space that moves the
layout toward the original. Each hint alone is neutral-to-worse; together they
are a genuine -7 raw / -83 aligned cell:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,407 | 1,806 | 9,908 |
| remove site 6 only (`field_0x28 != 0`, station blip) | 6,408 | 1,790 | 9,908 |
| remove site 10 only (`planetLockTimer > field_0x1b8`) | 6,476 | 1,696 | 9,908 |
| **remove sites 6 + 10 (landed)** | **6,400** | **1,723** | **9,908** |

Evidence that the original did not carry these two hints: the aligned metric
drops by 83 instructions while the size is unchanged, i.e. the branch layout
and block order move toward the original, not away. `.ARM.extab` is unchanged
(709), so the LSDA is untouched.

## 3. Search evidence (all size-exact unless noted)

Every measurement below is a full `link_so` + `tools/fn_diff.py` on this
worktree (`-j2`, `ulimit -v 12000000`).

### 3.1 Single-hint sweep (31 sites)

All 31 `__builtin_expect` sites were tried as remove and as flip 0<->1.
Best single edits are raw-neutral at 6,407 (`rm4`, `rm16`, `rm28`,
`flip16`); every other single edit is worse. No single edit improves raw.

### 3.2 Exhaustive removal-pair sweep

All C(31,2) = 465 removal pairs were built. The unique minimum is
`{6,10}` = **6,400**; the next best are 6,407 (`{4,16}`, `{4,28}`,
`{16,28}`), then 6,408. No other pair beats the base.

### 3.3 Triples

`{6,10}` + one more removal: best 6,400 (`+4`, `+16`, `+23`, `+28`; no
improvement). `{6,10}` + one flip: best 6,400 (`flip16`, `flip18`).
Triples grown from the other top pairs (`{16,28}`, `{4,16}`, `{4,28}`,
`{4,6}`, `{6,16}`, `{6,28}`, `{0,16}`) never beat 6,407. So `{6,10}` is a
local optimum in the hint space and 6,400 is the best known shape.

## 4. `campaign` in `sl` vs spilled `sp+0x90` — re-confirmed, not landed

The brief's primary target is the `campaign` allocation. It was re-confirmed
on this base and remains size-fatal, so it is *not* part of this change.

Original (campaign live in `sl` from the prologue to the `>= 2` test):

```
12e0f4  blx  Status::getCurrentCampaignMission
12e0f8  mov  sl, r0                 ; campaign -> r10
12e38a  cmp.w sl, #2                ; campaign >= 2
12e3c6  mov  sl, r0                 ; stations -> r10 (campaign dead)
12e3c8  mov.w fp, #0                ; planet-loop index -> r11
12e3ce  str.w sl, [sp, #116]        ; stations -> sp+0x74
12e3da  cmp  fp, r1
```

Ours (campaign spilled; `sl` reused as the loop index):

```
12e0f4  blx  Status::getCurrentCampaignMission
12e0f8  str  r0, [sp, #144]         ; campaign -> sp+0x90
12e38a  str  r0, [sp, #148]
12e38c  ldr  r0, [sp, #144]
12e38e  cmp  r0, #2
12e3c6  str  r0, [sp, #132]         ; stations -> sp+0x84
12e3c8  mov.w sl, #0                ; planet-loop index -> r10
12e3d6  cmp  sl, r1
```

The extra `str`+`ldr` pair is 4 B of the 6,400 residual, but forcing
`campaign` into `sl` is a register-allocation cascade, not a local edit:
`radar-draw-wave174-w11907.md` measured every campaign-in-`sl` source shape
at 9,816-9,912 B (size-fatal) and raw 9,200-9,600 (much worse). The greedy RA
trace there shows the zero/`missionBlocksPlanets` value (`%2481`) evicting
`campaign` (`%415`) from `$r10` because the shared constant `1` (`%121`) sits
in `$r11` with a higher weight. That residual is untouched here.

## 5. Reproduction

```sh
ulimit -v 12000000
cd /home/debian/GalaxyOnFire2/wt/w12409
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6400 aligned= 1723 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check      # RATCHET PASS
cmake --build cmake-build-match --target drift      # 0 field(s) across 0 class(es)
```

Section-level diff (scratch helper, ELF parser):

```sh
python3 /home/debian/GalaxyOnFire2/scratch/w12409/secdiff.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# .text 76,863 ; .ARM.extab 709 ; .note.gnu.build-id 20 ; .rel.plt identical
```

## 6. Scratch inventory (`/home/debian/GalaxyOnFire2/scratch/w12409`)

* `Radar.base.cpp` — exact base source (HEAD `Radar.cpp`).
* `mkvariant.py <name> <rm,rm> <fl,fl>` — regenerate a hint variant.
* `measure.sh <cpp>` — copy to `src/`, `link_so -j2`, `fn_diff`.
* `secdiff.py <orig.so> <ours.so>` — per-section differing-byte report.
* `addone.py`, `addrm.py`, `pairrm.py` — hint-space sweeps used above.
* `sweep_expect.py`, `pairs.py`, `expand.py` — earlier single/pair sweeps.
* `orig.asm`, `ours.asm` — objdump of `0x12e050..0x130704` for both binaries.
* `mi.txt`, `radar.ll`, `draw_*.txt` — RA/IR dumps (large; safe to delete).
