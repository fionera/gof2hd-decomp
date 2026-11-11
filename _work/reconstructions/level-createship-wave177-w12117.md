# `Level::createShip` — wave 177 (w12117): fixed-branch tail store order, raw 1,229 -> 1,225

Worktree `wt/w12117`, branch `wave177-w12117`, base `master 9445ac5fb`
(wave 176).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: source change landed.**  In the `shipClass == 1` (capital-ship)
branch the `fixed->setBV(volumes)` store moves from *before*
`fixed->setShipGroup(...)` to *after* `fixed->field_0x40 = 1;` (i.e. it is
now the last of the five tail statements before `ship = fixed;`).
`createShip` raw **1,229 -> 1,225** (-4), aligned **380 -> 383** (+3),
size **2,764/2,764** exact, insns **1,117/1,117** (exact).  `.text`
differing bytes **79,591 -> 79,587** (-4).  The `.ARM.extab` record stays
the original **336 B / 57 rows / `ttype 0xff`** cleanup-only shape, so
`.ARM.extab` is size- and base-exact (`0x26298` @ `0x1a3b78`) and
`.ARM.exidx` / `.rel.plt` stay byte-identical.

## 1. Result

| metric | before (wave 176) | after |
|---|---:|---:|
| raw same-address diff | **1,229** | **1,225** |
| instruction-index alignment | **380** | **383** |
| size | 2,764 | 2,764 |
| insns (orig/ours) | 1,117 / 1,117 | **1,117 / 1,117** |
| `.text` differing bytes | 79,591 | **79,587** |
| `.ARM.extab` size | `0x26298` (exact) | `0x26298` (exact) |
| `.ARM.extab` differing bytes | 1,321 | 1,321 |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | byte-identical |
| `.rodata`/`.data`/`.got`/`.dynsym`/`.dynstr`/`.hash`/`.rel.dyn` | identical | identical |

Full `check`: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, symbol parity 5370/5370,
`RATCHET PASS`.

## 2. The landed change

```cpp
        fixed->setShipGroup(Globals::globals->getShipGroup(type, race, false), type, false);
        lodManager->addObject(fixed->geometry);
        fixed->field_0x40 = 1;
        fixed->setBV(volumes);
        ship = fixed;
```

`PlayerFixedObject::setBV(Array<BoundingVolume *> *)` is a one-line field
store (`this->boundingVolumes = bv;`), `KIPlayer::setShipGroup` touches
only `shipGroupFlag`/`geometry`/`parentGeometry`/`player->transform`, and
`LODManager::addObject` only reads `geometry->hasLod()` and appends to its
object array.  None of them read `boundingVolumes`, so moving `setBV`
after them is semantically free (the relative order
`setShipGroup -> addObject` is preserved, which *does* matter because
`setShipGroup(geom, group, false)` rewrites `this->geometry`).

The move changes the scheduler's placement of the `setBV` argument setup
relative to the `setShipGroup`/`addObject` call sequence and removes 4
same-address bytes.  It also slightly perturbs the race-3/2/1 blocks
(`0x900..0x97f` bucket diffs 62/58 -> 59/56), which is where the 4 bytes
come from.

The full 120-permutation sweep of the five tail statements
(`setBV`, `setShipGroup`, `addObject`, `field_0x40 = 1`, `ship = fixed`)
on top of the wave-176 base gives:

| permutation (0=setBV,1=setShipGroup,2=addObject,3=field_0x40,4=ship) | raw | aligned |
|---|---:|---:|
| `01234` (wave-176 source order) | 1,229 | 380 |
| **`12304` (landed)** | **1,225** | **383** |
| `12340`, `12430`, `14230`, `41230` | 1,225 | 383 |
| `13024`, `13042`, `13402`, `14302`, `41302` | 1,226 | 383 |
| `21304`, `21340`, `21430`, `24130`, `42130` | 1,228 | 382 |

Every permutation keeps size 2,764 and `.ARM.extab` `0x26298`.

## 3. Sweeps that gave no change (all raw 1,229 / aligned 380 on the wave-176 base)

* **x/y/z block shape.** `int x; float y; float z;` split declaration,
  `(float)(int)(...)` explicit casts, `int` temporaries feeding `float y/z`
  (`iff_temps`), and the declaration orders `xyz`/`yxz`/`yzx` are all
  identical.  `xzy`/`zxy`/`zyx` are 1,233; `int x,y,z` is 2,365 (size
  2,756 — the `this` pointer gets spilled and the whole coordinate block
  sinks below `setEmpData`); `float x,y,z` is 2,366.
* **Guard subset.** 3 of the 10 candidate statement guards give size
  2,764 + `.ARM.extab` `0x26298`: `mission+setemp+setlevel` (the landed
  1,229 base), `mission+emp+setemp` (1,230) and `mission+armor+setemp`
  (1,594).  Every 4-guard combination is size 2,768 (one extra 4-byte
  landing pad); every 2-guard combination is `.ARM.extab` 4 B short.
