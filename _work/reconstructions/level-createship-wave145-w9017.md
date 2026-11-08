# `Level::createShip` — wave 145 (w9017): the `iif` +4 B is the `this` reload
# at the end of the case-3 capital block; the BoundingAAB zero-const is
# byte-neutral.  No landable source shape found (iif stays 2768 B).

Status: **note-only** (no `src/` change landed; master body kept).
Gate on the unchanged tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.
Worktree `wt/w9017`, base master `eb13ada9b`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`),
original 2764 B @ `0xc0214`.  Raw same-address diff (not fuzzy %) is the metric;
baseline **1978**.

## 0. Confirmed baseline and the `iif` shape

```
master (float z, float y, int x)   diff 1978  size 2764   .text 0x1337c8 (exact)
iif    (int x, int y, float z)     diff 1647  size 2768   .text 0x1337e8 (+32, = +4 rounded to 32B)
```
The `iif` shape is reproduced exactly as wave-144 reported: 1647 / 2768.
The `.ARM.extab` total stays **0x26298 (exact)**.

All 8 coordinate type combinations (measured this wave, full link):

| x,y,z types | raw diff | size |
|---|---:|---:|
| `iif` | **1647** | 2768 |
| `ifi` | 1651 | 2768 |
| `fii` | 1656 | 2772 |
| `iff` | 1981 | **2764** |
| `fif` | 1982 | **2764** |
| `ffi` | 1984 | **2764** |
| `fff` | 2091 | 2784 |
| `iii` | 2334 | 2792 |

Only `iff`/`fif`/`ffi` are size-exact, and all three are *worse* than master
(1978), so nothing is landable from the type vector alone.

## 1. Where the +4 B actually is (new, corrects wave-144 §2)

Wave-144 claimed the extra bytes are in the `BoundingAAB` zero-argument setup
("original reuses `r6=0`, iif materialises `movs r0,#0; str r0,[sp,#0]`").
That is **byte-neutral**: counting the capital-ship region gives

```
             str [sp,#0]   movs #0   strd    bytes
original         15           21      33      204
iif              21           27      27      204
```

The real +4 is a single instruction at the **end of the case-3 (race==0,
type!=14) block**:

```
original c0a7a  ldr  r4,[sp,#52]        ; type
         c0a7c  b.n  c0b32
iif      c0a7a  ldr.w sl,[sp,#52]       ; this   <-- +4 B
         c0a7e  ldr  r4,[sp,#60]        ; type
         c0a80  b.n  c0b3a
```

The cumulative size by `ArraySetLength` site is `-4` at the first capital
block, `0` at the next three, and `+4` only at the last one; the tail is then
identical.  So the whole delta is this one `this` reload.

Root cause is the `this` / `volumes` / `fixed` register map:

| value | original | iif |
|---|---|---|
| `this` | spilled `[sp,#56]`, reloaded into `r6` at `c071e`/`c0b5e` | `sl` (reloaded `[sp,#52]` at `c03e2` **and** `c0a7a`) |
| `fixed` | `r9` | `fp` |
| `volumes` | `fp` | `r9` |

`iif` keeps `this` in `sl` across the capital branch, but `sl` is reused for
`z` inside every `BoundingAAB` block, so it must reload `this` afterwards.
The original keeps `this` spilled and uses `r9` for `fixed`, so no reload is
needed at that point.

## 2. New sweeps tried this wave (all full-link, one function)

| variant | raw diff | size | note |
|---|---:|---:|---|
| `iif` + waypoint order `y,x,z` | 1646 | 2768 | best raw diff seen |
| `iif` + jitter order `X,Z,Y` | 1656 | 2772 | wave-144 |
| `iif` + `int z; float fz=(float)z;` | 1647 | 2768 | **identical codegen to `iif`** |
| `iif` + `float zero=0.0f` used as AAB 4th arg | 1647 | 2768 | zero still rematerialised |
| `iif` + `int zero=0` used as AAB 4th arg | 1647 | 2768 | same |
| `iif` + `Level *self = this` alias | 1647 | 2768 | same |
| `iif` + `int sc = shipClass` alias | 1647 | 2768 | same |
| `iif` + `int rr = race` alias | 1647 | 2768 | same |
| `iif` + `empTime = 15000` pre-init | 1647 | 2768 | same |
| `iif` + `missionPtr` condition inverted / not / nested | 1647 | 2768 | block layout unchanged |
| `iif` + `volumes` allocated before `fixed` | 2310 | **2760** | allocation order unfaithful |
| `iif` + `KIPlayer *ship;` (uninitialised) | 2322 | **2760** | drops the witnessed `mov.w r9,#0` |
| `fii` + `volumes` before `fixed` | 2060 | **2764** | size-exact but worse than 1978 |
| `iif` + `z=(float)(wz+jitterZ)-20000.0f` | 2356 | 2776 | float subtract |
| `iif` + coords before `new Player` | 2528 | 2808 | wave-144 |

`volumes`-before-`fixed` is a real -8 B lever (2768 -> 2760) but it swaps the
`new` order, which contradicts the original asm (`new PlayerFixedObject` at
`c0408` before `new Array` at `c0448`), so it is **METRIC-ONLY** and overshoots.

## 3. Conclusion / open item

The `iif` shape is confirmed as the best raw-diff shape (1647) but is 4 B too
long because of the `this` reload.  A shape that keeps `this` out of `sl`
(so `sl` can hold `x` and `r6`/`r9` hold the zero / `fixed` as in the original)
would land it, but no source perturbation tried changes that allocation:
`this` -> `sl` is forced by the `this`/`volumes`/`fixed` live-range order, and
the only lever that moves it (`volumes`-before-`fixed`) overshoots to 2760 and
regresses the diff to 2310.

The "witnessed local type" lever is exhausted: `float z` and
`int z; float fz = (float)z;` compile to identical code, and all 8 coordinate
type vectors were re-measured; only `iff`/`fif`/`ffi` are size-exact and all
are worse than master.

## 4. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9017/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch `/dev/shm/w9017/`: `fdiff.py`, `sweep*.py` (type/order/alias sweeps),
`seqdiff.py` / `cum2.py` (instruction-sequence and cumulative-size diffs),
`orig.asm` / `ours.asm` / `iif.asm`.
