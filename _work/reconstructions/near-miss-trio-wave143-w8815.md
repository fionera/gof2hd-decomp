# Near-miss trio wave 143 (w8815): MGame / MenuTouchWindow / ModStation —
# the r9 base is reproducible, the reference live range is not removable,
# and the ModStation hoist is pre-RA

Status: **no landed `src/` change.** Worktree `wt/w8815`, base `bf083ab41`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave142-w8716.md`, `near-miss-trio-wave141-w8618.md`,
`near-miss-trio-wave140-w8518.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.rel.plt` byte-identical.

## 0. Residual classification (raw same-address bytes, re-verified)

| target | size | raw diff | hunks |
|---|---:|---:|---|
| `MGame::OnUpdate` | 11440 B | 41 | 1 (2-byte shift at the ccm anchor) |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 4 clusters (2 + 26 B) |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 | 1 (scheduler/RA copy) |

## 1. MGame::OnUpdate — NEW: a volatile ccm read reproduces the original's
##    r9 base **and** the four fresh loads; the residual is the anchor load

Wave 141/142 established that the plain direct form re-colours
`&Globals::status` to `r5` (11392 B), and that only a value live across the
m23 condition forces `r9`.  This wave found the first shape that gets **both**
the `r9` base and the fresh `ldr r0,[r9]` at the ccm: make the ccm read
volatile so it cannot be CSE'd with the anchor, while the anchor stays live
across the whole condition:

```cpp
Status *st = Globals::status;
if ((*(Status *volatile *)&Globals::status)->getCurrentCampaignMission() == 23 &&
    !this->player->isMining() &&
    Globals::status->getMission()->isEmpty() &&
    st->getPlayingTime() - reinterpret_cast<long long &>(Globals::status->field_0x100) >
        3600000) { ... }
