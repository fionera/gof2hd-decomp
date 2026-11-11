# Radar::draw wave 175 (w11905) — campaign-in-`sl` re-search on the wave-174 base

**Status:** no source change landed, 2026-11-11. Worktree `wt/w11905`, branch
`wave175-w11905`, base `master` `5f599a137`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave174-w11907.md` (weights, force-vreg experiment) and
`radar-draw-wave174-w11909.md` (landed `int missionBlocksPlanets` + frame locals).

## 0. Baseline (rebuilt on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 6,817 |
| instruction-index alignment | 1,602 |
| instructions | 3,536 ours / 3,532 orig |
| whole-`.so` `.text` differing bytes | 80,439 |
| `.ARM.exidx` / `.ARM.extab` differing | 0 / 750 (sizes/base exact) |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4,507 / linked 4,507 / size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370) |

Note: the wave-175 brief quotes `.text` 80,447; the measured base is **80,439**
(the wave-174 merge commit `5f599a137` says `80,897 -> 80,439`).

**Build cost is ~1 s per variant.** `cmake --build cmake-build-match --target
link_so -j2` only recompiles `Radar.cpp` and relinks, so this pass used
*authoritative* clang builds for every cell instead of the patched-`llc`
screening harness the wave-174 notes used (`llc` gives raw≈7,237 / al≈2,049 on
this base and does not reproduce clang's allocation).

## 1. The residual, restated with machine vregs

`RA_TRACE=1` on the current base (patched LLVM 7.0.2 `llc`, `ra.log`):

```
select %117 size=32408 w=9.128101e-05 stage=1
  try %117 cand=$r10 intf=1 kind=1 i0=%411(w=5.951397e-05)
  try %117 cand=$r11 intf=1 kind=1 i0=%2471(w=6.574589e-05)
  assign %117 -> $r11 (tryEvict)
  FINAL %117 -> $r11
...
select %2471 size=33702 w=6.574589e-05 stage=1
  try %2471 cand=$r10 intf=1 kind=1 i0=%411(w=5.951397e-05)
  try %2471 cand=$r11 intf=1 kind=1 i0=%117(w=9.128101e-05)
  assign %2471 -> $r10 (tryEvict)
  FINAL %2471 -> $r10
...
select %411 size=20704 w=5.951397e-05 stage=1
  try %411 cand=$r10 intf=1 kind=1 i0=%2471(w=6.574589e-05)
  try %411 cand=$r11 intf=1 kind=1 i0=%117(w=9.128101e-05)
  FINAL %411 -> $noreg          # spilled
```

Mapping the machine vregs back to source (`before_ra.txt`):

* **`%411` = `campaign`** (`Radar.cpp:356`), machine `COPY $r0` of the
  `getCurrentCampaignMission()` return. One def + one use (`campaign >= 2`,
  line 533). Original: `12e0f8 mov sl,r0` / `12e38a cmp.w sl,#2`.
* **`%2471` = the shared zero merged with `missionBlocksPlanets`.** Defs at
  slots 240 (`plasmaInRange = 0`, `lockConfirmed = 0`), 864/1072/1280/1488
  (the four `mov 0` landing pads of the mission-type guard) and 1648
  (`mission->getType() != 172`). Uses at 35568/36512 (`if (missionBlocksPlanets)`
  and `|| missionBlocksPlanets`). This is one SSA value spanning slot 240..36512.
* **`%117` = `otherLock` from `Radar.cpp:531`** — the four-way
  `lockedAsteroid != nullptr || lockedGasCloud != nullptr || field_0x1c != nullptr
  || lockedStation != nullptr` chain. Machine defs are the three `t2MOVi 1`
  landing pads plus the `t2MOVCCi` of the `lockedStation` test; the use is
  `t2ORRrr %117, %2497` at `bb.172` = `if (!otherLock && !planetHit && !turretMode)`
  (line 594).

So the cascade is exactly: `%117` (w 9.13e-05) takes `r11`; `%2471`
(w 6.57e-05) takes `r10`; `%411` (w 5.95e-05) can evict neither and spills.

