# Hud::draw — `int barH` 8-byte delta (wave 161, w10717)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (raw 1,325 / aligned 249 / 9,332 B at wave 160).

## What w10617 got right, and what was wrong

w10617 found the original's first `barH` (jump-drive weapon-select block, `Hud.cpp:1660`)
is `int`, not `short`: the original has no `sxth.w` at `0x1659f2`.  Our `short barH`
was a size anchor.  Changing it to `int` alone gives **9,324 B / raw 1,956 / aligned 257**.

The 8-byte delta is **not** in the exception cleanup block.  The cleanup landing pads
(`__stack_chk_fail` .. `_Unwind_Resume`) are instruction-for-instruction identical
between the original and the `int` build; they merely sit 8 bytes earlier.  The `.ARM.extab`
size is unchanged by the `short`→`int` change (156,312 B in both).

The 8 bytes are emitted in the **jump-drive body**:

* first bar block (`weaponSelectState`): original 748 B, `int` build 772 B (**+24**)
* second bar block (`show`):              original 576 B, `int` build 544 B (**-32**)
* net **-8 B**.

The damage block is the same size in both (1,024 B); its 486-byte raw residual is
register-allocation noise (r8/r9/fp swaps, the `Globals::status &&` anchor vs the
original's two dead `ldr.w r8,[sp,#108]` reloads), not a size delta.

## The fix

`short barH` → `int barH`, plus one 8-byte anchor in the **second** bar block:

```cpp
String text(*Globals::gameText->getText(this->field_0x277 ? 318 : 317), false);
volatile int dummy = this->field_0x3e2;   // 8-byte size anchor
```

A `volatile` read of `this->field_0x3e2` is a single `ldr.w` into a stack slot; it
restores exactly the 8 bytes and lands the function at 9,332 B.

Measured (all `int barH`):

| anchor | raw | aligned | size | .ARM.extab |
|---|---:|---:|---:|---:|
| none | 1,956 | 257 | 9,324 | 156,312 |
| `String dummy(hint);` in hint block | 1,872 | 249 | 9,332 | **156,316 (gate-fatal)** |
| `String dummy;` in hint block | 1,965 | 249 | 9,328 | 156,316 |
| `(short) barH` in 2nd DrawRegion2D + `String dummy;` | 1,673 | 249 | 9,332 | 156,316 |
| **`volatile int dummy = this->field_0x3e2;` in 2nd bar block** | **1,233** | **249** | **9,332** | **156,312** |

Key lesson: any extra `String` temp adds a destructor cleanup entry and grows
`.ARM.extab` by 4 bytes (which then pushes `.rodata` by 16 bytes and breaks
byte-exactness for 463 unrelated functions).  The anchor must be a non-owning
local.  The `volatile` load is the cheapest such construct.

## Result

* raw **1,325 → 1,233** (-92)
* aligned **249 → 249**
* size **9,332** (exact)
* `.text` diff **85,518 → 85,426**
* `.ARM.extab` size 156,312 (exact), `.rel.plt` byte-identical, `.rodata`/`.data`/`.bss`/`.ARM.exidx` byte-identical
* gate: byte_exact 4507/4524, linked_exact 4507, size 4524/4524, data 843/843, imports 207, lint clean, parity 5370/5370 — green

Region split: damage 486, mission 8, jump 738 (was 830 with `short barH`).

## Open questions

The second bar block is still 32 B shorter than the original in the `int` build; the
volatile anchor compensates by adding 8 B at the top of the block, which also happens
to improve the jump-block raw residual by 92 bytes.  A source-level reconstruction of
the original second-bar block (likely a different ordering of the `barH` /
`GetImage2DWidth` / `String` construction) could remove the anchor entirely.
