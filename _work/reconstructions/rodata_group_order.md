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

---

# Finish (w3412, 2026-11-01): replace the bare `*` with `.text*`/`.data*`/`.bss*`

Status: landed. Branch `wave87-w3412`, based on master `3f58cfa2`. Baseline before the change:
**byte_exact 4115, linked_exact 4495, compared 4524, lint CLEAN, drift 0**, every ELF section
size/base exact (`.rodata` 0x41b67 @ 0x1c9e10).

## What was wrong, precisely

Gold's `--section-ordering-file` parser (`layout.cc:find_section_order_index`) returns a nonzero
index only for section names matched by the file; unmatched names keep index 0. Mergeable input
sections (`.rodata.str1.1`, `.rodata.str1.8`, `.rodata.cst*`, ...) are not `Input_section`
entries at all: `Output_section::add_merge_input_section` wraps them in an
`Output_merge_string`/`Output_merge_data` `Output_section_data`, whose `section_order_index` is
always 0. When the ordering file ends with a bare `*`, every plain `.rodata` input matches the
glob and gets the `*` line's nonzero index, so
`Input_section_sort_section_order_index_compare` sorts all plain `.rodata` inputs **after** the
merge groups. That is the 0x1e7570 grouping. Without an ordering file (the original link) every
input has index 0 and gold keeps input order, interleaving merge groups and plain `.rodata`.

## The fix

`tools/gen_section_order.py` now ends with three scoped globs instead of `*`:

```
.text*
.data*
.bss*
.bss.anchor
```

`.rodata*` is deliberately **not** listed, so plain `.rodata` inputs keep index 0 and interleave
with the merge groups in input order. The three globs preserve the previous placement of the
other unlisted inputs:

* `.text*` keeps plain `.text` (libcrypto/libgcc) and the libc++abi/libandroid_support `.text.*`
  leftovers after the listed `.text.*` functions.
* `.data*` keeps the unlisted `.data`/`.data.rel.ro` inputs after the explicitly listed `.data`
  entries.
* `.bss*` keeps the `.bss.*` inputs after gold's `** common` block. The common block is an
  `Output_section_data` with index 0; if `.bss.*` were also index 0 the common block would sort
  last (it is appended after the inputs), changing `.bss` size by +12. With `.bss*` the common
  block stays first and `.bss` stays exactly 0x69ac.

## Measured A/B

All variants are relinks of the same objects with the same gold command line
(`/tmp/relink2.sh OUT -Map=...`, gold 1.12 from NDK r18b), only the ordering file differs.
`byte` / `linked` are the verify counts; `first diff` is the byte offset inside `.rodata`
(VMA = 0x1c9e10 + offset).

| variant | ordering tail | byte | linked | `.rodata` size | `.bss` size | first `.rodata` diff |
|---|---|---:|---:|---:|---:|---|
| baseline | `*` | 4115 | 4495 | 0x41b67 | 0x69ac | 0x1d760 (VMA 0x1e7570) |
| A | `.text*` | **4179** | 4495 | 0x41b57 | 0x69b8 (+12) | 0x301eb (VMA 0x1f9ffb) |
| B (landed) | `.text* .data* .bss*` | **4171** | 4495 | 0x41b57 | **0x69ac** | 0x301eb (VMA 0x1f9ffb) |

Variant A has the higher function count but regresses `.bss` by 12 bytes (common block moves to
the end). Variant B is the strict structural improvement: byte 4115 -> 4171, linked unchanged,
and every section except `.rodata` size is still exact. The landed `check` line:

```
verify: byte 4171  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
  GAIN  byte_exact 4115 -> 4171 (lock with --update-baseline)
RATCHET PASS
```

`lint` is `CLEAN ✓` and `drift` is `0 field(s) across 0 class(es)`.

## What the new `.rodata` order matches

A map-file walk (`gold -Map`) over the 290 plain `.rodata` input sections shows **279 are
byte-identical at their new addresses**; the other 11 differ in content, not placement. The
first divergence is now inside `o075_VertexColorAlphaTextureShader.cpp.obj` at 0x301eb:

```
orig: ... ben\xc3\xb6tigt zu werden)\n}\x00\x00\x00 N11AbyssEngine29...
ours: ... ben\xc3\x83\xc2\xb6tigt zu werden)\n}\x00 N11AbyssEngine29...
```

i.e. the source has a double-encoded UTF-8 `Ã¶` (`c3 83 c2 b6`) where the original has `ö`
(`c3 b6`). This is a source-content bug, not an ordering bug. The other 10 sections are
per-object content/table differences: `Generator.cpp`, `Status.cpp`, `Level.cpp`,
`StarMap.cpp`, `ModStation.cpp`, `ImageFactory.cpp`, `LensFlare.cpp`, `MenuTouchWindow.cpp`,
`AutoPilotList.cpp`, `DialogueWindow.cpp` (e.g. `Generator.cpp` has a 0x24-byte table at
`.rodata+0x20c` that the original places before the rest of that object's `.rodata`).
The remaining `.rodata` size delta (-0x10) is the net of those content differences.

## Proven vs hypothesis

Proven:
* The bare `*` is the cause of the 0x1e7570 grouping; removing it from the `.rodata` inputs moves
  the first divergence to 0x1f9ffb.
* The landed variant is a strict gate improvement (byte 4115 -> 4171, linked unchanged, lint
  clean, drift 0) and keeps every section size/base exact except the `.rodata` content size.
* `.text*`/`.data*`/`.bss*` are each necessary to preserve the previous non-`.rodata` placement;
  in particular `.bss*` is what keeps `.bss` at 0x69ac.

Hypothesis (strongly supported, not fully closed):
* The remaining `.rodata` -0x10 size delta is entirely per-object content, not global ordering.
  279/290 sections matching at their new addresses and the first diff being a string-content bug
  support this, but the 11 sections have not all been fixed yet.
* The original's `.bss` common block is at the end (original `g_android_leaderboard_scores` is at
  0x21d2c0, `g_pVM` at 0x21d318). Variant A reproduces that placement and gains +8 byte_exact
  but costs +12 `.bss` bytes because our app `.bss` is 8 bytes larger before the common block and
  the `.bss.anchor` pad adds 4 more. That is a separate `.bss`-layout pass, not part of this one.

## Reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8          # baseline 4115 / 4495

# A/B without touching the tree: copy the generated ordering file, replace the '*' line
python3 - <<'PY'
lines = open('cmake-build-match/section_order.txt').read().splitlines()
out = []
for l in lines:
    if l == '*':
        out += ['.text*', '.data*', '.bss*']   # A: only '.text*'
    else:
        out.append(l)
open('/tmp/order_globs.txt', 'w').write('\n'.join(out) + '\n')
PY
# relink with the same gold command line, only --section-ordering-file changed
# (the exact command is the libgof2hdaa.so rule in cmake-build-match/build.ninja)
# then verify:
python3 tools/verify/verify.py --build-dir /tmp/verify_globs \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so --ours /tmp/globs_strip.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0

# landed change:
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8
python3 tools/ratchet.py --build-dir cmake-build-match --update-baseline
```

## Note after harvest

Master already contains the `benötigt` mojibake fix (`50c89e36`), so on current master the
first `.rodata` content diff is the next item after 0x1f9ffb, not the double-encoded `ö`.
The `-16 B` `.rodata` size delta is still open (DialogueWindow table region around 0x39a9c).
On current master the first `.rodata` content diff was 0x31780 (VMA 0x1fb590, not the
`0x1e9f90` typo in older notes): `Generator.cpp` emitted `kKaamoStationItems` before
`kAgentOfferItems`, while the original has the reverse. The w3386 follow-up removed the two
duplicate nine-value tables, so the whole `Generator.cpp` block `0x1fb590..0x1fb7d8` is now
byte-identical and the first diff is 0x1fb8c8 (`Hud.cpp`'s `g_Hud_raceBadge`). See
`_work/reconstructions/generator_rodata_order.md`.
