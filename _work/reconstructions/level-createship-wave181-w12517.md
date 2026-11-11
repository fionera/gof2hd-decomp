# `Level::createShip` — wave 181 (w12517): guard-shape relocation, raw 1,214 -> 1,213

Worktree `wt/w12517`, branch `wave181-w12517`, base `master 1e270aa0e`
(wave 180 harvest).  Target `_ZN5Level10createShipEiiiP8Waypointbb`
(`src/game/world/Level.cpp`, 2,764 B, VMA `0xc0214`).  Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(NDK r18b clang 7.0.2 `-Oz`).

**Status: source change landed.**  Builds on w12417's `empPoints` clamp
polarity (1,221 -> 1,214).  Two `EmpGuard` placements change:

```cpp
// (a) PlayerFighter branch: getShipGroup is hoisted OUT of the guard,
//     the guard now wraps only setShipGroup.
AEGeometry *grp = Globals::globals->getShipGroup(type, race, group);
{
    EmpGuard guard;
    fighter->setShipGroup(grp, type, hostile);
}

// (b) tail: the setLevel guard is removed entirely.
ship->setLevel(this);              // was { EmpGuard guard; ship->setLevel(this); }
```

`createShip` raw **1,214 -> 1,213** (-1), aligned **396** (unchanged),
size **2,764/2,764** exact, insns **1,117/1,117** exact.  `.text`
differing bytes **76,200 -> 76,199** (-1).  `.ARM.extab` stays the original
**336 B / 57 rows / `ttype 0xff`** cleanup-only record, size- and
base-exact (`156,312 B` @ `0x1a3b78`), content diff **713 -> 659** (-54).
`.ARM.exidx` byte-identical; `.rel.plt` byte-identical (0); `.rodata` exact.
File size 2,187,328 exact, 27/27 section headers exact, build-id 20.
Total section-byte delta **76,933 -> 76,878** (-55).

Full `check`: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size-exact 4524/4524, stub 0, missing 0, wrong_type 0, extra 0,
data 843/843, imports 207, lint CLEAN, drift 0, symbol parity 5370/5370,
`RATCHET PASS`.

## 1. Result

