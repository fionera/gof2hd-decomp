# wf113: `Level::createMission` (91.128167 %, 17172 B) — wave-101: the remaining
# turret-table/index shape is a *selector* shape, not a table shape

Scope: worktree `/home/debian/GalaxyOnFire2/wt/wf113`, branch `wave101-wf113`,
base `3856949ab` (master). Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address − 0x10000).
**No source change is committed**: every variant below is neutral or a regression.
The deliverable is this note.

## Baseline (unchanged, gate green before and after)

```text
_ZN5Level13createMissionEv   fuzzy 91.128167038 %   size 17172 B   @0xaffd8
                             orig 6055 insns / ours 6062 insns
gate: byte 4500 / linked 4500, size 4524/4524, stubs 0, missing 0, wrong_type 0,
      extra 0, data 843/843, imports 207, .text 0x1337c8, .rodata 0x41b67,
      .ARM.exidx 0x6f70, lint CLEAN, drift 0, RATCHET PASS
```

`cmake --build cmake-build-match --target check -j8` passes before and after this
pass. `report.json` is not modified/committed.

## Result in one line

The alienGuns table **data and layout are already exact** (both tables are at the
same `.rodata` addresses as the original: `kAlienStationTurrets` @ `0x1fcd74`,
`kAlienOutpostTurrets` @ `0x1fce34`, byte-identical). The remaining alienGuns
difference is the **selector**: the original emits an in-loop branchy
`ldr bigAlienWave; cbz; ldr A; add pc; b; ldr B; add pc; add row,k*24` and then
loads the six floats with **individual `ldr`s**; our source (pass 99) emits a
**branchless `it ne/movne`** select and pairs the loads into `ldrd`. The pass-98
`i < 0` anchor is what keeps the select in the loop, but it also forces the
branchless form and an `i` spill; no source spelling found reproduces the
original's branchy `cbz` with clang 7.0.2 at `-Oz`.

A second, independent finding: the original's **kBaseTurrets** loop uses
`add r2,r6,#12; ldmia r2,{r0,r1,r2}; stmia` (positive offsets from the row base),
i.e. 2-D row indexing, while the current 1-D `const float *t = kBaseTurrets[k]`
strength-reduces to `t+20` with negative offsets. Switching that loop to
`const float (*t)[6] = kBaseTurrets; t[k][...]` reproduces the original's
`ldmia`/`stmia` shape locally, but it shifts the function-wide register
allocation and **lowers** the score (91.128 → 91.086), so it is not landable on
its own.

## 1. Table data is exact (proof)

The station/outpost tables are byte-identical and at identical virtual addresses
in both links:

```text
kAlienStationTurrets  first row bytes 082fd2c534a41945f02684c200000080d80fc93f00000000
                      orig @0x1fcd74   ours @0x1fcd74
kAlienOutpostTurrets  orig @0x1fce34   ours @0x1fce34
```

The PC-relative literal deltas in `.text` differ only because the `add rX,pc`
sites differ (original `b32cc/b32d2`; ours `b32ca/b32ce`), not because the data
moved. So the "remaining table/index shape" is not a `.rodata` problem.

## 2. The alienGuns loop: original vs ours

Original (`b32d4`–`b3320`):

```text
b32c6 ldr  r0,[sp,#64]      ; bigAlienWave
b32c8 cbz  r0,b32d0         ; <-- branchy in-loop select
b32ca ldr  r1,[pc,#732]     ; kAlienStationTurrets
b32cc add  r1,pc
b32ce b.n  b32d4
b32d0 ldr  r1,[pc,#728]     ; kAlienOutpostTurrets
b32d2 add  r1,pc
b32d4 add.w r0,r1,r9        ; row = table + k*24
b32d8 mov  sl,r1            ; keep table base (for row[0] after the call)
b32dc ldr  r1,[r0,#8]       ; t[2]
b32de ldr  r2,[r0,#16]      ; t[4]
b32e0 str  r1,[sp,#116]     ; save t[2]
b32e2 str  r2,[sp,#204]     ; dir.y
b32e6 ldr  r1,[r0,#12]      ; t[3]
b32e8 ldr.w r8,[r0,#4]      ; t[1]
b32ec ldr  r0,[r0,#20]      ; t[5]
b32ee str  r1,[sp,#200]     ; dir.x
b32f2 str  r0,[sp,#208]     ; dir.z
b32f4 mov  r0,r5            ; &temp
b32f6 blx  MatrixRotateVector
...
b330c ldr.w r0,[sl,r9]      ; t[0] (2-D row recompute)
b3312 str.w r8,[sp,#204]    ; pos.y = t[1]
b3316 str  r0,[sp,#200]     ; pos.x = t[0]
b331a ldr  r0,[sp,#116]     ; t[2]
b331c str  r0,[sp,#208]     ; pos.z = t[2]
b3320 blx  MatrixRotateVector
```

Ours (pass 99, same slots `sp+200..208` / temp `sp+212`):

