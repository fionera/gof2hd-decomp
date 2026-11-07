# Hud::draw — wave 137 (w8218): the free `dividerOffset` use is the regen `pivotY`, and nothing cheaper exists

**Status:** research note, 2026-11-07 (wave 137). Worktree `wt/w8218`, base
`master 655e3bcc5`. **No source change landed**: the wave-135 pivotY anchor is
still the best measured variant. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (VMA `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size-exact 4524/4524, `data_exact 843/843`, imports
**207**, lint **CLEAN**, drift **0**, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`.

Measured baseline (exact, difflib on the normalised instruction stream):
**96.68307 % / 84 hunks / 3149 vs 3152 insns**, 9332 B.

---

## 0. TL;DR

1. The wave-135 `dividerOffset`-as-regen-`pivotY` anchor is present and is
   still the best variant. It is **free** (0 bytes: `strd r4, sl` replaces
   `strd r4, r4`, same 4 bytes) and is the only source construct found that
   reproduces the original's shield/armour/regen/gamma register allocation.
2. This wave swept every other candidate free/cheap use. **None keeps 9332 B
   and the allocation.** The pivotY use is the unique cheapest lever.
3. The one visible trace is the regen pivot store: ours `strd r4, sl, [sp,#12]`
   vs the original `strd r4, r4, [sp,#12]` (`0x163ff4`). It is the first of
   the 84 hunks. It is irreducible from source: the original's regen block
   has **no** `dividerOffset` use, so the reload at `0x163fb2` is a pure
   allocator artifact that the pivotY anchor reproduces at the cost of that
   one instruction.
4. New negative results (all measured on top of the landed pivotY anchor or
   replacing it, as noted): `srcX`/`srcY = dividerOffset` (9364 B / 69.96 %),
   armour-fill pivotY (9328 B / 73.71 %), shield-fill pivotY (9324 B /
   75.32 %), gamma-fill pivotY (9324 B / 75.45 %), angle (9344 B / 88.50 %),
   pivotX (9336 B / 88.62 %), pivotX+pivotY (9352 B / 86.72 %), volatile copy
   (9288 B / 76.04 %), gamma-condition use (9360 B / 83.83 %), `short`
   `dividerOffset`/`armorBarY` (96.65 % / 85 hunks), `long long`/`unsigned`
   (no-op), aliases (no-op), declaration/statement reordering (worse),
   float-association changes (worse), and every non-folding zero expression
   (folds back to the 9312 B / 75.17 % plain build).

---

## 1. Mechanism (why pivotY is the only free slot)

`PaintCanvas::DrawRegion2D(index, srcX, srcY, width, height, angle, pivotX,
pivotY, posX, posY)` lays its stack arguments out as
`[sp+0]=width, [sp+4]=height, [sp+8]=angle, [sp+12]=pivotX, [sp+16]=pivotY,
[sp+20]=posX, [sp+24]=posY`.

The original regen call (`0x163fb8`–`0x164004`) is byte-identical to ours
**except** the pivot store:

```
163ff0: e9cd 2401  strd r2, r4, [sp, #4]    ; height, angle=0
163ff4: e9cd 4403  strd r4, r4, [sp, #12]   ; pivotX=0, pivotY=0   <-- original
163ff4: e9cd 4a03  strd r4, sl, [sp, #12]   ; pivotX=0, pivotY=dividerOffset  <-- ours
163ff8: 2200       movs r2, #0              ; srcX=0
163ffa: e9cd 3605  strd r3, r6, [sp, #20]   ; posX, posY
163ffe: 2300       movs r3, #0              ; srcY=0
164000: ed8d 0a00  vstr s0, [sp]            ; width
```

`pivotY` is the only argument that is (a) after the `DrawImage2D` function
pointer load at `0x163f2e`, (b) a free replacement for an existing zero, and
(c) not consumed by the callee when `angle == 0`. Replacing it with
`dividerOffset` makes the value live-in to the regen block (`bb.35`), so the
greedy allocator hoists the spill reload from the gamma body to `bb.34`
(`0x163fb2`, `ldr.w sl,[sp,#96]`) and spills `this` at `0x163e04`
(`str r5,[sp,#104]`). That is exactly the original's allocation and 224-byte
frame.

The entire region `0x163e0c`–`0x163fb8` (shield block, armour block, armour
fill, `hasArmorRegen` branch and reload) is **byte-identical** between the
original and the pivotY build. The only differences in the shield/armour/
regen/gamma region are the one `strd` and the gamma-body `fp`/`r9` swap.

---

## 2. New sweep (wave 137)

All variants were built with the project's exact flags and scored with
`tools/verify`'s disassembler + `difflib` (autojunk off). `size` is ours;
`raw %`/`hunks` are against the 9332 B original. The baseline is the landed
pivotY anchor.

### 2.1 Other free/cheap slots in the regen call

| variant | size | raw % | hunks | note |
|---|---:|---:|---:|---|
| **regen `pivotY = dividerOffset` (landed)** | **9332** | **96.68307** | **84** | free, correct allocation |
| regen `srcX = dividerOffset` | 9364 | 69.96343 | 596 | allocation collapses |
| regen `srcY = dividerOffset` | 9364 | 69.96343 | 596 | same |
| regen `angle = (float) dividerOffset` | 9344 | 88.50119 | 308 | conversion + move |
| regen `pivotX = dividerOffset` | 9336 | 88.62446 | 308 | one extra store |
| regen `pivotX = pivotY = dividerOffset` | 9352 | 86.72482 | 346 | two extra stores |
| regen `pivotY = armorBarY + dividerOffset` (wave 134) | 9336 | 88.3 | — | the add this wave replaces |

### 2.2 Uses in the other draw blocks (regen anchor removed)

| variant | size | raw % | hunks |
|---|---:|---:|---:|
| armour-fill `pivotY = dividerOffset` | 9328 | 73.71429 | 522 |
| shield-fill `pivotY = dividerOffset` | 9324 | 75.32138 | 548 |
| gamma-fill `pivotY = dividerOffset` | 9324 | 75.45252 | 548 |
| gamma-condition `dividerOffset >= 0 &&` | 9360 | 83.83006 | 408 |

Only the regen block (`bb.35`) extends the interval across `0x163f2e` with the
right pressure; the armour/shield/gamma uses fall back to the plain 9312 B /
75.17 % allocation.

### 2.3 Types, aliases, order, association

| variant | size | raw % | hunks |
|---|---:|---:|---:|
| `unsigned int dividerOffset` | 9332 | 96.68307 | 84 |
| `long dividerOffset` | 9332 | 96.68307 | 84 |
| `long long dividerOffset` | 9332 | 96.68307 | 84 |
| `short dividerOffset` | 9332 | 87.87302 | 317 |
| `unsigned short dividerOffset` | 9332 | 96.65133 | 85 |
| `short armorBarY` | 9332 | 96.65133 | 85 |
| `long armorBarY` / `unsigned int armorY` | 9332 | 96.68307 | 84 |
| `dividerOffset2 = dividerOffset` alias (gamma / armour use) | 9332 | 96.68307 | 84 |
| `armorBarYCopy = armorBarY` alias | 9332 | 96.68307 | 84 |
| declaration order `armorBarY` first | 9332 | 96.68307 | 84 |
| `SetColor` before `dividerOffset` load | 9332 | 96.42914 | 88 |
| `dividerOffset` declared after `armorBarY` | 9332 | 96.42914 | 88 |
| `hullW` association `rate * (0.01f * field)` | 9332 | 96.46088 | 88 |
| `regenW` association `rate * (0.01f * field)` | 9332 | 96.52436 | 88 |
| `gammaBarY`/`gammaY` declaration swap | 9332 | 96.68307 | 84 |
| `gammaY`/`gammaBarY` `a + (a - b)` form | 9332 | 96.68307 | 84 |
| `hasArmorRegen != 0` | 9332 | 96.68307 | 84 |

### 2.4 Zero-valued expressions in the pivotY slot

Every one folds before register allocation and falls back to the **9312 B /
75.17077 % / 538 hunk** plain build:

`dividerOffset & (dividerOffset - dividerOffset)`,
`dividerOffset * (dividerOffset - dividerOffset)`,
`dividerOffset & (dividerOffset ^ dividerOffset)`,
`dividerOffset - (dividerOffset - 0)`,
`(dividerOffset == 0) ? 0 : 0`,
`(dividerOffset > 0) ? 0 : 0`.

`dividerOffset % (dividerOffset + 1)` does not fold, but costs a division:
9356 B / 70.44 %. A volatile copy (`*(volatile int *)&dividerOffset`) forces
the local to memory: 9288 B / 76.04 %. `__builtin_assume(dividerOffset == 0)`
is eliminated as a hint, so it also falls back to 9312 B / 75.17 %.

---

## 3. Why the trace cannot be removed from source

The original's regen block has no `dividerOffset` use: every instruction in
`0x163fb8`–`0x164004` is accounted for by the call arguments and the
`regenW` float math. The original's `dividerOffset` is live-in to `bb.35`
only because it is live-in to the gamma body (`bb.37`), and the allocator
chose to hoist the reload to `bb.34`. Our plain build has the same liveness
but the allocator reloads inside the gamma body instead. The pivotY anchor
forces the hoist by adding a real use in `bb.35`; any such use must be
materialised, and the only free slot is `pivotY`, whose store then differs
from the original's zero. The `strd r4, sl` vs `strd r4, r4` hunk is
therefore the irreducible price of the correct allocation.

The remaining 83 hunks are independent of this lever: the gamma-body
`fp`/`r9` swap (4 hunks), the damage-indicator dead `ldr.w r8,[sp,#108]`
reloads and `sl` rotation (~20), and the mission-panel / jump-drive-bar
register allocation (~50), plus the EH pad (3).

---

## 4. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w8218
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
python3 tools/verify/verify.py --build-dir cmake-build-match/verify \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w8218`: `score.py` (exact ratio + hunk count,
`-v` for hunk bodies), `try.sh` (patch → incremental link → score → restore),
`v_*.py` (one variant per file), `mi.sh` (`-mllvm -print-machineinstrs`
filtered to `Hud::draw`), `mi_pivot.txt` / `mi_plain.txt` (greedy-allocator
dumps), `orig_region.txt` / `ours_region.txt` (objdump of
`0x163d80`–`0x1640a0`), `Hud.base.cpp` (pristine wave-136 source).
