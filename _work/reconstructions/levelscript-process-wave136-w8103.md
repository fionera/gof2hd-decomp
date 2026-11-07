# LevelScript::process — wave 136 / w8103 (16/32-bit balance: `sl` is `delta`, not `this`)

Status: **analysis only, no source change landed, 2026-11-07**. Target
`_ZN11LevelScript7processEi` (`LevelScript::process`), original `0x137d18`,
**57232 B (0xdf90)**. Owned TU: `src/game/world/LevelScript.cpp`. Base master
`2e51d765d` (wave-135 head). Scratch helpers in `/dev/shm/w8103/` (not tracked):
`fast.py` (size/pct/diffbytes/width), `tbh2.py` (jump-table decoder),
`wcensus2.py` (width-pair census), `width2.py`.

## Result

```
                         before (master)   after (no code change)
size                     57232 / 57232     57232 / 57232   (size-exact)
official verify pct      58.8              58.8
process diffbytes        54316             54316
16-bit / 32-bit insns    12798 / 7909      12798 / 7909
byte_exact               4506              4506
linked_exact             4506              4506
size-exact               4524/4524         4524/4524
data_exact               843/843           843/843
imports                  207               207
lint                     CLEAN ✓           CLEAN ✓
drift                    0                 0
RATCHET                  PASS              PASS
```

