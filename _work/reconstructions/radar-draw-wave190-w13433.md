# Radar::draw wave 190 (w13433) — planet-lock store order
# (raw 6289 -> 6288, aligned 1912 -> 1913, size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13433`, branch `wave190-w13433`, base
`master` `7e34b44b1`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave190-w13402.md`, `radar-draw-wave190-w13404.md`,
`radar-draw-wave190-w13412.md`, `radar-draw-campaign-sl-wave167-w11206.md`.

## 0. The change

In the planet-lock hit branch inside the planet loop, the two independent stores were
emitted in the order `planetDockIndex = i; field_0x18 = planetTargets->data()[i];`.
The original stores `field_0x18` first and `planetDockIndex` second, which lets clang
reuse the already-loaded `planetTargets->data()[i]` value in the register that is still
live for the first store, saving one instruction byte in the loop tail:

```cpp
-                        this->planetDockIndex = i;
                         this->field_0x18 = this->planetTargets->data()[i];
+                        this->planetDockIndex = i;
```

The two stores are to independent fields of `this`, so source order does not change
semantics. This was the only raw-improving size-exact adjacent same-indent statement
swap in the whole `Radar::draw` body (exhaustive sweep with
`scratch/w13433/adjswap.py`).

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,289 | **6,288** |
| instruction-index alignment | 1,912 | 1,913 |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,543 / 3,532 | 3,543 / 3,532 |
| `.text` differing bytes (whole `.so`) | 72,885 | **72,884** |
| `.ARM.extab` differing bytes | 601 | 601 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,506 | **73,505** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370. The function is already
non-byte-exact, so `byte_exact` is unchanged; the gain is one raw `.text` byte.

The `aligned` metric rises by 1 because the byte is removed from the middle of the
planet-loop region and shifts the following instruction-index alignment; the task
prioritised the raw byte win and the function stays size-exact.

## 1. Negative results (all reverted)

Full `link_so -j2` + `tools/fn_diff.py` measurements, scratch
`/home/debian/GalaxyOnFire2/scratch/w13433/`:

| variant | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,289 | 1,912 | 9,908 |
| **planet-lock store swap (landed)** | **6,288** | **1,913** | **9,908** |
| `bool otherLock` | 9,264 | 2,719 | 9,864 |
| `int otherLockFlag` | 9,412 | 2,014 | 9,900 |
| `bool otherLock` + `bool notOtherLock` | 9,063 | 2,581 | 9,880 |
| `bool otherLock` + `int otherLockFlag` | 9,182 | 2,570 | 9,888 |
| hull/emp multiply operand reorder | 6,294 | 1,855 | 9,908 |
| `music_cab` branch order | 6,304 | 1,907 | 9,908 |
| `music_cba` branch order | 6,304 | 1,909 | 9,908 |
| `music_bac` branch order | 6,294 | 1,909 | 9,908 |
| `music_mask_first` | 6,312 | 1,910 | 9,908 |
| remove `field_0x1bc` hint | 6,470 | 1,834 | 9,916 |
| remove `alwaysEnemySeen` hint | 7,268 | 1,911 | 9,904 |
| remove `<5` alwaysEnemySeen hint | 7,292 | 1,910 | 9,904 |
| remove `!lockConfirmed` hint | 6,385 | 1,893 | 9,904 |
| campaign declaration first | 8,557 | 1,902 | 9,900 |
| campaign after enemies | 6,366 | 1,917 | 9,908 |
| campaign after transform | 9,078 | 1,865 | 9,912 |
| m24 enemy-type-first | 8,631 | 2,092 | 9,892 |
| guard split h0-first | 6,944 | 1,990 | 9,908 |
| `dock_2nd` | 8,484 | 2,040 | 9,904 |
| `counts_before_wreck` | 6,291 | 1,923 | 9,908 |
| `rm_field24_hint` | 8,421 | 1,846 | 9,904 |
| spark `int top` + add form | 6,305 | 1,887 | 9,908 |
| spark `int top` + sub form | 6,305 | 1,890 | 9,908 |
| spark y-add only | 6,292 | 1,912 | 9,908 |
| gas 24000 x ego-first | 6,289 | 1,913 | 9,908 |

Comparison-operand flips across the whole body were neutral (6,289 / 1,912 / 9,908)
or worse. The dominant residual remains the campaign-in-`sl` /
`missionBlocksPlanets`-in-`fp` register rotation described in w11206 §3 and
w13412 §1; `bool otherLock` reproduces the skeleton but grows the frame
(`0x128 -> 0x130`) and shrinks the function, so it is not size-exact.

## 2. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6288 aligned= 1913 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13433/`: `Radar.base.cpp` (6,289 base),
`harness.py`, `apply.py`, `adjswap.py`, `shiftmap.py`, `regdiff.py`, `worddiff.py`,
`cmpflip.py`, `cmpflip2.py`, `sweep*.py`, `batch*.py`, `v*.py`.

