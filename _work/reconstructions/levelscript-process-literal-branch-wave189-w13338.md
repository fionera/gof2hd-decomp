# LevelScript::process — wave 189 / w13338: literal-pool + branch-layout + expect re-sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13338`, branch `wave189-w13338`, base master
`1de8e9254` (raw **51,874**, `.text` **73,999**, `.ARM.extab` diff 639,
total 74,658).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13338`.

This pass re-maps the literal pools and branch displacements on the current
51,874 body, re-runs the whole `__builtin_expect` sweep (3,110 cells), a
comparison-operand swap sweep (524 cells), a constant-bearing adjacent
statement sweep (682 cells) and a constant-bearing 3-statement rotation sweep
(1,238 cells).  Seven independent, semantics-preserving edits compose to a
size-exact, `.rel.plt`-identical raw win:
**51,874 -> 51,862 (-12)**.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,874           51,862     (-12)
process aligned insns    21,005           21,017     (+12)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          73,999           73,987     (-12)
.ARM.extab diffbytes     639              639        (size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.note.gnu.build-id       20               20
total differing bytes    74,658           74,646     (-12)
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
included) from `tools/fn_diff.py` / `measure_w11901.py`.  `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after.

## Landed changes

### A. `translate` / `setComputerControlled` swap (-6)

Source lines 1960-1961 (`m_nState == 1` escort block):

```cpp
// before
m_pCamera->translate(0.0f, 0.0f, (float) (-2 * delta));
player->setComputerControlled(false);
// after
player->setComputerControlled(false);
m_pCamera->translate(0.0f, 0.0f, (float) (-2 * delta));
```

The two statements have disjoint receivers (camera vs player) and no data
dependence.  The original emits the `PlayerEgo::setComputerControlled(false)`
call/store ahead of the `TargetFollowCamera::translate` call in this block.
Measured alone: raw **51,874 -> 51,868** (-6), size-exact, `.rel.plt`
identical.

### B. `updateEvent3DAttributes(158)` / `play(158)` swap (-1)

Source lines 721-722 (mission-0 state-0 sound block):

```cpp
// before
Globals::sound->updateEvent3DAttributes(158, &pos, &zero158, false);
Globals::sound->play(158, nullptr, nullptr, 0.0f);
// after
Globals::sound->play(158, nullptr, nullptr, 0.0f);
Globals::sound->updateEvent3DAttributes(158, &pos, &zero158, false);
```

The original order at `0x13904e`/`0x139060` is `play(158)` then
`updateEvent3DAttributes(158)` (the same order as the preceding 157 pair at
`0x13902a`/`0x139040`), so the swap is faithful as well as independent.
Measured alone: raw **51,874 -> 51,873** (-1), size-exact, `.rel.plt`
identical.

### C. `pirate` load / `nextInt(10000)` swap (-1)

Source lines 2411-2412 (mission pirate spawn loop):

```cpp
// before
KIPlayer *pirate = (*enemies)[i];
int dy = Globals::rnd->nextInt(10000);
// after
int dy = Globals::rnd->nextInt(10000);
KIPlayer *pirate = (*enemies)[i];
```

The `(*enemies)[i]` array load and the `Globals::rnd->nextInt(10000)` virtual
call are independent.  Measured alone: raw **51,874 -> 51,873** (-1),
size-exact, `.rel.plt` identical.

### D. Whole-condition `__builtin_expect` at line 1903 (-1)

```cpp
// before
} else if (((RadioMessage *) ((*messages)[2]))->isOver() && __builtin_expect(m_nState == 1, 1)) {
// after
} else if (__builtin_expect(((RadioMessage *) ((*messages)[2]))->isOver() && __builtin_expect(m_nState == 1, 1), 1)) {
```

Branch-weight hint only; semantics-preserving.  Measured alone: raw
**51,874 -> 51,873** (-1), size-exact, `.rel.plt` identical.  The equivalent
sub-expression cell (wrapping only the `isOver()` operand) is also -1; the two
do not compose (either one alone gives 51,873).

The four edits compose additively: raw **51,865** (-9), `.text` **73,990**,
size 57,232/57,232, `.rel.plt` byte-identical, `.ARM.extab` content unchanged
(639, 156,312 B).

### E. Three distance-2 rotations (-3 more)

