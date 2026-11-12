# LevelScript::process — wave 188 / w13206: literal-pool / constant-island pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13206`, branch `wave188-w13206`, base master
`ce16f45f1` (wave-187 harvest 5: raw **51,930**, `.text` 74,223,
`.ARM.extab` content 639).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13206`.

Continuation of the w12906/w13006/w13106 literal-pool passes.  The pool layout
was re-mapped on the current 51,930 body, `movw`/`movt` order and the
`-mllvm -arm-constant-island-max-iteration` flag were re-checked, and a fresh
constant-bearing adjacent-statement sweep was run.  **Eight independent
statement swaps compose to a size-exact, `.rel.plt`-identical raw win:
51,930 -> 51,915 (-15).**

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,930           51,915     (-15)
process aligned insns    20,928           20,938     (+10)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,223           74,208     (-15)
.ARM.extab diffbytes     639              639        (size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
.got / .rel.dyn          0 / 0            0 / 0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.note.gnu.build-id       20               20
file size                2,187,328        2,187,328  (exact)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included) from `tools/fn_diff.py`; `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after.

## Landed swaps (1-based base line numbers; swap moves the first statement below the second)

| # | line | moved statement | across | note |
|---:|---:|---|---|---|
| 1 | 1240 | `m_pLevel->lodManager->forceUpdate(30, false);` | `m_nState = 12;` | independent; original emits `forceUpdate` then `str m_nState` |
| 2 | 2674 | `frame->setPosition(m_tempVec);` | `canvas->CameraSetLocal(...frame->getMatrix());` | original order is `setPosition` (0x14160c) then `CameraSetLocal` (0x141626) |
| 3 | 2954 | `(*enemies)[0]->setPosition(ref + …);` | `(*enemies)[1]->setPosition((*enemies)[3]->getPosition() + …);` | disjoint receivers; `enemies[3]` not written |
| 4 | 4631 | `tvA += Vector{-350.0f, 400.0f, -1500.0f};` | `m_pCamera->setTarget(player->geometry);` | disjoint values |
| 5 | 4633 | `m_pCamera->setFixed(false);` | `m_pLevel->lodManager->forceUpdate(delta, false);` | disjoint objects |
| 6 | 4677 | `player->setSpeed(0.02f);` | `m_nState++;` | disjoint |
| 7 | 4679 | `camera->setPosition(tv);` | `m_pLevel->lodManager->forceUpdate(delta, false);` | disjoint objects |
| 8 | 5303 | `(*enemies)[i]->player->turnEnemy();` | `(*enemies)[i]->setPosition((*enemies)[0]->getPosition());` | disjoint state vs position |

Each swap is a pure reorder of semantically independent statements; the
emitted stream changes only in scheduling/register pressure.  The combination
is worth **-15 raw bytes**, size stays exactly 57,232 and `.rel.plt` stays
byte-identical.

## Literal-pool layout on the current 51,930 body

Reference-clustered pool spans (PC-relative `ldr`/`vldr`/`ldrd` targets,
4-byte gap threshold): **72 on the original, 68 on ours**.  Same-address word
match over the whole function: **432/14,308 words (3.0%)**.  Diff split:
**2,808 word-aligned bytes inside the original's pool spans, 52,696 in code**
(the pool is only ~5% of the residual; the rest is code layout/RA).

Only two pool spans start at the same address and have the same word count:

| pool | words | differing words |
|---|---:|---|
| `0x142320` | 6 | 2 (`GOT_PREL` only: `000ce1d2`/`000ce1d6`, `000ce0fa`/`000ce0fc`) |
| `0x14275c` | 18 | 2 (`GOT_PREL` only: `000ce12c`/`000ce128`, `000cd7b8`/`000cd7bc`) |

The differences are `symbol - (add_pc + 4 - pool_addr)` values: the consuming
`add rX, pc` sites are 4/2 B apart in otherwise same-size code
(`0x14208e`/`0x14208a` and `0x1423f6`/`0x1423f6`).  They cannot be fixed
without moving the consuming instruction, which would move the pool.

### First pool: contents identical, placement +28 B

Original `0x1380a4` (27 words), ours `0x1380c0` (25 words), **+28 B**:

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

Words 0..21 are the same values in the same order.  The differences are
`GOT_PREL` words, the 4-byte `double 0.1` alignment pad (original pool is
4 mod 8, ours 0 mod 8), and the last word (original 30000.0 from mission-5,
ours 10000.0 from mission-81 state 0).  The island boundary follows the block
laid out after the deactivation loop; this is the same finding as w13106.

### No wrong constants

Every literal in the original's first pool exists in our `.s` in the same
relative order.  The `movw`/`movt` immediate multiset is identical: **210
pairs each**, with only two pool-data false positives (original `movt r0,
#43020` at `0x140a2c`; ours `movw r0, #32780` at `0x14105c` are literal-pool
words disassembled as instructions).  There is no movw/movt ordering
difference and no missing/wrong literal to fix by source reordering.

