# LevelScript::process — wave 160 / w10604: cold/error-path expect sweep + epilogue diff

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w10604`, base master `08af4633b` (wave 159).  Scratch `/dev/shm/w10604/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,055            55,064            (+9)
process aligned insns    17,059            17,040            (-19)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The landed edit removes the `__builtin_expect(..., 1)` wrapper from the mission-122
hacking guard (`if (m_nState >= 4 && m_nState <= 6) { if (isDockedToDockingPoint() &&
hackingWon() && getHackingGameDockIndex() >= 0) ... }`).  The guard's code shape is
identical with or without the hint; the aligned metric improves by 19 because the
greedy alignment re-syncs on a slightly different instruction run (the regions diff is
unchanged).  Raw bytes rise 9.  This is a marginal, alignment-noise-level tradeoff; the
coordinator should treat it as a "measured, not obviously structural" candidate.

## Epilogue diff (deliverable)

The epilogue is **byte-identical except one instruction**.  Original `0x144f56`:

```
144f56: f899 5011  ldrb.w r5, [r9, #17]     ; r5 = m_bCinematicBreak (byte @0x11)
144f5a: 2d00       cmp    r5, #0
144f5c: bf18       it     ne
144f5e: 2501       movne  r5, #1
144f60: 986b       ldr    r0, [sp, #428]    ; canary slot
144f62: f8db 1000  ldr.w  r1, [fp]          ; <-- original: canary GOT ptr in fp
144f66: 1a08       subs   r0, r1, r0
144f68: bf01       itttt  eq
144f6a: 4628       moveq  r0, r5
144f6c: b06c       addeq  sp, #432
144f6e: ecbd 8b0c  vpopeq {d8-d13}
144f72: b001       addeq  sp, #4
144f74: bf04       itt    eq
144f76: e8bd 0f00  ldmiaeq.w sp!, {r8, r9, sl, fp}
144f7a: bdf0       popeq  {r4, r5, r6, r7, pc}
144f7c: f719 ec52  blx    __stack_chk_fail@plt
```

Ours `0x144fe8` is the same sequence with `ldr.w r1, [sl]` instead of `[fp]`.  The
`itttt eq` / `vpopeq` / register-restore order is exact.

Root cause: a **callee-saved register allocation tie**.  The original keeps `this`
in `sl` (r10) and the stack-canary GOT pointer in `fp` (r11); ours keeps `this` in
`fp` and the canary in `sl`:

```
orig 137d26: 4682 mov sl, r0        ours 137d26: 4683 mov fp, r0
     137d30: f8d0 f000 ldr.w fp,[r0]      137d30: f8d0 a000 ldr.w sl,[r0]
```

This tie also explains the canary spill point difference: the original spills `fp`
to `[sp,#68]` at `0x138134` (after the `m_bStartSequence` block); ours spills `sl`
at `0x137f7c` (before it).  Reversing the tie would fix the prologue, the epilogue,
and ~6 spill/reload sites at once, but no source lever was found to flip it.

## `__builtin_expect` sweep (one guard at a time, full link)

Baseline 55,055 / 17,059 / 57,232.  `SIZE!` = not 57,232 (gate-fatal).

