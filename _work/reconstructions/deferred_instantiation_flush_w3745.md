# Deferred instantiation flush — w3745

Worker: w3745. Branch: `wave87-w3745`, based on master `2fec65ef`.
Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b
clang 7.0.2 `-Oz`). All measurements below were produced in this worktree.
Scratch output is under `/tmp/w3745/`.

This note answers one question: **what decides the order of the lazily
instantiated `Array<...>` copies relative to the strong `Level::*` functions in
`Level.cpp.obj`, and can source moves reproduce the original order without the
section-ordering file?** It is a follow-up to
`section_order_removal_w3745.md` §7 (which left the `Array<AbstractGun*>`
placement as an open question).

## 0. Result

A source-order change in `src/game/world/Level.cpp` is a strict improvement in
the plain (no `--section-ordering-file`) link and is gate-neutral:

| build | gate byte/linked | plain byte_exact | exact addresses | exact-address prefix |
|---|---:|---:|---:|---:|
| before (HEAD `2fec65ef`) | **4497 / 4497** | **2981** | **3173 / 4524** | **2296** |
| after (this pass) | **4497 / 4497** | **2986** | **3179 / 4524** | **2304** |

Delta: `+5` plain byte_exact, `+6` exact addresses, `+8` exact-address prefix.
Gate unchanged (`4497/4497`, `RATCHET PASS`), lint `CLEAN ✓`, drift `0`.
The first remaining plain mismatch is still `ArraySetLength<AbstractGun*>`
(original `0xc3dde`, ours `0xc3efc`), but the preceding sections
(`Array<AbstractGun*>::Array()`, `ArrayAdd<AbstractGun*>`, `createStaticObject`,
`ArrayAdd<KIPlayer*>`, `createShip`, `createRoute`, `setPlayerRoute`,
`createRadioMessages`) now match the original addresses.

The section-ordering file is still required: the plain link is `2986`, not
`4497`.

