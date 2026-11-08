# MGame::OnUpdate — 37-byte idle-contact residual, wave 149 / w9519

Status: **research pass, no `src/` change landed.** Worktree `wt/w9519`, base =
master `fece77a64` (wave-148 snapshot). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ `0x17c8d8`. Best measured shape is
still the pristine source: **11440 B, raw 37, aligned 109**. This note extends
`mgame-onupdate-wave132-w7719.md` / `...-wave129-w7661.md` /
`...-wave126-w7617.md` and records the exact residual plus a new sweep of
statement-order, scope, anchor-placement, type and `__builtin_expect` shapes.

## 1. Baseline (re-verified this session)

```
orig 0x17c8d8 11440   ours 0x17c8d8 11440
raw diff 37, 4 runs
aligned (instruction-index) 109
```

The four raw runs are:

| run | bytes | what |
|---|---:|---|
| `0x19e1` | 1 | mission-23 field pointer register (`r2` vs `r5`) |
| `0x19e4` | 1 | mission-23 `ldrd` base register (`r2` vs `r5`) |
| `0x28ba..0x28db` | 34 | mission-122 body: anchor `mov r0,r5` shifts the whole block by 2 |
| `0x28de` | 1 | mission-122 branch target `17e26c` vs `17e270` |

Exact instructions (function-relative addresses; `r9` = `&Globals::status`):

```
original                                   ours (pristine source)
17e2b6 ldr.w r2,[r9]   ; field             17e2b6 ldr.w r5,[r9]   ; field
17e2bc ldrd  r2,r3,[r2,#256]               17e2bc ldrd  r2,r3,[r5,#256]

17f192 ldr.w r0,[r9]   ; ccm               17f192 mov  r0,r5       ; anchor
17f196 blx   ccm                           17f194 blx  ccm
...
17f1b0 blx   createRadioMessage            17f1ae blx  createRadioMessage
17f1b4 b.w   17e26c                         17f1b2 mov  r0,r5
                                           17f1b4 b.w   17e270
17e26c ldr.w r0,[r9]   ; shared tail       17e26c ldr.w r0,[r9]   ; used only by m23
17e270 blx   getPlayingTime                 17e270 blx  getPlayingTime
```

The source statement is the mission-122 body (`src/game/menu/MGame.cpp:2810-2814`):

```cpp
Status *st = Globals::status;
int radioIndex = Globals::rnd->nextInt(2);
int radioBase = st->getCurrentCampaignMission() == 122 ? 3164 : 3166;
this->level->createRadioMessage(28, radioIndex + radioBase);
reinterpret_cast<long long &>(Globals::status->field_0x100) = st->getPlayingTime();
```

The original is the plain form (`Globals::status` at every site). The current
anchor is byte-neutral in size (the 2-byte `mov` at ccm replaces 2 bytes of the
4-byte `ldr.w`, and the 2-byte `mov` at getPlayingTime replaces the skipped
4-byte shared-tail load) but shifts the block and shares `r5` with the
mission-23 field pointer.

## 2. The allocator decision

`-mllvm -print-after-all` (scratch `base_all.log`, `base_prera.mir`) shows:

* The GOT base is `%1053 = t2LDRi12 %1052, 0` at MIR `42016B`, live to the
  last `Globals::status` use at MIR `102528B` (source line 3095, mission-92
  hint). It is a huge interval.
* The anchor `%142 = t2LDRi12 %1053, 0` is defined at MIR `48560B` and used at
  `49072B`/`49360B`, i.e. **inside** `%1053`'s interval. It takes `r5`; the
  greedy allocator therefore gives `%1053` `r9` (the next callee-saved
  register in its `r4 r5 r6 r8 r9 r10 r11` order).
* Without the anchor (`plain` build, 11392 B) the GOT base is split at bb.242
  (the mission-23 body): the pre-body piece takes `r5`, the body piece `r9`,
  and the split copies survive as `mov r9,r5` at `17e0d2`/`17e1b2` and
  `mov r5,r9` at `17e256`. Every `ldr.w rX,[r9]` becomes `ldr rX,[r5]`
  (2 bytes), which is the 48-byte size loss.
* The original has `r9` throughout with **no anchor instructions**, so the
  remaining unknown is what keeps `%1053` unsplit in the original. The anchor
  is the only source spelling found so far that reliably forces `r9`.

## 3. New hypotheses measured this wave

All variants replace only the mission-122 body or the listed condition; the
rest of the TU is pristine. `raw` is `python3 /dev/shm/fdiff.py
_ZN5MGame8OnUpdateEv`; `aligned` is `_work/reconstructions/align_diff.py`.

