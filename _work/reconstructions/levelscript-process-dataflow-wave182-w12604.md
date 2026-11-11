# LevelScript::process — DATA-FLOW pass (wave 182, w12604)

Target: `_ZN11LevelScript7processEi` in `src/game/world/LevelScript.cpp`.
Base (master 57e085059): raw **52,523**, aligned 22,148, size 57,232/57,232,
insn 20,727/20,830.

## Landed win

`src/game/world/LevelScript.cpp` line 1435 (body 831), Chain-A station-departure
intro:

```cpp
// before
m_pCamera->translate((float) delta, 0.0f, (float) (-2 * delta));
// after
m_pCamera->translate((float) delta, 0.0f, (float) -delta * 2.0f);
```

Measured: raw **52,523 -> 52,521** (-2 B), aligned 22,148 -> 22,155,
size 57,232/57,232 exact, insn 20,727 -> 20,730.
`.rel.plt` md5 `74ee0eaed2f9b6e74c859245f2135d01` == original.

The original emits the z term as a float multiply of the negated converted delta
(`vcvt.f32.s32` then `vmul`), not as an integer `-2 * delta` converted to float;
the int form forces an extra negate/convert pair. This is the same shape as the
already-landed `(float) -delta * k` cells elsewhere in the function.

## Landed win 2 (escort route arrays, body ~3002/3007)

`src/game/world/LevelScript.cpp` body ~3002/3007 (m_nState==2 escort route
construction):

```cpp
// before
(int) (r.y * 900000.0f + d.y * -900000.0f),
(int) (d.x * -900000.0f - r.x * 900000.0f),
// after
(int) ((r.y - d.y) * 900000.0f),
(int) ((d.x - r.x) * -900000.0f),
```

Measured against base 52,523 (win 1 had been reverted by a scratch-harness
restore at this point): raw **52,523 -> 52,513** (-10 B), aligned 22,148 ->
22,170, size 57,232/57,232 exact, insn 20,730 unchanged.
`.rel.plt` md5 unchanged (`74ee0ead...`).

A 5-way combinatorial sweep over the six route components (r0x/r0y/r0z/r1x/r1y
`(int)` forms) found `r0y + r1x` optimal; the individual components interact
strongly (r0all alone is +1 vs base). `r0y` alone = 52,515, `r1x` alone =
52,521.

## Landed win 3 (depth-scaled moveForward, body ~358)

`src/game/world/LevelScript.cpp` body ~358 (mission==1 intro, enemy[0] creep):

```cpp
// before
(*enemies)[0]->geometry->moveForward((float) delta * 0.2f * depth);
// after
(*enemies)[0]->geometry->moveForward((float) delta * (0.2f * depth));
```

Measured on top of wins 1+2: raw **52,513 -> 52,510** (-3 B), aligned 22,170 ->
22,178, size 57,232/57,232 exact, insn 20,730 unchanged. `.rel.plt` unchanged.
The original associates the two constants first (`0.2f * depth`) before the
delta multiply; the flat left-to-right chain was one `vmul` ordering off.

## Re-land of win 1 (after pass 2 accidentally reverted it)

Win 1's source edit was lost when a scratch sweep restored the pre-win-1 file.
Re-applied on top of wins 2+3: raw **52,510 -> 52,508** (-2 B), size
57,232/57,232 exact, `.rel.plt` unchanged. Net from master: 52,523 -> 52,508.

## Rejected (all measured against base 52,523 unless noted)

Single `(float)(N*delta)` -> `(float) delta * N.0f` / `(float) -delta * N.0f`
conversions (size-exact unless noted, raw shown):

| site | raw | note |
|---|---:|---|
| L309 `(-2*delta)` | 52,728 | worse |
| L1312 `(-2*delta)` | 54,786 | size 57,240 (reject) |
| L1354 `(-2*delta)` | 52,721 | worse |
| L1483 `(-2*delta)` | 52,550 | worse |
| L376 `(2*delta)` | 52,876 | worse |
| L1457 `(2*delta)` | 53,900 | size 57,224 (reject) |
| L2542 `(2*delta)` | 52,565 | worse |
| L376 `(-4*delta)` | 52,880 | worse |
| L567 `(delta*4)` | 52,854 | worse |
| L1087 `(-3*delta)` | 52,778 | worse |
| L1568 `(-18*delta)` | 52,742 | worse |
| L2049 `(-12*delta)` | 53,309 | size 57,224 (reject) |
| L2099 `(-12*delta)` | 53,321 | size 57,224 (reject) |
| L2636 `(3*delta)` | 52,616 | worse |
| L2939 `(6*delta)` | 53,534 | size 57,224 (reject) |

Escort route variants (single-component, size-exact): r0x 52,517; r0y 52,515;
r0z 52,525; r1x 52,521; r1y 52,525; r0all 52,524; r0r1 52,531.
`d.x*-900000 -> -d.x*900000` neutral. `speed*(float)delta` swap neutral.
`(int)((float)delta*0.5f)` -> `(int)(delta*0.5f)` neutral. `(long long)` cast
factoring worse (53,753, size 57,216).

Further probes on top of wins 1+2 (base 52,513): `enemyZ / -5000.0f` ->
`enemyZ * -0.0002f` = 52,514; drift-expression parenthesizations (all four
`* 0.4f * (float)delta` sites, and each individually) all worse (52,602-54,442,
some size-changing); `m_dirVec * ((float)delta * 1.7f)` de-parenthesized
size-grows to 57,368.

Combos re-measured from the handoff: `A+D` (speed-block restructure + L1319
int-mul) = 52,770 — worse than base; all other A/C combos also worse.

## Conclusion

Only the Chain-A z-term cell is a genuine data-flow win; the rest of the
`(float)(N*delta)` family is already in its optimal form. No other
bool/int/Vector/Array-index/multiply-vs-branch candidate probed here lowered
the raw diff while keeping size exact.
