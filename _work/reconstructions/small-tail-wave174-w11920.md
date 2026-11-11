# Small tail wave 174 (w11920): re-sweep of the four residual tails

Status: **no `src/` change landed**.  Worktree `wt/w11920`, branch
`wave174-w11920`, base master `be9dd429b`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11920`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` byte-identical.

Baselines re-confirmed exactly with `tools/fn_diff.py`:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 40 | 21 | 1996 |
| `_ZN10ModStationC1Ev` / `C2Ev` | 39 | 11 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`.  **All other
target TUs were restored to the base before each measurement**: a size change
in `ModStation.cpp`/`Generator.cpp`/`MGame.cpp` shifts the later functions'
link addresses and inflates the same-address raw metric (e.g. a 4-byte
MenuTouchWindow/Generator size change turns MGame's 16 raw into 705/5196 even
though its code is unchanged).  The numbers below are therefore per-target
isolated.

## ModStation::ModStation C1/C2 — re-confirmed 39/11, no new win

The residual is the nearKey yaw-table select.  The original materialises
`YawIPad` in the first literal-pool slot and `Yaw` in the second; we
materialise `Yaw` first.  Both sides end with the same values in the same
registers (`r3 = YawIPad`, `r4 = Yaw`) and the same select (`mov r0,r3;
cmp; str r3,[sp,#180]; str r4,[sp,#176]; it eq; moveq r0,r4`), but the two
`ldr`/`add` pairs are scheduled in the opposite order, so the literal pool
slots (`0xd56b4`/`0xd56b8`) and the surrounding `add`/`ldrb`/`ldr.w` schedule
differ.  The wave-173 compiler barrier is still required; without it the
function is 43 raw.

New shapes this wave (all size 1008, C1/C2 aliased, unless noted):

| shape | raw | aligned |
|---|---:|---:|
| master (fence + `iPad ? YawIPad : Yaw`) | **39** | **11** |
| `!iPad ? Yaw : YawIPad` | 39 | 11 |
| `iPad == 0 ? Yaw : YawIPad` | 39 | 11 |
| `iPad != 0 ? YawIPad : Yaw` | 39 | 11 |
| `(bool)`/`(int)`/`!!` condition | 39 | 11 |
| `&arr[0]` / `(const float *)` / `+0` operands | 39 | 11 |
| `if (iPad) yawTable=YawIPad; else yawTable=Yaw;` | 39 | 11 |
| `iPad ? (yawTable=YawIPad) : (yawTable=Yaw)` | 39 | 11 |
| fence before `pitchTable` / between tables / before `yawTable` | 39 | 11 |
| two fences | 39 | 11 |
| `(iPad ^ 1) ? YawIPad : Yaw` | 40 | 12 |
| `yawTable` declared before `pitchTable` | 42 | 12 |
| `(iPad & 1)` / `(bool)(iPad & 1)` | 586 | 32 |
| `(iPad + 0)` / `(iPad | 0)` / `(iPad ^ 0)` / `(iPad ? 1 : 0)` | 73 | 21 |
| `(iPad ^ 1) ? Yaw : YawIPad` / `(iPad - 1) ? ...` | 699 | 57 |
| `((iPad + 1) & 1) ? Yaw : YawIPad` | 570 | 24 |
| `yawTable`/`yawBits` declared after `pitchBits` | 698 | 52 |
| `yawBits` before `pitchBits` | 752 | 40 |
| local `yawTables[2]` array + index | 593-635 | 74-78 |

The literal-pool slot order is a hard register-allocation/scheduling tie:
every source spelling that keeps the 1008-byte frame canonicalises to the
same `Yaw`-first schedule.  A 4-byte win would require flipping the two
`ldr`/`add` pairs without changing the frame; no source shape tested does.

## MenuTouchWindow::OnTouchBegin — re-confirmed 28/16, no new win

The 28 raw bytes are exactly the two known greedy-RA ties:

1. **case 8**, first if, fourth term: original
   `ldr.w ip,[sl,#340]; add r3,ip`, ours `ldr.w r4,[sl,#340]; add r3,r4`
   (6 raw bytes).  `sl = this`, `[sl,#340] = this->listEntryWidth`; the value
   is otherwise identical.
2. **case 15**, the scrollbar-hit block: the whole `w - buttonInsetX -
   width` and `top + (leftMargin + height)` loads are register-permuted
   (`r3/r6/r5/r2` original vs `r2/r3/r6/r4/r5` ours), 22 raw bytes.

New shapes this wave (all size 1896 unless noted):

| shape | raw | aligned |
|---|---:|---:|
| master | **28** | **16** |
| case-8 local declaration swap (`buttonInsetX` first) | 30 | 18 |
| case-8 combined declaration | 28 | 16 |
| case-8 first/second/third/fourth term commuted | 28 | 16 |
| case-8 fourth term all 6 `a+b+c` permutations | 28-39 | 16-19 |
| case-8 parenthesised `a+(b+c)` / `(a+b)+c` | 28-33 | 16-17 |
| case-8 `Globals::layout->...` for top/leftMargin | 28 | 16 |
| case-8 local `top`/`leftMargin`/`width` aliases | 143-1356 | 107-209 |
| case-8 hoist `layout = Globals::layout` before if | 30 | 18 |
| case-8 hoist body reloads before if | 874 | 137 (1884 B) |
| case-15 `(leftMargin + top) + height` | 45 | 23 |
| case-15 `(top + leftMargin) + height` | 45 | 23 |
| case-15 `top + leftMargin + height` (no inner parens) | 45 | 23 |
| case-15 `leftMargin + top + height` | 45 | 23 |
| case-15 `w - (buttonInsetX + width)` | 90 | 26 |
| case-15 commuted `w - inset - width < x` | 28 | 16 |

Both ties are allocator choices with no source-level handle: the case-8
`ip`/`r4` pair is a single destination-register nibble, and the case-15
block is a permutation of otherwise identical loads.  The wave-169 29/15
form and the wave-173 shapes remain +1 raw, so master is kept.

## MGame::OnUpdate — re-confirmed 16/6, no new win

The residual is the m122 radio block:

```
orig  ldr.w r0,[r9]        ; status
      blx   getCurrentCampaignMission
      movw  r2,#3166
      ldr.w r1,[fp,#120]   ; this->level
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

The original loads `level` after the call into `r1`; we hoist it into `r5`.
Any source shape that removes the cross-call `level` live range (direct
`this->level->...`, `Level *level` after `radioBase`, `radioBase += ...`,
`Level &level`, `Level *const`, local `Status *status`, address local for
`field_0x100`) drops the whole function to **11392 B / 3974 insns**: the
allocator then colours `status` in `r5` instead of `r9`, and the shared
`inBlackMarketSystem` tail loses 24 instructions.  The 11440 anchor therefore
dominates.

New shapes this wave (all size 11440, 4002 insns, unless noted):

| shape | raw | aligned |
|---|---:|---:|
| master | **16** | **6** |
| `if/else` radioBase | 16 | 6 |
| `!= 122 ? 3166 : 3164` | 16 | 6 |
| `(mission - 122) ? 3166 : 3164` | 16 | 6 |
| `3164 + (mission == 122 ? 0 : 2)` | 16 | 6 |
| commuted `radioBase + radioIndex` | 16 | 6 |
| `radioBase += radioIndex` / `radioIndex += radioBase` | 16 | 6 |
| `int radioArg` local | 16 | 6 |
| `Level *const` / `Level &` / `Level *volatile` (hoisted) | 16 | 6 |
| `Status *status` local in the block | 16 | 6 |
| `Status *const status` at function top, used in block | 11372 | 1818 |
| `long long *pt = &status->field_0x100` hoisted | 11456 | 226 |
| `Status *status` local + `status->getPlayingTime()` next line | 11452 | 230 |
| direct `this->level->...` after `radioBase` | 5196 | 597 (11392 B) |
| `Level *level` after `radioBase` | 5196 | 597 (11392 B) |

The 16 raw bytes are the `ldr`/`blx` and `movw`/`ldr` swaps plus the
`r1`/`r2`/`r5` split; the block is the same 30 bytes on both sides, so the
function size is exact.  Forcing `status` back to `r9` without the cross-call
`level` live range is the only remaining lever and no source shape tested
does it.

## Generator::createMission — re-confirmed 40/21, no new win

The wave-172 volatile-anchor form is still best.  The residual is the frame
difference (104 vs 96), the `rem` slot (`sp+44` vs `sp+32`), the `bonus` slot
(`sp+96` vs the original's reused `fromSystem` slot `sp+16`), and the branchy
`cbz` vs the original's `cmp/it/subne`.  The two volatile anchors (`rem`,
`bv`) are needed for the `r6 = rawBonus` / `r5 = b` split; removing them
loses the split or grows the frame.

New shapes this wave (all size 1996 unless noted):

| shape | raw | aligned |
|---|---:|---:|
| master | **40** | **21** |
| `const` / `register` rawBonus | 40 | 21 |
| `bv = rawBonus * 1` / `<< 0` / `& -1` | 40 | 21 |
| `bv = b` | 45 | 31 |
| `bv = rawBonus` inside the if body | 44 | 25 |
| `bv` declared inside the else | 44 | 25 |
| `bv = rawBonus` before the if | 621 | 77 (2004 B) |
| `bv` both before and after the if | 601 | 73 (2004 B) |
| `volatile int rawBonus` | 670 | 151 (2004 B) |
| `int bonus` + `volatile bv` + `bv = rawBonus` | 758 | 171 (2000 B) |
| `int bonus` + `volatile bv` + `bv = b` | 638 | 121 (1996 B) |
| `int bonus` + `volatile bv` + `bv = b - rawBonus` | 635 | 144 (2000 B) |
| `int bonus` + no `bv` (volatile `rem`) | 716 | 88 (2000 B) |
| `int bonus` + no `bv` (plain `rem`) | 181 | 116 (2000 B) |
| non-volatile `bonus` + `bv = rawBonus` | 758 | 171 (2000 B) |
| empty `if (rawBonus ...)` anchors | 716 | 88 (2000 B) |

The original's plain-`int` bonus block is only reachable with the natural
`r6`/`r5` split, which clang 7 `-Oz` does not pick for our IR without the
volatile anchors.  Every anchor that keeps the split costs the extra
`bv` stack slot; every anchor that removes the slot loses the split.

## Conclusion

No measured win this wave.  The four tails are unchanged at
**40/21 (1996), 39/11 (1008), 28/16 (1896), 16/6 (11440)** raw/aligned/size.
`src/` is at base `be9dd429b`; only this note is added.  Whole-`.text`
differing bytes stay at **80,897** (`.text` size 1,259,464 on both sides).
Full `check` is green: byte_exact 4507, linked_exact 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370,
`.rel.plt` identical.
