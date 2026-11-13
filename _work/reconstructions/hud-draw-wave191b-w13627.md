# Hud::draw — wave 191b (w13627): damage-block anchor re-probe (no win)

Worker `w13627`, branch `wave191b-w13627`, base master `0dd777cc1`.
Original `/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch `/home/debian/GalaxyOnFire2/scratch/w13627`.

**No code change landed.** `src/` is at HEAD shape; the note is the only delta.
Gate on entry and exit (`check`): byte_exact **4508/4524**, linked_exact
**4508/4524**, size-exact **4524/4524**, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact **843/843**, imports **207**, lint **CLEAN**,
drift 0, parity **5370/5370**, **RATCHET PASS**.

| target | symbol | VMA | size | raw/aligned | insn |
|---|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9,332 | **19 / 6** | 3226/3226 |

Section deltas are unchanged from the wave-191b baseline: `.text` 71,199,
`.ARM.extab` 577, `.ARM.exidx` 0, `.rodata` 0, `.rel.plt` byte-identical.

## 0. The residual, restated

Only the `hitFlags & 0x24` / `& 0x18` damage block differs
(`0x164184-0x16419f`):

```
orig  164184 tst.w  r1,#36         f011 0f24
      164188 itt    ne             bf1c
      16418a movne.w r0,#300       f44f 7096
      16418e strne.w r0,[sl,#1208] f8ca 04b8
      164192 tst.w  r1,#24         f011 0f18
      164196 itt    ne             bf1c
      164198 movne.w r0,#300       f44f 7096
      16419c strne.w r0,[sl,#1204] f8ca 04b4

ours  164184 tst.w  r1,#36         f011 0f24
      164188 mov.w  r0,#300        f44f 7096
      16418c it     ne             bf18
      16418e strne.w r8,[sp,#208]  f8cd 80d0   <- anchor store
      164192 str.w  r0,[sl,#1208]  f8ca 04b8
      164196 tst.w  r1,#24         f011 0f18
      16419a it     ne             bf18
      16419c strne.w r0,[sl,#1204] f8ca 04b4
```

`diff_orig.py` reports exactly **2 groups / 6 instructions** for HEAD: the
`& 0x24` block is a *rotation* of the original, and the `& 0x18` block loses its
`movne` because `r0` is already unconditionally 300 from the rotated
`& 0x24` block. Both encodings are 20 B over the two blocks
(`&24` 14 + `&18` 6 = 20 ours; `&24` 10 + `&18` 10 = 20 original), so the
anchor is **size-free** but byte-wrong.

## 1. Mechanism (what the anchor actually buys)

`[sp,#108]` (`sp+0x6c`) holds **`&Globals::status`**, spilled at `0x164022`
(`str r6,[sp,#0x6c]`, `r6 = &Globals::status` from the gamma receiver setup).
It is live from the gamma call to the mission panel (`ldr r0,[r8]` at
`0x164558`).

* Original: `&Globals::status` lives in `r8`. It is reloaded at `0x164176`
  (dead — clobbered at `0x1641e4` by `Globals::w`) and at `0x164210` (live —
  used at `0x164558`). Because `r8` is occupied, `this` is shuffled into `r8`
  at `0x16431c` (`mov r8,r10`) and **`Globals::h` spills to `sp+0x3c`**
  (`0x164336`/`0x16436a`), which is why the frame is **224** (`sub sp,#0xe0`).
* Anchor-free (plain damage block): clang keeps `&Globals::status` in `fp` and
  `Globals::w` in `r8`; the dead `0x164176` reload is dropped, `Globals::h`
  stays in a register, frame **216** (`sub sp,#0xd8`), size **9,328**, raw
  **7,340**. So `original = anchor-free + 4 B (dead `ldr r8,[sp,#0x6c]`) +
  8 B frame growth`.
* HEAD's `Status **volatile p = &Globals::status;` unbraced in the `& 0x24`
  if forces `&Globals::status` into `r8` (its `strne r8,[sp,#0xd0]` is the
  volatile store), which reproduces the 224-byte frame and the exact
  `ldr.w r8,[sp,#0x6c]` at `0x164176`. But the store occupies the slot the
  original uses for the second `movne.w r0,#300`, so the block rotates.

The whole residual is therefore a `RegAllocGreedy` butterfly: the source has to
keep `&Globals::status` in `r8` **and** keep the `& 0x24` block plain. Any
construct that occupies `r8` at the block also emits an instruction inside the
block.

## 2. Measured matrix (this session, all on HEAD base unless noted)

Harness: `scratch/w13627/sweep3.py <spec.json>` replaces the four-if block
(and optional `extra` string replacements) on `Hud.cpp.base`, rebuilds with
`(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)`,
and prints `tools/fn_diff.py` raw/aligned/size. `diff_orig.py` gives the
per-instruction diff.

### 2a. Anchor placement (pointer anchor `Status **volatile p = &Globals::status;`)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| **HEAD — anchor unbraced in `& 0x24`** | **19** | **6** | **9332** |
| anchor braced in `& 0x24`, store after the if | 19 | 6 | 9332 |
| anchor in `& 0x18` (unbraced) | 7076 | 410 | 9336 |
| anchor in `& 0x18` (braced) | 7078 | 408 | 9336 |
| anchor in `& 0x18`, store before the if | 7080 | 410 | 9336 |
| anchor in `& 0x24` and `& 0x18` | 7090 | 416 | 9336 |
| anchor braced in `& 0x24` + store inside | 7092 | 408 | 9336 |
| anchor in `& 0x1` | 8899 | 1565 | 9456 |
| anchor in `& 0x2` | 8741 | 1901 | 9636 |
| anchor in `& 0x1` and `& 0x2` | 8857 | 1564 | 9460 |
| anchor in `& 0x1` and `& 0x24` | 8901 | 1562 | 9456 |
| anchor in `& 0x2` and `& 0x24` | 8733 | 1889 | 9636 |
| anchor before the block | 8070 | — | 9360 |
| anchor after the block | 7112 | — | 9336 |

The `& 0x18` placement is the interesting one: it gives the **exact**
`ldr.w r8,[sp,#0x6c]` at `0x164176`, the **exact** `& 0x24` block
(`itt ne / movne / strne`) and the exact `& 0x18` `tst`; only the `& 0x18`
block rotates (`it ne / strne(anchor) / mov r1,#300 / str r1`), which is +4 B
because there is no later block to absorb the unconditional `mov`.

### 2b. Braced anchor (anchor store inside the IT block)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| braced `Status **volatile p` + store inside | 7092 | 408 | 9336 |
| braced `(void) *(Status *volatile *)&Globals::status;` + store | 7093 | 410 | 9336 |
| `Status **volatile p;` declared outside, assigned braced | 7134 | 810 | 9336 |
| `volatile int &pad = this->field_0x4b8; pad = 300;` braced | 7340 | 914 | 9328 |
| `*(volatile int *)&this->field_0x4b8 = 300;` braced | 7340 | 914 | 9328 |

The braced form emits `ittt ne / strne r8,[sp,#0xd0] / movne r0,#300 /
strne r0,[sl,#0x4b8]` and a **plain** `& 0x18` block
(`itt ne / movne / strne`) — i.e. it is exactly `original + 4 B` (the anchor
store) and the dead `0x164176` reload is exact. It cannot shed the 4 B because
the `& 0x18` block needs its own `movne` once `r0` is only conditionally 300.

### 2c. Volatile *read* instead of a store

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `(void) *(Status *volatile *)&Globals::status;` unbraced in `& 0x24` | 25 | 8 | 9332 |
| same, braced | 7093 | 410 | 9336 |

The unbraced read is the closest non-HEAD shape: it keeps the 224-byte frame
and the `ldr.w r8,[sp,#0x6c]` (at `0x164174`, 2 B early), but the block becomes
`it ne / ldrne r0,[r8]` and the `& 0x18` `mov` moves into the `& 0x24` tail.

### 2d. Local `Status *` / `Status *volatile` aliases

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `Status *st = Globals::status;` before block, `st->` at panel | 8073 | 1151 | 9340 |
| `Status *volatile st = Globals::status;` before `& 0x24`, `st->` at panel | 1285 | 771 | 9332 |
| `Status *st` gamma-condition alias + anchor | 38 | 11 | 9332 |
| `Status *st` gamma-condition alias, plain | 7784 | 597 | 9340 |

`Status *st` gives the **exact** damage block and a 224-byte frame, but `this`
lands in `r11` instead of `r10` (all `[sl,#…]` stores become `[r11,#…]`) and
the function grows to 9,340. `Status *volatile st` keeps size 9,332 but drops
the frame to 216.

### 2e. `volatile int pad = 300;` (constant volatile store)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `volatile int pad = 300;` unbraced in `& 0x24` | 3043 | 889 | 9332 |
| `volatile int pad = 300;` before `& 0x24` | 3023 | 874 | 9332 |
| `volatile int pad = 0;` before `& 0x24` | 2892 | 875 | 9332 |
| `volatile int pad[2]; pad[0] = 0;` braced in `& 0x24` | 2910 | 890 | 9332 |
| `volatile int pad[2]` addr-of-status braced in `& 0x24` | 7092 | 408 | 9336 |

The `pad = 300` form emits the **exact** `itt ne / movne r0,#300 / strne`
shape but the `strne` targets the pad slot (`[sp,#0xc8]`), the `& 0x4b8`
store becomes unconditional, and the frame stays 216. It does not occupy `r8`,
so it cannot reproduce the 224-byte frame.

### 2f. `hitFlags` / sideImage ordering butterflies

| variant | raw | aligned | size |
|---|---:|---:|---:|
| plain block + `int hitFlags` moved before `sideImage`/`frontImage` | 3082 | 888 | 9332 |
| same + anchor in `& 0x24` | 7217 | 444 | 9336 |
| same + braced anchor | 7359 | 441 | 9340 |
| same + anchor in `& 0x18` | 7358 | 444 | 9340 |
| plain block + `sideImage`/`frontImage` swapped | 7377 | 923 | 9328 |
| plain block + cached `lowShield` bool | 7563 | 929 | 9324 |

Moving `hitFlags` first is the only butterfly found that keeps size 9,332 with
a **plain** damage block; it gets frame 216 plus one extra
`ldr r1,[r7,#8] / str r1,[sp,#0x38]` pair at `0x163bbc`, and `hitFlags` lands
in `r4` instead of `r1`.

### 2g. `Globals::status &&` guards (wave-102 shape, plain block)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| `if (this->field_0x4ac >= 1 && Globals::status)` | 7314 | 585 | 9344 |
| `if (this->field_0x4b0 >= 1 && Globals::status)` | 6972 | 409 | 9336 |
| `if (this->field_0x4b4 >= 1 && Globals::status)` | 6881 | 406 | 9336 |
| `if (this->field_0x4b8 >= 1 && Globals::status)` | 8133 | 846 | 9368 |

The guard forces the `this` split but always adds a real branch and grows the
function.

## 3. Other shapes measured (earlier session, same base)

All 9,332 B / 3,226 insn unless noted.

| variant | raw | aligned |
|---|---:|---:|
| `void *volatile p = &Globals::status;` | 19 | 6 |
| `volatile int pad[2]; pad[0] = (int)(uintptr_t)&Globals::status;` | 19 | 6 |
| `(hitFlags & 0x24) != 0` + anchor | 19 | 6 |
| `int v = 300;` + unbraced anchor | 19 | 6 |
| `(void) *(Status *volatile *)&Globals::status;` after the if | 7,134 | 426 (9,336) |
| `(void) Globals::status;` before the block | 7,386 | 905 (9,328) |
| `Status *st` just before the panel | 7,386 | 903 (9,328) |
| `Status **stp = &Globals::status;` just before the panel | 7,386 | 905 (9,328) |
| `Status *st` at function top, `st->` at panel | 9,253 | 1,708 (9,620) |
| `int w2 = Globals::w >> 1;` before `SetColor` in block 4 | 7,383 | 918 (9,328) |
| `int w2` + `int h2` before `SetColor` in block 4 | 7,616 | 552 (9,308) |
| `unsigned int hitFlags` | 468 | 86 |
| `if ((hitFlags & 0x24) != 0)` + `if ((hitFlags & 0x18) != 0)` | 468 | 86 |
| anchor in `& 0x18` block (unbraced) | 7,125 | 427 (9,336) |
| anchor + separate `if (hitFlags & 0x24)` store | 7,141 | 425 (9,336) |
| anchor + `else` store | 7,139 | 429 (9,336) |
| `(void) *(volatile int*)&this->field_0x4b8;` braced + store | 2,910 | 865 |
| `(void) *(volatile int*)&this->field_0x4b8;` unbraced | 7,335 | 919 (9,328) |
| `*(volatile int*)&this->field_0x4b8 = 300;` in `& 0x24` | 7,340 | 914 (9,328) |
| `(void) (Status *volatile) Globals::status;` braced | 7,340 | 914 (9,328) |
| `switch (hitFlags & 0x24) { case 0x24: … }` plain | 3,043 | 890 |
| `this->field_0x4b8 = (hitFlags & 0x24) ? 300 : this->field_0x4b8;` | 2,918 | 867 |
| `int &t8 = this->field_0x4b8;` plain | 7,340 | 914 (9,328) |
| two separate `if (hitFlags & 0x24)` (anchor if + store if) | 7,092 | 408 (9,336) |
| anchor (unbraced) in the `field_0x4b8 >= 1` block | 6,744 | 386 (9,336) |
| anchor in `field_0x4b0` / `0x4ac` / `0x4b4` blocks | 6,923 / 8,080 / 8,060 | (9,336 / 9,356 / 9,364) |
| `volatile int field_0x4b8` in `Hud.h` (plain block) | 6,823 | 416 (9,336) |
| `volatile int field_0x4b8` in `Hud.h` (with anchor) | 3,323 | 922 |
| `volatile int pad = 0;` in `field_0x4b8 >= 1` block, plain | 3,035 | 894 |
| all 24 permutations of the four damage ifs (plain) | best 7,384 | (9,340) |
| split `4` + `0x20` (plain) | 7,376 | (9,340) |

## 4. Why no win landed

The original block is the canonical plain form

```cpp
    if (hitFlags & 0x24)
        this->field_0x4b8 = 300;
    if (hitFlags & 0x18)
        this->field_0x4b4 = 300;
```

and the dead `ldr.w r8,[sp,#0x6c]` at `0x164176` plus the 224-byte frame are
allocator spill artefacts of `&Globals::status` being kept in `r8`. Every
source shape that keeps `&Globals::status` in `r8` (i.e. that reproduces the
dead reload and the frame) also emits an instruction inside the `& 0x24`
block, which rotates it. Every shape that leaves the block plain drops `r8`
back to `Globals::w`/`Globals::h` and collapses to the 9,328 / frame-216
allocation. HEAD's `Status **volatile p = &Globals::status;` unbraced in the
`& 0x24` if remains the unique 19/6 / 9,332 shape.

## 5. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13627
(ulimit -v 12000000; cmake --build cmake-build-match --target link_so -j2)
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj
# raw=19 aligned=6  size=9332/9332 insn=3226/3226
(ulimit -v 12000000; cmake --build cmake-build-match --target check -j2)
# RATCHET PASS
```

Scratch harness: `scratch/w13627/` — `sweep3.py <spec.json>` (multi-candidate
block sweep), `build1.py` / `build2.py` (single build + dump),
`diff_orig.py` (per-instruction diff vs the original), `orig_hud.dis`
(original disassembly of the whole function),
`ghidra/decomp2.log` (Ghidra decompilation; function at Ghidra `0x173b90`).
