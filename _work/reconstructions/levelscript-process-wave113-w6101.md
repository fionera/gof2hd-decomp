# LevelScript::process — wave 113 / w6101 (w5904 move subset on current master)

Status: **code change landed, 2026-11-05**. Branch `wave113-w6101`, base master
`a979ace18` (wave-110 landing + wave-112 report). Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**, 20169 instructions.

## Result

```
                         before (master)   after (landed)
official raw ratio       57.565732         58.001443      (+0.435711)
orig / ours insns        20169 / 20032     20169 / 20030
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
sections                 27                27
file size                2187328           2187328
.text                    0x1337c8          0x1337c8
.ARM.exidx               0x6f70            0x6f70
.ARM.extab               0x26298           0x26298   (exact)
.rodata (ours/orig)      0x41c27 / 0x41b67 0x41c27 / 0x41b67
```

The raw number is `difflib.SequenceMatcher(None, onorm, unorm, autojunk=False)
.ratio()*100` on `tools/verify/verify.py`'s reconciled instruction streams of
the linked `.so` (helper `/dev/shm/w6101/score.py`).  `.ARM.extab` stays
size-exact at `0x26298`; all 27 section headers and the exact file size are
unchanged.  The only differing section bytes remain `.text`, `.ARM.exidx` and
`.note.gnu.build-id`.

## What was applied

The wave-111 note (`levelscript-process-wave111-w5904.md`) lists eight moves
measured on the **pre-wave-110** base `7df93bd5a`.  On the current master the
same moves are not all additive; each was re-measured.  The landed file keeps
the best measured subset (six moves), all size-neutral at 57232 B:

| wave-111 move | location (current master) | landed? |
|---|---|---|
| 1 — swap `setTarget(m_pGeometry1)` (state 0) / `translate(-δ,δ,-12δ)` (state 1) | case 80 | **no** (see below) |
| 2 — `m_pLevel->getPlayer()->isInRocketControl()` → `player->isInRocketControl()` | case 92 state 1 | yes (faithful) |
| 3 — swap `m_pHud->visible = 0` (state 0) / `setTarget(m_pGeometry6)` (state 6) | case 80 | yes (as 3b, without move 1) |
| 4 — swap `setTarget(...)` / `translate(...)` inside state 0 | case 80 | **no** (depends on move 1) |
| 5 — swap `m_pHud->visible = 0` / `m_nScriptTimerA = 0` | case 92 state 1 | yes |
| 6 — swap `*scratch = GetDirVector()` / `*scratch = geometry->getRightVector()` | case 131 state 2 | yes |
| 7 — swap `(*enemies)[0]->setActive(true)` / `m_nState = 2` | case 91 states 0/1 | yes (as 7b) |
| 8 — swap `setTarget(ship)` / `setPosition(m_tempVec)` | case 106 state 2 | yes |

Move 7b is the clean equivalent of wave-111 move 7: state 0 stores
`m_nState = 2` directly (instead of `m_nState = 1; m_nState = 2;`) and state 1
does `(*enemies)[0]->setActive(true)`.  It emits the same 20030-instruction
stream and the same raw ratio as the literal double-store form, with no
redundant store in the source.

Move 3b is wave-111 move 3 applied without move 1: state 0 keeps its
`setTarget(m_pGeometry1)` and swaps only `m_pHud->visible = 0` with state 6's
`setTarget(m_pGeometry6)`.

## Measurements (current master, cumulative unless noted)

| variant | size | insns | raw % |
|---|---:|---:|---:|
| master (faithful) | 57232 | 20032 | 57.565732 |
| move 1 only | 57240 | 20035 | 57.461944 |
| moves 1–2 | 57232 | 20032 | 57.560757 |
| moves 1–3 | 57232 | 20030 | 57.921839 |
| moves 1–4 | 57232 | 20030 | 57.931789 |
| moves 1–5 | 57232 | 20031 | 57.960199 |
| moves 1–6 | 57232 | 20030 | 57.986517 |
| moves 1–7 | 57232 | 20030 | 57.996468 |
| all 8 moves (1–8) | 57232 | 20029 | 57.997910 |
| **landed: 2 + 3b + 5 + 6 + 7b + 8** | **57232** | **20030** | **58.001443** |

