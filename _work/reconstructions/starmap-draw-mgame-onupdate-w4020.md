# StarMap::draw + MGame::OnUpdate — residual register-allocation analysis (w4020)

Status: research pass, 2026-11-03. Worktree `wt/w4020`, branch `wave90-w4020`,
base master `360c7467`. Every number below was measured in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's NDK r18b clang 7.0.2
and the official `tools/verify/verify.py` (`autojunk=False`). **No source change
was landed**: both targets are already at their best measured shape and every
alternative tried regressed the byte count.

Baseline (unchanged): **byte_exact 4498 / linked_exact 4498**, size-exact
4524/4524, data_exact 843/843, imports 207, `.text 0x1337c8`, `.rodata 0x41b67`,
`.ARM.exidx 0x6f70` exact, `.ARM.extab 0x26290` (-8), lint `CLEAN`, drift 0.
`StarMap::draw` = 99.1% / 2644 B / **15 differing bytes**;
`MGame::OnUpdate` = 99.9% / 11440 B / **41 differing bytes** (one run
`0x17e093..0x17e0bb`).

Build note: this worktree is a git worktree, so the default `../_work` paths do
not resolve. Configure with
`-DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so
-DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`.
`FN=... verify-fn` is broken here (`--show` receives no argument); call
`tools/verify/verify.py --show <mangled>` directly.

Scratch drivers: `/dev/shm/w4020/fdiff.py` (raw byte differ),
`try_sm.py` / `try_sm2.py` / `try_flags.py` / `try_flags2.py` (StarMap),
`try_mg.py` / `try_mg2.py` / `try_mg3.py` (MGame).

## 1. StarMap::draw — 15 bytes, r8/fp role swap

Raw byte runs (all 15 bytes):

```
0xc933b, 0xc9340, 0xc9360, 0xc9362-3, 0xc93a4, 0xc93b2-3,
0xc94d6-7, 0xc94d9, 0xc99c4-5, 0xc9a66-7
```

The whole story is the two loop-invariant stack addresses in the second (route)
loop. Original:

```
c9338  add.w  r8, sp, #136        ; r8 = &Vector temp (sp+136)
c9360  ldr.w  fp, [sp, #16]       ; fp = &this->scratchVector (this+0x78)
c93a4  mov    r1, r8              ; Vector::operator= temp
c93b2  mov    r0, fp              ; ... destination = &this->scratchVector
c94d6  ldr    r0, [r4, #84]       ; this->systems
c94d8  ldr    r0, [r0, #0]        ; ->size()  (re-read every iteration)
```

Ours (current source, `Array<SolarSystem *> &systems = *this->systems;`):

```
c9338  add.w  fp, sp, #136        ; fp = &Vector temp
c9360  ldr.w  r8, [r4, #84]       ; r8 = this->systems (hoisted)
c93a4  mov    r1, fp
c93b2  ldr    r0, [sp, #16]       ; &scratchVector spilled to stack
c94d6  ldr.w  r0, [r8]            ; ->size()  (hoisted, no re-read)
```

The reference is a loop-invariant SSA value, so clang keeps it in r8; that
displaces the `Vector` temp into fp and `&this->scratchVector` onto the stack.
The original re-reads `this->systems` and keeps `&scratchVector` in fp.

### 1.1 The direct form and the Canvas/zero/scratchVector cascade

`for (uint32_t i = 0; i < this->systems->size(); i++)` restores the re-read and
puts the `Vector` temp back in r8, but the function shrinks to **2636 B**
(-8) with **1656 diff bytes** (93.6%). The extra 8 bytes in the original are
inside the loop:

```
original:  c9454 ldr.w sl,[sp,#20]   ; Globals::Canvas (cached in [sp,#20] at entry)
           c948a mov.w sl,#0         ; restore the 64-bit zero constant
direct:    (sl = Canvas hoisted before the loop; fp = zero constant)
```

So the direct form's allocator hoists `Globals::Canvas` from `[sp,#20]` into sl
and moves the zero constant to fp; with fp taken, `&this->scratchVector` is not
hoisted. The original has the zero constant in sl (live from `c936a` to
`c9454`), so sl is unavailable for Canvas and fp is free for
`&this->scratchVector`. It is a chicken-and-egg between three loop-invariant
values (`Vector` temp, `&scratchVector`, zero/Canvas) and only three registers
(r8/fp/sl).

### 1.2 Variants measured

All with the official verifier; `size` is ours, `diff` raw differing bytes.

