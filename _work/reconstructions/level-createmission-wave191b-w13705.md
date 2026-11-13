# `Level::createMission` (`_ZN5Level13createMissionEv`) — wave 191b / w13705

Target `src/game/world/Level.cpp`, function VMA `0xaffd8`, size **17,172 B**
(`0x4314`). Branch `wave191b-w13705`, base master `a51f7dc11`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, armeabi-v7a, image base 0x10000).

Continues `level-createmission-wave191b-w13680.md` (landed the alienGuns
semantic interleave, raw 2,636 -> 2,626 at aligned 841 -> 864, +23),
`...w13691.md`, `...w13685.md`, `...w13671.md`, `...w13638.md`.

```text
                                     size            raw    aligned  insn
base a51f7dc11 (w13680 landed)       17172/17172     2626    864     6206/6197
this pass (landed)                   17172/17172     2624    858     6206/6197
```

Landed: the alienGuns loop **reloads `gun` from `(*this->enemies)[i]` after
the `createStaticObject` store** (the binary's `str.w r0,[r1,fp,lsl#2]` then
`ldr.w r6,[r0,fp,lsl#2]`), and the `t`-select crutch is respelled
`(bigAlienWave - (i < 0) * 4)`.  `gun->cargo = nullptr` stays at the top of
the loop body and `setMaxHitpoints(1000)` stays after the second rotation.

Net: raw **2626 -> 2624 (-2)**, aligned **864 -> 858 (-6)**, size exact
17,172/17,172, `.ARM.extab` unchanged (577 differing bytes, size/base exact
at `0x1a3b78`), `.text` diff **71,354 -> 71,352 (-2)**, `.ARM.exidx` 0,
`.rodata` 0, `.rel.plt` byte-identical.  `check` green: byte_exact 4508,
linked_exact 4508, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370, `RATCHET PASS` (no baseline update needed).

## 1. The binary reloads `gun` after the store

The original loop body (0xb32a8..0xb32c2):

```text
b32a8 ldr.w r1,[r4,#248]      ; this->enemies
b32ac ldr   r1,[r1,#4]        ; ->data
b32ae str.w r0,[r1,fp,lsl#2]  ; enemies[i] = createStaticObject(...)
b32b2 mov.w r1,#1000
b32b6 ldr.w r0,[r4,#248]
b32ba ldr   r0,[r0,#4]
b32bc ldr.w r6,[r0,fp,lsl#2]  ; gun = enemies[i]  (reload, NEW object)
b32c0 ldr   r0,[r6,#4]        ; gun->player
b32c2 blx   Player::setMaxHitpoints
```

The checked-in source declared `KIPlayer *gun = (*this->enemies)[i];` *before*
the store, so `gun` was the **old** object (and was even uninitialised when the
slot was empty).  Declaring it after the store both matches the binary and
fixes the semantics (`gun` is the freshly created turret).  The dead-load form
`gun = enemies[i]; enemies[i] = create(...); gun = enemies[i];` compiles to the
identical code (the first load is eliminated), so the clean after-store form is
used.

## 2. Measured frontier: aligned 841 and raw 2626 are mutually exclusive

All rows size 17,172 and `.ARM.extab` 577 unless noted.  `G/C` = the
`gun`/`createStaticObject` pair, `H` = `setMaxHitpoints(1000)`, `CG` =
`cargo = nullptr`, `T` = the `t`-select, `A1/S1/A2` = rotate1 / setRotation /
rotate2, `P/SC/SG` = setPosition / setScaling / shipGroup.

| shape | raw | aligned | .text |
|---|---:|---:|---:|
| **landed: `C,G,CG,T,A1,S1,A2,H,P,SC,SG` + `- (i<0)*4`** | **2,624** | **858** | **8,757** |
| same with `+ (i<0)+(i<0)` crutch | 2,627 | 863 | 8,760 |
| `G,C,CG,T,A1,S1,A2,H,P,SC,SG` (no reload, checked-in) | 2,626 | 864 | 8,759 |
| `G,C,H,T,A1,S1,A2,CG,P,SC,SG` (B, `+ (i<0)+(i<0)`) | 2,636 | **841** | 8,769 |
| `C,G,H,T,A1,S1,A2,CG,P,SC,SG` (`+ (i<0)+(i<0)`) | 2,637 | 837 | 8,770 |
| `C,G,H,T,A1,S1,A2,CG,P,SC,SG` (`- (i<0)*4`) | 2,641 | **834** | 8,774 |
| both rotations first, `setRotation` after (A) | 2,626 | 845 | — |
| `S1`+`CG` before `T`, both rots first (F) | 2,626 | 844 | — |
| `gun` after store, cargo after `A2` (Dp) | 2,637 | 837 | — |
| `gun` after store, `S1`+`CG` before `T` (FA) | 2,627 | 840 | — |
| pointer-walk `t++` / `t += 6` | 2,991 | 873 | — |
| `+ (i<0)+(i<0)+(i<0)` crutch | 2,946 | 843 | — |
| plain `bigAlienWave` ternary (LICM-hoisted) | 4,371 | 874 | 17,168 |

