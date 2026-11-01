# Generator `.rodata` emission order (w3386)

Status: research note + landed source fix (2026-11-01). Worktree `wt/w3386`, branch
`wave87-w3386`, based on master `0a37381e`.

## Baseline actually measured in this worktree

The task brief says the baseline is `byte 4115 / linked 4495`. That figure predates the
`.rodata` output-group pass already on master. The measured baseline at `0a37381e` is:

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4171  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size_match` is `4523/4524` (only `Generator::createMission` is 1992 B vs the original's
1996 B), lint is `CLEAN`, drift is `0`. Every ELF section is size/base exact except
`.rodata` (`0x41b57` vs `0x41b67`, -16 B).

## Symptom and address correction

The brief calls the first `.rodata` diff `0x1e9f90`. The measured first differing byte is
**section offset `0x31780`, VMA `0x1fb590`** (`.rodata` base is `0x1c9e10`; `0x1c9e10 +
0x31780 = 0x1fb590`). The `0x1e9f90` figure in the brief / older notes is a typo.

First-diff / block-equality check (run from the repo root):

```
python3 - <<'PY'
import sys
sys.path.insert(0, "tools/verify")
from elf import Elf32

def rodata(path):
    e = Elf32(path)
    s = next(x for x in e.sections if x["name"] == ".rodata")
    return s, open(path, "rb").read()[s["offset"]:s["offset"] + s["size"]]

so, od = rodata("../_work/bins/android_2.0.16_libgof2hdaa.so")
sn, nd = rodata("cmake-build-match/libgof2hdaa.so")
for i, (a, b) in enumerate(zip(od, nd)):
    if a != b:
        print("first diff", hex(i), hex(so["addr"] + i), hex(a), hex(b))
        break
start = 0x1fb590 - so["addr"]
end = 0x1fb7d8 - so["addr"]
print("Generator block equal", od[start:end] == nd[start:end])
PY
```

On the baseline `.so` this prints `first diff 0x31780 0x1fb590 ...`; on the landed `.so` it
prints `first diff 0x31ab8 0x1fb8c8 ...` and `Generator block equal True`.

At `0x1fb590` the original has the nine `GENERATOR_SPECIAL_SELL_GOODS` values
`200,220,208,213,216,228,229,230,231`, immediately followed by the ten
`GENERATOR_SMUGGLE_GOODS` values `101,102,103,107,108,109,114,124,0,0`. Our baseline had
the two tables reversed: `kKaamoStationItems` first, `kAgentOfferItems` later (after
`kBlockedMissionStations`).

## Ground truth: original symbol dump and original `.rodata`

`_work/original_layout/original_layout.json` (HD 1.1.19 dump, `Classes/Game/Generator.cpp`)
lists exactly six constant tables, in this order:

```
_GENERATOR_SPECIAL_SELL_GOODS
_GENERATOR_SMUGGLE_GOODS
_SHIP_RACES
_GENERATOR_TYPE_PROBS
_GENERATOR_CAMPAIGN_STATIONS
_GENERATOR_SPECIAL_SELL_SHIPS
```

That is exactly the original `.rodata` order. Extract it with:

```
python3 - <<'PY'
import json
j = json.load(open("_work/original_layout/original_layout.json"))
for s in j["tu_symbols"]["gof2hd_1.1.19_ios"]["Classes/Game/Generator.cpp"]["symbols"]:
    if s.startswith("_GENERATOR") or s == "_SHIP_RACES":
        print(s)
PY
```

The original addresses (from the binary) are:

| original symbol | our name | VMA | size |
|---|---|---:|---:|
| `GENERATOR_SPECIAL_SELL_GOODS` | `kAgentOfferItems` | `0x1fb590` | 36 |
| `GENERATOR_SMUGGLE_GOODS` | `kKaamoStationItems` | `0x1fb5b4` | 40 |
| `SHIP_RACES` | `kShipRaces` | `0x1fb5dc` | 256 |
| `GENERATOR_TYPE_PROBS` | `kLootTypeChance` | `0x1fb6dc` | 20 |
| `GENERATOR_CAMPAIGN_STATIONS` | `kBlockedMissionStations` | `0x1fb6f0` | 208 |
| `GENERATOR_SPECIAL_SELL_SHIPS` | `kAgentOfferShips` | `0x1fb7c0` | 24 |
| (same TU, `.rodata.cst16`) | `kAgentEnemyRaces` | `0x1fb7d8` | 16 |

The original has no separate `kNeverUnlockedItems` or `kaamoSpecialItems` symbol. Our source
had **three copies of the same nine values**:

* `kAgentOfferItems` (used by `createMission`),
* `kNeverUnlockedItems` (used by `getItemBuyList`'s never-unlocked loop),
* `kaamoSpecialItems` (used by `isKaamoSpecialItem`).

LLVM merges identical internal constants. The merge kept `kAgentOfferItems` as the canonical
symbol (all three uses relocate to it), but the merged constant's emission position followed
the duplicate copies, not the canonical declaration. That is why the baseline object emitted
`kKaamoStationItems` first and `kAgentOfferItems` only after `kBlockedMissionStations`.

## Controlled experiments

### Compile-only probe

The exact compile command is in `cmake-build-match/compile_commands.json`. Appending
`-S -o /tmp/Generator.s` to that command and inspecting the `.rodata` definitions shows the
object order directly.

| variant | source change | object `.rodata` order |
|---|---|---|
| baseline | three duplicate arrays | `kKaamoStationItems`, `kShipRaces`, `kLootTypeChance`, `kBlockedMissionStations`, `kAgentOfferItems`, `kAgentOfferShips` |
| A | remove only `kaamoSpecialItems`; use `kAgentOfferItems` in `isKaamoSpecialItem` | still `kKaamoStationItems` first (`kAgentOfferItems` merged with `kNeverUnlockedItems`) |
| B (landed) | remove both `kaamoSpecialItems` and `kNeverUnlockedItems`; use `kAgentOfferItems` at both call sites | `kAgentOfferItems`, `kKaamoStationItems`, `kShipRaces`, `kLootTypeChance`, `kBlockedMissionStations`, `kAgentOfferShips` |

Variant B reproduces the original symbol-dump order exactly. The landed object symbol offsets
are:

```
kAgentOfferItems        .rodata+0x000  size 36
kKaamoStationItems      .rodata+0x024  size 40
kShipRaces              .rodata+0x04c  size 256
kLootTypeChance         .rodata+0x14c  size 20
kBlockedMissionStations .rodata+0x160  size 208
kAgentOfferShips        .rodata+0x230  size 24
kAgentEnemyRaces        .rodata.cst16+0x000 size 16
```

That table is the output of:

```
readelf -sW cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/Generator.cpp.obj \
  | grep -E 'kAgentOfferItems|kKaamoStationItems|kShipRaces|kLootTypeChance|kBlockedMissionStations|kAgentOfferShips|kAgentEnemyRaces'
