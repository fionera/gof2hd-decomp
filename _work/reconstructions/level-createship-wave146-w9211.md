# `Level::createShip` — wave 146 (w9211): the case-3 `this` reload is
# downstream of `type` being spilled at function entry in the `iif` shape.
# No landable (size-exact) source shape found; note-only.

Status: **note-only** (no `src/` change landed; master body kept).
Gate on the unchanged tree: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, data 843/843, imports 207, lint CLEAN.
Worktree `wt/w9211`, base master `31297fadc`.  Original binary
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`, image base 0x10000).

Target `_ZN5Level10createShipEiiiP8Waypointbb` (`Level::createShip`),
original 2764 B @ `0xc0214`.  Raw same-address diff bytes (not the
PYTHONHASHSEED-dependent fuzzy %) is the metric; baseline **1978**.

## 0. Baseline reproduced

```
master (float z, float y, int x)   diff 1978  size 2764  .text 0x1337c8 (exact)
iif    (int x, int y, float z)     diff 1647  size 2768  .text +4
```
`iif` reproduces wave-145's 1647 / 2768 exactly.  `.ARM.extab` total stays
0x26298 (size exact).

## 1. New root cause: `this` -> `sl` is caused by `type` being spilled at entry

Wave-145 stopped at "`this` is in `sl` and `sl` is reused for `pos.x`".
The *upstream* cause is the entry parameter map:

| value | original | iif |
|---|---|---|
| `race`   | `[sp,#60]` | `[sp,#56]` |
| `type`   | **`fp` until `c0336`**, then `[sp,#52]` | **`[sp,#60]` at entry** |
| `shipClass` | `r5` -> `sl` -> `r6` | `sl` until `c0330`, then `[sp,#48]` |
| `this`   | `r9` -> `[sp,#56]` at `c0332` | `r9` -> `[sp,#52]` at `c0330` |

Original `c032c..c0336`:
```
c032c cmp   r6,#1            ; shipClass in r6
c032e add.w sl,r0,#40        ; empPoints -> sl   (fp still holds type)
c0332 str.w r9,[sp,#56]      ; this -> memory
c0336 str.w fp,[sp,#52]      ; type -> memory
```
`iif` `c0328..c0330`:
```
c0328 cmp.w sl,#1            ; shipClass in sl
c032c add.w fp,r0,#40        ; empPoints -> fp   (fp free: type already spilled)
c0330 strd  sl,r9,[sp,#48]   ; shipClass + this -> memory (one 4-B store)
```
So in `iif` `type` is spilled at `c0220` (`str r3,[sp,#60]`), which frees
`fp` for `empPoints`, which frees `sl` for `this`.  `this` then has to be
reloaded every time `sl` is overwritten with `pos.x` (`vmov sl,s20`), i.e.
at `c03e2`, `c06aa`, `c0a7a`, `c0b36`.  In the original `type` stays in
`fp` until `c0336`, `empPoints` takes `sl`, and `this` has no register left
so it stays in memory and is loaded lazily once per branch
(`c071e` fighter / `c0b5e` capital), which is 4 B cheaper at the case-3
merge (`ldr r4,[sp,#52]` vs `ldr.w sl,[sp,#52]; ldr r4,[sp,#60]`).

The `strd sl,r9,[sp,#48]` (4 B) vs the original's two `str.w` (8 B) is the
`-4` at the first capital block that wave-145 measured; the `+4` at the
last capital block is the `this` reload.  Net +4.

## 2. The exact structural difference is the `fixed`/`volumes` register swap

Original: `fixed = r9`, `volumes = fp`, `pos.x = sl`, `this` = memory.
`iif`:    `fixed = fp`, `volumes = r9`, `pos.x = sl`, `this` = `sl`.

If `fixed` takes `r9` and `volumes` takes `fp` (as the original), `this`
cannot take `sl` and must spill, which removes the reload.  The only
source lever found that performs this swap is the allocation order:

```
PlayerFixedObject *fixed;  Array<BoundingVolume *> *volumes;
volumes = new Array<BoundingVolume *>();     // <- new order swapped
fixed   = new PlayerFixedObject(...);
```
=> **2310 / 2760** (same 2760 as wave-145's "volumes before fixed").
It is the right register swap but (a) the `new` order is unfaithful
(original: `new PlayerFixedObject` `c0408`, `new Array` `c0448`) and
(b) it overshoots by 4 B (2760 vs 2764).  Declaring both before the
assignments but assigning `fixed` first is codegen-neutral (1647 / 2768).

## 3. Sweeps tried this wave (all full-link, one function)

| variant | raw diff | size |
|---|---:|---:|
| `iif` (baseline of this wave) | 1647 | 2768 |
| `iif` + `Level *self = this;` at top, used for both lodManager + missionPtr + setLevel | 1647 | 2768 |
| `iif` + `Level *self = this;` inside capital branch | 1647 | 2768 |
| `iif` + `Level *self = this;` right before `if (shipClass==0)` (+uses) | 1647 | 2768 |
| `iif` + `LODManager *lm = lodManager;` at top / before `if` (used) | 2413 / 2198 | 2752 / 2760 |
| `iif` + `lm` inside capital branch | 2457 | 2772 |
| `iif` + `int mp = missionPtr;` local | 1647 | 2768 |
| `iif` + explicit `this->missionPtr` / `this->lodManager` | 1647 | 2768 |
| `iif` + `lodManager->addObject` moved before `setShipGroup` | **1644** | 2768 |
| `iif` + `Vector pos;` field-wise assignment | 1647 | 2768 |
| `iif` + `float fx/fy/fz` temps | 1647 | 2768 |
| `iif` + `x = wx - 20000 + jitterX` (arith reorder, x or y) | 1647 | 2768 |
| `iif` + jitter order X/Z/Y or Z/Y/X | 1647 | 2768 |
| `iif` + waypoint order wz/wy/wx | 1683 | 2768 |
| `iif` + `level` inlined | 1666 | 2768 |
| `iif` + `armor` inlined | 1764 | 2768 |
| `iif` + `pos`/`fixed`/`volumes` const | 1647 | 2768 |
| `iif` + `BoundingAAB((float)x,(float)y,(float)z,...)` | 1647 | 2768 |
| `iif` + `case 3` before `case 0` | 1656 | 2768 |
| `iif` + `setWreckedMeshId` moved before the AABs in case 3 | 1750 | 2768 |
| `iif` + both locals declared first, `volumes` assigned first | 2310 | **2760** |
| `iif` + `KIPlayer *ship;` (uninitialised) | 2322 | **2760** |
| `iif` + `KIPlayer *ship = 0;` | 1647 | 2768 |

No variant is both size-exact (2764) and better than master's 1978.

## 4. `.ARM.extab` record (the "93")

The createShip unwind entry is at `0x1ab09c` (both builds, size-exact
section); the inline LSDA starts at `extab+8` = `0x1ab0a4` and is 328 B.
Master: **93 differing LSDA bytes** (matches the task's "extab 93").
`iif`: 73 differing bytes.  All differences are in the call-site table
(`cs_start`/`cs_len`/`cs_lp` uleb128 triples) and are a pure consequence
of the `.text` layout; the header (`personality = 0x5eb60`, LSDA prel31)
and the action table are identical.  Matching the code byte-for-byte will
match the LSDA, so there is no independent extab lever here.

## 5. Conclusion / open item

The remaining `iif` gap is not a `this`-reload codegen quirk that a local
alias can fix (every alias form coalesces away).  It is the entry
parameter map: `type` must stay in `fp` until `c0336` so that `empPoints`
takes `sl` and `this` is forced to memory.  The `iif` coordinate vector
spills `type` at entry instead.  A source shape that keeps `type` in a
register through the `shipClass==1` hitpoint block *and* keeps the `iif`
diff win would land it; the `fixed`/`volumes` swap is the right direction
but the only lever that performs it changes the `new` order and
overshoots to 2760.

## 6. Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w9211/fdiff.py _ZN5Level10createShipEiiiP8Waypointbb
```
Scratch `/dev/shm/w9211/`: `fdiff.py`, `try.py` (replace body + rebuild +
score), `sweep*.py`, `seqdiff.py` / `sizediff.py` (instruction and
per-hunk byte deltas), `orig.asm` / `master.asm` / `iif.asm`,
`exidx.py` / `extabdump.py` (unwind/LSDA locator).
