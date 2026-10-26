# Validation

How the decompiled `libgof2hdaa.so` is checked against the original Android release
(`../_work/bins/android_2.0.16_libgof2hdaa.so`, ELF 32-bit ARM, armeabi-v7a softfp, stripped).

## The build that is being validated

`cmake/toolchain.cmake` compiles every TU in `src/` with the original's toolchain — NDK r18b clang
7.0.2, `-target armv7-none-linux-androideabi21 -march=armv7-a -mthumb -mfpu=neon -mfloat-abi=softfp
-std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g -stdlib=libc++`. The `link_so` target links those objects plus
the vendored static libs the original statically linked (libcrypto subset, zlib 1.2.3, libzip 0.9.3 —
`third_party/match_libs.cmake`; libcrypto is OpenSSL 1.1.0l built with real GCC 4.9 (NDK r13b,
fetched once — the original's `.comment` names `GCC: (GNU) 4.9.x 20150123`) at -O3 ARM;
zlib/libzip built with the app's ndk-build flags — `-Oz -g
-fstack-protector-strong -ffunction-sections -mthumb` and the SHA/CRYPTO_memcmp C files
`-marm`, matching the original's per-function ISA modes) into `libgof2hdaa.so` with the NDK
**gold** linker: `-shared -soname libgof2hdaa.so --build-id`, libc++abi.a, zlib+libzip pulled
`--whole-archive`, app-provided imports (FMOD, GLES, …) left unresolved as in the original.

The link inputs mirror ndk-build's: the app's prebuilt `libfmodex.so`/`libfmodevent.so` from the
original APK (`../_work/bins/apk_libs/`, NEEDED in the original's order) — FMOD re-exports the
libgcc helpers (`__aeabi_idiv`, `_Unwind_*`, ...), which therefore stay unversioned imports as in the
original — then libc++abi.a and libgcc.a last, with ndk-build's default `--exclude-libs=libgcc.a`
(the original defines `__aeabi_ldivmod` locally without exporting it).

Link-line order (2026-10-21) follows ndk-build: crtbegin, the objects in the original's link order
(`tools/gen_link_order.py`, recovered from the original's .dynsym, gold emits symbols in input
order), the system `-l` libs up to libandroid, FMOD, libc++abi.a, `-l m`, libandroid_support.a
(hidden posix_memalign wrapper over memalign), libgcc.a, `-l dl -l c`, crtend. Result: the
undefined-import set and the defined-export set are identical to the original (207 / 0 extra),
NEEDED identical. .dynsym ORDER is not yet identical (314/5578 positions): gold (same r18b
binary as the original's) emits dynamic symbols by iterating its symbol hash table, so the order
is a function of the complete symbol-insertion sequence (every object, every referenced symbol,
in input order) and is expected to converge only as object contents/membership converge — not a
knob to tune directly.

This is a **single, plain link** — no second pass, no `--version-script`, no `__attribute__`, no
`asm`. Its full flag set, stated honestly: `--section-ordering-file` (an approved deviation that
reproduces the original's interleaved .text function order from the original's symbol addresses;
local functions, absent from the original's .dynsym, are slotted after their predecessor in their
own object, i.e. source order — the original was a plain input-order link)
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
| `check`     | `tools/ratchet.py`    | **The enforced no-degradation gate** (also gates `data_exact`, exported data-object parity from `tools/datadiff.py`): fresh build vs `tools/verify_baseline.json` + the allowlist + the pinned import set + lint. Also runs as the pre-push hook (`git config core.hooksPath tools/hooks`). |

`sodiff.py` and `goal_lint.py` are pure Python (run anywhere); the `verify` pipeline uses the NDK
`objdump`; `layout.py`/`drift_scan.py` use the NDK clang (never the host compiler — host `-m32`
is i386, which aligns 8-byte scalars to 4 and reports wrong offsets for ARM).

## Current state — symbol parity (2026-10-21)

```
original: 5370   ours: 5370   extra: 0   missing: 0   (wrong_type: 0   absent: 0)
```

**Exact defined-export parity**: every function and object the original exports is present with
the correct signature, and we export nothing it does not. `tools/sodiff_allow.txt` is empty; any
extra export is a hard failure. The 12 formerly broken zero-size UB stubs have all been
reconstructed (see the fidelity section).

### Reducible (all fixed)
How the former 50 allowlisted extras were removed — the template for any new one:

- **C1/D1 alias duplicates of inline/template ctors and dtors.** clang (`-mconstructor-aliases`)
  emits a C1/D1 alias next to C2/D2 only for a *strong or weak_odr* definition. An implicitly
  instantiated / inline (`linkonce_odr`) one is RAUW'd to the C2/D2 variant, which is the only
  name the original exports. So: no explicit `template Array<T>::Array()` / `~Array()`
  instantiations (they are `weak_odr` and exported both variants for 12 `Array<T>`s — each is
  implicitly instantiated by its real user anyway), and empty ctors/dtors the original emits weak
  go inline in the header (`GameData`, `EaseInOutMatrix`, `KeyFrame`, `AEMath::Matrix`, the
  Resource*/Material/Camera/String set, ...).
- **`ParticleSettings` D1**: the original's `~ParticleSettings` is the implicit (weak) dtor,
  odr-used only by `__GLOBAL__sub_I_ParticleSettings.cpp`, which constructs
  `ParticleSettingsRef::init` then `::cur` (both `ParticleSettings` objects, not byte arrays) and
  registers the dtor with `__cxa_atexit`. Modelling the statics with their real type reproduces
  that initializer and the weak D2-only export.
- **Engine flag globals (15)**: `src/engine/core/EngineFlags.cpp` held invented never-set flags
  (`g_Engine_shaderModeFlag`, `g_Mesh_vboEnabledFlag`, ...). Only two still had a user, a bogus
  `getAppVersion` body that duplicated `Engine::InitGL` through `AE_getInitGL*` shims (they stood
  for `Engine::enableShader` / `Engine::EnablePostEffect`). The original's `getAppVersion` is a
  size-0 symbol aliasing `Engine::InitGL`, so the body was dropped and the TU deleted.
- **`gof2::kJNIEnvCallAnchors`**: a table odr-using the five `_JNIEnv::Call*Method` members. Their
  real callers (`NFC::iap_buy_*`, `NFC::isPad/getWidth/getHeight`,
  `FileInterfaceAndroid::Read/Write/Close`) are decompiled, so the table was deleted.

### Static initializers (.init_array)
The three `.init_array` entries are instruction-identical to the original's and in its order:
Engine.cpp (`Engine::tv_modes` as `Array<int>` with `atexit(~Array)`, then `renderer`, `vendor`),
the touch buffer (`touches = realloc(touches, sizeof(Touch))`), and ParticleSettings.cpp
(`ParticleSettingsRef::init`, `::cur`). Any new namespace-scope object with a non-trivial
ctor/dtor adds an entry the original does not have.

### Known import delta: `posix_memalign`
The original imports `memalign`, not `posix_memalign`: it links the NDK's `libandroid_support.a`
after `libc++abi.a` (ndk-build's `c++_static` for a pre-21 `APP_PLATFORM`), whose hidden
`posix_memalign` (at 0x19ae14, between the libc++abi code and libgcc's `__aeabi_ldivmod`) wraps
`memalign`. libc++abi's `fallback_malloc.o`/`stdlib_new_delete.o` are the users. Our link does not
pass `libandroid_support.a`, so `posix_memalign` stays an import; fixing it is a link-input change.

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

## Current state — per-function ASM fidelity (2026-10-21, metric v2)

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
  entry-exact symbol, link-dependent address words masked neutrally. Pool words consumed by the
  PIC idiom (`ldr rX, [pc, #N]` … `add rX, pc`, rX followed forward up to 16 instructions until
  consumed or overwritten) are always masked: they are PC-to-GOT deltas that
  change value on every relink, and before this rule one that accidentally equalled a function
  entry address was "named" and produced phantom linked_exact flips on untouched functions
  whenever unrelated code shifted the layout (±2-5 noise per relink, observed on BeamGun dtors /
  shader Init family).
- **Calls through gold interworking veneers resolve to their destination** (2026-10-21): a
  `bl`/`b.w` into a Thumb->ARM veneer (`bx pc; nop; ldr ip,[pc]; add pc,ip,pc; .word`) is named by
  the veneer's target (PLT entry or function). Before, such calls masked as local, so a call to a
  decompiler shim import could score `linked_exact`; 26 rows were demoted (linked 4184 -> 4158).
- **Data references compare by what they name** (2026-10-21): every PIC `ldr rX,[pc,#N]` /
  `add rX,pc` address is resolved — a GOT slot to the symbol its dynamic relocation binds
  (GLOB_DAT/ABS32; RELATIVE slots to the object they point at), a direct address to the exported
  data object (+offset) it lies in, or to the string literal it points at (content; long strings by
  length+CRC). Before this, all such words were masked, so a function reading the wrong global,
  string or callee-pointer could still score `linked_exact`: 52 such rows were found and demoted
  (e.g. shader `Init`s passing `"a0"`/`"BumpShader.vsh"` where the original passes `"a_position"`
  and embeds the GLSL source, `zip_close` passing zlib version "1.2.3" vs the original's "1.2.7",
  `PlayerEgo::shake` taking `AERandom::next` vs `nextInt`). linked 3316 -> 3264 is that correction,
  not a code regression.
- **Inline Thumb literal pools and tbb/tbh jump tables are decoded honestly** (2026-10-21; jump-table
  entries compare by raw value): the pool is found by a linear
  sweep that ignores pc-relative "loads" decoded out of pool data (they used to mark real code as
  pool and mask it); the code is then decoded from a copy with pool bytes replaced by NOPs, because
  binutils recovers IT state by scanning backwards and a pool word that looks like `IT` put
  spurious condition codes on the real instructions after it. An aligned pool word whose raw value
  is identical in both links compares equal even if one side's value happens to fall on an
  address/function entry of that link; words resolving to two different symbols never unify.
- `linked_exact` additionally requires equal function size; the fuzzy % uses
  `difflib(autojunk=False)` (autojunk floored every ≥200-instruction function to garbage scores,
  mis-steering the worst-first campaign).
- Zero-size symbols are classified, not dropped: **`stub_zero_size` = 0** (2026-07-21). All 12
  UB-collapsed bodies (MeshMerger render/ctors, ImageFactory::reload, MTitle key handlers,
  BeamGun/SpriteGun enemy setters, PlayerEgo::stopBoost, SpriteGun::update) were reconstructed
  from Ghidra in the wave-1 stub campaign — every never-assigned static function pointer they
  called through is gone. The classification stays in the metric: a non-void function with no
  `return`, or a call through a never-assigned static function pointer, is deleted to zero bytes
  by `-Oz` and would silently alias its neighbour; the ratchet keeps the count frozen at 0.

Current numbers (of **4523** compared): **byte_exact 1352, linked_exact 4452, avg 99.70%**
(the matching build compiles with `-g` since 2026-10-21, as ndk-build does — see `cmake/toolchain.cmake`).
`verify` lists the worst matches; many low scorers are genuine decompile gaps. This long-tail
correctness work is gated by `tools/ratchet.py` against `tools/verify_baseline.json`: byte_exact
monotone, linked_exact within a ±1 transient band, stubs/missing/extra frozen, exports pinned to
`tools/sodiff_allow.txt`, undefined-import set pinned (207 names, the original's count: the only delta is posix_memalign for memalign; a typo'd callee is a hard
failure), lint clean. `cmake --build … --target check` runs it; the pre-push hook
(`git config core.hooksPath tools/hooks`) makes it unavoidable. Improvements are locked with
`tools/ratchet.py --update-baseline` in the same commit.

The `drift` target (NDK-clang record layouts, AAPCS) currently reports **0 drifted fields**. The
74-field backlog first exposed by the real ABI probe was reconciled class-by-class against binary
layout evidence under the verify gate.