Widths are counted from the objdump byte column of the linked `.so` inside
`0x137d18..0x145ca8` (1 hex word = 16-bit, 2 hex words = 32-bit). This exactly
reproduces the mission's numbers: original **13044 16-bit / 7786 32-bit**,
baseline **12798 / 7909** (the mission's 12808/7904 is the pre-w7763 base).
So ours has **246 too few 16-bit and 123 too many 32-bit** encodings, and is
123 instructions short in total.

## NEW decode: the mission-dispatch chain keeps `delta` in `sl`, not `this`

The wave-134/135 notes said "ours dedicates `sl` to `this`". That is only half
true. At the head of the per-mission dispatch chain the two sides differ in
*which* cached value is hoisted into `sl` (r10):

```
orig  0x1387e8  ldr.w r8,  [sp,#96]   ; enemies
      0x1387ec  cmp   r0, #16
      0x1387ee  ldr.w r10, [sp,#76]   ; delta   <-- sl = delta
      0x1387f2  beq.w 13b014
ours  0x1387de  ldr.w r10, [sp,#88]   ; this    <-- sl = this
      0x1387e2  cmp   r0, #16
      0x1387e4  ldr   r6,  [sp,#96]   ; enemies
      0x1387e6  beq.w 13aad4
```

`[sp,#76]` is `delta` (prologue `strd r5,r6,[sp,#72]`: messages@72, delta@76);
`[sp,#88]` is `this` (prologue `str.w r10,[sp,#88]`). In the original `this` is
spilled and reloaded into a low scratch register at every case entry; in ours
`this` owns `sl` and `delta` is reloaded from the stack. This is the single
biggest source of the width deficit: `ldr r5,[sp,#88]` + `ldr r0,[r5,#20]`
(2+2 B) vs `ldr.w r10,[sp,#88]` + `ldr.w r0,[r10,#20]` (4+4 B).

Representative confirmation — case 105 entry (orig `0x13aea4`, ours `0x13a99c`):

```
orig  13aea4  ldr   r0,[sp,#88]        ; this -> r0
      13aea6  ldr   r0,[r0,#28]        ; m_nState
      13aea8  cmp   r0,#2
      13aeac  mov.w r0,r10,lsl #1      ; delta<<1  (r10 = delta)
ours  13a99c  ldr.w r0,[r10,#28]       ; this->m_nState (r10 = this)
      13a9a0  cmp   r0,#2
      13a9a4  ldr   r0,[sp,#76]        ; delta
      13a9a8  lsls  r0,r0,#1
```

The rest of the case-105 body is instruction-for-instruction the same apart
from this register swap. The same is true of case 131 state 1: from
`0x138abc` (orig) / `0x138a94` (ours) the two bodies are identical (same
r4/r5/r6/r8/r9/r11), differing only in the entry reload
(`ldr r5,[sp,#88]` vs `ldr.w r9,[sp,#88]`).

## Per-case width census (0x300-byte window at each case entry)

`d16 = orig16 - ours16` (positive = original uses more narrow encodings):

```
case  14: +22    case  40: +28    case  92: +58    case 125:  +6
case  16: +44    case  41: +51    case  94: +20    case 154: +20
case  21: -22    case  42: -10    case 102:+11     case 157: -34
case  24: +35    case  56: +20    case 105:+68
case  29: +48    case  80: +28    case 114:+56
case  91: +21
```

The largest deficits are cases 105 (+68), 92 (+58), 114 (+56), 41 (+51),
29 (+48), 16 (+44), 24 (+35). Cases 157 (-34), 21 (-22) and 42 (-10) go the
other way (ours is narrower there), so a global fix must not simply widen
everything.

## Experiments (all built and scored on pristine master; each reverted)

| variant | size | pct | 16-bit | note |
|---|---:|---:|---:|---|
| baseline | 57232 | **58.8** | 12798 | |
| prologue init order `emp` | 57308 | 58.9 | 12696 | +76, not size-neutral |
| prologue init order `pem` | 57292 | 58.5 | 12778 | +60 |
| prologue init order `mep/mpe/pme` | 57392 | 58.7 | 12684 | +160 |
| `LevelScript *self = this;` in case 125, all members `self->` | 57232 | 58.8 | 12798 | **byte-identical** (coalesced) |
| case 154 state-2 `float step` hoisted to top of arm | 57232 | 58.8 | 12798 | **byte-identical** |
| case 40 state-2 `unsigned` -> `int` loop counter | 57232 | 58.8 | 12798 | **byte-identical** |
| `int d = delta;` at top of process, `d` in case 105 | 57232 | 58.8 | 12798 | **byte-identical** (coalesced) |
| `int d = delta;` inside case 105, `d` in body | 57232 | 58.8 | 12798 | **byte-identical** |
| `const` on all three cached locals | 57232 | 58.8 | 12798 | **byte-identical** |
| swap case 139 / 142 in source | 57232 | 58.8 | 12798 | **byte-identical** |
| swap case 131 / 135 in source | 57240 | 58.4 | 12800 | +8 |
| swap case 102 state 0 / state 1 arms | 57244 | 51.6 | 12812 | +12, block re-roll |
| case 40 condition `msg && state` instead of `state && msg` | 57244 | 44.5 | 12902 | +12, re-roll |
| `Status *status = Globals::status;` for all post-dispatch uses | 57148 | 44.7 | 12688 | -84, bad re-colour |
| `long long d = delta;` at top, `(int) d` in case 105 | 57104 | 57.2 | 12804 | -128 |

Every local alias / declaration move / signedness / const change compiles
**byte-identically**: LLVM coalesces the SSA copies and the scheduler hoists
the same loads. Only changes that alter the *call order* (prologue
permutations) or the *CFG/block layout* (arm swaps, `else if` -> `&&`) move the
register colouring, and they all change the size.

## Why source edits cannot flip `sl`

The `sl` assignment is a global greedy-allocator decision between two long
live ranges (`this` and `delta`) that both span the whole mission switch.
`this` is used ~2019 times and `delta` ~397 times; ours gives `sl` to `this`,
the original gives it to `delta` and rematerialises `this` from `[sp,#88]` per
case. The choice is made once and is not reachable from any local source
shape tried here (aliases coalesce; declaration moves are hoisted).

## Next leads (new, not yet tried)

1. **Make `delta` live across the dispatch chain in the IR.** The original
   loads `[sp,#76]` at `0x1387ee`; ours loads `[sp,#88]`. A faithful statement
   that *uses* `delta` between the prologue and the dispatch (e.g. a `delta`
   clamp that the original performs) would raise its spill weight. Check the
   original's `0x137d82..0x1387c0` for a `delta` use we are missing — the
   call-count census is not the only gap.
2. **Case 105 (+68) is the single best local target.** Its body is otherwise
   instruction-identical; the deficit is `delta` (spilled in ours, in `sl` in
   the original) plus `enemies` (r6 in ours, r8 in the original). A faithful
   statement that keeps `enemies` in r8 and `delta` in r10 in that arm would
   recover ~30 16-bit encodings without touching the prologue.
3. **Case 92 (+58) and 114 (+56)** have not been statement-audited against the
   objdump in the last three waves; both are large state machines whose bodies
   may still contain missing statements (the w7766 `getPlayer` fixes were the
   last audit).

## Reproduction

```sh
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6      # RATCHET PASS, byte 4506
python3 /dev/shm/w8103/fast.py                          # 57232, pct 58.8, 12798/7909
python3 /dev/shm/w8103/tbh2.py                          # jump-table map (both sides)
python3 /dev/shm/w8103/wcensus2.py                      # width-pair census
```
