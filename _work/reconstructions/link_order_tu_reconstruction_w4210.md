# Link-order / TU reconstruction: removing the section-ordering file (w4210)

Status: landed source + build-system change, 2026-11-03. Worktree `wt/w4210`, branch
`wave92-w4210`, base `b379d00d`. Scratch work is under `/dev/shm/w4210`. This note extends
`link_order_tu_reconstruction_w4120.md` (which proved the original was linked without a
section-ordering file and that the object link order is already the original TU order).

## 0. Bottom line

The canonical build no longer uses a `--section-ordering-file`. A plain gold input-order link
now places **all 4,524 exported functions at their exact original addresses**:

```
verify: byte 4498  linked 4498  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section layout (plain link, no ordering file):

```
.text       0x1337c8 @ 0x69440     exact
.rodata     0x041b67 @ 0x1c9e10    exact (0 differing bytes)
.ARM.exidx  0x006f70 @ 0x19cc08    exact (3566 entries, 272 CANT, 194 duplicate targets)
.ARM.extab  0x026290 @ 0x1a3b78    original 0x26298, known -8 (six LSDA records)
.data       0x00296c @ 0x214000    exact
.bss        0x0069ac @ 0x216970    exact
.got        0x00430c @ 0x20fcf4    exact
```

The `.ARM.extab` -8 is the same residual the ordering-file build already had: it belongs to the
26 non-byte-exact function bodies, not to layout. Every exported function address matches the
original (`nm -nD` comparison: 4524/4524 common symbols at the same address).

## 1. Before / after

| build | byte_exact | linked_exact | .text | .ARM.exidx | .ARM.extab | exported funcs at original address |
|---|---:|---:|---:|---:|---:|---:|
| w4120 baseline, ordering file ON | 4498 | 4498 | `0x1337c8` | `0x6f70` | `0x26290` | 4524 / 4524 |
| w4120 baseline, plain link OFF | 3036 | 4498 | `0x1337e8` | `0x6f70` | `0x26290` | 3925 / 4524 |
| **w4210 plain link** | **4498** | **4498** | `0x1337c8` | `0x6f70` | `0x26290` | **4524 / 4524** |

The OFF progression measured during the pass:

```
3036  plain link, original source order
3074  Ship accessor block reordered to the original order
4493  all seven out-of-order TUs reordered (5 functions still shifted by the
      ParticleSystemManager template-instantiation position)
4498  ParticleSystemManager release/releaseSprites moved after reset
4498  BSphere merged into Vector (byte count unchanged; .ARM.exidx becomes exact)
```

## 2. Why the source order, not the address order, is the authority

The iOS 1.1.19 source-layout dump (`../source file list dump/gof2hd_1.1.19_ios.cpp`) lists each
object's symbols in **object symbol-table order**, which is the original source/emission order.
For every shared v1 function the Android 2.0.16 address order matches that dump order; the v2
functions are inserted at plausible points. Example, `Ship.cpp`:

```
iOS dump order (v1):  Ship::Ship, refreshValue, ~Ship, setRace, getRace, getIndex, getPrice,
                      getHandlingForShop, getHandling, getBaseHP, getCombinedHP, getMaxArmorHP,
                      getMaxHP, getMaxShieldHP, getShieldRegen, getBoostSpeed, ...
Android address:      Ship::Ship, refreshValue, ~Ship, setRace, getRace, getIndex, getPrice,
                      setCurrentWeaponSlot, getCurrentWeaponSlot, getHandlingForShop, getHandling,
                      getBaseHP, getCombinedHP, getMaxArmorHP, getMaxHP, getMaxShieldHP, ...
