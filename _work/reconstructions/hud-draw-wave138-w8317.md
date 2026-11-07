# Hud::draw — wave 138 (w8317): full hunk census, and `top` is `int` (zero-extended)

**Status:** source change landed, 2026-11-07 (wave 138). Worktree `wt/w8317`,
base `master 96b069792`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, symbol parity 5370/5370.

Score is unchanged at **96.68307 % / 84 hunks / 3149 vs 3152 insns**, 9332 B.
Whole-`.text` differing bytes go **85,535 → 85,534** (−1). All section sizes
stay exact (`.text` 1,259,464; `.ARM.extab` 156,312; `.rodata/.data/.bss/
.ARM.exidx/.rel.plt` byte-identical).

---

## 0. TL;DR

1. This note is the first complete **census of all 84 hunks** by source region
   (section 1). The two docking blocks (`dock1` 32 + `dock2` 28) are the
   largest remaining clusters, not the gamma/damage region.
2. New hypothesis tested: the original's dock locals (`frameH`, `total`,
   `barW`, `top`) are all `int` (no `sxth`/`uxth`), and the current `short`
   spellings are allocator tie-breaks. A 108-combination type sweep confirms
   the landed `short/short/unsigned short/int` combination is the **global
   optimum** for hunk count (84). Every all-`int` combination is worse
   (best 88 hunks, 3147 insns, 9328 B).
3. **Landed:** `short top` → `int top` in dock1. The original loads
   `field_0x3e2` with `ldrh.w` (zero-extend), not `ldrsh.w`; `int top` is the
   faithful spelling and costs nothing (same allocation, same 84 hunks,
   −1 `.text` byte). The second block already used `int top`.
4. New evidence on the damage block (section 3): the original's two dead
   `ldr.w r8,[sp,#108]` loads are `Globals::status` reloads for the mission
   panel (`r8` is live from `0x16436e` to `0x164566`), and the original's
   `Globals::status &&` null test is **eliminated** (status was already
   dereferenced). Clang 7.0.2 does not eliminate it in our build, so the
   anchor costs 3 instructions and the dead loads are missing.

---

## 1. Complete hunk census (84)

Addresses are function-relative (VMA − 0x163b90). Every hunk is
size-neutral; the function is 9332 B on both sides.

| region | func offset | hunks | character |
|---|---|---:|---|
| regen `pivotY` trace | `0x464` | 1 | `strd r4,sl` vs `strd r4,r4` (wave-135 anchor; irreducible) |
| gamma body | `0x4c2`–`0x4ee` | 4 | `fp`/`r9` swap for `field_0x448`/`field_0x44a` |
| damage indicators | `0x5e6`–`0x7e0` | 16 | 2 dead `ldr.w r8,[sp,#108]`; `Globals::status &&` null test; `sl`/`fp` rotation |
| dock1 (docking transfer bar) | `0x1cd4`–`0x1f44` | 32 | `short`/`unsigned short` locals, `barW`/`barH` register swap, branch targets |
| dock2 (jump-drive/cloak bar) | `0x1f44`–`0x2200` | 28 | `getText`/`halfW` order, `top`/`barYOffset` scheduling, branch targets |
| EH cleanup pad | `0x242c`–`0x2440` | 3 | branch-target layout |

The branch-target-only hunks in dock1/dock2 (`beq.w 0x1fc0` vs `0x1fc2`,
`cbz 0x1f3c` vs `0x1f3e`, …) are all consequences of the +3 instruction
count (section 2); they disappear if the count is ever fixed.

---

## 2. Dock-local type sweep (new, exhaustive)

The original's dock1 block has **no** `sxth`/`uxth`:

```
1658b4: mov  r5,r0              ; total   (getDockTotalAmount, int)
1658e2: vmov s0,r4              ; transferred
1658ea: vmov s2,r5              ; (float) total       <-- no sxth
1658f8: vmov s4,r9              ; (float) textH
165908: vmov s2,r0              ; frameH from [sp,#96] <-- no sxth
165942: mov  r8,r0              ; barW                 <-- no uxth
1659ca: vmov s0,r8 / vcvt.f32.s32 ; (float) barW       <-- signed convert
1659ec: add.w r2,r8,r8,lsr#31 / asrs / sub ...         ; barW/2 signed divide
165c10: ldrh.w r6,[sl,#994]     ; top = field_0x3e2    <-- zero-extend
```

So the original source is `int frameH`, `int total`, `int barW`, `int top`.
The current tree spells them `short/short/unsigned short/short` because that
is what keeps the 224-byte frame and 9332 B. I swept all 108 combinations
(`frameH`×`total`×`barW`×`barH`×`top`) with the project flags:

| frameH | total | barW | barH | top | hunks | ratio | insns |
|---|---|---|---|---|---:|---:|---|
| **short** | **short** | **unsigned short** | **int** | **int** | **84** | **96.68307** | 3149/3152 |
| short | short | unsigned short | int | short | 84 | 96.68307 | 3149/3152 |
| short | short | unsigned short | int | unsigned short | 84 | 96.68307 | 3149/3152 |
| short | int | short | int | any | 87 | 96.54078 | 3149/3153 |
| int | short | short | int | any | 88 | 96.55610 | 3149/3152 |
| int | int | int | short | short | 88 | 96.53748 | 3149/3147 |
| int | int | int | int | any (all-int) | 128 | 94.36060 | 3149/3146 |

