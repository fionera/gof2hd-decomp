# MGame::OnUpdate — idle-contact `Status*` r5/r9 residual, wave 119 / w6911

Status: **research pass, no `src/` change landed.** Worktree `wt/w6911`, branch
`wave119-w6911`, base = current master (`a22737824`). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ `0x17c8d8`. Best measured shape is
still the pristine source: **11440 B, 41 diff bytes, one run, fuzzy 99.9 %,
3862/3863 normalised instructions**. This note extends
`mgame-onupdate-wave119-w6726.md` and `mgame-onupdate-wave119-w6766.md` (same
wave, same residual) with a *new* MIR comparison of the `st`-anchor build vs
the plain build, a corrected account of the region split, and 30 further
measured source shapes. Nothing beats 41.

## 1. Baseline re-verified

```
orig 11440 ours 11440  pct 99.9  diffbytes 41  runs 1
run 0x17bb..0x17e3   (absolute 0x17e093 .. 0x17e0bb)
```

Residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2766-2772`):

```cpp
Status *st = Globals::status;
if (Globals::status->getCurrentCampaignMission() == 23 && !this->player->isMining() &&
    Globals::status->getMission()->isEmpty() &&
    st->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
    3600000) {
```

Original vs ours (linked disassembly):

```
original                                   ours
17e08c ldr.w r9,[r0]  ; &Globals::status   17e08c ldr.w r9,[r0]
17e090 ldr.w r0,[r9]  ; ccm arg            17e090 ldr.w r5,[r9]   ; st
17e094 blx   ccm                           17e094 mov   r0,r5
17e098 cmp   r0,#23                        17e096 blx   ccm
17e09a bne   17e176                        17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne   17e176
17e0a0 blx   isMining                      17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                         17e0a2 blx   isMining
17e0a6 bne   17e176                        17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]  ; mission            17e0a8 bne   17e176
17e0ac blx   getMission                    17e0aa ldr.w r0,[r9]
17e0b0 blx   isEmpty                        17e0ae blx   getMission
17e0b4 cmp   r0,#0                         17e0b2 blx   isEmpty
17e0b6 beq   17e176                        17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]  ; pt arg             17e0b8 beq   17e176
17e0bc blx   getPlayingTime                 17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]  ; field               17e0bc blx   getPlayingTime
                                            17e0c0 ldr.w r2,[r9]
```

Both blocks are 52 B; the run is a register-field permutation plus the induced
2-byte branch-offset shift. From `0x17e176` (mission-24) to the end of the
function the two disassemblies are byte-identical.

## 2. New evidence: the region split is the *same* in both builds

`-mllvm -print-after-all` dumps of the 7.0.2 build (scratch:
`/dev/shm/w6911/pa_st.err`, 318 MB; extracted post-RA block
`/dev/shm/w6911/vrm_st.mir`; the no-`st` dumps are `/tmp/w6726/pa_nost.err`,
`.../ra_nost.mir`, `.../vrm_nost.mir`).

**`st` build, MIR after `Greedy Register Allocator`** (`pa_st.err` line
3615717, function `_ZN5MGame8OnUpdateEv`), GOT-base vreg `%2931`:

```
42016B  %2931:gpr = t2LDRi12 %1052:tgpr, 0   ; load 4 from got
42032B  %128:gpr  = t2LDRi12 %2931:gpr, 0    ; st
...
42856B  %2930:gpr = COPY %2931:gpr           ; <- region split at bb.242
44520B  %2931:gpr = COPY %2930:gpr           ; <- end of the m23 body region
45704B  %2935:gpr = COPY %2931:gpr
47368B  %2931:gpr = COPY %2935:gpr
63016B  %2937:gpr = COPY %2931:gpr
66104B  %2931:gpr = COPY %2937:gpr
```

**plain build, MIR after greedy** (`ra_nost.mir`), GOT-base vreg `%2932`:

```
42016B  %2932:gpr = t2LDRi12 %1051:tgpr, 0
42032B  %1053:gpr = t2LDRi12 %2932:gpr, 0
...
42872B  %2931:gpr = COPY %2932:gpr           ; <- same split, +16B offset
44536B  %2932:gpr = COPY %2931:gpr
45720B  %2936:gpr = COPY %2932:gpr
47384B  %2932:gpr = COPY %2936:gpr
63032B  %2938:gpr = COPY %2932:gpr
```

So the **split structure is identical** in the two builds (the 16 B offset is
just the extra `ldr r5,[r9]` + `mov` + the shift it induces). The only
difference is the colour of the two pieces:

* `st` build: pre-body piece `[42016B,42856B)` → `r9`, body piece → `r9`
  (both copies coalesce away, so the target block has no `mov`);
* plain build: pre-body piece → `r5`, body piece → `r9`, so the copies
  survive as `mov r9,r5` at `0x17fa` / `mov r5,r9` at `0x197e` and the whole
  function recolours (11392 B / 5148 diff / 212 runs).

### 2.1 Why the original cannot be the plain source with a *longer* piece

The original loads the `Globals::status` GOT base exactly three times in the
whole function (`objdump -d` grep for `ldr.w r9,[rX]`): `0x17ca32`,
`0x17d296` (a function pointer, `blx r9`) and `0x17e08c`. From `0x17e08c`
onwards there is **no reload and no `mov` copy at all** until the epilogue —
`0x17e0da` is `movs r0,#1` (no copy), `0x17e262` is `movs r0,#1` (no copy),
`0x17e26c`/`0x17e274` are `ldr.w r0,[r9]`/`ldr.w r2,[r9]`. The only shuffle
is the stack spill around the wingmen loop (`str.w r9,[sp,#36]` at
`0x17e31c`, `ldr.w r9,[sp,#36]` at `0x17e348`).

