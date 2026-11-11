# LevelScript::process — wave 178 / w12201: whole-condition expect re-sweep

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12201`, branch
`wave178-w12201`, base master `1b48f990f` (wave-177 merge).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12201`.

Follow-up to the wave-176 whole-condition sweep
(`levelscript-process-expect-wave176-w12001.md`) and the wave-177 rare-guard
passes.  The base is now **53,507** after w12102 (aliases), w12103 (statement
reorders), w12104 (rare-guard expects) and w12105 (case-80 declaration order),
so every whole-condition cell was re-measured from scratch on this tree.

## Result (code win landed)

```
                         base (HEAD)      this wave
process raw diffbytes    53,507           53,488     (-19)
process aligned insns    18,941           18,985     (+44)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,732  20,830 / 20,732
.text diffbytes          79,251           79,232     (-19)
.ARM.extab diffbytes     731              729        (-2, size/base exact)
.rel.plt diffbytes       0                0          (byte-identical)
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
included), from `tools/fn_diff.py`; it is the primary metric.  `.rel.plt` md5
is `289b2a069f8504d1e8a3919e43915e04` before and after.

## The change

Three `__builtin_expect` hints in `LevelScript::process`.  They are
semantics-preserving (branch weights only) and each carries the same-line
`// lint: arm_builtin` waiver.

1. **Line 1446 — mission-0 state-2 post-intro timer guard (size-exact cell,
   raw 53,511 alone).**

   ```cpp
   // before
   if (m_nState == 2 && m_nScriptTimerA >= 2000) {
   // after
   if (__builtin_expect((m_nState == 2 && m_nScriptTimerA >= 2000), 0)) { // lint: arm_builtin
   ```

2. **Line 3156 — mission-0 states 9/10 arm, outer hint on the already-hinted
   `||` condition (size -4 B, raw 53,548 alone).**

   ```cpp
   // before
   } else if (__builtin_expect(m_nState == 9, 0) || __builtin_expect(m_nState == 10, 0)) { // lint: arm_builtin
   // after
   } else if (__builtin_expect((__builtin_expect(m_nState == 9, 0) || __builtin_expect(m_nState == 10, 0)), 0)) { // lint: arm_builtin
   ```

3. **Line 5173 — mission-specific 5700-frame station-departure transition
   (size +4 B, raw 53,543 alone).**

   ```cpp
   // before
   if (m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700) {
   // after
   if (__builtin_expect((m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700), 0)) { // lint: arm_builtin
   ```

The three cells are not additive individually: 1 and 2 are size-changing in
opposite directions (+4 B and -4 B), so together they restore the exact
57,232 B function size, and the pair alone measures **53,490**.  Adding cell 1
(the size-exact 1446 hint) to that pair gives the landed triple at **53,488**.
The final shape is size-exact and `.rel.plt`-identical.

## Method

### Sweep A — full whole-condition sweep (643 conditions x 2 = 1,286 cells)

The wave-176 parser (`levelscript-process-expect-sweep-w11901.py`) was re-run
unchanged on the post-wave-177 source.  It finds all `if`/`for` conditions in
`process` and the tail functions (643 total: 620 in `process`, 23 in the tail
functions).  Each condition is wrapped as `__builtin_expect((cond), e)` for
`e = 0` and `e = 1`, the real gold link is rebuilt (`ninja -C
cmake-build-match libgof2hdaa.so -j2`), and the candidate ELF is measured
directly (`.dynsym` address/size, `.text` same-address byte diff, `.rel.plt`
md5).

| scope | cells | size-exact | neutral (53,507) | worse | better | size-changing |
|---|---:|---:|---:|---:|---:|---:|
| `process` (620 conds) | 1,240 | 535 | 384 | 151 | **0** | 705 |
| tail functions (23 conds) | 46 | 34 | 31 | 0 | **3** | 12 |
| **total** | **1,286** | **569** | **415** | **151** | **3** | **717** |

