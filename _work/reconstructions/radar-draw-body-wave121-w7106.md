# Radar::draw pass (wave121, w7106) — full body/matrix residual classification

**Status:** research note, 2026-11-06. Worktree `wt/w7106`, based on current
`master` (`bd061a489`, wave-120 state). **No source change landed**; this commit
contains this note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`
(`sha256 2f7347aeda7f1d8dbbfa6744b1b4d09c36a36a26aa1689266c81090ed4ff5f45`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`) in
`src/game/weapons/Radar.cpp`. Original `9908 B`; ours `9908 B`.

Read with the earlier notes `radar-draw-wave119-w6976.md` (distance-first exact
prologue), `radar-draw-wave119-w6947.md` (`i==1||i==3` unfolded), 
`radar-draw-wave119-w6950.md` (planet/enemy/asteroid block order,
`norec_swapstores`), `radar-draw-wave119-w6967.md` (literal-pool root cause,
`vpush {d8-d13}` only via distance-first), `radar-draw-wave119-w6721.md`
(local `String` in landmark `i==0`), and `radar-draw-wave119-w6730.md`
(structural-sweep local optimum). This wave repeats those conclusions and
extends them with a complete 54-run residual classification and a concrete
patch proposal.

---

## 0. Baseline, gate and structural state

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
cmake --build cmake-build-match --target lint
```

Measured this wave (baseline source, unmodified):

```
osize=9908 usize=9908  raw=74.541284%  M=2600  n=3492/3484
```

`M` = matching instruction count (`difflib` matching-block size), `n` =
normalized instruction count original/ours (ours has **8 fewer** instructions).

Full gate:

```
compared 4524  avg 99.94%  byte_exact 4505  linked_exact 4505  fuzzy_perfect 4505
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 4505  linked 4505  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

Score command (uses the official verifier API exactly — normalized,
reconciled disassembly, `difflib.SequenceMatcher(None, on, un,
autojunk=False).ratio()`):

```sh
python3 /dev/shm/w7106/score_radar.py
# osize=9908 usize=9908 raw=74.541284% M=2600 n=3492/3484
```

`verify.py --show _ZN5Radar4drawEP6PlayerP3Hudi` confirms
`orig 9908B @0x12e050 | ours 9908B @0x12e050`.

---

## 1. Method

1. `tools/verify/verify.py --show _ZN5Radar4drawEP6PlayerP3Hudi` → side-by-side
   (`/dev/shm/w7106/radar_verify.txt`).
2. Raw `objdump -d` of the whole function from both ELFs
   (`radar_orig.dis`, `radar_ours.dis`).
3. A run generator (`runs_radar.py`) that aligns the two normalized streams and
   emits every maximal positional differing run with original/ours relative
   addresses and all differing lines (`radar_runs.txt`, `run_table.md`).
4. Each candidate source edit was applied, rebuilt with `link_so`, re-scored
   with `score_radar.py`, then reverted (`/dev/shm/w7106/Radar.base.cpp` is the
   pristine copy).

Result: **54 positional differing runs, 3222 differing instruction slots**.
Almost all residual is **block placement, register allocation, and literal-pool
placement**, not missing high-level statements. The coordinate-transform
prologue is already byte-identical.

---

## 2. Coordinate-transform prologue — already exact

The distance-first `CameraGetCurrent/CameraGetLocal` → `Matrix::operator=` →
camera X/Y/Z extraction → `MatrixGetInverse` → `Matrix::operator=` sequence is
byte-for-byte identical in both binaries at the same addresses:

```asm
12e144 ldr   r0,[sp,#140]        ; Globals::Canvas
12e146 ldr   r6,[r0,#0]
12e148 mov   r0,r6
12e14a blx   CameraGetCurrent
12e14e mov   r1,r0
12e150 mov   r0,r6
12e152 blx   CameraGetLocal
12e156 add.w r6,r4,#464          ; &this->transform (0x1d0)
12e15a mov   r1,r0
12e15c mov   r0,r6
12e15e blx   Matrix::operator=
12e162 ldr.w r1,[r4,#492]        ; transform.m[7]
12e166 add   r5,sp,#232          ; scratch Matrix
12e168 ldr.w r2,[r4,#508]        ; transform.m[11]
12e16c add.w r3,r4,#572          ; &cameraX (0x23c)
12e170 ldr.w r0,[r4,#476]        ; transform.m[3]
12e174 stmia r3!,{r0,r1,r2}      ; cameraX/Y/Z = m[3]/m[7]/m[11]
12e176 mov   r0,r5
12e178 mov   r1,r6
12e17a blx   MatrixGetInverse
12e17e mov   r0,r6
12e180 mov   r1,r5
12e182 blx   Matrix::operator=
```

