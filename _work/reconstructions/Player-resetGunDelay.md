# Player::resetGunDelay(int) — deferred guard-scheduling residual

Symbol: `_ZN6Player13resetGunDelayEi`

Current metric after wave 63: 90.5%, original 50 bytes, ours 50 bytes, not
linked. Original address: `0xa39aa`.

## Stuck diff

```text
orig                                      ours
ldr     r0, [r0, #0]                    | cmp     r1, #0
cbz     r0, return                      | blt     return
cmp     r1, #0                          | ldr     r0, [r0, #0]
blt     return                          | cbz     r0, return
```

Every subsequent instruction matches: array size/bounds check, selected inner
array load and null check, and the loop storing zero to each `Gun+0x6c` timer.

## Source shapes tested

The pinned NDK r18b clang `-Oz` build scheduled the independent negative-slot
test before the guns-pointer load for all of these equivalent forms:

1. Baseline cached `guns` pointer with two separate early-return guards.
2. `if (guns == 0 || slot < 0) return;` with the guns operand first.
3. Fully positive nesting: `if (guns != 0)`, then `if (slot >= 0)`, then the
   bounds and body.
4. The positive-conjunction form also exists in adjacent
   `Player::refillGunDelay`; its generated code schedules the slot comparison
   first too.

The final Sol retry restored the baseline body.

## Hypothesis / requeue condition

The null-pointer and signed-index guards are independent under the C++ as-if
rule. The original compiler scheduled the load first; the pinned build
schedules the comparison first. Enforcing the original order would require a
prohibited artificial dependency, `volatile`, or compiler forcing. Requeue
only with original-source evidence for a genuine dependency or after another
authentic `Player.cpp` change demonstrably alters this function's scheduling.
