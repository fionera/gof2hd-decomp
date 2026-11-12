# LevelScript::process — DATA-FLOW pass (wave 188, w13203)

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13203`, branch
`wave188-w13203`, base master `ce16f45f1` (wave-187 harvest 5, raw **51,930**,
`.text` 74,223, `.ARM.extab` 639).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13203`.

Continuation of the wave-182/183/185/186/187 data-flow passes (w12604, w12703,
w12903, w13003, w13103): bool vs int, `float` vs `int` casts, `Vector`
construction/assignment forms, `Array` indexing, multiply-vs-branch.

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,930           51,919     (-11)
process aligned insns    20,928           20,934
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          74,223           74,212     (-11)
.ARM.extab diffbytes     639              639        (size/base exact)
.note.gnu.build-id       20               20
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical)
byte_exact               4507             4507
linked_exact             4507             4507
size-exact               4524/4524        4524/4524
data_exact               843/843          843/843
imports                  207              207
lint                     CLEAN ✓          CLEAN ✓
drift                    0                0
symbol parity            5370/5370        5370/5370
stub_zero_size           0                0
missing / wrong_type     0 / 0            0 / 0
extra                    0                0
RATCHET                  PASS             PASS
```

`raw` is the same-address byte diff over the function body (literal pools
included) from `tools/fn_diff.py`; `.rel.plt` md5 is
`289b2a069f8504d1e8a3919e43915e04` before and after (byte-identical to the
original).  Total section diff after the pass: `.text` 74,212 + `.ARM.extab`
639 + build-id 20 = 74,871.

## Landed change (source line 4983, `case 6` chaser/station compare)

```cpp
// before
if (__builtin_expect(chaser->getPosition().x > (*enemies)[stationIndex]->getPosition().x, 1)) { // lint: arm_builtin
// after
if (__builtin_expect((*enemies)[stationIndex]->getPosition().x < chaser->getPosition().x, 1)) { // lint: arm_builtin
```

Evidence — original disassembly in the `m_nState == 6` block:

```
141bcc:  vldr  s16, [r4, #...]        ; chaser->getPosition().x (loaded first)
141bd0:  vldr  s0,  [r5, #...]        ; station->getPosition().x
141bd4:  vcmpe.f32 s16, s0
141bd8:  vmrs  APSR_nzcv, fpscr
141bdc:  ble.n ...
```

The original compares **station-first** (`s16 < s0` under a `ble` branch),
i.e. the source spelling that survives to the compare is
`station.x < chaser.x`, not `chaser.x > station.x`.  Both spellings are
semantically identical for non-NaN inputs (strict compares, same NaN
behaviour as the original's `vcmpe` + `ble`).  Measured alone on the wave-187
body: raw **51,930 -> 51,919** (-11 B), size 57,232/57,232 exact, `.text`
74,223 -> 74,212, `.rel.plt` unchanged.

## Rejected / neutral probes on this body (all size-exact unless noted; base 51,930)

- **`(float)(N*delta)` family**: `-2*delta`→`-delta*2.0f` all (54,750, size
  57,236), `-3` all 52,320, `-4` 55,009 (57,240), `-12` 53,217 (57,216),
  `-18` 52,190, `-20` 52,074 (57,236), `2*delta` all 54,990,
  `3*delta`→`delta*3.0f` 52,991, `4*delta` 52,309, `5` 52,858,
  `6` 53,175 (57,224), `10` 54,267 (57,248), `12` 52,103 (57,228),
  `13` 53,712 (57,244), `35` 52,068 (57,240), `200` 52,463.  Reverse
  (`delta*4`→`4*delta` etc.) neutral.
- `-delta*X` → `-(float)delta*X`: 0.8 neutral, 0.05 neutral, others
  worse/size-changing.  `(float) -delta` (30 sites) → `-(float) delta` =
  55,259 size 57,016.
- `(float)(double delta * K)` → float form: 0.3 54,757 (57,208), 0.1 55,006,
  0.5 51,931 (+1), 0.2 52,004, 2.2 53,322, 2.3 53,297, 2.4 52,792, 2.5 52,628.
- `nextInt(range)+500` → split 52,092; `nextInt(6000)-7000` 53,314 (57,240);
  `-3000` 53,296 (57,240); `-87000` 52,304; `-6000` 52,738 (57,228).
- `(float)(drift+3900)`/`(drift+500)` split builds fail (operator precedence
  with `Vector *`); `50000+3000*i` 54,510 (57,268); `dy-5000` split 53,559
  (57,216); `offsetY+2000` 54,599 (57,240); `timerA-4000` split 51,936;
  `timerA*delta` split 55,460 (57,340).
- `(float)((delta<<1)&-16)` → `(delta*2)&-16` neutral.  `depth`/`rumble` clamp
  `if (x > 1.0f)` rewrites worse (54,651/55,136, size-changing).
- state-1 `m_nState = 1;` before the two `setPosition` calls: 51,934 (+4).
- `Vector{...}` → `Vector(...)` neutral.  `scalar * (float)delta` /
  `(float)delta * scalar` global swaps neutral.
- L1437 `(float) -delta * 2.0f` → `(float)(-2*delta)` 51,932; L981
  `(-4*delta)` variants neutral/51,932; L1227 `delta*3.0f`→`(delta*3)`
  54,177 (57,240); L1148 reverse 52,294.
- L1080 `-(float)delta*0.5f` → `(float)-delta*0.5f` 54,165 (57,240).
- `(*m_pLevel->getEnemies())` → `(*enemies)` per-site sweep (63 sites, each
  occurrence replaced individually): **all worse** (best L1643 51,968,
  L1646 54,150 size 57,248, …).  Do not retry wholesale.
- `Globals::status` cached-local removal, `LevelScript *self = this` alias:
  neutral/worse (self alias 52,294).
- L4983 `>=` neutral (51,930); removing `__builtin_expect` or
  `expect(...,0)` grows size to 57,240.

## Open lead (not probed to completion)

`field_0xaa = player->autoTurretIsEnabled();` (source line ~4531): the
original emits `strb.w r0,[rX,#170]` immediately after the call at `13a802`,
whereas our build folds the store into the later `if (field_0xaa)` at
`13c73c`.  The two `field_0xab` (`#171`) stores at `13a168`/`13ad82` are
already in place.  Source around lines 4525–4540 and 4760–4770 is the place
to look for a statement-order / temporary-lifetime difference.

## Reproduction

```
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -- -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
python3 _work/reconstructions/measure_w11901.py cmake-build-match/libgof2hdaa.so
cmake --build cmake-build-match --target check -- -j2
```

Scratch harness (not tracked): `h.py`, `sweep.py`, `sweep_occ.py`,
`c1..c11.json` + `.res.json` under
`/home/debian/GalaxyOnFire2/scratch/w13203`.

## Additional probes after landing (all size-exact, base 51,919)

- **case-154 statement order** (source ~4525): the original emits
  `setTurretMode, setFreeLookMode, enableFirstPersonCam, hideShip,
  stopShooting, setComputerControlled, setVulnerable, setSpeed,
  autoTurretIsEnabled` while our source emits a different order.  Full
  reorder = 51,946; swap freelook/turret = 51,922; `setComputerControlled`
  before the assignment = 51,920; `enableFirstPersonCam` before `hideShip`
  = 51,938.  All worse: the whole case-154 body sits at `0x13a78a` in the
  original but `0x13c6cc` in ours (block placement, not statement order).
- **mission==1 prologue order** (source ~951): original emits
  `forceUpdate(delta,true); rotate; getPosition; setToSleep`, ours emits
  `rotate; setToSleep; getPosition; forceUpdate`.  Full reorder 51,923;
  `forceUpdate` before rotate alone 51,922; `getPosition` before
  `setToSleep` alone 51,920.  All worse (register-allocation dominated).
- **`moveForward` association** (source ~963): `(float)delta * (0.2f*depth)`
  -> `(float)delta * 0.2f * depth` = 51,922; `0.2f*depth*(float)delta` and
  `depth*0.2f*(float)delta` neutral (51,919).  Clang canonicalises the
  multiply tree anyway.
- **`m_bStartSequence`/`m_bStartSequenceOver` merge** (source ~1487): the
  original merges the two adjacent byte stores into
  `mov.w r1,#256; strh.w r1,[sl,#32]` (0x0100 -> 0x20=0, 0x21=1).  Moving
  `m_bStartSequenceOver = 1;` next to `m_bStartSequence = 0;` reproduces
  the merge exactly (verified: `mov.w r1,#256; strh.w r1,[fp,#32]`) and
  keeps size 57,232/57,232, but raw = 51,955 (+36, the 4-byte block shift
  misaligns the following ~100 B).  Rejected.
- **local declaration order** at the top of `process`: `Status *status`
  first/second changes size to 57,320 (call order changes); `enemies`
  before `player` = 51,931.

## Root cause of the residual: `this` register

The original keeps `this` in `sl` (r10) and `Globals::status` in `fp` (r11)
(`mov sl, r0` / `ldr.w fp, [r0]`); our build has them swapped
(`mov fp, r0` / `ldr.w sl, [r0]`).  Blocks that are otherwise
instruction-for-instruction identical (e.g. the mission-5 body at original
`0x138132` vs ours `0x138256`) differ only by that register choice, so the
swap is the single largest systematic source of raw diff.  No source-level
lever found yet to flip the allocation; the declaration-order probes above
do not affect it.
