# LevelScript::process — wave 192 / w13866

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13866`, branch
`wave192-w13866`, base master `f7153965e`.  HEAD after this session's landing:
`5e2766387`.

Session start (base): raw **53,192**, aligned **18,907**, `.text` 76,800,
`.ARM.extab` 594, `.ARM.exidx`/`.rodata`/`.rel.plt` diff 0, size 57,232/57,232.
Session end (HEAD): raw **53,200**, aligned **18,764**, `.text` 76,794,
`.ARM.extab` 594, `.ARM.exidx`/`.rodata`/`.rel.plt` diff 0, size 57,232/57,232,
insn 20,725 / 20,830.

## Landed in this worktree (do not redo)

| commit | variant | raw | aligned | .text |
|---|---|---:|---:|---:|
| `5e2766387` | mission-1 intro `setTarget` -> `play(14)` -> `getPosition` -> `updateEvent3DAttributes` (orig `0x13e30e`) | 53,201 -> **53,200** | 18,764 | 76,794 |
| `4d8360d08` | case-92 state-1: loop `field_0x128` before `field_0x38`; `m_bCinematicBreak=1` before `m_pHud->visible=0`; `m_nScriptTimerA = 0` + `m_nState++` after the two `getDockingTarget(0)` calls | 53,190 -> 53,201 | 18,899 -> 18,764 | 76,795 |
| `134ad5ee1` | case-14/15 source-order swap (case-15 at `0x13d4c6`, case-14 at `0x13d4f4`) | 53,190 | 18,899 | 76,784 |
| `a2480b327` | case-158 state-16 tail `initStreamOutPosition = 0` | 53,193 -> 53,190 | 18,899 | 76,784 |
| `a89435090` | case-64 state-1 camera order | 53,192 -> 53,193 | 18,907 -> 18,899 | 76,800 |

## Original disassembly facts (address-anchored)

### Shared epilogue `0x13b156` and shared continuation `0x144444`
`0x13b156` is a shared tail: `initStreamOutPosition = 0` followed by
`b.w 0x144f60` (function epilogue).  `0x144444` is the other major shared
continuation reached from many state blocks.  These two are the biggest
structural levers left; restructuring around them risks size and `.rel.plt`.

### `13b060` / `13b564` — literal-pool false leads
The aligned-block metric reports large blocks at `0x13b060` and `0x13b564`, but
the original bytes there are literal pools (no code to reorder).  Ignore them.

### Mission-3 `0x13b0ea`
Mission-3 state/dispatch region; no source-order win found.

### Mission-89 `0x139174`
Reconstructed order + failed reorder (54,891 / size 57,144 with
`getStation(10)`->`(100)` and dropping `Status *st`).  Const `100` alone is
neutral.

### `139b16` / `139b22` / `13f906` dispatch finding
The state dispatch / jump-table shape differs from ours at these addresses; the
original reaches the state bodies through a different dispatch chain.

### `138378` — separate switch
Not part of the main state switch; leave alone.

### Timer two-store finding
The original zeroes the 8-byte `m_nScriptTimerA` with a single `strd`, but in
some blocks emits two separate 32-bit stores.  Replacing one with two (case-14
tail) grew the function to 55,347 / size +60 — rejected.

### Case-15 `0x13d4c6..0x13d4f0` fallthrough
Case-15 body falls through into case-14's block; source order of the two cases
was swapped and landed (`134ad5ee1`).

### Case-94 state-1 `0x13e0e6` block (aligned n=614)
Largest aligned residual.  Two branch-shape variants tried and failed.  Needs
address-anchored (not global-aligned) analysis: the global metric pairs this
original region with `oursL5258`, i.e. the wrong address.

### `13e962` shared-timer continuation (aligned n=423)
Pairs with `oursL5297`, again a wrong-address pairing.  Needs local diff.

### `13dc3e` mission-3 scatter loop (raw cluster len 209)
Original keeps the loop index in `r6` with `cmp r6,#8` and reloads
`(*enemies)[i]` at loop top into `fp`; ours uses `fp` for the index and hoists
the enemy-pointer load.  Hoisting the enemy pointer was previously measured at
54,379 / size 57,216 (gate-fatal) — do not hoist.  A `y - 5000` source edit
(`int y = rnd->nextInt(10000); (float)(y - 5000)`) measured 53,205 / 18,765
alone: not a win, reverted.

