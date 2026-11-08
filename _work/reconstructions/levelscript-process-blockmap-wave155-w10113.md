# LevelScript::process — wave 155 / w10113: per-basic-block diff map and the
# callee-saved-allocation cluster (note-only, no source win)

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10113`, base master
`f4e0ea40b` (wave 154).  Scratch in `/dev/shm/w10113/`.

## Result (HEAD, source pristine)

```
process raw diffbytes    55,327
process aligned insns    17,176   (official _work/reconstructions/align_diff.py)
orig / ours size         57232 / 57232   (size-exact)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
RATCHET                  PASS
```

## 1. Method: per-case / per-basic-block diff map

The function is 20,807 orig / 20,646 ours instructions.  A plain
`difflib`/patience diff over the whole stream is dominated by the ~50%
register-operand noise and by literal pools, so the map was built in two
layers:

1. **Case-body anchors.**  The mission dispatch is a linear compare chain at
   `0x1387ce..0x13885e` plus two `tbh` tables (`0x138a8a` for 131..145,
   `0x138c3e` for 64..73).  Decoding the tables (`pc = tbh+4`, target =
   `pc + 2*entry`) and the chain gives both binaries' case-entry addresses.
   The case order is identical in both (as w9304 found); only the per-body
   spans differ.
2. **Within each case**, align the two instruction streams with
   `difflib` after normalising branch targets to instruction indices and
   registers to `R`.  This isolates structural (non-register) differences.

Per-case body span deltas (`ours - orig`, sorted by |mismatched insns|):

| case | orig entry | spanO | spanU | dSpan | mism insns | likely cause |
|---|---:|---:|---:|---:|---:|---|
| 16 | 0x13b014 | 44180 | 44298 | +118 | 13933 | shared tail; everything after 0x13b014 |
| 64 | 0x138c56 | 1386 | 1256 | -130 | 497 | inner-switch case-5 block placement + `this`/`status` regalloc |
| 125 | 0x139f9c | 98 | 1022 | +924 | 376 | inline body (w10103 owns) |
| 92 | 0x139ffe | 778 | 740 | -38 | 168 | `this` regalloc |
| 154 | 0x13a76e | 450 | 160 | -290 | 163 | w10103 owns |
| 70 | 0x1399a2 | 768 | 346 | -422 | 160 | shared exit-wormhole body placed elsewhere |
| 102 | 0x13a658 | 278 | 134 | -144 | 132 | cold-block placement |
| 158 | 0x138862 | 586 | 606 | +20 | 130 | `this`/`status` regalloc |
| 145 | 0x1392f6 | 590 | 534 | -56 | 126 | `this` regalloc |
| 157 | 0x13a308 | 464 | 524 | +60 | 97 | `this` regalloc |

## 2. Top-10 instruction-index regions (structural patience diff)

`so.tok`/`su.tok` = mnemonic+operands with registers normalised to `R`,
branch targets normalised to indices; `git diff --no-index --patience -U0`.
Total structural mismatch **17,311** (8,736 orig / 8,575 ours).

| # | orig addr | bytes | oins | uins | mism | likely cause |
|---|---|---:|---:|---:|---:|---|
| 0 | 0x13d7f8 | 2576 | 264 | 667 | 931 | case-106 state-5/6 body: `status` in r9 vs orig `[sp,#92]` reload; `this` fp vs sl |
| 1 | 0x13e3e4 | ~ | 796 | 124 | 920 | jump table + case body, table placed at different address |
| 2 | 0x13a61e | ~ | 292 | 93 | 385 | literal-pool placement before case-29 body |
| 3 | 0x13b43a | ~ | 344 | 40 | 384 | case-125 state-1 body (w10103) |
| 4 | 0x13ece8 | ~ | 331 | 49 | 380 | case-64 state-1/2 dispatch placement |
| 5 | 0x13c324 | ~ | 175 | 177 | 352 | PlayerWormHole::reset / FModSound::play block |
| 6 | 0x14351a | ~ | 295 | 56 | 351 | literal pool before tail |
| 7 | 0x140ff6 | ~ | 315 | 22 | 337 | m_nScriptTimerA >= 2001 case body |
| 8 | 0x13db8a | ~ | 36 | 297 | 333 | loop + literal pool placement |
| 9 | 0x139f98 | ~ | 48 | 245 | 293 | case-125 entry (w10103) |

The single largest *non-16, non-w10103* cluster is **case 64 (497 mismatched
insns)**; the single largest contiguous instruction-index cluster is
**0x13d7f8 (case 106 state-5/6, 931)**.  Both reduce to the same root cause:
the whole-function callee-saved register assignment.

## 3. Root cause: callee-saved allocation

Prologue, original vs ours (both 57,232 B, same frame 0x1b0):

```
orig                                  ours
mov sl, r0        ; this -> sl         mov fp, r0        ; this -> fp
ldr r4, [r0, #0]  ; status -> r4       ldr.w r9, [r0]    ; status -> r9
ldr r0, [r4, #0]  ; vtable -> r0       ldr.w r4, [r9]    ; vtable -> r4
blx getMission                         mov r0, r4
                                       blx getMission
ldr r0, [r4, #0]  ; vtable again       mov r0, r4        ; vtable CSE'd
blx getMission                         blx getMission
```

| value | original | ours |
|---|---|---|
| `this` | `sl` | `fp` |
| `&__stack_chk_guard` | `fp` | `sl` |
| `Globals::status` | `r4`, later spilled to `[sp,#92]` | `r9` (callee-saved, kept) |
| `messages` / `delta` / `enemies` / `&m_pLevel` | r5 / r6 / r8 / r4 | r5 / r6 / r8 / r4 (match) |

The original reloads `status` from `[sp,#92]` before every use (e.g. case-106
state 6 at `0x13d84a: ldr r5,[sp,#92]; ldr r1,[r5,#0]`), while ours uses `r9`
directly (`0x13ec32: ldr.w r1,[r9]`).  That is the ~165-instruction
`extra-orig:ldr` surplus.  The same swap makes every `this->m_nState` /
`this->m_nScriptTimerA` access use `sl` in the original and `fp` in ours.

This is the w10007 open question #1.  It is a whole-function greedy-allocator
outcome, not a local source shape: the IR operand counts are the same, and
the allocator's `getSpillWeight = (isDef+isUse)*blockFreq/entryFreq` gives
`this` and the canary nearly equal weights.

## 4. New hypotheses measured this wave (all size-exact unless noted)

| # | variant | size | raw | aligned |
|---|---|---:|---:|---:|
| 0 | HEAD (pristine) | 57232 | 55327 | 17176 |
| 1 | `__builtin_expect(getCurrentCampaignMission()==0, 1)` on the mission-0 guard | **57400** | 55685 | 22148 |
| 2 | 2nd dispatch call uses `Globals::status->` (no `status` local) | 57232 | 55327 | 17176 |
| 3 | remove the `status` local entirely (both calls `Globals::status`) | 57232 | 55327 | 17176 |
| 4 | `__builtin_expect(m_nState == 6, 1)` on the case-106 state-6 guard | 57232 | 55327 | 17176 |
| 5 | extra entry-block `this` use: `if (m_nState == 0x7fffffff) return false;` | **57396** | 55569 | 22338 |

Hypotheses 1 and 5 move codegen and are large regressions (+168 B / +164 B).
Variant 5 does change the callee-saved map (`this` moves from `fp` to `r9`,
frame grows to 0x1b8) but it still does not give `this` the `sl` slot, and the
extra `return false` exit costs more than it saves.  Hypotheses 2-4 are
codegen no-ops: clang hoists `Globals::status` into `r9` regardless of the
source spelling, and the case-106 expect does not change the dispatch
layout.  So the cluster is not reachable by a local guard/expect change.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10113/fdiff.py _ZN11LevelScript7processEi        # 55,327 / 57,232
python3 /dev/shm/w10113/align_official.py _ZN11LevelScript7processEi  # 17,176
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch scripts: `/dev/shm/w10113/{common,fdiff,align_official,mapper,
mapper2,structdiff,casetable,spans,casediff,casealign}.py`.

## 6. Open questions

1. Flipping `this`/canary (`sl`/`fp`) is the whole residual.  The original
   reloads `this` from `[sp,#88]` 328 times vs our 157; a source shape that
   increases the *hot-block* `this` use count (not just cold uses) is the
   only route found to change the greedy allocation.
2. The case-106 state-5/6 body (`0x13d7f8`) would match almost exactly if
   `status` spilled to `[sp,#92]`; see (1).
3. Case-70's -422 span is the shared exit-wormhole body (`0x139ac2`, 480 B)
   being emitted elsewhere in ours, not a missing statement.
