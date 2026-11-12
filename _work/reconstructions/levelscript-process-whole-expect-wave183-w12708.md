# LevelScript::process — wave 183 / w12708: whole-condition expect re-sweep on the w12605 base

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w12708`, branch
`wave183-w12708`, base master `9555474d8` (wave-182 harvest 5, raw **52,412**).
Scratch `/home/debian/GalaxyOnFire2/scratch/w12708`.

This pass re-runs the whole-condition `__builtin_expect` sweep requested after
wave 182.  The wave-179 sweep (`levelscript-process-whole-expect-wave179-w12301.md`)
ran on the 53,082 base and landed four cells, but the wave-180 w12403 statement
reorder replaced the body and dropped them; wave 181/182 then permuted the body
to 52,412.  Every whole-condition cell was therefore re-measured from scratch on
the current body, and this pass additionally covers **flip** and **remove** of
every existing hint, plus a cancelling-size pair search and greedy add-on.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    52,412           52,402     (-10)
process aligned insns    22,050           22,051     (+1)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,728  20,830 / 20,729
.text diffbytes          75,015           75,005     (-10)
.ARM.extab diffbytes     646              646        (size/base exact, 156,312 B @ 0x1a3b78)
.note.gnu.build-id       20               20         (unchanged)
total differing bytes    75,681           75,671     (-10)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 0 diff)
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

Three `__builtin_expect` edits in `LevelScript::process` (semantics-preserving
branch-weight hints, each `__builtin_expect` line carrying the same-line
`// lint: arm_builtin` waiver):

1. **Line 1446 — mission-0 state-2 post-intro timer guard (add, hint 0).**

   ```cpp
   // before
   if (m_nState == 2 && m_nScriptTimerA >= 2000) {
   // after
   if (__builtin_expect((m_nState == 2 && m_nScriptTimerA >= 2000), 0)) { // lint: arm_builtin
   ```

   This is the wave-179 cell 1, re-found on the new body (alone it is
   raw 52,416, size-exact; on top of the pair below it is the winning -2).

2. **Line 3147 — mission-0 state-8 camera move timer guard (remove, was hint 1).**

   ```cpp
   // before
   if (__builtin_expect(m_nScriptTimerA >= 8001, 1)) { // lint: arm_builtin
   // after
   if ((m_nScriptTimerA >= 8001)) {
   ```

   Removing the hint is size-changing alone (raw 52,456, size 57,228); it is
   the -4 B half of the cancelling pair.

3. **Line 5177 — mission-specific 5700-frame station-departure transition (add, hint 0).**

   ```cpp
   // before
   if (m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700) {
   // after
   if (__builtin_expect((m_nScriptTimerA >= 5700 && m_nScriptTimerA - delta < 5700), 0)) { // lint: arm_builtin
   ```

   This is the wave-179 cell 4, re-found on the new body (alone it is
   raw 52,456, size 57,236, the +4 B half of the pair).

Cells 2 and 3 are size-changing in opposite directions (-4 / +4) and cancel to
the exact 57,232 B function size; the pair alone measures raw **52,404**
(-8).  Adding cell 1 on top reaches raw **52,402** (-10).  The individual raw
deltas are not additive (the pair is 52,404, the triple 52,402).

## Method

### Add sweep — 643 conditions x 2 = 1,286 cells

The wave-176 parser (`levelscript-process-expect-sweep-w11901.py`) was re-run
unchanged on the current source.  It finds all `if`/`for` conditions in
`process` and the tail functions (643 total).  Each condition is wrapped as
`__builtin_expect((cond), e)` for `e = 0` and `e = 1`, the real gold link is
rebuilt (`ninja -C cmake-build-match libgof2hdaa.so -j2`), and the candidate
ELF is measured directly (`.dynsym` address/size, `.text` same-address byte
diff, `.rel.plt` md5).

| scope | conditions | cells | measured | size-exact | better | neutral | worse | size-changing |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| `process` + tail | 643 | 1,286 | 1,274 | 581 | **0** | 409 | 172 | 693 |

