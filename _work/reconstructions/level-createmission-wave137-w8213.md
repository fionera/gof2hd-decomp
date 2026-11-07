# w8213 (wave-137): `Level::createMission` — escort-loop statement order and the
# case-4 tail register map (one source win: the one-liner tail)

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w8213`, based on master `655e3bcc5`.
Continues `level-createmission-wave136-w8111.md`, `…-wave136-w8112.md`,
`…-wave135-w7775.md`.

Ground truth: `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; `_ZN5Level13createMissionEv` @ `0xaffd8`, size `0x4314` = 17,172 B).
Ghidra MCP was unavailable; the NDK `objdump` disassembly of the original is the
authority (and is what `tools/verify/verify.py` scores).

## Baseline (pristine HEAD, re-measured)

```text
_ZN5Level13createMissionEv   size 17172/17172 B   raw positional diff 5761
report row pct 92.1, 6055 orig / 6064 ours insns
.text diff 84709   .ARM.extab 1395   .rel.plt 4   .note.gnu.build-id 20
.rodata/.data/.bss/.ARM.exidx exact
gate: byte 4507/4524, linked 4507/4524, size 4524/4524, data 843/843,
      imports 207, lint CLEAN, drift 0, RATCHET PASS
```

## 1. Escort loop: statement order is already exact

The station-pirate escort loop (`0xb3bb8`-`0xb3cd8` original) is instruction-for-instruction
identical to ours **except** the `10000` constant:

```text
original preheader (b3b8a..b3bb6)              ours preheader (b3b76..b3b9e)
add.w  r3,sl,#352        ; &friendCount       add.w  r3,sl,#352
adds   r6,#1                                    adds   r6,#1
ldmia  r3,{r0,r1,r2,r3}                         ldmia  r3,{r0,r1,r2,r3}
add.w  r4,r9,r0                                 add.w  r4,r9,r0
add    r4,r1                                    add    r4,r1
add    r4,r2                                    add    r4,r2
add.w  r8,r4,r3                                 add.w  r8,r4,r3
cmp    r6,r8                                    cmp    r6,r8
blt.n  b3af0                                    blt.n  b3adc
ldr    r6,[sp,#20]       ; pirateEscorts        ldr    r6,[sp,#32]   ; pirates
movw   fp,#10000         ; HOISTED              ldr    r5,[sp,#20]   ; pirateEscorts
ldr    r5,[sp,#112]      ; first                add    r6,r5
add    r6,r5                                    ldr    r5,[sp,#104]  ; first
ldr    r5,[sp,#32]       ; pirates              add    r6,r5
add    r6,r5                                    str    r6,[sp,#92]
str    r6,[sp,#104]                             ldrd   r9,r5,[sp,#96]
ldrd   r9,r5,[sp,#96]                           b.n    b3cbc
b.n    b3ccc
```

The loop body is identical apart from the constant materialisation:

```text
original body (x3)                             ours body (x3)
mla    r0,r0,r6,fp                             mov    fp,sl
                                               movw   sl,#10000
                                               mla    r0,r0,r6,sl
                                               mov    sl,fp
```

Everything else matches: `createShip(8,0,getRandomEnemyFighter(8),nullptr,true,false)`,
the interleaved `vldr pirateBase.{x,y,z}` / `nextInt(20000)` / `nextInt(2)` pairs,
the `vcvt.s32.f32`→`vcvt.f32.s32` round-trip for `setPosition((int)pos.x,...)`,
`setToSleep()`, and `strb #1,[...,#303]`. `this->enemies` is reloaded before each of
the three operations, so the source must not cache the element pointer.

**Conclusion:** the escort loop's statement order and call-argument order are already
exact. The residual is a Greedy-RA rematerialisation tie-break (the early MachineLICM
hoist is undone in the body because `sl` still holds `this`); no source spelling of the
loop body or of `escortsEnd` changes it (20+ variants in w8111, 5 more below).

### New variants measured this wave (fresh `link_so` each)

| variant | raw diff | size | pct |
|---|---:|---:|---:|
| baseline | 5761 | 17172 | 92.1 |
| `Vector pos = pirateBase; pos.x += …; pos.y += …; pos.z += …;` | 5774 | 17172 | 92.0 |
| ternary `nextInt(2) != 0 ? -1 : 1` | 5761 | 17172 | 92.1 |
| `int escortsEnd = pirateEscorts; escortsEnd += first; escortsEnd += pirates;` | 5761 | 17172 | 92.1 |
| `int first = stationPirates + guns;` | 5761 | 17172 | 92.1 |

`Reassociate` canonicalises every `escortsEnd` permutation (loads rank 1 before the
`first` add rank 2), so the preheader order `pirates + pirateEscorts + first` is
unreachable from source.

## 2. Case-4 tail: the one-liner reproduces the original register map

Original tail (`b415c`-`b41d0`):

```text
b415c ldr   r0,[pc]          ; &Globals::globals
b415e mov.w fp,#1            ; true, hoisted
b4162 mov.w r9,#0            ; i
b4166 add   r0,pc
b4168 ldr.w sl,[r0]          ; &Globals::globals
b416c b.n   b41c8
b416e ldr.w r0,[sl]          ; getRandomEnemyFighter receiver
b4172 movs  r1,#8
b4174 blx   Globals::getRandomEnemyFighter(int)
b4178 mov   r5,r0
b417a ldr   r0,[sp,#120]     ; &Globals::rnd  (SPILLED)
b417c ldr.w r4,[r8,#264]     ; this->playerRoute
b4180 ldr   r6,[r0]          ; Globals::rnd
b4182 mov   r0,r4
b4184 blx   Route::length()
b4188 mov   r1,r0
b418a mov   r0,r6
b418c blx   AERandom::nextInt(int)
b4190 mov   r1,r0
b4192 mov   r0,r4
b4194 blx   Route::getWaypoint(int)
b4198 strd  r0,fp,[sp]       ; waypoint, true
b419c movs  r0,#0
b419e str   r0,[sp,#8]       ; false
b41a0 mov   r0,r8            ; this
...
b41c8 ldr.w r0,[r8,#248]     ; this->enemies->size() re-read every iteration
b41cc ldr   r0,[r0,#0]
b41ce cmp   r9,r0
b41d0 bcc.n b416e
```

