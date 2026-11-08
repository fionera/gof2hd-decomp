# LevelScript::process — wave 152 / w9806: w9706's case-125 else-if composes
# with w9707 at exactly 57,232 B (`.rel.plt` 0); case-125 entry span 88 B

Status: **code change landed**, 2026-11-08.  Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57,232 B (0xdf90)**, size-exact.  Owned TU `src/game/world/LevelScript.cpp`.
Worktree `wt/w9806`, base master `9d491d46a` (wave 151, w9707 landed).
Scratch in `/dev/shm/w9806/`.

## Result (landed)

```
                         HEAD (w9707)      composed        landed
process raw diffbytes    55,320            55,138          55,083
process aligned insns    17,244            17,374          17,396
orig / ours size         57232 / 57232     57232 / 57236   57232 / 57232
.ARM.extab diffbytes     1,327             1,431           1,431
.text diffbytes          87,271            87,034          87,034
.rel.plt                 0                 0               0  **BYTE-IDENTICAL**
.rodata / .data / .bss   exact             exact           exact
byte_exact               4507              4507            4507
linked_exact             4507              4507            4507
size-exact               4524/4524         4524/4524       4524/4524
data_exact               843/843           843/843         843/843
imports                  207               207             207
lint                     CLEAN ✓           CLEAN ✓         CLEAN ✓
drift                    0                 0               0
RATCHET                  PASS              PASS            PASS
```

The mission target is met: w9706's three source edits (case-125 state-0/1
`else-if`, case-42 arm order 0,6,7, case-42 state-0 `m_nState++`) compose with
w9707's landed prologue/faithfulness set at **exactly 57,232 B**, with
`.rel.plt` still byte-identical.

## 1. The three w9706 edits (not previously landed)

The w9706 note (`levelscript-process-wave150-w9706.md`) was committed as a
note only; its source edits were never on master.  Re-applied here on top of
w9707:

1. **case 125** (line ~3923): `if (m_nState == 1)` -> `else if (m_nState == 1)`.
   The original entry at `0x139f9c` is `cmp state,#0; bne.w 13b43e` and the
   state-0 body ends `b.w 13b518` (state-2 test), i.e. an `else-if` pair.
2. **case 42** arm order `6,0,7` -> `0,6,7`.  The original's entry test is
   state 0 first (`0x13c1a4: cbnz r0` then `inAlienOrbit`).
3. **case 42 state-0** `m_nState = 1` -> `m_nState++` (state is 0; the
   original at `0x13feda` is `adds r0,#1; str r0,[r2,#28]`).

w9706's fourth edit (case-154 state-9 `m_nState++`) is already landed by
w9707, so it is not repeated here.

Measured composition without a companion: **57,236 B (+4)**, raw 55,138,
aligned 17,374, `.rel.plt` 0.

## 2. The -4 B companion

The +4 is the interaction between w9706's case-125 block layout and w9707's
prologue accessor order.  On the w9706-only base the prologue order is +12 B
(not the +4 B it is on the w9707-only base); the case-125 else-if changes the
register allocation so the mission-0 body needs four extra bytes.

Swept levers (all built + linked + `.rel.plt`-checked on the composition):

| variant | size | raw | aligned | relplt |
|---|---:|---:|---:|---:|
| composition (no companion) | 57236 | 55138 | 17374 | 0 |
| **landed: case-142 state-0 `m_nState++`** | **57232** | **55083** | **17396** | **0** |
| case-144 state-2 `m_nState++` | 57232 | 55309 | 17363 | 0 |
| case-73 inner switch state-1 `m_nState++` | 57232 | 55318 | 17374 | 0 |
| case-42 state-6 `m_nState++` (inc67) | 57228 | 55028 | 17359 | 0 |
| case-42 state-5 `m_nState++` | 57252 | 55143 | 17699 | 6 |
| case-40 state-1 `m_nState++` | 57224 | 55329 | 17383 | 0 |
| case-40 state-2 `m_nState++` | 57228 | 55024 | 17345 | 0 |
| un-refetch case-154 s9 `getPlayer()` | 57228 | 55076 | 17364 | 0 |
| `expect(field_0xaa,0)` case-154 s0 | 57240 | 55023 | 17361 | 0 |
| un-refetch s9 + `expect(field_0xaa,0)` | 57232 | 55279 | 17371 | 0 |

The landed companion is the case-142 state-0 `m_nState = 1` -> `m_nState++`.
State is provably 0 in that arm, so the two forms are semantically identical;
the original emits `movs r0,#1` there, so this is a **composition lever**, not
a new faithfulness fix (our `++` emits `ldr; adds`).  It is the best raw
metric of the size-exact candidates and leaves `.rel.plt` untouched.

The `case-144 state-2` variant is the best aligned (17,363) but raw 55,309;
the `un-refetch + expect` pair is the only other 57,232/relplt-0 family and is
less faithful (the original re-fetches `getPlayer()` before every
`resetGunDelay`).

## 3. Case-125 entry span: 88 B, not the 98 B target

```
orig: case 125 entry 0x139f9c, next case 92 0x139ffe -> span 98
ours: case 125 entry 0x139d42, next case 92 0x139d9a -> span 88
```

The 10 B gap is structural: the original keeps the **state-0 body inline**
at the case entry (98 B) and outlines the state-1 test at `0x13b43e`; our
`if (s==0 && trig) ... else if (s==1)` form outlines the state-0 body and
inlines the state-1 test (88 B).  The nested `if (s==0) { if (trig) ... }`
form that keeps state-0 inline was measured by w9706 at +196 B (`ei01n`,
aligned 16,817); finding that 196 B is still the open lever.  The span target
is therefore reported as **88 B**, with the entry-table shape (a 88 B cold
block, not the old 1032 B) now matching the original's structure.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9806/fdiff.py _ZN11LevelScript7processEi        # 55083 / 57232
python3 /dev/shm/w9806/align_diff.py _ZN11LevelScript7processEi  # 17396
python3 /dev/shm/w9806/relplt.py                                 # 0
python3 /dev/shm/w9806/casetable.py                              # case-125 span 88
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

## 5. Open questions

1. The state-0 body is still outlined.  The nested `ei01n` shape keeps it
   inline (span ~98) at +196 B; a -196 B companion would land the faithful
   entry span.
2. The prologue `status` register is still `r9` vs the original `r4` (the
   +4 B that w9707 documented).  On this composition the accessor order is
   +12 B, so a source shape that gets `status` into `r4` would remove both the
   +4 and the need for the case-142 lever.
3. `ord076` (the original's literal 0,7,6 test order) still rotates nine
   `.rel.plt` entries; a statement-level reorder in the state-6/7 bodies that
   restores the original first-reference order would let it land.
