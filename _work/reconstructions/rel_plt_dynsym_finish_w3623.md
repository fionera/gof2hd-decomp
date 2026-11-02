# `.rel.plt` / `.dynsym` finish (w3623)

Status: research note + landed source/build changes (2026-11-02). Worktree `wt/w3623`,
branch `wave87-w3623`, based on current `master` (`3ae0d897`). Every number below is
from a command run against `../_work/bins/android_2.0.16_libgof2hdaa.so` or a
controlled rebuild of this tree. No baseline was updated.

## TL;DR

Starting from the w3616 state (`.rel.dyn` exact, `.rel.plt` 15, `.dynsym` 62,
`.dynstr` 345, `.hash` 42), two changes finish the **order**:

1. **Recreate the original `GameResource.cpp` TU.** `BuildResourceList.cpp`,
   `LowResourceTable.cpp` and `ResourceMaterial.cpp:loadPortraits` are one original
   TU. Merge them into `BuildResourceList.cpp` with `loadLowTexturesAndMaterials`
   *before* `loadPortraits` in source (so `.rodata.str1.1` keeps the original
   low-resource-before-portrait order), delete the two split files, and **keep**
   `BuildResourceList.cpp.obj`'s empty-`.text` marker (remove it from
   `DROP_EMPTY_TEXT`). The marker is the original's `CANTUNWIND` entry at
   `0x11b70e` and its 4-byte alignment is what puts `Node::Node(int)` at the
   original `0x11b710`.
2. **Reorder the libcrypto archive members** to `sha256.o`, `armv4cpuid.o`,
   `sha256-armv4.o` in `third_party/build_match_lib.cmake`. Gold scans archive
   members in order; `sha256.o` first makes `OPENSSL_cleanse` undefined before
   `armv4cpuid.o` is scanned, so the pull order becomes the original
   `sha256.o -> armv4cpuid.o -> sha256-armv4.o` instead of
   `sha256.o -> sha256-armv4.o -> armv4cpuid.o`.

Measured final section diffs vs the original (full `check` gate):

| section | before | after |
|---|---:|---:|
| `.rel.dyn` | 0 | **0** |
| `.rel.plt` | 15 | **12** |
| `.dynsym` | 62 | **14** |
| `.dynstr` | 345 | **0** |
| `.hash` | 42 | **0** |
| `.gnu.version` | 0 | **0** |
| `.text` | 119,547 | 119,547 |
| `.rodata` | 0 | 0 |
| `.data` | 0 | 0 |
| `.data.rel.ro` | 0 | 0 |
| `.ARM.exidx` | 2,735 | 2,733 |
| `.ARM.extab` | 119,204 (size -8) | 121,225 (size -8) |

Gate: `byte 4496  linked 4496  .text 0x1337c8  lint CLEAN  drift 0` before and after.
The `.dynsym` **order is now exact**: all 5,370 defined symbols are at the same
index (5,564/5,578 entries byte-identical). The remaining 14 `.dynsym` bytes are 13
`.bss` `st_value` differences (`FileInterfaceAndroid` / `AEFile` statics) and one
binding difference (`PaintCanvas::DrawMesh` is `STB_WEAK` in the original, `STB_GLOBAL`
in ours). The remaining 12 `.rel.plt` bytes are the six name-order entries for
`PlayerEgo::deleteHackingGame` (one moved symbol, still a `LevelScript::process`
block-layout issue).

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Section diff script (reads both `.so` files directly):

```python
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
O = '../_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
def sec(p, n):
    e = Elf32(p); s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset']+s['size']]
for n in ('.rel.dyn', '.rel.plt', '.dynsym', '.dynstr', '.hash', '.gnu.version',
          '.plt', '.got', '.text', '.rodata', '.data', '.data.rel.ro',
          '.ARM.exidx', '.ARM.extab'):
    a, b = sec(O, n), sec(U, n)
    print('%-14s %d/%d diff %d' % (n, len(a), len(b),
          sum(x != y for x, y in zip(a, b)) + abs(len(a)-len(b))))
```

