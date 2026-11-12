# LevelScript::process — wave 189 / w13336: full-body re-derivation + d2 swap

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13336`, branch
`wave189-w13336`, base master `8225c71a8` (raw **51,874**, `.text` 73,999,
`.ARM.extab` 639).  Scratch `/home/debian/GalaxyOnFire2/scratch/w13336`.

This pass re-derived the whole function's register/layout picture from the
original Thumb stream (prologue map, per-register use census, same-address
instruction census, call-sequence alignment) and then ran a **distance-2
statement-swap sweep** (not the adjacent d1 family owned by w13310, not the
operator-overload family of w13330, not the K=4..8 block family of w13332).

## Result (code win landed)

```
                         base (HEAD)      this pass
process raw diffbytes    51,874           51,870     (-4)
process aligned insns    21,005           21,005
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
orig / ours insns        20,830 / 20,726  20,830 / 20,726
.text diffbytes          73,999           73,995     (-4)
.ARM.extab diffbytes     639              639        (size/base exact @0x1a3b78, 156,312 B)
.ARM.exidx diffbytes     0                0          (byte-identical)
.rodata / .data / .bss   0 / 0 / 0        0 / 0 / 0
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
.note.gnu.build-id       20               20
file size                2,187,328        2,187,328  (exact)
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

## Landed change (d2 swap #1)

Source lines 760/762 (1-based, base), `case 6` of the mission-0 state
machine:

```cpp
// before
Vector zero{0.0f, 0.0f, 0.0f};
Vector pos = player->getPosition();
m_nScriptTimerA += delta;
// after
m_nScriptTimerA += delta;
Vector pos = player->getPosition();
Vector zero{0.0f, 0.0f, 0.0f};
```

`m_nScriptTimerA += delta` is independent of the two vector constructions
(disjoint receivers, `delta` is an argument), so the reorder is
semantics-preserving.  Measured alone on the base body: raw
**51,874 -> 51,871** (-3), size 57,232/57,232 exact, `.rel.plt` unchanged.

## Landed change (d2 swap #2)

Source lines 834/836 (1-based, base), mission-0 state 9:

```cpp
// before
m_pCamera->setPosition(5000.0f, 500.0f, -10000.0f);
m_nState = 10;
m_pGeometry5->setPosition(player->getPosition());
// after
m_pGeometry5->setPosition(player->getPosition());
m_nState = 10;
m_pCamera->setPosition(5000.0f, 500.0f, -10000.0f);
```

`m_pCamera` and `m_pGeometry5` are disjoint receivers and
`player->getPosition()` is a pure read, so the reorder is
semantics-preserving.  Measured on top of #1: raw **51,871 -> 51,870**
(-1), size exact, `.rel.plt` unchanged.  (The sweep measured this cell as
-1 alone on the base body; it composes with #1.)

## Full-body re-derivation notes (why the residual is shift-dominated)

* The function is size-exact but the instruction streams diverge from the
  prologue onward: only **1,654 bytes** of the 57,232-byte body match in
  runs >= 8 B.  The first divergence is the `{sl,fp}` prologue tie-break
  (`this`=fp / canary=sl in ours vs `this`=sl / canary=fp in the original),
  then the `&Globals::status` register.
* Original prologue map (unchanged from w10601/w10502): `this`=sl,
  `&Globals::status`=r4, canary=fp, `&m_pLevel`=r4->[sp,#80], messages=r5,
  delta=r6, enemies=r8, player=[sp,#84], Status* **not cached**.
  Ours: `this`=fp, `&Globals::status`=r9, canary=sl, Status* cached in r4.
* The `&Globals::status` r9-vs-r4 difference is a **size** difference, not
  just a register difference: ours emits `ldr.w r9,[r0]` / `ldr.w r0,[r9]`
  (4 B each) where the original emits `ldr r4,[r0]` / `ldr r0,[r4]` (2 B
  each).  That is **+4 B** at `0x137d5a..0x137d62`, and it shifts every
  following instruction by +4..+6 B, which is why the raw metric stays near
  52 KB even though the instruction sequences are otherwise close.
* Levers re-measured on the current body (all size-exact unless noted):
  * 24 declaration orders of `messages/player/enemies/status`: base order
    best (51,874); next best 51,878.
  * `Status *&status`, `Status &status = *Globals::status`, inline
    `Globals::status->` (no local), `Status **statusp = &Globals::status`:
    all 51,874/51,875, all keep `&Globals::status` in r9.
  * `LevelScript *lself = this` (any use count): byte-identical to base.
  * `Level *level = m_pLevel` alias: N=1 identical; N>=2 grows size.
  * `TargetFollowCamera *camera = m_pCamera` / `Radar *radar = m_pRadar`
    aliases: size not exact (57,092 / 57,168), raw worse.
  * Mission-1 block reorders (`forceUpdate` first, `getPosition` before
    `setToSleep`): raw 51,875..51,878, i.e. regressions.
  * Mission-0 state-0 `m_nState = 1` placement: before player setPosition
    51,878, after player 51,875, after camera (base) 51,874.
* Conclusion: the `this`/`&Globals::status` register tie-break is a global
  greedy-allocator outcome of the reconstructed body; no local source lever
  found in this pass flips it without a size change.  The remaining raw
  residual is dominated by the resulting +4..+6 B code shift, not by local
  statement order.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
cmake --build cmake-build-match --target check -j2
```

Scratch harnesses: `scratch/w13336/{measure.sh,sweep_d2.py,diffmap.py,ann.py,
callalign.py,secdiff.py}`.
