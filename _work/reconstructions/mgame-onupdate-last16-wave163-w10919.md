# MGame::OnUpdate — 37 -> 16 raw, wave 163 / w10919

Status: **one landed `src/` change** (`src/game/menu/MGame.cpp`).  Worktree
`wt/w10919`, base master `e51db2fc7` (wave-162 docs; gate byte_exact
4507/4524, raw 37 / aligned 109 / 11,440 B at OnUpdate).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Read with `wave162-merge-tests.md` and
`mgame-onupdate-last37-wave162-w10819.md`.

## 1. Baseline re-verified

```
orig 0x17c8d8 11440   ours 0x17c8d8 11440
raw diff 37, aligned (instruction-index) 109
```

The 37 bytes were the mission-122 body (`src/game/menu/MGame.cpp:2811-2815`):

```cpp
Status *st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = st->getCurrentCampaignMission() == 122 ? 3164 : 3166;
this->level->createRadioMessage(28, radioIndex + radioBase);
reinterpret_cast<long long &>(Globals::status->field_0x100) = st->getPlayingTime();
```

The live `Status *st` in r5 made the compiler CSE the condition field load
into r5 and emit `mov r0,r5` at the ccm and at the store tail, where the
original reloads `ldr.w r0,[r9]` (the GOT base `&Globals::status` in r9).
The original source has **no** live `Status*`: every `Globals::status`
access is a fresh `ldr.w rX,[r9]`.

## 2. The landed change: a `Level*` local instead of a `Status*` local

```cpp
int radioIndex = Globals::rnd->nextInt(2);
Level *level = this->level;
int radioBase = Globals::status->getCurrentCampaignMission() == 122 ? 3164 : 3166;
level->createRadioMessage(28, radioIndex + radioBase);
reinterpret_cast<long long &>(Globals::status->field_0x100) = Globals::status->getPlayingTime();
```

Why it works: `this->level` is a **non-rematerialisable load** that is live
across the `getCurrentCampaignMission()` call.  It occupies r5 over the
whole m122 body, so the greedy allocator can no longer split the long-lived
`&Globals::status` value into r5; it keeps it in **r9** for the whole
m23..m122 region (exactly the original).  With the GOT base in r9 the body
reloads `ldr.w r0,[r9]` at the ccm and at the shared store tail, and the
`mov r0,r5` pair disappears.  The `Status *st` local is gone, so the
condition field load also returns to a fresh `ldr.w r2,[r9]`.

| shape | size | raw | aligned |
|---|---:|---:|---:|
| base (wave-162, `Status *st`) | 11,440 | 37 | 109 |
| **LANDED: `Level *level` before the ccm** | **11,440** | **16** | **6** |

Remaining 16 raw bytes (3 runs: 5 + 6 + 5) are all inside the m122 body:

```
original                                   ours
17f192 ldr.w r0,[r9]                       17f192 ldr.w r0,[r9]
17f196 blx   ccm                           17f196 ldr.w r5,[fp,#120]  ; level hoisted
17f19a movw  r2,#3166                      17f19a blx   ccm
17f19e ldr.w r1,[fp,#120]  ; level         17f19e movw  r1,#3166
17f1a2 cmp   r0,#122                       17f1a2 cmp   r0,#122
17f1a4 it    eq                            17f1a4 it    eq
17f1a6 movweq r2,#3164                     17f1a6 movweq r1,#3164
17f1aa add   r2,r4                         17f1aa add   r1,r4
17f1ac mov   r0,r1                         17f1ac mov   r0,r5
17f1ae movs  r1,#28                        17f1ae movs  r1,#28
17f1b0 blx   createRadioMessage            17f1b0 blx   createRadioMessage
17f1b4 b.w   17e26c                        17f1b4 b.w   17e26c
```

