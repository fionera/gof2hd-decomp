# `__builtin_expect` / branch-weight sweep across the small residuals — wave 147 / w9318

Status: **one source change landed** (SpaceLounge::drawLounge expect removed),
2026-11-08. Worktree `wt/w9318`, base master `9c9744ca5` (wave 146).
Original binary `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`). Scratch: `/dev/shm/w9318/`.

## 0. Result

| | before | after |
|---|---:|---:|
| byte_exact / linked_exact | 4507 / 4507 | **4507 / 4507** |
| size-exact | 4524/4524 | 4524/4524 |
| data / imports / lint / drift | 843 / 207 / CLEAN / 0 | same |
| aligned total (15 fns, measured) | **24,232** | **24,222** |
| `.text` differing bytes | 85,928 | **86,018** |
| `.ARM.extab` differing bytes | 1,393 | **1,396** |
| `.rel.plt` / build-id | 4 / 20 | 4 / 20 |
| section sizes | all 27 exact | all 27 exact |

The landed change is **METRIC-ONLY on the aligned metric** and is a small
regression on the raw/`.text` metric:

```diff
-        if (__builtin_expect(agent->isKnown(), 1)) { // lint: arm_builtin anchor: ...
+        if (agent->isKnown()) {
```

`SpaceLounge::drawLounge` aligned **57 -> 47** (-10), raw **175 -> 265** (+90),
size 1948 exact, `.text` +90, `.ARM.extab` +3.  It also makes our instruction
count equal the original's (741 vs 741; the expect form has 740), which is why
the aligned metric improves.  Opcode-only alignment is *not* better
(13 -> 14), so the gain is operand/register-allocation alignment, not block
structure.  The old source comment ("plain form grows body 1948 -> 1952") is
**stale**: the plain form is 1948 B today.

The coordinator may prefer the expect form (raw/.text-faithful); see §4.

## 1. Method

For each of the six smallest residuals I applied one expect variant at a time
with **every other TU restored to base** (an earlier function's size change
shifts addresses and corrupts the same-address raw metric — this bit me on the
first ModStation batch), rebuilt `link_so`, and measured:

* raw same-address diff bytes (`/dev/shm/w9318/fdiff.py`),
* instruction-index alignment (`/dev/shm/w9318/align_diff.py`, the wave-146
  shared tool, with `OURS` re-pointed at this worktree),
* size, `.text` / `.ARM.extab` section diff (`/dev/shm/w9318/secdiff.py`).

## 2. Response table (raw / aligned / size)

`base` is the wave-146 tree.  Bold = improvement over base.

### MenuTouchWindow::OnTouchBegin (base 28 / 16 / 1896)
| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **28** | **16** | 1896 |
| `expect(messageShowing,0)` | 1730 | 794 | 1900 |
| `expect(choiceWindowOpen,0)` | 1767 | 826 | 1900 |

Pure register-allocation residual (`ip` vs `r4`, `r2` vs `r6`); no expect helps.

### ModStation::ModStation C1 (base 43 / 12 / 1008)
| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **43** | **12** | 1008 |
| `expect(getIndex()==101,0)` | 665 | 35 | 1008 |
| `expect(getIndex()==101,1)` | 693 | 112 | 1008 |

Pure register-allocation residual; no expect helps.

### Gun::calcCharacterCollision (base 463 / 57 / 1984)
| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **463** | **57** | 1984 |
| remove both existing expects | 1811 | 293 | **1908** |
| flip mine expect to 0 | 976 | 309 | 2000 |
| flip scatter expect to 1 | 1810 | 257 | **1908** |

Residual is register/stack-slot allocation (`r4`/`r5`, `[sp,#44]`/`[sp,#104]`);
both existing expects are size-load-bearing.

### Generator::createMission (base 445 / 59 / 1996)
| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **445** | **59** | 1996 |
| `expect(type<15,1) && ...` | 1780 | 246 | **1964** |
| `expect(type<15 && bit,0)` | 1520 | 230 | 1988 |
| `expect(type<15 && bit,1)` | 1789 | 269 | 1948 |
| flip existing `race<4&&type==13` to 0 | 1803 | 627 | 1992 |

The one block-layout difference (the `<15` `bhi` targets the post-switch merge
`0x97388` in the original but the post-`if` merge `0x973fa` in ours — LLVM jump
threads `type>14 => type!=13`) is **not** moved by any expect; every variant is
much worse and most change the size.

