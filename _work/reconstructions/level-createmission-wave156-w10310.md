# w10310 (wave 156): `Level::createMission` — alien-loop `this=r4` sweep;
# landed the w10210 branch-weight + dead-early-cargo shape (raw 5,487 -> 5,662,
# aligned 774 -> 748, size 17,172)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10310`, based on master
`08575efff` (wave 156). Continues `level-createmission-wave155-w10210.md`,
`…-wave154-w10012.md`, `…-wave153-w9912.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10310/fdiff.py`; instruction-index alignment is
`/dev/shm/w10310/align_diff.py` (worktree-local copies of the shared tools).
Register maps were read from `objdump` with `/dev/shm/w10310/regmap.py`.

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,831   .ARM.extab 1,292   .rel.plt 0 (byte-identical)
```

## 1. Landed change (`src/game/world/Level.cpp`, alien-gun block)

This is the w10210 shape, which was in the wave-155 note but was **not** in
master HEAD (HEAD still had `if (alienGuns != 0)` and the early cargo store):

```diff
-    if (alienGuns != 0) {
+    if (__builtin_expect(alienGuns != 0, 1)) {  // lint: arm_builtin
         ...
             gun->geometry->setRotation(v);
             gun->field_0x70 = 0;
-            gun->cargo = nullptr;
             Vector p2 = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
```

The early `gun->cargo = nullptr` is dead (DSE'd in HEAD); the faithful tail
store is kept.  `// lint: arm_builtin` is a same-line waiver (the file already
has one `__builtin_memcpy`).

## 2. Target vs landed register map (alien loop)

```text
        this        i            gun   k*24   t     v/sret  rotation/p2  temp
ORIG    r4(reload)  fp           r6    r9     sl    r5      r4 (reused)  r8 (+sp+200)
OURS    r5          r4(sp+120)   r4    r6     r5    r9      sl           sp+200
```

The original keeps the loop-carried `i` in `fp`, rematerialises `this`
(`ldr r4,[sp,#124]` at the loop tail `b336e`) and reuses `r4` for `&rotation`
and `&p2`.  Ours keeps `this` in `r5`, spills `i` to `sp+120`, and gives the
address scratch `sl`.  The target `this=r4` was **not** reached.

## 3. New hypotheses measured this wave (all 17,172 B unless noted)

Baseline for all rows is pristine HEAD (`5487 / 774`).  `this/i` is the
register map of the alien loop.

| variant | raw | align | size | this/i |
|---|---:|---:|---:|---|
| **HEAD** | **5487** | **774** | 17172 | fp / r4(spill) |
| **landed: `expect(alienGuns!=0,1)` + drop early cargo** | **5662** | **748** | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` only | 5683 | 753 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + drop field_0x70 | 5676 | 819 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + drop cargo | 5662 | 748 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + KVBR | 5683 | 775 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + KVRB | 5681 | 771 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + `Status**` local | 5689 | 755 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + `int fg=fortressGuns` | 5683 | 753 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + `Level *self=this` | 5683 | 753 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + drop cargo + KVBR | 5662 | 770 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + drop cargo + KVRB | 5660 | 766 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + drop cargo + `Status**` | 5668 | 750 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + drop cargo + `fg` | 5662 | 748 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + drop cargo + base-first | 5663 | 819 | 17172 | r5 / r4(spill) |
| `expect(alienGuns!=0,1)` + drop cargo + `short k` | 6504 | 905 | 17188 | sl / r5 |
| `Status**` local only | 5495 | 777 | 17172 | fp / r4(spill) |
| `Status*&` local only | 5495 | 777 | 17172 | fp / r4(spill) |
| `int fg=fortressGuns` only | 5487 | 774 | 17172 | fp / r4(spill) |
| `Level *self=this` (block or loop) only | 5487 | 774 | 17172 | fp / r4(spill) |
| `enemies` local inside loop | 6390 | 775 | 17160 | — |
| `enemies` local before loop | 6377 | 1206 | 17172 | — |
| `i` declared before `base`, `for(;…)` | 11714 | 1569 | 17156 | — |
| precompute `start`/`end` | 6587 | 1414 | 17164 | — |
| precompute `friendCount`/`field_164`/`field_168` | 6587 | 1414 | 17164 | — |
| drop `field_0x70` only | 6249 | **713** | 17164 | fp / r4(spill) |
| drop early cargo only | 6453 | 788 | 17164 | fp / r4(spill) |
| drop both early stores | 5821 | 849 | 17180 | fp / r9 |
| drop both early stores + no `(i<0)` hack | 6578 | 1262 | 17132 | — |
| no `(i<0)` hack (`bigAlienWave`) | 6592 | 1243 | 17140 | r5 / sl |
| `expect(bigAlienWave,0)` instead of the hack | 6592 | 1243 | 17140 | r5 / sl |
| `expect(alienGuns!=0,0)` | 14801 | 1562 | 17196 | fp / r4(spill) |
| `short k` | 5811 | 866 | 17180 | sl / r4(spill) |
| `unsigned k` | 5487 | 774 | 17172 | fp / r4(spill) |
| `Vector v(0,0,0)` | 5487 | 774 | 17172 | fp / r4(spill) |
| `Vector v; v = Vector(0,0,0)` | 6488 | 870 | 17188 | fp / r4(spill) |

### 3a. All 24 declaration permutations of `{rotation, v, k, base}`

The four declarations were permuted exhaustively (hack present).  Two families:

- `Mvkb`, `Mvbk`, `Mkvb`, `kMvb`: `5487 / 774 / 17172`, `this=fp i=r4(spill)`.
- `vMkb`, `vMbk`, `vkMb`, `vkbM`, `vbMk`, `vbkM`, `kvMb`, `kvbM`,
  `kbvM`, `bvMk`, `bvkM`, `bkvM`: `5485 / 792–862 / 17172`,
  `this=fp i=r4(spill)`.
- `Mkbv`, `Mbvk`, `Mbkv`, `kMbv`, `kbMv`, `bMvk`, `bMkv`, `bkMv`:
  `6522–6524 / 1251–1262 / 17156`, `this=r5 i=sl`.

No permutation produced `this=r4` in the loop body.  `KVBR`/`KVRB` reproduce the
preheader hoist (`ldr r4,[sp,#124]` before the rotation init, matching the
original `b3232`) but the allocator still moves `this` to `fp`/`r5` for the loop.

## 4. Why `this` does not stay in r4

The original's loop is a tight colouring where `this`, `&rotation` and `&p2`
share `r4` (their live ranges do not overlap), `v` is `r5`, `gun` is `r6`,
`k*24` is `r9`, `t` is `sl`, `i` is `fp`, and `temp` is `r8`.  In every
17,172-B source shape the allocator instead keeps `this` (and often
`&rotation`/`&p2`) in registers and spills `i`.  The `(i < 0)` size hack is
what forces `i` to be live through the middle of the body; removing it frees
`i` but shrinks the function to 17,140 B and lets `&p2` take `fp`, so `i`
still does not reach `fp`.  The missing 32 B of genuine code (w9812/w9912
open question) is still unidentified.

New observation: with `expect(alienGuns!=0,1)` the allocator moves `this`
from `fp` to `r5` (one step toward the target), and the aligned metric drops
774 -> 748.  The `drop-field_0x70`-only shape reaches aligned **713** but is
8 B short (17,164) and much worse in raw (6,249); if the missing 8 B is found
it is the best aligned base.

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 5662   (+175)
                           align  774 -> 748    (-26)
.text diff             85,831 -> 86,006  (+175)
.ARM.extab              1,292 -> 1,292
.rel.plt                    0 -> 0   (byte-identical)
.rodata/.data/.bss/.exidx exact -> exact
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

All 27 section sizes exact; file size exact (2,187,328 B).

## 6. Open questions

1. `i` must reach `fp` and `this`/`&rotation`/`&p2` must share `r4`.  No
   source shape found this wave; the allocator's spill choice is the blocker.
2. The missing 32 B of genuine code (removing `(i<0)`) is still unidentified.
   `drop-field_0x70` is 8 B short at aligned 713; finding it would combine with
   the branch hint.
3. The original's table selection is a branch (`cbz`), ours if-converts to
   `it/movne`; forcing the branch would add ~6 B.
4. The original's first/second rotate loads all six `t[k]` floats once and
   reuses `t[k][1]`/`t[k][2]`; ours reloads with `ldmia`.  Dropping the stores
   between the two rotates makes clang hoist the loads (individual loads), but
   the size becomes 17,180.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10310/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10310/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10310/regmap.py
```

Scratch: `/dev/shm/w10310/{fdiff.py,align_diff.py,regmap.py,regions.py,
dump.sh,batch.py,perm.py,Level.base.cpp}`.