The **original** allocation is the opposite pairing:

```
12e07a  mov.w fp, #0          ; shared zero -> r11
12e086  strb.w fp, [r4,#536]  ; plasmaInRange = 0
12e08c  strb.w fp, [r4,#304]  ; lockConfirmed = 0
12e0d8  mov.w fp, #0          ; missionBlocksPlanets = false -> r11 (same reg)
12e0f8  mov  sl, r0           ; campaign -> r10
12e38a  cmp.w sl, #2
12e38e  str  r0, [sp,#144]    ; otherLock -> spilled
12e3ba  str.w fp, [sp,#112]   ; missionBlocksPlanets -> spilled
```

i.e. the original has `r10 = campaign`, `r11 = zero/mbp`, `otherLock` spilled.
Our base has `r10 = zero/mbp`, `r11 = otherLock`, `campaign` spilled. The
required change is therefore a **three-way rotation**, not just "campaign into
`sl`": `otherLock` must lose `r11` *and* `campaign` must outrank the zero for
`r10`.

## 2. What the wave-174 notes did not try: moving the `otherLock` OR chain

`otherLock`'s weight (9.13e-05) is high because the four-way `||` chain gives it
**four defs**, each contributing its block frequency. Moving the assignment
earlier lengthens its live range and lowers its weight. Moving
`otherLock = ...` (line 531) to *before* `notOtherLock = !otherLock` (line 421)
does flip the allocation:

```
before_notol  raw=9288  al=1975  size=9824  campaign: mov sl, r0
```

and the prologue then matches the original exactly:

```
 2a: mov.w fp, #0          ; shared zero -> r11   (original: mov.w fp,#0)
 36: strb.w fp, [r4,#536]
 3c: strb.w fp, [r4,#304]
...
31a: cmp.w sl, #2          ; campaign in sl       (original: cmp.w sl,#2)
```

But the frame becomes **304** (original 296) and the function is **9,824 B
(-84)**, raw 9,288. So this is the same coupling the wave-174 note found with
the force-vreg experiment (-92 B there): *campaign-in-`sl` is size-short*, and
here it is short by 84 B. **Caveat:** this reordering is also semantically
different (`notOtherLock` would be derived from the OR-chain value instead of
`otherLockFlag`), so it is a probe, not a candidate.

All positions tried for the OR chain (after `int otherLock = otherLockFlag;`,
after `bool landmarkHit`, after `int notTurretMode`, before/after
`int notOtherLock`, before/after `int notTurretMode`) give the identical
`9824 / mov sl,r0`; only "after `notOtherLock`" differs (`9896 / mov fp,r0`).

## 3. Authoritative re-run of the wave-174 campaign-in-`sl` table

All cells below are real `link_so` + `fn_diff` builds on this base
(base = 6,817 / 1,602 / 9,908 / spill). "camp" is the instruction right after
the first `getCurrentCampaignMission` call.

