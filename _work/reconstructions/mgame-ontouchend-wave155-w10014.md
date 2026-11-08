# MGame::OnTouchEnd — wave 155 (w10014): initHudMenu(2) owns the shared tail; the ownership flip is NOT what dominates the raw

Status: source change landed, 2026-11-08. Worktree `/home/debian/GalaxyOnFire2/wt/w10014`,
base `4417db0b4` (wave 153 final). Owned TU `src/game/menu/MGame.cpp` (`OnTouchEnd` only).
Scratch `/dev/shm/w10014`. Full `check` green before and after: byte **4507/4524**,
linked **4507/4524**, size **4524/4524**, data **843/843**, imports **207**, lint CLEAN,
drift 0, RATCHET PASS. Target `_ZN5MGame10OnTouchEndEiiPv`, original 8080 B @ `0x17a144`.

| | base (wave 153, `expect(stream,1)`) | w9914 faithful owners | this wave |
|---|---:|---:|---:|
| size | 8080 | 8080 | **8080** |
| raw same-address diff | 4391 | 4424 | **4427** |
| aligned (align_diff) | 903 | 923 | **926** |
| shared `blx dockToAsteroid` owner | stream | asteroid (right) | **asteroid (right)** |
| shared `blx hudEvent(6)` owner | stream | autopilot (right) | **autopilot (right)** |
| shared `blx initHudMenu` owner | (1) @0x17adca | (1) @0x17adca | **(2) @0x17adca** |

## 1. Landed shape

Two independent changes on top of the wave-153 base:

1. **Faithful dock tail-merge owners** (the w9914 shape): named `asteroidTarget` local in
   the asteroid arm and `__builtin_expect(isDockingToDockingPoint(), 1)` on the middle arm.
   This is the only 8080 B shape where `dockToAsteroid` is owned by the asteroid arm and
   `hudEvent(6)` by the autopilot arm (w9814 §3).
2. **initHudMenu tail-merge owner**: reorder the three source arms from
   `0x200, 0x800, 0x400` to `0x400, 0x800, 0x200`. The two `initHudMenu` calls are mutually
   exclusive bits, so this is semantically identical; it makes the `0x400` arm the
   lower-numbered MBB, which is the block `BranchFolder::CreateCommonTailOnlyBlock`
   keeps when the `EstimateRuntime` tie-break is equal (`<=` keeps the last `SameTails`
   entry = the lower block number). The shared `blx` is now owned by the `(2)` site, as in
   the original.

Owner detection (`/dev/shm/w10014/owner_hud2.py`, walks back from the shared `blx`):

```
orig  initHudMenu blx@17afde arg=2  (arm1 @0x17adc6 branches forward)
ours  initHudMenu blx@17adca arg=2  (arm1 @0x17afb0 branches backward)
```

## 2. The finding: the ownership flip is worth only +3 raw, not the 3277-byte pre-dock run

The w9914 note attributed the 3277-byte `pre-dock` region diff to the initHudMenu
ownership flip.  That is **false**.  Flipping the owner in place moves exactly 3 raw
bytes in the `pre` region and leaves `pre-dock`, `dock` and `post` byte-for-byte
unchanged:

| region | base | this wave |
|---|---:|---:|
| `pre 0x17a144..0x17adc0` | 121 | **124** |
| `pre-dock 0x17adc0..0x17bb3a` | 3268 | **3268** |
| `dock 0x17bb3a..0x17bf7e` | 1014 | **1014** |
| `post 0x17bf7e..end` | 21 | **21** |

The 3268-byte `pre-dock` run is a **cold-block placement** difference, not the tail-merge
owner.  The original's cold cluster ends

```
17afd8  arm2: ldrd r0,r2,[fp,#116] ; movs r1,#2
17afde  blx  Hud::initHudMenu       ; arm2 falls through
17afe2  join: ldr r0,[fp,#248]      ; if (hudTouchFlags == 0)
17afe6  cbz  r0, 17b01a             ;   -> closeHudMenu
17afe8  (autopilot epilogue)        ;   fall-through (hot)
```

whereas ours places the join at `0x17aeb2`, immediately after the cloak arm, and emits
`cmp r0,#0 ; bne 17b9fc` (branch to the epilogue, fall through to `closeHudMenu`).  The
original's join is 0x14a bytes later, after `maneuverTouchEnd`, `menuTouchOpen`, the
`StarMap` delete, the mission `won` chain and arm2.  That reorders the whole 3.4 KB
`pre-dock` run and is what the raw metric measures.  It is the same class of
PGO/branch-weight block-placement signature as the `Status::missionCompleted` jump table
(KEY CONTEXT), not a source-shape bug.

## 3. New hypotheses measured (all size 8080 unless noted)

`ast/dock/stream` dock order unless stated.  "owner" = initHudMenu shared-blx arm.

