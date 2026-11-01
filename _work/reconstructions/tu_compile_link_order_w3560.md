# Original TU compile/link order reconstruction (w3560)

Status: research note + one gate-neutral source fix (2026-11-01). Worktree `wt/w3560`,
branch `wave87-w3560`, based on master `113737ee`. Every number below was measured
against `../_work/bins/android_2.0.16_libgof2hdaa.so` with this tree's NDK r18b gold
1.12 (`GNU gold (GNU Binutils 2.27.0.20170315) 1.12`).

## TL;DR

* The **object link order is correct**. The order-sensitive output sections are
  byte-identical to the original: `.rodata` (0/269159), `.data` (0/10604),
  `.data.rel.ro` (0/12264), `.got` (0/17164), `.dynamic` (0/328), `.init_array`
  (0/12), `.fini_array` (0/8) and `.plt` (0/44108). `.text` is size- and
  base-exact (`0x1337c8 @ 0x69440`).
* The `.dynsym` app region is exactly the **first-mention order** of global symbol
  names across `crtbegin_so.o` + the 203 `gof2` objects in `link_order.rsp`
  (simulation: 4935/4935 app-region symbols, exact). The remaining **10
  defined-symbol gaps** are *not* object link order: 6 are within-TU first-mention
  order (`RecordHandler::recordStoreWrite`, the three touch globals,
  `ResourceTexture::ResourceTexture(String const&, float)`) and 4 are archive
  member order (libzip `_zip_err_*`, OpenSSL `sha256_block_data_order_neon`).
* The `.plt` symbol order is 3668/3674; the single gap is
  `PlayerEgo::deleteHackingGame` (orig slot 2831, ours 2832), caused by
  `LevelScript::process`'s different call layout, not by link order.
* The **`.bss` internal order** is a second, previously under-used order witness:
  302 exported `.bss` objects, 279 in the same address order. Four localized
  groups differ (`FileInterfaceAndroid`, `AEFile`, `LODManager`, `Level`).
  Reordering the `LODManager` and `Level` definitions to the original order
  (experiment E1) fixes those two groups with **no gate change**
  (byte 4496 / linked 4496) and improves `.dynsym` differing bytes
  1768 -> 1760 and `.bss` same-order 279 -> 289.
* Three TU-membership moves that would fix the remaining app-region `.dynsym`
  gaps were re-tested at the current 4496 baseline and all regress `byte_exact`
  (E2 -519, E3 -11, E4 -1380) because they disturb PLT/GOT/`.rodata` order.
  The current TU split is therefore load-bearing; do not move these definitions
  without re-running the full gate.
* Two baseline facts contradict the task brief's "every ELF section is already
  size- and base-exact": `.ARM.extab` is **8 bytes short** (`0x26290` vs
  `0x26298`) and there is an extra `.missioncase` section (0xc0 bytes at
  `0x20b977`, from `#pragma clang section rodata=".missioncase"` in
  `Status.cpp`), which also makes `.shstrtab` +13 bytes. Neither shifts any
  other section address (`.missioncase` sits in existing padding).

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

`.text` `0x1337c8 @ 0x69440`.

Section fingerprint (stripped ours vs original; `diffbytes` is a literal byte
compare; `.bss` has no file bytes):

| section | orig size | ours size | diffbytes |
|---|---:|---:|---:|
| `.rodata` | 0x41b67 | 0x41b67 | **0** |
| `.data` | 0x296c | 0x296c | **0** |
| `.data.rel.ro` | 0x2fe8 | 0x2fe8 | **0** |
| `.got` | 0x430c | 0x430c | **0** |
| `.dynamic` | 0x148 | 0x148 | **0** |
| `.init_array` | 0xc | 0xc | **0** |
| `.fini_array` | 0x8 | 0x8 | **0** |
| `.plt` | 0xac4c | 0xac4c | **0** |
| `.text` | 0x1337c8 | 0x1337c8 | 119732 |
| `.dynsym` | 0x15ca0 | 0x15ca0 | 1768 |
| `.dynstr` | 0x2e318 | 0x2e318 | 6676 |
| `.hash` | 0x973c | 0x973c | 563 |
| `.gnu.version` | 0x2b94 | 0x2b94 | 52 |
| `.rel.dyn` | 0x7048 | 0x7048 | 68 |
| `.rel.plt` | 0x72d0 | 0x72d0 | 141 |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | 2710 |
| `.ARM.extab` | **0x26298** | **0x26290** | 119153 |
| `.shstrtab` | **0x117** | **0x124** | 256 |

