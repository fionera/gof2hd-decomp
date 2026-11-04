# MGame::OnUpdate — idle-contact `Status*` anchor sweep (wave 108, w5620)

Status: research pass, 2026-11-04. Worktree `wt/w5620`, branch `wave108-w5620`,
base `8f98ec4d6`. **No source change landed**: the function is already at its
best measured shape (41 diff bytes, size exact, 99.9 %). This note extends
wave-97 (`mgame-onupdate-r5-cache-wave97-w4717.md`), wave-105
(`mgame-onupdate-wave105-w5312.md`), wave-106 (`mgame-onupdate-wave106-w5408.md`)
and wave-107 (`mgame-onupdate-wave107-w5520.md`) with a new, decisive
constraint on *where* an RA anchor may live, plus a batch of new measured
shapes.

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ 0x17c8d8 (offset 0 = 0x17c8d8).

## 1. Baseline (re-verified this session)

```
size 11440 11440
diffbytes 41 runs 2
0x17ba 0x17ba 1
0x17bc 0x17e3 40
```

The whole residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2767-2776`). Original vs ours:

```
original                                    ours (current source)
17e08c ldr.w r9,[r0]   ; r9 = &Globals::status
17e090 ldr.w r0,[r9]                       17e090 ldr.w r5,[r9]   ; st
17e094 blx getCurrentCampaignMission       17e094 mov   r0,r5
17e098 cmp   r0,#23                        17e096 blx   getCurrentCampaignMission
17e09a bne   17e176                        17e09a cmp   r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne   17e176
17e0a0 blx   isMining                      17e09e ldr.w r0,[sl]
17e0a4 cmp   r0,#0                         17e0a2 blx   isMining
17e0a6 bne   17e176                        17e0a6 cmp   r0,#0
17e0a8 ldr.w r0,[r9]                       17e0a8 bne   17e176
17e0ac blx   getMission                    17e0aa ldr.w r0,[r9]
17e0b0 blx   isEmpty                       17e0ae blx   getMission
17e0b4 cmp   r0,#0                         17e0b2 blx   isEmpty
17e0b6 beq   17e176                        17e0b6 cmp   r0,#0
17e0b8 ldr.w r0,[r9]                       17e0b8 beq   17e176
17e0bc blx   getPlayingTime                 17e0ba mov   r0,r5
17e0c0 ldr.w r2,[r9]                       17e0bc blx   getPlayingTime
                                           17e0c0 ldr.w r2,[r9]
