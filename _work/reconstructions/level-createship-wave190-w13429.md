# `Level::createShip` — wave 190 (w13429): pool order / cold-block / callee-saved

Worktree `wt/w13429`, branch `wave190-w13429`, base master `40b585501`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13429`.

**Status: no landable win; `src/` left at the base.**  Base is raw **1,189**,
aligned **397**, size **2,764/2,764**, insns **1,117/1,117**, `.text` diff
**72,921**, `.ARM.extab` **156,312 B @ 0x1a3b78, diff 603**, `.ARM.exidx` 0,
`.rel.plt` byte-identical, `.rodata` content exact over the original
269,159 B (sh_size 269,351, +0xc0).  `check` green.

This pass took the three lenses the task asked for (literal-pool ordering,
branch-target/cold-block layout, callee-saved rotation) and swept them
systematically.  Every variant either ties the base exactly (raw 1,189,
`.text` 72,921) or loses.  The tie candidates are listed so later passes do
not re-run them.

## 0. What the residual actually is

Same-address instruction classification over the 2,764 B body: 85
`operand` (90 B), 69 `regalloc` (69 B), then ~700 B of stream shift.  The
first ~400 B of the function are byte-identical apart from the waypoint /
jitter stack slots; the rest is a systematic callee-saved rotation plus a
4-byte literal-pool shift.  Nothing in the diff is a semantic mismatch.

Register map (orig vs ours, from the disassembly):

| value | orig | ours |
|---|---|---|
| `fixed` | `r9` | `fp` |
| `volumes` | `fp` | `r9` |
| `x` (int, live to the ctor) | `fp` | `r4` |
| `y` | `r4` | `r6` |
| `z` | `r8` | `s16` (converted before `setEmpData`) |
| `race` | `r6` | `r4` |
| `this` | `[sp,#56]` / `r6` | `[sp,#56]` / `sl` |

Stack slots: orig `wpz/wpx/wpy = 40/44/48`, `jitter3/1/2 = 36/32/28`;
ours `wpz/wpx/wpy = 48/40/36`, `jitter3/1/2 = 44/32/28`.  `this=56`,
`type=52`, `race=60` in both.

## 1. Literal-pool ordering

The only pool mismatch in the whole function is the two words at
`0xc082c/0xc0830`: orig `{2004, -15852}`, ours `{-15852, 2004}` (from the
`[9]` hull box `ey/ez`).  The pool order equals the *load* order, which is
fixed by the `STRD` pairing: ours pairs `(ey,ez)` into one `strd`, orig
pairs `(ex,ey)` and `(ez,sx)`.  Every source-level attempt to change the
pairing is canonicalised away:

| variant | raw | aligned | .text |
|---|---:|---:|---:|
| base | 1,189 | 397 | 72,921 |
| `[10]` before `[9]` with `[9]=(2004,-15852)` | 1,192 | 399 | 72,924 |
| `[9]` before `[10]`, `[9]=(-15852,2004)` | 1,192 | 399 | 72,924 |
| named `ey9/ez9` locals | 1,189 | 397 | 72,921 |
| named `ex9..sz9` locals for all 6 box args | 1,189 | 397 | 72,921 |
| 4th arg `0` / `0.0` instead of `0.0f` | 1,189 | 397 | 72,921 |
| `-15852.0f` written `(float)-15852` | 1,189 | 397 | 72,921 |
| all 10 adjacent `[i]`/`[i+1]` swaps (not just `[9]`/`[10]`) | 1,206..1,219 | 439..447 | 72,938..72,951 |

The `[9]`/`[10]` order and `[9]`'s own `ey/ez` order are the only two that
keep raw 1,189; all the rest of the box order is already canonical.

## 2. Branch-target / cold-block layout

* **`switch (race)` case order.**  All **24** permutations of
  `case 0/1/2/3` (with `default: break;` kept immediately before `case 1`)
  were built.  `0,3,2,1` is the unique minimum at raw 1,189 / `.text`
  72,921; the next best is `0,2,3,1` at 1,274 / 73,006.  No gain.
* **`default:` placement.**  Moving `default: break;` to the end of the
  switch, or deleting it entirely, is exactly neutral (1,189 / 72,921).
* **`setWreckedMeshId` tail merge.**  The original merges the five
  `setWreckedMeshId` calls into **two** call sites (`c0a76` for race 3 and a
  shared `c0b2c` tail for the other four); ours emits **three**
  (`c06c0`, `c0a76`, `c0b30`).  Hoisting the call out of the switch with a
  `wreckId` local is much worse: raw 2,403, size 2,752 (not exact).
  The merge is a `BranchFolder` decision that depends on the callee-saved
  rotation, so it cannot be steered from source without changing the
  allocation.
* The `tbh` dispatch table at `0xc045c` is byte-identical in both.

## 3. Callee-saved rotation

Declaration/placement sweeps, all size-exact and all neutral or worse:

| variant | raw | aligned | .text |
|---|---:|---:|---:|
| base | 1,189 | 397 | 72,921 |
| jitter order `3,1,2` (current) | 1,189 | 397 | 72,921 |
| jitter order `3,2,1` | 1,189 | 485 | 72,921 |
| jitter order `1,2,3` / `1,3,2` / `2,1,3` / `2,3,1` | 1,190..1,191 | 397..485 | 72,922..72,923 |
| jitters as `int jit[3]` | 1,189 | 397 | 72,921 |
| waypoints as `int wp[3]` | 1,189 | 400 | 72,921 |
| `int wpz=0,wpx=0,wpy=0;` + if-assign | 1,189 | 397 | 72,921 |
| `int wpz,wpx,wpy;` with `z,x,y` assign order | 1,189 | 397 | 72,921 |
| waypoint block before the mission block | 1,603 | 487 | 73,335 |
| jitters before the waypoint block | 2,566 | 581 | 853,697 |
| `level` before the jitters | 2,521 | 535 | 850,728 |
| `Status *status` / `Globals *globals` locals | 2,405 / 2,572 | 594 / 1122 | 827,630 / 804,774 |
| `float fx=(float)x` / `int fx=x` / `fx` after `new` | 1,189 / 1,194 / 1,194 | 397 / 398 / 398 | 72,921 / 72,926 / 72,926 |
| no `kiFighter` local | 1,189 | 397 | 72,921 |
| no `g` local in the capital tail | 1,191 | 397 | 72,923 |

The identity jitter mapping (1st `nextInt` -> `x`, 2nd -> `y`, 3rd -> `z`,
which is what the original actually does) is *worse*: `ident_zxy` 1,191,
all-three-before-guard 1,206..1,210, `ident_x_before` 1,351.

## 4. EH / LSDA budget (the hard constraint)

Base LSDA: 336 B record, 57 rows, 315-byte call-site table, `.ARM.extab`
156,312 B.  Removing one 5-byte row gives table 310 -> extab **156,308**,
which shifts `.rodata` and explodes `.text` to 72,885 but `.ARM.extab`
content 603 -> 122,299 and `.ARM.exidx` 0 -> 1,307 (net loss).  Adding a
guard in the middle of a gap costs **two** rows (+10 B -> 156,316/156,320),
never one.

Row structure, orig vs ours (offsets relative to `0xc0214`):

```
orig                                  ours
0 (0, 406, 0, 0)                      0 (28, 4, 2496, 0)   mission
1 (406, 30, 2488, 0)  Player ctor     1 (32, 374, 0, 0)
2 (436, 94, 0, 0)                     2 (406, 30, 2486, 0) Player ctor
3 (530, 32, 2482, 0)  FixedObj ctor   3 (456, 10, 2484, 0) setEmpData
4 (562, 8, 0, 0)                      4 (466, 102, 0, 0)
5 (570, 4, 2476, 0)   Array ctor      5 (568, 4, 2476, 0)  Array ctor
...
29 (1224, 32, 2470, 0) Fighter ctor   29 (1286, 10, 2482, 0) setShipGroup
```

The original uses **real `new`** for `PlayerFixedObject` and
`PlayerFighter` (hence the ctor try-ranges) and guards neither
`getCurrentCampaignMission` nor `setEmpData`; ours uses placement `new` and
two `EmpGuard`s.  The two shapes happen to cancel to the same 57 rows /
315 bytes.  Switching to the faithful shape (no top guard, no `setEmpData`
guard, real `new PlayerFixedObject`) is size-exact with the right extab but
raw **2,026** — the real-`new` cleanup path is far more code.

Removing only the top guard (`notop`) is the biggest raw win available:
raw **1,153**, aligned **370**, `.text` **72,885** (-36), size exact — but
extab 156,308.  Every compensating guard on that body lands at 156,316 or
156,320.  A single +5-byte row only exists at a try-range boundary; the
mission row itself is that row, so `notop` cannot be made extab-exact from
source.

## 5. Other cells re-confirmed (all neutral, 1,189 / 72,921)

* `missionPtr` forms: `expect(!=1)&&!=23` (base), `!=23&&!=1` 1,190,
  no-expect / `!(==1||==23)` 1,196.
* cargo cleanup `0x2c,0x33,0x31` neutral; `0x33`-first 2,036; `switch` 1,192.
* `mission == 49||50||51||52||56` as a range / reversed / `switch`: neutral.
* `empPoints` clamp `<=`/`<21` neutral; `>`/`>=21` 1,196 (aligned 395).
* `armor` ternary, `hitpoints` scaling, `level` ternary: unchanged.
* `EmpGuard` `noexcept(false)`, `mission = 0`, `struct` at namespace scope:
  neutral.
* `notop` + `ship->setLevel` guard: 1,189 / 156,320.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13429
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1189 aligned=397 size=2764/2764 insn=1117/1117
python3 /home/debian/GalaxyOnFire2/scratch/w13429/textdiff.py \
  cmake-build-match/libgof2hdaa.so
# .text diff 72921, .ARM.extab 156312/156312 diff 603, .ARM.exidx 0
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch: `sw.py`..`sw33.py` (sweeps), `sidediff_base.txt`, `lsda_base.txt`,
`textdiff.py`, `lsda.py`, `diff.py`.

## 7. Files changed

* this note only (no `src/` change; tree left clean at the base).
