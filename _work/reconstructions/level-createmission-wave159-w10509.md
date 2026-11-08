# w10509 (wave 159): `Level::createMission` — both-unnamed alien block
# (drop dead `field_0x70` + early `cargo`); raw 5,487 -> 5,652, aligned 774 -> 709

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10509`, based on master
`cfde1f5f2` (wave 158). Continues `level-createmission-wave158-w10410.md`,
`…-wave158-w10409.md`, `…-wave157-w10309.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10509/fdiff.py`; instruction-index alignment is
`/dev/shm/w10509/align_diff.py` (worktree-local copies with `OURS` pointed at this
worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5487   align 774
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,372   .ARM.extab 1,284   .rel.plt 0 (byte-identical)
```

## 1. Landed source (`src/game/world/Level.cpp`, alien-gun block)

The original alien loop has **one** `cargo` store (`str r0,[r6,#80]` at
`0xb3372`) and **no** `field_0x70` store. The prior source had a `field_0x70`
store, an early `cargo` store and a named `Vector p2`. The landed shape is the
faithful one — both rotates unnamed, no `field_0x70`, no early `cargo`:

```diff
             v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
             gun->geometry->setRotation(v);
-            gun->field_0x70 = 0;
-            gun->cargo = nullptr;
-            Vector p2 = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
-            v = p2;
+            v = MatrixRotateVector(rotation, Vector{t[k][0], t[k][1], t[k][2]});
```

This reproduces the original's single `sp+212` rotate-temp slot (both rotates
and the `setPosition` vector reuse it) and removes the extra `strb [r6,#112]`
and `strb [r6,#112]`/`str [r6,#80]` stores. It is the best aligned metric of
every size-exact shape measured (709 vs 774).

## 2. Register map (alien loop)

```text
        this        i            gun   k*24   t     temp(sret)  rotation     t[k][1]
ORIG    r4(reload)  fp           r6    r9     sl    r5(sp+212)  r4(reused)  r8
OURS    fp          r9(sp+116)   r4    r6     r5    r6(sp+212)  sl          r8
```

The landed shape matches the original's stack layout exactly: rotate temp at
`sp+212` (r6 during the call), `v` at `sp+372`, `base` at `sp+224`, rotate
input at `sp+200`. It still differs in the register assignment: the original
rematerialises `this` from `sp+124` into `r4` at the loop head and tail (freeing
`r4` for `&rotation`) and keeps `i` in `fp`; ours keeps `this` in `fp`, spills
`i` to `sp+116`, and holds `&rotation` in `sl`. This is a pure RA tie-break
between `this` (4 loop uses, rematerialisable) and `i` (4 loop uses,
loop-carried). No source shape tried flips it (see §3).

## 3. Variants measured this wave (all 17,172 B unless noted)

Baseline for all rows is pristine HEAD (`5487 / 774`). raw / align / size.

| variant | raw | align | size |
|---|---:|---:|---:|
| **HEAD** | **5487** | **774** | 17172 |
| **landed: both unnamed, drop field+early cargo (RVKB)** | **5652** | **709** | **17172** |
| same, KVBR decl order | 5650 | 730 | 17172 |
| same, KVRB / VRKB / VRBK decl order | 5650 | 727 | 17172 |
| same, VKBR / VBRK / VBKR / KBVR / BVRK / BVKR / BKVR | 5650 | 730–750 | 17172 |
| same, RKVB / KRVB / RVBK | 5652 | 709 | 17172 |
| same, RKBV / RBVK / RBKV / KRBV / KBRV / BRVK / BRKV / BKRV | 6534–6536 | 1213–1224 | 17148 |
| KVBR decl only (w10410) | 5485 | 795 | 17172 |
| KVRB decl only | 5485 | 792 | 17172 |
| VKBR decl only | 5485 | 795 | 17172 |
| KVBR + temp=r5 (`Vector p`) | 6453 | 1232 | 17172 |
| KVBR + temp=r5 + w10409 raid-decl companion | 6450 | 1218 | 17172 |
| temp=r5 + raid-decl (w10409) | 6446 | 1196 | 17172 |
| KVBR + `__builtin_expect(alienGuns!=0,1)` | 5683 | 775 | 17172 |
| both-unnamed + `__builtin_expect(alienGuns!=0,1)` | 5837 | 830 | 17180 |
| both-unnamed, separate `float a,b,c` locals | 5652 | 709 | 17172 (identical codegen) |
| both-unnamed, `const float *row = t[k]; row[n]` | 6418 | 964 | 17164 |
| both-unnamed, single reused `Vector p` for both rotates | 16593 | 3139 | 17304 |
| both-unnamed + `Level *self` / `Array *enemies` local | 15655 | 2560 | 17224 |
| KVBR + cold `__builtin_expect` on raid/pf/convoy/bp/sp/ea/nl | 11375–15980 | 894–3692 | — |
| KVBR + `__builtin_expect(mission==nullptr,0)` / inAlienOrbit / isEmpty / supernova / hasWanted | 12054–16631 | 909–5756 | — |
| remove `(bigAlienWave + (i<0))` hack (both shapes) | 6579–6594 | 1259–1288 | 17132–17140 |

