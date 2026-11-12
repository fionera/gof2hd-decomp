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

## 6. Follow-up sweep (same worktree, after landing CBA)

After the CBA win was committed (`df06f0d0e`), a further sweep looked for
another size-exact lever.  Everything below is size-exact 1,432 unless
noted and none beats raw **451**:

* `case 12:` re-expressed as three early-return `if`s, as a negated
  `&&` chain, as `if (getStatusValue() != 0) break;`, with a cached
  `idx`/`orbit`/`camp` local, or with the outer `getStatusValue()` check
  hoisted inside the `||`: all byte-identical to CBA (451).
* `case 10:` body order sweep (all 6 permutations of `index`, `cargo`,
  `amount`) × `case 20` device (`(unsigned short)`, `(short)`,
  `(unsigned char)`, `& 0xffff`, no cast) × `case 12` order (CBA/ABC) ×
  rank-table form (extern GOT vs absolute `0x20b977`) × null-check
  on/off: 54 size-exact combinations, best is the committed
  `idx-cargo-amt` / `(unsigned short)` / CBA / extern at **451**;
  next is `amt-cargo-idx` at 452.
* `case 10:` re-query of `getProductionGoodIndex()` for `hasEquipment`
  (the original's shape), `Ship *gship` local, `idx2` local, and
  `hasEquipment` first: 1440 or worse.
* `case 6:` tail (`hasGood && hasSpecial`) rewritten as a `bool both`
  local, `int both`, ternary, `&`, reversed nesting, `!(...) break`,
  and two early `!` breaks: all byte-identical (451).  Declaration
  swap 458, `for (unsigned j = ...)` 477, cached `size()` 1215.
* Simple comparison flips (`missionCount >= v` -> `v <= missionCount`,
  same for `kills`/`stationsVisited`/`getCurrentLoad`/`goodsProduced`/
  `getProductionGoodAmount`, and `1 == getStatusValue()`): 964..1163
  except `case 11` (451).
* `case 16` combined into one `||` condition: 451; the `extra`-first
  order is 925/1444.
* `case 208` inner `&&` swaps (pendingProducts, blueprintIndex),
  `case 213`, `case 19`, `case 23`/`38`, `case 2`/`13`/`15` loop
  conditions: 538..1333, none exact.
* Loop counter type (`int`, `unsigned long`, `i = i + 1`) and
  `Mission *const m`: byte-identical (451).
* Forcing a 40-entry TBH (`case 39:` fallthrough to `case 38:`, table
  default 39): size 1436, raw 1197.

The residual is the documented layout lock-in: the rank-table dispatch
is 2 B longer than the original's `sub/cmp`, the inline constant island
is 2 words (rank table + `Globals::status`) where the original has one
word at the end, the `case 10:` body is 6 B short (64 vs 70 B), and the
`case 20:` `uxth` is a 2 B size device.  Raw 451 / aligned 205 is the
best measured size-exact point.