Source that produces it (`Radar.cpp` lines 363–367):

```cpp
this->transform = *(Matrix *) Globals::Canvas->CameraGetLocal(Globals::Canvas->CameraGetCurrent());
this->cameraX = this->transform.m[3];
this->cameraY = this->transform.m[7];
this->cameraZ = this->transform.m[11];
this->transform = AbyssEngine::AEMath::MatrixGetInverse(this->transform);
```

**Conclusion:** the "distance-first prologue" and the coordinate transform are
already exact. The prologue *residual* is only the frame/slot/literal-pool
consequence of the body's register pressure:

| run | original | ours | cause |
|---:|---|---|---|
| 1 | `vpush {d8-d13}` | `vpush {d8-d11}` | body needs 6 d-regs in original (distance-first `inSight` keeps `s24`/`s26` live) |
| 2 | `str r3,[sp,#100]` | `str r3,[sp,#112]` | `mode` stack slot shifts 12 B |
| 3,4,5,9 | `ldr r0,[pc,#728/700/684/368]` | `ldr r0,[pc,#632/604/588/272]` | literal-pool moved 0x60 earlier |
| 6,8,10,11,12 | branch targets `0xb8a/0x69e/0x2ac/0x290` | `0xb44/0x678/0x2c0/0x2a4` | body is 0x46 B shorter before the first pool |
| 7 | `str r0,[sp,#148]` | `str r0,[sp,#152]` | stack slot shift |
| 13 | `ldr r0,[pc,#152]` | `ldr r0,[pc,#56]` | literal-pool moved |

The first literal pool sits at `0x12e340` (original) vs `0x12e2e0` (ours) — a
0x60 B displacement that is the single largest driver of the low raw ratio,
because every `ldr [pc,#…]` and every intra-function branch past the pool
mismatches textually even when the target is semantically identical.

---

## 3. Full 54-run residual table

`n` = number of differing lines in the run. Addresses are function-relative
(original/ours). `first differing pair` is the first original→ours mismatch in
the run.

