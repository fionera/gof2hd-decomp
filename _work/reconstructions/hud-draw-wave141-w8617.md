# Hud::draw — wave 141 (w8617): full contiguous-hunk census and the dock2 String-first LSDA proof

**Status:** source change landed, 2026-11-07 (wave 141). Worktree `wt/w8617`,
base `master e18e0c2a4`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, symbol parity 5370/5370.

Raw same-address diff (primary metric): **1362 -> 1362 B** (tie; the landed
change is a faithful type spelling, see §4). Whole-`.text` differing bytes
**86,472** (unchanged). Function size stays **9332 B**; every section
size/base stays exact (`.ARM.extab 0x26298`, `.rodata 0x1c9e10`).

---

## 0. TL;DR

1. This note re-diffs the **full Thumb listing** as *contiguous same-address
   byte runs* (not difflib opcodes) and classifies every run by source region
   (§1). The largest contiguous run is **119 B at func-relative `0x2037`**
   (VMA `0x165bc7`) — the dock2 `String text` / `halfW` order, not the 21-byte
   difflib hunk reported by wave 140.
2. The faithful `String text`-before-`halfW` reshape reproduces the original's
   instruction order exactly (**1352 raw, 9332 B**) but is **EH-bound**: the
   `.ARM.extab` LSDA for `Hud::draw` grows by 4 B (`0x26298 -> 0x2629c`), which
   pushes `.rodata` from `0x1c9e10` to `0x1c9e20` and drops `byte_exact` to
   **4044** (verified with a full `check`). Rejected.
3. Decoding the LSDA call-site table (§3) shows why: the text-first build has
   **78 call-site entries** vs the original's and the landed build's **77**.
   The extra entry is the dock2 `GetTextWidth`/`GetTextHeight` split at
   `0x20e4`/`0x2130`; the original merges both into one `0x20e0 len 0xcc`
   entry. The split is a consequence of the dock1 block being 4 B longer than
   the original, which shifts every dock2 offset by +4.
4. A 256-combination type sweep (`frameH` x `total` x `barW` x `barH` in
   `{int, short, unsigned short, unsigned int}`) on the landed source finds
   **no** combination below 1362 raw that keeps 9332 B and `.ARM.extab`
   `0x26298`. A second 256-combination sweep on the text-first source finds
   **no** combination with `.ARM.extab` `0x26298` at all.
