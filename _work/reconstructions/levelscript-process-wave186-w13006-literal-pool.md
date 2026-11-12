# LevelScript::process — wave 186 / w13006: literal-pool / constant-island pass (continuation of w12906/w12706)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13006`, branch `wave186-w13006`, base master
`2d02567f6` (wave-185 harvest, raw **52,075**, `.text` 74,402).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13006`.

**Result: no size-exact raw win landed; `src/` is at the base.**  This pass
re-maps the literal-pool layout on the wave-185 body (which is 57 B shorter
than the wave-184 body w12906 analysed), re-proves that the pool *contents*
are faithful, and re-confirms that every remaining pool-byte difference is a
consequence of the global code-layout / register-allocation delta, not of
constant ordering.  `-mllvm -arm-constant-island-max-iteration` is still a
hard no-op (N=3..200 produce a byte-identical `.s`).

## Baseline (verified on this worktree)

```
process raw diffbytes    52,075
process aligned insns    20,766
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726
.text diffbytes          74,402
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
  (already carries `-mllvm -disable-postra-machine-sink`) to `process.s`.
  Promote the `LevelScript::process` `.LCPI4_N` labels to globals (rename
  `.LCPI4_` → `XCPI4_`, prepend `.globl`), assemble with the NDK clang
  integrated assembler, and read every label's exact section offset from
  `arm-linux-androideabi-nm -n`.  This gives the exact address and value of
  every constant-pool word on our side.
* Disassemble both `.so`s with the NDK r18b `arm-linux-androideabi-objdump`
  and cluster every PC-relative literal load (`ldr`/`vldr` resolved target in
  the `; (ADDR <...>)` comment) with a 4-byte gap threshold; this yields the
  original's pool spans.  `scratch/w13006/poolcmp.py` does the same-address
  word comparison.
* One build (compile + gold link + GNU strip + `reorder_shstrtab.py`) is
  ~1.5 s, so every candidate below was measured with the real link recipe.

## Pool layout on the wave-185 body

Reference-clustered pool spans: **95 on the original, 96 on ours**.  The
first span of each region and the delta to the original's nearest pool start
(ours is matched to the nearest original start; deltas are not additive):

