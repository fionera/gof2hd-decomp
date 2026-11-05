# MGame::OnUpdate — idle-contact `Status*` r5/r9 residual (wave 119, w6726)

Status: **research pass, no `src/` change landed.** Worktree `wt/w6726`, branch
`wave119-w6726`, base = current master (`432167221`). Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8. Best measured shape is
still the pristine source: **41 diff bytes, one run, size exact, 99.9 %**.
This note extends waves 97 / 105 / 106 / 107 / 108 / 112 with a new
`-print-after-all` MIR dump of the 7.0.2 build, a 7.0.1 `-debug-only=regalloc`
trace, and 29 further measured source shapes.

## 1. Baseline (re-verified this session)

```
size 11440 11440  pct 99.9
diffbytes 41 runs 1
0x17bb 0x17e3 41          (absolute 0x17e093 .. 0x17e0bb)
```

The whole residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2766-2772`):

```cpp
Status *st = Globals::status;
if (Globals::status->getCurrentCampaignMission() == 23 && !this->player->isMining() &&
    Globals::status->getMission()->isEmpty() &&
    st->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
    3600000) {
```

Original vs ours:

```
original                                   ours
17e08c ldr.w r9,[r0]  ; r9 = &Globals::status
17e090 ldr.w r0,[r9]                       17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission       17e094 mov   r0,r5
17e098 cmp r0,#23                          17e096 blx   getCurrentCampaignMission
17e09a bne 17e176                          17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne   17e176
17e0a0 blx isMining                        17e09e ldr.w r0,[sl]
17e0a4 cmp r0,#0                           17e0a2 blx   isMining
17e0a6 bne 17e176                          17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]                       17e0a8 bne   17e176
17e0ac blx getMission                      17e0aa ldr.w r0,[r9]
17e0b0 blx isEmpty                         17e0ae blx   getMission
17e0b4 cmp r0,#0                           17e0b2 blx   isEmpty
17e0b6 beq 17e176                          17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]                       17e0b8 beq   17e176
17e0bc blx getPlayingTime                  17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]                       17e0bc blx   getPlayingTime
                                           17e0c0 ldr.w r2,[r9]
```

Both blocks are 52 B (`4+4+4+4` vs `4+2+4+2+4`), so the run is a pure byte
permutation of the register field plus the induced branch-offset shift. From
0x17e176 (mission-24) to the end of the function the two disassemblies are
byte-identical.

The verifier's own scoring (`tools/verify/verify.py:score`) sees exactly
**two** `replace` opcodes over 3862/3863 normalised instructions:

```
replace 1992 1993 1992 1994     O: ldr.w r0,[r9]
                                U: ldr.w r5,[r9] + mov r0,r5
replace 2005 2006 2006 2007     O: ldr.w r0,[r9]
                                U: mov r0,r5
```

`2*3860 / (3862+3863) = 99.935 %` → reported `99.9`. Removing **one** of the
two replaces would give `99.961 %` → `100.0`; that is the only remaining
fuzzy-metric step, and it requires either a byte-exact (0-diff) shape or a
one-`mov` shape of exactly the same size.

## 2. New evidence this wave

### 2.1 7.0.2 `-print-after-all` MIR (pristine source)

Full pass dump: `/tmp/w6726/pa_nost.err` (4.23 M lines). Extracted stages:
`lia_nost.mir` (pre-RA, after LiveIntervals), `lrm_nost.mir` (Live Register
Matrix), `ra_nost.mir` (post-greedy), `vrm_nost.mir` (post-RA / VirtRegMap).

Reproduce:

```bash
CMD=$(cat /dev/shm/w6726/cc_cmd.txt | sed 's# -MD -MT [^ ]* -MF [^ ]*##; s# -o [^ ]*# -o /dev/null#')
eval "$CMD -mllvm -print-after-all" > /dev/null 2>/tmp/w6726/pa_nost.err   # ~35 s
```

In the no-`st` build the idle block is `bb.238..bb.241`; `%1053` is the
`&Globals::status` GOT base (`42016B %1053 = t2LDRi12 %1052, 0`,
`load 4 from got`) and lands in **`$r5`**, so every `ldr.w rX,[r9]` of the
original collapses to the 2-byte `ldr rX,[r5]` and the whole function
recolours (11392 B / 5148 diff / 212 runs).

### 2.2 7.0.1 `-debug-only=regalloc` trace

`/tmp/w6726/ra701.txt` (built by substituting
`/dev/shm/wf213/llvm701build/bin/clang++` into the same command plus
`-mllvm -debug-only=regalloc`). 7.0.1's GPR order is
`[ r0 r1 r2 r3 r12 lr r4 r5 r6 r8 r9 r10 r11 ]` (r7 = FP). In 7.0.1 `%1052`
first gets `$r6`, is evicted by `%127` (the `ldr r6,[fp,#72]!` pre-index
pointer), then gets `$r9` and stays — i.e. 7.0.1 already produces the
original's r9 colour for the no-`st` shape. 7.0.1 is therefore **not**
faithful to 7.0.2's `$r5` cost and cannot be used as an oracle here.

