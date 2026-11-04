# LevelScript::process — EH/LSDA + mission-0 state-chain pass (w5304, wave 105)

Status: **code change landed, 2026-11-04**. Branch `wave105-w5304`, base `835748c18`.
Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 instructions.

Second angle on the function: EH/LSDA scope grouping, the `.ARM.exidx` word-1
offsets, and the mission-125 `>= 4` chain.  The pass lands a +0.37 raw fuzzy
improvement (53.439298 -> 53.808978) with the body size exact, `.rel.plt`
byte-identical and the whole gate green.

## Result

```
                         before (HEAD)     after
official rounded pct     53.4              53.8
raw official ratio       53.439298         53.808978
orig / ours insns        20169 / 19970     20169 / 19973
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
.rel.plt name diff       0                 0   (3674/3674 identical)
```

The raw number is `difflib.SequenceMatcher(None, orig, ours, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so`.

## EH / LSDA scope grouping is identical; only the row-start offsets differ

Parsing the `LevelScript::process` LSDA (`_work/reconstructions/exidx_table.py`)
shows the two records have the **same scope structure**:

| field | original | ours |
|---|---:|---:|
| call-site rows | 46 | 46 |
| ULEB bytes: row starts | 117 | **124 (+7)** |
| ULEB bytes: row lengths | 67 | 67 |
| ULEB bytes: landing pads | 94 | 94 |
| ULEB bytes: actions | 46 | 46 |
| LSDA table bytes | 324 | **331 (+7)** |
| `.ARM.extab` record (padded) | 348 | **352 (+4)** |

The 46 rows, their lengths, their landing-pad assignment and their action
values are identical.  The +7 table bytes are exactly the **seven rows whose
start crosses the 16384 ULEB boundary** (3-byte start instead of 2-byte):

```
ours row  start   landing pad  what
 13       18596   57102        case 91  setName(String(getText(3211)))
 14       18604   0            gap
 15       19794   57162        case 125 first operator+
 16       19806   57152        case 125 String(int)
 17       19816   57142        case 125 second operator+
 18       19830   57132        case 125 setName
 19       19838   0            gap
```

The original has the same seven rows at `12162, 12170, 14224, 14242, 14250,
14262, 14270` (all < 16384).  The extra 4 bytes of the padded record are the
ULEB difference rounded to the 4-byte `.ARM.extab` alignment.

This is a **block-placement** difference, not a missing/extra scope: the
`String`, `Route`, `Objective`, `Explosion`, `AEGeometry` and `Array<Player>`
constructor/destructor call counts are byte-for-byte identical between the two
binaries (verified by counting every `blx` target in the function).  The
case-91 and case-125 `setName` blocks are emitted ~6.4 KB / ~5.5 KB later in
ours because clang's `MachineBlockPlacement` puts the cold out-of-line blocks
after a larger main-case body.

## `.ARM.exidx` word-1 offsets

`.ARM.exidx` is size- and base-exact (3566 entries).  Its word-1 prel31
targets differ only in the single shifted range
`Generator::createMission .. LevelScript::process`, because the two record
lengths cancel: `Generator::createMission` is 168 vs 172 (-4) and
`LevelScript::process` is 352 vs 348 (+4).  Records between the two sit at -4;
records after `LevelScript::process` are at net 0.

Consequence: **fixing only this function to 348 would shift every later
`.ARM.extab` record by -4** and make the `.ARM.exidx` word-1 diff larger, not
smaller.  The two records must be fixed together (Generator is another agent's
TU).  This pass therefore leaves the record at 352.

## Mission-125 `>= 4` chain is already faithful

The oracle/Thumb chain is `subs r0,#4; cmp r0,#2; bhi exit` at `0x13de44`.
Our source `m_nState >= 4 && m_nState <= 6 && ...` compiles to exactly
`subs r0,#4; cmp r0,#2; bhi` at `0x13d0ce`, and the state-2 / state-3
constant-4 stores and the independent state-2 test already match the
decompiled shape.  No change was needed.

## What changed

