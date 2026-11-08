# w9911 (wave 153): `Level::createMission` — temp=r5 shapes measured; the
# 17,172-B p+p2 shape shifts the gold `.text` pad and is not landable

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9911`, based on master
`4a6104a43` (wave 152). Continues `level-createmission-wave152-w9812.md`,
`…-wave151-w9811.md`, `…-wave151-w9711.md`, `…-wave148-w9508.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w9911/fdiff.py`; instruction-index alignment is
`/dev/shm/w9911/align_diff.py`; region diff is `/dev/shm/w9911/regions.py`;
temp-register probe is `/dev/shm/w9911/temreg.py`.

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5484   align 776
alien loop: temp=r9/r9  this=fp  gun=r4  i=[sp,#116]  k=r6  t=r5  rot/v=sl
target:     temp=r5/r5  this=r4  gun=r6  i=fp         k=r9  t=sl  rot/v=r4
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text size 0x1337c8 (matches original)
```

## 1. Every 17,172-B shape with `temp=r5` (first sret) is a raw regression

The only way to force the first `MatrixRotateVector` sret into r5 is to name
the *first* rotate (w9711's `p`+`p2`).  On the wave-152 tree (with w9812's
volatile status read) the measured variants are:

| alien-loop shape | size | raw | align | temp (1st/2nd) |
|---|---:|---:|---:|---|
| **HEAD p2-only (landed)** | **17172** | **5484** | **776** | r9 / r9 |
| `p`+`p2`, early `cargo=nullptr` only | 17172 | 6449 | 1212 | **r5 / r8** |
| `p`+`p2`, `field_0x70=0` only | 17176 | 6305 | 1218 | **r5 / r8** |
| `p`+`p2`, no probe | 17164 | 6460 | 1198 | **r5 / r8** |
| `p`+`p2`, both probes | 17196 | 6593 | 1233 | fp / fp |
| `p`+`p2` + `vMkb` order | 17172 | 6451 | 1225 | **r5 / r8** |

The two named temps always split the sret slot (`sp+212` then `sp+188`), so
the second rotate is r8, not the original's single `sp+212` r5 slot.  The
region diff collapses into one 5,349-byte run (`0xb2e04..0xb42e8`), i.e. the
whole tail of the function shifts.

## 2. The gold pad makes the 17,172-B p+p2 shape unlandable

`p`+`p2` + early cargo is exactly 17,172 B, but the `.text` section size grows
from `0x1337c8` to `0x1337e8` (+0x20): gold's stub-table auto-padding
(`min(delta*0.02, 0x40)`) reacts to the changed stub delta.  The 32-byte pad
shifts the functions after it, so `check` reports **byte_exact 4507 -> 2360**
(linked_exact stays 4507).  `RATCHET FAIL`.  The shape needs the wave-152
`tools/patch_gold_pad.py` cap lowered for this link (not wired into this
tree's `CMakeLists.txt`), so it was not landed.

## 3. Shapes that keep `.text 0x1337c8`

| variant | size | raw | align | temp | note |
|---|---:|---:|---:|---|---|
| **`v` before `Matrix rotation` (landed)** | **17172** | **5482** | **794** | r9/r9 | raw -2, align +18 |
| `v M k b`, `k` before `M` | 17172 | 5482 | 794 | r9/r9 | same code |
| `v k M b` | 17172 | 5482 | 794 | r9/r9 | same code |
| `M b v k` | 17156 | 6522 | 1251 | r9/r9 | -16 B |
| `Vector&` alias of `v` / of `base` / of `p2` | 17172 | 5484 | 776 | r9/r9 | exact no-op |
| `const Matrix&` alias of `rotation` | 17172 | 5484 | 776 | r9/r9 | exact no-op |
| `Level *self = this` | 17172 | 5484 | 776 | r9/r9 | clang canonicalises |
| `MatrixRotateVector` static helper (return) | 17172 | 5484 | 776 | r9/r9 | inlined, identical |
| out-param `rotateInto(Vector&,…)` helper | 17164 | 6522 | 1232 | r9/r8 | +copy |
| nested-scope temp (both rotates) | 17168 | 6586 | 1208 | r8/r8 | slots merge, r8 |
| nested-scope temp, no probes | 17172 | 5649 | 711 | r6/r6 | best align, raw +165 |
| `__builtin_expect(alienGuns != 0, 1)` | 17172 | 5683 | 755 | fp/fp | align -21, raw +199 |
| `__builtin_expect` on the table select | 17172 | 5484 | 776 | r9/r9 | no-op |

`p2` declared before `p` (default-ctor or `= v` copy-init) loses copy elision
and grows to 17,332 B; declaring `p2` outside the loop gives 17,324 B.  The
`MatrixRotateVector` helper is fully inlined and byte-identical to the
baseline.

## 4. Landed change (`src/game/world/Level.cpp`, alien-gun block)

```diff
     if (alienGuns != 0) {
-        Matrix rotation;
         Vector v = {0.0f, 0.0f, 0.0f};
+        Matrix rotation;
         int k = 0;
```

This is the original's declaration order (`v` is zero-initialised before
`rotation` is built; the original's `strd r0,r0,[sp,#372]` precedes the
`vst1` identity-matrix stores).  It is a pure source-order change.

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5484 -> 5482   (-2)
                           align  776 -> 794    (+18)
.text diff             87,131 -> 87,129  (-2)
.text size             0x1337c8 -> 0x1337c8 (unchanged)
.ARM.extab              1,324 -> 1,324
.rel.plt                    0 -> 0   (byte-identical)
.rodata/.data/.bss      exact -> exact
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
```

## 6. Open questions

1. `temp=r5` for *both* rotates needs one named temp whose sret is forwarded
   (copy elision) and stack-coloured across both calls.  `p+p2` splits the
   slot; a reused named temp loses elision on the second assignment.  A
   copy-init in two nested scopes merges the slot but lands in r8 (with the
   probes) or r6 (without).
2. The `p+p2` 17,172-B shape is only landable once the gold pad cap is
   wired into this tree (`tools/patch_gold_pad.py`, wave 152); with the
   default 0x40 cap it grows `.text` by 0x20 and drops byte_exact to 2360.
3. `this` is reloaded into fp at the loop head because its live interval is
   split at the `getPosition` call; the original keeps it in r4 and reloads
   at the loop tail.  No source alias/caching shape moved it.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9911/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9911/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9911/regions.py
python3 /dev/shm/w9911/temreg.py
```

Scratch: `/dev/shm/w9911/{fdiff.py,align_diff.py,regions.py,temreg.py,dump.py,
sbs.py,harness.py,run.py,batch*.py,Level.base.cpp,mi_base.txt}`.
