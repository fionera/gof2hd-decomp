# Radar::draw wave 191b (w13647) — target-bracket / blip-geometry order divergences, no size-exact raw win

**Status:** complete, 2026-11-13. Worktree `wt/w13647`, branch `wave191b-w13647`,
base `master` `4b2b1c10f`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.
Read with `radar-draw-wave191b-w13644.md` (blip-draw / store-order blocks),
`-w13636.md` (lock-image order landed), `-w13632.md` (four structural divergences),
`-w13614.md` (RA flip family), `-w13606.md` (pre-RA IR stability).
**No source change landed**: `src/` is untouched; every measured cell is raw-neutral
or raw-worse, size-exact unless noted.

## 0. Baseline (verified on this worktree)

| metric | value |
|---|---:|
| raw same-address diff | 6,013 |
| instruction-index alignment | 1,961 |
| function size | 9,908 / 9,908 |
| instructions | 3,541 ours / 3,532 orig |
| `.text` differing bytes | 71,197 |
| `.ARM.extab` differing bytes | 577 |
| `.ARM.exidx` / `.rodata` differing bytes | 0 / 0 |
| zero / `missionBlocksPlanets` reg | `sl` (r10) |
| `campaign` | spilled `sp+0x90` |
| original zero / campaign | `fp` (r11) / `sl` (r10) |

Build: `cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs`
then `cmake --build cmake-build-match --target link_so -j2` inside
`ulimit -v 12000000`.  `check` is green on the untouched base:
byte_exact 4,508 / linked_exact 4,508 / size 4,524 / data 843 / imports 207 /
lint CLEAN / drift 0 / parity 5,370.

## 1. New evidence: three genuine source-order divergences in the target-bracket / blip-geometry blocks

This pass reconstructed the planet target bracket, the landmark target bracket and the
asteroid-slot blip geometry from the Thumb disassembly.  Three places differ from the
current source.  Each is a real source-order error (not a compiler artefact), each is
size-exact alone, and each is raw-worse on this base.

### 1.1 Planet target bracket: X-first, not Y-first (`P`)

Source (`Radar.cpp:600`) is Y-first:

```cpp
if (this->screenY > top && top + size > this->screenY && this->screenX < left + size &&
    this->screenX > left) {
```

The original (`0x12e4f2`-`0x12e55a`) is X-first, in the order
`screenX > left`, `screenX < left+size`, `screenY > top`, `screenY < top+size`:

```
12e538 vmov r3,s2          ; left
12e53c cmp  r2,r3          ; screenX vs left
12e53e ble  12e4ec         ; screenX > left
12e540 add  r3,r1          ; left + size
12e542 cmp  r2,r3
12e544 bge  12e4ec         ; screenX < left+size
12e546 vmov r2,s0          ; top
12e54a ldr  r3,[r4,#256]   ; screenY
12e552 cmp  r3,r2
12e554 ble  12e4ec         ; screenY > top
12e556 add  r1,r2          ; top + size
12e558 cmp  r3,r1
12e55a bge  12e4ec         ; screenY < top+size
```

Fix: reorder to `screenX > left && screenX < left + size && screenY > top && screenY < top + size`.
With `P` the block becomes instruction-for-instruction identical to the original
(modulo the RA registers/spill slots).  Size-exact (9,908), insn 3,541 -> 3,540,
raw **6,019** (+6), aligned 2,000 (+39), `.text` 71,203 (+6), `.ARM.extab` 577.

### 1.2 Landmark target bracket: original starts with `screenX > left` (`L`)

Source (`Radar.cpp:441-443`) is:

```cpp
bool topOk = top < this->screenY;
if (this->screenY < top + size + size && topOk && (int) left + size + size > this->screenX &&
    (int) left < this->screenX) {
```

The original (`0x12e86a`-`0x12e8b2`) evaluates, in order:
`screenX > left`, `screenY < top + 2*size`, `screenY > top`, `screenX < left + 2*size`:

