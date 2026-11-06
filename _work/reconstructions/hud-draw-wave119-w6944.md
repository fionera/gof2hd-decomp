# Hud::draw — wave 119 (w6944): call-order census, `int` dock types + `short barH`, EH-pad destructor loss

**Status:** research note, 2026-11-06 (wave 119). Worktree `wt/w6944`, branch
`wave119-w6944`, base `master 7a031742d`. **No source change landed**: the best
measured variant is still HEAD at **9332 B / 96.000000 %**. Target
`_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`), original
size **9332 B** (file `0x163b90`).

Gate on entry and on exit (full `check`): **RATCHET PASS**, `byte_exact 4505`,
`linked_exact 4505`, size-exact 4524/4524, `stub_zero_size 0`, `missing 0`,
`wrong_type 0`, `extra 0`, `data_exact 843/843`, imports **207**, lint **CLEAN**,
drift **0**. 27 sections, file size 2,187,328 B.

Baseline measurement: **9332 B**, fuzzy **96.000000 %**, diff cost **160**,
normalised instruction streams **3149** (orig) / **3154** (ours), **101** diff
hunks. (The `verify.py` metric rounds to 96.0; the raw `SequenceMatcher` ratio is
95.986038 %.)

This note extends `hud-draw-wave119-w6923.md` (read it first — it has the full
anchor/gamma/dock sweep). Everything below is new.

---

## 0. TL;DR

1. **The `int` dock types are confirmed and the *call sequence* is byte-for-byte
   identical to the original except for one call position.** With
   `int frameH` + `int total` + `int barW`, the 420-entry `blx`-target sequence
   of ours matches the original 419/420; the only mismatch is that
   `PaintCanvas::GetImage2DWidth` is emitted **before** the second bar's
   `String text` construction in ours and **after** it in the original.
2. **`int` types alone are 4 B short (9328).** The 4 B gap is *distributed*, not
   a single missing instruction: per-region instruction-count deltas (ours −
   orig) are `mid +2`, `dock1 −5`, `dock2 +4`, `EH pad −2` (net −1 instruction,
   −4 B).
3. **`int frameH` + `int total` + `int barW` + `short barH` lands exactly at
   9332 B / 95.800000 %** (3149/3149 insns, cost 169, 105 hunks). The `short`
   on `barH` supplies the missing `sxth`, but it is *not* faithful (the original
   has no `sxth` for `barH`) and the score is still 0.2 pts below HEAD.
4. **Moving `String text` before `halfW` is the faithful order but breaks the
   EH pad**: it eliminates a `String::~String()` call at `0x1646a2` (mission
   status panel) and shrinks the function to **9256 B / 83.600000 %**.
5. **The 4 B gap is not reachable with any `short`/`int`/`unsigned` combination
   of the dock variables** (48 combinations measured; best 95.8 %).

---

## 1. Call-sequence census (new)

Method: extract every `blx <target>` from the address-tagged disassembly of both
sides and diff the target sequences with `difflib.SequenceMatcher`.

| build | `blx` count orig/ours | call-seq ratio | mismatch |
|---|---:|---:|---|
| HEAD (`short`/`short`/`unsigned short`) | 420 / 420 | 1.000000 | none |
| `int frameH`+`int total`+`int barW` | 420 / 420 | 0.997619 | `GetImage2DWidth` @ `0x165bac` (ours) vs `0x165bd4` (orig) |

So the `short` types in HEAD change *no* call positions; the `int` types change
exactly one, the second bar's `halfW` / `String text` ordering. The original
order is:

```
0x165ba6  ldr r0,[pc]        ; &Globals::gameText
0x165ba8  mov.w r1,#318
0x165bac  ldrb.w r2,[sl,#631]; field_0x277
0x165bb0  add r0,pc
0x165bb2  cmp r2,#0
0x165bb4  ldr r0,[r0,#0]
0x165bb6  ldr r0,[r0,#0]
0x165bb8  it eq
0x165bba  movweq r1,#317
0x165bbe  blx 0x62f70        ; GameText::getText
0x165bc2  mov r1,r0
0x165bc4  add r0,sp,#208
0x165bc6  movs r2,#0
0x165bc8  blx 0x5f028        ; String::String(const String&, bool)
0x165bcc  ldr.w r1,[sl,#892] ; image_0x37c
0x165bd0  ldr.w r0,[r9]
0x165bd4  blx 0x62d84        ; PaintCanvas::GetImage2DWidth  <-- halfW
0x165bd8  vmov s2,r0
0x165bdc  vmov.f32 s0,#96    ; 0.5f
0x165be4  vmul.f32 s0,s2,s0
0x165be8  vcvt.s32.f32 s18,s0
0x165bec  ldr.w r1,[sl,#892]
0x165bf0  ldr.w r0,[r9]
0x165bf4  blx 0x62d90        ; PaintCanvas::GetImage2DHeight  <-- barH
0x165bf8  mov r8,r0
```

