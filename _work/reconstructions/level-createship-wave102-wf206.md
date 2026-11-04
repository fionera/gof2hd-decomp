# Level::createShip — wave 102 (wf206): land the semantically-correct spawn

**Status:** source change landed. Target
`_ZN5Level10createShipEiiiP8Waypointbb` (original 2764 B @ `0xc0214`,
Thumb entry `0xc0215`, 1007 insns). The committed spawn was
semantically wrong (it dropped the waypoint-`x` term from `x` and added
`wp->x` to `wp->z`); this pass replaces it with the correct
`wp->x + jitterY - 20000 / wp->y + jitterZ - 20000 / wp->z + jitterX - 20000`
values. The official verifier score dips **74.4 -> 73.8** but the
function stays size-exact (2764 B) and every section size is unchanged.

Gate before and after (full `check`):

```text
before: byte 4500/4524  linked 4500/4524  size 4524/4524  data 843/843
        imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
        .ARM.extab 0x2628c  lint CLEAN  drift 0  RATCHET PASS
after:  byte 4500/4524  linked 4500/4524  size 4524/4524  data 843/843
        imports 207  .text 0x1337c8  .rodata 0x41b67  .ARM.exidx 0x6f70
        .ARM.extab 0x2628c  lint CLEAN  drift 0  RATCHET PASS
```

Only the function's own fuzzy score changes (74.4 -> 73.8); the ratchet
counts are unchanged.

## 1. The landed diff

```diff
     int armor = Globals::status->hardCoreMode() ? 650 : 1000;
     Player *player = new Player(armor, hitpoints, 1, 1, 0);
-    int x = jitterZ + jitterX - 20000;
-    float y = (float) (wy + jitterY - 20000);
-    float z = (float) (wz + wx - 20000);
+    int x = wz + jitterY - 20000;
+    float z = (float) (wx + jitterX - 20000);
+    float y = (float) (wy + jitterZ - 20000);
     try {
         player->setEmpData(empPoints, empTime);
     } catch (...) {
```

(`wz = waypoint->x`, `wy = waypoint->y`, `wx = waypoint->z` in this
function's local naming, so this is `x = wp->x + j1`, `y = wp->y + j2`,
`z = wp->z + j3`, each `- 20000`.)  The statements are written `x, z, y`
because that order scores 73.8 vs 73.6 for the natural `x, y, z` order;
`xzy`, `zxy` and `zyx` compile to *identical* machine code (verified by
byte-comparing the `.text` bodies). The `try`/`catch` anchor, the
`::operator new` + placement `new` pair and the rest of the body are
unchanged from the committed source.

The original's spawn is **all-`int`** (`add`/`sub` on r-registers, with
`vcvt.f32.s32` after each branch's `operator new`); the landed form
converts `y`/`z` to `float` at the definition point. The values are
identical, the types are not. The all-`int` form was measured but cannot
be landed without a gate regression (see §3).

## 2. Why the all-`int` form scores much lower

The original is plain `new` everywhere (`level-createship-wave101-wf104.md`
decodes its LSDA: 57 cleanup-only call-site entries, no catch handler).
A faithful plain-`new`, all-`int` reconstruction compiles with the same
57-entry LSDA shape, but the SelectionDAG list scheduler sinks the three
spawn `add`s **below** the `setEmpData` call (there is no `EH_LABEL`
barrier), which changes the live ranges across the call and therefore the
global Greedy-RA colouring — including the function prologue.

Original (and committed / landed) prologue:

```text
str  r1, [sp, #60]   ; race
mov  r9, r0          ; this
mov  fp, r3          ; type
mov  r5, r2          ; shipClass
```

All-`int` plain-`new` prologue:

```text
str  r3, [sp, #60]   ; type
mov  r9, r0          ; this
str  r1, [sp, #56]   ; race
mov  sl, r2          ; shipClass
```

The all-`int` build also loses the `cbz r0` / `cmp.w fp,#51` encodings
(`type` ends up in a low register), so its `.text` grows.

## 3. Measured table (official verifier, full `link_so` + `verify.py`)

All rows are `_ZN5Level10createShipEiiiP8Waypointbb`; `pct` is the
official (1-decimal) ratio, `usize` the body size.

| source shape | pct | usize | .text | .ARM.extab |
|---|---:|---:|---|---|
| committed (wrong spawn, `try` + placement) | **74.4** | **2764** | 0x1337c8 | 0x2628c |
| **landed** (correct mixed spawn `xzy`, `try` + placement) | **73.8** | **2764** | 0x1337c8 | 0x2628c |
| correct mixed spawn `xyz`, `try` + placement | 73.6 | 2764 | 0x1337c8 | 0x2628c |
| all-`int` correct, plain `new`, spawn after ctor | 63.1 | 2788 | 0x1337e8 | 0x26294 |
| all-`int` correct, plain `new`, spawn before ctor | 64.2 | 2796 | — | — |
| all-`int` correct, plain `new`, spawn before hitpoints | 64.6 | 2796 | — | — |
| all-`int` correct, plain `new`, spawn before `int level` | 64.0 | 2796 | — | — |
| all-`int` correct, `try` + placement | 61.6 | 2752 | — | — |
| all-`int` correct, placement `new`, no `try` | 61.7 | 2748 | — | — |
| all-`float` correct, `try` + placement | 65.0 | 2748 | — | — |
| all-`float` correct, plain `new` | 58.4 | 2776 | — | — |
| mixed correct, plain `new` + if-chain | 66.0 | 2764 | — | — |

Only the mixed (`int x`, `float y`, `float z`) forms land at exactly
2764 B.  Statement order matters by one decimal: `xzy`/`zxy`/`zyx` give
73.8, `xyz`/`yxz`/`yzx` give 73.6 (identical code for the first three). The all-`int` forms are 2748/2752 (placement) or 2788/2796
(plain `new`); none is 2764, so landing all-`int` would drop
`size-exact` to 4523/4524 and move `.text` to 0x1337e8 (+0x20).

The all-`int` spawn order is right (`ctor -> spawn -> setEmpData`) when
the spawn is written **before** `new Player` (or before the `hitpoints`
computation), but the prologue is still the wrong one in every one of
the 216 waypoint-decl x jitter-decl x spawn-order permutations tried.
The entry allocation (race->`[sp,#60]`, type->`fp`, shipClass->`r5`,
this->`r9`) and the all-`int` spawn's integer-pressure profile cannot be
reconciled by source shape with this compiler.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/wf206
cmake --build cmake-build-match --target check -j8
python3 tools/verify/verify.py \
  --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level10createShipEiiiP8Waypointbb
readelf -SW cmake-build-match/libgof2hdaa.so | grep -E '\.text|\.ARM.extab'
```

Scratch harness (under `/dev/shm/wf206`): `gen.py` (spawn/new/try/position
variant generator), `sweepA..F.py` (measured sweeps), `order.py`
(ctor/spawn/setEmpData order), `sizediff.py` (instruction-width diff),
`score.py` (fast single-function score).

## 5. Conclusion / next lever

The landed mixed form is the best *semantically correct* shape that keeps
every gate number intact. The true original source is all-`int` plain
`new`; reproducing it needs the list scheduler to keep the spawn adds
before `setEmpData` without an `EH_LABEL`, which in turn needs the
second-half live ranges to match the original. The remaining residual is
the same global RA colouring described in `level-createship-wave101-wf104.md`
(`volumes = fp, z = r4, x = sl, y = r8, this = r6, ship = r9`).
