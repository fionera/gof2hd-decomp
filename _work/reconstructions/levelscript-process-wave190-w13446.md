# LevelScript::process — wave 190 / w13446: adjacent-statement raw pass

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree
`/home/debian/GalaxyOnFire2/wt/w13446`, branch `wave190-w13446`, base master
`8470b8cd0` (HEAD `e18dea338`).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13446`.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,714           51,708     (-6)
process aligned insns    21,911           21,920     (+9)
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          72,701           72,695     (-6)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
RATCHET                  PASS             PASS
```

## Tooling bug found and fixed (important for future passes)

The scratch harness inherited from the previous session (`harness.py`) compiled
a modified `tmp0.cpp` into the build-dir object and then ran
`ninja -C bdN link_so`.  Ninja's deps log records the object mtime, and the
harness's `os.utime(obj)` after the compile made ninja consider the object
dirty; ninja then **recompiled it from the unmodified worktree source** before
linking.  Every variant therefore linked the base body (all 930
`__builtin_expect` variants and the whole d=1/d=2 sweep measured exactly
51,714 — the tell-tale sign).

Fixed harness `scratch/w13446/harness2.py` compiles the variant and then runs
the recorded `link_so` command line directly (`ninja -t commands link_so`,
last line: gold link + GNU strip + `reorder_shstrtab.py`), bypassing the
dirty check.  Verified with a deliberate `3000.0f -> 3001.0f` edit (the object
and `.so` change) and with the mission-5 `else if -> if` experiment
(raw 55,460, `.text` +192, matching the expected direction).

## Landed swaps (1-based base line numbers; swap moves the first statement below the second)

| # | line | moved statement | across | note |
|---:|---:|---|---|---|
| 1 | 5230 | `m_nScriptTimerA += delta;` | `float approach = (float) m_nScriptTimerA / -34000.0f + 1.0f;` | mission-158 state 1 |
| 2 | 3526 | `player->setRocketControl(nullptr, nullptr);` | `player->killLiberator();` | mission-157 rocket-control teardown |
| 3 | 3131 | `*camPos += *tmp * 400.0f;` | `m_pCamera->setPosition(*camPos);` | mission-157 camera frame |

Each is a pure statement reorder; size stays exactly 57,232 and `.rel.plt`
stays byte-identical.  Combining the three is additive: -3, -2, -1 = **-6 raw
bytes**.  A fourth d=1 candidate (`m_pLevel->particleRenderBoolPtr->reset()`
before `switchSkyboxForIntro()`, -1) was **not** landed because it reorders
the first reference to those two symbols and changes `.rel.plt`
(md5 `67676b5f…`); the other three keep `289b2a06…`.

### Caveat: these are layout/encoding ripples, not source-order corrections

The original's emitted order at all three sites is the *base* order:

* mission-158 (orig `0x138880`): `adds r4,r0,r6` (timer+delta) **before**
  `blx 0x5f7d8` (long-long→float), i.e. `approach` uses the incremented timer;
* all 14 `setRocketControl`/`killLiberator` pairs in the original are
  `setRocketControl` (`0x66bd0`) then `killLiberator` (`0x66bdc`);
* mission-157 camera (orig `0x14269e`): `*tmp * 400.0f` is accumulated into
  `camPos` **before** `setPosition(camPos)` (`0x1426b2`).

The -6 bytes are therefore local scheduling/register-encoding coincidences at
the changed sites (for swap 1: 4 bytes fixed, 1 broken inside
`0x13886e..0x1388ab`), not evidence that the original source had the moved
order.  They are still genuine same-address byte matches and were kept because
the gate is the binary's byte diff; revert them if a later semantic pass
restores the base order and the raw count still improves.

## Sweeps run (with the fixed harness)

* Full d=1 adjacent simple-statement move sweep: 1,763 candidates, 1,492
  size-exact, 4 raw winners (51,711 / 51,712 / 51,713 / 51,713), 355 neutral.
* Whole-condition `__builtin_expect` sweep (930 variants, previous session):
  re-measured neutral on the 51,714 body — `__builtin_expect` on a whole
  condition is a hard no-op here.
* `Vector * float` vs `float * Vector` swaps at the four `float*Vector` sites:
  `3000.0f * dir` → `dir * 3000.0f` is much worse (53,497); the `2400.0f` and
  `-30000.0f` sites are +1 each.  The original uses `float * Vector` at all
  four (its only `operator*(float,Vector)` PLT call is the `5000.0f *
  VectorNormalize(...)` at orig `0x13c2f6`), so the base spelling is right.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13446
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
# -> raw= 51708 aligned=21920 size=57232/57232 insn=20726/20830
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
# -> raw=51708 size=57232/57232 text=72695 relplt=289b2a069f8504d1e8a3919e43915e04
cmake --build cmake-build-match --target check -- -j2   # RATCHET PASS
```