i.e. the source order is `String text(...)` → `int halfW = ...` → `int barH =
...`, not the `halfW` → `String` order currently in `Hud.cpp`.

## 2. `int` dock types: the 4 B gap is distributed (new)

`int frameH` + `int total` + `int barW` (all three faithful — the original has
no `sxth`/`uxth` on `transferred`/`total`/`barW`/`frameH`, and uses
`vcvt.f32.s32`/signed `/2`):

| build | size | fuzzy | insns | cost | hunks |
|---|---:|---:|---:|---:|---:|
| HEAD | **9332** | **96.000000** | 3149/3154 | **160** | **101** |
| `int frameH`+`int total`+`int barW` | 9328 | 93.700000 | 3149/3148 | 242 | 145 |
| ... + `top` declared before `barYOffset` | 9328 | 93.900000 | 3149/3148 | 232 | 140 |
| ... + `String text` before `halfW` | 9256 | 83.600000 | 3149/3136 | 555 | 402 |

Per-region instruction-count deltas (ours − orig) for the plain `int` build:

| region | orig | ours | Δ |
|---|---:|---:|---:|
| prologue `0x163b90..0x164010` | 376 | 376 | 0 |
| gamma `0x164010..0x1640e0` | 66 | 66 | 0 |
| mid `0x1640e0..0x165880` | 2102 | 2104 | **+2** |
| dock1 `0x165880..0x165b40` | 240 | 235 | **−5** |
| dock2 `0x165b40..0x165d60` | 176 | 180 | **+4** |
| hint `0x165d60..0x165ee0` | 138 | 138 | 0 |
| EH pad `0x165ee0..0x166010` | 128 | 126 | **−2** |
| **total** | 3226 | 3225 | **−1** |

The `dock1 −5` is *not* five missing source statements; it is a register
allocation difference. The original keeps `barH` in `fp` and moves the canvas
pointer `fp → r5` before the `GetImage2DHeight` call:

```
0x165948  ldr.w r0,[fp]      ; canvas
0x16594c  mov r5,fp          ; save canvas  <-- ours has no such mov
0x16594e  blx 0x62d90        ; GetImage2DHeight
0x165952  mov fp,r0          ; fp = barH
...
0x16599e  ldr r0,[r5,#0]     ; canvas->SetColor
```

Ours instead keeps canvas in `fp` and puts `barH` in `r8`
(`mov r8,r0`), so the `mov r5,fp` is unnecessary. The other −4 are the
`ldr r3,[sp,#96]; ldr r3,[r3,#0]` pair (orig) vs the single `ldr.w r2,[fp]`
(ours) and scheduling of the `if (field_0x27a)` / `if (getType()==174)` image
loads.

## 3. `short barH` supplies the missing 4 B (new)

Adding `short` to the **first** bar's `barH` on top of the three `int` types
emits one `sxth` and lands exactly at 9332 B:

| build | size | fuzzy | insns | cost | hunks |
|---|---:|---:|---:|---:|---:|
| `int frameH`+`int total`+`int barW` + `short barH` (dock1) | **9332** | **95.800000** | 3149/3149 | 169 | 105 |
| ... + `top` declared before `barYOffset` | 9332 | 95.700000 | 3149/3149 | — | — |
| ... + `String text` before `halfW` | 9264 | 83.700000 | 3149/3139 | — | — |
| ... + `short barH` in dock2 too | 9320 | 85.200000 | 3149/3146 | — | — |
| ... + `short top` | 9332 | 95.800000 | 3149/3149 | — | — |
| ... + `short barOffset` | 9336 | 93.700000 | 3149/3151 | — | — |
| ... + `short y` | 9336 | 93.800000 | 3149/3149 | — | — |
| ... + `short fadeColor` | 9340 | 93.600000 | 3149/3152 | — | — |

The `sxth` is emitted at `0x1659f2` (`sxth.w r4,r8`) and the argument store
reorganises to `str r2,[sp,#16]; strd r2,r2,[sp,#8]; str r4,[sp,#4]`; the
original has no `sxth` and uses `strd r2,r2,[sp,#12]; strd fp,r2,[sp,#4]`. So
the 4 B is bought with an instruction the original does not have, which is why
the score is 0.2 pts below HEAD.

All 8 combinations of `{short,int}` for `frameH`/`total`/`barW` on top of
`short barH`:

