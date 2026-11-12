# Radar::draw wave 190 (w13412) — planet-lock `top + size > screenY` form
# (raw 6291 -> 6289, aligned 1841 -> 1912, size-exact, `.rel.plt` identical)

**Status:** landed, 2026-11-12. Worktree `wt/w13412`, branch `wave190-w13412`, base
`master` `a05e12479`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-wave189-w13346.md` (three blockers), `radar-draw-wave190-w13398.md`
(enemy-loop box order), `radar-draw-campaign-sl-wave167-w11206.md` (campaign/missionBlocksPlanets
register root cause) and `radar-draw-wave175-w11905.md` (campaign-in-sl cascade).

## 0. The change

The planet-lock hit test in the planet loop evaluated the lower bound of the y axis as
`top > this->screenY - size`. The original computes `top + size` and compares `screenY`
against it (`12e556 add r1,r2; 12e558 cmp r3,r1; 12e55a bge`), while the base canonicalised
the same test to `screenY - size` and compared against `top`
(`12e524 subs r1,r3,r1; 12e526 cmp r1,r2`). Rewriting the source in the original's
addition form makes clang emit the original shape:

```cpp
-                if (this->screenX > left && this->screenX < left + size && this->screenY > top &&
-                    top > this->screenY - size) {
+                if (this->screenX > left && this->screenX < left + size && this->screenY > top &&
+                    top + size > this->screenY) {
```

Semantics are identical (`top > screenY - size` <=> `top + size > screenY`); no overflow
concern because the operands are the same small screen-space integers.

| metric | base | after |
|---|---:|---:|
| raw same-address diff | 6,291 | **6,289** |
| instruction-index alignment | 1,841 | **1,912** |
| function size | 9,908 / 9,908 | 9,908 / 9,908 |
| instruction count | 3,542 / 3,532 | 3,543 / 3,532 |
| planet-loop region diff (`12e3c6-12e7aa`) | 952 | **950** |
| `.text` differing bytes (whole `.so`) | 72,959 | **72,957** |
| `.ARM.extab` differing bytes | 603 | 603 (no growth) |
| `.rel.plt` | byte-identical | **byte-identical** |
| total (text+extab+build-id) | 73,582 | **73,580** |

`RATCHET PASS`: byte_exact 4,507 / linked_exact 4,507 / size 4,524 / data 843 /
imports 207 / lint CLEAN / drift 0 / parity 5,370. The function is already
non-byte-exact, so `byte_exact` is unchanged; the gain is the structural alignment of the
planet-lock y test.

Other spellings measured on the same cell: `screenY < top + size` 6,292/1,839,
`screenY - size < top` 6,291/1,841 (byte-identical to base). `top + size > screenY` is the
only one that reproduces the original's `add`/`cmp` shape.

## 1. Register-rotation investigation (no source shape moves it)

The dominant residual is still the campaign / `missionBlocksPlanets` register rotation
(see w11206 §3 and w13346 §4). Confirmed on this base:

* original: shared zero and `missionBlocksPlanets` in **fp** (`12e07a`, `12e0d8`,
  `12ebda subs.w fp,r0,#172`), campaign in **sl** (`12e0f8 mov sl,r0`, `12e38a cmp.w sl,#2`),
  then `sl` reused for `stations` (`12e3c6 mov sl,r0; 12e3ce str.w sl,[sp,#116]`), planet
  loop counter `i` in **fp** (`12e3da cmp fp,r1`);
* ours: shared zero and `missionBlocksPlanets` in **sl**, campaign spilled to `sp+0x90`,
  `stations` spilled to `sp+0x84`, `i` in **sl**, `fp` free until `12e45a`.

Because `i` is `fp` vs `sl`, the whole planet loop / crosshair body / enemy loop is
register-renamed (region diffs 950 / 1,073 / 1,598) and the tail shifts by 6 B at the
wreck block. Measured cells (full `link_so -j2` + `tools/fn_diff.py`, scratch
`/home/debian/GalaxyOnFire2/scratch/w13412/`):

| body | raw | aligned | size |
|---|---:|---:|---:|
| base | 6,291 | 1,841 | 9,908 |
| `bool otherLock` (campaign->sl, frame +8) | 9,228 | 2,714 | **9,860** |
| campaign before `turretMode` (zero->fp, call order wrong) | 8,559 | 1,831 | **9,900** |
| campaign before `alienOrbit` | 6,298 | 1,846 | 9,908 |
| campaign after `planetTargets` | 9,175 | 1,966 | **9,920** |
| `int otherLockFlag` | 9,415 | 2,016 | **9,900** |
| `alwaysEnemySeen` bool->int | 9,054 | 2,857 | **9,924** |
| `alwaysEnemy` bool->int | 8,573 | 1,955 | **9,920** |
| `missionBlocksPlanets` bool | 6,428 | 1,845 | **9,912** |

`otherLock`/`missionBlocksPlanets`/`campaign` type toggles (`unsigned`, `char`, `short`,
`long`, `uint8_t`, `int32_t`, `intptr_t`, `const`, `register`, separate declaration,
explicit else-arm init, `(void)campaign`) are all byte-identical to the base except the
ones listed above; the allocator canonicalises them. No shape tried moves
`missionBlocksPlanets` from `sl` to `fp` while keeping the frame at `0x128`.

## 2. Enemy-loop start (`12ef4c`-`12efc4`) is not independently fixable

The original keeps `locked` in `r5` (reloaded at `12ef7e`, before the `VectorLength`
compare) and the `inSight` bool in `r6`; ours sinks the `locked` load to `12efc6` and uses
`fp` for the bool (because `fp` is free — the shared zero is in `sl`). Rewriting the
`inSight` block (`int inSight`, declaration reorder, single-expression form, ternary store,
`delta` first, `field_0x73 = false`) is byte-identical; `delta` first is worse
(6,316/1,850). This is downstream of the campaign rotation, not a separate lever.

## 3. Wreck x/y block (`12fc5a`-`12fca0`, 12 B shorter than the original)

The original rematerialises `screenX` in four arms (`12fc6a/70/88/94`); ours CSE-hoists it
(`12fc5e`) and merges the two `screenY + 11` arms. Forcing a volatile read in arm 2
(`field_0x11e && !field_0x11f`) drops the hoist and reproduces the original 82-byte block
(`12fc5a`-`12fcac`), but the function then grows to 9,920 because the following tractor
block is 6 B longer than the original's. Reordering the tractor guard
(`tractorMode2 && field_0x1c == nullptr && wreck`) does **not** produce the original's
26-byte `cbnz`/`eors`/`itttt eq` form (still 32 B, 6,293/1,831); `!ptr`, `== 0`, nested
`if`s, `expect(wreck,1/0)` and body reorder are all 6,293 or worse. The wreck+tractor pair
still needs a joint reconstruction (w13346 §6).

## 4. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
# -> raw= 6289 aligned= 1912 size=9908/9908 insn=3543/3532
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13412/`: `Radar.base.cpp` (6,291 base),
`harness.py`, `probe.py`, `idiff.py`, `lcsdiff.py`, `regdiff.py`, `pools.py`,
`v_ins.json`, `v_camp*.json`, `v_decl.json`, `v_mbp*.json`, `v_tr.json`, `v_wreck.json`,
`v_y.json`, `orig_radar.txt`, `ours_radar.txt`.
