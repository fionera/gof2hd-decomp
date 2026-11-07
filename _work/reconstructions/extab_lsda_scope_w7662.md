# `.ARM.extab` LSDA: `PaintCanvas::DrawStringColor` nounwind call (w7662)

Status: research note + one landed source fix, 2026-11-07. Worktree
`/home/debian/GalaxyOnFire2/wt/w7662`. Ground truth
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(image base 0x10000; real address = Ghidra address - 0x10000).

## Result

| metric | before | after |
|---|---:|---:|
| `.ARM.extab` differing bytes | 1433 | **1430** |
| `.ARM.extab` differing records | 14 | **13** |
| `.ARM.exidx` differing bytes | 0 | **0** |
| `.rel.plt` differing bytes | 0 | **0** |
| byte_exact / linked_exact | 4506 / 4506 | **4506 / 4506** |
| size-exact / data / imports | 4524, 843, 207 | same |
| lint / drift | CLEAN / 0 | CLEAN / 0 |

`.ARM.extab` is still size- and base-exact at `0x26298` @ `0x1a3b78`.

## The landed fix: `PaintCanvas::DrawStringColor`

The record owner is `PaintCanvas::DrawStringColor` (`0x78274`, 352 B), whose
body is byte-exact. Its last two LSDA rows were:

```
orig  (256, 8, 308) (264, 88, 0)
ours  (256,14, 308) (270, 82, 0)
```

Offset 256 is the `SetColor(savedColor)` invoke (`0x78374`); offset 264 is the
`ArrayReleaseClasses<String*>` call (`0x7837e`), followed by `Array::~Array`
and `operator delete`. In the original the `ArrayReleaseClasses` call is a
plain `call` (the gap row starts at 264), so clang considered it **nounwind**.
In ours it was an `invoke` sharing the `SetColor` landing pad 308, so the two
regions merged into `(256,14,308)`.

### Root cause (new hypothesis this wave)

The callee `ArrayReleaseClasses<String*>` is byte-exact (74 B) in both builds
and its `.ARM.extab` record is the same 12-byte empty LSDA (personality, no
call sites). The difference is purely the caller's `invoke` vs `call`.

`CodeGenFunction::TryMarkNoThrow` (clang 7 `CodeGenFunction.cpp:1219`) marks a
function `nounwind` when no instruction may throw, but it skips
`F->isInterposable()`. `linkonce_odr` is not interposable, so that is not the
blocker. The blocker is the indirect virtual-destructor `delete` inside the
template: at IR-gen it is an `invoke` (the delete expression needs a cleanup
that calls `operator delete` if the destructor throws), so `TryMarkNoThrow`
gives up. The landing pad is later optimised away, leaving a personality but
no call sites. The `FunctionAttrs` pass cannot recover it either because
`linkonce_odr` is `mayBeDerefined()` and the nounwind inference requires
`hasExactDefinition()` (`FunctionAttrs.cpp:1119`).

Adding `noexcept` to the template makes the callee's IR `call` sites
non-throwing, so `TryMarkNoThrow` marks it `nounwind`; the caller then emits a
plain `call`. This was verified on an isolated `PaintCanvas.cpp` compile:
the record becomes exactly the original `(256,8,308) (264,88,0)`.

### Why the fix is per-TU, not global

A global `noexcept` on `ArrayReleaseClasses` (declaration in `Array.h`,
definition in `ArrayReleaseClasses.h`, every explicit instantiation) was tried
in w7642 and shrinks `.ARM.extab` by 480 B (0x26298 -> 0x260b8): every caller
that includes `ArrayReleaseClasses.h` loses its landing pads. The original is
not globally noexcept — e.g. `ScrollTouchBox::~ScrollTouchBox` (`0x1355d8`)
calls the same `ArrayReleaseClasses<String*>` as an `invoke` with a terminate
landing pad at `0x1355fa`. So the original's `noexcept` is visible only in the
TU that defines/instantiates the specialization (`PaintCanvas.cpp`).

Landed as a per-TU opt-in:

```cpp
// PaintCanvas.cpp
#define GOF2_ARC_NOEXCEPT
...
// Array.h
#ifdef GOF2_ARC_NOEXCEPT
template<class T> void ArrayReleaseClasses(Array<T> &a) noexcept;
#else
template<class T> void ArrayReleaseClasses(Array<T> &a);
#endif
// ArrayReleaseClasses.h
template<class T>
void ArrayReleaseClasses(Array<T> &a)
#ifdef GOF2_ARC_NOEXCEPT
    noexcept
#endif
{ ... }
```

