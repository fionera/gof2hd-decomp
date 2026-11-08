# LevelScript::process — wave 164 / w11004: callee-saved map + call-site / inlining audit

Target `_ZN11LevelScript7processEi` (original `0x137d18`, **57,232 B = 0xdf90**),
size-exact. Owned TU `src/game/world/LevelScript.cpp`. Worktree `wt/w11004`,
base master `b3d3a1f68` (wave 163). Scratch `/dev/shm/w11004/`.

Result: **no source win landed** (analysis note only). The base is already the
best size-exact shape measured on both metrics.

```
                         base (HEAD)        original
process raw diffbytes    54,541             —
process aligned insns    17,427             —
orig / ours size         57,232 / 57,232   57,232
insn count (ours/orig)   20,693 / 20,830
.text diffbytes          84,652             —
.rel.plt                 0 (BYTE-IDENTICAL)
byte_exact               4507 / linked 4507 / size 4524 / data 843 / imports 207
```

## 1. Callee-saved map (confirmed from the linked binaries)

Prologue, original vs ours (image base 0x10000; both `push {r4,r5,r6,r7,lr}`,
`stmdb {r7,r8,r9,sl,fp}`, `vpush {d8-d13}`, `sub sp,#432`):

```
ORIGINAL 0x137d18                          OURS (HEAD) 0x137d18
mov sl, r0        ; this -> sl             mov fp, r0        ; this -> fp
ldr r0,[pc]; add r0,pc                     ldr r0,[pc]; add r0,pc
mov r4, sl        ; r4 = this              mov r4, fp        ; r4 = this
mov r6, r1        ; delta -> r6            mov r6, r1        ; delta -> r6
ldr.w fp,[r0]     ; fp = &__stack_chk_guard ldr.w sl,[r0]    ; sl = &__stack_chk_guard
ldr.w r0,[fp]                              ldr.w r0,[sl]
str r0,[sp,#428]  ; canary                 str r0,[sp,#428]
ldr.w r0,[r4,#24]! ; r4 = &m_pLevel        same
blx getMessages   ; messages -> r5         same
ldr r0,[r4]; blx getPlayer ; player -> [sp,#84]
ldr r0,[r4]; blx getEnemies; enemies -> r8
ldr r0,[pc]; add r0,pc                     same
ldr r4,[r0]       ; &Globals::status -> r4 ldr.w r9,[r0]    ; -> r9
ldr r0,[r4]       ; Status* reloaded       ldr.w r4,[r9]    ; Status* cached in r4
blx getMission                             mov r0,r4; blx getMission
...
ldr r0,[r4]       ; second dispatch reload mov r0,r4         ; cached value reused
blx getMission                             blx getMission
```

| value | original | ours (HEAD) |
|---|---|---|
| `this` | **sl (r10)** | **fp (r11)** |
| `&__stack_chk_guard` | **fp (r11)** | **sl (r10)** |
| `&Globals::status` | **r4** (spilled `[sp,#92]`) | **r9** (spilled `[sp,#92]`) |
| `Status*` value | reloaded `[r4]` per use | cached in r4 (`Status *status` local) |
| `messages` / `delta` / `enemies` | r5 / r6 / r8 | r5 / r6 / r8 (match) |
| `&m_pLevel` / `player` | `[sp,#80]` / `[sp,#84]` | `[sp,#80]` / `[sp,#84]` (match) |
| canary slot | `[sp,#428]` | `[sp,#428]` (match) |

Frame 0x1b0 in both; the stack layout is byte-identical. The residual is a
pure permutation: `this`<->canary and `r4`<->`r9`, plus the original's second
dispatch reloads the status pointer while ours reuses the cached value.
The epilogue mirrors it: original `ldr r1,[fp]` (canary), ours `ldr r1,[sl]`.

This is the same map recorded in
`levelscript-process-prologue-map-wave160-w10601.md`; the wave-163 mission-0
changes did not move it.

## 2. Call sites: every callee is a PLT call — no inlining difference

Both binaries call every callee through the PLT (`blx <plt>`); there is not a
single `bl`/`blx` to a local (non-PLT) function inside `process`.

| metric | original | ours |
|---|---:|---:|
| `blx` to PLT (immediate) | 2,356 | 2,364 |
| `blx rN` (virtual/indirect) | 462 | 457 |
| unique PLT callee names | **245** | **245** |

The **set of PLT callees is identical** (245 names; the only set-difference
entries are literal-pool words misdecoded as `_end+...`). So no external
callee was inlined by either build.

