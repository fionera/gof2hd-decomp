# TU merge verification: KeyFrame → Transform.cpp, Node → SystemPathFinder.cpp

Worker: w15097, branch `wave193-w15097`, base `bac641cc9` (current master).
Topic: verify that the two witness-mandated TU merges are present, correct in the linked
binary, and that re-splitting them regresses the gate. **No source change was needed**; this
note is evidence-only. Baseline before and after (the whole session) is unchanged:
byte_exact **4508/4524**, linked_exact **4508/4524**, size **4524/4524**, data **843/843**,
imports **207**, lint **CLEAN**, drift **0**.

## 1. Authoritative layout witnesses

From `_work/original_layout/tu_mapping.md`:

```
| engine/core/KeyFrame.cpp | removed(empty) | Svn_iPhone_AE2/Classes/Transform.cpp | w3613: KeyFrame ctor is emitted by Transform.cpp; empty split TU removed |
| engine/core/Node.cpp     | split+moved    | Classes/Game/SystemPathFinder.cpp    | Node was a local helper class of SystemPathFinder.cpp |
| engine/math/Transform.cpp        | exact(SD engine) | Svn_iPhone_AE2/Classes/Transform.cpp |
| game/world/SystemPathFinder.cpp  | exact(HD)        | Classes/Game/SystemPathFinder.cpp    |
```

From `_work/original_layout/original_layout_notes.md`:

```
- `SystemPathFinder.cpp` defines helper class **Node**.
  `Transform.cpp` also KeyFrame; `Engine.cpp` also ShaderBaseStruct.
```

Historical landing commits (already on master):

- `cd82cc41a` "Fold Node.cpp into SystemPathFinder.cpp (original TU, tu_mapping witness),
  functions in original .text order; unchanged bodies. RATCHET PASS" — deleted
  `src/engine/core/Node.cpp` (12 lines) and folded its two definitions into
  `src/game/world/SystemPathFinder.cpp`.
- `44b0ad146` "TU convergence: drop eight empty invented TUs (byte 4496/linked 4496)" — removed
  the then-empty `src/engine/core/KeyFrame.cpp` (include + comment only, emitted no symbols)
  along with seven other empty TUs. Commit text records: *"every ELF section is byte-identical
  except .note.gnu.build-id. Gate unchanged: byte 4496 / linked 4496, lint CLEAN, drift 0."*

## 2. Baseline measurement (fresh, before any experiment)

```
cmake --preset default                                  # cmake-build-match was absent
cmake --build cmake-build-match --target check -j4
cmake --build cmake-build-match --target drift -j4
cmake --build cmake-build-match --target lint  -j4
```

Output:

```
verify: byte 4508  linked 4508  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

Direct section comparison (NDK r18b `objcopy -O binary --only-section` + `cmp -l`) confirms the
authoritative residual in `docs/IDENTICALITY.md` §"Update 2026-11-25"; the four size-exact
sections are unchanged and `.ARM.extab` is 4 B short:

```
.text            orig=1259464 ours=1259464  differing_bytes=78772
.ARM.extab       orig=156312  ours=156308   (4 B short; 122,323 differing in the common prefix)
.ARM.exidx       orig=28528   ours=28528    differing_bytes=1307
.rel.plt         orig=29392   ours=29392    differing_bytes=4
.note.gnu.build-id orig=36    ours=36       differing_bytes=20
```

## 3. Evidence that TU ownership is already correct in the linked `.so`

Both binaries are stripped, so compare **dynamic** symbols with `objdump -T`.
Command (run once on the original and once on our output):

```
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OBJDUMP -T ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -iE 'KeyFrame|SystemPathFinder|_ZN4Node'
$OBJDUMP -T cmake-build-match/libgof2hdaa.so               | grep -iE 'KeyFrame|SystemPathFinder|_ZN4Node'
```

The two streams are identical (address, type, size and symbol for every row). Key rows:

| symbol | addr | size | type | orig = ours |
|---|---:|---:|---|---|
| `_ZN11AbyssEngine8KeyFrameC2Ev` (only KeyFrame ctor exported; no C1) | `0x75934` | `0x6c` | `w` | ✔ |
| `_ZN11AbyssEngine9TransformC1Ev` / `C2Ev` | `0x746a0` | `0x1e0` | `g` | ✔ |
| `_ZN11AbyssEngine9Transform18InsertKeyFrame_oldEPKfxi` | `0x7640c` | `0xfec` | `g` | ✔ |
| `_ZN4NodeC1Ei` and `_ZN4NodeC2Ei` (aliases, same addr) | `0x11b710` | `0x2c` | `g` | ✔ |
| `_ZN4NodeD1Ev` and `_ZN4NodeD2Ev` (aliases, same addr) | `0x11b758` | `0x10` | `g` | ✔ |
| `_ZN16SystemPathFinder13constructPathEP4Node` | `0x11ba96` | `0x7a` | `g` | ✔ |
| `_ZN16SystemPathFinder6searchEP4NodeS1_` | `0x11b968` | `0xc8` | `g` | ✔ |
| `_ZN16SystemPathFinder13getSystemPathEP5ArrayIP11SolarSystemEii` | `0x11b7a8` | `0x168` | `g` | ✔ |
| all `Array<KeyFrame*>` / `Array<Node*>` helpers | — | — | `w` | ✔ |

The KeyFrame ctor is **weak** (`w`, `C2` only) — it is emitted where used, exactly as an
inline/header definition from `Transform.cpp` would be. Node's ctor/dtor are **global strong**
(`g`) aliases, i.e. one definition in one TU, matching the original's `SystemPathFinder.cpp`
ownership; external callers can still reach `NodeC1Ei`/`NodeD1Ev`.

## 4. Evidence of per-TU emission (object files)

Stripped `.so` alone does not prove *which* TU emitted a weak symbol, so inspect our objects:

```
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
for o in $(find cmake-build-match/CMakeFiles/gof2.dir -name '*.obj'); do
  $NM "$o" 2>/dev/null | grep -q ' [TW] _ZN11AbyssEngine8KeyFrameC2Ev$' && echo "  $o"
