# LevelScript::process — wave 170 / w11605: case-11 arg, mission-78 tail, inlined-vs-called

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Base master `50716a763` (wave 170).  Scratch `/dev/shm/w11605/`.

Entry baseline (rebuilt from the clean tree):

```
raw 53,932   aligned 17,900   size 57,232/57,232   insn 20,722/20,830
gate: byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. Case-11 translate arg — landed (size-exact, aligned win, raw +12)

Original case 11 (`0x13bd2c`) computes the third `translate` argument as
`(float)(delta * 3)` (`add.w r0,r4,r4,lsl#1; vmov s6,r0; vcvt.f32.s32 s6,s6`).
Our source had `(float) delta * 3.0f` (`vmov.f32 s0,#8` + `vmul`).
Changing it to `(float) (delta * 3)` makes the 15-instruction block match the
original instruction-for-instruction (only `sl`/`fp` differ):

```
raw 53,932 -> 53,944 (+12)   aligned 17,900 -> 17,862 (-38)   size 57,232
```

The +12 raw is a literal-pool/block-placement artifact: the block grows 4 B
(14 -> 15 instructions) and clang moves the shared 14-word literal island
from after case 11's continuation (`0x13bb04`) to before case 11
(`0x13ba98`), shifting the whole `0x13ba7a..0x13cc57` region by 4 B.  The
function's symbol size stays 57,232 because 4 B of internal padding is
absorbed.

Spellings all measure identically (53,944 / 17,862): `3 * delta`,
`(delta << 1) + delta`, `delta + delta + delta`, `delta * 3u`, `delta * 3L`,
`delta * 3 + 0`, local `int d3 = delta * 3`, local `float fwd`, a
`TargetFollowCamera *cam` alias, and `(float) (-delta) * 0.7f`.  No
size-exact spelling avoids the island move.

## 2. Cold-block escort fade order — landed (raw-neutral, faithful)

Original `0x13c82e` fades `enemies[0]` before `enemies[1]`
(`[data,#0]` then `[data,#4]`); our source had them reversed.  Swapping to
`enemies[0]` then `enemies[1]` reproduces the original call order.  Measured
raw 53,932 / aligned 17,900 (unchanged; the bytes were already mismatched).

## 3. Mission-78 dead tail — confirmed, no size-exact live form

The tail is source after the last `case` but inside the `switch` braces
(`LevelScript.cpp:1242`), so clang drops it.  Original `0x13c88c` is the
post-switch tail (`if (state>=5 && state<=8) translate(-delta*0.8f,0,0);
return flag;`).  On this base moving it out (no `return`) is **+32 B**
(57,264; raw 55,523) and tail-merges the translate call with mission-1's
call at `0x137e7e` (the tail lands at `0x13976c`, not after the switch).
Adding the `return` is size-fatal.  Removing the dead block is a no-op
(confirmed dropped).

## 4. Inlined-vs-called split — re-confirmed, not a lever

The 245-name PLT callee set is identical; `TargetFollowCamera::translate` is
a PLT call in both (not inlined).  `LevelScript::resetCamera` (only same-TU
callee) is a PLT call 51 times in both.  No TU move changes `process`
inlining.

## 5. Other residuals found (not landed)

* **Case 5 `m_nScriptTimerA = 0`**: original `strd r0,r0,[sl,#144]` (4 B);
  ours emits two `str.w r0,[fp,#0x90/0x94]` (8 B, +4).  Ten source variants
  (`0LL`, cast, local, reference, split ifs, same-line, `> 18000`, state
  before timer, explicit `*(long long*)&`) all keep the split; only the
  `__builtin_expect(...,1)` form changes size (-32, raw +1,521).  This is the
  natural -4 companion for case 11 but could not be triggered from source.
* **Case 12 dead store**: original emits `m_nState = 13` between the return
  flag load and the epilogue branch; ours drops it (-8 B).  Moving it before
  the `return` is +8 B (57,240; raw 54,916).
* **Case 6/7 branch shapes are swapped**: original case 6 uses `ittt ge`
  and case 7 uses `blt`; ours is the reverse.  Removing case 6's
  `expect(...,0)` restores `ittt ge` but is raw +1 (53,933 / 17,896).
* Case 2 negation (`(float)(-delta) * 0.5f`) is +8 B alone (57,240,
  raw 54,936), and c9 (case-9 state/timer swap) is -8 B but raw +415.

## Final (landed)

```
process raw 53,932 -> 53,944   aligned 17,900 -> 17,862   size 57,232
.text diffbytes 81,825 -> 81,837
gate: byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN
```

No size-exact raw-non-regressing variant of the case-11 fix was found; the
landed shape is a structural (aligned -38) win with an honest raw +12.
