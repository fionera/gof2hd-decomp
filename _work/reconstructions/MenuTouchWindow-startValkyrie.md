# MenuTouchWindow::startValkyrie

Symbol: `_ZN15MenuTouchWindow13startValkyrieEv`

Status: landed partial at 89.2%, original/current size 612 bytes. The remaining
differences are concentrated in the Status/Options initialization block and
appear to be LLVM scheduling/register-allocation choices.

## Verified and landed

- The initial campaign-advance loop is an ascending fixed-trip loop:
  `for (int i = 0; i < 0x2d; ++i)`. Because its index is unused, Clang lowers
  it to the original `r6 = 45` countdown with `subs`.
- The value `0x1a0a` is stored to `Status + 0x84`. The old `+0x8c` target is
  `previousShip` and is not written here.
- The tail copies `this->fadeValue` to Options `+0x2c`, then directly calls
  `ApplicationManager::SetCurrentApplicationModule(unsigned int)` with module
  ID 5. The former `ModuleTransitionThunk` indirect call was invented and used
  the wrong receiver ABI.

## Remaining block

The original and current code perform the same stores and calls with the same
constants and verified offsets, but materialize the Status, hints, options, and
achievements holders in a different order. Verified offsets include Status
`+0x84`; hints `+0x08`, `+0x0d`, `+0x1c`, `+0x20`, `+0x17`, `+0x0a`, `+0x15`,
`+0x13`, `+0x0f`, `+0x1e`, `+0x24`, `+0x36`, `+0x32`, and `+0x38`; options
`+0x34`; and achievement calls `(0x17, 3)` and `(0x1e, 1)`.

The stuck region begins with different literal/global-load scheduling:

```text
original: ldr r0,lit; ldr r1,lit; add r0,pc; ldr r2,lit; add r1,pc; ...
current:  ldr r0,lit; movw r3,6666; ldr r1,lit; add r0,pc; ldr r2,[r4]; ...
```

All three loop spellings using a descending source induction
(`i != 0`, `i > 0`, and explicit `do/while (--i)`) canonicalized to a negative
counter with `adds`; the ascending fixed-count spelling alone recovered the
original loop. Requeue the initialization block only with authentic source
evidence for its pointer declaration/lifetime order. Do not force the schedule
with volatile, dummy uses, assembly, attributes, or invented shims.
