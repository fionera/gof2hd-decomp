# LevelScript::process — wave 187 / w13106: literal-pool / constant-island pass (continuation of w13006/w12906)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13106`, branch `wave187-w13106`, base master
`8b5d4281c` (wave-186 harvest 6: raw **51,968**, `.text` 74,274).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13106`.

**Result: no size-exact raw win landed; `src/` is at the base.**  The pool
*layout* was re-mapped on the wave-186 body (which is 107 B shorter than the
w13006 body).  A new structural finding is recorded below: the original's
mission-5 special is a **separate `if`**, not an `else if` of the
`m_bStartSequence` block (the deactivation loop's exit edge *falls through*
into the mission-5 check).  That is what makes the deactivation loop
condition non-water in the original and is the only lever that moves the
first constant island to its original address.  Reproducing it currently
costs 24 B of function size, so it cannot be landed alone.

## Baseline (verified on this worktree)

```
process raw diffbytes    51,968
process aligned insns    20,856
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,727
.text diffbytes          74,274
.rel.plt                 md5 289b2a069f8504d1e8a3919e43915e04 (byte-identical)
byte_exact               4507
linked_exact             4507
size-exact               4524/4524
data_exact               843/843
imports                  207
lint                     CLEAN
drift                    0
symbol parity            5370/5370
RATCHET                  PASS
```

## Method

* Compile `LevelScript.cpp` with the exact `compile_commands.json` command
  (already carries `-mllvm -disable-postra-machine-sink`) to `process.s` and
  inspect the `.LCPI4_*` island emission points directly.
* Disassemble both `.so`s with the NDK r18b `arm-linux-androideabi-objdump`
  and cluster every PC-relative literal load (`ldr`/`vldr` resolved target in
  the `; (ADDR <...>)` comment) with a 4-byte gap threshold; this yields the
  original's and our pool spans.  `scratch/w13106/poolcmp.py` compares the
  words at the same addresses (raw `.text` extracted with `objcopy -O binary
  --only-section=.text`; note `.text` vaddr is `0x69440`, not the `.ARM.exidx`
  vaddr `0x19cc08`).
* Every candidate below was measured with the real build + gold link + GNU
  strip + `reorder_shstrtab.py` recipe (`cmake --build ... --target link_so`).

## Pool layout on the wave-186 body

Reference-clustered pool spans: **95 on the original, 96 on ours**.  Only
four spans start at the same address: `0x138990`, `0x142320`, `0x14275c`,
`0x14588c`; of those only `0x142320` and `0x14275c` are genuine same-pool
alignments (the other two are different blocks' constants landing on the same
address).  Same-address word match over the whole function: **429/14,308**
words (1,716 B).

First pool — original `0x1380a4` (27 words), ours `0x1380c0` (25 words),
**+28 B**:

```
orig: d7fd2 d8400 37800000 40c90fdb c59c4000 3e4ccccd d83ec d83d2 [bf00bf00]
      9999999a 3fb99999 c63b8000 468ca000 c71c4000 c3fa0000 44fa0000 d832c
      c61c4000 43fa0000 c3960000 c4d48000 c3480000 d827e d827a d8280
      459c4000 46ea6000
ours: d7fd2 d8400 37800000 40c90fdb c59c4000 3e4ccccd d83e6 d83cc
      9999999a 3fb99999 c63b8000 468ca000 c71c4000 c3fa0000 44fa0000 d8326
      c61c4000 43fa0000 c3960000 c4d48000 c3480000 d827e d827a d8280
      461c4000
