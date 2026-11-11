# LevelScript::process — wave 174 / w11901: full expect re-sweep on the post-wave-173 base

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**), size-exact.
Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11901`, branch
`wave174-w11901`, base master `be9dd429b` (wave-173 merge).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11901`.

Follow-up to `levelscript-process-expect-sweep-wave173-w11801.md` (the wave-172
sweep) and `levelscript-process-alias-wave173-w11804.md` / the w11805 alias
pass.  The wave-173 alias set changed register allocation, so the whole-condition
sweep was re-run on the new base: a cell that was neutral on the wave-172 base
can win now (and vice versa).

## Baseline (post-wave-173 master `be9dd429b`, gate green)

```
process raw diffbytes    53,711
process aligned insns    18,228
orig / ours size         57232 / 57232   (size-exact)
orig / ours insns        20830 / 20728
.text diffbytes          80,897
.rel.plt                 0 differing bytes (md5 289b2a069f8504d1e8a3919e43915e04)
```

## Result: no size-exact raw win

**No whole-condition `__builtin_expect` cell and no existing-expect
flip/strip cell is a size-exact raw improvement on this base.**  `src/` is
left exactly at the base; this note and the two cell tables are the only
changes.

## Method

Condition parser: a comment/string/preprocessor-aware scanner over
`LevelScript.cpp` finds every `if`/`while`/`for` keyword and matches its
parenthesised condition (for `for`, only the middle clause).  On the wave-172
source it reproduces the wave-173 w11801 condition set exactly (620 process
conditions, 529 `if` + 91 `for`, identical line/kind set).  On the current
source it finds the same 620 process conditions (line numbers shifted by the
wave-173 alias edits), plus the 23 conditions in the functions after `process`
and the one `if` inside a `//` comment — the historical 644-condition set.

Each cell wraps one condition as `__builtin_expect((cond), e)` for `e = 0`
and `e = 1` and rebuilds the real link:

```
ninja -C cmake-build-match libgof2hdaa.so -j2
```

(then the normal gold link + GNU `strip --strip-all` + `reorder_shstrtab.py`).
The measurement reads the candidate ELF directly: `.dynsym` gives the
`_ZN11LevelScript7processEi` value/size, `.text` gives the same-address byte
diff over the original 57,232-byte body, and `.rel.plt` is md5'd
byte-for-byte.  The harness is validated by reproducing the wave-173 w11801
win on the wave-172 source: wrapping line 2533
`!((RadioMessage *) ((*messages)[2]))->isOver()` with `expect(..., 1)` gives
**raw 53,749, size 57,232** exactly as that note records.  Rows with
`size != 57,232` are rejected (SIZE IS SACRED).

The full 1,288-cell table is committed as
`levelscript-process-cells-wave174-w11901.tsv`
(columns: line, kind, variant, raw, size, text, relplt, cond).

### Sweep A — whole-condition expect (644 conditions x 2 = 1,288 cells)

| scope | cells | size-exact | neutral (53,711) | worse | better | size-changing | compile-fail |
|---|---:|---:|---:|---:|---:|---:|---:|
| `process` (620 conds) | 1,240 | 527 | 386 | 141 | **0** | 713 | 0 |
| tail functions (23 conds) | 46 | 34 | 34 | 0 | 0 | 0 | 12 |
| `//` comment (1 cond) | 2 | 2 | 2 | 0 | 0 | 0 | 0 |
| **total** | **1,288** | 563 | 422 | 141 | **0** | 713 | 12 |

The 12 compile failures are the pointer-only tail conditions
`if (m_pGeometryN)` (`__builtin_expect` needs an integral argument); they are
all in `render3D`, after `process`, and cannot affect the `process` body.
Every size-exact `process` cell has `.text` exactly 80,897 and `.rel.plt`
exactly the baseline md5.  The lowest `process` raw among the 527 size-exact
cells is **53,711** — i.e. neutral, never below the baseline.

Cells that are close but not wins (size-exact, raw > 53,711):

| line | kind | var | raw | condition |
|---:|---|---:|---:|---|
| 1034 | if | 1 | 53,712 | `m_nState == 3` |
| 1227 | if | 0 | 53,712 | `m_nScriptTimerA < 3001` |
| 1342 | if | 0 | 53,712 | existing `expect(!isFading(), 1)` (wrapped again) |
| 2233 | if | 1 | 53,712 | existing `expect(isAlwaysFriend(), 0)` (wrapped again) |
| 2828 | if | 1 | 53,712 | `messages[3]->isTriggered()` |
| 3731 | if | 0 | 53,712 | existing `expect(i == 3, 1)` (wrapped again) |

The wave-173 w11801 winning cell (`!messages[2]->isOver()`, now line 2540) is
**size-changing** on this base: variant 0 is 57,224 B / raw 54,148, variant 1
is 57,240 B / raw 54,921.  The alias set that produced the 53,711 base moved
that block out of the size-exact shape.

### Sweep B — existing-expect flip / strip (60 sites x 2 = 120 cells)

All 60 `__builtin_expect` sites in `process` were flipped (0<->1) and stripped
(removed), 120 cells.  Result: **10 neutral, 48 worse, 62 size-changing, 0
better**.  The neutral cells are exact no-ops (raw 53,711, `.text` 80,897):

lines 1035 (flip+strip), 1896 (strip), 2288 (strip), 2872 (flip),
3804 (flip+strip), 4657 (strip), 4708 (strip), 5146 (strip).

The table is committed as `levelscript-process-expects-wave174-w11901.tsv`
(columns: line, variant, raw, size, text, relplt, arg0).

### Pair exploration (30 pairs, all negative)

Because the wave-173 alias passes found additive/interacting wins, a bounded
pair check was run around the closest size-changing cells.  Two families were
tested with the same harness (both conditions wrapped at once):

* the eight +1-raw size-exact cells paired with the nearest -4/-8 cells in the
  same mission region (14 pairs);
* the dropped w11801 cell (line 2540, -8/+8 B) paired with the nearest
  opposite-delta cells (16 pairs).

Every pair is size-exact but raw-worse; the best is
`5302:1 + 4550:1` at **raw 53,812** (the next best are 53,838-53,896).
No pair beats 53,711.  This is consistent with the wave-173 w11801 note's
finding that the expect cells interact only weakly.

## Gate

`check` is green on the unchanged tree: byte_exact **4507/4524**, linked_exact
**4507/4524**, size-exact **4524/4524**, data_exact **843/843**, imports
**207**, lint **CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0
missing).  `process` remains at raw **53,711** / aligned **18,228** / size
**57,232**, `.text` 80,897, `.rel.plt` byte-identical.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53711 aligned= 18228 size= 57232/57232 insn= 20728/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

The sweep drivers and the fast ELF measurement script are committed alongside
as `levelscript-process-expect-sweep-w11901.py`,
`levelscript-process-expects-w11901.py` and `measure_w11901.py`.
