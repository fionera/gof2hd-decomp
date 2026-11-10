# Small tail wave 167 (w11217): five residual re-sweep, no new source win

Status: **0 `src/` changes landed**.  Worktree `wt/w11217`, branch
`wave167-w11217`, base master `d00057e6d` (wave 166).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11217`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` byte-identical.

Metrics: raw same-address diff bytes and instruction-index alignment
(`tools/fn_diff.py`).  Baseline residuals re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN3Gun22calcCharacterCollisionEv` | 463 | 57 | 1984 |
| `_ZN11SpaceLounge10drawLoungeEv` | 175 | 41 | 1948 |
| `_ZN10ModStationC1Ev` / `C2Ev` | 43 | 12 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 15 | 111 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`; the
incremental link is ~1.1 s, so the sweeps are exhaustive for the spellings
listed.

## MGame::OnUpdate — the one shape that changes the metric

The wave-165 volatile anchor is `15/111` (raw best).  The wave-163 shape
(`int radioIndex = nextInt(2); Level *level = this->level; int radioBase =
Globals::status->getCurrentCampaignMission()...; level->createRadioMessage(...)`)
is **16/6**: the `Level *level` local live across the `ccm` PLT call fixes
the m23 `ldrd` register pair (`ldrd r2,r3` matches the original) and drops
alignment 111 -> 6, at a cost of +1 raw byte (the level load is emitted
before the `ccm` call instead of after it).

| shape | raw | aligned | size |
|---|---:|---:|---:|
| volatile `Status *st` (master) | 15 | 111 | 11440 |
| `Level *level` before `radioBase` (wave-163) | 16 | 6 | 11440 |
| `Level *level` after `radioBase` | 5196 | 597 | 11392 |
| `Level &level = *this->level` before | 16 | 6 | 11440 |
| `radioBase + radioIndex` (either order) | 16 | 6 | 11440 |
| `Level *const level` | 16 | 6 | 11440 |
| `Status *st` non-volatile + level after | 918 | 109 | 11436 |
| `Status *st` non-volatile + level before | 918 | 114 | 11436 |
| `Status *volatile st` + level after + `st` ccm | 15 | 111 | 11440 |
| `Status *volatile st` + level before + `st` ccm | 31 | 117 | 11440 |
| `Status *volatile st` + level after + `Globals::status` ccm | 361 | 21 | 11440 |
| `Status *volatile st` + level before + `Globals::status` ccm | 360 | 27 | 11440 |
| volatile `Level *level` | 5206 | 601 | 11396 |
| `volatile int dummy` + level after | 5198 | 600 | 11400 |

The 11440-byte body requires the `Level *level` value live across the `ccm`
call (the `level`-after shapes are 11392/11396/11400 and shift the whole
function).  Yet the original loads `this->level` *after* the call
(`ldr.w r1,[fp,#120]` at 0x17f19e).  Clang 7 keeps our load before the call
(`ldr.w r5,[fp,#120]` at 0x17f196); that 4-byte shift is the entire 16-byte
residual.  No source spelling or per-TU flag (`-disable-postra-machine-sink`,
`-disable-postra-machine-licm`, `-enable-misched=false`,
`-machine-sink-split-probability-threshold=0`) rematerialises it after the
call.  The `ldrd` fix is real (5 raw bytes), but the m122 block grows by 6,
net +1 raw.  Master keeps the raw-best `15/111`.

## ModStation::ModStation C1/C2 — literal-load order, fixed in SelectionDAG

The residual is exactly the two `ldr` materialisations of the ternary
operands emitted in the opposite order:

```
orig  d5438 ldr r3,[pc,#632] (YawIPad)   d543a ldr r4,[pc,#636] (Yaw)
ours  d5438 ldr r4,[pc,#632] (Yaw)       d543a ldr r3,[pc,#636] (YawIPad)
```

Both then `mov r0,r3; cmp; it eq; moveq r0,r4`, so the selected pointer and
the stack stores are identical; only the two load/add pairs and the literal
pool slots at `0xd56b4/0xd56b8` are permuted.  The arrays are at the correct
addresses (`gHangarCamYawIPad 0x1fe058`, `gHangarCamYaw 0x1fe080` in both).

Shapes tried (all `43/12`, size exact, unless noted): current ternary;
`!Globals::iPad ? Yaw : YawIPad`; `if/else` both polarities; `YawIPad`
default + `if (!iPad)`; `Yaw` default + `if (iPad)`; `pitchTable`/`yawTable`
declaration swap (`46/13`); `yawBits` read before `pitchBits` (`752/40`,
1000 B); non-volatile reads (`667/59`); plain `float pitch = table[race]`
(`667/59`).  Per-TU flags `-enable-misched=false`,
`-disable-postra-machine-licm`, `-machine-sink-split-probability-threshold=0`,
`-misched-postra=false` are all neutral `43/12`; `-disable-machine-licm` is
`48/15`.  The load order is chosen in SelectionDAG, not the machine
scheduler, so no source spelling reaches it.

