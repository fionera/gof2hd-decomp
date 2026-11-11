# Radar::draw wave 181 (w12508) — music-tail `||` operand order (raw 6,379 -> 6,372)

**Status:** landed, 2026-11-11. Worktree `wt/w12508`, branch `wave181-w12508`,
base `master` `1e270aa0e` (wave-180 harvest). Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave180-w12407.md` / `-w12408.md` / `-w12409.md` (the
landed 6,379 shape this pass starts from). This note is self-contained: every
number was measured on `1e270aa0e` in this worktree.

## 0. Baseline and landed result

| metric | base | landed |
|---|---:|---:|
| function size | 9,908 | **9,908** |
| raw same-address diff | 6,379 | **6,372** (-7) |
| instruction-index alignment | 1,752 | **1,752** |
| instruction count | 3,543 ours / 3,532 orig | 3,542 / 3,532 |
| whole-`.so` `.text` differing bytes | 76,200 | **76,193** (-7) |
| `.ARM.exidx` differing bytes | 0 | 0 |
| `.ARM.extab` differing bytes | 713 (size/base exact at `0x1a3b78`, 156,312 B) | **713** (unchanged) |
| `.rel.plt` | byte-identical | byte-identical |
| `.rodata` / `.data` / `.dynsym` / `.dynstr` / `.hash` | 0 | 0 |
| `.note.gnu.build-id` differing bytes | 20 | 20 |
| total differing bytes | 76,933 | **76,926** |
| file size | 2,187,328 | 2,187,328 |

Gate after the change: byte_exact **4,507/4,524**, linked_exact
**4,507/4,524**, size-exact **4,524/4,524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift **0**, parity **5,370/5,370**. `RATCHET PASS`.

## 1. Landed change (two cells, net size 0)

```diff
@@ music tail, enemy-music stop guard (line ~1268)
-        if ((uint32_t) (current - 136) > 15 || ((1 << (current - 136)) & 0xe071) == 0 ||
-            __builtin_expect((current != 151 && alwaysEnemySeen), 0)) {  // lint: arm_builtin
+        if ((uint32_t) (current - 136) > 15 || __builtin_expect((current != 151 && alwaysEnemySeen), 0) ||  // lint: arm_builtin
+            ((1 << (current - 136)) & 0xe071) == 0) {
             Globals::sound->stop(current);
-            if (Globals::status->getCurrentCampaignMission() == 16 || Globals::status->getStation()->isAttackedByAliens() ||
-                Globals::status->inAlienOrbit()) {
+            if (Globals::status->inAlienOrbit() || Globals::status->getStation()->isAttackedByAliens() ||
+                Globals::status->getCurrentCampaignMission() == 16) {
                 Globals::sound->play(136, nullptr, nullptr, 0.0f);
```

Both are pure `||` operand permutations (the result of a short-circuit OR is
independent of operand order; the three operands of the second test are
`Status` queries with no observable side effects). The second cell restores the
original's **call order**: the original calls `inAlienOrbit` (12e0ec/1302ac),
`getStation()->isAttackedByAliens` (1302b8) and `getCurrentCampaignMission`
(1302c0) in exactly that order.

## 2. Why this moves bytes

The base source evaluated the stop guard as
`range || mask == 0 || expect(current != 151 && alwaysEnemySeen)`; clang
reordered the `expect`-weighted term ahead of the mask test and emitted the
`subs/it ne/movne; tst` sequence at `130290`-`13029a`. Moving the weighted term
after the range test but before the mask test (the landed shape) makes clang
keep the range test first and emit the mask test directly after it, matching
the original's block order at `130280`-`1302a4`:

```
orig  130280 sub.w r3,r1,#136 ; movs r2,#1 ; cmp r3,#15 ; strb ...
      13028e lsls r2,r3 ; movw r3,#0xe071 ; tst r2,r3 ; beq stop
      130298 cmp r1,#151 ; beq skip ; ldr alwaysEnemySeen ; eors ; bne skip
land  130282 movs r2,#1 ; subs.w r3,r1,#151 ; strb ...
      13028c sub.w r2,r1,#136 ; it ne ; movne r3,#1 ; cmp r2,#15 ; bhi stop
      130298 ldr alwaysEnemySeen ; ands r3,r6 ; bne stop ; lsls/ands mask
```

The second cell (call-order permutation) is what actually removes the last
bytes: the base called `getCurrentCampaignMission` first and only then
`inAlienOrbit`; the original calls `inAlienOrbit` first. The two cells are
nearly additive (`-7` total).

## 3. Search evidence (all size-exact unless noted)

Every measurement below is a full `link_so` + `tools/fn_diff.py` on this
worktree (`-j2`, `ulimit -v 12000000`).

### 3.1 Single `__builtin_expect` re-sweep (30 sites)

All 30 sites were removed and flipped. No single edit improves raw; the best
are raw-neutral at 6,379 (`R4`, `R12`, `F12`, `R15`, `F15`, `R22`). This
confirms the wave-180 note: the hint space is exhausted at 6,379.

### 3.2 Expect removal pairs (C(30,2) = 435)

The unique minimum is raw 6,379 (`{4,12}`, `{4,15}`, `{12,15}`, `{27,29}`);
no pair beats the base.

### 3.3 Music stop-guard operand permutations

Six permutations of `range || mask || expect`, on the base:

| order | raw | size |
|---|---:|---:|
| `range, mask, expect` (base) | 6,379 | 9,908 |
| `range, expect, mask` (landed cell 1) | **6,372** | 9,908 |
| `expect, range, mask` | 6,376 | 9,908 |
| `mask, range, expect` | 8,452 | **9,900** |
| `mask, expect, range` | 8,505 | **9,888** |
| `expect, mask, range` | 8,452 | **9,900** |

Only the two `range`-first orders are size-exact.

### 3.4 Stop-guard `campaign/isAttacked/inAlienOrbit` permutations

Six permutations of the three-way stop test, all on top of landed cell 1:

| order | raw | size |
|---|---:|---:|
| `campaign, isAttacked, inAlien` (base) | 6,379 | 9,908 |
| `campaign, inAlien, isAttacked` | 6,372 | 9,908 |
| `inAlien, isAttacked, campaign` (**landed, original call order**) | **6,372** | 9,908 |
| `inAlien, campaign, isAttacked` | 6,373 | 9,908 |
| `isAttacked, campaign, inAlien` | 6,357 | 9,908 |
| `isAttacked, inAlien, campaign` | 6,362 | 9,908 |

The landed `inAlien, isAttacked, campaign` is the order the original binary
actually calls (`1302ac inAlienOrbit`, `1302b8 isAttackedByAliens`,
`1302c0 getCurrentCampaignMission`). The two `isAttacked`-first permutations
score 15 / 10 bytes better but change the binary's call order; they are left as
alternatives below.

## 4. Alternatives measured but not landed

* `isAttackedByAliens() || getCurrentCampaignMission() == 16 || inAlienOrbit()`
  (with landed cell 1): raw **6,357**, size 9,908, `.ARM.extab` 713. 15 bytes
  better raw, but the compiled call order is `getStation/isAttacked`,
  `inAlienOrbit`, `campaign` — not the original's `inAlienOrbit`-first order.
* `isAttackedByAliens() || inAlienOrbit() || campaign == 16`: raw 6,362.
* Enemy-loop `update; wreck; field_0x75 = 0` (the original's call order,
  `12ed5e update` before `12ed66 isDead`): raw 6,381 alone (aligned 1,746),
  and 6,374 combined with the landed cells. Not landed (raw-neutral/worse).
* `int top` -> `float top` in the landmark crosshair block (w12407's remaining
  `top` cell): raw-neutral 6,379 alone, 6,372 combined. Not landed.

## 5. Reproduction

```sh
ulimit -v 12000000
cp /home/debian/GalaxyOnFire2/scratch/w12508/O1.cpp src/game/weapons/Radar.cpp
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6372 aligned= 1752 size=9908/9908 insn=3542/3532
cmake --build cmake-build-match --target check -j2    # RATCHET PASS
cmake --build cmake-build-match --target drift -j2    # 0 field(s)
```

Scratch: `/home/debian/GalaxyOnFire2/scratch/w12508/` — `Radar.base.cpp`
(base source), `O1.cpp` (landed source), `measure.sh` (build+measure),
`exp.py` / `pairsweep.py` (hint sweeps), `mkvar.py` + `v*.json` (variant
generators), `calls.py` (call-sequence diff), `secdiff.py` (section deltas),
`adiff.py` / `localdiff.py` / `hot.py` / `shift.py` (diff analysis).
