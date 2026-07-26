# Sparks::update(int) — deferred optimizer residual

Symbol: `_ZN6Sparks6updateEi`

Current metric after wave 63: 90.9%, original 116 bytes, ours 116 bytes, not
linked. Original address: `0x15e660`. The active check, elapsed update, particle
loop, field offsets, and `addParticle` call all match.

## Stuck tail

For `kind == 1`, the original emits:

```text
cmp.w   elapsed, #500
bgt     deactivate
ldr     lifetime, [this, #28]
lsls    elapsed, elapsed, #1
cmp     elapsed, lifetime
ble     return
b       deactivate
```

The pinned build instead folds the shift into the comparison:

```text
cmp.w   lifetime, elapsed, lsl #1
blt     deactivate
b       return
```

The other-kind 500 test is equivalent. At the shared deactivation block, the
original writes the active byte at `+0x10` before the elapsed word at `+0x0c`;
ours schedules the non-aliasing word store first. The functions remain the same
116-byte size, but the normalized tail has 50 original instructions versus 49
ours.

## Source shapes tested

All produced the same current machine code:

1. Baseline inverse early-return structure with an explicitly doubled local.
2. Direct positive deactivation branches, duplicated per kind.
3. Explicit `doubledElapsed <<= 1` with a shared `deactivate:` label.

All source forms wrote `active = 0` before `elapsed = 0`; clang independently
reordered the stores. The final Sol retry restored the baseline body.

## Hypothesis / requeue condition

Clang `-Oz` canonicalizes the scalar comparison to a shifted operand and freely
schedules the non-aliasing zero stores. No authentic value reuse or aliasing
dependency is visible in the binary. Do not retry with `volatile`, dummy
dependencies, fake calls, alias tricks, or compiler forcing. Requeue only if
new original-source evidence reveals a genuine construct that consumes the
doubled value or constrains the member-store order.
