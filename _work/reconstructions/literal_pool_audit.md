# Literal-pool audit (wave87 / w3390)

Scope: every function in `report.json` with `pct == 100.0` and `byte == false` — i.e. the
instruction stream and all intra-function branch offsets match, but the raw function bytes do
not. At the start of this pass that was **575 functions** (the task brief said 582; the fresh
`verify` run on this worktree reports 575).

All addresses below are virtual addresses in the original
`android_2.0.16_libgof2hdaa.so` (image base 0x10000, `.text` 0x69440..0x19cc08).

## 1. Method

For every target row, read the function bytes from both `.so` files at the symbol's own
`st_value` and compare every 4-byte aligned word:

```sh
python3 - <<'PY'
import sys, json, struct
sys.path.insert(0, 'tools/verify')
from elf import Elf32
eo = Elf32('../_work/bins/android_2.0.16_libgof2hdaa.so')
eu = Elf32('cmake-build-match/libgof2hdaa.so')
fo, fu = eo.functions(), eu.functions()
for r in json.load(open('report.json'))['rows']:
    if r['pct'] != 100.0 or r['byte']:
        continue
    n = r['name']; o, u = fo[n], fu[n]
    bo = eo.bytes_at(o['value'], o['size'])
    bu = eu.bytes_at(u['value'], u['size'])
    for off in range(0, min(len(bo), len(bu)) - 3, 4):
        wo = struct.unpack_from('<I', bo, off)[0]
        wu = struct.unpack_from('<I', bu, off)[0]
        if wo != wu:
            print(n, hex(o['value'] + off), hex(wo), hex(wu))
PY
```

Raw-word classification (value of the differing word in the original / in ours):

| class (orig, ours) | words | functions |
|---|---:|---:|
| `.text` / `.text` | 1479 | 410 |
| not in any section / not in any section | 1187 | 174 |
| `.ARM.extab` / `.ARM.extab` | 27 | 16 |
| `.plt` / `.plt` | 18 | 1 |
| `.plt` / `.text` | 6 | 4 |
| **total** | **2717** | **575** |

### The raw-word classification is misleading

ARM Android code is PIC. A literal pool word is normally **not** an absolute address, it is a
delta consumed by the standard idiom

```
ldr rX, [pc, #N]      ; pool word = delta
add rX, pc            ; rX = address
```

So a word whose raw value looks like `.text` (e.g. `0x00148622`) is usually a *delta* whose
resolved destination is `.rodata`, `.got`, `.bss`, etc. Classifying by the raw word therefore
over-counts `.text` by an order of magnitude. The coordinator's example
`Level::createCampaignMission` is exactly this: at `0xb4ea8` the words `0x00148622` / `0x001483fa`
are PIC deltas to `.rodata` (destinations `0x1fd45e` / `0x1fd436`), not function pointers.

The audit was therefore re-run resolving the PIC destination (`add rX,pc` address + delta),
including the ARM three-operand form `add rX, pc, rX` that the verifier's `add rX, pc` regex
does not currently recognise.

Corrected classification of the same 575 functions (destination section in original / ours):

| destination class | words | functions | meaning |
|---|---:|---:|---|
| `.rodata` | 863 | 189 | same string/table content, different address (string/table layout) |
| `.got` | 666 | 275 | same GOT symbol, different slot (GOT ordering) |
| `.bss` | 48 | 29 | same global, different address (`.bss` layout) |
| `.data` | 12 | 7 | same global, different address |
| `.data.rel.ro` | 5 | 5 | same vtable/pointer table, different address |
| `.text` | 2 | 2 | PIC delta to `.bss` (SHA224/SHA256 state), mis-read as `.text` |
| instruction words | 1153 | 181 | branch immediates; target resolves to the same symbol (PLT slot / local address) |

**There are no wrong function-pointer literal-pool words in this set.** Every differing
literal-pool word resolves to the same logical datum (same string bytes, same GOT symbol, same
global) on both sides; only its address differs. The real defects are layout/ordering defects,
not wrong callees.

## 2. Ranked causes

1. **GOT layout (666 words / 275 functions).** The GOT symbol *sequence* differed in a handful of
   places. Because GOT slots are 4 bytes, one misplaced symbol shifted every later slot by 4 and
   made hundreds of functions non-byte-exact even though their code was identical. This was the
   single highest-value class and it was fully fixed (see §3).
2. **`.rodata` string/table layout (863 words / 189 functions).** The final `.rodata` is a merge
   of `.rodata.str1.1`/`.rodata.str1.8`/`.rodata.cst*` inputs. Gold merges/sorts those inputs, so
   a single extra or reordered string literal changes the address of many later strings. First
   divergence: `0x1cafa3` — original `...purge\0pause\0FMOD_GOF2\0master...`, ours
   `...purge\0pause\0master...deutsch\0FMOD_GOF2`. This is a whole-section ordering problem, not
   a per-function bug; it is the largest remaining class.