| # | orig | ours | delta | | # | orig | ours | delta |
|---:|---:|---:|---:|---|---:|---:|---:|---:|
| 0 | 0x038c | 0x03a8 | **+28** | | 48 | 0x59c8 | 0x58d4 | -244 |
| 1 | 0x03a4 | 0x03a8 | +4 | | 49 | 0x5e8c | 0x608c | +512 |
| 2 | 0x03b8 | 0x03c0 | +8 | | 50 | 0x6294 | 0x6124 | -368 |
| 3 | 0x0824 | 0x0810 | -20 | | 51 | 0x66bc | 0x656c | -336 |
| 4 | 0x0c78 | 0x0c78 | **0** | | 52 | 0x6b18 | 0x6a10 | -264 |
| 5 | 0x0c88 | 0x0c90 | +8 | | 53 | 0x6f30 | 0x6f40 | +16 |
| 6 | 0x0ca0 | 0x0c90 | -16 | | 54 | 0x7074 | 0x6f40 | -308 |
| 7 | 0x10e4 | 0x105c | -136 | | 55 | 0x74a4 | 0x7364 | -320 |
| 8 | 0x1114 | 0x105c | -184 | | 56 | 0x78d4 | 0x7718 | -444 |
| 9 | 0x1604 | 0x1410 | -500 | | 57 | 0x7d68 | 0x7ea8 | +320 |
| 10 | 0x1624 | 0x17f8 | +468 | | 58 | 0x7d7c | 0x7ea8 | +300 |
| 11 | 0x19f0 | 0x17f8 | -504 | | 59 | 0x81cc | 0x8294 | +200 |
| 12 | 0x1a04 | 0x17f8 | -524 | | 60 | 0x81e4 | 0x8294 | +176 |
| 13 | 0x1e24 | 0x1d0c | -280 | | 61 | 0x8520 | 0x8738 | +536 |
| 14 | 0x1e34 | 0x1d0c | -296 | | 62 | 0x88dc | 0x8750 | -396 |
| 15 | 0x2244 | 0x21e8 | -92 | | 63 | 0x8d04 | 0x8c5c | -168 |
| 16 | 0x2258 | 0x21e8 | -112 | | 64 | 0x8d20 | 0x8c5c | -196 |
| 17 | 0x25b4 | 0x2588 | -44 | | 65 | 0x9188 | 0x9320 | +408 |
| 18 | 0x290c | 0x2940 | +52 | | 66 | 0x9554 | 0x9738 | +484 |
| 19 | 0x2de0 | 0x2cac | -308 | | 67 | 0x9564 | 0x9738 | +468 |
| 20 | 0x31ec | 0x33d0 | +484 | | 68 | 0x9974 | 0x9b64 | +496 |
| 21 | 0x34b8 | 0x33d0 | -232 | | 69 | 0x9dc4 | 0x9f40 | +380 |
| 22 | 0x3514 | 0x33d0 | -324 | | 70 | 0x9de4 | 0x9f40 | +348 |
| 23 | 0x3524 | 0x33d0 | -340 | | 71 | 0xa2cc | 0xa2f8 | +44 |
| 24 | 0x3608 | 0x3684 | +124 | | 72 | 0xa2dc | 0xa2f8 | +28 |
| 25 | 0x3838 | 0x381c | -28 | | 73 | 0xa480 | 0xa4a8 | +40 |
| 26 | 0x3878 | 0x3858 | -32 | | 74 | 0xa608 | 0xa608 | **0** |
| 27 | 0x389c | 0x3858 | -68 | | 75 | 0xaa44 | 0xaa44 | **0** |
| 28 | 0x38c8 | 0x3858 | -112 | | 76 | 0xad50 | 0xad5c | +12 |
| 29 | 0x3900 | 0x3960 | +96 | | 77 | 0xad6c | 0xad78 | +12 |
| 30 | 0x3a08 | 0x3a18 | +16 | | 78 | 0xb0f4 | 0xb12c | +56 |
| 31 | 0x3b3c | 0x3ba0 | +100 | | 79 | 0xb3e8 | 0xb334 | -180 |
| 32 | 0x3c10 | 0x3ba0 | -112 | | 80 | 0xb808 | 0xb70c | -252 |
| 33 | 0x3c40 | 0x3ba0 | -160 | | 81 | 0xb820 | 0xb70c | -276 |
| 34 | 0x3dec | 0x3de4 | -8 | | 82 | 0xbc30 | 0xbaf4 | -316 |
| 35 | 0x4084 | 0x410c | +136 | | 83 | 0xc020 | 0xc0c0 | +160 |
| 36 | 0x40a8 | 0x410c | +100 | | 84 | 0xc200 | 0xc210 | +16 |
| 37 | 0x40e0 | 0x410c | +44 | | 85 | 0xc364 | 0xc210 | -340 |
| 38 | 0x4458 | 0x4634 | +476 | | 86 | 0xc74c | 0xc5ec | -352 |
| 39 | 0x482c | 0x4a14 | +488 | | 87 | 0xc75c | 0xc5ec | -368 |
| 40 | 0x4bac | 0x4acc | -224 | | 88 | 0xcb98 | 0xcd9c | +516 |
| 41 | 0x4c0c | 0x4acc | -320 | | 89 | 0xcef8 | 0xcf78 | +128 |
| 42 | 0x4fb8 | 0x4f84 | -52 | | 90 | 0xd300 | 0xd338 | +56 |
| 43 | 0x4fd4 | 0x4f84 | -80 | | 91 | 0xd3f0 | 0xd338 | -184 |
| 44 | 0x5560 | 0x5468 | -248 | | 92 | 0xd74c | 0xd750 | +4 |
| 45 | 0x5578 | 0x5468 | -272 | | 93 | 0xdb74 | 0xdb74 | **0** |
| 46 | 0x558c | 0x5468 | -292 | | 94 | 0xdf58 | 0xdf64 | +12 |
| 47 | 0x55a0 | 0x5468 | -312 | | | | | |

Only four pool spans start at the same offset (`0xc78`, `0xa608`, `0xaa44`,
`0xdb74`), and of those only `0xa608`/`0xaa44` are genuine same-pool
alignments; `0xc78` and `0xdb74` are coincidences of different pools landing
on the same address.

### Diff split

Same-address byte diff over the function body: **2,888 bytes inside the
original's pool spans, 49,187 bytes in code**.  The diff is spread evenly
across the whole 57 KB (3.0-3.9 K per 4 KB bucket); there is no single
localised pool cluster left.

## First pool: contents identical, placement +28 B

Original pool starts at `0x1380a4` (offset `0x38c`), ours at `0x1380c0`
(offset `0x3a8`).  Word-for-word:

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

Words 0..21 are the same values in the same order.  The differences are:

* the `GOT_PREL` words, whose values are `symbol - (add_pc + 4 - pool_addr)`
  and therefore change with the address of the consuming `add rX, pc`;
* the 4-byte alignment pad: the original pool starts at `0x38c` (4 mod 8) so
  the `double 0.1` needs `bf00bf00`; ours starts at `0x3a8` (0 mod 8) so the
  pad disappears;
* the last word: original `46ea6000` (30000.0, mission-5
  `scratch->z + 30000`), ours `461c4000` (10000.0, mission-81 state-0
  `10000.0f`).  Both are covered by the same island; the island boundary is
  different because the code between the last user and the mission-5 user is
  different.