The only same-TU callee is `LevelScript::resetCamera(Level*)` (original TU
`Classes/Game/LevelScript.cpp`, same as `process`). It is a PLT call **51
times in both** binaries — clang did not inline it in the original either, so
TU membership of `resetCamera` is not a lever here.

Top callees and their original TUs (from `_work/original_layout/tu_mapping.md`):
`TargetFollowCamera` = `Classes/Game/TargetFollowCamera.cpp`,
`LODManager` = `Classes/Game/LODManager.cpp` (folder drift: ours
`engine/render/`), `RadioMessage` = `Classes/Game/RadioMessage.cpp`,
`AEGeometry` = `Classes/Game/AEGeometry.cpp` (folder drift: ours
`engine/render/`), `PlayerEgo` = `Classes/Game/PlayerEgo.cpp`,
`Level` = `Classes/Game/Level.cpp`, `Status` = `Classes/Game/Status.cpp`.
All are separate TUs in the original, so no same-TU inlining is expected.

### Call-count multiset difference (ours - original)

```
+1 AEGeometry::moveForward
+1 AEGeometry::setPosition(Vector)
-1 LODManager::forceUpdate
-1 TargetFollowCamera::setPosition(Vector)
+1 TargetFollowCamera::setPosition(fff)
+1 Level::getStarSystem
+1 Level::getPlayer
+1 Status::getStanding
+1 Standing::setStanding
+1 PlayerEgo::getPosition
+1 PlayerEgo::setAutoTurret
+1 PlayerEgo::autoTurretIsEnabled
```

Two of the plus entries are one real source mismatch: the mission-50/51
exit-wormhole block (source L1817) calls
`player->setAutoTurret(player->autoTurretIsEnabled())`, which the original
does **not** call there (original's first `setAutoTurret` is the case-154
block at `0x13a80c`). The pair is a PLT-order anchor, not original code:
removing it gives size 57,216 (-16), raw 54,915 (+374), aligned 17,271
(-156), and rotates `.rel.plt` by 18 bytes (`setAutoTurret` /
`autoTurretIsEnabled` move after `flashScreen`/`supernova`). It cannot land
without a +16 B companion and a PLT-order replacement.

The other `+1 Status::getStanding`/`+1 Standing::setStanding` pair is the
never-taken `if (delta == 0x7d)` PLT anchor in the case-125 state-1 block.
Removing it gives size 57,208 (-24), raw 55,024 (+483), aligned 17,426 (-1),
and rotates `.rel.plt` by 20 bytes (`setStanding` moves after
`updateSupernova`). Also not landable as-is.

## 3. `834 extra blx` is an alignment artefact, not a call-count gap

`fn_diff.py --json` on the base reports
`extra-ours:blx = 834`, `extra-orig:blx = 830`. The total call counts differ
by only 8, so the 834 is the number of `blx` instructions that difflib cannot
pair after normalising branch targets — the call *sequences* are reordered by
basic-block placement, not by different callees. The largest unmatched call
regions are the repeated cutscene-exit teardown blocks (e.g. orig
`0x13e206` vs ours `0x13ec8e`, 88 calls) and the mission-0 state-4/5 block
(orig `0x1389fc` vs ours `0x138ac6`, 30 calls). Reducing the aligned metric
needs block-order work in those regions, not inlining.

## 4. Declaration-order sweep (24 permutations, size-exact)

All four local declarations (`messages`, `player`, `enemies`, `status`)
permuted; built + linked each time. Base order is the best raw.

