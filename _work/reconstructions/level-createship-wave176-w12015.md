# `Level::createShip` — wave 176 (w12015): jitter declaration order, raw 1,232 -> 1,229

Worktree `wt/w12015`, branch `wave176-w12015`, base `master b21088985`
(wave 175).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: source change landed.**  The three `Globals::rnd->nextInt(40000)`
declarations are emitted in the order `jitter3, jitter2, jitter1` instead of
`jitter1, jitter2, jitter3`.  `createShip` raw **1,232 -> 1,229** (-3),
aligned **422 -> 380** (-42), size **2,764/2,764** exact, insns
**1,118/1,117 -> 1,117/1,117** (exact).  `.text` differing bytes
**80,166 -> 80,163** (-3).  The `.ARM.extab` record stays the original
**336 B / 57 rows / `ttype 0xff`** cleanup-only shape, so `.ARM.extab` is
size- and base-exact (`0x26298` @ `0x1a3b78`) and `.ARM.exidx` /
`.rel.plt` stay byte-identical.

## 1. Result

| metric | before (wave 175) | after |
|---|---:|---:|
| raw same-address diff | **1,232** | **1,229** |
| instruction-index alignment | **422** | **380** |
| size | 2,764 | 2,764 |
| insns (orig/ours) | 1,117 / 1,118 | **1,117 / 1,117** |
| `.text` differing bytes | 80,166 | **80,163** |
| `.ARM.extab` size | `0x26298` (exact) | `0x26298` (exact) |
| `.ARM.extab` differing bytes | 746 | 746 |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | byte-identical |
| `.rodata`/`.data`/`.got`/`.dynsym`/`.dynstr`/`.hash`/`.rel.dyn` | identical | identical |

Full `check`: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, symbol parity 5370/5370,
`RATCHET PASS`.

## 2. The landed change

```cpp
int jitter3 = Globals::rnd->nextInt(40000);
int jitter2 = Globals::rnd->nextInt(40000);
int jitter1 = Globals::rnd->nextInt(40000);
```

The three calls are independent (the RNG state is the only side effect and
the three values are summed separately into `x`, `y`, `z`), so the order is
semantically free.  The original binary stores `jitter1` at `sp+36`,
`jitter2` at `sp+32`, `jitter3` at `sp+28` (`c026e`/`c0278`/`c028c`), i.e.
the last-emitted value gets the lowest stack slot.  Emitting them 3,2,1
moves the spill slots of the three jitters one step closer to the original
layout, which removes 42 aligned instruction mismatches (mostly
`operand:ldr` stack-offset operands) and 3 same-address bytes.  It does not
change the EH shape at all: the cleanup-only LSDA record is unchanged
(336 B, 57 rows, `ttype 0xff`), so `.ARM.extab`/`.ARM.exidx` stay exact.

Splitting the declaration (`int jitter3, jitter2, jitter1;` then
assignments) or moving the declaration to the top of the function gives the
same 1,229/380; the in-place form above is the cleanest.

## 3. `.ARM.extab` record accounting (why the try/catch was 8 B short)

Decoded with `_work/reconstructions/extab_lsda_decode.py` and
`scratch/w12015/recsizes.py` (createShip is the only record with a size
difference; every other record is exact):

| shape | record | rows | call-site table | `ttype` | `.ARM.extab` |
|---|---:|---:|---:|---|---|
| original | **336 B** | **57** | 315 B | `0xff` (cleanup only) | `0x26298` |
| wave-173 `try`/`catch(...)` around `setEmpData` | 328 B | 54 | 299 B | `0x00` (catch-all) | `0x26290` (-8) |
| wave-174/175 3 cleanup guards (base) | **336 B** | **57** | 315 B | `0xff` | `0x26298` |
| `setEmpData`+`setLevel` guards only | 332 B | 56 | 310 B | `0xff` | `0x26294` (-4) |

The original is a **cleanup-only** record: 28 landing pads, one per `new`
expression (operator-new is a no-pad gap row, the constructor call is the
cleanup pad), 29 gap rows, `ttype 0xff`, no type table.  `catch (...)` adds
a catch-all type-table entry and drops the two placement-`new` cleanup pads,
which is the 8-byte shortfall.  The 3-guard shape reaches 336 B by adding
three guard pads and dropping the two placement-`new` pads; the record size
matches but the row structure does not.

