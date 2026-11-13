# `Gun::calcCharacterCollision` wave 191b (w13822): fresh RA/hoist angle —
# 463/57 plateau holds, no src change

Worker: w13822. Branch `wave191b-w13822`, base master `6b680e716`.
Original: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Symbol `_ZN3Gun22calcCharacterCollisionEv`, objdump vaddr `0x152f90`, size
`1984 B`, 660 insns. Scratch: `/home/debian/GalaxyOnFire2/scratch/w13822`.
Predecessors: `gun-calccollision-wave191b-w13794.md` (early-out/temp audit, 40+
candidates), `...-w13766.md` (faithful C-D-E tail landed, E1 slot flip),
`...-w13752.md`, `...-w13736.md`.

## 0. Result

**No `src/` change landed.** HEAD re-measured this session:

| build | raw | aligned | size | insns | `.text` | `.ARM.extab` | `.ARM.exidx` | `.rodata` |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| HEAD `6b680e716` | **463** | **57** | 1984/1984 | 660/660 | 0x1337c8 | 0x26298 | 0x6f70 | 0x41b67 |

Section **addresses** unchanged (`.text`@0x69440, `.ARM.exidx`@0x19cc08,
`.ARM.extab`@0x1a3b78, `.rodata`@0x1c9e10). `.rel.plt` untouched. `check` was
not re-run because `src/` is untouched; the last full gate remains byte_exact
4508/4524, linked_exact 4508/4524, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, parity 5370/5370.

This pass took the requested fresh angle (Ghidra decompile vs source on the
`BSphere`/`Vector` temporaries, float-compare order, early-out ordering) but
instead of re-deriving the known root cause it **pinned the residual to three
exact instruction windows in the ground truth** and swept 46 new source shapes
against them. None beats 463/57.

## 1. New hard evidence: the original has no hoisted `sp+104` address

The residual is not logic; it is the physical register roles. The original
keeps `i*3` in **r4** and treats `sp+104` as a per-block scratch in **r5**;
ours keeps the long-range `sp+104` address in **r4** and `i*3` in **r5**.
Direct objdump witnesses (function-relative; both `.so` place the symbol at
`0x152f90`, so addresses are directly comparable):

### 1a. Scatter radius slot (orig 0x1530d2 vs ours)

```
ORIG 1530d2: ac0b   add r4, sp, #44    ; playerPosition  @ sp+44
ORIG 1530dc: ad1a   add r5, sp, #104   ; delta           @ sp+104
OURS 1530d2: ac1a   add r4, sp, #104   ; delta           @ sp+104
OURS 1530dc: ad0b   add r5, sp, #44    ; playerPosition  @ sp+44
```

### 1b. View block: one `add r5,sp,#104` + `mov`s, not three `add`s

```
ORIG 1533c8: ad1a        add  r5, sp, #104     ; &view, once
ORIG 1533ca: 4601        mov  r1, r0
ORIG 1533cc: 223c        movs r2, #60
ORIG 1533ce: 4628        mov  r0, r5           ; memcpy dst
ORIG 1533d0: f70b ef08   blx  __aeabi_memcpy
ORIG 1533de: 4629        mov  r1, r5           ; MatrixSetTranslation arg
ORIG 1533ec: 462a        mov  r2, r5           ; TransformSetLocal arg

OURS 1533c8: 4601        mov  r1, r0
OURS 1533ca: a81a        add  r0, sp, #104     ; memcpy dst   (remat)
OURS 1533cc: 223c        movs r2, #60
OURS 1533ce: f70b ef0a   blx  __aeabi_memcpy
OURS 1533dc: a91a        add  r1, sp, #104     ; MST arg      (remat)
OURS 1533ea: ac1a        add  r4, sp, #104     ; TSL arg      (remat)
OURS 1533f4: 4622        mov  r2, r4
```

The original materialises the view address **once** and reuses r5 (a
callee-saved scratch) across the two calls; ours rematerialises it three times
because r4 is the long-range `%513` value and cannot be repurposed.

### 1c. i reload: orig reloads `fp` from `[sp,#40]`, ours keeps `fp` live

