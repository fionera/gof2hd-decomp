# Small tail wave 176 (w12020): four residual tails, LSDA-level analysis

Status: **no `src/` change landed**.  Worktree `wt/w12020`, branch
`wave176-w12020`, base master `b21088985`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12020`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` byte-identical (0).
Whole-`.text` differing bytes **80,166**; `.ARM.extab` 746 (size/base exact);
`.ARM.exidx` 0; `.rodata` 0; `.data` 0.

Baselines re-confirmed exactly with `tools/fn_diff.py`:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 40 | 21 | 1996 |
| `_ZN10ModStationC1Ev` / `C2Ev` | 39 | 11 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`, one target
TU at a time (the other three restored to base before each measurement).

## New tool this wave: ARM EHABI LSDA decoder

The residual landing-pad layout is now understood at the LSDA level.  The
`.ARM.extab` generic-model entry is `personality word` followed by the LSDA
at `entry+4`.  The personality word is a 24-bit signed offset from the word
itself (`0x7feb729c` at `0x1a78c4` -> `0x1a78c4 - 0x148d64 = 0x5eb60`).
The LSDA header at `0x1a78d4` is `ff ff 01 97 01` = LPStart omit, TType
omit, call-site encoding `uleb128`, call-site table length 151.  A 30-line
decoder is enough:

```python
def uleb(d,o):
    r=s=0
    while True:
        b=d[o]; o+=1; r|=(b&0x7f)<<s
        if not (b&0x80): break
        s+=7
    return r,o
