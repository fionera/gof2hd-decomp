# Small tails — wave 189 / w13339: five-target semantic re-derivation, no new size-exact win

Status: **research note, no `src/` change landed.** Worktree `wt/w13339`, branch
`wave189-w13339`, base master `1de8e9254`. Original
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13339`.

The five smallest size-exact residuals were re-derived from the original
disassembly with a semantic/operand/polarity lens rather than another
statement-order sweep.  Every candidate either changes the function size,
re-colours the whole body, or loses to the landed shape.  The tree is left
clean at the base.

## 0. Baseline (all five at base source)

| symbol | VMA | size | raw | aligned | insn |
|---|---|---:|---:|---:|---:|
| `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9332 | **32** | 21 | 3226/3226 |
| `_ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE` | `0x971b8` | 1996 | **40** | 21 | 744/744 |
| `_ZN10ModStationC1Ev` / `C2Ev` | `0xd52d0` | 1008 | **11** | 5 | 374/374 |
| `_ZN15MenuTouchWindow12OnTouchBeginEiiPv` | `0x129bd4` | 1896 | **28** | 16 | 754/754 |
| `_ZN5MGame8OnUpdateEv` | `0x17c8d8` | 11440 | **10** | 112 | 4003/4002 |

Raw is GOT/layout-sensitive; every number below was measured with *all five*
targets at base source unless stated.

## 1. `ModStation::ModStation()` — the farKey operand is semantically wrong

The landed source computes

```cpp
int pitchBits = *(const volatile int *) &pitchTable[race];
int yawBits   = *(const volatile int *) &yawTable[race];
float yaw   = *(const float *) &pitchBits;   // bits(pitchBits)
float pitch = *(const float *) &yawBits;     // bits(yawBits)
MatrixSetRotation(nearKey, yaw, pitch, ...);            // x=bits(pitchBits), y=bits(yawBits)
MatrixSetRotation(farKey, pitch, ...);                  // x=bits(yawBits)
```

The original farKey first argument is **bits(pitchBits)**, not bits(yawBits):
`d546e mov r4,r2` copies `r2 = pitchTable[race]` and `d54f0 mov r2,r4`
passes it as farKey x.  The semantically faithful form (renaming
`pitch`/`yaw` so farKey uses bits(pitchBits), or using `yaw` in the landed
naming) is raw **47** / aligned 15 / size 1008.  The residual moves to the
table-pointer loads (`r4=yawIPad, r3=yaw` vs original `r3=yawIPad, r4=yaw`)
and hoists `mov r4,r2` before the yaw load.  New cells measured:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| landed (farKey = bits(yawBits)) | **11** | 5 | 1008 |
| faithful farKey + `pitchTable` first | 47 | 15 | 1008 |
| faithful farKey + `yawTable` first | 46 | 13 | 1008 |
| faithful farKey + `if (iPad) yawTable=... else ...` | 47 | 15 | 1008 |
| faithful farKey + inline table selection | 698 | 52 | 1000 |
| rename `pitch=bits(pitchBits)`, farKey=`pitch` | 43 | 12 | 1008 |

The landed 11-byte shape is still the best raw; the semantic fix is a net
raw loss.

## 2. `Hud::draw()` — the unbraced anchor is semantically load-bearing

The landed damage block is

```cpp
if (hitFlags & 0x24)
    Status **volatile p = &Globals::status;
    this->field_0x4b8 = 300;          // unconditional in source
```

so ours emits `mov.w r0,#300; it ne; strne r8,[sp,#208]; str.w r0,[sl,#1208]`
and reuses `r0=300` in the following `0x18` block.  The original is the clean
conditional `itt ne; movne r0,#300; strne r0,[sl,#1208]` in **both** blocks.
Bracing the `if` is semantically faithful and makes both damage blocks
byte-identical, but the anchor store is then 4 bytes too many:

| shape | raw | aligned | size |
|---|---:|---:|---:|
| landed unbraced anchor | **32** | 21 | 9332 |
| braced `if` + anchor | 7091 | 423 | 9336 |
| braced `if`, no anchor | 7339 | 928 | 9328 |
| braced `if` + `*(volatile int*)&field = 300` | 7339 | 928 | 9328 |
| braced `if` + volatile store + tail `volatile int = 0` | 7019 | 927 | 9332 |
| braced `if` + volatile store + tail `Status **volatile` | 7334 | 927 | 9336 |
| unbraced `if` + volatile read anchor | 38 | 23 | 9332 |
| two separate `if (hitFlags & 0x24)` statements | 7091 | 423 | 9336 |

The unbraced anchor's unconditional field store is what lets clang hoist
`300` and recover the original's 9332-byte frame/allocation; removing it
re-colours the whole function.  The 13-byte dock residual is the pure
`Canvas`/`top` `r5`/`r6` colouring tie and resisted `int`/`unsigned short`/
`const int`/`y`-instead-of-`top*2` variants (all raw 32).

## 3. `MenuTouchWindow::OnTouchBegin()` — case-15 fresh vs cached layout

The original reloads `Globals::layout` after the first `GetImage2DWidth` call
and reads **both** `field_0xc_leftMargin` (`[r2,#12]`) and
`field_0x20_top` (`[r2,#32]`) from the fresh pointer.  The landed shape keeps
the block-entry `Layout &layout` live across the call (r6) and uses it for
`field_0xc`, so only 26 bytes differ.  New cells (all 1896 B / 754 insn
unless noted):

