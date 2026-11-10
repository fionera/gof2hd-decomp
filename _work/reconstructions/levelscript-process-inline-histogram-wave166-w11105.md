# LevelScript::process — wave 166 / w11105: callee inlining histogram (re-verified)

Target `_ZN11LevelScript7processEi` (original `0x137d18`, 57,232 B = 0xdf90),
size-exact.  Worktree `wt/w11105`, branch `wave166-w11105`, base master
`85c1f88df` (wave 165).  Scratch `/dev/shm/w11105/`.

Baseline (HEAD, unchanged):

```
raw 54,247   aligned 17,532   size 57,232/57,232   insn 20,709/20,830
gate byte_exact 4507 / linked 4507 / size 4524 / data 843 / imports 207
```

The wave-165 note `levelscript-process-inline-histogram-wave165-w11105.md`
answered the same question; this pass re-derived the histogram independently
with a stricter parser (separate `bl`/`blx`, PLT vs local vs indirect) and
adds one new structural finding (the mission-78 shared tail is dead code).

## TU membership (authority: `_work/original_layout/original_layout.json`)

`tu_symbols['gof2hd_1.1.19_ios']['Classes/Game/LevelScript.cpp']` contains
**only** the `LevelScript` methods plus the weak template instantiation
`ArrayAdd<Player*>(Player*, Array<Player*>&)`.  No `Level`, `AEGeometry`,
`Status`, `Standing`, `PlayerEgo`, `LODManager` or `TargetFollowCamera`
function is in that TU, so (no LTO in the original build) none of them can
have been inlined into `process`.

## Histogram (independent re-measurement)

Parsed with `objdump -d` over the whole function, classifying each call:

| metric | original | ours |
|---|---:|---:|
| `blx <plt>` direct calls | 2,355 | 2,368 |
| unique PLT callee names | **245** | **245** |
| PLT callee set difference | **none** | **none** |
| `blx rN` indirect (virtual/GOT) | 462 | 448 |
| `bl` to a local (non-PLT) target | 1 | 1 |
| `LevelScript::resetCamera(Level*)` calls | **51** | **51** |

The 245-name PLT callee set is **identical**; the only "local" call on each
side is the literal-pool word the disassembler renders as `_end+0xff1251xx`.
`ArrayAdd<Player*>` is inlined in both (no call in either body).  Therefore
**no callee is inlined in the original that our tree calls**, and no
same-TU declaration move / TU merge can change the inlining.

### Call-count deltas (ours - original), 17 names

```
+1 AEGeometry::moveForward(float)
+1 AEGeometry::setPosition(const Vector&)
-1 AbyssEngine::PaintCanvas::TransformGetTransform(unsigned)
+1 AbyssEngine::ApplicationManager::SetCurrentApplicationModule(unsigned)
-1 TargetFollowCamera::setPosition(const Vector&)
+1 TargetFollowCamera::setPosition(float,float,float)
+1 TargetFollowCamera::translate(float,float,float)
+1 Level::getStarSystem()
+1 Level::getPlayer()
+1 Layout::startFade(bool,int,int)
+1 Status::getStation()
+1 Status::getStanding()
+1 Station::setAttackedFriends(bool)
+1 Standing::setStanding(int,int)
+1 PlayerEgo::getPosition()
+1 PlayerEgo::setAutoTurret(bool)
+1 PlayerEgo::autoTurretIsEnabled()
```

Net direct +13, indirect -14: 14 call sites that are indirect (`blx rN`,
vtable/GOT) in the original are direct PLT calls in ours.  This is a
call-site/virtual-dispatch shape difference, not inlining: the callee *set*
is unchanged.  The wave-165 note attributes `getStarSystem`,
`getStanding`/`setStanding` and `autoTurretIsEnabled`/`setAutoTurret` to the
three deliberate PLT-order anchors, and `getPlayer`/`getPosition`/
`moveForward`/`AEGeometry::setPosition` to unisolated source-shape
differences.

## New finding: the mission-78 (0x4e) shared tail is dead code

Original mission-78 substate bodies all jump to a shared tail at `0x13c88c`:

```
13c88c  ldr  r0,[sl,#28]        ; m_nState
13c890  subs r0,#5
13c892  cmp  r0,#3
13c894  bhi  13c8ba
13c896  ldr  r0,[sp,#76]        ; delta
        ... vcvt/negs ...
13c8a6  vldr s2,[pc]            ; 0.8f @0x13c8f8
13c8ae  vmul.f32 s0,s0,s2
13c8b6  blx  TargetFollowCamera::translate(float,float,float)
13c8ba  ldrb r5,[sl,#17]        ; m_bCinematicBreak
13c8be  b    144f5a             ; shared epilogue (returns r5)
```

The switch default (`bhi 0x13c88c` at `0x138380`) and every substate
`break` reach it.  Our source has the same tail at
`src/game/world/LevelScript.cpp:1237`, but it is **inside** the
`switch (m_nState)` block (after `case 11`'s closing brace), so it is
unreachable and clang drops it (no DWARF line info for lines 1237/1238).
Our mission-78 arm instead falls through to the shared active tail
(`0x137f76`), a known pass-2 approximation.

Measured shapes (full link, `tools/fn_diff.py`):

| shape | raw | aligned | size | note |
|---|---:|---:|---:|---|
| **HEAD** | **54,247** | **17,532** | **57,232** | tail dead |
| tail moved outside the switch (no `return`) | 55,044 | 20,015 | 57,232 | size-exact, raw +797 |
| tail outside + `return m_bCinematicBreak != 0;` | 55,447 | 20,308 | 57,364 (+132) | faithful early return, size-fatal |

Moving the tail out makes it live but emits it inline after the switch
(the original keeps it out of line at `0x13c88c`), shifting the rest of the
function; both shapes are raw regressions, so nothing was landed.

One further declaration-order check on this base: the wave-160 best raw
order `messages,status,enemies,player` measures raw 55,511 / aligned 17,353 /
size 57,316 (+84) here (the wave-160 base was 54,917 / 16,934 / +60), so it
is still size-fatal and worse than HEAD on raw.

## Conclusion

* The 245-name PLT callee set is identical; no callee is inlined in the
  original that we call.  `LevelScript::resetCamera` (the only same-TU
  callee) is a PLT call 51 times in both.  TU membership is not a lever.
* The histogram differs only in call *counts* (net direct +13 / indirect
  -14), from call-site shape and the three PLT-order anchors, not inlining.
* New actionable residual for a future pass: the mission-78 shared tail
  (`0x13c88c`) is currently dead code at `LevelScript.cpp:1237`; making it
  live is structurally faithful but needs a -132 B companion to stay
  size-exact.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 54247 aligned= 17532 size=57232/57232 insn=20709/20830
```