```
ORIG 153196: 9809        ldr  r0, [sp, #36]
ORIG 153198: f8dd b028   ldr.w fp, [sp, #40]   ; i reloaded
ORIG 15319c: 9e06        ldr  r6, [sp, #24]
...
OURS 153196: 9809        ldr  r0, [sp, #36]
OURS 153198: ac1a        add  r4, sp, #104     ; remat instead
OURS 15319a: 9e06        ldr  r6, [sp, #24]
...
OURS 153278: f8dd b028   ldr.w fp, [sp, #40]   ; extra reload (compensation)
```

### 1d. k-loop rotation (unchanged from w13794)

```
ORIG 15343e: 2500        movs r5, #0
ORIG 153440: e05c        b.n  1534fc           ; header at bottom
ORIG 153442: 6840        ldr  r0, [r0, #4]     ; body
ORIG 153444: ac1a        add  r4, sp, #104     ; kpos @ sp+104, per iteration

OURS 15343e: 2500        movs r5, #0
OURS 153440: f8d9 00b4   ldr.w r0, [r9, #180]  ; header at top
OURS 153444: 6801        ldr  r1, [r0, #0]
OURS 153446: 428d        cmp  r5, r1
OURS 153448: f4bf af06   bcs.w 153258
```

Clusters 1a–1c are one physical fact: the original's `sp+104` address is
**rematerialised per block** (`add r5,sp,#104` at 0x1533c8, `add r4,sp,#104`
at 0x153444, `add r6,sp,#104` at 0x1535f8/0x153626 for the mine branch) and
never occupies a callee-saved register across the function. Ours hoists one
`t2ADDri %stack.4` to the e-loop preheader, MachineCSE folds it into `%513`,
and Greedy RA's `(1u<<29)+Size` priority gives `%513` (span ≈14608) r4 over
`i*3` (`%99`, span ≈12832) → r5. That single swap cascades into 1a–1c and the
k-loop rotation. (Mechanism detail: w13794 §4, w13766 §2.)

## 2. Candidates measured this session (46 new shapes)

All builds `link_so -j2`; measurement `tools/fn_diff.py --ours
cmake-build-match/libgof2hdaa.so _ZN3Gun22calcCharacterCollisionEv`.
"neutral" = byte-identical to HEAD (463/57/1984/660, sections unchanged);
these prove the source spelling of those constructs is not the residual.

### 2a. Neutral (463 / 57 / 1984 / 660)

| variant | change |
|---|---|
| `mine_int`, `scatter_int`, `bomb_int` | `bool` → `int` for the type flags |
| `mine_const`, `scatter_const`, `wt_nonconst`, `ki_const`, `const_posvec` | `const`/type qualifier shuffles |
| `explosive_b`, `eoc_bool` | `int` → `bool` for `explosive`/`explodeOnContact` |
| `outer_or` | `!(outer && collide)` → `!outer \|\| !collide` |
| `box_rev` | `!(A && … && !outer)` → `!(A && …) \|\| outer` |
| `ki_outer`, `ki_not` | `ki != nullptr` → `ki`; `(ki == nullptr \|\| …)` → `(!ki \|\| …)` |
| `geoms_truthy`, `owner_not`, `playergun_ne`, `explosive_ne`, `scatter_ne`, `mine_ne`, `asteroid_ne` | `ptr`/`bool` truthiness respellings |
| `dv_noconst` | `const volatile int &dv` → `volatile int &dv` |
| `radius_half_div`, `mine_r_impl` | `* 0.5f` → `/ 2.0f`; `(float) enemy->radius` → implicit |
| `k_incr_body`, `k_cmp_swap`, `k_postinc` | `++k` moved into body / `size() > k` / `k++` |

### 2b. Regressions (all size ≠ 1984 unless noted)

