# Near-miss trio wave 144 (w8918): MGame / MenuTouchWindow / ModStation —
# anchor-position sweep, partial-volatile layout, and the pitch-save copy

Status: **no landed `src/` change.** Worktree `wt/w8918`, base `f5093474f`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave143-w8815.md`, `near-miss-trio-wave142-w8716.md`,
`near-miss-trio-wave141-w8618.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.rel.plt` byte-identical.

## 0. Residual classification (raw same-address bytes, re-verified)

| target | size | raw diff | hunks |
|---|---:|---:|---|
| `MGame::OnUpdate` | 11440 B | 41 | 1 (anchor CSE at the ccm/pt loads) |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 4 clusters (2 + 26 B) |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 | 1 (copy placement) |

## 1. MGame::OnUpdate — anchor position sweep (NEW)

The wave-143 shape is `Status *st = Globals::status;` before the condition,
used only at the pt call.  The compiler CSEs that load with the ccm load
(`ldr r5,[r9]; mov r0,r5`), which is the 41-byte residual.  This wave swept
*where* the anchor is materialised and *which* of the four status loads it
feeds, looking for a position that keeps `r9` but leaves all four loads fresh.

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`st` before ccm, used at pt) | **41** | 11440 | |
| `Status **stp = &Globals::status` (all uses via `*stp`) | 5148 | 11392 | base rematerialises to r5 |
| same, `*stp` also used at the m23 body tail | 5148 | 11392 | still r5 |
| `Status *st` used at field (pt direct) | 4993 | 11436 | |
| `Status *st` used at body tail (pt direct) | 6265 | 11528 | body re-rolls |
| nested inner `if` + anchor before outer `if` | 828 | 11440 | same 41-byte ccm hunk + body shift |
| nested inner `if` + anchor inside | 5132 | 11396 | loses r9 |
| comma `(st = Globals::status, ...)` at ccm | 41 | 11440 | identical to HEAD |
| comma anchor before mission, `st` at mission+pt | 5129 | 11396 | loses r9 |
| comma anchor before mission, `st` at mission only | 5148 | 11392 | loses r9 |
| comma anchor before pt, `st` at pt+field | 5148 | 11392 | loses r9 |
| `Status *volatile st` | 6076 | 11340 | stack traffic |
| `Status *st = *(Status *volatile *)&Globals::status` | 41 | 11440 | identical to HEAD |

Conclusion: the anchor must be a plain `[r9]` load before the ccm to force
`r9`; any comma/volatile/pointer spelling either loses `r9` or reproduces
the same CSE.  The 41 bytes are unchanged and remain irreducible with this
compiler.  (The nested-with-anchor shape is the only new shape that keeps
size 11440, but it still carries the same 41-byte hunk plus a body shift.)

## 2. MenuTouchWindow::OnTouchBegin — partial-volatile layout and explicit
##    temporaries (NEW)

The 26-byte case-15 cluster is the `Layout &layout` reference value cached in
`r6` across the first `GetImage2DWidth` call.  The original has all four
`Globals::layout` accesses fresh.  This wave tried to force a fresh load while
keeping the reference's colouring (`fp = Canvas`, `r4 = &Globals::layout`).

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`Layout &layout`, B=ref, C=dir) | **28** | 1896 | |
| B=dir, C=ref (reference for `top`) | 29 | 1896 | closest structural swap |
| B=dir, C=dir (all direct) | 358 | 1892 | re-colours fp/r5 |
| `int w = Globals::w` local | 358 | 1892 | |
| `PaintCanvas *canvas = Globals::Canvas` local | 355 | 1892 | |
| `Layout *lp = Globals::layout` + comma reload | 358 | 1892 | |
| `int right = w - buttonInsetX` local | 358 | 1892 | |
| `int right` + `int bottom` locals | 338 | 1892 | |
| `Layout *layout` (pointer, not reference) + locals | 308 | 1888 | |
| volatile `Globals::layout` for all four | 127 | 1896 | keeps size, re-colours |
| volatile for C only (B stays reference) | 39 | 1896 | |
| volatile for B only (C direct) | 127 | 1896 | |
| volatile for B+C (A reference) | 127 | 1896 | |
| case-1/2 y-sum reorder (`left+top+scroll`, `scroll+top+left`, nested) | 28 | 1896 | 2-byte `ip`/`r4` cluster unchanged |
| `int scroll = this->scrollOffset` local | 28 | 1896 | |
| y-sum reorder with `field_0x108` first / all-in-one | 28 | 1896 | |

The 2-byte cluster at `0x3fe` (`ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]`
for `this->scrollOffset` in the case-1/2 `okButton->setPosition` y-sum) is a
pure RA choice and is insensitive to every sum reordering / local tested.
The 26-byte cluster is the reference's live range: making the load volatile
does force fresh loads but re-colours `fp`/`r5` and costs 11 more bytes.
HEAD stays at 28.

## 3. ModStation ctor — faithful `far = pitch` re-confirmed at 43;
##    HEAD's `far = yaw` is still METRIC-ONLY

Re-measured on this base: HEAD (`far = yaw`) = **11** raw / 1008 B;
faithful `far = pitch` = **43** raw / 1008 B.  The 43-byte diff is exactly:

1. literal-pool order: original `Pitch, YawIPad, Yaw`; ours `Pitch, Yaw,
   YawIPad` (the `ldr r3`/`ldr r4` pair and `add r3,pc`/`add r4,pc` swap);
2. `mov r4,r2` (save pitch) is hoisted above the yaw load (ours `d545a`;
   original `d546e`), and `str.w r8,[sp,#184]` moves with it.

New probes (all reverted, all 43 unless noted):

| variant | raw | size |
|---|---:|---:|
| HEAD (`far = yaw`) | **11** | 1008 |
| `far = pitch` | 43 | 1008 |
| `int pitch` + `*(const float *)&pitch` at both calls | 43 | 1008 |
| `int pitch = pitchBits` copy + `*(const float *)&pitch` | 43 | 1008 |
| `float pitch` declared before `float yaw` | 43 | 1008 |
| `(void)pitch` after the near call | 43 | 1008 |
| `float farPitch = pitch` before far call | 43 | 1008 |
| `!iPad ? Yaw : YawIPad` spelling | 43 | 1008 |

MIR trace: the RA coalesces `pitchBits` to `r4` (callee-saved) and inserts
`$r4 = COPY $r2` at the definition (after the yaw load).  The post-RA
top-down list-latency scheduler then hoists it above the yaw load.  With
`-mllvm -enable-post-misched=false` the hoist remains (a later pass also
moves it), so the placement is not recoverable from source spelling.  The
original instead coalesces `pitchBits` to `r2` and copies to `r4` at the
end of the block.  HEAD stays for the 11-byte metric; the semantic fix is
the one-line `farKey, yaw,` -> `farKey, pitch,`.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8918
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8918/fdiff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w8918/`: `fdiff.py`, `bdiff.py` (raw
byte-run report), `dd.py` (per-function disasm), `cc.sh` (single-TU compile),
`try_mg.sh` / `try_mtw.sh` / `try_mod.sh` (variant drivers), `*.cpp.base`
(pristine sources), `mod.ll` / `mir.txt` (LLVM IR and machine-instr dumps).

## 5. Files changed

`_work/reconstructions/near-miss-trio-wave144-w8918.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
