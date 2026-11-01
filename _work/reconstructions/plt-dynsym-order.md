# The PLT / .dynsym order is the dominant byte_exact blocker

Measured at `e9b51728` (byte_exact 1592 / linked 4494).

## Symptom

`RepairBeam::update` sits at its exact original address (0xa72f8), is size-exact and its
instruction stream is identical, yet it is not byte-exact. Every differing byte is a
`blx <plt>` offset:

```
orig: blx 61eb4 <Level::getEnemies@plt>   ours: blx 61ec0
orig: blx 62034 <Level::getPlayer@plt>    ours: blx 621cc
```

Both binaries have 3674 PLT entries. The `.plt` content is byte-identical but the entry
**order** differs (`MTitle::OnTouchMove` @0x61df4 orig vs @0x61e00 ours; `TransformRelease`
@0x5ebcc vs 0x5ec14; `glBindBuffer` @0x5ed04 vs 0x5ebd8). A `diff` of the two PLT symbol
sequences shows 1354 differing lines.

## Cause

The PLT order follows the `.dynsym` order, which gold builds by inserting each symbol at the
first point it is **defined or referenced**. The call sites are at identical `.text` addresses
in both files, so the difference is not the reference order — it is the definition order,
i.e. TU membership / link order.

`difflib.SequenceMatcher` over the two `.dynsym` name lists (5370 defined symbols) gives 77
matching blocks and these first divergences:

| ours index | orig index | block |
|---|---|---|
| 164 | 242 | the 11 `Java_net_fishlabs_...` JNI functions are 78 slots too early |
| 206 | 223 | the 6 `g_android_*` data symbols are too early |
| 400 | 323 | `AELabelObject`, `GetStringLength`, `Curve*`, `getAppVersion`, `esMatrixMultiply` too late |
| 5134+ | — | zlib/libzip/openssl heavily reordered |

Every one of these shifts all later symbols, and with them every later PLT slot, so a single
early move can flip hundreds of functions to byte-exact.

## Method

```python
# .dynsym (sh_type 11), names via sh_link -> string table
import struct, difflib
# ... parse both files into name lists, then:
difflib.SequenceMatcher(None, orig_names, ours_names, autojunk=False).get_opcodes()
```

PLT symbol sequence:

```
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .plt ORIG | grep -o "<[^>]*@plt>:" | sed 's/[<>:]//g' > /tmp/plt_o.txt
```

Do not reorder `.dynsym`/`.plt` with linker tricks — change the TU link order or the TU split
(see `_work/original_layout/tu_mapping.md`).