```text
b32d0 cmp.w r1,r2,asr #31   ; (bigAlienWave + (i<0))
b32d4 it    ne
b32d6 movne r6,r0
b32d8 add.w r0,r6,r8
b32dc ldrd  fp,r1,[r0,#4]   ; t[1],t[2] paired
b32e2 ldrd  r1,r2,[r0,#12]  ; t[3],t[4] paired
```

The original's load order is `t[2], t[4], t[3], t[1], t[5], t[0]`; the reversed
pairs (`t4` before `t3`, `t2` before `t1`) are exactly why the ARM load-store
optimizer does **not** form `ldrd` in the original. Our branchless select lets
the scheduler/RA order the loads as adjacent ascending pairs, so `ldrd` forms.
The original also keeps `i` in `fp`; ours spills `i` to `sp+116` and reloads it
because the `i < 0` anchor gives `i` an extra use.

## 3. Variant measurements (official verifier, same normalisation as `verify.py`)

All variants are on top of the pass-99 base; `pct` is the raw
`difflib.SequenceMatcher(autojunk=False).ratio()*100` (not rounded).

| variant (alienGuns table/selector/vector shape) | size | ours insns | pct |
|---|---:|---:|---:|
| **pass-99 base: 2-D `t[k][...]`, `(bw+(i<0))`, both inline** | **17172** | **6062** | **91.128167** |
| 2-D, plain `bigAlienWave ? A : B`, both inline | 17144 | 6059 | 88.426614 |
| 2-D, `if/else`, both inline | 17144 | 6059 | 88.426614 |
| 2-D, `switch`, both inline | 17172 | 6062 | 91.128167 |
| 2-D, named `turretDir`/`turretPos`, anchor | 17168 | 6060 | 88.800000 |
| 2-D, named both, plain selector | 17136 | 6053 | 88.404361 |
| 1-D row ptr `const float *t = table[k]`, both inline, anchor | 17180 | 6063 | 91.054629 |
| 1-D row ptr, plain selector (pass-98 D) | 17144 | 6055 | 88.406276 |
| 2-D `table[k]` into `const float *t`, both inline, anchor | 17180 | 6063 | 91.054629 |
| 2-D `table[k]` into `const float &t[6]`, both inline, anchor | 17172 | 6062 | 91.128167 (identical) |
| 2-D, inline first + named second | 17180 | 6065 | 90.973597 |
| 2-D, named first + inline second | 17160 | 6056 | 88.910907 |
| 2-D, inline first + field-wise second | 17172 | 6063 | 90.988612 |
| 2-D, field-wise `y,x,z` both | 17176 | 6061 | 88.610102 |
| 2-D, field-wise `x,y,z` both | 17160 | 6058 | 88.714604 |
| 2-D, `const float *r = t[k]` + `r[...]`, both inline | 17180 | 6063 | 91.054629 |
| 2-D, scalar temps `dx,dy,dz` then `Vector{...}` | 17172 | 6062 | 91.128167 (identical) |
| 2-D, `Vector(a,b,c)` ctor form | 17172 | 6062 | 91.128167 (identical) |

Selector-anchor sweep (2-D, both inline); every one below is size 17172:

| anchor | pct |
|---|---:|
| `bigAlienWave + (i < 0)` **(base)** | **91.128167** |
| `bigAlienWave + (i >> 31)` | 91.128167 |
| `bigAlienWave ^ (i < 0)` | 91.128167 |
| `bigAlienWave + (i < 0 ? 1 : 0)` | 91.128167 |
| `bigAlienWave + (i <= -1)` | 91.128167 |
| `bigAlienWave - (i < 0)` | 91.128167 |
| `bigAlienWave + (k > 6)` | 90.621133 (17180) |
| `bigAlienWave + (k == 7)` | 90.651044 (17180) |
| `bigAlienWave \| (i < 0)` | 89.939754 |
| `bigAlienWave + (i & 0x80000000)` | 89.939754 |
| `bigAlienWave + (i == -1)` | 91.054629 (17164) |
| `volatile int bw = bigAlienWave; bw ? A : B` | 91.111661 |

The branchy `cbz` was not reachable from any spelling tried: plain selector
(hoisted out of the loop), `if/else`, `switch`, `volatile` local,
`volatile int *`, `bool bigAlienWave`, `int *bw = &bigAlienWave`, `A[k]`/`B[k]`
row-select, and `&A[k]`/`&B[k]` row-pointer select all produce either a hoisted
select or an `it/movne` branchless select.

## 4. kBaseTurrets: 2-D indexing is the original shape, but not landable alone

Original kBaseTurrets loop (`b342e`–`b3470`):

```text
b342e add.w r2,r6,#12       ; &row[3]
b343e ldmia r2,{r0,r1,r2}   ; row[3..5]
b3442 stmia r3!,{r0,r1,r2}  ; dir = ...
...
b3462 ldmia.w r6,{r0,r1,r2} ; row[0..2]
b3468 stmia r3!,{r0,r1,r2}  ; pos = ...
```

