# Small tail wave 159 (w10418): Gun / ModStation / MenuTouchWindow / MGame::OnUpdate
# new hypotheses — no source win (note only)

Status: **note only, no `src/` change**. Worktree `wt/w10418`, base master
`9e4795206` (wave 157). Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `small-tail-wave158-w10318.md`, `small-tail-wave157-w10217.md`,
`small-tail-wave155-w10017.md`.

Gate on entry and exit (no src edit): `byte_exact 4507`, `linked_exact 4507`,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370, RATCHET PASS.  All four bodies stay size-exact.

Metrics: raw same-address diff bytes (`/dev/shm/w10418/fdiff.py`) and
instruction-index alignment (`/dev/shm/w10418/align_diff.py`, path-adjusted
copy of the committed tool).

| fn | raw before | raw after | aligned before | aligned after | size |
|---|---:|---:|---:|---:|---:|
| `Gun::calcCharacterCollision` (`_ZN3Gun22calcCharacterCollisionEv`) | 463 | 463 | 57 | 57 | 1984 |
| `ModStation::ModStation` C1 (`_ZN10ModStationC1Ev`) | 43 | 43 | 12 | 12 | 1008 |
| `MenuTouchWindow::OnTouchBegin` (`_ZN15MenuTouchWindow12OnTouchBeginEiiPv`) | 29 | 29 | 15 | 15 | 1896 |
| `MGame::OnUpdate` (`_ZN5MGame8OnUpdateEv`) | 37 | 37 | 109 | 109 | 11440 |

## 1. Gun::calcCharacterCollision — inline `getPosition` fixes the stack
##    colouring but the `i*3` register is a hard Greedy-RA choice

The wave-154 inline shape is reproduced and confirmed to give the original
stack layout:

```cpp
int distance = (int) AbyssEngine::AEMath::VectorLength(
        enemyPosition - ((Level *) this->level)->getPlayer()->getPosition());
```

Original vs inline shape (1984 B, raw 467, aligned 67):

```
orig  add.w r4, fp, fp, lsl #1     ; i*3
ours  add.w r5, fp, fp, lsl #1
orig  mov   r6, r4                 ; save i*3
ours  mov   r6, r5
orig  add   r4, sp, #44            ; getPosition sret
ours  add   r5, sp, #44
orig  add   r5, sp, #104           ; delta temp
ours  add   r4, sp, #104
```

So the *pair* (`i*3`, getPosition sret) shares one register and the delta
temp takes the other; the original pairs them on r4, the compiler picks r5.
The stack layout itself is now correct.  New shapes measured this wave (all
1984 B, all reverted):

| shape | raw | aligned |
|---|---:|---:|
| HEAD (named `playerPosition`) | **463** | **57** |
| inline `getPosition` | 467 | 67 |
| inline + `unsigned i3 = i*3; i3/3` indexing | 1964 B | — |
| inline + `i3` used as `(unsigned*)data() + i3` | 467 | 67 |
| inline + `i3` on only one of position/velocity | 467 | 67 |
| inline + `i3 = i + i + i` | 467 | 67 |
| inline + byte pointers `(char*)data() + i*12` | 467 | 67 |
| inline + `positions[i]` / `velocities[i]` | 467 | 67 |
| inline + velocity loaded before position | 482 | 90 |
| inline + `enemyPosition` first | 550 | 110 |
| inline + `i3` pointer pair (`pd`/`vd`) | 489 | 71 |

Every `i*3` spelling folds back to the same `t2ADDrs` and the VRM still
picks r5.  The inline shape is a *worse* raw/aligned pair than HEAD even
though its stack layout matches the original, so it is not landed.  A source
shape that makes Greedy-RA pick r4 for `%526` without changing the 1984-byte
body is still open.

## 2. ModStation::ModStation — volatile yaw load is the only lever; all
##    new volatile-placement shapes are worse

HEAD remains **43 / 12 / 1008**.  The residual is the literal-pool register
order (`YawIPad`→r3/r4 vs r4/r3), `mov r0,r3` before `cmp r1,#0`, and the
PostRA schedule of `str r8,[sp,#184]` / `mov r4,r2` / `ldr r3,[r0,r8,lsl#2]`.

The hint "the literal-pool order and mov-before-cmp come from the volatile
yaw load" was tested by moving the `volatile` between the two loads:

| shape | raw | aligned |
|---|---:|---:|
| HEAD (both `const volatile int` loads) | **43** | **12** |
| pitch non-volatile, yaw volatile | 666 | 60 |
| pitch volatile, yaw non-volatile | 44 | 18 |
| both non-volatile | 667 | 59 |
| `volatile float` loads (`vldr`) | 1016 B | — |
| yaw load before pitch load | 1000 B | — |
| inline `(Globals::iPad ? ... : ...)[race]` | 1000 B | — |
| `volatile int` locals | 558/565 | 124/180 |
| `const volatile float *yawTable` (pointer volatile) | 667 | 59 |

