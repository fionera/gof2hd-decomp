# `Layout::Layout` C1/C2 — wave 188 / worker w13212

Target `_ZN6LayoutC1Ev` / `_ZN6LayoutC2Ev` (aliased, one body), original
vaddr `0xd1028`, size **7,072 B**, source `src/game/ui/Layout.cpp`.
Branch `wave188-w13212`, base master `ce16f45f1` (wave-187 harvest; raw
**5,353**).

## Landed: one size-exact raw win

| metric | before | after |
|---|---:|---:|
| raw same-address bytes | **5,353** | **5,352** (-1) |
| aligned (difflib) | 2,681 | 2,713 |
| size | 7,072/7,072 | **7,072/7,072** |
| instructions | 2,521/2,552 | 2,521/2,552 |

Pure statement swap of two independent device-constant stores:

```
this->field_0x8c = ...
this->field_0x90 = ...
this->eventBannerScale = ...
this->field_0x9c = ...
```

became

```
this->field_0x9c = ...
this->field_0x90 = ...
this->eventBannerScale = ...
this->field_0x8c = ...
```

i.e. top-level units 42 and 45 exchanged (0-based, statements only).  The
linked delta matched the compile-only proxy one-for-one (proxy 5,287 ->
5,286; linked 5,353 -> 5,352).  Gate after the change: byte_exact 4507,
linked_exact 4507, size-exact 4524/4524, data 843/843, imports 207, lint
CLEAN, drift 0, parity 5370/5370; `check` PASS.

## Method

The ctor was parsed into **204 top-level units** (statements / complete
if-else chains, including braceless `if`/`else` chains).  Candidates were
scored with a batched compile-only proxy:

* a reduced TU (`prefix.h` with the same 11 includes as `Layout.cpp`, then
  free functions `void fN(Layout* self) { ... }` with `this->` rewritten to
  `self->`), compiled with the exact `compile_commands.json` flags plus
  `-mllvm -enable-machine-outliner=0` (the outliner otherwise rewrites
  cross-function common sequences in a multi-function batch),
* each function section is extracted from the object with a small ELF32
  parser; every 4-byte word carrying a relocation is masked on both sides
  and the remaining bytes are compared with the original body at
  `0xd1028`,
* a single free function reproduces the linked body byte-for-byte
  (`cmp`-verified against the object section), and the batch proxy baseline
  is 5,287 vs linked 5,353 (constant +66).

Throughput ~35 candidates/s with 4 parallel workers (batches of 100
functions per clang invocation).

Searches run from the wave-187 order:

* **2-opt exhaustive** (20,103 valid adjacent/non-adjacent swaps; invalid
  declaration-after-use orders skipped): one win, units 42/45
  (`field_0x8c` / `field_0x9c`), proxy 5,287 -> 5,286.

### Second win (K=4 exhaustive, post-win re-sweep)

After landing the 42/45 swap a fresh K=4 exhaustive sweep (4,551 valid
window permutations) found a larger move: units **97 and 100** exchanged
(`field_0x148` / `field_0x204`).  Proxy 5,286 -> **5,279** (-7); linked raw
**5,352 -> 5,345** (-7), size exact 7,072/7,072, instruction count
unchanged 2,521/2,552, aligned 2,713.  `check` PASS with the same gate
counts.

Further K=5..7 / 3-opt / distance-11..20 passes are recorded below as they
land.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w13212
ulimit -v 12000000
cmake --build cmake-build-match --target link_so -j2
python3 tools/fn_diff.py --ours cmake-build-match/libgof2hdaa.so _ZN6LayoutC1Ev
# raw=5352 aligned=2713 size=7072/7072 insn=2521/2552
```

Scratch `/home/debian/GalaxyOnFire2/scratch/w13212/`: `units.py`
(204-unit parser), `batch_score.py` (batched proxy), `sweep.py`
(parallel W/K/move search), `prefix.h`, `Layout.base.cpp`.