```

The original has **four fresh `ldr.w rX,[r9]` loads and no cached `Status*`**;
the GOT base (`&Globals::status`) is `r9` in both. Both blocks are 52 bytes:
ours trades one 4-byte load for a 4-byte load + a 2-byte `mov`, and the second
4-byte load for a 2-byte `mov`, so the size is identical while the instruction
slots inside the block differ by one. The whole residual is that pure
greedy-RA colour choice plus the 2-byte branch-offset shift it induces.

## 2. New finding this wave: the anchor must be live *across the ccm call*

The `st` local is an RA anchor: it occupies `r5` inside the compact region and
forces `tryAssign` for the compact GOT-base piece to fail, which makes
`tryEvict` hand the compact piece `r9` (see wave 107 §2). This wave I swept
*where* the anchor may be placed and found a sharp rule:

| anchor shape (all size-exact unless noted) | size | diff | runs |
|---|---:|---:|---:|
| baseline: `st` loaded at top, used at `getPlayingTime` | **11440** | **41** | **2** |
| `st` loaded at top, used at `getMission` (play fresh) | 11396 | 5129 | 243 |
| `st` loaded at top, used at `getMission`+`getPlayingTime` | 11436 | 4982 | 300 |
| `st` loaded at top, used at `getPlayingTime` (ccm via `st`) | 11440 | 41 | 2 |
| `st` loaded at top, used at `getPlayingTime`+`field` | 11436 | 4981 | 302 |
| `st` loaded at top, used at `getPlayingTime`+`getMission` | 11436 | 4982 | 300 |
| `st` assigned at `getMission` site, used at `getPlayingTime` | 11396 | 5129 | 243 |
| `st` assigned at `getMission` site, used at `getPlayingTime`+`field` | 11392 | 5145 | 209 |
| `st` assigned at `getMission` site, used at `field` | 11392 | 5145 | 209 |
| `st` assigned at `getPlayingTime` site (comma), used at `field` | 11392 | 5148 | 208 |
| `st` assigned at `getPlayingTime` site (comma), used at `getPlayingTime`+`field` | 11392 | 5148 | 208 |
| `st` loaded at top, used at m24 ccm+getMission (m23 clean) | 11396 | 5133 | 241 |
| `st` loaded at top, used at m24 ccm only (m23 clean) | 11392 | 5148 | 208 |
| `st` loaded at top, used at m24 getMission only (m23 clean) | 11392 | 5148 | 208 |
| `st` declared before outer `elapsedTime` if, m24 ccm+getMission | 11440 | 66 | 4 |
| `st` declared before outer `elapsedTime` if, m24 ccm only | 11440 | 41 | 2 |
| `st` declared before outer `elapsedTime` if, m24 getMission only | 11440 | 41 | 2 |
| `st` declared before outer `elapsedTime` if, m24 getPlayingTime | 11248 | 4781 | 200 |
| `long long last;` + comma-assign field, `st` at `getPlayingTime` | 11440 | 66 | 3 |
| `long long last;` + assign inside subtraction, `st` at play | 11440 | 41 | 2 |
| `long long last;` + comma-assign field, no `st` | 11432 | 5006 | 277 |
| `long long last;` + assign inside subtraction, no `st` | 11392 | 5148 | 208 |
| `Status *st = *(Status *volatile *)&Globals::status;` | 11440 | 41 | 2 |
| volatile read of `Globals::status` at the ccm call | 11440 | 853 | 36 |
| `Status **`/reference/`auto`/`const` spellings (re-confirmed) | 11440 | 41 | 2 |

**The rule.** Every anchor that keeps the m23 block's four loads fresh *and*
still holds `r9` for the GOT base has `st` loaded **before the
`getCurrentCampaignMission` call** and used **at `getPlayingTime`**. If the
anchor is loaded later (at `getMission`), or used earlier (at `getMission`), the
value is no longer live across all four calls: greedy then allocates it to
`r4` (verified in the disassembly of the `st@mission` variant: `ldr r4,[r5,#0]`
/ `mov r0,r4`, GOT base back in `r5`), the compact piece's `tryAssign` finds
`r5` free, and the whole function re-colours to the no-`st` shape (11392 B).

An anchor placed in the **m24** block cannot help either: the GOT-base split is
decided before the m24 block's values are assigned, so the compact piece still
takes `r5` (11396/11392 B).

A volatile read of `Globals::status` at the ccm call does force a genuinely
fresh 4-byte load (`ldr.w r5,[r9]` / `ldr.w r0,[r9]` / … / `mov r0,r5`), but the
extra live value re-colours the entire function: 853 diff bytes in 36 runs.

## 3. Why LLVM 7.0.1 debug dumps cannot predict the 7.0.2 decision

The NDK r18b clang is `Android (4751641 based on r328903) clang version 7.0.2`
built from AOSP `toolchain/llvm` at `1d739ffb0366421d383e04ff80ec2ee591315116`
(LLVM 7.0.2svn); it is a release build, so `-debug-only=regalloc` is compiled
out. `-print-after-all` works and was used here (`/dev/shm/w5620/dumpmir.sh`).

The LLVM 7.0.1 assertions build at `/dev/shm/wf213/llvm701build/bin/clang++`
does print the greedy cost tables, but it is **not** faithful for this
decision. For the *no-`st`* source it reports

```
$r4	static = 0.001131664868
$r5	static = 0.01162447788   worse than $r4
$r8	static = 0.004806068686  worse than $r4
$r9	static = 0.0008871843236
Best split candidate of vreg %1052 ... Split for $r9 in 13 bundles
```

