# Near-miss trio wave 140 (w8518): MGame / MenuTouchWindow / ModStation —
# new allocation levers, no source win

Status: **no landed `src/` change.** Worktree `wt/w8518`, base `d4857f174`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave139-w8418.md`, `near-miss-trio-wave138-w8318.md`,
`long-tail-wave137-w8220.md`, `mgame-onupdate-wave129-w7661.md`,
`mgame-onupdate-r5-cache-wave97-w4717.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.text` differing bytes 85,273 (unchanged),
`.rel.plt` byte-identical.

## 0. Residual classification (raw same-address bytes, re-verified)

| target | size | raw diff | hunks |
|---|---:|---:|---|
| `MGame::OnUpdate` | 11440 B | 41 | 1 (2-byte shift, ccm anchor + pt reuse) |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 2 (2 B `ip`/`r4`; 26 B layout-value rotation) |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 | 1 (scheduler: `mov r4,r2` + `str r8`) |

## 1. MGame::OnUpdate — the anchor's *global* effect, and a new near-shape

The wave-129/139 conclusion is re-confirmed: the `Status *st` anchor is the only
source spelling that pins `&Globals::status` to `r9` for the whole function; the
plain IR gives `r5` and re-rolls to 11392 B.

**New probe family (all reverted):** any local that is live *across* the m23
condition forces the GOT base to `r9`, not just a `Status*` anchor.  A cached
`PlayerEgo *player = this->player;` (used at both the m23 and m24 `isMining`
calls) reproduces the original's **exact four fresh status loads**:

```
17e08c  ldr.w r9,[r0]        ; &Globals::status
17e090  ldr.w r0,[r9]        ; ccm      MATCH
17e0aa  ldr.w r0,[r9]        ; mission  MATCH
17e0ba  ldr.w r0,[r9]        ; pt       MATCH
17e0c2  ldr.w r2,[r9]        ; field    MATCH
```

but the cached player costs a `mov r0,r5` at each `isMining` (the receiver must
be in `r0`), so the m23 body shifts by 2 B and the raw diff is **231**, not 41.
The `(player = this->player, ...)` comma form (load after the ccm call) is the
best of this family at **231**; declaring the local before the `if` gives 250;
m24-only gives 862; m23-only gives 11396 B / 5124.

The anchor is therefore not special as a *value* — it is special because it is
the only value that is both (a) live across the condition and (b) consumed by a
*call* at the first use, so it takes `r5` and pushes the GOT base to `r9`.  Any
other such value adds an unavoidable receiver copy.

Other new MGame probes (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| baseline (`st` at pt) | **41** | 11440 | |
| `st` at ccm+pt | 41 | 11440 | identical code |
| `st` at ccm+field | 4993 | 11436 | ccm CSE, field `ldrd` saves 4 |
| `st` at field only | 4993 | 11436 | |
| `st` at ccm+mission+pt | 4982 | 11436 | |
| `st` at ccm+mission+pt+field | 5033 | 11432 | |
| `st` at ccm+mission+field | 4966 | 11436 | |
| `st` after ccm (comma) at pt+field | 5145 | 11392 | anchor no longer forces r9 |
| `st` after ccm at pt | 5129 | 11396 | |
| `st` at m23 ccm + m24 ccm | 230 | 11440 | exact status loads, +2 shift at m23 ccm |
| `st` at m23 pt + m24 ccm | 4818 | 11436 | |
| `long long &fld = ...` | 5117 | 11396 | |
| `long long fld = ...` | 5040 | 11432 | |
| `field < pt - 3600000` | 4951 | 11436 | |
| `PlayerEgo *p` (both isMining) | 231..250 | 11440 | exact status loads, +2 shift |
| `PlayerEgo *p` (m24 only) | 862 | 11440 | |
| `PlayerEgo *p` (m23 only) | 5124 | 11396 | |

Conclusion: the 41-byte residual is the anchor's `ldr r5,[r9]; mov r0,r5`
(6 B) vs `ldr r0,[r9]` (4 B) at the ccm, compensated by `mov r0,r5` (2 B) vs
`ldr r0,[r9]` (4 B) at pt.  Every source shape that removes the ccm copy also
removes the `r9` colour.

## 2. MenuTouchWindow::OnTouchBegin — new alias/order probes, still 28

Cluster A (`ip` vs `r4`, 2 B) and cluster B (layout-value rotation, 26 B) are
unchanged.  New probes (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (pointer `top` + reference `leftMargin`) | **28** | 1896 | |
| two `Layout&` aliases (`layout`, `layout2`) | 28 | 1896 | folded |
| two `Layout*` pointers | 28 | 1896 | folded |
| `(*Globals::layout).field_0xc_leftMargin` | 358 | 1892 | all-direct shape |
| `Globals::layout[0].field_0xc_leftMargin` | 358 | 1892 | all-direct shape |
| `top + leftMargin + height` (leftMargin fresh) | 358 | 1892 | all-direct shape |
| `leftMargin + top + height` (leftMargin fresh) | 358 | 1892 | all-direct shape |
| fresh leftMargin + third compare via reference | 314 | 1888 | |
| fresh leftMargin + `(void)layout.field_0x20_top` | 358 | 1892 | |
| case-3 sum reorder `lew + left + top` | 537 | 1896 | |
| case-3 sum reorder `left + lew + top` | 537 | 1896 | |
| case-3 sum reorder `top + left + lew` | 28 | 1896 | same as HEAD |
| case-3 `left + (top + lew)` / `(left+top)+lew` | 28 | 1896 | same as HEAD |

The all-direct 1892 shape is now fully classified: it keeps `&Globals::layout`
in **`fp`** (loaded from `[sp,#16]` once) and uses `r5` for a second fresh GOT
copy, whereas the original rematerialises `&Globals::layout` from `[sp,#16]`
into `r4`/`r2`/`r0` at each of the three comparisons and keeps `fp` for
`Globals::Canvas`.  Making `leftMargin` fresh removes the live layout *value*
(`r6`) across the first `GetImage2DWidth`, which is exactly what makes the
allocator keep the address live in `fp`/`r5` instead of rematerialising it.

## 3. ModStation ctor — Ghidra confirms the faithful far x-arg is `pitch`

Ghidra (address `0xe52d0`, i.e. ELF vaddr `0xd52d0` + 0x10000; note the earlier
`0x1e52d0` guess is wrong) decompiles both calls as:

```c
uVar13 = *(undefined4 *)(&UNK_0020e030 + iVar3 * 4);   // gHangarCamPitch[race]
MatrixSetRotation(&near, &nearKey, uVar13, yawTable[race], 0, 2);
MatrixSetRotation(&far,  &farKey,  uVar13, farYawTable[race], 0, 2);
```

So the original's far `MatrixSetRotation` x-argument is **`pitch`**, not `yaw`.
HEAD's `far = yaw` is semantically wrong but scores 11 raw bytes because it
reuses the yaw value in `r4` for both calls.  The faithful `far = pitch` shape
is **43** raw bytes / 1008 B: the scheduler hoists `mov r4,r2` (save pitch)
above the yaw load and swaps the `gHangarCamYawIPad`/`gHangarCamYaw` LDRLIT
order; the far call itself (`d54e0..d5502`) is byte-identical.  All of
`far=pitch`, `const` locals, `float farPitch = pitch`, `pitchTable[race]`
recompute, and declaration-order swaps land at 43 (one at 46); non-volatile
yaw is 44; both non-volatile is 667.  The 11-byte HEAD shape is kept for the
byte metric; the semantic fix is a one-line change (`farKey, yaw,` ->
`farKey, pitch,`) if the project ever prefers correctness over the raw count.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8518
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8518/fdiff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w8518/`: `fdiff.py` (worktree-local copy of
`/dev/shm/fdiff.py`), `dis.py` (per-function objdump of orig/ours),
`normdiff.py` (instruction-sequence diff), `try.py` (build + fdiff),
`mg_try*.py` / `mtw_try*.py` / `mod_try*.py` (variant drivers).

## 5. Files changed

`_work/reconstructions/near-miss-trio-wave140-w8518.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
