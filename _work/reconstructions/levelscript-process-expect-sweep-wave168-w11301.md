# LevelScript::process — wave 168 / w11301: size-exact companion for the L5132+L3111 near-miss

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact.  Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11301`,
base master `631c26f0b` (wave 167).  Scratch `/dev/shm/w11301/`.

## Baseline (wave-167 merge, unchanged)

```
process raw diffbytes    54,031
process aligned insns    17,764
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          82,526
.ARM.extab diffbytes     670
.rel.plt                 0        (BYTE-IDENTICAL)
byte_exact 4507 / linked_exact 4507 / size 4524 / data 843 / imports 207
```

## Result (landed)

```
process raw diffbytes    54,029   (-2)
process aligned insns    17,786   (+22, indicative only)
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          82,524   (-2)
.ARM.extab diffbytes     670     (unchanged)
.rel.plt                 0        (BYTE-IDENTICAL)
RATCHET PASS (byte 4507 / linked 4507 / size 4524 / data 843 / imports 207 / lint CLEAN)
```

Three hints, one commit:

| # | line | cell | e | guard |
|---|---|---|---:|---|
| 1 | 5132 | full if | 0 | `m_nScriptTimerA >= 5500` (explosion update) |
| 2 | 3111 | full if | 1 | `m_nScriptTimerA >= 8001` (state-8 reframe) |
| 3 | 4871 | for cond | 1 | `i < 22` (destroyed-ship count loop) |

Cells 1+2 are the wave-167 near-miss pair (`L5132:0 + L3111:1`): it is
**54,032 / size 57,236** on this base (the wave-167 note records 54,030;
measurement here uses `tools/fn_diff.py`'s raw metric and the pair alone is
+4 B, i.e. gate-fatal).  Cell 3 is the size-exact companion: alone on the
final base it is 54,055 / size 57,232 (worse), but on the pair base it is
**54,029 / size 57,232**, i.e. it shifts the pair's +4 B tail back to the
original length while also dropping 3 common-region bytes.  Net -2 raw,
-2 `.text`, function size exact, `.rel.plt` byte-identical, `.ARM.extab`
unchanged (670).

## Method

The wave-167 note left the pair size-changing (+4) with no companion.  A
fresh candidate parser (`/dev/shm/w11301/gen.py`) enumerates *every*
`if`/`while`/`for` condition plus **every `&&`/`||` subexpression at any
nesting depth** (791 cells) in the `process()` body (lines 605-5306), i.e.
the nested-operand space the wave-167 sweep only sampled (4 cells).
`/dev/shm/w11301/sweep.py` compiles one candidate TU with the exact NDK r18b
clang 7.0.2 `-Oz` command (4-way parallel; a compile is ~1 s) and reads the
`process` size from the object's symtab.  Only cells with size 57,232 on the
pair base were linked and raw-measured (69 cells, ~1.5 s each).

The 69 size-exact cells on the pair base were all raw-measured; the best was
cell 3 at 54,029.  Adding any of the 23 next-best cells on top of cell 3
does not improve it (best addition 54,052).

## Tested but not landed (on the pair base, size-exact)

| line | cell | e | raw | extab | note |
|---|---|---:|---:|---:|---|
| 3123 | if-node | 1 | 54,034 | 671 | `m_nState == 9` / `== 10` sub-operand |
| 3123 | if-whole | 0/1 | 54,046 | 670 | nested expect around the landed pair |
| 4596 | if-whole | 1 | 54,062 | 670 | `messages[4]->isOver()` |
| 5126 | if-whole | 0 | 54,064 | 673 | `m_nScriptTimerA >= 5700 && ...` |
| 4596 | if-whole | 0 | 54,071 | 670 | |
| 2893 | if-node | 0 | 54,073 | 673 | `field_178 > 7` |
| 4649 | if-whole | 0/1 | 54,093 | 670 | `messages[6]->isOver()` |
| 2744 | if-whole | 0 | 54,104 | 673 | `m_nScriptTimerA >= 8001` (other arm) |
| 4970 | if-node | 1 | 54,104 | 673 | |
| 2552 | for-whole | 1 | 54,120 | 673 | |
| 2735 | if-whole | 1 | 54,123 | 670 | |

Cells whose size is not 57,232 on the pair base were rejected (e.g.
`L2893:0` is 57,228 alone; `L3123:1` node is 57,232 but raw 54,034).

## Reproduction

```sh
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j2
```
