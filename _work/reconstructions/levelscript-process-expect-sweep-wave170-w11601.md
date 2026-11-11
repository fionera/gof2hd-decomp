# LevelScript::process — wave 170 / w11601: full expect + existing-expect + operand-order sweep

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**), size-exact.
Owned TU `src/game/world/LevelScript.cpp`.  Base master `50716a763` (wave 170).
Scratch `/dev/shm/w11601`.

## Baseline (wave-170 master, gate green)

```
process raw diffbytes    53,932
process aligned insns    17,900
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          81,825
.rel.plt                 0        (BYTE-IDENTICAL, md5 289b2a069f8504d1e8a3919e43915e04)
```

## Method

Single-TU harness: the exact NDK r18b clang 7.0.2 `-Oz` command from `compile_commands.json`
(with `-mllvm -disable-postra-machine-sink`), then the real gold link line from `build.ninja`
with the candidate object substituted, then GNU strip + `reorder_shstrtab.py`.  The harness
reproduces the master `.so` exactly except the 20-byte `.note.gnu.build-id` (debug paths).
`raw` = same-address differing bytes in the 57,232-byte function body; `.text` = whole-section
differing bytes; `.rel.plt` md5 compared byte-for-byte.  Every candidate was size-checked
(`nm -S` == 0xdf90) before linking.

Three sweeps on the wave-170 base (620 `if`/`while`/`for` conditions in `process`):

1. **Whole-condition expect** — each condition wrapped in `__builtin_expect((cond), e)` for
   e = 0 and 1 (1,240 candidates; 75 transient compile/link failures re-run separately).
2. **Existing-expect** — the 59 `__builtin_expect` sites flipped (0<->1) and stripped (118).
3. **Operand-order** — adjacent same-operator `&&`/`||` operands swapped (123).

The full cell table (1,566 rows) is committed alongside as
`levelscript-process-cells-wave170-w11601.tsv` (columns: sweep, site, line, variant,
kind/op, expr, raw-or-status, text_diff, rel_plt).  `aligned` was measured with
`tools/fn_diff.py` for the landed cells; computing it for all 1,566 cells was out of the
time box (objdump of the 57 KB body twice per cell).

## Landed (four commits, all size-exact, `.rel.plt` byte-identical)

| # | commit | line | change | raw | aligned | .text |
|---|---|---|---:|---|---:|---:|
| 1 | `59963f261` | 1440 | remove inner `expect(m_nState == 2, 0)`, wrap whole `&&` with `expect(..., 1)` | 53,932 -> 53,911 | 17,900 -> 17,900 | 81,825 -> 81,789 |
|   |  | 2823 | `expect(player->isDockedToDockingPoint(), 1)` | (in #1) | | |
|   |  | 4266 | `expect(m_nState == 2, 0)` (mission-42 cargo chain) | (in #1) | | |
| 2 | `b5171597b` | 1149 | `expect(m_nScriptTimerA >= 6001, 0)` (mission-0 state-1) | 53,911 -> 53,857 | 17,900 -> 17,900 | 81,789 -> 81,735 |
| 3 | `42e9d7dc5` | 4410 | flip `expect(m_nScriptTimerA >= 324, 0)` -> `1` | 53,857 -> 53,816 | 17,900 -> 17,936 | 81,735 -> 81,694 |
|   |  | 4296 | flip `expect(!field_0x13c && rnd<=9, 1)` -> `0` | (in #3) | | |
|   |  | 1114 | strip `expect(m_nScriptTimerA >= 1100 - delta, 0)` | (in #3) | | |
|   |  | 5122 | flip `expect(m_nScriptTimerA < 6000, 1)` -> `0` | (in #3) | | |
| 4 | `eaec3564e` | 2277 | swap `&&` operands (mission-42 state-2 `isTriggered() && m_nState == 2`) | 53,816 -> 53,812 | 17,936 -> 17,932 | 81,694 -> 81,690 |

Final: **raw 53,812 / aligned 17,932 / size 57,232 / `.text` 81,690 / `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04` (byte-identical)**.

## Best measured-but-not-landed cells (on the base named)

All are size-exact and `.rel.plt`-identical unless marked.  `raw` is the value with that one
cell applied to the stated base.

| base raw | line | variant | cell | raw | note |
|---:|---:|---|---|---:|---|
| 53,932 | 4267 | expect 0 | `hasCargo(201)||...||hasCargo(205)` | 53,924 | same codegen as line 4266; only one of the pair can land (both = 0xdf8c) |
| 53,932 | 4433 | expect 1 | `m_nScriptTimerA > 2000 && < 5000` | 53,930 | -2; adding to the landed set costs +1 |
| 53,857 | 4003 | strip | `expect(m_nState == 3, 1)` | 53,852 | -5 alone; +6 when added to the landed set |
| 53,857 | 3160 | flip | `expect((m_nState == 11), 1)` -> 0 | 53,856 | -1 alone; size becomes 0xdf94 in the landed set |
| 53,857 | 4296 | strip | `expect(!field_0x13c && rnd<=9, 1)` | 53,855 | flip (landed) is better |
| 53,857 | 4410 | strip | `expect(m_nScriptTimerA >= 324, 0)` | 53,849 | flip (landed) is better |

Combination interactions are real: 4266+4267 together shrink the function to 0xdf8c; the four
landed expects in #3 are additive, but adding 4003/3160 on top is worse or size-changing.
