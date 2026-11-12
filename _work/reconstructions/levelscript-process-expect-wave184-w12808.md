# LevelScript::process — wave 184 / w12808: expect add/flip/remove re-sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12808`, branch
`wave184-w12808`, base master `4c06c2662` (wave-183 harvest 6, raw **52,281**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12808`.

This pass re-runs the whole-condition `__builtin_expect` sweep on the current
52,281 body and additionally covers (a) every leaf comparison of every
`&&`/`||` condition and (b) flip/remove of every existing hint in
`LevelScript::process`.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,281           52,280     (-1)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
ours insns               20,725            20,725
.text diffbytes          74,842           74,841     (-1)
.ARM.extab diffbytes     646              646        (size/base exact)
.note.gnu.build-id       20               20         (unchanged)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
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

## The change

One hint removed (semantics-preserving; the `// lint: arm_builtin` waiver text
stays on the line):

**Line 4284 — mission-142 gas-cloud cargo spark probe.**  Removing the
`__builtin_expect(..., 1)` from the `VectorLength(...) < 3000.0f` guard is
size-exact (57,232 B) and lowers the raw diff by one byte:

```cpp
// before
if (__builtin_expect((AbyssEngine::AEMath::VectorLength( // lint: arm_builtin
            e0A->getPosition() - e0A->getRoute()->getWaypoint(0)->getPosition())
    < 3000.0f), 1)) {
// after
if ((AbyssEngine::AEMath::VectorLength( // lint: arm_builtin
            e0A->getPosition() - e0A->getRoute()->getWaypoint(0)->getPosition())
    < 3000.0f)) {
```

The extra paren pair left by the removal is valid C++ and is exactly the text
that was measured; it does not affect codegen.

## Sweep results (no other cell landed)

* **Whole-condition add sweep** — 620 `if`/`for` conditions x {0,1} = 1,240
  cells on the current body.  558 size-exact; **none** has raw < 52,281 (the
  best size-exact cells are neutral at 52,281).  No size-changing cell has
  raw < 52,281 either.
* **Leaf-comparison add sweep** — 223 top-level `&&`/`||` operands x {0,1} =
  446 cells.  197 size-exact; **none** improves (best are neutral 52,281).
* **Flip/remove of existing hints** — 68 hints x {flip, remove} = 136 cells.
  65 size-exact; exactly one improves: `REMOVE_4284` -> **52,280** (landed).
  The next best size-exact flips/removes are neutral or worse; the best
  size-changing removals are 57,228 B at raw 52,337–52,356.

The wave-183 harvest-4 cells (lines 1449/3151/4736/5187) are already in the
base and were re-measured as part of the flip/remove sweep: flipping or
removing them is neutral or worse on this body, so they are kept.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12808
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52280 aligned= 20701 size= 57232/57232 insn= 20725/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```

The sweep drivers (`sweep_proc.py`, `leaves.py`, `flipremove.py`, `runner.py`)
and result tables (`cells.tsv`, `leaves_res2.tsv`, `fr_res3.tsv`) live in
`/home/debian/GalaxyOnFire2/scratch/w12808/` (scratch, not tracked).