| run | n | orig rel | ours rel | first differing pair (orig -> ours) | class |
|---:|---:|---|---|---|---|
| 1 | 1 | `0x00008`-`0x00008` | `0x00008`-`0x00008` | `vpush {d8-d13}` -> `vpush {d8-d11}` | frame / stack slot / literal-pool immediate |
| 2 | 1 | `0x0000e`-`0x0000e` | `0x0000e`-`0x0000e` | `str r3,[sp,#100]` -> `str r3,[sp,#112]` | frame / stack slot / literal-pool immediate |
| 3 | 1 | `0x00014`-`0x00014` | `0x00014`-`0x00014` | `ldr r0,[pc,#728]` -> `ldr r0,[pc,#632]` | frame / stack slot / literal-pool immediate |
| 4 | 1 | `0x00034`-`0x00034` | `0x00034`-`0x00034` | `ldr r0,[pc,#700]` -> `ldr r0,[pc,#604]` | frame / stack slot / literal-pool immediate |
| 5 | 1 | `0x0004a`-`0x0004a` | `0x0004a`-`0x0004a` | `ldr r0,[pc,#684]` -> `ldr r0,[pc,#588]` | frame / stack slot / literal-pool immediate |
| 6 | 1 | `0x00084`-`0x00084` | `0x00084`-`0x00084` | `bne.w 0xb8a` -> `bne.w 0xb44` | frame / stack slot / literal-pool immediate |
| 7 | 1 | `0x00098`-`0x00098` | `0x00098`-`0x00098` | `str r0,[sp,#148]` -> `str r0,[sp,#152]` | frame / stack slot / literal-pool immediate |
| 8 | 1 | `0x00166`-`0x00166` | `0x00166`-`0x00166` | `beq.w 0x69e` -> `beq.w 0x678` | frame / stack slot / literal-pool immediate |
| 9 | 1 | `0x0018a`-`0x0018a` | `0x0018a`-`0x0018a` | `ldr r3,[pc,#368]` -> `ldr r3,[pc,#272]` | frame / stack slot / literal-pool immediate |
| 10 | 1 | `0x001dc`-`0x001dc` | `0x001dc`-`0x001dc` | `beq.n 0x2ac` -> `beq.n 0x2c0` | waypoint tail + otherLock#1 + landmark guard + otherLock#2 |
| 11 | 1 | `0x001e6`-`0x001e6` | `0x001e6`-`0x001e6` | `beq.n 0x2ac` -> `beq.n 0x2c0` | waypoint tail + otherLock#1 + landmark guard + otherLock#2 |
| 12 | 1 | `0x0024a`-`0x0024a` | `0x0024a`-`0x0024a` | `cbz r0,0x290` -> `cbz r0,0x2a4` | waypoint tail + otherLock#1 + landmark guard + otherLock#2 |
| 13 | 1 | `0x00264`-`0x00264` | `0x00264`-`0x00264` | `ldr r0,[pc,#152]` -> `ldr r0,[pc,#56]` | waypoint tail + otherLock#1 + landmark guard + otherLock#2 |
| 14 | 136 | `0x0028e`-`0x0040a` | `0x0028e`-`0x0040c` | `b.n 0x2ac` -> `b.n 0x2c0` | waypoint tail + otherLock#1 + landmark guard + otherLock#2 |
| 15 | 309 | `0x00414`-`0x00748` | `0x00416`-`0x00754` | `cmp r0,#148` -> `cmp r0,#152` | planet-section test + planet loop |
| 16 | 185 | `0x00758`-`0x00976` | `0x00764`-`0x0097e` | `add.w r0,r4,#376` -> `cmp.w sl,#2` | block-order shift: original planet-lock tail/landmark setup vs ours landmark body |
| 17 | 163 | `0x0097c`-`0x00b24` | `0x00982`-`0x00b2c` | `ldr r2,[r2,#4]` -> `str r6,[sp,#0]` | block-order shift: original planet-lock tail/landmark setup vs ours landmark body |
| 18 | 254 | `0x00b2a`-`0x00e06` | `0x00b30`-`0x00e02` | `mov r1,r6` -> `str.w r0,[r4,#420]` | block-order shift: original planet-lock tail/landmark setup vs ours landmark body |
| 19 | 1 | `0x00e12`-`0x00e12` | `0x00e0e`-`0x00e0e` | `bgt.n 0xe4e` -> `bhi.n 0xe6a` | enemy loop body |
| 20 | 3 | `0x00e1c`-`0x00e22` | `0x00e18`-`0x00e1e` | `bmi.n 0xe4e` -> `blt.n 0xe6a` | enemy loop body |
| 21 | 1 | `0x00e32`-`0x00e32` | `0x00e2e`-`0x00e2e` | `bgt.n 0xe4e` -> `bhi.n 0xe6a` | enemy loop body |
| 22 | 164 | `0x00e3c`-`0x00ffe` | `0x00e38`-`0x0100c` | `bmi.n 0xe4e` -> `blt.n 0xe6a` | enemy loop body |
| 23 | 66 | `0x01004`-`0x010b4` | `0x01012`-`0x010c6` | `it ne` -> `bne.w 0x112c` | enemy loop body |
| 24 | 1 | `0x010ba`-`0x010ba` | `0x010cc`-`0x010cc` | `cbz r0,0x10ee` -> `cbz r0,0x1104` | enemy loop body |
| 25 | 1 | `0x010be`-`0x010be` | `0x010d0`-`0x010d0` | `cmp r8,r5` -> `cmp r8,r3` | enemy loop body |
| 26 | 2 | `0x010c6`-`0x010c8` | `0x010d8`-`0x010da` | `b.n 0x10f8` -> `b.n 0x110e` | enemy loop body |
| 27 | 1 | `0x010cc`-`0x010cc` | `0x010de`-`0x010de` | `cbz r0,0x113e` -> `cbz r0,0x115a` | enemy loop body |
| 28 | 7 | `0x010ea`-`0x010f8` | `0x010fc`-`0x0110a` | `ldr r6,[sp,#104]` -> `movs r0,#1` | enemy loop body |
| 29 | 67 | `0x010fe`-`0x011b8` | `0x01110`-`0x011ca` | `ldr r0,[sp,#140]` -> `movs r6,#17` | enemy loop body |
| 30 | 15 | `0x011c0`-`0x011ec` | `0x011d0`-`0x01200` | `.word 0x46bb8000` -> `.word 0x000e127c` | enemy lock tail + gas-cloud loop head |
| 31 | 47 | `0x011f0`-`0x01280` | `0x01204`-`0x01296` | `addeq.w r1,r4,#216` -> `addeq.w r0,r4,#232` | enemy lock tail + gas-cloud loop head |
| 32 | 58 | `0x01286`-`0x0133c` | `0x0129e`-`0x0135e` | `blx 0xfff3615c` -> `blx 0xfff387d8` | enemy lock tail + gas-cloud loop head |
| 33 | 1 | `0x01340`-`0x01340` | `0x01362`-`0x01362` | `addeq.w r1,r4,#116` -> `addeq.w r0,r4,#116` | enemy lock tail + gas-cloud loop head |
| 34 | 247 | `0x01348`-`0x01604` | `0x0136a`-`0x01632` | `movs r5,#17` -> `movs r6,#17` | enemy lock tail + gas-cloud loop head |
| 35 | 1 | `0x01610`-`0x01610` | `0x0163e`-`0x0163e` | `bgt.n 0x165c` -> `bgt.n 0x164a` | gas-cloud loop tail + asteroid setup |
| 36 | 142 | `0x0161a`-`0x017b6` | `0x01648`-`0x017d4` | `bmi.n 0x165c` -> `bpl.n 0x1650` | gas-cloud loop tail + asteroid setup |
| 37 | 116 | `0x017ba`-`0x018f8` | `0x017d8`-`0x01910` | `strne.w r0,[r4,#328]` -> `strne r0,[r4,#56]` | gas-cloud loop tail + asteroid setup |
| 38 | 38 | `0x01900`-`0x01964` | `0x01918`-`0x01984` | `mov.w r1,#1065353216` -> `vcvt.s32.f32 s0,s0` | asteroid loop body |
| 39 | 70 | `0x01968`-`0x01a38` | `0x01988`-`0x01a58` | `ldmia r3,{r1,r2,r3}` -> `blx 0xfff33fe4` | asteroid loop body |
| 40 | 40 | `0x01a3c`-`0x01aac` | `0x01a5c`-`0x01acc` | `str r0,[sp,#144]` -> `str r0,[sp,#148]` | asteroid loop body |
| 41 | 119 | `0x01ab4`-`0x01c0a` | `0x01ad4`-`0x01c2a` | `.word 0x000e0844` -> `.word 0x000e0826` | asteroid loop body |
| 42 | 1 | `0x01c12`-`0x01c12` | `0x01c32`-`0x01c32` | `cbz r2,0x1c20` -> `cbz r5,0x1c46` | asteroid loop body |
| 43 | 11 | `0x01c18`-`0x01c34` | `0x01c38`-`0x01c52` | `cbz r3,0x1c34` -> `cbnz r3,0x1c4c` | asteroid loop body |
| 44 | 103 | `0x01c3c`-`0x01d4c` | `0x01c58`-`0x01d6a` | `cmp r3,#0` -> `adds r2,#11` | asteroid loop body |
| 45 | 13 | `0x01d52`-`0x01d72` | `0x01d6e`-`0x01d8e` | `ldr r0,[r0,#0]` -> `movs r1,#0` | asteroid loop body |
| 46 | 77 | `0x01d7a`-`0x01e3a` | `0x01d96`-`0x01e52` | `b.n 0x1d96` -> `cmp r1,r0` | asteroid loop body |
| 47 | 132 | `0x01e3e`-`0x01fa8` | `0x01e56`-`0x01fcc` | `str r0,[r1,#0]` -> `str r0,[r4,#8]` | asteroid loop body |
| 48 | 489 | `0x01fae`-`0x02544` | `0x01fd4`-`0x0255c` | `vmov s2,r0` -> `vmov s4,r0` | asteroid tail + music tail |
| 49 | 68 | `0x0254c`-`0x02606` | `0x02562`-`0x02620` | `blx 0xfff33720` -> `blx 0xfff33bc4` | music tail |
| 50 | 4 | `0x0260e`-`0x02616` | `0x02626`-`0x0262c` | `str r1,[sp,#0]` -> `cbz r1,0x263c` | music tail |
| 51 | 27 | `0x0261c`-`0x0265c` | `0x02630`-`0x02670` | `blx 0xfff37db8` -> `movs r1,#0` | music tail |
| 52 | 1 | `0x02660`-`0x02660` | `0x02674`-`0x02674` | `add r0,sp,#232` -> `add r0,sp,#184` | music tail |
| 53 | 1 | `0x02666`-`0x02666` | `0x0267a`-`0x0267a` | `b.n 0x266a` -> `b.n 0x2684` | music tail |
| 54 | 27 | `0x0266a`-`0x026b0` | `0x0267e`-`-0x0001` | `add r0,sp,#196` -> `b.n 0x268a` | music tail (tail-merge of local `String` dtors) |

