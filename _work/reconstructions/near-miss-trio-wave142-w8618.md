# Near-miss trio wave 142 (w8618): reference-to-object spellings, anchor placement,
# and far-call scheduling — all re-confirm the local optima

Status: **no landed `src/` change.** Worktree `wt/w8618`, base `e18e0c2a4` (on top of
the wave-141 note commit `588df3a43`). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave141-w8618.md`, `...-wave140-w8518.md`,
`...-wave139-w8418.md`, `...-wave138-w8318.md`, `mgame-onupdate-wave129-w7661.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.rel.plt` byte-identical.

## 0. Residuals re-verified (raw same-address bytes, sizes exact)

| target | size | raw diff |
|---|---:|---:|
| `MGame::OnUpdate` | 11440 B | 41 |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 |

## 1. New hypothesis: the MenuTouchWindow "reference-to-object" trick on MGame

Wave 141 found that in `MenuTouchWindow` the `Layout &layout = *Globals::layout;`
object reference is the only spelling that keeps `&Globals::layout` in `r4`
(16-bit `ldr r2,[r4,#0]`) and `&Globals::Canvas` in `fp`.  This wave applied the
same idea to `MGame`: replace the `Status *st` pointer anchor with
`Status &st = *Globals::status;` and use `st.getX()` (object reference: address
materialised, value rematerialised at each use).

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`Status *st` anchor at pt) | **41** | 11440 | |
| `Status &st = *Globals::status;` + `st.` for ccm+pt | 41 | 11440 | identical code to HEAD |
| `Status &st = *Globals::status;` + `st.` for ccm+mission+pt+field | 5033 | 11432 | `r5` colour |
| `Status &st = *Globals::status;` + `st.` for ccm only | 5148 | 11392 | `r5` colour |
| `Status *&st = Globals::status;` + `st->` everywhere | 5148 | 11392 | `r5` colour |
| `Status &st = *Globals::status;` + direct ccm/mission, `st.` at pt | 41 | 11440 | identical to HEAD |

So the object-reference spelling behaves exactly like the pointer anchor in this
TU: it is *not* rematerialised, and the 41-byte residual is unchanged.  The
wave-141 conclusion (exact status loads and the exact subtraction are mutually
exclusive at `-Oz`) is re-confirmed from a new direction.

## 2. New hypothesis: a non-status value in `r5` to force `&Globals::status` to `r9`

The 41 bytes exist because the anchor occupies `r5`, pushing `&Globals::status`
into `r9`; without it the whole function re-colours to `r5` and shrinks 48 B
(11392).  This wave tried to occupy `r5` with a *different* long-lived value so
that the status pointer itself could be loaded fresh:

| anchor value | raw | size | note |
|---|---:|---:|---|
| `GameText *gt = Globals::gameText;` | 5148 | 11392 | load sunk to body, no `r9` |
| `FModSound *snd = Globals::sound;` | 5148 | 11392 | load sunk, no `r9` |
| `GameText *gt` + `Status &st` | 41 | 11440 | identical to HEAD |

Only a value *used inside the m23 condition* is live across it; `gameText` /
`sound` are sunk to their first body use, so they do not force `r9`.

## 3. New hypothesis: anchor placed inside the nested `if`

Wave 141's split (outer 3-term `if` + inner 1-term `if`) was re-measured with
the anchor in different positions:

| variant | raw | size |
|---|---:|---:|
| nested, no anchor | 5132 | 11396 |
| nested, anchor at top, direct inner | 5132 | 11396 |
| nested, anchor at top, `st` in inner | 828 | 11440 |
| nested, anchor inside outer block, `st` in inner | 5132 | 11396 |
| nested, anchor at top, `st` in both | 4976 | 11436 |

The 11440/828 shape has exact size but a 2-byte shift over a much larger range
(the branch targets move by 2), so it is worse than HEAD's 41.

## 4. ModStation: faithful `far = pitch` is a pure instruction-scheduling loss

`MatrixSetRotation(farKey, pitch, ...)` (the faithful source; the original's
`r4` is the saved pitch) is 43 raw / 1008 B.  The near-call region is a
permutation of the original:

```
orig 0x186: ldr.w r3,[r0,r8,lsl#2]   ; yaw
     0x18a: movs  r0,#2
     0x18c: str.w r8,[sp,#184]       ; race spill
     0x190: movs  r1,#0
     0x192: add.w r8,sp,#256
     0x196: strd  r1,r0,[sp]
     0x19a: add   r1,sp,#320
     0x19c: mov   r0,r8
     0x19e: mov   r4,r2              ; save pitch
     0x1a0: blx
ours 0x186: str.w r8,[sp,#184]
     0x18a: mov   r4,r2
     0x18c: ldr.w r3,[r0,r8,lsl#2]
     0x190: movs  r0,#2
     ...
```

The pointer-literal load order is also swapped (`r3`/`r4`).  This wave tried
every source-level perturbation that could move the scheduler:

| variant | raw | size |
|---|---:|---:|
| faithful `far = pitch` | 43 | 1008 |
| swap `yaw`/`pitch` declaration order | 43 | 1008 |
| `const float` locals | 43 | 1008 |
| `const float *const` tables | 43 | 1008 |
| `yawTable` declared before `pitchTable` | 46 | 1008 |
| `float farPitch = pitch;` before near / after near / after far-translation | 43 | 1008 |
| near call uses `yawTable[race]` / `*(float*)&yawBits`, far uses `pitch` | 43 | 1008 |
| far call uses `*(float*)&pitchBits` / `(float)pitch` / `pitchTable[race]` | 43 | 1008 |
| `const float zero = 0.0f;` / `const RotationOrder order = 2;` locals | 43 | 1008 |
| `Matrix farKey;` declared early | 687 | 1000 |
| far translation moved before near rotation | 631 | 992 |
| far call uses local `yaw` instead of re-evaluating the ternary | 580 | 984 |
| `float yaw; float pitch;` declared then assigned | 43 | 1008 |

The scheduler consistently hoists `str.w r8,[sp,#184]` and `mov r4,r2` above the
yaw load.  HEAD's `far = yaw` (semantically wrong, but the far call's *bytes*
are identical because the value is only read through `mov r2,r4`) remains the
best metric at **11 raw / 1008 B**.

The literal-pool tail also differs by 4 bytes in the faithful variant
(`0x3e4/0x3e8`: `0x8c16/0x8c38` vs `0x8c3a/0x8c0c`), a consequence of the same
code-order change.

## 5. MenuTouchWindow: new spellings, all 1892 or worse

| variant | raw | size |
|---|---:|---:|
| HEAD (`Layout &layout`, ref used for buttonInsetX + leftMargin) | **28** | 1896 |
| `Layout *layout = Globals::layout;` pointer local, all `->` | 28 | 1896 |
| direct `Globals::layout->` everywhere | 358 | 1892 |
| `Layout *&layout = Globals::layout;` reference-to-pointer | 358 | 1892 |
| outer function-scope `layout` pointer (no inner decl) | 1762 | 1912 |
| outer pointer + direct second comparison | 1761 | 1908 |
| `Layout *volatile layout` | 1730 | 1924 |
| `Layout *const &layout = Globals::layout;` | 358 | 1892 |
| ref for first comparison, direct for second | 358 | 1892 |
| ref for first + second-leftMargin, direct top | 28 | 1896 |
| direct first, ref for second `if` | 314 | 1888 |
| `field_0xc_leftMargin` explicit first in the sum | 28 | 1896 |
| `listEntryWidth` first / local | 34 | 1896 |

The 28 bytes are the same four clusters as wave 141.  The object reference is
load-bearing for the `r4`/`fp` allocation (16-bit `ldr r2,[r4,#0]`), and any
spelling that lets the reference die before the `GetImage2DWidth` call loses
4 bytes of size (1892).  Making the second comparison direct (which is what the
original does) requires the reference to stay live across the call, which in
turn re-introduces the `r6` CSE.  No source shape found that has both.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8618
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8618/fdiff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w8618/`: `fdiff.py`, `dd.py`,
`batch_mg.py` / `batch_mtw.py` / `batch_mod.py` (variant drivers),
`mg_v*.py` / `mtw_v*.py` / `mod_variants*.py` (this wave's probes),
`cc_mgame.sh` / `cc_mtw.sh` / `cc_mod.sh`.

## 7. Files changed

`_work/reconstructions/near-miss-trio-wave142-w8618.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
