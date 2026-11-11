# Small tail wave 175 (w11920): re-sweep of the four residual tails

Status: **no `src/` change landed**.  Worktree `wt/w11920`, branch
`wave175-w11920`, base master `5f599a137`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11920`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` byte-identical (0).
Measured whole-`.text` differing bytes **80,439** (`.text` size 1,259,464 on
both sides); `.ARM.extab` 750 (size/base exact), `.ARM.exidx` 0, `.rodata`
0, `.data` 0.  (The task brief quoted 80,447; the checked-in HEAD
`5f599a137` already measures 80,439.)

Baselines re-confirmed exactly with `tools/fn_diff.py`:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 40 | 21 | 1996 |
| `_ZN10ModStationC1Ev` / `C2Ev` | 39 | 11 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`.  All other
target TUs were restored to base before each measurement (a size change in
one TU shifts the later functions' link addresses and inflates the
same-address raw metric).

## Generator::createMission — re-confirmed 40/21/1996, no new win

The residual is unchanged from waves 172–174: the frame is 104 vs the
original 96, `rem` sits at `sp+44` vs `sp+32`, `bonus` at `sp+96` vs the
original's reused `fromSystem` slot `sp+16`, and our `cbz` differs from the
original's `cmp/it/subne`.  The wave-172 volatile-anchor form is still best:

```cpp
volatile int bonus;
volatile int bv;          // volatile anchor: fixes r6/r5 assignment
...
int remVal = rawBonus % 50;
int b = rawBonus + remVal;
volatile int rem = remVal;
if (b % 50 != 0) {
    b = rawBonus - rem;
}
bv = rawBonus;            // volatile anchor (dead store)
bonus = b;
```

### New finding this wave: the "plain bonus" shape is one instruction short of exact

Dropping the volatile `bonus` (plain `int bonus`) lets the allocator reuse
the dead `fromSystem` slot: `bonus` lands at `sp+16` exactly as in the
original, and the frame drops to 96.  With a plain `int bonus` and a
volatile `rem` (no `bv` anchor) the block is still branchy (`cbz`), but an
**unconditional volatile reload** forces the original `cmp/it/subne`:

```cpp
int bonus;
...
int remVal = rawBonus % 50;
int b = rawBonus + remVal;
volatile int rem = remVal;
int r = rem;              // unconditional volatile read -> reload hoisted
if (b % 50 != 0) {
    b = rawBonus - r;
}
bonus = b;
```

This produces the original bonus block byte-for-byte
(`adds r5,r1,r6; str r1,[sp,#44]; ...; ldr r0,[sp,#44]; cmp r1,#0; it ne;
subne r5,r6,r0; str r5,[sp,#16]`) with the correct `r6 = rawBonus` /
`r5 = b` split and frame 96 — but the function is **2000 B** (raw 90,
aligned 31, 746 insns vs 744).  The extra 4 bytes are one extra
landing-pad `b.n` stub plus one `nop` literal-pool pad.  The LSDA length and
entry count are identical to the original (dumped at `0x1a78c4`); only the
landing-pad block placement differs, so the extra stub is a
MachineBlockPlacement artifact, not an EH-table difference.  Every
formulation of this shape (with or without `int r`, ternary, truthy
condition, rem declared before `b`, `*(volatile int *)&rem`, fences at four
positions, `rem` at function scope) stays at 2000 B / raw 90–762.

### Shape table (all size 1996 unless noted; landed baseline 40/21)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master (landed) | **40** | **21** | 1996 |
| `bv = rawBonus` after `bonus = b` | 41 | 21 | 1996 |
| `volatile int &bv = bonus` (reference alias) | 114 | 96 | 1996 |
| `volatile int *bv = &bonus; *bv = rawBonus` | 114 | 96 | 1996 |
| landed + `*(volatile int *)&remVal` in subtraction | 40 | 21 | 1996 |
| plain `int bonus` + `*(volatile int *)&fromSystem = rawBonus` | 767 | 182 | 2004 |
| plain `int bonus` + `*(volatile int *)&toSystem = rawBonus` | 747 | 182 | 2004 |
| `volatile int rem = rawBonus % 50` (no `remVal`) | 614 | 67 | 2000 |
| `b = rawBonus - rem * (b % 50 != 0)` | 619 | 68 | 2004 |
| `volatile int b` + volatile `bonus`, no `bv` | 599 | 72 | 2000 |
| plain `rem` + volatile `bv` + volatile `bonus` | 644 | 121 | 2004 |
| plain `int bonus` + volatile `bv` + volatile `rem` | 758 | 171 | 2000 |
| plain `int bonus` + volatile `rem`, no anchor | 716 | 88 | 2000 |
| plain `int bonus` + volatile `rem` + `int r = rem` | 90 | 31 | 2000 |
| same, ternary / truthy / rem-before-b / cast / fence | 90 | 31 | 2000 |
| same, subtraction uses `remVal` | 669 | 143 | 2004 |
| landed + `int r = rem` reload | 672 | 154 | 2008 |
| plain `rem` + `*(volatile int *)&remVal` in subtraction | 716 | 88 | 2000 |
| plain `rem` + `volatile int *remp = &remVal` | 716 | 88 | 2000 |
| plain `rem` + volatile read hoisted before the `if` | 90 | 31 | 2000 |
| plain `int bonus` + volatile `rem` at function scope | 762 | 170 | 2000 |
| `int bonus = 0` initializer + `if ((type|4) != 12)` | 716 | 88 | 2000 |
| plain `int bonus` + volatile `rem` at top + `int r` | 169 | 113 | 2000 |
| plain `int bonus` + volatile `rem` + fence after rem | 715 | 87 | 2000 |
| plain `int bonus` + volatile `rem` + fence before store | 716 | 88 | 2000 |
| plain `int bonus` + volatile `rem` + fence before `if` | 715 | 87 | 2000 |

Conclusion: the original's `bonus` is a plain `int` reusing the dead
`fromSystem` slot and `rem` is a plain `int` spilled to the dead `status`
slot; clang 7 `-Oz` only reaches that block shape with a volatile `rem`,
which costs the extra landing-pad stub.  A 4-byte win requires removing
that stub while keeping frame 96 — no source shape tested does.

## ModStation::ModStation C1/C2 — re-confirmed 39/11/1008, no new win

The residual is the nearKey yaw-table select.  The original materialises
`YawIPad` into `r3` (pool slot `0xd56b4`) and `Yaw` into `r4` (slot
`0xd56b8`); ours materialises `Yaw` into `r4` (slot `0xd56b4`) and
`YawIPad` into `r3` (slot `0xd56b8`).  The tables themselves are at the
same `.rodata` addresses in both binaries (`gHangarCamPitch` 0x1fe030,
`gHangarCamYawIPad` 0x1fe058, `gHangarCamYaw` 0x1fe080); only the
`ldr [pc]` / `add pc` schedule and therefore the literal-pool addends and
slot order differ.  The wave-173 compiler barrier is still required
(without it raw is 43).

New shapes this wave (all size 1008, C1/C2 aliased):

| shape | raw | aligned |
|---|---:|---:|
| master (fence + `iPad ? YawIPad : Yaw`) | **39** | **11** |
| `!iPad ? Yaw : YawIPad` | 39 | 11 |
| separate `yawIPadTable` / `yawStdTable` locals | 39 | 11 |
| `yawIPadTable` declared before `pitchTable` | 42 | 12 |
| `yawTable` load before `pitchBits` | 752 | 40 (1000 B) |
| `(iPad != 0)` condition | 752 | 40 (1000 B) |
| fence after `yawTable` | 44 | 18 |

The literal-pool slot order is a hard scheduling/RA tie: every source
spelling that keeps the 1008-byte frame canonicalises to the same
`Yaw`-first schedule.

## MenuTouchWindow::OnTouchBegin — re-confirmed 28/16/1896, no new win

The 28 raw bytes are the two known greedy-RA ties:

1. **case 8**, first if, fourth term: original
   `ldr.w ip,[sl,#340]; add r3,ip`, ours `ldr.w r4,[sl,#340]; add r3,r4`
   (6 raw bytes).  `sl = this`, `[sl,#340] = this->listEntryWidth`; the
   value is identical, only the destination register differs.
2. **case 15**, the scrollbar-hit block: the whole `w - buttonInsetX -
   width` and `top + (leftMargin + height)` loads are register-permuted
   (22 raw bytes).

Wave-173/174 shapes remain at 28 or worse; no new shape tested this wave
moves either tie without losing the 1896-byte frame.  Both are allocator
choices with no source-level handle (a single destination-register nibble
and a permutation of otherwise identical loads).

## MGame::OnUpdate — re-confirmed 16/6/11440, no new win

The residual is the m122 radio block: the original loads `this->level`
after `getCurrentCampaignMission()` into `r1`, ours hoists it before the
call into `r5`.  Any shape that removes the cross-call `level` live range
drops the whole function to 11392 B / 3974 insns (the shared
`inBlackMarketSystem` tail loses 24 instructions), so the 11440 anchor
dominates.

Full declaration-order permutation sweep this wave (`radioIndex` / `level`
/ `radioBase`, all 6 orders):

| order | raw | aligned | size |
|---|---:|---:|---:|
| `radioIndex, level, radioBase` (master) | **16** | **6** | 11440 |
| `level, radioIndex, radioBase` | 33 | 8 | 11440 |
| `level, radioBase, radioIndex` | 1239 | 170 | 11436 |
| `radioBase, radioIndex, level` | 1241 | 154 | 11436 |
| `radioBase, level, radioIndex` | 1240 | 170 | 11436 |
| `radioIndex, radioBase, level` | 5196 | 597 | 11392 |

The 16 raw bytes are the `ldr`/`blx` and `movw`/`ldr` swaps plus the
`r1`/`r2`/`r5` split; the block is the same 30 bytes on both sides, so the
function size is exact.

## Conclusion

No measured win this wave.  The four tails are unchanged at
**40/21 (1996), 39/11 (1008), 28/16 (1896), 16/6 (11440)** raw/aligned/size.
`src/` is at base `5f599a137`; only this note is added.  Whole-`.text`
differing bytes stay at **80,439** (`.text` size 1,259,464 on both sides).
Full `check` is green: byte_exact 4507, linked_exact 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` identical.

The remaining levers, in order of expected payoff:

1. **Generator**: remove the one extra landing-pad `b.n` stub from the
   plain-`bonus` + volatile-`rem` shape (frame 96, bonus at `sp+16`,
   `cmp/it/subne`) without growing the function; that shape already has
   the correct bonus block and only needs the 4-byte landing-pad/literal
   pool fix to become near-exact.
2. **ModStation**: flip the `YawIPad`/`Yaw` literal-pool slot order while
   keeping the 1008-byte frame.
3. **MenuTouchWindow**: move the case-8 `listEntryWidth` load from `r4`
   to `ip`, and re-permute the case-15 scrollbar block.
4. **MGame**: keep `status` in `r9` while loading `level` after the call.