Register map: **`this`=r8, `i`=r9, `Globals::globals`=sl, `&Globals::rnd`=[sp,#120],
`true`=fp**.

The checked-in `int idx` form produced `this`=r9, `i`=r8, `&rnd`=r4, i.e. `this`/`i`
swapped, and evaluated `nextInt` before `getRandomEnemyFighter`. Folding the temporary
back into the single `createShip(...)` statement restores the original evaluation order
and the `this`=r8 / `i`=r9 / `globals`=sl map. The only remaining difference is that
`&Globals::rnd` then takes `fp`, so `true` is rematerialised:

```text
ours (one-liner)
b4156 ldr   r0,[pc]          ; &Globals::globals
b4158 add   r0,pc
b415a ldr.w sl,[r0]
b415e ldr   r0,[pc]          ; &Globals::rnd
b4160 ldr.w r8,[sp,#124]     ; this
b4164 add   r0,pc
b4166 ldr.w fp,[r0]          ; fp = &Globals::rnd
...
b4196 str   r0,[sp,#0]       ; waypoint
b4198 movs  r0,#1            ; true rematerialised
b419a str   r0,[sp,#4]
```

That costs 2 bytes (`raw 5761 -> 5763`), but the instruction stream is closer to the
original: report pct **92.1 -> 92.3**, `.text` diff **84,709 -> 84,711**, size still
exactly 17,172 B.

### Tail variants measured this wave

(pct from the single-symbol scorer; the full-run `verify` value for the landed variant is 92.3.)

| variant | raw diff | size | pct |
|---|---:|---:|---:|
| **one-liner (landed)** | **5763** | **17172** | **92.3** |
| one-liner + `Globals *g = Globals::globals` local | 5763 | 17172 | 92.3 |
| one-liner + `bool hostile = true` local | 5763 | 17172 | 92.3 |
| one-liner + `Level *self = this` alias | 5763 | 17172 | 92.3 |
| one-liner + `AERandom *rnd = Globals::rnd` local | 5764 | 17172 | 92.3 |
| one-liner + `const unsigned n` / plain `unsigned n` | 5769 | 17172 | 92.2 |
| one-liner + `1, 0` literals | 5763 | 17172 | 92.3 |
| one-liner + `AERandom **volatile prnd = &Globals::rnd` | 5876 | 17176 | 91.2 |
| one-liner + `AERandom *volatile rnd = Globals::rnd` | 5876 | 17176 | 91.2 |
| one-liner + `volatile bool hostile = true` | 5920 | 17184 | 91.1 |
| one-liner + `bool hostile = this->enemies != nullptr` | 5920 | 17184 | 91.1 |
| faithful `i < this->enemies->size()` + one-liner | 5904 | 17164 | 91.4 |
| faithful condition + `int idx` | 5905 | 17164 | 91.2 |
| `int idx` (previous source) | 5761 | 17172 | 92.1 |

The faithful per-iteration `this->enemies->size()` condition is 8 B short (17,164) and
puts `this` in `sl`; the `volatile unsigned n` latch is the cheapest size-exact way to
hold 17,172 and is what leaves `this` in `r8`. No formulation found that both re-reads
the size and keeps `this`=r8 / `&rnd` spilled.

## 3. Correction to earlier notes: case-183 block order

The case-183 convoy offset if-chain in the original is **0, 1, 2, else**:
`b1418 beq -> b1438` is case 2, `b141c beq -> b142c` is case 1. Ours emits 0, 2, 1
(literal-pool reorder). Reordering the source to 0,1,2 is neutral (clang
canonicalises). This corrects the wave-135/136 claim that 0,2,1 was original.

## Conclusion

Landed: the one-liner tail (original call order + `this`=r8 / `i`=r9 / `globals`=sl).
The escort loop is already statement-exact; its `10000` remat is a Greedy-RA decision
unreachable from source. The tail's `&rnd`-in-`fp` vs `true`-in-`fp` swap is the last
2-byte source-level difference in that block.

## Gate counts (before -> after)

```text
byte_exact      4507 -> 4507   (4507/4524)
linked_exact    4507 -> 4507   (4507/4524)
size_exact      4524 -> 4524
data_exact       843 -> 843
imports          207 -> 207
lint            CLEAN   -> CLEAN
drift           0       -> 0
RATCHET         PASS    -> PASS
.text diff      84709 -> 84711
.ARM.extab      1395  -> 1395
.rel.plt           4  -> 4
.note.gnu.build-id 20 -> 20
```

## Commands / scratch

```text
cmake --preset default -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w8213/rawfn.py       # positional byte diff inside the symbol
python3 /dev/shm/w8213/score1.py      # verify.py pct for one symbol
python3 /dev/shm/w8213/regiondiff.py  # per-region raw diff
python3 /dev/shm/w8213/secdiff.py     # section size/diff
cmake --build cmake-build-match --target check -j6
```

Scratch: `/dev/shm/w8213/{Level.base.cpp,Level.T1.cpp,rawfn.py,score1.py,
regiondiff.py,secdiff.py,runvar2.py,t_tail*.py,t_esc.py,orig_full.asm,ours_full.asm}`.