## 3. Additional sweeps on the post-win base (6288 / 1913 / 9908)

All measured with `scratch/w13433/harness.py` (full `link_so -j2` + `fn_diff.py`),
restored after each cell unless noted.

### Register-pressure / type toggles

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `bool notTurretMode` + `bool notOtherLock` | 9,522 | 2,012 | 9,880 |
| `bool notTurretMode` only | 9,522 | 1,982 | 9,872 |
| inline `!turretMode && !otherLock` | 9,522 | 1,982 | 9,872 |
| `missionBlocksPlanets` as `unsigned`/`short`/`long`/`char`/`int8_t`/`uint16_t`/`int16_t`/`int32_t` | 6,288 | 1,913 | 9,908 |
| `missionBlocksPlanets` as `bool` | 6,425 | 1,916 | 9,912 |
| `campaign` as `long`/`long long`/`int32_t` | 6,288 | 1,913 | 9,908 |
| `campaign` as `unsigned` | 6,289 | 1,913 | 9,908 |
| `campaign` as `short`/`char`/`int8_t`/`int16_t` | 8,440 | 1,928 | 9,912 |
| `alienOrbit` as `bool` | 6,297 | 1,843 | 9,908 |
| `alienOrbit` as any integer type | 6,288 | 1,913 | 9,908 |
| `turretMode` as any integer type | 9,276 | 2,755 | 9,864 |
| `onPlanetCourse` as any integer type | 6,288 | 1,913 | 9,908 |
| `otherLock`/`notTurretMode`/`notOtherLock` as `unsigned` | 6,288 | 1,913 | 9,908 |
| `otherLockFlag` as ternary / OR-chain / `= true` | 6,288 | 1,913 | 9,908 |

### Loop and condition forms

* Loop counter `uint32_t i` -> `int i` in any subset of the four loops: neutral
  (6,288 / 1,913 / 9,908).
* Loop test `i < size()` -> `i != size()` in any subset: 6,289-6,290 (worse);
  `++i` -> `i++` neutral.
* `&&` operand swaps in pure two-operand guards (e.g. `!turretMode && !landmarkHit`,
  `field_0x28 != 0 && ...`, `stationId != currentStation && ...`): neutral or worse;
  `field_0x1c == nullptr && tractorAuto` 6,477, `i == 0 && !alienOrbit` 8,512,
  `field_0x11d && ...` 6,789.
* Flipping each of the 31 `__builtin_expect(..., 0/1)` hints one at a time: all worse
  or neutral (best neutral cells 6,288 / 1,913 / 9,908).
* Moving `campaign` before the zero stores: 8,643 / 1,924 / 9,900 (not size-exact).
* Swapping/chaining `plasmaInRange = 0; lockConfirmed = 0;`: 6,290.
* `(void) campaign;` at four sites: byte-identical (the use is eliminated).
* `__builtin_assume(campaign < 1000);`: neutral.

### Per-TU `-mllvm` flag sweep (Radar.cpp only, reverted)

`-regalloc-csr-first-time-cost=0/1/2/5/10/100/1000`,
`-condsider-local-interval-cost=false`, `-split-spill-mode=1/2`,
`-disable-spill-fusing`, `-disable-spill-hoist`: all byte-identical to the base
(6,288 / 1,913 / 9,908). `-enable-deferred-spilling` 8,773 / 9,916;
`-sink-insts-to-avoid-spills` 9,742 / 10,048.

### Remaining structural residual

The campaign-in-`sl` / `missionBlocksPlanets`-in-`fp` rotation is unchanged: the
original materialises the shared zero in `fp` at `12e07a`, ours in `sl`, so
`campaign` is spilled to `sp+0x90` and the planet/enemy loop counters and stack
slots shift. This is the same cascade described in w11206 §3 and w13412 §1; no
source shape tried here moves it. The extra `mov r5,fp` at `12f79e` is a direct
consequence (ours reloads the enemy loop counter from `fp`, the original from
`sp+0x98`).
