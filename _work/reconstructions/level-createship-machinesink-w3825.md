# Level::createShip — MachineSink is the x-sink mechanism (wave89-w3825)

**Status:** research note, 2026-11-03. Worktree `wt/w3825`, branch
`wave89-w3825`, base `5710f6d0`. No source change landed: every tested
source shape either ties or loses to the current 74.1% fuzzy. This commit
contains only this note.

Target: `_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B, `0xc0214`).

## Baseline

`cmake --build cmake-build-match --target check -j8`:

```text
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`createShip`: **74.1%**, 2764 B both sides, not byte/linked exact. `.text`
`0x1337c8`, lint `CLEAN`, drift 0. Instruction stream: orig 1007 vs ours
1008; mnemonic-only difflib ratio 87.4%.

## New evidence: where the x sink happens

The wave-88 note established *that* clang sinks the spawn `x` past
`Player::setEmpData`; it did not identify the pass. Dumping the clang 7
pipeline (`-mllvm -print-after-all -mllvm
-filter-print-funcs=_ZN5Level10createShipEiiiP8Waypointbb`) pins it down.

At the LLVM-IR level (last IR dump, `Safe Stack instrumentation pass`) the
three spawn computations are all **before** the `setEmpData` invoke:

```text
%117 = add nsw i32 %24, %26      ; wx + jitterX
%118 = sub nsw i32 %117, 20000   ; x
%119 = add nsw i32 %23, %30      ; wy + jitterY
%120 = sub nsw i32 %119, 20000   ; y
%121 = sitofp i32 %120 to float
%122 = add nsw i32 %22, %28      ; wz + jitterZ
%123 = sub nsw i32 %122, 20000   ; z
%124 = sitofp i32 %123 to float
invoke void @_ZN6Player10setEmpDataEii(...)
```

The SelectionDAG scheduler also keeps them before the call: in the
`Expand ISel Pseudo-instructions` dump, block `bb.17` (`%ir-block.109`)
contains `t2ADDrr`/`t2SUBrr` for y, z **and** x, followed by the `tBL
setEmpData`. So ISel is not the culprit.

The **Machine code sinking** pass then moves the integer x add/sub (and its
`COPY`) out of `bb.17` into `bb.20` (`%ir-block.127`, the post-invoke block
that holds the `shipClass` branch). y and z stay in `bb.17` because each is
consumed there by a `VSITOS` (int→float) that is not sinkable; x has no
non-debug use in `bb.17`, so it sinks to the common dominator of its uses.
This is exactly the observed final shape: y/z converted before the call
(s18/s16), x recomputed after the call from the adjacent `jX`/`wz` slots
with a single `ldrd`.

The original keeps x in `fp` across the call, so in the original build the
integer x was **not** sunk. Two possible causes remain:

1. the original's x has a non-sinkable use in the invoke block, or
2. the original's block structure has no post-invoke split, i.e.
   `setEmpData` is a plain call and the DAG scheduler puts the adds before
   it.

Cause 2 is consistent with the original's EH shape (see below), but our
no-try builds do not reproduce it: without the `try`, `setEmpData` is
emitted as a `tail call` and the DAG scheduler hoists it **above** the
adds, so x ends up after the call anyway (measured 2752 B / 61.5%).

## Original EH shape (cleanup-only, no catch)