---

## 4. Detailed classification by region

### 4.1 Waypoint tail → `otherLock` chain → landmark guard (runs 10–14)

Original builds the `otherLock` predicate as a **straight `cbnz/cbnz/cmp+beq`
chain** with `otherLock` in `r5`:

```asm
002ac mov  r1,r4
002ae ldr.w r0,[r1,#12]!      ; &lockedAsteroid
002b2 str  r1,[sp,#124]
002b4 cbnz r0,0x2c2           ; lockedAsteroid != null
002b6 ldr  r0,[r4,#56]        ; lockedGasCloud
002b8 cbnz r0,0x2c2
002ba ldr  r0,[r4,#28]        ; field_0x1c
002bc cmp  r0,#0
002be beq.w 0xb72
002c2 movs r5,#1              ; otherLock = true
```

Ours if-converts the same predicate, keeps `otherLock` in `r9`, and emits
`mov.w r9,#1`:

```asm
002c2 mov  r1,r4
002c4 ldr.w r0,[r1,#12]!
002c8 str  r1,[sp,#124]
002ca ldr  r6,[r5,#0]
002cc cbnz r0,0x2da
002ce ldr  r0,[r4,#56]
002d0 cbnz r0,0x2da
002d2 ldr  r0,[r4,#28]
002d4 cmp  r0,#0
002d6 beq.w 0x25aa            ; cold otherLock=false tail
002da mov.w r9,#1
```

