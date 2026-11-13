# `Level::createShip` — wave 191b (w13542): LSDA-row budget analysis, no landable win

Worktree `wt/w13542`, branch `wave191b-w13542`, base master `23a6ebf8b`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13542`.

**Status: no landable win.  Tree left at the pristine baseline.**
Raw **1,164**, aligned **400**, size **2,764/2,764** exact,
`.ARM.extab` **156,312**, `.ARM.exidx` content diff **0**,
`.rel.plt` byte-identical.

## 0. The real constraint is `.ARM.exidx`, not `.ARM.extab` size

`tools/ratchet.py` does not check either section, so the "`.ARM.extab` must
stay 156,312" rule is really "`.ARM.exidx` must stay byte-identical".
Measured this session:

| variant | raw | size | extab | exidx diff |
|---|---:|---:|---:|---:|
| baseline | 1,164 | 2,764 | 156,312 | **0** |
| drop top `EmpGuard` (w13529) | 1,128 | 2,764 | 156,308 | 1,307 |
| drop ship `EmpGuard` | 1,160 | 2,764 | 156,304 | 1,327 |
| drop ship + guard `getShipGroup` | 1,165 | 2,764 | **156,312** | **0** |

Any change to this function's LSDA record size shifts every later record and
therefore every later `prel31` in `.ARM.exidx`.

## 1. LSDA row arithmetic (why the budget is so tight)

Generic personality record: `[4B prel31 pers][(size+1)*4B unwind][LSDA]`.
The LSDA header is 5 bytes when `TType` is omitted
(`LPStart=0xff`, `TType=0xff`, call-site enc `0x01`, ULEB length).
So `record = 16 + 5 + call_site_table`, rounded up to 4.

`call_site_table` is a sequence of rows `(start, len, lp, action)`, each field
ULEB128.  In this function `lp` is always ~2,400-2,500 (2 bytes), so a row costs

* **5 bytes** when `start < 128` **and** `len < 128`,
* **6 bytes** when `start >= 128` (or `len >= 128`).

Record 336 needs `call_site_table` in **312..315** (21 + 315 = 336).
The baseline sits exactly at the ceiling: 57 rows, **315** bytes.

| variant | rows | cst | record |
|---|---:|---:|---:|
| baseline (top+setEmpData+setShipGroup guards) | 57 | 315 | 336 |
| drop top guard | 56 | 310 | 332 |
| drop setShipGroup guard | 55 | 304 | 328 |
| drop top+setEmpData | 55 | 305 | 328 |
| drop top+setShipGroup | 54 | 299 | 320 |
| guard added at a late call (1 row) | 57 | 316 | 340 |
| guard added at an early call (2 rows, `start<128`) | 58 | 319 | 340 |
| guard added at `inAlienOrbit` (2 rows) | 58 | 321 | 344 |

**Consequence:** from the drop-top form (310) the only way back to 336 is a
single **5-byte** row, i.e. a guard on the *first* call in the function (the
"before" row is omitted only for the first invoke).  That is exactly the
non-original top `EmpGuard`; every other guard adds 6, 9, 10 or 11 bytes.
From the drop-setShipGroup form (304) the only way back is a 9..11-byte
(two-row) guard, and every measured one is >= 1,165 raw.

## 2. Measured variants (all size-exact 2,764 unless noted)

### extab 156,312 (`.ARM.exidx` diff 0) — the only admissible class

| variant | raw | aligned | notes |
|---|---:|---:|---|
| baseline | **1,164** | 400 | top guard + setEmpData guard + setShipGroup guard |
| drop setShipGroup + guard `getShipGroup` | 1,165 | 400 | best admissible; +1 raw |
| drop setShipGroup + guard `setLevel` | 1,165 | 400 | |
| drop setShipGroup + guard `addObject` | 1,168 | 399 | |
| drop setShipGroup + guard `getLevel` | 1,191 | 434 | |
| drop setShipGroup + guard `gameWon` | 1,194 | 433 | |
| drop setShipGroup + guard `inAlienOrbit` | 1,166 | 400 | |
| drop setShipGroup + guard `jitter2` | 1,464 | 453 | |
| drop setShipGroup + guard `hardCoreMode` | 2,440 | 492 | |
| drop setShipGroup + guard `getShipGroup`(tail) | 2,283 | 493 | size 2,788 |
| drop setEmpData + guard `getShipGroup` | 2,171 | 506 | size 2,784 |
| drop top + guard `setLevel` | 1,164 | 400 | tie |
| drop top + guard `getShipGroup` | 1,164 | 400 | tie |
| drop top + guard `inAlienOrbit` | 1,165 | 400 | |
| drop top + guard `getLevel` | 1,189 | 434 | |
| drop top + guard `gameWon` | 1,192 | 433 | |
| drop top + guard `jitter2` | 1,462 | 453 | |
| `(getLevel()>20?20:getLevel())*5+40` clamp (form b) | 1,174 | 396 | matches original branch shape |
| `(getLevel()<=20?getLevel():20)*5+40` (form a) | 1,167 | 398 | |
| current folded clamp (form c) | **1,164** | 400 | |
| cargo branch order 0x2c/0x33/0x31 (original order) | 1,164 | 400 | neutral |
| `float fx` removed (`(float)x` inline) | 1,169 | 401 | |
| x/y declaration swap | 1,164 | 400 | neutral |
| `Level *self = this` + `setLevel(self)` | 1,164 | 400 | neutral |
| `empTime` initialised at declaration | 1,164 | 400 | neutral |
| `ship` declared late / `= 0` / `= (KIPlayer*)0` | 1,164 | 400 | neutral |
| `volumes`/`fixed` declaration permutations | 1,164 | 400 | neutral |
| `missionPtr` without `__builtin_expect` | 1,171 | 391 | worse |
| `missionPtr != 23 && != 1` | 1,171 | 391 | worse |

### extab 156,308 or 156,304 (exidx shifted) — not landable under the rule

| variant | raw | extab |
|---|---:|---:|
| drop top `EmpGuard` | **1,128** | 156,308 |
| drop top + faithful spawn (`x=wpx+j3`, `y=wpy+j1`, `z=wpz+j2`) | 1,154 | 156,308 |
| drop top + spawn int/float/float (bg_35) | 1,154 | 156,308 |
| drop ship `EmpGuard` | 1,160 | 156,304 |
| drop top + guard `setShipGroup` | 1,384 | 156,296 |

### Regular-`new` (faithful ctor cleanups) — size / extab landmines

The original uses regular `new PlayerFighter(...)` / `new PlayerFixedObject(...)`
(its landing-pad cluster has four delete pads: `sl`, `fp`, `r9`, `r5`).
Our placement-new form has only two.  Every regular-new variant measured:

| variant | raw | size | cst | record |
|---|---:|---:|---:|---:|
| fixed only | 2,429 | 2,768 | 326 | 348 |
| fighter only | 2,278 | 2,764 | 326 | 348 |
| both | 2,439 | 2,772 | 337 | 360 |
| drop all guards + both | 2,096 | **2,764** | **315** | **336** |
| drop top+ship + fixed | 2,134 | **2,764** | **315** | **336** |

`drop all guards + both` is the only *fully faithful* form that is both
size- and record-exact, but its raw is 2,096 because the regular-`new`
allocation moves the whole hull block.

## 3. Body diffs that remain (for the next worker)

The body is byte-identical to the original from `0xc033a` onward except for a
systematic register swap.  The original allocates
`fighter=sl, fixed=r9, volumes=fp, ship=r9, player=r5`; we get
`fighter=fp, fixed=fp, volumes=r9, ship=fp, player=r5`.  This single swap is
worth ~40 raw bytes (blocks at `[fp,#4]` vs `[r9,#4]`, `mov r3,r4` vs
`mov r3,sl`, `mov r1,sl` vs `mov r1,r6`, ...).  It is caused by the
placement-new temporary (`mov r6,r0` then `mov fp,r6`) and by `ship` being
allocated to `fp`; declaration order, `ship = 0`, `Level *self = this`,
`volumes`-before-`fixed`, and regular-`new` variants all failed to flip it.

