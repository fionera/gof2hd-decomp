# Near-miss trio wave 138 (w8318): MGame::OnUpdate / MenuTouchWindow::OnTouchBegin /
# ModStation ctor — residual classified, no source win

Status: **no landed `src/` change.** Worktree `wt/w8318`, base `96b069792`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Read with `long-tail-wave137-w8220.md`, `long-tail-wave135-w8118.md`,
`near-miss-wave133-w7741.md`, `menutouchwindow-ontouchbegin-wave132-w7719.md`,
`modstation-ctor-wave130-w7682.md`, `mgame-onupdate-wave129-w7661.md`.

Gate before/after (source pristine): byte_exact **4507 -> 4507**,
linked_exact **4507 -> 4507**, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0, RATCHET PASS. `.text` 85,535 differing bytes,
`.ARM.extab` 1,395 (unchanged).

## 0. Targets and residual classification

| target | size | verify residual | object ratio / diffops |
|---|---:|---:|---:|
| `MGame::OnUpdate` | 11440 B | 41 bytes, 3 real insns | 96.19 / 15 |
| `MenuTouchWindow::OnTouchBegin` | 1896 B | 29 bytes, 2 clusters | 94.00 / 16 |
| `ModStation::ModStation` (C2) | 1008 B | 11 bytes, 5 insns | 95.28 / 34 |

The three residuals are all **Greedy-RA register/scheduling choices**, not IR
differences.  The exact source shapes are known; the allocator does not pick
them.  Details and every variant measured this wave are below.

## 1. MGame::OnUpdate — the `Status *st` anchor is load-bearing, and its CSE is the residual

Residual (original vs HEAD), `src/game/menu/MGame.cpp:2768`:

```
17e090  ldr.w r0,[r9]        |  ldr.w r5,[r9] ; mov r0,r5     (first ccm)
17e0b8  ldr.w r0,[r9]        |  mov r0,r5                     (getPlayingTime)
```

Everything else in the 11440-byte function is byte-identical.  `r9` is
`&Globals::status` in both; the original reloads `[r9]` afresh before *every*
use, HEAD caches the anchor value in `r5` and reuses it.

### 1.1 MIR evidence (NDK clang `-mllvm -print-after-all`)

HEAD (`/dev/shm/w8318/mg_pa.txt`, VRR dump of `_ZN5MGame8OnUpdateEv`):

```
bb.238:
  %r0  = tLDRLIT_ga_pcrel @_ZN7Globals6statusE
  %r9  = t2LDRi12 %r0, 0        ; r9 = &Globals::status
  %r5  = t2LDRi12 %r9, 0        ; r5 = Globals::status   <- the `Status *st` anchor
  DBG_VALUE $r5, "st"
  $r0  = COPY %r5               ; first ccm  (CSE'd with the anchor)
  tBL  getCurrentCampaignMission
...
bb.240:
  %r0  = t2LDRi12 %r9, 0        ; getMission (fresh)
...
bb.241:
  $r0  = COPY %r5               ; getPlayingTime (anchor reuse)
```

The plain source (`Globals::status->` everywhere, no anchor) has exactly the
original IR (three fresh `t2LDRi12 %r9,0`), but the allocator then puts
`&Globals::status` in **r5** instead of r9 and the whole function re-rolls
(11392 B, 198 diffops, ratio 90.65).  The anchor is what forces r9; the anchor
value is what the first ccm and getPlayingTime reuse.

### 1.2 Variants measured (object-level, `fullscore.py`)

Baseline (anchor + `st->getPlayingTime`) = **96.1897 / 15 diffops**.
Anchors at other sites all move the CSE or re-roll the function:

| variant | ratio | diffops | note |
|---|---:|---:|---|
| baseline: anchor, m23 `Globals::status->` x3, `st->` at m23 pt | **96.19** | 15 | only residual |
| `st->` at m23 first ccm | 96.19 | 15 | identical code |
| `st->` at m23 getMission | 95.92 | 24 | CSE spreads to getMission |
| `st->` at m23 getMission + ccm | 95.92 | 24 | |
| `st->` at m24 ccm (v10) | 96.11 | 18 | m23 first ccm still r5/mov |
| `st->` at m24 getPlayingTime | 95.87 | 26 | |
| `st->` at m24 ccm + pt | 95.98 | 22 | |
| `st->` at m23 field_0x100 | 84.78 | 280 | fp becomes the address |
| `st->` at m23 all four uses | 84.78 | 280 | fp becomes the address |
| `st->` at m23 body tail field | 84.77 | 279 | |
| `Status *&st = Globals::status` (block) | 90.65 | 198 | == plain |
| `Status *&status` at function scope | 90.65 | 198 | == plain |
| `Status *const st` | 96.19 | 15 | identical |
| `Status *volatile st` | 85.80 | 258 | |
| `Status *volatile sv; st = sv` | 85.74 | 259 | |
| `st = *(Status *volatile *)&Globals::status` | 96.19 | 15 | identical |
| `st = (Status*)((char*)Globals::status + 0)` | 96.19 | 15 | identical |
| comma-assign at isMining / getMission / getPlayingTime | 90.6-90.7 | 197-198 | address -> fp |
| lambda `[&]{ Status *s = Globals::status; ... }()` | 90.65 | 198 | == plain |
| anchor declared inside m23 body (3 forms) | 90.5-90.6 | 198-201 | == plain |
| anchor deleted (plain) | 90.65 | 198 | r5, 11392 B |

The plain IR is right; the register is wrong.  The anchor IR is wrong; the
register is right.  No source shape found that gets both.

## 2. MenuTouchWindow::OnTouchBegin — two clusters, both pure register naming

### 2.1 Case-3 iPad hit test: `ip` vs `r4` (2 insns)

`src/game/ui/MenuTouchWindow.cpp:2431` (`this->listEntryWidth`):

```
129fd2  ldr.w ip,[sl,#340]   |  ldr.w r4,[sl,#340]
129fd8  add   r3,ip          |  add   r3,r4
```

`ip` (r12) in the original, `r4` in HEAD.  The value is a short-lived
`this->listEntryWidth` load; r4 is free in the original block too.  This is a
Greedy-RA low-register preference.  Not probed further this wave (it is the
smaller of the two clusters).

### 2.2 Case-15 scrollbar block: one-register rotation (27 insns)

Original (`12a1bc..12a24c`) vs HEAD:

| value | original | HEAD |
|---|---|---|
| heapBufB | r3 | r2 |
| field_0x1e0 | r6 | r3 |
| `Globals::layout` value | r2 (fresh) | r6 (live across the 1st call) |
| `Globals::w` | r5 | r4 |
| buttonInsetX | r2 | r5 |
| `&Globals::Canvas` | fp | fp |
| `&Globals::layout` | remat `[sp,#16]` -> r4 | remat `[sp,#16]` -> r4 |

Root cause: `src/game/ui/MenuTouchWindow.cpp:2346` has
`Layout &layout = *Globals::layout;` and uses `layout.field_0x20_top` in the
second comparison.  That keeps the layout *value* (`%29`) live across the
first `GetImage2DWidth` call.  The original reloads `Globals::layout` fresh at
`12a218` and reads both `field_0xc_leftMargin` and `field_0x20_top` from it.

### 2.3 Variants measured (`mtwscore.py`, object-level)

HEAD = **94.0041 / 16 diffops**.  The all-16 combinations of
`layout.` vs `Globals::layout->` at the four layout access sites were compiled:

| top access | ratio | diffops | note |
|---|---:|---:|---|
| `layout.field_0x20_top` (HEAD) | **94.00** | 16 | layout value live across call |
| `Globals::layout->field_0x20_top` (all-direct, D) | 92.48 | 23 | fresh value, but `&Globals::layout` -> fp, `&Globals::Canvas` -> r5 |