A constant-bearing 3-statement rotation sweep (`scratch/w13338/sweep_rot2.py`,
1,238 candidates) found three further independent, semantics-preserving
rotations.  Each is a cyclic rotation of three consecutive simple statements
with disjoint receivers:

| source line | rotation | raw alone |
|---:|---|---:|
| 1662 | `m_nScriptTimerA = m_nScriptTime;` before `forceUpdate` + `m_nState = 4` | -1 |
| 4834 | `setPosition(wgeometry4->getPosition())` before `setLookAtCam(true)` + `setTarget(wgeometry4)` | -1 |
| 5247 | `enemies[0]->geometry->setPosition(...)` and `player->geometry->rotate(...)` before `m_pCamera->setTarget(enemies[0]->geometry)` | -1 |

All three receivers are disjoint (`m_nState`/timer vs LOD manager; camera
position vs look-at flag vs target pointer; enemy geometry position vs player
geometry rotation vs camera target pointer).  Composed with A-D: raw
**51,862** (-12), `.text` **73,987**, size 57,232/57,232, `.rel.plt`
byte-identical, `.ARM.extab` 639 unchanged.

Other size-exact rotation wins were rejected as semantically wrong:
line 5229 (`approach` must read the incremented timer), line 4411
(`setPosition` must read the updated `m_tempVec`), line 2531 (`setPosition`
must read the updated `camPos.x`), line 2674 (`setPosition` after
`delete frame` is a use-after-free), line 2695 / 3158 / 4603 (matrix/position
consumers must read the updated vector).

## Literal-pool map on the current 51,874 body

Reference-clustered pool spans (PC-relative `ldr`/`vldr`/`ldrd` targets,
4-byte gap threshold): **71 on the original, 68 on ours**; 775 vs 771 pool
references.  Same-address pools:

| pool | words | differing words |
|---|---:|---|
| `0x142320` | 6 | 2 (`GOT_PREL` only: `000ce1d2`/`000ce1d6`, `000ce0fa`/`000ce0fc`) |
| `0x14275c` | 18 | 2 (`GOT_PREL` only: `000ce12c`/`000ce128`, `000cd7b8`/`000cd7bc`) |

The differences are `symbol - (add_pc + 4 - pool_addr)` values; the consuming
`add rX, pc` sites are 4/2 B apart in otherwise same-size code
(`0x14208e`/`0x14208a` and `0x1423f6`/`0x1423f6`).  Each differing word costs
exactly one raw byte (total 4 B).  They cannot be fixed without moving the
consuming instruction, which would move the pool.

First pool: original `0x1380a4` (27 words), ours `0x1380c0` (25 words),
**+28 B**.  Words 0..21 are the same values in the same order; the differences
are the `GOT_PREL` words, the 4-byte `double 0.1` alignment pad
(`bf00bf00`, original pool is 4 mod 8, ours 0 mod 8), and the last word
(original `459c4000` = 5000.0 / `46ea6000` = 30000.0, ours `461c4000` =
10000.0).  The island boundary follows the block laid out after the
deactivation loop; the content is faithful, only the placement differs.

### Mission-5 `if` experiment (still not landable)

The original reaches the mission-5 check at `0x13812a` from both arms of
`if (m_bStartSequence != 0)` (`beq.w 0x13812a` at `0x137f82`, fall-through
from the active tail), i.e. it is a separate `if`, not the `else if` used in
our source.  Changing only that `else if` to `if`:

| candidate | raw | aligned | size | `.rel.plt` |
|---|---:|---:|---:|---|
| base (`else if`) | 51,874 | 21,005 | 57232/57232 | `289b2a06…` |
| mission-5 `if` | 55,257 | 22,616 | **57216/57232 (-16)** | `9568f1e6…` |

The `if` form moves the first island to `0x1380a0` (4 B before the original's
`0x1380a4`) with the original's 26-word content, but the function is 16 B
short and `.rel.plt` changes (the `FModSound::play` first reference moves).
Landing it needs both a +16 B size compensation and a `.rel.plt` order
restoration; not attempted here (w13336 owns the full-body/RA shape).

## `__builtin_expect` re-sweep (3,110 cells)

The tracked whole-condition (`levelscript-process-expect-sweep-w11901.py`),
sub-expression (`levelscript-process-subs-sweep-w12004.py`) and flip/strip
harnesses were re-run from scratch on the 51,874 body with the fast
compile+gold-link harness (`scratch/w13338/fast.py`, exact
`compile_commands.json` command incl. `-mllvm -disable-postra-machine-sink`).

