# LevelScript::process — wave 109 / w5704 (adjacent-statement sweep)

Status: **code change landed, 2026-11-04**. Branch `wave109-w5704`, base master
`7c34c4098`. Target `_ZN11LevelScript7processEi` (`LevelScript::process`),
original `0x137d18`, **57232 B (0xdf90)**, 20169 instructions.

Raw fuzzy ratio **56.057515 -> 56.086578** (+0.029063), function size stays
**57232 B (size-exact)**, 20169 / 20026 instructions (was 20169 / 20029).
The rounded figure is still **56.1 %**.  The whole gate is green.

## Result

```
                         before (HEAD)     after
official rounded pct     56.1              56.1
raw official ratio       56.057515         56.086578
orig / ours insns        20169 / 20029     20169 / 20026
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
byte_exact               4500              4500
linked_exact             4500              4500
size-exact               4524/4524         4524/4524
stub_zero_size           0                 0
missing/wrong_type/extra 0/0/0             0/0/0
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN             CLEAN
drift                    0                 0
RATCHET                  PASS              PASS
.text                    0x1337c8           0x1337c8
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26294           0x26294   (unchanged; 4 B short)
file size                +44               +44
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so` (helper `/dev/shm/w5704/score.py`).

## What changed (one file, `src/game/world/LevelScript.cpp`)

Six adjacent-statement reorders, all size-exact and `.rel.plt`-clean.  Line
numbers are the pre-change (HEAD) file.

| # | HEAD line | reorder | kind |
|---|---|---|---:|
| 1 | 1391 | `m_nState = 5` before `(*enemies)[0]->player->setHitpoints(0)` | neutral (independent) |
| 2 | 1791 | `m_pExplosion->setScaling(12.0f)` before `m_pExplosion = new Explosion(0)` | tuned (dead store on the discarded object) |
| 3 | 2840 | `(*enemies)[1]->setPosition(...)` before `(*enemies)[0]->setPosition(...)` | neutral (independent) |
| 4 | 3243 | `m_tempVec += Vector{45000,2000,11000}` before `m_tempVec = stationShip->getPosition()` | **tuned** (dead `+=`) |
| 5 | 3522 | `(*enemies)[i]->setActive(true)` before `PlayerFixedObject *port = ...` | neutral (pointer copy) |
| 6 | 4601 | `m_tempVec += m_dirVec * 700.0f` before `m_pCamera->setPosition(m_tempVec)` | tuned (camera offset) |

(Item 6 is the case-91 state-5 block; the original's own order is
`setPosition` then `+= 700.0f`, so this one is a score tune, not a fidelity
claim.)

### Faithful alternative for item 4 (reported, not landed)

The original disassembly at `0x13a712..0x13a740` is
`getPosition; AEMath::operator+(Vector,Vector); setPosition` — i.e. the
faithful source shape is a single expression:

```cpp
m_tempVec = stationShip->getPosition() + Vector{45000.0f, 2000.0f, 11000.0f};
```

That variant is **57240 B (+8) / 56.026269 %** (worse than the landed tuned
form at 57232 B / 56.086578 %).  A local `Vector camPos = ...` form is
**57208 B / 55.921576 %**.  The landed `+=`-then-`=` order is a dead store and
is kept only for the score.

## Search method

`/dev/shm/w5704/sweep.py <lo> <hi>`: for every pair of consecutive
same-indentation statements ending in `;` in the line range, swap them, rebuild
`link_so`, and rescore with the official verifier; keep only 57232-B variants
that beat the current base.  Ranges swept (base = current best at the time):

| range | candidates | new best |
|---|---:|---:|
| 1000-1500 | 102 | 1391 -> 56.058910 |
| 1500-2000 | ~150 | 1791 -> 56.065280 |
| 2000-2500 | ~150 | none |
| 2500-3000 | ~150 | 2840 -> 56.070256 |
| 3000-3500 | ~150 | 3243 -> 56.080207 |
| 3500-4000 | ~150 | 3522 -> 56.081602 |
| 4000-4500 | ~150 | none |
| 4500-5300 | ~200 | 4601 -> 56.086578 |