The cold `otherLock=false` tail lives at `0x25aa` in ours (`ldr.w r9,[r4,#20];
cmp.w r9,#0; it ne; movne.w r9,#1; b.w 0x2de`) versus a straight fall-through
in the original. The landmark guard (`getSystem`/`getCurrentCampaignMission`,
`109/110/111`, `campaign > 93`) is the same sequence but shifted by the
0x14 B prologue difference. **No missing statement** — only block placement and
the `r5`→`r9` register choice.

### 4.2 Planet section + planet loop (run 15, 309 lines)

Same body. The residual is entirely the 0x60 B literal-pool displacement and
the `mode` stack slot (`[sp,#148]` vs `[sp,#152]`), which shifts every
`cmp r0,#148`→`cmp r0,#152` and every loop-carried branch. The planet loop
head, `getStations`/`getStationEnumIndex`, and `DrawString` order match.

### 4.3 Planet-lock tail vs landmark setup — the major block-order mismatch (runs 16–18)

This is the single largest structural divergence. **Original** places the
planet-lock tail *before* the landmark loop setup:

```text
12e6ee  cold tail: prologue's !alienOrbit autopilot computation
12e748  planet-lock tail
12e79c  literal pool (5 words)
12e7a8  landmark loop setup
12e7d4  landmark loop body
12eb42  landmark tail
```

**Ours** places an *invented recompute* of the autopilot comparison there and
moves the planet-lock tail *after* the landmark block:

```text
12e6c8  cold tail
12e722  invented recompute (autopilot comparison)
12e780  landmark setup
12e7a8  literal pool
12e7bc  landmark body (loop head rotated out of line)
12eb42  landmark tail
12ebd4  planet-lock tail after landmark block
```

The invented recompute is source lines 615–625:

```cpp
if (!alienOrbit) {
    int autoPilotTarget = this->level->getPlayer()->getAutoPilotTarget();
    SolarSystem *system = (SolarSystem *) (intptr_t) Globals::status->getSystem();
    int stationIndex = system->getStationEnumIndex(
            ((Array<int> *) ((SolarSystem *) (intptr_t) Globals::status->getSystem())->getStations())
                    ->data()[this->planetDockIndex]);
    onPlanetCourse = this->level->getPlayer()->goingToPlanet() &&
                     !this->level->getPlayer()->isDockingToPlanet() &&
                     autoPilotTarget == ((Array<int> *) this->planetTargets)->data()[stationIndex];
    this->field_0x1a8 = onPlanetCourse;
}
```

Original computes this **once**, in the prologue cold tail at `0x12e6ee`, and
branches back to `0x12e1bc`. Ours recomputes it and writes `field_0x1a8`
twice (one `strb` in the prologue, one at `0x12e722`), which is why the call
multiset is inflated (see §6).

### 4.4 Enemy loop (runs 19–29, ~310 lines)

The enemy loop is where the **distance-first `inSight`** shape matters. Original
loads the crosshair first, converts to int, then does the distance test *before*
the bounds test:

```asm
; original, ~0xe3c..0x10b4
vldr  s26,[crosshair]        ; PlayerEgo::crosshairPos.x
vldr  s24,[crosshair+4]      ; PlayerEgo::crosshairPos.y
vcvt  ...                    ; (int)crosshairPos
... VectorLength(egoPos - targetPos) < 60000.0f ...
; then screenX/screenY bounds
```

Ours does the bounds test first and the distance test last (source lines
796–801):

```cpp
if (this->screenX > cx - this->field_0x124 && this->screenY > cy - this->field_0x124 &&
    this->screenX < cx + this->field_0x124 && this->screenY < cy + this->field_0x124 &&
    AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) {
    inSight = true;
}
```

The original order is:

```cpp
enemy->field_0x73 = 0;
float crossX = PlayerEgo::crosshairPos.x;
float crossY = PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > (int) crossX - this->field_0x124 &&
    this->screenX < (int) crossX + this->field_0x124 &&
    this->screenY > (int) crossY - this->field_0x124 &&
    this->screenY < (int) crossY + this->field_0x124) {
    inSight = true;
}
enemy->field_0x73 = inSight;
```

Restoring this shape is the *only* way to get `vpush {d8-d13}` (it keeps
`s24`/`s26` live across the loop), but it re-rolls every block after the planet
loop and measured **67.995983 %** (see §7, variant A).

The rest of the enemy loop residual is register split: original keeps
`i`/`bossSeen`/`wreck` in different registers than ours (`cmp r8,r5` vs
`cmp r8,r3` at run 25), and the `it ne`/`bne.w` if-conversion at run 23.

### 4.5 Enemy lock tail + gas-cloud loop (runs 30–34, ~370 lines)

Same body, shifted by ~0x22 B. Run 30 contains literal-pool words
(`.word 0x46bb8000` original vs `.word 0x000e127c` ours) — i.e. the pool
boundary falls inside the run, so the raw `difflib` sees a completely different
token stream. Runs 31/33/34 are `addeq.w r1,r4,#…` vs `addeq.w r0,r4,#…` and
`movs r5,#17` vs `movs r6,#17`: register allocation only.

### 4.6 Gas-cloud tail + asteroid setup (runs 35–37, ~260 lines)

Same body. Run 37 (`strne.w r0,[r4,#328]` vs `strne r0,[r4,#56]`) is a
`strne` width/offset difference from the register-allocation shift, not a
different store target (`#328` is `field_0x148` in the original, written as
`strne.w`; ours uses a different base register and offset encoding).

### 4.7 Asteroid loop (runs 38–47, ~600 lines)

The asteroid loop is the second-largest residual. The main semantic candidate is
the **dead/wreck predicate**: original likely evaluates `isDead() || isDying()`
twice (once for `dead`, once for `wreck`), while the baseline uses a single
`dead` local reused for both:

```cpp
bool dead = asteroid->isDead() || asteroid->isDying();            // line 1098
bool wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying()); // line 1103
```

