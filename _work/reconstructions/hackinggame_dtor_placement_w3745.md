# Section-order removal: `HackingGame` dtor placement (w3745)

Status: research note + landed source pass, 2026-11-03. Worktree `wt/w3745`,
branch `wave87-w3745`, based on master `2d76a0a1`. Every number below was
produced by a command run in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so` (NDK r18b clang 7.0.2 / gold 1.12).

This is the next frontier after the `MenuTouchWindow` dtor pass
(`_work/reconstructions/menutouchwindow_dtor_weakcopy_w3745.md`). The plain
(no `--section-ordering-file`) link placed `HackingGame::~HackingGame()` at
`0x14f094` instead of the original `0x14ef40`; it was the first plain
no-order mismatch.

## 1. Baseline

The brief's `4496` baseline is stale; this HEAD is `4497 / 4497`.

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (`readelf -SW cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78   (original 0x26298; known -12)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Gate `.so` sha256: `19a4e7f9d22e28d20515faf19c7e636e5712153ed1a8fd98d11ba3f3c14c8b11`.

Plain no-order baseline (`relink_noorder.py` + `verify.py` + `addr_match.py`,
commands in §6):

```text
compared 4524  avg 99.9%  byte_exact 3131  linked_exact 4497  fuzzy_perfect 4497
3427/4524 same; prefix 3249;
first (1371968, '_ZN11HackingGameD1Ev', 1372308)
```

`1371968 = 0x14ef40` (original), `1372308 = 0x14f094` (ours).

## 2. Original object order vs baseline object order

The original's own `.dynsym` address order is the authority:

```text
0x14ecc8  HackingGame::HackingGame(int, int, int, int, int)   # C1/C2
0x14ed9c  HackingGame::reInit()
0x14ef40  HackingGame::~HackingGame()                         # D1/D2
0x14ef42  HackingGame::getRewardItem()
0x14ef48  HackingGame::getRewardAmount()
0x14ef50  HackingGame::rotateRightCW(bool)
0x14ef8c  HackingGame::rotateLeftCW(bool)
0x14efc8  HackingGame::solvableInNSteps(int, int, int, int, int*)
0x14f0a4  HackingGame::gameWon(int*)
0x14f0c6  HackingGame::rotateLeftCW(int*)
0x14f0e8  HackingGame::rotateRightCW(int*)
0x14f10a  HackingGame::getDockingIndex()
0x14f110  HackingGame::gameWon()
0x14f138  HackingGame::update(int)
0x14f1cc  HackingGame::isRotating()
0x14f1e4  HackingGame::render2D()
```

The baseline `HackingGame.cpp.obj` `.text.*` order was:

```text
HackingGame::HackingGame
HackingGame::reInit
HackingGame::rotateRightCW(bool)
HackingGame::rotateLeftCW(bool)
HackingGame::solvableInNSteps
HackingGame::~HackingGame()          <-- emitted after solvableInNSteps
HackingGame::getRewardItem
HackingGame::getRewardAmount
HackingGame::rotateRightCW(int*)     <-- int* overloads reversed
HackingGame::rotateLeftCW(int*)
HackingGame::gameWon(int*)
HackingGame::getDockingIndex
HackingGame::gameWon()
HackingGame::update
HackingGame::isRotating
HackingGame::render2D
```

The source order was already the original address order (ctor, reInit, dtor,
getRewardItem, getRewardAmount, rotateRightCW(bool), rotateLeftCW(bool),
solvableInNSteps, gameWon(int*), rotateLeftCW(int*), rotateRightCW(int*), ...),
so the mismatch is **not** a source-order error in the usual sense. It is
clang's lazy `llvm::Function` creation order.

## 3. Mechanism: `.text.*` order is LLVM `Function` creation order

Clang 7 creates the `llvm::Function` object for a function at its **first
reference or definition**, not when its definition is codegen'd. A forward
declaration (`void f();`) does not create it (`CodeGenModule::EmitGlobal`
returns early for a body-less declaration); a call in an already-emitted body
does. The object's `.text.*` section order follows the module's function list,
i.e. that creation order.

