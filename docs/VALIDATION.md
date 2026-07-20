# Validation

How the decompiled `libgof2hdaa.so` is checked against the original Android release
(`../_work/bins/android_2.0.16_libgof2hdaa.so`, ELF 32-bit ARM, armeabi-v7a softfp, stripped).

## The build that is being validated

`cmake/toolchain.cmake` compiles every TU in `src/` with the original's toolchain — NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi21 -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp
-fPIC -frtti -fstack-protector -Oz -stdlib=libc++`. The `link_so` target links those objects plus
the vendored static libs the original statically linked (libcrypto subset, zlib 1.2.3, libzip 0.9.3 —
`third_party/match_libs.cmake`; zlib/libzip built `-mthumb` and the SHA/CRYPTO_memcmp C files
`-marm`, matching the original's per-function ISA modes) into `libgof2hdaa.so` with the NDK
**gold** linker: `-shared -soname libgof2hdaa.so --build-id`, libc++abi.a, zlib+libzip pulled
`--whole-archive`, app-provided imports (FMOD, GLES, …) left unresolved as in the original.

This is a **single, plain link** — no second pass, no `--version-script`, no `__attribute__`, no
`asm`. Its full flag set, stated honestly: `--section-ordering-file` (an approved deviation that
reproduces the original's interleaved .text function order from the original's symbol addresses)
and `--unresolved-symbols=ignore-all` (undecompiled engine internals import as shims; the import
set is pinned by the ratchet, so a typo'd callee cannot slip in as a silent new import).
`--allow-multiple-definition` was removed after it was found masking a real triplicate definition
of the Vector/Matrix operators — duplicate strong definitions are hard link errors now. Any symbol
the original does not export but we do is removed by reshaping the C++, or recorded below as an
irreducible delta (machine-checked via `tools/sodiff_allow.txt`).

## Targets

| Target      | Tool                  | What it checks |
|-------------|-----------------------|----------------|
| `diff`      | `tools/sodiff.py`     | Whole-`.so` **symbol parity**: defined, exported `.dynsym` names, ours vs the original, against the documented irreducible delta (`--allow tools/sodiff_allow.txt`). Exit 0 = parity modulo the allowlist. |
| `verify`    | `tools/verify/`       | **Per-function ASM match** for every function defined in both `.so`s: `byte_exact` / `linked_exact` / fuzzy %, plus `stub_zero_size` (UB-collapsed bodies) and the missing/wrong_type/extra deltas. Auto-syncs `report.json` to the repo root. |
| `verify-fn` | `tools/verify/`       | `FN=<mangled> … verify-fn` — side-by-side disassembly of one function (`\|` structural diff, `~` link-address diff only). |
| `lint`      | `tools/goal_lint.py`  | Structural invariants. **Hard** rules (no `asm` in any spelling/`__attribute__`/`extern "C"`/linker hacks, templates without explicit instantiation, no include cycles) must be 0 everywhere. **Soft** rules (one class per file, no `void*` incl. `typedef`/`using` laundering, no `union`) are gated as a per-file **ratchet** vs `tools/goal_lint_baseline.json`. |
| `drift`     | `tools/layout.py`     | Every `field_0xNN` sits at offset 0xNN under the **real ABI** (NDK clang, ARM AAPCS; forced layout completion so every class is actually checked). |
| `check`     | `tools/ratchet.py`    | **The enforced no-degradation gate**: fresh build vs `tools/verify_baseline.json` + the allowlist + the pinned import set + lint. Also runs as the pre-push hook (`git config core.hooksPath tools/hooks`). |

`sodiff.py` and `goal_lint.py` are pure Python (run anywhere); the `verify` pipeline uses the NDK
`objdump`; `layout.py`/`drift_scan.py` use the NDK clang (never the host compiler — host `-m32`
is i386, which aligns 8-byte scalars to 4 and reports wrong offsets for ARM).

## Current state — symbol parity (2026-07-20)

```
original: 5370   ours: 5423   extra: 53 (all allowlisted)   missing: 0   (wrong_type: 0   absent: 0)
```

**Zero missing, zero wrong_type** — every function and object the original exports, with the correct
signature, is present (12 of them currently as broken zero-size UB stubs — see the fidelity section).
The **53 extra** symbols are all documented in `tools/sodiff_allow.txt`, so the `diff` target exits
0; any extra beyond the list is a hard failure. They break down as 36 ctor/dtor aliases (below),
15 engine flag globals (below), `SHA256_version`, and `gof2::kJNIEnvCallAnchors`:

### Reducible (already fixed)
Moving an empty/simple **out-of-line** ctor/dtor **inline into the header** makes clang emit it weak
and fold the redundant complete-object variant (C1/D1) into the base-object variant (C2/D2) the
original exports. Done for `GameData`, `EaseInOutMatrix::~EaseInOutMatrix`, `KeyFrame::KeyFrame`
(40 → 36). This is the template for any further reductions.

### Irreducible under the "no linker scripts" rule (33 symbols)
**Compiler-emitted C1/C2 & D1/D2 constructor/destructor alias duplicates.** clang emits both the
complete-object (C1/D1) and base-object (C2/D2) variant; for a class with no virtual bases they are
identical and the original's link kept only one name. The inline trick above removes the duplicate
**only when an out-of-line odr-use of the surviving variant happens to remain** — verified
empirically that for most of these classes inlining *also* drops the C2/D2 the original keeps
(`Camera`, `Material`, `ParticleSettings`, `ResourceTransform` each went `missing` when inlined).
Forcing the out-of-line copy back would require `__attribute__((used))` or a `--version-script`,
both forbidden. These are therefore left as a documented delta:

- `AbyssEngine::ResourceMesh` C1`(const char*,u16,bool)`, D1
- `AbyssEngine::ResourceTexture` C1`(const char*,float)`, C1`(const String&,float)`, D1
- `AbyssEngine::ResourceMaterial` C1`(u16,BlendMode)`, C1`(u16,u16,BlendMode)`
- `AbyssEngine::ResourceTransform` D1
- `AbyssEngine::AEMath::Matrix` C1`()`
- `AbyssEngine::Camera` C1`(float×5)`
- `AbyssEngine::Material` C1`(Material*)`, C1`()`, D1
- `AbyssEngine::String` C1`(char)`, C1`(int)`, C1`(float)`, C1`(long long)`, C2`(const String&)`
- `ParticleSettings` D1
- `SpriteGun` C1`(Gun*,int)`
- 11 `Array<T>` instantiations (`Array<TouchSlider*>`, `Array<BluePrint*>`, `Array<GameRecord*>`,
  `Array<PendingProduct*>`, `Array<KeyCode*>`, `Array<Array<ImagePart*>>`,
  `Array<Array<String*>>`, `Array<signed char*>`, `Array<void*>`, `Array<bool>`) — C1 and/or D1/D2.

`AbyssEngine::String` is additionally the polymorphic `{vtable,data,length}` class noted as needing
a full re-model; its copy-ctor delta (`C2(const String&)` vs the original's `C2(const String&,bool)`)
is part of that and is not safe to change piecemeal.

### Engine flag globals (15 symbols)
`src/engine/core/EngineFlags.cpp` (commit dcdcb0e1) holds never-set engine flags
(`g_Mesh_vboEnabledFlag`, `g_Engine_shaderModeFlag`, `g_Camera_frustumEnabledFlag`, …,
`g_MeshRelease_freeFn`) split into their own TU **with external linkage** so `-Oz` cannot
constant-fold the null through the flag-gated engine functions (the original keeps them as
internal data, accessed GOT-indirect). Hidden visibility would change the GOT codegen and
`__attribute__` is banned, so the 15 exports stay as a documented delta.

### Vendored / not-yet-wired (2 symbols)
- `SHA256_version` — a data symbol from the vendored libcrypto subset; the original's static link
  garbage-collected it. Not part of the decompiled source.
- `gof2::kJNIEnvCallAnchors` — an internal table whose purpose is to odr-use the five
  `_JNIEnv::Call*Method` member functions (which the original **does** export). It must stay until
  its real consumer in the JNI dispatch path is decompiled and references it; internalizing it now
  GCs the table and drops those five exported methods.

## Structural invariants — all at 0

The full readable-cleanup sweep is complete: `goal_lint src` reports **0 for every criterion**
(`void_ptr 0`, `union_decl 0`, `one_class 0`, `banned_token 0`, `explicit_inst 0`, `circular_inc 0`),
and symbol parity held throughout. What this required, and the handful of ABI-mandated exceptions
that are documented inline rather than removed:

- **`void*` → real types.** ~4180 `void*` were retyped to the real pointed-to type (recovered from
  cast sites / usage). The irreducible remainder carries a same-line `// lint: void_ptr` waiver: the
  **69 exported function signatures the original itself exports with `void*` params** (e.g.
  `loadingScreen(PaintCanvas*, int, void*)`, `ModStation::OnTouchEnd(int,int,void*)`) and the
  undefined-import decompiler shims — changing either would change a mangled name and break parity.
