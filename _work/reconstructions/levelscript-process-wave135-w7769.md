# LevelScript::process — wave 135 / w7769 (case-64 body + 2-call anchor: size puzzle)

Status: **analysis note, no landed source change**, 2026-11-07. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**. Owned TU: `src/game/world/LevelScript.cpp`.
Base master `bbffef482` (wave-134 merge). Scratch helpers in `/dev/shm/w7769/`
(not tracked): `harness.py`, `case70.py`, `anchor_gen.py`, `regions.py`,
`callcount.py`, `findsym.py`, `relplt.py`.

## Result

```
                         before (master)   after (no change)
official verify pct      58.2              58.2
orig / ours size         57232 / 57232     57232 / 57232   (size-exact)
process diffbytes        54359             54359
whole .text diffbytes    85022             85022
.ARM.extab diffbytes     1416              1416
.rel.plt                 exact             exact
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

## The key premise correction: the case-64 body is already in master

The wave-134 base **already contains** the faithful case-64 state-1 body
(`forceUpdate(delta,false)` + `translate(0.0f, delta * 0.2, 0.0f)`, where
`0.2` is a `double` literal, so clang emits `vcvt.f64.s32 / vmul.f64 /
vcvt.f32.f64`).  It landed in w7750 (`700b80556`) and survived the merge
reconciliation `aa9b7c395`.  Therefore the w7743 landing on this base is only:

* the three `m_pLevel->getPlayer()` -> `player` fixes (`-16 B`), and
* the 2-call anchor (`switchSkyboxForIntro` + `PlayerStation::setVisible`,
  `-48 B`).

Net **-64 B** (57168), not 57232.  The explicit `(float)((double)delta*0.2)`
spelling is byte-identical to `delta * 0.2` (measured: 0 B, 0 diffbytes), so
the "body" contributes nothing on this base.

## Measured variants (all on wave-134 master; size / process diffbytes)

| variant | size | diffbytes | .extab | .rel.plt |
|---|---:|---:|---|---|
| base | 57232 | 54359 | exact | exact |
| body (explicit cast) | 57232 | 54359 | exact | exact |
| gp fixes | 57216 | 55261 | exact | exact |
| 2-call anchor (B+G) | 57184 | 55145 | exact | exact |
| body + gp + 2-call anchor | 57168 | 55086 | exact | exact |
| + case-40 state-1 `translate(0,0,-2*delta)` (+36) | 57204 | 55210 | exact | exact |
| + case-125 state-2 tail `setFreeze` reorder (-8) | 57196 | 54781 | exact | exact |
| body + gp + full anchor + case-40 translate | 57252 | 54846 | exact | exact |
| body + gp + full anchor + case-40 + case-125 tail + rg2 | 57236 | 54602 | exact | exact |
| body + gp + full anchor + case-40 + case-125 tail + rg2 + dir154 | 57236 | 54598 | exact | exact |
| body + gp + full anchor + case-125 expect/else-if | **57232** | **54446** | **+4 (156316)** | **+5** |

The last row is the only size-exact variant measured, but `__builtin_expect`
+ `else if` on case 125 changes the LSDA call-site row count, growing
`.ARM.extab` by 4 B (156312 -> 156316).  That shifts every following symbol and
drops `byte_exact` 4506 -> 4044 (ratchet FAIL).  It also needs a
`// lint: arm_builtin` waiver.

## New evidence: the missing `translate` is case-40 state 1 (confirmed)

Call-multiset audit over the function range:

```
orig 57  ours 56  TargetFollowCamera::translate(f,f,f)
orig 37  ours 36  PaintCanvas::TransformGetTransform
orig 23  ours 24  Level::getPlayer
```

The missing translate is **case 40 state 1**, original `0x13e6c0`:

```
13e6c0  movs r4,#0
13e6c2  sub.w r0, r4, sl, lsl #1     ; r0 = -2*delta
13e6ce  vcvt.f32.s32 s0, s0
13e6da  blx TargetFollowCamera::translate(0,0,(float)(-2*delta))
```

