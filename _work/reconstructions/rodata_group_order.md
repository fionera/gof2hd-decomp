# `.rodata` output-group ordering — first divergence at 0x1e7570

Status: research note (2026-11-01), coordinator experiment. Baseline: master `e01eba7a`
(byte_exact 4115, every section size/base exact, `.rodata` size 0x41b67).

## The symptom

`.rodata` is size- and base-exact, but its *content order* differs from 0x1e7570 on:

```
orig  0x1d760: "20FileInterfaceAndroid\0" "13FileInterface\0" <CRC32 table> ...
ours  0x1d760: 00..00 3f 80 00 00 ... "data/audio/FMOD_GOF2.fev" ...
```

`0x1d760` is the offset inside `.rodata` (VMA `0x1c9e10 + 0x1d760 = 0x1e7570`). The original
starts with plain `.rodata` (typeinfo names, zlib CRC32 table, shader sources), then interleaves
the mergeable pools later. Ours starts with `.rodata.cst8` / `.rodata.str1.8` (FModSound) and
puts plain `.rodata` at 0x1dc90.

## Root cause: the trailing `*` in `section_order.txt`

`tools/gen_section_order.py` ends the ordering file with:

```
.data
.data.__cxa_terminate_handler
.data.__cxa_unexpected_handler
.data._ZL5cause
*
.bss.anchor
```

Gold gives every section not named in the file the same index as the `*` line. That index is
after every listed `.text.*` section, so all unlisted `.rodata` / `.rodata.*` inputs are sorted
together at the end. Gold then emits the mergeable groups (`.rodata.cst*`, `.str1.1`, `.str1.8`)
before the non-mergeable `.rodata` sections, producing the grouping above. The `*` is needed for
the `.text` leftovers (plain `.text` from libcrypto/libgcc, 133 libc++abi/libandroid_support
`.text.*` sections), so simply deleting it breaks `.text` order.

## Experiment: delete `*`, list the `.text` leftovers explicitly

Take the generated file, remove `*`, and append the 134 unlisted `.text*` names in first-input
order (`.text` first, then the 133 `.text.*` names). Result:

| variant | `.text` size | `.rodata` size | `.rodata` first diff |
|---|---|---|---|
| baseline (`*`) | 0x1337c8 | 0x41b67 | 0x1d760 |
| no `*` + explicit `.text` leftovers | 0x1337c8 | 0x41b57 (-16) | **0x301eb** |

So the `*` line is definitely the cause of the 0x1d760 grouping. With it gone, `.rodata` matches
up to 0x301eb (197,099 bytes in) — the remaining difference is a 16-byte size delta and a small
tail reorder, not the 101,025-byte block.

## What still needs solving

1. `.rodata` is 16 bytes smaller (0x41b57 vs 0x41b67). Find the missing 16 bytes of alignment
   padding (likely a `.str1.1`/`.str1.8` alignment boundary or a 16-byte-aligned table).
2. The remaining tail diff starts at 0x301eb. Dump both tails and identify the section/string
   group that is out of order.
3. Removing `*` also changed `.data.rel.ro` (-8) and `.bss` (+12) in the first explicit-list
   attempt; those need explicit `.data.rel.ro` / `.bss` entries or a different leftover list.
4. The 134-name leftover list must be generated deterministically by `gen_section_order.py`
   (walk the marker inputs in link order, collect `.text*` names not already listed).

## Commands used

```
cp cmake-build-match/section_order.txt /tmp/section_order.bak
# remove '*' and append the 134 unlisted .text* names
cmake --build cmake-build-match --target link_so -j24
# compare .rodata with a small ELF section reader (objcopy is unusable on /tmp here)
```