### SpaceLounge::drawLounge (base 175 / 57 / 1948)
| variant | raw | aligned | size | `.text` | extab |
|---|---:|---:|---:|---:|---:|
| base | **175** | 57 | 1948 | **85,928** | **1,393** |
| **remove expect (landed)** | 265 | **47** | 1948 | 86,018 | 1,396 |
| remove expect + `pad*2` | 514 | 57 | 1948 | 86,198 | 1,405 |
| `int pad3 = pad` (instead of `short`) | 445 | 54 | 1948 | 86,198 | 1,405 |
| `expect(mode==2,1)` | 1612 | 370 | **1936** | — | — |
| `expect(visibleButtonCount>2,0)` | 1294 | 111 | 1952 | — | — |
| `expect(x-pad<buttonInsetX,0)` | 486 | 68 | 1952 | — | — |
| `expect(pad+x+width>w-inset,0)` | 493 | 68 | 1952 | — | — |
| `expect(isKnown()||isStoryAgent(),1)` on name | 1577 | 231 | 1956 | — | — |

The `itt gt` tail block is a real source-shape difference (see §3), but no
variant that fixes it keeps `.text` at baseline.

### MGame::OnUpdate (base 37 / 109 / 11440)
| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | **37** | **109** | **11440** |
| reload `Globals::status` for the tail store | 914 | 109 | 11392 |
| same + `expect(mission>=122,1)` | 1198 | 203 | 11392 |

The 109 aligned diffs are almost entirely a **one-instruction cascade**: the
`Status *st` local makes the mission-122 block emit `mov r0,r5; b.w 17e270`
(entering the shared tail at the `blx`) where the original emits
`b.w 17e26c` (reloading `Globals::status` first).  Removing `st` fixes the
instruction but costs 48 B of code; no expect recovered it.  This confirms the
wave-146 w9220 open question.

## 3. Why the small residuals do not respond

Every one of the six smallest residuals is dominated by **register-allocation
ties**, not block layout:

* MenuTouchWindow: `ip` vs `r4`, `r2` vs `r6` on the same loads.
* ModStation: `r3`/`r4` base-register tie on `gHangarCamYawIPad`/`gHangarCamYaw`.
* Gun: `r4` vs `r5` and the `[sp,#44]`/`[sp,#104]` slot swap.
* SpaceLounge: `r9` vs `fp` for `layout->field_0x94`, plus the `itt gt` tail.
* Generator: one jump-threading branch-target choice + a `movs r0,#0; str` vs
  `movs r6,#0` block.
* MGame: the `st` local (one extra `mov`).

`__builtin_expect` only feeds `!prof` branch weights; it changes block
*placement* and branch relaxation, not the register allocator.  The one place
where it does change placement (Generator's `<15` test) is a jump-threading
decision made in IR (`SimplifyCFG`), before `LowerExpectIntrinsic`, so the
weights cannot reach it.

**`__builtin_expect_with_probability` does not exist in clang 7.0.2** —
`error: use of undeclared identifier '__builtin_expect_with_probability'`.
The only weight knob is `-mllvm -likely-branch-weight` / `-unlikely-branch-weight`
(already used per-file for StarMap and SpaceLounge at 7:1).

## 4. Faithfulness note

The original was almost certainly built with PGO/`!prof` weights (wave 146
w9214).  A source `__builtin_expect` is a reconstruction stand-in for those
weights, so "remove the expect" is not obviously more faithful than "keep it":
the wave-112 w6015 note chose the expect form because it reproduces the EH
landing-pad ordering at 7:1.  The landed variant keeps the same 1948 B and the
same 7:1 per-file flag but loses 3 extab bytes and 90 `.text` bytes.  It is
labelled **METRIC-ONLY (aligned)**; the coordinator may prefer to keep the
expect form if `.text` is weighted above the aligned metric.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j64
python3 /dev/shm/w9318/fdiff.py _ZN11SpaceLounge10drawLoungeEv   # 265 / 1948
python3 /dev/shm/w9318/align_diff.py _ZN11SpaceLounge10drawLoungeEv  # 47
python3 /dev/shm/w9318/secdiff.py                                 # .text 86,018, extab 1,396
cmake --build cmake-build-match --target check -j64
```

Scratch: `fdiff.py`, `align_diff.py`, `secdiff.py`, `opdiff.py`, `showdiff.py`,
`m.sh`, `mtry.py`, and the `*.base.cpp` snapshots of all six TUs.

## 6. Open questions

1. The aligned metric and the raw/`.text` metric disagree on SpaceLounge.  A
   variant that keeps `.text` at 85,928 while matching the original's 741
   instructions would settle it.
2. Generator's `<15` `bhi` target (`0x97388` vs `0x973fa`) is a `SimplifyCFG`
   jump-threading difference; a source shape that hides `type != 13` from the
   threader (without changing the CFG) is still missing.
3. MGame::OnUpdate needs the 48 B companion for the `st`-free source (w9220
   open item 2); the aligned metric would then collapse from 109 to ~2.
4. The medium/large residuals (Layout, LevelScript, createShip, Status, Radar,
   MGame::OnTouchEnd, createMission, Hud) were not swept this pass; they have
   real block-layout deltas and are the better `!prof` targets.
