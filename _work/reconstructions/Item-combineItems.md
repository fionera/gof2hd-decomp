# `Item::combineItems` deferred reconstruction

## Target

- Mangled: `_ZN4Item12combineItemsEP5ArrayIPS_ES3_`
- TU: `src/game/mission/Item.cpp` (`Classes/Game/Item.cpp`)
- Original address: `0xe034a`
- Current gate: 99.2%, original/current size 338/338, not linked-exact

## Remaining diff

The complete function is structurally identical apart from entry scheduling:

```text
original 0xe0352  mov r6, r1
original 0xe0354  mov r9, r0
original 0xe0356  cmp r0, #0
original 0xe0358  beq.w 0xe047a

current  0xd4b6c  mov r6, r1
current  0xd4b6e  cmp r0, #0
current  0xd4b70  beq.w 0xd4c94
current  0xd4b74  mov r9, r0
```

At the tail, original `0xe0478` copies `r9` to `r6` for the
`stationItems == nullptr` result, then `0xe047a` returns `r6`. The first null
edge returns `stationItems` directly from `r6`.

## Attempts

1. Moving the existing `ItemArray *result = items;` declaration ahead of both
   null checks was semantically neutral but produced no byte change.
2. Fresh analysis identified LLVM MachineSink behavior: `items` is dead on the
   first null edge, so an ordinary alias remains eligible to sink.
3. A final fresh retry found no normal-C++ restructuring that makes `items`
   genuinely live on that edge while still returning exactly `stationItems`.

All attempted source edits were reverted. No gate regression was landed.

## Blocker and requeue condition

The likely original shape used a single-exit result/phi whose allocation kept
`items` in `r9` from entry. With the current pipeline, aliases and conditional
result variables simplify to the same SSA liveness. Forcing the copy earlier
would require observable optimizer opacity (volatile state or an external side
effect) or unsafe pointer manipulation, none of which is semantically justified.

Requeue only with a proven normal-C++ formulation that changes the value's
entry-edge liveness without adding instructions or observable behavior.
