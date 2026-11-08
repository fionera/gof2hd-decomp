# MGame::OnTouchEnd — wave 150 (w9714): move the branch-weight hint to the asteroid arm

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9714`,
base `db9658a51` (wave 150 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9714`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 150, `expect(stream,1)`) | this wave (`expect(ast,1)`) |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4392 | **4400** |
| aligned (align_diff) | 904 | **923** |
| shared `blx dockToAsteroid` owner | stream (wrong) | **asteroid (right)** |
| `.text` differing bytes | 86,863 | **86,871** |

## 1. The finding: the merge owner follows the branch weight, not the source order

The two `dockToAsteroid` sites (asteroid arm and stream arm) are tail-merged by the
machine BranchFolder.  In the original the merged `blx` block is owned by the **asteroid**
arm (`0x17bcf2 mov r0,r8` falls through into `0x17bcf4 blx dockToAsteroid`; the stream arm
does `0x17bf60 b.n 17bcf4`).  With the wave-149 `expect(isDockingToStream(), 1)` the
stream arm is the hot branch, so the merge keeps the stream copy and the asteroid arm
branches forward (`0x17bc4c b.n 17bf44`).

The minimal fix is to move the same hint to the asteroid test:

```cpp
-                    if (this->player->isDockingToAsteroid()) {
+                    if (__builtin_expect(this->player->isDockingToAsteroid(), 1)) { // lint: arm_builtin ...
                         this->player->dockToAsteroid(this->radar->getLockedAsteroid(), this->radar);
                         this->hud->hudEvent(6, this->player, 0);
                     } else if (this->player->isDockingToDockingPoint()) {
                         ...
-                    } else if (__builtin_expect(this->player->isDockingToStream(), 1)) {
+                    } else if (this->player->isDockingToStream()) {
                         this->player->dockToAsteroid(this->radar->lockedStation, this->radar);
                         this->hud->hudEvent(6, this->player, 0);
```

The source order stays `asteroid, dockingPoint, stream` (the original's test order), and
the shared block is now asteroid-owned:

```
ours (expect(ast,1))                          original
17bc3c ldr.w r0,[fp,#128]  ; radar            17bce4 ldr.w r0,[fp,#128]
17bc40 blx getLockedAsteroid                  17bce8 blx getLockedAsteroid
17bc44 mov r1,r0                              17bcec mov r1,r0
17bc46 ldr.w r2,[fp,#128]                     17bcee ldr.w r2,[fp,#128]
17bc4a mov r0,r8                              17bcf2 mov r0,r8
17bc4c blx dockToAsteroid   ; shared block    17bcf4 blx dockToAsteroid
...
17bf5e ldr r1,[r2,#36]                        17bf5e ldr r1,[r2,#36]
17bf60 b.n 17bc4c           ; stream arm      17bf60 b.n 17bcf4
```

The stream test/body/tail-branch and the literal pool are at the original addresses
(`0x17bf4e`, `0x17bf56`, `0x17bf60`, pool `0x17bf64`), so the tail no longer shifts.

## 2. Variants measured (all size 8080 unless noted)

All variants are the wave-149 `sepif` shape with the hint moved/removed.  "owner" is the
arm whose argument setup falls through into the shared `blx dockToAsteroid`.

| variant | raw | aligned | owner |
|---|---:|---:|---|
| base wave 150 (`expect(stream,1)`, ast/dock/stream) | 4392 | 904 | stream (wrong) |
| **ast/dock/stream + `expect(ast,1)` (landed)** | **4400** | **923** | **asteroid (right)** |
| ast/dock/stream, no hint | 4406 @ 8084 B | 916 | stream (wrong) |
| stream/ast/dock, no hint (w9614) | 4419 | 917 | asteroid (right) |
| stream/ast/dock + `expect(dock,1)` | 4420 | 922 | asteroid |
| stream/dock/ast, no hint | 4692 @ 8084 B | 967 | asteroid |
| ast/stream/dock, no hint | 4394 | 913 | stream (wrong) |
| ast/stream/dock + `expect(dock,1)` | 4386 | 913 | stream (wrong) |
| stream/ast/dock + `expect(dock,1)+expect(stream,1)` | 4392 | 929 | stream (wrong) |
| ast/dock/stream + `expect(ast,1)+expect(stream,1)` | 4531 | 956 | — |
| ast/dock/stream + `expect(ast,1)+expect(dock,1)` | 4408 | 926 | asteroid |
| ast/dock/stream + `expect(ast,1)+expect(autopilot,1)` | 4686 @ 8076 B | 938 | — |
| common `dockToAsteroid` after the if-chain (`bool doDock`) | 4662 @ 8076 B | 952 | after chain |
| `goto` join (target var, label inside asteroid arm) | 4703 @ 8076 B | 958 | — |
| `goto` join (stream body at end) | 4394 | 913 | stream (wrong) |
| hudEvent hoisted (`bool doHud`) | 4700 @ 8084 B | 923 | — |
| inverted dockpoint test | 4697 @ 8076 B | 922 | — |

Key negatives: every `goto`/common-call/hudEvent-hoist shape either keeps the stream owner
or loses the exact 8080 B size (the shared call block loses the `mov r0,r8`, or the
`goto` adds a branch).  `expect(ast,1)` is the only lever that flips the merge owner
without moving the literal pool or changing the size.

## 3. Honest assessment

The merge direction is now faithful and the hint is on the branch the original's PGO
profile made hot (the asteroid arm).  The price versus the wave-149 baseline is
**+8 raw / +19 aligned** (not +27/+13 as in w9614): the asteroid arm now contains the
call and the inlined `hudEvent(6)`, so the campaign cold cluster starts `+0x18` later than
in the base.  The original's asteroid arm ends with a short `b.n hudEvent` (hudEvent block
lives at `0x17bb50`, before the chain), so its cluster starts at `0x17bbc`; none of the
tried source shapes reproduces that early hudEvent placement without a size regression
(the hoisted variants are 8084 B / 8076 B).

The no-hint alternative (`stream/ast/dock`, w9614) is 4419/917: it trades 19 raw for
6 aligned.  `ast/stream/dock` is the best raw shape (4394/913) but still stream-owned.
The landed shape is the best asteroid-owned 8080 B variant on raw and the only one that
keeps the original's test order.

## 4. Reproduction

```
cp /dev/shm/w9714/final.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9714/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4400 / 8080
python3 /dev/shm/w9714/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 923
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9714/{fdiff.py,align_diff.py,regiondiff.py,bin.py,owner.sh,gen*.py,
measure.sh,final.cpp,v*.cpp}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
