# AbstractGun -> BeamGun.cpp TU merge study (w15096)

Status: research note (2026-11-25), **no source change landed**. Worktree
`wt/w15096`, branch `wave193-w15096`, based on master `bac641cc9`. Every claim below is
backed by a command against `../_work/bins/android_2.0.16_libgof2hdaa.so` or by a
controlled A/B rebuild of this tree with the NDK r18b clang 7.0.2 / gold link.

## TL;DR

* The current tree **already has the original HD TU shape**: `AbstractGun` is
  header-only and there is no `AbstractGun.cpp`. The HD source dump attributes the
  `AbstractGun::is*` bodies and its `typeinfo`/`typeinfo name` to
  `Classes/Game/BeamGun.cpp`, and our linked `.dynsym` is address-for-address
  identical to the original for every `AbstractGun` symbol.
* Re-introducing an **empty** `AbstractGun.cpp` TU immediately before `BeamGun.cpp`
  is gate-neutral and leaves every ELF section byte-identical except
  `.note.gnu.build-id` (**Experiment 1: no-op**).
* Actually moving the four `is*` bodies out of line into `BeamGun.cpp` is a
  **regression**: byte_exact **4508 -> 1378**, `extra` 0 -> 2, three new exported
  `AbstractGun` RTTI/vtable/dtor symbols, and `.data.rel.ro` grows by 48 bytes
  (**Experiment 2: reject**).
* Conclusion: the requested fold is already realized by the header-only source
  shape. Commit the note only; do not land either experiment.

## 1. Ground-truth TU evidence

`_work/original_layout/tu_mapping.md:182`:

```
| game/weapons/AbstractGun.cpp | removed(empty) | Classes/Game/BeamGun.cpp | w3613: AbstractGun.h is header-only and its code is emitted by BeamGun.cpp; empty split TU removed |
| game/weapons/BeamGun.cpp     | exact(HD)      | Classes/Game/BeamGun.cpp |  |
```

`_work/original_layout/original_layout.json` ->
`tu_symbols['gof2hd_1.1.19_ios']['Classes/Game/BeamGun.cpp']` lists exactly:

```
_BeamGun::BeamGun(int, Gun*, int, Level*)      (x2)
_BeamGun::~BeamGun()                           (x3)
_BeamGun::render()
_BeamGun::update(int)
_BeamGun::setEnemies(Array<Player*>*)
_BeamGun::setEnemy(Player*)
_BeamGun::translate(AbyssEngine::AEMath::Vector const&)
_BeamGun::replaceGun(unsigned int, int)
_AbstractGun::isRocketGun()
_AbstractGun::isBombGun()
_AbstractGun::isMineGun()
_AbstractGun::isAutoTurret()
_MUZZLE_MESHES
_typeinfo name for BeamGun
_typeinfo name for AbstractGun
_vtable for BeamGun
_typeinfo for AbstractGun
_typeinfo for BeamGun
```

Notably, the **HD** dump has **no** `~AbstractGun` and no `_vtable for AbstractGun`
in this TU. (The SD dump additionally lists `~AbstractGun` x2 and
`_vtable for AbstractGun`, so SD and HD differ here.) The HD file list also contains
no `AbstractGun.cpp` source file at all, only `AbstractGun.h` and `BeamGun.cpp/.h`.

