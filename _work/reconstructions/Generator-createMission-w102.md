# Generator::createMission — hoisting `costs`/`itemId`/`amount` to the top of the body
# (74.3 % → 76.6 %, 1996 B; wave 102, worker wf205)

Status: **source change landed** (wave 102, 2026-11-04, worktree `wf205`, branch
`wave102-wf205`, base `53d9ee4d5`).

Target: `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE`
(original @ `0x971b8`, size 1996, image base 0x10000).

Gate before/after (full `check`, re-measured on this worktree):

```
before: byte 4500  linked 4500  size 4524/4524  data 843/843  imports 207  lint CLEAN  drift 0
after : byte 4500  linked 4500  size 4524/4524  data 843/843  imports 207  lint CLEAN  drift 0
```

`.text` unchanged at `0x1337c8`; `.rodata` `0x41b67`; `.ARM.exidx` `0x6f70`.
`.ARM.extab` moved from `0x2628c` to `0x26290` (original `0x26298`), i.e. 4 bytes closer.

## The change

The wave-101 note (`generator-createmission-faithful-loop-slot-chain-w101.md`) showed the
faithful `int pick` loop is instruction-identical to the original but the whole residual is one
stack-slot chain rooted at the `amount`/`difficulty` schedule.  This pass found that the chain
is broken by **where the locals are declared**, not by the loop body:

```cpp
    uint32_t race = agent->getRace();
    int level;
    int costs;      // <-- hoisted
    int itemId;     // <-- hoisted
    int amount;     // <-- hoisted

    bool selected = false;
```

and assigning them at their original first-write sites (`costs = ...`, `itemId = 0; amount = 0;`)
instead of declaring them there.  With `costs`/`itemId`/`amount` declared at the top, clang
7.0.2 colours the frame `96` bytes and reproduces the original's early slot map:

```
original  16=galaxy/bonus  20=&__stack_chk_guard  24=agent/difficulty  28=targetStation  32=status/rem  36=race/type  40=rnd
r4        16=galaxy/bonus  20=&__stack_chk_guard  24=targetStation     28=agent            32=status/rem  36=race/type  40=rnd
```

i.e. six of the seven slots are now right (only `agent`/`targetStation` at 24/28 are swapped),
and `strd r6,r0,[sp,#32]` is reproduced.  Before this pass the function was frame 96 but with a
different slot order and a different early `strd`.

The `type == 8` arm keeps a real `int tec` temporary:

```cpp
        amount = Globals::rnd->nextInt(15) + 5;
        int tec = (*items)[itemId]->getTecLevel();
        difficulty = tec;
```

which is the +4 B that keeps the function at the original 1996 B (without it the body is 1992 B,
`size-exact` would drop to 4523/4524).

## Measured variants (official verifier, one `link_so` per row)

| variant | source change | pct | usize |
|---|---|---:|---:|
| HEAD | landed wave-101 shape | 74.3 | 1996 |
| `base_ip` | faithful loop, no hoisting | 70.9 | 1996 |
| `q1` | hoist `costs`/`itemId`/`amount` | 76.9 | 1992 |
| **`r4`** | `q1` + `int tec` temp | **76.6** | **1996** |
| `r7` | `r4` + real `0.5f` (drop the anchor) | 76.8 | 1992 |
| `w7` | `r4` + `items` hoisted too | 76.9 | 1992 |
| `d1` | `int type` instead of `int32_t` | 76.6 | 1996 |
| `v1` | `tec` before `amount` (call order flips) | 76.3 | 1988 |
| `x1`/`x2`/`x3` | swap `getRoutes()` if/else, `!ptr`, `== 0` | 76.8 | 1992 |
| `z1`/`z2`/`z3` | nested ifs, swapped `&&`, `goto` form | 76.8 | 1992 |

`r4` is the best **size-exact** shape found.  Everything that scores 76.8–76.9 is 1992 B and
would break `size-exact 4524/4524`.

## What is proven vs still hypothesis

Proven (measured, reproducible):

* Declaring `costs`/`itemId`/`amount` at the top of the body is what moves the frame to the
  original's slot map and reproduces `strd r6,r0,[sp,#32]`; it is worth +2.3 points on its own.
* `r4` is 1996 B with `nO = nU = 727`, and the full gate is unchanged.
* The remaining 4-byte gap between `r7` (real `0.5f`, 1992 B) and the original is **one
  instruction**: the original emits `cmp r0,#0; beq.w 97586` for the `getRoutes() == nullptr`
  arm (out-of-line block, wide branch), while ours emits an inline `cbz r0, ...`.  It is not the
  `pick == 12` ternary — that select already survives in `r4`
  (`movs r2,#12; cmp r0,#25; it ne; movne r2,r6`).
* The `0.5000001f` literal is still a +4-byte anchor (a `vldr` from the literal pool instead of
  `vmov.f32 s2,#96`); with a real `0.5f` the body is 1992 B.

Hypothesis (not proven):

* That making the `getRoutes() == nullptr` arm out-of-line (as the original does) would let the
  real `0.5f` land at 1996 B and score ~76.8 %.  Every source spelling tried
  (`__builtin_expect` both ways, `!ptr`, `== 0`, swapped if/else, nested ifs, `goto`, `int found`,
  do/while, `for(;;)`) leaves the arm inline; it is pure `MachineBlockPlacement`.
* That the `agent`/`targetStation` slot swap at 24/28 is a consequence of the same
  `MachineBlockPlacement` ordering (the original stores `agent` first into the *lower* slot even
  though `targetStation` is stored first in program order).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf205
cmake --build cmake-build-match --target check -j8      # RATCHET PASS, byte 4500
python3 tools/verify/verify.py --show _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE
# pct=76.6 osize=1996 usize=1996 nO=727 nU=727
```

Scratch harness (`/dev/shm/wf205`, not committed): `splice.py`, `measure.sh`, `scorefn.py`,
`nd4.py`, `findgap.py`, candidate bodies `r4.cpp` (landed), `q1.cpp`, `r7.cpp`, `w7.cpp`.