Current source (`const float *t = kBaseTurrets[k]`) compiles to
`ldrd r0,r1,[sl,#-8]; ldr r2,[sl]` and `sub.w r2,sl,#20; ldmia` (negative
offsets). Changing it to `const float (*t)[6] = kBaseTurrets; t[k][...]`
reproduces the original's positive-offset `ldmia`/`stmia` shape exactly, but
the global RA shifts (`r5`/`r6` swap) and the score drops:

| variant | size | ours insns | pct |
|---|---:|---:|---:|
| **base (1-D row ptr)** | **17172** | **6062** | **91.128167** |
| kBaseTurrets 2-D | 17172 | 6061 | 91.086167 |
| kBaseTurrets 2-D + `pirateFortress` local reorder (best of 6) | 17172 | 6061 | 91.086167 |
| kBaseTurrets 2-D + `b3dd0` bound reorder | 17172 | 6063 | 90.724542 |

The kStationTurrets loop (`kStationTurrets[localRace][k]`, `t[4..6]`/`t[1..3]`)
already matches the original **byte-for-byte** in the linked disassembly
(`b0cf0`–`b0ddc`), including its `ldrd` pairs, so it was left untouched.

## 5. Other source-shape probes (all neutral or regressions)

| probe | result |
|---|---|
| `bool raid` / `int hard` / ternary `raid` assignment | 91.128167 (neutral) |
| `(security == 3) & raid` → `&&` | 87.218417 (17196) |
| `bool alienWave` | 88.551383 (17164) |
| `bool alienWave` + `b3dd0` reorder | 88.419315 (17156) |
| `bool alienWaveFlag = alienWave != 0` local | 91.128167 (neutral) |
| `b3dd0` bound reorder (`start + pirates + extraAliens`) | 90.864075 (17164) |
| `start` grouping `(a+b)+c` / `a+(b+c)` / `guns` first | 91.128167 (neutral) |
| siegedStation cargo `switch` case order / if-chain order | 91.128167 (neutral) |
| alienGuns `Matrix/Vector/Vector` declaration order (6 perms) | best is current `R,V,B`; others 90.73–90.98 |
| pirateFortress `Matrix/Vector/Vector` order (6 perms) | best is current `R,V,B`; others 89.87–89.97 |
| siegedStation `Matrix/Vector/Vector` order (6 perms) | best is current `R,V,B`; others 90.67–90.96 |
| alienGuns last-three statements `setScaling/shipGroup/cargo` (6 perms) | best is current `S,G,C`; others 90.98–91.11 |
| `RaidCoords3` struct → `int[3]` | 91.128167 (neutral) |

## 6. What remains (unchanged from pass 98/99)

1. **alienGuns selector**: original is an in-loop branchy `cbz` with individual
   `ldr`s; ours is an in-loop branchless `it/movne` with `ldrd` pairs and an
   `i` spill. This is the single largest controllable cluster (~20 insns).
   It looks like a clang 7.0.2 `-Oz` select-lowering difference that the
   `i < 0` anchor cannot reproduce; the original's branchy form is smaller
   (7 insns) than the branchless form with a computed condition (9 insns), but
   the compiler still chooses branchless.
2. **raid/guard stack-slot swap** (`raid` at `sp+88` ours vs `sp+84` original;
   guard the mirror): 17 raid refs / 2 guard refs in both, purely a
   StackSlotColoring outcome. No size-preserving source perturbation tried
   flips it.
3. **EH-pad tail** (`orig[5970:6017]`): block order of the landing-pad `b.n`
   chains, not a different try/catch structure (`.ARM.extab` identical).
4. **b2e04** merged predicate `(i != X) | !alienWave`: reachable only with a
   bool-typed `alienWave`, which costs 8 B and 2.6 pp.
5. **convoy switch** ascending block layout: not reachable from case order.

## 7. Method / commands

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j8

# fast single-function score (imports tools/verify/verify.py; ~0.25 s)
python3 /dev/shm/wf113/score1.py _ZN5Level13createMissionEv

# exact side-by-side
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN5Level13createMissionEv
```

Scratch (not committed): `/dev/shm/wf113/` (`Level.base.cpp`, `score1.py`,
`diffmap.py`, `showidx.py`, `regiondiff.py`, `swapdecl.py`, `batch1..27.py`).

## 8. Gate counts (before → after)

```text
byte_exact      4500 → 4500   (4500/4524)
linked_exact    4500 → 4500   (4500/4524)
size_exact      4524 → 4524
stub_zero_size     0 → 0
missing            0 → 0
wrong_type         0 → 0
extra              0 → 0
data_exact       843 → 843
imports          207 → 207
.text          0x1337c8 → 0x1337c8
.rodata        0x41b67  → 0x41b67
.ARM.exidx     0x6f70   → 0x6f70
lint            CLEAN   → CLEAN
drift           0       → 0
RATCHET         PASS    → PASS
```