| variant | raw | aligned | owner | blx addr |
|---|---:|---:|---|---|
| base (wave 153, `expect(stream,1)`) | 4391 | 903 | (1) | 0x17adca |
| w9914 faithful owners, source `200,800,400` | 4424 | 923 | (1) | 0x17adca |
| **landed: faithful owners + source `400,800,200`** | **4427** | **926** | **(2)** | **0x17adca** |
| faithful + `400,200,800` | 4431 | 1035 | (1) | 0x17ae94 |
| faithful + `200,400,800` | 4429 | 1033 | (2) | 0x17ae94 |
| faithful + `800,400,200` | 6071 | 1082 | (1) | 0x17af5c |
| faithful + `800,200,400` | 6071 | 1080 | (2) | 0x17af5c |
| non-faithful `expect(stream,1)` + `400,800,200` | 4394 | 906 | (2) | 0x17adca |
| non-faithful `expect(stream,1)` + `400,200,800` | 4398 | 1015 | (1) | 0x17ae94 |
| faithful + `expect(0x400,1)` | 6030 | 1186 | (1) | 0x17ad84 |
| faithful + `expect(0x200,0)` | 4711 | 1129 | (2) | 0x17aea6 |
| faithful + `expect(0x800,0)` | 4765 | 1123 | (2) | 0x17ae94 |
| faithful + `expect(hudMenuOpen,1)` | 4424 | 923 | (1) | 0x17adca |
| faithful + `expect(hudMenuOpen,0)` | 4859 | 950 | (1) | 0x17adbc |
| faithful + `expect(hudTouchFlags==0,0)` | 5756 | 1070 | (1) | 0x17adc4 |
| faithful + `expect(weaponSlot>=0,0/1)` | 4424 | 923 | (1) | 0x17adca |
| faithful + `expect(wingmanCommand>=0,0/1)` | 4424 | 923 | (1) | 0x17adca |
| faithful + `expect(isAutoPilot&&!was,1)` | 4701 | 921 | (1) | 0x17adca |
| faithful + arm2 moved after `UseKhadorDrive` | 4656 | 1135 | (1) | 0x17adca |
| faithful + `Hud *hud`/`Level *level` locals in arms | 4424 | 923 | (1) | 0x17adca |
| faithful + `expect(dock,1)` no local + `400,800,200` | 4670 | **914** | (2) | 0x17adca |
| faithful + `expect(ast,1)` (hudEvent wrong) + `400,800,200` | 4402 | 925 | (2) | 0x17adca |

The `800,...` permutations move the join to `0x17af5c` (closest to the original's
`0x17afe2`) but pay 1.6 KB of raw because they put the cloak test first.  The
`expect(dock,1)` no-local shape has the best aligned score (914) but a 290-byte `post`
literal-pool penalty (raw 4670).

## 4. Honest assessment

The mission's two constraints are met: faithful dock owners **and** the initHudMenu `(2)`
owner, at 8080 B, with the dock chain unmoved (`dock` region diff unchanged at 1014).
The price is **+3 raw / +3 aligned** over the w9914 faithful-owner shape, all of it in
the `pre` region (the machine test order becomes `0x400, 0x800, 0x200`; the original's
`0x200, 0x800, 0x400` order is not recoverable from source without a PGO profile).

The w9914 open question — "flip the initHudMenu merge owner without re-laying the
function" — is answered: the source-arm permutation does it with **no** re-lay (the
`pre-dock`/`dock`/`post` regions are byte-identical to the faithful-owner base).  What it
does **not** do is recover the 4391/903 pair, because that pair is the non-faithful
`expect(stream,1)` shape, and the 3268-byte `pre-dock` run is the cold-block placement
(join at `0x17aeb2` vs `0x17afe2`), not the tail-merge owner.

Next lever (not this wave): the `if (hudTouchFlags == 0)` join placement.  The original
falls through to the autopilot epilogue and branches to `closeHudMenu`; ours does the
opposite.  A hint on that condition re-lays the function (5756/1070), so it needs the
same kind of source anchor as the `Status::missionCompleted` case blocks.

## 5. Reproduction

```
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10014/fdiff.py _ZN5MGame10OnTouchEndEiiPv      # 4427 / 8080
python3 /dev/shm/w10014/align_diff.py _ZN5MGame10OnTouchEndEiiPv  # 926
python3 /dev/shm/w10014/owner_hud2.py                             # initHudMenu arg=2
python3 /dev/shm/w10014/regions.py _ZN5MGame10OnTouchEndEiiPv
cmake --build cmake-build-match --target check -j6                # RATCHET PASS
```

Scratch: `/dev/shm/w10014/{fdiff.py,align_diff.py,harness4.py,owner_hud2.py,regions.py,
adiff.py,blocks.py,v1..v12.json,MGame.base.cpp,MGame.faithful.cpp}`.  The authoritative
binary is `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.