| variant | raw | aligned | size/insns | note |
|---|---:|---:|---|---|
| **HEAD** | **463** | **57** | 1984/660 | — |
| `swap_earlyouts` | 475 | 65 | 1984/660 | `isActive`/`getHitpoints` swapped |
| `hitpos_first` | 484 | 63 | 1984/660 | hitPositions before hitFlags |
| `life_before_hv` | 492 | 63 | 1984/660 | lifetimes before setHitVector |
| `lastki_after_imp` | 622 | 66 | 1984/660 | lastHitKIPlayer after impact block |
| `mine_negr` | 980 | 185 | 1988/660 | mine box uses `-r` |
| `impact_inner_rm` | 1088 | 99 | 1980/658 | drop redundant inner impact null-check |
| `outer_int` | 1342 | 117 | 1976/656 | `bool outer` → `int outer` |
| `radius_ternary_if` | 1454 | 113 | 1976/659 | radius ternary → if/else |
| `camcur_cached` | 1448 | 282 | 1968/655 | cache `CameraGetCurrent()` |
| `box_negr` | 1585 | 140 | 1980/658 | box test uses `-r` |
| `icount` | 1681 | 151 | 1988/662 | cache `positions.count` |
| `kcount_comma` | 1715 | 266 | 1960/653 | cache `enemies->size()` in k-loop |
| `damage_075` | 1723 | 140 | 1976/658 | `*3.0f*0.25f` → `*0.75f` |
| `positions_ptr` | 1740 | 362 | 1940/656 | cache `positions.data()` |
| `velocities_ptr` | 1773 | 372 | 1932/655 | cache `velocities.data()` |
| `ecount` | 1782 | 141 | 1988/662 | cache `enemies->size()` in e-loop |
| `scatter_before_imp` | 1788 | 211 | 1972/658 | scatter block before impact block |
| `enemies_local` | 1852 | 523 | 1964/665 | cache `this->enemies` |
| `canvas_top` | 1864 | 264 | 1896/651 | cache `Globals::Canvas` at fn top |
| `view_decl_ki` | 1883 | 372 | 2064/690 | `Matrix view;` before `ki` |
| `view_decl_asteroid` | 1884 | 392 | 2056/688 | `Matrix view;` before asteroid branch |
| `view_decl_eoc` | 1892 | 382 | 2048/684 | `Matrix view;` before explodeOnContact |
| `view_decl_boxtest` | 1909 | 384 | 2072/692 | `Matrix view;` before outer gate |

The `view_decl_*` family is the direct test of the w13766 hypothesis that the
original's pre-RA stack objects for `view` and the 12-byte temps did not
overlap: declaring `Matrix view;` earlier does start its lifetime earlier, but
it also forces a **separate 60-byte stack object** (the frame is already
`sub sp,#0xd0` with slots at sp+44/sp+104 and only ~40 B slack below the guard
at sp+204), so the frame/prologue and instruction count grow by 8–22 insns and
the win is lost. The original frame is byte-identical to ours (`push
{r4-r7,lr}; …; sub sp,#0xd0`), so its pre-RA objects must have been merged too;
the difference is only *which* value RA keeps in a callee-saved register.

## 3. Conclusion

The early-out ordering, `outer`/`outerCollide` gate, float compare order, both
60-byte `Vector`/`Matrix` temporaries and the C-D-E scatter tail are already
faithful (w13794/w13766). This pass adds exact disassembly witnesses that the
original rematerialises `sp+104` per block and keeps `i*3` in r4, and shows
that 46 further source respellings — including every truthiness/const/type
variant of the flags, both box-test polarities, all three k-loop spellings, the
scatter/impact block order, and five `Matrix view;` lifetime-extension shapes —
are either byte-identical to HEAD or strictly worse. The residual remains the
single pre-RA StackColoring merge → EarlyMachineLICM hoist → MachineCSE `%513`
→ Greedy RA priority cascade. HEAD (463 / **57** / 1984 / 660) stays.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13822
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Gun22calcCharacterCollisionEv
OBJ=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJ -d --start-address=0x1533a0 --stop-address=0x153450 cmake-build-match/libgof2hdaa.so
```

Scratch helpers: `run_variants.py` (build+measure+sections), `batch6.py`–
`batch10.py` (this session's 46 variants), `dis.sh` (objdump window),
`head.so` (HEAD snapshot), `ir_head.ll` (function IR), MIR dumps
`presc_fn.mir`/`sc_fn.mir`/`licm_fn.mir`/`cc_greedy.mir`.
