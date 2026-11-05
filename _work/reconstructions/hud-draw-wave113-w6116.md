# Hud::draw — wave-113: dock-block register allocation / LSDA sweep (94.9 % holds) — w6116

**Status:** analysis-only pass, 2026-11-05 (wave 113). Worktree `wt/w6116`,
branch `wave113-w6116`, base `master a979ace18` (wave-112 harvest). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (Ghidra `Hud_draw` @ `0x173b90`, file `0x163b90`).

Gate on entry and exit (full `check`): **byte_exact 4500 / linked_exact 4500**,
size-exact **4524/4524**, `stub_zero_size 0`, `missing 0`, `wrong_type 0`,
`extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**, drift **0**,
**RATCHET PASS**. Sections unchanged: `.text 0x1337c8`, `.rodata 0x41c27`
(base `0x1c9e10`), `.ARM.exidx 0x6f70`, **`.ARM.extab 0x26298`**, file size
**2,187,328 B**, 27 sections.

**No source change was landed**: every measured variant scored `<= 94.9 %`, so
this commit is the note only. The landed source is the wave-112 `T P B H W D`
dock shape at **94.9 %, 9332 B**.

This note extends `hud-draw-wave112-w6014.md`, `hud-draw-wave111-w5916.md`,
`hud-draw-wave109-w5717.md`, `hud-draw-wave108-w5603.md`,
`hud-draw-oracle-dock-w103-wf315.md` and `hud-draw-dock-order-w4415.md`.

---

## 0. TL;DR

The faithful dock order is confirmed from the binary and from the REA decompile:

```
frameH = GetImage2DHeight(image_0x37c)     // int, stored to sp#96
textH  = GetTextHeight(font)               // int, r9
transferred = getDockTransferedAmount()    // int, r4
total       = getDockTotalAmount()         // int, r5
field_0x279 -> r8 (loaded once)
getText(0x279 ? 3205 : 3204) -> r6
String(" ") at sp#196
progress = transferred / total; if (0x279) progress = 1 - progress
barOffset = textH + frameH * 2.5
operator+(sp#208, r6, sp#196); StringD1Ev(sp#196)
barW = GetImage2DWidth(image_0x37c)        // r8
barH = GetImage2DHeight(image_0x37c)       // fp (Canvas moved fp->r5 first)
```

i.e. source order **P B T W H D** (`progress`, `barOffset`, `String text`,
`barW`, `barH`, `dt/fade`), with `int frameH` and `int transferred`.

The faithful variant reproduces the original dock block **instruction for
instruction from `0x165880` to `0x16594e`** (identical addresses, identical
opcodes) except:

1. the `frameH` / `Globals::font`-GOT spill slots are swapped (`#92`/`#96`);
2. `barW` is allocated `r6` instead of `r8`;
3. the `mov r5, fp` (Canvas save) is missing, so `barH` goes to `r8` instead
   of `fp`;
4. consequently the `Globals::gameText` GOT pointer stays in `fp` instead of
   being spilled to `sp#96` and reloaded (4 extra instructions in the
   original: `ldr r3,[sp,#96]` + `ldr r3,[r3,#0]` twice, and `mov r5,fp`).

The 8-byte size deficit of the faithful variant (9324 vs 9332) is **entirely**
inside this dock block. The rest of the function is byte-for-byte the same
source as the landed variant.

The landed (tuned) shape `T P B H W D` with `short frameH`/`short transferred`
keeps the exact stack map (frameH `#96`, font `#92`) and the 9332 B size, which
is why it scores higher on the fuzzy metric even though its dock statement
order is not the original's.

---

## 1. Measured variants (all Hud-only, real C++)

Each row: `cmake --build cmake-build-match --target link_so` then
`tools/verify/verify.py` (fuzzy % over the normalised instruction stream).

### 1.1 Faithful dock order (`P B T W H D`), declaration-type cross product

| variant | size | pct |
|---|---:|---:|
| landed (`T P B H W D`, `short`/`short`) | 9332 | **94.9** |
| faithful, `int frameH` + `int transferred` | 9324 | 93.6 |
| faithful, `short frameH` + `short transferred` | 9324 | 93.5 |
| faithful, `short frameH` + `int transferred` | 9324 | 93.8 |
| faithful, `int frameH` + `int16` cast | 9324 | 93.8 |
| faithful, `unsigned int frameH` | 9324 | 93.6 |
| faithful, `long frameH` | 9324 | 93.6 |
| faithful, `(int)` cast on frameH | 9324 | 93.6 |
| faithful, `int frameH; frameH = ...` | 9324 | 93.6 |
| faithful, `int frameH, textH;` then assign | 9324 | 93.6 |
| faithful, `unsigned int transferred` | 9324 | 93.6 |
| faithful, both `unsigned int` | 9324 | 93.5 |

None of the type variations moves the `frameH` slot from `#92` to `#96`.

### 1.2 Dock statement-order re-sweep on the current (wave-112) base

`T`=`String text`, `P`=`progress`+`if`, `B`=`barOffset`, `W`=`barW`,
`H`=`barH`; `dt/fade` always last. Landed block is `T P B H W D`.

| order | size | pct |
|---|---:|---:|
| `T P B H W D` (landed) | 9332 | **94.9** |
| `T B P H W D` | 9332 | **94.9** |
| `T P B W H D` | 9332 | 94.7 |
| `T B P W H D` | 9332 | 94.7 |
| `P B T H W D` | 9324 | 93.7 |
| `P B T W H D` (faithful) | 9324 | 93.5 |
| `P T B H W D` | 9340 | 91.7 |
| `P T B W H D` | 9340 | 91.5 |
| `T B H P W D` | 9344 | 89.8 |
| `T P H W B D` | 9348 | 84.9 |

The local optimum is unchanged; the full 720-permutation sweep from wave 112
still stands.

### 1.3 `top` / `barYOffset` declaration position (faithful base)

| variant | size | pct |
|---|---:|---:|
| top+barYOffset before `String text` | 9332 | 94.6 |
| top+barYOffset before `String text`, `int top` | 9332 | 94.6 |
| top before `dt` | 9324 | 93.7 |
| top before `barW` | 9324 | 93.7 |
| top between `barW`/`barH` | 9324 | 93.6 |
| top after `dt` (faithful) | 9324 | 93.6 |
| top after `fade` | 9324 | 93.6 |
| barYOffset before top | 9324 | 93.5 |
| top before `String text`, barYOffset after `fade` | 9328 | 93.5 |

Moving `top`/`barYOffset` before `String text` is the only shape that makes the
faithful dock block size-exact (9332), but it also moves the `field_0x3e2` load
before `getText`, so the instruction order no longer matches the original.

### 1.4 Second bar (`float filled`) and other statement shapes

| variant | size | pct | Hud LSDA |
|---|---:|---:|---:|
| `float filled = progress * (float) halfW;` (landed) | 9332 | **94.9** | 480 |
| `const float filled = ...` | 9332 | 94.9 | 480 |
| `float filled = progress * halfW;` | 9332 | 94.9 | 480 |
| `float filled;` declared before `SetColor`, assigned after `DrawImage2D` | 9356 | 84.3 | **476** |
| `float filled = ...` before `SetColor` | 9356 | 83.8 | 476 |
| faithful + `const float filled` | 9324 | 93.6 | 480 |
| faithful + `float filled = progress * halfW` | 9324 | 93.6 | 480 |
| faithful + `float filled;` hoisted before `SetColor` | 9348 | 84.3 | **476** |

The `float filled;`-before-`SetColor` form **does** merge the Hud LSDA row to
the original's 476 B, but the total `.ARM.extab` then becomes `0x26294`
(Generator's `-4` is no longer cancelled), `.text` grows to `0x1337e8`, and the
fuzzy score collapses to 84.3 %. This is the shape the wave-113 hint refers to:
the second-bar merge is real, but it must be paired with a dock-block shape that
adds +4 extab back (or with a Generator fix), and no such docking shape was
found.

### 1.5 Gamma-ray block (`Status *st`) — the reload is real

The original's gamma block is:

```
r6 = &Globals::status
r4 = *r6                      ; st
r0 = r4; getStation(); getIndex() -> r5
r0 = *r6                      ; RELOAD Globals::status
str r6,[sp,#108]
getCurrentCampaignMission() -> r2
r0 = r4; r1 = r5; getGammaRayDamagePerSecond()
```

This is exactly what the source

```c++
Status *st = Globals::status;
if (st->getGammaRayDamagePerSecond(st->getStation()->getIndex(),
                                   Globals::status->getCurrentCampaignMission()) > 0.0f)
```

produces (`Globals::status` only in the second argument). Measured:

| variant | size | pct |
|---|---:|---:|
| landed (`st->` for all three) | 9332 | **94.9** |
| `Globals::status` only in arg2 (block matches original) | 9312 | 75.0 |
| `Globals::status` for all three (no `st`) | 9312 | 75.0 |
| `Status *&st = Globals::status;` | 9312 | 75.0 |
| `Globals::status` for the outer object only | 9332 | 94.9 |
| `Globals::status` for arg1 only | 9332 | 94.9 |
| local `int stationIndex/campaignMission` | 9332 | 94.9 |

The correct gamma block costs 8 bytes of stack frame (`sub sp,#216` vs `#224`,
all slots shifted by 8) and 20 bytes of code; the `st`-local shape keeps the
frame but emits the wrong (CSE'd) reload. The two cannot be combined with the
source forms tried.

### 1.6 Boost-button colour select

Original emits `ite pl; mvnpl r1,#255; mvnmi r1,#208` (both arms materialised,
Canvas loaded *after* `getBoostRate`). Ours emits `it mi; mvnmi r1,#208` with
the default hoisted, and loads Canvas before the call.

| variant | size | pct |
|---|---:|---:|
| landed `rate < 1.0f ? 0xffffff2f : 0xffffff00` | 9332 | **94.9** |
| `rate >= 1.0f ? 0xffffff00 : 0xffffff2f` | 9332 | 94.8 |
| `unsigned int c = rate < 1.0f ? ...; SetColor(c);` (braced) | 9324 | 93.3 |
| `unsigned int c = rate >= 1.0f ? ...; SetColor(c);` (braced) | 9324 | 93.3 |
| if/else into a local, then `SetColor(c)` | 9324 | 93.3 |

### 1.7 `Globals::status` in the 4th damage indicator

The original has no null check before `field_0x4b8` (`ldr.w r0,[sl,#1208];
cmp r0,#1; blt`), while our source has `if (Globals::status && ...)`.

| variant | size | pct |
|---|---:|---:|
| landed `Globals::status &&` | 9332 | **94.9** |
| remove the check | 9264 | 83.6 |
| `st &&` (local) | 9368 | 63.9 |
| `Globals::status &&`, operands swapped | 9336 | 88.6 |

---

## 2. What the remaining deltas are

For the landed 94.9 % shape the difflib opcodes (normalised stream, 3149 vs
3155 instructions) are dominated by:

* **dock block** (`0x1cfc..0x1e90`): the `String text` operator+ is scheduled
  before the float math (source `T` before `P`), so 12+6 opcode lines differ;
* **second bar** (`0x1f00..0x21b2`): `filled`/`barH`/`barW` register choice and
  the `Globals::gameText` pointer kept in `fp` instead of spilled;
* **gamma block** (`0x480`): CSE'd `Globals::status` load;
* **boost block** (`0x1ade..0x1b88`): `ite` vs `it` and Canvas load order;
* **mission panel** (`0x1f00`, `0x1f44`): same `Globals::gameText`-in-`fp`
  effect as the dock block;
* **tail branch labels** (`loc_2444` vs `loc_2446`): a 2-byte intra-function
  shift that survives even when the function size is exact.

The single root cause behind the dock block, the mission panel and part of the
second bar is the **`Globals::gameText` GOT pointer staying in `fp`** in our
build. In the original it is spilled to `sp#96` (and to `sp#92` for the font
GOT pointer) because `fp` is used for `barH`. Reproducing that requires the
original's `barH -> fp` allocation, which in turn requires `barW -> r8` and the
`mov r5,fp` Canvas save. No source-level shape tried (statement order, `int` vs
`short` vs `unsigned`, `top`/`barYOffset` position, `String` temporary form,
`filled` form) moved the allocator to that assignment while keeping the
function size at 9332 B.

---

## 3. Files / scratch

Scratch harness (outside the repo): `/dev/shm/w6116/` — `Hud.cpp.landed`
(94.9 % baseline), `Hud.cpp.faith` (faithful dock order), `score.sh`,
`batch11.py`…`batch19.py`, `blocks4.py` (difflib opcode dump with addresses),
`lsda_dump.py`.
