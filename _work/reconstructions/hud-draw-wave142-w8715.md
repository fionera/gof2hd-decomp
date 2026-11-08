# Hud::draw — wave 142 (w8715): full contiguous-hunk census re-run and the 4-field LSDA decode

**Coordinator addendum (wave-142 merge):** the dock2 reshape is *not* landed: it splits the
LSDA call-site table, grows .ARM.extab 156312 -> 156316, shifts .rodata and drops byte_exact
4507 -> 4044. Keep the gate-green source (raw 1362); the reshape is a lead for a future pass
that also removes one LSDA entry.
**Status:** source change (text-first dock2 reshape) measured, 2026-11-08 (wave 142).
Worktree `wt/w8715`, base `master 1d10d0694` (wave 141). Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (VMA `0x163b90`).

Gate on entry: **RATCHET PASS**, `byte_exact 4507`, `linked_exact 4507`,
size-exact 4524/4524, `data_exact 843/843`, imports **207**, lint **CLEAN**,
drift **0**, parity 5370/5370.

Raw same-address diff (primary metric): **1362 -> 1352 B** (function stays
9332 B). The landed source change is the faithful dock2 `String text`-before-
`halfW` order. It is **EH-bound**: `.ARM.extab` grows `156312 -> 156316`
(`0x26298 -> 0x2629c`), which pushes `.rodata` from `0x1c9e10` to `0x1c9e20`
and drops `byte_exact` to **4044** (full `check`: `RATCHET FAIL`). The
coordinator should decide whether to take the 10-byte raw win and pay the
section-size delta. The base source (1362) is the gate-green fallback.

---

## 0. TL;DR

1. Re-ran the full contiguous same-address hunk census. The residual is
   unchanged from wave 141: damage **481**, dock1 **316**, dock2 **542**,
   literal pools 10, gamma 5, regen 1, EH 7 = **1362**.
2. The largest single contiguous run is still **119 B at func+0x2037**
   (`0x165bc7`), the dock2 `String text` / `halfW` order. The faithful
   text-first reshape gives **1352 raw** (10 B better) and keeps 9332 B, but
   `.ARM.extab` grows by 4 B.
3. Decoded the `.ARM.extab` LSDA with the correct **4-uleb** call-site entry
   format (`start, length, landing-pad, action`). The text-first build has
   **78 entries / 459 B** vs the original's **77 / 454 B**. The extra entry is
   the draw sequence split at the `filled` computation:
   `(0x20e4, 0x2a, 0x2440)` + `(0x2130, 0x7e, 0x2446)` vs the original's one
   `(0x20e0, 0xcc, 0x2444)`. The two sub-ranges have different landing pads
   (extra `b.n` stub at `0x165fd0`), so they cannot be merged by contiguity
   alone.
4. New negative results (all keep 9332 B unless noted): `filled` placement
   (before `SetColor` / before `DrawImage2D` / inline) is a codegen no-op
   (1352, extab +4); `const` locals are no-ops; `getText` pointer-temp is
   2048/9336 (extab exact); declaring `float filled;` before `SetColor`
   merges the EH regions (extab **156312 exact**) but explodes the frame to
   9368 B / 8001 raw; `volatile filled` is 8017/9388 (extab exact);
   `fadeColor` ternary is 1823/9332.

---

## 1. Contiguous same-address hunk census (base, 1362)

| region | func offset | raw diff |
|---|---|---:|
| regen `pivotY` trace | `0x467` | 1 |
| gamma body | `0x4c2`-`0x4f0` | 5 |
| damage indicators | `0x5e6`-`0x7e2` | 481 |
| literal pool (damage) | `0x974`-`0x992` | 8 |
| dock1 (docking transfer bar) | `0x1cd4`-`0x1f44` | 316 |
| dock2 (jump-drive/cloak bar) | `0x1f44`-`0x2200` | 542 |
| EH cleanup pad | `0x242c`-`0x2440` | 7 |
| literal pool (tail) | `0x2450`-`0x2474` | 2 |
| **total** | | **1362** |

Largest contiguous runs: 119 B `0x2037` (dock2 String/halfW), 75 B `0x1eaa`
(dock1 `(float)barH*2.5f` s32/u32), 65 B `0x696` (damage block-2 shift), 52 B
`0x1dbc` (dock1 `mov r5,fp`), 50 B `0x1fbb` (dock2 `top`/`barYOffset`), 48 B
`0x1f1e` (dock1 `DrawImage2D` tail), 48 B `0x62f` (damage block-1 shift),
46 B `0x7b4` (damage block-4 rotation).

