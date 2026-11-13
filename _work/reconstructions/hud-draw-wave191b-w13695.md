# Hud::draw — wave 191b (w13695): plain-block / anchor-elsewhere sweep (no win)

Worker `w13695`, branch `wave191b-w13695`, base master `9c3e116bc`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13695`.

**No code change landed.** `Hud::draw` stays at the HEAD shape (raw 19 /
aligned 6 / 9,332 B / 3,226 insns). Tree left clean; gate re-run green:
`check` **RATCHET PASS** — byte_exact 4508/4524, linked_exact 4508/4524,
size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0, parity
5370/5370. Section sizes all exact; content deltas unchanged
(`.text` 71,271, `.ARM.extab` 577, `.ARM.exidx` 0, `.rodata` 0).

| target | symbol | VMA | size | raw/aligned | insn |
|---|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9,332 | **19 / 6** | 3226/3226 |

## 0. Residual restated (unchanged)

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

Both blocks are 28 B.  HEAD pays for the 4-byte anchor store by re-using
`r0=300` in the second half (one fewer `movne`); the plain block spends its
4 bytes on the second `movne`.  Net size is therefore identical (9,332).

## 1. New evidence: Ghidra confirms the original source is the *plain* block

Decompiled `_ZN3Hud4drawExxP9PlayerEgobjj` from the checked-in Ghidra
project (`_work/bins/android_2.0.16_libgof2hdaa.so_ghidra/...`, image base
0x10000, function at Ghidra `0x173b90` = ELF `0x163b90`).  The damage block
is exactly the plain source shape, with **no anchor and no store**:

```c
  if ((uVar19 & 1) != 0)  { param_1[0x4ac] = 300; ... }
  if ((int)(uVar19 << 0x1e) < 0) { param_1[0x4b0] = 300; ... }
  if ((uVar19 & 0x24) != 0) { param_1[0x4b8] = 300; ... }
  if ((uVar19 & 0x18) != 0) { param_1[0x4b4] = 300; ... }
```

`uVar19 = param_7[8]` is `hitFlags = ego->field_0x20`.  The decompiler also
shows `Globals::status` cached in `pcVar24` for the gamma block *and* the
mission panel (`pcVar24 = *(&UNK_... + ...)` at the gamma block, re-used at
the mission panel), i.e. the "clean" `Globals::status` shape.  There is **no
`Globals::status` use inside the damage block** — so the original's dead
`ldr.w r8,[sp,#108]` at `0x164176` is a register-allocator artefact, not a
source-level read/store.

Consequence: the original's frame-224 / `#108` spill map is produced by the
rest of the function's IR, not by any damage-block statement.  Our plain
reconstruction (anchor removed) drops to frame 216 / `#96` spill because our
IR differs from the original's somewhere else; the HEAD anchor compensates
for that difference and coincidentally reproduces the original's whole
stack map (HEAD is byte-identical outside the 19-byte block).

## 2. Measured this session

`size`/`frame`/`block` from the single-TU `Hud.cpp` object; `dHEAD` is the
object-level byte diff vs HEAD's object (masks nothing, so it is an upper
bound on the linked raw; HEAD itself is 0).  Linked numbers are
`tools/fn_diff.py` on `libgof2hdaa.so`.

