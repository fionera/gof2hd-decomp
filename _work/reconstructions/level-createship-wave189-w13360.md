# `Level::createShip` — wave 189 (w13360): fixed-branch float local pass

Worktree `wt/w13360`, branch `wave189-w13360`, base master `26fbed661`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13360`.

**Status: landed a size-exact raw win.**  Base was raw **1,202**,
aligned 484, size **2,764/2,764**, insns 1,116/1,117.  The landed
variant is raw **1,197** (-5), aligned **483**, size **2,764/2,764**,
insns 1,116/1,117; `.text` diff **73,565 -> 73,560** (-5);
`.ARM.extab` diff stays **610** (size- and base-exact 156,312 B @
`0x1a3b78`); `.ARM.exidx` 0; `.rel.plt` byte-identical; `.rodata` 0.

## 0. Bottom line

The remaining `createShip` bytes are dominated by register allocation,
not source order.  The w13303/w13325 coordinate-type/order sweeps and
the w13304 RA map all left the base at raw 1,202.  This pass took a
different lens: the `shipClass == 1` branch's `BoundingAAB` argument
staging.  Introducing a single explicit float local for the integer `x`
at the head of that branch

```cpp
} else if (shipClass == 1) {
    float fx = x;
    ...
    new (fixed) PlayerFixedObject(type, race, player, nullptr, fx, y, z);
    ...
    (*volumes)[0] = new BoundingAAB(fx, y, z, ...);
```

lets clang keep the converted value as the branch's float `x` instead of
re-materialising `vmov s0, r4; vcvt.f32.s32 s20, s0` inside the
`PlayerFixedObject` constructor staging.  The whole function stays
2,764 B; the change only re-colours the ctor argument block
(`0xc0404..0xc0442`), removing 5 differing bytes there and leaving every
other region unchanged.

`y` and `z` are already `float` in the base source, so no local is
needed for them (`float fx` alone is sufficient; adding `fy`/`fz` is
codegen-neutral).

## 1. Measurements

| variant | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| base | 1,202 | 484 | 2,764 | 1,116/1,117 |
| `float fx = x;` at branch head, used in ctor + all AAB calls | **1,197** | **483** | **2,764** | 1,116/1,117 |
| same, used only in ctor | 1,197 | 483 | 2,764 | 1,116/1,117 |
| same, used only in AAB calls | 1,197 | 483 | 2,764 | 1,116/1,117 |
| `float fy` / `float fz` added too | 1,197 | 483 | 2,764 | 1,116/1,117 |
| declaration after `::operator new` | 1,202 | 484 | 2,764 | 1,116/1,117 |
| declaration after the `Array` new | 1,202 | 484 | 2,764 | 1,116/1,117 |
| `float fx = x;` hoisted above the `shipClass` branch | 2,514 | 996 | 2,740 | 1,116/1,117 |

Only the `fx` local matters; `fy`/`fz` are neutral.  Hoisting the
conversion above the branch (common to the fighter path) changes the
whole allocation and loses 24 B of size, so the local stays inside the
fixed branch.

## 2. Section state

* `.text` differing bytes **73,565 -> 73,560** (size exact 1,259,464 B).
* `.ARM.extab` differing bytes **610** (unchanged), size- and base-exact
  **156,312 B @ `0x1a3b78`**; `.ARM.exidx` **0**.
* `.rel.plt` **0** (byte-identical, 29,392 B); `.rodata` **0**;
  `.data` **0**; imports **207**; parity **5,370/5,370**.
* `check` (ratchet) **PASS**: byte_exact 4507, linked_exact 4507,
  size-exact 4524/4524, data 843/843, lint CLEAN, drift 0.

## 3. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13360
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
# -> raw=1197 aligned=483 size=2764/2764 insn=1116/1117
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

## 4. Files changed

* `src/game/world/Level.cpp` — one `float fx = x;` plus the fixed-branch
  ctor/AAB uses.
* `docs/IDENTICALITY.md`, `docs/VALIDATION.md` — wave-189 numbers.
* this note.
