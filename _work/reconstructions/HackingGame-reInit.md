# `HackingGame::reInit` reconstruction checkpoint

Target: `_ZN11HackingGame6reInitEv`

- Original: `0x14ed9c`, 420 bytes.
- Current: 420 bytes, 89.8%.
- TU: `src/game/menu/HackingGame.cpp`.

The setup, difficulty switch, array offsets, random calls, copies, solver
call, and canary path are already equivalent. The first structural residual
is the 40-iteration shuffle trip counter:

```text
original  movs r6, #40
          ...
          subs r6, #1
          ...
          cmp r6, #0
          bne shuffle

current   mvn.w r6, #39
          ...
          adds r6, #1
          ...
          cmp r6, #0
          bne shuffle
```

The original loop has an initial branch to its condition, then two
`nextInt(6)` calls and the swap body. The decrement is scheduled before
independent swap loads. The current negative trip counter is semantically
equivalent but two bytes wider, shifting several internal branches.

Three natural source variants were compiled and directly gated:

- signed positive condition: `i > 0; --i`;
- unsigned countdown: `unsigned i = 40; i != 0; --i`;
- condition-side post-decrement: `int i = 40; i-- != 0`.

All three canonicalized to the same current 420-byte output. Restoring the
outer scramble induction variable from unsigned to signed was also neutral.

The later residual before `solvableInNSteps` is:

```text
original  movs r0, #0; movs r2, #0; strd r0, r6, [sp];
          mov r0, this; movs r3, #0
current   movs r5, #0; mov r0, this; movs r2, #0; movs r3, #0;
          strd r5, r6, [sp]
```

The current build then reuses `r5` to clear the outer induction value after
a successful solver call. Verified constants are 40 shuffles, `nextInt(6)`,
six-element target/current/working arrays, `difficulty * 2`, and solver
arguments `(type, 0, 0, 0, local)`. Verified offsets are difficulty `+0x0`,
target `+0x4`, current `+0x1c`, working `+0x34`, rotation flags `+0x128`,
and timers `+0x12c`.

Requeue only with new original-source evidence for the shuffle induction or
solver-call expression. Do not add volatile counters, dummy uses, asm,
attributes, or other compiler-forcing constructs.
