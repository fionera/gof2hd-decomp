# MGame::OnUpdate — the last 37 raw bytes, wave 162 / w10819

Status: **research pass, no `src/` change landed** (the pristine w10718 shape is
kept).  Worktree `wt/w10819`, base master `29248c1d0` (wave-161 docs; gate
byte_exact 4507/4524, raw 37 / aligned 109 / 11,440 B at OnUpdate).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).

Read with `wave161-merge-tests.md` and
`mgame-onupdate-last19-wave160-w10718.md`.

## 1. Baseline re-verified

```
orig 0x17c8d8 11440   ours 0x17c8d8 11440
raw diff 37, 4 runs
aligned (instruction-index) 109
```

Four raw runs:

| run | bytes | what |
|---|---:|---|
| `0x17e2b9` | 1 | m122-124 condition field pointer register (`r2` vs `r5`) |
| `0x17e2bc` | 1 | m122-124 condition `ldrd` base register (`r2` vs `r5`) |
| `0x17f192..0x17f1b4` | 34 | m122 body: anchor `mov r0,r5` shifts the block by 2 |
| `0x17f1b6` | 1 | m122 branch target `17e26c` vs `17e270` |

Exact instructions (`r9` = `&Globals::status`):

```
original                                   ours (pristine w10718)
17e2b6 ldr.w r2,[r9]   ; condition field   17e2b6 ldr.w r5,[r9]   ; condition field
17e2bc ldrd  r2,r3,[r2,#256]               17e2bc ldrd  r2,r3,[r5,#256]
17f192 ldr.w r0,[r9]   ; ccm               17f192 mov  r0,r5       ; anchor
17f196 blx   ccm                           17f194 blx  ccm
...
17f1b0 blx   createRadioMessage            17f1ae blx  createRadioMessage
17f1b4 b.w   17e26c                         17f1b2 mov  r0,r5
                                           17f1b4 b.w   17e270
17e26c ldr.w r0,[r9]   ; shared tail       17e26c ldr.w r0,[r9]   ; used only by m23
17e270 blx   getPlayingTime                 17e270 blx   getPlayingTime
```

The source statement is the mission-122 body (`src/game/menu/MGame.cpp:2811-2815`):

```cpp
Status *st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = st->getCurrentCampaignMission() == 122 ? 3164 : 3166;
this->level->createRadioMessage(28, radioIndex + radioBase);
reinterpret_cast<long long &>(Globals::status->field_0x100) = st->getPlayingTime();
```

## 2. Why the anchor is needed (MIR, re-derived this wave)

The `-mllvm -print-after-all` pre-RA MIR (scratch `onupdate_prera.mir`) shows
the m23-124 region starts at `bb.206`:

```
%983:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6statusE
%984:gpr  = t2LDRi12 %983, 0        ; &Globals::status  (long interval)
%985:gpr  = t2LDRi12 %984, 0        ; Globals::status pointer (anchor)
```

`%984` is the long-lived `&Globals::status` value.  The anchor `%985` (the
`st` load) is defined inside `%984`'s interval and takes `r5`; the greedy
allocator therefore gives `%984` `r9`.  Without the anchor, `%984` is split
at the m23 body: the pre-body piece takes `r5`, the body piece `r9`, and the
split copies survive as `mov r9,r5` / `mov r5,r9`; every `ldr.w rX,[r9]`
becomes a 2-byte `ldr rX,[r5]`, which is the 48-byte size loss (11392 B).

The original has `r9` throughout with **no** anchor instructions, so the
remaining unknown is the source construct that keeps `%984` unsplit without
occupying `r5` with a `Globals::status` value.

## 3. New hypotheses measured this wave

All variants replace only the mission-122 body (and, where noted, the
m122-124 condition); the rest of the TU is pristine.  `raw` is
`python3 /dev/shm/w10819/fdiff.py _ZN5MGame8OnUpdateEv`; `aligned` is
`/dev/shm/w10819/align_diff.py`.

