# Radar::draw pass (wave122, w7210) — body/matrix residual classification

**Status:** research note, 2026-11-06. Worktree `wt/w7210`, based on current
`master` (`29cd275f6`, wave-121 state). **No source change landed**; this commit
contains this note only. The target remains the pristine baseline
`src/game/weapons/Radar.cpp`
(`sha256 2f7347aeda7f1d8dbbfa6744b1b4d09c36a36a26aa1689266c81090ed4ff5f45`).

Target `_ZN5Radar4drawEP6PlayerP3Hudi` (`Radar::draw(Player*, Hud*, int)`) in
`src/game/weapons/Radar.cpp`. Original `9908 B`; ours `9908 B`.

This wave re-derives the complete residual from the current tree and focuses on
the four requested areas: the distance-first prologue, the planet-lock tail
slot, the rotated landmark loop head, and the `otherLock`/`status` register
split. It also records a fresh set of measured source variants (including the
faithful distance-first shape with float locals, which is the `67.995983 %`
variant from `radar-draw-body-wave121-w7106.md`).

Read with the earlier notes `radar-draw-wave119-w6976.md` (distance-first exact
prologue), `radar-draw-wave119-w6947.md` (`i==1||i==3` unfolded),
`radar-draw-wave119-w6950.md` (planet/enemy/asteroid block order,
`norec_swapstores`), `radar-draw-wave119-w6967.md` (literal-pool root cause,
`vpush {d8-d13}` only via distance-first), `radar-draw-wave119-w6721.md`
(local `String` in landmark `i==0`), and `radar-draw-wave119-w6730.md`
(structural-sweep local optimum). The conclusions below are consistent with
those notes; the new material is the exact address/register evidence for the
four focus areas and the variant matrix in §7.

---