```
12e894 vmov r3,s2          ; left
12e898 cmp  r0,r3          ; screenX vs left
12e89a ble  12e8d0         ; screenX > left
12e89c vmov r6,s0          ; top
12e8a0 add.w r5,r6,r2,lsl #1   ; top + 2*size
12e8a4 cmp  r1,r5          ; screenY vs top+2size
12e8a6 bge  12e8d0         ; screenY < top+2size
12e8a8 cmp  r1,r6          ; screenY vs top
12e8aa ble  12e8d0         ; screenY > top
12e8ac add.w r1,r3,r2,lsl #1   ; left + 2*size
12e8b0 cmp  r0,r1          ; screenX vs left+2size
12e8b2 bge  12e8d0         ; screenX < left+2size
```

Fix: `screenX > (int) left && screenY < top + size + size && topOk && (int) left + size + size > screenX`.
Size-exact, insn 3,541 -> 3,540, raw **6,020** (+7), aligned 1,994 (+33),
`.text` 71,204 (+7), `.ARM.extab` 577.

### 1.3 Asteroid-slot blip geometry: `slotCount <= 3` before `field_0x1b0` (`A`)

Source (`Radar.cpp:1127-1129`) is:

```cpp
if (this->screenX > left && this->screenX < left + size * 2 && this->screenY > top &&
    this->screenY < top + size * 2 && !asteroid->asteroidFlag && this->field_0x1b0 &&
    slotCount <= 3) {
```

The original (`0x12fbb4`-`0x12fbc8`) tests `!asteroidFlag`, then `slotCount <= 3`, then
`field_0x1b0`:

```
12fbb4 ldrb r0,[r6,#292]   ; asteroid->asteroidFlag
12fbb8 cbnz r0,12fbd6      ; !asteroidFlag
12fbbE ldr  r0,[sp,#128]   ; slotCount
12fbc0 cmp  r0,#3
12fbc2 bgt  12fbda         ; slotCount <= 3
12fbc4 ldrb r0,[r4,#432]   ; field_0x1b0
12fbc8 cbz  r0,12fbda      ; field_0x1b0
12fbca ldr  r0,[r4,#88]    ; asteroidSlots
12fbcc ldr  r1,[sp,#128]
12fbce str.w r9,[r0,r1,lsl #2]
```

Fix: swap `this->field_0x1b0` and `slotCount <= 3`.  Size-exact, insn 3,541 (unchanged),
raw **6,029** (+16), aligned 1,966 (+5), `.text` 71,213 (+16), `.ARM.extab` 577.

## 2. Cells measured this pass (all reverted; base 6,013 / 1,961 / 9,908)

| cell | raw | aligned | size | insn | `.text` | `.ARM.extab` |
|---|---:|---:|---:|---:|---:|---:|
| base | **6,013** | **1,961** | 9,908 | 3541 | 71,197 | 577 |
| `P` (planet bracket X-first) | 6,019 | 2,000 | 9,908 | 3540 | 71,203 | 577 |
| `L` (landmark bracket X-first) | 6,020 | 1,994 | 9,908 | 3540 | 71,204 | 577 |
| `A` (asteroid slotCount order) | 6,029 | 1,966 | 9,908 | 3541 | 71,213 | 577 |
| `P+L` | 6,026 | 2,018 | 9,908 | 3539 | 71,210 | 577 |
| `P+L+A` | 6,042 | 2,021 | 9,908 | 3539 | 71,226 | 577 |
| `P+L+A+os` | 6,043 | 2,017 | 9,908 | 3539 | 71,227 | 577 |
| `T7+T8` (gas `targetPos` up + enemy tractor store swap; w13644 1.7a+1.8) | 6,204 | 1,985 | 9,908 | 3541 | 71,388 | 577 |
| `T7+T8+P+L+A` | 6,233 | 2,038 | 9,908 | 3539 | 71,417 | 577 |

`os` = w13632 §1.1 planet-loop `onScreen`-first.  No cell reduces raw below 6,013;
the best structural-alignment cell is base itself (1,961).  `.ARM.extab` is 577 for
every size-exact cell.

## 3. Why no raw win: the first cumulative address shift is the RA, not the block order