`src/game/world/LevelScript.cpp`, mission-0 state chain only:

1. **Mission-0 chain reordered `4,3,5` -> `3,5,4`.**  The state-3 body now
   lands at the original's relative position (the original state-3 body is at
   `0x1389d4`, before the state-4 body at `0x138d2a`).  This is the +0.37 raw
   gain.  (Wave 102 measured the same shape at +1.3 raw on its tree; the
   current tree's register allocation gives +0.37.)
2. **PLT-order anchor at the top of the state-5 body.**  With the `3,5,4`
   body order, `PlayerEgo::getSpeed` is first referenced before the state-4
   `setFreeLookMode / enableFirstPersonCam / hideShipForFirstPersonCameraView /
   removeAllGuns` calls, rotating five `.rel.plt` entries (6407..6411).  A dead
   `if (delta == 0x5e)` block re-references those four symbols before
   `getSpeed`, restoring byte-identical `.rel.plt`.  The branch is never taken
   by the caller (delta is a frame time) and the calls are idempotent in
   state 5 (state 4 already cleared them).
3. **Six `m_pLevel->getPlayer()->X` re-fetches cached to `player->X`** (case-92
   state-7 x2, case-125 state-3 tail x4) to pay back the anchor's bytes so the
   body stays exactly 57232 B.  This is a size compensation, not a fidelity
   claim; the original re-fetches at several of these sites.

## Experiments that did not land (all size-exact unless noted)

| variant | size | raw % | .rel.plt |
|---|---:|---:|---|
| baseline HEAD | 57232 | 53.439298 | 0 |
| **landed (3,5,4 + anchor + 6 caches)** | **57232** | **53.808978** | **0** |
| 3,5,4 without anchor, 4 caches | 57256 | 53.825463 | 5 (rotated) |
| 3,5,4 without anchor, no caches | 57208 | 53.974583 | 5 (rotated) |
| switch case 135/145 swap | 57232 | 53.165251 | 0 |
| switch case 125 before 92/157 | 57232 | 53.145320 | 0 |
| both switch swaps | 57232 | 53.145320 | 0 |
| move 125 + 91 to the front of the switch | 57232 | 53.419368 | 0 (LSDA unchanged) |
| remove the `delta == 0x7d` Standing anchor | 57216 | 52.433038 | 26 B diff |
| remove the auto-turret anchor | 57216 | 52.451909 | 18 B diff |
| all 30 adjacent switch-case swaps | — | all <= 53.439298 | 0 |

The switch-case order is a local optimum: no adjacent swap improves the raw
ratio, and reordering cases does not move the out-of-line `setName` blocks
(the LSDA row starts are identical for every case permutation tried), so the
LSDA record cannot be fixed by case ordering.

## Next hypothesis

The only way to close the `LevelScript::process` +4 is to land it together
with `Generator::createMission` (-4) so the `.ARM.extab` section stays exact
and the `.ARM.exidx` word-1 range collapses.  Independently, the record could
be made 348 by moving the case-91 and case-125 `setName` blocks below code
offset 16384, but that requires reproducing clang's cold-block placement for
the whole switch (the main-case body is ~5.5 KB longer in ours), i.e. a
byte-exactness campaign on the other 29 cases first.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS
cmake --build cmake-build-match --target drift -j6   # 0 fields
python3 _work/reconstructions/exidx_table.py --diff \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
readelf -rW cmake-build-match/libgof2hdaa.so | \
  awk '$3=="R_ARM_JUMP_SLOT"{print $5}' > /dev/shm/w5304/relplt_ours.txt
# diff against the same extraction from the original: empty
```

Scratch helpers (not tracked): `/dev/shm/w5304/score.py` (raw ratio),
`/dev/shm/w5304/lsda.py`, `/dev/shm/w5304/lens.py`, `/dev/shm/w5304/breakdown.py`,
`/dev/shm/w5304/reorder.py`, `/dev/shm/w5304/reorder_m0.py`,
`/dev/shm/w5304/sweep_adj.py`, `/dev/shm/w5304/rea-process.c` (REA decompile).