Baseline: `.rel.dyn 0`, `.rel.plt 15`, `.dynsym 62`, `.dynstr 345`, `.hash 42`,
`.gnu.version 0`, `.text 119547`, `.rodata 0`, `.data 0`, `.data.rel.ro 0`,
`.ARM.exidx 2735`, `.ARM.extab 119204`.

## 2. Experiment 1 — merge the GameResource TU

The original source dump (`_work/original_layout/tu_mapping.md`) says
`Classes/Game/GameResource.cpp` is `BuildResourceList + loadPortraits +
loadLowTexturesAndMaterials + ResourceTexture ctor`. Our tree split it into three
TUs. The split is why `ResourceTexture::ResourceTexture(String const&, float)` was
first mentioned by `ResourceMaterial.cpp` (dynsym index 3881) instead of the
GameResource TU (original 3879).

Change:

* append `loadLowTexturesAndMaterials` and `loadPortraits` to
  `src/engine/render/BuildResourceList.cpp`, in that order;
* delete `src/engine/render/LowResourceTable.cpp` and
  `src/engine/render/ResourceMaterial.cpp`;
* remove `BuildResourceList.cpp.obj` from `DROP_EMPTY_TEXT` in
  `tools/gen_exidx_markers.py` (the marker is the original `0x11b70e` CANT);
* update `tools/gen_link_order.py` (the old LowResourceTable/ResourceMaterial swap
  is now a no-op).

Controlled A/B (full `check`):

| # | change | byte | linked | `.rel.plt` | `.dynsym` | `.dynstr` | `.hash` | `.rodata` | `.data` | `.data.rel.ro` | `.ARM.exidx` |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | HEAD | 4496 | 4496 | 15 | 62 | 345 | 42 | 0 | 0 | 0 | 28528 (2735) |
| A | merge, portraits before low | 4040 | 4496 | 12 | 50 | 211 | 33 | 0 | 0 | 0 | 28520 (missing marker) |
| B | merge, low before portraits | 4040 | 4496 | 12 | 50 | 211 | 33 | 0 | 0 | 0 | 28520 (missing marker) |
| C | B + keep `BuildResourceList` marker | **4496** | **4496** | **12** | **50** | **211** | **33** | **0** | **0** | **0** | **28528 (2733)** |

Experiment A/B without the marker is `byte 4040`: the merged TU's last function
(`ResourceTexture(String)`) ends at `0x11b70e`, the next TU (`SystemPathFinder`)
starts 2-byte aligned, and `.ARM.exidx` is 8 bytes short, which shifts `.rodata`
base by `-0x10` and costs 456 `byte_exact`. Keeping the marker restores the CANT
entry at `0x11b70e`, aligns `Node::Node(int)` to `0x11b710`, and makes
`.ARM.exidx` size-exact again.

`.rodata` is 0 only with `loadLowTexturesAndMaterials` before `loadPortraits` in
source (the original's low-resource strings at `.rodata 0x1993e` precede the
portrait strings at `0x1b255`). The section-ordering file still puts
`loadPortraits` before `loadLowTexturesAndMaterials` in `.text`, matching the
original addresses.

Side effect: `.ARM.extab` differing bytes rise 119,204 -> 121,225 (size stays
`0x26290`, -8). The merged TU's per-function `.ARM.extab` input sections are
emitted in source order (`loadLow` before `loadPortraits`), while the original's
extab order follows its `.text` order (`loadPortraits` before `loadLow`). The
gate does not compare extab bytes; the alternative (portrait before low in source)
costs 9,000 `.rodata` bytes and is worse.

## 3. Experiment 2 — libcrypto archive member order

`libcrypto_gof2.a` was built `armv4cpuid.o, sha256.o, sha256-armv4.o`. Gold scans
archive members in order. Our `.dynsym` showed
`sha256.o -> sha256-armv4.o -> armv4cpuid.o`; the original shows
`sha256.o -> armv4cpuid.o -> sha256-armv4.o`.

Change `third_party/build_match_lib.cmake`:

```diff
-    foreach(_s "${OBJDIR}/armv4cpuid.S" "${SRC}/crypto/sha/sha256.c" "${OBJDIR}/sha256-armv4.S")
+    foreach(_s "${SRC}/crypto/sha/sha256.c" "${OBJDIR}/armv4cpuid.S" "${OBJDIR}/sha256-armv4.S")
```

