# GameResource TU convergence (w3624)

Status: research note + landed pass (2026-11-02). Worktree `wt/w3624`, branch
`wave87-w3624`, based on master `3ae0d897`. Every number below is from a controlled
rebuild of this tree against `../_work/bins/android_2.0.16_libgof2hdaa.so`.

## TL;DR

The original `Classes/Game/GameResource.cpp` is **one TU** containing
`BuildResourceList`, `loadPortraits`, `loadLowTexturesAndMaterials` and
`ResourceTexture::ResourceTexture(String const&, float)`. Our tree had split it into
`BuildResourceList.cpp` + `LowResourceTable.cpp` + `ResourceMaterial.cpp`. Merging them
back into `src/engine/render/GameResource.cpp` is gate-neutral on the function counts
(byte_exact 4496, linked_exact 4496) but is a **strict byte improvement** in the link
metadata:

| section | before diff | after diff |
|---|---:|---:|
| `.dynsym` | 62 | **50** |
| `.dynstr` | 345 | **211** |
| `.hash` | 42 | **33** |
| `.rel.plt` | 15 | **12** |
| `.ARM.exidx` | 2735 | **2733** |
| `.text` / `.rodata` / `.data` / `.data.rel.ro` / `.got` / `.plt` | 119547 / 0 / 0 / 0 / 0 / 0 | unchanged |

The merge fixes the three GameResource `.dynsym` slots and the three corresponding
`.rel.plt` entries outright; the only remaining defined-symbol order gap is the known
libcrypto `sha256_block_data_order_neon` member order.

## 1. Baseline (before the pass)

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`.text` size/address `0x1337c8 @ 0x69440`, `.rodata` `0x41b67 @ 0x1c9e10`, lint
`CLEAN`, drift `0`. Section byte diffs vs the original (raw `cmp` over each section):

```
.dynsym  62   .dynstr 345   .hash 42   .gnu.version 0
.rel.dyn  0   .rel.plt 15   .text 119547   .rodata 0
.data 0   .data.rel.ro 0   .ARM.exidx 2735   .ARM.extid -8 (size)
.got 0    .plt 0
```

## 2. Original-TU evidence

`_work/original_layout/original_layout.json`, witness `gof2hd_1.1.19_ios`,
`Classes/Game/GameResource.cpp` (object `GameResource-d6dee39462cc8dc5a2ac265e80401c60.o`):

```
BuildResourceList(AbyssEngine::Engine*)
loadPortraits(AbyssEngine::Engine*)
loadLowTexturesAndMaterials(AbyssEngine::Engine*)
AbyssEngine::ResourceTexture::ResourceTexture(AbyssEngine::String const&, float)
```

The original `.text` order (nm on the stripped original; addresses are real VAs) is
also exactly one-TU shaped:

| address | size | symbol |
|---:|---:|---|
| `0x0e8f54` | `0x29d34` | `BuildResourceList(Engine*)` |
| `0x112c88` | `0x32` | `ResourceTexture::ResourceTexture(char const*, float)` (weak) |
| `0x112cbc` | `0x34` | `ResourceMaterial::ResourceMaterial(uint16,uint16,BlendMode)` (weak) |
| `0x112cf0` | `0x30` | `ResourceMesh::ResourceMesh(char const*, uint16, bool)` (weak) |
| `0x112d20` | `0x2c` | `ResourceMaterial::ResourceMaterial(uint16,BlendMode)` (weak) |
| `0x112d4c` | `0x543a` | `loadPortraits(Engine*)` |
| `0x118188` | `0x354a` | `loadLowTexturesAndMaterials(Engine*)` |
| `0x11b6d2` | `0x3c` | `ResourceTexture::ResourceTexture(String const&, float)` (weak) |

The weak inline constructors are emitted immediately after the first function that uses
them; `ResourceTexture(String const&, float)` is emitted after `loadPortraits` and
`loadLowTexturesAndMaterials`, i.e. at the end of the TU.