Other shapes: two `Layout&` aliases (16), `Layout *lp` alias (16), volatile
`top` read (23), volatile layout pointer (22), second alias for top+left (23).
Variant D's case-15 block is actually *closer* in register roles (heapBufB=r3,
field_0x1e0=r6, layout=r2, buttonInsetX=r2) but swaps fp/r5 for the two GOT
addresses and keeps `&Globals::layout` in fp instead of rematting it.  The
`[sp,#16]` reload pattern is the discriminator: HEAD reloads it 3x like the
original; D reloads it 2x.

## 3. ModStation ctor — near call's `mov r4,r2` scheduling

Residual (`src/game/menu/ModStation.cpp:334`), near `MatrixSetRotation`:

```
d5456  ldr.w r3,[r0,r8,lsl #2]   ; yaw   |  d5456  str.w r8,[sp,#184]
d545a  movs  r0,#2               |  d545a  ldr.w r4,[r0,r8,lsl #2]   ; yaw
d545c  str.w r8,[sp,#184]        |  d545e  movs  r0,#2
d5460  movs  r1,#0               |  d5460  movs  r1,#0
d5462  add.w r8,sp,#256          |  d5462  add.w r8,sp,#256
d5466  strd  r1,r0,[sp]          |  d5466  strd  r1,r0,[sp]
d546a  add   r1,sp,#320          |  d546a  add   r1,sp,#320
d546c  mov   r0,r8               |  d546c  mov   r3,r4
d546e  mov   r4,r2               |  d546e  mov   r0,r8
```

The original's far `MatrixSetRotation` x-argument is **`pitch`** (r2 copied to
r4 at `d546e`); HEAD uses `yaw` (r4 loaded from the yaw table).  The *faithful*
`far = pitch` source is therefore semantically right but scores worse by the
byte metric because of scheduling:

| variant | ratio | diffinsns | note |
|---|---:|---:|---|
| HEAD `far = yaw` (volatile yaw load) | **95.28** | 34 | wrong value, best bytes |
| `far = pitch` (volatile yaw) | 94.44 | 40 | pool order Pitch,Yaw,YawIPad (wrong) |
| `far = pitch` + non-volatile yaw load | 93.89 | 44 | **pool order Pitch,YawIPad,Yaw (right)**, roles right, `Yaw` LDRLIT sunk + `mov r4,r2` hoisted |
| + declaration order swap / explicit `if` / separate yawIPad var | 93.89 | 44 | identical |
| + `float farPitch = pitch` before/after near call | 93.89 | 44 | identical |
| + `volatile float farPitch` | 64.07 | 258 | |
| both loads non-volatile | 86.55 | 97 | whole function re-rolls |

`p_nv` (`far = pitch`, non-volatile `yawBits`) is byte-identical to the
original **except** the `ldr r4,[pc,#620]` (Yaw literal) is scheduled 14 bytes
later and the `mov r4,r2` is hoisted above the `ldr.w r3` yaw load.  The far
call itself (`d54e0..d5502`) is exact.  This is the same
`BURRSort`/`NodeQueueId` LDRLIT tie-break described in wave 130.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w8318
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6        # RATCHET PASS, byte 4507
```

Scratch (not committed), `/dev/shm/w8318/`: `cc.sh` (exact per-TU compile),
`ccpa.sh` (same + `-mllvm -print-after-all`), `fullscore.py` / `mtwscore.py` /
`modscore.py` (object-level normalised sequence diffs), `mg_pa.txt` /
`mtwH_pa.txt` / `mtwD_pa.txt` (MIR dumps), `gen*.py` / `gmtw*.py` (variant
generators), `x_*.o`, `z_*.o`, `mtw_*.o`, `mod_*.o` (measured variants).

## 5. Files changed

`_work/reconstructions/near-miss-trio-wave138-w8318.md` only.
Gate: byte 4507 / linked 4507, size 4524/4524, data 843/843, imports 207,
lint CLEAN, drift 0.
