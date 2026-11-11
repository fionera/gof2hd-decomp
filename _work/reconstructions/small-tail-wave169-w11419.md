# Small tail wave 169 (w11419): three-residual re-sweep, no new source win

Status: **0 `src/` changes landed**.  Worktree `wt/w11419`, branch
`wave169-w11419`, base master `ec3ceb07c` (wave 168 docs).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11419`.

Gate on entry and exit: baseline unchanged (`byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, drift 0, parity 5370/5370, `.rel.plt` byte-identical).  No
`src/` file differs from master; the tree is clean.

Metrics: raw same-address diff bytes and instruction-index alignment
(`tools/fn_diff.py`).  Baseline residuals re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` / `C2Ev` | 43 | 12 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 15 | 111 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`; the
incremental link is ~1.1 s.  This note extends
`small-tail-wave168-w11318.md`; the w11217/w11318 tables still stand.

## MGame::OnUpdate — root cause of the m23 `ldrd` pair found; still 15 raw

The 15 raw bytes are two runs:

* `0x17e2bf..0x17e2c9` (5 bytes): m23 64-bit `ldrd r2,r3` (orig) vs
  `ldrd r3,r5` (ours).
* `0x17f18c..0x17f196` (10 bytes): the m122 volatile `st` store/reload
  (`str r2,[sp,#120]` + `ldr r0,[sp,#120]`) replacing the original's
  single `ldr.w r0,[r9]`.

**New diagnosis of the `ldrd` tie.**  The `volatile Status *st` store is
emitted at `0x17f18c`, *before* the `nextInt` call, and the value it
stores is the `Globals::status` pointer that the m23 condition already
loaded into `r2` at `0x17e2b6`.  Because the volatile store cannot be
CSE'd, `r2` must stay live from m23 all the way to m122, so the m23
`ldrd` is forced off `r2` (`r3,r5`) and the zero constant moves to `r3`.
That also explains why the wave-163 `Level *level` shape (no volatile)
fixes m23: it has no long-lived `r2`.

The `Level *level` local before the ccm is what forces the GOT base into
`r9` and keeps size 11440 (the natural source form is 11392 and CSEs
35 `Globals::status` reloads).  Its only cost is that clang emits the
level load before the `ccm` call (`ldr.w r5,[fp,#120]` at `0x17f196`)
instead of after it (`ldr.w r1,[fp,#120]` at `0x17f19e`), which is the
whole 16-byte residual of the 16/6 shape.