### Mission-3 state-1 docked branch `0x13e130`
Original:
```
lookBehind()
setRotationAroundTarget(true)
setFreeLookMode(true)        <-- unconditional, BEFORE the field_0xab test
if (field_0xab) setTurretMode(true)
b 143cd8                     <-- shared continuation
```
`else` branch at `0x143cce` is only `setPosition(m_dirVec)` (no
`setFreeLookMode`).  Applying this (move `setFreeLookMode` into the docked
branch, remove from `else`) produced emitted code matching the original
instruction-for-instruction at `13eb6c..13eb94`, but measured **54,204 / 18,764
/ size exact / `.text` 77,798**: locally exact, globally invisible (the aligned
metric pairs it with the wrong region) and raw +1004 from branch-offset shift.
Not landed.

### State-16 out-of-line block `0x13f902` (aligned n=206)
Original `13f90c..13f94e`: `isFading` -> `enableFillScreen(true)` ->
`nextCampaignMission(true)` -> `switch_to_target_setting = 1` (WORD store) ->
`SetCurrentApplicationModule(2)` -> `b.w 0x13b156` (shared tail:
`initStreamOutPosition = 0`, `b.w 0x144f60`).  Ours inlines the same block at
`13d5a2..13d5e8` in a different order.  Rejected variants: body reorder
53,495 / 18,897 (size exact); `default:` 55,502 / size +36 / `.rel.plt` diff 9;
`__builtin_expect(m_nState == 16, 0)` 55,190 / size +12.  Remaining angle:
restructure so clang places it out of line at the mission-0 region end.

### `13d59e` insert cluster (aligned n=206) = ours' state-16 inline site
Ours `13d598: ldr.w r0,[fp,#28]; cmp r0,#16; bne 137f78` then `13d5a2` loads
`Globals` and calls `Layout::isFading` etc.  Original has this block
out-of-line at `13f90c` with the shared tail `13b156`.  Same finding as above.

### `1434fc` raw cluster (217) — register allocation only
Original `1434f2..14351a`:
```
1434f2 ldr.w r0,[fp,#4]; ldr r0,[r0,#28]; blx RadioMessage::isOver
1434fc cmp r0,#0; beq.w 144842
143502 ldr.w r9,[sp,#88]          <-- slot 0x58
143506 ldr r2,[sp,#76]            <-- slot 0x4c
143508 ldr.w r0,[r9,#144]!
14350c ldr.w r1,[r9,#4]
143510 adds r0,r0,r2
143512 adc.w r1,r1,r2,asr #31
143516 strd r0,r1,[r9]
14351a b.w 14484e
```
Ours at `143626..14364e` is the same shape but uses `[sp,#72]` (0x48) instead of
`[sp,#88]` and branches to `144646`/`144652`: pure stack-slot / register
allocation, no source order to change.

### `143cf7` raw cluster (195)
Original `143cf6..143d10`: `strb r4,[r6,#17]` (`m_bCinematicBreak=1`) ->
`strd r4,r4,[r8]` (timer=0) -> `Globals::sound->stop(...)` -> `m_nState++`
(`ldr/adds/str` at `143d0a`) -> `b 144e36`.  Ours at `143cfa..143d1e` has
`Vector::operator=` / `AEMath::pl` / `setPosition`, i.e. the two regions are
paired by the global metric but are different code.  Needs address-anchored
analysis.

### `13ed5e` delete cluster (aligned n=218, oursL1710)
Original `13ed42..13ed88` is case-16 state-1 (`setLookAtCam(false)` ->
`setTarget` -> `setComputerControlled(false)` -> hud/radar stores ->
`resetCamera` -> `forceUpdate` -> `m_bCinematicBreak=0` -> `m_nState=2`); ours
differs in dispatch shape (`isTriggered()` then `cmp r1,#3; bne` vs original
`isTriggered()` alone then `beq 144e36`).  Source at
`src/game/world/LevelScript.cpp:1703`.

### Case-92 (`13a130..13a2c6`) — NOT case-125
Earlier misattribution corrected: this block is case-92.  Variant table in
`scratch/w13866/try_case92.py` / `case92_full.patch`.

## Aligned clusters (HEAD build, `ablocks.py 150`)