done    # -> only .../src/engine/math/Transform.cpp.obj
for o in $(find cmake-build-match/CMakeFiles/gof2.dir -name '*.obj'); do
  $NM "$o" 2>/dev/null | grep -q ' [TW] _ZN4NodeC1Ei$' && echo "  $o"
done    # -> only .../src/game/world/SystemPathFinder.cpp.obj
```

`nm -C -S --defined-only` on those two objects shows the expected shape:

- `Transform.cpp.obj`: `W AbyssEngine::KeyFrame::KeyFrame()` size `0x6c`, all
  `Array<AbyssEngine::KeyFrame*>` / `Array<AbyssEngine::Transform*>` weak instantiations, and
  every `AbyssEngine::Transform::*` method (C1/C2/D1/D2, `InternUpdate` `0x840`,
  `InsertKeyFrame_old` `0xfec`, …).
- `SystemPathFinder.cpp.obj`: `T Node::Node(int)` `0x2c`, `T Node::~Node()` `0x10`, all
  `Array<Node*>` weak instantiations, and every `SystemPathFinder::*` method.

## 5. Evidence that every relevant function is byte-exact

`cmake-build-match/verify/report.json` (`name/pct/byte/linked/osize/usize`) filtered for
`KeyFrame|Transform|Node|SystemPathFinder`: **every row is `pct 100.0, byte true,
linked true, osize == usize`**. Spot values:

```
_ZN11AbyssEngine8KeyFrameC2Ev                        100.0 True True   108/108
_ZN11AbyssEngine9TransformC1Ev                       100.0 True True   480/480
_ZN11AbyssEngine9Transform12InternUpdateExb          100.0 True True  2112/2112
_ZN11AbyssEngine9Transform18InsertKeyFrame_oldEPKfxi 100.0 True True  4076/4076
_ZN4NodeC1Ei / C2Ei                                  100.0 True True    44/44
_ZN4NodeD1Ev / D2Ev                                  100.0 True True    16/16
_ZN16SystemPathFinder13constructPathEP4Node          100.0 True True   122/122
_ZN16SystemPathFinder6searchEP4NodeS1_              100.0 True True   200/200
_ZN16SystemPathFinder13getSystemPathEP5Array...      100.0 True True   360/360
```

Link order is correct too: `tools/source_order.txt` line 34 lists
`src/engine/math/Transform.cpp`, line 128 lists `src/game/world/SystemPathFinder.cpp`; neither
`src/engine/core/KeyFrame.cpp` nor `src/engine/core/Node.cpp` exists (headers only for KeyFrame /
`Node.h` declares Node). The CMake glob guard (`tools/source_order.txt must list exactly the src/
TUs`) passes.

## 6. Controlled A/B re-split experiments (fresh, from clean baseline)

Each experiment was applied to the tracked tree, `check` run, then reverted with
`git checkout -- <files>` / `rm <new file>` and baseline re-verified (`byte 4508` restored).
Both start from the same clean baseline `4508/4508`.

### A. KeyFrame re-split: give `KeyFrame()` an out-of-line definition in its own TU

Changes: `KeyFrame.h` inline body → `KeyFrame();` declaration; new
`src/engine/core/KeyFrame.cpp` with the identical body (`__builtin_memset(this,0,0x48)`,
scale/localScale = 1s, channels = 0, alpha = 1.0f); add that file to `tools/source_order.txt`
immediately after `Transform.cpp`.

Measured:

| metric | baseline | A: KeyFrame split | delta |
|---|---:|---:|---:|
| byte_exact | 4508 | **1720** | −2788 |
| linked_exact | 4508 | **4505** | −3 |
| size-exact | 4524 | 4524 (no missing/absent/wrong_type) | 0 |
| data_exact | 843 | 843 | 0 |
| imports | 207 | 207 | 0 |
| extra dynsym | 0 | **1** (`_ZN11AbyssEngine8KeyFrameC1Ev`) | +1 |
| lint | CLEAN | **FAIL** (`arm_builtin` in keyframe TU) | fail |

### B. Node re-split: move `Node::Node`/`~Node` back to `src/engine/core/Node.cpp`

Changes: remove the two definitions from `SystemPathFinder.cpp`; new
`src/engine/core/Node.cpp` with the same bodies; add it to `tools/source_order.txt` after
`SystemPathFinder.cpp`.

Measured:

| metric | baseline | B: Node split | delta |
|---|---:|---:|---:|
| byte_exact | 4508 | **4030** | −478 |
| linked_exact | 4508 | **4506** | −2 |
| size-exact | 4524 | 4524 (no missing/absent/wrong_type) | 0 |
| data_exact | 843 | 843 | 0 |
| imports | 207 | 207 | 0 |
| extra dynsym | 0 | 0 | 0 |
| lint | CLEAN | CLEAN | 0 |

Both experiments are strict regressions, so neither was committed. (The KeyFrame experiment
also isolates *why* the original `KeyFrame.cpp` is empty: it must not emit its own `C1`; the
header-inline ctor lets the complete-object ctor fold into the `C2` the original exports.)

## 7. Proven vs. hypothesis

**Proven (command-backed above):**

1. The current tree already implements both witness-mandated merges: KeyFrame ctor is emitted
   only by `Transform.cpp.obj`; `Node::Node`/`~Node` are emitted only by
   `SystemPathFinder.cpp.obj`.
2. The linked `.so` symbol placement (address + size + binding) for every KeyFrame, Transform,
   Node and SystemPathFinder symbol is **identical to the original** (dynamic-symbol `objdump -T`
   diff empty).
3. Every one of those functions is already byte-exact and size-exact in `report.json`.
4. Re-splitting either merge — measured on this tree under the full `check` gate — is a strict
   regression (`byte_exact 4508→1720` for KeyFrame, `4508→4030` for Node). Reverting restores
   `4508/4508`, `RATCHET PASS`, drift 0, lint CLEAN.

**Hypothesis / not separately A/B'd:**

- That re-adding an *empty* `KeyFrame.cpp` TU (the pre-w3613 arrangement) is byte-neutral is
  supported only by the historical `44b0ad146` landing measurement ("every ELF section
  byte-identical except build-id, gate unchanged"), not by a fresh A/B here. The fresh A/B above
  tests the stronger, non-empty split.
- `SystemPathFinder.h` both includes `engine/core/Node.h` and forward-declares `class Node;`.
  This is redundant but cosmetic; it was left untouched because no measured gain was
  demonstrated and the task forbids speculative source churn.

## 8. Reproduction summary

```
cmake --preset default
cmake --build cmake-build-match --target check -j4      # 4508 / 4508, data 843, imports 207
cmake --build cmake-build-match --target drift -j4      # 0 drifted fields
cmake --build cmake-build-match --target lint  -j4      # CLEAN

# symbol placement (stripped -> use -T, not nm)
$OBJDUMP -T ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -iE 'KeyFrame|SystemPathFinder|_ZN4Node'
$OBJDUMP -T cmake-build-match/libgof2hdaa.so            | grep -iE 'KeyFrame|SystemPathFinder|_ZN4Node'

# per-TU emission (our objects)
$NM -C -S --defined-only cmake-build-match/CMakeFiles/gof2.dir/src/engine/math/Transform.cpp.obj
$NM -C -S --defined-only cmake-build-match/CMakeFiles/gof2.dir/src/game/world/SystemPathFinder.cpp.obj
```

A/B restore procedure used: `git checkout -- src/engine/core/KeyFrame.h tools/source_order.txt`
and `rm src/engine/core/KeyFrame.cpp` (resp. `SystemPathFinder.cpp` + `Node.cpp`), then
`cmake --build cmake-build-match --target check -j4` → `RATCHET PASS` (4508/4508).