- **`union` → single member.** Every union whose members are the *same field under different names*
  (a `field_0xNN` placeholder + a descriptive name, or two identical-purpose fields — e.g. `Array`'s
  `size_`/`count`, the `Layout`/`ListItem`/`Status`/`Radar` field-alias unions) was **collapsed** to
  one member by renaming all users tree-wide to the kept name (each cross-file rename receiver-checked,
  since `field_0xNN` names are reused across unrelated classes). Function-local bit-pun unions became
  `__builtin_memcpy`/`reinterpret_cast`. Only **genuine type-puns** remain — members reinterpreting the
  same bytes as *different types/sizes* (`int`/`float`, array/`Matrix`, wide field over named bytes,
  pointer over handle/flag-bytes) — which model the binary's real memory layout and carry a same-line
  `// lint: union_decl` waiver. A scan confirms 0 same-type placeholder-alias unions remain.
- **one class per file** is enforced on **headers** (the public-API surface; every `.h` defines one
  class). Class/struct definitions inside a `.cpp` are single-TU implementation helpers (recovered
  data overlays / view structs) and are not separate public classes.
- **`extern "C"` is hard-banned**, with a per-site `// lint: extern_c` waiver allowed **only** for the
  genuinely ABI-mandated boundary — verified by checking the symbol is exported **unmangled** in the
  original `.so` (JNI `Java_*`, NDK `ndk*`/`loadAPK`/`ExitFunction`, the Blowfish C functions, `F`,
  …) or is the standard GL/libc C ABI (`glVertexPointer`, `sinf`, …). All 104 sites were confirmed
  mandated; a *gratuitous* `extern "C"` (no waiver) fails the gate. `asm`/`__asm`/`__attribute__`/
  `__declspec` are hard-banned and absent.

