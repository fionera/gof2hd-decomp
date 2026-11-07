# Near-miss trio wave 141 (w8618): MGame / MenuTouchWindow / ModStation —
# nested-if reproduces the exact MGame status loads, but re-rolls the subtraction

Status: **no landed `src/` change.** Worktree `wt/w8618`, base `e18e0c2a4`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `near-miss-trio-wave140-w8518.md`, `near-miss-trio-wave139-w8418.md`,
`near-miss-trio-wave138-w8318.md`, `mgame-onupdate-wave129-w7661.md`,
`mgame-onupdate-r5-cache-wave97-w4717.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.rel.plt` byte-identical.

## 0. Residual classification (raw same-address bytes, re-verified)

| target | size | raw diff | hunks |
|---|---:|---:|---|
| `MGame::OnUpdate` | 11440 B | 41 | 1 (2-byte shift, anchor CSE) |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 28 | 4 clusters (see below) |
| `ModStation::ModStation` (C1/C2) | 1008 B | 11 | 1 (scheduler) |

## 1. MGame::OnUpdate — NEW: the nested-`if` shape gets the exact four fresh loads

The wave-129/138/139/140 conclusion was that *every* source shape that removes
the anchor CSE also re-colours `&Globals::status` from `r9` to `r5`.  This wave
found a shape that keeps `r9` **and** emits the four fresh `ldr r0,[r9]`:

```cpp
if (Globals::status->getCurrentCampaignMission() == 23 && !this->player->isMining() &&
    Globals::status->getMission()->isEmpty()) {
    if (Globals::status->getPlayingTime() -
            reinterpret_cast<long long &>(Globals::status->field_0x100) > 3600000) {
        ... body ...
    }
} else if (...m24...) {
```

i.e. split the four-term `&&` into an outer 3-term `if` and an inner 1-term
`if`.  The address `&Globals::status` is now used in **both** blocks, so it is
kept whole in `r9`; the anchor is gone, so the ccm/mission/pt loads are fresh:

```
17b4  ldr.w r9,[r0]        ; &Globals::status
17b8  ldr.w r0,[r9]        ; ccm       MATCH
17d2  ldr.w r0,[r9]        ; mission   MATCH
17e2  ldr.w r0,[r9]        ; pt        MATCH
```

But the split changes the basic-block structure of the comparison and the
subtraction re-rolls:

```
orig 17e0b8..17e0d8:  ldr r0,[r9]; blx pt; ldr r2,[r9]; ldr r4,[pc];
                      ldrd r2,r3,[r2,#256]; subs r0,r0,r2; sbcs r1,r3;
                      subs r0,r4,r0; mov.w r2,#0; sbcs.w r0,r2,r1; bge.n
nested:               ldr r0,[r9]; blx pt; mov r2,r0; ldr r0,[r9]; ldr r4,[pc];
                      ldrd r3,r5,[r0,#256]; subs r2,r2,r3; sbcs r1,r5;
                      subs r2,r4,r2; mov.w r3,#0; sbcs.w r1,r3,r1; bge.w
```

The compiler picks `r0` (not `r2`) for the field receiver and therefore inserts
`mov r2,r0`; the `bne`/`bge` also widen.  Net **11460 B (+20)**, raw diff 5054.
Every attempt to force the field receiver to `r2` inside the nested block
(`long long pt`/`long long field` locals, `Status *st2` before the field,
`st` for both pt+field, reversed/`>=` comparisons) still emits `mov r2,r0`
and stays 11460 B.  Using `st` for the field collapses back to the plain
`r5` colour.  So the exact status loads and the exact subtraction are
mutually exclusive in this compiler: splitting the condition is what buys
`r9`, and splitting is also what forces `mov r2,r0`.

Other new MGame probes (all reverted):

