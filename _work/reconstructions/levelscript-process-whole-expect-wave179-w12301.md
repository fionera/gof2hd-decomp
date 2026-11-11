# LevelScript::process — wave 179 / w12301: whole-condition expect re-sweep on the w12203 base

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12301`, branch
`wave179-w12301`, base master `64014ce6f` (wave-178 merge).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w12301`.

This is the whole-condition `__builtin_expect` sweep requested after wave 178.
The wave-178 sweep (`levelscript-process-whole-expect-wave178-w12201.md`) ran on
the 53,507 base, but its three landed cells were then superseded by w12203
(`23928e085`), a pure statement reorder that took the function to **53,082**.
The statement reorder changes the block layout, so every whole-condition cell
was re-measured from scratch on the new base.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    53,082           53,068     (-14)
process aligned insns    20,149           20,188     (+39)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,729
.text diffbytes          77,936           77,922     (-14)
.ARM.extab diffbytes     700              700        (size/base exact)
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

Four `__builtin_expect` hints in `LevelScript::process`.  They are
semantics-preserving branch-weight hints, each with the same-line
`// lint: arm_builtin` waiver.

1. **Line 1446 — mission-0 state-2 post-intro timer guard (size-exact,
   raw 53,086 alone).**

   ```cpp
   // before
   if (m_nState == 2 && m_nScriptTimerA >= 2000) {
   // after
   if (__builtin_expect((m_nState == 2 && m_nScriptTimerA >= 2000), 0)) { // lint: arm_builtin
   ```

2. **Line 3156 — mission-0 states 9/10 arm, outer hint on the already-hinted
   `||` condition (size -4 B, raw 53,129 alone).**

   ```cpp
   // before
   } else if (__builtin_expect(m_nState == 9, 0) || __builtin_expect(m_nState == 10, 0)) { // lint: arm_builtin
   // after
   } else if (__builtin_expect((__builtin_expect(m_nState == 9, 0) || __builtin_expect(m_nState == 10, 0)), 0)) { // lint: arm_builtin
   ```

3. **Line 4289 — mission-0 gas-cloud sparks probe (size-exact, raw 53,079
   alone; the new cell on this base).**

   ```cpp
   // before
   if (((PlayerGasCloud *) (*lvl4280->getGasClouds())[i])->getSparks() != nullptr) {
   // after
   if (__builtin_expect(((PlayerGasCloud *) (*lvl4280->getGasClouds())[i])->getSparks() != nullptr, 0)) { // lint: arm_builtin
   ```

4. **Line 5173 — mission-specific 5700-frame station-departure transition
   (size +4 B, raw 53,123 alone).**

   ```cpp
   // before
   if (m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700) {
   // after
   if (__builtin_expect((m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700), 0)) { // lint: arm_builtin
   ```

Cells 2 and 4 are size-changing in opposite directions (+4 / -4) and cancel to
the exact 57,232 B function size.  Cells 1 and 3 are size-exact.  The individual
raw deltas are not additive (the pair alone is 53,073, the triple 53,070, the
quadruple 53,068).

## Method

### Sweep — full whole-condition sweep (643 conditions x 2 = 1,286 cells)

The wave-176 parser (`levelscript-process-expect-sweep-w11901.py`) was re-run
unchanged on the post-w12203 source.  It finds all `if`/`for` conditions in
`process` and the tail functions (643 total: 620 in `process`, 23 in the tail
functions).  Each condition is wrapped as `__builtin_expect((cond), e)` for
`e = 0` and `e = 1`, the real gold link is rebuilt (`ninja -C
cmake-build-match libgof2hdaa.so -j2`), and the candidate ELF is measured
directly (`.dynsym` address/size, `.text` same-address byte diff, `.rel.plt`
md5).

| scope | conditions | cells | measured | size-exact | better | neutral | worse | size-changing |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| `process` | 620 | 1,240 | 1,240 | 534 | **1** | 383 | 150 | 706 |
| tail functions | 23 | 46 | 34 | 34 | 0 | 34 | 0 | 0 |
| **total** | **643** | **1,286** | **1,274** | **568** | **1** | **417** | **150** | **706** |