| shape | raw | aligned | size | camp |
|---|---:|---:|---:|---|
| base | **6,817** | **1,602** | **9,908** | `str r0,[sp,#144]` |
| `bool alienOrbit` (from int) (`AOB`) | 6,824 | **1,597** | 9,908 | spill |
| `expect(!alienOrbit,0)` in campaign cond (`E`) | 9,316 | 3,080 | 9,900 | **`mov sl,r0`** |
| `expect(whole campaign cond,1)` (`W`) | 9,464 | 2,436 | 9,912 | **`mov sl,r0`** |
| `expect(planetTargets!=nullptr…,1)` (`L3`) | 9,627 | 2,040 | 9,940 | **`mov sl,r0`** |
| `expect(!lockConfirmed,0)` (`S`) | 6,869 | 1,599 | 9,916 | spill |
| `bool missionBlocksPlanets` (`B`) | 6,893 | 1,622 | 9,912 | spill |
| `int turretMode` (`TM`) | 9,241 | 1,986 | 9,840 | **`mov sl,r0`** |
| `bool notTurretMode` (`NT`) | 9,521 | 1,707 | 9,852 | **`mov sl,r0`** |
| `bool otherLock` (`OL`) | 9,233 | 1,960 | 9,844 | **`mov sl,r0`** |
| `int landmarkHit` (`LH`) | 8,639 | 1,799 | 9,928 | spill |
| `int enemyHit` (`EH`) | 8,662 | 1,826 | 9,932 | spill |
| `int marked` (`MK`) | 8,428 | 1,603 | 9,912 | spill |
| `int alwaysEnemy` (`AE`) | 8,585 | 1,660 | 9,912 | spill |
| `bool stationId` (`SI`) | 8,547 | **1,552** | 9,920 | spill |
| `bool warpGate` (`WG`) | 8,555 | **1,554** | 9,916 | spill |
| `bool last` (`LAST`) | 8,241 | 1,647 | 9,924 | spill |
| `bool inCenter` (`IC`) | 7,770 | 1,605 | 9,912 | spill |
| `bool slotCount` (`SC`) | 7,496 | 1,630 | 9,912 | spill |
| `bool frame` (`FR`) | 6,897 | 1,630 | 9,916 | spill |
| `bool distance` (`DIST`) | 7,138 | 1,616 | 9,916 | spill |
| `bool left` (planet block) | 8,544 | **1,552** | 9,916 | spill |
| `bool cx` (enemy block) | 8,557 | **1,561** | 9,916 | spill |
| `int gasHit` (from bool) | 8,231 | 1,580 | 9,904 | spill |
| `bool wreck` (enemy block) | 7,278 | 1,570 | 9,904 | spill |
| `bool current` (music tail) | 6,933 | 1,606 | 9,792 | spill |
| `bool race` | 6,831 | 1,612 | 9,916 | spill |
| `int isTarget` / `int inSight` / `int navWreck` / `int hasDockNav` / `int candidateWreck` | 6,817 | 1,602 | 9,908 | spill (no-op) |

### 3.1 Campaign-condition shapes (all reverted)

| shape | raw | aligned | size | camp |
|---|---:|---:|---:|---|
| base `campaign >= 2 && …` | **6,817** | **1,602** | **9,908** | spill |
| `2 <= campaign` | 6,817 | 1,602 | 9,908 | spill (identical) |
| `bool planetBlock = …; if (planetBlock)` | 6,817 | 1,602 | 9,908 | spill (identical) |
| `if (!(campaign < 2 \|\| …))` | 6,817 | 1,602 | 9,908 | spill (identical) |
| `campaign >= 2` moved to the end | 9,356 | 2,109 | 9,892 | `mov r9,r0` |
| `campaign >= 2` in the middle | 8,555 | 1,740 | 9,916 | spill |
| `expect(campaign >= 2, 1)` | 8,562 | 1,708 | 9,916 | spill |
| `expect(campaign >= 2, 0)` | 9,707 | 3,317 | 9,988 | spill |

### 3.2 `otherLock` OR-chain shapes (all reverted)

`OL_OLD` = the four-way `||` chain at line 531.

