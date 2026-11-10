# Radar::draw wave 167 (w11206) — campaign / space-lounge block, re-measured

**Status:** complete, 2026-11-10. Worktree `wt/w11206`, branch `wave167-w11206`,
base master `d00057e6d`. Target `src/game/weapons/Radar.cpp`,
`_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`), 9,908 B.

Read with `radar-draw-campaign-sl-wave166-w11106.md` and the wave-166 merge tests.
All numbers below are measured on the *current* base (which already contains
w11107's sound-ladder/decl-order pass and w11118's per-TU
`-disable-postra-machine-licm`), not on w11106's 7,546 base.

## 0. Baseline (verified)

| metric | value |
|---|---:|
| function size | 9,908 |
| raw same-address diff | 7,487 |
| instruction-index alignment | 1,454 |
| instructions | 3,536 ours / 3,532 orig |
| `.rel.plt` | byte-identical |

## 1. The wave-166 "next wave" list is stale on this base

Every item on w11106's *Measured, not landed* list is now **worse** than the
base; none is a win.  The landmark `!lockConfirmed` block it targeted is
already byte-identical in the base (see §2), so adding the expect only
re-outlines it.

| variant | raw | size | verdict |
|---|---:|---:|---|
| base | **7,487** | 9,908 | — |
| landmark `expect(!lockConfirmed, 0)` (line 500) | 7,533 | 9,916 | +46 raw, +8 B |
| `expect(stationId != currentStation && isTarget, 0)` on body1 | 8,442 | 9,912 | +955 raw, +4 B |
| remove campaign-guard `expect(..., 0)` | 8,866 | 9,976 | +1,379 raw, +68 B |
| remove guard expect + inner expect | 8,719 | 9,964 | +1,232 raw, +56 B |
| planet-block `expect(!lockConfirmed, 0)` | 7,585 | 9,916 | +98 raw, +8 B |
| planet-block `expect(!lockConfirmed, 0)` + `isDockingToPlanet,1` | 7,520 (w11106 base) | 9,916 | old base only |

The `+8 B` in both lock-landmark/planet variants is the extra `sp+220`
`String` destructor site in the exception cleanup tail, as w11106 noted;
the current base's tail (1306a0–1306ee) is already instruction-for-instruction
identical to the original (5 `StringD1` calls in the landing pads, 11 in the
whole function), so the +8 B is not a simple destructor swap.

## 2. Landmark block is already matched

The original's landmark station-lock block is `12e3be`–`12e3f4`
(`stationLockTimer += mode; if (timer > field_0x1b8) { if (!lockConfirmed) play(26); lockedStation = field_0x28; }`).
The base emits the same `ldrb [r4,#536]; cbnz; …play…; ldr [r4,#420]; ldr/str [r4,#36]`
shape at `130350` (only the PC-relative literal address and stack slots differ).
Adding `expect(!lockConfirmed, 0)` turns the `cbnz` fall-through into an
outlined `cmp/beq.w` block, which is a regression.  (The wave-166 note cited
`12f85a`–`12f89a`; that range is the *gas*-lock block, also already identical.)

## 3. Root cause of the campaign-block mismatch: `missionBlocksPlanets` register

The campaign block's block order differs because of one register choice:

* original: `missionBlocksPlanets` lives in **fp** (`12ebda`: `subs.w fp,r0,#172;
  it ne; movne.w fp,#1`), the shared zero constant is also fp, and
  **campaign lives in sl** (`12e0f8: mov sl,r0`, used at `12e38a: cmp.w sl,#2`).
  `stations` is then kept in sl and spilled to `sp+116` around clobbers
  (`12e3c6: mov sl,r0; 12e3ce: str.w sl,[sp,#116]`).
* ours: `missionBlocksPlanets` lives in **sl** (`12eba8`: `subs.w sl,r0,#172`),
  the zero constant is sl, `campaign` is spilled to `sp+144`
  (`12e0f8: str r0,[sp,#144]`), and `stations` is spilled to `sp+132`.

Fixing that one allocation would align the whole campaign block (the first
2-byte divergence is exactly `12e3c6`/`12e3d0`), but no source shape tried
moves it.  Swept (all neutral or worse):

* all 6 permutations of `missionBlocksPlanets`/`alwaysEnemySeen`/`alwaysEnemy`
  (all 7,487 — the allocator ignores decl order here);
* all 24 legal permutations of `otherLock`/`landmarkHit`/`notTurretMode`/
  `notOtherLock` (7,487 or 7,488);
* `missionBlocksPlanets`/`alwaysEnemySeen`/`alwaysEnemy` as `int`/`char`/
  `uint8_t` (7,642–8,969);
* `stations` as `*const`, `marked` hoisted out of the loop with a per-iteration
  reset, `planetTargets->size()` cached, `campaign` unsigned, `alienOrbit`
  bool, `turretMode` int (all 7,487–9,206);
* `expect` cells: `campaign>=2` 1/0, `!alienOrbit` 0, guard 0→1,
  planet-lock `>=1` 1→0, `isDockingToPlanet` 1→0, `cur != 120` 1→0,
  `onScreen`/`screenX` 1→0, sound-ladder `<3`/`<5` 0→1, `bossSeen` 1→0,
  `alwaysEnemySeen` 1→0 (best 7,493–7,494; rest 7,537–9,532);
* chain-first-condition `expect(...,0)` with the guard expect removed
  (9,077 raw / 10,020 B).

## 4. Other measurements

* Raw diff is dominated by a same-address shift, not by local instruction
  differences: the first 0x370 bytes differ in only ~25 bytes, the
  gas/asteroid lock region `12f850`–`12fc50` is essentially byte-identical,
  and the enemies block (`12e7a8`–`12f7d0`) carries a −40…−70 B shift that is
  compensated later.  This is why a +4/+8 B shape change is gate-fatal here.
* The exception cleanup tail (`1306a0`–`1306ee`) and the literal pool are
  already matched (2-byte offset only).

## 5. Reproduction

```sh
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN5Radar4drawEP6PlayerP3Hudi
cmake --build cmake-build-match --target check -j2
# scratch: /dev/shm/w11206/{sweep1..6.py,try.sh,base.dis,orig.dis,adiff.py}
```

No source change landed: every candidate measured worse than the 7,487-byte
base.  Tree left clean at `d00057e6d`.
