# Hud::draw — wave 119 (w6923): anchor-position sweep, gamma reload, `int` dock types

**Status:** research note, 2026-11-06 (wave 119). Worktree `wt/w6923`, branch
`wave119-w6923`, base `master 298979092`. **No source change landed**: every
variant measured at or below the 95.986038 % baseline. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (file `0x163b90`).

Gate on entry and on exit (full `check`): **RATCHET PASS**, `byte_exact 4505`,
`linked_exact 4505`, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**,
drift **0**. 27 sections, file size 2,187,328 B.

Baseline measurement: **9332 B**, fuzzy **95.986038 %**, diff cost **160**,
normalised instruction streams **3149** (orig) / **3154** (ours), **101** diff
hunks.

This note extends `hud-draw-wave119-w6745.md` and `hud-draw-wave119-w6715.md`
(read those first). Everything below is a *new* measurement made this wave with
the full link + the official `verify.py` metric.

---

## 0. TL;DR

1. **The `Globals::Canvas &&` anchor at the `field_0x4b8` block is a unique
   global optimum over the whole function.** Prepending `Globals::Canvas &&` to
   each of the 39 `if` statements in `Hud::draw` (with the current anchor
   removed) gives **9332 B only at position #15** (the current site). Every other
   position lands at 9252–9596 B and 54.9–86.0 %. The current source is a strong
   local optimum.
2. **Removing the anchor is not a 6-byte change.** The anchor's own cost is 6 B
   (`ldr r5,[r9]; cmp r5,#0; beq`), but its removal shrinks the function by
   **68 B** (9264) through a *cascade* of register-allocation changes that starts
   at `0x5c0` — 0x1a2 bytes **before** the anchor — and grows monotonically to
   −68 by `0x21de`. The anchor is therefore not padding; it is the perturbation
   that lands the allocator on the original's shape.
3. **`int` dock types are real but still 4 B short.** `int frameH` + `int total`
   + `int barW` gives **9328 B / 93.663649 %** (matches `w6715`). A full
   120-permutation sweep of the dock-bar declaration block on top of the `int`
   types finds a *new* best order, **`BWTOF`** (`barW`, `barH`, `top`,
   `barYOffset`, fade-block) at **93.917739 %** — still below HEAD because the
   4-byte size error rotates the whole EH landing pad.
4. **The gamma reload can be obtained together with the `this` split** by hoisting
   the station index into an `int` local (`h3` below): 9332 B, reload **and**
   split present, but the receiver becomes the *second* `Globals::status` load
   (clang evaluates the hoisted index first), scoring **95.859115 %** — 0.13
   points below HEAD. No spelling that keeps the receiver as the first load also
   keeps the split.

---

## 1. Where the 160 cost lives (HEAD, per-0x100 block)

| region | cost | hunks | what |
|---|---:|---:|---|
| `0x00400` | 9 | 6 | gamma block (`st` spill shape + `field_0x44a`/`field_0x448` reg swap) |
| `0x00700` | 14 | 8 | `Globals::Canvas &&` anchor + `SetColor` base-load order |
| `0x01900` | 1 | 1 | — |
| `0x01a00` | 25 | 17 | boost `SetColor` base-load hoist (6 B) + branch renames |
| `0x01b00` | 8 | 5 | fire-image ternary load sinking |
| `0x01c00` | 19 | 14 | fire-image else arm + dock-bar landing pad |
| `0x01d00` | 31 | 15 | dock bar (`short`→`int` narrowing, `fp`/`r9` moves) |
| `0x01e00` | 5 | 5 | dock bar `DrawRegion2D` |
| `0x01f00` | 23 | 14 | second bar (`r4/r5/fp` rotation, `String`/`halfW` order) |
| `0x02000` | 21 | 12 | second bar (`GetImage2DWidth` hoist, rotation) |
| `0x02200` | 1 | 1 | — |
| `0x02300` | 3 | 3 | EH landing-pad filler |

Instruction-level: **69** same-mnemonic pairs (of which **31** are
branch-target-only, i.e. pure address renames from the size deltas) and **24**
different-mnemonic instructions.

## 2. Delta profile (ours − orig) of HEAD

Address of the first instruction of each equal block, `delta = ours − orig`:

```
idx     0  0x0000  0x0000  +0
idx   368  0x0468  0x046a  +2     gamma block (internal alignment)
idx   374  0x047c  0x047c  +0
idx   614  0x073c  0x0742  +6     <-- anchor
idx   622  0x0752  0x075c +10
idx   623  0x0756  0x0762 +12
idx   627  0x0762  0x0768  +6
idx   632  0x0772  0x0772  +0     anchor absorbed
idx   664  0x07c0  0x07c0  +0     (long identical run, 1646 insns)
idx  2310  0x1a48  0x1a4e  +6     boost SetColor Canvas hoist
idx  2363  0x1ade  0x1af4 +22
idx  2370  0x1af8  0x1b02 +10
idx  2428  0x1bae  0x1bb0  +2
idx  2479  0x1c38  0x1c38  +0
idx  2546  0x1cf8  0x1d06 +14     dock bar
idx  2569  0x1d36  0x1d36  +0
idx  2609  0x1db8  0x1db8  +0
idx  2755  0x1f60  0x1f6c +12     second bar
idx  2882  0x20e0  0x20e6  +6
idx  3006  0x225a  0x225a  +0
idx  3127  0x2392  0x238e  -4     EH landing pad
idx  3133  0x23a0  0x23a0  +0
```

Because `SequenceMatcher` compares branch targets literally, every one of these
non-zero deltas turns the branches that cross it into a `b.n 0xXXXX` vs
`b.n loc_XXXX` diff. Fixing the deltas to zero is worth ~31 of the 160 cost.

## 3. Anchor-position sweep (new)

With the current `Globals::Canvas &&` removed from `if (this->field_0x4b8 >= 1)`,
`Globals::Canvas && ` was prepended to each of the **39** `if` statements in
`Hud::draw`, one at a time. Full link + official metric:

| # | site | size | fuzzy |
|---:|---|---:|---:|
| **15** | **`field_0x4b8 >= 1` (current)** | **9332** | **95.986038** |
| 12 | `field_0x4ac >= 1` | 9336 | 86.004121 |
| 39 | `if (show)` (second bar) | 9280 | 85.174603 |
| 22 | `hasVolatileGoods()` (mission type 174) | 9276 | 84.997619 |
| 23 | `hackingGameActive && !isInTurretMode()` | 9272 | 83.473567 |
| 32 | `hasCloak && field_0x498 >= 0` | 9268 | 83.455065 |
| 34 | `field_0x484 >= 0` | 9268 | 83.455065 |
| 18 | `hasVolatileGoods()` (mission type 12) | 9268 | 83.404796 |
| 21 | `hasVolatileGoods()` (mission type 184, 2nd) | 9272 | 83.346563 |
| 37 | `field_0x277 \|\| field_0x276 \|\| weaponSelectState` | 9268 | 82.960140 |
| 28 | `hasAutoTurret()` | 9596 | 58.237667 |
| 7 | `hasShieldBar` | 9252 | 67.037331 |
| 1 | `!mining && eventQueueDirty && …` | 9336 | 54.933586 |

Only #15 reaches 9332 B; no position beats 95.986038 %.

Anchor *spellings* at #15 (all 9332 B):

| spelling | size | fuzzy |
|---|---:|---:|
| `Globals::Canvas && field_0x4b8 >= 1` (HEAD) | 9332 | **95.986038** |
| `Globals::Canvas && field_0x4b8 > 0` | 9332 | 95.986038 |
| `Globals::Canvas != 0 && field_0x4b8 >= 1` | 9332 | 95.986038 |
| `Globals::Canvas && 1 <= field_0x4b8` | 9332 | 95.986038 |
| `PaintCanvas *canvas = Globals::Canvas;` before the `if` | 9332 | 95.986038 |
| `field_0x4b8 >= 1 && Globals::Canvas` | 9264 | 83.259212 |
| `field_0x4b8 >= 1 && Globals::Canvas != 0` | 9264 | 83.259212 |
| `field_0x4b8 >= 1 && field_0x4b8 <= 300` | 9264 | 83.322745 |
| `field_0x4b8 > 0 && field_0x4b8 >= 1` | 9264 | 83.386277 |
| `PaintCanvas *canvas = Globals::Canvas;` inside the `if` | 9264 | 83.386277 |
| inner `if (Globals::Canvas)` around `SetColor` | 9276 | 69.885642 |

## 4. Why removing the anchor is not a 6-byte change (new)

`if (this->field_0x4b8 >= 1)` with no anchor builds to **9264 B / 83.386277 %**
(−68 B). The delta profile of that build is *not* a constant −6: it starts at
−4 at `0x5c0` (before the anchor site), then −6, −8, −12, −16, … and reaches
−68 by `0x21de`. The anchor therefore changes the greedy allocator's decisions
across the whole function; its 6 bytes of code are incidental. This is why every
"remove the anchor and add N bytes elsewhere" variant (N = 2, 4, 6) fails.