## `-mllvm -arm-constant-island-max-iteration` diagnostic

Compiled with the exact `compile_commands.json` command plus
`-mllvm -arm-constant-island-max-iteration=N`:

| N | `.s` md5 |
|---:|---|
| 3 | `b54e9a1014e99a2de7933695bca859cd` |
| 5 | `b54e9a1014e99a2de7933695bca859cd` |
| 10 | `b54e9a1014e99a2de7933695bca859cd` |
| 50 | `b54e9a1014e99a2de7933695bca859cd` |
| 200 | `b54e9a1014e99a2de7933695bca859cd` |

Identical to the base `process.s`.  The pass converges immediately; the flag
is a hard no-op and cannot move the first island.

## The mission-5 `if` experiment (still not landable)

Changing only the `else if (getCurrentCampaignMission() == 5)` to a separate
`if` moves the first island to the original's boundary with the original's
27-word content (pad + last word `46ea6000`), but the function shrinks:

| candidate | raw | aligned | size | `.rel.plt` |
|---|---:|---:|---:|---|
| base (`else if`) | 51,930 | 20,928 | 57232/57232 | `289b2a06…` |
| mission-5 `if` | 55,255 | 22,592 | **57208/57232 (-24)** | `9568f1e6…` |
| `if` + `int` loop var / `while` / increment-in-body | 55,255 | 22,592 | **57208 (-24)** | `9568f1e6…` |
| `if` + `__builtin_expect(isWingMan(),1)` | 55,285 | 22,592 | **57208 (-24)** | `9568f1e6…` |
| `if` + `m_bStartSequenceOver` strh merge | 55,261 | 22,592 | **57208 (-24)** | `9568f1e6…` |
| `if` + block-local `st` removed | 55,266 | 22,592 | **57208 (-24)** | `9568f1e6…` |
| `if` + getter order variants | 55,261–55,263 | 22,592 | **57208 (-24)** | `9568f1e6…` |

The `if` form's first region is exactly the original's layout 4 B early
(pool `0x1380a0` vs `0x1380a4`, mission-5 check `0x40a` vs `0x412`); the
remaining 20 B is a whole-function block-layout/branch-width cascade.  Every
compensating source variant tried stays on the 57208 plateau, and the block
reorder changes `.rel.plt` to `9568f1e6…`.  Landing it therefore requires both
a size-exact compensation and a `.rel.plt` order restoration, not just the
`if` change.

## Constant-bearing adjacent-statement sweep

The w13101 harness was rebuilt from its note.  A full line-based sweep of
`process` (lines 605–5372) was filtered to adjacent simple statements at equal
indentation that mention a numeric literal, `Vector{`, `Globals::`,
`nextInt`, `setPosition`, `translate`, `setRotation` or `forceUpdate`:
**857 candidates** measured with the real build + gold link + GNU strip +
`reorder_shstrtab.py` recipe.  Fourteen are size-exact and `.rel.plt`-
identical raw winners.  Eight are semantically independent (table above); the
other six change evaluation order (`camPos +=` vs `routePoints`,
`camPos.y += 800` vs `setPosition(camPos)`, `*camPos +=` vs
`setPosition(*camPos)`, `m_tempVec +=` vs `setPosition(m_tempVec)`,
`m_tempVec +=` vs `chaser->setPosition(m_tempVec + …)`, `m_nScriptTimerA +=`
vs `approach = …`) and were left out.

The eight safe swaps compose to **-15 raw bytes** with no size or `.rel.plt`
change.

## Recommendation for the next pass

The first pool remains +28 B and its content is already faithful.  The only
lever that aligns it is the deactivation-loop / mission-5 block layout, which
requires the `if` form plus a 24 B size compensation and a `.rel.plt` order
restoration.  The two same-address pools (`0x142320`, `0x14275c`) differ only
in `GOT_PREL` words; a 4-byte local schedule change at the consuming
`add rX, pc` site would win 8 B but would move the pool unless exactly
compensated between the `add` and the pool.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13206
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51915 size=57232/57232 text=74208 relplt=289b2a069f8504d1e8a3919e43915e04
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51915 aligned=20938 size=57232/57232 insn=20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

Scratch harnesses (`poolcmp.py`, `poolwords.py`, `poolseq.py`, `poolsame.py`,
`poolprint.py`, `diffsplit.py`, `secdiff.py`, `try_variants.py`,
`try_variants2.py`, `try_expect.py`, `sweep_pairs.py`, `apply_swaps.py`,
`base.s`, `if.s`, `base.dis`, `if.dis`, `orig.dis`, `ours.dis`) live in
`/home/debian/GalaxyOnFire2/scratch/w13206/`; they are scratch, not tracked.