The `.rodata` order pins the **source** order. Unique string literals (set difference
against the other two functions' literals, first occurrence in the original file):

| function | unique strings | first..last `.rodata` offset |
|---|---:|---|
| `BuildResourceList` | 1501 | `0x1cb1e9 .. 0x1e3709` |
| `loadLowTexturesAndMaterials` | 93 | `0x1e374e .. 0x1e5010` |
| `loadPortraits` | 156 | `0x1e5082 .. 0x1e5c70` |

So the original source order was **BuildResourceList, loadLowTexturesAndMaterials,
loadPortraits** (the `.text` order is Build, loadPortraits, loadLow because clang emits
the forward-referenced callees in call order: `BuildResourceList` calls `loadPortraits`
then `loadLowTexturesAndMaterials`).

## 3. Controlled A/B

Three trees were built with the same toolchain/link line:

* **A (baseline):** the three split TUs.
* **B (landed):** one `GameResource.cpp` with source order Build, loadLow, loadPortraits.
* **C (control):** one `GameResource.cpp` with source order Build, loadPortraits,
  loadLow.

The merged file is the concatenation of the three old bodies with the includes deduped
and the two forward declarations kept; the committed
`src/engine/render/GameResource.cpp` is variant B.

```
# after replacing the three .cpp files with GameResource.cpp
cmake --build cmake-build-match --target check -j8
```

| variant | byte_exact | linked_exact | `.rodata` diff | `.dynsym` diff | `.dynstr` diff | `.hash` diff | `.rel.plt` diff | `.ARM.exidx` diff |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| A split TUs | 4496 | 4496 | 0 | 62 | 345 | 42 | 15 | 2735 |
| **B merged (Build, Low, Portrait)** | **4496** | **4496** | **0** | **50** | **211** | **33** | **12** | **2733** |
| C merged (Build, Portrait, Low) | 4494 | 4496 | 9000 | 50 | 211 | 33 | 12 | 2733 |

Variant C proves the source order is not free: putting `loadPortraits` before
`loadLowTexturesAndMaterials` moves ~9 KB of portrait strings ahead of the low-resource
strings and drops `byte_exact` by 2 (4496 -> 4494). Variant B keeps `.rodata`
byte-identical and keeps the function counts at the baseline.

### Exact GameResource `.dynsym` slots after the merge

```
index  original / merged (identical)
4065   _Z13loadPortraitsPN11AbyssEngine6EngineE
4066   _Z27loadLowTexturesAndMaterialsPN11AbyssEngine6EngineE
4067   _ZN11AbyssEngine12ResourceMeshC2EPKctb
4069   _ZN11AbyssEngine15ResourceTextureC2EPKcf
4070   _ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf
4071   _ZN11AbyssEngine16ResourceMaterialC2EtNS_9BlendModeE
4073   _ZN11AbyssEngine16ResourceMaterialC2EttNS_9BlendModeE
```

Before the merge the three weak ctors were rotated (`ResourceMaterial` before
`ResourceTexture(String)`), and the same rotation appeared in `.rel.plt` at entries
2527/2529/2535. Both are gone after the merge. The only remaining `.dynsym` mismatch is
the 12-slot libcrypto rotation (`sha256_block_data_order_neon` first vs last in its
archive member group), which is a separate archive-member-order issue.

## 4. Tooling cleanup

`tools/gen_link_order.py` had a special case swapping `LowResourceTable.cpp` before
`ResourceMaterial.cpp` so the split objects' `.rodata` chunks landed in the original
order. With the single `GameResource.cpp` TU that pair no longer exists; the special
case is dead. It was removed and the comment updated. The only remaining special case is
the data-only `GameText.cpp` placement.

`tools/gen_exidx_markers.py`'s `DROP_EMPTY_TEXT` also named the two old objects
(`BuildResourceList.cpp.obj`, `LowResourceTable.cpp.obj`) whose empty-`.text` sentinels
the original lacks. Those names are dead after the merge; the single `GameResource.cpp.obj`
sentinel is the one the original keeps at `0x11b712` (after
`ResourceTexture(String const&, float)`), so the merged object must **not** be added to
the drop set. The dead names were removed. Rebuild after both cleanups: byte 4496 /
linked 4496, same section diffs as variant B, and the `.ARM.exidx` CANTUNWIND entries in
the GameResource region are `0xe8f58` / `0x11b712` in both binaries.

## 5. Proven vs hypothesis

**Proven (binary/witness evidence):**

* The original `GameResource.cpp` TU owns all four functions (source-layout dump).
* The original `.rodata` order is Build, loadLow, loadPortraits (unique-string offsets
  in the original binary).
* Merging the split TUs in that source order is a strict improvement: no gate metric
  regresses, and `.dynsym`/`.dynstr`/`.hash`/`.rel.plt`/`.ARM.exidx` differing bytes all
  fall. The three GameResource `.dynsym` slots and `.rel.plt` entries become exact.
* The opposite source order is measurably wrong (`.rodata` 9000 bytes differ,
  `byte_exact` -2).

**Hypothesis / still open:**

* The original source order is exactly Build, loadLow, loadPortraits. The `.rodata`
  witness is strong but only orders the emitted literals; a different source shape that
  emits the same literal order would be indistinguishable here.
* The remaining `.dynsym`/`.dynstr`/`.hash` delta is entirely the libcrypto
  `sha256_block_data_order_neon` archive-member order. The merge removed the only
  app-side defined-symbol order gap; the libcrypto one needs the archive member pull
  order, not a source TU change.
* `.rel.plt` still has the known 6-entry `PlayerEgo::deleteHackingGame` rotation
  documented in `rel_plt_rel_dyn_ordering_w3616.md`.
