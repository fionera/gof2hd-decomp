# LevelScript::process — wave 185 / w12906: literal-pool / constant-island pass (continuation of w12706)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w12906`, branch `wave185-w12906`, base master
`f390b0e68` (wave-184 harvest 10, raw **52,132**, `.text` 74,591).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12906`.

**Result: no size-exact raw win landed; `src/` is at the base.**  This pass
re-maps the literal-pool layout on the wave-184 body, proves the pool
*contents* are still faithful, and localises every remaining pool-byte
difference to the global code-layout/register-allocation delta.  The
`-mllvm -arm-constant-island-max-iteration` knob is a hard no-op on this
function (N=3..200 produce a byte-identical `.s`).

## Baseline (verified on this worktree)

```
process raw diffbytes    52,132
process aligned insns    20,658
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,725
.text diffbytes          74,591
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

* Compile the TU with the exact `compile_commands.json` command (which already
  carries `-mllvm -disable-postra-machine-sink`) to `process.s`; assemble a
  copy with the `.LCPI4_N` labels promoted to globals and read their offsets
  from the object symbol table.  That gives the **exact** address of every one
  of the 707 constant-pool words in the linked function (the assembler merges
  the per-constant `.p2align 2` islands into contiguous runs).
* Disassemble both `.so`s with the NDK r18b
  `arm-linux-androideabi-objdump` and compare the function bodies at the same
  address (`tools/fn_diff.py`, `_work/reconstructions/measure_w11901.py`).
* One build (compile + gold link + GNU strip + `reorder_shstrtab.py`) is
  ~1.5 s, so every candidate below was measured with the real link recipe.

## Pool layout on the wave-184 body

Our side has **59 exact constant runs (707 words = 2,828 B)**.  The first run
of each region and the delta to the original's nearest pool start:

| our run | orig | delta | | our run | orig | delta |
|---:|---:|---:|---|---:|---:|---:|
| 0x003a8 | 0x0038c | **+28** | | 0x0a608 | 0x0a608 | **0** |
| 0x007fc | 0x00824 | -40 | | 0x0aa44 | 0x0aa44 | **0** |
| 0x00c68 | 0x00c78 | -16 | | 0x0ad5c | 0x0ad50 | **+12** |
| 0x0102c | 0x010e4 | -184 | | 0x0b12c | 0x0b0f4 | +56 |
| 0x013f0 | 0x01604 | -532 | | 0x0b334 | 0x0b3e8 | -180 |
| 0x017f8 | 0x019f0 | -504 | | 0x0b6f0 | 0x0b808 | -280 |
| 0x01cd8 | 0x01e24 | -332 | | 0x0baf4 | 0x0bc30 | -316 |
| 0x021e8 | 0x02244 | -92 | | 0x0bed8 | 0x0c020 | -328 |
| 0x02588 | 0x025b4 | -44 | | 0x0c210 | 0x0c364 | -340 |
| 0x02940 | 0x0290c | +52 | | 0x0c5dc | 0x0c74c | -368 |
| 0x02c90 | 0x02de0 | -336 | | 0x0c958 | 0x0cb98 | -576 |
| 0x02fec | 0x031ec | -512 | | 0x0cd9c | 0x0cef8 | -348 |
| 0x033d0 | 0x034b8 | -232 | | 0x0cf78 | 0x0d300 | -904 |
| 0x0381c | 0x03838 | -28 | | 0x0d338 | 0x0d74c | -1044 |
| 0x03de4 | 0x04084 | -672 | | 0x0d750 | 0x0db74 | -1060 |
| 0x04224 | 0x04458 | -564 | | 0x0db74 | 0x0df58 | -996 |
| 0x04634 | 0x0482c | -504 | | 0x0df64 | 0x0df58 | **+12** |
| 0x04acc | 0x04bac | -224 | | | | |
| 0x04f64 | 0x04fb8 | -84 | | | | |
| 0x05424 | 0x05560 | -316 | | | | |
| 0x058d4 | 0x059c8 | -244 | | | | |
| 0x05c88 | 0x05e8c | -516 | | | | |
| 0x06124 | 0x06294 | -368 | | | | |
| 0x06570 | 0x066bc | -332 | | | | |
| 0x06a18 | 0x06b18 | -256 | | | | |
| 0x06f40 | 0x07074 | -308 | | | | |
| 0x07340 | 0x074a4 | -356 | | | | |
| 0x07700 | 0x078d4 | -468 | | | | |
| 0x07b5c | 0x07d68 | -524 | | | | |
| 0x07ea8 | 0x081cc | -804 | | | | |
| 0x08294 | 0x08520 | -652 | | | | |
| 0x08738 | 0x088dc | -420 | | | | |
| 0x08c1c | 0x08d04 | -232 | | | | |
| 0x08f4c | 0x09188 | -572 | | | | |
| 0x09320 | 0x09554 | -564 | | | | |
| 0x09738 | 0x09974 | -572 | | | | |
| 0x09b64 | 0x09dc4 | -608 | | | | |
| 0x09f40 | 0x0a2cc | -908 | | | | |

