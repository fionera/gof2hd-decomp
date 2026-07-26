# PlayerFighter::cloak(int, bool) — deferred store-scheduling residual

Symbol: `_ZN13PlayerFighter5cloakEib`

Current metric: 91.7%, original 68 bytes, ours 68 bytes, not linked. Original
address: `0xdcd58`. The RNG call, constants 5000/4000, selected duration,
offsets, and final flag value all match.

## Stuck diff

After the RNG/positive-duration join, the original stores the active byte
before computing and storing the duration:

```text
orig                                      ours
movs    r0, #1                          | add.w   r0, r1, #4000
strb.w  r0, [r5, #720]                 | str.w   r0, [r5, #716]
add.w   r0, r1, #4000                  | movs    r0, #1
str.w   r0, [r5, #716]                 | strb.w  r0, [r5, #720]
strb.w  r4, [r5, #729]                   strb.w  r4, [r5, #729]
```

Root confirmed this on a clean rebuilt linked shared object. A worker's
loose-object inspection appeared exact, but that object view is not the
campaign gate authority.

## Source shapes tested

1. Baseline separate unsigned selected-duration temporary.
2. In-place normalization of the `dur` parameter before the same field stores.

Both produce the current linked code. Two further Sol audits found no hidden
value or control dependency among the three fields.

## Hypothesis / requeue condition

The byte and word fields do not alias, so clang is free to compute and issue
the duration store first despite source order. Do not force ordering with
`volatile`, comma/dependency tricks, aliasing, opaque helpers, or compiler
controls. Requeue only with evidence of a genuine source dependency or after
another authentic `PlayerFighter.cpp` change shifts scheduling.