This is consistent with the historical cleanup commit `44b0ad146` ("TU convergence:
drop eight empty invented TUs", byte 4496/linked 4496), which deleted our
`AbstractGun.cpp` (it contained only `#include "AbstractGun.h"` plus blank lines) and
left every ELF section byte-identical except `.note.gnu.build-id`.

Current source state (unchanged by this study):

* `src/game/weapons/AbstractGun.h` is header-only: `virtual ~AbstractGun() {}` and
  the four `is*` predicates are in-class inline bodies returning `0`.
* `BeamGun.h` includes `AbstractGun.h`; `BeamGun : AbstractGun`.
* `grep -R "AbstractGun::" src` finds no out-of-line definition.
* `tools/source_order.txt` contains `src/game/weapons/BeamGun.cpp` (line 192) and no
  `AbstractGun.cpp`.

## 2. Baseline (current tree, clean)

Commands:

```
cmake --build cmake-build-match --target check -j4
cmake --build cmake-build-match --target lint  -j4
cmake --build cmake-build-match --target drift -j4
```

Recorded output:

```
verify: byte 4508  linked 4508  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Gate table: **byte_exact 4508/4524, linked_exact 4508/4524, size 4524/4524,
data 843/843, imports 207, lint CLEAN, drift 0.**

The resulting `cmake-build-match/libgof2hdaa.so` has
`sha256 001322327e952b2ae9ff71285ebf8caa90322635b16fb8373902ae981f373c00`.
Per-section dump diff against the original:

```
section                       orig size   new size  orig base   new base  diffbytes
.ARM.exidx                        28528      28528     19cc08     19cc08       1307
.ARM.extab                       156312     156308     1a3b78     1a3b78     122327
.note.gnu.build-id                   36         36        1cc        1cc         20
.rel.plt                          29392      29392      5751c      5751c          4
.text                           1259464    1259464      69440      69440      78772
(5 differing rows)
```

This is exactly the documented baseline residual.

## 3. Experiment 1 — re-introduce an empty `AbstractGun.cpp` TU

Hypothesis: maybe the original build had a separate (empty) TU whose mere presence in
the input link order matters for section layout.

Change (temporary, reverted):

1. Created `src/game/weapons/AbstractGun.cpp` containing only
   `#include "game/weapons/AbstractGun.h"`.
2. Inserted `src/game/weapons/AbstractGun.cpp` immediately before
   `src/game/weapons/BeamGun.cpp` in `tools/source_order.txt`.
3. Rebuilt and ran the full `check` target.

Measured result:

```
verify: byte 4508  linked 4508  stubs 0  missing 0  extra 0  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

A section-by-section comparison of the Experiment-1 `.so` against the pre-experiment
build produced exactly one differing row:

```
section                       orig size   new size  orig base   new base  diffbytes
.note.gnu.build-id                   36         36        1cc        1cc         20
(1 differing rows)
```

All 26 other sections were byte-identical (same size, same base, zero differing
bytes), including `.text`, `.rodata`, `.data`, `.data.rel.ro`, `.dynsym`, `.dynstr`,
`.hash`, `.rel.dyn` and `.rel.plt`.

**Proven:** adding/removing an empty `AbstractGun.cpp` is byte-neutral except for the
build-id. It does not change TU membership of any emitted code. Reverted with
`git checkout --`; current `.so` and tree are back to the baseline above.

## 4. Experiment 2 — real out-of-line fold into `BeamGun.cpp`

Hypothesis: force the four `AbstractGun::is*` bodies to be owned by `BeamGun.cpp`
instead of every header-including gun TU emitting weak inline copies.

Change (temporary, reverted):

1. In `src/game/weapons/AbstractGun.h`, changed the four in-class bodies to
   declarations:
   `virtual bool isRocketGun(); virtual int isBombGun(); virtual int isMineGun();
   virtual int isAutoTurret();`.
2. Appended `AbstractGun::isRocketGun/isBombGun/isMineGun/isAutoTurret` definitions
   returning `0` to `src/game/weapons/BeamGun.cpp`.
3. Rebuilt and ran the full `check` target.

Measured result — **RATCHET FAIL**:

```
verify: byte 1378  linked 4508  stubs 0  missing 0  extra 2  avg 99.94
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 3  missing 0  allowed 0   imports: 207
  FAIL  byte_exact 4508 -> 1378
  FAIL  extra 0 -> 2
  FAIL  exported symbols beyond allowlist: ['_ZN11AbstractGunD0Ev',
          '_ZN11AbstractGunD2Ev', '_ZTV11AbstractGun']
RATCHET FAIL
```

The first out-of-line virtual (`isRocketGun`) becomes the vtable key function, so
clang emits the `AbstractGun` vtable and non-inline destructor from `BeamGun.cpp`;
the stripped original does not export those symbols. This changes RTTI/data layout
and then cascades through symbol order and code addresses. Per-section diff against
the original (20 differing rows) showed, among others:

```
.data.rel.ro    12264 -> 12312  (+48)
.dynsym         89248 -> 89296  (+48)
.dynstr        189208 -> 189268 (+60)
.got            17164 -> 17172  (+8)
.text         1259464 -> 1259496 (+32)
.rodata        269159 -> 269159 (same size, base shifted 0x1c9e10 -> 0x1c9f50)
```

**Proven:** the naive out-of-line fold is a regression, not an improvement. Reverted
with `git checkout -- src/game/weapons/AbstractGun.h src/game/weapons/BeamGun.cpp`;
rebuild returned to `byte 4508 / linked 4508 / data 843 / imports 207 / RATCHET PASS`.

## 5. Why the current header-only shape already matches the original symbols

The header-inline `is*` methods are emitted as weak COMDAT functions from every TU
that includes `AbstractGun.h`, exactly as clang does for inline virtuals. The linker
keeps one copy. The surviving addresses in our `.so` are identical to the original:

```
  4439: 00147d1d     4 FUNC    WEAK   DEFAULT   12 _ZN11AbstractGun12isAutoTurretEv
  4440: 00147d19     4 FUNC    WEAK   DEFAULT   12 _ZN11AbstractGun9isMineGunEv
  4594: 001568c5     4 FUNC    WEAK   DEFAULT   12 _ZN11AbstractGun11isRocketGunEv
  4595: 001568c9     4 FUNC    WEAK   DEFAULT   12 _ZN11AbstractGun9isBombGunEv
  4754: 0020e294     8 OBJECT  WEAK   DEFAULT   17 _ZTI11AbstractGun
  4755: 00203747    14 OBJECT  WEAK   DEFAULT   15 _ZTS11AbstractGun
```

`readelf -sW` output for these six lines is **byte-for-byte identical** between
`../_work/bins/android_2.0.16_libgof2hdaa.so` and our `.so` (same symbol index,
same value, same size, same binding/type/section). The original likewise exports no
`~AbstractGun`/`_ZTV11AbstractGun`.

Object-level evidence (`arm-linux-androideabi-nm -C` on the build objects) shows the
weak copies are emitted from the gun TUs, dominated by `BeamGun.cpp.obj`:

| object | `AbstractGun::is*` weak defs | `typeinfo for AbstractGun` |
|---|---:|---:|
| `BeamGun.cpp.obj` | 4 | 1 |
| `BombGun.cpp.obj` | 2 | 0 |
| `MineGun.cpp.obj` | 3 | 0 |
| `ObjectGun.cpp.obj` | 4 | 1 |
| `RocketGun.cpp.obj` | 3 | 0 |
| `SentryGun.cpp.obj` | 4 | 0 |
| `SpriteGun.cpp.obj` | 4 | 1 |

The linker's one-copy selection is what matters; the dump's ownership witness
(`BeamGun.cpp`) is compatible with this and is the only readable source-level
witness we have.

## 6. Proven vs hypothesis

**Proven by controlled rebuild:**

1. Current tree baseline is the documented gate: byte 4508/4524, linked 4508/4524,
   size 4524/4524, data 843/843, imports 207, lint CLEAN, drift 0.
2. Re-adding an empty `AbstractGun.cpp` before `BeamGun.cpp` changes no ELF section
   except `.note.gnu.build-id` and does not move any gate count.
3. Moving the four `is*` bodies out of line into `BeamGun.cpp` regresses
   byte_exact 4508 -> 1378, adds two `extra` verify symbols and three exported
   symbols beyond the allowlist, and grows `.data.rel.ro` by 48 bytes.

**Proven from the binary/dumps:**

4. The HD source dump assigns the four `AbstractGun::is*` bodies and the
   `AbstractGun` typeinfo/typeinfo-name to `Classes/Game/BeamGun.cpp`, and the HD
   file list has no `AbstractGun.cpp`.
5. Our final `.dynsym` entries for `AbstractGun` are address-for-address identical
   to the original; the original exports no `~AbstractGun` or `_ZTV11AbstractGun`.

**Hypothesis / unprovable from the stripped binary:**

6. Because the original `.so` is stripped (ICO/linker GC), we cannot directly prove
   *which* object originally emitted the weak vtable/typeinfo or weak inline copies.
   The HD dump plus the matching exported symbol set is the strongest available
   evidence. The current header-only implementation is consistent with it and is
   already byte-optimal among the three tested shapes.

## 7. Reproduction notes

* Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (mode 444; use
  `objcopy --dump-section` into a new file rather than copying the `.so`).
* Section-diff helper used here compares `readelf -SW` `sh_addr`/`sh_size` and
  `objcopy --dump-section` bytes per section; a plain file `cmp` is misleading
  because the build-id/timestamps differ on every relink.
* Toolchain prefix:
  `.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-`.
* No `--update-baseline`, no source change, no linker-flag change was made for this
  study. Worktree is left clean.