The call multiset confirms this: original `isDead` 17 / `isDying` 13; ours
`isDead` 17 / `isDying` 13 in the whole function but the *asteroid* block
calls differ. Prior notes measured the two-call shape at ~71–72 %. Runs 42–47
are the offscreen-wreck x/y branches and the asteroid lock tail; all are block
placement and register allocation.

### 4.8 Asteroid tail + music tail (runs 48–54, ~700 lines)

Run 48 (489 lines) is the asteroid tail plus the whole music tail merged into
one positional run because the first `vmov s2,r0` vs `vmov s4,r0` shifts the
entire remainder. Runs 49–54 are the music switch/ambient/race-music lowering:
`cmp r0,#108`/`#101` vs `#108`/`#101` reordered, `movw r1,#2242` vs
`movwlt r1,#2241` (same `ite lt` pattern, different register), and the local
`String` destructor tail-merge (run 54: original `add r0,sp,#196` vs ours
`b.n 0x268a`). All are register allocation and block placement.

---

## 5. Concrete faithful patch proposals

The following are the likely original source shapes. Each was applied and
measured; **all score below the tuned baseline** (see §7). They are recorded so
a future pass can combine them with a block-placement fix.

### 5.1 Enemy `inSight` distance-first

```cpp
enemy->field_0x73 = 0;
float crossX = PlayerEgo::crosshairPos.x;
float crossY = PlayerEgo::crosshairPos.y;
bool inSight = false;
if (AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f &&
    this->screenX > (int) crossX - this->field_0x124 &&
    this->screenX < (int) crossX + this->field_0x124 &&
    this->screenY > (int) crossY - this->field_0x124 &&
    this->screenY < (int) crossY + this->field_0x124) {
    inSight = true;
}
enemy->field_0x73 = inSight;
```

Restores `vpush {d8-d13}` and the exact prologue, but re-rolls the body.

### 5.2 Remove the invented recompute (lines 615–625)

Delete the `if (!alienOrbit) { … this->field_0x1a8 = onPlanetCourse; }` block
after the planet loop. Original computes it once in the prologue cold tail and
branches back to `0x12e1bc`. This removes the extra `getStations`,
`getStationEnumIndex`, 2× `getSystem`, `goingToPlanet`, `isDockingToPlanet`,
`getAutoPilotTarget`, and 3× `getPlayer` calls.

### 5.3 Planet-lock `field_0x14 == 0` arm — swap the stores

```cpp
} else if (this->field_0x14 == 0) {
    this->field_0x18 = 0;
    this->planetLockTimer = 0;
} else {
    this->planetLockTimer = 0;
}
```

The baseline writes `planetLockTimer` then `field_0x18`; swapping prevents clang
from merging/hoisting the two stores and is the `norec_swapstores` shape.

### 5.4 Landmark `i==0` arm — assign `this->lockLabel` directly

```cpp
KIPlayer *landmark = this->landmarks->data()[i];
this->lockLabel = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                     this->cameraX, this->cameraY, this->cameraZ);
Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + this->originX,
                            this->screenY + Globals::layout->field_0x4 * 2, false);
```

Original has `String::operator=` at `0x12eb0a` + `String::~String` at
`0x12eb10`; the baseline uses a tuned local `String label` (w6721 shape) and is
therefore short one `String::operator=`.

### 5.5 Landmark `i==1||i==3` arm — unfold to if/else-if

```cpp
} else if ((i & 0x7fffffff) == 1) {
    Globals::Canvas->DrawImage2D(this->image_0xbc, this->screenX, this->screenY, 17, 68);
} else if ((i & 0x7fffffff) == 3) {
    Globals::Canvas->DrawImage2D(this->image_0xb8, this->screenX, this->screenY, 17, 68);
}
```

Original is an unfolded `bic` + `cmp #1` + `cmp #3`; the baseline uses
`__builtin_expect((i & 0x7fffffff) == 1 || (i & 0x7fffffff) == 3, 0)` and clang
folds it to `(i|2)==3`.

### 5.6 Asteroid dead/wreck predicate — two separate expressions

```cpp
bool dead = asteroid->isDead() || asteroid->isDying();
...
bool wreck = asteroid->hasCargo && (asteroid->isDead() || asteroid->isDying());
```

Keep both as written (do not reuse `dead` for `wreck`). Prior notes measured the
two-call shape at ~71–72 %.

