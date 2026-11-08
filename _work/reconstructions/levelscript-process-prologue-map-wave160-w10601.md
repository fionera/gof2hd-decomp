# LevelScript::process — wave 160 / w10601: prologue / callee-saved map

Target `_ZN11LevelScript7processEi` (`LevelScript::process`), original
`0x137d18`, **57,232 B (0xdf90)**, size-exact.  Owned TU
`src/game/world/LevelScript.cpp`.  Worktree `wt/w10601`, base master
`08af4633b` (wave 159).  Scratch `/dev/shm/w10601/`.

Result: **no source win landed**.  The current base is already the best
size-exact shape on both metrics.  This note records the exact
callee-saved permutation of the original and of every measured size-exact
shape, and why the `this=sl` prologue cannot be reached without changing
the frame size.

```
                         base (landed)      original
process raw diffbytes    55,055             —
process aligned insns    17,059             —
orig / ours size         57,232 / 57,232   57,232
frame                    432                432
.text diffbytes          85,722             —
byte_exact               4507               4507
linked_exact             4507               4507
size-exact               4524/4524          4524/4524
data_exact               843/843            843/843
imports                  207                207
lint                     CLEAN ✓            CLEAN ✓
RATCHET                  PASS               PASS
```

## 1. The original callee-saved map (from the binary)

Prologue (objdump of the original, image base 0x10000):

```
137d18  push {r4,r5,r6,r7,lr}
137d1a  add  r7, sp, #12
137d1c  stmdb sp!, {r7,r8,r9,sl,fp}
137d20  vpush {d8-d13}
137d24  sub  sp, #432
137d26  mov  sl, r0              ; this            -> sl (r10)
137d28  ldr  r0, [pc,#888]       ; &__stack_chk_guard
137d2a  mov  r4, sl              ; r4 = this
137d2c  mov  r6, r1              ; delta           -> r6
137d2e  add  r0, pc
137d30  ldr.w fp, [r0]           ; &__stack_chk_guard -> fp (r11)
137d34  ldr.w r0, [fp]           ; canary value
137d38  str  r0, [sp,#428]       ; canary          -> [sp,#428]
137d3a  ldr.w r0, [r4,#24]!      ; &m_pLevel       -> r4, [sp,#80]
137d3e  blx  Level::getMessages  ; messages        -> r5
137d44  ldr  r0, [r4]
137d46  blx  Level::getPlayer    ; player          -> [sp,#84]
137d4e  ldr  r0, [r4]
137d50  blx  Level::getEnemies   ; enemies         -> r8
137d56  ldr  r0, [pc,#848]
137d5a  add  r0, pc
137d5a  ldr  r4, [r0]            ; &Globals::status -> r4, [sp,#92]
137d5c  ldr  r0, [r4]            ; Status* reloaded per use (NOT cached)
137d5e  blx  Status::getCurrentCampaignMission
```

| value | register | spill |
|---|---|---|
| `this` | **sl (r10)** | `[sp,#88]` |
| `&Globals::status` | **r4** | `[sp,#92]` |
| canary base (`&__stack_chk_guard`) | **fp (r11)** | — |
| canary value | r0 -> `[sp,#428]` | `[sp,#428]` |
| `&m_pLevel` | r4 (pre-inc `ldr.w r0,[r4,#24]!`) | `[sp,#80]` |
| `messages` | r5 | `[sp,#72]` |
| `delta` | r6 | `[sp,#76]` |
| `enemies` | r8 | `[sp,#96]` |
| `player` | r0 | `[sp,#84]` |
| r9 | free at entry | — |

Frame 432, canary at `[sp,#428]`.  `Status*` is **not** cached: every use
is `ldr r0,[r4]` (48 reloads of `[sp,#92]` in the body).  The epilogue
checks `ldr.w r1,[fp]` (fp = canary base).

## 2. The current base map

```
137d26  mov  fp, r0              ; this            -> fp (r11)
137d2a  mov  r4, fp              ; r4 = this
137d2c  mov  r6, r1              ; delta           -> r6
137d30  ldr.w sl, [r0]           ; &__stack_chk_guard -> sl (r10)
137d34  ldr.w r0, [sl]
137d38  str  r0, [sp,#428]
137d3a  ldr.w r0, [r4,#24]!      ; &m_pLevel       -> r4, [sp,#80]
137d3e  blx  Level::getMessages  ; messages        -> r5
137d44  ldr  r0, [r4]
137d46  blx  Level::getPlayer    ; player          -> [sp,#84]
137d4e  ldr  r0, [r4]
137d50  blx  Level::getEnemies   ; enemies         -> r8
137d56  ldr  r0, [pc,#880]
137d5a  add  r0, pc
137d5a  ldr.w r9, [r0]           ; &Globals::status -> r9, [sp,#92]
137d5e  ldr.w r4, [r9]           ; Status* cached in r4 (source local)
137d62  mov  r0, r4
137d64  blx  Status::getCurrentCampaignMission
```

