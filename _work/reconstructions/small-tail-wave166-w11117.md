# Small tail wave 166 (w11117): five residual re-sweep, no new source win

Status: **0 `src/` changes landed**.  Worktree `wt/w11117`, branch
`wave166-w11117`, base master `85c1f88df` (wave 165).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11117`.

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

## What each residual actually is

### `ModStation::ModStation` C1/C2 — root cause re-diagnosed
The wave-164 note called this a literal-pool-order tie.  It is not a
`.rodata` order problem: the arrays are in the original order
(`gHangarCamYawIPad` at `0x1fe058`, `gHangarCamYaw` at `0x1fe080`) in both
binaries.  The *function-local* literal pool differs because the scheduler
loads the two table pointers in the opposite order:

```
orig  d5438 ldr r3,[pc,#632] (YawIPad)   d543a ldr r4,[pc,#636] (Yaw)
ours  d5438 ldr r4,[pc,#632] (Yaw)       d543a ldr r3,[pc,#636] (YawIPad)
```

Both then `mov r0,r3; cmp; it eq; moveq r0,r4`, so the selected pointer and
the stack stores are semantically identical; only the two load/add pairs are
permuted.  Every source spelling tried (ternary, if/else, `!iPad` inverted,
explicit pointer locals, `__builtin_expect`, declaration reorder, array
declaration swap, `yawTable` reuse in the farKey call) leaves the scheduler
choice unchanged at `43/12` (or worsens it).  Pure scheduler/RA tie.

### `MenuTouchWindow::OnTouchBegin` — two RA ties
`0x3fe` case-8 `buttonInsetX`: original keeps it in `ip`, ours in `r4`
(`ldr.w ip,[sl,#340]` vs `ldr.w r4,[sl,#340]`).  `0x60c..` case-15
`heapBufB[field_0x1e0]` block permutes `r2/r3/r6` and hoists the
`field_0xc_leftMargin` load before the `GetImage2DHeight` call.  Direct
`Globals::layout`, local `Layout *`, `const`, declaration reorder,
`__builtin_expect` on the case-8 guard, and condition-sum reorder all stay
at `28/16` or worse.

### `SpaceLounge::drawLounge` — clamp association
Right-edge clamp is `addgt.w r1,r6,sl; subgt r5,r0,r1` in the original
(`r1 = width + 2*pad`, then `x = (w-inset) - r1`).  Clang reassociates our
`x = w - inset + (-width - pad - pad)` into
`movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6; addgt.w r9,r1,r0`.
`-(width+pad+pad)`, `-(pad+pad+width)`, `-(width+2*pad)`, plain left-assoc,
two-statement `x = w-inset; x -= width+pad+pad;`, named `totalWidth` /
`rightEdge` / `pad2` locals all still emit the itttt form; the best faithful
spelling (`-(width+pad+pad)`) is `176/42` (condition sum order worsens by 1
byte).  Base `175/41` remains best.

### `Gun::calcCharacterCollision` — RA + stack-colour tie
`i*3` in `r4` (orig) vs `r5` (ours); the `enemyPosition - playerPosition`
scratch Vector is `sp+104` (orig) vs `sp+44` (ours).  `int`/`size_t` loop
var, pointer arithmetic, swapped `position`/`velocity` declarations, and
`data()` locals were re-tried; base `463/57` best (`int i` 464/58,
`swap_decl` 478/80, `ptr_locals` 1657/182/1992 B).

### `MGame::OnUpdate` — volatile anchor is a local optimum
Baseline `volatile Status *st` is `15/111`.  The wave-163 shape
(`Level *level` before `radioBase`, direct `Globals::status`) is `16/6` —
one raw byte worse but structurally faithful.  The residual is:
* `ldrd r2,r3` / `mov.w r2,#0` (orig) vs `ldrd r3,r5` / `mov.w r3,#0`
  (ours) in the m23 64-bit idle-time compare (`0x19e4`), and
* the volatile `st` spill/reload at `0x28b4` (`str r2,[sp,#120]` +
  `ldr r0,[sp,#120]`) replacing the original `ldr.w r0,[r9]`
  (`&Globals::status` rematerialisation).

Swept: every `st`/`Globals::status` receiver combination for the ccm call
and the final `field_0x100`/`getPlayingTime` pair, plain/const/volatile/
`*(Status *volatile *)&Globals::status` pointers, `volatile int
radioIndex`/`radioBase`, `volatile Level *level`, declaration orders,
`__builtin_expect` on the m23 guard, `3600000` spellings, and
`-mllvm -disable-postra-machine-sink` on the TU.  The only shape that keeps
the 11440-byte frame and the `[r9]` reload is `volst_unused` (store emitted,
st never read) at `361/21/11440 insn 4002/4002`; its 2-byte store shifts
every following branch offset, so raw explodes even though aligned is 21.
No shape beats `15/111` on raw.

## Per-TU flag probes (reverted)

`-mllvm -disable-postra-machine-sink` on ModStation.cpp `179/34`, on
MenuTouchWindow.cpp `90/36`, on Gun.cpp `518/62`, on MGame.cpp `26/116`,
and on SpaceLounge.cpp (on top of its branch-weight flags) `312/65`.
`-mllvm -likely-branch-weight=7 -mllvm -unlikely-branch-weight=1` on
ModStation.cpp is neutral `43/12`.  All reverted; `CMakeLists.txt`
unchanged.

## Conclusion

All five residuals are pure scheduler / greedy-RA / stack-colouring ties at
this toolchain, not source-shape errors.  No size-exact raw win was found;
the tree is left clean at the wave-165 baseline.