## 0. Baseline, gate and structural state

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6
cmake --build cmake-build-match --target drift -j6
```

Measured this wave (baseline source, unmodified):

```
osize=9908 usize=9908  pct=74.5%  (verify.py report.json)
```

Full gate after restoring the baseline:

```
compared 4524  avg 99.93%  byte_exact 4506  linked_exact 4506  fuzzy_perfect 4506
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
verify: byte 4506  linked 4506  stubs 0  missing 0  extra 0  avg 99.93
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓
```

Score command (official verifier, normalized/reconciled disassembly,
`difflib.SequenceMatcher(None, on, un, autojunk=False).ratio()`):

```sh
python3 tools/verify/verify.py \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
```

`verify.py --show _ZN5Radar4drawEP6PlayerP3Hudi` confirms
`orig 9908B @0x12e050 | ours 9908B @0x12e050`.

---

## 1. Method

1. `verify.py --show _ZN5Radar4drawEP6PlayerP3Hudi` → side-by-side
   (`/dev/shm/w7210/radar_verify.txt`, 3492 original rows).
2. Raw `objdump -d --start-address=0x12e050 --stop-address=0x1306c0 -M force-thumb`
   of both ELFs (`/dev/shm/w7210/radar_orig.dis`, `radar_ours.dis`).
3. A run generator (`/dev/shm/w7210/runs.py`, `runtable.py`) that aligns the two
   normalized streams and emits every maximal positional differing run with the
   original address of the first differing row.
4. Candidate source edits applied to a pristine copy
   (`/dev/shm/w7210/Radar.base.cpp`), rebuilt with `link_so`, re-scored with the
   official verifier, then reverted.

Result: **54 positional differing runs, 3222 differing instruction slots**
(ours has 3484 normalized instructions vs the original's 3492 — 8 fewer).
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

The prologue *residual* is only the frame/slot/literal-pool consequence of the
body's register pressure (run 1: `vpush {d8-d13}` vs `vpush {d8-d11}`; runs 2/7:
`mode`/`turretMode` stack slots shifted by 12 B; runs 3/4/5/9/13: literal-pool
`ldr [pc,#…]` immediates). The first literal pool sits at `0x12e340` (original)
vs `0x12e2e0` (ours) — a 0x60 B displacement that is the single largest driver
of the low raw ratio, because every `ldr [pc,#…]` and every intra-function
branch past the pool mismatches textually even when the target is semantically
identical.

---

## 3. Full 54-run residual table

`n` = number of differing lines in the run. Addresses are the original's
absolute `.text` address of the first differing row. `first differing pair` is
the first original→ours mismatch in the run.

| run | n | orig addr | first differing pair (orig -> ours) | class |
|---:|---:|---|---|---|
| 1 | 1 | `0x12e058` | `vpush {d8-d13}` -> `vpush {d8-d11}` | frame / d-reg pressure |
| 2 | 1 | `0x12e05e` | `str r3,[sp,#100]` -> `str r3,[sp,#112]` | frame / stack slot |
| 3 | 1 | `0x12e064` | `ldr r0,[pc,#728]` -> `ldr r0,[pc,#632]` | literal pool |
| 4 | 1 | `0x12e084` | `ldr r0,[pc,#700]` -> `ldr r0,[pc,#604]` | literal pool |
| 5 | 1 | `0x12e09a` | `ldr r0,[pc,#684]` -> `ldr r0,[pc,#588]` | literal pool |
| 6 | 1 | `0x12e0d4` | `bne.w 0xb8a` -> `bne.w 0xb44` | block placement |
| 7 | 1 | `0x12e0e8` | `str r0,[sp,#148]` -> `str r0,[sp,#152]` | frame / stack slot |
| 8 | 1 | `0x12e1b6` | `beq.w 0x69e` -> `beq.w 0x678` | block placement |
| 9 | 1 | `0x12e1da` | `ldr r3,[pc,#368]` -> `ldr r3,[pc,#272]` | literal pool |
| 10 | 1 | `0x12e22c` | `beq.n 0x2ac` -> `beq.n 0x2c0` | route tail / otherLock#1 |
| 11 | 1 | `0x12e236` | `beq.n 0x2ac` -> `beq.n 0x2c0` | route tail / otherLock#1 |
| 12 | 1 | `0x12e29a` | `cbz r0,0x290` -> `cbz r0,0x2a4` | route tail / otherLock#1 |
| 13 | 1 | `0x12e2b4` | `ldr r0,[pc,#152]` -> `ldr r0,[pc,#56]` | literal pool |
| 14 | 136 | `0x12e2de` | `b.n 0x2ac` -> `b.n 0x2c0` | route tail / otherLock#1 / landmark guard |
| 15 | 309 | `0x12e456` | `cmp r0,#148` -> `cmp r0,#152` | planet-section test + planet loop |
| 16 | 185 | `0x12e798` | `add.w r0,r4,#376` -> `cmp.w sl,#2` | **planet-lock tail vs landmark setup** |
| 17 | 163 | `0x12e9b6` | `ldr r2,[r2,#4]` -> `str r6,[sp,#0]` | **planet-lock tail vs landmark body** |
| 18 | 254 | `0x12eb68` | `mov r1,r6` -> `str.w r0,[r4,#420]` | **planet-lock tail vs landmark body** |
| 19 | 1 | `0x12ee1e` | `bgt.n 0xe4e` -> `bhi.n 0xe6a` | enemy loop bounds |
| 20 | 3 | `0x12ee28` | `bmi.n 0xe4e` -> `blt.n 0xe6a` | enemy loop bounds |
| 21 | 1 | `0x12ee3e` | `bgt.n 0xe4e` -> `bhi.n 0xe6a` | enemy loop bounds |
| 22 | 164 | `0x12ee48` | `bmi.n 0xe4e` -> `blt.n 0xe6a` | **enemy distance-first vs bounds-first** |
| 23 | 66 | `0x12f01e` | `it ne` -> `bne.w 0x112c` | enemy if-conversion |
| 24 | 1 | `0x12f0d6` | `cbz r0,0x10ee` -> `cbz r0,0x1104` | enemy block placement |
| 25 | 1 | `0x12f0da` | `cmp r8,r5` -> `cmp r8,r3` | enemy register split |
| 26 | 2 | `0x12f0e2` | `b.n 0x10f8` -> `b.n 0x110e` | enemy block placement |
| 27 | 1 | `0x12f0ec` | `cbz r0,0x113e` -> `cbz r0,0x115a` | enemy block placement |
| 28 | 7 | `0x12f106` | `ldr r6,[sp,#104]` -> `movs r0,#1` | enemy register split |
| 29 | 67 | `0x12f11a` | `ldr r0,[sp,#140]` -> `movs r6,#17` | enemy register split |
| 30 | 15 | `0x12f1d8` | `.word 0x46bb8000` -> `.word 0x000e127c` | literal pool boundary |
| 31 | 47 | `0x12f202` | `addeq.w r1,r4,#216` -> `addeq.w r0,r4,#232` | enemy lock tail register split |
| 32 | 58 | `0x12f28c` | `blx 0xfff3615c` -> `blx 0xfff387d8` | enemy lock tail register split |
| 33 | 1 | `0x12f34a` | `addeq.w r1,r4,#116` -> `addeq.w r0,r4,#116` | enemy lock tail register split |
| 34 | 247 | `0x12f350` | `movs r5,#17` -> `movs r6,#17` | gas-cloud loop register split |
| 35 | 1 | `0x12f616` | `bgt.n 0x165c` -> `bgt.n 0x164a` | gas-cloud tail / asteroid setup |
| 36 | 142 | `0x12f61e` | `bmi.n 0x165c` -> `bpl.n 0x1650` | gas-cloud tail / asteroid setup |
| 37 | 116 | `0x12f7c2` | `strne.w r0,[r4,#328]` -> `strne r0,[r4,#56]` | gas-cloud tail register split |
| 38 | 38 | `0x12f8f8` | `mov.w r1,#1065353216` -> `vcvt.s32.f32 s0,s0` | asteroid loop body |
| 39 | 70 | `0x12f972` | `ldmia r3,{r1,r2,r3}` -> `blx 0xfff33fe4` | asteroid loop body |
| 40 | 40 | `0x12fa3e` | `str r0,[sp,#144]` -> `str r0,[sp,#148]` | asteroid loop body |
| 41 | 119 | `0x12fab4` | `.word 0x000e0844` -> `.word 0x000e0826` | literal pool boundary |
| 42 | 1 | `0x12fbfa` | `cbz r2,0x1c20` -> `cbz r5,0x1c46` | asteroid offscreen x/y |
| 43 | 11 | `0x12fc02` | `cbz r3,0x1c34` -> `cbnz r3,0x1c4c` | asteroid offscreen x/y |
| 44 | 103 | `0x12fc28` | `cmp r3,#0` -> `adds r2,#11` | asteroid offscreen x/y |
| 45 | 13 | `0x12fd4c` | `ldr r0,[r0,#0]` -> `movs r1,#0` | asteroid lock tail |
| 46 | 77 | `0x12fd78` | `b.n 0x1d96` -> `cmp r1,r0` | asteroid lock tail |
| 47 | 132 | `0x12fe3c` | `str r0,[r1,#0]` -> `str r0,[r4,#8]` | asteroid lock tail |
| 48 | 489 | `0x12ff9c` | `vmov s2,r0` -> `vmov s4,r0` | asteroid tail + music tail |
| 49 | 68 | `0x13051e` | `blx 0xfff33720` -> `blx 0xfff33bc4` | music tail |
| 50 | 4 | `0x1305f2` | `str r1,[sp,#0]` -> `cbz r1,0x263c` | music tail |
| 51 | 27 | `0x130600` | `blx 0xfff37db8` -> `movs r1,#0` | music tail |
| 52 | 1 | `0x13064e` | `add r0,sp,#232` -> `add r0,sp,#184` | music tail local `String` |
| 53 | 1 | `0x130654` | `b.n 0x266a` -> `b.n 0x2684` | music tail local `String` |
| 54 | 27 | `0x13065a` | `add r0,sp,#196` -> `b.n 0x268a` | music tail local `String` dtors |

---

## 4. The four requested focus areas

### 4.1 Distance-first prologue / enemy `inSight` (runs 1, 19–22)

The original's enemy `inSight` test is **distance-first**: it loads
`PlayerEgo::crosshairPos` into `s26`/`s24`, computes
`VectorLength(egoPos - targetPos)`, compares against `60000.0f`, and only then
does the four `screenX/screenY` bounds tests:

```asm
; original, enemy offscreen branch, 0x12ef4c..0x12efba
12ef4c ldr   r0,[pc,#688]        ; &PlayerEgo::crosshairPos
12ef50 strb.w r6,[r8,#115]       ; enemy->field_0x73 = 0
12ef54 add   r5,sp,#232
12ef56 add   r0,pc
12ef58 ldr   r0,[r0,#0]
12ef5a vldr  s26,[r0]            ; crossX
12ef5e vldr  s24,[r0,#4]         ; crossY
12ef62 mov   r0,r5
12ef64 ldrd  r2,r1,[sp,#108]     ; egoPos
12ef68 blx   AEMath::operator-   ; egoPos - targetPos
12ef6c mov   r0,r5
12ef6e vcvt.s32.f32 s24,s24
12ef72 vcvt.s32.f32 s26,s26
12ef76 blx   AEMath::VectorLength
12ef7a vmov  s0,r0
12ef7e ldr   r5,[sp,#116]
12ef80 vcmpe.f32 s0,s20          ; length < 60000.0f ?
12ef84 vmrs  APSR_nzcv,fpscr
12ef88 bpl.n 12efc0              ; no -> inSight = false
12ef8a vmov  r1,s26              ; crossX (int)
12ef8e ldr.w r0,[r4,#292]        ; field_0x124
12ef92 ldr.w r2,[r4,#252]        ; screenX
12ef96 movs  r6,#0
12ef98 subs  r3,r1,r0            ; crossX - size
12ef9a cmp   r2,r3
12ef9c ble.n 12efc0
12ef9e add   r1,r0               ; crossX + size
12efa0 cmp   r2,r1
12efa2 bge.n 12efc0
12efa4 vmov  r1,s24              ; crossY (int)
12efa8 ldr.w r2,[r4,#256]        ; screenY
...
```

Ours does the bounds tests first and the distance test last
(`Radar.cpp` lines 796–801):

```cpp
bool inSight = false;
if (this->screenX > cx - this->field_0x124 && this->screenY > cy - this->field_0x124 &&
    this->screenX < cx + this->field_0x124 && this->screenY < cy + this->field_0x124 &&
    AbyssEngine::AEMath::VectorLength(this->egoPos - this->targetPos) < 60000.0f) {
    inSight = true;
}
```

Ours converts the crosshair to `int` before the `VectorLength` call and keeps
only `s0`/`s2` live, so it needs `vpush {d8-d11}`. The original keeps
`crossX`/`crossY` in `s26`/`s24` (callee-saved `d12`/`d13`) across the call,
which is exactly why the original's prologue is `vpush {d8-d13}`. The faithful
source shape is:

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

Measured: `68.0 %`, `9896 B` (variant A2). It restores `vpush {d8-d13}` and the
exact prologue, but re-rolls every block after the planet loop and drops the
raw ratio by 6.5 points (see §7).

### 4.2 Planet-lock tail slot (runs 16–18)

This is the single largest structural divergence. The original emits the planet
block's tail (`if (!turretMode && !landmarkHit) { … }`) **immediately after the
planet loop, before the landmark loop is outlined**:

```text
12e3d4  planet loop
12e6ee  prologue cold tail (autopilot comparison; b 12e1bc)
12e748  planet-lock tail  <-- HERE
12e79c  literal pool (5 words)
12e7a8  landmark loop setup
12e7d4  landmark loop body
12eb42  landmark tail
12ec24  enemies
```

Ours emits the planet-lock tail **after the landmark tail**, because the
invented post-loop recompute (source lines 615–625) sits between the planet loop
and the tail:

```text
12e3d4  planet loop
12e6c8  prologue cold tail (autopilot comparison; b 12e1bc)
12e722  invented recompute (getPlayer/getAutoPilotTarget/getSystem/getStations/...)
12e776  landmark loop setup (rotated head)
12e7a8  literal pool
12e7b4  landmark loop body
12eb02  landmark tail
12ebd4  recompute tail (`strb field_0x1a8`)
12ebec  planet-lock tail  <-- HERE
12ec36  enemies
```

The original has **one** `strb.w r0,[r4,#424]` (`field_0x1a8`) at `0x12e1d0`
and **one** `getAutoPilotTarget` / `getStationEnumIndex` / `getPlanetTargets`
call each. Ours has a second `strb` at `0x12ebe8` and a second set of those
calls at `0x12e722..0x12e770`. The recompute is the only reason the tail is
displaced. The faithful patch is to delete lines 615–625:

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

Measured: `63.5 %`, `9832 B` (variant B). It fixes the planet-lock tail slot
(`0x12e758`, within 0x10 of the original's `0x12e748`) and removes the duplicate
calls, but it makes the landmark loop body outline to `0x12f490` (after the
enemies block), collapsing the ratio. The tail slot and the landmark outline
are coupled; see §5.

### 4.3 Rotated landmark loop head (runs 16–18)

The original's landmark loop is **top-tested**: the setup at `0x12e7a8` falls
through to the loop head at `0x12e7d4`

```asm
12e7a8 add.w r0,r4,#376          ; &this->field_0x178
12e7ac str   r0,[sp,#132]
12e7ae add.w r0,r4,#364          ; &this->field_0x16c
12e7b2 str   r0,[sp,#128]
12e7b4 ldr   r0,[sp,#148]        ; turretMode
12e7b6 add.w r9,r4,#396          ; &this->field_0x18c
12e7ba str.w sl,[sp,#144]
12e7be mov.w sl,#0               ; i = 0
12e7c2 eor.w r0,r0,#1
12e7c6 str   r0,[sp,#108]
12e7c8 eor.w r0,r5,#1
12e7cc ldr   r5,[sp,#156]
12e7ce str   r0,[sp,#116]
12e7d0 movs  r0,#0
12e7d2 str   r0,[sp,#152]
12e7d4 ldr.w r0,[r4,#312]        ; landmarks
12e7d8 ldr   r1,[r0,#0]          ; size
12e7da cmp   sl,r1
12e7dc bcs.w 12eb42              ; i >= size -> landmark tail
12e7e0 cmp.w sl,#2               ; i == 2?
...
```

Ours **rotates** the loop: the setup at `0x12e776` ends with
`b.n 0x12eaf6`, and the condition is at the bottom (`0x12eaf6`), with
`bcc.w 0x12e7b4` jumping back to the body:

```asm
12e776 ldr   r0,[sp,#152]
12e778 str.w sl,[sp,#104]
12e77c mov.w sl,#0               ; i = 0
12e780 eor.w r0,r0,#1
12e784 str   r0,[sp,#108]
12e786 add.w r0,r4,#396
12e78a str   r0,[sp,#100]
12e78c add.w r0,r4,#376
12e790 str   r0,[sp,#144]
12e792 add.w r0,r4,#364
12e796 str   r0,[sp,#132]
12e798 movs  r0,#0
12e79a str.w fp,[sp,#116]
12e79e str   r0,[sp,#148]
12e7a0 str.w r9,[sp,#128]
12e7a4 b.n   12eaf6              ; jump to bottom-tested condition
12e7a8 <literal pool>
12e7b4 cmp.w sl,#2               ; body starts here
...
12eaf6 ldr.w r0,[r4,#312]        ; landmarks
12eafa ldr   r1,[r0,#0]          ; size
12eafc cmp   sl,r1
12eafe bcc.w 12e7b4              ; i < size -> body
```

This is a `MachineBlockPlacement`/loop-rotation decision, not a source
difference: the `for` loop is the same. Removing the `continue` in the loop
(variant Q) and removing the `__builtin_expect` hints (variant P) do not
un-rotate it; Q is byte-identical to the baseline (`74.5 %`, `9908 B`), P drops
to `71.4 %`, `9904 B`.

### 4.4 `otherLock` / `status` register split (runs 10–14, 25, 31–34)

The original keeps `otherLock` in **`r5`** and `status` in **`r6`**:

```asm
12e2fc mov   r1,r4
12e2fe ldr.w r0,[r1,#12]!        ; &lockedAsteroid
12e302 str   r1,[sp,#124]
12e304 cbnz  r0,12e312           ; lockedAsteroid != null
12e306 ldr   r0,[r4,#56]         ; lockedGasCloud
12e308 cbnz  r0,12e312
12e30a ldr   r0,[r4,#28]         ; field_0x1c
12e30c cmp   r0,#0
12e30e beq.w 12ebc2
12e312 movs  r5,#1               ; otherLock = true   (r5)
12e314 ldr.w r0,[r4,#312]        ; landmarks
12e318 cbz   r0,12e372
12e31a ldr   r6,[sp,#156]        ; status             (r6)
12e31c ldr   r0,[r6,#0]
12e31e blx   Status::getStation
12e322 blx   Station::getIndex
12e326 cmp   r0,#109
12e328 beq.n 12e338
12e32a ldr   r0,[r6,#0]
12e32c blx   Status::getStation
12e330 blx   Station::getIndex
12e334 cmp   r0,#110
12e336 bne.n 12e354
12e338 movs  r1,#0
12e33a mov   r5,r6               ; skip path: otherLock dead, r5 reused for status
12e33c b.n   12e376
```

On the landmark-loop path (`0x12e354..0x12e36e`) `r5` stays `otherLock = 1`;
only on the `109/110` skip path does clang reuse `r5` for `status` after
`otherLock` becomes dead. That is direct evidence that the original's
`otherLock` is `r5` and `status` is `r6`.

Ours keeps `otherLock` in **`r9`** and `status` in **`r5`**:

```asm
12e310 ldr   r5,[sp,#156]        ; status             (r5)
12e312 mov   r1,r4
12e314 ldr.w r0,[r1,#12]!
12e318 str   r1,[sp,#124]
12e31a ldr   r6,[r5,#0]          ; status->...        (r6)
12e31c cbnz  r0,12e32a
12e31e ldr   r0,[r4,#56]
12e320 cbnz  r0,12e32a
12e322 ldr   r0,[r4,#28]
12e324 cmp   r0,#0
12e326 beq.w 1305fa
12e32a mov.w r9,#1               ; otherLock = true   (r9)
12e32e ldr.w r0,[r4,#312]
12e332 cbz   r0,12e36c
12e334 mov   r0,r6
12e336 blx   Status::getStation
12e33a blx   Station::getIndex
12e33e cmp   r0,#109
12e340 beq.n 12e36c
12e342 mov   r0,r6
...
```

The `mov r5,r6` at `0x12e33a` in the original is a giveaway that `status` was
in `r6` and the compiler reused `r5` for the `otherLock` value on the
`109/110` path. Ours never does this because `status` already occupies `r5`.
Moving the `Status *status = Globals::status;` declaration after the
`otherLock` block (variant T) does not fix the split; it scores `73.3 %`,
`9920 B`.

---

## 5. Concrete patch proposal (the shape to aim for)

The faithful source shape is now pinned:

1. **Distance-first `inSight`** (§4.1, variant A2).
2. **One autopilot computation, no recompute** (§4.2, delete lines 615–625).
3. **`i==0` landmark arm assigns `this->lockLabel` directly** (not a local
   `String label`), matching the original's `String::operator=` at `0x12eb0a`
   and `String::~String` at `0x12eb10`:

   ```cpp
   KIPlayer *landmark = this->landmarks->data()[i];
   this->lockLabel = this->calcDistance(landmark->posX, landmark->posY, landmark->posZ,
                                        this->cameraX, this->cameraY, this->cameraZ);
   Globals::Canvas->DrawString(radarFont(), this->lockLabel, this->screenX + this->originX,
                               this->screenY + Globals::layout->field_0x4 * 2, false);
   ```

4. **Unfold `i==1` / `i==3`** into two `else if` arms (the original is an
   unfolded `bic` + `cmp #1` + `cmp #3`; the baseline folds to `(i|2)==3`):

   ```cpp
   } else if (__builtin_expect((i & 0x7fffffff) == 1, 0)) { // lint: arm_builtin anchor: Radar::draw
       Globals::Canvas->DrawImage2D(this->image_0xbc, this->screenX, this->screenY, 17, 68);
   } else if (__builtin_expect((i & 0x7fffffff) == 3, 0)) { // lint: arm_builtin anchor: Radar::draw
       Globals::Canvas->DrawImage2D(this->image_0xb8, this->screenX, this->screenY, 17, 68);
   }
   ```

5. **Planet-lock `field_0x14 == 0` arm — swap the stores** (`norec_swapstores`):

   ```cpp
   } else if (this->field_0x14 == 0) {
       this->field_0x18 = 0;
       this->planetLockTimer = 0;
   } else {
       this->planetLockTimer = 0;
   }
   ```

6. **Asteroid dead/wreck predicate**: the original tests `hasCargo` first and
   duplicates the `isDead() || isDying()` pair on both paths
   (`0x12faa6..0x12fad4`), giving 17 `isDead` / 13 `isDying` calls; ours tests
   `dead` first and has 16 / 12. The faithful-looking source is the existing
   two separate expressions, but clang's short-circuit ordering differs.

The blocker is that **the faithful shape is 8–170 B shorter and re-rolls every
block after the planet loop**. The measured variants (all below the `74.5 %`
baseline) are in §7. The best faithful combination found this wave is
`B + G1` (remove recompute + mark the landmark guard likely) at `66.0 %`,
`9816 B`: it fixes the planet-lock tail slot but still outlines the landmark
loop to `0x12f490`.

The next productive attack is to keep the faithful shape and recover the
original block placement, specifically:

- make the landmark loop body stay at `~0x12e7a8` while the planet-lock tail is
  at `~0x12e748` (the original's ordering). A source-level way to do this is to
  make the landmark guard's then-branch hot enough that `MachineBlockPlacement`
  keeps the loop body near the front (variant `G1` is a first step, `66.0 %`);
- recover the `otherLock`/`status` split (`r5`/`r6`) by reducing `otherLock`'s
  live range or by making `status` a later load (§4.4);
- recover the first literal pool at `0x12e340` (ours `0x12e2e0`) by making the
  route block's `scannerAvailable` branch hot enough to be placed before the
  pool (the original has it at `0x12e2e0..0x12e2fc`, before the landmark guard
  and pool; ours places it after the pool at `0x12e2f4`).

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
  3× `getPlayer`) — §4.2;
- the local `String label` in the landmark `i==0` arm (`String::operator=`
  deficit of 1) — §5.3;
- the asteroid predicate short-circuit ordering (`isDead`/`isDying` 17/16,
  13/12) — §5.6.

---

## 7. Variants measured this wave

Source restored to the pristine baseline after each. All below the baseline
`74.5 %` / `9908 B`:

| variant | what | size | pct |
|---|---|---:|---:|
| **baseline** | unmodified | **9908** | **74.5** |
| A | distance-first `inSight`, int `cx`/`cy` | 9960 | 65.3 |
| A2 | distance-first `inSight`, faithful float `crossX`/`crossY` | 9896 | 68.0 |
| B | remove post-loop recompute | 9832 | 63.5 |
| AB | A + B | 9884 | 57.5 |
| ABC | A + B + swapped `field_0x18`/`planetLockTimer` | 9880 | 63.6 |
| ABCE | ABC + faithful `this->lockLabel` in `i==0` | 9896 | 64.6 |
| ABCEF | ABCE + unfolded `i==1`/`i==3` | 9904 | 62.5 |
| ABCEFH | ABCEF + `!hasCargo && dead` | 9908 | 63.1 |
| BG1 | B + landmark guard `__builtin_expect(..., 1)` | 9816 | 66.0 |
| BG0 | B + landmark guard `__builtin_expect(..., 0)` | 9840 | 63.2 |
| ABG1 | A + BG1 | 9872 | 57.3 |
| ABG1C | ABG1 + swap | 9876 | 58.3 |
| ABG1CE | ABG1C + faithful `i==0` | 9884 | 59.2 |
| ABG1CEF | ABG1CE + unfolded arm | 9816 | 58.3 |
| A2BG1 | A2 + BG1 | 9824 | 61.8 |
| A2BG1CEF | A2 + BG1 + C + E + F | 9764 | 59.5 |
| S | `__builtin_expect(scannerAvailable, 1)` | 9908 | 74.5 |
| BS | B + S | 9832 | 63.5 |
| R1 | `__builtin_expect(onScreen, 1)` on route block | 9916 | 73.1 |
| R0 | `__builtin_expect(onScreen, 0)` on route block | 9920 | 71.8 |
| T | move `Status *status` after `otherLock` block | 9920 | 73.3 |
| M | move recompute after planet-lock tail | 9928 | 71.5 |
| N | move recompute before planet block | 9920 | 58.1 |
| Q | remove `continue` in landmark loop (nested `if`) | 9908 | 74.5 |
| P | remove landmark `__builtin_expect` hints | 9904 | 71.4 |

The pattern is consistent: every faithful shape is 8–170 B **shorter** than the
tuned baseline and re-rolls every block after the planet loop, dropping the raw
ratio by 1–17 points. The baseline is a strong `difflib` local optimum that
trades a few extra instructions for block placement that happens to align.

---

## 8. Remaining root cause

The true source shape is essentially pinned (distance-first `inSight`; one
autopilot computation; both landmark arms assign `this->lockLabel`; unfolded
`i==1`/`i==3`; likely two asteroid dead checks). But that faithful shape is
100–170 B shorter and re-rolls every block after the planet loop, scoring
58–68 %. The blocker is **`MachineBlockPlacement` / `BranchFolder` / register
allocation**, specifically:

- `otherLock`/`status` in `r5`/`r6` (original) vs `r9`/`r5` (ours);
- first literal pool at `0x12e340` (original) vs `0x12e2e0` (ours) — a 0x60 B
  displacement that alone mismatches every `ldr [pc,#…]` and every branch past
  the pool;
- planet-lock tail slot (original before landmark setup at `0x12e748`; ours
  after landmark tail at `0x12ebec`);
- rotated landmark loop head (original top-tested at `0x12e7d4`; ours
  bottom-tested at `0x12eaf6`);
- enemy-loop index/`bossSeen`/`wreck` register split.

No source-only edit found this wave improves the baseline. The next productive
attack is the faithful shape (`A2 + B + C + E + F`) **plus** a block-placement
fix that keeps the landmark loop near the front and the planet-lock tail in its
original slot, then re-scoring. That combination was not reachable this wave
because removing the recompute alone (B) moves the landmark body to `0x12f490`.

---

## 9. Artifacts

Scratch (not committed, `/dev/shm/w7210`):
`Radar.base.cpp` (pristine source), `radar_verify.txt`, `radar_orig.dis`,
`radar_ours.dis`, `runs.py`, `runtable.py`, `variants.py`, `runone.py`.
The analysis is self-contained in this note; the scratch files are reproducible
from the commands in §0/§1.