5. **Landed:** `barH` is spelled `short` instead of `unsigned short`. Both are
   allocator tie-breaks (the original's `barH` is `int`, no extend), but
   `short` emits `vcvt.f32.s32`, matching the original's signed conversion
   (`1659ca: vmov s0,r8; vcvt.f32.s32`), whereas `unsigned short` emits
   `vcvt.f32.u32`. Raw is a tie at 1362 (the `sxth` vs `uxth` extend is the
   same byte count); the change is a faithfulness fix, not a metric win.

---

## 1. Contiguous same-address hunk census

`regions.py` groups adjacent differing bytes (gap 1) and sums per source
region. Every run is size-neutral; the function is 9332 B on both sides.

| region | func offset | raw diff |
|---|---|---:|
| regen `pivotY` trace | `0x467` | 1 |
| gamma body | `0x4c5`-`0x4f0` | 5 |
| damage indicators | `0x5e6`-`0x7e1` | 481 |
| literal pool (damage) | `0x974`-`0x990` | 8 |
| dock1 (docking transfer bar) | `0x1cd6`-`0x1f4d` | 316 |
| dock2 (jump-drive/cloak bar) | `0x1f50`-`0x21b7` | 542 |
| EH cleanup pad | `0x242c`-`0x243e` | 7 |
| literal pool (tail) | `0x245c`-`0x2460` | 2 |
| **total** | | **1362** |

Largest contiguous runs (all regions):

| raw | func offset | VMA | character |
|---:|---|---|---|
| 119 | `0x2037`-`0x20ad` | `0x165bc7` | dock2 `String`/`halfW` order |
| 75 | `0x1eaa`-`0x1ef4` | `0x165a3a` | dock1 `(float)barH*2.5f` (`s32` vs `u32`) |
| 65 | `0x696`-`0x6d6` | `0x164226` | damage block-2 (4-byte shift) |
| 52 | `0x1dbc`-`0x1def` | `0x16594c` | dock1 `mov r5,fp` / `barH` register |
| 50 | `0x1fbb`-`0x1fec` | `0x165b4b` | dock2 `top`/`barYOffset` schedule |
| 48 | `0x1f1e`-`0x1f4d` | `0x165aae` | dock1 `DrawImage2D` tail |
| 48 | `0x62f`-`0x65e` | `0x1641bf` | damage block-1 (4-byte shift) |
| 46 | `0x7b4`-`0x7e1` | `0x164344` | damage block-4 rotation |

The damage region's 481 raw is almost entirely the 4-byte shift caused by the
two dead `ldr.w r8,[sp,#108]` loads (8 B) and the extra `Globals::status &&`
null test (12 B). The dock1 region's 316 raw is the `frameH`/`gameText` stack
slot swap (`sp+96`/`sp+92`), the `barW`/`barH` register swap, and the missing
`mov r5,fp` Canvas preservation. The dock2 region's 542 raw is the
`String`/`halfW` order plus the same register rotation.

---

## 2. The largest run: dock2 `String text` before `halfW`

The original constructs the jump-drive `String text` **before** `halfW`:

```
165ba6: ldr r0,[pc,#188]      ; Globals::gameText
165ba8: mov.w r1,#318
165bac: ldrb.w r2,[sl,#631]   ; field_0x277
...
165bbe: blx GameText::getText
165bc2: mov r1,r0
165bc4: add r0,sp,#208
165bc6: movs r2,#0
165bc8: blx String::String(String const&, bool)
165bcc: ldr.w r1,[sl,#892]    ; image_0x37c
165bd0: ldr.w r0,[r9]         ; Canvas
165bd4: blx PaintCanvas::GetImage2DWidth
...
165be8: vcvt.s32.f32 s18,s0   ; halfW
```

The landed tree computes `halfW` first and constructs `String text` second.
The faithful reshape

```cpp
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
```

measures **1352 raw / 9332 B** (10 bytes better) but **`.ARM.extab`
`0x26298 -> 0x2629c`**. Full `check` on that build: `byte_exact 4044`,
`linked_exact 4507`, `RATCHET FAIL` (`.rodata` shifts to `0x1c9e20`).
Rejected on section-size grounds.

---

## 3. Why: the LSDA call-site table grows by one entry

`Hud::draw`'s `.ARM.exidx` entry is at `0x1a1e60`; its `.ARM.extab` entry is
at `0x1c57cc` (generic model, personality `0x5eb60`). The LSDA starts at
`0x1c57dc` (`ff ff 01 <len>` = omit/omit/uleb128/call-site-table-length).
Decoding the call-site table (`lsda2.py`):

| build | table length | entries | table end |
|---|---:|---:|---:|
| original | 454 | **77** | `0x1c59a7` |
| landed (`unsigned short`) | 453 | **77** | `0x1c59a6` |
| text-first | 459 | **78** | `0x1c59ac` |

The split is **not** a consequence of the dock1 length: the landed
halfW-first build has the same `0x20e4`/`0x2130` split (its table length is
453 because it instead merges the two 4-byte icon-draw entries `0x2044`/
`0x2064` into one `0x1fb0 len 0xba` range). `int barH` (dock1 4 B shorter)
also keeps the split (`0x20dc`/`0x2128`, table 459). The gap always covers the
`float filled = progress * (float) halfW;` argument computation between
`DrawImage2D` and `DrawRegion2D`; any source reshape that moves `filled`
(inline, pre-declared, or between the two calls) changes the whole-function
allocation and grows the function (see §5).

The tables are identical through entry 67 (start `0x1dbe`/`0x1dbc`). The
text-first build adds an entry at `0x20e4`:

```
orig : 71: 2044   4 2434 | 72: 2064   4 2432 | 73: 20e0  cc 2444 | 74: 21ac e2 0
landed: 71: 206a   4 2432 | 72: 20e4  2a 243e | 73: 2130  7e 2444 | 74: 21ae e0 0
text  : 71: 2048   4 2434 | 72: 2068   4 2432 | 73: 20e4  2a 2440 | 74: 2130 7e 2446 | 75: 21ae e0 0
```

The original merges the whole dock2 draw sequence into one `0x20e0 len 0xcc`
range with a single landing pad `0x2444`. Both our builds keep the
`DrawImage2D` and `DrawRegion2D`/`DrawString` invokes in two ranges separated
by the `filled` argument-computation gap; text-first additionally keeps the
two icon-draw entries separate (matching the original) while the landed
halfW-first build merges them, which is why the two builds have different
table lengths at the same 77/78-entry counts.

The landed build's LSDA is also not byte-exact (3 bytes differ at
`0x1c57df`, `0x1c5806`, `0x1c5818`), but its *size* is exact, which is what
the `.rodata` base depends on.