So in the original the GOT base is `r9` continuously across the m23 body,
i.e. the pre-body piece and the body piece got the *same* register. `r5` is
provably dead from `0x17e044` (last read) to `0x17e12a` (next write) in the
original, so a pre-body piece `[0x17e08c,0x17e0da)` would have been given
`r5` — exactly what the plain build does. The original therefore has either
no split at bb.242 or a piece that spans `0x17e12a`; in both cases the
allocator's `r5` is unavailable for it.

### 2.2 The body's `Globals::status` uses are *not* what creates the split

Deleting the m23 body's final line
(`reinterpret_cast<long long &>(Globals::status->field_0x100) = Globals::status->getPlayingTime();`)
still leaves the condition block in `r5` (`plain_nofinal`, 11416 B,
`ldr r5,[r0,#0] / ldr r0,[r5,#0]`). Replacing the body's two uses with a
body-local alias (`Status *sb = Globals::status;`) recolours everything
(11520 B). So the split at bb.242 is a property of the whole `%2932` live
range, not of the body's use sites.

## 3. Variants measured this session

Object-level (`/dev/shm/w6911/cc.sh` = the exact clang 7.0.2 line;
`/dev/shm/w6911/vb.py` compiles a variant and prints
`size + first 7 instructions of the idle block`). Baseline row is the current
source.