## 1. Baseline (before the change)

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (`readelf -S -W cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78
.rodata          0x041b67 @ 0x1c9e10
```

The brief's `4496` baseline is stale; this HEAD is `4497/4497`.

Plain no-order baseline:

```sh
cmake --build cmake-build-match --target link_so -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/defer/noorder_base.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/defer/verify_base \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/defer/noorder_base.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/defer/noorder_base.so
```

```text
byte_exact 2981  linked_exact 4497  fuzzy_perfect 4497
addr_match: 3173/4524 same; prefix 2296;
            first (0xbe680, '_ZN5ArrayIP11AbstractGunEC2Ev', 0xc01d4)
```

## 2. The mechanism (clang 7.0.2 source)

The NDK r18b compiler is `Android (4751641 based on r328903) clang version
7.0.2`, i.e. the LLVM 7.0.0 release branch. The relevant sources were fetched
under `/tmp/w3745/clang70/` from `llvmorg-7.0.0`:

```sh
cd /tmp/w3745/clang70
for f in CodeGenModule.cpp CodeGenModule.h ASTContext.cpp Sema.cpp \
         SemaTemplateInstantiate.cpp SemaTemplateInstantiateDecl.cpp \
         CodeGenFunction.cpp CodeGenAction.cpp ModuleBuilder.cpp; do
  curl -sL -o "$f" "https://raw.githubusercontent.com/llvm/llvm-project/llvmorg-7.0.0/clang/lib/CodeGen/$f" \
    || curl -sL -o "$f" "https://raw.githubusercontent.com/llvm/llvm-project/llvmorg-7.0.0/clang/lib/Sema/$f"
done
```

Key facts (file:line in the downloaded 7.0.0 tree):

- `CodeGenModule::Release()` (`CodeGenModule.cpp:391`) calls
  `EmitDeferred()` once, after the whole TU has been processed.
- `CodeGenModule::EmitDeferred()` (`CodeGenModule.cpp:1749`) swaps
  `DeferredDeclsToEmit` into a local `CurDeclsToEmit`, then for each entry calls
  `EmitGlobalDefinition()` and recursively drains any newly queued decls (DFS).
- `CodeGenModule::EmitGlobal()` (`CodeGenModule.cpp:2068`) emits a function
  eagerly iff `MustBeEmitted(Global) && MayBeEmittedEagerly(Global)`;
  otherwise it queues the decl. `MayBeEmittedEagerly()`
  (`CodeGenModule.cpp:1987`) returns **false** for
  `TSK_ImplicitInstantiation`, so every implicit `Array<...>` copy is deferred.
- `ASTContext::DeclMustBeEmitted()` (`ASTContext.cpp:9672`) returns false for
  discardable (`weak_odr`) template copies.
- `Sema::PerformPendingInstantiations()` (`SemaTemplateInstantiateDecl.cpp:5178`)
  drains the AST-level pending-instantiation deque at end of TU; `Sema.cpp:908`
  calls it from `ActOnEndOfTranslationUnit`.
- `CodeGeneratorImpl::HandleTopLevelDecl()` (`ModuleBuilder.cpp:152`) emits each
  top-level decl as it is parsed; its `HandlingTopLevelDeclRAII` destructor
  (`ModuleBuilder.cpp:45`) only flushes `DeferredInlineMethodDefinitions`, not
  the `CodeGenModule` deferred queue.

The important consequence for section order is **not** the deferred flush
itself. With `-ffunction-sections`, the object's `.text.<mangled>` section
order is the order in which clang materialises the LLVM `Function` object
(`llvm::Function::Create`), not the order in which the body is emitted. A
strong function is materialised at its first *reference* (usually a call in an
earlier function's body); a template copy is materialised when its declaration
is first needed. The deferred queue then decides *which* template bodies are
generated at end of TU and in what order, but their module position was already
fixed by first use.

This is why moving a definition in the `.cpp` changes the object section order:
it changes which function body is generated first, and therefore which
`Function::Create` calls happen first.

### 2.1 Minimal reproductions

`/tmp/w3745/defer/t5.cpp` proves strong functions are materialised in
first-use order, not source order:

```cpp
struct T { void player(); void asteroid(); void init(); };
void T::init() { player(); asteroid(); }
void T::asteroid() { }
void T::player() { }
```

```sh
CC=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$CC -target armv7-none-linux-androideabi21 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -ffunction-sections -funwind-tables -c /tmp/w3745/defer/t5.cpp -o /tmp/w3745/defer/t5.o
$RE -SW /tmp/w3745/defer/t5.o | sed -n 's/.*\.text\.\([^ ]*\) .*/\1/p' | uniq
```

```text
_ZN1T4initEv
_ZN1T6playerEv
_ZN1T8asteroidEv
```

`/tmp/w3745/defer2/t16.cpp` proves a template copy lands after every strong
function that was materialised by the first body, not at its call site:

```cpp
#include "engine/core/Array.h"
struct AbstractGun;
struct Level { Array<AbstractGun*> *enemyGuns; void init(); void a(); void b();
               void c(); void d(); void e(); void f(); };
void Level::init() { a(); b(); c(); d(); e(); f(); }
void Level::a() { enemyGuns = new Array<AbstractGun*>(); ArraySetLength(3, *enemyGuns); }
void Level::b() { } void Level::c() { } void Level::d() { } void Level::e() { } void Level::f() { }
```

Section order: `init, a, b, c, d, e, f, Array<AbstractGun*>::Array(),
ArraySetLength<AbstractGun*>`. `init` materialises `a..f` in call order; `a`'s
body then materialises the two template copies, which are appended after `f`.

## 3. Target order vs baseline object order

Original dynsym / `cmake-build-match/section_order.txt` around `Level`:

```text
createGun
Array<AbstractGun*>::Array()
ArrayAdd<AbstractGun*>
createStaticObject
ArrayAdd<KIPlayer*>
createShip
createRoute
setPlayerRoute
createRadioMessages
ArraySetLength<AbstractGun*>
ArraySetLength<Player*>
getBoundingVolume
Array<RadioMessage*>::Array()
ArraySetLength<RadioMessage*>
getPlayer
```

Baseline `Level.cpp.obj` section order (same region):

```text
createGun
ArrayReleaseClasses<Vector*>
Array<Vector*>::~Array()
ArrayReleaseClasses<Array<Vector*>*>
Array<Array<Vector*>*>::~Array()
createStaticObject
ArrayAdd<KIPlayer*>
Array<AbstractGun*>::Array()
ArraySetLength<AbstractGun*>
ArrayAdd<AbstractGun*>
createShip
...
```

So the baseline has the `Array<AbstractGun*>` ctor/SetLength/Add block *after*
`createStaticObject`/`ArrayAdd<KIPlayer*>`; the original wants the ctor/Add
immediately after `createGun`, and `ArraySetLength` much later.

Reproduce:

```sh
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/Level.cpp.obj \
  | grep -oE '\.text\._Z[^ ]*' | sed 's/^\.text\.//' | uniq
sed -n '2395,2425p' cmake-build-match/section_order.txt
```

## 4. Controlled A/B experiments

Each row is a full `check` rebuild plus the plain no-order relink. The source
moves were scratch edits; the tree was restored between rows. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` + `verify.py` + `addr_match.py`.

| # | source move | gate byte/linked | plain byte | exact addrs | prefix | first plain mismatch |
|---|---|---:|---:|---:|---:|---|
| 0 | baseline | **4497 / 4497** | **2981** | 3173 | 2296 | `Array<AbstractGun*>::Array()` `0xbe680` vs `0xc01d4` |
| A | move `createStaticObjects()` after `createGun()` | 4491 / 4497 | 2977 | — | 2297 | `ArrayAdd<AbstractGun*>` `0xbe69c` vs `0xbe6d0` |
| B | A + move `createStaticObject()` before `createGun()` + `extern template` for `ArraySetLength<AbstractGun*>` + move `createShip()` after `createRadioMessages()` | 4491 / 4497 | 2542 | — | 2299 | `ArrayAdd<KIPlayer*>` `0xc01f0` vs `0xc0fbc` |
| C | move `createStaticObjects()` after `createGun()`, `assignGuns()` after `createStaticObject`/`createShip`/`createRoute`/`setPlayerRoute` | **4497 / 4497** | **2986** | 3179 | 2304 | `ArraySetLength<AbstractGun*>` `0xc3dde` vs `0xc3efc` |
| D | C + move `connectPlayers()` after `assignGuns()` | **4497 / 4497** | **2986** | 3179 | 2304 | same (neutral) |
| E | D + move `getBoundingVolume()` after `createRadioMessages()` | **4497 / 4497** | **2986** | 3179 | 2304 | same (neutral) |

Rows A and B are the previous worker's §7.2 measurements (reproduced here for
context). Row C is the first *gate-neutral* combination and is the landed
variant. Rows D and E are neutral, so they were not kept (each only reorders a
large function body without changing the linked address order).

### 4.1 Landed source order (`src/game/world/Level.cpp`)

```text
createGun                 1259
createStaticObjects       1420
createMission             1521
...
connectPlayers            4301
createStaticObject        4460
createShip                4863
createRoute               4998
setPlayerRoute            5016
assignGuns                5025
getBoundingVolume         5316
createRadioMessages       5369
```

The two load-bearing moves are:

1. `createGun()` before `createStaticObjects()`. `createGun` is the first user
   of `Array<AbstractGun*>::Array()` and `ArrayAdd<AbstractGun*>`; with
   `createStaticObjects` first, its body materialised `createStaticObject` and
   `ArrayAdd<KIPlayer*>` first, so the `Array<AbstractGun*>` copies landed after
   them. Moving `createGun` first makes the ctor/Add land at the original
   `0xbe680`/`0xbe69c`.
2. `assignGuns()` after `createStaticObject`/`createShip`/`createRoute`/
   `setPlayerRoute`. `assignGuns` is the first user of
   `ArraySetLength<AbstractGun*>`; its body now runs after `createRadioMessages`
   has been materialised, so the setlength copy no longer lands between the
   ctor and `ArrayAdd<AbstractGun*>`.

### 4.2 Resulting `Level.cpp.obj` order (row C)

```text
createGun
ArrayReleaseClasses<Vector*>
Array<Vector*>::~Array()
ArrayReleaseClasses<Array<Vector*>*>
Array<Array<Vector*>*>::~Array()
Array<AbstractGun*>::Array()        <- original 0xbe680  (now exact)
ArrayAdd<AbstractGun*>              <- original 0xbe69c  (now exact)
createStaticObject                  <- original 0xbe6c0  (now exact)
ArrayAdd<KIPlayer*>                 <- original 0xc01f0  (now exact)
createShip                          <- original 0xc0214  (now exact)
ArrayAdd<int>
createRoute                         <- original 0xc0ce0  (now exact)
setPlayerRoute                      <- original 0xc0dc8  (now exact)
createRadioMessages                 <- original 0xc0de4  (now exact)
Array<Player*>::Array()
ArraySetLength<Player*>
Array<Player*>::~Array()
getBoundingVolume
Array<BoundingVolume*>::Array()
ArraySetLength<BoundingVolume*>
ArraySetLength<AbstractGun*>        <- original 0xc3dde  (ours 0xc3efc)
Array<RadioMessage*>::Array()
ArraySetLength<RadioMessage*>
getPlayer
```

## 5. Flag experiments

### 5.1 `-fno-delayed-template-parsing`

Compiled `Level.cpp` with the exact build command plus
`-fno-delayed-template-parsing` and compared the object section order and
`.text` disassembly:

```sh
CC=.cache/ndk/android-ndk-r18b/toolchains/llvm/prebuilt/linux-x86_64/bin/clang++
RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
COMMON="-target armv7-none-linux-androideabi16 -march=armv7-a -mthumb -mfpu=neon \
  -mfloat-abi=softfp -std=c++11 -fPIC -frtti -fstack-protector-strong -Oz -g \
  -stdlib=libc++ -isystem .cache/ndk/android-ndk-r18b/sources/cxx-stl/llvm-libc++/include \
  -isystem .cache/ndk/android-ndk-r18b/sources/cxx-stl/llvm-libc++abi/include \
  -isystem .cache/ndk/android-ndk-r18b/sources/android/support/include \
  --sysroot=.cache/ndk/android-ndk-r18b/sysroot \
  -isystem .cache/ndk/android-ndk-r18b/sysroot/usr/include \
  -isystem .cache/ndk/android-ndk-r18b/sysroot/usr/include/arm-linux-androideabi \
  -D__ANDROID_API__=16 -I src -I third_party/fmod/inc -I third_party/gl \
  -I third_party/jni -I third_party/libzip -I third_party/crypto \
  -ffunction-sections -funwind-tables"
$CC $COMMON -fno-delayed-template-parsing -c src/game/world/Level.cpp \
  -o /tmp/w3745/defer/Level_nodtp.o
$RE -SW /tmp/w3745/defer/Level_nodtp.o \
  | grep -oE '\.text\._Z[^ ]*' | sed 's/^\.text\.//' | uniq > /tmp/w3745/defer/order_nodtp.txt
$RE -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/world/Level.cpp.obj \
  | grep -oE '\.text\._Z[^ ]*' | sed 's/^\.text\.//' | uniq > /tmp/w3745/defer/order_base.txt
diff /tmp/w3745/defer/order_base.txt /tmp/w3745/defer/order_nodtp.txt
```

Result: **identical section order** and identical `.text` disassembly (only the
object filename in the `objdump` header differs). The flag is neutral on this
target (it is an MSVC-compatibility parser flag). It was not landed.

### 5.2 `extern template` for `ArraySetLength<AbstractGun*>` alone

The isolated variant (add
`extern template void ArraySetLength<AbstractGun*>(unsigned int, Array<AbstractGun*>&);`
to `Level.cpp` and an explicit instantiation in `ArrayInstantiations.cpp`) was
measured as part of row B: it removes the `Level.o` copy but does not by itself
move the plain frontier past `ArrayAdd<KIPlayer*>`; it is gate-regressive when
combined with the `createStaticObject` move. It was not landed.

## 6. Proven vs hypothesis

**Proven by measurement in this worktree**

- Baseline is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`, lint
  `CLEAN`, drift `0`; plain no-order is `byte 2981`, `linked 4497`, prefix
  `2296`, first mismatch `Array<AbstractGun*>::Array()` (`0xbe680` vs
  `0xc01d4`).
- The landed source order gives `byte 4497 / linked 4497` (gate-neutral) and
  plain `byte 2986`, exact addresses `3179/4524`, prefix `2304` — a strict
  improvement over the `2981 / 3173 / 2296` baseline.
- The moved sections `Array<AbstractGun*>::Array()`, `ArrayAdd<AbstractGun*>`,
  `createStaticObject`, `ArrayAdd<KIPlayer*>`, `createShip`, `createRoute`,
  `setPlayerRoute`, `createRadioMessages` now match the original addresses.
- Clang 7.0.2's object section order is the `Function::Create` order, not the
  source order: `t5.cpp` shows strong functions in first-use order; `t16.cpp`
  shows a template copy appended after every strong function materialised by
  the first body.
- `-fno-delayed-template-parsing` is neutral for `Level.cpp` (identical
  section order and `.text`).

**Hypothesis / still open**

- The exact position of a deferred template body inside the module list is not
  fully explained by the 7.0.0 source. `ArraySetLength<AbstractGun*>` is first
  referenced inside `assignGuns` but its `define` appears after
  `getBoundingVolume`'s body in the IR. The deferred queue is drained at
  `Release()`, yet template copies are interleaved with strong functions. The
  working model is "materialised at first use, body emitted later", but the
  precise `DeferredDeclsToEmit` ordering for this TU was not reconstructed.
- The remaining plain blocker is `ArraySetLength<AbstractGun*>`: the original
  places it at `0xc3dde`, immediately after `createRadioMessages`; ours places
  it at `0xc3efc`, after `getBoundingVolume`'s body and the
  `Array<BoundingVolume*>` copies. Moving `getBoundingVolume`'s definition
  after `createRadioMessages` did not change the order (row E), so this is not
  a simple source-order issue.
- The original's `Array<BoundingVolume*>::Array()` /
  `ArraySetLength<BoundingVolume*>` are at `section_order.txt` line 2007, in an
  earlier TU; our `Level.o` also emits duplicate weak copies. The linker keeps
  the first copy, so the duplicates do not affect the linked address order, but
  they do affect the per-object section list.
- No claim is made that the ordering file can be deleted today: the plain link
  is `2986`, not `4497`.

## 7. Reproduction commands (row C)

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8        # 4497 / 4497, RATCHET PASS
cmake --build cmake-build-match --target lint -j8         # CLEAN ✓
cmake --build cmake-build-match --target drift -j8        # 0 field(s)
cmake --build cmake-build-match --target link_so -j8
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/defer/noorder_final.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/defer/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/defer/noorder_final.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
# -> byte_exact 2986  linked_exact 4497  fuzzy_perfect 4497
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/defer/noorder_final.so
# -> 3179/4524 same; prefix 2304;
#    first (0xc3dde, '_Z14ArraySetLengthIP11AbstractGunEvjR5ArrayIT_E', 0xc3efc)
```