3. **Local `.text` layout (1153 instruction words / 181 functions).** Branch immediates differ
   while the resolved target name is identical. Two sub-causes:
   - PLT slot order (the branch reaches the same imported symbol through a different PLT entry);
   - local (non-exported) code shifted by a few bytes, e.g. `abort_message` at `0x193e64`
     (orig) vs `0x193e40` (ours), which moves the 7 `__cxa_*` callers and the SHA224/SHA256
     wrappers. `.text` section base/size is exact but local function addresses are not.
4. **`.bss` layout (48 words / 29 functions).** `.bss` base is `0x216970` (orig) vs `0x216980`
   (ours) because our link has an extra 8-byte `.aezdata` section before it; 575 of 843 exported
   objects have a different address. This is a link-layout issue.
5. **`.data` / `.data.rel.ro` (17 words / 12 functions).** Same class of global/vtable layout
   drift.

## 3. Fixes landed in this pass

All five fixes are real source changes; none touches the linker flags. Each was measured with a
fresh `link_so` + `verify`.

| # | change | why | byte_exact |
|---|---|---|---:|
| 1 | move free `Engine *GetEngine()` from `src/engine/core/ApplicationManager.cpp` to `src/platform/android/NdkHooks.cpp` | original TU starts at `GetEngine` (`NdkHooks.cpp` header comment already says `.text 0x93c88..0x950dc`). `GetEngine()` is the first `gEngine` reference; leaving it in the early `ApplicationManager.cpp` object created the `gEngine` GOT slot ~139 slots too early and shifted the whole shader GOT block by +4. | 3921 → 4087 (+166) |
| 2 | move `opensubkeyfile()` before `F()` in `src/engine/file/BlowfishSubkeyFile.cpp` | original `.text` order is `opensubkeyfile` (`0x6b2a0`) then `F` (`0x6b2d4`); this is the first-reference order for `SubkeyFile` vs `S` in the GOT | 4087 → 4090 (+3) |
| 3 | move `GlowPPShader::GlowPPShader()` before `SetInActive()` in `src/engine/render/shaders/GlowPPShader.cpp` | constructor references the vtable before `RenderEffect` references `firstRenderGlow`/`switchGlow`; matches original `.text` order (ctor `0x8cd1c` first) | 4090 → 4093 (+3) |
| 4 | move `BoundingAAB::BoundingAAB(...)` to the top of `src/engine/math/BoundingAAB.cpp` | original `.text` order puts the ctor (`0x98104`) before `projectCollisionOnSurface` (`0x98280`), so the vtable GOT slot precedes `AEMath::Absf` | 4093 → 4096 (+3) |
| 5 | move `ndk_resetNativeItemInformationList()` to the top of `src/engine/render/LODManager.cpp` | original `.text` order puts it first (`0x94e38`); its `delete[]` is then the first `_ZdaPv` reference, before the `curTouchSize`/`maxTouchSize`/`touches` GOT slots | 4096 → 4105 (+9) |

After the fixes the **GOT symbol sequence is identical** (0 symbol-address deltas across all 767
GOT slots). `linked_exact` stayed at 4494 throughout; `.text`/`.ARM.exidx`/`.bss` section
placement stayed exact; `drift` reports 0 fields; `lint` is clean.

### Final state

```
byte_exact   4105   (was 3921, +184)
linked_exact 4494
avg_match    99.89
target funcs  391   (was 575)
```

Remaining target classification after the fixes:

| destination class | words | functions |
|---|---:|---:|
| `.rodata` | 863 | 189 |
| `.got` (same slot address; function base shifted) | 72 | 52 |
| `.bss` | 48 | 29 |
| `.data` | 12 | 7 |
| `.data.rel.ro` | 5 | 5 |
| `.text` (PIC to `.bss`) | 2 | 2 |
| instruction words (same target) | 1153 | 181 |

## 4. Reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target verify  -j8
cmake --build cmake-build-match --target check   -j8     # RATCHET PASS
cmake --build cmake-build-match --target drift   -j8     # 0 fields
```

To inspect one function side by side:

```sh
FN=_ZN5Level21createCampaignMissionEv cmake --build cmake-build-match --target verify-fn
```

To re-run the audit, use the script in §1; to resolve PIC destinations, read the word at the
`ldr rX,[pc,#N]` target and add it to the address of the following `add rX,pc` (Thumb: PC = add
address + 4; ARM: PC = add address + 8).

## 5. Notes / next targets

- `.rodata` is the dominant remaining class. It cannot be fixed per function: gold merges and
  sorts the `.rodata.str*` inputs, so the fix is to reproduce the original TU/string set and
  emission order. The first actionable divergence is `FMOD_GOF2` (FModSound) at `0x1cafa3`;
  the next large block is the `LowResourceTable.cpp` texture-path strings (original `0x1993e`,
  ours `0x1a55e`).
- The 72 remaining `.got` words are *not* GOT-order defects any more: the slot address is the
  same on both sides, but the function's base address moved (e.g. `__cxa_guard_*` call a local
  `abort_message` at `0x193e40` instead of `0x193e64`). They need local `.text` layout work.
- `.bss` needs the extra `.aezdata` section (8 bytes, placed before `.bss`) removed or moved, plus
  matching per-object `.bss` sizes/order.
