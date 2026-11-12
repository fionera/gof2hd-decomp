# LevelScript::process — wave 190 / w13418: prologue register-allocation lever

Target `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`, original
`0x137d18`, **57,232 B = 0xdf90**, size-exact).  Worktree `wt/w13418`, branch
`wave190-w13418`, base master `808d12f16` (raw **51,732**, aligned **21,931**,
`.text` 72,938, `.ARM.extab` 603, `.rel.plt` md5
`289b2a069f8504d1e8a3919e43915e04`).  Scratch
`/home/debian/GalaxyOnFire2/scratch/w13418`.

Task: take the prologue register-allocation lever.  Ours keeps `this` in
`fp`, the stack-canary base in `sl`, and `&Globals::status` in `r9`; the
original uses `this`->`sl`, canary->`fp`, `&Globals::status`->`r4` (and does
not cache the `Status *` value).  Find a source shape that shifts the
prologue without breaking size.

## Result

No size-exact prologue flip was found.  The prologue map is a whole-function
live-interval outcome, not a local declaration-order effect.  Every
size-exact source shape measured below keeps `this=fp, canary=sl,
&status=r9` (and the `Status *` value cached in `r4`).  A fresh adjacent-
statement sweep of the current body did find one new size-exact raw win,
landed below.

```
                         base (HEAD)      this pass
process raw diffbytes    51,732           51,731     (-1)
process aligned insns    21,931           21,936
orig / ours size         57232 / 57232    57232 / 57232   (size-exact)
.text diffbytes          72,938           72,937     (-1)
.ARM.extab diffbytes     603              603        (size/base exact 156,312 B @ 0x1a3b78)
.rel.plt                 md5 289b2a06…    md5 289b2a06…   (byte-identical, 29,392 B)
file size                2,187,328        2,187,328  (exact)
```

## Landed change

Source lines 2147/2148 (mission-89 wormhole block):

```cpp
-                        m_pCamera->setLookAtCam(true);
-                        m_pCamera->setPosition(player->getPosition());
+                        m_pCamera->setPosition(player->getPosition());
+                        m_pCamera->setLookAtCam(true);
```

`TargetFollowCamera::setLookAtCam` only stores the `lookAtCam` flag;
`setPosition(const Vector&)` only copies the three position components and
does not read `lookAtCam`; `player->getPosition()` is a pure read.  The
reorder is semantics-preserving.  Measured alone: raw **51,732 -> 51,731**
(-1), size 57,232/57,232 exact, `.text` 72,938 -> 72,937, `.rel.plt`
md5 unchanged.

## Exact prologue (base vs original)

Original `0x137d26..0x137d5e`:

```
mov  sl, r0            ; this          -> sl
ldr  r0, [pc,#888]
mov  r4, sl
mov  r6, r1            ; delta         -> r6
add  r0, pc
ldr.w fp, [r0]         ; canary base   -> fp
ldr.w r0, [fp]
str  r0, [sp,#428]
ldr.w r0, [r4,#24]!    ; &m_pLevel     -> r4
blx  Level::getMessages
mov  r5, r0            ; messages      -> r5
ldr  r0, [r4]
blx  Level::getPlayer
strd r4, r0, [sp,#80]
ldr  r0, [r4]
blx  Level::getEnemies
mov  r8, r0            ; enemies       -> r8
ldr  r0, [pc,#848]
add  r0, pc
ldr  r4, [r0]          ; &Globals::status -> r4
ldr  r0, [r4]          ; Status* value, not cached
blx  Status::getCurrentCampaignMission
```

Ours is the same instruction sequence with `this=fp`, canary=`sl`,
`&status=r9`, and `ldr.w r4,[r9]; mov r0,r4` for the cached `Status *`.
The `&status` load is 6 B longer (`ldr.w r9,[r0]` + `ldr.w r4,[r9]` +
`mov r0,r4` = 10 B vs `ldr r4,[r0]` + `ldr r0,[r4]` = 4 B), so the first
branch target is `0x137e8a` vs the original `0x137e84`.

## Shapes measured (all size-exact unless noted)

Declaration-order sweep: all 24 permutations of
`messages/player/enemies/status` (and all 24 declare-then-assign orders)
keep `this=fp, canary=sl, &status=r9`.  Base `MPES` is best at 51,732;
`MEPS`/`PMES` 51,744, `EPMS` 51,736, `PEMS`/`EMPS` 51,746.

`Status` forms: `Status *const`, `auto`, `Status &status =
*Globals::status`, `Status *&status`, `Status **statusp =
&Globals::status`, no local, local for only the first/second dispatch,
two-statement declaration, `(Status *)` cast, `*&`/`&*`, `register` — all
51,732/51,733 and all keep `&status=r9`.  The no-local forms drop the
cached value but still keep `&status=r9` (raw +1).

