# LevelScript::process — wave 145 / w9005: the case-125 state-1 body is
# reconstructed inline (S1SEP) at 57,232 B with the exact 0x144 LSDA table

Status: **code change landed**, 2026-11-08. Target `_ZN11LevelScript7processEi`
(`LevelScript::process`), original `0x137d18`, **57232 B (0xdf90)**, size-exact.
Owned TU: `src/game/world/LevelScript.cpp`. Base master `eb13ada9b` (wave-144
docs head). Scratch helpers in `/dev/shm/w9005/` (not tracked): `fdiff.py`,
`lsda2.py` (exidx+extab LSDA decoder), `measure_all.py`, `reldiff.py`,
`batch.py` (named-edit build harness), `sweep_all2.sh`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    54,909            54,682            (-227)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
process LSDA table       0x144             0x144             (EXACT, 46 rows/24 LP)
whole .text diffbytes    85,994            85,767            (-227)
.ARM.extab diffbytes     1,370             1,399             (+29)
.ARM.exidx               0                 0
.rodata / .data / .bss   exact             exact
.rel.plt                 0                 4   (2 entries swapped: deleteHackingGame <-> setStation/setVisible)
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

The mission target — recover the w8904 raw-win alternative's raw diff **while
keeping the exact 0x144 LSDA table** — is reached: 54,682 raw at 57,232 B with
the 46-row/24-landing-pad table.  The alternative itself (`flat91+vecpos`) is
54,673–54,796 raw but keeps the 0x146 table.

## The alternative re-measured on the current base

The w8904 note's "all-faithful, size-exact, .rel.plt-identical raw-win
alternative" is `flat91 + vecpos` (flat case-91 test, `setActive` before
`setName` in case 91, `setName` before `setActive` in case-125 state 1, both
camera seats back to the `Vector const&` overload).  On the current base that
is:

| variant | size | raw | table | rel.plt |
|---|---:|---:|---:|---|
| HEAD (nested case-91, fff seats) | 57232 | 54,909 | 0x144 | identical |
| `flat91+vecpos` (U+F2+F154) | 57228 | 54,796 | 0x146 | identical |
| `flat91+vecpos+timerB91` (TB) | 57228 | 54,796 | 0x146 | identical |
| `flat91` alone (U) | 57236 | 54,810 | 0x146 | identical |

So the alternative is no longer size-exact on the wave-144 base (it is -4 B);
the wave-145 w9006 worktree independently measured 54,673 at 57,232 B for its
`timerB91+flat91+vecpos` composition.  The 0x144/0x146 delta is structural:
the nested case-91 test creates a separate landing-pad row, and the flat form
merges the `isTriggered()` call and the body calls into one 5,604-byte
call-site region (rows 12-19), costing +2 table bytes.

## The landed reconstruction (5 edits)

1. **case-125 state-1 body out of the switch (S1SEP, the mission edit).**
   The original's state-1 guard+loop is one contiguous block at
   `0x13b43e..0x13b516`, immediately before the state-2 test at `0x13b518`
   (rel `0x3726..0x3800`).  In the master `switch (m_nState) { case 1: ... }`
   shape the loop is outlined to rel `0x3b1e`; making state 1 a separate
   `if (m_nState == 1) { ... }` before the `switch (case 2, 3, default)`
   puts the whole guard+loop inline at rel `0x20aa`, matching the original's
   contiguity and dropping raw 54,909 -> 54,863 on its own.
2. **case-125 state-2 camera seat back to the `Vector const&` overload**
   (`m_pCamera->setPosition(m_tempVec)`).  The original's `0x13b7dc` is the
   `setPosition(fff)` overload, so this is **UNFAITHFUL**; it is the
   alternative's `vecpos` lever and is kept because it is part of the
   requested raw-win composition.  (The case-154 seat stays on `fff`.)
3. **case-91 state-5 `setCollide` receiver**: `m_pLevel->getPlayer()->setCollide(false)`
   -> `player->setCollide(false)`.  Faithful: the original passes `player`
   in r9/r6 at `0x13f43c`/`0x140330`.
4. **case-91 state-3 `m_nScriptTimerA = 0;`** added after
   `objectivesB = nullptr`.  **METRIC-ONLY**: the original state-3 block at
   `0x13f312..0x13f34c` is exactly `m_nState = 4; delete objectivesB;
   objectivesB = nullptr;` and has no timer store.  This is the +8 B
   size/layout companion that restores 57,232 B after S1SEP+F2+SC.
