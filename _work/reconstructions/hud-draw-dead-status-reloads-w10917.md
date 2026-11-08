# Hud::draw — dead `Globals::status` reloads (wave 163, w10917)

Target: `_ZN3Hud4drawExxP9PlayerEgobjj` (`src/game/ui/Hud.cpp`, `Hud::draw`),
original **9,332 B**, VMA `0x163b90`.

| metric | before (wave 162) | after |
|---|---:|---:|
| raw same-address diff | **1,227** | **1,015** |
| aligned (instruction-index) diff | **248** | **229** |
| size | 9,332 | 9,332 |
| `.text` diff bytes | 85,042 | 84,830 |
| `.ARM.extab` diff | 725 | 725 |
| `.rel.plt` diff | 0 | 0 |
| `.rodata`/`.data`/`.bss`/`.ARM.exidx` | 0 | 0 |

Gate: `check` green — byte_exact 4507/4524, linked_exact 4507/4524,
size-exact 4524/4524, data_exact 843/843, imports 207, lint CLEAN,
drift 0, parity 5370/5370; all 27 section sizes exact, file size
2,187,328 B, `.rel.plt` byte-identical.

## Landed change

```cpp
     if (this->field_0x4b4 >= 1) {
+        (void) *(Status * volatile *) &Globals::status;
         Globals::Canvas->SetColor(255, 255, 255, (unsigned char) (this->field_0x4b4 * 255 / 300));
         ...
     }
-    if (Globals::status && this->field_0x4b8 >= 1) {
+    if (this->field_0x4b8 >= 1) {
```

The volatile re-read of `&Globals::status` emits

```
16428c  ldr.w r8, [sp, #108]   ; &Globals::status   (reload)
164294  ldr.w r1, [r8]         ; volatile read
```

in the `field_0x4b4` block, and dropping the `&& Globals::status` guard
removes the 8-byte `ldr/cmp/beq` null test at `1642ee` while keeping the
live tail reload at `16436e` (`ldr r6,[sp,#108]` / `mov r8,r6`).  Net
size is exactly 9,332 B.

## Why this shape

The original reloads `&Globals::status` from `[sp,#108]` three times in
the damage block:

| addr | instruction | live? |
|---|---|---|
| `164176` | `ldr.w r8,[sp,#108]` | dead (r8 clobbered at `1641e0` by `Globals::w`) |
| `164210` | `ldr.w r8,[sp,#108]` | dead (r8 clobbered at `16431c`) |
| `16436e` | `ldr r6,[sp,#108]` | live (`mov r8,r6`, used at `164558`) |

and has **no** null test on the `field_0x4b8` guard
(`1642f2 ldr r0,[sl,#1208]; 1642f6 cmp r0,#1; 1642f8 blt`).

The baseline had one reload (`16428c`) plus the 8-byte
`ldr r8,[r8]; cmp r8,#0; beq` test at `1642ee`.

## Measured variants (damage-block sweep)

| variant | raw | aligned | size |
|---|---:|---:|---:|
| baseline `Globals::status && field_0x4b8 >= 1` | 1,227 | 248 | 9,332 |
| `field_0x4b8 >= 1 && Globals::status` (reproduces the two dead reloads) | 6,793 | 502 | 9,336 |
| same, `!= 0` / `!!` / `&& true` / ternary / `__builtin_expect` | 6,793 | 502 | 9,336 |
| same + `__builtin_assume(Globals::status != 0)` | 6,793 | 502 | 9,336 |
| same, `volatile int dummy` in the jump block removed | 6,780 | 519 | 9,328 |
| volatile read in block 3, plain block-4 guard (**landed**) | **1,015** | **229** | **9,332** |
| volatile read at end of block 3 | 1,102 | 234 | 9,332 |
| volatile read before block 4 | 1,102 | 235 | 9,332 |
| volatile read at start of block 2 | 6,964 | 511 | 9,336 |
| volatile read after block-2 guard | 6,964 | 511 | 9,336 |
| `__builtin_prefetch(&Globals::status)` in block 3 | 1,037 | 232 | 9,332 |
| `(Globals::status, true)` comma / `(void)Globals::status` | 7,401 | 636 | 9,272 |

### Region split of the landed build (raw, same-address)

| region | bytes | baseline | landed |
|---|---:|---:|---:|
| prologue `163b90-16412c` | — | 6 | 6 |
| damage `16412c-1643cc` | 672 | 481 | **273** |
| mission `1643cc-165700` | — | 8 | 4 |
| jump `165700-166000` | — | 732 | 732 |
| tail `166000+` | — | 0 | 0 |

## Open lead (next wave)

`field_0x4b8 >= 1 && Globals::status` reproduces the **two dead reloads
at `164176`/`164210` exactly** (the compiler hoists the `&Globals::status`
load and re-materialises it after each clobber).  Its damage block is only
**214** raw bytes (vs 273 landed), but it costs the 8-byte null test at
`1642fa`, so the function is 9,336 B.  Removing the `cmp/beq` while keeping
the `ldr` (or forcing the original's `mov r8,sl` / `mov sl,fp` /
`mov sl,r8` shuffle, which is 4 B longer than ours) would give 9,332 B and
a damage block near 214.  Attempts with a 4-byte filler in the damage
block (`__builtin_prefetch`, volatile int read) did **not** re-sync the
mission block, because the extra 4 B is offset by the shorter register
shuffle.

## Reproduction

```sh
cmake --build cmake-build-match --target link_so -j6
python3 /dev/shm/w10917/fdiff.py _ZN3Hud4drawExxP9PlayerEgobjj
python3 _work/reconstructions/align_diff.py _ZN3Hud4drawExxP9PlayerEgobjj
cmake --build cmake-build-match --target check -j6
```
