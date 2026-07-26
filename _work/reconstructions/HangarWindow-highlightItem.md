# HangarWindow::highlightItem — landed partial, deferred prologue scheduling

Symbol: `_ZN12HangarWindow13highlightItemEP8ListItem`

Landed metric: 92.5%, original 100 bytes, ours 100 bytes, not linked. Original
address: `0x14bbec`.

The landed pass reverses the two source assignments so clang emits the original
late store order: `selectedItem` at `this+0x68`, then `sellConfirmPending` at
`this+0xd2`. All later instructions align.

## Residual

```text
original                                ours
mov     r5, r1                        | cbz     r1, exit
mov     r4, r0                          mov     r4, r0
cbz     r1, exit                      | mov     r0, r1
mov     r0, r5                        | mov     r5, r1
```

The selectable call and remainder then converge.

## Source shapes gated

1. Minimal assignment-order reversal (landed).
2. Ordered `item` and `this` local aliases, with the parameter used for the
   null guard and aliases throughout the body.
3. The same aliases plus nested null/selectable guards instead of the original
   short-circuit `&&`.

All three produce identical prologue codegen. Requeue only with evidence of a
genuine source lifetime/control dependency that makes both arguments live
before the null check; do not use volatile or opaque barriers.