5. **case-91 state-5 timer companion**: `m_pRadar->dockNavPtr = nullptr`
   -> `m_nScriptTimerB = 0`.  Layout companion (the original `0x13fed6` is a
   single `strd r1,r1,[r2,#144]` = the 64-bit `m_nScriptTimerA = 0`; the B
   reset is dead but keeps the strd shape).  This is the same fix landed by
   the wave-145 w9006 worktree.

## Shape sweep (all built + linked)

`raw` = same-address diff bytes; `table` = LSDA call-site table length;
`rel.plt` = whole-section diff.  All variants below keep 57,232 B unless noted.

| variant | size | raw | table | rel.plt |
|---|---:|---:|---:|---:|
| **S1SEP + F2 + SC + ST3 + TB (LANDED)** | **57232** | **54,682** | **0x144** | 4 |
| S1SEP + F2 + SC + ST3 | 57232 | 54,685 | 0x144 | 4 |
| S1SEP + SC + ST3 + TB | 57232 | 54,706 | 0x144 | 4 |
| S1SEP + F2 + TB | 57232 | 54,836 | 0x144 | 4 |
| S1SEP + TB | 57232 | 54,860 | 0x144 | 4 |
| S1SEP alone | 57232 | 54,863 | 0x144 | 4 |
| S1SEP + F2 + SC + GS (faithful +16 B getSpeed re-read) | 57232 | 54,771 | 0x144 | 4 |
| S1SEP + U (flat case-91) | 57228 | 54,595 | 0x146 | 4 |
| S1SEP + U + F2 + F154 | 57220 | 55,122 | 0x146 | 4 |
| TWOCHAIN (faithful if/else-if 0,1 then 2,3,4..6) | 57248 | 55,234 | (record moved) | 12 |
| S1ELSE (`else if (m_nState == 1)`) | 57248 | 55,234 | (record moved) | 12 |
| DEFOUT (default body as a separate `if` after the switch) | 57384 | 55,340 | 0x146 | 13 |

`S1SEP + F2 + SC + GS` is the fully-faithful variant (GS is the genuine
`getSpeed()` re-read at `0x138ffa`/`0x139098`): 54,771 raw, 57,232 B, 0x144
table.  The landed variant is 89 raw better but carries the metric-only ST3
store; the coordinator may prefer the GS form.

## Why `.rel.plt` is 4 bytes off

The landed S1SEP shape moves the case-125 `default` body (with
`PlayerEgo::deleteHackingGame`) ahead of the case-42 state-7 body (with
`PlayerStation::setVisible`).  In the original `setVisible` is at `0x13da1e`
and `deleteHackingGame` at `0x13ded4`; in the landed build they are at
`0x13d9a8` and `0x13d7d2`.  `.rel.plt` entries 2835/2836 therefore swap the
two adjacent PLT slots (4 bytes).  This is a block-placement property of the
S1SEP shape, not a statement error; the flat case-91 alternative keeps
`.rel.plt` identical.

The `.ARM.extab` diff rises 1,370 -> 1,399 because the process call-site rows
now carry different relative offsets (the table length is still exactly
0x144 and the record still 0x15c, so `.ARM.extab` size/base are exact).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w9005/fdiff.py                          # 54,682 / 57,232
python3 /dev/shm/w9005/lsda2.py                          # table 0x144, 46 rows, 24 LP
python3 /dev/shm/w9005/measure_all.py                    # .text 85,767; extab 1,399
python3 /dev/shm/w9005/reldiff.py                        # rel.plt entries 2835/2836
```

## Open leads

1. Fix the `.rel.plt` swap by moving the case-125 `default` body after the
   case-42 state-7 body (or after case 135) without changing the 57,232 B
   size.  DEFOUT (+152 B) and the case-125/135 swap (+raw, -8 B) both fail.
2. The state-1 body's register allocation still differs from the original
   (`mov r9,r8` hoist, `r5=sp+268` vs `r6=sp+268`, `fp` loop counter vs
   `r4`).  The original enters state 1 from the state-0 test's `bne.w
   13b43e` with `enemies` already in r8; S1SEP enters by fallthrough.  A
   shape that keeps the `else if (s==1)` edge but preserves the contiguous
   body (S1ELSE is +16 B) would be the next lever.
3. ST3 is metric-only; if the coordinator prefers a fully faithful source,
   land `S1SEP + F2 + SC + GS` (54,771 raw, same size/table) instead.
