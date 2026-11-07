# MGame::OnUpdate — idle-contact `Status*` residual, wave 129 / w7661

Status: **research pass, no `src/` change landed.** Worktree `wt/w7661`, base =
master `fee528cbe` (wave-128 snapshot). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ `0x17c8d8`. Best measured shape is
still the pristine source (the `Status *st` anchor at the mission-23 idle
check): **11440 B, 41 diff bytes, one run, 99.9 %**. This note extends
`mgame-onupdate-wave126-w7617.md` / `...-wave123-w7318.md` /
`...-wave119-w6956.md` / `...-r5-cache-wave97-w4717.md` with a new
**operand-order** family (`rev2`) and a fresh batch of order-preserving
micro/nested/local variants. Nothing beats 41 bytes.

## 1. Baseline (re-verified this session)

```
orig 11440 0x17c8d8   ours 11440 0x17c8d8
diff bytes 41  runs 1   (function-relative 0x17bb..0x17e3,
                         absolute 0x17e093..0x17e0bb)
```

Residual is the mission-23 idle-contact condition
(`src/game/menu/MGame.cpp:2766-2772`). Original vs ours (`r9` =
`&Globals::status`):

```
original                                   ours (pristine source)
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

The original has **four fresh `ldr.w rX,[r9]` loads** and no anchor. The
baseline anchor is byte-neutral in the condition (`ldr r5` + two `mov` =
`4+2+2 = 8 B` vs two fresh `ldr r0,[r9]` = `4+4 = 8 B`) but it forces the
`&Globals::status` GOT base onto `r9` for the whole if/else chain. That is
worth +48 B across the chain: every other `Globals::status` load in the
m24/m122/hints code is `ldr.w rX,[r9]` (4 B) instead of `ldr rX,[r5]`
(2 B). Plain source is 11392 B precisely because it loses that +48.

## 2. Ghidra ground truth (plain source)

Correct Ghidra address is **`0x18c8d8`** (image base `0x10000`); `0x17c8d8`
resolves to `SpaceLounge::startChat`. Command:

```bash
GHIDRA_INSTALL_DIR=/tmp/ghidra_12.1.2_PUBLIC \
rea decompile /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so 0x18c8d8 --format md \
  > /dev/shm/w7661_decomp2.md
