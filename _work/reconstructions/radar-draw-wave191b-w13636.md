# Radar::draw wave 191b (w13636) — lock-image operand order landed (raw tie, aligned −4)

**Status:** complete, 2026-11-13. Worktree `wt/w13636`, branch `wave191b-w13636`,
base `master` `57ce1a4c8`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13614.md`, `-w13606.md`, `-w13593.md`, `-w13587.md`.

**Landed:** one semantically faithful operand-order reconstruction in the enemy
lock-image selector.  `raw` is unchanged (6,013), `aligned` improves 1,965 →
1,961, size stays 9,908/9,908, `.text` differing bytes 71,197 → 71,197
(unchanged), `.ARM.extab` 577 → 577 (unchanged).  The full `check` gate is
green (byte_exact 4508, linked_exact 4508, size-exact 4524, data 843/843,
imports 207, lint CLEAN, parity 5370/5370).

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,965 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,197 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` | 0 / 0 |
| zero/`missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` (ours) |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=.../android_2.0.16_libgof2hdaa.so
-DGOF2_FMOD_LIBDIR=.../apk_libs`, then
`cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.

## 1. Landed change: enemy lock-image evaluation order

The binary checks `Player::enemyFlagsLo` (offset 0x5c) **before**
`Player::carriesFriendCargoFlag` (offset 0x5d); our source had the two branches
swapped.  Original `0x12f350`:

```
12f350 ldr  r2,[r8,#4]        ; enemy->player
12f354 ldrb r1,[r2,#92]       ; enemyFlagsLo  (0x5c)  <- first
12f358 cmp  r1,#0
12f35a ldr  r1,[sp,#40]       ; &image_0x5c  (sp+0x28 = r4+0x5c)
...
12f386 ldrb r1,[r2,#93]       ; carriesFriendCargoFlag (0x5d) <- second
12f38c ldr  r1,[sp,#28]       ; &image_0x64  (sp+0x1c = r4+0x64)
12f390 addeq r1,r4,#116       ; &image_0x74
```

The image mapping itself already matched (`enemyFlagsLo → image_0x5c`,
`carriesFriendCargoFlag → image_0x64`, else `image_0x74`); only the evaluation
order differed.  Source fix:

```cpp
if (enemy->player->enemyFlagsLo) {
    lockImage = this->image_0x5c;
} else if (enemy->player->carriesFriendCargoFlag) {
    lockImage = this->image_0x64;
} else {
    lockImage = this->image_0x74;
}
```

This is also the only 0x5c/0x5d order mismatch in the whole function: the other
four pairs (frame image, bar image, `else`-branch image, wreck image) already
test 0x5c first.

## 2. Cells measured this pass (all size 9,908 unless noted)

### 2.1 `T_update` family (w13614 shape: `targets` local for the `update` call)

| cell | raw | aligned | size | insn | zero |
|---|---:|---:|---:|---:|---|
| base | **6,013** | **1,965** | 9,908 | 3541 | sl |
| `T` (= w13614 `T_update`) | 6,697 | 2,079 | 9,908 | 3526 | fp |
| `T` + music `A,C,B` | 6,720 | 2,073 | 9,908 | 3527 | fp |
| `T` + sound `inAlienOrbit` first | 6,712 | 2,074 | 9,908 | 3526 | fp |
| `T` + asteroid `=0`/`+= mode` | 6,877 | 2,079 | 9,908 | 3526 | fp |
| `T` + all three semantic fixes | 6,900 | 2,068 | 9,908 | 3527 | fp |
| `T` + lock-image + `onScreen`-first | 6,692 | 2,075 | 9,908 | 3526 | fp |

Combining `T_update` with the correct semantic fixes never recovers the raw
cost of the hoisted `targets` pointer.

### 2.2 Semantically correct fixes on the base colouring