| variant (source shape) | size | idle block at the ccm load |
|---|---:|---|
| **baseline: `Status *st` at top, used at ccm + `getPlayingTime`** | **11440** | `ldr.w r9,[r0] / ldr.w r5,[r9] / mov r0,r5` |
| `st` at top, used at ccm + pt, `Status *const` | 11440 | same |
| `st` at top, used at ccm + pt, `auto *` | 11440 | same |
| `Status &st = *Globals::status`, used at ccm + pt | 11440 | same |
| `Status *st` + `Status *s0 = st;` (two locals) | 11440 | same |
| `Status *st` + `Status &r = *st;` (ref alias) | 11440 | same |
| `(*st).getCurrentCampaignMission()` / `(*st).getPlayingTime()` | 11440 | same |
| `static_cast<Status *>(st)->...` at ccm | 11440 | same |
| `23 == Globals::status->getCurrentCampaignMission()` (yoda) | 11440 | same |
| `3600000 < st->getPlayingTime() - ...` | 11440 | same |
| parenthesised `(st->getPlayingTime() - ...) > 3600000` | 11440 | same |
| `bool m23 = ...; if (m23 && ...)` (boolvar) | 11440 | same |
| `st` used at ccm + `field_0x100` only (`ccm_field`) | 11436 | `ldr r5,[r9] / mov r0,r5` … `ldrd r2,r3,[r5,#256]` (folds) |
| `st` used at pt + `field_0x100` only (`pt_field`) | 11436 | same fold |
| `st` used at mission + pt (3 uses: ccm+mission+pt) | 11436 | `ldr.w r9,[r0] / ldr.w r5,[r9] / mov r0,r5` |
| `st` used at ccm only (`ccm_only`) | 11392 | `ldr r5,[r0,#0] / ldr r0,[r5,#0]` (anchor loses r5) |
| `st` used at ccm + mission (`ccm_mission`) | 11396 | `ldr r5,[r0,#0] / ldr r4,[r5,#0]` |
| `st` declared before the `m_nScriptTime > 5000` if | 11392 | `mov r0,r5` … `ldr r0,[sp,#36]` (spilled) |
| `st` declared before the `elapsedTime > 5000` if | 11392 | same |
| nested `if (ccm == 23) { if (!isMining() && …) {` | 11400 | `ldr r5,[r0,#0] / ldr r0,[r5,#0]` |
| `!isMining() && Globals::status->getCurrentCampaignMission() == 23` (reorder) | 11516 | different block entirely |
| plain source, no `st` at all (control) | 11392 | `ldr r5,[r0,#0] / ldr r0,[r5,#0]` |
| plain source, body's final strd deleted (`plain_nofinal`) | 11416 | `ldr r5,[r0,#0] / ldr r0,[r5,#0]` |
| plain source, body uses a body-local `sb` | 11520 | recoloured |

Every `st`-anchor spelling that keeps the emitted block size at 11440 lands on
the *identical* 41-byte shape (`ldr.w r5,[r9] / mov r0,r5` at the ccm load and
`mov r0,r5` at the `getPlayingTime` load). `pt_only` and `st_top_ccm_pt` are
byte-for-byte the same object code as the baseline.

## 4. Why the anchor family is capped at 41 bytes

An anchor `Status *st = Globals::status;` placed before the condition is
machine-CSE'd with the ccm load, so its byte budget is

```
+4   the ldr.w r5,[r9] that replaces the ccm ldr.w r0,[r9]
+2   the mov r0,r5 the ccm call now needs
-2   per later use that replaces a 4-byte ldr.w rX,[r9] with a 2-byte mov
```

Byte-neutrality therefore needs **exactly two** uses, and the second must be
at or after the `getPlayingTime` load for the anchor to win `r5` (with only
one use the anchor loses `r5` and the whole function recolours — `ccm_only`
11392). `getPlayingTime` is the earliest such site, so the shift window is
fixed at `0x17e090..0x17e0bc` → 41 diff bytes, 1 run, two `replace` opcodes
(`2*3860/(3862+3863) = 99.935 %` → `99.9`).

A one-`replace` shape (which would round to 100.0) would need the ccm load to
stay `ldr.w r0,[r9]` while the anchor is still in `r5`; that requires the
compiler to emit `ldr.w r0,[r9] / mov r5,r0` instead of
`ldr.w r5,[r9] / mov r0,r5`. Every spelling tried (two locals, reference
alias, `(*st).`, `static_cast`, `auto`, `const`, yoda, `bool` temporary)
produces the same `ldr r5 / mov r0,r5` direction.

## 5. Structural state (unchanged — no `src/` change)

```
sections            27 (exact, both)
file size           2,187,328 B (exact, both)
.ARM.extab          unchanged by this pass (still 4 B short, Generator::createMission)
.rodata             unchanged (mission rank table is the last input)
```

## 6. Gate (this worktree, pristine source restored)

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

## 7. Commands used

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j6
```

Scratch (not committed): `/dev/shm/w6911/` — `MGame.base.cpp` (pristine TU),
`cc.sh` (exact compile line), `vb.py` (variant -> size + idle block),
`cmp.py`, `linkmeasure.sh`, `pa_st.err` (7.0.2 `-print-after-all`, `st`
build), `vrm_st.mir`, `v/*.o` (all variants above), `rea/rea.json` (Ghidra
decompilation of `MGame::OnUpdate`).