Minimal reproduction (compiled with the tree's exact clang command):

```cpp
void f(); void g();
void a(){ f(); }
void b(){ g(); }
void f(){}
void g(){}
```

`readelf -SW` section order is `a, f, b, g`, not the source order `a, b, f, g`:
`a`'s body creates `f` before `b` is emitted. Likewise

```cpp
void f();
void a(){ f(); }
void b(){}
void f(){}
```

emits `a, f, b`. The same rule applied to the `HackingGame` call graph exactly
predicts the baseline object order above:

* `HackingGame` (ctor) calls `reInit` -> `reInit` is created next.
* `reInit` calls `rotateRightCW(bool)`, `rotateLeftCW(bool)`,
  `solvableInNSteps` -> those are created next, in that order.
* The dtor, `getRewardItem`, `getRewardAmount` are then created by their own
  definitions.
* `rotateRightCW(bool)`/`rotateLeftCW(bool)` bodies call the `int*` overloads
  when *their* definitions are emitted, creating `rotateRightCW(int*)` then
  `rotateLeftCW(int*)`.
* `render2D` calls `gameWon(int*)`, but that definition is emitted later.

So the dtor lands after `solvableInNSteps`, and the `int*` overloads are
reversed, exactly as observed.

## 4. Landed source change

Only the order of the top-level definitions changed; no body was edited. The
new order is chosen so the first-reference/definition creation order is the
original address order:

```text
before (source order):
  ctor, reInit, dtor, getRewardItem, getRewardAmount,
  rotateRightCW(bool), rotateLeftCW(bool), solvableInNSteps,
  gameWon(int*), rotateLeftCW(int*), rotateRightCW(int*),
  getDockingIndex, gameWon(), update, isRotating, render2D

after:
  ctor, dtor, getRewardItem, getRewardAmount, reInit,
  solvableInNSteps, gameWon(int*),
  rotateLeftCW(bool), rotateRightCW(bool),
  rotateLeftCW(int*), rotateRightCW(int*),
  getDockingIndex, gameWon(), update, isRotating, render2D
```

Why this order works:

* `reInit` must be defined after `dtor`/`getRewardItem`/`getRewardAmount`,
  otherwise its body creates `rotateRightCW(bool)`/`rotateLeftCW(bool)`/
  `solvableInNSteps` before those three definitions.
* `solvableInNSteps` is defined immediately after `reInit`; it is already
  created by `reInit`'s body, so its definition creates nothing new.
* `gameWon(int*)` is defined next; it is not called before this point, so it is
  created here (original `0x14f0a4`).
* `rotateLeftCW(bool)` is defined before `rotateRightCW(bool)` so their
  `int*` callees are created as `rotateLeftCW(int*)` then
  `rotateRightCW(int*)`, matching `0x14f0c6` then `0x14f0e8`.
* The remaining definitions follow in the original address order.

The resulting object order is exactly the original:

```text
HackingGame::HackingGame
HackingGame::reInit
HackingGame::~HackingGame
HackingGame::getRewardItem
HackingGame::getRewardAmount
HackingGame::rotateRightCW(bool)
HackingGame::rotateLeftCW(bool)
HackingGame::solvableInNSteps
HackingGame::gameWon(int*)
HackingGame::rotateLeftCW(int*)
HackingGame::rotateRightCW(int*)
HackingGame::getDockingIndex
HackingGame::gameWon()
HackingGame::update
HackingGame::isRotating
HackingGame::render2D
```

The source now carries a short comment above the definitions recording the
order and the reason, so it is not "fixed" back.

## 5. Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8`; `plain` is
`relink_noorder.py` (the exact gold line minus `--section-ordering-file`)
followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | gate `.so` sha256 | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---|---:|---:|---:|---|
| baseline | **4497 / 4497** | `19a4e7f9…` | **3131** | 3427 / 4524 | 3249 | `HackingGame::~HackingGame` `0x14ef40` vs `0x14f094` |
| reorder (landed) | **4497 / 4497** | `19a4e7f9…` | **3132** | **3439 / 4524** | **3283** | `RadioMessage::RadioMessage(int,int,int,int)` `0x1516cc` vs `0x1517c2` |

Landed delta: **+1 plain byte_exact, +12 exported functions at their original
address, +34 exact-address prefix**, gate unchanged.

The whole `HackingGame` block is now address-exact in the plain no-order link
(`nm -nSD` on both files gives identical addresses for all 18 exported
`HackingGame` rows). All `HackingGame` rows are `byte=True` in the plain verify
report except `rotateLeftCW(bool)` and `rotateRightCW(bool)`; those two still
differ only in their `Globals::sound->play` PLT slot, not in placement.

### Gate `.so` side effects

None. The gate `.so` is **byte-identical** before and after:

```sh
sha256sum /tmp/w3745_hg/gate_base.so /tmp/w3745_hg/gate_after.so
# both 19a4e7f9d22e28d20515faf19c7e636e5712153ed1a8fd98d11ba3f3c14c8b11
cmp -l /tmp/w3745_hg/gate_base.so /tmp/w3745_hg/gate_after.so | wc -l
# 0
```

This is expected: the section-ordering file already placed every function at
the same address, and the reorder does not change any body or the set/order of
`.ARM.exidx`/`.ARM.extab` records in the final link.

## 6. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745_hg/final_noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745_hg/verify_final \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745_hg/final_noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745_hg/final_noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/menu/HackingGame.cpp.obj \
  | sed -n 's/^ *\[ *[0-9]*\] \(\.text\.[^ ]*\).*/\1/p' \
  | sed 's/^\.text\.//' | c++filt

# original address order
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nSD --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep '_ZN11HackingGame'

# gate .so A/B (baseline vs landed source)
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_hg/gate_after.so
git stash push -- src/game/menu/HackingGame.cpp
cmake --build cmake-build-match --target link_so -j8
cp cmake-build-match/libgof2hdaa.so /tmp/w3745_hg/gate_base.so
git stash pop
cmake --build cmake-build-match --target link_so -j8
sha256sum /tmp/w3745_hg/gate_base.so /tmp/w3745_hg/gate_after.so
cmp -l /tmp/w3745_hg/gate_base.so /tmp/w3745_hg/gate_after.so | wc -l   # 0
```

## 7. Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3131 / 4497`, prefix 3249,
  3427 / 4524, first mismatch `HackingGame::~HackingGame` `0x14ef40` vs
  `0x14f094`.
* Clang 7's object `.text.*` order is the `llvm::Function` creation order
  (first reference or definition), demonstrated by the minimal `a(){f();}
  b(){g();} f(){} g(){}` reproduction (`a, f, b, g`).
* The baseline `HackingGame.cpp.obj` order is exactly predicted by that rule
  from the baseline source order and call graph.
* Reordering only the top-level definitions (no body edits) makes the object
  order exactly the original `.dynsym` address order.
* The reorder is gate-neutral: `4497 / 4497`, lint `CLEAN ✓`, drift 0, every
  ELF section size/base unchanged, and the gate `.so` is byte-identical
  (`sha256` match, `cmp -l` 0).
* The plain no-order build improves to `3132 / 4497`, prefix 3283,
  3439 / 4524; the whole `HackingGame` block is address-exact.
* The next plain no-order frontier is
  `RadioMessage::RadioMessage(int, int, int, int)` (`0x1516cc` original,
  `0x1517c2` ours).

**Hypothesis / open**

* The ordering file still cannot be removed: the plain no-order build is
  `3132`, not `4497`.
* The `RadioMessage` block is a separate TU and needs its own emission-order
  pass; no claim is made about its cause yet.
