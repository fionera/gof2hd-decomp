# ModStation::ModStation ctor wave 132 (w7719): pool order is reachable, schedule is not

Date: 2026-11-07. Worktree `wt/w7719`. Target `_ZN10ModStationC1Ev` / `C2`,
`src/game/menu/ModStation.cpp`, 1008 B, 5 differing instructions at HEAD.
Extends `modstation-ctor-wave130-w7682.md` (and 123/124/125).

## 1. New data points

The wave-130 note proved the `Yaw`/`YawIPad` LDRLIT pick is a bottom-up
`BURRSort`/`NodeQueueId` tie-break and that the faithful `far = pitch` variant gets the
original register roles (`r3 = YawIPad`, `r4 = Yaw`) but the pool order
`Pitch, Yaw, YawIPad`. Two new source levers were found this wave.

### 1.1 A non-canonical select *does* flip the pool

`Globals::iPad ? YawIPad : Yaw` is canonicalised by InstCombine to
`select (icmp eq 0), Yaw, YawIPad`, so the DAG order is `Yaw, YawIPad`. Writing the
condition as `(signed char) Globals::iPad > 0` survives as
`select (icmp sgt 0), YawIPad, Yaw`, and the emitted pool becomes
**`Pitch, YawIPad, Yaw`** — the original order. But the condition code becomes `gt`
(`it gt; movgt`) instead of `eq`, the register roles mirror (`r0 = Yaw; movgt r0,
YawIPad`), and the function shrinks to 1000 B / 257 diff lines. So the pool order is
reachable, but not with the original `eq` select.

### 1.2 A non-volatile yaw load flips the pool *and* keeps the roles

Keeping `far = pitch` and dropping `volatile` from the **yaw** load only:

```cpp
int pitchBits = *(const volatile int *) &pitchTable[race];
int yawBits   = *(const int *)          &yawTable[race];
```

emits the original pool order `Pitch, YawIPad, Yaw` **and** the original roles
(`r3 = YawIPad`, `r4 = Yaw`, `moveq r0,r4`), 1008 B. The yaw `ldr.w r3,[r0,r8,lsl #2]`
is byte-identical to the original. The remaining 15 diff lines are pure scheduling:
the `Yaw` LDRLIT/`add r4,pc` pair is sunk after the pitch load
(`0x84f8`/`0x84fe` instead of `0xd543a`/`0xd5444`), and the pitch copy `mov r4,r2`
moves up. Dropping `volatile` from the pitch load instead (or both) changes the whole
function (259 diffs); dropping it from the yaw load is the only useful direction.

## 2. Why the committed baseline still wins

The committed baseline is `far = yaw` with both loads volatile: it keeps the original
pool order and only 5 differing instructions (the `r3`/`r4` yaw role + the `mov r3,r4`
and race-spill placement). The faithful `far = pitch` variants are 15-17 diff lines
because they also move the race spill and the pitch copy. The gate counts byte-exact
functions, not diff bytes, so nothing is gained by landing a variant with more diffs.

## 3. Conclusion

The pool order is not an irreducible fact: it follows the `select` arm order in the
DAG. The original's `eq` select with `YawIPad` as the `$false` (tied) operand gives
`YawIPad` latency 3 / `Yaw` latency 1, so the bottom-up scheduler picks `YawIPad` first
and the reversed emission is `Pitch, Yaw, YawIPad`. To match, the source must both
(a) keep the `eq` select shape and (b) make the `Yaw` LDRLIT ready/scheduled first.
No source spelling tested (waves 123-132, ~1000 shapes) produces both.

Gate before/after: byte 4506 -> 4506, linked 4506 -> 4506, size 4524/4524, data 843/843,
imports 207, lint CLEAN, drift 0, RATCHET PASS.