The frontier is a hard trade-off: every shape that reaches aligned <= 841
puts `H` in the binary's faithful position (immediately after the reload) and
`CG` before `setPosition`, which replaces the 2-byte `movs r1,#0` (cargo) with
the 4-byte `mov.w r1,#1000` (setMaxHitpoints) at the store point and shifts the
loop's byte stream by +10..15 raw.  Every shape that keeps raw <= 2,626 leaves
`CG` at the store point and `H` after `A2`.  No shape in the measured space
achieves raw <= 2,626 **and** aligned <= 841.

## 3. Levers swept (all size 17,172 unless noted)

### 3a. `H`/`CG` insertion x `gun` placement (full 8 x 8 grid)

`gun` before store (no reload): raw<=2,630 only at `H` after `A2`, `CG` before
`T` (2,626/864); aligned<=845 only at `H` after reload, `CG` before `P`
(2,636/841).  `gun` after store (reload): 2,624/858 at `H` after `A2`,
`CG` before `T`; 2,641/834 at `H` after reload, `CG` before `P`.  `gun` before
store with the `- (i<0)*4` crutch breaks size (17,168) unless the reload is
present.

### 3b. tail permutation `{P,SC,SG}` (24 orders)

`PSG` (`setPosition; setScaling; shipGroup`) is the best raw at every
`H`/`CG` slot.  Next best at the landed slots: `SPG` 2,626/862, `PGS`
2,627/860, `GPS` 2,628/861, `SGP` 2,630/865, `GSP` 2,633/864.

### 3c. `t`-select crutch (with the reload shape)

`- (i<0)*4`, `- (i<0)*2/3/5/8`, `+ (i<0)*4`, `- ((i<0)<<2)`,
`^ ((i<0)+(i<0))` all tie at 2,624/858 (they compile to the same
`and.w r1,#4,sl,lsr#29` cmov).  `+ (i<0)+(i<0)` = 2,627/863;
`== (i>=0)` = 2,628/859; `+ (i>>31)` = 2,627/863.  The two-term
`+ (i<0)`, `^ (i<0)`, `| (i<0)`, `||`, `&& (i>=0)` forms are 17,168 or
17,160 (LICM / branch forms, whole-function re-colour).  The crutch is
size-load-bearing: the plain ternary is hoisted by LICM and gives 17,160.

### 3d. temporary lifetimes / loop form / declarations

`Vector v` init form (`{0,0,0}` / `Vector v;` / `Vector v(0,0,0)`) neutral.
Named rotation temps (`Vector r1 = ...`, `setRotation(MatrixRotateVector(...))`)
change size (17,168-17,184).  Declaration order of `{rotation,v,k,base}` neutral
unless `base`/`rotation` move before `v` (17,168-17,236).  `for`/`while`,
`i++`/`++i`, `k` type: neutral.  `enemies` alias, `v` inside the loop, or no
`gun` local: size-breaking.

## 4. Why the +23 cannot be fully recovered while keeping raw 2,626

The aligned cost is the greedy-RA live-range of `gun`: the binary keeps
`this` in `r4`, `gun` in `r6`, `&v`/`&rotation` in low `r4`, and the
`MatrixRotateVector` return slot in `r5`; our loop puts `gun` in `r4`, `this`
in `r8`, `&v`/`&rotation` in `fp`, and the return slot in `r9`, so every
`add`/`ldr` in the loop body is a 4-byte high-register form.  The faithful
`H`/`CG` order (aligned 834-841) recovers the instruction *sequence* but pays
+10..15 raw because the 4-byte `mov.w r1,#1000` sits where the 2-byte
`movs r1,#0` sat.  The landed shape is the best `.text`-diff point on the
frontier.

## 5. Reproduce

```sh
cd /home/debian/GalaxyOnFire2/wt/w13705
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Level13createMissionEv
```

Expected: `raw=2624 aligned=858 size=17172/17172 insn=6206/6197`.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13705/`
(`measure.py`, `mkbody.py`, `sweep_hcg2.py`, `sweep_full2.py`,
`sweep_crutch.py`, `sweep_mult.py`, `sweep_forms.py`, `sweep_temps.py`,
`full2_results.txt`, `Level.base.cpp`, `WIN.so`, `H0C4.so`, `B.so`, `C.so`).
