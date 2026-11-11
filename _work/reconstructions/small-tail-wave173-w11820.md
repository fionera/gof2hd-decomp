# Small tail wave 173 (w11820): ModStation fence win

Status: **1 `src/` change landed**.  Worktree `wt/w11820`, branch
`wave173-w11820`, base master `3fafa5b03`.  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11820`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` byte-identical.

Baselines re-confirmed exactly with `tools/fn_diff.py`:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | 40 | 21 | 1996 |
| `_ZN10ModStationC1Ev` / `C2Ev` | 43 | 12 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`.

## ModStation::ModStation C1/C2 — landed raw 43 -> 39

The constructor's residual is the nearKey yaw-table select: the original
materialises `YawIPad` in the first literal-pool slot and `Yaw` in the
second, while we materialise `Yaw` first.  The wave-146 faithful form uses
`pitch` (not `yaw`) as the second `MatrixSetRotation` argument; with that
form the old compiler barrier (removed as redundant in wave 115) is
load-bearing again.

A same-line compiler barrier is enough to recover four bytes without
emitting an instruction:

```cpp
    // Compiler barrier only (no instruction emitted): keeps the two table
    // address materialisations ahead of the iPad load, matching the original.
    __atomic_signal_fence(__ATOMIC_SEQ_CST);
    const float *pitchTable = gHangarCamPitch;
    const float *yawTable = Globals::iPad ? gHangarCamYawIPad : gHangarCamYaw;