Only `PaintCanvas.cpp` defines the macro, so only its `ArrayReleaseClasses`
call site changes. `.ARM.extab` 1433 -> 1430 (exactly the 3 differing bytes of
this record); every other record is untouched; the full gate is green.

## Remaining 13 differing records (1430 bytes)

Owner = the function at the exidx entry (the nearest preceding dynsym is the
*previous* function, so `exidx_raw_audit.py`'s printed names are off by one).

| record owner | body byte-exact? | diff bytes | cause |
|---|---|---:|---|
| `PlayerJumpgate::PlayerJumpgate` | **yes** | 15 | cleanup-trampoline assignment (234/236/238) |
| `RecordHandler::recordStoreRead` | **yes** | 3 | two identical cleanup trampolines (5078/5100) swapped |
| `Radar::Radar` | **yes** | 7 | two identical cleanup trampolines (2462/2464) swapped |
| `ModStation::ModStation` | no (99.2%) | 2 | call-site range start differs by 8 |
| `Generator::createMission` | no (90.0%) | 27 | body layout |
| `Level::createMission` | no (92.3%) | 139 | body layout |
| `Level::createShip` | no (70.5%) | 70 | body layout |
| `Radar::draw` | no (75.3%) | 29 | body layout |
| `LevelScript::process` | no (60.2%) | 292 | body layout |
| `HangarWindow::render` | no (94.4%) | 762 | body layout |
| `Hud::draw` | no (96.6%) | 40 | body layout |
| `SpaceLounge::drawLounge` | no (95.7%) | 14 | body layout |
| `MGame::OnTouchEnd` | no (81.9%) | 30 | body layout |

The three byte-exact records are the same ARM-backend artifact documented in
w7642: two or more `landingpad` blocks whose cleanup code is identical. The
backend emits separate `b.n <cleanup>` trampolines and assigns them to call
sites in IR cleanup-block order; the machine code is identical, only the LSDA
landing-pad operands differ.

* `Radar::Radar` (`0x12d484`): trampolines 2462/2464/2466 are all
  `b.n 0x12de28`. Original rows 1/3/11/13/15 -> 2462 and 19/21 -> 2464;
  ours has the two groups exactly swapped (rows 1/3/11/13/15 -> 2464,
  19/21 -> 2462). This is a pure IR cleanup-block order swap.
* `PlayerJumpgate::PlayerJumpgate` (`0xa4fa4`): trampolines 234/236/238 all
  branch to the same "destroy base" cleanup; original assigns rows 1/3/7 ->
  238, 4 -> 236, 5 -> 234; ours 1/7 -> 236, 2/4 -> 234, 5 -> 238.
* `RecordHandler::recordStoreRead` (`0xcbd34`): pads 5078/5100 are both
  `b.n 0xcd24e` (destroy local `String path`); rows 161/162/164 are swapped.

## Negative / attempted this wave

* **Global `noexcept`**: fixes `DrawStringColor` but `.ARM.extab` shrinks by
  480 B (0x26298 -> 0x260b8) because every TU that includes
  `ArrayReleaseClasses.h` loses landing pads. Reverted.
* **Explicit instantiation of `ArrayReleaseClasses<String*>` in
  `PaintCanvas.cpp`**: no LSDA change (still `invoke`).
* **`noexcept` on `String::~String` only**: no LSDA change (the destructor is
  already implicitly `noexcept`; the `delete` expression's IR-gen invoke is
  what blocks `TryMarkNoThrow`).
* **Dummy earlier call to the specialization in the same TU**: the dummy was
  inlined/removed and the LSDA did not change.
* **Guard-pattern `ArrayReleaseClasses.h` before `Array.h`**: cannot work in
  this tree because `PaintCanvas.h` includes `Array.h` first; the per-TU
  macro is the minimal equivalent.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w7662
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6   # RATCHET PASS, byte 4506
cmake --build cmake-build-match --target lint  -j6   # CLEAN
cmake --build cmake-build-match --target drift -j6   # 0 fields

python3 _work/reconstructions/exidx_raw_audit.py \
  /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
# extab differing bytes 1430, records differ 13, len-diff 0
```

Scratch LSDA decoders used this pass live in `/dev/shm/w7662/`
(`detail.py`, `lsda2.py`, `callers2.py`, `objextab.py`).
