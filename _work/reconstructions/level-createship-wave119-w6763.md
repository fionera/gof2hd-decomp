# Level::createShip — wave 119 (w6763): 74.4 % is a hard local optimum

**Status:** research note, 2026-11-05. No source change landed. Worktree
`wt/w6763`, branch `wave119-w6763`. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`).

## Result

```
createShip fuzzy (raw)            74.4 (unchanged)
createShip size                   2764 B (unchanged)
createShip LSDA record            328 B (original: 336 B, -8)
Generator::createMission LSDA     168 B (original: 172 B, -4)
.ARM.extab size / addr            0x2628c (original 0x26298, -12)
.text                             0x1337c8 (unchanged)
.rel.dyn                          0x7048 (unchanged)
byte_exact / linked_exact         4505 / 4505
imports / data / lint / drift     207 / 843 / CLEAN / 0
```

`check`: **RATCHET PASS**. No section, import, data or symbol movement.

## Corrected `.ARM.extab` accounting (the hint's premise is stale)

Parsing `.ARM.exidx` + `.ARM.extab` of both links (3566 records each,
identical function sets) gives exactly **two** records with a size
difference — there is no anonymous `0x19ae8c` record:

| function | original | ours | delta |
|---|---:|---:|---:|
| `Generator::createMission` @`0x971b8` | 172 | 168 | **-4** |
| `Level::createShip` @`0xc0214` | 336 | 328 | **-8** |
| total | | | **-12** |

`0x26298 - 0x2628c = 12` matches. The wave-119 hint's "createShip 332 B,
4 B short" was measured on the pre-`w6729` shape; HEAD's record is 328 B.
A 336 B createShip record (typed catch, below) moves `.ARM.extab` to
`0x26294`, still 4 B short because `createMission` is also -4. Closing
the extab gap therefore needs **both** records, not just createShip.

## Where the instruction streams first diverge

The first 100 instructions are byte-identical. The first structural diff
is a single spill store:

```
orig  c031a cmp r0,#20          ours  c031a cmp r0,#20
      c031c ble.n c0322               c031c str.w r9,[sp,#56]   <- spill of `this`
      c031e movs r0,#20               c0320 ble.n c0326
      c0320 b.n c0328                 c0322 movs r0,#20
      c0322 ldr r0,[r4,#0]            c0324 b.n c032c
      c0324 blx getLevel              c0326 ldr r0,[r4,#0]
      c0328 add.w r0,r0,r0,lsl#2      c0328 blx getLevel
      c032c cmp r6,#1                 c032c add.w r0,r0,r0,lsl#2
      c032e add.w sl,r0,#40           c0330 cmp r6,#1
      c0332 str.w r9,[sp,#56]         c0332 add.w sl,r0,#40
      c0336 str.w fp,[sp,#52]         c0336 str.w fp,[sp,#52]
      c033a bne.n c0352               c033a bne.n c0352
```

Only the `str.w r9,[sp,#56]` (spill of `this`) moved: ours spills it at
the end of the `empPoints` ternary's *condition* block, the original at
the end of the *merge* block. Everything from `cmp r6,#1` on is shifted
by 4 bytes but otherwise identical up to the branch.

The cause is a live-range-split choice in the greedy register allocator
(`-mllvm -print-after-all` on the current source: `%681` = `this` is
stored to `%stack.0` at 2648B, in `bb.14`, right after
`t2CMPri %196, 20`; the original keeps it in `r9` until the merge block).
It is downstream of the same RA state that makes the body differ:

| value | original | ours |
|---|---|---|
| `ship` | `r9` | `fp` |
| `volumes` | `fp` | `r9` |
| `fixed` (capital) | `r9` (coalesced with ship) | `r6` (then `mov fp,r6`) |
| `fighter` | `sl` | `fp` (coalesced with ship) |
| x / y / z (capital) | `sl` / `r8` / `r4` | `r6` / `r8` / `sl` |
| `this` reload | per branch (`r6` at `c071e`/`c0b5e`) | once at `c03fc` into `sl` |

Because ours keeps `this` in `sl` across both branches, `sl` is not
available for `fighter`/`x`, so `ship`/`volumes` land on `fp`/`r9`
instead of `r9`/`fp`. Normalising register names alone would raise the
raw score to **83.8**, so the remaining ~9.4 points are pure RA.

## What was tried this wave (all measured with the official scorer)

All rows 2764 B unless noted. `extab` is the section size.

| variant | raw % | extab | note |
|---|---:|---:|---|
| **HEAD (tuned spawn, mixed int x / float y,z, try/trap)** | **74.4** | 0x2628c | best |
| jitter call order YXZ | 74.4 | 0x2628c | tie |
| jitter call order YZX (HEAD) | 74.4 | 0x2628c | tie |
| jitter call order XYZ / ZYX | 74.2 | 0x2628c | worse |
| jitter call order XZY / ZXY | 74.1 | 0x2628c | worse |
| waypoint order wz,wy,wx (HEAD) | 74.4 | 0x2628c | tie |
| waypoint order wy,wz,wx | 74.2 | 0x2628c | worse |
| other 4 waypoint orders | 73.0-73.1 | 0x2628c | worse |
| hitpoint switch 0x2c,0x33,0x31 (HEAD) | 74.4 | 0x2628c | tie |
| hitpoint switch 0x2c,0x31,0x33 | 74.4 | 0x2628c | tie |
| other 4 switch orders | 73.4-73.8 | 0x2628c | worse |
| `setShipGroup(..., group)` / `getShipGroup(..., group)` (HEAD) | 74.4 | 0x2628c | tie |
| `getShipGroup(..., false)` | 74.4 | 0x2628c | tie |
| `setShipGroup(..., false)` | 74.3 | 0x2628c | worse |
| `setShipGroup(..., hostile)` | 73.9 | 0x2628c | worse |
| `getShipGroup(..., hostile)` | 73.8 | 0x2628c | worse |
| `armor` as `int armor = 1000; if (...) armor = 650;` | 74.4 | 0x2628c | tie |
| `armor` if/else | 74.4 | 0x2628c | tie |
| `hitpoints` base as if-form | 73.5 | 0x2628c | worse |
| cargo test `0x31 || 0x2c` | 74.4 | 0x2628c | tie |
| cargo test split into two `if`s | 74.3 | 0x2628c | worse |
| `KIPlayer *ship = 0` / `NULL` / decl top / split assign | 74.4 | 0x2628c | tie |
| `PlayerFixedObject *ship` + casts | 35.1 | 0x26290 | 2700 B |
| `ship = fixed` moved before `setBV` | 74.4 | 0x2628c | tie |
| `volumes` allocated before `fixed` | 59.3 | 0x26290 | 2760 B |
| remove `kiFighter` alias | 74.4 | 0x2628c | tie |
| `if (!shipClass)` | 74.4 | 0x2628c | tie |
| `if (shipClass != 1)` (body unchanged) | 34.8 | 0x26290 | 2700 B |
| `this->lodManager` / `this->missionPtr` | 74.4 | 0x2628c | tie |
| `Level *self = this` + `self->` | 74.4 | 0x2628c | tie |
| `int mp = missionPtr` local | 63.7 | 0x2628c | 2744 B |
| `float zero = 0.0f` for the 24 `BoundingAAB` ex args | 74.4 | 0x2628c | tie |
| `const` / `static const` zero | 74.4 | 0x2628c | tie |
| `empTime` declared before `empPoints` | 74.4 | 0x2628c | tie |
| `int empTime = 15000; if (...) empTime = 45000;` | 74.4 | 0x2628c | tie |
| `empTime` ternary | 72.3 | 0x2628c | worse |
| `empPoints` via a local `empLevel` | 70.7 | 0x2628c | 2756 B |
| `empPoints` computed after the branch | 63.2 | 0x2628c | 2768 B |
| `empPoints` if/else, temp, split mul-add | 74.4 | 0x2628c | tie |
| `empPoints` inverted `<=` ternary | 74.3 | 0x2628c | worse |
| `empPoints` `20 < g()` / `g() >= 21` | 74.4 | 0x2628c | tie |
| all-int spawn, faithful (x=wz+jX, y=wy+jY, z=wx+jZ) | 61.0 | 0x2628c | 2752 B |
| all-int spawn, tuned | 61.0 | 0x2628c | 2752 B |
| all-int spawn inserted before/after `new`/`try` | 61.0 | 0x2628c | 2752 B |
| `int x,y,z;` group decl + assignment | 61.0 | 0x2628c | 2752 B |
| all-float spawn (faithful) | 64.9 | 0x2628c | 2748 B |
| `int xi; float x = (float) xi;` | 64.8 | 0x2628c | 2748 B |
| no `try`, placement new | 70.5 | 0x26280 | 2776 B |
| no `try`, real `new` for all three | 64.6 | 0x26294 | 2768 B |
| all-int + no `try` + real `new` | 63.1 | 0x26294 | 2788 B |
| all-int + `try` + real `new` | 61.6 | 0x262a4 | 2792 B |
| `try` around `new Player` | 62.0 | 0x26294 | 2780 B |
| `try` around `new Player` + `setEmpData` | 60.3 | 0x26294 | 2776 B |
| catch body `throw;` | 49.8 | 0x262a0 | 2740 B |
| catch body `return nullptr;` | 66.4 | 0x2628c | 2772 B |
| catch body empty | 65.5 | 0x2628c | 2772 B |
| catch body `__builtin_unreachable()` | 66.4 | 0x2628c | 2760 B |
| catch body `Globals::status->getLevel()` + trap | 46.0 | 0x262a0 | 2748 B |
| **typed `catch (std::bad_alloc &)` + catch-all** | **74.4** | **0x26294** | **gate FAIL byte 4234** |
| typed `catch (std::exception &)` + catch-all | 74.4 | 0x26294 | gate FAIL byte 4230 |
| typed `catch (std::bad_exception &)` + catch-all | 74.4 | 0x26294 | gate FAIL (dynsym shift) |
| typed `catch (AbyssEngine::String &)` + catch-all | 74.4 | 0x26294 | `.rel.dyn` 0x7050 |
| typed `catch (FileInterface &)` / `AENormalFile` / `AEPakFile` | 74.4 | 0x26294 | `.rel.dyn` 0x7050 |
| typed `catch (std::bad_alloc &)` only (no catch-all) | 66.3 | 0x26290 | 2772 B |
| typed `catch (std::bad_alloc &)` empty + catch-all | 41.0 | 0x26294 | 2776 B |

## Why the typed catch still cannot land

`catch (std::bad_alloc &)` before `catch (...)` restores the 336-byte
createShip record and keeps 74.4, but gold orders dynamic symbols by
first reference and `Level.o` is line 113 of `tools/source_order.txt`.
`_ZTISt9bad_alloc` moves from dynsym 5524 to 3438, which rewrites
`.dynsym`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt` and the
link-time relocation addends inside hundreds of literal pools:
`byte_exact` drops 4505 -> 4234 (`linked_exact` stays 4505). The same
happens for every libc++ exception type (`std::exception` -> byte 4230,
`std::bad_exception` -> 5316 -> 5317). Project classes keep their early
dynsym index but need a new GOT slot (`.rel.dyn` 0x7048 -> 0x7050).
No typeinfo in the current link has both an early dynsym index and a
pre-existing GOT slot, so the typed-catch route is gate-dead.

## Remaining root cause

One live-range-split decision in the greedy register allocator:

1. the original spills `this` (`r9`) at the end of the `empPoints` merge
   block (`c0332`), reloads it per branch (`r6` at `c071e`/`c0b5e`) and
   uses `sl`/`fp`/`r9` for `fighter`/`volumes`/`ship`;
2. ours spills `this` one block earlier (`c031c`), reloads it once into
   `sl` (`c03fc`) and is therefore forced to use `fp`/`r9` for
   `ship`/`volumes`.

Every source shape tried this wave (spawn type/order/position, jitter
order, waypoint order, switch order, declaration order, `ship`/`fixed`/
`volumes` aliasing, `this` aliasing, try placement, catch body, typed
catch) either ties 74.4 or loses. The current source is a hard local
optimum for this function; a source shape that makes the allocator split
`this` at the merge block (or that reproduces the original's
no-catch/real-`new` block structure without the cleanup-pad live-range
perturbation) is still open.

## Reproduction

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 tools/verify/verify.py --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
```

Scratch helpers (not committed): `/dev/shm/w6763/measure.py` (pct /
size / extab / text / rel.dyn in ~0.3 s), `sweep10.py`-`sweep33.py`
(the sweeps in the table), `show.txt` (side-by-side disasm),
`pipeline.txt` (`-mllvm -print-after-all` for the `this` spill).