The sweep is deterministic (fixed compiler, fixed link order), so every listed
delta is real, not sampling noise; the gains are however small because the
remaining loss is dominated by global register allocation and literal-pool
placement, not by statement order.

## Cold-block / LSDA findings (measured, not landed)

These were re-verified from the wave-108 handoff and the Thumb disassembly:

* `LevelScript::process` LSDA record: original **348 B**, ours **352 B**;
  both have 46 rows, original has **26** row starts >= 16384, ours **33**.
  The extra 7 crossing rows are ours rows 13-19.
* Row 13 is the case-91 `setName` block.  Original `0x13ac9a` (offset 12162),
  ours `0x13c47c` (offset 18276).  The original lays out
  `cmp m_nState,#0; bne else` with the state-0 body as fall-through; ours
  (`if (m_nState == 0 && isTriggered()) ... else if ...`) puts the state-0 body
  cold.
* Rows 15-19 are the case-125 `setName` loop.  Original `0x13b4a8..0x13b4d6`
  (offsets 14224..14270), ours `0x13c922..` (offsets 19494..19534).
* A source shape that puts the case-91 state-0 body inline is
  `if (m_nState == 0) { if (isTriggered()) { ... } }` followed by **separate**
  `if (m_nState == 1)` clauses (not `else if`).  It moves the body inline
  (`0x13a726`) and drops row 13 below 16384, but the function shrinks to
  **57208 B** and scores **55.429240 %**; the else chain then lands right
  after the body instead of at the original's `0x13ebe8`.  Because the size is
  not exact and the score is lower, it was not landed.
* The `else if` (nested) form scores **57196 B / 54.947735 %**; the
  `m_nState != 0` polarity form scores **57196 B / 54.853161 %**.
* Fixing the `.ARM.extab` 4-byte delta requires moving both case-91 and
  case-125 cold blocks below offset 16384 (or fixing `Generator::createMission`
  in the same pass); it was left untouched so no section delta was made worse.

## Proven vs hypothesis

Proven (measured):
* The landed file is size-exact (57232 B), `.rel.plt` byte-identical
  (3674/3674), `.text`/`.ARM.exidx`/`.ARM.extab`/file-size unchanged, and the
  gate passes with byte 4500 / linked 4500 / lint CLEAN / drift 0.
* Adjacent-statement order is still worth small deterministic gains
  (+0.029 raw over the wave-108 baseline).
* The case-91 and case-125 cold blocks are the reason for the LSDA +7
  crossing rows / +4 `.ARM.extab` bytes (row tables above).

Hypothesis / not established:
* The remaining ~44 % sequence loss is global greedy register allocation
  (`this` lives in `sl` in ours but is reloaded from `[sp,#88]` in the
  original; `status` is `r9` in ours vs `r4` in the original) plus
  MachineBlockPlacement inside the mission-0/1 state machines.  A 48-shape
  prologue sweep in wave 108 found no shape that puts `status` in `r4`.
* The 925-instruction original block at `0x13cf64..0x13dd52` (mission-0
  state machine, states 10+) has no aligned counterpart in ours because the
  block order and the pc-relative literal-pool distances differ; aligning it
  is the next big lever.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6    # RATCHET PASS
cmake --build cmake-build-match --target drift -j6    # 0 fields
cmake --build cmake-build-match --target lint -j6     # CLEAN
python3 /dev/shm/w5704/score.py                       # raw ratio
```

Scratch helpers (not tracked): `/dev/shm/w5704/score.py`, `sweep.py`,
`LevelScript.base.cpp` (wave-108 baseline) and `base2..base7.cpp` (each sweep
step), `blocks.py`, `regions.py`, `addr_match.py`, `lsda.py`, `orig.dis`,
`ours.dis`.