```
n=234 replace orig 13b564..13b7ee ours 13b580..13b58a oursL1555   (literal-pool false lead)
n=206 insert  orig 13d59e..-00001 ours 13d598..13d7be oursL932    (state-16 fallthrough)
n=402 replace orig 13d74a..13db9e ours 13d99a..13d99a oursL5258
n=354 replace orig 13dbd8..13dfac ours 13d9c0..13d9c0 oursL5258
n=614 replace orig 13e0e6..13e726 ours 13d9d2..13d9d2 oursL5258   (case-94 state-1)
n=423 replace orig 13e962..13e97a ours 13dbce..13e00a oursL5297   (shared timer continuation)
n=218 delete  orig 13ed5e..13ef8e ours 13ee5c..-00001 oursL1710
n=196 replace orig 1402f8..1402f8 ours 13fe44..14004c oursL2282
n=324 replace orig 141028..1413a2 ours 1410ea..1410ea oursL2702
n=211 replace orig 144a12..144c36 ours 14480e..14480e oursL4769
n=242 replace orig 145c42..145c44 ours 1459d8..145c4e oursL3178
```

## Raw clusters (`topruns.py`, HEAD)

`1434fc` (217), `13b060` (214, literal), `13dc3e` (209), `143cf7` (195),
`13d2da` (189), `143085` (184), `1433a1` (181), `13b742` (181), `13e257` (179),
`13ae9f` (174), `13c992` (168), `140177` (166), `13d44a` (166), `13fb44` (164),
`13ea10` (161), `13ad08` (161), `141dba` (160), `13a6c2` (159), `13bef0` (154),
`138185` (154), `13e767` (153), `13b426` (153), `13a8a7` (152), `143955` (147),
`140277` (146).

## Rejected (do not retry)

w13838 orders `e1690e27c`, `becf87b93`, `da7334718`, `c525de677`; w13835
`planetHit`; case-102 st1 full reorder (18,851); case-69 st1 full reorder +
`freeze=0` (55,004, `.text` 358,922); line-1755 block (53,444 / 18,941);
line-2337 + `freeze=0` (55,104, `.ARM.extab` 1,237); case-154 st0 variants;
case-125 st3 full reorder (+6) and three partials (+1 each); `default:`
state-16 (55,502, size +36, `.rel.plt` diff 9); `__builtin_expect(m_nState ==
16, 0)` (55,190, size +12); two 32-bit stores for `m_nScriptTimerA = 0` in
case-14 tail (55,347, size +60); case-158 tail with `initStreamOutPosition = 1`
(53,199); case-158 tail reordered without the store (53,495); case-125 full
reorder (55,174 / 18,840 / size 57,240), variant 2 (55,172 / 18,840 / 57,240),
case-125 st2 full reorder (53,210 / 18,904), `setTarget` before `getPosition`
only (53,206), camera-call reorder only (53,194 / 18,904); state-16 tail
reorder (53,495); case-92 st1 loop `field_0x128=0` before `field_0x38=3` alone
(53,191); +hud/radar swap (53,191 / 18,901); hud/radar swap only (53,190 /
18,901); case-94 state-1 docked-branch reorder (54,194 / 18,899); mission-89
case-5 reorder + `getStation(10)`->`(100)` + drop `Status *st` (54,891 /
18,933 / size 57,144); mission-3 st3 reorder + const `10` (53,191); mission-3
case-3 scatter loop hoist (54,379 / size 57,216); `Vector ref;` hoisted to
case-92 case scope (55,154 / 18,768 / size 57,244); mission-3 `y-5000` edit
(53,205 / 18,765 alone); mission-3 docked-branch unconditional
`setFreeLookMode` (54,204 / 18,764 / `.text` 77,798).

## Next angles

1. Address-anchored analysis of the `13e0e6` case-94 state-1 block (n=614) and
   the `13e962` shared-timer continuation (n=423).  Both need a local diff of
   the original region against ours; use `cmp2.py` / `firstdiff.py` on the
   per-TU object with `-l` line info to map to source lines.
2. Shared `0x13b156` epilogue / `0x144444` continuation restructure (risk:
   size, `.rel.plt`).

## Tools

```bash
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN11LevelScript7processEi
python3 scratch/w13866/sectdiff.py
python3 scratch/w13866/ablocks.py 150
python3 scratch/w13866/topruns.py
python3 scratch/w13866/adiff.py LO HI [ctx]
python3 scratch/w13866/callseq.py
python3 scratch/w13866/cmp2.py OA OB UA UB
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
```