| shape | raw | aligned | size |
|---|---:|---:|---:|
| landed (local for `field_0xc`, direct for top) | **28** | 16 | 1896 |
| local for top, direct for `field_0xc` | 29 | 15 | 1896 |
| local in first condition, direct for both second fields | 362 | 76 | 1892 |
| local in third condition, direct for both second fields | 322 | 74 | 1888 |
| all-direct (local unused) | 362 | 76 | 1892 |
| `(top + left) + height` reorder (landed locals) | 45 | 23 | 1896 |

The local reference is what forces `&Globals::layout` into `r4` (2-byte
`ldr r4,[sp,#16]`); dropping it makes clang use `fp` (`ldr.w fp,[sp,#16]`,
+2 B) and re-colours the block.  The 2-byte case-3/8 `ip`/`r4` tie is a pure
greedy-RA choice.

## 4. `Generator::createMission()` — `bhi` fold and frame 96 vs 104

The original does not fold `type > 14 ⇒ type != 13`: `9735a bhi.n 97388`
still enters the `race < 4 && type == 13` test, while ours jumps straight to
`973fa`.  The frame is also 96 (original) vs 104 (ours); the original bonus
block reads `rem` unconditionally (`ldr r0,[sp,#32]`) and stores `bonus` once
to `sp+16`.  The faithful body (`int bonus`, `volatile int rem`,
`int remCopy = rem`) is instruction-for-instruction identical but **2000 B**
(one extra EH landing pad + literal-pool `nop`).  New cells:

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| landed (`volatile bonus` + `bv`) | **40** | 21 | 1996 | 744 |
| faithful body (frame 96) | 90 | 31 | 2000 | 746 |
| `volatile uint32_t type` | 1662 | 406 | 2004 | 754 |
| `uint32_t t = type;` used in mask only | 40 | 21 | 1996 | 744 |
| `uint32_t t = type;` used in both tests | 40 | 21 | 1996 | 744 |
| mask-first (`mask && type < 15`) | 1816 | 251 | 1960 | 734 |
| no `__builtin_expect` | 1560 | 271 | 1956 | 737 |

`volatile type` does not recover the fold and costs 8 bytes.  The extra
landing pad is the LSDA call-site entry for the `nextInt` at `0x977ac`
mapping to the dead `sp+68` Mission-String cleanup instead of the live
`sp+80` `agentName` cleanup; it is a compiler EH-layout artifact, not a
source-order lever.

## 5. `MGame::OnUpdate()` — m122 can be made exact, m23 then breaks

Moving the anchor *before* `nextInt` and using `Globals::status` directly in
the m122 body reproduces the original m122 block exactly

```
mov r4,r0
ldr.w r0,[r9]
blx   getCurrentCampaignMission
```

but reserves `r2` for the anchor store (`str r2,[sp,#120]`), so the m23
64-bit compare becomes `ldrd r3,r5` instead of `ldrd r2,r3`, and the extra
2-byte store shifts the literal pool (+361 raw).  Measured (all size-exact
unless noted):

| shape | raw | aligned | size | insn |
|---|---:|---:|---:|---:|
| landed anchor (m23 r4/r5, m122 r5) | **10** | 112 | 11440 | 4003 |
| pre-`nextInt` `Status *volatile = Globals::status` | 361 | 21 | 11440 | 4002 |
| `void *volatile` / `int volatile` / cast pointer variants | 361 | 21 | 11440 | 4002 |
| anchor after `createRadioMessage` | 5211 | 597 | 11396 | 3976 |
| anchor between `nextInt` and the campaign call | 5212 | 597 | 11396 | 3976 |
| `Status *st` after `nextInt` + dummy after call | 345 | 22 | 11440 | 4002 |
| two dummy stores before `nextInt` | 946 | 134 | 11444 | 4004 |

The landed 10-byte shape remains the best raw; the m23/m122 ties are
independent greedy-RA choices.

## 6. Reproduction

```sh
ulimit -v 12000000
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
  _ZN3Hud4drawExxP9PlayerEgobjj \
  _ZN9Generator13createMissionEP5AgentP5ArrayIP11SolarSystemE \
  _ZN10ModStationC1Ev _ZN10ModStationC2Ev \
  _ZN15MenuTouchWindow12OnTouchBeginEiiPv _ZN5MGame8OnUpdateEv
# raw=32/40/11/11/28/10, sizes 9332/1996/1008/1008/1896/11440
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
```

Gate on exit: `byte_exact 4507/4524`, `linked_exact 4507/4524`,
size 4524/4524, `stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`,
data 843/843, imports 207, lint CLEAN, drift 0, parity 5370/5370.
`.text` diff **73,999**, `.ARM.extab` 639 (size/base exact), `.ARM.exidx` 0,
`.rel.plt` 0 (byte-identical), `.rodata` 0, build-id 20, total **74,658**;
file size 2,187,328 exact.

Scratch (not tracked): `/home/debian/GalaxyOnFire2/scratch/w13339/`
(`insdiff.py`, `twodiff.py`, `secdiff.py`, `mg*.py`, `mod*.py`, `mtw*.py`,
`hud*.py`, `gen*.py`, `*.base.cpp`).
