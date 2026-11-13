# NOTE (coordinator, 2026-11-13 18:02 UTC): the code change below is NOT landed on master.
# It improves createShip (raw 1,190->1,154, aligned 366->339) but shrinks `.ARM.extab`
# 156,312 -> 156,308, which shifts every later record: raw extab contentdiff 597 -> 122,320,
# `.ARM.exidx` 0 -> 1,307, `.data` 0 -> 1.  Net loss until a +4 B extab compensation is found.

# `Level::createShip` — wave 192 (w13884): drop the invented campaign-mission guard

Worktree `wt/w13884`, branch `wave192-w13884`, base master `51b1cd1fe`.
Target `_ZN5Level10createShipEiiiP8Waypointbb` (`src/game/world/Level.cpp`,
2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2, `-Oz`).  Scratch `scratch/w13884`.

This is the second independent `createShip` agent in wave 192; the first
(w13871) landed the `setBV`/`setShipGroup` tail order.  The change below was
previously landed on `wave191b-w13788` (commit `6c6b77344`, not in this
branch's history) and is re-applied here on the current base.

**Status: landed.**  Raw **1,190 -> 1,154** (-36), aligned **366 -> 339**
(-27), size **2,764/2,764** exact, insns 1,117/1,117.  `.text`
76,845 -> 76,809.  `.ARM.extab` **156,312 -> 156,308** (-4; the createShip
record shrinks 336 -> 332 B, so every later record shifts down 4 B and
`.ARM.exidx` contentdiff goes 0 -> 1,307 as its prel31 targets follow).
`.data` gains one 4-byte content diff: the relocated `{extab_start,
extab_size}` word at `.data+0x1e4c` (0x26298 -> 0x26294) follows the shrink.
`.rodata`, `.rel.plt`, `.got` stay byte-identical.  Gate: RATCHET PASS,
byte_exact 4508/4524, linked 4508, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

## 0. Baseline re-verified

```
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb   # raw=1190 aligned=366 size=2764/2764
# .text contentdiff 76,845; .ARM.extab sh_size 156,312; .ARM.exidx contentdiff 0
```

## 1. The landed change

Only the campaign-mission read changes:

```cpp
-    int mission;
-    {
-        EmpGuard guard;
-        mission = Globals::status->getCurrentCampaignMission();
-    }
+    int mission = Globals::status->getCurrentCampaignMission();
```

`EmpGuard` is the local `struct EmpGuard { ~EmpGuard() {} };` used elsewhere in
the function; the original has **no** cleanup region for this call.  The win is
entirely in the `.text` cleanup/landing-pad block at `0x97c..0x9c7`: removing
the guard deletes one landing pad and one unwind row.

## 2. LSDA evidence (why the guard is invented)

`lsda_dec.py` on the original vs the base build vs this change:

| | original | base | landed (M) |
|---|---|---|---|
| rows | 57 | 57 | 56 |
| clen | 315 | 315 | 310 |
| row0 | `0x000/0x196 lp=0` | `0x01c/0x004 lp=0x9c0` | `0x000/0x196 lp=0` |
| row1 | `0x196/0x01e lp=0x9b8` | `0x020/0x176 lp=0` | `0x196/0x01e lp=0x9b8` |
| row2 | `0x1b4/0x05e lp=0` | `0x196/0x01e lp=0x9b6` | `0x1c8/0x00a lp=0x9b4` |

The original's first row already spans `0x0..0x196` with no landing pad, so
`getCurrentCampaignMission` cannot be inside a try scope.  The base build's
`0x01c/0x004 lp=0x9c0` row is exactly the invented `EmpGuard` destructor.  With
the guard removed, rows 0 and 1 match the original byte-for-byte.  The
original's landing pads exist only for `new` expressions (Player,
PlayerFixedObject, Array, the BoundingAAB blocks, PlayerFighter); there is no
row for `getCurrentCampaignMission`, `setEmpData` or `getShipGroup`.

### `.ARM.extab` record anatomy

Original createShip exidx entry: index 1635, `w2=0x0000b178`; extab record at
`0x1ab09c` (section offset 29988), next record at `0x1ab1ec` -> **336 B**.
After M: same exidx entry, same record start, next record at `0x1ab1e8` ->
**332 B**.  So the section shrinks 4 B (`0x26298` -> `0x26294`) and every later
record shifts down 4 B; the naive `.ARM.extab` contentdiff therefore jumps to
~122,320 even though the record itself is 4 B *smaller*.  The relocated
`{extab_start, extab_size}` data word at `.data+0x1e4c` follows the shrink.

## 3. Variants measured on this base (all keep function size 2,764 unless noted)

| variant | raw | aligned | notes |
|---|---:|---:|---|
| base | 1190 | 366 | |
| **M (landed)** | **1154** | **339** | one guard removed; extab -4 |
| S (drop `setEmpData` guard) | 2188 | 471 | size 2780 |
| G (drop `getShipGroup` guard) | 1185 | 388 | worse |
| MS | 2188 | 489 | size 2780 |
| MG | 1412 | 435 | size 2760 |
| SG | 2187 | 443 | size 2780 |
| MSG | 2187 | 380 | size 2776 |

Only **M** is a win.  Other probes from this session (all neutral or worse,
reverted): `C1_threeifs` (three separate tail `if`s, 1190/366 neutral),
`C2_switch` (switch on type, 1190/366 neutral), `I1`/`I2` (int `x/y/z` before
`setEmpData`, floats in branch: 2292/479, size 2752), `I3` (ints + floats after
`operator new`: 2109/511, size 2752).  The w13871 dead end `bb998763a` (drop
the `float fx` local, pass int `x` to the ctor/AABs) is worse on both metrics
(raw 1,195 / aligned 367) and was not re-applied.

## 4. Remaining divergences

1. **Spawn block.**  The original computes three ints
   (`c03c8..c03e4`: `x = wpx+j3-20000`, `y = wpy+j1-20000`,
   `z = wpz+j2-20000`, one `movw r1,#20000`, `ldr` not `ldrd`) and then
   converts.  Our source uses `float z = (float)(wpx+wpy-20000)`,
   `int x = wpz+jitter3-20000`,
   `float y = (float)(jitter1+jitter2+jitter3-20000)`.  The faithful all-int
   form shrinks the function to 2,748 B, so it needs a compensating +16 B
   elsewhere.
2. **`this`/`missionPtr` spill.**  The original spills `this` to `sp+56` after
   the empPoints clamp (`c0332`) and reloads it into `r6` at `c03fe`; ours
   spills inside the clamp and reloads into `sl`.
3. **Tail type dispatch.**  The original is three separate compares
   (`c073c cmp r4,#44 / beq`, `c0740 ldr [sp,#52] / cmp #51 / beq`,
   `cmp #49 / bne`) with a shared cargo-delete path; ours emits an IT-block.
4. **`setWreckedMeshId(0x4780)` shared tail.**  The original's race-0 type-14
   block branches to a shared `movw r1,#0x4780` at `c0b2c`; ours inlines a
   second call site.  Hoisting it previously grew the function to 2,776 B.
5. **Remaining invented guards.**  `setEmpData` and `getShipGroup` are still
   wrapped in `EmpGuard`; removing either alone breaks the size (2,780 /
   2,760 B) because it reshuffles the whole frame and register allocation.
   The original LSDA has no row for either.
