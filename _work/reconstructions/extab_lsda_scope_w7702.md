# `.ARM.extab` LSDA scopes: lifetime-marker EH scopes, `PlayerJumpgate` landed (w7702)

Status: research note + one landed source fix, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7702`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).

## Result

| metric | before | after |
|---|---:|---:|
| `.ARM.extab` differing bytes | 1435 | **1420** |
| `.ARM.extab` differing records | 14 | **13** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.rel.plt` differing bytes | 0 | **0** |
| byte_exact / linked_exact | 4506 / 4506 | **4506 / 4506** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

Full `check` is green (`RATCHET PASS`, byte 4506, linked 4506, imports 207).

## New hypothesis this wave: automatic-variable lifetime markers are the EH scopes

The previous waves treated the LSDA landing-pad groups as opaque "cleanup scopes"
and tried lexical braces / `goto` / comma expressions.  The missing piece is that
**every automatic variable declaration pushes a `NormalEHLifetimeMarker` cleanup
scope** (`CGDecl.cpp:1345`, `EHStack.pushCleanup<CallLifetimeEnd>`), even though
the `llvm.lifetime.start/end` intrinsics themselves are deleted by the `-Oz`
optimizer.  The landing pad is cached per innermost EH scope
(`CGExpr.cpp:getInvokeDestImpl`), so the *variable scope* — not the lexical
`{ }` — determines the LSDA grouping.

Confirmed on a minimal constructor: after `S *s = new S();`, every later invoke
in the same block shares the landing pad of `s`'s lifetime marker; a nested
`{ int r; }` creates a second pad, and the pad ends at the end of the block.
`clang -Xclang -disable-llvm-passes -emit-llvm` shows the `lifetime.start`
calls; the optimized IR has already dropped them but the LSDA survives.

## Landed fix: `PlayerJumpgate::PlayerJumpgate` (15 bytes, body stays byte-exact)

Original rows:

```
(0,48,0) (48,16,238) (66,4,224) (74,8,238) (90,22,236)
(126,12,234) (156,20,222) (202,8,238) (210,54,0)
```

Before (ours): `(48,8,236) (58,6,234) (74,8,234) (90,48,238) ... (202,8,236)`.
The raw IR showed two lifetime scopes: `volumes` (L1, around the array `new` +
`ArraySetLength`) and `radius` (L2, around the whole radius/setRadius/sphere
tail).  The original has **no L1** (the array `new` and `ArraySetLength` share
the function-level pad) and **two scopes** where ours has one L2 (the radius
computation vs `setRadius`+sphere `new`).

Source fix (semantics-preserving; the emitted body is still byte-exact 264 B):

```cpp
this->boundingVolumes = new Array<BoundingVolume *>();
ArraySetLength(1, *this->boundingVolumes);

int radius;
{
    int r;
    if (Globals::status->inAlienOrbit()) {
        r = 7500;
    } else {
        r = ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getRace() == 1 ? 11250 : 7500;
    }
    radius = r;
}
this->player->setRadius(radius);
```

* Dropping the `volumes` local removes L1, so the `operator new` / `ArraySetLength`
  invokes join the enclosing pad (238).
* The nested `int r` scope is L3 and ends before `setRadius`, giving the
  original B/C split (236 for `inAlienOrbit/getSystem/getRace`, 234 for
  `setRadius` + sphere `new`).
* `radius = r` is copy-propagated, so the body is unchanged.

Measured on the isolated TU (`/dev/shm/w7702/pjtest.cpp` harness) and then on
the full link: the record is byte-identical and `verify` keeps the function
byte-exact.

## Remaining 13 differing records (1420 bytes)

| owner | diff bytes |
|---|---:|
| `Generator::createMission` | 24 |
| `Level::createMission` | 139 |
| `Level::createScene` | 9 |
| `Level::createShip` | 70 |
| `RecordHandler::recordStoreRead` | 3 |
| `ModStation::ModStation` | 2 |
| `Radar::Radar` | 7 |
| `Radar::draw` | 29 |
| `LevelScript::process` | 291 |
| `HangarWindow::render` | 762 |
| `Hud::draw` | 40 |
| `SpaceLounge::drawLounge` | 14 |
| `MGame::OnTouchEnd` | 30 |

`RecordHandler::recordStoreRead` and `Radar::Radar` are still pure stub-order
swaps: in both, two landing-pad stubs with identical `b.n <cleanup>` bodies are
assigned to the two lifetime scopes in the opposite order.  The scopes are now
identified (`equipmentCount` vs `equipment` in `recordStoreRead`; the early
`labelStrings` temporaries vs the later `new String` group in `Radar::Radar`),
but the stub order is chosen by `MachineBlockPlacement`, not by the source
scope.  A `volatile` sink or a CFG change that flips the block order is the
next hypothesis.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7702
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# extab differing bytes 1420, records 13
```

Scratch harnesses: `/dev/shm/w7702/cc.sh` (exact TU compile flags),
`/dev/shm/w7702/objlsda.py` (object `.ARM.extab` decoder),
`/dev/shm/w7702/vtest.py` (variant body/row tester).