## 4. The `(i < 0)` hack is load-bearing for size

Removing the `(i < 0)` hack gives the original's `cbz` table select but shrinks
the function to 17,140 B (base block) / 17,132 B (both-unnamed). The hack adds
32 B, exactly compensating the original's individual `ldr`/`str` vector
construction (ours emits `ldrd`/`strd`, ~10 B) plus other unidentified code.
Keep the hack until that 32 B is recovered.

## 5. Why raw and aligned move in opposite directions

The raw metric is dominated by shifts, not by the alien block itself:

* `0xb1418..0xb15e9` (1,500 B) — pre-existing literal-pool shift.
* `0xb2e04..0xb2fd1` (462 B) — the `if (i == X && alienWave)` /
  `else if (i == X && pirateFortress)` guards. The original if-converts both
  into branchless `(i != X) | !flag` (`subs`/`mov`/`it ne`/`movne`/`ldr`/`eor`/
  `orrs`/`bne`), 8 B and 4 B larger than our short-circuit `ldr`/`cmp`/`beq`/
  `cmp`/`bne`. This is a sample-PGO branch-weight signature; no source form
  (`&&`, `||`, `!`, `&`, `*`, `__builtin_expect`, `__builtin_expect_with_probability`
  is not available in clang 7) reproduces it at `-Oz`.
* `0xb2fdc..0xb35xx` — the alien block, shifted by the guard difference.
* later regions — further local shifts.

The both-unnamed shape is 44 B shorter than the original in the alien block, so
the tail shifts more than the baseline and raw rises by 165 even though the
instruction-index alignment improves by 65. Conversely, KVBR moves the `this`
reload to the original's `0xb3232` position (raw −2) but reorders the rotation
init after `getPosition`, adding 21 aligned mismatches.

## 6. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5487 -> 5652   (+165)
                           align  774 -> 709    (-65)
.text size             0x1337c8 -> 0x1337c8 (exact)
.text diff             85,372 -> 85,537  (+165)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.ARM.extab diff         1,284 -> 1,284
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

## 7. Open questions

1. The aligned metric cannot go below 709 without fixing the register
   assignment (`this`→r4 remat, `i`→fp, `gun`→r6, `k`→r9, `t`→sl, temp→r5).
   The both-unnamed shape has the right stack layout but a cyclic permutation of
   the register map. A source shape that forces `this` to be rematerialised
   (short live range split into head + tail) is still unknown.
2. The guard if-conversion at `0xb2e04`/`0xb2f2a` is a sample-PGO signature.
   Reproducing it from source would realign the whole tail and drop raw by
   thousands; it needs either a clang flag (not allowed) or a source form that
   makes clang emit the branchless `orrs` at `-Oz` (none found).
3. The 32 B added by the `(i < 0)` hack is the original's individual
   `ldr`/`str` vector construction (~10 B) plus unidentified code.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10509/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10509/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w10509/{fdiff.py,align_diff.py,aldiff.py,aldiff2.py,regions.py,
secdiff.py,harness.py,v*.py,Level.base.cpp,orig_full.asm,ours_full.asm}`.
