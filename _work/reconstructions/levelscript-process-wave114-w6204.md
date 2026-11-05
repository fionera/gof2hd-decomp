# LevelScript::process — wave 114 / w6204 (adjacent-statement + declaration-order sweep)

Status: **code change landed, 2026-11-05**. Branch `wave114-w6204`, base master
`d8557f7e2` (wave-113 harvest). Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)   after (landed)
official raw ratio       58.001443         58.023432      (+0.021989)
orig / ours insns        20169 / 20030     20169 / 20032
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
sections                 27                27
file size                2187328           2187328
.text                    0x1337c8          0x1337c8
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26298           0x26298   (exact)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

The raw number is `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False).ratio()*100`
on `tools/verify/verify.py`'s reconciled instruction streams of the linked `.so`
(helper `/dev/shm/w6204/fast.py`, reproduces the master baseline 58.001443 exactly).
All 27 section headers, the exact file size and `.ARM.extab` are unchanged.

## What was applied (4 changes, all size-neutral)

1. **case 157 (supernova finale): declaration order.** `int fighterIndex = 10;` is now
   declared first, before `stationIndex`/`chaserIndex`. The original initialises the
   three escort slots in the order station(12) → chaser(11) → fighter(10) but the
   register allocator gives the *first-declared* slot the `r1`/`sp+64` home; putting
   `fighterIndex` first reproduces the original's `mov.w r9,#imm / mov.w fp,#imm /
   mov.w r1,#imm` + three `moveq` block shape exactly (only the r9/fp immediate
   values stay swapped).
2. **case 92 state 5 (line 3011):** swap `field_0x128 = 50000;` before `field_0x38 = 4;`.
3. **case 92 state 2 (line 3109):** swap `resetCamera(m_pLevel);` before
   `m_pRadar->enabled = 1;`.
4. **case 125 state 2 (line 3953):** swap `m_tempVec = leader->getPosition();` before
   `m_pCamera->setTarget(leader);`.

Changes 2–4 are not obviously "faithful" (the original's block order is the pre-change
order in each case), but the metric is the binary: each one shifts the following
block placement / register allocation toward the original's, and the combination is
additive at exactly 57232 B.

## Measurements (all on the current master baseline)

Adjacent-statement sweeps (same-indent, both lines end in `;`, no control keywords):

| region | pairs | best variants |
|---|---:|---|
| lines 606–1540 (mission-0/1) | 222 | `m_nState = 13;` before `forceUpdate` 58.004975; `setVisible` before `freeze = 0` 58.003532 |
| lines 1540–5406 (case bodies) | 1630 | case-157 chaser/fighter decl swap 58.007861; case-125 setTarget/getPosition swap 58.003532; case-92 field_0x128/field_0x38 swap 58.003532; case-92 radar/resetCamera swap 58.002886 |
| adjacent declaration pairs (whole fn) | 80 | case-157 decls 58.007861; `wreck`/`boss` decls 58.007861 |

Case-157 declaration permutations (6 orders):

| order | % | size |
|---|---:|---:|
| station, chaser, fighter (master) | 58.001443 | 57232 |
| station, fighter, chaser | 58.007861 | 57232 |
| chaser, station, fighter | 58.001443 | 57232 |
| chaser, fighter, station | 58.007861 | 57232 |
| **fighter, station, chaser (landed)** | **58.012836** | **57232** |
| fighter, chaser, station | 58.012836 | 57232 |

Second-round sweep on the fighter-first base (1128 pairs, lines 1540–3999):

| variant | % | size |
|---|---:|---:|
| fighter-first alone | 58.012836 | 57232 |
| + case-125 setTarget/getPosition swap | 58.019900 | 57232 |
| + case-125 swap + case-92 field_0x128 swap | 58.021989 | 57232 |
| + case-125 swap + case-92 radar/resetCamera swap | 58.021344 | 57232 |
| **+ all three (landed)** | **58.023432** | **57232** |
| + all three + case-92 `field_0x38`/`forceUpdate` swap (line 2836) | 58.082328 | **57236 (+4 B)** |
| + case-125 `port->setName`/`field_0x74` swap | 57.179819 | 57224 |
| + case-125 `awake`/`pirate` decl swap | 57.179819 | 57224 |

The best measured variant overall is 58.082328 but it grows the function to 57236 B,
which would change the exact file size / size-exact state, so it was not landed.

## Dead ends (measured)

* **case 91 faithful state machine** (`m_nState = 1; setActive(true); setName(...)` in
  state 0 and `m_nState = 2;` in state 1, matching the original at `0x13ac42`) gives
  57.981542 at 57232 B — 0.0199 raw *below* the landed shape. The current source's
  `m_nState = 2`/`setActive` split emits a closer instruction stream.
* **Prologue accessor order.** The original calls `getMessages`, `getPlayer`,
  `getEnemies` (in that order). Reordering the three initialisers to match gives
  57.138589 at 57324 B; only the master order (enemies, player, messages) is
  size-exact.
* **Main-switch case order.** Reordering the 31 `case` blocks to the original's
  `.text` body order (158,131,64,135,145,142,144,139,69,73,67,70,29,41,80,56,125,92,
  157,40,102,154,24,14,21,42,91,94,105,114,16) gives 57.740627 at 57232 B. Ascending
  source order stays best.
* **Adjacent-statement sweep on the fighter-first base, lines 4000–5406** (502 pairs):
  no variant above 58.012836.

## Proven vs hypothesis

Proven (measured, deterministic — fixed compiler + fixed link order):

* The landed file is size-exact (57232 B); `.text`/`.ARM.exidx`/`.ARM.extab`, the
  27-section count and the exact 2,187,328 B file size are unchanged; the gate passes
  with byte 4500 / linked 4500 / lint CLEAN / drift 0.
* The landed combination beats the master faithful shape by +0.021989 raw.
* The four changes are individually non-negative on the base they were measured on;
  the second-round sweep results were obtained on the fighter-first base.

Hypothesis / not established:

* The remaining ~42 % sequence loss is still global register allocation (`this` in
  `sl` vs the original's reload-from-`sp+88` pattern) and MachineBlockPlacement in
  the shared-tail region, as recorded in the wave-110/111/113 notes. The original
  reuses `sl` as a general scratch register (delta, loop counters, vector pointers)
  while our build keeps `this` in `sl` for the whole function; that is the dominant
  single difference in the instruction multiset.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target drift -j6      # 0 fields
```

## Gate at the end of this pass

```
byte_exact 4500  linked_exact 4500  size-exact 4524/4524  data_exact 843/843
imports 207  lint CLEAN  drift 0  RATCHET PASS
sections 27  file size 2187328  .ARM.extab 0x26298
```