| variant | raw | size | note |
|---|---:|---:|---|
| HEAD (`Status *st` anchor at pt) | **41** | 11440 | |
| nested outer/inner `if` (direct) | 5054 | 11460 | exact status loads, +20 B |
| nested + `long long pt`/`field` locals | 5054 | 11460 | same |
| nested + `Status *st2` before field | 5054 | 11460 | same |
| nested, `st` at pt+field | 5148 | 11392 | back to `r5` |
| `Status *volatile *stp = &Globals::status;` | 5148 | 11392 | plain `r5` |
| `Status *st = *(Status *volatile *)&Globals::status;` | 41 | 11440 | identical to HEAD |

## 2. MenuTouchWindow::OnTouchBegin — the reference is load-bearing for `r4`

The wave-138/139/140 finding is re-confirmed and extended: the `Layout &layout`
reference is the *only* spelling that keeps `&Globals::layout` in `r4` (loaded
from `[sp,#16]` at `0x5e8`) and `&Globals::Canvas` in `fp`, matching the
original.  Any direct/pointer/comma spelling moves `&Globals::layout` to `fp`
and `&Globals::Canvas` to `r5` and shrinks the function to **1892 B** (size
regression, not acceptable).

The 28 bytes are now classified as four clusters:

| offset | orig | ours | cause |
|---|---|---|---|
| `0x3fe` | `ldr.w ip,[sl,#340]` | `ldr.w r4,[sl,#340]` | case-3 RA choice |
| `0x60c..0x630` | layout value fresh (`r2`) | cached in `r6` | reference live across 1st call |
| `0x640..0x652` | leftMargin+top from one fresh value | leftMargin from `r6`, top fresh | same |
| `0x672` | `ldr r5,[r2,#40]` | `ldr r4,[r2,#40]` | 3rd-comparison RA choice |

New probes (all reverted, all 1892 B unless noted):

| variant | size | note |
|---|---:|---|
| HEAD | 1896 | |
| direct leftMargin only | 1892 | `fp`/`r5` swap |
| pointer `Layout *layout` + comma reassign | 1892 | `fp`/`r5` swap |
| comma `(lp = Globals::layout, ...)` | 1892 | `fp`/`r5` swap |
| reference for 3rd comparison too | 1896 | identical to HEAD |
| reference for top, direct leftMargin | 1888 | +extra value load |

## 3. ModStation ctor — `far = pitch` is faithful but 43; HEAD stays 11

Re-measured: HEAD (`far = yaw`) = **11** raw / 1008 B.  The faithful
`far = pitch` = **43** raw / 1008 B.  The near call itself is where the
difference lives:

```
orig 0x186..0x1a0:  ldr.w r3,[r0,r8,lsl#2]  ; yaw
                    movs r0,#2
                    str.w r8,[sp,#184]      ; spill race
                    movs r1,#0
                    add.w r8,sp,#256
                    strd r1,r0,[sp]
                    add r1,sp,#320
                    mov r0,r8
                    mov r4,r2               ; save pitch
                    blx
ours (far=pitch):   str.w r8,[sp,#184]
                    mov r4,r2
                    ldr.w r3,[r0,r8,lsl#2]
                    movs r0,#2
                    ...
                    mov r0,r8
                    blx
```

The far call (`0x220..0x232`) is byte-identical in both.  The scheduler hoists
the `race` spill and `mov r4,r2` above the yaw load.  Removing the `yaw` local
(using a fresh table load at the near call) and `float nearYaw`/`farPitch`
locals do not move it.  HEAD's `far = yaw` keeps the 11-byte metric.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8618
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4507
python3 /dev/shm/w8618/fdiff.py _ZN5MGame8OnUpdateEv
```

Scratch (not committed), `/dev/shm/w8618/`: `fdiff.py`, `dd.py` (byte-level
disasm diff), `try_mg.py` / `try_mtw.py` / `try_mod.py` (single-TU variant
drivers), `batch_mg.py` / `batch_mtw*.py` (variant sweeps), `measure.py`
(apply variant + link + fdiff), `cc_mgame.sh` / `cc_mtw.sh` / `cc_mod.sh`.

## 5. Files changed

`_work/reconstructions/near-miss-trio-wave141-w8618.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