i.e. 7.0.1 picks `$r9` (cost 0.000887) because `$r5` costs 0.0116. The real
7.0.2 build picks `$r5` for the same source (11392 B), so its `$r5` cost must be
below 0.000872; the wave-106 table (`$r5 = 0.0004561142685`) is consistent with
that. The two versions differ by ~25× in the compact-region `$r5` cost, so the
7.0.1 dumps are only structural hints.

An attempt to build a true 7.0.2 with assertions was started but abandoned:
there is no official 7.0.2 release tarball (7.0.2 is an AOSP/NDK version
string), and the trunk snapshot copied to `/dev/shm/w5620/llvm702src` is
`7.0.0svn` and incomplete (missing `llvm/Support/VersionTuple.h`).

## 4. Machine-IR facts confirmed this wave (7.0.2 `-print-after-all`)

Pre-RA MIR at `bb.238` (baseline and no-`st` are identical here apart from
`%128`'s live range):

```
42000B  %1052:tgpr = tLDRLIT_ga_pcrel @_ZN7Globals6statusE
42016B  %1053:gpr  = t2LDRi12 %1052, 0        ; GOT entry = &Globals::status
42032B  %128:gpr   = t2LDRi12 %1053, 0        ; Globals::status
42064B  $r0 = COPY %128
42080B  tBL getCurrentCampaignMission
...
42352B  %1059:gpr  = t2LDRi12 %1053, 0        ; fresh load for getMission
...
42592B  $r0 = COPY %128                        ; getPlayingTime (baseline only)
42688B  %1066:gpr  = t2LDRi12 %1053, 0        ; fresh load for field
```

The no-`st` VRM dump shows the GOT entry itself in `r5` and live across the
whole block (`bb.239/240 liveins: $r5, $r6, $r10, $r11`), with 2-byte
`ldr rX,[r5]` at every `Globals::status` use — that is the 48-byte shrinkage.

## 5. What is proven vs hypothesis

Proven:

* The residual is exactly the m23 `Status*` cache decision; every other byte of
  the 11440-byte function is identical, and both blocks are 52 bytes.
* The original IR is the no-`st` shape (four distinct loads, GOT base `r9`).
* The `st` local is an RA anchor, and the anchor only works when it is live
  across the `getCurrentCampaignMission` call **and** used at
  `getPlayingTime`. Loading it later, or using it at `getMission`, drops it to
  `r4` and re-colours the whole function.
* An m24-block anchor is too late (the split is already decided).
* A volatile ccm re-read forces the fresh load but re-colours the function
  (853 diff).
* The 7.0.1 assertions build is not faithful for this split (`$r5` cost 25×
  higher than 7.0.2).

Hypothesis (not proven): the original source contains a code-neutral IR
difference elsewhere (a different vreg numbering / live-range that raises the
7.0.2 compact-region `$r5` cost above `$r9`'s 0.000872) without adding an
instruction. No C++ construct producing that was found; every anchor that
leaves the m23 loads fresh costs at least one 2-byte `mov` somewhere in the
compact region, and the only byte-neutral placement is the current baseline.

## 6. Gate (note-only pass)

`check` green with the pristine source restored:

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.91
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

Before -> after: byte 4500 -> 4500, linked 4500 -> 4500. No ELF section,
`.ARM.extab` (0x26298) or file-size change was introduced.

## 7. Commands used

```bash
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w5620/fdiff.py _ZN5MGame8OnUpdateEv      # size / diffbytes / runs
python3 /dev/shm/w5620/try2.py /dev/shm/w5620/b_*.json    # multi-edit variant sweep
bash /dev/shm/w5620/dumpmir.sh                            # -print-after-all MIR
bash /dev/shm/w5620/ra701.sh out.txt                      # LLVM 7.0.1 -debug-only=regalloc
cmake --build cmake-build-match --target check -j6
```

Scratch drivers in `/dev/shm/w5620`: `fdiff.py`, `measure.sh`, `try2.py`,
`vtest.py`, `dumpmir.sh`, `ra701.sh`, `gen_batch*.py`, `b_*.json`,
`mg_all.txt` (7.0.2 `-print-after-all`, 1.1 M lines), `ra_nost2.txt`
(7.0.1 `-debug-only=regalloc`).