```

So the original compiler emitted functions in **definition order**. Our clang 7.0.2 does not:
it creates the `llvm::Function` object at the **first use** (a call or the definition), and the
object's position in the module list is creation order. A callee referenced before its
definition is therefore hoisted immediately after the first caller. This is the "callee-hoist"
blocker from w4120. It is a frontend/Module-order property, not a target or optimization flag
(tested at `-O0`/`-Oz`, all targets, `-femit-all-decls`, `-fno-inline`, ...).

The fix is not to fight clang; it is to choose the definition order so that the hoists land on
the original addresses. The emitted order is:

```
for each definition D in source order:
    if D not yet created: create D
    emit D's body; every not-yet-created callee C is created immediately after D
```

The body-emission point is the definition position; the address of a function is its creation
point. This lets a function be *created* early (by an earlier caller) while its *body* is emitted
later, which is exactly what the original source order relies on.

## 3. The seven out-of-order TUs

w4120 measured these exported-function inversion counts (pairs out of original order):

```
660 / 48 funcs   Agent.cpp
310 / 93         Ship.cpp
138 / 27         BluePrint.cpp
 17 / 11         LodMeshMerger.cpp
 11 / 17         PlayerGasCloud.cpp
  6 /  6         ParticleSettings.cpp
  4 / 40         ParticleSystemManager.cpp
