# `.ARM.extab` residual — wave 192 (w14001), 2026-11-14

Cross-cutting task: attribute the 555 differing `.ARM.extab` bytes in the
wave-192 residual set. Every residual function is **size-exact**, so these are
LSDA *record-shape* differences, not code-size differences.

Base: `wave192-w14001` @ `8233c752d` (master `8233c752d`).
Ours: `cmake-build-match/libgof2hdaa.so`. Orig:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`.

## Method

Tool: `_work/reconstructions/extab_map.py` + the committed decoder
`_work/reconstructions/extab_decode_w14001.py` (a worktree-aware map copy is
`extab_map_wt_w14001.py`). It parses
`.ARM.exidx` (8-byte entries, prel31), resolves each entry to its `.ARM.extab`
record, derives the record byte range from the sorted extab-offset set, and
diffs the records at the same offsets. Each LSDA is then decoded field by field.

Facts established by the decode:

- `.ARM.extab`: size `0x26298` (156,312 B) at file offset **`0x1a3b78`** in both
  binaries; **3,294 records at identical offsets**.
- Personality routine at image `0x2026d8` (file `0x5eb60`); LSDA header byte
  `0xff` => `lp_enc = 0xff` (omit), `tt_enc = 0xff` (no type table),
  `cs_enc = 0x1` (ULEB128 call-site encoding).
- `lsda_off = 0x10` (two unwind words) for every record except
  `MGame::OnTouchEnd`, which is `0xc` (one unwind word).
- **555 differing bytes, confined to exactly 9 records.** All differences are in
  the call-site table (`start` / `length` / `landing-pad` / `action`); the type
  table is empty everywhere, so no type-info bytes differ.

## Result: 9 differing records

`fn addr` = image address of the function start. `extab off` = file offset of
the record. `cs_len` = call-site-table byte length (ULEB). `trips` = number of
call-site entries. `cs_len` and `action` columns are **orig / ours**. `action` is the raw
action-record bytes as they appear in the record (not a decoded action
value); `none` means the record carries no action bytes at all.

| function | fn addr | extab off | rec size | diff B | cs_len | trips | action |
|---|---|---:|---:|---:|---|---|---|
| `_ZN11LevelScript7processEi` | 0x137d18 | 0x1c2af8 | 348 | 189 | 0x144 / 0x144 | 46 / 46 | `000000` / `000000` |
| `_ZN5Level13createMissionEv` | 0x0affd8 | 0x1a9ac4 | 956 | 144 | **0x3a6 / 0x3a5 (+1)** | 148 / 148 | **`00` / `0000`** |
| `_ZN5Level10createShipEiiiP8Waypointbb` | 0x0c0214 | 0x1ab09c | 336 | 93 | 0x13b / 0x13b | 57 / 57 | none |
| `_ZN5Radar4drawEP6PlayerP3Hudi` | 0x12e050 | 0x1c203c | 132 | 43 | 0x70 / 0x70 | 19 / 19 | none |
| `_ZN12HangarWindow6renderEv` | 0x1490d8 | 0x1c30fc | 948 | 36 | 0x39f / 0x39f | 156 / 156 | none |
| `_ZN5MGame10OnTouchEndEiiPv` | 0x17a144 | 0x1c8034 | 280 | 27 | 0x104 / 0x104 | 44 / 44 | `000000` / `000000` |
| `_ZN11SpaceLounge10drawLoungeEv` | 0x170d38 | 0x1c7138 | 168 | 14 | 0x91 / 0x91 | 25 / 25 | `0000` / `0000` |
| `_ZN5RadarC1EP5Level` | 0x12d484 | 0x1c1f08 | 256 | 7 | 0xe9 / 0xe9 | 39 / 39 | `0000` / `0000` |
| `_ZN10ModStationC1Ev` | 0x0d52d0 | 0x1ada6c | 112 | 2 | 0x59 / 0x59 | 15 / 15 | `000000` / `000000` |
| **total** | | | | **555** | | | |

`_ZN5RadarC1EP5Level` stands for the Radar C1/C2 pair (`C2` is a tail-call
alias; same record). `_ZN10ModStationC1Ev` likewise for ModStation C1/C2.

The other 7 residual rows — `Layout` C1/C2, `Status::missionCompleted`,
`Gun::calcCharacterCollision`, `Hud::draw`, `MenuTouchWindow::OnTouchBegin`,
`MGame::OnUpdate` — have **byte-identical LSDA records**. Their residual is
`.text` only and cannot move `.ARM.extab`.

## Decoded per-record diffs

Trip tuple = `(start, length, landing_pad, action)`, record-relative.
"diff offsets" are record-relative byte offsets of the differing bytes.

### `LevelScript::process` — record `0x1c2af8`, 348 B, 189 differing B

Diff offsets:
`[22,23,26,27,29,33,34,35,36,39,40,42,46..342]` (all 46 trips differ).

Selected trips (orig vs ours):

| trip | orig | ours |
|---|---|---|
| 0 | `(0, 5832, 0, 0)` | `(0, 5622, 0, 0)` |
| 1 | `(5832, 6, 57116)` | `(5622, 6, 57104)` |
| 2 | `(5838, 748, 0)` | `(5628, 788, 0)` |
| 3 | `(6586, 8, 57080)` | `(6416, 8, 57082)` |
| 5 | `(8226, 8, 57106)` | `(7718, 10, 57126)` |
| 19 | `(14270, 5718, 0)` | `(12178, 12390, 0)` |

Same trip count (46), same `cs_len` (0x144), same action bytes. The whole
call-site table is shifted because the `.text` block lengths differ (trip 0 is
5832 B orig vs 5622 B ours). This is a pure scheduling/scope-extent effect.

### `Level::createMission` — record `0x1a9ac4`, 956 B, 144 differing B

**The only structural difference in the whole residual set.** `cs_len` is
`0x3a6` orig vs `0x3a5` ours (**+1 byte**), and the action record is `00` orig
vs `0000` ours — i.e. the original's action slot is a 1-byte ULEB (`0`) while
ours encodes a 2-byte ULEB (`0x80 0x00`). Both decode to action 0, but the
*encoding* differs: the original's first call-site entry has `action = 0`
written as a single byte, while ours emits a non-canonical 2-byte ULEB. The
record is still 956 B in both because our +1 `cs_len` byte is absorbed by a
shorter landing-pad field elsewhere (see below).

Diff offsets:
`[19,29,42,54,67,79,92,105,117,129,142,155,162,174,186,198,211,223,236,248,261,273,285,291,295,298,302,307,310,314,319,322,326,332,335,339,344,347,351,353,360,372,385,397,398,409,410,422,423,434,446,459,471,484,496,509,522,534,546,559,572,584,597,609,621,633,645,657,669,681,693,706,718,731,743,756,769,782,795,808,820,826,830,833,837,839,843,846,850,852,856,859,863,869,872,876,882,886,890,896,904,907,908,910..952]`

90 of 148 trips differ, almost all by a landing-pad delta of **-2** (orig is
2 higher than ours), e.g.:

| trip | orig | ours |
|---|---|---|
| 1 | `(228, 4, 17100)` | `(228, 4, 17098)` |
| 3 | `(860, 8, 17060)` | `(860, 8, 17058)` |
| 21 | `(4234, 20, 17080)` | `(4234, 20, 17078)` |

The `-2` LP delta tracks the `.text` 4,265 B residual: the landing pads point 2
bytes lower in our code. The extra `cs_len` byte is a real encoding difference
(1-byte vs 2-byte action ULEB) but does not change record size.

### `Level::createShip` — record `0x1ab09c`, 336 B, 93 differing B

Diff offsets:
`[21..35,37..40,43,44,45,48,54,56,59,62,65,70,72,73,76,81,84,87,92,94,95,98,103,106,109,114,116,117,120,125,128,131,136,138,139,142,147,150,153,158,160..162,164,169,172,173,175,177,180,182,183,186,187,188,195,206,217,228,239,250,261,272,283,294,299,302,305,306,308,313,316,319,324,327,330,332]`

| trip | orig | ours |
|---|---|---|
| 0 | `(0, 406, 0, 0)` | `(28, 4, 2496, 0)` |
| 1 | `(406, 30, 2488)` | `(32, 374, 0)` |
| 7 | `(652, 30, 2448)` | `(654, 30, 2454)` |
| 9 | `(702, 28, 2446)` | `(704, 30, 2452)` |

Trip 0/1 differ in *shape* (orig has one big 406 B protected region with no
landing pad, then a 30 B region with LP 2488; ours has a 28 B region with LP
2496 then a 374 B region with no LP). This is the `EmpGuard`/scope-structure
divergence in `createShip`, not a scheduling artifact. Later trips differ by
±6 in start/length and ±6 in LP.

### `Radar::draw` — record `0x1c203c`, 132 B, 43 differing B

Diff offsets:
`[28,33,34,37,39,40,41,43,45,49,50,52,55,56,57,58,61,62,63,64,67,68,69,70,73,76,79,81,84,85,86,87,90,91,92,93,96,99,102,104,105,111,123]`

| trip | orig | ours |
|---|---|---|
| 1 | LP 9818 | LP 9810 |
| 3 | `(2394, 10, 9856)` | `(2350, 12, 9854)` |
| 5 | `(2562, 10, 9852)` | `(2506, 10, 9850)` |
| 11 | `(2736, 14, 9820)` | `(2670, 10, 9812)` |

LP deltas -8 / -2; starts and lengths shift with the `.text` 6,993 B residual.

### `HangarWindow::render` — record `0x1c30fc`, 948 B, 36 differing B

Diff offsets:
`[29,253,258,260,262,265,271,277,283,611,615,618,621,624,642,648,666,672,690,696,714,720,738,744,762,768,793,799,804,805,808,811,822,825,831,833]`

| trip | orig | ours |
|---|---|---|
| 1 | LP 9544 | LP 9532 |
| 39 | len 150 | len 172 |
| 40 | `(2696, 140, 9562)` | `(2688, 150, 9550)` |
| 98 | len 128 | len 130 |
| 99 / 100 | LP 9652 / 9650 | LP 9626 / 9614 |
| 103/104/107/108/111/112/115/116/119/120 | LP | ±2 |

### `MGame::OnTouchEnd` — record `0x1c8034`, 280 B, 27 differing B

Note: `lsda_off = 0xc` (one unwind word), unlike every other record (`0x10`).

Diff offsets:
`[117,123,125,208,212,213,218,219,223,229,231,232,235,241,247,249,250,253,254,259,260,265,266,271,272,273,274]`

| trip | orig | ours |
|---|---|---|
| 17 | `(1188, 10, 7972)` | `(1192, 10, ...)` |
| 18 | `(1202, 12, 7968)` | `(1206, 8, ...)` |
| 32 | len 172 | len 232 |
| 33 | start 4060 | start 4120 |
| 36 | len 420 | len 366 |
| 37..43 | | shifted +6 |

### `SpaceLounge::drawLounge` — record `0x1c7138`, 168 B, 14 differing B

Diff offsets: `[27,51,62,73,79,100,103,106,112,114,115,118,120,139]`

LP ±2 on trips 1, 5, 7, 9, 20; trip 10 LP 1878 vs 1868; trip 14 start 1460 vs
1472; trip 15 start 1492 vs 1496; trip 16 `(1522, 44, 1878)` vs
`(1526, 38, 1880)`; trip 17 `(1574, 34)` vs `(1572, 36)`.

### `Radar::C1` — record `0x1c1f08`, 256 B, 7 differing B

Diff offsets: `[28,40,88,100,112,136,149]`. **Landing-pad ±2 only**:
trips 1/3/11/13/15 `2462 -> 2464`, trips 19/21 `2464 -> 2462`. Pure
scheduling; the protected extents are identical.

### `ModStation::C1` — record `0x1ada6c`, 112 B, 2 differing B

Diff offsets: `[37,39]`. Only trip 3 differs: orig `(390, 30, 922, 0)` vs ours
`(398, 22, 922, 0)`. Same LP (922), start +8 / length -8: one 8-byte
instruction has moved across a scope boundary. This is the smallest and most
localized residual and was the only candidate for an extab-only source fix
(see below).

### ModStation C1 — source experiment (rejected)

The only extab-local source attempt made this pass: in `src/game/menu/ModStation.cpp`
the two bitcast declarations feeding `yawTable`/`pitchTable` were swapped so the yaw
load is emitted first. Result: `_ZN10ModStationC1Ev` went from 374 to 373
instructions and the record stayed at the same offset, but the **function size broke
1008 -> 1000 B** (raw 11 -> 702, aligned 5 -> 51). `SIZE IS SACRED`, so this is
gate-fatal; it was reverted and the tree rebuilt to baseline. **No source change
landed.** The trip-3 split (`orig (390,30,922,0)` vs `ours (398,22,922,0)`) is
therefore still open; any fix must preserve the 1008 B code size.

## Interpretation and fixability

1. **LSDA bytes are largely a consequence of `.text` shape.** `start`, `length`
   and `landing_pad` are code addresses/extents; they change whenever the
   function's `.text` shifts, even with an identical `try`/`catch` structure.
   Only **record size**, **trip count**, and **action bytes** are structural.
   In this residual set only `createMission` changes a structural field
   (action encoding `00` vs `0000`, `cs_len` +1); every other difference is a
   scheduling/extent effect.
2. **No extab-local edit is possible for 8 of 9 records.** They converge only
   when their function's `.text` becomes byte-exact. `Radar::C1` (LP ±2),
   `ModStation::C1` (one 8-byte instruction across a scope boundary), and the
   `-2` LP deltas in `createMission` are the closest to local, but all are still
   `.text` scheduling.
3. **`createMission`'s 1-byte vs 2-byte action ULEB** is the one encoding
   difference that is *not* a pure address shift. It may be fixable
   independently of full `.text` equality if the source shape that produces the
   first call-site entry's action slot can be matched, but the record size is
   already correct (956 B), so it yields no `.ARM.extab` byte-count win on its
   own unless it also changes the LP fields.
4. **`createShip` trip 0/1** is a genuine scope-structure divergence (one 406 B
   unprotected region vs a 28 B protected region + 374 B unprotected region),
   consistent with the known `EmpGuard`/scope issue in that function.

## Stale numbers in the prior map

`_work/reconstructions/extab-record-map-wave192.md` is stale on two points:

- It states **597** differing bytes at base `c3d1ae5f2`. The current base
  (`8233c752d`) is **555 B** (a later Radar extab −45 B change landed).
- Its `createShip` "EmpGuard-free = 332 B" claim predates the current **336 B**
  state; the record is 336 B in both binaries today.

## Caveats

- `_work` is a **symlink** to `/home/debian/GalaxyOnFire2/gof2-decomp/_work`;
  `git status` shows every `_work/...` as deleted plus `?? _work`. Do not
  `git add -A`; commit this file by hash (`git hash-object -w` +
  `git update-index --add --cacheinfo`).
- `_work/reconstructions/extab_map.py` hardcodes `OURS` to the main repo's
  `cmake-build-match/libgof2hdaa.so`, not this worktree's. Pass/patch the path
  when re-running from a nested worktree.
