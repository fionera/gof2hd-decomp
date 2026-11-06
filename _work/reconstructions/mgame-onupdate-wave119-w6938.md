# MGame::OnUpdate — mission-23 idle-contact `Status*` residual, wave 119 / w6938

Status: **research pass, no `src/` change landed.** Worktree `wt/w6938`, branch
`wave119-w6938`, base = current master (`98f531de9`). Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5MGame8OnUpdateEv`, 11440 B @ `0x17c8d8`. Best measured shape is
still the pristine source (the `Status *st` anchor at the mission-23 idle
check): **11440 B, 41 diff bytes, one run, fuzzy 99.9 %,
3862/3863 normalised instructions**. This note extends
`mgame-onupdate-wave119-w6911.md`, `...-w6726.md` and `...-w6766.md` with a
new object-level sweep (60+ further source shapes), the exact linked
disassembly of both streams, and a corrected account of *why* the anchor
family is capped.

## 1. Baseline re-verified

```
orig 11440B ours 11440B  pct 99.9  diffbytes 41  runs 1  ninsn 3862/3863
run 0x17bb..0x17e4   (absolute 0x17e093 .. 0x17e0bc)
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

Linked disassembly, original vs ours (`r9` = `&Globals::status`, the GOT base):

```
original                                   ours
17e088 ldr  r0,[pc,#872]                   17e088 ldr  r0,[pc,#872]
17e08a add  r0,pc                          17e08a add  r0,pc
17e08c ldr.w r9,[r0]                       17e08c ldr.w r9,[r0]
17e090 ldr.w r0,[r9]      ; ccm           17e090 ldr.w r5,[r9]   ; st
17e094 blx  ccm                            17e094 mov  r0,r5
17e098 cmp  r0,#23                         17e096 blx  ccm
17e09a bne  17e176                         17e09a cmp  r0,#23
17e09c ldr.w r0,[sl]                       17e09c bne  17e176
17e0a0 blx  isMining                       17e09e ldr.w r0,[sl]
17e0a4 cmp  r0,#0                          17e0a2 blx  isMining
17e0a6 bne  17e176                         17e0a6 cmp  r0,#0
17e0a8 ldr.w r0,[r9]      ; mission        17e0a8 bne  17e176
17e0ac blx  getMission                     17e0aa ldr.w r0,[r9]
17e0b0 blx  isEmpty                        17e0ae blx  getMission
17e0b4 cmp  r0,#0                          17e0b2 blx  isEmpty
17e0b6 beq  17e176                         17e0b6 cmp  r0,#0
17e0b8 ldr.w r0,[r9]      ; pt arg         17e0b8 beq  17e176
17e0bc blx  getPlayingTime                 17e0ba mov  r0,r5
17e0c0 ldr.w r2,[r9]      ; field           17e0bc blx  getPlayingTime
17e0c4 ldr  r4,[pc,#816]                   17e0c0 ldr.w r2,[r9]
17e0c6 ldrd r2,r3,[r2,#256]                17e0c4 ldr  r4,[pc,#816]
                                           17e0c6 ldrd r2,r3,[r2,#256]
```

Both blocks are 52 B; the run is a register-field permutation plus the induced
2-byte branch-offset shift. From `0x17e176` (mission-24) to the end of the
function the two disassemblies are byte-identical.

Note the original is the **plain** shape: it reloads `Globals::status` from
`r9` at all four sites (ccm / mission / pt / field) and never caches the
`Status*`. The `Status *st` anchor in our source is a tuning device: it is
machine-CSE'd with the ccm load, producing `ldr r5,[r9] / mov r0,r5` at ccm
and `mov r0,r5` at the pt load.

## 2. Why the anchor family is capped at 41 bytes

Byte budget of the four `Globals::status` value loads in the condition:

* plain: `4 + 4 + 4 + 4 = 16 B`
* anchor with `k` uses via `mov`: `4 (anchor ldr) + 2k + 4(4-k) = 20 - 2k`