The original loads `this->level` **after** the ccm call (into r1, then
`mov r0,r1`); our local is loaded **before** it (into r5).  The load cannot
be sunk across the opaque call, so a source-level `Level *level` local
always places it before the call.  The remaining lever is a construct that
keeps a value live across the ccm (forcing r9) while the level load itself
stays after the call; every such spelling tried either re-splits the GOT
base (size 11,392) or adds instructions.

## 3. Variants measured this wave

All variants replace only the m122 body; `raw` is
`python3 /dev/shm/w10919/fdiff.py _ZN5MGame8OnUpdateEv`; `aligned` is
`/dev/shm/w10919/align_diff.py`.

| variant | size | raw | aligned | note |
|---|---:|---:|---:|---|
| **LANDED: `Level *level` before ccm** | **11,440** | **16** | **6** | GOT base r9, body reloads |
| `Level *level` before `nextInt` | 11,440 | 33 | 8 | load even earlier |
| `Level *level` after the ccm (`L3`) | 11,392 | 5,148 | 597 | level interval misses the ccm; GOT base splits to r5 |
| `Level *const` / `Level &` before ccm | 11,440 | 16 | 6 | same as landed |
| `Level *volatile` before ccm | 11,396 | 5,162 | 600 | stack spill |
| helper `radioBaseFor(Status*)` + level | 11,440 | 16 | 6 | helper inlines, same shape |
| `Status *st` ccm-only, store plain | 11,436 | 914 | 109 | size -4 |
| `Status *volatile st` ccm-only, store plain | 11,440 | 15 | 111 | best raw but volatile stack spill; condition regalloc changes |
| `Status *volatile st` + level | 11,444 | 916 | 131 | size +4 |
| `Status &st` + level | 11,440 | 16 | 6 | reference transparent |
| `Status **statusPtr` + level | 11,392 | 5,148 | 597 | pointer rematerialised |
| `long long radioIndex` / `long long` copy + level | 11,440 | 16 | 6 | 64-bit value folded |
| `long long &last` field alias in body | 11,492 | 5,253 | 761 | stack frame grows |
| `long long &last` field alias in the m122 condition | 11,440 | 854 | 59 | changes m23/m24 blocks |
| `__builtin_expect` on the `== 122` ternary | 11,440 | 16 | 6 | no change |
| `__builtin_expect` on the m122 condition (0/1) | 11,424/11,392 | 4,998/5,141 | 795/636 | layout/size changes |
| `__builtin_expect` on the outer idle guards | 11,344..11,480 | 5,689..9,026 | 1,378..3,240 | layout changes |

## 4. Gate

```
cmake --build cmake-build-match --target check -j6
  verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
  RATCHET PASS      (goal_lint CLEAN, drift 0)
```

Before -> after: byte_exact 4507 -> 4507, linked_exact 4507 -> 4507
(the function is still not byte-exact; the raw residual drops 37 -> 16).
`.text` differing bytes **85,042 -> 85,021** (-21).  `.ARM.extab` 725,
`.rel.plt` 0 (byte-identical), `.note.gnu.build-id` 20, all section sizes
exact, file size 2,187,328 B.  OnUpdate size **11,440 B** exact.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10919
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10919/fdiff.py _ZN5MGame8OnUpdateEv
python3 /dev/shm/w10919/align_diff.py _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j6
```

Scratch (not committed), `/dev/shm/w10919/`: `fdiff.py`, `align_diff.py`,
`rawd.py`, `secdiff.py`, `sweep*.py`, `MGame.base.cpp`, `mir_st.log`,
`mir_plain.log`, `mir_l2.log`, `onupdate_prera.mir`.

## 6. Files changed

* `src/game/menu/MGame.cpp` — m122 body: `Status *st` replaced by a
  `Level *level = this->level` local before the ccm; all `Globals::status`
  accesses are direct again.  Size 11,440 unchanged; raw 37 -> 16,
  aligned 109 -> 6.
* `_work/reconstructions/mgame-onupdate-last16-wave163-w10919.md` (this note).