| value | register | spill |
|---|---|---|
| `this` | **fp (r11)** | `[sp,#88]` |
| `&Globals::status` | **r9** | `[sp,#92]` |
| canary base | **sl (r10)** | — |
| canary value | r0 -> `[sp,#428]` | `[sp,#428]` |
| `&m_pLevel` | r4 (pre-inc) | `[sp,#80]` |
| `messages` | r5 | `[sp,#72]` |
| `delta` | r6 | `[sp,#76]` |
| `enemies` | r8 | `[sp,#96]` |
| `player` | r0 | `[sp,#84]` |
| `Status*` | r4 (cached by `Status *status`) | — |

Frame 432, canary at `[sp,#428]` — the **stack layout is byte-identical to
the original**.  Only the register assignment differs: `this` and the
canary base are swapped (fp/sl), and `&Globals::status` is r9 instead of
r4 because r4 is occupied by the cached `Status*`.

The first 0x40 bytes of the function differ by exactly **5 bytes**
(`0x137d26`, `0x137d2a`, `0x137d33`, `0x137d34` register bits, plus the
two literal-pool `ldr` offsets at `0x137d28`/`0x137d56`, which are a
downstream artefact of the body).  The first 0x80 bytes differ by 66 B.

## 3. Root cause: greedy-allocator tie-break, not source order

`-mllvm -print-machineinstrs` after the Virtual Register Rewriter shows the
entry block (base):

```
16B   %12083:rgpr    = COPY $r1        ; delta
32B   %11431:gprnopc = COPY $r0        ; this
48B   %796:tgpr      = LDRLIT @__stack_chk_guard
64B   %13197:gpr     = t2LDRi12 %796   ; canary base
80B   %798:gpr       = t2LDRi12 %13197
96B   t2STRi12 %798, %stack.0
112B  %12230:gpr     = COPY %11431     ; this copy
128B  %799:gpr, %12230:gpr = t2LDR_PRE %12230, 24
```

After VR rewrite: `%12083 -> r6`, `%11431 -> r11 (fp)`,
`%13197 -> r10 (sl)`, `%12230 -> r4`, `%12273 -> r5`.

The IR and the machine-instruction order are identical to the original
(`mov this` then `ldr canary`).  The difference is purely which of
`{sl, fp}` the allocator hands to `this` (`%11431:gprnopc`) and to the
canary base (`%13197:gpr`).  The two values are in **different register
classes** (`gprnopc` vs `gpr`), so the allocator's per-class allocation
order decides the tie.  No C++ declaration-order or pointer/reference
form tried in this wave changes that class assignment.

## 4. Declaration-order sweep (24 permutations, current base)

All builds `link_so`, measured with `/dev/shm/w10601/fdiff.py` and
`_work/reconstructions/align_diff.py`.  `EXACT` = ours size 57,232.

| order (messages/player/enemies/status) | raw | aligned | ours size |
|---|---:|---:|---:|
| **messages,player,enemies,status (base)** | **55,055** | **17,059** | **57,232** |
| messages,enemies,player,status | 55,067 | 17,105 | 57,232 |
| player,messages,enemies,status | 55,067 | 17,108 | 57,232 |
| player,enemies,messages,status | 55,069 | 17,110 | 57,232 |
| enemies,messages,player,status | 55,069 | 17,110 | 57,232 |
| enemies,player,messages,status | 55,059 | 17,061 | 57,232 |
| player,enemies,status,messages | 55,078 | 17,121 | 57,232 |
| enemies,status,player,messages | 55,078 | 17,077 | 57,232 |
| player,status,enemies,messages | 55,081 | 17,121 | 57,232 |
| status,enemies,player,messages | 55,093 | 17,078 | 57,232 |
| status,player,enemies,messages | 55,094 | 17,122 | 57,232 |
| messages,player,status,enemies | 55,389 | 17,284 | 57,248 (+16) |
| player,messages,status,enemies | 55,389 | 17,285 | 57,248 (+16) |
| messages,enemies,status,player | 55,390 | 17,638 | 57,216 (-16) |
| enemies,messages,status,player | 55,390 | 17,630 | 57,216 (-16) |
| messages,status,player,enemies | 55,220 | 16,915 | 57,284 (+52) |
| player,status,messages,enemies | 55,220 | 16,937 | 57,284 (+52) |
| status,messages,player,enemies | 55,219 | 16,921 | 57,284 (+52) |
| status,player,messages,enemies | 55,217 | 16,937 | 57,284 (+52) |
| **messages,status,enemies,player** | **54,917** | **16,934** | **57,292 (+60)** |
| status,messages,enemies,player | 55,237 | 16,937 | 57,300 (+68) |
| status,enemies,messages,player | 55,237 | 16,929 | 57,300 (+68) |
| enemies,status,messages,player | 55,238 | 16,927 | 57,300 (+68) |
| messages,enemies,status,player | 55,390 | 17,638 | 57,216 (-16) |

