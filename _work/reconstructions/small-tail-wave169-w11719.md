# Small tail wave 169 (w11719): ModStation / MenuTouchWindow / MGame re-sweep

Status: **0 `src/` changes landed**.  Worktree `wt/w11719`, branch
`wave169-w11719`, base master `41ef4c8b6` (wave 171).  Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w11719`.

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4507`,
`linked_exact 4507`, size 4524/4524, `data_exact 843/843`, imports 207,
lint CLEAN, parity 5370/5370, `.rel.plt` byte-identical.

Baseline residuals re-confirmed exactly:

| symbol | raw | aligned | size |
|---|---:|---:|---:|
| `_ZN10ModStationC1Ev` / `C2Ev` | 43 | 12 | 1008 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | 28 | 16 | 1896 |
| `_ZN5MGame8OnUpdateEv` | 16 | 6 | 11440 |

Every shape below was built with `-j2` and `ulimit -v 12000000`.
This note extends `small-tail-wave169-w11619.md` and
`small-tail-wave168-w11318.md`; only **new** shapes are recorded here.

## ModStation::ModStation C1/C2 — literal-pool order is decided by the first select's DAG node order

Re-confirmed the residual: the original materialises the two yaw-table
addresses as `ldr r3,=YawIPad; ldr r4,=Yaw` (pool slots 0xd56b4/0xd56b8),
ours as `ldr r4,=Yaw; ldr r3,=YawIPad`.  The select itself is the same
canonical `icmp eq iPad,0; select %eq, Yaw, YawIPad` in both (`it eq;
moveq r0,r4`), and the register assignment (r3=YawIPad, r4=Yaw) is
identical.  The Pos select a few instructions earlier (`camKeyX/Y/Z`) is
byte-identical between original and ours: both materialise the *false*
operand (PosIPad) first.  So the yaw difference is not a global
true/false-visit rule but a per-select DAG/schedule tie.

New shapes this wave (all size exact unless noted):

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| master ternary | 43 | 12 | 1008 | 374 |
| `yawTable = YawIPad; if (!iPad) yawTable = Yaw;` | 43 | 12 | 1008 | 374 |
| `float farYaw = (iPad ? YawIPad : Yaw)[race];` before `yawTable` (GVN merges the two selects) | 714 | 77 | 992 | 367 |
| same, `const void *farYawTable` before `yawTable` (prevents merge; flips pool order to YawIPad-first) | 577 | 70 | 1008 | 376 |
| `const void *farYawTable` + volatile `Globals::iPad` on the farKey select | 43 | 12 | 1008 | 374 |
| `const void *farYawTable` + volatile `Globals::iPad` on the yawTable select | 533 | 70 | 1008 | 375 |
| `const float *farYawTable` before `yawTable` (GVN merges) | 604 | 71 | 984 | 366 |
| `float pitch` declared before `float yaw` | 43 | 12 | 1008 | 374 |
| `float farPitch = pitch;` used by farKey | 43 | 12 | 1008 | 374 |
| pitch/yaw bitcasts inlined in both calls | 43 | 12 | 1008 | 374 |
| farKey uses the nearKey `yaw` value (second select eliminated) | 604 | 71 | 984 | 366 |

The `const void *` form is the only one that flips the literal-pool order
while keeping 1008 B, but it costs +2 instructions: clang spills
`Globals::iPad` to `[sp,#172]` and reloads it instead of re-reading
`[sl]`.  Forcing a volatile reload on either select removes the spill but
either restores the Yaw-first order (farKey volatile) or shifts the
Pitch slot behind YawIPad (yawTable volatile, 533/70).  The original's
`Pitch, YawIPad, Yaw` pool order is not reachable from source at this
toolchain: it needs the YawIPad `GlobalAddress` node created before the
select while the select stays canonical `icmp eq` with true=Yaw, and
every construct that creates the node early either merges the selects or
changes the compare/select lowering.

## MenuTouchWindow::OnTouchBegin — 29/15 near-miss, case-8 `ip`/`r4` tie

Residual re-decoded: (a) case 8 4th condition `ldr.w ip,[sl,#340]` vs
`ldr.w r4,[sl,#340]` + `add r3,ip` vs `add r3,r4` (4 raw bytes, pure RA);
(b) case 15 first condition register permutation (`r3/r6/r2/r5` vs
`r2/r3/r6/r5`) because our `Layout &layout` local pins the layout pointer
in `r6`; (c) case 15 second condition reads `leftMargin` from the cached
`r6` (orig reloads `Globals::layout` into `r2` and reads both fields
from it).

New shapes this wave (all size exact unless noted):

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| master (`layout.top + (layout.leftMargin + ...)`) | 28 | 16 | 1896 | 754 |
| `layout.field_0x20_top + (Globals::layout->field_0xc_leftMargin + ...)` | **29** | **15** | 1896 | 754 |
| same, first cond via `layout.buttonInsetX` | 29 | 15 | 1896 | 754 |
| `Globals::layout->field_0xc_leftMargin + layout.field_0x20_top + ...` | 45 | 23 | 1896 | 754 |
| both case-15 fields via `Globals::layout` (local removed) | 362 | 76 | 1892 | 752 |
| first cond via local, both fields global | 362 | 76 | 1892 | 752 |
| both fields global, second `if` via `layout` | 322 | 74 | 1888 | 751 |
| `PaintCanvas *canvas = Globals::Canvas` local for the three calls, both fields global | 359 | 75 | 1892 | 752 |
| `canvas` local + `layout` local kept live by the second `if` | 388 | 81 | 1892 | 751 |
| `canvas` local + master second cond (`layout.leftMargin`) | 360 | 80 | 1892 | 752 |
| `layout` local used in both `if`s (live across all calls), both fields global | 322 | 74 | 1888 | 751 |

The 29/15 shape is a raw-neutral/aligned-better near-miss: it moves the
`leftMargin` load onto the reloaded `Globals::layout` (matching the
original) but leaves `top` on the cached `r6`, so the `adds` operand
order still differs.  Raw is +1, so master is kept (raw is ground truth).
Removing the local entirely lets the compiler pick `fp = &Globals::layout`
instead of `fp = &Globals::Canvas`, which loses the two-instruction
`[sp,#16]` spill/reload pair (1892 B).  Keeping the local live across the
first `GetImage2DWidth` call is what pins `fp = &Globals::Canvas`; there
is no source shape that both keeps the local live and reads both case-15
fields from the reload.

## MGame::OnUpdate — level-load hoist is coupled to the 11440 frame

Re-confirmed the wave-163 shape is the size-exact optimum.  New shapes:

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| master (nextInt, `Level *level`, radioBase) | 16 | 6 | 11440 | 4002 |
| `Level *level` before nextInt | 33 | 8 | 11440 | 4002 |
| nextInt, `int mission = ccm()`, `Level *level`, radioBase | 5196 | 597 | 11392 | 3974 |

Moving the level load after the `getCurrentCampaignMission` call (the
original's `ldr.w r1,[fp,#120]`) always drops the frame to 11392 and
loses 24 instructions elsewhere; the 11440 anchor is the whole function's
stack colouring, so the hoist cannot be removed without a compensating
source change elsewhere in `OnUpdate`.

## Conclusion

No new size-exact raw win.  ModStation's pool order is a SelectionDAG
tie that every early-YawIPad construct either merges away or re-schedules;
MenuTouchWindow's 29/15 shape is the closest structural match but +1 raw;
MGame's level-load hoist is coupled to the 11440-byte frame.  Tree left
clean at the wave-171 baseline (`RATCHET PASS`).