The pool chain is still globally shifted by the code-size delta in front of
each island; only `0xa608`, `0xaa44` (and the last pool, right-aligned at the
function end) start at an original pool address.

## First pool: contents identical, placement +28 B

Original pool `0x38c` (27 words) vs ours `0x3a8` (25 words):

```
orig: d7fd2 d8400 37800000 40c90fdb c59c4000 3e4ccccd d83ec d83d2 [bf00bf00]
      9999999a 3fb99999 c63b8000 468ca000 c71c4000 c3fa0000 44fa0000 d832c
      c61c4000 43fa0000 c3960000 c4d48000 c3480000 d827e d827a d8280
      459c4000 46ea6000
ours: d7fd2 d8400 37800000 40c90fdb c59c4000 3e4ccccd d83e6 d83cc
      9999999a 3fb99999 c63b8000 468ca000 c71c4000 c3fa0000 44fa0000 d8326
      c61c4000 43fa0000 c3960000 c4d48000 c3480000 d827e d827a d8280
      1176256512
```

Words 0..21 are the same values in the same order (`0x37800000`=1.5258789e-5,
`0x40c90fdb`=2*pi, `0xc59c4000`=-5000, `0x3e4ccccd`=0.2, the double 0.1,
`0xc63b8000`=-3000, `0x468ca000`=18000, `0xc71c4000`=-10000, `0xc3fa0000`=-500,
`0x44fa0000`=2000, `0xc61c4000`=-10000, `0x43fa0000`=500, `0xc3960000`=-300,
`0xc4d48000`=-1700, `0xc3480000`=-200).  The differences are:

* the `GOT_PREL` words (`d83ec`/`d83e6`, `d83d2`/`d83cc`, `d832c`/`d8326`, and
  the three `Level::programmedStation`/`doInstantJump`/`initStreamOutPosition`
  words), whose values are `symbol - (add_pc + 4 - pool_addr)` and therefore
  change with the address of the consuming `add rX, pc`;
* the 4-byte alignment pad: the original pool starts at `0x38c` (4 mod 8) so
  the `double 0.1` needs `bf00bf00`; ours starts at `0x3a8` (0 mod 8) so the
  pad disappears;
* the last word: original `46ea6000` (30000.0, mission-5 `scratch->z+30000`),
  ours `461c4000` (10000.0, mission-81 `m_nScriptTimerA/10000`).  Both are
  block-A constants; the island pass picked a different one to keep in the
  first pool.

The +28 B is **not** a constant-ordering problem.  It is the placement of the
`st->getWingmen()` loop (source lines 1506-1514, `field_0x12d = 0`) relative
to the pool:

```
orig:  block A ends 0x38a (b.n 0x40c) | pool 0x38c..0x3f8 | loop 0x3f8..0x12a
ours:  block A ends 0x38a (b.n 0x3a0) | loop 0x38c..0x3a6 | pool 0x3a8..0x40c
```

The loop body is byte-identical (`ldr [r4,#4]; ldr.w [r0,r6,lsl#2]; mov r0,r5;
blx isWingMan; cbz; strb.w r8,[r5,#301]; adds; ldr [r4]; cmp; bcc`), only its
position differs.  In the original the pool is inserted at the end of block
A's water; in ours the loop is laid out first and the pool after it.  If the
loop were placed after the pool, the first pool would move to `0x38c` and its
first 22 words would match (the pad reappears and only the `GOT_PREL` words
would remain different) — a potential ~80-byte raw win.

## Aligned regions

* `0xa608..0xa620` (6-word pool): words `40c28f5c`, `3e4ccccd`, `3ecccccd`,
  `3dcccccd` match; the two `GOT_PREL` words differ by 4 and 2 because the
  consuming loads are at `0x142084`/`0x142080` and `0x14215c`/`0x14215a`.
* `0xaa44..0xaa8c` (18-word pool): all 15 literal words match; the three
  `GOT_PREL` words differ by 4/14/12 because the `add rX, pc` sites are at
  `0x142408`/`0x14240c`, `0x142624`/`0x142632`, `0x142658`/`0x142664`.
* `0xad50` original vs `0xad5c` ours (+12): the 11-word pool content is
  identical except `GOT_PREL` offsets (`000cda2a`/`000cda26`,
  `000cd798`/`000cd790`), i.e. a pure shift.  The code in `0xaa90..0xad50` is
  12 B longer in ours; the main contributors are a 4-instruction reorder
  around the inlined `m_pCamera->setFixed(true)` / `MatrixSetTranslation`
  pair (source lines 3024-3029) and an 8-byte `m_tempVec`/`awake` store pair
  at `0xad14` (source line 3960).