Size 11440 (the original's) therefore requires **exactly `k = 2`** cached
uses.  The anchor is always loaded before the first call after its
declaration, i.e. before the ccm call, so the ccm load is always one of the
two CSE'd uses.  The only byte-neutral pair that also keeps the anchor in a
callee-saved register is **(ccm, pt)** — the current source.  Every other pair
measured this wave either loses the anchor register (and recolours the whole
function) or folds the field access (4 B short):

| variant (object level) | size | idle block |
|---|---:|---|
| **baseline: `st` used at ccm + pt** | **11440** | `ldr.w r9,[r0] / ldr.w r5,[r9] / mov r0,r5` … `mov r0,r5` |
| `st` used at pt only (CSE'd with ccm) | 11440 | identical to baseline |
| `Status *const st` / `auto *st` / `Status &st = *Globals::status` (ccm+pt) | 11440 | identical to baseline |
| `st` used at ccm + field | 11436 | `ldr r5,[r9] / mov r0,r5` … `ldrd r2,r3,[r5,#256]` (fold) |
| `st` used at mission + pt | 11436 | `ldr r5,[r9] / mov r0,r5` ×3 |
| `st` used at ccm + mission + pt | 11436 | `mov r0,r5` ×3 |
| `st` used at ccm + mission + pt + field (`all_st`) | 11432 | `mov r0,r5` ×3, `ldrd [r5,#256]` |
| `Status &st = *Globals::status` used at all four sites | 11432 | `ldr.w r5,[r9] / mov r0,r5` ×3 |
| `st` used at ccm only | 11392 | anchor loses `r5`, whole fn recolours |
| `st` used at mission only | 11396 | anchor loses `r5` |
| plain source, no anchor (control) | 11392 | `ldr r5,[r0,#0] / ldr r0,[r5,#0]` (whole fn recolours) |
| `st` declared before the `m_nScriptTime` if | 11392 | spilled |
| `st` declared before the `elapsedTime > 5000` if | 11392 | spilled |
| `st` declared before the `startSequenceOver` block | 11336 | GOT base in `fp` |
| `st` declared before the `jumpDriveActive` if | 11372 | GOT base in `r6` |
| `st` declared before the `shouldSwitchToStandardCam` if | 11316 | GOT base in `fp` |
| `st` declared before the `isDockedToPlanet` if | 11320 | GOT base in `fp` |
| `Status *&st` / `Status **stp = &Globals::status` (all uses) | 11392 | plain shape, `r5` |
| `bool mining = this->player->isMining();` hoisted before the if | 11432 | `ldr.w r0,[r9]` at ccm (!) but isMining before ccm |
| `bool idle = <whole condition>; if (idle)` | recolours | — |
| nested `if (ccm == 23) { Status *st = ...; if (...) }` | 11400 | GOT base in `r5`, anchor in `r4` |
| `Mission *ms = Globals::status->getMission();` before the if | 11444 | getMission hoisted before ccm |
| `int ccm = Globals::status->getCurrentCampaignMission();` | 11440 | identical to baseline |
| `this->player->isMining() == false` | 11440 | identical to baseline |
| mission-122 radio branch also caches `Status *st` | 11440 | 99.8 %, 78 B, 5 runs (worse) |
| mission-24 branch also uses `st` at pt / ccm+pt | 11436 | m23 block unchanged, m24 shifts |

Full-link measurements of the non-11440 candidates are all far worse because
the whole function recolours: `all_st` 92.4 % / 5033 B / 270 runs,
`ccm_field` 92.7 % / 4981 B / 307 runs, `ccm_mission_pt` 92.5 % / 4982 B /
303 runs, plain 87.2 % / 5148 B / 212 runs.

## 3. New evidence: the original has *no* split copies at the m23 body

The mission-23 body starts at `0x17e0da` in the original with
`movs r0,#1 / strb.w r0,[fp,#93]` — no register shuffle.  The plain build,
by contrast, emits the greedy allocator's region-split copies:

```
plain build:
  17e0d2  mov r9, r5     ; pre-body piece (r5) -> body piece (r9), m23 body
  17e1b2  mov r9, r5     ; same at the m24 body
  17e256  mov r5, r9     ; restore the pre-body piece
```

`r5` is provably dead in the original from `0x17e044` (last read) to
`0x17e12a` (next write, `Globals::gameText`), so the allocator would hand the
pre-body piece `r5`.  In the original the GOT base is instead `r9`
*continuously* (`0x17e08c` … `0x17f192`), i.e. the pre-body and body pieces
were coalesced onto the same register.  The anchor is what forces that in our
build: `%128` occupies `r5` across the ccm/isMining/getMission/isEmpty calls,
so `%1053` (the GOT base) cannot take `r5` and coalesces onto `r9` with the
body piece.  The original therefore has a live value in `r5` across the
condition that the plain source does not — but that value is *not* a cached
`Status*` (the original reloads `Globals::status` from `r9` at every site).

Every spelling that keeps the 11440-byte budget lands on the identical
41-byte shape; the two `replace` opcodes are
`2*3860/(3862+3863) = 99.935 %` → `99.9`.

## 4. Structural state (unchanged — no `src/` change)

```
sections            27 (exact, both)
file size           2,187,328 B (exact, both)
.ARM.extab          unchanged by this pass (still 4 B short, Generator::createMission)
.rodata             unchanged (mission rank table is the last input)
```

## 5. Gate (this worktree, pristine source restored)

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

## 6. Commands used

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

Scratch (not committed): `/dev/shm/w6938/` — `MGame.base.cpp` (pristine TU),
`cc.sh` (exact compile line), `bb.py` (variant -> size + idle block),
`fullmeas.py` (variant -> full-link measurement), `measure.py`, `gen1..gen8.py`
+ `g1..g8.json` (the variant sweeps above), `all.mir` / `plain.mir` +
`gra_debug_all.txt` / `plain_trace.txt` (llc 7.0.0svn greedy traces; note the
trace's final colour for `%1053` is `r4`, which does **not** match the 7.0.2
object, so the trace is only indicative).
