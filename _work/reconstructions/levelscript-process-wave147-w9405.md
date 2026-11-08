# LevelScript::process — wave 147 / w9405: per-case block-order target +
# `__builtin_expect` on the case-154/157 mission/state guards

Status: **note-only, no source win** (source restored pristine).  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9405`, base master `cbe3b5f7f` (wave 147).  Scratch in
`/dev/shm/w9405/` (not tracked).

```
                         baseline (HEAD)   this pass
process raw diffbytes    54,913            54,913   (source pristine)
orig / ours size         57232 / 57232     57232 / 57232
align_diff TOTAL         15,740            15,740
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
RATCHET                  PASS              PASS
```

Metric note: `/dev/shm/fdiff.py` and `_work/reconstructions/align_diff.py`
both hard-code the *master* worktree (`gof2-decomp/cmake-build-match`).  I
used local copies in `/dev/shm/w9405/` that point at `wt/w9405`; the logic is
identical.

## 0. Per-case entry table (reproduced from w9303, base cbe3b5f7f)

The case entry ORDER is identical in both binaries; the residual is the
per-case span (`dSpan`), i.e. where the backend places the outlined cold
blocks.  Target region (cases 92..24):

```
case      orig     ours  dEntry   spanO   spanU    dSpan
92      139ffe   13a152    -340     778     798      -20
157     13a308   13a470    -360     464     462       +2
40      13a4d8   13a63e    -358     384     346      +38
102     13a658   13a798    -320     278     134     +144
154     13a76e   13a81e    -176     450     224     +226
24      13a930   13a8fe     +50     130     148      -18
```

**Case 154 is the case whose block order differs most in the target region**
(`dSpan +226`): the original keeps the `m_nState == 0` body inline (446 B,
`13a76e..13a92c`, ending `b.w 144e36`) and outlines the states 1..10 jump
table to `0x13e8e8`; our baseline outlines state 0 (`beq.w 13c658`) and keeps
the states 1..10 `tbh` inline (`13a830`).  This is the same finding as w9304
§0.2, now confirmed on the cbe3b5f7f base.

Globally the largest `dSpan` is still case 125 (`-856`: the original outlines
the nested `switch (m_nState)` bodies to `0x13b43e/0x13b518`, ours keeps them
inline).  Case 125 state-0/state-1 expects were also measured (below).

## 1. New experiments (all size-exact unless the size column says otherwise)

`__builtin_expect` forms on the case-154 first guard
`if (m_nState == 0 && messages[0]->isTriggered())`:

| form | size | raw |
|---|---:|---:|
| baseline | 57232 | **54,913** |
| `expect(A && B, 1)` (whole) | 57248 | 55,253 |
| `expect(A && B, 0)` (whole) | 57320 | 55,397 |
| `expect(A, 1) && B` (first) | 57332 | 55,078 |
| `expect(A, 0) && B` (first) | 57328 | 55,349 |
| `A && expect(B, 1)` (second) | 57228 | 55,207 |
| `A && expect(B, 0)` (second) | 57320 | 55,397 |
| `expect(A,1) && expect(B,1)` | 57248 | 55,253 |
| `expect(A != 0, 0) == 0 && B` | 57332 | 55,078 |

The `whole`/`second` forms DO reproduce the original structure (state-0 body
inline, `bne.w` to the cold chain) — verified by disassembly: with
`expect(A && B, 1)` the entry is
`ldr.w r0,[fp,#28]; cmp r0,#0; bne.w 13c7a0` and the state-0 body falls
through, exactly like orig `13a76e: cmp r0,#0; bne.w 13e8e8`.  But the
outlined chain lands at `0x13c7a0` (orig `0x13e8e4`), so total size grows
+16..+100 and raw gets worse.  The +16 variant
(`expect(A && B, 1)`) becomes size-exact if the line-1812 `autoTurret` PLT
anchor is also removed (`154whole1+anchor`: **size 57232, raw 55,357,
align 22,480**) — both metrics still worse than baseline, so not landed.

Case-154 `else if (m_nState == N)` guards, `expect(..., 1)`:

| N | size | raw |
|---|---:|---:|
| 1 | 57168 | 55,402 |
| 2 | 57336 | 55,482 |
| 3 | 57220 | 55,467 |
| 4 | **57232** | 55,389 |
| 5 | 57216 | 55,435 |
| 6 | 57204 | 55,433 |
| 7 | 57228 | 55,381 |
| 8 | 57228 | 55,351 |
| 9 | 57256 | 55,391 |
| 10 | **57232** | 54,928 |

Case-157 guards (state 0/1/2, state-4 completion, state-5 weak-device and
strong-arm, states 6..12, timer guards):

| variant | size | raw |
|---|---:|---:|
| `expect(!isRunningHDonWeakDevice, 1)` | 57232 | 54,913 |
| `expect(!isRunningHDonWeakDevice, 0)` | 57232 | 54,913 |
| `expect(m_nState==0, 1)` (state-0 `new AEGeometry`) | 57240 | 55,330 |
| `expect(m_nState==0, 0)` | 57224 | 55,319 |
| `expect(m_nState==1, 1)` on the `&& isOver()` first term | 57228 | 54,932 |
| `expect(m_nState==2, 1)` | 57200 | 55,391 |
| `expect(isOver() && timer>=18001, 1)` (state-4 completion) | 57232 | 54,913 |
| `expect(isOver() && timer>=18001, 0)` | 57244 | 55,472 |
| `expect(isRunningHDonWeakDevice, 1)` (state-5) | 57220 | 55,436 |
| `expect(isRunningHDonWeakDevice, 0)` (state-5) | 57224 | 55,426 |
| `expect(m_nState==6, 1)` | **57232** | 55,306 |
| `expect(m_nState==7, 1)` | **57232** | 55,199 |
| `expect(m_nState==10, 1)` | 57224 | 55,513 |
| `expect(m_nState==11, 1)` | 57216 | 55,554 |
| `expect(m_nState==12, 1)` | 57220 | 55,053 |
| `expect(timer>=12001 && isOver(), 1)` | **57232** | 54,914 |
| `expect(timer>=6001, 1)` (state 9) | **57232** | 54,913 |
| `expect(timer>=3001, 1)` (state 10) | **57232** | 54,913 |
| `expect(timer>=5000 && ..., 1)` (state 12) | **57232** | 54,913 |

Case-125 (global largest `dSpan -856`) state guards:

| variant | size | raw |
|---|---:|---:|
| `expect(m_nState==0, 1) && isTriggered()` | 57232 | 54,914 |
| `expect(m_nState==0 && isTriggered(), 1)` | 57232 | 54,914 |
| `expect(m_nState==0, 0) && ...` | 57244 | 55,487 |
| `expect(m_nState==1, 1)` | 57228 | 55,373 |
| `expect(m_nState==1, 0)` | 57248 | 55,354 |

The case-125 `expect(state==0, 1)` keeps the nested-switch bodies inline
(span stays 954 B vs orig 98 B) and moves raw by exactly +1 byte
(align 15,741).  So the weight does not reach the outlining decision.

Combos (MERGE LESSON: test both orders):

| combo | size | raw |
|---|---:|---:|
| case-154 `whole1` + remove `m_bCinematicBreak=1` (state-5 strong arm) | 57248 | 55,245 |
| case-154 `whole1` + `expect(!isRunningHDonWeakDevice, 0)` | 57264 | 55,285 |
| case-157 state-4 completion `whole1` + state-5 `whole1` | 57308 | 55,373 |
| case-157 state-4 completion `whole1` + state-5 `whole0` | 57224 | 55,423 |
| case-157 state-4 completion `whole1` + `expect(state==2,1)` | 57224 | 55,299 |
| case-157 state-5 `whole1` + `expect(state==2,1)` | 57228 | 55,200 |

Removing the extra `m_bCinematicBreak = 1` from the state-5 strong arm (w9303
§"Two real source differences", not in the original) alone is **size-neutral**
(57232) but raw rises to 54,993; it is a genuine divergence but not a metric
win on its own.

## 2. Why the expects cannot close the gap

`__builtin_expect` only feeds a 2-level `branch_weights` hint.  It reliably
flips the *local* fall-through decision (case-154 state-0 inline vs outlined),
but the residual is the **global cold-block order**: the original's outlined
state-4 completion block sits immediately before the state-5 strong arm
(orig `0x144960..0x144a12` then `0x144a14`), while ours places the state-4
completion block at the far end (`resetGunDelay` at ours `0x144aba` vs orig
`0x144982`) and the strong arm at `0x144384` (orig `0x144a2a`, `+0x6a6`).
That ordering is decided by MachineBlockPlacement's cold-block list, which a
single `expect` on a mutually-exclusive `m_nState` chain cannot express.

## 3. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9405/fdiff.py _ZN11LevelScript7processEi       # 54913, 57232/57232
python3 /dev/shm/w9405/align_diff.py _ZN11LevelScript7processEi  # TOTAL 15740
python3 /dev/shm/w9405/table.py                                  # per-case entry table
```
Sweep drivers: `/dev/shm/w9405/sweep.py` .. `sweep9.py` (line-based
`__builtin_expect` patcher + build + raw measurement).

## 4. Open items

1. The state-4 completion block placement (orig before the state-5 strong
   arm, ours at the end) is the concrete cold-block-order residual.  It needs
   a source-shape change (e.g. hoisting the completion `if` out of the
   `else if (m_nState == 4)` arm) or a multi-block weight pattern, not a
   single `__builtin_expect`.
2. Case 125's nested `switch (m_nState)` outlining (`-856 B`) is still the
   largest single `dSpan`; the state-0 expect does not reach it.
3. The state-5 strong-arm `m_bCinematicBreak = 1` is genuinely absent from the
   original (w9303) but removing it is size-neutral and raw-worse; it should
   be landed together with the block-order fix.
