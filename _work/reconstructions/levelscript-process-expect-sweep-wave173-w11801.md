# LevelScript::process — wave 173 / w11801: full expect re-sweep on the post-wave-172 base

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**), size-exact.
Owned TU `src/game/world/LevelScript.cpp`.  Worktree `wt/w11801`, branch `wave173-w11801`,
base master `3fafa5b03` (merge of wave172, after the alias/state-8 passes).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11801`.

Follow-up to `levelscript-process-expect-sweep-wave171-w11701.md` and
`levelscript-process-alias-wave171-w11704.md`.  The wave-172 alias set changed register
allocation, so the whole-condition sweep was re-run on the new base: a cell that was neutral
on the wave-171 base can now win (and vice versa).

## Baseline (post-wave-172 master, gate green)

```
process raw diffbytes    53,750
process aligned insns    18,049
orig / ours size         57232 / 57232   (size-exact)
.text diffbytes          81,503
.rel.plt                 0        (BYTE-IDENTICAL, md5 289b2a069f8504d1e8a3919e43915e04)
```

## Method

Single-TU harness: the exact NDK r18b clang 7.0.2 `-Oz` command from
`compile_commands.json` (with `-mllvm -disable-postra-machine-sink`), then the real gold
link line from `build.ninja` with the candidate object substituted, then GNU strip +
`reorder_shstrtab.py`.  The harness reproduces the master `.so` exactly except the 20-byte
`.note.gnu.build-id`.  `raw` = same-address differing bytes in the 57,232-byte function body
(plus the size delta); `.text` = whole-section differing bytes; `.rel.plt` md5 is the md5 of
the `objcopy -O binary --only-section=.rel.plt` bytes.  Every candidate was size-checked
(`nm -S` == 0xdf90) before linking.

**Sweep A (whole-condition expect).**  Every `if`/`while`/`for` condition in `process`
(620 conditions) was wrapped in `__builtin_expect((cond), e)` for e = 0 and 1 (1,240 cells).
For `for` loops only the middle clause was wrapped (`for (init; expect((cond), e); inc)`).
This includes the 57 conditions that already carry an existing `__builtin_expect`.  The full
cell table is committed alongside as `levelscript-process-cells-wave173-w11801.tsv`
(columns: line, kind, variant, raw, size, rel_plt).  Rows with `size` != 57,232 were rejected
(SIZE IS SACRED).

The historical wave-171 "644 conditions" set also counted 1 `if` inside a `//` comment and 23
conditions in the functions after `process` (`resetCamera`, `lookBehind`, `render3D`,
`skipSequence`, `canSkipCutsceneNow`, `skipCutscene`).  Those 24 extra cells were re-swept
too: all are neutral for the `process` body (its bytes are independent of later functions);
12 of the 46 tail cells are compile-fail because `__builtin_expect` needs an integral
argument for the pointer-only `if (m_pGeometryN)` tests.

**Sweep B (existing-expect strip/flip).**  All 61 `__builtin_expect` sites in `process` were
flipped (0<->1) and stripped (122 cells) on both the pre-landing and post-landing bases.

## Result of Sweep A on the pre-landing base (raw 53,750)

534 of the 1,240 cells are size-exact; 386 are neutral (raw 53,750); 706 change the function
size.  Exactly **one** size-exact cell is a raw win:

| line | kind | variant | condition | raw | delta |
|---:|---|---:|---|---:|---:|
| 2533 | if | 1 | `!((RadioMessage *) ((*messages)[2]))->isOver()` (mission-0 state-2 conclude) | 53,749 | -1 |

Every size-changing cell has raw >= 53,750 (the closest are +4 B shapes at raw 53,792), so no
pair of them can compensate to a size-exact win.  On the wave-171 base the same cell (then
line 2526) was neutral at 53,794 for both variants — this is exactly the "neutral before, wins
now" case the re-sweep was looking for.

## Landed (one commit, size-exact, `.rel.plt` byte-identical)

| # | line | change | raw | aligned | .text |
|---|---|---|---:|---|---:|
| 1 | 2533 | `expect(!isOver(), 1)` (mission-0 state-2 conclude) | 53,750 -> 53,749 | 18,049 -> 18,049 | 81,503 -> 81,502 |

Final: **raw 53,749 / aligned 18,049 / size 57,232 / `.text` 81,502 / `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04` (byte-identical)**.

## Post-landing re-sweep

The whole 620-condition sweep and the 61-site existing-expect flip/strip sweep were re-run on
the landed base (raw 53,749): **no additional size-exact cell wins** (all >= 53,749).  The
landed cell is a local optimum for this sweep.

## Gate

`check` is green: byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact
**4524/4524**, data_exact **843/843**, imports **207**, lint **CLEAN**, drift **0**, symbol
parity **5370/5370** (0 extra / 0 missing).  `process` is still not byte-exact, so the
byte/linked counts are unchanged; only the `.text` differing-byte count drops by one.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53749 aligned= 18049 size= 57232/57232 insn= 20729/20830
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```
