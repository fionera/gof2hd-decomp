# `.ARM.extab` LSDA scopes: `RecordHandler::recordStoreRead` landed (w7720, second pass)

Status: research note + one landed source fix, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7720`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).

## Result

| metric | before | after |
|---|---:|---:|
| `.ARM.extab` differing bytes | 1420 | **1417** |
| `.ARM.extab` differing records | 13 | **12** |
| `.ARM.extab` size / base | 156,312 / 0x1a3b78 | same |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.rel.plt` differing bytes | 0 | **0** |
| `.text` differing bytes | 85,379 | **85,379** (no body change) |
| byte_exact / linked_exact | 4506 / 4506 | **4506 / 4506** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

Full `check` is green (`RATCHET PASS`, byte 4506, linked 4506, imports 207).

## Landed fix: `RecordHandler::recordStoreRead` (3 bytes, body stays byte-exact)

The remaining record was a pure swap of two identical landing-pad trampolines
(`b.n 0xcd24e`, destroy `String path` at `sp+0x9c`) at function offsets 5078 and
5100:

```
orig  row 161 (Read shipCount)      -> 5078
      rows 162,164 (new/SetLength)  -> 5100
      rows 167,169,171,184          -> 5100
ours  row 161                       -> 5100
      rows 162,164                  -> 5078
      rows 167,169,171,184          -> 5100
```

The two scopes are the `shipCount` lifetime marker (created at the `Read`) and
the `shipList` lifetime marker (created at the `new`).  The fix is to move the
`shipList` declaration **after** the `Read`, so its lifetime marker is pushed
after the `shipCount` scope has already cached its landing pad:

```cpp
int shipCount = 0;
AEFile::Read(shipCount, fd);
Array<Ship *> *shipList = 0;
if (shipCount >= 1) {
    shipList = new Array<Ship *>();
    ArraySetLength(shipCount, *shipList);
    ...
    reinterpret_cast<Array<Ship *> *&>(rec->field_0x184) = shipList;
}
```

This is semantics-preserving (the pointer is only used inside the `if`) and the
emitted `.text._ZN13RecordHandler15recordStoreReadEi` section is **byte-identical**
to the original (5416 B).  The object's `.ARM.extab` record now matches all 205
rows exactly (`objlsda.py` comparison: `diff count 0`), so the linked record is
byte-identical too.

Why the same trick was *not* already found: earlier waves moved the `new` or
`ArraySetLength` statements and changed the body; they did not try moving the
*declaration* of the pointer while leaving the `new` in place.  The declaration
position controls the lifetime-marker (EH scope) order without changing the
machine code.

## Remaining 12 differing records (1417 bytes)

| owner (exidx fn) | diff bytes | body byte-exact? |
|---|---:|---|
| `Generator::createMission` | 24 | no |
| `Level::createMission` | 139 | no |
| `Level::createScene` | 9 | no |
| `Level::createShip` | 70 | no |
| `ModStation::ModStation` | 2 | no |
| `Radar::Radar` | 7 | **yes** |
| `Radar::draw` | 29 | no |
| `LevelScript::process` | 291 | no |
| `HangarWindow::render` | 762 | no |
| `Hud::draw` | 40 | no |
| `SpaceLounge::drawLounge` | 14 | no |
| `MGame::OnTouchEnd` | 30 | no |

`Radar::Radar` (7 bytes) is now the only remaining record owned by a
byte-exact body.  It has the same shape as the old `recordStoreRead` record:
pads 2462 and 2464 are both `b.n 0x12de28` (destroy `Radar::lockLabel` at
`sp+0x10`).  Rows 1/3/11/13/15 (the `labelStrings` block) use 2462 in the
original and 2464 in ours; rows 19/21 (`getEnemyGuns` + the first
`Image2DCreate` group) use 2464 in the original and 2462 in ours.  The two
scopes are the outer ctor cleanup (created at the first invoke) and the
`enemyGuns` lifetime scope (created at `getEnemyGuns`).

Source variants tested this pass (all keep the 2488-byte body unless noted):

* remove the `{ }` around `data()[0]`; add `{ }` around `data()[1..3]`, around
  each assignment, or around the whole `labelStrings` block — rows unchanged.
* split `playerGuns`/`enemyGuns` declarations from their initialisers, use
  `= 0`, parenthesised init — rows unchanged, body byte-identical.
* declare `enemyGuns` before the `playerGuns` `if` — body byte-identical, rows
  unchanged.
* declare `enemyGuns` after the `labelStrings` assignment / before
  `data()[1]` / after `data()[3]` / at the top of the ctor — the relative pad
  order flips (early group lower than late group, matching the original), but
  the cleanup chain moves: body differs by 24-420 bytes and the pad values
  become 2390-2468 instead of 2462/2464.  The `enemyGuns`-at-top variant is
  the closest (24 differing tail bytes, all in the cleanup chain).

The next hypothesis is to find a declaration position that flips the
scope-to-pad mapping while keeping the cleanup chain byte-identical; the
`enemyGuns`-at-top variant shows the mapping *can* be flipped, but its
lifetime marker also reorders the `String` temporary cleanup chain.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7720
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# extab differing bytes 1417, records 12
```

Scratch harnesses: `/dev/shm/w7720/cc.sh`, `cc_radar.sh`, `objlsda.py`,
`secbytes.py`, `rhtry*.py`, `radtry*.py`.