| frameH | total | barW | size | fuzzy |
|---|---|---|---|---|
| short | short | ushort | 9340 | 93.600000 |
| short | short | int | 9336 | 93.700000 |
| short | int | ushort | 9336 | 93.700000 |
| short | int | int | **9332** | **95.800000** |
| int | short | ushort | 9336 | 93.700000 |
| int | short | int | 9336 | 93.800000 |
| int | int | ushort | **9332** | **95.800000** |
| int | int | int | **9332** | **95.800000** |

## 4. `String text` before `halfW` loses an EH-pad destructor (new)

The faithful order (String → halfW) is reachable but the compiler then deletes
the `String::~String()` call at `0x1646a2` (mission status panel, after the
`DrawString` at `0x16469c`):

```
orig 0x1646a0  add r0,sp,#208
orig 0x1646a2  blx 0x5ee30        ; String::~String()
orig 0x1646a6  b.w 0x164eba
ours 0x164692  b.n 0x164972        ; no destructor
```

That removes one call and shrinks the EH landing pad, giving **9256 B /
83.600000 %** (3149/3136 insns, cost 555, 402 hunks). The loss is a *global*
stack-slot/EH-state effect of the declaration move, not a local scheduling
change — the call sequence then differs by exactly that one destructor.

## 5. Stack-slot swap in dock1 (new)

The original assigns `frameH` to `[sp,#96]` and the `Globals::font` pointer to
`[sp,#92]`; ours (both HEAD and the `int` builds) assigns them the other way
round:

```
orig 0x16588c  str r0,[sp,#96]    ; frameH
orig 0x165894  str r1,[sp,#92]    ; font
ours 0x16588c  str r0,[sp,#92]    ; frameH
ours 0x165894  str r1,[sp,#96]    ; font
```

Everything downstream that reloads `frameH` (`0x1658f6`, `0x1659d8`) or the
layout pointer (`0x165a10`, `0x165a76`, `0x165ae8`) therefore differs by a slot
number, contributing several of the pure-register-rename hunks. Reordering the
`frameH`/`textH` declarations (measured on HEAD) gives 95.600000 %, so the swap
is not a free fix.

## 6. Other variants measured this session (all `int`-types base unless noted)

| variant | size | fuzzy |
|---|---:|---:|
| HEAD | **9332** | **96.000000** |
| `int3` + `int stationIdx` gamma hoist | 9332 | 95.700000 |
| `int3` + `short barH` + `int stationIdx` gamma hoist (via `st`) | 9332 | 95.800000 |
| `int3` + `short barH` + `int stationIdx` gamma hoist (via `Globals::status`) | 9332 | 95.700000 |
| `int3` + `unsigned short barH` | 9332 | 95.800000 |
| `int3` + `int halfW` (no change) | 9332 | 95.800000 |
| `int3` + `short textH` | 9328 | 93.700000 |
| `int3` + `short transferred` | 9324 | 93.400000 |
| `int3` + `short halfW` | 9336 | 93.400000 |
| `int3` + `short barH` + `short halfW` | 9340 | 93.600000 |

## 7. Conclusion

The `int` dock types are faithful and the call sequence proves the rest of the
dock block is structurally correct, but the original's extra 4 B are a
*register-allocation* artefact (the `mov r5,fp` canvas save plus the
`ldr r3,[sp,#96]; ldr r3,[r3,#0]` pair), not a source construct. Buying the 4 B
with `short barH` reaches 9332 B but costs an `sxth` the original does not have,
so HEAD's `short`/`short`/`unsigned short` shape still wins on the fuzzy metric
(96.000000 % vs 95.800000 %).

The next lever is unchanged: find a source construct that raises the `this`
split weight at `bb.28` (gamma) without adding a branch, or one that makes the
dock1 allocator keep `barH` in `fp` (and therefore emit `mov r5,fp`) without
introducing an `sxth`.

## 8. Reproduction

```
cd /home/debian/GalaxyOnFire2/wt/w6944
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6          # RATCHET PASS, byte 4505
```

Scratch harness in `/dev/shm/w6944`: `build.sh` (link_so + score), `score.py`
(official metric: size / pct / insn), `dump.py` (address-tagged objdump of both
sides), `range.py <lo> <hi>` (side-by-side disassembly), `diff.py` /
`diff2.py` / `delta2.py` (normalised diff with cost, address-tagged diff,
per-equal-block delta profile), `regcost.py` (per-0x100 diff cost table),
`measure.py` / `batch_int.py` (`run(label, [(old,new),…])` — restore pristine
`Hud.cpp`, apply exactly-once replacements, build, score). Pristine copy:
`/dev/shm/w6944/Hud.cpp.orig`.