The +28 B is **not** a constant-ordering problem.  It is the placement of the
deactivation `st->getWingmen()` loop (source lines 1505-1514, `field_0x12d = 0`)
relative to the pool:

```
orig: deactivation preheader ends 0x38a (b.n 0x40c) | pool 0x38c..0x3f8 | loop 0x3f8..0x12a
ours: deactivation preheader ends 0x38a (b.n 0x3a0) | loop 0x38c..0x3a6 | pool 0x3a8..0x40c
```

The loop body is byte-identical (`ldr [r4,#4]; ldr.w [r0,r6,lsl#2]; mov r0,r5;
blx isWingMan; cbz; strb.w r8,[r5,#301]; adds; ldr [r4]; cmp; bcc`), only its
position differs.  In the original the pool is inserted at the water after the
loop preheader; in ours the constant-island pass inserts it at the water after
the loop-exit branch.  If the loop were laid out after the pool, the first pool
would move to `0x38c` and its first 22 words would match (the pad reappears and
only the `GOT_PREL` words would remain different) — a potential ~80-byte raw
win.

## Aligned / near-aligned pools

* `0xa608..0xa61c` (5-word pool): 3 words match; the two `GOT_PREL` words
  differ by 4 and 2 because the consuming `add rX, pc` sites are at
  `0x14208a`/`0x14208e` and `0x14215a`/`0x14215c`.  The code before them is
  the same size; only the local instruction schedule differs by one 4-byte
  instruction.
* `0xaa44..0xaa78` (13-word pool): 11 words match; the two `GOT_PREL` words
  differ by 4 because the `add rX, pc` sites are 4 bytes apart.  Again a pure
  local-schedule difference in otherwise same-size code.
