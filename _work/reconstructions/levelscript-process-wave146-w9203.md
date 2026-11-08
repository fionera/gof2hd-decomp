# LevelScript::process — wave 146 / w9203: re-land the case-125 S1SEP body
# (S1SEP + F2) at 57,232 B with the exact 0x144 LSDA table

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `31297fadc` (wave-145
docs head; the wave-145 merge fix `71b64dbfe` had dropped w9005's source).
Scratch helpers in `/dev/shm/w9203/` (not tracked): `fdiff.py`,
`measure_all.py`, `reldiff.py`, `regions.py`, `var.py` (named-edit build
harness), `perm42.py` (case-42 arm-order harness), `lsda2.py`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,895            54,869            (-26)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
process LSDA table       0x144             0x144             (EXACT, 46 rows/24 LP)
whole .text diffbytes    85,651            85,625            (-26)
.ARM.extab diffbytes     1,370             1,399             (+29)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 4   (2 entries swapped: deleteHackingGame <-> setVisible)
.note.gnu.build-id       20                20
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The landed change (2 edits, both from w9005)

1. **S1SEP** — the case-125 state-1 body is lifted out of the inner
   `switch (m_nState)` into a separate `if (m_nState == 1) { ... }` before the
   `switch (case 2, 3, default)`.  This makes the state-1 guard+loop one
   contiguous block, matching the original `0x13b43e..0x13b516` immediately
   before the state-2 test at `0x13b518`.  On this base S1SEP alone is
   **57,232 B / raw 54,918**.
2. **F2 (METRIC-ONLY)** — the case-125 state-2 camera seat is the
   `m_pCamera->setPosition(m_tempVec)` `Vector const&` overload instead of the
   witnessed `setPosition(fff)` at the original `0x13b7dc`.  This is the
   w8904/w9005 `vecpos` lever; it is kept because it is the best raw number at
   the exact size/table.

## Why the mission's -213 is stale on this base

The w9005 note measured `S1SEP+F2+SC+ST3+TB` at **54,682 / 57,232** on base
`eb13ada9b` (raw 54,909).  The wave-145 merge fix kept w9002 (case-42
state-4/5 reorder) and w9004 (genuine state-5 `getSpeed` re-read) and dropped
w9005's source.  On the current merged base the same composition is
**57,256 B / raw 55,038** — the two layout companions (ST3 `m_nScriptTimerA=0`
and TB `m_nScriptTimerB=0`) now each add +8 B, and the raw is worse than HEAD.
The S1SEP body itself is size-neutral on this base; only the companions are
not.  Measured subset matrix (all from `/dev/shm/w9203/var.py`):

| variant | size | raw | LSDA | `.rel.plt` |
|---|---:|---:|---:|---:|
| HEAD | 57232 | 54,895 | 0x144 | 0 |
| **S1SEP + F2 (LANDED)** | **57232** | **54,869** | **0x144** | **4** |
| S1SEP alone | 57232 | 54,918 | 0x144 | 4 |
| S1SEP + SC | 57232 | 54,948 | 0x144 | 4 |
| S1SEP + F2 + SC | 57232 | 54,899 | 0x144 | 4 |
| S1SEP + F2 + SC + ST3 | 57240 | 55,272 | 0x144 | 4 |
| S1SEP + F2 + SC + TB | 57248 | 55,149 | 0x144 | 4 |
| S1SEP + F2 + SC + ST3 + TB | 57256 | 55,038 | 0x144 | 4 |

So on the merged base the full w9005 composition is **+24 B and +143 raw**;
only `S1SEP+F2` is a size-exact raw win.  The LSDA call-site table stays the
exact 0x144 (46 rows / 24 landing pads) for every S1SEP subset.

## `.rel.plt` is still 4 bytes off (the open problem)

The swap is exactly the w9005 one: `.rel.plt` entries 2835/2836 are
`PlayerStation::setVisible` / `PlayerEgo::deleteHackingGame` in the original
but reversed in ours.  The PLT order is the first-reference order in the
object, so it needs `setVisible`'s call site **below** `deleteHackingGame`'s:

```
orig : setVisible 0x13da1e   deleteHackingGame 0x13ded4   (state-7 block, then case-125 default)
ours : deleteHackingGame 0x13d7da   setVisible 0x13d9b0   (case-125 default, then state-7 block)
```

The S1SEP shape makes MachineBlockPlacement put the case-125 `default`
(hacking) block immediately after the case-125 state-3 block, i.e. *before*
the case-42 state-7 block; the original puts it immediately after state-7.

### What was swept this wave (all built + linked)

| lever | size | raw | `.rel.plt` | note |
|---|---:|---:|---:|---|
| case-42 arm orders 607/670/706/076/067/760 (S1SEP+F2) | 57232/57228/57228/57220/57220/57224 | 54,869..55,215 | 4/4/24/18/4/18 | order 760 moves setVisible 8 PLT slots early (18) |
| DEFOUT (default as `if` after switch) | 57380 | 55,480 | 4930 | compiler DCEs the whole hacking block |
| DEF_FIRST (default arm before case 2) | 57224 | 55,297 | 4 | hacking still before state-7 |
| SWAP125135 (main-switch case swap) | 57232 | 54,869 | 4 | **byte-identical code**: main-case source order is irrelevant |
| EXPLICIT (`case 4:5:6:` instead of default) | 57532 | 55,315 | 34 | jump table changes |
| S7SEP (state-7 `else if` -> separate `if`) | 57232 | 54,869 | 4 | no codegen effect |
| IN42 (hacking moved into case 42 body) | 57308 | 55,524 | 4 | deleteHackingGame lands after `lookBehind` |
| IN7 (hacking inside state-7 arm) | 57408 | 55,341 | 8 | block still placed late |
| TAIL125 (hacking after main switch) | 57376 | 55,459 | 27 | deleteHackingGame at 0x1457e0 |

The closest structural fix is IN42: it does put `setVisible` (0x13d87e)
before `deleteHackingGame` (0x13f5c2), but the hacking block then lands after
`LevelScript::lookBehind`'s first call (0x13ea18), so the swap moves to
`deleteHackingGame` <-> `lookBehind` (still 2 entries / 4 B).  The original
order is `setVisible, deleteHackingGame, lookBehind`; the block has to land
between state-7 and the mission-91 `lookBehind` call.  Every source-level
placement tried so far either keeps the hacking block before state-7 or
pushes it past `lookBehind`; `MachineBlockPlacement` is not following source
order here.

## Honest metric note

`F2` is **METRIC-ONLY**: the original's `0x13b7dc` is the `setPosition(fff)`
overload, so the `Vector const&` seat contradicts the witnessed asm.  The
faithful size-exact variant is **S1SEP alone (57,232 B / raw 54,918)**; the
coordinator may prefer it.  `S1SEP+F2` is the best raw at the exact size,
LSDA table, and section sizes.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9203/fdiff.py _ZN11LevelScript7processEi   # 54,869 / 57,232
python3 /dev/shm/w9203/lsda2.py                              # table 0x144, 46 rows, 24 LP
python3 /dev/shm/w9203/measure_all.py                        # .text 85,625; extab 1,399
python3 /dev/shm/w9203/reldiff.py                            # rel.plt entries 2835/2836
```

## Open leads

1. Land the hacking block between the state-7 block and the mission-91
   `lookBehind` call without a mission guard: the IN42 shape proves the order
   is fixable but adds +76 B and pushes the block past `lookBehind`.
2. A source shape that keeps the case-125 `default` block *after* state-7
   (the original's layout) at 57,232 B would close both the 4-byte `.rel.plt`
   swap and probably a large part of the raw diff.
3. On this base the full w9005 composition is size-negative (+24 B); if the
   coordinator wants its -143 raw on the w9005 base it must be re-derived
   against w9002/w9004, not re-applied.
