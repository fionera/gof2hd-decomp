# LevelScript::process — state-11 integer z + mission-42 chain 0,6,7 (wave 191b, w13802)

Target: `_ZN11LevelScript7processEi` (`src/game/world/LevelScript.cpp`), size 57,232 B.
Branch base: `master` @ `aa4e4aad2cbd5a6b49d430cca88b78569321b365`.
Status: **note-only — no size-exact variant beats the current baseline on raw/aligned/.text/extab.**
`src/` left clean.

## What was reconstructed (and why it is faithful)

### 1. state-11 integer z (`case 11`, original @0x13bd2c)

Original:

```
13bd2c: 9c13        ldr   r4, [sp, #76]
13bd2e: ed1f 1afb   vldr  s2, [pc, #-1004]   ; 0.7f @0x13b944
13bd32: 4260        negs  r0, r4
13bd34: ee02 4a10   vmov  s4, r4
13bd38: ee00 0a10   vmov  s0, r0
13bd3c: eb04 0044   add.w r0, r4, r4, lsl #1 ; 3*delta (INTEGER)
13bd40: eeb8 0ac0   vcvt.f32.s32 s0, s0
13bd44: ee03 6a10   vmov  s6, r0
13bd48: eeb8 2ac2   vcvt.f32.s32 s4, s4
13bd4c: eeb8 3ac3   vcvt.f32.s32 s6, s6
13bd50: f8da 0014   ldr.w r0, [sl, #20]
13bd54: ee20 0a01   vmul.f32 s0, s0, s2
13bd58: ee12 2a10   vmov  r2, s4
13bd5c: ee13 3a10   vmov  r3, s6
13bd60: ee10 1a10   vmov  r1, s0
13bd64: f726 ec90   blx   TargetFollowCamera::translate
```

So the z argument is `(float)(delta * 3)` (integer multiply then convert), not
`(float)delta * 3.0f`.  The source change is:

```c++
- m_pCamera->translate((float) -delta * 0.7f, (float) delta, (float) delta * 3.0f);
+ m_pCamera->translate((float) -delta * 0.7f, (float) delta, (float) (delta * 3));
```

With this change the emitted state-11 block is instruction-for-instruction identical to
the original except that the camera pointer lives in `fp` instead of `sl`.

### 2. mission-42 chain order `0,6,7` (original entry @0x13c1a4)

Original test order at the mission-42 dispatch:

```
13c1a4: 9816        ldr   r0, [sp, #88]
13c1a6: 69c0        ldr   r0, [r0, #28]        ; m_nState
13c1a8: b938        cbnz  r0, 13c1ba           ; state==0 first
13c1aa: 6820        ldr   r0, [r4, #0]
13c1ac: f726 e910   blx   Status::inAlienOrbit
13c1b0: 2800        cmp   r0, #0
13c1b2: f003 80a1   beq.w 13f2f8               ; state 0 && !inAlienOrbit
13c1b6: 9816        ldr   r0, [sp, #88]
13c1b8: 69c0        ldr   r0, [r0, #28]
13c1ba: 2807        cmp   r0, #7
13c1bc: f001 839e   beq.w 13d8fc               ; state 7
13c1c0: 2806        cmp   r0, #6
13c1c2: f048 8638   bne.w 144e36               ; otherwise tail
```

i.e. the emitted test order is `0, 7, 6`.  The source order that reproduces it is
`if (state==0 && !inAlienOrbit) … else if (state==6) … else if (state==7) …` (the
compiler lays the state-7 test before the state-6 test).  Our `0,6,7` build emits exactly:

```
13bf86: 2807        cmp   r0, #7
13bf88: f002 80d5   beq.w 13e136
13bf8c: 2806        cmp   r0, #6
13bf8e: f048 85b8   bne.w 144b02
```