### 2.3 Source-layout dump

`../source file list dump/gof2hd_1.1.19_ios.cpp`, MGame.cpp section: one TU
containing the whole MGame class (ctor/dtor, OnInitialize, reset,
handleAccelerometer, maneuverTouchEnd, freeCamTouch*, pauseSounds,
resumeSounds, OnTouchBegin/Move, useCloak, UseKhadorDrive,
startChargingJumpDrive, OnTouchEnd, startJumpScene, closePauseMenu,
setCinematicMode, switchCamera, nextCamId, OnRelease, OnKeyPress, OnUpdate,
updateJumpScene, dockEvent, dialogueEvent, successCheck, gameOverCheck,
OnResume, OnSuspend, OnRender2D/3D, togglePause, OnKeyRelease). No extra
statics witnessed.

### 2.4 Register availability at 0x17e08c (original)

`r5` is free: last write `17e034 ldr r5,[r4,#0]`, last read
`17e044 strb r0,[r5,#0]`, next write `17e12a ldr r5,[r0,#0]` (gameText).
`r6` is free too (last write `17e034 ldr r5,[r4,#0]`, next `17e414
strh r2,[r6,#52]`). `r9` is a scratch before 0x17e08c. So the original did
**not** have a live value in r5 — the allocator simply chose r9 for `%1053`.

## 3. Mechanism (re-confirmed)

`RegAllocGreedy` (`/dev/shm/wf110/llvm702src/lib/CodeGen/RegAllocGreedy.cpp`):

```cpp
  if (ExtraRegInfo[Reg].Stage == RS_Split) {
    Prio = Size;                       // split ranges: dequeued late
  } else { ... Prio = (1u << 29) + Size; ... Prio |= (1u << 31); }
```

`%1053` (GOT base) is split by the wingmen-radio loop (spill/reload at
0x17e31c / 0x17e348), so it is `RS_Split`. The `Status*` value `%128`
(`42032B %128 = t2LDRi12 %1053, 0`) is a multi-MBB global range; it is
dequeued before `%1053`, takes r5 (CostPerUse 0, first free), and forces
`%1053` to r9. Without it, `tryAssign` returns r5 immediately for `%1053`
(`Cost == 0`, no `tryEvict`).

The new measurements sharpen the *range-length* requirement: the anchor must
be **defined at the ccm load** (so machine CSE merges it with the ccm load)
and must be **live through the `getPlayingTime` load**. Any shorter span
loses r5 to `%1053`:

* `decl_top_ccm_mission` (st live 0x17e090..0x17e0a8) →
  `ldr r5,[r0,#0]` (%1053→r5) then `ldr r4,[r5,#0]` (st→r4): anchor loses.
* `decl_top_mission_pt` (st live 0x17e090..0x17e0b8, but ccm is CSE'd too, so
  3 uses) → `%1053`→r9, `st`→r5, but 3 `mov`s and 11436 B.
* `assign_at_mission` (`(st = Globals::status)->getMission()`, def at
  0x17e0a4) → anchor loses r5, 11396 B / 5129 diff.
* `comma_before_mining_*` (def at 0x17e09c) → anchor loses r5, 11396 B.

## 4. Variants measured this wave (all from the pristine baseline)

`cmake --build cmake-build-match --target link_so` + `tools/verify` diff
(`/dev/shm/w6726/fdiff.py`). `usize` is the linked `nm` size of
`_ZN5MGame8OnUpdateEv`; baseline row is the current source.