The other ~10-byte block is the `empPoints` clamp: the original spills
`this` (`str.w r9,[sp,#56]`) *after* `add.w sl,r0,#40`, we hoist it above the
`cmp #20` branch.  Branch-shape (`form a`/`form b`), initialisation and
declaration-order variants all leave the spill hoisted; `form b` matches the
original's `ble/movs #20/b` shape exactly but is still +10 raw overall.

The 200/214-instruction diff block in `base.diff.txt` is the race-0 non-14
hull block and is entirely the same register swap (the instruction sequence
itself is close); it is not a scheduling difference.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13542
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb      # raw=1164 aligned=400 size=2764/2764
python3 scratch/w13542/recinfo.py cmake-build-match/libgof2hdaa.so
# -> rec=336 cst=315 rows=57
```

Scratch generators used: `gen_gen.py` (independent top/emp/sg drops + any
single guard from the `gen_td.py` table), `gen_rn2.py` (regular-new matrix),
`gen_ep.py` (empPoints forms), `gen_misc.py` (cargo / missionPtr),
`gen_big.py` (guard spanning the whole pre-Player region), `gen_ship2.py`,
`gen_decl.py`, `gen_arm.py`, `gen_spill.py`, `gen_cat.py` (try/catch),
`recinfo.py` (record/cst/rows for one function).

## 5. Files changed

* this note only; `src/game/world/Level.cpp` is unchanged from `HEAD`.
