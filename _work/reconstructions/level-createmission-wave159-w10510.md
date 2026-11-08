# w10510 (wave 159): `Level::createMission` — alien-loop `this=r4` via a
# hoisted loop-bound local (`const int end`); raw 5,487 -> 6,381,
# aligned 774 -> 1,341, size 17,172 B

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10510`, based on master
`cfde1f5f2` (wave 158). Continues
`level-createmission-wave158-w10410.md`, `…-wave156-w10310.md`,
`…-wave154-w10012.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10510/fdiff.py`; instruction-index alignment is
`/dev/shm/w10510/align_diff.py`; section diff is `/dev/shm/w10510/secdiff.py`
(worktree-local copies, `OURS` pointed at this worktree's
`cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,372   .ARM.extab 1,284   .rel.plt 0 (byte-identical)
```

## 1. Landed change (`src/game/world/Level.cpp`, alien-gun block)

The alien block was reordered to the wave-158 `KVBR` shape (`k, v, base,
rotation`) and a `const int end` local was hoisted out of the `for` condition:

```diff
     if (alienGuns != 0) {
-        Matrix rotation;
-        Vector v = {0.0f, 0.0f, 0.0f};
         int k = 0;
+        Vector v = {0.0f, 0.0f, 0.0f};
         Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
+        Matrix rotation;
+        const int end = this->friendCount + alienGuns + this->field_164 + this->field_168;
         for (int i = this->friendCount + this->field_164 + this->field_168;
-             i < this->friendCount + alienGuns + this->field_164 + this->field_168; i++, k++) {
+             i < end; i++, k++) {
```

This is the **first size-exact (17,172 B) source shape found that keeps
`this` in `r4` through the alien loop**.  The loop body starts with
`ldr r4,[sp,#124]` / `mov r0,r4` (ours `0xb3298`/`0xb32a2`), matching the
original's `mov r0,r4` at `0xb329a`.

## 2. Register map (alien loop)

```text
        this        i     k*24  t     v      rotation  p2    gun   temp  bound
ORIG    r4(reload)  fp    r9    sl    r5     r4        r4    r6    r8    r3 (recomputed)
CEND    r4(reload)  r9    r6    r5    sl     fp        fp    r4    r8    sp+104
KVBR    fp          r4*   r6    r5    r9     sl        sl    r4    r8    r3 (recomputed)
BASE    fp          r4*   r6    r5    r9     sl        sl    r4    r8    r3 (recomputed)
```
`*` = spilled to `sp+116` inside the body.

Key: hoisting the loop bound into a stack local (`end`, `sp+104`) removes the
tail's need for `this`; the body's `this` reload then becomes a short interval
and greedy assigns it `r4` (the reload has an `r4` hint from the preheader
reload).  The loop-carried `i` then takes `r9`, and `k*24` takes `r6`.  This is
the **opposite** of the original, where `i=fp` and `this` is rematerialised at
the tail into `r4`.

## 3. Variants measured this wave (all 17,172 B unless noted)

Baseline for all rows is pristine HEAD (`5487 / 774`).  `this/i` is the
register map of the alien loop.

| variant | raw | align | size | this/i |
|---|---:|---:|---:|---|
| **HEAD (RVKB)** | **5487** | **774** | 17172 | fp / r4(spill) |
| KVBR | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `Level *volatile self` in body | 6539 | 893 | 17196 | — |
| KVBR + `Level *self` in body | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `Level &self = *this` (block) | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `Array<KIPlayer*> *enemies` in body | 6389 | 796 | 17160 | fp / r4(spill) |
| KVBR + `Array<KIPlayer*> *enemies` before loop (body only) | 15655 | 2560 | 17224 | sl / ? |
| KVBR + `Array<KIPlayer*> *enemies` before loop (base+body) | 6378 | 1229 | 17172 | sl / ? |
| KVBR + `KIPlayer *baseShip` | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `const int end` **(landed)** | **6381** | **1341** | 17172 | **r4 / r9** |
| KVBR + `int end` | 6381 | 1341 | 17172 | r4 / r9 |
| KVBR + `const int start; i<start+alienGuns` | 6506 | 1414 | 17172 | r4 / r9 |
| KVBR + `const int start; const int end` | 6506 | 1414 | 17172 | r4 / r9 |
| KVBR + `int end=alienGuns` (full bound) | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `int end=alienGuns+field_164` | 6655 | 1308 | 17220 | r5 / ? |
| RVBK + `const int end` | 6587 | 1414 | 17164 | r4 / ? |
| KVBR + `end` before rotation | 6381 | 1341 | 17172 | r4 / r9 |
| KVBR + `t[i-start]`, `const int start` | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `(i<start)` hack + start/end | 6653 | 1427 | 17200 | sl / ? |
| KVBR + `while` loop | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `do/while` + pre-check | 12596 | 1298 | 17160 | sl / ? |
| KVBR + `do/while` (no pre-check) | 13490 | 1382 | 17184 | r9 / ? |
| KVBR + `int guns=alienGuns+fortressGuns` hoisted | 7451 | 1811 | 17176 | fp / ? |
| KVBR + volatile `Status *status` hoisted | 15999 | 3239 | 17224 | fp / ? |
| KVBR + `storesafter` (field_0x70/cargo after 2nd rotate) | 5643 | 736 | 17172 | fp / r4(spill) |
| KVBR + `Vector pos` local | 5485 | 795 | 17172 | fp / r4(spill) |
| KVBR + `setScaling` before `setPosition` | 5486 | 799 | 17172 | fp / r4(spill) |
| KVBR + `shipGroup` before `setPosition` | 5489 | 799 | 17172 | fp / r4(spill) |
| KVBR + `setPosition(...,p2)` (no `v=p2`) | 5485 | 798 | 17172 | fp / r4(spill) |
| KVBR + second rotate inline (no `v=p2`) | 6583 | 1251 | 17168 | sl / ? |
| KVBR + `setScaling(6.0f+(i<0))` | 6484 | 898 | 17188 | fp / ? |
| KVBR + `shipGroup=localRace+(i<0)` | 13929 | 984 | 17180 | fp / ? |

## 4. Why the landed shape is a regression

The original recomputes the loop bound from `this->friendCount/field_164/
field_168` at the tail (`ldmia`), so `this` is live at the tail and is
rematerialised into `r4` there.  The landed shape hoists the bound into a
stack local (`sp+104`), which removes the tail `this` use and lets `this`
take `r4` for the body head; but it also changes the loop condition from a
recomputed `ldmia` to a single `ldr` of `sp+104`, and shifts `i`/`k` into
`r9`/`r6`.  The result is a large raw/aligned regression even though the
`this=r4` target is reached.

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 6381   (+894)
                           align  774 -> 1341   (+567)
.text size             0x1337c8 -> 0x1337c8 (exact)
.text diff             85,372 -> 86,266  (+894)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.ARM.extab diff         1,284 -> 1,297   (+13)
.rel.plt                    0 -> 0   (byte-identical)
.rodata/.data/.bss/.ARM.exidx 0 -> 0
.note.gnu.build-id         20 -> 20
file size             2,187,328 -> 2,187,328 (exact)
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

`check` is green.

## 6. Open questions

1. The shape that would match the original is: `this` rematerialised at the
   loop tail into `r4`, `i` in `fp`, `k*24` in `r9`, `t` in `sl`, `v` in `r5`,
   `rotation`/`p2` sharing `r4`.  The landed `end` local gets `this=r4` but
   moves `i` to `r9` and the bound to a stack slot.  A shape that keeps the
   recomputed `ldmia` bound *and* puts the body `this` reload in `r4` was not
   found.
2. The `end` local's stack slot is `sp+104`; the original's `this` spill is
   `sp+124` and the tail reload is `ldr r4,[sp,#124]`.  If the bound local
   could be rematerialised (recomputed) instead of stored, the tail would
   match; `const int` did not trigger rematerialisation in clang 7.0.2 `-Oz`.
3. `storesafter` (field_0x70/cargo after the second rotate) is the best
   aligned variant at size 17,172 (`5643 / 736`, aligned -38) but still keeps
   `this=fp`; it is a candidate if the coordinator prefers the aligned metric
   over the `this=r4` shape.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10510/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10510/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10510/secdiff.py
python3 /dev/shm/w10510/regmap.py
```

Scratch: `/dev/shm/w10510/{fdiff.py,align_diff.py,secdiff.py,regmap.py,
runvar.py,runvar2.py,alien_block.txt,v_*.txt,Level.mir,pmi.log}`.
