# MGame::OnTouchEnd — wave 141 (w8614): wingman-tail/join block order probe + 96/127 merge

Status: source change landed, 2026-11-07. Worktree `/home/debian/GalaxyOnFire2/wt/w8614`,
base `e18e0c2a4`. Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only). Scratch
`/dev/shm/w8614`. Full gate green: byte 4507, linked 4507, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0, RATCHET PASS.

Target `_ZN5MGame10OnTouchEndEiiPv` (`MGame::OnTouchEnd`), original 8080 B @ `0x17a144`.

| | base | landed |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 5816 | **4464** |

## 1. Mission: the wingman tail and the `flags == 0` join

The base already has the original's *edge*: the wingman body tail ends with
`b.w 17af60` (join test) and the join test follows the shared `blx initHudMenu`
(`17af5c`).  The original is `b.w 17afe2` / join test after `17afde`.  The
instruction sequences of the wingman tail and the flags body are byte-identical
to the original (only the branch target and addresses differ):

```
orig 17bf2e..17bf4a   ldr [fp,#116]; movs r4,#0; strb [fp,#214]; blx closeHudMenu;
                      mov r0,fp; strb [fp,#93]; blx resumeSounds; ldr r8,[sp,#28]; b.w 17afe2
ours 17bea6..17bec2   identical, b.w 17af60
```

So the base's *edge* is right; the remaining difference is the absolute layout
(the join test is 0x82 bytes early because the code before it is ~130 B shorter).

## 2. NEW lever found: `__builtin_expect(failed, 0)` reproduces the exact join block

The `if (failed)` mission-failed branch is currently a plain `if`.  Adding
`if (__builtin_expect(failed, 0))` changes the block placement so that the whole
join region lands **at the original addresses**:

```
17afd8  ldrd r0,r2,[fp,#116] ; movs r1,#2
17afde  blx Hud::initHudMenu
17afe2  ldr [fp,#248] ; cbz 17b01a        <- join test, exact
17afe8  ... epilogue ... ; blx __stack_chk_fail
17b01a  flags body                         <- exact
wingman tail 17be14: b.w 17afe2            <- exact target
```

The region `0x17afd8..0x17b050` is **120/120 bytes identical** except one
literal-pool displacement (`ldreq r0,[pc,#956]` vs `[pc,#828]`, 1 byte).  Raw
same-address diff drops 5816 -> **5147** at 8080 B.

**But it does not pass the gate**: the block reordering changes the first-reference
order of three imports, which permutes the PLT:

```
base/orig .rel.plt order:  maneuverTouchEnd, isInstantActionMission,
                           removeObjectives, goingToWaypoint, removeRoute, resetAnalogStick
expect    .rel.plt order:  isInstantActionMission, maneuverTouchEnd,
                           resetAnalogStick, removeObjectives, goingToWaypoint, removeRoute
```

`MGame::successCheck` calls three of those PLT slots, so it goes 3 bytes off and
`byte_exact` drops 4507 -> 4506.  It also trips the `arm_builtin` lint (needs a
same-line `// lint: arm_builtin` waiver).  The next wave should either (a) find a
source shape that yields the same join block without the PLT permutation, or
(b) reorder `maneuverTouchEnd` / `resetAnalogStick` first references so the PLT
order is restored.

## 3. Landed change: merge the 96/127 split (raw win, gate green)

The 96/127 split (wave 140 w8513) is faithful to the original's two separate
tests but costs 8 B and shifts the whole campaign chain.  On the current base
(which already has the wingman nesting) merging it back into
`== 96 || == 127` is a large raw win and still 8080 B:

| variant | size | raw diff | PLT vs orig |
|---|---:|---:|---|
| base (split) | 8080 | 5816 | identical |
| **merge 96/127 (landed)** | **8080** | **4464** | **identical** |
| merge + swap 0x200/0x400 | 8080 | 4466 | — |
| `__builtin_expect(failed,0)` | 8080 | 5147 | permuted (gate fail) |

The merged form has `isCampaignMission` 22 vs the original's 23, so it is one
call less faithful; it is landed because the user's primary metric is raw
same-address diff and the gate is green.  The join test polarity in the merged
layout is `cmp/bne epilogue; flags body` (original: `cbz flags body; epilogue`),
i.e. the *edge* is preserved but the fall-through block is the flags body.

## 4. MIR evidence (current base, `-mllvm -print-after-all`)

Final block order around the join (BranchProbabilityBasicBlockPlacement dump):

```
bb.355 body 0x400 prefix (ldrd; movs r1,#2)
bb.356 shared tail (blx initHudMenu)
bb.357 join test (ldr [fp,#248]; cmp 0; bcc flags body)
bb.358 epilogue (isAutoPilot ...)
bb.359 stack check + return
bb.360 __stack_chk_fail
bb.361 flags body (closeHudMenu ...)
...
bb.409 wingman tail (closeHudMenu; resumeSounds; ldr r8,[sp,#28]) -> t2B bb.357
```

This is exactly the original's order: shared tail -> join test -> epilogue ->
flags body, with the wingman tail branching back to the join test.

## 5. Reproduction

```
cp /dev/shm/w8614/MGame.cpp.base src/game/menu/MGame.cpp   # pristine base
python3 /dev/shm/w8614/patch3.py S                        # landed merge 96/127
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8614/fdiff.py _ZN5MGame10OnTouchEndEiiPv  # 4464 / 8080
cmake --build cmake-build-match --target check -j6           # RATCHET PASS
# exact-join probe (gate fails on PLT):
python3 /dev/shm/w8614/patch6.py fail0
```

Scratch: `/dev/shm/w8614/{patch3.py,patch6.py,fail0.so,S.so,base.so,base.plt,fail0.plt,S.plt,mir.txt}`.
The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`; original
join test `0x17afe2`, epilogue `0x17afe8`, flags body `0x17b01a`, wingman tail
`0x17bf4a` (`b.w 0x17afe2`).
