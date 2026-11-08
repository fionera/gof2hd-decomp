# w9509 (wave 148): `Level::createMission` — raid-block orderings cannot
# produce the target prologue (it needs `this`=r4 in the alien loop, which
# w9508 showed is unreachable); landed the `Vector p` rotate temp in both
# turret loops: raw 5561 -> 5495, align 716 -> 731

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w9509`, based on master
`fece77a64` (wave 148). Continues `level-createmission-wave148-w9411.md`,
`…-wave148-w9410.md`, `…-wave145b-w9014.md`, `…-wave146-w9310.md`, and
`…-wave148-w9508.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(`_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B). Ghidra MCP
unavailable; NDK `objdump` is the authority. Raw same-address byte diff is
`/dev/shm/w9509/fdiff.py`; instruction-index alignment is
`/dev/shm/w9509/align_diff.py` (worktree-local copies of the shared tools).

## Baseline (pristine HEAD, re-measured in this worktree)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw diff 5561   align 716
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, parity 5370/5370, RATCHET PASS
.text 86,225   .ARM.extab 1,309   .rel.plt 4   .note.gnu.build-id 20
```

## 1. The target raid prologue is downstream of `this`=r4 in the alien loop

The original's raid prologue (`b3752`):

```text
b3752  ldr.w r8, [sp,#108]   ; &Globals::status
b3756  ldr   r3, [sp,#84]    ; raid
b3758  ldr.w fp, [sp,#40]    ; fortressGuns
b375c  cmp   r3, #0
b375e  beq.w b396c
b3762  ldr   r0, [sp,#120]   ; &Globals::globals
b3764  ldr   r6, [sp,#68]    ; raidRace
b3766  ldr   r0, [r0]
b3768  mov   r1, r6
b376a  blx   getRandomEnemyFighter
b376e  mov   r9, r0            ; raidType
b3770  ldr.w r0, [r8]          ; Globals::status (deref)
b3774  blx   hardCoreMode
b3778  cmp   r6, #8
b377a  bne.n b37e2
b377c  mov   r8, r0            ; hard
```

Ours (HEAD):

```text
b3752  ldr   r4, [sp,#48]    ; fortressGuns -> r4
b3754  ldr   r3, [sp,#88]    ; raid
b3756  cmp   r3, #0
b3758  beq.w b3964
b375c  ldr   r0, [sp,#108]   ; &Globals::status
b375e  ldr   r6, [sp,#68]
b3760  ldr.w fp, [r0]        ; status pointer -> fp (cached local)
b3764  ldr   r0, [sp,#120]
b3766  mov   r1, r6
b3768  ldr   r0, [r0]
b376a  blx   getRandomEnemyFighter
b376e  mov   sl, r0          ; raidType
b3770  mov   r0, fp
b3772  blx   hardCoreMode
b3776  add   r4, r8          ; guns = alienGuns + fortressGuns (early)
b3778  cmp   r6, #8
```

Every difference (register choice, `#40` vs `#48`, `#84` vs `#88`) is a
consequence of the whole-function allocation. In the original the raid block
also runs with `this`=r4 (`b37e2 add.w r2,r4,#352`), `guns`=r5
(`b37e6 add.w r5,sl,fp`), `raidType`=r9, `fortressGuns`=fp, `alienGuns`=sl;
ours has `this`=r5, `guns`=r4, `raidType`=sl. The chain starts in the alien
gun loop: the original has `this`=r4 / `i`=fp / temp=r5, ours has
`this`=r5 / `i`=r4 (spilled) / temp=fp.

## 2. Raid-block source orderings swept (all keep the alien map r5/fp)

`S` = `Status *status = Globals::status;`, `G` = `int guns = alienGuns +
fortressGuns;`, `R` = `int raidType = …getRandomEnemyFighter(raidRace);`,
`H` = `bool hard = …hardCoreMode();`. `faith` = no cached `status`, `G` after
the `if (raidRace == 8 …)` block (the original's witnessed order).

| variant | size | raw | align | alien (this,temp) |
|---|---:|---:|---:|---|
| **HEAD `S G R H`** | 17172 | 5561 | 716 | r5, fp |
| `G R H` (no local, `G` top) | 17172 | 5590 | 784 | r5, fp |
| `R G H` (no local) | 17172 | 5590 | 784 | r5, fp |
| `R H G` (faith, `G` after if) | **17156** | 6526 | 1185 | r5, fp |
| `G R H` with cached `status` | 17172 | 5561 | 716 | r5, fp |
| faith + `__builtin_expect(raid,1)` | 17176 | 14887 | 1686 | r6, fp |
| faith + `__builtin_expect(raid,0)` | 17256 | 16582 | 2280 | r5, fp |
| faith, `guns` inlined in the loop bound | 17156 | 6526 | 1185 | r5, fp |
| `int guns` hoisted above `if (raid)` | 17148 | 6387 | 807 | r5, fp |

No raid-block ordering changes the alien-loop map: the raid block is after
the alien loop and its intervals do not feed back into it. The faithful raid
block (`R H G`) is 16 B short (17,156) — the original's `ldr.w fp,[sp,#40]`
(+2) and `ldr.w r8,[sp,#108]` (+2) are only reachable once `this`=r4, and the
remaining 12 B are the `guns` add sinking.

**w9508 (`level-createmission-wave148-w9508.md`) proved the sret-temp hoist
`temp=r5` is not reachable from source** (MachineLICM does hoist it, Greedy RA
rematerialises it; every shape that keeps 17,172 B leaves the choice). So the
target prologue is currently unreachable. w9508 landed a note only, so there
is no source to merge with this pass; the merge-order question is moot.

## 3. New hypothesis that did land: `Vector p` rotate temp in *both* turret loops

w9014's `Vector p = MatrixRotateVector(...); v = p;` in the alien loop alone
is a regression on the current base (raw 5782, align 805). Adding the same
temp to the **pirate-fortress loop** as well flips the pirate-fortress
preheader back to the pristine allocation and is a large win:

| variant | size | raw | align |
|---|---:|---:|---:|
| HEAD | 17172 | 5561 | 716 |
| alien `p` only | 17180 | 5782 | 805 |
| pirate `p` only | 17156 | 6452 | 797 |
| **both `p` (landed)** | **17172** | **5495** | **731** |
| both `p` + `v M k b` | 17172 | 5493 | 750 |
| both `p` + `setScaling` after `setPosition` | 17180 | 5821 | 806 |
| both `p` + uncached `(*enemies)[i]` setScaling | 17180 | 5793 | 813 |

Region table (function-relative):

| region | bytes | HEAD | landed |
|---|---:|---:|---:|
| `0x0000..0x32c2` pre-alien | 12994 | 2588 | 2587 |
| `0x32c2..0x33bc` alien-gun loop | 250 | 215 | 230 |
| `0x33bc..0x3426` alien tail | 106 | 17 | 103 |
| `0x3426..0x3504` pirate-fortress loop | 222 | 196 | 199 |
| `0x3504..0x377a` pirate tail (case4Tail) | 630 | 196 | **27** |
| `0x377a..0x3994` raid block | 538 | 484 | 484 |
| `0x3994..0x4314` tail | 2432 | 1865 | 1865 |
| **total** | | **5561** | **5495** |

The win is the `case4Tail` region (196 -> 27); the cost is the alien tail
(17 -> 103). This is the w9311 trade-off in reverse: the `p` temp makes the
pirate preheader allocate like the pristine tree while the alien loop keeps
the `cachedgun` allocation. The raid block itself is unchanged (484).

## 4. Landed change (`src/game/world/Level.cpp`, both turret loops)

```diff
             const float (*t)[6] = (bigAlienWave + (i < 0)) ? kAlienStationTurrets : kAlienOutpostTurrets;
-            v = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[k][3], t[k][4], t[k][5]});
+            v = p;
             gun->geometry->setRotation(v);
...
-            v = MatrixRotateVector(rotation, Vector{t[3], t[4], t[5]});
+            Vector p = MatrixRotateVector(rotation, Vector{t[3], t[4], t[5]});
+            v = p;
             gun->geometry->setRotation(v);
```

This is the original's witnessed `rotate -> temp -> Vector::operator= -> v`
chain (the sret temp is the named `p`); it is not a metric-only hack.

## 5. Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
parity          5370/5370 -> 5370/5370
RATCHET         PASS    -> PASS
.text diff      86,225 -> 86,159   (-66)
.ARM.extab      1,309  -> 1,309
.rel.plt        4      -> 4
.note.gnu.build-id 20  -> 20
_ZN5Level13createMissionEv raw 5561 -> 5495 (-66), align 716 -> 731 (+15), size 17172
```

All 27 section sizes remain exact; file size exact; `.rodata`, `.data`,
`.bss`, `.ARM.exidx`, `.dynsym`, `.dynstr` byte-identical.

## 6. Open questions

1. The alien tail (17 -> 103) is the price of the landed `p` temp. A shape
   that keeps the alien tail at 17 *and* the pirate tail at 27 would save
   another ~86 raw bytes; it needs the alien loop's `this`/`i`/temp map
   (r5/r4/fp) to coexist with the pirate preheader's pristine map.
2. The raid prologue (`ldr.w fp,[sp,#40]` / `ldr.w r8,[sp,#108]`) still
   needs `this`=r4 in the alien loop, which w9508 showed is not reachable
   from source with the frame at 17,172 B.
3. `both p + v M k b` is raw 5493 (2 B better) at align 750 (+19 vs landed);
   left out because the landed `M v k b` is the faithful declaration order.

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w9509/fdiff.py _ZN5Level13createMissionEv
python3 /dev/shm/w9509/align_diff.py _ZN5Level13createMissionEv
```

Scratch: `/dev/shm/w9509/{fdiff.py,align_diff.py,rdiff.py,alienmap.py,
measure.sh,rsweep*.py,combo*.py,final*.py,Level.base.cpp,Level.a1p1.cpp}`.