```

Result: **raw 43 -> 39, aligned 12 -> 11, size 1008 exact** (374 insns).
The literal-pool *slot order* is still `Yaw`-first (`0xd56b4` / `0xd56b8`),
so the remaining 39 bytes are the pool order plus the surrounding
`add`/`ldrb`/`ldr.w`/`str` schedule permutation.  The barrier is a
compiler-only fence (`__ATOMIC_SEQ_CST`/ACQUIRE/RELEASE/ACQ_REL all give
39; RELAXED is a no-op and leaves 43).

Fence placement sweep (all size 1008, C1/C2 aliased):

| placement | raw | aligned |
|---|---:|---:|
| no fence (master) | 43 | 12 |
| before `MatrixSetTranslation` | 103 | 44 |
| after `MatrixSetTranslation` / before `pitchTable` (**landed**) | **39** | **11** |
| between `pitchTable` and `yawTable` | 39 | 11 |
| before `yawTable` | 39 | 11 |
| after `yawTable` | 42 | 12 |
| before `pitchBits` | 42 | 12 |
| between `pitchBits` and `yawBits` | 44 | 12 |
| after `yawBits` | 42 | 12 |
| before `MatrixSetTranslation` + after it | 99 | 42 |
| before `pitchTable` + after `yawTable` | 44 | 18 |

Other new shapes (all 1008 B, 374 insns): `const float *const` tables,
`&arr[0]`, `(const float *)` casts, `arr + 0`, `(bool)`/`static_cast<bool>`
on `iPad`, `__builtin_expect(iPad, 0/1)`, and pointer-to-volatile tables
all stay at 39/11.  A volatile pointer variable costs +3 insns (279/154).

## MenuTouchWindow::OnTouchBegin — re-confirmed 28/16, no new win

The 28 raw bytes are exactly (a) case 8's 2-byte `ip`/`r4` tie
(`ldr.w ip,[sl,#340]` + `add r3,ip` vs `r4`) and (b) case 15's 26-byte
cached-vs-reloaded `leftMargin`/`top` block.  The wave-169 29/15 form
(`layout.field_0x20_top + (Globals::layout->field_0xc_leftMargin + ...)`)
still moves the `leftMargin` load after the call but leaves `top` cached
and is +1 raw, so master is kept.

New shapes this wave (all size 1896 unless noted):

| shape | raw | aligned |
|---|---:|---:|
| master | 28 | 16 |
| case-15 29/15 form | 29 | 15 |
| case-15 29/15 + case-8 global-top / reorder / paren / top-local | 29 | 15 |
| case-8 all-global, no local assignment | 28 | 16 |
| case-8 `Layout &layoutRef` / `Layout *layoutPtr` local | 28 | 16 |
| case-8 4th-cond all 6 `a+b+c` permutations | 28-33 | 16-17 |
| case-8 `(unsigned)`/`(long)`/`(short)` cast of `listEntryWidth` | 28-30 | 16-17 |
| case-8 local `int`/`unsigned`/`volatile int` copy of `listEntryWidth` | 62-1800 | 26-240 |
| second-if term reorderings | 28 | 16 |
| `__atomic_signal_fence` before/after case-8 locals | 28-34 | 16-18 |
| volatile loads of `listEntryWidth`/`top` | 28 | 16 |
| case-15 height-first / outer-sum reorder | 116-296 | 35-64 |

The case-8 register tie and the case-15 `fp`-vs-`[sp,#16]` spill choice are
both greedy-RA ties; no source shape tested moves either without losing
the 1896-byte frame.

## MGame::OnUpdate — re-confirmed 16/6, no new win

The m122 radio block is the wave-163 "level mid" shape.  The residual is
the hoisted `this->level` load before `getCurrentCampaignMission()` plus the
`r1`/`r2`/`r5` register split.  Any shape that loads `level` after the call
drops the whole function to 11392 B (24 instructions vanish from the shared
`inBlackMarketSystem` tail), so the 11440 anchor dominates.

New shapes this wave (all size 11440, 4002 insns):

| shape | raw | aligned |
|---|---:|---:|
| master | 16 | 6 |
| `radioBase + radioIndex` (commuted) | 16 | 6 |
| `Level &level = *this->level` | 16 | 6 |
| `(*level).createRadioMessage(...)` | 16 | 6 |
| `ccm() != 122 ? 3166 : 3164` | 16 | 6 |
| `ccm() - 122 == 0 ? 3164 : 3166` | 16 | 6 |
| `if (ccm() == 122) ... else ...` | 16 | 6 |
| `__atomic_signal_fence` before/after the level load | 16 | 6 |
| `Level *volatile` reload | 5206 | 600 |
| reorder + fence (natural load-after-call) | 5191 | 601 |

## Generator::createMission — re-confirmed 40/21, no new win

The landed wave-172 shape (volatile `rem` after the `b` add, volatile `bv`
anchor) remains the best.  The residual 40 bytes are the frame difference
(104 vs 96: the two volatile anchor slots at `sp+92/96` vs the original's
single bonus slot at `sp+16`), the `rem` slot (`sp+44` vs `sp+32`), and the
branchy `cbz` vs the original's `cmp/it/subne`.

New shapes this wave (all size 1996 unless noted):

| shape | raw | aligned |
|---|---:|---:|
| landed | 40 | 21 |
| `bv` declared inside the `else` block | 44 | 25 |
| both volatile declarations moved just before the bonus `if` | 44 | 25 |
| single volatile `bonus` reused as the `bv` anchor | 114 | 96 |
| `volatile int &bv = bonus` | 114 | 96 |
| `volatile int &bv = fromSystem` / `toSystem` | 810-830 | 77 |
| `volatile int *remp = &fromSystem` / `&toSystem` | 851-871 | 149 |
| `volatile int &remp = fromSystem` / `toSystem` | 851-871 | 149 |
| `int bonus` + volatile `bv` | 758 | 171 (2000 B) |
| `volatile int b` instead of `volatile int bonus` | 599-638 | 72-144 (2000 B) |
| non-volatile `rem` + fence | 659 | 143 (2004 B) |
| `__builtin_expect(b % 50 != 0, 1)` | 40 | 21 |
| `__builtin_expect(b % 50 != 0, 0)` | 140 | 64 (2004 B) |
| ternary / if-else / no-brace condition forms | 40 | 21 |
| commuted `remVal + rawBonus` / `b += remVal` | 40 | 21 |

The volatile anchors are needed for the `r6`/`r5` split; every shape that
removes them either loses the split or grows the frame/size.

## Conclusion

One measured win landed: `ModStation::ModStation` C1/C2 raw **43 -> 39**,
aligned **12 -> 11**, size exact 1008.  Whole-`.text` differing bytes
**81,503 -> 81,499** (4-byte drop, C1/C2 aliased).  `.ARM.extab` stays 711,
`.rel.plt` byte-identical, all 27 section sizes/bases exact.  Full `check`
green: byte_exact 4507, linked_exact 4507, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, parity 5370/5370.