## 5. Gamma block: reload + split is reachable, receiver is not (new)

Original gamma block (file `0x164010`):

```
ldr  r6,[r0,#0]      ; r6 = &Globals::status
ldr  r4,[r6,#0]      ; load #1  -> getStation + receiver
mov  r0,r4 ; blx Status::getStation ; blx Station::getIndex ; mov r5,r0
ldr  r0,[r6,#0]      ; load #2  -> mission
str  r6,[sp,#108]    ; late spill of &Globals::status
blx  Status::getCurrentCampaignMission
mov  r2,r0 ; mov r0,r4 ; mov r1,r5 ; blx Status::getGammaRayDamagePerSecond
```

Measured variants (full link + official metric):

| variant | size | pct | reload | split |
|---|---:|---:|---|---|
| HEAD (`Status *st`, 3 uses) | **9332** | **95.986038** | no | yes |
| plain 3× `Globals::status` | 9312 | 74.988090 | yes | no |
| `st` receiver + `Globals::status` arg2 | 9312 | 74.988090 | yes | no |
| `Status **stp = &Globals::status` | 9312 | 74.988090 | yes | no |
| `st` receiver + `Globals::status` getStation | 9332 | 95.986038 | no | yes |
| two locals `st`, `st2` (CSE'd) | 9332 | 95.986038 | no | yes |
| `Globals::status` receiver + `st` arg2 | 9332 | 95.986038 | no | yes |
| `Globals::status` receiver + `st` getStation | 9312 | 74.988090 | yes | no |
| **`int stationIdx = Globals::status->getStation()->getIndex();` hoisted before the `if`** | **9332** | **95.859115** | **yes** | **yes** |
| `st` + hoisted idx from `st` + `Globals::status` arg2 | 9312 | 74.988090 | yes | no |
| `st` + hoisted idx + `int mission` local | 9312 | 74.988090 | yes | no |
| `Globals::status` receiver + hoisted idx + `int mission` | 9328 | 73.336509 | yes | no |

The hoisted-index shape produces the original's two loads *and* the `this`
split, but because C++ evaluates the object expression before the arguments and
the index statement precedes the `if`, the receiver is load #2 (`mov r0,r5`,
`mov r1,r4`) instead of load #1 (`mov r0,r4`, `mov r1,r5`) — three extra diff
hunks that cost 0.127 points.

## 6. `int` dock types: new best declaration order (new)

`int frameH`, `int total`, `int barW` (confirmed from the original: no
`sxth`/`uxth`, `vcvt.f32.s32`, signed `/2`). All variants are **4 B short of
9332**, which rotates the EH landing pad and costs ~2 points on its own.

