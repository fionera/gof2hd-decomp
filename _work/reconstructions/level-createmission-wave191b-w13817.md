# Level::createMission — wave 191b w13817

Target: `_ZN5Level13createMissionEv` (0xaffd8, 17,172 B). Base master `342cf7a86`
(HEAD before this pass: raw 3,734 / aligned 465 / size 17,172 / insn 6201/6197,
`.text` differing bytes 76,158, `.ARM.extab` 594, `.ARM.exidx` 0, `.rodata` 0).

Slice assigned to this worker: the **case-1 `enemyRace` reload** and the
**shared tail** (the `goto case4Tail` loop at 0xb4152..0xb41ec plus the
exception landing pad at 0xb41f0..0xb42e8).  The w13773 "load `gun` after the
store" and case-4 tail work is already landed and was not redone.

All numbers below are from `tools/fn_diff.py` on a `link_so` build.  Every
variant keeps `.rel.plt` byte-identical.  Size must stay 17,172: a function
whose size changes shifts every following function, which is why the
size-exact column matters.

## Landed: raid block reads `Globals::globals` directly

`case` raid block: `Globals *volatile &globals = Globals::globals;` was an
artificial reload anchor.  Reading `Globals::globals->getRandomEnemyFighter(...)`
directly is both more faithful and a (tiny) instruction win:

| | raw | aligned | size | `.text` | `.ARM.extab` | `.ARM.exidx` |
|---|---:|---:|---:|---:|---:|---:|
| before | 3,734 | 465 | 17,172 | 76,158 | 594 | 0 |
| after  | 3,735 | 464 | 17,172 | 76,159 | 594 | 0 |

The `b3752..b399e` register-pressure cluster drops 102 -> 101.  (`AERandom
*volatile &rnd` is still needed; removing it too gives aligned 464 as well but
raw 3,736.)

## Finding: case-1 `enemyRace` reload is unreachable from source

Original case-1 attacker loop (0xb1608..0xb1648):

```
b158c  mov.w r9, #1              ; preheader
b1590  movs  r6, #0              ; i = 0
b1592  add   r0, pc
b1594  ldr.w r8, [r0]            ; globals
b1598  b.n   b1644
b159a  nop                       ; 4-byte alignment before the switch jump table
b159c  ... switch jump table (data) ...
b1608  ldr   r5, [sp, #116]      ; enemyRace reloaded *inside* the loop
b160a  ldr.w r0, [r8]
b160e  mov   r1, r5
b1610  blx   getRandomEnemyFighter
```

Ours hoists the `ldr r5, [sp, #116]` into the preheader at 0xb158c.  The
preheader is the same size in both builds (the extra 2-byte load pushes the
`b.n` from 0xb1598 to 0xb159a and removes the alignment `nop`), so the only
net cost is the 2-byte reload the loop body loses; the region is +2 for the
original.  Every source shape tried for the loop reproduces HEAD byte for
byte:

- `const int race = enemyRace;` copy temp inside the body,
- `int race = enemyRace;` copy temp,
- signed `int i` / `!=` loop forms,
- `(void)`/address-taken forms,

and `volatile int enemyRace` over-forces it (raw 16,452, aligned 2,125,
size 17,232).  The reload is a register-allocator consequence of the whole
switch's live ranges, not a local source shape.

## Finding: the shared tail wants the `enemies` alias, but that is -8 B

The case-4 tail loop (Ghidra: `iVar15 = getRandomEnemyFighter(globals, 8);
... createShip(...); setToSleep(...)`) reloads `this->enemies` for the store,
the `setToSleep` receiver and the loop bound.  The original's tail keeps
`this` in **r8**; ours keeps it in **sl**.  Introducing the local alias

```
Array<KIPlayer *> *enemies = this->enemies;
for (unsigned i = 0; i < enemies->size(); i++) {
    (*enemies)[i] = this->createShip(...);
    (*enemies)[i]->setToSleep();
}
```

frees a register, moves `this` to r8 in the tail and collapses the tail
cluster from cost 106 to 42 — aligned 465 -> **403** — but the build is
17,164 B (-8).  The -8 is not local to the tail: it is spread over the same
`b3274..b3510`, `b3752..b399e`, `b3dd0..b3dfa` register-pressure clusters
(unchanged by the alias) plus the landing pad, so no small compensating
statement restores exactly +8 without destroying the win.

Measured tail variants (all `.rel.plt` identical):

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---|
| HEAD | 3,734 | 465 | 17,172 | 6201/6197 |
| full alias (T1) | 3,786 | **403** | **17,164** | 6197/6197 |
| alias + volatile refs removed | 3,788 | 402 | 17,164 | 6197/6197 |
| alias for store only, `this->enemies` bound (S1) | 3,620 | 469 | 17,172 | 6201/6197 |
| alias for store+`setToSleep`, bound `this->enemies` (T6) | 3,742 | 471 | 17,172 | 6201/6197 |
| alias + hoisted `n` (T8) | 3,771 | 472 | 17,172 | 6201/6197 |
| no alias, `this->enemies->size()` bound (p_c4noalias) | 3,787 | 457 | 17,168 | 6197/6197 |
| no alias + `unsigned n` hoisted (T4/T5) | 3,772 | 466 | 17,172 | 6201/6197 |
| alias + `KIPlayer *ship` declared outside the loop (X1) | 3,787 | **402** | **17,164** | 6197/6197 |
| alias + `ship` outside + named `type` temp (X4) | 3,787 | **402** | **17,164** | 6197/6197 |
| alias + `this->enemies = enemies;` (U1) | 3,760 | 478 | 17,172 | 6200/6197 |
| alias declared before `ArraySetLength` (W1) | 4,448 | 480 | 17,160 | 6196/6197 |

Note `p_c4noalias` (the "faithful" reload-every-use body) is -4 B, and adding
the alias for the bound only (T6) is size-exact but aligned 471.  The aligned
402-403 of the full alias cannot be landed under the size-exact gate until the
remaining register-pressure clusters are reconstructed.  The coupling is
exact: every size-exact tail shape lands at aligned 466-472, and every
aligned 402-403 shape is -8 B, so the missing 8 bytes are the same
register-allocation decision that the alias changes in the `b32xx`/`b37xx`
clusters, not a statement missing from the tail.

## Landing pad (0xb41f0..0xb42e8)

The tail is a clang EH cleanup dispatch: a run of `b.n` to shared cleanup
blocks, with one embedded 4-byte data word at 0xb423e.  Its layout follows the
scoped-destructor order, so it is compiler-generated; the full alias shifts
`this` to r8 here too and is the only source lever found that changes it.