| variant | size | diff |
|---|---:|---:|
| current (`systems` reference, `systems.size()`) | 2644 | **15** |
| direct (`this->systems->size()`) | 2636 | 1656 |
| direct + `Vector &sv = this->scratchVector;` at loop top | 2636 | 1656 |
| direct + `Vector *sv = &this->scratchVector;` at loop top | 2636 | 1656 |
| direct + `Vector &sv` before loop, used in body | 2636 | 1656 |
| direct + `PaintCanvas *canvas = Globals::Canvas;` inside `if (routes...)` | 2636 | 1535 |
| direct + `PaintCanvas *canvas` at loop top | 2652 | 2199 |
| `Vector &sv` bound before loop, used through animated-route block | 2636 | 1656 |
| `Vector &sv` bound before first loop (spans both loops) | 2644 | 15 |

The last row is the important negative: a `Vector &` reference is fully
rematerializable (`add rX, r4, #0x78`), so it does not force a register and the
allocator falls back to the current 15-byte shape.

### 1.3 Flag sweep (new flags this pass)

Applied per-TU with `set_source_files_properties(... COMPILE_OPTIONS ...)` in
`CMakeLists.txt`; the existing `-likely-branch-weight=7
-unlikely-branch-weight=1` was kept.

| flag | base (2644) | direct (2636) |
|---|---:|---:|
| `-hoist-cheap-insts` | 15 | 1656 |
| `-disable-postra-machine-licm` | 15 | 1871 |
| `-disable-machine-licm` | 2473 (2664 B) | 2410 (2660 B) |
| `-disable-licm-promotion` | 15 | 1656 |
| `-avoid-speculation` | 15 | 1656 |
| `-licm-max-num-uses-traversed=1` / `=100` | 15 | 1656 |
| `-hoist-cheap-insts -disable-postra-machine-licm` | 15 | — |
| `-hoist-cheap-insts -avoid-speculation` | — | 1656 |

No flag improves the base. `-disable-machine-licm` is much worse. Earlier
passes already covered `-regalloc-csr-first-time-cost`, `-optimize-regalloc`,
`-spp-rematerialization-threshold`, `-enable-deferred-spilling`,
`-sink-insts-to-avoid-spills`, `-regalloc=basic/pbqp` (see
`starmap-draw-mgame-onupdate-eh-w3840.md`).

### 1.4 What is proved / what remains

Proved: the 15 bytes are exactly the r8/fp swap plus the `this->systems` bound
hoist; the direct form's 8-byte deficit is the hoisted `Globals::Canvas`
(`ldr sl,[sp,#20]` + `mov sl,#0` removed from the loop). The allocator prefers
Canvas (2 uses/iteration) over `&scratchVector` (1 use) once sl is free.

Remaining: a source shape that makes the allocator keep `&this->scratchVector`
in fp while re-reading `this->systems`. A `Vector &` local is not enough
(rematerialized). The original's source witness for the scratchVector access
pattern is still missing.

## 2. MGame::OnUpdate — 41 bytes, cached Status* in r5

Original (r9 = `&Globals::status`, loaded at `17e08c`):

```
17e08c  ldr.w r9, [r0]        ; r9 = &Globals::status
17e090  ldr.w r0, [r9]        ; Globals::status
17e094  blx   Status::getCurrentCampaignMission
17e0a8  ldr.w r0, [r9]        ; getMission
17e0b8  ldr.w r0, [r9]        ; getPlayingTime
17e0c0  ldr.w r2, [r9]        ; field_0x100
```

Ours (`Status *st = Globals::status;` used at getPlayingTime):

```
17e08c  ldr.w r9, [r0]
17e090  ldr.w r5, [r9]        ; st
17e094  mov   r0, r5          ; CSE'd with the first use
17e096  blx   getCurrentCampaignMission
17e0aa  ldr.w r0, [r9]        ; getMission (fresh)
17e0ba  mov   r0, r5          ; getPlayingTime (st)
17e0c0  ldr.w r2, [r9]        ; field_0x100 (fresh)
```

`ldr r5,[r9]` + `mov r0,r5` + `mov r0,r5` = 4+2+2 = 8 B; original
`ldr r0,[r9]` + `ldr r0,[r9]` = 4+4 = 8 B. Size is exactly neutral.

### 2.1 Why removing `st` fails

All `Globals::status->` gives **11392 B** (-48) and **5148 diff bytes**. The
allocator then puts `&Globals::status` in **r5** instead of r9 and spills it to
`[sp,#36]` before the `Globals::gameText` use at `17e122`; the whole function's
allocation shifts. The 48-byte deficit is the wide `ldr.w rX,[r9]` encodings
(4 B each) that the original pays throughout the region; with r5 they become
2-byte `ldr rX,[r5]`.

### 2.2 r5 liveness in the original (new evidence)