| variant | size | pct |
|---|---:|---:|
| HEAD (`short`/`short`/`unsigned short`) | 9332 | **95.986038** |
| `int frameH` only | 9328 | 94.302492 |
| `int total` only | 9328 | 93.970168 |
| `int barW` only | 9328 | 94.097112 |
| `int frameH`+`int total` | 9328 | 93.822455 |
| `int frameH`+`int barW` | 9328 | 93.748017 |
| `int total`+`int barW` | 9324 | 93.572449 |
| all three `int`, HEAD order `WBTFO` | 9328 | 93.663649 |
| all three `int`, **`BWTOF`** | 9328 | **93.917739** |
| all three `int`, `BWFTO` / `BWTFO` / `BWOFT` / `BWOTF` | 9328 | 93.822455 |
| all three `int`, `WBTOF` | 9328 | 93.758933 |
| all three `int`, `WBOTF` / `WBOFT` | 9328 | 93.663649 |
| all three `int` + anchor at `field_0x4ac` (#12) | 9332 | 87.577344 |

(`W`=barW, `B`=barH, `F`=the `field_0x468`/`fadeIn`/`fade` block, `T`=top,
`O`=barYOffset.) All 120 permutations were measured; `BWTOF` is the best.
`top`/`barOffset`/`barYOffset` type changes (`int`, `unsigned`, `short`) are
neutral on top of the `int` types (9328, 93.663649) except `short barOffset`
(9328, 93.917739).

## 7. Other variants measured this wave (all neutral at 9332 / 95.986038 unless noted)

### Boost `SetColor` (`0x1adc`)

| variant | size | pct |
|---|---:|---:|
| HEAD | 9332 | **95.986038** |
| `float boostRate = ego->getBoostRate();` local | 9268 | 86.199778 |
| `unsigned int boostColor` local | 9264 | 86.354249 |
| `unsigned int boostColor = 0xffffff00; if (…) …` | 9264 | 86.354249 |
| `bool low` local | 9264 | 86.354249 |
| `if`/`else` two `SetColor` calls | 9264 | 86.417792 |
| `(float)` cast of `getBoostRate()` | 9332 | 95.986038 |
| explicit parens around the condition | 9332 | 95.986038 |
| `0xffffff2fU : 0xffffff00U` | 9332 | 95.986038 |
| `!(getBoostRate() >= 1.0f)` | 9332 | 95.954307 |
| `getBoostRate() >= 1.0f ? 0xffffff00 : 0xffffff2f` | 9332 | 95.954307 |

### Fire-image ternary (`0x1b7a`)

| variant | size | pct |
|---|---:|---:|
| HEAD | 9332 | **95.986038** |
| `if`/`else` assigning `fireImage` | 9332 | 95.986038 |
| init to `autoTurretOnImage`, `if` overwrites | 9332 | 95.986038 |
| `const unsigned int &fireImage` | 9332 | 95.986038 |
| `bool pressed` local | 9332 | 95.986038 |
| ternary inlined in both `DrawImage2D` calls | 9364 | 92.395437 |
| `unsigned int *fireImages = &this->orbitMarkerIdleImage;` + index | 9328 | 93.145033 |

### Gamma body (`gammaY` / `gammaBarY`)

| variant | size | pct |
|---|---:|---:|
| swap declaration order | 9332 | 95.986038 |
| `x + (x - y)` parenthesised form | 9332 | 95.986038 |
| swap + parenthesised | 9332 | 95.986038 |
| `x*2 - y` | 9348 | 86.552490 |

### Second progress bar

| variant | size | pct |
|---|---:|---:|
| `filled = (float) halfW * progress` | 9332 | 95.986038 |
| `filled = progress * halfW` | 9332 | 95.986038 |
| `(int) (filled * 2.0f)` instead of `filled + filled` | 9332 | 95.986038 |
| `unsigned int top` | 9332 | 95.986038 |
| `y = barOffset + (top << 1)` | 9332 | 95.986038 |
| `unsigned int fadeColor` | 9332 | 95.986038 |
| `progress = rate * 1.05f` (no re-call) | 9240 | 82.856234 |
| `halfW` without the inner `(float)` cast | 9332 | 95.986038 |

### Misc

| variant | size | pct |
|---|---:|---:|
| `unsigned int hitFlags` | 9332 | 95.986038 |
| `int savedColor` | 9332 | 95.986038 |
| `unsigned int dividerOffset` | 9332 | 95.986038 |
| `int armorY` | 9332 | 95.986038 |
| `if (this->field_0x470 > 0)` | 9332 | 95.986038 |
| `if (this->field_0x46c >= 501)` | 9332 | 95.986038 |
| `if (this->field_0x4ac > 0)` | 9332 | 95.986038 |
| `if (ego->getBoostRate() >= 1.0f && !field_0x474)` | 9336 | 87.722081 |
| `float shieldRate` hoisted before `sideImage`/`frontImage` | 9328 | 87.746032 |
| `frontImage` declared before `sideImage` | 9332 | 95.763922 |

## 8. Conclusion

The landed 9332 B / 95.986038 % shape remains the best measured. The three
remaining levers are all blocked by the same mechanism:

* the **`Globals::Canvas &&` anchor** is a unique global optimum over all 39
  `if` sites, and its removal cascades −68 B of allocation changes;
* the **gamma reload** needs the receiver to be the first `Globals::status`
  load, which is incompatible with the hoisted index that restores the `this`
  split;
* the **`int` dock types** are faithful but 4 B short, and no source construct
  found this wave restores exactly 4 B without rotating the frame.

The next step is unchanged from `w101`/`w102`: find the construct *outside* the
gamma block that raises the `this` split weight at `bb.28` without adding a
branch, or the construct that supplies the missing 4 B for the `int` dock
types.

## 9. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w6923
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS, byte 4505
```

Scratch harness in `/dev/shm/w6923`: `score.py` (size / byte / linked / pct /
insn count, plus `diff` and `show` modes), `build.sh` (link_so + score),
`batch.py` (`run(label, old, new)` — restore pristine `Hud.cpp`, apply one
exactly-once replacement, build, score), `dump.py` (address-tagged objdump of
both sides), `gotstat.py` (GOT-relocation census of the function's
`ldr/add pc` pairs), `regcost.py` (per-0x100 diff cost table).
