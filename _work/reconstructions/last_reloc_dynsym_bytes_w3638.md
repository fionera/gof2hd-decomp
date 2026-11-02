# Last reloc / dynsym bytes (w3638)

Status: research note, 2026-11-02. Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or from a controlled rebuild of this tree.
Baseline at branch start: **byte_exact 4496 / linked_exact 4496**, `.text 0x1337c8`,
lint `CLEAN`, drift `0` (the task's stated baseline).

## 1. Commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8     # ratchet + lint + verify
cmake --build cmake-build-match --target drift -j8     # layout drift
```

Section-byte comparison (all ELF sections are already size- and base-exact):

```python
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
O = '../_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
def sec(p, n):
    e = Elf32(p)
    s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset'] + s['size']]
for n in ('.dynsym', '.dynstr', '.hash', '.gnu.version', '.rel.dyn', '.rel.plt',
          '.text', '.rodata', '.data', '.data.rel.ro', '.got', '.plt',
          '.ARM.exidx', '.ARM.extab'):
    a, b = sec(O, n), sec(U, n)
    print('%-14s size %7d/%7d diff %7d' %
          (n, len(a), len(b), sum(x != y for x, y in zip(a, b)) + abs(len(a) - len(b))))
```

Object-file relocation order (this is the order gold allocates PLT slots in):

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -r cmake-build-match/CMakeFiles/gof2.dir/src/game/world/LevelScript.cpp.obj
```

## 2. Landed fixes (already committed on this branch)

| commit | change | measured effect |
|---|---|---|
| `3ae8a7a4` | move `FileInterfaceAndroid` / `AEFile` static-member definitions to the top of their TUs (original `.bss` emission order) | gate unchanged 4496/4496; `.dynsym` differing bytes **14 -> 1** |
| `bdb8144c` | mark only the out-of-line `PaintCanvas::DrawMesh` definition `inline` (reproduces the original `STB_WEAK`, `st_info 0x22`) | gate unchanged 4496/4496; `.dynsym` differing bytes **1 -> 0** |

With those two, `.dynsym` is **byte-identical**. Do not mark the header declaration
`inline` (that produces `-Wundefined-inline` in every TU and was reverted).

## 3. Section table after the two fixes (current tree)

| section | size orig/ours | differing bytes |
|---|---:|---:|
| `.dynsym` | 89248 / 89248 | **0** |
| `.dynstr` | 189208 / 189208 | **0** |
| `.hash` | 38716 / 38716 | **0** |
| `.gnu.version` | 11156 / 11156 | **0** |
| `.rel.dyn` | 28744 / 28744 | **0** |
| `.rel.plt` | 29392 / 29392 | **12** |
| `.text` | 1259464 / 1259464 | 117381 |
| `.rodata` | 269159 / 269159 | **0** |
| `.data` | 10604 / 10604 | **0** |
| `.data.rel.ro` | 12264 / 12264 | **0** |
| `.got` | 17164 / 17164 | **0** |
| `.plt` | 44108 / 44108 | **0** |
| `.ARM.exidx` | 28528 / 28528 | 2707 |
| `.ARM.extab` | 156312 / 156304 | 121225 (size -8) |

So the entire relocation/dynamic-symbol metadata block is exact except **12 bytes in
`.rel.plt`**. `.ARM.extab` remains 8 bytes short (the known irreducible delta from the seven
still-mismatched bodies).

## 4. The 12 remaining `.rel.plt` bytes: a 6-entry rotation

`.rel.plt` entries 2831..2836 (readelf `R_ARM_JUMP_SLOT`, same offsets and types in both
files) are the same six symbols in a rotated order:

| slot | original | ours |
|---|---|---|
| 2831 | `StarSystem::switchPlanetForIntro` @ PLT `0x66cb4` | `PlayerEgo::deleteHackingGame` @ `0x66cb4` |
| 2832 | `Level::switchSkyboxForIntro` @ `0x66cc0` | `StarSystem::switchPlanetForIntro` @ `0x66cc0` |
| 2833 | `ParticleSystemManager::reset` @ `0x66ccc` | `Level::switchSkyboxForIntro` @ `0x66ccc` |
| 2834 | `PlayerEgo::startSmokeEmission` @ `0x66cd8` | `ParticleSystemManager::reset` @ `0x66cd8` |
| 2835 | `PlayerStation::setVisible` @ `0x66ce4` | `PlayerEgo::startSmokeEmission` @ `0x66ce4` |
| 2836 | `PlayerEgo::deleteHackingGame` @ `0x66cf0` | `PlayerStation::setVisible` @ `0x66cf0` |

The original order is exactly the source order of the six call sites in
`LevelScript::process`. Ours puts `deleteHackingGame` first and `PlayerStation::setVisible`
last.

## 5. Cause (proven): first-reference order in `LevelScript::process`

Each of the six symbols is referenced **exactly once** in the whole `.text` (verified by
`objdump -dr -j .text | grep <mangled>@plt`); all six references are inside
`LevelScript::process`. Gold allocates a PLT slot at the first relocation it sees, and the
object file's relocations for a function section are in offset order, so the PLT order is the
first-reference order inside `LevelScript::process`.

First-reference addresses / function-relative relocation offsets (`process` is 57232 bytes,
identical size in both binaries; function base `0x137d18`):

| symbol | original addr / offset | ours addr / offset |
|---|---|---|
| `switchPlanetForIntro` | `0x13d1ec` / `0x54d3` | `0x13ded2` / `0x61ba` |
| `switchSkyboxForIntro` | `0x13d1f4` / `0x54db` | `0x13deda` / `0x61c2` |
| `ParticleSystemManager::reset` | `0x13d212` / `0x54f9` | `0x13def8` / `0x61e0` |
| `startSmokeEmission` | `0x13d396` / `0x567d` | `0x13e184` / `0x646c` |
| `PlayerStation::setVisible` | `0x13da1e` / `0x5d05` | `0x13e49e` / `0x6786` |
| `deleteHackingGame` | `0x13ded4` / `0x61bb` | `0x13d15a` / `0x5442` |

The four mission-1 references and `PlayerStation::setVisible` (case 42) are in the same
relative order in both files; only `deleteHackingGame` (case 125, `m_nState >= 4` block) is
misplaced. In the original that block is emitted after the mission-1 and case-42 cold
blocks; in ours it is emitted before them.

This is a **block-placement permutation**, not a source-semantics difference: the two
binaries' `LevelScript::process` bodies are the same size (57232 B) and the state-125
condition code is instruction-for-instruction the same shape (range check
`subs r0,#4; cmp r0,#2; bhi`, then `isDockedToDockingPoint`, `hackingWon`,
`getHackingGameDockIndex`). The cold-region order is swapped:

```
original:  mission-1 case 9/10/11/12  ->  case 42 setVisible  ->  case 125 state 3 / >=4
ours:      case 125 state 3 / >=4     ->  mission-1 case 9/10/11/12  ->  case 42 setVisible
```

## 6. Controlled experiments (all gate counts recorded)

All variants rebuilt with `cmake --build cmake-build-match --target check -j8`; no
`--update-baseline` was ever run.

| variant | byte | linked | `.rel.plt` | `.text` diff | verdict |
|---|---:|---:|---:|---:|---|
| baseline (case 125 before case 92) | 4496 | 4496 | 12 | 117393 | — |
| case 125 moved before case 91 | 4496 | 4496 | 12 | **117381** | strict `.text` improvement, PLT unchanged |
| case 125 moved after case 16 (end of switch) | 4496 | 4496 | 12 | 117381 | same as above; case position does not move the block |
| swap `m_nState == 2` / `== 3` source order | 4496 | 4496 | 12 | 117400 | worse; reverted |
| split `else if (m_nState >= 4 ...)` into a separate `if` | **2831** | 4496 | 12 | +32 B size | **regression**; reverted |
| `else if (m_nState >= 6)` (previous worker) | **4495** | 4496 | 21 | 117288 | **regression**; reverted |

The split-`if` regression is instructive: it changes the CFG enough to grow `.text` by 32
bytes, which shifts every later function and drops `byte_exact` to 2831. The original is a
single `if / else if / else if` chain (`cmp r0,#3; bne` then `subs r0,#4; cmp r0,#2; bhi`),
so the chain shape is already correct.

The `case 125 before case 91` move is gate-neutral and reduces `.text` differing bytes
117393 -> 117381; it is committed with the note.

## 7. Proven vs hypothesis

**Proven**

- `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rodata`, `.data`,
  `.data.rel.ro`, `.got`, `.plt` are byte-identical (0 differing bytes).
- The only remaining reloc/dynsym-metadata difference is the 12-byte `.rel.plt` rotation in
  entries 2831..2836.
- The rotation is exactly the first-reference order of the six symbols inside
  `LevelScript::process`; each symbol has exactly one `.text` reference.
- `LevelScript::process` has identical size (57232 B) in both binaries; the difference is a
  permutation of cold basic blocks (case-125 `m_nState >= 4` block vs the mission-1 / case-42
  cold blocks), not a size or semantics difference.

**Hypothesis / still open**

- *Why* LLVM 7.0.2's block placement puts the case-125 cold chain before the mission-1 cold
  chain in our build but after it in the original is not identified. Moving `case 125` in the
  source (before case 91, after case 16), swapping the `m_nState == 2/3` tests, and splitting
  the chain all leave the six-symbol order unchanged. The remaining lever is the exact
  ordering/shape of the other cold blocks in `LevelScript::process` (the function is still a
  117381-byte `.text` near-miss), so this is a `LevelScript::process` reconstruction task, not
  a linker/flag task.
- No linker trick was attempted or is recommended: the `.plt` content is already
  byte-identical; only the slot assignment order differs, and that is driven by object-file
  relocation order.

## 8. Reproduction

```sh
git checkout wave87-w3638
cmake --preset default
cmake --build cmake-build-match --target check -j8   # byte 4496 linked 4496, RATCHET PASS
cmake --build cmake-build-match --target drift -j8   # DRIFT: 0 field(s)
# section diff: see the python snippet in section 1
```
