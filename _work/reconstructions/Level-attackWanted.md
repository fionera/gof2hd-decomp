# Level::attackWanted(int) — partial landed, eager-index residual

Symbol: `_ZN5Level12attackWantedEi`

Wave 63 changed the logical wingman loop to zero-based with physical access at
`i + 1`. That made the complete loop tail exact and improved the function from
90.0% to 97.5% at 108/108 bytes.

## Remaining linked diff

```text
orig                                      ours
mov     r5, r0                            mov     r5, r0
ldrb.w  r0, [r0, #668]                    ldrb.w  r0, [r0, #668]
mov     r8, r1                          | cbnz    r0, return
cbnz    r0, return                      | mov     r8, r1
```

Everything after this is structurally identical, including the landed loop
tail:

```text
adds r6, #1
...
subs r1, r6, #1
cmp  r1, r0
blt  loop
```

The original address is `0xc6388`. A final worker reported a loose-object view
as exact, but root rebuilt and linked the clean tree and confirmed the
eager-save residual above with authoritative `verify-fn`; the 97.5 report is
current.

## Hypothesis / requeue condition

`index` has no semantic use when `field_29c` is already set, so the pinned
compiler sinks `r8 = index` after the early return. Preloading wanted/wingman
state would add unmatched observable work. Do not retry with dummy aliases,
`volatile`, artificial dependencies or escapes, or helper calls. Requeue only
if new original-source evidence reveals genuine pre-guard work or another
authentic `Level.cpp` change alters allocation.
