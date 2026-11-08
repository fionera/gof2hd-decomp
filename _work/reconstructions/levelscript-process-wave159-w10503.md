# LevelScript::process — wave 159 / w10503: case-154 state-9 expect +
# case-16 state-4 increment landed; -4 aligned companion not found

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10503`, base master
`cfde1f5f2` (wave 158).  Scratch `/dev/shm/w10503/`.

## Result (landed)

```
                         before (HEAD)     after (landed)
process raw diffbytes    55,094            55,087            (-7)
process aligned insns    17,129            17,133            (+4)
orig / ours size         57232 / 57232     57232 / 57232     (size-exact)
.text diffbytes          85,372            85,365            (-7)
.ARM.extab diffbytes     1,284             1,284             (neutral)
.ARM.extab size          156,312           156,312           (exact)
.ARM.exidx               0                 0                 **BYTE-IDENTICAL**
.rel.plt                 0                 0                 **BYTE-IDENTICAL**
.rodata / .data / .bss   exact             exact
byte_exact               4507              4507
linked_exact             4507              4507
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
RATCHET                  PASS              PASS
```

The wave-158 w10402 note claimed these two edits were landed, but the source
at `cfde1f5f2` did not contain them (the note-only commit `486e3e511` touched
no `src/` file).  This pass actually lands them.

## 1. Landed source change (2 edits)

| # | line | edit | region |
|---|------|------|--------|
| H9b | 4675 | `if (m_nScriptTimerA >= 2001)` -> `__builtin_expect(..., 1)` | case-154 state-9 completion |
| E14b | 1730 | `m_nState = 5` -> `m_nState++` (state is 4) | case-16 state-4 arm |

Both are semantically identical: H9b is a hint-only branch weight, and E14b
increments a known state value.  H9b changes exactly one instruction
(`0x144af0 ldr r0,[sp,#84]` -> `mov r0,r5`, a global register-allocation
change in the state-9 tail), worth raw -2.  E14b is +4 B in the case-16
state-4 block but repacks the literal pool (two `nop`s at `0x1408b4`
disappear), keeping the function size exact; it is worth raw -5.

## 2. LSDA row table (deliverable)

Decoded with `_work/reconstructions/extab_lsda_decode.py`.  The process
exidx record is **byte-for-byte identical to the pre-change record**: 46
rows, call-site length 324, **182 differing bytes** in both builds.
`.ARM.extab` stays 1,284 differing bytes, size exact 156,312.

| field | original | landed |
|---|---:|---:|
| record length | 348 B | 348 B |
| `lpStart` enc | `0xff` | `0xff` |
| `ttype` enc | `0xff` | `0xff` |
| call-site enc | `0x01` | `0x01` |
| call-site length | 324 | 324 |
| call-site rows | 46 | 46 |
| record differing bytes | — | **182** |

Rows (orig -> ours) are unchanged from the wave-157 note's table.

## 3. The -4 aligned companion search (all measured, full link)

All variants start from the landed build.  None reached aligned <= 17,129
while keeping raw <= 55,087, size 57,232 and extab 1,284.

### 3a. Statement moves in the case-154 state-9 body

Adjacent swaps of the 20 simple statements and "move statement i to
front/end" (40 variants): aligned 17,133..17,168, raw 55,081..55,094,
extab 1,284.  Best neutral: `move_to_front_00` 55,087 / 17,133.
`move_to_end_02` (raw 55,081) is 17,149.  The block already matches the
original instruction-for-instruction, so every reorder breaks a match.

Other moves measured: `timer0_before_aa` 55,092/17,137;
`state_before_timer0` 55,087/17,135; `hud_radar_swap` 55,085/17,139;
`freeze_speed_swap` 55,087/17,135; `vuln_pos_swap` 55,086/17,139;
`enemies0_before_1` 55,086/17,165; `aa_before_cinematic` 55,092/17,135.

### 3b. Different expect placement (case-154 / case-16)

| variant | raw | aligned | extab |
|---|---:|---:|---:|
| w10402 landed | 55,087 | 17,133 | 1,284 |
| expect on `messages[7]->isOver()` (1) | 55,164 | 19,011 | 1,972 |
| expect on `messages[7]->isOver()` (0) | 55,200 | 19,019 | 1,972 |
| `field_0xaa` expect(1) | 55,087 | 17,133 | 1,284 |
| `field_0xaa` expect(0) | 55,166 | 17,139 | 1,967 |
| `field_0xaa == 1` | 55,178 | 17,133 | 1,970 |
| `field_0xaa != 0` | 55,087 | 17,133 | 1,284 |
| state-10 msg expect(1) | 55,204 | 17,199 | 1,288 |
| state-10 dock expect(1) | 55,087 | 17,133 | 1,284 |
| state-10 dock expect(0) | 55,069 | 17,150 | 1,973 |
| state-9 `for` cond expect(1) | 55,200 | 17,281 | 1,972 |
| state-9 body expect(1) | 55,087 | 17,133 | 1,284 |
| c16 isOver expect(1) | 55,087 | 17,133 | 1,284 |
| c16 isOver expect(0) | 55,405 | 17,279 | 1,288 |
| c16 shipgroup expect(1) | 55,087 | 17,133 | 1,284 |
| c16 shipgroup expect(0) | 55,209 | 17,167 | 1,971 |
| reversed `2001 <= timer` expect(1) | 55,087 | 17,133 | 1,284 |
| bool-temp expect(1) | 55,087 | 17,133 | 1,284 |