| guard | expect | raw | aligned | size |
|---|---|---:|---:|---:|
| L615 `mission==0` | 0 | 55,479 | 22,008 | 57,432 |
| L615 `mission==0` | 1 | 55,623 | 21,928 | 57,424 |
| L620 `introMsg->isOver()&&state==0` | 0 | 55,489 | 22,298 | 57,372 |
| L620 `introMsg->isOver()&&state==0` | 1 | 55,602 | 19,482 | 57,432 |
| L653 `msg6->isOver()` | 0 | 55,123 | 17,113 | 57,240 |
| L653 `msg6->isOver()` | 1 | 55,055 | 17,059 | **neutral** |
| L670 `messages[7]->isOver()` | 0 | 55,238 | 17,085 | 57,236 |
| L670 `messages[7]->isOver()` | 1 | 55,055 | 17,059 | **neutral** |
| L687 `messages[10]->isOver()` | 0 | 55,055 | 17,059 | **neutral** |
| L687 `messages[10]->isOver()` | 1 | 55,421 | 19,662 | 57,184 |
| L709 `state==5&&messages[12]->isOver()` | 0 | 55,122 | 18,679 | 57,276 |
| L709 `state==5&&messages[12]->isOver()` | 1 | 55,520 | 19,672 | 57,492 |
| L728 `messages[13]->isOver()` | 0 | 55,290 | 17,062 | 57,232 |
| L728 `messages[13]->isOver()` | 1 | 55,055 | 17,059 | **neutral** |
| L731 `messages[14]->isTriggered()` | 0 | 55,055 | 17,059 | **neutral** |
| L731 `messages[14]->isTriggered()` | 1 | 55,454 | 19,688 | 57,192 |
| L740 `messages[8]->isOver()` | 0 | 55,055 | 17,059 | **neutral** |
| L740 `messages[8]->isOver()` | 1 | 55,055 | 17,059 | **neutral** |
| L883 `messages[17]->isOver()` | 0 | 55,134 | 18,134 | 57,248 |
| L920 `messages[22]->isOver()` | 0 | 55,218 | 18,637 | 57,240 |
| L930 `state==16` | 0 | 55,184 | 18,606 | 57,244 |
| L930 `state==16` | 1 | 55,141 | 18,560 | 57,244 |
| L931 `!isFading()` | 0 | 55,055 | 17,059 | **neutral** |
| L931 `!isFading()` | 1 | 54,916 | 18,193 | 57,264 |
| L962 `msg2->isOver()&&state==0` | 0 | 55,436 | 18,671 | 57,424 |
| L962 `msg2->isOver()&&state==0` | 1 | 55,276 | 18,702 | 57,540 |
| L966 `state==1&&!isFading()` | 0 | 55,436 | 18,671 | 57,424 |
| L966 `state==1&&!isFading()` | 1 | 55,276 | 18,702 | 57,540 |
| L1024 `messages[3]->isOver()` | 0 | 55,249 | 17,060 | 57,240 |
| L1024 `messages[3]->isOver()` | 1 | 55,230 | 17,122 | 57,244 |
| L1031 `!isFading()` | 0 | 55,055 | 17,059 | **neutral** |
| L1031 `!isFading()` | 1 | 55,058 | 17,059 | 57,232 |
| L1356 `messages[2]->isOver()` | 0 | 55,055 | 17,059 | **neutral** |
| L1380 `state==3&&messages[4]->isTriggered()` | 0 | 55,055 | 17,059 | **neutral** |
| L1383 `state==4&&messages[4]->isOver()` | 0 | 55,081 | 17,148 | 57,240 |
| L1395 `state==6&&messages[5]->isOver()` | 0 | 55,204 | 17,148 | 57,236 |
| L1425 `state==0` | 0 | 55,190 | 17,079 | 57,224 |
| L1430 `messages[0]->isOver()` | 0 | 55,240 | 17,050 | 57,224 |
| L1430 `messages[0]->isOver()` | 1 | 55,191 | 17,058 | 57,224 |
| L1455 `m_bStartSequence!=0` | 0 | 55,115 | **16,719** | 57,264 |
| L1455 `m_bStartSequence!=0` | 1 | 55,373 | 18,428 | 57,420 |
| L1507 `state==0` (mission 5) | 0 | 55,243 | 17,130 | 57,216 |
| L1517 `messages[1]->isTriggered()&&state==0` | 0 | 55,055 | 17,059 | **neutral** |
| L1522 `mission==50` | 0 | 55,246 | 17,086 | 57,224 |
| L1526 `mission==51` | 0 | 55,058 | 17,247 | 57,232 |
| L1532 `count>=3&&...` | 0 | 55,302 | 17,162 | 57,232 |
| L3992 `state==3` (remove) | — | 55,378 | 17,091 | 57,224 |
| L3992 `state==3` | 0 | 55,254 | 17,131 | 57,264 |
| L4026 `state>=4&&state<=6` | 0 | 55,080 | 20,176 | 57,236 |
| L4026 `state>=4&&state<=6` | 1 | 55,407 | 17,206 | 57,248 |
| L4027 hacking guard (remove) | — | **55,064** | **17,040** | **57,232** |
| L4027 hacking guard | 0 | 55,134 | 21,397 | 57,280 |

**Which guards flip layout / are neutral.**  The guards whose `expect` is a no-op
(identical 55,055 / 17,059 / 57,232) are L653(1), L670(1), L687(0), L728(1),
L731(0), L740(0/1), L931(0), L1031(0), L1356(0), L1380(0), L1517(0): clang already
lays those branches out that way, so the hint is ignored.  Every other guard changes
the layout, and almost all of them change the function size (block reordering adds
alignment padding), so they are gate-fatal.  The only size-exact non-neutral result is
the landed L4027 removal.

## Structural finding: the state-3 / hacking block boundary

Original `0x13dcb8`:

```
13dcb8: bne.w 13de44     ; m_nState != 3 -> hacking check
... state-3 block ...
13dd02: blt.w 144e36     ; timer < 7001 -> tail
... state-3 completion ...
13de40: b.w 144e36       ; completion -> tail
13de44: subs r0, #4      ; m_nState in [4,6] check
```

So in the original **both** exits of the `m_nState == 3` block go to the shared tail;
the `m_nState >= 4` hacking check is only reached when `m_nState != 3` at entry.  That
is `else if` semantics.  Ours has two independent `if`s, so the state-3 completion
falls through into the check (`13ea28 strb r9,[r5,#17]` then `13ea2c ldr r0,[fp,#28]`).

Rewriting L4026 as `else if` is semantically correct but **loses** on both metrics
(raw 55,407 / aligned 20,588 / size 57,232) because with `expect(m_nState==3, 1)` the
else branch is treated as cold and clang moves the whole hacking block to the cold
region near `0x145ac2`.  `else if` + removing the L3992 expect shrinks the function to
57,216 (-16, gate-fatal).  This remains the clearest open structural lead.

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10604/fdiff.py _ZN11LevelScript7processEi        # 55,064 / 57,232
python3 /dev/shm/w10604/align_diff.py _ZN11LevelScript7processEi  # 17,040
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## Open questions

1. Flip the `this`/canary callee-saved tie (sl<->fp).  It is the epilogue's only
   difference and would fix ~6 spill sites; no source lever found yet.
2. `else if` at L4026 is the right control-flow shape but needs a way to keep the
   hacking block inline (the original does not treat it as cold).
3. The 371-instruction region at `0x13de0a` is the state-3 completion tail plus the
   hacking block; the remaining differences there are register allocation
   (`player` in `sl` vs reloaded; constant 1 in `r1` vs hoisted into `fp`), not
   control flow.