`LevelScript *self = this` (any use count), `LevelScript &self = *this`,
`Level *level = this->m_pLevel` for 1 getter: byte-identical to base.
`Level *level = m_pLevel` for 2 getters: 57,288 (+56), `this=r6`.
For 3 getters: 57,240 (+8), `this=fp`.  For all `m_pLevel` uses:
57,260 (+28), `this=sl`, canary=`r9`, `&status=fp` (the only shape that
makes the first `m_pLevel` load a plain `ldr.w r4,[sl,#24]` instead of the
pre-increment, which changes `this`'s register class from `gprnopc` to
`gpr`).

`PlayerEgo *volatile player`: 57,296 (+64), `this=sl`, canary=`fp`,
`&status=r9`.  `Array<...> *volatile messages`: 57,304 (+72), `this=sl`,
canary=`r9`.  `Level *volatile level`: 57,240 (+8), `this=fp`.

Dead checks at the top (e.g. `if (delta == -12345) return false;`):
57,380 (+148), `this=sl`, canary=`fp`, `&status=r9`.
`if (m_pLevel == 0) return false;`: 57,396 (+164), same map.
`__builtin_expect(mission == 0, 1)`: 57,336 (+104), `this=sl`,
canary=`fp`, `&status=r9`.

Caching the `Status *` value for all 98 uses: 57,220 (-12), `this=sl`,
canary=`r9`, `&status=fp`.  Reverting 1 use: 57,228 (-4).  Adding
`Level *level` all uses + the c106-s6 `Status *st` (`v18`) gives
57,236 (+4), `this=sl`, canary=`r9`, `&status=fp`.

## Why the map is body-driven

The original body uses `sl` (this) 904x and reloads `this` from
`[sp,#88]` 328x; ours uses `fp` (this) 1,131x and reloads `[sp,#88]`
173x.  The allocator's `{sl,fp}` tie-break is between the `gprnopc`
`this` COPY and the `gpr` canary base, and the `&status` r9-vs-r4 choice
is between the long-lived `&status` value and the transient `&m_pLevel`
address (`%0`) that occupies `r4` across the three getter calls.  No
local declaration/alias form changes those intervals without also
changing the emitted body (and hence the size).

## Sweep notes

A fresh adjacent-statement (`d=1`) sweep of the current body re-found the
documented `switchPlanetForIntro`/`switchSkyboxForIntro` cell (source
829/830): raw 51,731 but `.rel.plt` md5 `015ca6a2…` (rejected).  No new
size-exact `.rel.plt`-clean raw win was found in the first ~450 adjacent
pairs (source lines 605-2000).

The resumed sweep (pairs starting at source line 2384, 400 pairs, through
line ~4000) found one further size-exact cell at source lines 2674/2675,
the state-6 camera-frame block:

```cpp
-                    frame->setPosition(m_tempVec);
-                    canvas->CameraSetLocal(canvas->CameraGetCurrent(), frame->getMatrix());
+                    unsigned cam = canvas->CameraGetCurrent();
+                    frame->setDirection(facing, Vector{0.0f, 1.0f, 0.0f});
+                    frame->setPosition(m_tempVec);
+                    canvas->CameraSetLocal(cam, frame->getMatrix());
```

(The raw adjacent swap `CameraSetLocal` before `setPosition` also measures
51,730, but it is not semantics-preserving: `AEGeometry::getMatrix()`
returns the transform's live local matrix and `setPosition` writes its
translation, so `CameraSetLocal` must see the post-`setPosition` matrix.
`CameraGetCurrent()` is a pure read, so hoisting it is equivalent and keeps
the effectful call order.)  Raw **51,731 -> 51,730** (-1), aligned
21,936 -> 21,939, size 57,232/57,232 exact, `.text` 72,937 -> 72,936,
`.rel.plt` md5 `289b2a06…` unchanged.

The next resumed chunk (pairs from source line 3167, 400 pairs, through
line ~3737) found four adjacent cells; the two that also match the original
call order were landed together in the `getStatusValue() < 10` teardown:

```cpp
-                        resetCamera(m_pLevel);
-                        player->setTurretMode(false);
+                        player->setTurretMode(false);
+                        resetCamera(m_pLevel);
...
-                            wcamera->setRumblePercentage(0.0f, 0);
-                            player->killLiberator();
+                            player->killLiberator();
+                            wcamera->setRumblePercentage(0.0f, 0);
```

Raw **51,730 -> 51,728** (-2), aligned 21,940, size 57,232/57,232 exact,
`.text` 72,936 -> 72,934, `.rel.plt` unchanged.  (The other two cells,
`m_nState++`/`resetCamera` and `*camPos += *tmp * 400.0f`/`setPosition`,
each measure 51,729 alone; the latter is not semantics-preserving, and the
former is order-neutral but adds nothing on top of the pair above.)
Combined A+B+C+D measured 51,727 but includes the unsafe `setPosition`
reorder.

The remaining pairs (source line ~3738 to the end of the function) were not
swept in this pass.