Shapes tried this wave (all size exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master: `Status *volatile st` + `Level *level` | 15 | 111 | 11440 |
| `Level *level` before ccm, direct `Globals::status` (wave-163) | 16 | 6 | 11440 |
| `Level *level` before ccm + `long long radioIndex` | 16 | 6 | 11440 |
| natural form (`this->level` direct, no volatile/local) | 5196 | 597 | 11392 |
| `Level *level` after ccm / `this->level` direct | 5196 | 597 | 11392 |
| `int mission = ...ccm...; Level *level; radioBase = mission...` | 5196 | 597 | 11392 |
| `volatile Status *st` + `this->level` direct | 15 | 111 | 11440 |
| `Status *st = *(Status *volatile *)&Globals::status` + level before ccm | 16 | 6 | 11440 |
| same, volatile load after `nextInt` | 16 | 6 | 11440 |
| `Level *level` then `nextInt` then volatile-load `st` | 33 | 8 | 11440 |
| `st` then `nextInt` then `Level *level` (volatile load) | 362 | 20 | 11440 |
| `Status *volatile st = *(Status *volatile *)&Globals::status` | 5205 | 600 | 11396 |
| `Status *st` non-volatile + level after | 918 | 109 | 11436 |
| `Status *st` non-volatile + volatile anchor after ccm | 915 | 110 | 11436 |
| `Status *st` used at the shared store tail | 5249 | 757 | 11488 |
| `st` declared after `nextInt` (volatile local) | 5206 | 599 | 11396 |

m23-expression probes (all neutral `15/111`): constant on the left
(`3600000 < diff`), `>= 3600001`, `3600000LL`, parenthesised, pointer
spelling `*(long long *)&field`.  `field < getPlayingTime() - 3600000`
is `3594/307`; `getPlayingTime() > field + 3600000` is `4469/384/11436`.
Volatile field reads are `4485/395/11444`.  `Status *volatile st`
spelled via casts (`(Status *)(uintptr_t)`, `&*`, `0 +`) are all
neutral `15/111`.

The 15/111 shape is a genuine local optimum: it is the only size-exact
shape with both the m122 level load *after* the ccm and the GOT base in
`r9`.  The m23 `ldrd` and the m122 store/reload are two faces of the
same long-lived `r2`; removing the volatile store costs +1 raw byte
(16/6), so raw-first keeps 15/111.

## ModStation::ModStation C1/C2 — load order is a SelectionDAG operand tie

Residual re-decoded: the two `ldr` literal materialisations of the
ternary operands are emitted in the opposite order, and the literal-pool
slots `0xd56b4/0xd56b8` follow that order:

```
orig  d5438 ldr r3,[pc,#632] (YawIPad)   d543a ldr r4,[pc,#636] (Yaw)
ours  d5438 ldr r4,[pc,#632] (Yaw)       d543a ldr r3,[pc,#636] (YawIPad)
```

Both then `mov r0,r3; cmp #0; it eq; moveq r0,r4`, so the select is
semantically identical; only the two load/add pairs and the literal
slots are permuted.  The wave-168 conclusion (InstCombine canonicalises
`u ? A : B` to `icmp eq u,0; select %eq, B, A`, so the DAG emits the
`u==0` arm first) is confirmed.

New shapes this wave (size exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master ternary | 43 | 12 | 1008 |
| `if/else` (both polarities) | 43 | 12 | 1008 |
| `Globals::iPad == 0 ? Yaw : YawIPad` | 43 | 12 | 1008 |
| `Globals::iPad != 0 ? YawIPad : Yaw` | 43 | 12 | 1008 |
| `(bool) Globals::iPad ? ...` | 43 | 12 | 1008 |
| `(Globals::iPad ? true : false) ? ...` | 43 | 12 | 1008 |
| `Globals::iPad > 0 ? ...` | 43 | 12 | 1008 |
| `(Globals::iPad & 1) ? ...` | 572 | 24 | 1008 |
| `(~Globals::iPad & 1) ? ...` | 572 | 24 | 1008 |
| `((Globals::iPad + 1) & 1) ? ...` | 572 | 24 | 1008 |
| `(Globals::iPad ^ 1) ? Yaw : YawIPad` | 696 | 57 | 1000 |
| `Globals::iPad == 1 ? YawIPad : Yaw` | 696 | 55 | 1000 |
| `== 1` + farKey `yawTable[race]` | 604 | 71 | 984 |

`== 1` is the only spelling that produces the original load order
(`cmp #1`), but it changes the select lowering and the frame.  The
`& 1` spellings keep size but emit `tst #1` and reorder the whole block.
No new size-exact raw win.

## MenuTouchWindow::OnTouchBegin — two RA ties, new shapes neutral

Residual re-decoded (same two runs as w11217/w11318):

* case 8 `0x129fd2`: `ldr.w ip,[sl,#340]` (orig) vs `ldr.w r4,[sl,#340]`
  (ours), and `add r3,ip` vs `add r3,r4` — 2 raw bytes, pure greedy-RA.
* case 15 `0x12a1e0..`: orig reloads `Globals::layout` after the
  `GetImage2DWidth` call and reads both `field_0xc_leftMargin` and
  `field_0x20_top` from that reload; ours reads `leftMargin` from the
  cached pointer kept by the `Layout &layout` reference.

New shapes this wave (size exact unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master | 28 | 16 | 1896 |
| case-8 4th cond `listEntryWidth` first | 34 | 18 | 1896 |
| case-8 4th cond `leftMargin` first | 28 | 16 | 1896 |
| case-8 4th cond parenthesised | 28 | 16 | 1896 |
| case-8 4th cond `listEntryWidth` local | 28 | 16 | 1896 |
| case-8 3rd+4th cond via `Globals::layout` | 28 | 16 | 1896 |
| case-15 second cond swapped (top via ref, leftMargin via global) | 29 | 15 | 1896 |
| case-15 second cond both via `Globals::layout` (ref removed) | 362 | 76 | 1892 |
| case-15 `Layout *&layout = Globals::layout` | 362 | 76 | 1892 |
| case-15 `Layout *const &layout` | 362 | 76 | 1892 |
| case-15 `Layout *volatile &layout` | 127 | 76 | 1896 |
| case-15 `Layout *volatile layout` | 1759 | 258 | 1924 |
| case-15 `bool hitX` + `Layout *l` after first call | 369 | 96 | 1892 |
| case-15 `hitX` + both fields via `Globals::layout` | 362 | 76 | 1892 |
| case-15 third cond via reference | 291 | 74 | 1892 |

The reload form is 1892 bytes because the compiler hoists
`&Globals::layout` into `fp` and turns each reload into one `ldr [fp]`
instead of `ldr [sp,#16]; ldr [r2]`, saving two instructions globally.
The master 1896 form keeps the layout pointer in `r6` and reloads from
`[sp,#16]`, which matches the original instruction count but caches
`leftMargin`.  `Layout *volatile &layout` restores the reload structure
and size 1896, but the volatile reloads reorder the whole case-15 block
(127 raw).  The `ip`/`r4` choice in case 8 is a greedy-RA tie.

## Conclusion

All three residuals are SelectionDAG operand-order / greedy-RA ties at
this toolchain.  The MGame `ldrd` tie now has a concrete root cause (the
long-lived `r2` created by the m122 volatile store), but every shape that
removes it costs at least +1 raw byte.  No size-exact raw win was found;
the tree is left clean at the wave-168 baseline.