```

Disassembly (ours, 11440 B, raw 853):

```
17e08c  ldr.w r9,[r0]        ; &Globals::status   MATCH
17e090  ldr.w r5,[r9]        ; anchor (extra 4 B)
17e094  ldr.w r0,[r9]        ; ccm                MATCH
17e098  blx ccm
...
17e0bc  mov r0,r5            ; pt (2 B vs 4 B)
17e0be  blx pt
```

So the original's `r9` colour is *not* evidence of a hidden anchor value in
the source; it is the allocator's response to the anchor's live range.  The
anchor load itself is the 2-byte shift (853 raw).  Every shape that removes
the anchor load also removes the `r9` colour.

New probes (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`Status *st` at pt) | **41** | 11440 | |
| volatile ccm + anchor at pt | 853 | 11440 | r9 base + fresh ccm; extra anchor load |
| volatile ccm + comma anchor before pt | 5148 | 11392 | anchor live range too short, base -> r5 |
| volatile ccm + comma anchor before mission | 5129 | 11396 | |
| `Status *&st = Globals::status` | 5148 | 11392 | same as no-anchor |
| `Status *const st` | 41 | 11440 | same as HEAD |
| `Status *st` at ccm only | 5148 | 11392 | |
| `Status *st` at ccm+mission | 5129 | 11396 | |
| `Status *st` at ccm+field | 4993 | 11436 | |
| `Status *st` at all four | 5033 | 11432 | |
| `Status *st` at m23 pt + m24 pt | 4781 | 11436 | |
| `bool due = ...; if (due)` | 5148 | 11392 | |
| reversed subtraction `3600000 < pt - field` | 5148 | 11392 | |
| `>= 3600001` | 5148 | 11392 | |
| `3600000LL` | 5148 | 11392 | |
| `(*(Status **)&Globals::status)->` | 5148 | 11392 | |
| `Status *volatile st` | 6093 | 11340 | stack traffic |
| `GameText *gt` anchor (used in body) | 8284 | 11192 | body re-rolls |

Conclusion: the 41-byte residual is the anchor CSE, now fully explained.  The
original's source is the plain direct form; our compiler's allocator picks
`r5` for it, and the only way to force `r9` is a value live across the
condition, which costs the anchor load.  Irreducible with this compiler.

## 2. MenuTouchWindow::OnTouchBegin — exhaustive reference/direct sweep

The case-15 block has four `Globals::layout` accesses:

* A = first comparison `buttonInsetX`
* B = second comparison `leftMargin`
* C = second comparison `top`
* D = third comparison `buttonInsetX`

All 2^4 combinations of `Layout &layout = *Globals::layout;` (reference,
cached in `r6`) vs `Globals::layout->` (direct, fresh) were measured:

| A | B | C | D | raw | size |
|---|---|---|---|---|---|
| * | ref | dir | * | **28** | 1896 |
| * | dir | ref | * | 29 | 1896 |
| * | dir | dir | * | 358 | 1892 |
| * | ref | ref | * | 304/308 | 1892/1888 |
| * | dir | dir | ref | 314 | 1888 |

The original has **both** B and C fresh (`ldr r2,[sp,#16]; ldr r2,[r2,#0];
ldr r5,[r2,#12]; ldr r6,[r2,#32]`), i.e. the all-direct shape.  But the
all-direct shape re-colours `&Globals::layout` from `r4` to `fp` and
`Globals::Canvas` from `fp` to `r5`, which is 4 B shorter (1892).  The
reference value live across `GetImage2DWidth` is what keeps `fp = Canvas`,
`r4 = &Globals::layout` and `[sp,#16] = &Globals::layout` — the original's
colouring — at the cost of caching the `Layout*` in `r6`.

New probes (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD | **28** | 1896 | |
| `Layout *&lp = Globals::layout` (all) | 358 | 1892 | |
| `Layout **lp = &Globals::layout` (all) | 358 | 1892 | |
| `PaintCanvas *canvas` local inside block | 355 | 1892 | |
| `PaintCanvas *canvas` local at function top | 1701 | 1896 | re-colours whole function |
| `int listEntryWidth` local (case 8) | 62 | 1896 | |
| case-8 sum reorder `lew+top+left` | 33 | 1896 | |
| case-8 sum reorder `(left+top)+lew` | 28 | 1896 | same as HEAD |

The case-8 `ip` vs `r4` cluster (2 B) is independent: it is present in the
all-direct shape too.  The 26-byte cluster is the reference's live range.

## 3. ModStation ctor — HEAD is METRIC-ONLY; faithful `far = pitch` is 43

HEAD's `far = yaw` is semantically wrong (Ghidra: the far
`MatrixSetRotation` x-arg is `pitch`) but scores 11 raw bytes.  The faithful
`far = pitch` is **43** raw / 1008 B on this base.  The diff is:

1. literal-pool order: original `Pitch, YawIPad, Yaw`; ours
   `Pitch, Yaw, YawIPad` (the `ldr r3`/`ldr r4` pair and the
   `add r3,pc`/`add r4,pc` order swap);
2. the RA copy `mov r4,r2` (save pitch) and the `str.w r8,[sp,#184]`
   (spill `race`) are hoisted above the yaw load (ours `d5456`/`d545a`;
   original `d545c`/`d546e`).

New probes (all reverted, all 43 unless noted):

| variant | raw | size |
|---|---:|---:|
| HEAD (`far = yaw`) | **11** | 1008 |
| `far = pitch` | 43 | 1008 |
| `!iPad ? Yaw : YawIPad` | 43 | 1008 |
| `float farPitch = pitch` before/after near call | 43 | 1008 |
| `reinterpret_cast<const float &>` | 43 | 1008 |
| union bitcast | 43 | 1008 |
| `volatile int race` | 867 | 1016 |
| `volatile float` table loads | 745 | 1016 |
| pitch volatile / yaw non-volatile | 44 | 1008 |
| pitch non-volatile / yaw volatile | 666 | 1008 |
| no table locals | 690 | 1000 |
| pitch local only | 690 | 1000 |
| yaw local only | 46 | 1008 |

`-mllvm -enable-misched=false` produces the *same* instruction order as the
default build, so the hoist is **not** the machine scheduler: it is the
pre-RA/register-allocator copy placement.  The literal-pool order is also a
RA artefact (the compiler canonicalises the ternary spelling).  HEAD stays
for the raw metric; the semantic fix is a one-line change
(`farKey, yaw,` -> `farKey, pitch,`) if the project prefers correctness
over the 11-byte count.  **HEAD is METRIC-ONLY.**

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8815
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8815/fdiff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w8815/`: `fdiff.py`, `dd.py`/`dd2.py`/
`dd3.py`/`dd4.py` (per-function disasm), `bdiff.py` (raw byte-run report),
`seqdiff.py` (instruction-sequence diff), `mv*.py` / `mtwv*.py` / `modv*.py`
(variant drivers), `cc.sh` (single-TU compile), `*.cpp.base` (pristine
sources), `mg_decomp.json` (Ghidra decompile of `MGame::OnUpdate`).

## 5. Files changed

`_work/reconstructions/near-miss-trio-wave143-w8815.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