---

## 4. Landed change

```cpp
-            unsigned short barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
+            short barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
```

The original's `barH` is `int` (`mov fp,r0`, no extend), so both 16-bit
spellings are allocator tie-breaks. `short` is the faithful *signed* spelling:
it emits `sxth` + `vcvt.f32.s32`, matching the original's
`vmov s0,r8; vcvt.f32.s32` conversion at `0x1659ca`; `unsigned short` emits
`uxth` + `vcvt.f32.u32`. Raw is a tie at 1362 (the extend is the same byte
count either way), and every section size stays exact.

---

## 5. Sweeps (all on the landed source unless noted)

| variant | raw | size | `.ARM.extab` |
|---|---:|---:|---|
| `barH unsigned short` (wave-140) | 1362 | 9332 | `0x26298` |
| **`barH short` (landed)** | **1362** | **9332** | **`0x26298`** |
| `barH int` / `long` | 1873 | 9328 | `0x26298` |
| `barH unsigned int` | 1874 | 9328 | `0x26298` |
| text-first (`String` then `halfW`) | 1352 | 9332 | `0x2629c` |
| text-first + `barH int` | 1967 | 9324 | `0x2629c` |
| text-first + `barH short` | 1352 | 9332 | `0x2629c` |
| text-first + `top` before `barYOffset` | 1429 | 9332 | `0x2629c` |
| text-first + inline `filled` (either `*2.0f` or `x+x`) | 8001 | 9368 | `0x2629c` |
| text-first + `filled` declared before `SetColor` | 8005 | 9368 | `0x2629c` |
| text-first + `filled` between `SetColor`/`DrawImage2D` | 3734 | 9340 | `0x262a0` |
| text-first + cached `Globals::w >> 1` | 2010 | 9336 | `0x262a0` |
| `int barH` + `PaintCanvas *canvas` local | 1873 | 9328 | `0x26298` |
| `frameH`/`total`/`barW`/`barH` 256-combination sweep | best 1362 | 9332 | `0x26298` |
| text-first 256-combination sweep | best 1352 | 9332 | `0x2629c` (never exact) |
| `Status *st` local before damage block | 1362 | 9332 | `0x26298` |
| `Status *st` before gamma + damage + panel | 7979 | 9316 | `0x26298` |
| `Status *st` before gamma + damage only | 8656 | 9368 | `0x26298` |
| `textH` before `frameH` | 1419 | 9332 | `0x26298` |
| `gammaY` reassociation | 1377 | 9332 | `0x26298` |
| `gammaBarY` reassociation | 7042 | 9328 | `0x26298` |
| `barW` before dock1 `String text` | 3792 | 9344 | `0x26290` |
| dock1 `String text` after `barH` | 2236 | 9336 | `0x2628c` |
| dock2 `Mission *m` cache | 2005 | 9312 | `0x262a4` |
| dock2 `showIcon` bool hoist | 1942 | 9332 | `0x262a4` |

The 8 literal-pool bytes at `0x974`-`0x990` are GOT-relative addends
(`0xac0b0` vs `0xac0b4` etc.); the GOT entry order differs by one slot, which
is a link/symbol-order artifact, not a `Hud::draw` source issue.

---

## 6. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8617
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w8617`: `regions.py` (contiguous raw runs),
`side.py` (address-aligned side-by-side), `lsda.py`/`lsda2.py`/`lsda3.py`
(LSDA call-site-table decoder), `tryvar.py`/`sweep.py`/`sweep_tf.py`
(rebuild+measure sweeps), `Hud.base.cpp` (pristine wave-140 source),
`B.so` (landed wave-140), `T.so` (text-first), `I.so` (`int barH`).