```

Words 0..6 are the same values in the same order.  The differences are all
consequences of placement:

* the `GOT_PREL` words (`d83ec`/`d83e6`, `d83d2`/`d83cc`, `d832c`/`d8326`,
  and the three `Level::programmedStation`/`doInstantJump`/
  `initStreamOutPosition` words) change with the address of the consuming
  `add rX, pc`;
* the 4-byte alignment pad: original `0x1380a4` is 4 mod 8, so the `double 0.1`
  needs `bf00bf00`; ours `0x1380c0` is 0 mod 8, so the pad disappears;
* the last word: original `46ea6000` (30000.0, mission-5 `scratch->z+30000`),
  ours `461c4000` (10000.0, mission-81 state-0 `10000.0f`).  The island
  boundary follows the block laid out after the loop.

### Why the first pool is +28 (new finding)

The deactivation loop (source lines 1506-1514, `field_0x12d = 0`) is laid out
*before* the island in ours and *after* it in the original:

```
orig: block A ends 0x1380a0 (b.n 0x138124) | nop 0x1380a2 | pool 0x1380a4..0x138110
      | body 0x138110..0x138124 | cond 0x138124 (bcc 0x138110) | mission-5 0x13812a
ours: block A ends 0x1380a2 (b.n 0x1380b8) | body 0x1380a4..0x1380be
      | cond 0x1380b8 (bcc 0x1380a4; b 0x138784) | pool 0x1380c0..0x138124
```

`ARMConstantIslands::initializeFunctionInfo` treats every block that does not
fall through as water and `findAvailableWater` picks the latest water in range
with the least padding.  In ours the loop-condition block ends in an
unconditional `b .LBB4_127` (the loop exit is the switch guard at line 1571,
far away), so it is water and wins over block A.  In the original the loop
condition ends in `bcc` and **falls through into the mission-5 check at
`0x13812a`**, so it is not water; the island is placed after block A.

The fall-through is real: the original's `m_bStartSequence != 0` true path
(`0x137f86..0x138128`) reaches `0x13812a` by loop-exit fall-through, and the
false path (`beq.w 0x13812a` at `0x137f82`) also reaches it.  So the source
structure is

```c
if (m_bStartSequence != 0) { ...deactivation... }
if (mission == 5) { ... } else if (mission == 26 ...) { ... } ...
```

not `else if`.  Our source currently joins the mission-5 check as an
`else if`, which forces the true path to branch to the switch guard and makes
the loop condition water.

### The `if` experiment (measured, not landed)

Changing only `else if (getCurrentCampaignMission() == 5)` to
`if (getCurrentCampaignMission() == 5)` moves the first island to
`0x1380a0` with the original's content (last word `46ea6000`, 30000.0) and
moves the loop body/condition/mission-5 block into the original's order.  It
is 4 B early (pool `0x1380a0` vs `0x1380a4`, no pad) because the function
shrinks:

| candidate | raw | aligned | size | .rel.plt |
|---|---:|---:|---:|---|
| base (`else if`) | 51,968 | 20,856 | 57232/57232 | 289b2a06… |
| `if` | 55,278 | 22,524 | **57208/57232 (-24)** | 9568f1e6… |
| `if` + `m_bStartSequenceOver=1` strh merge | 55,284 | 22,524 | **57208/57232 (-24)** | 9568f1e6… |

The 24 B split: the last laid-out block is different (mission-78 state-2 in
ours, mission-157 in the original), so the right-aligned last pool is 11
words (44 B) instead of 14 words (56 B) — 12 B; the code before it is a
further 12 B shorter.  Because the function is 24 B short, every later
function shifts and same-address raw explodes (`text` diff 359,296).  Best
global-shift match of the `if` body is 2,777 B (shift +32) vs 5,264 B for the
base at shift 0, so the `if` body is also structurally less similar overall
unless the missing code is recovered.  **Landing it requires a pass that
recovers the 12 B of missing code and the last-block order (mission-157 vs
mission-78 state-2) at the same time.**

## Aligned pools

* `0x142320` (5 words): 3 literals match; the two `GOT_PREL` words differ by
  4 and 2 because the consuming `add rX, pc` sites are 4/2 B apart.
* `0x14275c` (13 words): 11 literals match; the two `GOT_PREL` words differ by
  4 (`000ce12c`/`000ce128`, `000cd7b8`/`000cd7bc`), again a pure local
  schedule difference in same-size code.
* `0x138990` and `0x14588c` are different blocks' constants at the same
  address (e.g. original `[GOT, 7.0, -50000]` vs ours `[GOT, GOT, -10000]`).

## Constant multiset / movw-movt order

The movw/movt immediate multiset is identical (210 pairs each) except for one
false positive on each side: the original's `movt r0,#43020` at `0x140a2c`
and our `movw r1,#26636` at `0x140a2e` are **pool data** disassembled as
instructions, not real materialisations.  There is no movw/movt ordering
difference and no missing/wrong literal.

