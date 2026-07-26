# FModSound::stopAllSoundFXEvents() — deferred induction scheduling

Symbol: `_ZN9FModSound20stopAllSoundFXEventsEv`

Current metric: 85.7%, original 50 bytes, ours 50 bytes, not linked. Original
address: `0x92dac`. All field offsets, the virtual call, mask, bounds, and
function size match.

## Stuck diff

```text
original                                ours
movs    r0, #0                        | movs    r5, #0
b.n     0x22                          | b.n     0x20
...
mov     r0, r5                          mov     r0, r5
adds    r5, r0, #1                   | adds    r5, #1
```

The original keeps the current index in caller-clobbered `r0`, derives the next
index in call-preserved `r5`, indexes the category array with the old `r0`, and
copies `r5` back to `r0` on both the skip and post-call edges.

## Source shapes gated

1. One persistent `for` index with `continue` and `break`.
2. A current `i` plus short-lived call-live `next`, with explicit assignments
   on the skip and post-call paths.
3. A `for` iteration expression carrying `i`, plus an explicit `next = i + 1`
   inside the body.

All three retained the 50-byte function but compiled to the same baseline
register allocation: persistent `r5`, initialization in `r5`, and in-place
increment.

## Requeue condition

Do not force registers with `volatile`, opaque helpers, compiler controls, or
alias tricks. Requeue only after identifying a binary-witnessed value or
callee/control dependency that requires the current index to remain in `r0`
and the next index to survive in `r5`.