Why it works: `sha256.o` is pulled first (it defines `SHA224_Init`, referenced by
`RecordHandler`), which makes `OPENSSL_cleanse` undefined. `armv4cpuid.o` is the
next member and is pulled for `OPENSSL_cleanse`; `sha256-armv4.o` is pulled last
for `sha256_block_data_order`. With the old order, `sha256-armv4.o` was scanned
before `armv4cpuid.o`, so it was pulled first and `armv4cpuid.o` needed a second
scan.

Measured (full `check`):

| # | change | byte | linked | `.rel.plt` | `.dynsym` | `.dynstr` | `.hash` | defined same index |
|---|---|---:|---:|---:|---:|---:|---:|---:|
| C | merge only | 4496 | 4496 | 12 | 50 | 211 | 33 | 5358/5370 |
| D | C + libcrypto order | **4496** | **4496** | **12** | **14** | **0** | **0** | **5370/5370** |

The archive is not `--whole-archive`; only the pull order is observable, and the
`.text` order is still reproduced by the section-ordering file. This is a
build-input order change, not a linker trick.

Remaining `.dynsym` 14 bytes (all names/indices exact):

```
13 entries: FileInterfaceAndroid::{env,clazz,context,fileCounter,methodFileExist,
            methodRead,methodWrite,methodCloseRead,methodCloseWrite} and
            AEFile::{file,fileInterface,pakFileEntryList,appRoot}
            st_value differs (.bss layout, not order)
 1 entry:  PaintCanvas::DrawMesh(Mesh*, Matrix&, Matrix&, unsigned, Matrix&)
            st_info 0x22 (STB_WEAK) original vs 0x12 (STB_GLOBAL) ours
```

## 4. Experiment 3 — `deleteHackingGame` (still open)

The last 12 `.rel.plt` bytes are one moved symbol. Current entry comparison:

```
R_ARM_JUMP_SLOT 3674/3674  same offset 3674  same info 3668  same name 3668
  name-order opcodes 2:
    insert 2831 ours ['_ZN9PlayerEgo17deleteHackingGameEv']
    delete 2836 orig ['_ZN9PlayerEgo17deleteHackingGameEv']
```

Original PLT order (slots 2829-2836):

```
2829 StarSystem::switchSunForSupernovaIntro
2830 PlayerEgo::setDockingState
2831 StarSystem::switchPlanetForIntro
2832 Level::switchSkyboxForIntro
2833 ParticleSystemManager::reset
2834 PlayerEgo::startSmokeEmission
2835 PlayerStation::setVisible
2836 PlayerEgo::deleteHackingGame
```

Ours has `deleteHackingGame` at 2831 and shifts the other five down one slot.
It is first referenced at object offset `0x5442` in `LevelScript::process`
(case 125); the mission-0 intro block's first `switchPlanetForIntro` is at
`0x61ba`. In the original the mission-0 intro block is at `0x54d4` and
`deleteHackingGame` is at `0x61bc`, i.e. the two blocks are in the opposite
order. This is a basic-block-layout difference in the 54.6%-matched
`LevelScript::process`, not a TU/link-order difference.

Controlled experiments (all reverted):

| # | change | result |
|---|---|---|
| E1 | move `case 125` after `case 91` in source | PLT order unchanged; object offsets unchanged (`deleteHackingGame 0x5442`, `switchPlanet 0x61ba`). Clang lays switch bodies out by value, not source order. |
| E2 | `-mllvm -likely-branch-weight=80 -mllvm -unlikely-branch-weight=1` for `LevelScript.cpp` | PLT order unchanged. |
| E3 | `-mllvm -disable-block-placement` for `LevelScript.cpp` | Function starts at `0x137d29` (not `0x137d19`), size 57,140; PLT order regresses massively. |
| E4 | `__builtin_expect(mission == 0, 1)` | Function size 57,164; PLT order regresses massively. |
| E5 | never-taken anchor block before `switchSun` calling `switchPlanet, switchSkybox, reset, startSmokeEmission, setVisible` | Fixes the five slots but puts them before `switchSun`; function grows 57232 -> 57280, `.text` +64. |
| E6 | E5 + `setDockingState` in the same block | Correct order, function 57280 (+48), `.text` +64. |