`.ARM.extab` is 8 bytes short. The two shortened LSDA records are
`ndk_iapBoughtConsumable` (`0x945d8`, 368 -> 360) and `RemoveTouches`
(`0x95150`, 88 -> 80); the code bytes of both functions are exact. The extra
`.missioncase` section is the only other section-set difference.

## 2. Model: gold's `.dynsym` order is first-mention order

Gold 1.12's `Symbol_table` is an `Unordered_map<(name_key, version_key), Symbol*>`
whose bucket count is `number_of_input_files() * 1024` (gold `main.cc`), far more
than the 5578 dynamic symbols, so iteration is insertion order. `add_from_relobj`
calls `namepool_.add_with_length()` for every global symbol of every object,
defined or undefined. `Sized_relobj_file::do_add_symbols` walks the object's
global symtab in ELF order; clang 7.0.2 emits that table **sorted
lexicographically by mangled name**. Therefore the `.dynsym` order is the first
object that *mentions* a name, and within one object it is lexicographic.

Simulation (self-contained; reads the `.symtab` global entries in file order):

```python
import sys, os
sys.path.insert(0, '/tmp/w3560')
from symtab import parse          # ELF32 .symtab reader, file order
objs = ['cmake-build-match/crtbegin_so_renamed.o']
objs += [l.strip() for l in open('cmake-build-match/link_order.rsp') if l.strip()]
keys, order = {}, []
for o in objs:
    for s in parse(o):
        n = s['name']
        if n and s['bind'] in (1, 2) and n not in keys:
            keys[n] = len(keys) + 1
            order.append(n)
ours = [l.rstrip('\n').split(' ', 4)[4] for l in open('/tmp/w3560/ours.dynsym')
        if l.rstrip('\n').split(' ', 4)[3] != '0']
sim = [n for n in order if n in set(ours)]
print(len(sim), sim == ours[:len(sim)])   # -> 4935 True
```

Result: the first **4935** defined symbols of our `.dynsym` are exactly the
simulated first-mention order. The remaining 435 are the archive tail
(libc++abi/libgcc/libcrypto/zlib/libzip), whose order is archive-member pull
order, not app-object order.

## 3. Proof that the object link order is correct

The order-sensitive data sections are byte-identical (§1). That is the strongest
witness: `.rodata` string-literal chunks, `.data` initialised data,
`.data.rel.ro` vtables/pointer tables and `.got` slot order all follow input
object order. Two controlled A/B checks:

### E5 — remove the `LowResourceTable.cpp` / `ResourceMaterial.cpp` override

`tools/gen_link_order.py` carries one explicit link-order override (the original
`.rodata.str1.1` witness says `LowResourceTable.cpp` was linked before
`ResourceMaterial.cpp`, even though `loadPortraits` has the lower `.text`
address). Disabling it:

```
verify: byte 4494  linked 4496   (was 4496 / 4496)
```

So the override is worth **+2 byte_exact** and the `.rodata` witness is real.

### E6 — remove the `GameText.cpp` / `Engine.cpp` override

Disabling it is currently a **no-op**: `GameText.cpp.obj` is already at link
position 31 and `Engine.cpp.obj` at 33, so the `fi > si` swap never fires.
Gate unchanged (4496/4496) and no section differs. The override is dead code at
this HEAD; keep it as documentation of the original `.data` order.

## 4. Remaining `.dynsym` order gaps (10)

Defined-symbol `difflib.SequenceMatcher(autojunk=False)`: 5370/5370 symbols,
5360 matched, ratio 0.998138, **10 gaps**:

| # | original symbols | our first mention | diagnosis |
|---|---|---|---|
| 1 | `RecordHandler::recordStoreWrite`, `recordStoreWritePreview` (2) | `NdkAutosave.cpp` (link pos 83) | original first mention is inside `NdkHooks.cpp` (pos 82), before `NdkAutosave.cpp` |
| 2 | `curTouchSize`, `maxTouchSize`, `touches` (3) | `LODManager.cpp` (pos 105) | original first mention is before `ArrayRemove<AEGeometry*>`, i.e. in an object between `NdkHooks.cpp` and `LODManager.cpp` |
| 3 | `ResourceTexture::ResourceTexture(String const&, float)` (1) | `ResourceMaterial.cpp` | original `GameResource` TU emitted it before the `ResourceMaterial` ctors |
| 4 | libzip `_zip_err_str`, `_zip_err_type`, `_zip_nerr_str` (3) | archive member order | `zip_err_str.o` pull order |
| 5 | `sha256_block_data_order_neon` (1) | archive/script symbol order | OpenSSL tail |

The `.plt` symbol order is 3668/3674 with one gap:
`PlayerEgo::deleteHackingGame` is at slot 2831 in the original and 2832 in ours.
It is first referenced inside `LevelScript::process`, whose call layout is still
52.8% matched; this is a source-shape issue, not a link-order issue.

## 5. `.bss` internal order: a second order witness

`.bss` is size- and base-exact, but its internal object order is observable from
the exported `.bss` symbol addresses. 302 exported `.bss` objects; 279 are in the
same address order (ratio 0.950331). Four localized groups differ:

| group | original order (ascending address) | ours |
|---|---|---|
| `FileInterfaceAndroid` | `env, clazz, context, fileCounter, methodFileExist, methodRead, methodWrite, methodCloseRead, methodCloseWrite` | `fileCounter, env, methodWrite, methodCloseWrite, methodRead, methodCloseRead, clazz, context, methodFileExist` |
| `AEFile` | `file, fileInterface, pakFileEntryList, appRoot` | `appRoot, pakFileEntryList, file, fileInterface` |
| `LODManager` | `touches, maxTouchSize, curTouchSize` | `curTouchSize, maxTouchSize, touches` |
| `Level` | `initStreamOutPosition, comingFromAlienWorld, r, g, b, r_min, g_min, b_min, i_r, i_g, i_b, lastMissionFreighterHitpoints, i_a, initStreamOutPositionAfterCutscene, programmedStation, doInstantJump, energyCellsForNextJump` | `r, g, b, initStreamOutPosition, comingFromAlienWorld, ...` (different) |

Mechanism: clang emits a static datum at the **first point of either its
definition or its first odr-use**, whichever comes first.

* `AEFile`/`FileInterfaceAndroid` are static members whose out-of-line
  definitions sit at the end of the `.cpp` but whose first uses are earlier in
  the file. The LLVM IR global order equals the first-use order:
  `fileCounter, env, methodWrite, methodCloseWrite, methodRead, methodCloseRead,
  clazz, context, methodFileExist` for `FileInterfaceAndroid`, and
  `appRoot, pakFileEntryList, file, fileInterface` for `AEFile`. Reordering the
  out-of-line definitions does **not** move them.
* `LODManager` (`touches`, `maxTouchSize`, `curTouchSize` are namespace-scope
  variables) and `Level` (the `r/g/b` definitions were at the top of the file,
  before any use) do follow definition order. Moving the `Level::r/g/b`
  definitions to the bottom lets the first-use order take over.

## 6. Controlled A/B experiments

All builds are the full `check` target; "before" is the unmodified baseline
(byte 4496 / linked 4496). No `--update-baseline` was used.