`lint` also stays a **ratchet**: `--update-baseline` records the current (now all-zero) counts, and a
file may not raise its count — so new code can't reintroduce a `void*`, `union`, or second header
class. The self-test (inject a `void*` into a `.cpp`, a `union` into a `.h`, a second class into a
`.h`) trips three gate failures, confirming the gate is live. The scanner was hardened 2026-07-20
(all inline-`asm` spellings including `asm volatile`/`__asm__`; `enum class` no longer miscounts as
a class; `typedef`/`using` `void` aliases count as `void_ptr`). Waivers are strictly **same-line**:
the 726 historical adjacent-line `void_ptr` waivers were mechanically migrated onto their sites
(append-only comment edits), so a waiver can never silently cover a different, new violation on a
nearby line.

## Current state — per-function ASM fidelity (2026-07-20, metric v2)

A separate axis from symbol parity. The verify metric was rebuilt for structural honesty:

- **Intra-function branch targets compare literally** (they are link-independent) — a function
  branching to the wrong place inside itself can no longer score `linked_exact` (this caught
  `ApplicationManager::SoundResume` scoring 100% with objectively different control flow).
- **Cross-function calls/tail-calls compare by resolved symbol name**, seen through the PLT on
  either side, for every callee exported by both binaries — a call to the wrong function fails
  even though all absolute addresses differ between the two links (this caught
  `PaintCanvas::GetMeshResourceId` calling a `paintcanvas_ext_strcmp` shim where the original
  calls `String::Compare`). Callees local to either binary mask neutrally.
- **Literal-pool words are compared as data**, not garbage-decoded instructions: constants by
  exact value (mission timers/coordinates now diff for real), function-pointer words by
  entry-exact symbol, link-dependent address words masked neutrally.
- `linked_exact` additionally requires equal function size; the fuzzy % uses
  `difflib(autojunk=False)` (autojunk floored every ≥200-instruction function to garbage scores,
  mis-steering the worst-first campaign).
- Zero-size symbols are classified, not dropped: **`stub_zero_size` = 12** exported functions
  (MeshMerger render/ctors, ImageFactory::reload, MTitle key handlers, BeamGun/SpriteGun enemy
  setters, PlayerEgo::stopBoost, SpriteGun::update) are UB-collapsed bodies — a non-void function
  with no `return`, or a call through a never-assigned static function pointer, which `-Oz`
  deletes to zero bytes so the symbol aliases its neighbour. These are broken bodies to
  reconstruct from Ghidra, not "missing" symbols.

Current numbers (of **4511** compared): **byte_exact 1115, linked_exact 2125, avg 72.33%**.
`verify` lists the worst matches; many low scorers are genuine decompile gaps. This long-tail
correctness work is gated by `tools/ratchet.py` against `tools/verify_baseline.json`: byte_exact
monotone, linked_exact within a ±1 transient band, stubs/missing/extra frozen, exports pinned to
`tools/sodiff_allow.txt`, undefined-import set pinned (1095 names; a typo'd callee is a hard
failure), lint clean. `cmake --build … --target check` runs it; the pre-push hook
(`git config core.hooksPath tools/hooks`) makes it unavoidable. Improvements are locked with
`tools/ratchet.py --update-baseline` in the same commit.

The `drift` target (NDK-clang record layouts, AAPCS) currently reports **74 drifted fields across
8 classes** (PlayerFighter 26, Radar 18, GameData 16, ParticleSystemMesh 5, PlayerAsteroid 3,
IParticleSystem 2, Hud 3 pad-marker names, PlayerTurret 1 stale name) — real pre-existing layout
bugs the old host-i386 probe could not see. Reconciling each against Ghidra `get_struct_layout`
under the verify gate is open struct-campaign work.