## `-mllvm -arm-constant-island-max-iteration` diagnostic

Compiled with the exact command plus
`-mllvm -arm-constant-island-max-iteration=N`:

| N | `.s` md5 |
|---:|---|
| 3 | `dfaf4687d4a5180688fd639de4c9d4b3` |
| 5 | `dfaf4687d4a5180688fd639de4c9d4b3` |
| 10 | `dfaf4687d4a5180688fd639de4c9d4b3` |
| 50 | `dfaf4687d4a5180688fd639de4c9d4b3` |
| 200 | `dfaf4687d4a5180688fd639de4c9d4b3` |

Identical to the base `process.s`.  The pass converges immediately; the flag
is a hard no-op and cannot move the first island.

## Source-order experiments (all measured with the real link recipe)

| candidate | raw | aligned | size | .rel.plt |
|---|---:|---:|---:|---|
| base | 51,968 | 20,856 | exact | 289b2a06… |
| mission==1: `angle`/`rotate` moved after the `depth`/`moveForward` block (reorders first uses of 1.5258789e-5/2*pi vs -5000/0.2) | 51,973 | 20,883 | exact | 289b2a06… |
| deactivation loop `isWingMan()` `__builtin_expect(...,1)` | 51,968 | 20,856 | exact (identical code) | 289b2a06… |
| deactivation `getWingmen()!=0` `__builtin_expect(...,1)` | 51,968 | 20,856 | exact (identical code) | 289b2a06… |
| deactivation loop `isWingMan()` `__builtin_expect(...,0)` | 55,026 | 20,852 | **57240 (+8)** | 289b2a06… |
| deactivation `getWingmen()!=0` `__builtin_expect(...,0)` | 54,610 | 20,413 | **57272 (+40)** | 289b2a06… |

The first-use reorder is a raw loss (+5) because the wave-186 pool order
already matches the original word-for-word; any reorder breaks it.

## Recommendation for the next pass

The only lever that can align the first pool (and the whole early pool chain)
is the deactivation-loop block layout.  The original's mission-5 special is a
separate `if`; that is proven by the loop-exit fall-through into
`0x13812a`.  A future pass should:

1. change the mission-5 check to a separate `if` (moves the island to
   `0x1380a0`, content correct);
2. recover the 12 B of code that the `if` form loses (compare the original's
   `0x138000..0x138128` deactivation block against ours; the original uses a
   `strh` merge for `m_bStartSequence`/`m_bStartSequenceOver` and a
   `ldr r0,[pc]`/`ldr r6,[sp]` pair where ours uses `ldrd`/`ldr.w`);
3. fix the last-block order so the right-aligned last pool becomes the
   mission-157 14-word pool (12 B).

With size exact, the first pool would start at `0x1380a4` with the original's
27 words and the region `0x1380a4..0x13812a` (pool + loop body + condition)
would match except for the relocation words — a ~100 B raw win.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13106
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51968 size=57232/57232 text=74274 relplt=289b2a069f8504d1e8a3919e43915e04
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51968 aligned=20856 size=57232/57232 insn=20727/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`poolcmp.py`, `structdiff.py`, `try.sh`, `process.s`,
`p_*.s`, `if_head.dis`, `if_tail.dis`, `orig_head.dis`) live in
`/home/debian/GalaxyOnFire2/scratch/w13106/`; they are scratch, not tracked.
