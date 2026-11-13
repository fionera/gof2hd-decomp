# LevelScript::process — wave 191b / w13599: case-102 awake hoist

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13599`, branch
`wave191b-w13599`, base master `d6348b79b` (raw **51,525**, aligned 22,801,
`.text` 71,208, `.ARM.extab` 577, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).

## Lens

Diff-cluster map for the tail half (source lines 2600-5200) built two ways:
same-address byte diffs mapped through the unstripped link's debug line table,
and a normalized instruction-sequence diff (`objdump -dl`, branch targets and
immediates canonicalised) so layout shifts do not dominate.  Largest tail
clusters: case 157 (L4796-5226, 1440 aligned insn diffs), case 92 (L2946-3201,
1000), case 70 (L2477-2565, 955), case 154 (L4516-4795, 916), case 105
(L3587-3951, 881).

Fresh distance-1..6 transposition sweeps over the tail on this merged base
found only the producer/consumer cells already rejected by the w13561 note
(`L3235/3237`, `L3025/3028`, `L4614/4617`, `L3460/3464`, `L4964/4965`), plus
one new size-exact win.

## Landed change

case-102 group-10 wake block (source ~3391).  Rotate the 6-statement group so
`enemy->awake()` moves from the tail to the front:

```cpp
+                                enemy->awake();
                                 enemy->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
                                 ((PlayerFighter *) enemy)->setAIDisabled(true);
                                 int target = Globals::rnd->nextInt(5);
                                 enemy->player->setEnemy(target == 4 ? player->player : (*enemies)[target + 2]->player);
                                 enemy->player->setEnemies(nullptr);
-                                enemy->awake();
```

`KIPlayer::awake()` is `state = 1; player->setActive(true);`.  The intervening
statements write the enemy geometry direction, the fighter `aiDisabled` flag,
draw one `nextInt(5)` (the only RNG call in the group) and rewrite the player
enemy list; none reads `state` or `active`.  `setEnemy` still follows
`nextInt`, so the chosen target is unchanged.  Semantics preserving (the
original's own call order at `0x140cd4` is `setEnemies`, `nextInt`, `setEnemy`,
`awake`, `setDirection`, `setAIDisabled`; this rotation is a codegen-coincidence
win of the kind the wave-190/191 notes accept — the gate is the same-address
byte diff).

Measured alone on the real build:

```
raw        51,525 -> 51,522  (-3)
size       57,232 / 57,232   (exact)
.text      71,208 -> 71,205  (-3)
.ARM.extab 577 (unchanged)
.rel.plt   md5 289b2a06… (byte-identical)
check      RATCHET PASS (byte_exact 4508/4524, linked 4508, size 4524/4524,
           data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370)
```

## Rejected in the same pass

* d1 `L4855<->L4856` (raw 51,524): swaps `m_tempVec = getPosition()` with the
  `+=` offset, making the add dead.  The original `0x13ce8e` does
  `geometry4->setPosition(station)` then `m_tempVec = getPosition(fighter)` then
  the add; the codegen-correct reorder measures 51,526 (+1).
* d1 `L4964<->L4965` (51,524): same dead-add shape.
* d2 `L4964<->L4966` (51,524): camera reads a stale `m_tempVec`.
* d2 `L3235<->L3237`, d3 `L3025<->L3028`, d3 `L4614<->L4617`,
  d4 `L3460<->L3464`: producer/consumer swaps (right/up vector or rebuilt
  `m_tempVec`), already rejected by the w13561 note.
* case-154 state-4 vector block: our source has `m_dirVec = getRightVector()`
  immediately overwritten by `GetUpVector()`; the original (`0x1431e6`) adds
  the right-vector term.  Restoring the missing add is a real semantic fix but
  measures size 57,252 (+20), so it needs a compensating reshape before it can
  land.