```

### Full gate A/B

```
# baseline
cmake --build cmake-build-match --target check -j8

# edit src/game/mission/Generator.cpp (remove the two duplicate tables)
cmake --build cmake-build-match --target verify -j8
cmake --build cmake-build-match --target check -j8
python3 tools/ratchet.py --build-dir cmake-build-match --update-baseline
cmake --build cmake-build-match --target drift -j8
```

| metric | baseline | after fix |
|---|---:|---:|
| `byte_exact` | 4171 | **4177** |
| `linked_exact` | 4495 | 4495 |
| `size_match` | 4523/4524 | 4523/4524 |
| `.text` | `0x1337c8` | `0x1337c8` |
| `.ARM.exidx` | `0x6f70` | `0x6f70` |
| `.ARM.extab` | `0x26298` | `0x26298` |
| `.rodata` | `0x41b57` | `0x41b57` |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` |
| `.data` | `0x296c` | `0x296c` |
| `.bss` | `0x69ac` | `0x69ac` |
| lint | CLEAN | CLEAN |
| drift | 0 | 0 |

The six functions that flip from `pct==100` but not-byte to byte-exact are exactly the
Generator functions that reference the tables:

```
Generator::isKaamoSpecialItem(int)
Generator::getItemBuyList(Station*)
Generator::getShipBuyList(Station*)
Generator::getLootList(int,int)
Generator::generateStationIndex(Array<SolarSystem*>*,int)
Generator::createAgents(Station*)
```

No function size changed, no other function's `pct`/`linked`/size changed, and no exported
symbol/import changed.

## Generator block is now byte-identical

The whole Generator plain-`.rodata` block is `0x1fb590..0x1fb7d8` (0x248 bytes). Comparing it
directly against the original:

```
Generator block 0x31780 0x319c8 equal True
```

(`0x31780` is the section offset, `0x319c8` the exclusive end offset; VMA range
`0x1fb590..0x1fb7d8`.) The first 16 bytes of the following merge-constants block
(`kAgentEnemyRaces` at `0x1fb7d8`) also match.

## Next `.rodata` divergence (not Generator)

After the fix, the first differing byte is **section offset `0x31ab8`, VMA `0x1fb8c8`**:

```
orig: 8f c2 f5 3c 8f c2 f5 3c 3d 0a 57 3e ...   (floats: 0.03f, 0.03f, 0.21f, ...)
ours: a6 04 00 00 a3 04 00 00 a5 04 00 00 a4 04 00 00 8f c2 f5 3c ...
```

The four extra ints are `1190,1187,1189,1188` = `Hud.cpp`'s `g_Hud_raceBadge[4]` in
`.rodata.cst16`. Find the source with:

```
grep -n -E '1190, 1187, 1189, 1188' src/game/core/Globals.cpp src/game/world/StarMap.cpp \
  src/game/menu/ModStation.cpp src/game/ui/Hud.cpp
```

`src/game/ui/Hud.cpp:43` is the 4-int table; the other files have longer tables. The original
places that table at `0x1fdde8` (and other copies later), not here. This is a merge-constant
ordering issue in `Hud.cpp`, not a Generator issue. The Generator `.rodata` work is closed;
the next worker should start at `Hud.cpp`'s `g_Hud_raceBadge`.

## Proven vs hypothesis

Proven:

* The baseline first diff is VMA `0x1fb590` (offset `0x31780`), not `0x1e9f90`.
* The original has one nine-value table, not three; the HD symbol dump names it
  `GENERATOR_SPECIAL_SELL_GOODS` and lists it first among the Generator constants.
* Removing both duplicate copies changes the clang-7 `-Oz` object `.rodata` order to the
  original order and makes the entire Generator block `0x1fb590..0x1fb7d8` byte-identical.
* The full gate improves `byte_exact` 4171 -> 4177 with `linked_exact`, all section sizes,
  lint and drift unchanged.
* The next first diff is `Hud.cpp`'s `g_Hud_raceBadge` merge constant at `0x1fb8c8`.

Hypothesis (not needed for this pass):

* The original source probably used the six uppercase `GENERATOR_*`/`SHIP_RACES` names from
  the dump. Renaming our local symbols to those names should be debug-info-only after the
  strip, but it was not tested as a full link because the current names already produce the
  exact original `.rodata` order.
