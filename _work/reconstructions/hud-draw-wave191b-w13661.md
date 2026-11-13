# Hud::draw — wave 191b (w13661): damage-block pad-position sweep (no win)

Worker `w13661`, branch `wave191b-w13661`, base master `6c02aaeb4`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13661`.

**No code change landed.** `Hud::draw` stays at the HEAD shape (raw 19 /
aligned 6 / 9,332 B / 3,226 insns). Tree left clean; gate re-run green:
`check` **RATCHET PASS** — byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.

| target | symbol | VMA | size | raw/aligned | insn |
|---|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9,332 | **19 / 6** | 3226/3226 |

## 0. Residual restated

Only the `hitFlags & 0x24` / `& 0x18` damage block differs
(`0x164184-0x16419f`, 19 bytes in 2 groups):

```
orig  164184 tst.w  r1,#36         f011 0f24
      164188 itt    ne             bf1c
      16418a movne.w r0,#300       f44f 7096
      16418e strne.w r0,[sl,#1208] f8ca 04b8
      164192 tst.w  r1,#24         f011 0f18
      164196 itt    ne             bf1c
      164198 movne.w r0,#300       f44f 7096
      16419c strne.w r0,[sl,#1204] f8ca 04b4

HEAD  164184 tst.w  r1,#36         f011 0f24
      164188 mov.w  r0,#300        f44f 7096
      16418c it     ne             bf18
      16418e strne.w r8,[sp,#208]  f8cd 80d0   <- anchor store
      164192 str.w  r0,[sl,#1208]  f8ca 04b8
      164196 tst.w  r1,#24         f011 0f18
      16419a it     ne             bf18
      16419c strne.w r0,[sl,#1204] f8ca 04b4
```

The shipped block is the plain source
`if (hitFlags & 0x24) this->field_0x4b8 = 300;` (each store carries its own
`movne`).  HEAD's unbraced anchor
`if (hitFlags & 0x24) Status **volatile p = &Globals::status; this->field_0x4b8 = 300;`
hoists the shared `mov r0,#300` into the join and spends the freed slot on the
anchor's `strne r8,[sp,#208]`.

## 1. New evidence: the anchor store *is* the use of the `0x164176` reload

MIR (`-mllvm -print-machineinstrs`, "After Greedy Register Allocator" section)
of HEAD, `bb.65` (the `hitFlags & 2` block):

```
dead %702 = t2LSLri %25, 30, def $cpsr
%2935 = t2LDRi12 %stack.51, 0          ; <- the 0x164176 ldr.w r8,[sp,#108]
t2Bcc %bb.67, 5, $cpsr
t2B %bb.66
...
bb.68: t2STRi12 %2935, %stack.2, 0     ; <- the anchor store at 0x16418e
```

`%stack.51` is `&Globals::status` (spilled at `0x164022` from `%2926`), and
`%2935` is its reload vreg.  So in HEAD the `0x164176` reload is **live**
(consumed by the anchor store at `0x16418e`); the `0x164210` reload
(`%2935 = load %stack.51`) is dead (redefined before any use).

The original has **two dead** reloads (`0x164176` and `0x164210`, both
clobbered before use) and no anchor store.  So the target is
*plain block + 2 dead reloads + frame 224*, i.e. the original must obtain the
`0x164176` reload from a construct that adds no instruction inside the block.

`odiff2.py` (object-level proxy: byte diff of the `Hud.cpp` object's
`.text._ZN3Hud4draw...` against the original, masking relocation fields)
reproduces HEAD's linked raw exactly (19), so it is a reliable 0.4 s screen.

## 2. Measured this session

`size`/`frame`/`block`/`fs`/`after` from the single-TU `Hud.cpp` object
(`screen2.py`); `proxy` is the object-level raw (`odiff2.py`).  Linked numbers
are `tools/fn_diff.py` on `libgof2hdaa.so`.

| variant | size | frame | block | fs | after `lsls` | proxy raw |
|---|---:|---:|---|---:|---|---:|
| **HEAD** anchor in `&0x24` | **9332** | 224 | rotated | 112 | `ldr.w r8,[sp,#108]` | **19** |
| plain, no anchor | 9328 | 216 | **exact** | 104 | `itt mi` | 5843 |
| plain + `__builtin_expect(hitFlags&0x24,0)` | 9328 | 216 | **exact** | 104 | `itt mi` | 5843 |
| plain + `!=0` / braced / `int b`/`bool b` / `switch` / `(hitFlags&4)\|\|(hitFlags&0x20)` / `(void)0` / `if(1)` | 9328 | 216 | **exact** | 104 | `itt mi` | 5843 |
| plain + `field = (h&0x24)?300:field` | 9332 | 216 | rotated | 104 | `itt mi` | 2375 |
| plain + `(h&0x24)&&Globals::status` (either order, `!=0`, nested) | 9340 | 224 | rotated | 112 | `itt mi` | 5749 |
| anchor in `&0x18` (plain `&0x24`) | 9336 | 224 | `&0x24` exact | 112 | `ldr.w r8,[sp,#108]` | 5617 |
| plain + nested pad after `&0x18` | 9332 | 224 | `&0x24` exact | 112 | `ldr.w r8,[sp,#108]` | 2389 |
| `Status *volatile st = Globals::status;` unbraced in `&0x24` | 9332 | 224 | rotated | 112 | `itt mi` | 759 |
| `if(h&0x24){ (void)*(Status*volatile*)&Globals::status; }` + uncond store | 9332 | 224 | rotated | 112 | `itt mi` | **25** |
| `volatile int p=(int)&Globals::status;` unbraced in `&0x24` | 9332 | 224 | rotated | 112 | `ldr.w r8,[sp,#108]` | 19 |
| `{Status**volatile pad=&Globals::status;}` before `field_0x4b8 -= dt;` | 9332 | 224 | **exact** | 112 | `ldr.w r8,[sp,#108]` | 318 |
| ... same, linked | 9332 | 224 | **exact** | 112 | `ldr.w r8,[sp,#108]` | **385 / 33** |
| ... same at end of hint block (`0x165d4c`) | 9336 | 224 | **exact** | 112 | `ldr.w r8,[sp,#108]` | 500 (linked 660/87) |
| ... same before final `}` | 9336 | 216 | **exact** | 104 | `itt mi` | 5846 |
| `{volatile int pad=0;}` at best position | 9332 | 216 | **exact** | 104 | `itt mi` | 2477 |
| `{volatile int pad=(int)&Globals::status;}` at best position | 9336 | 224 | **exact** | 112 | `ldr.w r8,[sp,#108]` | 5223 |
| `{Status*volatile pad=Globals::status;}` at best position | 9336 | 224 | **exact** | 112 | `ldr.w r8,[sp,#108]` | 5223 |
| pad at end of boost-flash `if` branch (needs braces) | 9340 | 216 | **exact** | 104 | `itt mi` | 5917 |
| pad at end of boost-flash `else` branch | 9336 | 216 | **exact** | 104 | `itt mi` | 5788 |
| declaration order `sideImage/frontImage/hitFlags` = SHF/FHS/HSF/HFS | 9332 | 216 | rotated | 104 | `itt mi` | 2496–2524 |
| `if (field_0x4b8 >= 1 && Globals::status)` guard | 9368 | 224 | rotated | 112 | — | — |

A full pad-position sweep (insert `{ Status **volatile pad = &Globals::status; }`
before every indent-4 statement boundary in `Hud::draw`, 70 candidates) found
only **two** positions that give `size 9332/9336 + frame 224 + both blocks exact
+ first spill 112 + the `0x164176` dead reload`: the `field_0x4b8` block tail
(proxy 318 / linked 385) and the hint-block tail (proxy 500 / linked 660).
Both are worse than HEAD's 19.

## 3. Why no win

The original's `0x164176` reload is dead, so it cannot be the operand of a
store.  Every source construct that keeps `&Globals::status` live across the
damage block (and therefore reproduces the dead reload + frame 224) emits an
instruction:

* a `volatile` store → 2 B (`str r6,[sp,#208]`) or 4 B (`strne.w r8,[sp,#208]`)
  inside the block, or 2 B later in the function;
* a `volatile` read → `it ne / ldrne r0,[r8]` (raw 25, the closest non-HEAD
  shape);
* a null test (`&& Globals::status`) → `ldr/cmp/beq` (+8 B).

The plain block is 9,328 B / frame 216 and drops the `0x164176` reload
entirely.  For a 9,332 B byte-exact result the extra 4 B would have to be the
dead reload itself, which requires the live range without an instruction — no
spelling found does that.  The best pad construction
(`{Status**volatile pad=&Globals::status;}` at the `field_0x4b8` block tail)
achieves the exact block, frame, spill map and both dead reloads, but its
2-byte `str r6,[sp,#208]` at `0x16438c` shifts every following branch/literal
(raw 385); the alignment `nop` at `0x164502` absorbs the size but not the
displacement.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13661
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj
# raw=19 aligned=6 size=9332/9332 insn=3226/3226
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch harness `scratch/w13661/`: `cc.sh` (single-TU compile with the
original flags), `screen2.py` (size/frame/block/first-spill/`after`),
`odiff2.py` (object-level raw proxy, masks relocations; matches linked raw for
HEAD), `bdiff.py` (linked byte diff vs original), `idiff.py`
(instruction-level diff), `linkmeasure.sh`, `sweep_pad.py` (70-position pad
sweep), `b1.py`/`b2.py`/`b3.py` (block-spelling batches), `Hud.cpp.base`
(pristine HEAD copy).
