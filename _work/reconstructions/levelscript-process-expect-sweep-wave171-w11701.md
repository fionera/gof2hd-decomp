# LevelScript::process — wave 171 / w11701: full expect re-sweep on the w11601 base

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**), size-exact.
Owned TU `src/game/world/LevelScript.cpp`.  Base master `41ef4c8b6` (wave 171, after the
w11601 expect sweep).  Scratch `/home/debian/GalaxyOnFire2/scratch/w11701`.

## Baseline

```
process raw diffbytes    53,812
process aligned insns    17,932
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          81,661
.rel.plt                 0        (BYTE-IDENTICAL, md5 289b2a069f8504d1e8a3919e43915e04)
```

## Method

Single-TU harness: the exact NDK r18b clang 7.0.2 `-Oz` command from
`compile_commands.json` (with `-mllvm -disable-postra-machine-sink`), then the real gold
link line from `build.ninja` with the candidate object substituted, then GNU strip +
`reorder_shstrtab.py`.  The harness reproduces the master `.so` exactly except the 20-byte
`.note.gnu.build-id`.  `raw` = same-address differing bytes in the 57,232-byte function
body; `.text` = whole-section differing bytes; `.rel.plt` md5 compared byte-for-byte.
Every candidate was size-checked (`nm -S` == 0xdf90) before linking.

**Sweep A (whole-condition expect, re-run on the current base).**  Every `if`/`while`/`for`
condition in `process` (644 conditions) was wrapped in `__builtin_expect((cond), e)` for
e = 0 and 1 (1,288 cells).  This is the w11601 sweep repeated on the post-w11601 base,
because the landed expects change the register allocation and literal-pool layout and a
cell that was neutral on the old base can become a win (and vice versa).  The table below
is the final re-sweep measured on the post-`ea56d2180` base (raw 53,794); no cell in it
beats 53,794.  The two landed cells were found by the first pass of the sweep on the
53,812 / 53,810 bases.

**Sweep B (existing-expect strip/flip).**  The three "best measured but not landed" cells
from w11601 were re-tested on the current base.

The full cell table is committed alongside as
`levelscript-process-cells-wave171-w11701.tsv` (columns: line, kind, variant, raw, size,
text_diff, rel_plt_md5).  Rows with `size` in the raw column changed the function size and
were rejected (SIZE IS SACRED).  `aligned` was measured with `tools/fn_diff.py` only for
the landed cells.

## Landed (two commits, both size-exact, `.rel.plt` byte-identical)

| # | commit | line | change | raw | aligned | .text |
|---|---|---|---:|---|---:|---:|
| 1 | `8bd0c64f3` | 4433 | `expect(m_nScriptTimerA > 2000 && < 5000, 1)` (mission 95 wingman drift) | 53,812 -> 53,810 | 17,932 -> 17,942 | 81,661 -> 81,659 |
| 2 | `ea56d2180` | 4243 | `expect(VectorLength(enemies[0] - waypoint0) < 3000.0f, 1)` (mission 142 cargo pickup) | 53,810 -> 53,794 | 17,942 -> 17,956 | 81,659 -> 81,643 |

Final: **raw 53,794 / aligned 17,956 / size 57,232 / `.text` 81,643 / `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04` (byte-identical)**.

## Notes

* The mission-142 cargo-proximity test (line 4243) is the largest single-cell win found:
  -16 raw bytes.  The `__builtin_expect` must share its line with the `// lint: arm_builtin`
  waiver (the lint scanner is same-line only), so the waiver sits on the
  `__builtin_expect((AEMath::VectorLength(` line, not on the closing paren.
* Most whole-condition cells are either neutral (raw unchanged) or change the function
  size.  The size-changing cells cannot be landed individually; pairing them would require
  an exact compensation, and the w11601 note already showed the obvious pairs
  (4266+4267) shrink to 0xdf8c.
* Re-tested existing-expect cells on the current base (all worse than 53,810):
  strip `expect(m_nState == 3, 1)` -> 54,952; strip `expect((m_nState == 11), 1)` ->
  53,837; strip `expect(!field_0x13c && rnd<=9, 0)` -> 53,818; flip `m_nState == 11`
  -> size 57,236.  Removing the first wingman-loop expect -> 53,825.
* The first basic block (mission-0 intro state 0) is still 8 instructions / 28 bytes
  larger than the original, which shifts every literal pool.  The largest component is the
  placement of the tail's first wingman loop *before* the first literal pool instead of
  after it (11 instructions); the rest is register-allocation / loop-rotation noise
  (`sl` vs `fp` for `this`, vtable CSE).  No expect cell in this sweep moves that block's
  size to 0x38c.
