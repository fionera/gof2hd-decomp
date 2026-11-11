# Small tail wave 168 (w11318): five residual re-sweep, no new source win

Status: **0 `src/` changes landed**.  Worktree `wt/w11318`, branch
`wave168-w11318`, base master `631c26f0b` (wave 167).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11318`.

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
incremental link is ~1.1 s.  This note extends
`small-tail-wave167-w11217.md` (same five residuals) and only records the
**new** shapes tried this wave; the w11217 table still stands.

## ModStation::ModStation C1/C2 — literal-load order is a SelectionDAG tie

Residual re-decoded: the two `ldr` literal materialisations of the ternary
operands are emitted in the opposite order, and the literal-pool slots
`0xd56b4/0xd56b8` follow that order:

```
orig  d5438 ldr r3,[pc,#632] (YawIPad)   d543a ldr r4,[pc,#636] (Yaw)
ours  d5438 ldr r4,[pc,#632] (Yaw)       d543a ldr r3,[pc,#636] (YawIPad)
```

The IR is identical in both (`select i1 (icmp eq iPad,0), Yaw, YawIPad`,
i.e. the true operand is the non-iPad table).  Clang 7's InstCombine
*always* canonicalises `u ? A : B` to `icmp eq u,0; select %eq, B, A`, so
the true operand is always the `u==0` value; the original materialises the
`u!=0` value first.  New shapes tried (all size exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| `yawTable` after `pitchBits` (S1) | 698 | 52 | 1000 |
| direct ternary in `yawBits` read (S2) | 698 | 52 | 1000 |
| no `yawTable` local at all (S3) | 698 | 52 | 1000 |
| `yawBits` read before `pitchBits` (S8) | 752 | 40 | 1000 |
| `const float *const yawTable` (S5) | 43 | 12 | 1008 |
| `&gHangarCamYawIPad[0]` arms (S6) | 43 | 12 | 1008 |
| `*(yawTable + race)` (S7) | 43 | 12 | 1008 |
| local `yawIPad` / `yawStd` arm pointers (T1-T4) | 43 | 12 | 1008 |
| `iPad == 1 ? YawIPad : Yaw` (non-canonical IR) | 696 | 55 | 1000 |
| `(iPad & 1) ? ...` | 572 | 24 | 1008 |
| no `pitchTable` local (X1) | 46 | 13 | 1008 |
| `yawTable` declared before `pitchTable` (X2) | 46 | 13 | 1008 |
| `&gHangarCamPitch[0]` (X3) | 43 | 12 | 1008 |
| `*(yawTable + race)` for yawBits (X4) | 43 | 12 | 1008 |

`iPad == 1` is the only spelling that produces the original's load order
(true operand = YawIPad), but it changes the compare to `cmp #1` and the
whole select lowering (`strd`, 1000 B).  Pre-RA scheduler probes on the TU
(`-pre-RA-sched=list-ilp`, `source`, `default`, `list-hybrid`,
`-disable-block-placement`) are all neutral `43/12`; `-regalloc=pbqp/basic`
on the TU is also neutral.  The order is fixed in SelectionDAG, not the
scheduler.

## MenuTouchWindow::OnTouchBegin — two RA ties, new shapes neutral

Residual re-decoded (same two runs as w11217):

* case 8 `this->listEntryWidth` reload: orig `ldr.w ip,[sl,#340]` vs ours
  `ldr.w r4,[sl,#340]` (2 raw bytes; `ip` vs `r4`, same load).
* case 15 `heapBufB` block (26 raw bytes): orig keeps `Globals::w` in `r5`
  and `layout->buttonInsetX` in `r2`; ours uses `r4`/`r5`.  Orig also
  reloads `Globals::layout` into `r2` after the `GetImage2DWidth` call and
  reads both `field_0xc_leftMargin` and `field_0x20_top` from that reload;
  ours reads `leftMargin` from the pre-call reference (`r6`).

New shapes (all size exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| case-15 first cond parenthesised `w - (inset + width)` | 90 | 26 | 1896 |
| case-15 first cond via locals `wNow/insetNow/widthNow` | 90 | 26 | 1896 |
| case-15 first cond `x + width + inset > w` | 363 | 91 | 1888 |
| case-15 second cond `Globals::layout->leftMargin` (ref removed) | 362 | 76 | 1892 |
| case-15 `Layout *layout` pointer local | 28 | 16 | 1896 |
| case-15 `layout.buttonInsetX` for first cond | 28 | 16 | 1896 |
| case-15 second cond reversed `(height + leftMargin)` | 296 | 35 | 1892 |
| case-15 second cond no parens | 45 | 23 | 1896 |
| case-15 second cond `layout.field_0x20_top` (ref) | 308 | 73 | 1892 |
| case-15 second cond both ref fields | 308 | 73 | 1892 |
| case-8 4th cond term permutations (5 of 6) | 28-34 | 16-18 | 1896 |
| case-8 `listEntryWidth` first / mid / local | 34 / 28 / 34 | 18 / 16 / 18 | 1896 |
| case-8 `Globals::layout->buttonInsetX` in 3rd cond | 28 | 16 | 1896 |
| case-8 `Globals::layout->field_0x20_top` in 4th cond | 28 | 16 | 1896 |

`-regalloc=pbqp` and `-regalloc=basic` on the TU are neutral `28/16`.
The `ip`/`r4` choice and the case-15 live-range split are greedy-RA ties.

## SpaceLounge::drawLounge — clamp association is a reassociation tie

Right-edge clamp: orig `addgt.w r1,r6,sl; subgt r5,r0,r1`
(`x = (w-inset) - (width + 2*pad)`, reusing the `sl = pad<<1` already live
for `top`); ours `movgt r1,#0; subgt.w r1,r1,fp,lsl#1; subgt r1,r1,r6;
addgt.w r9,r1,r0` (`x = (w-inset) + (-2*pad - width)`).  New shapes:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| `- width - pad - pad` | 468 | 64 | 1944 |
| `- (width + pad*2)` | 445 | 54 | 1948 |
| `right` local + `- (width+pad+pad)` | 176 | 42 | 1948 |
| `right` local + `- width - pad - pad` | 468 | 64 | 1944 |
| `- width - (pad+pad)` | 445 | 54 | 1948 |
| `- (pad+pad+width)` | 445 | 54 | 1948 |
| `w - (inset + width + pad + pad)` | 477 | 77 | 1952 |
| `+ (-(width+pad+pad))` | 176 | 42 | 1948 |
| `- (pad*2 + width)` | 445 | 54 | 1948 |
| `total = width+pad+pad; right - total` | 176 | 42 | 1948 |
| `total = pad+pad+width; right - total` | 445 | 54 | 1948 |
| `total` then `w - inset - total` | 176 | 42 | 1948 |
| `(w - inset) - (width+pad+pad)` | 176 | 42 | 1948 |
| `total = width + pad*2; w - inset - total` | 445 | 54 | 1948 |

Clang 7 `-Oz` always reassociates `(a - b) - c` into `a + (-b - c)` and
rematerialises `pad*2` from `fp` instead of reusing the live `sl`; the
`width + sl` form is unreachable from source.  Master keeps `175/41`.

## Gun::calcCharacterCollision — re-confirmed, no new shape

Baseline `463/57` (1984 B) re-confirmed.  The three residuals are the
`i*3` index in `r4` (orig) / `r5` (ours), the scratch
`enemyPosition - playerPosition` Vector at `sp+104` (orig) / `sp+44`
(ours), and the outer-loop rotation at `0x153440` (orig `b.n 1534fc` to the
shared header; ours duplicates the `i < positions.count` check inline).
All are RA / stack-colouring / loop-rotation ties.  No new source shape was
found this wave.

## MGame::OnUpdate — volatile anchor remains the raw optimum

Baseline `15/111` (11440 B) re-confirmed.  Residual is the m23 64-bit
`ldrd r2,r3` (orig) / `ldrd r3,r5` (ours) plus the m122 volatile `st`
spill/reload.  New shapes:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| `long long lastMark` local in m23 | 5048 | 411 | 11432 |
| `long long idleFor` local in m23 | 5009 | 721 | 11452 |
| `Status *st23` local in m23 | 5041 | 277 | 11432 |
| wave-163 (`Level *level` before `radioBase`, direct status) | 16 | 6 | 11440 |
| wave-163 + volatile `st` | 31 | 117 | 11440 |
| direct status + `Level *level` after `radioBase` (no anchor) | 5196 | 597 | 11392 |

The wave-163 shape is the only size-exact shape that fixes the m23 `ldrd`
(16/6), but it costs +1 raw byte because the `Level *level` load is hoisted
before the `ccm` call; the original loads it after (`ldr.w r1,[fp,#120]` at
`0x17f19e`).  Raw-first keeps master's `15/111`.

## Conclusion

All five residuals are SelectionDAG / reassociation / greedy-RA /
stack-colouring / loop-rotation ties at this toolchain, not source-shape
errors.  No new size-exact raw win was found; tree left clean at the
wave-167 baseline.