The original's landing pad at `c0bcc` is `mov r8,r0; mov r0,r5; blx
_ZdlPv; mov r0,r8; blx _Unwind_Resume` — a cleanup pad, not a catch. There
is no `__cxa_begin_catch` anywhere in the function. The pad variants delete
`r5` (24×, the `BoundingAAB` allocations), `sl` (PlayerFighter), `fp`
(Array<BoundingVolume*>), and `r9` (PlayerFixedObject). So the original has
no `try/catch`; the pads come from the plain `new` expressions. The current
source's `try { setEmpData } catch (...) { __builtin_trap(); }` is an
anchor, not the original EH shape. Replacing the placement-new
PlayerFighter/PlayerFixedObject with plain `new` (the faithful shape) adds
the missing cleanup pads but changes allocation and drops the fuzzy score
(2752–2792 B, 61.6–64.3%).

## Stack-slot colouring is downstream of the sink

Original frame (sp+28..60): `jZ@28, jY@32, jX@36, wz@40, wx@44, wy@48,
type@52, this@56, race@60`.
Current frame: `jX@28, wz@32, jZ@36, jY@40, wx@44, wy@48, type@52,
this@56, race@60`.

The waypoint/jitter values are the same set; only the colouring differs.
The current `ldrd r1,r0,[sp,#28]` for the sunk x is possible only because
`jX` and `wz` are adjacent; the original's `jX@36`/`wx@44` are not, so a
sunk x would need two loads. This is a consequence, not a cause: changing
declaration/assignment order never moved the slots to the original layout
without also losing the fuzzy score.

## Source-shape experiments (official verifier, 2764 B unless noted)

| variant | fuzzy |
|---|---|
| current (`int x`, `float z`, `float y`, jumbled wp names, try/trap) | **74.1** |
| current + jitter call order YZX | 74.2 |
| current + jitter call order YXZ / ZYX | 74.1 |
| current + jitter call order XZY / ZXY | 74.0 |
| faithful wp naming (`wx=wp->x` etc., decl `wz,wx,wy`) | 72.6–73.9 |
| faithful wp naming + all-int x,y,z | 61.4 |
| all-int x,y,z, jumbled names | 61.1 |
| all-float x,y,z | 64.4 |
| `int x`, `float y`, `int z` etc. (6 type orders) | 60.9–61.5 |
| plain `new PlayerFighter` / `new PlayerFixedObject` (type,race) | 63.5 (2772 B) |
| plain `new`, no try | 64.3 (2768 B) |
| `PlayerFixedObject(type, race, ...)` only | 74.0 |
| spawn before `new Player` (try / no try) | 62.7 (2776 B) / 61.1 (2800 B) |
| try around `new Player` instead of setEmpData | 61.5 (2792 B) |
| `int jitter[3]`, `int w[3]`, ternary wp loads | 73.9–74.1 |
| `const/static float zero` for BoundingAAB ex | 74.1 |
| `Vector pos` / explicit `float fx,fy,fz` before the branch | 43.7 |
| `wx - 20000 + jitterX` / `jitterX + wx - 20000` | 74.1 |
| 6 wp declaration × 6 else orders × 6 jitter orders | 74.0–74.2 |
| `int wx, wy, wz` decl with jitter decl ZYX, calls XYZ | 73.9 |

The only gain found is the jitter **call** order YZX (74.2). It is a
semantic permutation of the three independent `nextInt(40000)` results and
contradicts the wave-97 conclusion that the original calls X,Y,Z, so it was
not landed.

## Fixed-ship branch

The fixed branch is structurally identical to the original; the only
differences are register names and the loop-invariant `0.0f`:

* original: `ship=r9`, `volumes=fp`, `race=r6`, `type=r4`, `x=sl`, `z=r4`,
  `y=r8`, zero hoisted once into `r6` and stored with `strd r6,r0,[sp]`;
* ours: `ship=fp`, `volumes=r9`, `race=r4`, `type=r2` (reloaded from
  `[sp,#52]`), `x=r6`, `z=sl`, `y=r8`, zero rematerialised as `movs
  r0,#0; str r0,[sp]` before every `BoundingAAB`.

All of these are downstream of the spawn live-range difference. The
`PlayerFixedObject(race, type, ...)` argument order is still the semantic
bug from wave-88 (original passes `type, race`); fixing it alone scores
74.0.

## Remaining root cause

The single remaining root cause is the MachineSink decision: the original's
integer `x` is live across the `setEmpData` invoke (in `fp`) while ours is
recomputed in the post-invoke block. A source shape that makes the integer
`x` non-sinkable — or that gives the original's no-catch block structure
with the adds scheduled before a plain `setEmpData` call — should recover
the original allocation and the ~89% `NOHINT`-like result. The current
source is a local optimum at 74.1%.