* `0xc78`: the original's pool there is `[code-ptr GOT, 7.0, -50000, 500,
  code-ptr GOT, -10000, code-ptr GOT, -34000, ...]`; ours has the identical
  sequence starting 16 bytes earlier at `0xc68`.  The `0xc78` "match" is our pool's 5th word
  landing on the original's 1st word.  The `GOT_PREL` values differ by 0x22
  because the whole block is 34 bytes earlier.
* `0xdb74`: original 9-word pool `[GOT, 10.0, 3000, 100, 0.33, -1, ...]`
  vs ours 17-word pool `[100, 25, -12000, 3000, 0.8, 1.7, ...]` — the last
  laid-out block differs (mission-78 state-2 in ours, mission-157 in the
  original), so the pool contents are different blocks' constants.

## Constant multiset is still faithful

Every literal in the original's first pool exists in our `.s` in the same
relative order (`d7fd2`, `d8400`, `37800000`, `40c90fdb`, `c59c4000`,
`3e4ccccd`, the two `Globals::layout` GOT words, the double 0.1, `c63b8000`,
`468ca000`, `c71c4000`, `c3fa0000`, `44fa0000`, the `Globals::Canvas` GOT,
`c61c4000`, `43fa0000`, `c3960000`, `c4d48000`, `c3480000`, the three
`Level::programmedStation`/`doInstantJump`/`initStreamOutPosition` GOT words).
The `movw`/`movt` immediate multisets agree; there is no movw/movt ordering
difference.  There is no missing or wrong literal to fix by source reordering.

## `-mllvm -arm-constant-island-max-iteration` diagnostic

Compiled with the exact command plus
`-mllvm -arm-constant-island-max-iteration=N`:

| N | `.s` md5 |
|---:|---|
| 3 | `9858e7e846f1799dc934c947d30069fb` |
| 5 | `9858e7e846f1799dc934c947d30069fb` |
| 10 | `9858e7e846f1799dc934c947d30069fb` |
| 50 | `9858e7e846f1799dc934c947d30069fb` |
| 200 | `9858e7e846f1799dc934c947d30069fb` |

Identical to the base `process.s`.  The pass converges immediately; the flag
cannot move the first pool and is not a candidate for landing.

## Source-order experiments (all measured with the real link recipe)

| candidate | raw | size | rel.plt |
|---|---:|---:|---|
| base | 52,075 | 57232/57232 | 289b2a06… |
| `Status *&status` (reference local) | 52,076 | exact | 289b2a06… |
| `Status *const status` | 52,075 | exact | 289b2a06… |
| `Status &status = *Globals::status` + `status.` uses | 52,075 | exact | 289b2a06… |
| `status` declared first | 55,270 | **57312 (+80)** | 289b2a06… |
| `Level *level = m_pLevel` + `level->` for the 3 getters | 55,146 | **57240 (+8)** | 289b2a06… |
| mission==1: `forceUpdate` first, `getPosition` before `setToSleep` (original order) | 52,079 | exact | 289b2a06… |
| mission==1: `forceUpdate` first only | 52,078 | exact | 289b2a06… |
| mission==1: `getPosition` before `setToSleep` only | 52,076 | exact | 289b2a06… |
| state-0: `m_nState = 1` before both `setPosition` calls | 52,079 | exact | 289b2a06… |
| state-0: `m_nState = 1` between the two `setPosition` calls | 52,076 | exact | 289b2a06… |
| state-0: camera `setPosition` before player `setPosition` | 52,091 | exact | 289b2a06… |
| state-0: `m_nState = 1` after the `MatrixSetTranslation` block | 55,108 | **57240 (+8)** | 289b2a06… |
| deactivation: `m_bStartSequenceOver = 1` next to `m_bStartSequence = 0` (merges to `strh`) | 52,111 | exact | 289b2a06… |
| deactivation: both flags after `setVulnerable` (merges to `strh`) | 52,117 | exact | 289b2a06… |
| deactivation: drop `Status *st` local, use `Globals::status->` | 52,142 | exact | 289b2a06… |
| deactivation: `field_0x24 > 7000` for `>= 7001` | 52,075 | exact | 289b2a06… |
| deactivation: `getPlayer()` for `getPlayer() != 0` | 52,075 | exact | 289b2a06… |
| wingmen loop: `int` loop variable | 52,075 | exact | 289b2a06… |
| wingmen loop: `while` form | 52,075 | exact | 289b2a06… |
| wingmen loop: increment in body | 52,075 | exact | 289b2a06… |
| wingmen loop: `do`/`while` with count guard | 55,437 | **57248 (+16)** | 939a641c… |
| wingmen loop: cache `n = wingmen->count` | 55,343 | **57272 (+40)** | 289b2a06… |
| wingmen loop: hoist `getEnemies()` before the `if` | 55,213 | **57200 (-32)** | 289b2a06… |
| wingmen loop: move loop after `field_0x24 += delta` | 55,169 | **57284 (+52)** | 289b2a06… |
| wingmen loop: move loop before `forceUpdate(30,false)` | 52,634 | exact | 289b2a06… |
| wingmen loop: `__builtin_expect(getWingmen()!=0,0)` | 55,142 | **57240 (+8)** | 289b2a06… |
| wingmen loop: `__builtin_expect(getWingmen()!=0,1)` | 52,075 | exact | 289b2a06… |
| mission-0 state machine: swap state-4 / state-5 arms | 55,072 | **57236 (+4)** | e185e74a… |
| mission-0 state machine: move state-3 arm before state-4 | 55,157 | **57248 (+16)** | a8d49cee… |
| prologue: `messages/enemies/player` declaration order | 52,087 | exact | 289b2a06… |
| prologue: `player/messages/enemies` declaration order | 52,087 | exact | 289b2a06… |
| prologue: `enemies/messages/player` declaration order | 52,089 | exact | 289b2a06… |

The only size-exact, `.rel.plt`-identical candidates are all raw *losses*
(+1 to +67).  The best is `Status *const status` / the reference form, which is
byte-identical to the base.

## Why the pools cannot be aligned by constant reordering

For a pool to contribute a raw win, its words must match the original's words
*at the same address*.  Because the code before each pool differs in size, the
pool starts differ (table above).  For the first pool the original's word at
our pool start (`0x3a8`) is its index 7 (`d83d2`, a relocation-dependent GOT
offset); to match it our pool would have to start with that word, i.e. the
first seven constants (`__stack_chk_guard` GOT, `Globals::status` GOT,
1.5258789e-5, 2*pi, -5000, 0.2, two `Globals::layout` GOTs) would have to be
first-used *after* the double 0.1.  Those are the very first constants used in
the prologue, so the only way to achieve it is to make the code before the
pool 28 B shorter — i.e. reproduce the original's prologue register assignment
and the deactivation-loop block placement, not reorder constants.

## Recommendation for the next pass

The only lever that can align the first pool (and therefore the whole early
pool chain) is the machine-block layout of the deactivation `st->getWingmen()`
loop around `0x38c`.  It must be laid out *after* the first constant island, as
in the original.  The loop is byte-identical and the source is already
faithful; the remaining difference is clang's block-placement / water
decision.  The prologue register rotation (`this`=`sl`, canary=`fp` in the
original vs `this`=`fp`, canary=`sl` in ours) is the other half of the same
global RA delta.  A source construct that flips the callee-saved assignment
without changing the function size is the unexplored angle; every loop-form
and declaration-order variant tried here leaves the pool at `0x3a8`.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13006
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52075 size=57232/57232 text=74402 relplt=289b2a069f8504d1e8a3919e43915e04
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52075 aligned=20766 size=57232/57232 insn=20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`cmd.txt`, `process.s`, `process_g.s`, `poolcmp.py`,
`pooldiff.py`, `try.sh`, `process_N*.s`) live in
`/home/debian/GalaxyOnFire2/scratch/w13006/`; they are scratch, not tracked.
