# Hud::draw — wave 122 (w7218): marker per-arm image load + `int` dock total

**Status:** code change + analysis, 2026-11-06 (wave 122). Worktree `wt/w7218`,
base `master 29cd275f6`. Target `_ZN3Hud4drawExxP9PlayerEgobjj`
(`src/game/ui/Hud.cpp`, `Hud::draw`), original size **9332 B** (file `0x163b90`).

Gate on entry and exit (full `check`): **RATCHET PASS**, `byte_exact 4506`,
`linked_exact 4506`, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**, lint
**CLEAN**, drift **0**.

Official per-function score: **95.986038 -> 95.271342 %** (raw
`SequenceMatcher`), size **9332 B** both, normalised instruction streams
**3149 / 3154 -> 3149 / 3153**, diff cost **160 -> 178**, hunks **72 -> 75**.
The score drop is *not* in the marker block: the marker/radar sub-draws become
instruction-for-instruction identical to the original. The drop is the
pre-existing 6-byte boost-block Canvas hoist (see
`hud-draw-wave121-w7118.md`) plus the allocator perturbation of the dock block.

---

## 0. TL;DR

1. The wave-121 note's claim that the marker/radar block is already
   instruction-identical is **wrong by one instruction per arm**.  The current
   source's ternary makes clang select the *address* of the image member
   (`add.w r0,sl,#736; ldr r1,[r0]`); the original loads the *value* in each
   arm (`ldr.w r1,[sl,#736]` / `[sl,#740]`).
2. Reconstructing the original as two full `if/else` draw arms (the iPad check
   duplicated in each arm, tail-merged by clang) makes the marker block
   **byte-identical** to the original: same 7-instruction prologue, same
   per-arm `ldr.w`, same shared iPad tail, same false-arm `b.n`.
3. The original's `total` (dock amount) is **`int`**, not `short`: the original
   does `blx getDockTotalAmount; mov r5,r0` with no `sxth`, while the old
   `short total` emits `sxth.w r0,r8`.  Changing it to `int` is required to keep
   the function at 9332 B once the marker arms are duplicated (marker alone is
   9336 B).
4. Every other faithful spelling measured is worse than the new 95.271342:
   the boost load-after-call collapse (9264 B), the second-bar
   `String`-before-`GetImage2DWidth` order (9260 B), all-int dock types
   (9328 B), and the hoisted-`stationIdx` gamma form (9332 B / 95.859115).
   The old HEAD (95.986038) remains a *fuzzy-metric* local optimum because it
   keeps the wrong ternary load shape and the wrong `short total`, which
   happen to steer the allocator; the new source is the faithful one.

---

## 1. Marker block: per-arm value load (the real difference)

Original `0x16570a..0x1657cc` (file VMA):

```
16570a ldrb.w r0,[sl,#644]        ; touchFlags
16570e lsls   r0,r0,#27           ; bit 0x10
165710 bmi.n  16571a
165712 ldrb.w r0,[sl,#1189]       ; fireForTutorial
165716 cmp    r0,#0
165718 beq.n  1657c0              ; -> autoTurretOnImage arm
16571a ldrh.w r3,[sl,#998]        ; field_0x3e6 (y)
16571e ldrh.w r2,[sl,#996]        ; field_0x3e4 (x)
165722 ldr.w  r1,[sl,#736]        ; orbitMarkerIdleImage  <- value load
165726 ldr    r6,[sp,#92]         ; &Globals::iPad
165728 ldr.w  r0,[r9]             ; Canvas
16572c ldrb   r6,[r6,#0]
16572e cbz    r6,16573e
165730 movs   r6,#68
165732 movs   r5,#17
165734 strd   r5,r6,[sp]
165738 blx    DrawImage2D(...,0x11,0x44)
16573c b.n    165742
16573e blx    DrawImage2D(...,3 args)
165742 ...                        ; isDockingToAsteroid chain
1657c0 ldrh.w r3,[sl,#998]
1657c4 ldrh.w r2,[sl,#996]
1657c8 ldr.w  r1,[sl,#740]        ; autoTurretOnImage  <- value load
1657cc b.n    165726              ; tail-merge into the shared draw
```

Old HEAD `0x165710..0x1657cc` (same semantics, different codegen):

```
165720 add.w  r0,sl,#736          ; address of orbitMarkerIdleImage
165724 ldr    r6,[sp,#92]
165726 ldr    r1,[r0,#0]          ; value load AFTER the address select
165728 ldrh.w r3,[sl,#998]
16572c ldrb   r6,[r6,#0]
16572e ldrh.w r2,[sl,#996]
165732 ldr.w  r0,[r9]
165736 cbz    r6,165746
...
1657c8 add.w  r0,sl,#740
1657cc b.n    165724
```

The ternary `cond ? this->A : this->B` is folded by clang into a select of the
two member *addresses*, then one load.  The original keeps the loads in the
arms, i.e. the source duplicated the whole draw in each arm:

```cpp
if ((this->touchFlags & 0x10) || this->fireForTutorial) {
    if (Globals::iPad)
        Globals::Canvas->DrawImage2D(this->orbitMarkerIdleImage, this->field_0x3e4, this->field_0x3e6, 0x11, 0x44);
    else
        Globals::Canvas->DrawImage2D(this->orbitMarkerIdleImage, this->field_0x3e4, this->field_0x3e6);
} else {
    if (Globals::iPad)
        Globals::Canvas->DrawImage2D(this->autoTurretOnImage, this->field_0x3e4, this->field_0x3e6, 0x11, 0x44);
    else
        Globals::Canvas->DrawImage2D(this->autoTurretOnImage, this->field_0x3e4, this->field_0x3e6);
}
```

With this source the emitted marker block is (shifted +6 by the boost block,
otherwise identical):

