# LevelScript::process — wave 161 / w10705: epilogue/canary tie + cold-path expect sweep

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w10705`, base master `01c7f92d6` (wave 160).  Scratch `/dev/shm/w10705/`.

## Baseline (current tree, unchanged)

```
raw diffbytes   55,048
aligned insns   17,049
orig/ours size  57,232 / 57,232
byte_exact      4507/4524   linked_exact 4507/4524   size 4524/4524
data_exact      843/843     imports 207              lint CLEAN   drift 0
```

## 1. Epilogue / canary: the tie DOES flip — but it is not landable

The w10604 epilogue finding is confirmed and explained.  In the base tree the
callee-saved tie is `this`=fp, canary=sl:

```
orig 137d26: 4682        mov    sl, r0        ; this
     137d30: f8d0 b000   ldr.w  fp, [r0]      ; canary GOT ptr
ours 137d26: 4683        mov    fp, r0
     137d30: f8d0 a000   ldr.w  sl, [r0]
```

Adding `__builtin_expect(m_bStartSequence != 0, 0)` at the shared-tail guard
(L1455, `if (m_bStartSequence != 0)`) flips the tie to the original's map:
`sl`=this, `fp`=canary.  The prologue becomes byte-identical (up to the
literal-pool `ldr r0,[pc,#…]` offset), and the mid-function return epilogue
becomes **byte-identical**:

```
ours (expect 0) 144ea8: 986b       ldr   r0, [sp,#428]
                144eaa: f8db 1000  ldr.w r1, [fp]      <-- was [sl]
                144eae: 1a08       subs  r0, r1, r0
                144eb0: bf01       itttt eq
                ... same itttt/vpopeq/ldmiaeq/popeq sequence as orig ...
```

Measured (one guard, full link):

| variant | raw | aligned | fn size | .text size | .rel.plt diff |
|---|---:|---:|---:|---:|---:|
| base | 55,048 | 17,049 | 57,232 | 1,259,464 | 0 |
| **L1455 `expect(...,0)`** | **55,371** | **16,749** | **57,252 (+20)** | **1,259,496 (+32)** | **9 B** |

So the epilogue fix is real, but the cold move costs +20 function bytes
(+32 rounded `.text`) **and reorders PLT relocations** (the m_bStartSequence
block is the first reference for `getWingmen`/`getEnemies`-adjacent slots).
The original keeps the block **inline** at `0x137f7c` (body is the fall-through
of `cmp r0,#0; beq 13812a`), so moving it cold is structurally wrong for
`.rel.plt` even though it flips the tie.  This is the key new finding: the
tie flip and the inline-block placement are in tension.

### The -20 B companion search (no size-exact hit)

The current base makes the expect cost +20 (not the +32 seen on the wave-160
base).  Exhaustive combinations of the remaining `expect` hints were built and
linked.  Function size is quantised in 4-byte steps and the best combinations
land on 57,240/57,244/57,248/57,256 — never 57,232:

| combo (on top of L1455:0) | raw | aligned | fn size |
|---|---:|---:|---:|
| +1507:0+1469:1 | 55,340 | 16,792 | 57,240 |
| +1522:0+1469:1 | 55,434 | 16,733 | 57,240 |
| +1526:0 | 54,823 | 16,699 | 57,260 |
| +1526:0+1469:1+1425:0 | 54,859 | 16,624 | 57,256 |
| +1507:0+1469:1+930:0 | 53,936 | 18,311 | 57,272 |
| +B8 (move unreachable 0.8f block) | 55,210 | 19,686 | 57,284 |

The `0.8f` block move (w10603's -20/-24 standalone lever) interacts badly:
+76 B when combined, because it re-orders the same cold region.

**Conclusion:** the L1455 expect(0) shape cannot be made size-exact with any
`expect`-only companion on this tree.  A real -20 B code-shape change is needed
(not a hint), and it must also preserve the inline PLT-call order.

## 2. `__builtin_expect` sweep on the CURRENT base (deliverable)

One guard at a time, full link.  Baseline 55,048 / 17,049 / 57,232.
`NEUTRAL` = identical raw/aligned/size (clang already lays the branch out
that way; the hint is ignored).  Everything else **flips layout**, and almost
everything that flips also changes the function size (gate-fatal).

| guard | expect | raw | aligned | fn size | verdict |
|---|---|---:|---:|---:|---|
| L615 mission==0 | 0 | 55,468 | 22,005 | 57,428 | flip |
| L620 introMsg->isOver()&&state==0 | 0 | 55,486 | 22,314 | 57,372 | flip |
| L620 | 1 | 55,658 | 19,434 | 57,464 | flip |
| L653 msg6->isOver() | 0 | 55,123 | 17,103 | 57,240 | flip |
| L653 | 1 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L670 messages[7]->isOver() | 0 | 55,225 | 17,074 | 57,236 | flip |
| L670 | 1 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L687 messages[10]->isOver() | 0 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L687 | 1 | 55,462 | 19,662 | 57,184 | flip |
| L728 messages[13]->isOver() | 0 | 55,291 | 17,052 | 57,232 | flip (size-exact) |
| L728 | 1 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L731 messages[14]->isTriggered() | 0 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L731 | 1 | 55,454 | 19,676 | 57,192 | flip |
| L740 messages[8]->isOver() | 0 / 1 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L883 messages[17]->isOver() | 0 | 55,127 | 18,121 | 57,252 | flip |
| L920 messages[22]->isOver() | 0 | 55,200 | 18,576 | 57,240 | flip |
| L930 state==16 | 0 | 55,233 | 18,595 | 57,244 | flip |
| L930 | 1 | 55,153 | 18,538 | 57,244 | flip |
| L931 !isFading() | 0 | 55,245 | 17,294 | 57,240 | flip |
| L931 | 1 | 54,910 | 18,181 | 57,264 | flip (raw win, +32) |
| L962 msg2->isOver()&&state==0 | 0 / 1 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L966 state==1&&!isFading() | 0 | 55,107 | 18,614 | 57,436 | flip |
| L966 | 1 | 55,542 | 18,686 | 57,536 | flip |
| L1024 messages[3]->isOver() | 0 | 55,245 | 17,050 | 57,240 | flip |
| L1024 | 1 | 55,217 | 17,111 | 57,244 | flip |
| L1031 !isFading() | 0 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L1031 | 1 | 55,051 | 17,049 | 57,232 | flip (size-exact, +3) |
| L1356 messages[2]->isOver() | 0 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L1380 state==3&&messages[4]->isTriggered() | 0 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L1383 state==4&&messages[4]->isOver() | 0 | 55,084 | 17,157 | 57,240 | flip |
| L1395 state==6&&messages[5]->isOver() | 0 | 55,201 | 17,138 | 57,236 | flip |
| L1425 state==0 | 0 | 55,192 | 17,068 | 57,224 | flip (-8) |
| L1430 messages[0]->isOver() | 0 | 55,213 | 17,041 | 57,224 | flip (-8) |
| L1430 | 1 | 55,185 | 17,048 | 57,224 | flip (-8) |
| L1507 state==0 (mission 5) | 0 | 55,254 | 17,120 | 57,216 | flip (-16) |
| L1517 messages[1]->isTriggered()&&state==0 | 0 | 55,048 | 17,049 | 57,232 | **NEUTRAL** |
| L1522 mission==50 | 0 | 55,238 | 17,076 | 57,224 | flip (-8) |
| L1526 mission==51 | 0 | **54,959** | 17,237 | **57,232** | **flip, size-exact raw win** |
| L1526 | 1 | 55,225 | 17,092 | 57,224 | flip |
| L1532 count>=3&&… | 0 | 55,311 | 17,152 | 57,232 | flip (size-exact) |
| L4026 state>=4&&state<=6 | 0 | 55,211 | 21,167 | 57,264 | flip |
| L4026 | 1 | 55,051 | 17,056 | 57,232 | flip (size-exact, +3) |
| L1455 m_bStartSequence != 0 | 0 | 55,371 | **16,749** | 57,252 | **tie flip, +20** |
| L1455 | 1 | 55,373 | 18,428 | 57,420 | flip |

**Neutral set (hint ignored):** L653(1), L670(1), L687(0), L728(1), L731(0),
L740(0/1), L931(0), L962(0/1), L1031(0), L1356(0), L1380(0), L1517(0).
**Size-exact flippers:** L728(0), L1031(1), L1526(0), L1532(0), L4026(1),
plus the whole neutral set.  Only **L1526(0)** improves raw (-89).

## 3. L1526:0 — size-exact raw win, but `.rel.plt` breaks

`} else if (__builtin_expect(Globals::status->getCurrentCampaignMission() == 51 &&
messages != nullptr, 0))` gives raw **54,959** (-89), aligned 17,237 (+188),
function 57,232 B and `.text` 1,259,464 B (both exact).  But the mission-51
cold move rotates two `.rel.plt` entries (16 raw bytes) and shifts the PLT
slots used by `MGame::successCheck`, `MGame::switchCamera`,
`MGame::dialogueEvent`, `MGame::gameOverCheck` and `BombGun::update`, so
`byte_exact` drops **4507 -> 4502**.  `check` therefore FAILS.  Not landed.

## 4. Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10705/fdiff.py _ZN11LevelScript7processEi        # 55,048 / 57,232
python3 /dev/shm/w10705/align_diff.py _ZN11LevelScript7processEi  # 17,049
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS (base)
```

## Open questions

1. A **real** -20 B code-shape change (not an expect hint) that keeps the
   m_bStartSequence block inline and its PLT-call order intact would let the
   L1455 tie flip land at 57,232.  The cold move itself is what breaks
   `.rel.plt`; the tie flip alone is desirable.
2. L1526:0 is the best raw lever found (-89) and is size-exact; it needs a
   companion that restores the two rotated `.rel.plt` entries (or the
   coordinator may accept a `.rel.plt` reorder if the PLT-slot shift is
   re-baselined).
3. The `expect`-neutral set above is safe to add anywhere without codegen
   effect; do not waste passes on them.
