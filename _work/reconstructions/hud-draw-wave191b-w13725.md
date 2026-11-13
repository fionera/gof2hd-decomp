# Hud::draw — wave 191b (w13725): `&Globals::status` liveness / damage-block sweep (no win)

Worker `w13725`, branch `wave191b-w13725`, base master `ea2ec2eec`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13725`.

**No code change landed.** `Hud::draw` stays at the HEAD shape (raw 19 /
aligned 6 / 9,332 B / 3,226 insns). Tree left clean; gate re-run green:
`check` **RATCHET PASS** — byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370.

| target | symbol | size | raw/aligned | insn |
|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | 9,332 | **19 / 6** | 3226/3226 |

## 0. Residual restated

Only the `hitFlags & 0x24` / `& 0x18` damage block differs
(`0x164184-0x16419f`, 19 bytes).  The shipped block is the *plain* source
(Ghidra `hud_decomp.c` confirms it: four independent
`if ((uVar32 & mask) != 0) *(this + off) = 300;` with no anchor and no store):

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

Both blocks are 28 B.  HEAD's anchor is a **substitution**: the 4-byte
`strne r8,[sp,#208]` takes the slot of the plain block's second
`movne.w r0,#300`, and the shared `mov r0,#300` is hoisted (which also makes
`str.w r0,[sl,#1208]` unconditional).  So HEAD pays *nothing extra* for the
frame-224 layout; it only changes the block bytes.

## 1. New evidence: the original's exact stack-slot map is reachable with the plain block

`spmap.py` (all `[sp,#N]` references in the linked `.so`) shows HEAD's slot map
is byte-for-byte the original's except the single anchor store at `sp+208`:

```
orig  (0,11)(4,12)(8,3)(12,11)(16,3)(20,11)(24,3)(28,1)(32,1)(40,1)
      (60,2)(64,5)(68,10)(72,4)(76,5)(80,3)(84,3)(88,3)(92,10)
      (96,27)(100,27)(104,8)(108,23)(112,16)(116,9)(120,38)(160,2)(220,3)
HEAD  ... identical ..., plus (208,1)   <- anchor store
plain all slots shifted −8 (frame 216)
```

`sp+208` is a `String` temp (`add r0,sp,#208` appears 30× later in the
mission panel); the anchor's alloca is dead-coloured onto it.

**A volatile read of `Globals::status` at the end of the `field_0x4b8`
block reproduces the original's exact slot map with the plain block**:

```cpp
    if (this->field_0x4b8 >= 1) {
        ...
        this->field_0x4b8 -= dt;
        (void)*(Status * volatile *)&Globals::status;   // anchor read
    }
```

MIR (Greedy RA, `allr1.txt`) shows why: the read forces the
`&Globals::status` reload at the *end* of the block:

```
19032B  %2937:gpr = t2LDRi12 %stack.50, 0     ; reload &Globals::status
19076B  %2936:gpr = COPY %2937:gpr
19184B  dead %816:gpr = t2LDRi12 %2936:gpr, 0 ; the volatile read
```

which is exactly the original's `0x16436e ldr r6,[sp,#108]; mov r8,r6`.
Without the read, plain reloads `&Globals::status` much later
(`0x1643e2 ldr.w r8,[sp,#96]`) and the whole slot map shifts by −8.

But the read is a 2-byte `ldr r0,[r6]` at `0x16438c`; the `0x164502`
alignment nop absorbs it, so the size stays 9,332, yet every instruction in
`0x16438c-0x164502` shifts by 2 → **raw 385** (worse than HEAD's 19).

## 2. Measured this session

`size`/`frame`/`block` from the linked `.so` (`h.py`); `BLOCK` = the 28 bytes
at `0x164184` equal the original exactly.

| variant | raw | al | size | fr | block |
|---|---:|---:|---:|---:|---|
| **HEAD** anchor in `&0x24` | **19** | **6** | 9332 | 224 | rotated |
| plain, no anchor | 7340 | 914 | 9328 | 216 | **BLOCK** |
| plain + volatile read after `field_0x4b8 -= dt` | 385 | 33 | 9332 | 224 | **BLOCK** |
| plain + `{Status**volatile p=&Globals::status;}` after `field_0x4b8 -= dt` | 385 | 33 | 9332 | 224 | **BLOCK** |
| plain + read after the `field_0x4b8` `DrawImage2D` | 397 | 33 | 9332 | 224 | **BLOCK** |
| plain + read after the `field_0x4b8` `SetColor` | 6713 | 400 | 9336 | 224 | **BLOCK** |
| plain + `{Status**volatile p=&Globals::status;}` after `&0x18` | 7070 | 407 | 9336 | 224 | **BLOCK** |
| plain + read after `&0x18` | 7070 | 407 | 9336 | 224 | **BLOCK** |
| plain + `__builtin_prefetch(&Globals::status)` after `field_0x4b8 -= dt` | 6618 | 384 | 9336 | 224 | **BLOCK** |
| plain + `Status *volatile st` after damage, `st->` in panel | 1255 | 770 | 9332 | 224 | **BLOCK** |
| plain + `Status *volatile st` before damage, `st->` in panel | 1285 | 771 | 9332 | 224 | block |
| plain + `Status *volatile st` before panel | 3085 | 861 | 9332 | 224 | block |
| plain + `Status **volatile st` before panel, `(*st)->` | 3019 | 862 | 9332 | 224 | block |
| `Status *volatile p = Globals::status;` in `&0x24` block | 898 | 72 | 9332 | 224 | block |
| `Status *volatile const p = Globals::status;` in `&0x24` | 898 | 72 | 9332 | 224 | block |
| `volatile int p = (int)&Globals::status;` in `&0x24` | 19 | 6 | 9332 | 224 | rotated |
| `volatile short/char p = (short/char)(int)&Globals::status;` in `&0x24` | 19 | 6 | 9332 | 224 | rotated |
| plain + read before the hint block (end of function) | 659 | 86 | 9336 | 224 | **BLOCK** |
| plain + read before panel / after last `SetColor` | 7340 | 914 | 9328 | 216 | block |
| plain + `{volatile int p=0;}` after `&0x18` | 2932 | 889 | 9332 | 216 | block |
| plain + `{volatile int p=hitFlags;}` after `&0x18` | 3043 | 886 | 9332 | 216 | block |
| plain + `{volatile int p=sideImage;}` after `&0x18` | 7979 | 706 | 9348 | 224 | block |
| plain + `{volatile int p=300;}` after `&0x18` | 6807 | 899 | 9336 | 216 | block |
| plain + `{Status**volatile p=&Globals::status;}` after `&0x18` + addr-taken | 7070 | 407 | 9336 | 224 | **BLOCK** |
| plain + `{volatile int p[1]; (void)p;}` after `&0x18` | 2798 | 649 | 9332 | 224 | block |
| plain + `hitFlags` first / `unsigned` / `const` | 3082 / 7340 / 7340 | 888 / 914 / 914 | 9332 / 9328 / 9328 | 216 | block |
| plain + `__builtin_expect` on any damage guard | 7340 | 914 | 9328 | 216 | block |
| plain + `int w/h` locals (all damage uses) | 8774 | 1325 | 9128 | 224 | **BLOCK** |

Negative results (all → plain 7340/9328 or worse): `(void)Globals::status`,
`(void)&Globals::status`, `if(status){}`, `__builtin_assume`,
`__builtin_unreachable`, ternary/comma/`&0` tricks, `Status **st` /
`Status *st` / `Status *const st` aliases (non-volatile), `volatile int pad`,
`unsigned/short/long hitFlags`, `!=0`, hex/`u`/`L` literals, braces,
`switch`, `else if`, `(hitFlags&4)||(hitFlags&0x20)`,
`(void)*(volatile char/short/void*)*`, `*(volatile int*)Globals::status`,
`volatile int p=(int)&Globals::status`, `__builtin_prefetch(...,1,0)`,
`volatile Status **p` (needs a cast and is then eliminated),
`volatile Status *p` (eliminated), `int pad[2]={0,0}` / `long long pad=0`
(eliminated), anchor at function top (9252 B, frame 216).

## 3. Why no win

The original is *plain block + frame 224 + the dead `0x164176` reload*.  Our
plain build is 4 B short (9,328) and frame 216.  Every construct that
reproduces the original's exact slot map with the plain block emits at least
one extra instruction:

* a volatile read/store of `&Globals::status` **must** sit at the end of the
  `field_0x4b8` block (earlier positions are dead/merged and give frame 216;
  later positions give frame 216 or 9,336 B) and costs 2 B (`ldr r0,[r6]` /
  `str r6,[sp,#208]`);
* the only 2-byte hole after the damage block is the `0x164502` literal-pool
  alignment `nop`, so the 2 B is absorbed there but shifts
  `0x16438c-0x164502` by 2 → raw 385;
* a read placed *at* `0x164502` (boost block) leaves the damage block at frame
  216, so it cannot pay for the frame.

HEAD's anchor is the only shape that keeps frame 224 at zero net cost, because
its 4-byte store *replaces* the plain block's second `movne`.  There is no
0-byte way to keep `&Globals::status` live across the damage blocks, so the
residual stays at 19.

**Lead for the next worker:** the original's `0x16436e` reload is the
`&Globals::status` reload at the end of the `field_0x4b8` block.  The original
source therefore has a *use of the address* there (or something that makes the
RA hoist it) that emits no instruction.  Finding that source construct — not
another anchor spelling — is the way to raw 0.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13725
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj
# raw=19 aligned=6 size=9332/9332 insn=3226/3226
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch harness `scratch/w13725/`: `h.py` (write variant → build → raw/aligned/
size/frame/block), `sweep.py` (`run()` over the 4-if block), `spmap.py`
(`[sp,#N]` slot map vs original), `idiff.py` (instruction-level diff vs
original), `probe.py`, `cc.sh` / `cc2.sh` (single-TU compile with the original
flags), `gd.py` + `proj.rep` (Ghidra project copy), `hud_decomp.c` (Ghidra
decompile), `allr1.txt` / `allp.txt` (`-print-after-all` of the R1 read
variant and plain), `b1.py`…`b32.py` (the variant batches above).
