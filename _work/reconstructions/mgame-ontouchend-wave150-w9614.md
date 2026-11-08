# MGame::OnTouchEnd — wave 150 (w9614): the `lockedStation` tail-branch via branch reorder (no hint)

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w9614`,
base `7eed2966e` (wave 149 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w9614`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 149, `expect(stream,1)`) | this wave (no hint) |
|---|---:|---:|
| size | 8080 | **8080** |
| raw same-address diff | 4393 | **4420** |
| aligned (align_diff) | 904 | **917** |
| `.text` differing bytes | 86,449 | **86,476** |
| `lockedStation` arm | `blx dockToAsteroid; b.n` (owns call) | **`b.n 17bea4`** (tail-branches) |
| `isDockingToAsteroid` arm | `b.n` forward to stream-owned call | **owns the shared `blx`** |

## 1. The finding: the tail merge keeps the *later* arm

The two `dockToAsteroid` sites (`isDockingToAsteroid` and `isDockingToStream`) are tail-merged
by the machine BranchFolder.  The merged `blx dockToAsteroid` block is placed after whichever
arm comes **later** in the IR/machine block order; the earlier arm gets a branch to it.

Evidence (base source, `-mllvm -print-after-all`, pre-placement machine dump): the merged call
block `bb.373` has predecessors `bb.372` (stream arg setup, fallthrough) and `bb.368` (asteroid
arg setup, `t2B %bb.373`).  The source order is asteroid, dock, stream, so the *stream* arm is
later and owns the call.  The original binary is the opposite: `0x17bcf4 blx dockToAsteroid`
is owned by the asteroid arm (`0x17bce4..0x17bcf2` falls through into it) and the stream arm
does `0x17bf60 b.n 17bcf4`.  The original's BranchFolder kept the *earlier* arm because the PGO
profile made the asteroid arm hotter; without a profile we cannot get that direction with the
same source order.

## 2. The landed change: move the stream arm in front

```cpp
-                    if (this->player->isDockingToAsteroid()) {
+                    if (this->player->isDockingToStream()) {
+                        this->player->dockToAsteroid(this->radar->lockedStation, this->radar);
+                        this->hud->hudEvent(6, this->player, 0);
+                    } else if (this->player->isDockingToAsteroid()) {
                         this->player->dockToAsteroid(this->radar->getLockedAsteroid(), this->radar);
                         this->hud->hudEvent(6, this->player, 0);
                     } else if (this->player->isDockingToDockingPoint()) {
                         this->radar->lockedEnemy = nullptr;
                         this->player->dockToDockingPoint(nullptr, this->radar);
                         this->hud->hudEvent(6, this->player, 0);
-                    } else if (__builtin_expect(this->player->isDockingToStream(), 1)) {
-                        this->player->dockToAsteroid(this->radar->lockedStation, this->radar);
-                        this->hud->hudEvent(6, this->player, 0);
                     } else if (...menu...) {
```

With the stream arm first and the asteroid arm second, the asteroid arm is the later of the
two `dockToAsteroid` arms, so it owns the merged call block and the stream arm tail-branches
into it — exactly the original's merge direction, with no `__builtin_expect`:

```
ours (stream_ast_dock)                        original
17bc2e blx isDockingToStream                  17bf4e ldr  r0,[r4]
17bc38 cmp r1,#0                              17bf50 blx  isDockingToStream
17bc3a beq.w 17be88                           17bf54 cbz  r0,17bf80
17bc3e ldr.w r2,[fp,#128]  ; radar            17bf56 ldr.w r2,[fp,#128]
17bc42 ldr   r1,[r2,#36]   ; lockedStation    17bf5a ldr.w r0,[fp,#88]
17bc44 b.n   17bea4        ; tail branch      17bf5e ldr   r1,[r2,#36]
                                              17bf60 b.n   17bcf4   ; tail branch
...
17be88 blx isDockingToAsteroid                17bcda ldr.w r8,[sl]
17be98 blx getLockedAsteroid                  17bce8 blx  getLockedAsteroid
17bea2 mov r0,r8                              17bcf2 mov  r0,r8
17bea4 blx dockToAsteroid  ; shared block     17bcf4 blx  dockToAsteroid
```

The conditions are mutually exclusive in practice (`isDockingToStream()` reads
`dockingToStream`; `isDockingToAsteroid()` reads `dockingState`; `isDockingToDockingPoint()`
reads `dockedFlag`), so the reorder does not change behaviour.

## 3. Variants measured (all 8080 B unless noted)

All variants are the wave-149 `sepif` shape with the `expect` removed.

| variant | size | raw | aligned | merge owner |
|---|---:|---:|---:|---|
| base wave 149 (`expect(stream,1)`) | 8080 | 4393 | 904 | stream (wrong) |
| no hint, source order ast/dock/stream | 8084 | 4407 | 916 | stream (wrong) |
| **stream/ast/dock (landed)** | **8080** | **4420** | **917** | **asteroid (right)** |
| stream/ast/dock + `swapauto` epilogue | 8080 | 4422 | 915 | asteroid |
| ast/stream/dock | 8080 | 4395 | 913 | stream (wrong) |
| ast/dock/stream + asteroid named temp | 8080 | 4421 | 923 | stream (wrong) |
| ast/dock/stream + `int doAsteroid` precomputed, body last | 8092 | 4555 | 883 | — |
| dock/ast/stream | 8084 | 4697 | 931 | — |
| dock/stream/ast | 8084 | 4697 | 970 | — |
| stream/dock/ast | 8084 | 4693 | 967 | — |
| stream/ast/dock + `expect(stream,1)` | 8080 | 4385 | 926 | stream (wrong) |
| stream/ast/dock + `expect(ast,1)` | 8080 | 4634 | 936 | asteroid |
| stream/ast/dock + `expect(stream,0)` | 8088 | 4580 | 910 | — |
| common `dockToAsteroid` after the if-chain (`bool doDock`) | 8076 | 4662 | 952 | after chain |
| common `dockToAsteroid` + `dockTarget != nullptr` | 8108 | 4733 | 967 | after chain |
| nested `else { if (...) }` forms | same as flat | | | |

Only `stream/ast/dock` gives the asteroid-owned shared block at exactly 8080 B without a hint.
The common-call forms place the call after the whole chain (stream side) and miss the size.

## 4. Honest assessment

The merge direction is now faithful and the branch-weight hint is gone.  The price is
**+27 raw / +13 aligned / +27 `.text` bytes**: moving the stream test in front shifts the
campaign cold cluster (`0x17b2f8..0x17b726`, `0x17b8b6..0x17bb6c`, `0x17bba4..0x17bf58`) by
a few bytes, and the same-address metric counts every shifted byte.  The dock chain itself
now differs by only 6 bytes (`0x17bf5e..0x17bf64`).  The base with the hint had a better
same-address score only because the hint happened to leave the campaign cluster aligned; its
`lockedStation` arm still owned the call (`blx + b.n`, the +4 B the wave-149 note describes).

## 5. Reproduction

```
cp /dev/shm/w9614/MGame.stream_ast_dock.cpp src/game/menu/MGame.cpp
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9614/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4420 / 8080
python3 /dev/shm/w9614/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 917
cmake --build cmake-build-match --target check -j6               # RATCHET PASS
```

Scratch: `/dev/shm/w9614/{fdiff.py,align_diff.py,blockdiff.py,measure.sh,gen*.py,ir.sh,
MGame.*.cpp,allir.txt,pre.txt,ote_mbp.txt,sad_diff.txt}`.  The authoritative binary is
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