| variant | size | raw | aligned | note |
|---|---:|---:|---:|---|
| **base (pristine)** | **11440** | **37** | **109** | anchor at ccm+pt |
| plain (no anchor) | 11392 | 5148 | 597 | GOT base r5, split copies |
| anchor after `radioIndex` | 11444 | 922 | 109? | +4 B |
| anchor before `radioIndex` (base) | 11440 | 37 | 109 | |
| `radioBase` before `radioIndex` | 11440 | 54 | — | worse |
| plain, `radioBase` first | 11436 | 1237 | — | |
| anchor ccm-only | 11436 | 914 | — | |
| anchor pt-only | 11440 | 318 | 346 | ccm plain, field r2 |
| anchor store-only | 11492 | 5253 | — | |
| anchor store+pt | 11488 | 5201 | — | |
| `Status *&st` reference anchor | 11392 | 5148 | 597 | plain |
| `Status **stp` pointer anchor | 11392 | 5148 | 597 | plain |
| `Status *const st` | 11440 | 37 | 109 | same |
| `(Status *) Globals::status` cast | 11440 | 37 | 109 | same |
| `&*Globals::status` | 11440 | 37 | 109 | same |
| nested block around body | 11440 | 37 | 109 | same |
| outer `int radioIndex/radioBase` declarations | 11392 | 5148 | 597 | plain |
| `(void) st` / self-assign / sizeof / typeid / dead `if` | 11392 | 5148 | 597 | anchor removed |
| `long long hour = 3600000` local | 11392 | 5148 | 597 | plain |
| `*(long long *)&field` spelling | 11440 | 37 | 109 | same |
| local `long long *field` pointer | 11440 | 888 | — | worse |
| anchor at top of OnUpdate, used in wormhole block | 11348 | 10774 | — | CSE across whole function |
| anchor at top of `elapsedTime` block | 11396 | 8702 | — | |
| anchor at top of script block, used in m122 | 11440 | 890 | — | m23 ccm gets `mov` |
| `Globals::gameText` anchor before m23 | 11176 | 7807 | — | CSE across whole function |
| `Globals::rnd` anchor before m23 | 11420 | 5022 | — | |
| `__builtin_expect` m122 cond, 0 / 1 | 11456 / 11432 | 4456 / 4917 | — | |
| `__builtin_expect` m23 cond, 0 / 1 | 11444 / 11508 | 5111 / 5170 | — | |
| `__builtin_expect` m23 pt term, 0 / 1 | 11444 / 11428 | 5111 / 4904 | — | |
| `__builtin_expect` outer elapsedTime, 0 / 1 | 11324 / 11348 | 5959 / 5689 | — | |
| `__builtin_expect` outer scriptTime, 0 / 1 | 11480 / 11364 | 6212 / 9026 | — | |
| `else if` chain swap 23<->122 | 11440 | 607 | — | |
| `else if` chain swap 23<->24 | 11440 | 69 | 123 | |
| wormhole block moved before elapsedTime | 11444 | 6170 | — | |
| m23 condition reorder: isMining first | 11516 | 5288 | — | wave-129 `rev2` |
| m23 condition reorder: pt first | 11432 | 5032 | — | |
| m23 condition reorder: empty first | 11392 | 5145 | 597 | plain |
| `st` anchor for m23 field only | 11436 | 4993 | — | |
| `st` anchor for m23 field + m122 anchor | 11436 | 4994 | — | |
| `st` anchor for m23 pt only, field plain | 11440 | 41 | 346 | closest size |

The closest new shape is `st_pt_only_field_plain` (11440 B, raw 41, aligned
346): it makes the m122 ccm plain and the m23 field load `r2`, but the m23
`getPlayingTime` anchor adds a `mov` and shifts everything after it.

## 4. Conclusion / open question

The 37-byte / 109-aligned residual is still a hard local optimum for every
order-preserving source spelling tried (waves 97/105/106/107/108/112/119/123/
126/129/132, now 149). The new sweep confirms:

* The anchor is the only source spelling that forces the GOT base to `r9`.
* The anchor's own cost is byte-neutral but its register (`r5`) is shared
  with the mission-23 field pointer, and its `mov` shifts the m122 block.
* The original has `r9` with no anchor instructions, so the remaining unknown
  is the source construct that keeps `%1053` unsplit without occupying `r5`
  with a `Globals::status` value. Candidate untested levers:
  1. A local in a block *within* `%1053`'s MIR interval (42016B..102528B)
     that occupies `r5` but emits no instruction (a value the compiler
     rematerialises). The wormhole anchor is outside the interval; the
     gameText/rnd anchors change the whole-function CSE.
  2. The cross-TU `src/game/core/Globals.h` `&Globals::status` allocation
     hint (affects all 331 `Globals::status` sites; a full-gate pass of its
     own).

## 5. Gate (pristine source restored)

```
cmake --build cmake-build-match --target check -j6
  verify: byte 4507  linked 4507  stubs 0  missing 0  extra 0  avg 99.93
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
  RATCHET PASS      (goal_lint CLEAN, drift 0)
```

Before -> after: byte 4507 -> 4507, linked 4507 -> 4507. Target unchanged at
11440 B / raw 37 / aligned 109.

## 6. Commands used

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9519/fdiff.py _ZN5MGame8OnUpdateEv
python3 _work/reconstructions/align_diff.py _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j6
```

Scratch (not committed), all under `/dev/shm/w9519/`:
`MGame.base.cpp` (pristine TU), `cc.sh` (exact clang 7.0.2 line),
`base_all.log` / `plain_all.log` (`-mllvm -print-after-all`),
`base_prera.mir` / `base_postra.mir` / `plain_prera.mir` / `plain_postra.mir`,
`fdiff.py`, `align_diff.py`, `rawdiff.py`, `dis.py`, `batch1.py`..`batch29.py`.
