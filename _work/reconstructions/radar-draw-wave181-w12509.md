# Radar::draw wave 181 (w12509) — music-tail short-circuit reorder (raw 6379 -> 6356)

**Status:** landed, 2026-11-11. Worktree `wt/w12509`, branch `wave181-w12509`,
base `master` `1e270aa0e` (wave-180 harvest). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave180-w12407.md` (crosshair float cells), `-w12408.md`
(music-tail if/else + `alwaysEnemySeen` expect removal) and `-w12409.md`
(two branch hints dropped). This note is self-contained; every number below was
measured on `1e270aa0e` in this worktree.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,379 | **6,356** (-23) |
| instruction-index alignment | 1,752 | 1,760 (+8) |
| instruction count | 3,543 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 76,200 | **76,177** (-23) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 713 (size/base exact at `0x1a3b78`, 156,312 B) | **713** (unchanged) |
| `.rel.plt` | byte-identical | **byte-identical** |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| total differing bytes | 76,933 | **76,910** |
| file size | 2,187,328 | 2,187,328 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (three `||`-operand reorders, net size 0)

All three are pure reorderings of side-effect-free `||` operands. The
short-circuit *set* is unchanged (the operands are pure), and in the music-tail
case the shift is still guarded by the first operand (see 1.1).

```diff
@@ music tail, `field_0x1bc >= 1` stop predicate (line ~1268)
-        if ((uint32_t) (current - 136) > 15 || ((1 << (current - 136)) & 0xe071) == 0 ||
-            __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
+        if ((uint32_t) (current - 136) > 15 ||
+            __builtin_expect((current != 151 && alwaysEnemySeen), 0) ||  // lint: arm_builtin
+            ((1 << (current - 136)) & 0xe071) == 0) {

@@ music tail, `play(136)` guard (line ~1271)
-            if (Globals::status->getCurrentCampaignMission() == 16 || Globals::status->getStation()->isAttackedByAliens() ||
-                Globals::status->inAlienOrbit()) {
+            if (Globals::status->getStation()->isAttackedByAliens() ||
+                Globals::status->getCurrentCampaignMission() == 16 || Globals::status->inAlienOrbit()) {

@@ ambient music, `play(145)` guard (line ~1317)
-                if ((Globals::status->inAlienOrbit()) || Globals::status->getStation()->isAttackedByAliens()) {  // lint: arm_builtin
+                if (Globals::status->getStation()->isAttackedByAliens() || (Globals::status->inAlienOrbit())) {  // lint: arm_builtin
```

### 1.1 Safety of the shift reorder

The first operand `(uint32_t) (current - 136) > 15` is false only when
`current - 136` is in `[0, 15]`, so when the shift `1 << (current - 136)` is
reached (first false, second false) the shift count is in range. When the
first operand is true the whole `||` short-circuits. No UB is introduced.

## 2. Attribution (all size 9,908)

| variant | raw | aligned |
|---|---:|---:|
| base (wave-180 harvest) | 6,379 | 1,752 |
| `A` music stop predicate, swap operands 2 and 3 | **6,372** | 1,754 |
| `B` `play(136)` guard, swap operands 1 and 2 | 6,391 | 1,752 |
| `C` ambient `play(145)` guard, swap operands | 6,378 | 1,755 |
| `A + C` | 6,371 | 1,757 |
| **`A + B + C` (landed)** | **6,356** | 1,760 |

`B` alone is raw-worse, but it is strongly synergistic with `A`/`C`
(`A+C` 6,371 -> `A+B+C` 6,356). All three cells are size-exact and leave
`.ARM.extab` untouched.

## 3. Search performed (all variants full `link_so` + `fn_diff`, `-j2`,
`ulimit -v 12000000`)

Every edit is applied to the wave-180 base source; each variant is a fresh
build. Scratch: `/home/debian/GalaxyOnFire2/scratch/w12509/`.

### 3.1 Exhaustive adjacent-operand swap of every uniform `&&`/`||` chain

72 top-level conditions (uniform operator) in `Radar::draw`, 132 adjacent
swaps. The only raw winners are the three landed cells (`A` 6,372, `C` 6,378;
`B` is a synergy cell). Everything else is >= 6,379 or size-fatal.

### 3.2 Greedy second pass from the 6,356 shape

* All 132 adjacent swaps re-applied on top of `A+B+C`: no improvement
  (best neutral 6,356).
* 213 reversal / move-to-front / move-to-back variants of the 31
  three-or-more-operand conditions: no improvement (best neutral 6,356).
* All 60 single `__builtin_expect` remove/flip variants on top of `A+B+C`:
  no improvement (best neutral 6,356; `rm0` 6,357).

So 6,356 is a local optimum in the condition/hint space.

### 3.3 Other cells re-confirmed neutral/worse on this base

* `int missionBlocksPlanets` else-init (two forms): identical bytes (6,379).
* `plasmaInRange`/`lockConfirmed` store spellings (`false`, chained,
  reversed, `lockConfirmed = plasmaInRange`): identical bytes.
* `int missionBlocksPlanets = 0` / `= false ? 1 : 0`: identical bytes.
* `campaign` declaration moves (before `inAlienOrbit`, after the array
  assignments): 6,386 / 6,456 (and size-fatal variants).
* `bossSeen = shipGroup == 10 ? true : false` / `!= 0`: identical bytes.
* gas blip `Sprite *blip` / `KIPlayer *candidate` alias removal: 6,482 / 6,391.
* tractor condition reorders: 6,381-6,385 (one size-fatal 9,912).
* landmark crosshair condition order (original asm order, int/float forms):
  6,384-6,385.

## 4. Remaining residual

Unchanged from wave 174/179/180: the original keeps `campaign` in `sl`
(`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`) and later reuses `sl` for the
stations array (`12e3c6 mov sl,r0`, `12e4be ldr.w r0,[sl,#4]`), with the
planet-loop index in `fp`. Ours spills `campaign` to `sp+0x90`, keeps
`stations` at `sp+0x84` and uses `sl` for the loop index. The wave-174
greedy-RA analysis (`%2481` zero/`missionBlocksPlanets` evicting `%415`
`campaign` from `$r10`) still applies; every campaign-in-`sl` source shape
measured in waves 174-181 is size-short (9,816-9,872 B), so the allocation
cannot be fixed without the ~28-39 instructions of code the original carries
elsewhere. The music-tail block order (ours places the `field_0x1bc = 0`
common tail before the `else` branch) and the `marked` stack normalization
(`12e6c2`) are the other visible residuals.

## 5. Reproduction

```sh
ulimit -v 12000000
cd /home/debian/GalaxyOnFire2/wt/w12509
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6356 aligned= 1760 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check      # RATCHET PASS
cmake --build cmake-build-match --target drift      # 0 field(s) across 0 class(es)
```

Section-level diff (scratch helper `secdiff.py`, ELF parser):

```sh
python3 /home/debian/GalaxyOnFire2/scratch/w12509/secdiff.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# .text 76,177 ; .ARM.extab 713 ; .note.gnu.build-id 20 ; .rel.plt identical
```

## 6. Scratch inventory (`/home/debian/GalaxyOnFire2/scratch/w12509`)

* `Radar.base.cpp` — exact base source (HEAD `Radar.cpp`).
* `Radar.final.cpp` — landed source.
* `measure.sh <cpp>` — copy to `src/`, `link_so -j2`, `fn_diff`.
* `sweep.py <spec.json>` — batch build+measure driver.
* `condgen.py` / `cond.json` — 132 adjacent-swap variants.
* `revgen.py` / `revbig.json` — 213 reversal/move variants.
* `greedy.py` / `greedy2.py` / `greedy3.py` — second-pass sweeps from 6,356.
* `secdiff.py` — per-section differing-byte report.
* `sbs.py`, `dump.py`, `calls.py`, `segcount.py`, `multiset.py` — disassembly
  diff helpers.