Dropping `volatile` on the yaw load fixes the pool order (r3=YawIPad,
r4=Yaw) but the PostRA scheduler sinks the `Yaw` LDRLIT and hoists
`mov r4,r2`, giving 44/18 — worse on both metrics.  Both-volatile remains
the best pair.  No new shape beats 43/12.

## 3. MenuTouchWindow::OnTouchBegin — the w10318 28/16 shape is the raw
##    optimum; no shape improves both metrics

HEAD is the wave-156 shape (A direct, B1 `field_0x20_top` via the inner
`Layout &layout` reference, B2 `field_0xc_leftMargin` direct, E direct):
**29 / 15 / 1896**.  The w10318 candidate (B1 direct, B2 via the reference)
is **28 / 16 / 1896**.

Full 2x2x2 sweep of (A, B1, B2, E) ∈ {direct, ref}^4 (all reverted):

| A | B1 (top) | B2 (leftMargin) | E | raw | aligned | size |
|---|---|---|---|---|---|---|
| direct | ref | direct | direct | **29** | **15** | 1896 |
| direct | direct | ref | direct | **28** | 16 | 1896 |
| ref | ref | direct | direct | 29 | 15 | 1896 |
| ref | direct | ref | direct | 28 | 16 | 1896 |
| ref | direct | direct | direct | 358 | 76 | 1892 |
| direct | direct | direct | direct | 358 | 76 | 1892 |
| direct | ref | ref | direct | 304 | 73 | 1892 |
| direct | direct | direct | ref | 314 | 74 | 1888 |
| direct | ref | direct | ref | 288 | 73 | 1892 |
| direct | direct | ref | ref | 287 | 74 | 1892 |

A (the first `buttonInsetX`) is free: it does not change the result.  The
size-1896 shapes are exactly the two *mixed* B1/B2 shapes; both-direct and
both-ref collapse the inner reference and shrink to 1892.  New spellings
tried this wave (all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| `Layout *layout = Globals::layout;` (pointer local, B2 via it) | 28 | 16 | 1896 |
| `(void) &layout;` + all direct | 358 | 76 | 1892 |
| `volatile Layout &layout` + all direct | 358 | 76 | 1892 |
| top-level `layout` pointer for B1/B2 (no inner ref) | 1762 | 820 | 1912 |

The raw-28 shape has one extra instruction-index mismatch (`imm:ldr`, the
literal-pool offset) that the raw-29 shape does not.  No shape improves both
raw and aligned, so HEAD (29/15) is kept.

## 4. MGame::OnUpdate — each `mov r0,r5` can be removed separately, but
##    removing both collapses the `&Globals::status` colouring

HEAD remains **37 / 109 / 11440**.  The two `mov r0,r5` are the `Status *st`
value (`%142`) in r5 at the mission-122 `getCurrentCampaignMission` and
`getPlayingTime` receivers.  New shapes measured this wave (all reverted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| HEAD (`st` for ccm + store RHS) | **37** | 109 | 11440 |
| ccm direct, store RHS via `st` | 318 | **19** | 11440 |
| ccm via `st`, store RHS direct | 914 | 109 | 11436 |
| both direct (`st` dead) | 5148 | 597 | 11392 |
| both direct, no `st` declaration | 5148 | 597 | 11392 |
| store LHS via `st`, ccm+store direct | 5253 | 761 | 11492 |
| store LHS+RHS via `st`, ccm direct | 5201 | 757 | 11488 |
| `volatile Status *st`, all direct | 5148 | 597 | 11392 |

The ccm `mov` is removed by `Globals::status->getCurrentCampaignMission()`
(raw 318 / aligned 19) and the store `mov` by
`Globals::status->getPlayingTime()` (raw 914 / aligned 109, size 11436).
Removing both makes `st` dead and the allocator moves `&Globals::status`
from r9 to r5, shrinking the body 48 B (every `ldr.w r0,[r9]` becomes a
2-byte `ldr r0,[r5]`).  The original keeps `&Globals::status` in r9 with no
live `Status*` in r5; a source shape that pins r9 without a live `Status*`
is still open.  The ccm-direct shape is the closest structurally (aligned
19) but its 2-byte `ldr.w` at the ccm site shifts the tail, so raw is 318.
Neither is a net raw win, so HEAD is kept.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w10418
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
python3 /dev/shm/w10418/fdiff.py <mangled>
python3 /dev/shm/w10418/align_diff.py <mangled>
```

Scratch (not committed), `/dev/shm/w10418/`: `fdiff.py`, `align_diff.py`,
`sbs.py`, `measure.sh`, `sweep_mod*.py`, `sweep_mtw*.py`, `sweep_gun*.py`,
`sweep_mg.py`, `*.base.cpp`, `*.sbs.txt`.

## 6. Files changed

* `_work/reconstructions/small-tail-wave159-w10418.md` (this note).
  No `src/` change; all four bodies stay size-exact at their wave-157
  values (1984 / 1008 / 1896 / 11440).
