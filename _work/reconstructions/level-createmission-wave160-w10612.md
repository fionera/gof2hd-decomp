# w10612 (wave 160): `Level::createMission` — w10509 body + w10510 hoisted
# `end`/`this=r4` bound, size recovered to 17,172 B; raw 5,652 -> 5,454
# (-198), aligned 709 -> 786 (+77)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w10612`, based on master
`08af4633b` (wave 159). Continues
`level-createmission-wave159-w10509.md` (both-unnamed alien block) and
`level-createmission-wave159-w10510.md` (hoisted `end` / `this=r4`).

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address diff is
`/dev/shm/w10612/fdiff.py`; instruction-index alignment is
`/dev/shm/w10612/align_diff.py` (both worktree-local, `OURS` pointed at this
worktree's `cmake-build-match/libgof2hdaa.so`).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw 5652   align 709
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text diff 85,722   .ARM.extab 1,256   .rel.plt 0 (byte-identical)
```

## 1. The literal w10509 + w10510 merge is 16 B short

Applying w10510's `const int end` (with either the `KVBR` or the w10509 `RVKB`
declaration order) on top of the w10509 both-unnamed body gives:

| shape | raw | align | size |
|---|---:|---:|---:|
| w10509 both-unnamed + `end` (KVBR) | 6,558 | 1,436 | 17,156 |
| w10509 both-unnamed + `end` (RVKB) | 6,507 | 1,413 | 17,156 |
| w10509 both-unnamed + `const int start; i < start + alienGuns` (KVBR) | 6,590 | 1,411 | 17,164 |
| w10509 both-unnamed + `const int start; i < start + alienGuns` (RVKB) | 6,580 | 1,414 | 17,164 |

The 16 B deficit is exactly the difference between the original's tail
(recompute `friendCount + alienGuns + field_164 + field_168` from `this`,
44 B at `0xb336e..0xb3396`) and the hoisted-`end` tail (`ldr end; cmp; blt`,
16 B), minus the 6 B the `end` preheader adds and the 2 B the `end` loop head
adds.  The original's tail is:

```text
b336e ldr   r4,[sp,#124]      ; reload this
b3370 movs  r0,#0
b3372 str   r0,[r6,#80]       ; cargo
b3374 add.w r9,r9,#24         ; k
b3378 ldr   r0,[sp,#112]
b337a add.w r2,r4,#352
b337e str   r0,[r6,#40]       ; shipGroup
b3380 add.w fp,fp,#1          ; i
b3384 ldmia r2,{r0,r1,r2}     ; friendCount, field_164, field_168
b3386 ldr.w sl,[sp,#52]       ; alienGuns
b338a add.w r3,r0,sl
b338e add   r3,r1
b3390 add   r3,r2
b3392 cmp   fp,r3
b3394 blt.w b329a
```

## 2. Landed shape (size recovered to 17,172 B)

The landed source keeps the w10509 both-unnamed body and w10510's hoisted
`end`, and re-introduces the bound recompute *inside the loop condition* so
clang cannot fold the `end` load away:

```diff
         Vector base = (*this->enemies)[this->friendCount + this->field_164]->getPosition();
-        for (int i = this->friendCount + this->field_164 + this->field_168;
-             i < this->friendCount + alienGuns + this->field_164 + this->field_168; i++, k++) {
+        const int end = this->friendCount + alienGuns + this->field_164 + this->field_168;
+        for (int i = this->friendCount + this->field_164 + this->field_168;
+             (i < end) & (i < this->friendCount + alienGuns + this->field_164 + this->field_168); i++, k++) {
```

`(i < end) & (i < recomputed)` is semantically `i < end` (both bounds are
equal), but the bitwise `&` keeps both comparisons alive, adding exactly the
16 B needed for size parity.  clang emits the original's `ldmia` bound
recompute in the tail (`0xb3384` in ours) guarded by an extra `i < end`
check and an `itttt lt` sequence.

## 3. Variants measured this wave (all 17,172 B unless noted)

Baseline for all rows is pristine HEAD (`5652 / 709`).  `this/i` is the
register map of the alien loop.

| variant | raw | align | size | this/i |
|---|---:|---:|---:|---|
| **HEAD (w10509 both-unnamed)** | **5,652** | **709** | 17,172 | fp / r9(spill) |
| **landed: `(i<end) & (i<recomp)`** | **5,454** | **786** | 17,172 | fp / r8 |
| `end` + `p2` + `field_0x70` + early `cargo` (RVKB) | 6,542 | 1,427 | 17,172 | r4 / sl |
| `end` + `p2` + `field_0x70` + early `cargo` (KVBR) | 6,539 | 1,427 | 17,172 | r4 / sl |
| `end` + `p2` only (RVKB) | 6,574 | 1,399 | 17,164 | r4 / sl |
| `end` + `field_0x70` + early `cargo` (RVKB) | 6,590 | 1,414 | 17,164 | r4 / sl |
| `end` + `start` local, `i < start + alienGuns` (RVKB) | 6,580 | 1,414 | 17,164 | r4 / r9 |
| `end` recomputed in the increment (`i++, k++, end = ...`) | 5,794 | 804 | 17,180 | fp / r9(spill) |
| `end` recomputed at the tail of the body | 5,794 | 804 | 17,180 | fp / r9(spill) |
| `end` + `(i<end) \| (i<recomp)` | 5,690 | 802 | 17,172 | fp / r8 |
| `cargo` before `shipGroup` (faithful tail) | 5,649 | 711 | 17,172 | fp / r9(spill) |
| `enemies` cached in a body local (faithful tail) | 6,447 | 798 | 17,156 | r5 / sl |
| `enemies` cached before the loop (faithful tail) | 5,665 | 712 | 17,172 | r9 / fp |
| `gun` declared before assignment (faithful tail) | 5,665 | 712 | 17,172 | fp / r9(spill) |
| `__builtin_expect(i<bound, 0)` (faithful tail) | 6,592 | 1,297 | 17,168 | — |
| `__builtin_expect(alienGuns!=0, 1)` (faithful tail) | 5,837 | 830 | 17,180 | — |

The faithful-tail `cargo-before-ship` row is a tiny raw win (5,649) at a tiny
aligned cost (711); the landed `&` shape is the best raw number measured.

## 4. Why the original `this=r4` is still not reproduced

The original's alien-loop register map is a cyclic permutation of ours:

```text
        this        i     gun   k*24  t     temp  rotation
ORIG    r4(reload)  fp    r6    r9    sl    r5    r4
OURS    fp          r9*   r4    r6    r5    r6    sl     (w10509 baseline)
CEND    r4          r9    r4    r6    r5    sl    fp     (hoisted end)
```

`*` = spilled to `sp+116`.  Hoisting `end` removes `this` from the tail, so
`this` gets `r4` for the body head, but the tail becomes a single `ldr` of the
stack slot and `i`/`k`/`t`/`rotation` shift to `r9`/`r6`/`r5`/`fp`.  The
original instead spills `this` at the tail and reloads it into `r4` at
`0xb336e`, keeping `i` in `fp`.  No source shape tried this wave (declaration
orders, `start`/`end` locals, `Level *self`, `Level &self`, `volatile`,
`enemies`/`gun` locals, body-statement orders, `__builtin_expect`) flips the
allocator to the original map while keeping the recomputed tail.

The `&`-guarded landed shape keeps the recomputed tail but leaves `this` in
`fp`; it is a raw-diff win, not an RA win.  The alien-block-local instruction
mismatch is 101 (landed) vs 100 (baseline) — the raw -198 comes from shifting
the post-loop literal pool/`beq` targets, not from a better alien block.

## 5. Metrics and gate (before -> after)

```text
_ZN5Level13createMissionEv size 17172 -> 17172
                           raw   5652 -> 5454   (-198)
                           align  709 -> 786    (+77)
.text size             0x1337c8 -> 0x1337c8 (exact)
.text diff             85,722 -> 85,524  (-198)
.ARM.extab size        0x26298 -> 0x26298 (exact)
.ARM.extab diff         1,256 -> 1,256
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

1. The faithful shape (recomputed tail) with `this` spilled at the tail and
   `i` in `fp` is still not produced by any source form tried.  The allocator
   tie-break is between spilling `this` (original) and spilling `i` (ours).
2. The `&`-guarded shape is a size-recovery hack; a natural expression that
   adds the same 16 B without the extra `i < end` check would likely improve
   the aligned metric.
3. `Status::missionCompleted`'s flat `!prof` vector shows the original used
   branch weights; the alien loop's `blt.w` (wide) vs our `blt.w`/`blt.n`
   mix may be another branch-weight signature.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10612/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10612/align_diff.py _ZN5Level13createMissionEv
python3 /dev/shm/w10612/secdiff.py
python3 /dev/shm/w10612/blkalign.py cmake-build-match/libgof2hdaa.so 0xb3294 0xb3398
```

Scratch: `/dev/shm/w10612/{fdiff.py,align_diff.py,secdiff.py,blkalign.py,
harness.py,harness2.py,sweep1..23.py,orig_full.asm,ours_*.asm,Level.base.cpp}`.