12 cells (the six one-line `if (m_pGeometryN)` statements in `render3D`,
lines 5375-5380) fail to build under the sweep's wrapper and are recorded as
`FAIL`; they are tail functions and cannot be landed size-exactly anyway.

**No single size-exact cell improves the current body** (the wave-179 winners
are now size-changing or worse: 1446:0 is +4, 4293:0 is -8/raw 53,315,
5177:0 is +4/raw 52,456).

### Flip / remove sweep — 67 existing hints x 2 = 134 cells

Every existing `__builtin_expect` in `process` and the tail functions was
flipped (0<->1) and removed (`__builtin_expect(X, e)` -> `(X)`), rebuilt and
measured.  68 of the 134 cells are size-exact; **none** improves the base
(best size-exact raw is 52,412).

### Cancelling-size pair search — 578 pairs

All size-changing cells were grouped by size delta; the top 8 cells of each
delta group {-4,-8,...,-24} were paired with the top 8 of the matching +delta
group (578 pairs), rebuilt and measured.  Four pairs are size-exact and
raw-lowering:

| pair | raw | size | `.rel.plt` |
|---|---:|---:|---|
| **remove:3147 + add:5177:0** | **52,404** | 57,232 | `289b2a06…` (identical) |
| remove:3147 + add:5168:0 | 52,391 | 57,232 | `07752cde…` (differs — not landable) |
| add:5084:0 + add:5168:0 | 52,398 | 57,232 | `07752cde…` (differs — not landable) |
| flip:5084 + add:5168:0 | 52,398 | 57,232 | `07752cde…` (differs — not landable) |

Only `remove:3147 + add:5177:0` keeps `.rel.plt` byte-identical, so it is the
landed pair.

### Greedy add-on — 648 cells, then 647 cells

All 648 remaining size-exact cells were added to the pair one at a time.  Only
line 1446 variant 0 improves: raw **52,402**.  All 647 remaining size-exact
cells were then added to the triple one at a time; no addition beats 52,402,
so the triple is a local optimum.

## Rejected / not landed

* The 693 size-changing cells alone (none is size-exact).
* The 172 size-exact cells with raw > 52,412.
* The 409 neutral size-exact cells (no raw movement).
* The 12 `render3D` `FAIL` cells.
* `remove:3147 + add:5168:0` (52,391, size-exact but `.rel.plt` differs) and
  the other two `.rel.plt`-differing pairs.
* Every other cancelling pair (all raw >= 52,412).
* Every size-exact addition to the landed triple (all neutral at 52,402).

## Gate

`check` is green: byte_exact **4507/4524**, linked_exact **4507/4524**,
size-exact **4524/4524**, data_exact **843/843**, imports **207**, lint
**CLEAN**, drift **0**, symbol parity **5370/5370** (0 extra / 0 missing),
stub_zero_size **0**, missing **0**, wrong_type **0**, extra **0**,
`RATCHET PASS`.  `process` is raw **52,402** / aligned **22,051** / size
**57,232**, `.text` **75,005**, `.ARM.extab` **646** (size- and base-exact),
`.rel.plt` byte-identical (md5 `289b2a069f8504d1e8a3919e43915e04`).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12708
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 52402 aligned= 22051 size= 57232/57232 insn= 20729/20830
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
cmake --build cmake-build-match --target drift -- -j2   # 0 fields
cmake --build cmake-build-match --target lint  -- -j2   # CLEAN
```

The sweep driver is the wave-176 script
`levelscript-process-expect-sweep-w11901.py` (run unchanged); the flip/remove
and combination harnesses (`flipremove.py`, `combo.py`, `runpairs.py`,
`runeutral.py`, `runtriple.py`) and result tables (`cells.tsv`,
`flipremove.tsv`, `pairres.tsv`, `neutralres.tsv`, `tripleres.tsv`) live in
`/home/debian/GalaxyOnFire2/scratch/w12708/` (scratch, not tracked).
