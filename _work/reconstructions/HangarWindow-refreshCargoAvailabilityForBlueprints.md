# `HangarWindow::refreshCargoAvailabilityForBlueprints` checkpoint

Target: `_ZN12HangarWindow37refreshCargoAvailabilityForBlueprintsEv`

- Original: `0x14900c`, 188 bytes.
- Landed: 188 bytes, 98.6%.
- Previous: 188 bytes, 93.0%.
- TU: `src/game/ui/HangarWindow.cpp`.

The landed source snapshots `bp->ingredientCounters` as an `Array<int> *`
but reloads its `data()` pointer for each ingredient:

```cpp
Array<int> *counters = bp->ingredientCounters;
if (counters->data()[j] > 0) {
    // cargo scan
}
```

This restores the original per-iteration `ldr r0, [r8, #4]` data-pointer
load. The ingredient list, cargo list, inner cargo scan, item-index call,
craftable flag at `ListItem +0x45`, loop bounds, calls, and function size now
match.

Only one three-instruction schedule remains after `getIngredientList()`:

```text
original  mov r6, r0
          cbz r5, end
          ldr.w r8, [r8]

landed    cbz r5, end
          ldr.w r8, [r8]
          mov r6, r0
```

Here `r5` is cargo, `r6` is the ingredient list, and `r8` initially holds
the blueprint before becoming `ingredientCounters`. Both values moved past
the cargo null guard are dead on its null edge, so LLVM may sink them.

A direct `bp->ingredientCounters->data()[j]` expression was compiled and
gated twice. It disturbed the cargo/ingredient register assignment, added a
dereference in the loop, and regressed the decoded instruction alignment.
It was rejected. Requeue only with new authentic source evidence for
pre-guard liveness; do not add dummy uses, volatile aliases, asm,
attributes, or other compiler-forcing constructs.
