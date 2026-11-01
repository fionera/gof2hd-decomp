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

## Update: the no-`*` variant scores +58 byte_exact

Measured with `link_so` + `verify` (not `check`; `ratchet.py` does not treat a hand-edited
`section_order.txt` as stale, so `check` can report the previous `.so`):

| variant | byte_exact | linked_exact | .text | .rodata | .bss |
|---|---:|---:|---|---|---|
| baseline (`*`) | 4115 | 4495 | 0x1337c8 | 0x41b67 | 0x69ac |
| no `*`, explicit 134 `.text*` leftovers | **4173** | 4495 | 0x1337c8 | 0x41b57 (-16) | 0x69b8 (+12) |

So the grouping fix is worth **+58 functions** even before the size delta is fixed. The remaining
work is exactly: recover the 16 missing `.rodata` bytes and the 12 extra `.bss` bytes without
losing the 4173.

### `.rodata` -16 clue

After the mojibake fix the first diff is at 0x31780 (VMA 0x1e9f90), not 0x301eb. It is the
`Generator.cpp` static-table emission order: the original has `kAgentOfferItems`
(200,220,208,213,216,228,229,230,231) at 0x31780, then `kKaamoStationItems` (101,102,103,107,
108,109,114,124,0,0); ours emits `kKaamoStationItems` first and `kAgentOfferItems` at 0x3198c.
This is a TU function/use-order issue, not a linker-group issue.

The 16-byte size delta is around 0x39a9c (DialogueWindow's `.rodata`): `g_dw_briefingCounts`
starts at orig 0x399a0 vs ours 0x39998 (-8), and the original has 16 extra bytes of `0xffffffff`
table data before it. The table immediately before `g_dw_briefingCounts` is
`g_dw_speakerFaceParts`/`g_dw_speakerFaces`-shaped data (`0xffffffff` sentinels, small face-part
indices); ours has a `0x7447756e` ("nGuT") word where the original has `0xffffffff`. Check the
DialogueWindow face-part table sizes and the TU's emission order.

### `.bss` +12 clue

The no-`*` variant makes `.bss` unlisted (index 0) and `.bss.anchor` listed last, which changes
the `.bss` input order and adds 12 bytes of alignment padding. Adding an explicit `.bss` line
before `.bss.anchor` made it worse (+20) and lost 8 byte_exact. The `.bss` order must reproduce
the `*` group's order (the original's), not simply put `.bss` first.