i.e. the original body is:

```cpp
} else if (m_nState == 1) {
    m_pCamera->translate(0.0f, 0.0f, (float) (-2 * delta));
    if (((RadioMessage *) ((*messages)[4]))->isOver()) { ... }
```

Our source is missing this statement.  It measures **+36 B** on the
`body+gp+2-call-anchor` base and **+20 B** on the full-anchor base (non-additive
block re-roll).  This is a genuine faithfulness fix and the best size filler
found.

The missing `TransformGetTransform` is a **tail-merge**, not a missing
statement: the original emits two adjacent calls at `0x141164`/`0x141184`
(vtable slots `[r2,#12]` and `[r2,#20]`), ours merges them into one shared call
at `0x1411e2`.  The source already has both statements.

## The ±4 B wall

The 2-call-anchor landing needs **+64 B**.  The full-anchor landing needs
**-16 B** (or +0 with the case-125 expect, which breaks `.extab`).  The
case-40 translate (+36) plus the case-125 tail reorder (-8) and the second
`resetGunDelay` receiver (-8) lands at **57236**, four bytes over.  Every
anchor keep-set sweep (`A..G` subsets) moves in 8-byte steps:

| anchor calls kept | size with case-40 translate |
|---|---:|
| 3 (`ABG`/`ACG`/`BCG`/...) | 57220 |
| 4 (`BCDG`/`BEFG`/...) | 57228 |
| 5 (`BCDEFG`/`ACDEFG`/...) | 57236 |
| 7 (full) | 57252 |

No keep-set + case-40 translate gives exactly 57232.  A faithful **+4 B**
edit is the missing piece.  Tested and rejected as 0 B on this base:
`m_nState = 4` -> `++` (case 40 state 3), `m_nState = 3` -> `++` (case 64
state 2), `m_nState = 5` -> `++` (case 40 state 4), the case-154 state-2
`GetDirVector` reorder, and the case-70 state-2-before-state-1 reorder
(the last is size-neutral but drops pct to 57.8).

## Other measured facts

* The case-70 state-2/state-1 swap is faithful (original `0x13c04e` tests
  `cmp r1,#2` before `cmp r1,#1`) but size-neutral and diff +16; not a win.
* The case-125 state-2 tail order is wrong: the original is
  `setLookAtCam(true); setVisible(false); setFreeze(true)` (`0x13b730/738/740`);
  ours has `setFreeze(true)` before `m_bCinematicBreak`.  Moving it is -8 B.
* The second `resetGunDelay` receiver (`LevelScript.cpp:3181`) is the only
  remaining extra `Level::getPlayer` call (orig 23, ours 24).  Fixing it is -8 B
  on the 2-call-anchor base.

## Next pass

1. Find a faithful **+4 B** statement (or a -4 B one for the 57236 variant) to
   land `body + gp + 2-call anchor + case-40 translate + case-125 tail +
   rg2` at exactly 57232.  That variant is gate-green on sections and
   `.rel.plt`, and its diff (54598) is +239 vs base — still worse than the
   +209 the coordinator measured, but the case-40 translate is a real fix.
2. Alternatively, find a source shape for the case-125 `else if` that does not
   add an LSDA call-site row (keep `.ARM.extab` at 156312); that variant is
   size-exact at 57232 and diff 54446 (+87).
3. The ideal metric point remains the **B-only** anchor (w7743 lead #1):
   make the real case-42 state-7 `PlayerStation::setVisible` block
   (`0x13da1e` orig) precede the case-125 `deleteHackingGame` block so the
   `G` call is unnecessary.

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w7769/harness.py "body,gp,anch=ACDEFG,c40,c125,rg2"   # 57236
python3 /dev/shm/w7769/harness.py "body,gp,exp125"                     # 57232, extab +4
```