The damage 481 is still the 4-byte shift from the two dead
`ldr.w r8,[sp,#108]` loads (8 B, absent in ours) and the extra
`Globals::status &&` null test (12 B, added in wave 117 as a size anchor).
Removing the anchor collapses the frame to 9272 B (7455 raw); moving it to
the 1st/2nd/3rd damage block is 6900-7300 raw. The anchor position is optimal.

## 2. The text-first reshape (landed source change)

```cpp
-            int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
-            String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
+            String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
+            int halfW = (int) ((float) Globals::Canvas->GetImage2DWidth(this->image_0x37c) * 0.5f);
```

The instruction stream now matches the original byte-for-byte through the
`String` ctor, `halfW` (`GetImage2DWidth * 0.5f`) and `barH`
(`GetImage2DHeight`) at `0x165ba4..0x165bf6` (orig `0x165ba6..0x165bf8`,
shifted by 2 because the dock1 `sxth` makes our dock1 4 B longer). Raw drops
1362 -> 1352; every other region is unchanged (damage 481, dock1 316, dock2
542 -> 485, EH 7 -> 5, tail 2 -> 6).

## 3. The 4-field ARM EHABI call-site table

The LSDA header is `ff ff 01 c6 03` (LPStart omit, TType omit, uleb table
length). Each call-site entry is **four** ulebs, not three:
`(start, length, landing-pad, action)`. Decoding orig gives exactly 77
entries / 454 B (matches wave-141's note). Text-first gives 78 / 459 B.

```
orig : 71: 2044   4 2434 0 | 72: 2064   4 2432 0 | 73: 20e0  cc 2444 0 | 74: 21ac  e2 0000 0
text : 71: 2048   4 2434 0 | 72: 2068   4 2432 0 | 73: 20e4  2a 2440 0 | 74: 2130  7e 2446 0 | 75: 21ae e0 0000 0
```

Encoding sizes: orig 73+74 = 7+6 = 13 B; text 73+74+75 = 6+6+6 = 18 B. The
+5 B is the split. Merging 73+74 would save 6 B (table 453 -> padded 456 ->
`.ARM.extab` 156312 exact). The split is caused by the `filled` computation
between `DrawImage2D` and `DrawRegion2D`: the first sub-range ends at
`0x210e` (right after `DrawImage2D`), the second starts at `0x2130` (mid
`filled`), and their landing pads differ (`0x2440` stub vs `0x2446` cleanup
body; ours has an extra `b.n` stub at `0x165fd0`). Moving `filled` before
either call is a codegen no-op (compiler re-sinks it).

## 4. Sweeps (all measured, base 1362 unless noted)

| variant | raw | size | `.ARM.extab` |
|---|---:|---:|---:|
| base (wave 141) | 1362 | 9332 | 156312 |
| **text-first (landed)** | **1352** | **9332** | **156316** |
| text-first + `filled` before `SetColor`/`DrawImage2D`/inline | 1352 | 9332 | 156316 |
| text-first + `const halfW`/`const barH`/`const barYOffset` | 1352 | 9332 | 156316 |
| text-first + dock2 `barH` short/unsigned short | 1352 | 9332 | 156316 |
| text-first + dock1 `int barH` | 1967 | 9324 | 156316 |
| text-first + `fadeColor` ternary | 1823 | 9332 | 156316 |
| text-first + `getText` pointer-temp | 2048 | 9336 | **156312** |
| text-first + `float filled;` before `SetColor` | 8001 | 9368 | **156312** |
| text-first + `volatile float filled` before `SetColor` | 8017 | 9388 | **156312** |
| text-first + `String` after `DrawImage2D` | 7624 | 9344 | 156300 |
| text-first + dock1 `text += " "` | 1777 | 9328 | 156324 |
| base + `transferred unsigned` / `total unsigned` | 1363 | 9332 | 156312 |
| base + `barOffset` frameH-first | 1362 | 9332 | 156312 |

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8715
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6   # text-first: FAIL byte 4507 -> 4044
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w8715`: `fdiff.py`, `measure.py`, `regiondiff.py`
(raw diff per region), `lsda4.py` (4-uleb call-site decoder), `side.py`
(address-aligned Thumb side-by-side), `rawruns.py` (contiguous raw runs),
`runvar.py`/`vrun.py` (rebuild+measure), `Hud.base.cpp` (pristine wave-141
source).