| # | change | byte | linked | size | dynsym gaps | other observable | verdict |
|---|---|---:|---:|---:|---:|---|---|
| E1 | reorder `LODManager` + `Level` `.bss` definitions to the original order (and `AEFile`/`FileInterfaceAndroid` definitions, which are no-ops) | **4496** | **4496** | 4524/4524 | 10 | `.bss` same-order 279 -> **289**; `.dynsym` diffbytes 1768 -> **1760**; lint CLEAN; drift 0 | **landed** |
| E2 | merge `NdkAutosave.cpp` into `NdkHooks.cpp` (delete `NdkAutosave.cpp`) | 3977 (-519) | 4496 | 4524/4524 | **9** | PLT same-index 3668 -> 3657; `recordStoreWrite@plt` slot 1017 -> 1008 | not landed |
| E3 | move `curTouchSize`/`maxTouchSize`/`touches` to `NdkAutosave.cpp` | 4485 (-11) | 4496 | 4524/4524 | **9** | GOT-slot order changes; `.bss` order unchanged | not landed |
| E4 | move `loadPortraits` to the end of `BuildResourceList.cpp` | 3116 (-1380) | 4496 | 4524/4524 | — | `.rodata` string order changes | not landed |
| E5 | disable `LowResourceTable`/`ResourceMaterial` link-order override | 4494 (-2) | 4496 | 4524/4524 | — | `.rodata` chunks move | not landed |
| E6 | disable `GameText`/`Engine` link-order override | 4496 | 4496 | 4524/4524 | 10 | no-op (GameText already before Engine) | not landed |

E1 is the only change that is safe at this HEAD. It is gate-neutral but a strict
improvement in binary identity: it restores the original `.bss` order for two
TUs and removes 8 differing `.dynsym` bytes. E2/E3/E4 all fix their target
`.dynsym` gap but cost `byte_exact` through PLT/GOT/`.rodata` order, which is
exactly the "TU split is load-bearing" effect.

## 7. Conclusions

1. **Object link order is reconstructed correctly.** The byte-identical
   `.rodata`, `.data`, `.data.rel.ro`, `.got`, `.dynamic`, `.init_array`,
   `.fini_array` and `.plt` sections prove it; the `gen_link_order.py` address
   heuristic plus the two documented overrides is sufficient at this HEAD.
2. **The remaining `.dynsym` gaps are within-TU first-mention order and archive
   member order**, not object link order. The app-region simulation is exact
   (4935/4935).
3. **The TU split is load-bearing.** Moving `ndk_autosave`, the touch globals or
   `loadPortraits` to their original-looking TUs fixes `.dynsym` but regresses
   `byte_exact` by 519 / 11 / 1380 respectively, because the object's section
   order determines PLT/GOT/`.rodata` first-reference order. A future pass that
   wants those `.dynsym` symbols must reproduce the original *within-object
   section order* at the same time, not just the TU membership.
4. **`.bss` order is a useful independent witness** and can be fixed safely where
   it follows definition order (E1). `FileInterfaceAndroid`/`AEFile` need their
   first uses reordered (or an odr-use anchor), which is a code-shape change and
   was not attempted here.
5. The brief's "every ELF section is size- and base-exact" is currently false:
   `.ARM.extab` is -8 and `.missioncase` is an extra section. Both are code/EH
   issues, not link-order issues.

## 8. Reproduction

```bash
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# section fingerprint + dynsym/plt/.bss order (same script used for every table)
python3 /tmp/w3560/fp.py > /tmp/w3560/fp.txt

# .dynsym defined-symbol alignment
python3 - <<'PY'
import difflib, struct
# ... ELF32 SHT_DYNSYM reader (see tools/verify/elf.py); names only, shndx != 0
PY

# .plt symbol sequence
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .plt ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -o '<[^>]*@plt>:' | sed 's/[<>:]//g' > /tmp/plt_o.txt
$OD -d -j .plt cmake-build-match/libgof2hdaa.so        | grep -o '<[^>]*@plt>:' | sed 's/[<>:]//g' > /tmp/plt_u.txt
```

The E1 patch is exactly the four declaration-order changes in
`src/engine/file/AEFile.cpp`, `src/engine/file/FileInterfaceAndroid.cpp`,
`src/engine/render/LODManager.cpp` and `src/game/world/Level.cpp` (see the
commit); only the `LODManager` and `Level` parts change the object, the
`AEFile`/`FileInterfaceAndroid` parts are no-ops because those symbols are
emitted at first use.