## SpaceLounge::drawLounge — clamp association

Right-edge clamp is `addgt.w r1,r6,sl; subgt r5,r0,r1` in the original
(`r1 = width + 2*pad` via the `sl = pad<<1` already live for `pad*2+width`,
then `x = (w-inset) - r1`).  Clang reassociates our
`x = w - inset + (-width - pad - pad)` into
`movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6; addgt.w r9,r1,r0`
(the itttt form).

Shapes tried: current `+ (-width - pad - pad)` **175/41** (best);
`- (width + pad + pad)` 176/42; `- width - pad - pad` 468/64 (1944 B);
`- (width + pad*2)` / `- width - pad*2` / `- (pad*2 + width)` /
`+ (-(pad*2) - width)` all 445/54; `rightEdge` local + `- (width+pad+pad)`
176/42; `rightEdge` + `- (width+pad*2)` 445/54; `totalWidth` local 176/42;
condition reorders (`x+pad+width`, `(pad+x)+width`, `pad+(x+width)`,
`width+x+pad`) all 176/42; `(top + leftMargin)`-style add reorders 445/54.
Per-TU `-disable-postra-machine-licm` 178/43, `-enable-misched=false`
178/43, `-disable-machine-licm` 317/63.  The clamp is a pure RA/reassociation
tie; `pad` lands in `fp` (ours) vs `r9` (orig) because `x` takes `r9`
(ours) vs `r5` (orig); renaming `centerX` to the mutable variable and
declaration reorders do not move it.

## MenuTouchWindow::OnTouchBegin — two RA ties

* case 8 `buttonInsetX` (2 raw bytes): original reloads
  `ldr.w ip,[sl,#340]` for the second-if third condition; ours reuses the
  live local in `r4`.  Using `Globals::layout->buttonInsetX` directly in that
  condition (or all four second-if conditions) is `28/16` / `47/36`; a
  separate `buttonInsetX2` local is `28/16`.  The local is required by the
  first if, so the reload cannot be forced.
* case 15 `heapBufB` block (26 raw bytes): original loads
  `field_0xc_leftMargin` into `r5` after the `ldr r2,[r2,#0]`/`ldr r0,[fp]`
  pair and `field_0x20_top` into `r6`, then `adds r1,r6,r5`; ours hoists
  leftMargin into `r4` before `ldr.w r1,[r0,r1,lsl#2]` and uses `r5` for top
  (`adds r1,r4,r5`).  Shapes tried: direct `Globals::layout->field_0xc_leftMargin`
  (362/76, 1892 B); `(top + leftMargin) + height` / `(leftMargin + top) + height`
  45/23; `(height + leftMargin)` swap 296/35 (1892 B); `volatile` leftMargin
  35/21; `int h = GetImage2DHeight(...)` split 617/155 (756 insn).  The
  hoist is a scheduler/RA choice; no spelling reaches the original order.

## Gun::calcCharacterCollision — RA + stack-colour tie

Baseline `463/57` (1984 B) re-confirmed.  The residuals are the `i*3`
index in `r4` (orig) vs `r5` (ours), the scratch `enemyPosition -
playerPosition` Vector at `sp+104` (orig) vs `sp+44` (ours), and a loop
rotation at `0x153440` where ours emits an inline `i < positions.count`
check (`ldr.w r0,[r9,#180]; ldr r1,[r0]; cmp r5,r1; bcs`) while the
original branches to the shared header (`b.n 0x1534fc`).  The wave-166 note
already swept `int`/`size_t` loop var, pointer arithmetic, swapped
`position`/`velocity` declarations, `data()` locals and per-TU
`-disable-postra-machine-sink`; re-measured and unchanged.  No source win.

## Per-TU flag probes (reverted)

`-mllvm -disable-postra-machine-licm`, `-enable-misched=false`,
`-disable-machine-licm`, `-machine-sink-split-probability-threshold=0`,
`-misched-postra=false` on ModStation.cpp and SpaceLounge.cpp.  Only
`-disable-machine-licm` moves anything (worse).  `CMakeLists.txt` unchanged.

## Conclusion

All five residuals are scheduler / greedy-RA / stack-colouring ties at this
toolchain, not source-shape errors.  The only metric-moving shape is the
wave-163 `MGame::OnUpdate` form (16/6 vs 15/111): it fixes the `ldrd` pair
and 105 alignment diffs but costs +1 raw byte, and the project's raw-first
rule keeps the landed `15/111`.  Tree left clean at the wave-166 baseline.
