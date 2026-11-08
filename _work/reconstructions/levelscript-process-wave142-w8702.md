# LevelScript::process — wave 142 / w8702: case-125 shape re-sweep on the wave-141 base;
# the w8506 `s1first` win does not reproduce; two independent-call reorders land at raw 54,102

Status: **code change landed**. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `1d10d0694` (wave-141 docs head).
Scratch helpers in `/dev/shm/w8702/` (not tracked): `fdiff.py`, `measure.py`,
`tablediff.py` (per-row uleb width diff), `calls.py` (LSDA invoke -> callee map),
`runs.py` (matching-run map), `gen.py` (case-125 shape generator), `c91.py`
(case-91 statement-order variants), `sweep.sh` / `combo.sh`.

## Result

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,108            54,102            (-6)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
whole .text diffbytes    84,567            84,561            (-6)
.ARM.extab diffbytes     1,395             1,395             (size 156,312 exact)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0 (byte-identical) 0 (byte-identical, 29,392 B)
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The landed edits are two statement-order swaps of **independent** calls (semantically
identical, no call added/removed):

1. **case 91 state 0** (source line 2797): `setName(...)` now precedes
   `(*enemies)[0]->setActive(true)`.  Original Thumb is `setActive` at `0x13ac70`
   then `setName` at `0x13ac9e`, so this is a *layout* reorder, not a semantic
   correction; it moves the case-91 `setName` block 12 bytes earlier (rel
   `0x48ec` -> `0x48e0`) and aligns 5 more same-address bytes.
2. **case 125 state-1 loop** (source line 3926): `(*enemies)[i]->setActive(true)`
   now precedes `port->setName(...)`.  Original Thumb is `setName` at `0x13b4d2`
   then `setActive` at `0x13b4f6`; again a layout reorder of independent calls,
   worth 1 more byte.

Both keep the process LSDA call-site table at `0x146` (limit `0x147`), so
`.ARM.extab` stays exactly 156,312 B and `.rodata` does not move.

## The wave-140 `s1first` win does not reproduce on this base

The wave-140 note (`levelscript-process-wave140-w8506.md`) recorded
`s1first_cases_r2` at raw **54,058** with table `0x148`.  Re-applied to the
current wave-141 base it is **54,156** (table `0x148`, size 57232) — 48 bytes
*worse* than the master switch shape, not 204 better.  The difference is the
wave-141 source edits (w8601 case-40 nest + s4gp, w8604 mission-0, w8606
case-105), which re-roll `MachineBlockPlacement`; the wave-140 number was a
property of the older base, not of the `s1first` statement shape.

## Case-125 shape sweep on the current base

`raw` = same-address diff bytes; `table` = process LSDA call-site table length
(original `0x144`; `<= 0x147` keeps `.ARM.extab` at 156,312).

| variant | size | raw | table | note |
|---|---:|---:|---:|---|
| **master (`switch` 1,2,3,default)** | **57232** | **54,108** | **0x146** | local optimum |
| `chain_nosw` (separate if-chain instead of switch) | 57232 | 54,108 | 0x146 | byte-identical to master |
| `sw_1_3_2`, `sw_1_d_2_3` | 57232 | 54,108 | 0x146 | byte-identical to master |
| `s1first` (state-1 test first, state-0 in `else if`) | 57232 | 54,156 | 0x148 | wave-140 shape |
| `nested` (`if(0){if(msg)} else if(1)`) | 57232 | 54,140 | 0x14b | |
| `twochain` (faithful if/else-if 0,1 then 2,3,4..6) | 57240 | 54,996 | 0x148 | +8 B |
| `twochain_sep` (state-1 as a separate `if`) | 57216 | 55,037 | 0x146 | -16 B |
| `sw_2_1_3` | 57216 | 55,070 | 0x14b | |
| `sw_d_1_2_3` | 57224 | 55,059 | 0x14b | |
| `sw456` (`case 4: case 5: case 6:`) | 57380 | 55,229 | 0x149 | |
| `sw0` (`case 0:` inside the switch) | 57372 | 55,310 | 0x14b | |
| `sw_after` (state-0 `if` after the switch) | 57696 | 55,091 | 0x148 | |
| `s2_faith` (faithful ambush-loop decl order) | 57216 | 55,063 | 0x146 | -16 B |
| `s2_piratefirst` | 57216 | 55,095 | 0x146 | -16 B |
| `s1_lt11` / `s1_pp` / `s1_ne11` / `s1_portfirst` / `s1_nz` | 57232 | 54,108 | 0x146 | byte-identical to master |
| `s1_routeonce` (cache `Route *r` once) | 57216 | 55,060 | 0x146 | -16 B |

The case-125 statement sequence is already a strong local optimum: every
restructure either produces identical bytes or moves cold blocks and raises the
same-address diff.  The state-1 loop body is outlined to rel `0x3aca` in ours
vs `0x376e` in the original, but none of the loop spellings tried (`i < 11`,
`++i`, `i != 11`, hoisted `port`, cached `Route*`) changes that placement.

## The exact-table experiment (case-91 nested) and why it is not landed

The only source shape found that shaves the 2 extra table bytes is the wave-139
case-91 **nested** test:

```cpp
if (m_nState == 0) {
    if (((RadioMessage *) ((*messages)[3]))->isTriggered()) { ... }
} else if (m_nState == 1 && player->isDockedToDockingPoint()) { ... }
```

It moves the case-91 `setName` from rel `0x48ec` to `0x2bfa`, so the process
table becomes exactly **`0x144`** (matching the original byte-for-byte in
length).  But it is **-12 B** (57220) and raw **54,169** (+1,061): the whole
case-91 block moves, shifting every block between `0x2bfa` and `0x48ec`.  It
needs a +12 B companion to restore 57232, and no size-exact combination found
this pass beats master's raw.  Recorded as a lead: if a future pass matches the
case-91 block placement, this is the table-shrink lever.

## Table structure (re-confirmed)

The residual `+2` vs the original is not a width crossing at `0x4000` alone; it
is a **row-alignment** difference:

* ours row 14 is the `String(int)` ctor at `0x3af6` with a landing pad, while
  original row 14 is the gap `0x2f8a..0x3790` (`+1`);
* ours row 17 is the gap `0x3b16..0x48ec` (`-1`);
* ours rows 18/19 start at `0x48ec`/`0x48f4` (3-byte ulebs) because the
  case-91 `setName` block sits after the case-125 String calls, while original
  rows 18/19 start at `0x37b6`/`0x37be` (2-byte ulebs) (`+2`).

Net `+2` -> table `0x146`.  The 24-invoke multiset is identical to the
original; only the block order differs.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8702/measure.py                      # raw / size / table / extab
python3 /dev/shm/w8702/tablediff.py                    # row-by-row uleb width diff
python3 /dev/shm/w8702/calls.py                        # LSDA invoke -> callee
```