### 3c. Case-16 `m_nState` forms / cold-region shrink

| variant | raw | aligned | extab |
|---|---:|---:|---:|
| `m_nState = 4; m_nState++;` (folds to `=5`) | 55,092 | 17,130 | 1,284 |
| `m_nState = m_nState + 1` | 55,087 | 17,133 | 1,284 |
| `++m_nState` | 55,087 | 17,133 | 1,284 |
| state-4 `m_nState++` before the `for` (removed) | 55,179 | 17,131 | 1,288 |
| state-4 `m_nState++` after the loop | 55,396 | 17,252 | 1,964 |
| state-4 `m_nState++` after reset | — | — | — |
| cache `enemies` in state-9 | 55,163 | 17,176 | 1,959 |
| cache `KIPlayer *e0/e1` | 55,123 | 17,215 | 2,035 |
| cache `Level *lvl` | 55,087 | 17,133 | 1,284 |
| cache `PlayerEgo *pe` | 55,087 | 17,133 | 1,284 |
| cache `TargetFollowCamera *cam` | 55,090 | 17,148 | 1,284 |
| `player = m_pLevel->getPlayer()` reload | 55,274 | 23,801 | 29,283 |
| `PlayerEgo *pp = m_pLevel->getPlayer()` | 55,185 | 17,135 | 1,966 |
| c16 loop `++i` / `i != n` / cond expect | 55,092 | 17,130 | 1,284 |
| c16 loop cached count | 55,279 | 17,194 | 1,282 |
| case-16 state-3 `m_nState++` | 55,087 | 17,133 | 1,284 |

### 3d. Other levers re-tested on this base

| variant | raw | aligned | extab |
|---|---:|---:|---:|
| E14b reverted (`m_nState = 5`) | 55,092 | 17,130 | 1,284 |
| H9b reverted | 55,089 | 17,132 | 1,284 |
| both reverted (= baseline) | 55,094 | 17,129 | 1,284 |
| case-142 state-0 `m_nState++` (D) | 55,226 | 17,129 | 1,288 |
| case-24 state-1 `m_nState++` (F) | 55,101 | 17,143 | 1,285 |
| case-42 state-0 `m_nState++` (C) | 55,173 | 17,134 | 1,289 |
| C+F | 55,104 | 17,141 | 1,285 |
| D + case-154 hud/radar swap | 55,227 | 17,135 | 1,288 |

The wave-157 `CF`/`F` aligned wins (16,983/16,982) do **not** reproduce on
the wave-158 base: the greedy alignment re-syncs differently after the
wave-158 edits.  `D` still gives aligned 17,129 but costs raw +132 and
extab +4.

## 4. Why the +4 aligned is hard to remove

The aligned diff change is exactly 3 original instructions becoming
unmatched: `0x14056a ldr r1,[sp,#88]`, `0x14056c movs r0,#5` (E14b) and
`0x1449fc ldr r0,[sp,#84]` (H9b), plus one greedy shift.  These are the very
instructions the two edits rewrite.  Reverting E14b recovers 3 (->
17,130); reverting H9b recovers 1 (-> 17,132); reverting both is the
17,129 baseline.  No order-preserving statement move, caching shape, or
alternative expect placement in the two blocks recovered the missing
matches while keeping the raw win.

## 5. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10503/fdiff.py _ZN11LevelScript7processEi        # 55,087 / 57,232
python3 /dev/shm/w10503/align_diff.py _ZN11LevelScript7processEi  # 17,133
python3 _work/reconstructions/extab_lsda_decode.py <orig> cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

## 6. Open questions

1. The `0x1449fc ldr r0,[sp,#84]` match is lost because the H9b expect
   keeps `player` in `r5`; forcing a stack reload without adding a call
   would recover it.
2. The `D` lever (case-142 state-0 `m_nState++`) is the only edit that still
   reaches aligned 17,129 on this base; it needs a -132 raw / -4 extab
   companion.
3. The case-154 state-10 block in the original (a 22-iteration `isDead`
   loop at `0x144a14`) is absent from our source; reconstructing it is the
   largest remaining structural gap in this region.