| cell | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | **6,013** | **1,965** | 9,908 | 3541 |
| lock-image order (landed) | **6,013** | **1,961** | 9,908 | 3541 |
| `onScreen`-first | 6,014 | 1,961 | 9,908 | 3541 |
| lock-image + `onScreen`-first | 6,014 | 1,957 | 9,908 | 3541 |
| all three semantic fixes (music/sound/asteroid) | 6,213 | 1,953 | 9,908 | 3542 |
| all three + `onScreen`-first | 6,214 | 1,949 | 9,908 | 3542 |
| `unsigned campaign` | 6,014 | 1,965 | 9,908 | 3541 |
| lock-image + `unsigned campaign` | 6,014 | 1,961 | 9,908 | 3541 |

### 2.3 `barWidth` store order (fresh angle)

The binary stores `this->barWidth = (int) hull` **before** the hull
`DrawRegion2D` call (`0x12f2a6 vstr s2,[r4,#388]` precedes `0x12f2c4 blx
DrawRegion2D`); our source stores it after.  Moving the store before the call
is size-fatal:

| cell | raw | aligned | size |
|---|---:|---:|---:|
| base | **6,013** | **1,965** | 9,908 |
| `barWidth` before `DrawRegion2D` | 8,314 | 1,912 | **9,912** |
| + lock-image | 8,314 | 1,908 | **9,912** |

### 2.4 Faithful `while` loop (explicit `++i` on every `continue` path)

The w13614 `while` probe (which lets `continue` skip the increment) is not
semantically faithful.  Adding `++i` before both `continue`s and at the body
end flips the RA (zero = `fp`) and lifts `aligned` to 3,008, but is size-fatal:

| cell | raw | aligned | size | insn | zero |
|---|---:|---:|---:|---:|---|
| base | **6,013** | **1,965** | 9,908 | 3541 | sl |
| faithful `while` | 9,647 | 3,008 | **9,988** | 3535 | fp |
| faithful `while` + lock-image | 9,647 | 3,004 | **9,988** | 3535 | fp |

### 2.5 Declaration / frame perturbations (all raw-neutral)

| cell | raw | aligned | size |
|---|---:|---:|---:|
| `stations` / `planetHit` declaration swap | 6,013 | 1,965 | 9,908 |
| `bool planetHit; planetHit = false;` split | 6,013 | 1,965 | 9,908 |
| `mode` through a local copy (`modeArg`) | 6,013 | 1,965 | 9,908 |

## 3. What is left (evidence, not fixed)

* **RA swap is still the dominant residual.**  Ours: zero/`missionBlocksPlanets`
  in `sl`, `campaign` spilled.  Original: zero/mbp in `fp`, `campaign` in `sl`.
  This changes the planet-loop preheader spill widths: original
  `str.w fp,[sp,#0x70]` (4 B) + `str.w sl,[sp,#0x74]` (4 B); ours
  `str r0,[sp,#0x84]` (2 B) + `str r0,[sp+0x90]` (2 B).  The 4-byte shift
  propagates through the whole planet/landmark/enemy loops (the dense
  `0x300`–`0x1700` diff region).
* **Frame slot `mode`:** original `sp+0x64`, ours `sp+0x68` — the very first
  differing byte (`0x12e05e`).  Only `mode` differs among the argument spills
  (`player` `sp+0x88` and `hud` `sp+0x54` agree).
* **Block layout:** the campaign `isTarget` block is in-line in the original
  (`0x12e474`) but out-of-line in ours (`0x12e5ca`), so the call sequence
  diverges even though the call multiset is identical (344 calls).
* `barWidth`-before-`DrawRegion2D` and the faithful `while` are both correct
  shapes but size-fatal on their own.

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1961 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check
# -> RATCHET PASS (byte 4508, linked 4508, size 4524, data 843/843, imports 207)
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13636/`: `cell2.py` (1 s
compile+link+strip+`fn_diff` cell), `Radar.base.cpp` (pristine source),
`c1.py`–`c6.py` (the sweeps above), `secdiff.py` (section byte-diff),
`orig.dis` / `ours.dis`, `draw_base.mir`.