# d = bytes at 0x1a78d4; skip ff ff 01, read uleb len, then 26 x 4 ulebs
```

`Generator::createMission` has **26 call sites in both binaries and the same
151-byte table length**, but ours emits **one extra, unreferenced landing
pad**.  Decoded landing-pad offsets (function-relative):

| call site | orig lp | ours lp |
|---|---:|---:|
| cs10 | 0x778 | 0x778 |
| cs11 | 0x77a | **0x77c** |
| cs13 | 0x776 | 0x776 |
| cs15 | 0x77c | **0x77e** |

Ours has an extra `b.n` stub at `0x77a` (`0x97932`) that **no LSDA entry
references**; cs11's pad moved to `0x77c` and cs15's to `0x77e`.  The extra
2-byte stub plus the 2-byte literal-pool `nop` pad is exactly the 4-byte
size excess of the wave-175 "plain bonus" shape.

## Generator::createMission — re-confirmed 40/21/1996, no new win

The wave-175 plain-`int bonus` + `volatile int rem` + `int r = rem` shape
was rebuilt and re-confirmed: **raw 90, aligned 31, size 2000**, frame 96,
`bonus` at `sp+16`, exact `cmp/it/subne` bonus block.  The only remaining
difference is the `rem` slot (`sp+44` vs the original's reused status slot
`sp+32`) plus the extra unreferenced landing pad above.

New observations this wave:

* The extra pad is **not** caused by the volatile `rem` slot: the
  `int race2 = agent->getRace();` hoist also removes it (size 1996) while
  `remVal` stays at `sp+44`.  It is a MachineBlockPlacement artifact of the
  `getStanding`/`getRace`/`getMissionBonus` invoke region.
* Several `__builtin_expect` placements remove the pad but change real code
  (each is size-exact 1996, no good):

  | expect site | raw | aligned | size |
  |---|---:|---:|---:|
  | `(type \| 4) == 12`, 0 | 1098 | 106 | 1996 |
  | `Globals::status->getCurrentCampaignMission() <= 15`, 0 | 1188 | 97 | 1996 |
  | `type == 6`, 0 | 1755 | 136 | 1996 |
  | final-loop `stationIsInSystem`, 0 | 1792 | 150 | 1996 |
  | `type == 12`, `difficulty >= 10`, `(reward+rem)%50`, `(costs+costRem)%50` | 40 | 21 | 2000 (no change) |

* Full expect sweep over 19 branch sites on the landed base: only
  `pick == 12` (74), `type == 11` (84), `type == 3` (146), `type == 5`
  (190), `(type|4)==12` (654) change raw, all worse; nine sites leave the
  landed 40/21/1996 untouched.
* `volatile short/char rem` -> 598/2000; `volatile long long rem` ->
  666/2004; `bv = rawBonus {+0,-0,^0,|0,&-1,*1,/1}` all 40/1996 (no-op
  after folding).
* `int race2 = agent->getRace();` hoist: 579/34/1996 (pad gone, call order
  wrong).  `Standing *standing` local: 94/31/2000.  Both together:
  100/33/2004.  `standing` on the landed base: 121/48/2000.

Conclusion unchanged: the original's plain-`int` `remVal` spills to the
dead status slot `sp+32`; clang 7 `-Oz` only reaches the `cmp/it/subne`
block with a volatile read, which adds a stack slot and an EH block that
costs one 2-byte pad + one 2-byte nop.

## ModStation::ModStation C1/C2 — re-confirmed 39/11/1008, no new win

The residual is still the nearKey yaw-table select: original materialises
`YawIPad` in pool slot `0xd56b4` and `Yaw` in `0xd56b8`; ours is reversed
(`Yaw` first).  The register assignment is identical (`r3 = YawIPad`,
`r4 = Yaw`, `moveq r0,r4`); only the `ldr [pc]`/`add pc` schedule and pool
slot order differ.

New shapes this wave (all size 1008, C1/C2 aliased):

| shape | raw | aligned |
|---|---:|---:|
| master (fence + `iPad ? YawIPad : Yaw`) | **39** | **11** |
| `int ipad = Globals::iPad;` before `pitchTable` | 39 | 11 |
| `bool ipad = Globals::iPad != 0;` | 39 | 11 |
| `Globals::iPad != 0 ? ...` | 39 | 11 |
| `Globals::iPad == 0 ? Yaw : YawIPad` (inverted) | 39 | 11 |
| `__builtin_expect(Globals::iPad, 0/1)` | 39 | 11 |
| `__builtin_expect(Globals::iPad != 0, 0)` | 39 | 11 |
| `if (__builtin_expect(Globals::iPad, 1)) yawTable=YawIPad; else yawTable=Yaw;` | 39 | 11 |
| `yawIPadTable` local declared before `pitchTable` | 42 | 12 |
| `yawTable` declared before `pitchTable` | 42 | 12 |
| `float pitch = pitchTable[race]; float yaw = yawTable[race];` | 631 | 72 |
| non-volatile `*(const int *)` bit-cast | 631 | 72 |
| `pitchBits` before `yawTable` declaration | 698 | 52 (1000) |
| `yawBits` before `pitchBits` | 752 | 40 (1000) |
| farKey rotation uses `yawTable[race]` | 604 | 70 (984) |

The pool order is invariant under every source spelling that keeps the
1008-byte frame: clang 7 always emits the `moveq` false operand (`Yaw`)
first.  A 28-byte win needs the two `ldr`/`add` pairs flipped without
changing the frame; no source shape tested does.

## MenuTouchWindow::OnTouchBegin — re-confirmed 28/16/1896, no new win

The 28 raw bytes remain exactly the two known greedy-RA ties:

1. case 8, fourth term: `ldr.w ip,[sl,#340]` (orig) vs `ldr.w r4,[sl,#340]`
   (ours), 6 bytes.  `sl = this`, `[sl,#340] = this->listEntryWidth`; `r4`
   is dead at that point in both.
2. case 15 scrollbar-hit block: a 5-register permutation
   `heapBufB/field_0x1e0/layout/w/buttonInsetX` = orig `r3/r6/r2/r5/r2`,
   ours `r2/r3/r6/r4/r5` (22 bytes).  `&Globals::layout` is hoisted into
   `r4` (orig) vs `r5` (ours) before the `scrollSlots` loop.

New shapes this wave (all size 1896):

| shape | raw | aligned |
|---|---:|---:|
| master | **28** | **16** |
| case-8 4th term all 12 parenthesised operand permutations | 28-33 | 16-17 |
| case-8 `int entryH = this->listEntryHeight;` local | 28 | 16 |
| case-8 `int entryW = this->listEntryWidth;` local | 62 | 26 |
| case-8 `(int)` cast of `listEntryWidth` | 28 | 16 |
| case-15 `Layout *layoutPtr = Globals::layout;` | 28 | 16 |
| case-15 all-`Globals::layout` (no local) | 362 | 76 (1892) |
| case-15 all-`layout` local reference | 308 | 73 (1892) |
| case-15 `w - (inset + width)` | 90 | 26 |
| case-15 width-before-inset | 337 | 85 (1888) |
| case-15 `inset + width < w - x` | 359 | 93 (1892) |
| case-15 `x + inset + width > w` | 359 | 94 (1892) |
| case-15 `__atomic_signal_fence` before the first if | 806 | 130 (1892) |

Both ties are allocator choices with no source-level handle that keeps the
1896-byte frame.

## MGame::OnUpdate — re-confirmed 16/6/11440, no new win

The residual is still the m122 radio block (the rest of the 11,440-byte
function is byte-identical):

```
orig  ldr.w r0,[r9]        ; status
      blx   getCurrentCampaignMission
      movw  r2,#3166
      ldr.w r1,[fp,#120]   ; this->level  (loaded after the call)
      cmp   r0,#122
      it    eq
      movweq r2,#3164
      add   r2,r4          ; radioBase + radioIndex
      mov   r0,r1
      movs  r1,#28
      blx   createRadioMessage
ours  ldr.w r0,[r9]
      ldr.w r5,[fp,#120]   ; level hoisted before the call
      blx   getCurrentCampaignMission
      movw  r1,#3166
      cmp   r0,#122
      it    eq
      movweq r1,#3164
      mov   r0,r5
      adds  r2,r1,r4
      movs  r1,#28
      blx   createRadioMessage
```

New shapes this wave (all size 11440 unless noted):

| shape | raw | aligned |
|---|---:|---:|
| master (`radioIndex, level, radioBase`) | **16** | **6** |
| `unsigned radioIndex` / `unsigned radioBase` | 16 | 6 |
| `(int)(radioIndex + radioBase)` | 16 | 6 |
| `Level *level;` then `level = this->level;` after radioBase | 5196 | 597 (11392) |
| `*(Level *volatile *)&this->level` after radioBase | 5196 | 597 (11392) |
| `Status *status = Globals::status;` before `level` | 5196 | 597 (11392) |
| `Level *level` declared first | 33 | 8 |
| `if (level) level->createRadioMessage(...)` | 5334 | 759 (11504) |
| enclosing-condition `__builtin_expect(ccm() >= 122, 1)` | 1023 | 211 |
| enclosing-condition `__builtin_expect(!jumpDriveActive && !usingJumpDrive, 0)` | 1558 | 267 |
| `volatile int radioIndex` | 5525 | 651 (11396) |
| `volatile int radioBase` | 1526 | 225 (11444) |
| commuted `radioBase + radioIndex` | 705 | 116 |

The 11440 anchor requires the cross-call `level` live range; any shape that
loads `level` after the call (the original's order) re-colours `status`
from `r9` to `r5` and the shared `inBlackMarketSystem` tail loses 24
instructions (11392 B / 3974 insns).  Forcing `status` back to `r9` while
loading `level` after the call is the only remaining lever; no source shape
tested does it.

## Conclusion

No measured win this wave.  The four tails are unchanged at
**40/21 (1996), 39/11 (1008), 28/16 (1896), 16/6 (11440)** raw/aligned/size.
`src/` is at base `b21088985`; only this note is added.  Whole-`.text`
differing bytes stay at **80,166**; `.ARM.extab` 746, `.rel.plt` identical.
Full `check` green: byte_exact 4507, linked_exact 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.

Remaining levers, in order of expected payoff:

1. **Generator**: remove the one unreferenced landing pad at `0x97932`
   (LSDA cs11 pad) from the plain-bonus + volatile-`rem` shape without
   changing the `getStanding`/`getRace`/`getMissionBonus` call order.  That
   shape already has the exact bonus block and frame 96; the pad is the
   only 4-byte excess.
2. **ModStation**: flip the `YawIPad`/`Yaw` literal-pool slot order while
   keeping the 1008-byte frame (all tested spellings canonicalise to
   `Yaw`-first).
3. **MenuTouchWindow**: move case-8's `listEntryWidth` load from `r4` to
   `ip`, and re-permute the case-15 scrollbar block.
4. **MGame**: keep `status` in `r9` while loading `level` after
   `getCurrentCampaignMission()`.