| shape | raw | aligned | size | camp |
|---|---:|---:|---:|---|
| base `a \|\| b \|\| c \|\| d` | **6,817** | **1,602** | **9,908** | spill |
| split into two `\|\|` statements | 6,817 | 1,602 | 9,908 | spill (identical) |
| `lockedStation` first | 6,827 | 1,607 | 9,908 | spill |
| `(a \| b \| c \| d)` bitwise (`OR4`) | 8,876 | 2,908 | 9,908 | **`mov fp,r0`** |
| `(a \| b \| c) \|\| d` | 8,927 | 1,897 | 9,928 | **`mov fp,r0`** |
| `(a \| b) \|\| (c \| d)` | 9,229 | 1,803 | 9,960 | spill |
| `(a \|\| b) \| (c \|\| d)` | 8,656 | 1,826 | 9,932 | spill |
| `expect(a,0) \|\| expect(b,0) \|\| expect(c,0) \|\| d` | 9,189 | 1,710 | 9,900 | **`mov fp,r0`** |
| `expect(a,1) \|\| …` | 8,744 | 2,785 | 9,920 | spill |
| `expect(whole,0)` | 9,189 | 1,710 | 9,900 | **`mov fp,r0`** |
| `expect(d,0)` only | 6,817 | 1,602 | 9,908 | spill (identical) |
| `expect(a,0)` only | 6,824 | 1,654 | 9,908 | spill |
| `int otherLock` direct (drop `otherLockFlag`) | 9,189 | 1,897 | 9,876 | spill |
| `int otherLock = 1; if (…) otherLock = …` | 9,189 | 1,897 | 9,876 | spill |
| ternary form | 9,189 | 1,897 | 9,876 | spill |
| `bool otherLockFlag = 1; if (…) …` (kept) | 6,817 | 1,602 | 9,908 | spill (identical) |

Note `OR4` and the `\|`-chain variants are the only *size-exact* shapes that
move `campaign` into a callee-saved register (`fp`, not `sl`): they kick
`otherLock` out of `r11`, but the zero still wins `r10`, so `campaign` lands in
`r11` instead of `r10`.

### 3.3 Campaign-declaration position (shorter live range ⇒ higher weight)

Moving `int campaign = Globals::status->getCurrentCampaignMission();` later in
the prologue (to shrink its live range and raise its spill weight):

| anchor the decl is moved before | raw | aligned | size | camp |
|---|---:|---:|---:|---|
| `this->enemies = …` (base) | **6,817** | **1,602** | **9,908** | spill |
| `this->transform = …` | 6,894 | 1,607 | 9,908 | spill |
| `bool onPlanetCourse;` | 9,084 | 1,624 | 9,912 | spill |
| first `DrawImage2D` | 9,425 | 1,694 | 9,852 | `ldrd r2,ip,[r4,#76]` |
| `if (this->route != nullptr)` | 8,992 | 1,831 | 9,916 | `mov r8,r0` |
| `bool otherLockFlag;` | 9,473 | 1,926 | 9,836 | `mov r1,r4` |
| `otherLock = …` / `if (campaign >= 2 …)` | 8,746 | 1,747 | 9,900 | `cmp r0,#93` |

`const int campaign` and `(int)` cast are no-ops (6,817 / 1,602 / 9,908).

### 3.4 Authoritative pair search

All pairs of {`E`,`W`,`L3`,`OE0`,`OEB`,`OEW`,`CL2`,`AOB`,`S`,`B`,`TM`,`NT`,`OL`,
`LH`,`EH`,`MK`,`AE`,`SI`,`LAST`,`WG`,`IC`,`SC`,`FR`,`DIST`,`RH`,`OFF`} (325
combos) — only five cells are size-exact **and** campaign-in-`sl`:

| combo | raw | aligned | size |
|---|---:|---:|---:|
| `E+S` | 9,318 | 3,085 | 9,908 |
| `E+BWG` | 9,327 | 3,064 | 9,908 |
| `E+IC` | 9,319 | 3,094 | 9,908 |
| `E+FR` | 9,333 | 3,062 | 9,908 |
| `E+DIST` | 9,328 | 3,064 | 9,908 |

A second pair sweep over {`AOB`,`E`,`W`,`NOEXP`,`OR4`,`BST`,`BWG`,`SI`,`IC`,`FR`,
`S`,`B`,`TM`,`LH`,`MK`,`AE`} (120 combos) found **no** pair with raw < 6,817.
Best raws: `AOB+NOEXP` 6,827 / 1,517 / 9,908, `NOEXP+S` 6,852 / 1,523 / 9,912,
`NOEXP+B` 6,854 / 1,495 / 9,912.

## 4. Other measured cells (all reverted)

### 4.1 Music-tail block (`if (field_0x1bc >= 1) … else …`, line 1261)

