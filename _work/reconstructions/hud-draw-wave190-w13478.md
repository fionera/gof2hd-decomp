# Wave 190 (w13478): `Hud::draw` dock-transfer register flip (raw 32 -> 19)

Worker: w13478. Branch `wave190-w13478`, base master `7db176c4d`. Original:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2 `-Oz`, image base 0x10000).
Scratch: `/home/debian/GalaxyOnFire2/scratch/w13478` (not tracked).

## 0. Result

| target | symbol | VMA | size | before raw/aligned | after raw/aligned |
|---|---|---|---:|---:|---:|
| `Hud::draw` | `_ZN3Hud4drawExxP9PlayerEgobjj` | `0x163b90` | 9,332 | 32 / 21 | **19 / 6** |

Gate after the change: `check` **RATCHET PASS** - byte_exact 4507/4524,
linked_exact 4507/4524, size-exact 4524/4524, stub_zero_size 0, missing 0,
wrong_type 0, extra 0, data_exact 843/843, imports 207, lint CLEAN, drift 0,
symbol parity 5370/5370.

Section deltas: `.text` differing bytes **71,956 -> 71,943** (-13);
`.ARM.extab` content diff unchanged at **578** (size/base exact 156,312 B @
`0x1a3b78`); `.note.gnu.build-id` 20. Total (text+extab+build-id)
**72,554 -> 72,541**. `.rel.plt` byte-identical. `.rodata` content exact
over the original 269,159 B (`sh_size` still 269,644, +0x1e5). File size
exact (2,187,328 B), 27/27 section headers.

## 1. The landed change

In the jump-drive / docking-transfer progress-bar block
(`src/game/ui/Hud.cpp`, ~line 1664):

```cpp
            float fadeIn = (float) this->field_0x468 / 1000.0f;
            float fade = 1.0f;
            if (fadeIn < 1.0f)
                fade = fadeIn;
```

becomes

```cpp
            float fadeIn = (float) this->field_0x468 / 1000.0f;
            float fade = fadeIn < 1.0f ? fadeIn : 1.0f;
```

Semantically identical (`fade` is the clamped fade-in).  The emitted
instructions are the same `vcmpe.f32 / vmrs / it mi / vmovmi.f32` sequence;
what changes is the *IR shape* feeding register allocation.

### Why it wins

The remaining 13-byte dock-transfer residual was a pure
`RegAllocGreedy` tie-break: the shipped code keeps the
`&Globals::Canvas` GOT-address copy in `r5` and the `top = field_0x3e2`
load in `r6`; ours had them swapped (`copy=r6`, `top=r5`).  Both live
ranges have one def and two uses in the same hot block, so their spill
weights tie and the allocation order decides the pair.

The `if` form materialises the `1.0f` default as a separate
`float fade = 1.0f` store/`vmov` before the compare; the `?:` form keeps
only the compare result.  That changes the vreg numbering / live-range
set the allocator sees and flips the tie: the `&Globals::Canvas` copy is
now allocated first and takes `r5`, `top` takes `r6`, and every one of
the 13 one-byte register-operand differences in
`0x16594c-0x165a4a` disappears.  The function size is unchanged
(9,332 B, 3,226 instructions) because the instruction sequence is
identical.

## 2. Remaining residual: the damage block (19 bytes)

Everything outside the damage-direction block is now byte-identical.
The only residual is `0x164184-0x16419f`:

```
orig  164184  tst.w  r1, #36
      164188  itt    ne
      16418a  movne.w r0, #300
      16418e  strne.w r0, [sl, #1208]   ; field_0x4b8
      164192  tst.w  r1, #24
      164196  itt    ne
      164198  movne.w r0, #300
      16419c  strne.w r0, [sl, #1204]
ours  164184  tst.w  r1, #36
      164188  mov.w  r0, #300
      16418c  it     ne
      16418e  strne.w r8, [sp, #208]    ; dead volatile address store (anchor)
      164192  str.w  r0, [sl, #1208]    ; unconditional
      164196  tst.w  r1, #24
      16419a  it     ne
      16419c  strne.w r0, [sl, #1204]
```

The unbraced `Status **volatile p = &Globals::status;` anchor inside
`if (hitFlags & 0x24)` is load-bearing for the *whole* function: in the
greedy MIR it becomes its own basic block (`bb.68`, `volatile store 4
into %ir.11`), which moves the `field_0x4b8` store into the join block
`bb.69` where it shares the `300` materialisation with the
`field_0x4b4` store.  That saves the second `movne` (4 B) and pays for
the anchor store (4 B), so the block stays 28 B and the function stays
9,332 B; it also reproduces the shipped stack-frame size (0xe0 first
chunk / 0x2e0 total) and the shipped register allocation everywhere else.
Removing the anchor gives the exact shipped damage block but drops the
frame to 0x2d8 / 9,328 B and changes the allocation of the whole
function (raw 7,339).

Measured this wave (all 9,332 B unless noted), on top of the landed
`?:` change:

| variant | raw | aligned |
|---|---:|---:|
| **`float fade = fadeIn < 1.0f ? fadeIn : 1.0f;` (landed)** | **19** | **6** |
| base (`if (fadeIn < 1.0f) fade = fadeIn;`) | 32 | 21 |
| `if (1.0f > fadeIn)` | 32 | 21 |
| `if (!(fadeIn >= 1.0f))` | 9,320 B | - |
| `bool forward = !this->field_0x279;` | 32 | 21 |
| `progress = forward ? progress : 1.0f - progress;` | 32 | 21 |
| `(short) this->field_0x3e2` for `top` | 34 | 22 |
| `unsigned int barW` (unsigned `/2`; 9,328 B) | 1,447 | 117 |

The `(short)` cast is not a substitute: clang still emits `ldrh` (the
field is `unsigned short`) and adds a `sxth`-free `ldrsh` variant that
costs one byte.

## 3. Reproduction

```sh
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so \
    _ZN3Hud4drawExxP9PlayerEgobjj
# raw=19 aligned=6 size=9332/9332 insn=3226/3226
cmake --build cmake-build-match --target check -j2   # RATCHET PASS
cmake --build cmake-build-match --target diff -j2    # SYMBOL PARITY 5370/5370
```

## 4. Files changed

`src/game/ui/Hud.cpp` (the `fade` clamp above), this note, and the
inline count updates in `docs/VALIDATION.md` / `docs/IDENTICALITY.md`.
