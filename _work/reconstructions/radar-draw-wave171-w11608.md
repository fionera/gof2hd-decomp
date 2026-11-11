# Radar::draw wave 171 (w11608) — gas-loop index scope, allocation re-probe

**Status:** one small source change landed, 2026-11-11. Worktree `wt/w11608`,
branch `wave169-w11608`, base `master` `50716a763`. Target
`src/game/weapons/Radar.cpp`, `_ZN5Radar4drawEP6PlayerP3Hudi`
(`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave169-w11508.md` (campaign/`sl` lever) and
`radar-draw-wave170-w11507.md` (condition-chain expects, raw 7,095).

## 0. Baseline (rebuilt on this base)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,095 |
| instruction-index alignment | 1,595 |
| instructions | 3,537 ours / 3,532 orig |
| `.text` differing bytes | 81,825 |
| `.ARM.extab` differing bytes | 711 |
| `.rel.plt` | byte-identical (0) |
| gate | RATCHET PASS (byte 4507, linked 4507, size 4524, data 843, imports 207, lint CLEAN) |

## 1. Landed: gas-cloud loop index declared in the `for`

The gas-cloud loop was written with the index hoisted out of the loop:

```cpp
uint32_t i = 0;
bool gasHit = false;
for (; i < this->gasClouds->size(); ++i) {
```

The original's loop index is a normal scoped `for` induction variable. Moving it
in (`for (uint32_t i = 0; ...)`) reorders the two zero-materialisations at the
loop preheader (`movs r1,#0` / `movs r5,#0` and the two stores) and saves one
raw byte:

| variant | raw | aligned | size | extab |
|---|---:|---:|---:|---:|
| entry base | 7,095 | 1,595 | 9,908 | 711 |
| **gas `i` inside `for` (LANDED)** | **7,094** | 1,598 | 9,908 | 711 |
| gas `i` as `size_t` inside `for` | 7,094 | 1,598 | 9,908 | 711 |
| gas `i` as `int` inside `for` | 7,095 | 1,598 | 9,908 | 711 |

Exit: raw **7,094**, aligned 1,598, size 9,908, `.text` **81,824**,
`.ARM.extab` 711, `.rel.plt` 0. `check` = RATCHET PASS.

The aligned count worsens by 3 but raw (the byte-identicality ground truth)
improves; the instruction streams are the same modulo the preheader ordering.

## 2. Rejected variants (all size 9,908 unless noted)

### Campaign / `missionBlocksPlanets` allocation (the dominant residual)

The whole-function register assignment is still a cyclic permutation of the
original: ours `{missionBlocksPlanets, planet i} = sl`, `campaign = sp+144`,
`locked = r8`, gas `i = fp`; original `{missionBlocksPlanets, planet i} = fp`,
`campaign = sl`, `locked = sp+88`, gas `i = sp+152`. The original gives a CSR to
the single-use `campaign` interval; ours spills it because a higher-weight
interval (the gas-loop index) takes `fp` first.

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `campaign` `short` / `int8_t` / `signed char` | 8,442 | 1,591 | 9,912 |
| `campaign` `unsigned short` / `uint16_t` | 7,345 | 1,681 | 9,912 |
| `campaign` `unsigned char` | 7,347 | 1,635 | 9,908 |
| `campaign` `long long` / `int32_t` | 7,095 | 1,595 | 9,908 |
| `campaign` `unsigned long` / `uint32_t` | 7,096 | 1,595 | 9,908 |
| `campaign` `const` / decl-then-assign / `+0` copy | 7,095 | 1,595 | 9,908 |
| `campaign` `volatile` | 8,615 | 2,750 | 9,856 |
| `campaign` `static` | 9,275 | 1,650 | 9,972 |
| `campaign` `bool = get() >= 2` | 9,605 | 2,403 | 8,572 |
| `campaign` before `turretMode`/`alienOrbit` | 8,117 | 1,597 | 9,900 |
| `missionBlocksPlanets` `int`/`short`/`uint8_t`/`int8_t`/`unsigned char` | 7,234 | 1,656 | 9,912 |
| `missionBlocksPlanets` uninitialised | 9,529 | 1,660 | 9,860 |
| mission block moved after the getters / before the transform | 9,487 | 1,744 | 9,872 |
| `alienOrbit` `bool` | 7,101 | 1,576 | 9,908 |
| `alienOrbit` `unsigned`/`char` | 7,095 | 1,595 | 9,908 |
| `mission` `const` pointer | 7,095 | 1,595 | 9,908 |
| `planetTargets` truthiness (`p &&`) instead of `p != nullptr` | 7,095 | 1,595 | 9,908 |
| campaign condition nested (`if (campaign >= 2) { if (...) }`) | 7,095 | 1,595 | 9,908 |

### `locked` (enemy-loop local) placement

`KIPlayer *locked = this->lockedEnemy;` is in `r8` in ours, spilled at `sp+88`
in the original. Moving/scoping it all worsens raw:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| decl moved after `update(enemy)` | 8,656 | 1,597 | 9,896 |
| decl moved before `update(enemy)` | 8,656 | 1,596 | 9,896 |
| decl at loop top | 9,206 | 2,033 | 9,972 |
| decl after the `continue` guard | 7,095 | 1,595 | 9,908 |
| decl outside the loop | 8,721 | 1,698 | 9,924 |
| `KIPlayer *const locked` | 7,095 | 1,595 | 9,908 |

### Asteroid x/y offset block (`field_0x11d/0x11e/0x11f/0x120`)

Ours hoists `x = this->screenX` out of the branch chain; the original reloads it
per arm. Reconstructing the original's nested shape does not help raw (the
1081-byte `12fc58..130091` region is dominated by block reordering, not this
block):

| variant | raw | aligned | size |
|---|---:|---:|---:|
| nested `else if (0x11e)` form | 7,115 | 1,588 | 9,908 |
| reorder `0x11e && 0x11f` arm first | 7,115 | 1,588 | 9,908 |
| `int x = screenX; int y = screenY;` init form | 7,538 | 1,595 | 9,920 |
| `expect(0x11d && !0x11f && !0x120, 0)` | 7,515 | 1,612 | 9,908 |
| `expect(0x11d && !0x11f && !0x120, 1)` | 7,514 | 1,618 | 9,920 |
| `expect(0x11e && !0x11f, 0/1)` | 7,095 | 1,595 | 9,908 |

### Loop-variable spellings

| variant | raw | size |
|---|---:|---:|
| planet loop `int` | 7,095 | 9,908 |
| enemy loop `int` | 7,095 | 9,908 |
| asteroid-slot `k < 5` / `i < 5` | 7,095 | 9,908 |
| cargo loop `int` / `unsigned int` | 7,096 / 7,095 | 9,908 |

## 3. Exception-cleanup pads

Unchanged: ours still has 7 `StringD1Ev` cleanup sites vs the original's 6
(the extra `sp+220` site, known from wave 11107/11508). The extra pad sits in
the `13017c..13062d` / `130648..130701` regions; no reshaping tried here moved
it without a large raw regression.

## 4. Conclusion

Base after this pass: raw **7,094** / aligned 1,598 / 9,908 B, `.rel.plt` 0,
gate green. The dominant lever remains the `missionBlocksPlanets`/`campaign`
callee-saved swap; every source spelling tried (declaration position/order,
type, condition nesting, `locked` scope, loop-variable scope) leaves the
allocator's cyclic register permutation intact.

## 5. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /dev/shm/w11608/{measure.sh,regions.py,adiff.py,exp*.py}
```
