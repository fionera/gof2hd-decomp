# MGame::OnUpdate — idle-contact `Status*` residual, wave 119 / w6956

Status: **research pass, no `src/` change landed.** Worktree `wt/w6956`, branch
`wave119-w6956`, base = master `db1106935`. Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ `0x17c8d8`. Best measured shape is
still the pristine source (the `Status *st` anchor at the mission-23 idle
check): **11440 B, 41 diff bytes, one run, fuzzy 99.9 %,
3862/3863 normalised instructions**. This note extends
`mgame-onupdate-wave119-w6938.md`, `...-w6911.md`, `...-w6766.md` and
`...-w6726.md` with a fresh 7.0.2 greedy-MIR comparison of the anchor build
vs the plain build, a full 16-way `{Globals::status, st}` site sweep, and
40 further measured source shapes. Nothing beats 41 bytes.

## 1. Baseline re-verified

```
orig 11440B ours 11440B  pct 99.9  diffbytes 41  runs 1  ninsn 3862/3863
run 0x17bb..0x17e4   (absolute 0x17e093 .. 0x17e0bc)
```

Residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2764-2772`):

```cpp
Status *st = Globals::status;
if (Globals::status->getCurrentCampaignMission() == 23 && !this->player->isMining() &&
    Globals::status->getMission()->isEmpty() &&
    st->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
    3600000) {
```

Original vs ours (linked disassembly, `r9` = `&Globals::status`):

```
original                                   ours
17e08c ldr.w r9,[r0]                       17e08c ldr.w r9,[r0]
17e090 ldr.w r0,[r9]   ; ccm               17e090 ldr.w r5,[r9]   ; st
17e094 blx  ccm                            17e094 mov  r0,r5
17e098 cmp  r0,#23                         17e096 blx  ccm
17e09a bne  17e176                         17e09a cmp  r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne  17e176
17e0a0 blx  isMining                       17e09e ldr.w r0,[sl]
17e0a4 cmp  r0,#0                          17e0a2 blx  isMining
17e0a6 bne  17e176                         17e0a6 cmp  r0,#0
17e0a8 ldr.w r0,[r9]   ; mission           17e0a8 bne  17e176
17e0ac blx  getMission                     17e0aa ldr.w r0,[r9]
17e0b0 blx  isEmpty                        17e0ae blx  getMission
17e0b4 cmp  r0,#0                          17e0b2 blx  isEmpty
17e0b6 beq  17e176                         17e0b6 cmp  r0,#0
17e0b8 ldr.w r0,[r9]   ; pt arg             17e0b8 beq  17e176
17e0bc blx  getPlayingTime                 17e0ba mov  r0,r5
17e0c0 ldr.w r2,[r9]   ; field              17e0bc blx  getPlayingTime
                                           17e0c0 ldr.w r2,[r9]
```

Both blocks are 20 B of status loads + `ldrd`; the run is a register-field
permutation plus the induced 2-byte branch-offset shift. From `0x17e176`
(mission-24) to the end of the function the two disassemblies are
byte-identical.

## 2. Fresh 7.0.2 greedy MIR comparison (anchor vs plain)

`-mllvm -print-after-all` on the NDK r18b clang 7.0.2 (scratch
`/dev/shm/w6956/cur_all.log`, 318 MB; extracted `cur_greedy.mir`,
`cur_vrr.mir`; plain build `plain_cur.log` -> `plain_greedy.mir`,
`plain_vrr.mir`).

**Anchor build** (`Status *st` at ccm+pt), after greedy:

```
42016B  %2931:gpr = t2LDRi12 %1052:tgpr, 0        ; &Globals::status (GOT)
42032B  %128:gpr  = t2LDRi12 %2931:gpr, 0         ; st
42064B  $r0 = COPY %128                            ; ccm arg
42352B  %1059:gpr = t2LDRi12 %2931:gpr, 0         ; mission
42608B  $r0 = COPY %128                            ; pt arg
42688B  %1066:gpr = t2LDRi12 %2931:gpr, 0         ; field
42856B  %2930:gpr = COPY %2931:gpr                 ; region split at bb.242
44520B  %2931:gpr = COPY %2930:gpr                 ; end of m23 body region
```

Post-RA (`cur_vrr.mir`): `$r9 = t2LDRi12 ...got`,
`$r5 = t2LDRi12 $r9, 0` (st), `$r0 = COPY $r5` (ccm),
`$r0 = COPY $r5` (pt), `$r2 = t2LDRi12 $r9, 0` (field).

**Plain build** (no `st`), after greedy:

```
42016B  %2932:gpr = t2LDRi12 %1051:tgpr, 0        ; &Globals::status (GOT)
42032B  %1053:gpr = t2LDRi12 %2932:gpr, 0         ; ccm
42352B  %1059:gpr = t2LDRi12 %2932:gpr, 0         ; mission
42592B  %1064:gpr = t2LDRi12 %2932:gpr, 0         ; pt
42688B  %1067:gpr = t2LDRi12 %2932:gpr, 0         ; field
42872B  %2931:gpr = COPY %2932:gpr                 ; same split, +16 B
44536B  %2932:gpr = COPY %2931:gpr
```

Post-RA: `$r5 = t2LDRi12 ...got` (GOT base in **r5**), then every status
load is the 2-byte `ldr rX,[r5,#0]`; the split copies survive as
`mov r9,r5` / `mov r5,r9` and the whole function recolours
(11392 B / 5148 diff / 212 runs).

So the region split at `bb.242` is **identical in both builds**; the only
difference is the colour of the pre-body piece. In the anchor build `%128`
(`st`) is a global range dequeued before the `RS_Split` GOT base, takes
`r5`, and forces the pre-body piece to `r9`. In the plain build the
pre-body piece is the "main" interval and takes the first free callee-saved
register, which is `r5`.

### 2.1 Why the plain pre-body piece gets `r5` (not `r4`)

`r7` is the frame pointer (prologue `push {r4,r5,r6,r7,lr}` /
`add r7,sp,#12`), `r10`/`r11` hold `&this->player` and `this`, and `r6` is
live-in (`fp+72`, the `elapsedTime` pre-indexed load at `0x17dc0a`).
`r4` is taken inside the pre-body piece by the `3600000` constant load
(`ldr r4,[pc,#816]` at `0x17e0c4`). The allocation order is
`r4 r5 r6 r8 r9 r10 r11`, so the first free register for a value spanning
`[0x17e08c, 0x17e0da)` is `r5`.

`r5` is provably dead in that range in the original: last read
`0x17e044 strb r0,[r5,#0]`, next write `0x17e12a ldr r5,[r0,#0]`
(`Globals::gameText`). The only nearby `r5` use (`0x17e12a`) is *after* the
`bb.242` split point, so it cannot constrain the pre-body piece. `r8` is
also free until `0x17e124 mov r8,r0` (again after the split point).

Consequence: to make the plain source allocate `r9` without an anchor, the
pre-body piece would have to extend past `0x17e12a`/`0x17e124` (i.e. no
split at `bb.242`), or `r5` must be occupied across `[0x17e08c,0x17e0da)`
by a value that emits no instruction in that range. Neither is reachable
from source: every candidate value in the original's final code is either
already assigned (`r4/r6/r10/r11`) or written only after the split.

## 3. Full 16-way site sweep

`{Globals::status, st}` at each of the four status sites (ccm / mission /
pt / field), `Status *st = Globals::status;` declared before the `if`
(`/dev/shm/w6956/v7.json`). Only the two byte-neutral 2-`mov` shapes are
size-exact:

| sites (ccm,mission,pt,field) | size | pct | insn |
|---|---:|---:|---:|
| **`st,G,st,G` (baseline)** | **11440** | **99.9** | **3862/3863** |
| **`G,G,st,G` (pt only; CSE'd with ccm)** | **11440** | **99.9** | **3862/3863** |
| `st,G,st,st` | 11436 | 92.7 | 3862/3862 |
| `G,G,st,st` | 11436 | 92.7 | 3862/3862 |
| `st,G,st,st`/`G,G,G,st`/`st,G,G,st` | 11436 | 92.5 | 3862/3861..2 |
| `st,G,G,G`/`G,G,G,G` (plain) | 11392 | 87.2 | 3862/3834 |
| `G,st,G,G` | 11396 | 87.1 | 3862/3836 |
| `st,st,st,st` | 11432 | 92.4 | 3862/3861 |
| `G,st,st,st` | 11432 | 92.4 | 3862/3861 |

The anchor load is always placed at the declaration point (before the ccm
call) and machine-CSEs with the ccm load, so `G,G,st,G` is *identical* to
the baseline. There is no 11440-byte shape with a plain ccm load.

## 4. Further measured variants (all full-link, pristine base)

`size`/`pct`/`insn` from `/dev/shm/w6956/fz.py` (replays `verify.py`'s
`score()` on the single symbol); `cmake --build ... --target link_so`
between every row.

| variant | size | pct | insn |
|---|---:|---:|---:|
| **baseline** | **11440** | **99.9** | **3862/3863** |
| `reg_st` (`register Status *st`) | 11440 | 99.9 | 3862/3863 |
| `st_plus0` (`Globals::status + 0`) | 11440 | 99.9 | 3862/3863 |
| `st_addr` (`&*Globals::status`) | 11440 | 99.9 | 3862/3863 |
| `local_const` (`const long long idle=3600000`) | 11440 | 99.9 | 3862/3863 |
| `static_const` | 11440 | 99.9 | 3862/3863 |
| `vol_pt` (`*reinterpret_cast<Status *volatile *>`) | 11440 | 99.9 | 3862/3863 |
| `vol_ccm_pt` | 11440 | 99.9 | 3862/3863 |
| `ccm_only_m24pt` (`st` at ccm + m24 pt) | 11440 | 99.7 | 3862/3863 |
| `p_cache` (`PlayerEgo *p=this->player` + anchor) | 11440 | 99.0 | 3862/3863 |
| `vol_pt_field` | 11436 | 92.7 | 3862/3862 |
| `field_only` (anchor at field only) | 11436 | 92.5 | 3862/3861 |
| `ccm_only_m24ccmpt` | 11436 | 92.4 | 3862/3862 |
| `const_st_all` (`st` at all four) | 11432 | 92.4 | 3862/3861 |
| `mining_hoist_stpt` / `mining_hoist_stccmpt` | 11432 | 92.3 | 3862/3861 |
| `int_mining_pt` / `mining_pt_only` | 11432 | 92.3 | 3862/3861 |
| `st_before_mining_pt` | 11432 | 92.2 | 3862/3860 |
| `body_field_plain` (`st` in body, cond plain) | 11444 | 87.1 | 3862/3849 |
| `body_field_pt` / `pre_body_field` | 11524 | 81.7 | 3862/3874..5 |
| `body_both_plain` / `body_both_pt` | 11520 | 81.6 | 3862/3874..5 |
| `comma_after_mining` (`(st=…, getMission()->isEmpty())`) | 11396 | 87.1 | 3862/3836 |
| `comma_before_mining` | 11396 | 87.1 | 3862/3836 |
| `comma_after_mission` (`(st=…, st->getPlayingTime())`) | 11392 | 87.2 | 3862/3834 |
| `assign_at_field` (`(st=…)->field_0x100`) | 11392 | 87.2 | 3862/3834 |
| `p_cache_plain` / `p_only_plain` | 11396 / 11392 | 87.1 | 3862/3835..6 |
| `p_ls_cache` (cache player + levelScript) | 11392 | 87.1 | 3862/3835 |
| `p_gt_plain` (cache player + gameText) | 11180 | 77.6 | 3862/3749 |
| `mining_hoist` | 11384 | 87.0 | 3862/3832 |
| `mining_st_ccm` | 11384 | 87.0 | 3862/3832 |
| `vol_st` (`Status *volatile st`) | 11340 | 82.1 | 3862/3800 |
| `elapsed_status` (`… && Globals::status != nullptr`) | 11484 | 76.8 | 3862/3881 |
| `script_status` | 11360 | 81.4 | 3862/3833 |
| `st_inside_elapsed` | 11392 | 60.1 | 3862/3841 |
| plain control | 11392 | 87.2 | 3862/3834 |

New negative evidence:

* **`comma_after_mining`** defines the anchor *after* the ccm call, so the
  ccm load is plain — but the GOT base then takes `r5` first
  (`ldr r5,[r0]` / `ldr r4,[r5]`), the anchor lands in `r4`, and the whole
  function recolours (11396 B). The anchor only wins `r5` when it is a
  global range that is dequeued before the `RS_Split` GOT base, which
  requires it to be defined at/before the ccm load — i.e. exactly the CSE
  site.
* **`p_cache`** (`PlayerEgo *p = this->player;` before the `if`) puts `p`
  in `r4` and keeps the GOT base in `r9` (because the anchor still holds
  `r5`), but adds the `ldr.w r4,[sl]` before the block and turns the
  isMining receiver into `mov r0,r4` (99.0 %).
* **`p_only_plain`** (same `p`, plain condition) still gives the GOT base
  `r5`: `p` takes `r4`, and `r5` is free. Occupying *both* `r4` and `r5`
  needs a second global range live across the condition; every candidate
  (`this->levelScript`, `Globals::gameText`, `Globals::rnd`, …) is either
  dead before the condition or changes the byte-exact m23 body.
* **`Status::field_0x100` as a real `int64_t`** (instead of
  `int32_t` + `reinterpret_cast<long long &>`, tested by editing
  `Status.h` + `MGame.cpp` in scratch and compiling with the exact NDK
  line) produces the *identical* idle block (`ldr.w r5,[r9]` +
  `mov r0,r5`), so the TBAA/type of the field is not the lever.

## 5. Conclusion

The residual is a hard local optimum for every anchor spelling and every
plain spelling tried across waves 97/105/106/107/108/112/119 (now 150+
measured shapes). The exact shape needs the plain source with `r9`; clang
7.0.2 hands the plain pre-body piece `r5` because nothing occupies `r5`
across `[0x17e08c,0x17e0da)` and the region split at `bb.242` keeps the
piece away from the later `r5`/`r8` writes. The only untested lever remains
the cross-TU one from wave 112 §5 (make the `&Globals::status` GOT load
non-rematerialisable via `src/game/core/Globals.h`), which is a full-gate
pass of its own.

## 6. Structural state (unchanged — no `src/` change)

```
sections            27 (exact, both)
file size           2,187,328 B (exact, both)
.ARM.extab          unchanged by this pass (still 4 B short, Generator::createMission)
.rodata             unchanged (mission rank table is the last input)
```

## 7. Gate (this worktree, pristine source restored)

```
cmake --build cmake-build-match --target check -j6
  RATCHET PASS
  verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
  lint  : CLEAN
  drift : 0 field(s) across 0 class(es)
```

Before -> after: byte 4505 -> 4505, linked 4505 -> 4505. Target unchanged at
11440 B / 41 diff bytes / 99.9 %.

## 8. Commands used

```bash
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w6956/fz.py cmake-build-match/libgof2hdaa.so
bash /dev/shm/w6956/cc.sh src/game/menu/MGame.cpp /dev/shm/w6956/cur.o \
     -mllvm -print-after-all 2> /dev/shm/w6956/cur_all.log
cmake --build cmake-build-match --target check -j6
```

Scratch (not committed): `/dev/shm/w6956/` — `MGame.base.cpp` (pristine TU),
`cc.sh` (exact compile line), `fz.py` (single-symbol score replay),
`harness.py` / `h3.py` (variant harness), `block.py` (idle-block dump),
`v5..v22.json` + `v5..v22.py` (the variant sweeps above), `cur_greedy.mir`
/ `cur_vrr.mir` / `plain_greedy.mir` / `plain_vrr.mir` (7.0.2 MIR).