The record size is `16 B header + 4-byte-aligned LSDA`.  The 3-guard record
is `5 B LSDA header + 315 B call-site table` = 320 B (no padding); the
2-guard record is `5 + 310 = 315`, padded to 316 -> 332 B.  Adding a
5-byte call-site row and losing the 1-byte pad is exactly +4 B.

## 4. Why the 2-guard shape (raw 1,196) cannot reach 336 B

Dropping the `mission` guard gives the `setEmpData`+`setLevel` shape:
raw **1,196**, aligned 445, size 2,764, `.ARM.extab` **332 B** (4 short).
With the jitter 3,2,1 order it is raw **1,193**, aligned **353** -- better
than the landed 1,229 -- but still 4 B short of the section size.  A 5-byte
call-site row is only produced by a guard whose call site starts below
offset 128 (so the start ULEB stays 1 byte).  The only such call in the
function that can carry a guard without changing the hot path is
`getCurrentCampaignMission` at offset 28; the `nextInt` calls at 88/98/108
and the first `getLevel` at 114 all change the code/register allocation
badly when guarded (raw 2,261..2,467, size 2,748..2,760).  The mission
guard is therefore the only +4 shape, and it costs 36 raw (1,193 -> 1,229)
because its cleanup landing-pad thunk is a new 2-byte `b.n` in the tail and
shifts every other thunk branch target.

Measured variants (full link, size-checked; `.ARM.extab` is the section
size; `raw`/`aligned` are the `fn_diff` metrics):

| shape | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---|
| 3 guards, jitter 1,2,3 (base) | 1,232 | 422 | 2,764 | `0x26298` |
| **landed: 3 guards, jitter 3,2,1** | **1,229** | **380** | **2,764** | **`0x26298`** |
| 3 guards, jitter 3,2,1, `int x,y,z` | 2,365 | 500 | 2,756 | `0x26298` |
| 3 guards, jitter 3,2,1, `float x,y,z` | 2,364 | 514 | 2,772 | `0x26298` |
| 2 guards (`setEmpData`+`setLevel`), jitter 1,2,3 | 1,196 | 445 | 2,764 | `0x26294` |
| 2 guards, jitter 3,2,1 | 1,193 | 353 | 2,764 | `0x26294` |
| `try`/`catch(...)` only (wave 173) | 1,199 | 351 | 2,764 | `0x26290` |
| `try`/`catch(...)` + mission guard | 1,426 | 420 | 2,768 | `0x26298` |
| `try`/`catch(...)` + `setLevel` guard | 1,452 | 466 | 2,768 | `0x2629c` |
| plain `new` (all 28 sites), no guards | 1,981 | 477 | 2,764 | `0x26298` |
| plain `new`, no guards, jitter 3,2,1 | 1,978 | 480 | 2,764 | `0x26298` |

The plain-`new` shape is the faithful row structure (28 pads, 57 rows,
`ttype 0xff`) but its raw is far worse: without an invoke/guard barrier the
scheduler sinks the `x`/`y`/`z` computation past `setEmpData` (the original
computes all three as `int` before the call and converts them to `float`
only at the `PlayerFixedObject`/`PlayerFighter` constructor call).  The
guard shape keeps the coordinate block above the call and is the only shape
that is both size-exact and close in raw.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12015
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
python3 _work/reconstructions/extab_lsda_decode.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -j2
cmake --build cmake-build-match --target drift -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12015/`: `Level_pristine.cpp`
(base), `Level_j321.cpp` (landed), `Level_sesl.cpp`/`Level_sesl321.cpp`
(2-guard 332 B shape), `harness.py` (build + raw/aligned/size/extab),
`wsweep.py` (wp-declaration x jitter-order sweep), `newsweep.py`
(placement/plain-new x guard-subset sweep), `tcsweep.py` (try/catch guard
sweep), `third*.py` (third-guard placement sweep), `recsizes.py`,
`rowcalls.py`, and the `*.s` disassembly dumps.
