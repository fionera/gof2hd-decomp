# `Status::missionCompleted` — wave 190 (w13396): case-12 `||` order

Worktree `wt/w13396`, branch `wave190-w13396`, base master `8ffa01f05`.
Target `_ZN6Status16missionCompletedEbbx` (`src/game/mission/Status.cpp`,
1,432 B, VMA `0xabf38`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13396`.

**Status: landed.**  Raw **466 -> 451** (-15), aligned 193 -> 205,
size 1,432/1,432 exact.  `.text` differing bytes **73,012 -> 72,997**
(-15); `.ARM.extab` 603 (unchanged, size/base exact at `0x1a3b78`,
156,312 B); build-id 20.  Total (text+extab+build-id)
**73,635 -> 73,620** (-15).  `.rodata` unchanged (size 269,351, +0xc0;
original 269,159 B content exact), `.fini_array` 8 B @ `0x20cba0`,
`.dynamic`/`.rel.dyn`/`.rel.plt`/`.got`/`.data.rel.ro` byte-identical,
27/27 section headers, file size 2,187,328 exact.  Gate: byte_exact
4507/4524, linked_exact 4507/4524, size 4524/4524, data 843/843,
imports 207, stub/missing/extra 0, lint CLEAN, drift 0, parity
5370/5370 (`RATCHET PASS`).

## 1. What changed

The `case 12:` body's three-way `||` was reordered from the source
order

```cpp
Globals::status->currentCampaignMission != 0x5c ||
Globals::status->inAlienOrbit() ||
Globals::status->station->getIndex() != 0x71
```

to

```cpp
Globals::status->station->getIndex() != 0x71 ||
Globals::status->inAlienOrbit() ||
Globals::status->currentCampaignMission != 0x5c
```

All three operands are side-effect-free getters (`inAlienOrbit()` is
`station->equals(playerStation)`; `getIndex()` is a plain field read),
so the reorder is semantically identical.

## 2. Why this is a win

The reorder makes clang emit the `getIndex()` test first and fold the
`currentCampaignMission` test into the `getIndex()` compare's IT block
(`ittt eq; ldreq; ldreq.w; cmpeq`), shortening the `case 12:` body by
4 B.  The body is at `ac2b2..ac2e8` (54 B) instead of `ac2b2..ac2ec`
(58 B), which pulls the following case bodies 4 B closer to the
original's addresses and removes 15 same-address byte diffs overall.

Measured permutations of the three `||` operands (all size 1,432
unless noted):

| order (A=`currentCampaignMission`, B=`inAlienOrbit`, C=`getIndex`) | raw | aligned |
|---|---:|---:|
| **CBA (landed)** | **451** | 205 |
| BCA | 455 | 198 |
| ABC (base) | 466 | 193 |
| ACB | 466 | 200 |
| BAC | 466 | 200 |
| CAB | 486 | 132 (size 1,428) |

The wave-190 neutral-cell note listed "case-12 `||` reorders are 466
or worse"; CBA and BCA were not among the cells measured there.

## 3. Structural checks

* `.text` 0x1337c8 exact, 72,997 differing bytes (was 73,012).
* `.ARM.extab` size/base exact, 603 differing bytes (unchanged);
  `.ARM.exidx` 0.
* `.rodata` sh_size 269,351 (+0xc0), original 269,159 B content exact;
  `.fini_array` 8 B @ `0x20cba0`; `.dynamic`, `.rel.dyn`, `.rel.plt`,
  `.got`, `.data.rel.ro`, `.dynsym`, `.dynstr`, `.hash`, `.init_array`
  byte-identical; 27/27 headers; file size 2,187,328 exact.
* `check` RATCHET PASS, `lint` CLEAN, `drift` 0 fields.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13396
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN6Status16missionCompletedEbbx     # raw=451 aligned=205 size=1432/1432
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 5. Files changed

* `src/game/mission/Status.cpp` — `case 12:` `||` operand order
  CBA.
* `docs/IDENTICALITY.md` — wave-190 residual table and section totals.
* this note.