| variant | size | frame | block | dHEAD | linked raw/aligned |
|---|---:|---:|---|---:|---:|
| **HEAD** anchor in `&0x24` | **9332** | 224 | rotated | 0 | **19 / 6** |
| plain (no anchor) | 9328 | 216 | **EXACT** | 7271 | — |
| `{volatile int pad=0;}` after `sideImage` (w13661 idx20) | 9332 | 224* | **EXACT** | 2906 | 2923 / 877 |
| **plain + `(void)*(Status*volatile*)&Globals::status;` after `field_0x4b8 -= dt;`** | **9332** | **224** | **EXACT** | **400** | **385 / 33** |
| same, after `GetImage2DHeight(frontImage)` | 9332 | 224 | EXACT | 412 | — |
| `{Status**volatile pad=&Globals::status;}` between `&0x24`/`&0x18` | 9336 | 224 | EXACT | 7025 | — |
| `{volatile int pad=(int)&Globals::status;}` between `&0x24`/`&0x18` | 9336 | 224 | EXACT | 7025 | — |
| nested `if(h&0x24){anchor; if(h&0x24) store;}` | 9336 | 224 | `ittt`+store | 7023 | — |
| anchor at end of function (last statement) | 9336 | 224 | EXACT | 667 | — |
| `Status *volatile st=Globals::status;` before mission panel, `st->getMission()` | 9332 | 224 | EXACT | 3073 | — |
| `Status *volatile st=Globals::status;` before damage block | 9360 | 224 | EXACT | 7960 | — |
| `Status *st=Globals::status;` before mission panel | 9328 | 216 | EXACT | 7271 | — |
| `{volatile int pad=300;}` inside `&0x24` block | 9332 | 216 | `ittt`+2B store | 3002 | — |
| `{volatile int pad=0;}` inside `&0x24` block | 9332 | 216 | pad splits block | 2891 | — |
| unused `volatile long long/double/int[2]/char[8]` at top | 9328 | 216 | EXACT | 7271 | — |
| `if (Globals::status && field_0x4b8>=1)` (either order) | 9368 | 224 | other | 8020 | — |
| `Status *volatile st` at top + `st->getMission()` | 9248 | 216 | EXACT | 8787 | — |
| `hitFlags` as `unsigned`/`char`/`short`/`long` | 9328 | 216 | EXACT | 7271 | — |
| zero-cost `&Globals::status` uses (`if(&st){}`, `(void)(&st!=0)`, `__builtin_assume`, …) | 9328 | 216 | EXACT | 7271 | — |

\* frame 224 but the pad occupies `sp+216`, shifting every slot by −4
(prologue `str r2,[sp,#108]` vs original `#112`), hence linked raw 2923.

### 2a. The `385` variant in detail

`plain + (void)*(Status * volatile *)&Globals::status;` at the tail of the
`field_0x4b8` block is the *only* construction found that reproduces the
original's exact damage block **and** the exact `0x164176` reload **and**
frame 224.  The volatile read is emitted as a single 2-byte instruction
because `r6 = &Globals::status` is still live from the `0x16436e` reload:

```
16436e ldr   r6,[sp,#108]      ; original, unchanged
...
164388 str.w r0,[sl,#1208]
16438c ldr   r0,[r6,#0]        ; <-- extra 2-byte volatile read
16438e ldr   r0,[pc,#404]      ; original options[17] load, shifted +2
```

The 2 bytes are absorbed by the alignment `nop` at `0x164502` (the nop
disappears), so the size stays 9,332 — but every instruction between
`0x16438c` and `0x164502` shifts by 2, giving raw 385 / aligned 33.  This is
the same floor as w13661's best pad (`{Status**volatile pad=...}` at the
block tail, linked 385/33), reached here with a read instead of a store.

### 2b. Why every anchor-elsewhere shape is 9,336

Moving the anchor store out of the damage block (between `&0x24`/`&0x18`,
after `&0x18`, at the end of the function, …) gives the exact plain block
and the exact `0x164176` reload, but the store itself is 4 bytes and there
is nothing left to absorb it: the function becomes 9,336 B.  The store is
`str.w r8,[sp,#208]` (4 B) because `r8` is a high register; even a
`volatile int` pad is 4 B (`movs r0,#0` + `str r0,[sp,#N]`).  A 2-byte
conditional pad store is possible inside the block
(`ittt ne / strne r0,[sp,#N]`), but then the block is no longer plain and
the layout drops back to frame 216.

## 3. Why no win

The original's block is plain (Ghidra-confirmed) and its `0x164176` reload
is dead.  Our reconstruction needs a live `&Globals::status` use to force the
original's frame-224 / `#108` stack map; every such use costs at least one
instruction.  The only affordable spelling is the 2-byte volatile read,
which is absorbed by the `0x164502` alignment nop but shifts the 0x176-byte
range before it (raw 385).  HEAD's anchor instead spends the plain block's
spare 4 bytes on the store and keeps raw 19.  No spelling found reaches
raw 0 without changing the surrounding byte-identical code.

## 4. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13695
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj
# raw=19 aligned=6 size=9332/9332 insn=3226/3226
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch harness `scratch/w13695/`: `cc.sh` (single-TU compile with the
original flags), `batch.py` (`run()` → size/frame/block), `cmp_head.py`
(object diff vs `v_head.obj`), `dd.py` (block disassembly), `sweep2.py` /
`sweep3.py` (pad/volatile-read position sweeps), `b14.py`…`b32.py`
(construct batches), `decomp.py`/`fns.py`/`list.py` (Ghidra decompile of the
original), `Hud.cpp.base` (pristine HEAD copy), `hud_decomp.txt` (Ghidra
output).
