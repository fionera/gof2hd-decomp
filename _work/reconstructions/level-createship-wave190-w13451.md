# `Level::createShip` — wave 190 (w13451): first hull box before `ArraySetLength`

Worktree `wt/w13451`, branch `wave190-w13451`, base master `8d8f43bcd`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13451`.

**Status: landed a size-exact, `.rel.plt`/`.ARM.exidx`-identical raw win.**
Raw **1,189 -> 1,183** (-6), aligned 397 -> 404, size **2,764/2,764**
exact, insns 1,117/1,117.  `.text` diff **72,595 -> 72,589** (-6);
`.ARM.extab` content diff 584 -> 585 (+1) but size/base exact
(156,312 B @ `0x1a3b78`); `.ARM.exidx` 0; `.rel.plt` byte-identical;
`.rodata` content exact over the original 269,159 B.  Net
(text+extab+build-id) **73,199 -> 73,194** (-5).

## 0. What changed

In the `race == 0 && type != 14` hull-box arm the first box was emitted
inline:

```cpp
ArraySetLength(3, *volumes);
(*volumes)[0] = new BoundingAAB(fx, y, z, 0.0f, -73.0f, 123.0f, 3000.0f, 2860.0f, 10430.0f);
```

The `new` expression is hoisted into a local that is constructed *before*
`ArraySetLength`, and the store happens afterwards:

```cpp
BoundingVolume *b0 = new BoundingAAB(fx, y, z, 0.0f, -73.0f, 123.0f, 3000.0f, 2860.0f, 10430.0f);
ArraySetLength(3, *volumes);
(*volumes)[0] = b0;
```

This is semantically identical to the base: `ArraySetLength` still runs
before the first slot is written (the `BoundingAAB` constructor does not
touch `volumes`).  It moves the `ArraySetLength` call site from `0x86c` to
`0x8ae`, which lets clang shorten the arm's `b.n` to the shared tail and
drops 6 raw bytes.

## 1. Measurements

| variant | raw | aligned | size | .text | extab | net |
|---|---:|---:|---|---:|---:|---:|
| base | 1,189 | 397 | 2,764 | 72,595 | 584 | 73,179 |
| **`b0` before `ArraySetLength` (landed)** | **1,183** | **404** | **2,764** | **72,589** | **585** | **73,174** |
| move `ArraySetLength` after the `[0]` store (semantically wrong) | 1,183 | 404 | 2,764 | 72,589 | 585 | 73,174 |
| same trick in the `type == 14` arm | 1,195 | 401 | 2,764 | 72,601 | 585 | 73,186 |
| same trick in case 1 | 1,248 | 407 | 2,764 | 72,654 | 587 | 73,241 |
| swap `[0]`/`[1]` stores | 1,215 | 422 | 2,764 | 72,621 | 584 | 73,205 |
| `[0]` stored, `ArraySetLength`, re-store `[0]` | 1,615 | 533 | 2,812 | — | — | worse |

The win is specific to the `race == 0` non-14 arm; the same hoist in the
other arms is neutral or a loss.

## 2. Other cells swept this pass (all neutral at raw 1,189 / `.text` 72,595)

Re-confirmed on the current base:

* faithful all-int coordinates before `setEmpData` (`int x,y,z`, convert
  only in the capital branch): raw 2,447 / size 2,748 (not size-exact).
* original jitter pairing (`jitter1 -> y`, `jitter2 -> x`, `jitter3 -> z`)
  with all-int coordinates: 2,448 / 2,748.
* all-int coordinates declared after `setEmpData`: 2,447 / 2,748.

## 3. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift **0**, parity 5370/5370.
`tools/verify_baseline.json` untouched.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13451
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1183 aligned=404 size=2764/2764 insn=1117/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13451/textdiff.py \
  cmake-build-match/libgof2hdaa.so
# .text diff 72589, .ARM.extab 156312/156312 diff 585, .ARM.exidx 0
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `body_base.cpp`, `v_c0else.cpp`, `sweep_asl2.py`,
`sweep_asl3.py`, `sweep_faithful3.py`, `textdiff.py`.

## 5. Files changed

* `src/game/world/Level.cpp` — hoist the first `race == 0` hull box into a
  `b0` local constructed before `ArraySetLength`.
* `docs/IDENTICALITY.md` — wave-190 numbers.
* this note.