| variant (what `st` is used at / how it is defined) | usize | diff | runs |
|---|---:|---:|---:|
| **baseline: `st` at top, used at ccm + m23 `getPlayingTime`** | **11440** | **41** | **1** |
| `assign_ccm_pt`: `(st = Globals::status)->ccm`, `st->pt` | 11440 | 41 | 1 |
| `decl_top_stccm_pt` / `decl_top_stccm_stpt` | 11440 | 41 | 1 |
| `st_const` (`Status *const st = …`) | 11440 | 41 | 1 |
| `st_auto` (`auto *st = …`) | 11440 | 41 | 1 |
| `st_ref` (`Status &st = *Globals::status`, `st.getPlayingTime()`) | 11440 | 41 | 1 |
| `st_deref` (`(*st).getPlayingTime()`) | 11440 | 41 | 1 |
| `st_pt_paren` (parenthesised difference) | 11440 | 41 | 1 |
| `st_pt_ll` (`3600000LL`) | 11440 | 41 | 1 |
| `st_pt_gt` (`3600000 < …`) | 11440 | 41 | 1 |
| `baseline_plus_void_st` (`(void)st;` in the body) | 11440 | 41 | 1 |
| `assign_at_mission` (`(st = …)->getMission()`, `st->pt`) | 11396 | 5129 | 240 |
| `assign_at_ccm` (`(st = …)->ccm`, `st->getMission()`) | 11396 | 5129 | 240 |
| `decl_top_ccm_mission` | 11396 | 5129 | 240 |
| `comma_before_mining_mission_pt` | 11396 | 5129 | 240 |
| `comma_before_mining_pt` | 11396 | 5129 | 240 |
| `comma_before_mining_mission` | 11396 | 5129 | 240 |
| `comma_after_mining_mission_pt` | 11396 | 5129 | 240 |
| `assign_mission_pt_plain` (`(st = …) && st->getMission() && st->pt`) | 11400 | 5131 | 228 |
| `decl_top_mission_pt` (st at top, used at mission + pt) | 11436 | 4982 | 303 |
| `assign_at_ccm_mission_pt` (3 uses) | 11436 | 4982 | 303 |
| `st_at_top_used_field_and_pt` (3 uses) | 11436 | 4981 | 307 |
| `ccm_field` (`st` used at ccm + `field_0x100`) | 11436 | 4993 | 301 |
| `ccm_mission_pt_3use` | 11432 | 5033 | 270 |
| `assign_at_mission_field` | 11392 | 5145 | 214 |
| `assign_at_pt_field` | 11392 | 5148 | 212 |
| `decl_top_ccm_only` | 11392 | 5148 | 212 |
| `no_st_plus_dummy_use` | 11392 | 5148 | 212 |
| `st_then_assign_pt` | 11392 | 5148 | 212 |
| control: no `st` at all | 11392 | 5148 | 212 |

Nothing beats 41. `assign_ccm_pt` produces the *identical* 41-byte,
one-run shape (`ldr.w r5,[r9] / mov r0,r5 / … / mov r0,r5`), confirming the
baseline is the unique minimum for this anchor family.

## 5. Why a one-`mov` shape is out of reach

`st`'s byte budget is `+4` (the `ldr.w r5,[r9]` that replaces the ccm
`ldr.w r0,[r9]`) minus `2` per `mov r0,r5`. Byte-neutrality therefore needs
exactly **two** uses, and the second use must be at or after the
`getPlayingTime` load for the anchor to win r5. The `getPlayingTime` load is
the earliest such site, so the shift window is fixed at
`0x17e090..0x17e0bc` → 41 diff bytes.

The only escape would be a shape where the ccm load itself stays
`ldr.w r0,[r9]` *and* some other global range occupies r5 across
0x17e088 — but every such range is visible in the final code, and the
original has r5 genuinely free from 0x17e044 to 0x17e12a. The remaining
untested lever is still wave 112 §5: make the `&Globals::status` GOT load
non-rematerialisable via `src/game/core/Globals.h` (`static Status *status;`,
331 GOT-base sites binary-wide) — a separate cross-TU pass with its own full
gate measurement.

## 6. Structural state (unchanged — no `src/` change)

```
sections            27 (exact, both)
file size           2,187,328 B (exact, both)
.text               0x1337c8 @ 0x69440   (size/base exact)
.ARM.exidx          0x6f70   @ 0x19cc08  (size/base exact)
.ARM.extab          0x26290 ours / 0x26298 orig (-8 B, createShip only)
.rodata             0x41c27 ours / 0x41b67 orig (+192 B rank table)
.got                0x430c   @ 0x20fcf4  (size/base exact)
```

No section, `.ARM.extab` or file-size delta was introduced by this pass.

## 7. Gate (note-only pass)

`cmake --build cmake-build-match --target check -j6` with the pristine source
restored:

```
verify: byte 4503  linked 4503  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS      (goal_lint CLEAN)
drift:  0 field(s) across 0 class(es)
```

Before -> after: byte 4503 -> 4503, linked 4503 -> 4503. Target unchanged at
41 diff bytes / 99.9 % / 11440 B.

## 8. Commands used

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6726/fdiff.py                       # size / pct / diff / runs
cmake --build cmake-build-match --target check -j6
```

Scratch (not committed): `/dev/shm/w6726/` — `MGame.base.cpp` (pristine TU),
`fdiff.py`, `cc_cmd.txt`, `orig_dis.txt`, `rea.c`, `rea.json`;
`/tmp/w6726/` — `try.py`, `batch8.py`..`batch12.py`, `pa_nost.err`,
`lia_nost.mir`, `lrm_nost.mir`, `ra_nost.mir`, `vrm_nost.mir`, `ra701.txt`.
