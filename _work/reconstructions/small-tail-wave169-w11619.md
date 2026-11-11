# Small tail wave 169 (w11619): ModStation / MenuTouchWindow / MGame re-sweep

Status: **0 `src/` changes landed**.  Worktree `wt/w11619`, branch
`wave171-w11619` (task label wave169), base master `50716a763` (wave 170).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/dev/shm/w11619`.

Gate on exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, parity 5370/5370, `.rel.plt` byte-identical.

Baseline residuals re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` / `C2Ev` | 43 | 12 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`.

## Clean-rebuild correction: `.rodata` layout does match

The w11519 note reported the yaw-table literal-pool slots `0xd56b4/0xd56b8`
as carrying `Yaw`/`YawIPad` (opposite of the original).  A clean rebuild
confirms the `.rodata` **addresses** of `gHangarCamPosIPad` (0x1fdf40),
`gHangarCamPos` (0x1fdfb8), `gHangarCamPitch` (0x1fe030), `gHangarCamYaw`
(0x1fe058, 2.5 at index 8) and `gHangarCamYawIPad` (0x1fe080, 2.35 at
index 8) are **identical** to the original, and the literal-pool *slot
order* is the only literal difference: the original stores `YawIPad` in
`0xd56b4` (loaded into `r3` first), ours stores `Yaw` there (`r4` first).
That is 2 raw bytes.  The remaining 41 raw bytes are the instruction
schedule of the nearKey block (same instruction multiset, permuted).

## ModStation::ModStation C1/C2 — still a SelectionDAG/schedule tie

Residual: `ldr r3,=YawIPad; ldr r4,=Yaw` (orig) vs `ldr r4,=Yaw;
ldr r3,=YawIPad` (ours), plus the surrounding `add`/`ldrb`/`ldr.w`/`str`
schedule permutation.  New shapes this wave (all size exact):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master ternary | 43 | 12 | 1008 |
| `yawTable = Yaw; if (iPad) yawTable = YawIPad;` | 43 | 12 | 1008 |
| `if (iPad) ... else ...` (then=YawIPad) | 43 | 12 | 1008 |
| `switch (iPad) { case 0: Yaw; default: YawIPad; }` | 43 | 12 | 1008 |
| `*(const bool *)&iPad ? YawIPad : Yaw` | 43 | 12 | 1008 |
| `int ipad = iPad; ipad ? YawIPad : Yaw` | 43 | 12 | 1008 |
| `bool ipad = iPad; ipad ? YawIPad : Yaw` | 43 | 12 | 1008 |
| `iPad > 0 ? YawIPad : Yaw` | 43 | 12 | 1008 |
| `yawBits` non-volatile | 47 | 20 | 1008 |
| `pitchBits` non-volatile | 668 | 61 | 1008 |

The `if`/`else` form keeps the correct `.rodata` order (`YawIPad` first,
because the then-branch is the first AST reference) but SelectionDAG
still visits the canonicalised `select (icmp eq), Yaw, YawIPad` true
operand (`Yaw`) first, so both the literal slot and the load order stay
`Yaw`-first.  `InstCombine` canonicalises every `icmp ne` select to
`icmp eq` with swapped arms; only `iPad == 1` (tried in earlier waves)
avoids it, but changes the compare to `cmp #1` and the size to 1000.

## MenuTouchWindow::OnTouchBegin — case-15 cached-vs-reloaded `leftMargin`

The 28 raw bytes are (a) case 8 `ldr.w ip,[sl,#340]` vs `r4` (2 bytes)
and (b) the case-15 block (26 bytes): the original reloads
`Globals::layout` after `GetImage2DWidth` and reads **both**
`field_0xc_leftMargin` and `field_0x20_top` from that reload; ours caches
`leftMargin` in `r6` from the pre-call `Layout &layout` reference.
New shapes this wave:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master (`Globals::layout->top + (layout.leftMargin + ...)`) | 28 | 16 | 1896 |
| `layout.top + (Globals::layout->leftMargin + ...)` | 29 | 15 | 1896 |
| `layout.top + (layout.leftMargin + ...)` | 308 | 73 | 1892 |
| both fields via `Globals::layout` (parens kept) | 362 | 76 | 1892 |
| both fields via `Globals::layout` (no local at all) | 362 | 76 | 1892 |
| `Layout **layoutPtr = &Globals::layout` + `(*layoutPtr)->` | 362 | 76 | 1892 |
| `Globals::layout` first cond + both fields global | 362 | 76 | 1892 |
| first cond via `layout`, both fields global | 362 | 76 | 1892 |
| second if via `layout` as well | 322 | 74 | 1888 |
| top-level `layout` used for leftMargin | 1781 | 818 | 1912 |
| case-8 4th-cond `a+b+c` permutations (6/6) | 28-33 | 16-17 | 1896 |

The reload form is structurally faithful but loses exactly two
instructions: the original spills the *address* of `Globals::layout` to
`[sp,#16]` and reloads it (`ldr r2,[sp,#16]; ldr r2,[r2,#0]`), while the
reload form keeps it in `fp` (`ldr.w r2,[fp]`).  That is a greedy-RA
choice between `&Globals::layout` and `&Globals::Canvas` for `fp`.
`Layout **` and `if`-form variants do not force the spill.  The
`layout.top + (Globals::layout->leftMargin + ...)` form is the closest
structurally (aligned 15) but is +1 raw byte, so master is kept.

## MGame::OnUpdate — the 16/6 form is the size-exact optimum

The m122 block is already the wave-163 "level mid" shape.  Re-confirmed
that any shape which loads `this->level` after the
`getCurrentCampaignMission()` call (including a `volatile` reload) makes
the whole function 11392 bytes: the stack-slot colouring moves to lower
offsets (`sp+340/128/876/740` vs `sp+468/256/1004/868`) and the
`inBlackMarketSystem` shared tail is laid out differently, so 24
instructions vanish elsewhere.  New shapes:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| master (`level` before radioBase, direct status) | 16 | 6 | 11440 |
| `Level *level` first, then nextInt/radioBase | 33 | 8 | 11440 |
| `Level *level`, `radioBase`, `radioIndex` | 1239 | 170 | 11436 |
| nextInt, `radioBase`, `level` (natural) | 5196 | 597 | 11392 |
| nextInt, `radioBase`, `Level *volatile` reload | 5196 | 597 | 11392 |
| nextInt, `radioBase`, `this->level->...` | 5196 | 597 | 11392 |
| `radioBase = ccm(); radioBase = ...` | 705 | 116 | 11440 |
| `radioBase = 3166; if (ccm()==122) radioBase = 3164;` | 705 | 116 | 11440 |

The 11440 anchor is therefore not a live-range of `level` but the whole
function's stack colouring / shared-tail layout; the original's
`ldr.w r1,[fp,#120]` after the call is unreachable from source at this
toolchain without losing the anchor.

## Conclusion

No new size-exact raw win.  ModStation's load order is fixed by
`InstCombine`'s `icmp ne -> icmp eq` select canonicalisation;
MenuTouchWindow's reload form is 2 instructions short because of a
greedy-RA `fp` assignment; MGame is at its 11440-byte local optimum.
Tree left clean at the wave-170 baseline.