| order (messages/player/enemies/status) | raw | aligned | size |
|---|---:|---:|---:|
| **messages,player,enemies,status (base)** | **54,541** | **17,427** | **57,232** |
| enemies,player,messages,status | 54,545 | 17,429 | 57,232 |
| messages,enemies,player,status | 54,553 | 17,420 | 57,232 |
| player,messages,enemies,status | 54,553 | 17,423 | 57,232 |
| enemies,messages,player,status | 54,555 | 17,425 | 57,232 |
| player,enemies,messages,status | 54,555 | 17,425 | 57,232 |
| enemies,player,status,messages | 54,561 | 17,436 | 57,232 |
| player,enemies,status,messages | 54,565 | 17,436 | 57,232 |
| enemies,status,player,messages | 54,565 | 17,445 | 57,232 |
| player,status,enemies,messages | 54,568 | 17,436 | 57,232 |
| status,player,enemies,messages | 54,581 | 17,437 | 57,232 |
| status,enemies,player,messages | 54,580 | 17,446 | 57,232 |
| messages,player,status,enemies | 55,065 | 17,640 | 57,268 |
| player,messages,status,enemies | 55,065 | 17,641 | 57,268 |
| messages,enemies,status,player | 55,217 | 17,985 | 57,244 |
| enemies,messages,status,player | 55,217 | 17,977 | 57,244 |
| messages,status,player,enemies | 55,599 | 17,273 | 57,308 |
| player,status,messages,enemies | 55,599 | 17,332 | 57,308 |
| status,messages,player,enemies | 55,598 | 17,279 | 57,308 |
| status,messages,enemies,player | 55,607 | 17,330 | 57,308 |
| status,player,messages,enemies | 55,596 | 17,332 | 57,308 |
| enemies,status,messages,player | 55,296 | 17,272 | 57,316 |
| status,enemies,messages,player | 55,295 | 17,274 | 57,316 |

Every size-exact order keeps the base map (`this=fp`, canary=sl,
`&status`=r9, `Status*`=r4). The `status`-early orders improve aligned
(17,272-17,332) but are +76 B and raw-worse.

## 5. `Level *self = m_pLevel` alias sweep

The only lever known to flip `this` to `sl` (w10601/w10502) re-measured on the
wave-163 base. `N` = number of `m_pLevel->` uses after the first
`getMessages()` replaced by `self->`.

| N | raw | aligned | size |
|---:|---:|---:|---:|
| 0 (base) | 54,541 | 17,427 | 57,232 |
| 1 | 54,541 | 17,427 | 57,232 |
| 2 | 55,085 | 17,432 | 57,240 |
| 3 | 55,459 | 21,088 | 57,304 |
| 4 | 55,497 | 21,439 | 57,308 |
| 5 | 55,447 | 21,069 | 57,304 |
| 6 | 55,488 | 21,069 | 57,304 |
| 7 | 55,530 | 21,071 | 57,296 |
| 8 | 55,086 | 21,497 | 57,244 |
| 9 | 55,520 | 21,073 | 57,296 |
| 10 | 55,461 | 21,073 | 57,296 |
| 11 | 55,375 | 21,068 | 57,288 |
| 12 | 55,386 | 21,069 | 57,288 |
| 13 | 55,372 | 21,071 | 57,288 |
| 14 | 55,252 | 20,714 | 57,252 |
| 15 | 55,447 | 21,943 | 57,192 |

N=1 is size-exact but codegen-identical to the base (the alias is coalesced
away). Every N that actually flips the map is size-fatal and raw-worse.

## 6. Other measured shapes (not landed)

| variant | raw | aligned | size | note |
|---|---:|---:|---:|---|
| remove `Status *status` local (all `Globals::status`) | 54,542 | 17,424 | 57,232 | +1 raw, -3 aligned; map unchanged |
| mission-0 arm order 4,3,5 (original order) | 55,204 | 18,010 | 57,240 | +8 B, raw +663 |
| 4,3,5 + remove Standing anchor | 55,344 | 18,007 | 57,216 | rel.plt 30 B |
| remove wormhole auto-turret pair | 54,915 | 17,271 | 57,216 | rel.plt 18 B |
| remove Standing `delta==0x7d` anchor | 55,024 | 17,426 | 57,208 | rel.plt 20 B |

## 7. Conclusion / open questions

* The callee-saved map is a pure permutation (`this` sl/fp, canary fp/sl,
  `&Globals::status` r4/r9). It is a greedy-allocator register-class tie
  (`%this:gprnopc` vs `%canary:gpr`), not a declaration-order or TU effect:
  all 24 declaration orders and every size-exact shape keep `this=fp`.
* No callee is inlined in either binary; the 245-name PLT callee set is
  identical and the only same-TU callee (`resetCamera`) is a PLT call in
  both. TU membership is not a lever for this function.
* The 834 `extra-ours:blx` are block-order alignment artefacts. The real
  call-count gap is 8, of which 2 are the wormhole auto-turret PLT anchor and
  2 are the never-taken `delta==0x7d` Standing anchor. Both anchors are
  `.rel.plt`-order critical and size-fatal to remove.
* A size-exact `this=sl` shape (the original map) would need a -40..-56 B
  companion for the `Level *self` alias; none was found. The base is the
  best measured shape.

## 8. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j8
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 54541 aligned=17427 size=57232/57232 insn=20693/20830
```