| sweep | cells | size-exact | size-exact better |
|---|---:|---:|---:|
| whole-condition add (693 conditions x {0,1}) | 1,386 | 1,064 | 2 (one in `process`) |
| sub-expression add (`&&`/`||`, inner-expect, `!(...)`) | 1,686 | 1,330 | 1 (same site) |
| flip/strip existing hints (138 x 1) | 138 | 69 | 0 |

The only size-exact `process` winner is the line-1903 whole-condition cell
(-1), also present as the `andor` sub-expression cell.  The two other -1
whole-condition cells are in `LevelScript::resetCamera` (line 5375) and
`LevelScript::skipCutscene` (line 5434); both **change the edited function's
size** (`resetCamera` 0x7c -> 0x80, `skipCutscene` 0x19c -> 0x1a0), which
breaks `size-exact` and shifts every later function (whole-`.text` diff jumps
to ~399 KB), so they are rejected.  (They do move the `process` raw by -1 each
through the inlined call displacements, but the net `.text` cost is enormous.)

## Comparison-operand swap sweep (524 cells)

Every top-level comparison in an `if`/`while`/`for` condition was rewritten
`a <op> b` -> `b <op'> a` (`<`<->`>`, `<=`<->`>=`, `==`/`!=` unchanged) and
measured.  **No size-exact, `.rel.plt`-identical win** on the current body
(all 524 cells are >= 51,874).

## Constant-bearing adjacent-statement sweep (682 cells)

Adjacent simple statements at equal indentation where either mentions a
numeric literal, `Vector{`, `Globals::`, `nextInt`, `setPosition`, `translate`,
`setRotation` or `forceUpdate` were swapped and measured.  Six size-exact,
`.rel.plt`-identical wins were found; three are semantically safe and landed
(B, C above plus the -6 A swap), the other three are rejected:

* line 5229 `m_nScriptTimerA += delta;` / `float approach = (float) m_nScriptTimerA / -34000.0f + 1.0f;`
  — `approach` must read the incremented value (original `0x138888` adds
  `delta` before the `l2f`/`vdiv`), so swapping is semantically wrong;
* line 2532 `camPos.y += 800.0f;` / `m_pCamera->setPosition(camPos);`
  — `setPosition` must read the updated `camPos`;
* line 2674 `frame->setPosition(m_tempVec);` /
  `canvas->CameraSetLocal(..., frame->getMatrix());`
  — `CameraSetLocal` reads the matrix that `setPosition` writes (w13309).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13338
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51862 size=57232/57232 text=73987 relplt=289b2a069f8504d1e8a3919e43915e04
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51862 aligned=21017 size= 57232/57232 insn=20726/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
cmake --build cmake-build-match --target drift -- -j2   # 0 drifted fields
```

Scratch harnesses (`fast.py`, `pools.py`, `bmap.py`, `sweep_expect.py`,
`sweep_adj.py`, `sweep_cmp.py`, `sweep_rot2.py`, `expect_res.jsonl`,
`adj_res.jsonl`, `cmp_res.jsonl`, `rot2_res.jsonl`) live in `/home/debian/GalaxyOnFire2/scratch/w13338/`; they are
scratch, not tracked.

## Current-master re-test (2026-11-12, after w13330/w13351)

Base: `master` `580751537`, `LevelScript::process` raw **51,844**, aligned 20,992.
The four literal/branch edits were re-tested individually on this base:

| edit | raw | delta |
|---|---:|---:|
| whole-condition `__builtin_expect` at line 1903 | 51,844 | 0 |
| `int dy` / `KIPlayer *pirate` swap at line 2408 | 51,846 | +2 |
| `m_pCamera->translate` / `player->setComputerControlled` swap at line 1957 | 51,850 | +6 |
| all three together | 51,852 | +8 |

The line-721 `play(158)`/`updateEvent3DAttributes(158)` swap was already landed by
w13330 and is not re-counted. The three 3-statement rotations from the later
`1152150e9` commit do land on current master: raw **51,844 -> 51,842** (-2),
aligned 20,992 -> 20,998, `.text` 73,567 -> 73,565, size 57,232/57,232.