| shape | raw | aligned | size |
|---|---:|---:|---:|
| base (expect 1) | **6,817** | **1,602** | **9,908** |
| drop the `__builtin_expect` (`NOEXP`) | 6,820 | **1,500** | 9,908 |
| `expect(…, 0)` | 8,530 | 1,670 | 9,900 |
| swap the `if`/`else` bodies (invert the test) | 6,818 | 1,603 | 9,908 |

`NOEXP` is the single largest *aligned* improvement found (−102) but costs
+3 raw bytes, i.e. `.text` 80,439 → 80,442, so it is not a win under the
raw/`.text` metric. `NOEXP` is a pure hint removal (semantically identical),
so it was also crossed with every `int`↔`bool` local mutation (~50 builds):
the best cells are `NOEXP + int isTarget/navWreck/inSight/hasDockNav/
candidateWreck` at **6,820 / 1,500 / 9,908** and `NOEXP + bool alienOrbit` at
6,827 / 1,517 / 9,908 — still raw-worse than the base.

### 4.2 `int`↔`bool` sweep of every local in `Radar::draw`

~50 single mutations; every size-9,908 cell is raw-worse or raw-neutral. The
notable aligned-only improvements (`bool stationId` 1,552, `bool warpGate`
1,554, `bool left` 1,552, `bool cx` 1,561) are all size-fatal (9,916/9,920).

## 5. Conclusion

* The wave-174 finding reproduces on the wave-174 base: **every size-exact
  campaign-in-`sl` shape is raw-worse than the base**, and every shape that
  moves `campaign` into `sl` *and* leaves the rest of the allocation alone is
  size-short (9,824–9,912).
* The residual is a three-way rotation (`otherLock`→`r11`, zero/mbp→`r10`,
  `campaign`→spill) and the decisive ordering is
  `w(otherLock) 9.13e-05 > w(zero/mbp) 6.57e-05 > w(campaign) 5.95e-05`.
  `campaign` has exactly one def + one use in both binaries, so its weight can
  only be raised by shrinking its live range; every source move that does so
  either keeps it spilled or changes the block layout enough to lose >2,000 raw
  bytes.
* Best measured cells: raw **6,817** (base, unchanged), aligned **1,500**
  (`NOEXP`, raw 6,820), campaign-in-`sl` raw **9,288** (`otherLock` chain moved
  before `notOtherLock`, size 9,824).
* `src/` is left at the base. Gate after the pass: byte 4,507 / linked 4,507 /
  size 4,524 / data 843 / imports 207 / lint CLEAN / drift 0 / parity 5,370,
  `.text` **80,439**, `.ARM.extab` 750, `.rel.plt` 0. `RATCHET PASS`.

## 6. Reproduction

```sh
ulimit -v 12000000
# base
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
```

Scratch (all under `scratch/w11905/tmp/`):
`mkll.sh`/`mkobj.sh` (clang → .ll → patched llc), `eval_fast.py`,
`eval_obj.py`, `regcheck.py`, `viewdiff.py`, `vd2.py` (aligned diff that
normalises `bl`/`blx` to the callee name — `fn_diff.py`'s aligned metric does
**not** — it normalises branch targets to instruction indices but leaves the
callee address alone, so ours (`bl 0 <sym>`) and the original
(`blx <addr> <sym@plt>`) never compare equal and every call site counts as a
mismatch; the aligned numbers here are therefore uniformly ~30 higher than a
callee-normalised count), `fullsweep.py`,
`textdiff.py` (`.text`/`.ARM.exidx`/`.ARM.extab` differing-byte counter),
`asweep.py` (authoritative single/pair/triple sweep), `pairbest.py`,
`sweep_ol.py`, `sweep_ol2.py`, `sweep_olf.py`, `sweep_olmove.py`,
`sweep_camp.py`, `sweep_types2.py`, `sweep_music.py`, `movesweep.py`,
`before_ra.txt`, `ra.log`, `radar.ll`, `vd2_base.txt` (the 26 MB
`mi.txt` machine-instruction dump was deleted after use).
