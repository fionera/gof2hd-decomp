# MGame::OnUpdate — idle-contact `Status*` residual, wave 126 / w7617

Status: **research pass, no `src/` change landed.** Worktree `wt/w7617`, base =
master `bf365a964` (wave-125 final snapshot). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ `0x17c8d8`. Best measured shape is
still the pristine source (the `Status *st` anchor at the mission-23 idle
check): **11440 B, 41 diff bytes, one run, 99.9 %**. This note extends
`mgame-onupdate-wave123-w7318.md` / `...-w6956.md` / `...-w6938.md` with a
fresh 7.0.2 `-print-after-all` MIR trace of the idle block, the exact
`isReMaterializable` property of the ARM GOT load, and a new two-anchor family
that reproduces the original's plain `ccm`/`isMining` loads but still cannot
beat 41 bytes.

## 1. Baseline (re-verified this session)

```
orig 11440 0x17c8d8   ours 11440 0x17c8d8
diff bytes 41  runs 1   (function-relative 0x17bb..0x17e3)
```

Residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2766-2772`). Original vs ours (`r9` =
`&Globals::status`):

```
original                                   ours (current source)
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

## 2. New MIR evidence (7.0.2 `-print-after-all`, filtered to OnUpdate)

The pre-RA block (`Live Register Matrix`, dump line 855769 of the scratch log)
shows the idle block is bb.238 and the body is bb.242:

```
41984B bb.238 (%ir-block.1355):     ; idle condition
42000B   %1052:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6statusE
42016B   %1053:gpr  = t2LDRi12 %1052, 0        ; GOT base
42032B   %128:gpr   = t2LDRi12 %1053, 0        ; st (anchor)
...
42848B bb.242 (%ir-block.1375):     ; m23 body
42856B   %2930:gpr = COPY %2931:gpr            ; split copy at body entry
```

After greedy (dump line 865884, post-RA):

```
42016B renamable $r9 = t2LDRi12 ... got        ; GOT base
42032B renamable $r5 = t2LDRi12 $r9, 0         ; st
42064B $r0 = COPY $r5                           ; ccm arg
42608B $r0 = COPY killed $r5                    ; pt arg
42720B renamable $r4 = tLDRpci %const.17        ; 3600000
```