`top` is free (all three spellings give identical code once the field is
zero-extended); `frameH`/`total`/`barW` are load-bearing. The all-`int` form
is 4 B short (9328 B) — the missing 4 B is the `mov r5,fp` (2 B) Canvas
preservation plus a 2 B `mov` in the dock block, both allocator artefacts of
keeping `Canvas` in `fp` instead of `r6`, not a missing source statement.

### Landed change

```cpp
-            short top = this->field_0x3e2;
+            int top = this->field_0x3e2;
```

This matches the original's `ldrh.w` zero-extend and the identical `int top`
already used in the second (jump-drive) block. It changes the dock1 hunk body
from `ldrsh.w r4` to `ldrh.w r4` (the register still differs from the
original's `r6`), so the hunk count is unchanged but one `.text` byte is
recovered.

---

## 3. Damage block: where the dead `ldr.w r8,[sp,#108]` comes from

`sp+108` holds `Globals::status` (stored at `0x164022`, `str r6,[sp,#108]`,
where `r6 = Globals::status`). The original has two **dead** reloads:

```
164176: ldr.w r8,[sp,#108]   ; dead: r8 overwritten at 1641e4 (Globals::w)
164210: ldr.w r8,[sp,#108]   ; dead: r8 overwritten at 16436e (status again)
```

The *live* load is at `0x16436e` (`ldr r6,[sp,#108]; mov r8,r6`), and `r8` is
then used as `Globals::status` in the mission panel at `0x164558`/`0x164566`
(`ldr.w r0,[r8]`). So the original keeps `Globals::status` in `r8` across the
damage block and the mission panel; the two earlier loads are the scheduler
hoisting the same value into `r8` before each clobber.

The original's fourth damage condition is only `field_0x4b8 >= 1`
(`0x1642f2: ldr r0,[sl,#1208]; cmp r0,#1; blt`), i.e. the source's
`Globals::status && …` null test was **eliminated** (status is provably
non-null after the gamma call dereferences it). Clang 7.0.2 does not do this
in our build: ours emits `ldr.w r0,[r8]; cmp r0,#0; beq` (+3 instructions,
`0x75e`). Removing the anchor entirely collapses the frame (9268 B / 86.47 %),
so the anchor stays.

Tried and rejected in this wave (all on top of the landed `int top`):

| variant | hunks | ratio | insns |
|---|---:|---:|---|
| `Globals::status &&` added to block 1 (`0x4ac`) | 302 | 85.75 | 3149/3158 |
| … to blocks 1+2 | 546 | 76.08 | 3149/3160 |
| … to blocks 1+2+3 | 340 | 86.81 | 3149/3164 |
| … to block 2 / block 3 | 291 / 301 | 88.94 / 88.55 | 3149/3154 / 3155 |
| `Status *st = Globals::status;` before damage, `st &&` | 84 | 96.68307 | 3149/3152 |
| `Status *st` cached through mission panel | 590 | 64.20 | 3149/3141 |
| remove anchor (`field_0x4b8 >= 1` only) | 336 | 87.09 | 3149/3148 |

Caching the receiver does not make clang eliminate the test (it treats the
global pointer as possibly changed by the intervening calls), so the dead
loads cannot be reproduced from source without changing the whole liveness.

---

## 4. Dock1 / dock2 reshapes tried (all worse)

On top of the landed `int top`:

| variant | hunks | ratio | insns |
|---|---:|---:|---|
| dock2 `String text` before `halfW` (original order) | 120 | 94.84 | 3149/3150 |
| dock2 `barYOffset` after `y` | 87 | 96.37 | 3149/3152 |
| dock1 `int barH` before `unsigned short barW` | 114 | 94.76 | 3149/3151 |
| dock1 `PaintCanvas *canvas` local (barW only) | 88 | 96.24 | 3149/3152 |
| dock1 `PaintCanvas *canvas` local (barW+barH) | 166 | 93.60 | 3149/3152 |
| dock1+dock2 `PaintCanvas *dockCanvas` local | 684 | 53.80 | 3149/3126 |
| dock1 `PaintCanvas *dc` local | 202 | 89.94 | 3149/3146 |
| `barW` = `short` / `int` / `unsigned int` / `long` | 125 / 115 / 121 / 115 | 94.46 / 94.79 / 94.40 / 94.79 | — |

The `barW`/`barH` register swap (`barW` in `r8`, `barH` in `fp` in the
original; `barW` in `fp`, `barH` in `r8` in ours) and the `mov r5,fp` Canvas
preservation are pure allocator tie-breaks driven by the whole-function
register state; no local source reordering reproduces them while keeping
9332 B.

---

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8317
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

Scratch harness in `/dev/shm/w8317`: `hunks.py` (full hunk dump with
function-relative addresses), `hunklen.py` (per-hunk byte sizes),
`lenhunks.py`, `try.py` / `exp.py` (patch→incremental link→score),
`Hud.base.cpp` (pristine wave-137 source), `hunks_top.txt` (the 84-hunk
census above), `orig_full.asm` (original `Hud::draw` disassembly).