```

Output (lines ~858-866) confirms the original C is plain, with no local
`Status*` anchor:

```c
iVar5 = Status::getCurrentCampaignMission(Globals::status);
if ((iVar5 == 0x17) && (iVar5 = PlayerEgo::isMining(*(PlayerEgo **)pMVar1), iVar5 == 0)) {
  pMVar14 = (Mission *)Status::getMission(Globals::status);
  iVar5 = Mission::isEmpty(pMVar14);
  if (iVar5 == 0) goto LAB_0018e176;
  lVar39 = Status::getPlayingTime(Globals::status);
  iVar5 = (int)((ulonglong)(lVar39 - *(longlong *)(Globals::status + 0x100)) >> 0x20);
...
```

So the original really is `Globals::status->...` at all four sites, in the
order ccm, isMining, mission/isEmpty, pt, field.

## 3. Why the anchor is load-bearing (MIR recap)

`-mllvm -print-after-all` (scratch logs below) shows the
`&Globals::status` GOT base is a **region-split** range at the m23 body
(`bb.242`): it is live through the body but has no use until the shared
tail. The split creates a COPY at the body entry.

* Baseline: the anchor `%128` is a global range (ccm..pt), takes `r5`;
  the GOT-base pre-body piece takes `r9`; the body piece also takes `r9`,
  so the split COPY coalesces away (`%2930 = COPY %2931` becomes a no-op).
* Plain: no anchor, so the GOT-base **pre-body piece takes `r5`** and the
  body piece takes `r9`; the split COPY survives as `mov r9,r5` at the
  body entry (`0x17e0d2` in the object dump), and the whole chain recolours
  to `r5` (11392 B).

The wave-126/123 conclusion stands: the anchor is the only source spelling
that reliably forces the pre-body piece to `r9`, and it necessarily adds
the two `mov r0,r5` (ccm, pt).

## 4. NEW: the `rev2` operand-order family

The register choice is **not** unique to the anchor. Moving the
mission/isEmpty check to the *end* of the `&&` chain gives the original's
four fresh `ldr.w r0,[r9]` loads with `r9` and **no split and no anchor**:

```cpp
if (Globals::status->getCurrentCampaignMission() == 23 && !this->player->isMining() &&
    Globals::status->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
    3600000 &&
    Globals::status->getMission()->isEmpty()) {
```

Object-level block (`rev2b`, size **11436 B**):

```
17b4 ldr.w r9,[r0]        ; GOT base
17b8 ldr.w r0,[r9]        ; ccm        MATCH
17bc blx  ccm             MATCH
17c0 cmp  r0,#23          MATCH
17c2 bne                  MATCH
17c4 ldr.w r0,[sl]        ; isMining   MATCH
17c8 blx  isMining        MATCH
17cc cmp  r0,#0           MATCH
17ce bne                  MATCH
17d0 ldr.w r0,[r9]        ; pt         (original has mission here)
17d4 blx  getPlayingTime
17d8 mov  r2,r0           ; extra (pt result must survive the field load)
17da ldr.w r0,[r9]        ; field
17de ldr  r4,[pc]         ; 3600000
17e0 ldrd r3,r5,[r0,#256]
17e4 subs r2,r2,r3
...
17f4 blx  getMission
17f8 blx  isEmpty
17fc cmp  r0,#0
17fe beq
```

No `mov r0,r5`, no split copy, `r9` throughout. The MIR confirms the GOT
base is a **single unsplit interval** in `$r9` (no COPY at the body). The
shape is 4 B short of 11440 because the reordered comparison needs the
extra `mov r2,r0` (the pt result must be preserved across the field load)
and the mission branch is a separate block. It is *not* the original
because the original evaluates mission/isEmpty **before** pt (disasm
`0x17e0a8` mission, `0x17e0b8` pt), i.e. the source order is
ccm, isMining, mission, pt, field.

This is a genuinely new lever: the `r9` choice tracks the **last
`Globals::status` use in the condition**, not the anchor per se. When the
last use is a call (`getMission`), the whole interval stays in `r9`; when
the last use is the field `ldrd` load, the allocator splits at the body
and the pre-body piece falls to `r5`. The original has the field load
last *and* `r9`, so it must contain something else that keeps the interval
unsplit.

## 5. Order-preserving variants measured this session (object level)

All snippets replace only the condition block (pristine TU otherwise).

| variant | size | ccm load | GOT base |
|---|---:|---|---|
| **baseline `Status *st` at ccm+pt** | **11440** | `ldr r5,[r9] / mov r0,r5` | r9 |
| plain (no anchor) | 11392 | `ldr r0,[r5]` | r5 (split) |
| `rev2b` mission check last | 11436 | `ldr.w r0,[r9]` | **r9, unsplit** |
| `rev2c` same, one line | 11436 | `ldr.w r0,[r9]` | r9 |
| `rev2d` `!isEmpty()==false` last | 11436 | `ldr.w r0,[r9]` | r9 |
| nested `if (ccm&&!mining&&empty){ if (pt-field>...)` | 11396 | `ldr r0,[r5]` | r5 |
| nested `if (ccm){ if (!mining&&empty&&pt...)` | 11400 | `ldr r0,[r5]` | r5 |
| nested `if (ccm&&!mining){ if (empty&&pt...)` | 11416 | `ldr r0,[r5]` | r5 |
| nested + `bool empty` local | 11416 | `ldr r0,[r5]` | r5 |
| nested + `Mission *m` local | 11416 | `ldr r0,[r5]` | r5 |
| nested + `Status *st` local | 11416 | `ldr r0,[r5]` | r5 |
| nested + `long long t` local | 11396 | `ldr r0,[r5]` | r5 |
| `(empty = getMission()->isEmpty())` in condition | 11392 | `ldr r0,[r5]` | r5 |
| `(t = getPlayingTime())` in condition | 11392 | `ldr r0,[r5]` | r5 |
| `(f = field)` in condition | 11392 | `ldr r0,[r5]` | r5 |
| `(c = ccm)` in condition | 11392 | `ldr r0,[r5]` | r5 |
| `(p = this->player)` in condition | 11392 | `ldr r0,[r5]` | r5 |
| `(st = Globals::status)->field` in condition | 11392 | `ldr r0,[r5]` | r5 |
| `(st = Globals::status)->pt` in condition | 11392 | `ldr r0,[r5]` | r5 |
| `&& true` / `&& 1` / extra parens / `!!` | 11392 | `ldr r0,[r5]` | r5 |
| `3600000 < pt - field` | 11392 | `ldr r0,[r5]` | r5 |
| `3600000LL` | 11392 | `ldr r0,[r5]` | r5 |
| ternary wrapper | 11392 | `ldr r0,[r5]` | r5 |
| `long long idle = pt - field; if (... && idle > 3600000)` | 11452 | (hoisted before ccm) | r9 |
| `Status *st` at all four sites | 11432 | `ldr r5,[r9] / mov r0,r5` | r9 |
| `Status *st` at ccm+pt, `st` via `(st=...)` comma | 11440 | `ldr r5,[r9] / mov r0,r5` | r9 |
| `Status *st` at ccm+pt, ccm via `Globals::status` | 11440 | `ldr r5,[r9] / mov r0,r5` | r9 |
| `Status *st` at ccm+pt, pt via `(st=Globals::status)` | 11440 | `ldr r5,[r9] / mov r0,r5` | r9 |
| `Status *st` at ccm+field / mission+field | 11436 | `ldr r5,[r9] / mov r0,r5` | r9 |
| `Status *st` at ccm+mission | 11396 | `ldr r5,[r9] / mov r0,r5` | r9 |
| `Status *st` at ccm only | 11392 | `ldr r0,[r5]` | r5 |
| `PlayerEgo *p` cached, m23 isMining only | 11396 | `ldr r0,[r5]` | r5 |
| `PlayerEgo *p` cached, m23+m24 isMining | 11396/11440 | (see wave 123 `p24`) | r9 |
| `volatile Status *st` | 11340 | `ldr r0,[r5]` | r5 |
| `Status *&st` / `Status **stp` | 11392 | `ldr r0,[r5]` | r5 |
| reordered `!isMining() && ccm==23 && ...` | 11516 | (different block) | r9 |

Micro-variants of the individual operands (`23 == ccm`, `ccm-23==0`,
`!isEmpty()`, `(bool)isEmpty()`, `isEmpty()==true`, `isEmpty()!=0`,
`isMining()==false`, `isMining()==0`, `false==isMining()`, etc.) were swept
(72 combinations) and **all land on 11392 / r5**. The only order-preserving
shape at 11440 is still the anchor.

## 6. Conclusion / open question

The 41-byte / one-run residual is still a hard local optimum for every
order-preserving source spelling tried (waves 97/105/106/107/108/112/119/
123/126/129, now 300+ measured shapes). The new `rev2` family proves the
`r9` GOT base is reachable **without an anchor** when the last
`Globals::status` use in the condition is a call; the original has the
field `ldrd` last and still keeps `r9`, so the remaining unknown is what
keeps the interval unsplit in the original. Candidate untested levers:

1. The cross-TU `src/game/core/Globals.h` lever (make the
   `&Globals::status` GOT load non-rematerialisable / change its
   allocation hints). It affects all 331 `Globals::status` sites and is a
   full-gate pass of its own.
2. A source construct in the *body* that adds a `Globals::status` use
   inside `bb.242` (breaking the compact region) without changing the
   emitted body bytes — e.g. a tail-merged store written differently.
   The original body has no such use (`0x17e0da`..`0x17e174`), so this
   would have to be an IR-level effect, not an extra load.

## 7. Gate (pristine source restored)

```
cmake --build cmake-build-match --target check -j6
  verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.94
  data:   exact 843 of 843 exported objects
  sodiff: extra-beyond-allow 0  missing 0  allowed 0  imports: 207
  RATCHET PASS      (goal_lint CLEAN)
```

Before -> after: byte 4506 -> 4506, linked 4506 -> 4506. Target unchanged at
11440 B / 41 diff bytes / 99.9 %.

## 8. Commands used

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

Scratch (not committed), all under `/dev/shm/w7661/`:

* `MGame.pristine.cpp` / `MGame.base.cpp` — exact baseline TU
* `vtest.py`, `vtest3.py`, `full.py` — object/full-link variant harnesses
* `batch.py`..`batch8.py` — the variant sweeps above
* `x_*.cpp` / `y_*.cpp` — generated variant TUs
* `base_all.log`, `plain_all.log`, `mir1.log` — `-mllvm -print-after-all`
* `base_greedy_onupdate.mir`, `base_novregs_onupdate.mir`,
  `plain_greedy_onupdate.mir`, `rev2b_novregs.mir` — extracted OnUpdate MIR
* `RegAllocGreedy.cpp` — LLVM 7.0.2 greedy allocator source