---

## 6. Call-multiset deltas (baseline vs original)

```text
String::operator=            6 / 5
SolarSystem::getStations     2 / 3
SolarSystem::getStationEnumIndex 1 / 2
Level::getPlayer            29 / 32
Status::getSystem            5 / 7
KIPlayer::isDead            17 / 16
KIPlayer::isDying           13 / 12
PlayerEgo::goingToPlanet     1 / 2
PlayerEgo::isDockingToPlanet 4 / 5
PlayerEgo::getAutoPilotTarget 1 / 2
```

Extra calls come from:
- the invented planet-course recompute (`getStations`, `getStationEnumIndex`,
  2× `getSystem`, `goingToPlanet`, `isDockingToPlanet`, `getAutoPilotTarget`,
  3× `getPlayer`) — §5.2;
- the local `String label` in the landmark `i==0` arm (`String::operator=`
  deficit of 1) — §5.4.

The `isDead`/`isDying` deltas (17/16, 13/12) are the asteroid predicate — §5.6.

---

## 7. Variants measured this wave

Source restored to baseline after each. All below baseline `74.541284 %`:

| variant | size | raw % | M |
|---|---:|---:|---:|
| **baseline** | **9908** | **74.541284** | **2600** |
| A: distance-first `inSight` float locals | 9896 | 67.995983 | 2370 |
| B: remove recompute | 9832 | 63.451850 | 2204 |
| C: B + swapped `field_0x18`/`planetLockTimer` | 9828 | 71.445020 | 2482 |
| D: A + C | 9816 | 66.945125 | 2324 |
| E: C + faithful `this->lockLabel` in `i==0` | 9844 | 71.153293 | 2474 |
| F: E + faithful `if/else-if` landmark arm | 9840 | 70.994674 | 2466 |
| G: F + distance-first `inSight` | 9828 | 66.493806 | 2308 |

Prior notes also record:
- `norec + swapped stores` = `9840 B / 70.733813 %`
- faithful distance-first alone = `9896 B / 67.995983 %`
- faithful `lockLabel` alone ≈ `71.61 %`
- faithful `if/else-if` arm alone = `9900 B / 73.211982 %`

The pattern is consistent: every faithful shape is 8–170 B **shorter** than the
tuned baseline and re-rolls every block after the planet loop, dropping the raw
ratio by 1–11 points. The baseline is a strong `difflib` local optimum that
trades a few extra instructions for block placement that happens to align.

---

## 8. Remaining root cause

The true source shape is now essentially pinned:

1. distance-first `inSight` (§5.1);
2. one autopilot computation, no recompute (§5.2);
3. both landmark arms assign `this->lockLabel` (§5.4);
4. unfolded `i==1` / `i==3` if/else-if (§5.5);
5. likely two asteroid dead checks (§5.6).

But that faithful shape is 100–170 B shorter and re-rolls every block after the
planet loop, scoring 63–68 %. The blocker is **`MachineBlockPlacement` /
`BranchFolder` / register allocation**, specifically:

- `otherLock`/`status` in `r5`/`r6` (original) vs `r9`/`r5` (ours);
- first literal pool at `0x12e340` (original) vs `0x12e2e0` (ours) — a 0x60 B
  displacement that alone mismatches every `ldr [pc,#…]` and every branch past
  the pool;
- planet-lock tail slot (original before landmark setup; ours after);
- rotated landmark loop head;
- enemy-loop index/`bossSeen`/`wreck` register split.

No source-only edit found this wave improves the baseline. The next productive
attack is to make the faithful shape (A–G) *and* recover the block placement,
e.g. by reordering the planet-lock tail before the landmark loop (matching the
original `0x12e748` slot) while keeping the single autopilot computation, then
re-scoring. That combination was not reachable in this wave because removing
the recompute alone (B) collapses the layout to 63.45 %.

---

## 9. Artifacts

Scratch (not committed, `/dev/shm/w7106`):
`Radar.base.cpp` (pristine source), `score_radar.py`, `radar_verify.txt`,
`radar_orig.dis`, `radar_ours.dis`, `radar_runs.txt`, `run_table.md`,
`runs_radar.py`, `try_variants.py`.

The analysis is self-contained in this note; the scratch files are reproducible
from the commands in §0/§1.
