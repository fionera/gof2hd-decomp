# Hud::draw — wave-115: docking-bar types + second-bar declaration order — w6317

**Status:** landed pass, 2026-11-05 (wave 115). Worktree `wt/w6317`, branch
`wave115-w6317`, base `master b847d5d46` (wave-114 harvest). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

| metric | before | after |
|---|---:|---:|
| fuzzy (`verify.py` ratio) | **94.8604 %** | **95.4632 %** |
| size | 9332 B | 9332 B |
| Hud LSDA record / call-site table | 480 / 459 | **476 / 454** (= original) |
| `.ARM.extab` | 0x26298 | **0x26294** (4 B short, see §4) |
| `.text` | 0x1337c8 | 0x1337c8 |
| sections / file size | 27 / 2,187,328 | 27 / 2,187,328 |

Gate (`check`): **RATCHET PASS**, `byte_exact 4500`, `linked_exact 4500`,
size-exact 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**.

---

## 1. What changed

Two independent, real-C++ edits (5 lines moved / 2 type words):

### 1.1 Docking bar (`Hud.cpp` ~1630)

```c++
short frameH      = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int   textH       = Globals::Canvas->GetTextHeight((unsigned int) (uintptr_t) Globals::font);
int   transferred = ego->getDockTransferedAmount();          // was short
int   total       = ego->getDockTotalAmount();
float progress    = (float) transferred / (float) total;
if (this->field_0x279)
    progress = 1.0f - progress;
barOffset = (int) ((float) textH + (float) frameH * 2.5f);
String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
short barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);   // was int, after barH
int   barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);  // was int, before barW
```

i.e. the declaration order becomes the original's `frameH, textH, transferred,
total, progress, if, barOffset, text, barW, barH` (`F T R O P B S W H`), and
`transferred`/`barW` change type.  This is the faithful statement order; only
the `short`/`int` widths are tuned.

### 1.2 Jump/cloak bar (`Hud.cpp` ~1678)

```c++
float progress = 1.0f;
if (rate * 1.05f < 1.0f)
    progress = (this->field_0x277 ? ego->getDriveChargeRate() : ego->getCloakRate()) * 1.05f;
int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);  // moved up
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);           // moved down
int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
int barYOffset = Globals::layout->field_0x218;
this->field_0x464 += dt;
float fade = (float) this->field_0x464 / 1000.0f;
int fadeColor = 0xffffffff;
if (fade < 1.0f)
    fadeColor = (int) (fade * 255.0f) - 256;
int top = this->field_0x3e2;          // moved from before `field_0x464 += dt`
int y = barOffset + top * 2;
```

`GetImage2DWidth` now precedes the `String` construction, and `top` is
declared immediately before its only use.  This is what merges the second
bar's EH landing pad: the Hud LSDA becomes **476 B / 454 call-site rows**, the
original's exact record (previously 480 / 459).

---

## 2. Measured variants (all 9332 B unless noted)

Baseline (wave-112 shape, `T P B H W`, `short/short`): **94.8604 %**.

### 2.1 Docking-bar declaration order — 120 permutations of `{S,P,B,W,H}`

With `short frameH / int transferred / short barW / int barH`:

| order | fuzzy |
|---|---:|
| `P B S W H` (landed) | **95.4632** |
| `B P S W H` | 95.4632 |
| `P S H B W` | 94.8421 |
| `P B S H W` | 94.1139 |
| `S P B H W` | 93.3397 |

`F T R O` must stay in that order at the front (all 24 permutations tried; only
`F T R O` reaches 95.4632, the next best is 94.8120).

### 2.2 Docking-bar types (72 combinations of `frameH/transferred/barW/barH`)

| types | fuzzy |
|---|---:|
| `short/int/short/int` (landed) | **95.4632** |
| `short/int/int/short` | 95.3197 |
| `int/int/int/short` | 95.4589 |
| `unsigned short/int/short/int` | 95.4315 |
| `int/int/int/int` (faithful) | 93.6 |

### 2.3 Second-bar statement order

All 120 permutations of `{halfW, String text, barH, top, barYOffset}` with the
rest fixed: best `W S H T Y` = 95.3197.  Then a single-statement move search
over the full 12-statement block found `W S H Y d f c i T y` = **95.4632**
(`top` moved to just before `y`).  A swap search (66 swaps) and 250 random
valid permutations found nothing better.

### 2.4 Second-bar types

`top` ∈ {int, short, unsigned short, unsigned int} × `barYOffset` ∈ {int,
short, unsigned short} × `fadeColor` ∈ {int, unsigned int}: all 95.4632 or
less.  `float filled` hoisted after `DrawImage2D` is byte-identical.

---

## 3. Why the score moves

The docking bar is now statement-for-statement the original's order, so the
`String(" ")` temp is constructed at `sp+196` and the `operator+` is deferred
to after `barOffset`, as in the original.  The second-bar reorder makes clang
emit the `GetImage2DWidth` call before the `String` ctor and merges the
`SetColor`/`DrawImage2D`/`DrawRegion2D` landing pad, which removes the extra
2-byte `b.n` stub at `0x165fd4` and re-aligns ~40 trailing `b.n loc_XXXX`
instructions.

Remaining deltas (unchanged from wave 114): the `Status *st` gamma-block
schedule at `0x163ff0` (ours spills `&Globals::status` before `getStation`,
the original after `getIndex`; ours reloads `Globals::status` once, the
original twice), the `gammaY`/`gammaBarY` `fp`/`r9` swap at `0x164032`, the
`ldrd r5,r8` / `ldrd r8,r5` swap at `0x164e0c`, the `Globals::status &&`
null guard at `0x1642cc` (3 instructions the original does not have), and the
docking bar's `barW`/`barH` register choice.

---

## 4. `.ARM.extab` change (report)

Before this pass the Hud LSDA was **+4** (480 vs 476) and *cancelled*
`Generator::createMission`'s **-4** (168 vs 172), so the section total was
`0x26298` (size-exact).  The merge in §1.2 makes the Hud record exactly 476,
so the section is now **0x26294** (4 B short) and the only differing record is
`Generator::createMission` (-4).  `.text` stays `0x1337c8` and the file stays
2,187,328 B / 27 sections.

If the section total must stay at `0x26298`, the alternative is the smaller
docking-bar-only change (`transferred` int, `barW` short before `barH`, no
second-bar reorder): **94.9389 %**, Hud LSDA 480 / 459, `.ARM.extab`
`0x26298` unchanged.

---

## 5. Reproduction

```
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

Scratch harness (outside the repo): `/dev/shm/w6317/` — `score2.py`
(`pct` + raw ratio), `build_score.sh`, `lcsaddr.py` (LCS diff with addresses),
`perm5b.py` (120 dock permutations), `docktypes2.py` (72 dock type combos),
`move2c.py` / `swap2b.py` / `rand2c.py` (second-bar order search),
`exidx_table`-based LSDA check.