Individual moves on the master base:

| move | size | insns | raw % |
|---|---:|---:|---:|
| 2 | 57232 | 20033 | 57.564300 |
| 3b | 57232 | 20030 | 57.926814 |
| 5 | 57232 | 20033 | 57.589175 |
| 6 | 57240 | 20035 | 57.496766 |
| 7 (literal double-store) | 57232 | 20032 | 57.585632 |
| 8 | 57232 | 20031 | 57.567164 |

Other subsets measured:

| subset | size | insns | raw % |
|---|---:|---:|---:|
| 2 + 5 + 6 + 7 + 8 | 57232 | 20032 | 57.635382 |
| 3b + 5 + 6 + 7 + 8 | 57240 | 20033 | 57.892642 |
| 2 + 3b + 5 + 6 + 7 | 57232 | 20031 | 58.000000 |
| 2 + 3b + 5 + 6 + 7 + 8 + 9 | 57232 | 20031 | 57.975124 |
| 2 + 3b + 5 + 6 + 7b + 8 (landed) | 57232 | 20030 | 58.001443 |

Move 9 is the wave-111 note's "highest measured" extra: swap the case-92
state-1 tail `m_nState++` with the state-2 `m_nScriptTimerB = 0` (leaving two
consecutive `m_nState++`).  On current master it lowers the score
(58.001443 → 57.975124), so it was not landed.

A targeted adjacent-statement sweep over the modified regions
(case 106 1343–1380, case 80 2569–2700, case 91 2776–2800, case 92
2883–2930, case 131 4038–4100; 177 consecutive same-indent statement pairs)
found no variant above 58.001443 at 57232 B.

## Why moves 1 and 4 were dropped

On the wave-111 base, move 1 was the first step of a monotone chain.  On
current master it grows the function to 57240 B and drops the raw ratio to
57.461944 on its own; moves 1–4 only recover to 57.931789.  The full 1–8
chain reaches 57.997910, i.e. 0.003533 raw below the landed subset, because
the case-80 cross-arm placement of move 1 is worse than leaving state 0's
`setTarget(m_pGeometry1)` in place.  Move 4 is only meaningful after move 1
(it swaps the two statements move 1 moved into state 0), so both are omitted.

## Proven vs hypothesis

Proven (measured, deterministic — fixed compiler + fixed link order):

* The landed file is size-exact (57232 B), `.text`/`.ARM.exidx`/`.ARM.extab`
  and the 27-section/exact-file-size state are unchanged; the gate passes with
  byte 4500 / linked 4500 / lint CLEAN / drift 0.
* The landed subset beats the master faithful shape by +0.435711 raw and the
  full eight-move chain by +0.003533 raw.
* Moves 2, 3b, 5, 6, 7b, 8 are individually non-negative except move 6, which
  is a size-neutral tune only in combination with 3b/2 (it grows to 57240 B
  alone); the landed combination is size-exact.

Hypothesis / not established:

* The remaining ~42 % sequence loss is still global register allocation
  (`this` in `r9` vs the original's `sl`, `&this->m_pLevel` in `fp` vs `r4`)
  and MachineBlockPlacement in the mission-0/1/78 state machines, as recorded
  in the wave-110/111 notes.  The case-80/91/92/106/131 statement order is now
  locally exhausted for this pass.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS
cmake --build cmake-build-match --target lint -j6       # CLEAN
cmake --build cmake-build-match --target drift -j6      # 0 fields

python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11LevelScript7processEi
```

## Gate at the end of this pass

```
byte_exact 4500  linked_exact 4500  size-exact 4524/4524  data_exact 843/843
imports 207  lint CLEAN  drift 0  RATCHET PASS
```