The three "better" cells are all in tail functions and are **size-changing for
their own function**, so they cannot be landed without breaking the global
size-exact count:

| line | function | variant | raw | own size |
|---:|---|---:|---:|---|
| 5356 | `resetCamera` | 0, 1 | 53,506 | 124 -> 128 |
| 5415 | `skipCutscene` | `for` 0 | 53,506 | 412 -> 416 |

Their process-raw change is a literal-pool side effect of the tail function
size change, not a `process` codegen win.  The wave-177 w12101 note already
rejected the `resetCamera` cell for the same reason.

The full table is committed as
`levelscript-process-whole-wave178-w12201.tsv` (columns: line, kind, variant,
raw, size, text, relplt, cond).

### Combination search

All combinations below were rebuilt with the real gold link and accepted only
if the final `process` size was exactly 57,232 B and `.rel.plt` stayed
byte-identical.

* **Best individual process cells.**  No process cell beats the base
  individually; the best size-exact process cells are the 384 neutral cells at
  53,507 and the best non-neutral size-exact cell is 53,508 (e.g. line 1507
  `wingman->isWingMan()` expect 0).
* **Cancelling size-changing pairs.**  The top 8 cells in each size-delta
  group {-16, -12, -8, -4, +4, +8, +12, +16} were tested as cancelling pairs
  (256 pairs).  Best:

  | pair | raw | size |
  |---|---:|---:|
  | **5173:0 + 3156:0** | **53,490** | **57,232** |
  | 3898:0 + 4561:1 | 53,500 | 57,232 |
  | 5173:0 + 1446:1 | 53,507 | 57,232 |
  | 5315:for:1 + 4561:1 | 53,517 | 57,232 |

* **Adding size-exact cells to the best pair.**  All 420 size-exact process
  cells with raw <= 53,530 were added to `{5173:0, 3156:0}` one at a time.
  Only line 1446 variant 0 improves: **53,488**.  Every other addition is
  53,490 or worse.
* **Adding the remaining size-exact cells.**  The 115 size-exact process cells
  with raw > 53,530 were added to the triple one at a time; the best is 53,513
  (line 1297/1303 `if` expect 1), so the triple is a local optimum.
* **Second-best pair.**  Adding the 35 near-worse size-exact cells (raw
  53,508..53,530) to `{3898:0, 4561:1}` gives 53,500 at best, worse than the
  triple.
* **Extra cancelling size-changing cells on the triple.**  `5057:0` (+8) +
  `4695:1` (-8) is 53,698; `2940:0` (+12) + `2924:0` (-12) is 53,595.  Both are
  size-exact but worse.

## Rejected / not landed

* The three tail cells above (size-changing for `resetCamera`/`skipCutscene`).
* `5173:0 + 1446:1` (53,507, size-exact but neutral).
* `5173:0 + 4561:1` (53,538), `3898:0 + 1446:1` (53,542), and the other
  cancelling pairs in the table above.
* Every size-exact process cell with raw > 53,530 added to the triple.

## Gate

`check` is green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0 missing),
stub_zero_size **0**, missing **0**, wrong_type **0**, extra **0**,
`RATCHET PASS`.  `process` is raw **53,488** / aligned **18,985** / size
**57,232**, `.text` **79,232**, `.ARM.extab` **729** (size- and base-exact),
`.rel.plt` byte-identical (md5 `289b2a069f8504d1e8a3919e43915e04`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12201
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53488 aligned= 18985 size= 57232/57232 insn= 20732/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=53488 size=57232/57232 text=79232 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
cmake --build cmake-build-match --target lint -- -j2    # CLEAN
```

The sweep driver is the wave-176 script
`levelscript-process-expect-sweep-w11901.py` (run unchanged); the combination
harnesses (`apply_cells.py`, `search_add.py`, `search_pairs.py`) live in
`/home/debian/GalaxyOnFire2/scratch/w12201/` (scratch, not tracked).