```

After this pass every one of the seven has **0 inversions** in its strong exported functions.
The reorder is purely a block move: the multiset of non-blank source lines is unchanged in every
file (verified with `sort | md5sum`), and no anchor, `volatile`, `asm`, attribute or linker trick
was added.

### 3.1 Per-TU definition orders

The general recipe is:

1. Sort the exported function blocks by the original Android address (the iOS dump order is the
   same for v1 functions; use it to resolve overloads and to place v2 additions).
2. For every function that is called by an earlier function, move its **definition** later so
   that its first use is not the hoist point, while ensuring an earlier caller still creates its
   `llvm::Function` at the target address.
3. Rebuild and compare the object's `.text.*` section order with `nm -nD` on the original.

The concrete moves:

* **Ship.cpp** (the only TU with three hoists):
  * Keep `getCargoValue` before `refreshValue`. The constructor calls `refreshValue`, so clang
    hoists it to slot 2 (its original address). If `refreshValue` were placed before
    `getCargoValue`, clang would also hoist `getCargoValue` to slot 3, which the original does
    not have.
  * `removeCargo(int,int)` is defined **after** `setCargo`, but `removeCargo(Item*)` tail-calls
    it, so its `llvm::Function` is created at the original `removeCargo(int,int)` address. Its
    body is emitted later, so its call to `setCargo` creates `setCargo` at the original
    `setCargo` address (after `removeCargo(int)`), not after `removeCargo(int,int)`.
  * `clone` is defined **after** `setPrice`, but `makeShip` calls it, so `clone` is created at
    its original address. Its body is emitted later, so `addMod` is created after `setPrice`
    (its original address), not after `clone`.
  * `setEquipment(Array<Item*>*)` must precede `setEquipment(Item*)` (the overload reorder tool
    cannot tell them apart; this was fixed by hand).
* **PlayerGasCloud.cpp**: original order is `setSparkInSight, isSparkAlive, getSparks,
  getPosition, setPosition, translate, explode, ...`; the v2 `hasExploded` and the two
  `ArrayAdd` copies follow `explode`. No hoist adjustment needed.
* **BluePrint.cpp / Agent.cpp / LodMeshMerger.cpp / ParticleSettings.cpp**: pure block reorder to
  the original address order; 0 inversions.
* **ParticleSystemManager.cpp**:
  * `release` and `releaseSprites` are defined **after** `reset`. The destructor calls `release`
    and `cameraToggle` calls `releaseSprites`, so both are created at their original addresses
    (before `render3d`). Their bodies are emitted after `reset`, which makes the implicit
    `ArrayReleaseClasses<ParticleSystemMesh*>` / `<ParticleSystemSprite*>` instantiations appear
    after `reset`, exactly as in the original. (Our clang emits implicit template
    instantiations at first use, so the body-emission position is what controls them.)
  * The initial reorder left one inversion (`releaseSprites` before `cameraToggle`); moving
    `release`/`releaseSprites` after `reset` fixed it.

## 4. TU membership: `BSphere` belongs to `Vector.cpp`

The SD dump attributes `BSphere::Merge(const BSphere&)` and `BSphere::operator=` to
`Classes/Game/Vector.cpp`, not a separate `BSphere.cpp`. Our tree had a separate
`src/engine/math/BSphere.cpp`, which produced an extra empty-`.text` `CANTUNWIND` sentinel:
plain-link `.ARM.exidx` was 3567 entries / 273 CANT / 0x6f78 (8 bytes larger than the original),
with the extra sentinel at `BSphere.cpp`'s empty `.text` address (`0x74000`).

Landed: moved the two `BSphere` definitions into `Vector.cpp` (next to the existing
`BSphere::Merge(const Transform&)`) and deleted `BSphere.cpp`. Result:

```
.ARM.exidx  0x6f70 exact (3566 entries, 272 CANT, 194 duplicate targets)
.ARM.extab  0x26290 @ 0x1a3b78 (original 0x26298, unchanged -8)
```

This merge was previously reported to cost 456 byte_exact under the ordering-file build
(w3613). It still does: the ordering file places local functions from the object's source order,
and merging changes that order. With the ordering file removed, the plain link gives 4498, so
the merge is now the correct move.

## 5. Build-system change

`CMakeLists.txt` no longer generates or consumes a section-ordering file:

* removed `option(GOF2_MATCH_LAYOUT ...)`, the `gen_section_order.py` call, the
  `gen_exidx_markers.py` marker renaming, the `crtbegin_so.o` `.text -> .text.crtbegin` rename,
  and the `--section-ordering-file` argument;
* kept `-ffunction-sections -funwind-tables` (the original used them; the 193 empty-`.text`
  sentinels prove it);
* kept `tools/gen_link_order.py` (the object link order is still the original TU order);
* the link is a plain gold link with the original's flags: `-shared -soname --build-id -s -z now
  --exclude-libs=libgcc.a`, crtbegin, objects, whole-archive zlib/libzip, the original `-l`
  libs, FMOD, libcrypto, libc++abi, libandroid_support, libgcc, crtend.

`tools/gen_section_order.py` and `tools/gen_exidx_markers.py` remain in the tree as historical
analysis tools but are no longer on the build path.

## 6. `--unresolved-symbols=ignore-all` and `tools/sodiff_allow.txt`

* The link line has **no** `--unresolved-symbols=ignore-all`; a shared-library link allows the
  original's app-provided imports by default. The import set is 207 and ratchet-pinned.
* `tools/sodiff_allow.txt` is **empty** (comments only). `sodiff` reports
  `extra-beyond-allow 0  missing 0  allowed 0`.

Neither is needed.

## 7. Remaining work

* **26 non-byte-exact bodies.** The plain link now has the same 26 body/register-allocation
  deltas as the ordering-file build (`Layout::Layout`, `LevelScript::process`,
  `Status::missionCompleted`, `Generator::createMission`, `Radar::draw`, `Level::createShip`,
  `MGame::OnTouchEnd`, `Globals::Globals`, `HangarWindow::render`, `Gun::calcCharacterCollision`,
  ...). They are the only source of the `.ARM.extab` -8.
* **`.dynsym`/`.dynstr`/`.hash` order** still differs (324 / 1268 / 133 bytes) because gold's
  symbol hash iteration follows the complete insertion sequence; it converges only as the
  remaining bodies/TU contents converge.
* The two historical tools (`gen_section_order.py`, `gen_exidx_markers.py`) can be deleted once
  no analysis note depends on them.

## 8. Reproduction

```sh
cmake -S . -B cmake-build-plain -G Ninja \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs \
  -DGOF2_MATCH_OPT=-Oz
cmake --build cmake-build-plain --target check -j8
cmake --build cmake-build-plain --target drift -j8
```

The per-TU reorder/hoist probes are in `/dev/shm/w4210` during the pass; the durable evidence is
the iOS dump order above plus the address/inversion counts in this note.