The per-`0x100` equal-byte profile is flat only up to `0x12e350` (9 / 1 / 1 differing
bytes for `0x12e050`/`0x12e150`/`0x12e250`), then jumps to 148 at `0x12e350` and
~240-250 from `0x12e450` to `0x12ec50`.  The streams resync for a whole window
(`0x12f750`-`0x12fc50` has 0-4 differing bytes), then shift again.

The first cumulative shift is at `0x12e3d4` (ours 4 bytes shorter), at the
`stations`/`planetHit` materialisation before the planet loop header:

```
orig  12e3c6 mov sl,r0            ours  12e3c6 str r0,[sp,#132]
      12e3c8 mov.w fp,#0                12e3c8 mov.w sl,#0
      12e3cc movs r0,#0                 12e3cc movs r0,#0
      12e3ce str.w sl,[sp,#116]         12e3ce str r0,[sp,#144]
      12e3d2 str r0,[sp,#152]           12e3d0 ldr.w r0,[r4,#320]
      12e3d4 ldr.w r0,[r4,#320]   <-- loop header
```

The original keeps `stations` in `sl` (4-byte `str.w sl`) and spills it to `sp+0x74`;
ours spills `stations` straight from `r0` (2-byte `str r0`) and uses `sl` for the
`planetHit` zero.  This is the same `campaign`-in-`sl` vs `campaign`-spilled RA swap
described in w13606/w13614: once `campaign` dies at `0x12e38a`, the original reuses
`sl` for `stations`; ours cannot, so the whole planet/landmark/enemy loop region is
shifted by 4 bytes.  Every block-level source fix above changes only local bytes and
cannot recover that shift.

### 3.1 RA-flip probes (diagnostic; all size-fatal)

`turretMode`/`alwaysEnemySeen`/`otherLock` type spellings were re-probed to see whether
the zero/`missionBlocksPlanets` colouring can be flipped to the original's `fp` without
changing the loop:

| cell | raw | aligned | size | insn | zero | campaign |
|---|---:|---:|---:|---:|---|---|
| base | **6,013** | **1,961** | 9,908 | 3541 | sl | spill |
| `int turretMode` (also `unsigned`/`char`/`int8_t`/`uint8_t`) | 9,226 | 2,447 | **9,860** | 3511 | **fp** | **sl** |
| `int alwaysEnemySeen` | 9,191 | 2,698 | **9,952** | 3537 | **fp** | sl |
| `bool otherLock` | 9,249 | 2,414 | **9,860** | 3511 | **fp** | sl |
| `int turretMode` + `E1` (campaign block no `expect`) | 9,264 | 2,488 | **9,932** | 3532 | **fp** | sl |

`int turretMode` gives the original's full colouring (`zero=fp`, `campaign=sl`) and the
original's instruction count is approached, but the frame grows from `0x128` to `0x130`
(an extra `str r6,[sp,#0xa4]` spill before the campaign call) and the function shrinks
by 48 bytes.  No size-exact spelling was found.  The original's `turretMode` is a
`bool` (`isInTurretMode` returns `bool`); the non-bool spelling is therefore not
faithful even though it flips the allocator.

## 4. Conclusion

Three more genuine source-order divergences are confirmed and documented above
(planet target-bracket X-first, landmark target-bracket X-first, asteroid-slot
`slotCount`/`field_0x1b0` order).  All are size-exact but raw-worse on this base.
The residual is still the single greedy-RA colouring: `campaign` is spilled in ours
and lives in `sl` in the original, which shifts the whole planet/landmark/enemy-loop
region by 4 bytes starting at `0x12e3d4`.  No size-exact raw win was found; `src/` is
untouched and the worktree is clean.

## 5. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6013 aligned= 1961 size=9908/9908 insn=3541/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13647/`: `cellfd.py` / `final.py`
(compile+link+strip+`fn_diff`(+section-diff) cell harness), `Radar.base.cpp` (pristine
source), `adiff2.py` / `callblocks.py` (address- and call-block-aligned diffs),
`profile.py` (per-`0x100` equal-byte profile), `mergewalk.py` / `firstshift.py`
(first cumulative address shift), `zsweep.py` (zero-register/RA sweep), `secdiff.py`,
`orig.dis` / `ours.dis`.