**Every size-exact order has the identical base map**
(`this=fp, canary=sl, &Globals::status=r9, Status*=r4, frame 432`).
The `messages,status,enemies,player` order (the wave-158 lead) is the best
raw/aligned shape measured (54,917 / 16,934) but is **+60 B** on the
current base (w10503 moved it from the -8 B the wave-159 note recorded),
so it is gate-fatal.  Its map is completely different:
`this=r6, &m_pLevel=r5, messages=sl, delta=r8->r5, enemies=r8,
&Globals::status=r9, Status*=r4, canary=fp, frame 432`.

## 5. `status` form sweep (base declaration order)

| form | raw | aligned | size | map |
|---|---:|---:|---:|---|
| `Status *status = Globals::status;` (base) | 55,055 | 17,059 | 57,232 | this=fp, canary=sl, status=r9, val=r4 |
| `Status *&status = Globals::status;` | 55,056 | 17,068 | 57,232 | this=fp, canary=sl, status=r9, val=r0 |
| `Status &status = *Globals::status;` | 55,055 | 17,059 | 57,232 | this=fp, canary=sl, status=r9, val=r4 |
| no local (`Globals::status->`) | 55,056 | 17,068 | 57,232 | this=fp, canary=sl, status=r9, val=r0 |
| `Status *const status = ...` | — | — | 57,232 | same as base |
| `Status *status = *&Globals::status;` | — | — | 57,232 | same as base |
| `Status *status = (Status *)Globals::status;` | — | — | 57,232 | same as base |

The `Status *&` (reference-to-pointer) form is size-exact for **all 24**
declaration orders (it stabilises the frame at 432), but its best raw is
55,056 (+1) and best aligned 17,068 (+9).  It does not flip `this`.

## 6. `Status *st` local in the state-6 block

Moving `Status *st = Globals::status;` to any of the 5 legal positions in
the state-6 block (before `pp`, after `pp`, after each of the four
`Globals::status->field_*` stores) gives the identical base map and size
57,232.  Removing `st` (using `Globals::status->field_60`) is size-exact
but raw 55,058 (+3).  `Status *&st` is raw 55,058, `Status &st` raw
55,055 (identical to base).

## 7. `Level *self = m_pLevel;` self-alias sweep

Replacing the first N `m_pLevel->` uses (after the first `getMessages`)
with `self->`:

| N | size | this | canary | map |
|---:|---:|---|---|---|
| 0–2 | 57,232 | fp | sl | base map |
| 3 | 57,292 | **sl** | **fp** | frame **424**, &m_pLevel=r4->[sp,#64], self=r9, messages=r5, delta=r6, enemies=r8, status=[sp,#84], Status*=r4, canary=[sp,#420], this=[sp,#80], player=[sp,#76] |
| 4 | 57,284 | sl | fp | frame 424 |
| 5 | 57,284 | sl | fp | frame 424 |
| 6–8 | 57,276 | sl | fp | frame 424 |
| 9 | 57,268 | sl | fp | frame 424 |
| 10–13 | 57,260 | sl | fp | frame 424 |
| 14 | 57,224 | sl | fp | frame 424 |
| 15–16 | 57,184 | fp | r9 | frame 424 |
| 17 | 57,360 | sl | — | frame 424 |
| 18–30 | 57,236..57,252 | fp | r9 | frame 424 |

This is the only lever that flips `this` to **sl** and the canary to
**fp**, but it shrinks the frame by 8 B (424) and moves every local
offset, so it is not the original source shape.  N=14 is the closest
size (57,224, -8 B) but measures raw **55,504** / aligned **20,542** —
much worse than the base.  Combining the self alias with the
`Status *&status` form does not restore the frame to 432 (all N=3..14
remain 424).

## 8. Conclusion / open questions

* The original prologue is `this=sl, &Globals::status=r4, canary=fp`,
  frame 432, no cached `Status*`.  The base is `this=fp,
  &Globals::status=r9, canary=sl`, frame 432, `Status*` cached in r4.
* The stack frame and spill offsets already match the original exactly;
  only the `{sl,fp}` tie-break differs.  It is a greedy-allocator
  register-class tie (`%11431:gprnopc` vs `%13197:gpr`), not a source
  declaration-order effect.
* The only `this=sl` shapes found (`Level *self = m_pLevel;` for the
  first 3..14 `m_pLevel->` uses) shrink the frame to 424 and lose 449+
  raw bytes.  A source lever that flips the `gprnopc`/`gpr` allocation
  order without adding a local would land the original prologue.
* The `messages,status,enemies,player` reorder is the best raw shape
  (54,917 / 16,934) but is +60 B on the w10503 base; a -60 B companion
  (or a reorder that is size-exact) is still open.  The
  `Status *&status` form makes the reorder size-exact but neutralises
  its win (55,068 / 17,061).

## 9. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10601/fdiff.py _ZN11LevelScript7processEi        # 55,055 / 57,232
python3 /dev/shm/w10601/align_diff.py _ZN11LevelScript7processEi  # 17,059
cmake --build cmake-build-match --target check -j6                 # RATCHET PASS
```

Scratch harnesses: `/dev/shm/w10601/{fdiff,align_diff,sweep_map,fastbuild.sh}`.