The existing anchors in `LevelScript::process` are load-bearing:

* `if (delta == 0x7d) Standing::setStanding(...)` — 8 bytes; removing it moves
  `Standing` after `setRotationAroundTarget`.
* `if (delta == 0x7c) player->setAutoTurret(player->autoTurretIsEnabled())` —
  24 bytes; removing it moves the auto-turret pair after `PlayerWormHole::open`.
* `if (delta == 0x7b) switchSunForSupernovaIntro()` — 8 bytes; removing it moves
  `switchSun` after `deleteHackingGame`.
* `if (m_nState == 0x7f) player->setRocketControl(...)` — 16 bytes; **not**
  needed for PLT order (the real first `setRocketControl` is at `0x1512`, before
  `Standing` at `0x1e26`), but it is currently a size filler.

A correct fix needs the original `LevelScript::process` block layout (mission-0
intro before the switch case bodies), not another anchor. Not landed.

## 5. Proven vs hypothesis

**Proven (measured above):**

* The original `GameResource.cpp` TU is `BuildResourceList + loadLow +
  loadPortraits + ResourceTexture(String)`; merging them and keeping the TU's
  empty-`.text` marker makes `.rel.dyn` exact, `.dynsym` order exact,
  `.dynstr`/`.hash` exact, and leaves `.rodata`/`.data`/`.data.rel.ro` exact.
* `loadLow` must precede `loadPortraits` in source for `.rodata` 0; the
  section-ordering file independently keeps `.text` in the original order.
* The `BuildResourceList` empty-`.text` marker is the original `CANTUNWIND` at
  `0x11b70e`; without it `.ARM.exidx` is 8 bytes short and `.rodata` shifts by
  `-0x10`, costing 456 `byte_exact`.
* Gold's libcrypto pull order follows archive member order: with
  `sha256.o, armv4cpuid.o, sha256-armv4.o` the `.dynsym` order is exactly the
  original 5,370/5,370 defined symbols.
* The remaining `.rel.plt` 12 bytes are exactly the `deleteHackingGame` slot
  move, caused by the mission-0 intro/case-125 block order in
  `LevelScript::process`.

**Hypothesis / still open:**

* The original `GameResource.cpp` source order is
  `BuildResourceList, loadLow, loadPortraits` (witnessed by `.rodata`); the
  `.text` order `loadPortraits, loadLow` comes from the original's separate
  `.text` ordering. The extab order is the opposite of our merged source order,
  which is why `.ARM.extab` differing bytes rise; the original's extab order
  follows its `.text` order, not its `.rodata` order.
* The 13 `.bss` `st_value` differences are a `.bss` layout issue
  (`FileInterfaceAndroid`/`AEFile` statics), not a dynsym order issue.
* `PaintCanvas::DrawMesh` is weak in the original (likely an inline/weak
  definition); making it weak in our tree was not attempted.
* `deleteHackingGame` can be fixed only by reproducing the original
  `LevelScript::process` basic-block layout or by spending ~48 bytes of
  function size on an anchor (which currently has no free space).

## 6. Reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

Section diff: the Python snippet in §1.

Defined-symbol order:

```python
import sys, difflib
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
def defined(p):
    return [s['name'] for s in Elf32(p)._symbols(SHT_DYNSYM) if s['shndx'] != 0]
o = defined('../_work/bins/android_2.0.16_libgof2hdaa.so')
u = defined('cmake-build-match/libgof2hdaa.so')
print('defined', len(o), len(u), 'same idx', sum(a == b for a, b in zip(o, u)))
sm = difflib.SequenceMatcher(None, o, u, autojunk=False)
for tag, i1, i2, j1, j2 in sm.get_opcodes():
    if tag != 'equal':
        print(tag, 'orig', i1, i2, o[i1:i2], 'ours', j1, j2, u[j1:j2])
```

Relocation entry comparison: the Python snippet in
`_work/reconstructions/rel_plt_rel_dyn_ordering_w3616.md` §5.

Archive member order:

```sh
ar t third_party/openssl/libcrypto_gof2.a
# sha256.o
# armv4cpuid.o
# sha256-armv4.o
```