* **`ship` handling.** `KIPlayer *ship = nullptr;` moved to the top of the
  function, before the `setEmpData` guard, assigned right after the
  placement-`new` in either branch, or declared without an initialiser
  (that one is size 2,756) — all 1,229.
* **`fixed`/`volumes` declaration order.** `volumes` declared first,
  `fixed` split from its `new`, `volumes` split from its `new`, `volumes`
  declared `= nullptr` then assigned, plain `new Array<...>` vs
  `new Array<...>()` — all 1,229 (the compiler normalises the
  declaration-only variants back to the same MIR).
* **`Level *self = this;` alias** for `missionPtr`/`lodManager`/`setLevel`
  — 1,229.
* **Expression-form flips** that the compiler canonicalises: `type != 14
  ? 5 : 25`, `20 + level*14 + (...)`, `40 + getLevel()*5`, the
  `switch (type)` case order `2c,31,33`, swapping the `case 0`
  `if (type == 14)` arms, and moving `default: break;` to the end of the
  race switch — all 1,229.
* **`empPoints` hoisted above the `switch (type)` / the mission-49 block**
  — 1,229 / 1,257.
* **Fighter-branch reorders** (`setShipGroup` after the `missionPtr`
  block, the `type == 0x2c || 0x31` block before the `missionPtr` block)
  — 1,761 / 2,083.
* **`fixed->setBV(volumes)` immediately after the `Array` `new`** — 1,492,
  size 2,756 (not exact).

## 4. Why the 2-guard shape still cannot be landed

Dropping the `mission` guard (the shape that matches the original's
`(0,406,0,0)` first LSDA row exactly) is the best raw shape known:

| shape | raw | aligned | size | `.ARM.extab` |
|---|---:|---:|---:|---|
| 2 guards (`setEmpData`+`setLevel`), wave-176 tail | 1,193 | 353 | 2,764 | `0x26294` (-4) |
| 2 guards, landed tail (`12304`) | **1,189** | 356 | 2,764 | `0x26294` (-4) |
| 2 guards + `empPoints` guard, landed tail | 1,225 | 383 | 2,764 | `0x262a0` (+12) |
| 3 guards, landed tail (**landed**) | **1,225** | 383 | 2,764 | **`0x26298`** |

The 2-guard LSDA is `5 B header + 310 B call-site table = 315 B`, padded
to 316 -> 332 B.  Reaching 336 B needs exactly one extra 5-byte row
(start < 128, length < 128, landing pad >= 128), and the only call below
offset 128 that can carry a guard without wrecking the register
allocation is `getCurrentCampaignMission` at offset 28 — i.e. the
`mission` guard itself, which costs 36 raw.  Guarding the `nextInt` calls
(0x58/0x62/0x6c) or the first `getLevel` (0x72) changes the allocation
badly (raw 2,261..2,467, size 2,748..2,760).  The landed tail reorder
does not change this: 2-guard + `12304` is still 4 B short.

## 5. Remaining mismatch structure

The residual 1,225 bytes are dominated by the fixed (`shipClass == 1`)
branch, `0x1c0..0x57f` and `0x880..0x9bf`, where almost every byte of the
`BoundingAAB` argument setup differs.  The instruction *sequence* is
identical; the difference is a pure register-allocation swap:

| value | original | ours |
|---|---|---|
| `fixed` | `r9` (from the `operator new` result) | `r6` then `fp` |
| `volumes` | `fp` (from the `Array` `operator new`) | `r9` |
| `type` | `fp` (kept from the prologue) | `[sp,#52]` reloaded |
| `race` | `r6` | `r4` |
| `0.0f` arg | hoisted `movs r6,#0` once | rematerialised per call |

Because `volumes` sits in `r9` instead of `fp`, every
`ldr.w r0,[fp,#4]` in the race cases becomes `ldr.w r0,[r9,#4]`, and the
`BoundingAAB` float-bit registers become `r6`/`sl` instead of `sl`/`r4`.
The trigger is that our build reloads `this` into `sl` at `0xc03fc`
(`ldr.w sl,[sp,#56]`) and keeps it live to the epilogue, while the
original leaves `this` on the stack and reloads it late (`0xc0b5e`).
`fixed`/`volumes`/`ship` declaration-order and aliasing variants all
normalise back to our allocation; a source-level lever for this swap was
not found this wave.

The `0x100..0x13f` bucket (26 bytes) is a single scheduling difference:
`str.w r9,[sp,#56]` (the `this` spill) sits at `0xc031c` in ours and at
`0xc0332` in the original.  The `0x0000..0x007f` buckets (5 bytes) are
spill-slot assignments for the waypoint/jitter locals.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12117
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

Scratch `/home/debian/GalaxyOnFire2/scratch/w12117/`: `Level_base.cpp`
(wave-176 base), `v_12304.cpp` (landed), `h.py` (build + raw/aligned/size/
extab harness with a source-hash cache), `e3.py`/`e5.py`/`e14.py` (guard
subset and tail-permutation sweeps), `sbs.py` (aligned side-by-side
disassembly diff), and the `*.s` dumps.