| metric | before (wave 180) | after |
|---|---:|---:|
| raw same-address diff | **1,214** | **1,213** |
| instruction-index alignment | 396 | 396 |
| size | 2,764 | 2,764 |
| insns (orig/ours) | 1,117 / 1,117 | 1,117 / 1,117 |
| `.text` differing bytes | 76,200 | **76,199** |
| `.ARM.extab` size | 156,312 (exact) | 156,312 (exact) |
| `.ARM.extab` differing bytes | 713 | **659** |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.rel.plt` | byte-identical | byte-identical |

## 2. Why it helps

The two edits are LSDA-shape neutral at the section level (`.ARM.extab`
size stays exactly 156,312 and the record base stays `0x1a3b78`), but they
move two call-site ranges inside the existing cleanup-only record:

* (a) `getShipGroup` is a call that cannot throw in the original's view; in
  the base body it sat inside the `setShipGroup` guard's protected range, so
  the guard's landing-pad range started 4 bytes too early.  Hoisting the
  call out and guarding only `setShipGroup` aligns the range start.  This
  also frees the `grp` value in a callee-saved register across the guard
  setup, which is what removes the 1 raw byte.
* (b) the tail `setLevel` call has no cleanup in the original — the
  `EmpGuard` around it is a reconstruction artifact.  Dropping it removes a
  landing-pad row that the original does not have.

The `.ARM.extab` content diff drop (713 -> 659) is the direct evidence that
both ranges are now closer to the original's 57-row table; the size and base
are unchanged, so this cannot shift any other TU's `.rodata`/`.text`.

Equivalent 1,213 forms (all size 2,764, insn 1,117, extab 156,312, diff 659):

* `AEGeometry *grp = ...; { EmpGuard guard; fighter->setShipGroup(grp, ...); }`
  (landed)
* assign `grp` on its own line before the guard (same codegen)
* `const` getShipGroup result + `const_cast` at the call
* the landed form plus a trailing `(void)0;` in the guard block

## 3. Sweeps that did not beat 1,213

All measured on the wave-181 base (size 2,764 unless noted):

* **Guard removal alone** (`mission` guard, `rm=M`): raw **1,178**, but
  `.ARM.extab` **156,308** (4 B short of the original).  The shortfall is
  final-record padding in another TU (`fn 0x19ae8c`), so it is not a
  size-exact win.  Its LSDA row 0 becomes exactly the original's
  `(0,406,0,0)`, confirming the mission guard is spurious, but the extab
  size constraint keeps it out.
* `rm=L` (setLevel guard only): 1,209, extab 156,304.
* `rm=M add=delcargo`: 1,214, extab exact (713) — neutral.
* `v_L_gsgA` (guard around the `setShipGroup` *call*, not the callee):
  1,214, extab diff 658 (best extab content, no raw win).
* `v_MgsgA` (mission guard removed + gsgA): 1,214 but extab 156,320.
* `v_M_inalien2/3`: 1,221 (extab 156,320); `v_M2_*` (guards around wreck
  ids / ctors): 1,222-2,015 (extab 156,316/320).
* `v_L_g2/3`: 2,060 (size 2,760); `v_L_g4`: 1,404 (2,768); `v_L_g5`:
  1,408 (2,768); `v_Lg1_*` extra-guard sweep: all >= 1,408.
* `v_iii` (`int y, z`): 2,238 (size 2,752); `v_j123`: 1,217;
  `v_bnp_iii`: 2,238; `v_bnp_iff`: 2,469 (2,776).
* `v_conv_after_xint`: 2,050 (2,752); `v_E_iii`/`v_ME_iii`/`v_MEL_iii`/
  `v_EL_iii`: 2,224-2,240.
* Coordinate/jitter/tail statement-order permutations: 1,214-1,220.
* `switch (type)` case reorders: only `0x2c,0x33,0x31` and `0x2c,0x31,0x33`
  = 1,214; others 1,341-1,386.
* `-mllvm -disable-machine-sink` on Level.cpp: base 2,471 (2,744/1,092,
  extab 156,320); `v_iii` 2,052 (2,764, extab 156,320) — not a win.

## 4. Remaining mismatch structure

The residual is still dominated by the fixed (`shipClass == 1`) branch
`0xc03d4..0xc06d4` / `0xc0a54..0xc0b94` register swap (`fixed` r9 vs r6,
`volumes` fp vs r9, float bits `sl/r8/r4` vs `r6/r8/sl`) and the `this`
spill at `0xc031c` vs `0xc0332`.  The trigger is the coordinate block: the
original keeps `int x,y,z` in `fp/r4/r8` across `setEmpData` and converts
after, while our build converts y/z before the call and sinks x below it.
The all-int body is 2,752 B (12 B short); the size-exact int/float
combinations are only `int x, float y, float z` (1,214),
`float x, int y, float z` (1,230) and `float x, float y, int z` (1,233).

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w12517
ulimit -v 12000000
touch src/game/world/Level.cpp        # avoid ninja 1-second mtime race
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN5Level10createShipEiiiP8Waypointbb
cmake --build cmake-build-match --target check -j2
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w12517/`: `h.py` (splice +
forced recompile + raw/size/insn/extab harness), `fn_base.cpp` (wave-180
base body), `v_L_g1.cpp` (landed form), `v_L_g6/7/8.cpp` (equivalent
forms), `cdiff2.py`, `disrange.py`, `buckets.py`, `rowenc.py`, `lsda_fn.py`,
and the `v_*`/`gen_*`/`sweep_*` variant generators.

**Caveat for the next worker:** `h.py` writes `Level.cpp` and immediately
invokes ninja; if the previous build of `Level.cpp.obj` happened in the same
wall-clock second, ninja treats it as up to date and measures the *old*
`.so`.  Always `touch src/game/world/Level.cpp` (or sleep 1.1 s) between
splices, and confirm the `createShip` dynsym size in the `.so` before
trusting a measurement.
