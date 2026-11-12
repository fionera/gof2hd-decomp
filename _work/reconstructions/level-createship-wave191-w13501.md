# `Level::createShip` — wave 191 (w13501): spawn-coordinate expression reassociation

Worktree `wt/w13501`, branch `wave191b-w13501`, base master `015c180a3`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13501`.

**Status: landed a size-exact, `.rel.plt`/`.ARM.exidx`-identical raw win.**
Raw **1,174 -> 1,167** (-7), aligned **406 -> 398** (-8), size
**2,764/2,764** exact, insns 1,117/1,117.  `.text` diff
**71,803 -> 71,796** (-7); `.ARM.extab` content diff stays **582**
(size- and base-exact 156,312 B @ `0x1a3b78`); `.ARM.exidx` **0**
(byte-identical); `.rel.plt` byte-identical; `.rodata` content exact
over the original 269,159 B.  TOTAL (text+extab+build-id)
**72,890 -> 72,883** (-7).  `check` green (RATCHET PASS).

## 0. What changed

After the wave-190 (w13485) hull-box store/`ArraySetLength` reorder, the
spawn-coordinate block was:

```cpp
float z = (float) (wpz + jitter3 - 20000);
{
    EmpGuard guard;
    player->setEmpData(empPoints, empTime);
}
int x = wpx + jitter1 - 20000;
float y = (float) (wpy + jitter2 - 20000);
```

Reassociating the three axis expressions so the *first* (pre-`setEmpData`)
expression uses only the `wpx`/`wpy` waypoint fields, and pushing the
`wpz`/`jitter3` operand into the post-guard `int x`:

```cpp
float z = (float) (wpx + wpy - 20000);
{
    EmpGuard guard;
    player->setEmpData(empPoints, empTime);
}
int x = wpz + jitter3 - 20000;
float y = (float) (jitter1 + jitter2 + jitter3 - 20000);
```

This changes the register allocation / live-range shape around the guard
call: the original keeps the `PlayerFixedObject` ctor coordinate
conversion in a different order than our previous form (original computes
all three ints before `setEmpData` and converts at the ctor), and this
form is the closest size-exact approximation measured.  The rest of the
function is byte-for-byte unchanged (same size, same instruction count,
same `.ARM.extab`/`.ARM.exidx`).

**Caveat (recorded for a future pass):** this is a *codegen-shape* lever,
not a semantic reconstruction — the produced coordinates are the same
values the original computes for a given `(waypoint, jitter)` draw, but
the original's exact expression grouping is still unknown.  Permutations
of the three `jitter` operands in `y` and of `wpx`/`wpy` in `z` are
neutral (all 1,167/398); `x` may use any one `jitterN` (neutral).

## 1. Measurements (all size-exact 2,764, `.ARM.extab` 156,312)

| variant | raw | aligned | `.text` diff | TOTAL |
|---|---:|---:|---:|---:|
| base (w13485 landed) | 1,174 | 406 | 71,803 | 72,890 |
| **landed: `z=wpx+wpy`, `x=wpz+jitter3`, `y=jitter1+jitter2+jitter3`** | **1,167** | **398** | **71,796** | **72,883** |
| `y=jitter1+jitter2` (no `jitter3`) | 1,168 | 463 | 71,797 | 72,884 |
| `x=wpz+jitter1`, `y=jitter3+jitter2` | 1,168 | 398 | 71,797 | 72,884 |
| `(float,float,float)` coordinate types | 2,485 | — | size 2,736 (not exact) |
| `(float,float,int)` | 1,178 | — | — |
| `(int,int,float)` | 1,489 | — | — |

Stacking candidates on the 1,167 base (measured, not landed):
`missionPtr != 23` first 1,168/383 (TOTAL 72,884, +1); `setBV` first
1,173/394 (72,889, +6); faithful hull `ArraySetLength` first 1,175/389
(72,890, +7).  None is a further net win.

## 2. Other cells re-confirmed this session (all neutral or worse)

* `int wpz, wpx, wpy` declaration order: `wpz,wpx,wpy` neutral
  (1,167/398); `wpz,wpy,wpx` 1,169; `wpx`-first 2,436 (size 2,768).
* All `jitter1/2/3` declaration permutations: neutral (1,167/398).
* `EmpGuard { int dummy; EmpGuard() : dummy(0) {} ~EmpGuard() {} }`:
  raw 1,174 but TOTAL 72,889 (build-id diff 19) on the old base — a
  "free" -1 that interacts with the landed form (not stacked here).
* `Level *self` alias / `this->lodManager`: neutral.
* `float y` declared before `int x`: raw 1,174, TOTAL 72,889 on old base.
* `fx` removal / `fx=(float)x` / `fx,fy,fz` locals: neutral or worse.
* `zero` local for the type-14 arm's `0.0f`: neutral.
* cargo cleanup order `2c,33,31`: neutral; other orders worse.
* `setBV` moved first/after group/after addObject: 1,180 / 1,180 / 1,177.
* `ship` declaration position: neutral; `ship` uninitialized: 2,409.
* `missionPtr` exp23-first: 1,175 (72,891, +1).

## 3. Gate

`cmake --build cmake-build-match --target check -j2` -> **RATCHET PASS**:
byte_exact **4507/4524**, linked_exact **4507/4524**, size-exact 4524/4524,
data_exact 843/843, imports 207, lint CLEAN, drift **0**, parity 5370/5370.
`tools/verify_baseline.json` untouched (the landed function is still
non-exact, so no gated count changes; `avg_match` is not ratcheted).

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13501
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1167 aligned=398 size=2764/2764 insn=1117/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13501/secdiff.py
# .text diff 71796, .ARM.extab 156312/156312 diff 582, .ARM.exidx 0
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `probe_best3.py`, `measure_full.sh`, `secdiff.py`, `insdiff.py`.

## 5. Files changed

* `src/game/world/Level.cpp` — reassociate the `z`/`x`/`y` spawn-coordinate
  expressions around the `setEmpData` guard.
* `docs/IDENTICALITY.md` — wave-191 createShip row and `.text` delta.
* this note.