So the anchor `%128` is **r5**, not r4, even though r4 is free at 42032. The
`3600000` constant is materialised in **r4** at 42720 (inside the same block
bb.241, after the anchor's last use at 42608), and the anchor's live range
still spans bb.238..bb.241; the allocator's order is `r4 r5 r6 r8 r9 r10 r11`,
so r4 is the first free register *only if* the anchor's range does not reach
the constant. In the baseline it does, so the anchor wins r5 and r4 stays for
the constant. That is why the baseline anchor (declared before ccm, CSE'd with
the ccm load) is the only spelling that reliably forces the GOT base to r9.

The split at bb.242 is a **compact-region split** (`calcCompactRegion` /
`splitAroundRegion` in `RegAllocGreedy.cpp`): `%1053` is live *through*
bb.242..bb.246 (the m23 body) with no use until the shared tail bb.247
(`0x17e26c ldr.w r0,[r9]`). The compact region is isolated to a separate
interval; the pre-body piece is the "remainder". In the anchor build both
pieces get r9 and the COPYs coalesce away; in the plain build the pre-body
piece gets r5 and the body piece r9, producing the `mov r9,r5` copies at
`0x17fa`/`0x18da` and the whole-function recolour.

## 3. The GOT load is `isReMaterializable`

`grep isReMaterializable lib/Target/ARM/ARMInstrInfo.td` shows the `AI_ldr1`
multiclass (which defines `t2LDRi12`) is inside a
`let canFoldAsLoad = 1, isReMaterializable = 1 in { ... }` block. So
`%1053 = t2LDRi12 %1052, 0` is marked rematerialisable. It is nevertheless
kept live in r9 and the split is emitted as a COPY, not a re-load, so
rematerialisation is not what creates the r5/r9 difference. The wave-112
"make the GOT load non-rematerialisable" hypothesis therefore has no direct
instruction-level lever in `Globals.h`: the flag is on the instruction, not on
the global. (A `volatile` on `Globals::status` changes only the *value* load
`%128`, not the GOT-address load `%1053`; measured below.)

## 4. New two-anchor family (plain ccm + isMining, r9 GOT base)

Because the anchor is CSE'd with the ccm load whenever it is defined before
the `if`, the only way to keep the ccm load plain is to define the anchor
*after* the ccm call. A single post-ccm anchor takes r4 (not r5) and leaves
the GOT base in r5. Two anchors whose live ranges overlap occupy both r4 and
r5, forcing r9 with a plain ccm:

```cpp
Status *st, *st2;
if (Globals::status->getCurrentCampaignMission() == 23 &&
    (st2 = Globals::status, !this->player->isMining()) &&
    (st = Globals::status, st2->getMission()->isEmpty()) &&
    st->getPlayingTime() - reinterpret_cast<long long &>(st->field_0x100) > 3600000) {
```

Object-level block:

```
17b4 ldr.w r9,[r0]        ; GOT base   MATCH
17b8 ldr.w r0,[r9]        ; ccm        MATCH
17bc blx  ccm             MATCH
17c0 cmp  r0,#23          MATCH
17c2 bne                   (2-byte branch offset shift)
17c4 ldr.w r0,[sl]        ; isMining   MATCH
17c8 ldr.w r5,[r9]        ; st2        +4 B
17cc blx  isMining        MATCH
17d0 cmp  r0,#0           MATCH
17d2 bne                  MATCH
17d4 mov  r0,r5           ; mission    -2 B
17d6 ldr.w r4,[r9]        ; st         +4 B
17da blx  getMission      MATCH
17de blx  isEmpty         MATCH
17e2 cmp  r0,#0           MATCH
17e4 beq                  MATCH
17e6 mov  r0,r4           ; pt         -2 B
17e8 blx  getPlayingTime  MATCH
17ec ldrd r2,r3,[r4,#256] ; field      -4 B
```

Full-link measurement: **11440 B, 41 diff bytes, 2 runs** (baseline: 11440 B,
41 diff bytes, 1 run). The ccm and isMining instructions are now
instruction-identical to the original, but the two extra anchor loads shift
the block and the run structure is worse (2 runs vs 1). It is not kept.

Single post-ccm anchors that do *not* overlap (`mission_field`, `mission_pt`,
`after_ccm_ptfield`, `after_ccm_mptf`, `after_mining_ptfield`,
`after_mission_ptfield`) all leave the GOT base in r5 (the anchor takes r4),
i.e. they fall back to the 11392-byte plain shape or a 11436/11438 variant.

## 5. Variants measured this session (object level, `block.py`)

| variant | GOT base | anchor reg | ccm load |
|---|---|---|---|
| **baseline `Status *st` at ccm+pt** | **r9** | **r5** | `ldr r5,[r9] / mov r0,r5` |
| `Status *&st` (reference alias) | r5 | — | `ldr r0,[r5]` |
| `Status **stp = &Globals::status` | r5 | — | `ldr r0,[r5]` |
| `Status *volatile &st` | r5 | — | `ldr r0,[r5]` |
| volatile value anchor at pt | r9 | r5 | `ldr r5,[r9] / mov r0,r5` |
| volatile value anchor at pt+field | r9 | r5 | `ldr r5,[r9] / mov r0,r5` |
| anchor at ccm+field | r9 | r5 | `ldr r5,[r9] / mov r0,r5` |
| anchor at mission+pt | r9 | r5 | `ldr r5,[r9] / mov r0,r5` |
| anchor at ccm+mission | r5 | r4 | `ldr r4,[r5] / mov r0,r4` |
| `after_ccm_pt` | r5 | r4 | `ldr r0,[r5]` |
| `after_ccm_ptfield` | r5 | r4 | `ldr r0,[r5]` |
| `after_ccm_mission_pt` | r5 | r4 | `ldr r0,[r5]` |
| `after_ccm_mptf` | r5 | r4 | `ldr r0,[r5]` |
| `after_mining_ptfield` | r5 | r4 | `ldr r0,[r5]` |
| `after_mission_ptfield` | r5 | r4 | `ldr r0,[r5]` |
| `mission_field` (two anchors) | r5 | r4 | `ldr r0,[r5]` |
| `mission_pt` (one anchor) | r5 | r4 | `ldr r0,[r5]` |
| **`two_overlap` (two anchors, overlap)** | **r9** | **r5/r4** | **`ldr r0,[r9]`** |

## 6. Conclusion

The 41-byte / one-run residual is still a hard local optimum. The original
shape — four fresh `Globals::status` loads from a continuous r9 GOT base, no
anchor, no split copies — needs the pre-body piece of `%1053` to be coloured
r9 without a value occupying r5 in bb.238..bb.241. Every source spelling that
occupies r5 (the anchor family) adds a `mov` at ccm; every spelling that
leaves r5 free recolours the whole function to r5 (11392 B). The two-anchor
family is the first shape that reproduces the plain ccm and isMining loads and
the r9 GOT base, but its two extra loads still cost 41 bytes and split the run
in two. The only untested lever remains the cross-TU one (change the
`&Globals::status` GOT-load attributes in `src/game/core/Globals.h`), which
affects all 331 `Globals::status` sites and is a full-gate pass of its own.

## 7. Gate (pristine source restored)

```
cmake --build cmake-build-match --target check -j8
  verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports 207
  RATCHET PASS
```

Before -> after: byte 4506 -> 4506, linked 4506 -> 4506. Target unchanged at
11440 B / 41 diff bytes / 99.9 %.

## 8. Commands used

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5MGame8OnUpdateEv
cmake --build cmake-build-match --target check -j8
```

Scratch (not committed): `/dev/shm/w7617/` — `MGame.plain.cpp` (plain TU),
`MGame.anchor.cpp` (pristine TU), `cc.sh` (exact clang 7.0.2 line),
`block.py` (object idle-block dump), `measure.py` (linked diff bytes/runs),
`gen*.py` + `v_*.cpp` (the variant sweeps above), `plain_all.log` /
`anchor_all.log` (7.0.2 `-print-after-all`, filtered to OnUpdate).