* Last pool: right-aligned at the function end.  Original `0xdf58` (14 words:
  100, -3000, 0.8, 3000, 6500, 25, 1.7 + 7 `GOT_PREL`), ours `0xdf64`
  (11 words: 8000, 500, 1000, 10000, 2400, 0.006, 4800 + 4 `GOT_PREL`).
  The last laid-out block is the mission-78 state-2 block (source lines
  1382-1385) in ours but the mission-157 block (source lines 5175-5202) in the
  original, so the pool content differs by block placement, not by a missing
  or wrong literal.

## Constant multiset is still faithful

All of the original's last-pool literals exist in our `.s`:
`42c80000` x6, `c63b8000` x3, `3f4ccccd` x2, `453b8000` x9, `45cb2000` x1,
`41c80000` x1, `3fd9999a` x3.  The `movw` immediate multiset is identical
except for one `movw r0,#32780` at `0x14105c`, which is **pool data**
(`f248000c`) disassembled as an instruction, not a real materialisation.  The
original's single `movt r0,#43020` at `0x140a2c` is likewise pool data.  There
is no movw/movt ordering difference.

## `-mllvm -arm-constant-island-max-iteration` diagnostic

Compiled with the exact command plus `-mllvm -arm-constant-island-max-iteration=N`:

| N | `.s` vs N=3 |
|---:|---|
| 3 | — |
| 5 | identical |
| 10 | identical |
| 50 | identical |
| 200 | identical |

The pass converges immediately; the flag cannot move the first pool.

## Source-order experiments (all measured with the real link recipe)

| candidate | raw | size |
|---|---:|---:|
| base | 52,132 | 57232/57232 |
| mission==1: `forceUpdate` first, `getPosition` before `setToSleep` (original order) | 52,136 | exact |
| mission==1: `forceUpdate` first only | 52,135 | exact |
| prologue: `Status *&status` reference | 52,133 | exact |
| prologue: `Status *const status` | 52,132 | exact |
| prologue: `Level *level = m_pLevel` + use `level->` | 55,118 | **57240 (+8)** |
| prologue: `status` declared first | 55,141 | **57312 (+80)** |
| prologue: `status` after `messages` | 55,142 | **57312 (+80)** |
| prologue: `status` after `player` | 55,207 | **57260 (+28)** |
| loop2: `while` form / `continue` form | 52,132 | exact (identical code) |
| loop2 inner `if` `__builtin_expect(...,1)` | 52,132 | exact (identical code) |
| loop2 inner `if` `__builtin_expect(...,0)` | 55,014 | **57240 (+8)** |
| loop2 guard `__builtin_expect(...,1)` | 52,132 | exact (identical code) |
| loop2 guard `__builtin_expect(...,0)` | 55,079 | **57280 (+48)** |
| deactivation `if` `__builtin_expect(...,1)` | 52,276 | exact |
| deactivation last term `__builtin_expect(...,1)` | 52,132 | exact (identical code) |
| deactivation last term `__builtin_expect(...,0)` | 54,969 | **57292 (+60)** |
| move loop2 before `programmedStation` block | 55,676 | **57548 (+316)** |
| swap `programmedStation` group after loop2 | 55,676 | **57548 (+316)** |
| move `initStreamOutPosition` before `programmedStation` | 52,180 | exact |
| reorder `initStreamOutPosition`/`doInstantJump`/`programmedStation` | 52,183 | exact |
| swap `setFixed`/`MatrixSetTranslation` source order (line 3024/3029) | 52,175 | exact |
| mission-78 state-2 `expect(...,1)` | 54,792 | **57272 (+40)** |
| mission-78 state-2 plain `if` | 54,399 | **57240 (+8)** |

The loop2/`programmedStation` moves change the function size by +316 B and also
perturb `.rel.plt`, so they are gate-fatal even before raw is considered.

## Recommendation for the next pass

The only lever that can align the first pool (and therefore the whole early
pool chain) is the machine-block layout of the `st->getWingmen()` loop around
`0x38c`.  It must be laid out *after* the first constant island, as in the
original.  The loop is byte-identical and the source is already faithful; the
remaining difference is clang's block-placement/water decision, which is
driven by the same register-allocation delta that makes `this`=`fp` (ours) vs
`this`=`sl` (original).  A source construct that flips that callee-saved
assignment without changing the function size is the unexplored angle.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12906
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=52132 size=57232/57232 text=74591 relplt=289b2a069f8504d1e8a3919e43915e04
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52132 aligned=20658 size=57232/57232 insn=20725/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`pools.py`, `process_labels.s`, `try.sh`, `variants.py`,
`process.s`, `orig.dis`, `ours.dis`) live in
`/home/debian/GalaxyOnFire2/scratch/w12906/`; they are scratch, not tracked.