```
165720 ldrh.w r3,[sl,#998]
165724 ldrh.w r2,[sl,#996]
165728 ldr.w  r1,[sl,#736]
16572c ldr    r6,[sp,#92]
16572e ldr.w  r0,[r9]
165732 ldrb   r6,[r6,#0]
165734 cbz    r6,165744
...
1657c6 ldrh.w r3,[sl,#998]
1657ca ldrh.w r2,[sl,#996]
1657ce ldr.w  r1,[sl,#740]
1657d2 b.n    16572c
```

The address-tagged normalised diff (`realdiff.py`) now has **zero instruction
hunks in the marker/radar sub-draws** (`0x1b7a..0x1c8e`).  The only hunk in the
range is a literal-pool alignment artefact at the end of the radar block
(`0x1c8e`: original `b.w 0x8e4; nop; .word`, ours `b.w 0x8e4; .word`), not a
sub-draw instruction.  The next instruction hunk is the dock block at
`0x1cfc`.

---

## 2. `total` is `int`

Original dock block `0x1658a6..0x1658b4`:

```
1658a6 mov r0,r5
1658a8 blx getDockTransferedAmount
1658ac mov r4,r0                 ; transferred (int)
1658ae mov r0,r5
1658b0 blx getDockTotalAmount
1658b4 mov r5,r0                 ; total: no sxth -> int
...
1658ea vmov s2,r5
1658f2 vcvt.f32.s32 s2,s2
```

Old `short total` emitted `sxth.w r0,r8` before the `vmov`; `int total`
removes it.  The other dock locals are **not** int in the original under the
current allocator: making `frameH` and `barW` int as well drops the score to
93.66 % / 9328 B (see §4), so only `total` is changed here.

---

## 3. Why the score drops

The marker arms are 2 bytes *shorter* than the old address-select block, but
duplicating the draw changes the global register allocation.  The boost block
remains the wave-121 42-byte hoist (original 36 B: Canvas loaded after
`getBoostRate()` in `r0`, ours loaded before in `r4` with `mov r5,r4` /
`mov r4,r5`).  The +6 B boost shift plus the dock-block allocation perturbation
costs more than the marker block saves, hence 95.986038 -> 95.271342.

The marker block itself is now exact.  The remaining 75 hunks are the same
allocator artefacts catalogued in wave 121 (boost hoist, `Globals::status`
spill timing, second-bar `String`/`GetImage2DWidth` order, gamma reload) plus
the dock `frameH`/`barW` `sxth`/`ubfx` shapes.

---

## 4. Variant matrix (official raw `SequenceMatcher`, full link)

| variant | size | insns | raw pct | cost |
|---|---:|---:|---:|---:|
| **old HEAD (ternary, `short total`)** | **9332** | 3149/3154 | **95.986038** | **160** |
| **landed: marker arms + `int total`** | **9332** | 3149/3153 | **95.271342** | **178** |
| marker arms only (`short total`) | 9336 | 3149/3154 | 93.384103 | 242 |
| marker arms + all-int dock (`frameH`/`total`/`barW`) | 9332 | 3149/3148 | 94.965857 | 192 |
| marker arms + `int total` + `int frameH` | 9332 | 3149/3148 | 95.124663 | 187 |
| marker arms + `int total` + `int barW` | 9328 | 3149/3152 | 93.477226 | 241 |
| marker arms + `int total` + hoisted `stationIdx` | 9332 | 3149/3153 | 95.144399 | 182 |
| marker arms + `int total` + boost `if/else` | 9268 | 3149/3147 | 86.785260 | 453 |
| marker arms + `int total`, anchor removed | 9268 | 3149/3148 | 83.500079 | 554 |
| `int total` only | 9328 | 3149/3153 | 93.970168 | 228 |
| all-int dock only | 9328 | 3149/3148 | 93.663649 | 242 |
| second-bar `String` before `halfW` | 9260 | 3149/3142 | 83.611509 | 560 |
| second-bar faithful `T W H D O Y` | 9260 | 3149/3142 | 83.547926 | 565 |
| second-bar `barH` before `String` | 9260 | 3149/3142 | 82.784931 | 608 |
| boost `if/else` load-after-call | 9264 | 3149/3146 | 86.417792 | 470 |
| hoisted `stationIdx` gamma | 9332 | 3149/3154 | 95.859115 | 164 |
| no `Globals::Canvas &&` anchor | 9264 | 3149/3147 | 83.386277 | 563 |
| plain `Globals::status` (no `st` local) | 9312 | 3149/3148 | 74.988090 | 897 |

Other marker spellings that compile to the old HEAD code (95.986038, 9332 B):
`unsigned int fireImage; if/else assignment`, `bool fire = cond; ...`,
`int fire = cond; ...`, and `int width = GetImage2DWidth(); String text(...);
int halfW = width * 0.5f;`.  Duplicating the ternary *inside* each iPad branch
(`if (iPad) draw(cond ? A : B, 5); else draw(cond ? A : B, 3);`) gives
9364 B / 92.395437.

---

## 5. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w7218
cmake --build cmake-build-match --target check -j6     # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target lint          # CLEAN
cmake --build cmake-build-match --target drift -j6     # 0 fields
python3 tools/verify/verify.py --show _ZN3Hud4drawExxP9PlayerEgobjj
```

Scratch harness in `/dev/shm/w7218`: `test.sh` (link + raw score),
`score2.py`, `realdiff.py` (address-tagged normalised diff), `combo.py`,
`combo3.py`, `combo4.py`, `sweep_types3.py` (64-way dock type sweep).
Pristine HEAD copy: `/dev/shm/w7218/Hud.cpp.orig`; landed variant:
`/dev/shm/w7218/Hud.cpp.ifint`.
