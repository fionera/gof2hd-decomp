# Hud::draw — wave 126 (w7614): gamma `Globals::status` reload vs the 224-byte frame

**Status:** research note, 2026-11-07 (wave 126). Worktree `wt/w7614`, base
`master bf365a964`. **No source change landed**: the best measured variant is
still HEAD at **9332 B / 96.600000 %** (85 diff hunks). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`.

This note extends `hud-draw-wave123-w7314.md` (boost if/else, `Globals::status`
anchor, `short barW`) and `hud-draw-wave125-w7520.md` (dock1 local type
tie-breaks). The w6923 note already measured the gamma reload; this pass
re-measures it on the wave-125 allocator and isolates *why* it loses.

---

## 0. TL;DR

The gamma block is the only place where our instruction stream is structurally
wrong, and the faithful spelling *does* reproduce the original's gamma block
byte-for-byte:

```cpp
if (st->getGammaRayDamagePerSecond(st->getStation()->getIndex(),
                                   Globals::status->getCurrentCampaignMission()) > 0.0f)
```

The original re-reads `Globals::status` for the mission argument
(`ldr r0,[r6,#0]` at `0x164028`); HEAD uses `st->getCurrentCampaignMission()`
(`mov r0,r4`).  The faithful spelling matches the original's 19-instruction
gamma block exactly, but it **shrinks the whole stack frame from 224 to 216
bytes** (one 4-byte spill slot + 8-byte alignment), which moves every
`[sp,#N]` operand by 8 and collapses the official score to **75.2 %**
(538 hunks).  The `st` use for the mission call is load-bearing for the
original's 224-byte frame.

Hoisting the station index (`int stationIdx = ...;`) restores 9332 B and the
reload *and* the `this` split, but the receiver becomes `Globals::status`
load #2 (`mov r0,r5`) instead of load #1 (`mov r0,r4`): **96.5 % / 86 hunks**,
0.1 pt below HEAD.  No spelling found keeps both the 224-byte frame and the
load-#1 receiver.

---

## 1. Measured baseline

```
_ZN3Hud4drawExxP9PlayerEgobjj orig=9332 ours=9332
  ratio=96.600000  insns=3149/3152  hunks=85
```

The 85 hunks are: gamma block (5), shield/armour spill schedule (16),
dock1 register/type tie-breaks (~40), second-bar scheduling (~20), EH pad (4).

---

## 2. Gamma block: faithful reload matches, then breaks the frame

Original `0x164010` (instruction indices from the verify normaliser):

```
366 ldr  r6,[r0,#0]   ; r6 = &Globals::status
367 ldr  r4,[r6,#0]   ; load #1 -> getStation + receiver
368 mov  r0,r4
369 blx  Status::getStation
370 blx  Station::getIndex
371 mov  r5,r0
372 ldr  r0,[r6,#0]   ; load #2 -> mission
373 str  r6,[sp,#108]
374 blx  Status::getCurrentCampaignMission
375 mov  r2,r0
376 mov  r0,r4        ; receiver = load #1
377 mov  r1,r5
378 blx  Status::getGammaRayDamagePerSecond
```

HEAD (`Status *st`, three uses) is semantically identical but uses `r4` for the
mission call (`mov r0,r4`, no reload) and spills `&Globals::status` early.

Faithful variant:

```cpp
    Status *st = Globals::status;
    if (st->getGammaRayDamagePerSecond(st->getStation()->getIndex(),
                                       Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

produces the original block exactly (same 19 instructions, same registers
`r6`/`r4`/`r5`, same `ldr r0,[r6,#0]` reload), but:

| build | size | insns | raw % | hunks |
|---|---:|---:|---:|---:|
| HEAD (`st` for mission) | 9332 | 3149/3152 | **96.600000** | **85** |
| faithful reload (`Globals::status` for mission) | 9312 | 3149/3146 | 75.200000 | 538 |
| `Globals::status` for station + mission, `st` receiver | 9312 | 3149/3146 | 75.200000 | 538 |
| plain 3× `Globals::status` (no `st`) | 9312 | 3149/3146 | 75.200000 | 538 |
| `float gamma = ...; if (gamma > 0.0f)` | 9312 | 3149/3146 | 75.200000 | 538 |
| two locals `st`, `st2` (CSE'd) | 9332 | 3149/3152 | 96.600000 | 85 |

### Why the frame shrinks

Stack-slot census of the original vs the faithful reload (absolute `[sp,#N]`
offsets used anywhere in the function):

```
orig slots: 0 4 8 12 16 20 24 28 32 40 60 64 68 72 76 80 84 88 92 96 100 104 108 112 116 120 160 220
reload    : 0 4 8 12 16 20 24 28 32 40 56 60 64 68 72 76 80 84 88 92  96 100 104 108 112 152 212
```

The reload variant has **one fewer 4-byte local** in the middle band and the
whole frame is 8 bytes smaller (`sub sp,#216` vs `#224`); `addeq sp,#216` vs
`#224` in the epilogue.  HEAD's slot set is *identical* to the original's.
The DWARF location lists contain no `DW_OP_fbreg` at all — every named local is
in a register — so the missing slot is a register-allocator spill, not a named
source local.  The `st` local's use for the mission call is what makes the
allocator reserve the extra slot; the faithful reload removes that pressure.

---

## 3. Hoisted station index: reload + split, wrong receiver

```cpp
    int stationIdx = Globals::status->getStation()->getIndex();
    if (Globals::status->getGammaRayDamagePerSecond(stationIdx,
                                                    Globals::status->getCurrentCampaignMission()) > 0.0f) {
```

| variant | size | raw % | hunks | reload | `this` split |
|---|---:|---:|---:|---|---|
| hoisted index, `Globals::status` receiver | 9332 | 96.500000 | 86 | yes | yes |
| `Status *&st = Globals::status;` + hoisted index | 9332 | 96.500000 | 86 | yes | yes |
| `Status *const &st = ...` + hoisted index | 9332 | 96.500000 | 86 | yes | yes |
| `Status *st` + hoisted index + `Globals::status` arg2 | 9312 | 75.200000 | 538 | yes | no |

The hoisted shape restores the 224-byte frame and the two `Globals::status`
loads, but C++ evaluates the receiver after the hoisted index, so the receiver
is **load #2** (`mov r0,r5`, `mov r1,r4`) instead of the original's load #1
(`mov r0,r4`, `mov r1,r5`).  That is three extra hunks and costs 0.1 pt.

---

## 4. Second bar: faithful `String`-before-`halfW` order

The original constructs `String text` before calling `GetImage2DWidth`
(`0x165bcc..0x165bd4`); HEAD emits `halfW` first.  Swapping is faithful but
changes the allocator:

| variant | size | raw % | hunks |
|---|---:|---:|---:|
| HEAD (`halfW` first) | 9332 | **96.600000** | **85** |
| `String` first | 9328 | 94.700000 | 121 |
| `String` first + best dock1 types (`int`,`short`,`short`,`short`,`int`) | 9332 | 96.000000 | 97 |
| `String` first + 72 dock1 type/order combos | 9332 | ≤96.000000 | ≥97 |

The best `String`-first combination is still 0.6 pt below HEAD.

---

## 5. Dock1 type sweep (confirms wave 125)

144 combinations of `frameH`×`total`×`barW`×`top`×`barYOffset` type and the
second-bar order were measured against the full link.  The unique maximum is
the landed wave-125 form:

```
short frameH, short total, unsigned short barW, int barH,
short top, unsigned int barYOffset, halfW-first
-> 9332 B / 96.600000 % / 85 hunks
```

Every faithful all-`int` dock form is 9328 B / 94.3-94.7 %; the `short`/`int`
mix is an allocator tie-break, not a faithful type.

---

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7614
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4506
python3 tools/verify/verify.py --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7614`: `hdiff.py` (address-tagged normalised
diff), `score.py` (raw ratio + hunks), `sweep.py` (144-combo dock sweep),
`sweep2.py` (second-bar type sweep), `combo.py` (gamma/order/dock combos).
Baseline source snapshot: `/dev/shm/w7614/Hud.cpp.orig`.  The worktree source
is unchanged from HEAD.