The state-0 arm is moved ahead of the state-6 arm; the state-7 arm stays last.  The
existing comment block in `LevelScript.cpp` already documents the `.rel.plt` rationale
(state-7's first `PlayerStation::setVisible` must precede `PlayerEgo::deleteHackingGame`).

## Measured table (current base, this session)

`raw`/`aligned`/`size` are from `tools/fn_diff.py`; `.text`/`.ARM.extab` are section
diff-byte counts; `rel.plt` is the md5 of `.rel.plt` (baseline `289b2a069f8504d1e8a3919e43915e04`).

| variant | raw | aligned | size | .text | .ARM.extab | .rel.plt |
|---|---:|---:|---:|---:|---:|---|
| **baseline** (chain 6,0,7; float z) | **53,037** | **19,875** | **57,232** | **75,193** | **587** | 289b2a06 |
| s11 only | 55,013 | 19,864 | 57,240 (+8) | 402,002 | 1,272 | ok |
| chain `067` only | 54,751 | 20,097 | 57,216 (−16) | 357,925 | 587 | ok |
| chain `067` + rumble swap | 54,751 | 20,097 | 57,216 (−16) | 357,925 | 587 | ok |
| s11 + chain `067` | 54,473 | 20,086 | 57,224 (−8) | 354,103 | 587 | ok |
| **s11 + chain `067` + volatile-geometry (best size-exact)** | 53,634 | 20,051 | **57,232** | 75,790 | 589 | ok |
| s11 + chain `067` + volatile-int pad (state-0 tail) | 54,049 | 21,807 | 57,232 | 76,205 | 589 | ok |
| s11 + chain `067` + two `(*enemies)[i]` exprs | 55,557 | 23,891 | 57,344 | 412,461 | 1,512 | ok |
| s11 + chain `067` + `KIPlayer *volatile` | 55,522 | 23,886 | 57,352 | 412,982 | 1,513 | ok |

Other chain permutations (from the prior w13787 sweep, still valid on this base):

| chain | raw | aligned | size | .rel.plt |
|---|---:|---:|---:|---|
| `076` | 54,915 | 20,221 | 57,240 | changed (78fd4383) |
| `670` | 54,945 | 20,149 | 57,240 | changed (0a16daf7) |
| `706` | 55,057 | 20,080 | 57,228 | changed (6e5a3b9c) |
| `760` | 54,974 | 20,314 | 57,240 | changed (78fd4383) |
| `607` (baseline) | 53,037 | 19,875 | 57,232 | ok |

## Why no size-exact variant is a net win

The faithful reconstruction needs **+8 B** relative to `s11 + chain 067` (which is
57,224).  The only clean way found to add exactly those 8 bytes without disturbing the
literal pool is a `volatile` geometry pointer in the pre-switch state-0 loop:

```c++
             for (int i = 0; i < 3; ++i) {
                 KIPlayer *e = (*enemies)[i];
                 e->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
-                e->geometry->setVisible(true);
+                AEGeometry *volatile g = e->geometry;
+                g->setVisible(true);
             }
```

This restores the original body width (the original reloads `enemies->data`,
`data[i]` and `->geometry` for the second call; the volatile pointer forces a
store+load instead, same instruction count).  It also fixes the literal-pool cascade:
`s11 + chain 067` inserts an 8-byte `nop` pad (`bf00 bf00` at `0x13d18c` plus `bf80` at
`0x13d176`) before the 0.3 double at `0x13d190`; the original has no such pad.  With the
volatile pointer the `.text` diff collapses from 354,103 to 75,790.

But the result is still **worse than the current baseline on every metric**:
raw +597, aligned +176, `.text` +597, `.ARM.extab` +2.  The baseline's chain order
`6,0,7` is structurally wrong (it emits `blx inAlienOrbit; cmp #7; bne` at `0x13d63a`
with no state-0 `cbnz`), yet it happens to place the mission-42 blocks at addresses that
diff fewer bytes against the original.  The faithful `0,6,7` order moves the mission-42
entry ~0x234 bytes earlier, which dominates the same-address raw metric (this function is
already ~93 % byte-different).

An `int volatile pad = delta;` at the state-0 tail also restores 57,232 but is worse
still (aligned 21,807) because inserting instructions before the giant switch shifts every
downstream branch-target index used by the aligned metric.

## Exact source diff of the best size-exact variant (not landed)

```diff
@@ state-0 loop (pre-switch, ~line 635)
             for (int i = 0; i < 3; ++i) {
                 KIPlayer *e = (*enemies)[i];
                 e->geometry->setDirection(Vector{1.0f, 0.0f, 0.0f}, Vector{0.0f, 1.0f, 0.0f});
-                e->geometry->setVisible(true);
+                AEGeometry *volatile g = e->geometry;
+                g->setVisible(true);
             }
@@ case 11 (~line 1219)
-                m_pCamera->translate((float) -delta * 0.7f, (float) delta, (float) delta * 3.0f);
+                m_pCamera->translate((float) -delta * 0.7f, (float) delta, (float) (delta * 3));
@@ case 42 chain: move the state-0 arm ahead of the state-6 arm
-                if (m_nState == 6) {
+                if (m_nState == 0 && !Globals::status->inAlienOrbit()) {
+                    ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->reset(true);
+                    m_nState = 1;
+                } else if (m_nState == 6) {
                     ...
-                } else if (m_nState == 0 && !Globals::status->inAlienOrbit()) {
-                    ((PlayerWormHole *) (*m_pLevel->getLandmarks())[3])->reset(true);
-                    m_nState = 1;
                 } else if (m_nState == 7) {
```

(Exact diff: `scratch/w13802/LevelScript.A.cpp` vs `LevelScript.baseline.cpp`.)

Measured: raw 53,634 / aligned 20,051 / size 57,232 exact / `.text` 75,790 /
`.ARM.extab` 589 / `.rel.plt` md5 `289b2a06…` (clean).

## Next angle

The remaining 8-byte shortfall in `s11 + chain 067` and the ~0x234-byte shift of the
mission-42 entry both point at the **pre-switch / state-0 body**: the original
`0x137e84..0x137f76` body is 0xf2 bytes and reloads the enemy pointer for the second
`setVisible`; our `-Oz` build caches it.  Matching that reload exactly (without the
`volatile` store/load) would both close the size gap and remove the index shift, and is
the most likely path to a genuine aligned win.
