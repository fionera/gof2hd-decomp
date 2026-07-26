# `MGame` sound-loop MachineSink deferral

## Targets

- TU: `src/game/menu/MGame.cpp`
- `_ZN5MGame11pauseSoundsEv`: 96.4%, 72/72 bytes
- `_ZN5MGame12resumeSoundsEv`: 96.2%, 64/64 bytes

## Residual

Both functions call `Level::getEnemies()`, which returns the nullable
`Array<KIPlayer *> *` stored in `Level::enemies`. Their only executable
structural difference is the placement of the returned-pointer copy:

```text
original  blx Level::getEnemies()
          mov r4, r0
          cbz r0, return

current   blx Level::getEnemies()
          cbz r0, return
          mov r4, r0
```

The null edge immediately returns from the void function. The pointer is used
only on the non-null edge, where `r4` holds it for the Array count/data loop.
LLVM MachineSink may therefore place the copy in that successor.

Aliases, cached pointers or references, positive-condition versus early-return
spelling, `for`/`while` loop variants, pointer-comparison variants,
cast-to-void, and deleting the known-null pointer all retain equivalent SSA or
optimize away. A dereference is undefined on the null edge; a call, store,
volatile access, barrier, or opaque fake use changes behavior or violates the
campaign rules.

Everything else in both bodies is structurally identical or link-only.
Requeue only with real type/source evidence that gives the array pointer an
observable null-edge use. Confidence is high.