12 cells (the six one-line `if (m_pGeometryN)` statements in `render3D`) fail
to build under the sweep's wrapper and are recorded as `FAIL`.  They are tail
functions, so they cannot be landed size-exactly anyway.

The full table is committed as
`levelscript-process-whole-wave179-w12301.tsv` (columns: line, kind, variant,
raw, size, text, relplt, cond).

### Combination search

All combinations below were rebuilt with the real gold link and accepted only
if the final `process` size was exactly 57,232 B and `.rel.plt` stayed
byte-identical (`289b2a069f8504d1e8a3919e43915e04`).

* **Best individual size-exact cell.**  Only line 4289 variant 0 improves:
  **53,079** (-3).  Every other size-exact cell is neutral (53,082) or worse.
  The 417 neutral cells include the comment-only `m_nState == 0x7f` cell.
* **Adding every size-exact cell to 4289:0.**  413 candidate conditions tested
  (all size-exact cells except the fixed one, deduped by line/kind).  No
  addition beats 53,079; the result is either 53,079 (neutral) or worse.
* **Cancelling size-changing pairs.**  The top 6 cells in each size-delta group
  {-4, -8, ..., -60} were paired with the top 6 of the matching +delta group
  (325 pairs).  Best landable pair:

  | pair | raw | size |
  |---|---:|---:|
  | **5173:0 + 3156:0** | **53,073** | **57,232** |
  | 4596:0 + 5016:0 | 53,091 | 57,232 |
  | 5173:0 + 1446:1 | 53,098 | 57,232 |
  | 5315:for:1 + 4561:1 | 53,110 | 57,232 |

  `5164:0 + 1446:1` measures 53,081 at size 57,232 but its `.rel.plt` md5 is
  `07752cde942efd50fdec71192614b8e0` (the call order changed), so it is not
  landable.
* **Adding size-exact cells to the best pair.**  All 413 size-exact candidates
  were added to `{5173:0, 3156:0}` one at a time.  Only line 4289 variant 0
  improves: **53,070**.
* **Adding size-exact cells to the triple.**  All 412 remaining size-exact
  candidates were added to `{5173:0, 3156:0, 4289:0}` one at a time.  Only line
  1446 variant 0 improves: **53,068**.
* **Adding size-exact cells to the quadruple.**  All 412 remaining size-exact
  candidates were added to the landed quadruple one at a time.  No addition
  beats 53,068; the quadruple is a local optimum.
* **Flipping the 66 existing `__builtin_expect` variants.**  No flip is a
  size-exact improvement; the best flips are neutral (53,082).  The existing
  hints are already at their best variants on this base.

## Rejected / not landed

* The 150 size-exact cells with raw > 53,082 (best is 53,083).
* The 417 neutral size-exact cells (no raw movement).
* The 12 `render3D` `FAIL` cells (the sweep wrapper does not build for
  the six one-line `if (m_pGeometryN)` statements).
* `5164:0 + 1446:1` (53,081, size-exact but `.rel.plt` differs).
* Every size-changing pair except `5173:0 + 3156:0` that does not restore the
  exact 57,232 B size or the exact `.rel.plt`.

## Gate

`check` is green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0 missing),
stub_zero_size **0**, missing **0**, wrong_type **0**, extra **0**,
`RATCHET PASS`.  `process` is raw **53,068** / aligned **20,188** / size
**57,232**, `.text` **77,922**, `.ARM.extab` **700** (size- and base-exact),
`.rel.plt` byte-identical (md5 `289b2a069f8504d1e8a3919e43915e04`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12301
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 53068 aligned= 20188 size= 57232/57232 insn= 20729/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=53068 size=57232/57232 text=77922 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
cmake --build cmake-build-match --target lint -- -j2    # CLEAN
```

The sweep driver is the wave-176 script
`levelscript-process-expect-sweep-w11901.py` (run unchanged); the combination
harnesses (`combo_add.py`, `pair_search.py`, `triple_add.py`, `quad_add.py`,
`flip_expect.py`) live in `/home/debian/GalaxyOnFire2/scratch/w12301/`
(scratch, not tracked).