The r5 uses in the original are `17e034` (`ldr r5,[r4,#0]`, the
`Globals::switch_to_target_setting` address) and `17e044` (`strb r0,[r5,#0]`),
then the next r5 use is `17e12a` (`ldr r5,[r0,#0]`, `Globals::gameText`).
**r5 is not referenced anywhere between `17e044` and `17e12a`.** The
`&this->camera` value that lives in r5 from `17da24`
(`ldr.w r0,[r5,#244]!` = `this->camera`, r5 = this+0xf4) is last used at
`17db2e` and overwritten at `17dce4`, long before the condition. So r5 is free
at `17e08c`; the original's choice of r9 is a whole-function greedy-allocator
decision (the `&Globals::status` interval spans `17e08c..17e846`, with a spill
at `17e31c`), not a forced live value. The `st` local is a reconstruction hack
that reproduces r9 by occupying r5 at the start.

### 2.3 Variants measured

| variant | size | diff |
|---|---:|---:|
| current (`Status *st` used at getPlayingTime) | 11440 | **41** |
| no `st` (all `Globals::status->`) | 11392 | 5148 |
| `Status *&st` / `Status **st` / `Status *const &st` / `Status *volatile &st` | 11392 | 5148 |
| `Status &st = *Globals::status;` used at getPlayingTime | 11440 | 41 |
| `st` used at mission-24 getPlayingTime instead | 11440 | 270 |
| `st` used at >=122 getPlayingTime instead | 11440 | 540 |
| `st` used at inAlienOrbit getPlayingTime instead | 11444 | 5007 |
| `st` used at mission-23 tail `field_0x100 = getPlayingTime()` | 11520 | 6233 |
| `TargetFollowCamera *&camera = this->camera;` + no `st` | 11392 | 5982 |

The `TargetFollowCamera *&camera` experiment was run to test whether the
`&this->camera` live range (r5 from `17da24`) is what pins r9. It is not: the
reference is rematerialized (`ldr r0,[fp,#244]`), the `ldr.w r0,[r5,#244]!`
pattern disappears, and the allocation is still 11392 B.

### 2.4 What is proved / what remains

Proved: the 41 bytes are exactly the `ldr r5,[r9]; mov r0,r5` / `mov r0,r5`
caching of `Status*`; the `st` local is required for the 11440 B size because
without it the allocator picks r5 for `&Globals::status` and spills it before
`17e122`. r5 is not referenced in the condition region in the original.

Remaining: a source witness for the allocation that puts `&Globals::status` in
r9 while leaving r5 free. Every direct spelling either removes the size (11392)
or keeps the `mov` (41). The original almost certainly has no `Status *st`
local at all; the r9 choice is a property of the whole-function IR that our
reconstruction does not reproduce.

## 3. Side observations

### 3.1 ModStation::ModStation — volatile/atomic-fence anchor is load-bearing

The ctor currently uses `__atomic_signal_fence(__ATOMIC_SEQ_CST)` plus
`*(const volatile int *)` reads of `gHangarCamPitch`/`gHangarCamYaw`. Replacing
that block with direct `gHangarCamPitch[race]` / `(Globals::iPad ?
gHangarCamYawIPad : gHangarCamYaw)[race]` reads keeps the size at 1008 B but
raises the raw diff from **42** to **667** bytes. The anchor is therefore
load-bearing; it was reverted.

### 3.2 .ARM.extab -8: RecordHandler::recordStoreWrite cleanup ranges

`recordStoreWrite` is byte-exact (3988 B) but its LSDA call-site ranges differ:
orig `(1040,14,3892)` vs ours `(1040,4,3888)+(1048,6,3886)`. Offset 1040 =
`0xcecbc` = `blx Status::getStationStack()`; the original's single 14-byte
range covers the `getStationStack` invoke and the following
`AEFile::Write((int32_t) 3, fd)` invoke, ours splits at `0xcecc0` (after the
`getStationStack` call) and resumes at `0xcecc4` (`movs r0,#3`). This is a
source-scope temporary-lifetime difference, not a table-placement bug; the
body bytes are identical either way. The other four `.ARM.extab` deltas
(`LevelScript::process`, `HangarWindow::render`, `Hud::draw`,
`Level::createShip`, `Generator::createMission`) are body/codegen residuals.

## 4. Conclusion

Both targets are source-shape / register-allocation residuals, not missing
code, wrong constants, wrong callees, or flag problems. No change improves the
byte count without regressing the gate, so the tree is left at the
4498/4498 baseline. The single most important remaining root cause for both is
the same: a loop-invariant value that the original keeps in a specific
callee-saved register (fp for `&StarMap::scratchVector`; r9 for
`&Globals::status`) while our reconstruction reaches only indirectly, and every
direct spelling makes clang choose a different register.