| variant | size | raw | aligned | note |
|---|---:|---:|---:|---|
| **base (w10718, `st` ccm + store)** | **11440** | **37** | **109** | anchor at ccm+pt |
| ccm direct, store via `st` (w10518) | 11440 | 318 | **19** | best aligned; store `mov` remains |
| `Status &st = *Globals::status`, ccm direct, store `st.getPlayingTime()` | 11440 | 318 | 19 | same as w10518 |
| `Status &st`, ccm via ref, store direct | 11436 | 914 | 109 | size -4 |
| `Status &st`, ccm direct, store target `st.field_0x100` | 11492 | 5253 | 761 | size +52 |
| helper `radioBaseFor(Globals::status)` + store via `st` | 11440 | 318 | 19 | helper inlines to the same |
| helper `radioBaseFor(Globals::status)` + store direct | 11392 | 5148 | 597 | no anchor, plain |
| ccm direct, store direct, `(void)st` | 11392 | 5148 | 597 | anchor removed |
| ccm direct, store direct, `if (st == nullptr) return;` | 11412 | 9427 | 1461 | size -28 |
| ccm direct, store direct, `__builtin_expect(st == nullptr, 0)` | 11412 | 9427 | 1461 | size -28 |
| ccm direct, store direct, `needsRedraw = st->field_0x17c` | 11500 | 5276 | 760 | size +60 |
| ccm direct, store direct, `needsRedraw = (st->getShip() != nullptr)` | 11508 | 5253 | 764 | size +68 |
| `volatile long long` condition field (ccm direct, store via `st`) | 11444 | 4488 | 392 | size +4 |
| `Status *volatile st`, both via `st` | 11440 | 331 | 116 | stack reloads |
| comma-assign `st` at the condition field, store target via `st` | 11492 | 5253 | 761 | size +52 |

## 4. Conclusion / open question

The 37-byte residual is still a hard local optimum for every order-preserving
source spelling tried.  The new data confirms:

* Every `Status &` / helper spelling that keeps the anchor produces exactly
  the w10518 shape (318/19): the ccm receiver becomes `ldr.w r0,[r9]` but the
  store keeps `mov r0,r5; b.w 17e270`.
* Every spelling that removes the anchor (plain `Globals::status`, `(void)st`,
  helper with no anchor) drops to 11392 B because `%984` splits to `r5`.
* Keeping `st` live with an extra use costs >= 28 B (size-fatal).
* The `volatile` condition field forces the r2 load but costs 4 B.

The remaining unknown is the source construct that keeps `%984` in `r9`
without a live `Status*` in `r5`.  Candidate untested levers:

1. A local in a block *within* `%984`'s MIR interval that occupies `r5` but
   emits no instruction (a value the compiler rematerialises) — e.g. a
   branch-weight-induced allocation difference.
2. The cross-TU `src/game/core/Globals.h` `&Globals::status` allocation hint
   (affects all 331 `Globals::status` sites; a full-gate pass of its own).
3. A source construct that makes the store tail merge with the m23 tail but
   still reloads `Globals::status` for `getPlayingTime` (the original's
   `b.w 17e26c`).

## 5. Gate (pristine source restored)

```
cmake --build cmake-build-match --target check -j6
  verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
  RATCHET PASS      (goal_lint CLEAN, drift 0)
```

Before -> after: byte 4507 -> 4507, linked 4507 -> 4507.  Target unchanged at
11440 B / raw 37 / aligned 109.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10819
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10819/fdiff.py _ZN5MGame8OnUpdateEv
python3 /dev/shm/w10819/align_diff.py _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j6
```

Scratch (not committed), `/dev/shm/w10819/`: `fdiff.py`, `align_diff.py`,
`rawdiff.py`, `sweep.py`, `sweep2.py`, `sweep3.py`, `sweep4.py`,
`MGame.base.cpp`, `base_all.log`, `onupdate_prera.mir`.
