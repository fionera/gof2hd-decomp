# `.dynsym` app-region order (w3447)

Status: research note + one landed source fix (2026-11-01). Worktree `wt/w3447`, branch
`wave87-w3447`, based on master `78e994de`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with this tree's NDK r18b gold
(`GNU gold (GNU Binutils 2.27.0.20170315) 1.12`; the original's `.note.gnu.gold-version` is
`gold 1.12`).

## TL;DR

* The `.dynsym` **app region** is the first **4935** defined symbols (indices `0..4934`); the
  archive tail starts at index `4935` (`adler32`). Both binaries have exactly **5370** defined
  symbols.
* The app-region order is **not** a hash artifact. It is exactly the **first-occurrence order of
  global symbol names** across `crtbegin_so.o` + the 204 `gof2` objects in link order, restricted
  to symbols that are defined in the final `.dynsym`. Within one object the global symbol table is
  sorted **lexicographically by mangled name** (clang 7.0.2 MC), so source declaration order inside
  a TU does not matter; **which object first mentions the name** does.
* This model was verified by simulation: replaying the first-occurrence order over the current
  link reproduces **4935/4935** app-region symbols in exactly our `.dynsym` order.
* Baseline at this HEAD: **byte 4253, linked 4496, size 4524/4524, lint CLEAN, drift 0**.
  `.dynsym` alignment: **5352 matched, ratio 0.996648, 14 gaps** (difflib `SequenceMatcher`,
  `autojunk=False`). Only **9 app-region symbols** were out of order, in 4 groups.
* One fix is landed: moving `BSphere::Merge(const Transform&)` from `BSphere.cpp` to `Vector.cpp`
  fixes the 3-symbol Matrix/BSphere group with **no gate change**:
  `byte 4253`, `linked 4496`, `size 4524/4524`, lint `CLEAN`, drift `0`;
  dynsym **5352 -> 5355 matched, ratio 0.996648 -> 0.997207, gaps 14 -> 11**.
* Three further experiments fixed more dynsym symbols but regressed `byte_exact`; they are
  recorded below and were **not** landed.

## Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4253  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`cmake --build cmake-build-match --target lint` -> `CLEAN ✓`; `... --target drift` ->
`0 field(s) across 0 class(es)`. (The task brief's `byte 4182 / linked 4495` is stale for this
HEAD.)

## 1. Model: first-occurrence order, not gold's hash order

Gold's `Symbol_table` is `std::unordered_map<(name_key, version_key), Symbol*>`, but the table is
constructed with `command_line.number_of_input_files() * 1024` buckets (gold `main.cc`), which is
far more than the 5578 dynamic symbols. With no bucket collisions, iteration order is insertion
order. `add_from_relobj` calls `namepool_.add_with_length()` for **every global symbol of every
object, defined or undefined**, so the insertion order is the first time a name is *mentioned* in
the input stream. `Sized_relobj_file::do_add_symbols` walks the object's global symbols in ELF
symtab order; clang 7.0.2's MC emits that table sorted lexicographically by mangled name.

Evidence for the lexicographic order (one object, `readelf -sW`):

```
$OD -sW cmake-build-match/exidx_markers/objects/o018_Vector.cpp.obj | grep GLOBAL
  ... _ZN11AbyssEngine6AEMath10VectorLerpERKNS0_6VectorES3_f
  ... _ZN11AbyssEngine6AEMath11VectorCrossERKNS0_6VectorES3_
  ... _ZN11AbyssEngine6AEMath12VectorLengthERKNS0_6VectorE
  ... _ZN11AbyssEngine6AEMath13VectorIsEqualERKNS0_6VectorES3_
  ... _ZN11AbyssEngine6AEMath15VectorNormalizeERKNS0_6VectorE
  ... _ZN11AbyssEngine6AEMath6VectoraSERKS1_
  ... _ZN11AbyssEngine6AEMath6VectorcvPfEv
  ...
```

`10VectorLerp < 11VectorCross < 12VectorLength < 13VectorIsEqual < 15VectorNormalize <
6Vector... < 9VectorDot < dv...` is exactly ASCII order on the mangled names.

### Simulation (proves the model)

Script used (self-contained; `symsim.py` is the ELF32 `.symtab` reader):

```python
# symsim.py: read global symbols (sh_info..end) of an ELF32 object in file order
import struct
from pathlib import Path
def sections(data):
    e_shoff=struct.unpack_from("<I",data,0x20)[0]
    e_shentsize=struct.unpack_from("<H",data,0x2E)[0]
    e_shnum=struct.unpack_from("<H",data,0x30)[0]
    out=[]
    for i in range(e_shnum):
        o=e_shoff+i*e_shentsize
        name,typ,flags,addr,off,size,link,info,align,ent=struct.unpack_from("<IIIIIIIIII",data,o)
        out.append(dict(typ=typ,off=off,size=size,link=link,info=info,ent=ent))
    return out
def symtab_globals(path):
    data=Path(path).read_bytes(); secs=sections(data)
    st=next(s for s in secs if s["typ"]==2)
    strs=secs[st["link"]]; strtab=data[strs["off"]:strs["off"]+strs["size"]]
    def cstr(i):
        e=strtab.find(b"\0",i); return strtab[i:e].decode("utf-8","replace")
    out=[]; ent=st["ent"] or 16
    for i in range(st["info"], st["size"]//ent):
        o=st["off"]+i*ent
        nm,val,sz,info,other,shndx=struct.unpack_from("<IIIBBH",data,o)
        out.append((cstr(nm), info>>4, info&0xf, shndx))
    return out

# replay first occurrence over crtbegin + link_order.rsp
objs=["cmake-build-match/crtbegin_so_renamed.o"]
objs += [l.strip() for l in open("cmake-build-match/exidx_markers/link_order.rsp") if l.strip()]
keys={}; order=[]
for o in objs:
    for name,bind,typ,shndx in symtab_globals(o):
        if name not in keys:
            keys[name]=len(keys)+1; order.append(name)
ours=[n for l in open("/tmp/ours_dynsym.tsv")
      for b,t,sh,v,s,n in [l.rstrip("\n").split("\t")] if int(sh)!=0]
sim=[n for n in order if n in set(ours)]
print(len(sim), sim[:4935] == ours[:4935])   # -> 4935 True
```

The first 4935 simulated names are **identical, in order**, to the first 4935 defined symbols of
our `.dynsym`. The archive tail is not covered by this replay (it comes from the archive member
selection order), which is why only the app region is compared.

### Consequence

A symbol moves in `.dynsym` only when the **first object that mentions it** changes. Reordering
declarations inside a TU does nothing (the object symtab is lexicographic). Moving a definition to
another TU changes the order only if the new TU is the first mention; if an earlier object already
mentions the name as an undefined reference, the definition move is invisible to `.dynsym`.

## 2. Baseline alignment

```
defined 5370 5370
ratio 0.996648 blocks 16 matched 5352
gaps 14
```

App-region gaps (indices are defined-symbol indices):

| gap | original symbols | our first-mention object | diagnosis |
|---|---|---|---|
| o664-665 / o688 | `MatrixRotateVector`, `MatrixTransformVector` | `BSphere.cpp` | original `Vector.cpp` mentions them |
| o673 | `BSphere::Merge(Transform)` | `BSphere.cpp` | original `Vector.cpp` defines it |
| o1827-1828 | `RecordHandler::recordStoreWrite`, `recordStoreWritePreview` | `NdkAutosave.cpp` | original `NdkHooks.cpp` mentions them |
| o1891-1893 | `curTouchSize`, `maxTouchSize`, `touches` | `LODManager.cpp` (late) | original first mentions them before `ArrayRemove<AEGeometry*>` |
| o3879 | `ResourceTexture::ResourceTexture(String const&, float)` | `ResourceMaterial.cpp` | original `GameResource` TU emits it before the `ResourceMaterial` ctors |

The remaining 8 gaps are in the archive tail (`libzip` `_zip_err_*`, OpenSSL `sha256_*`/`_edata`
tail) and are outside the app region.

## 3. Experiment A (landed): `BSphere::Merge(Transform)` -> `Vector.cpp`

### Change

`src/engine/math/BSphere.cpp`: remove `BSphere::Merge(const Transform&)` (and its now-unused
`CopyVector` helper / Matrix forward declarations).
`src/engine/math/Vector.cpp`: add the same body (plus `BSphere.h`/`Matrix.h`/`Transform.h`
includes and the two Matrix forward declarations). The body is unchanged, so the `.text` bytes are
unchanged; only the owning TU / first-mention order changes.

### Why it works

The original `.text` already places `BSphere::Merge(Transform)` at `0x73f31`, inside the
`Vector.cpp` region (`VectorIsEqual` `0x73dcf` .. `VectorLerp` `0x73eed` .. `Merge(Transform)`
`0x73f31` .. `Merge(BSphere)` `0x74001`). Moving the definition into `Vector.cpp` makes
`Vector.obj` mention `MatrixRotateVector`, `MatrixTransformVector` and
`Merge(Transform)`; because the object symtab is lexicographic, they land exactly between
`13VectorIsEqual` and `6VectorcvPfEv` / between `6VectormLEf` and `dv...`, matching the original
`o664-665` and `o673`.

### Measured

| metric | before | after |
|---|---:|---:|
| byte_exact | 4253 | **4253** |
| linked_exact | 4496 | **4496** |
| size exact | 4524/4524 | **4524/4524** |
| lint | CLEAN | **CLEAN** |
| drift | 0 | **0** |
| dynsym matched | 5352 | **5355** |
| dynsym ratio | 0.996648 | **0.997207** |
| dynsym gaps | 14 | **11** |
| app-region out-of-order | 9 | **6** (4 groups -> 3 groups) |

The 3 fixed symbols are `MatrixRotateVector`, `MatrixTransformVector`, `BSphere::Merge(Transform)`.
`.text`/`.rodata`/`.data`/`.bss`/`.dynsym`/`.plt` sizes and bases are unchanged.

## 4. Experiment B (not landed): merge `NdkAutosave.cpp` into `NdkHooks.cpp`

### Observation

The original `.dynsym` has `ndk_autosave` at `o1880`, between `ndkEntrance` (`o1879`) and
`pConstToNonConst` (`o1881`) — i.e. inside the lexicographic `NdkHooks.cpp` block — and
`recordStoreWrite`/`recordStoreWritePreview` at `o1827-1828`, between `saveOptions` and
`RecordHandler::C1`. That is exactly the lexicographic order of a single `NdkHooks.obj` that
defines `ndk_autosave` and mentions `recordStoreWrite`. Our tree has a separate
`NdkAutosave.cpp`, so those names are first mentioned at `o1889-1890` (after `zip_open`).

### Change tested

Move the `ndk_autosave` body from `src/platform/android/NdkAutosave.cpp` into
`src/platform/android/NdkHooks.cpp` (append at end) and delete `NdkAutosave.cpp`.

### Measured

| metric | baseline | merge |
|---|---:|---:|
| byte_exact | 4253 | **4179 (-74)** |
| linked_exact | 4496 | 4496 |
| size exact | 4524/4524 | 4524/4524 |
| dynsym matched | 5352 | **5357** |
| dynsym ratio | 0.996648 | **0.997579** |
| dynsym gaps | 14 | **10** |

The 2 fixed symbols are `recordStoreWrite` and `recordStoreWritePreview`. The regression is a
**PLT-order** effect, not a codegen change: `recordStoreWrite@plt` moves from index **1017**
(original and baseline) to **1009**, and 74 functions that call it become instruction-identical but
not byte-identical. The original PLT sequence around the block is:

```
1007 ndk_autosave@plt
1012 checkFirstCreditPackBoughtWriteAction@plt
1014 RecordHandler::C1@plt
1015 RecordHandler::saveOptions@plt
1017 RecordHandler::recordStoreWrite@plt
1018 RecordHandler::recordStoreWritePreview@plt
```

Baseline reproduces this exactly. In the merged object, clang emits `ndk_autosave`'s section at its
first call site (inside `ndk_checkPlaytimeAndSpendOfferwallCredits`, section 44), before
`checkFirst...` (section 79), so the `recordStoreWrite` relocation is scanned earlier and the PLT
slot moves. The dynsym order is right but the PLT order is wrong; the merge is not a strict
improvement.

**Hypothesis (not proven):** the original `NdkHooks.cpp` has `ndk_autosave` in the same TU but its
body section is emitted after `checkFirst...`; a source arrangement that keeps the first call
before `checkFirst` while emitting the body after it would satisfy both observables. No such
arrangement was found in this pass.

## 5. Experiment C (not landed): `loadPortraits` -> `BuildResourceList.cpp`

### Observation

The original `.dynsym` has `ResourceTexture::ResourceTexture(String const&, float)` at `o3879`,
before the two `ResourceMaterial` ctors at `o3880-3881`. `tu_mapping.md` says
`BuildResourceList.cpp` is the original `GameResource.cpp` and that `GameResource.cpp` also
contained `loadPortraits` and the `ResourceTexture` ctor. Our `loadPortraits` lives in
`ResourceMaterial.cpp`, so the ctor is first mentioned there, after the `ResourceMaterial` ctors.

### Change tested

Move `loadPortraits` from `src/engine/render/ResourceMaterial.cpp` to the **end** of
`src/engine/render/BuildResourceList.cpp` (add `game/core/String.h`).

### Measured

| metric | baseline | move |
|---|---:|---:|
| byte_exact | 4253 | **4251 (-2)** |
| linked_exact | 4496 | 4496 |
| size exact | 4524/4524 | 4524/4524 |
| dynsym matched | 5352 | **5356** |
| dynsym ratio | 0.996648 | **0.997393** |
| dynsym gaps | 14 | **9** |

The fixed symbol is `ResourceTexture(String const&, float)`. The PLT order now matches the original
(`ResourceTexture(char*)` 2527, `ResourceMaterial(tt)` 2528, `ResourceMaterial(t)` 2530,
`loadPortraits` 2533, `ResourceTexture(String)` 2536). The remaining -2 is a **literal-pool /
`.rodata` string-order** difference inside `loadPortraits` and `loadLowTexturesAndMaterials`: the
functions are instruction-identical, but their literal words point at different `.rodata` strings
(e.g. `loadPortraits+0x47` orig `0x000d22eb` vs ours `0x000d09d4`). Placing `loadPortraits`
*before* `BuildResourceList` was worse (`byte 4248`), so the at-end placement is the better of the
two but still not strict.

**Hypothesis (not proven):** the original `GameResource.cpp` also contained
`loadLowTexturesAndMaterials` (`LowResourceTable.cpp` is the other split), and the exact
`.rodata` string order is only reproduced after that TU is merged and its function order is
matched. This is a `.rodata`-order task, not a dynsym-order task.

## 6. Experiment D (not landed): move the touch globals to `NdkAutosave.cpp`

### Observation

The original `.dynsym` places `curTouchSize`/`maxTouchSize`/`touches` at `o1891-1893`,
between `zip_open` (`o1890`, first mentioned by `NdkHooks.cpp`) and
`ArrayRemove<AEGeometry*>` (`o1894`, first mentioned by `LODManager.cpp`). The original `.bss`
places them at `0x217b90..0x217b98`, immediately after the `NdkHooks`/`VirtualInput` globals and
before `PlayerEgo::crosshairPos`; ours are at `0x218c20..0x218c28`, much later. Both observables
point at the object between `NdkHooks.cpp` and `LODManager.cpp` — i.e. `NdkAutosave.cpp` — not
`LODManager.cpp`.

### Change tested

Move the three definitions from `src/engine/render/LODManager.cpp` to
`src/platform/android/NdkAutosave.cpp` (with `extern` declarations left in `LODManager.cpp`).

### Measured

| metric | baseline | move |
|---|---:|---:|
| byte_exact | 4253 | **4242 (-11)** |
| linked_exact | 4496 | 4496 |
| size exact | 4524/4524 | 4524/4524 |
| dynsym matched | 5352 | **5358** |
| dynsym ratio | 0.996648 | **0.997765** |
| dynsym gaps | 14 | **10** |

The 3 fixed symbols are `curTouchSize`, `maxTouchSize`, `touches`. The `.bss` addresses did **not**
move (`0x218c20/24/28` before and after), so the original `.bss` order is still unreproduced; the
-11 is a GOT-slot-order effect (the lost functions are instruction-identical but their literal
words point at different `.got` slots): `GetTouchCount`, `RemoveTouches`, `AddTouch`, `GetTouch`,
`MeshMerger::D1/D2`, `LodMeshMerger::D1/D2`, `RecordHandler::addHashToOptions`,
`RecordHandler::addHash`, `ndk_resetNativeItemInformationList`.

**Hypothesis (not proven):** the touch globals belong to `NdkAutosave.cpp`; landing this needs the
`.bss` input order fixed as well (the original `.bss` order is
`sound_off, gEngine, rotateAccelValues, APKArchive, ZIPArchive, hrwhs_country_code, gRealWidth,
gRealHeight, rootDirectory, ZIPDirectory, forceExit, stateChanged, currentstate,
gb_android_offerwallCreditAmount, touches, maxTouchSize, curTouchSize, ...`, while ours is a
different permutation).

## 7. Remaining app-region work

After the landed Experiment A the app region still has 6 out-of-order symbols:

1. `RecordHandler::recordStoreWrite` / `recordStoreWritePreview` (2) — Experiment B fixes dynsym
   but costs 74 byte_exact via the PLT.
2. `curTouchSize` / `maxTouchSize` / `touches` (3) — Experiment D (move to `NdkAutosave.cpp`)
   fixes the dynsym position but costs 11 byte_exact via GOT-slot order and does not move the
   `.bss` addresses. The original `.bss` order is `touches`, `maxTouchSize`, `curTouchSize`
   (ascending address), reverse of our declaration order; the dynsym order is `curTouchSize`,
   `maxTouchSize`, `touches` in both.
3. `ResourceTexture::ResourceTexture(String const&, float)` (1) — Experiment C fixes dynsym but
   costs 2 byte_exact via `.rodata` string order.

Archive-region gaps (out of scope here): `libzip` `_zip_err_str` / `_zip_err_type` /
`_zip_nerr_str` (archive-member order) and the OpenSSL tail
`_edata`/`_end`/`__bss_start`/`__cxa_begin_cleanup`/`__cxa_call_unexpected` vs
`sha256_block_data_order_neon` (script-symbol vs archive-member insertion order).

## 8. Reproduction

```bash
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8

# .dynsym dump (ELF32, SHT_DYNSYM=11, names via sh_link)
python3 /tmp/dynsym_dump.py ../_work/bins/android_2.0.16_libgof2hdaa.so > /tmp/orig_dynsym.tsv
python3 /tmp/dynsym_dump.py cmake-build-match/libgof2hdaa.so        > /tmp/ours_dynsym.tsv

# alignment (defined symbols only)
python3 - <<'PY'
import difflib
def defined(p):
    return [l.rstrip("\n").split("\t")[5] for l in open(p)
            if int(l.rstrip("\n").split("\t")[2]) != 0]
o=defined("/tmp/orig_dynsym.tsv"); u=defined("/tmp/ours_dynsym.tsv")
sm=difflib.SequenceMatcher(None,o,u,autojunk=False)
print(sm.ratio(), len(sm.get_matching_blocks()),
      sum(b.size for b in sm.get_matching_blocks()))
PY

# PLT sequence
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .plt ORIG | grep -o "<[^>]*@plt>:" | sed 's/[<>:]//g' > /tmp/plt_o.txt
$OD -d -j .plt cmake-build-match/libgof2hdaa.so | grep -o "<[^>]*@plt>:" | sed 's/[<>:]//g' > /tmp/plt_u.txt
```

`/tmp/dynsym_dump.py` is the standard ELF32 parser used by the other dynsym notes: it prints
`bind<TAB>type<TAB>shndx<TAB>value<TAB>size<TAB>name` for every `.dynsym` entry.

## 9. Conclusion

* **Proven:** the app-region `.dynsym` order is the first-occurrence order of global names over the
  link inputs; each object's contribution is lexicographically sorted by mangled name. The
  simulation reproduces all 4935 app-region symbols exactly.
* **Proven:** moving `BSphere::Merge(Transform)` into `Vector.cpp` fixes the Matrix/BSphere group
  with zero gate cost; this is landed.
* **Measured, not landed:** merging `NdkAutosave.cpp` into `NdkHooks.cpp` fixes the RecordHandler
  group in dynsym but costs 74 byte_exact because the `recordStoreWrite` PLT slot moves.
* **Measured, not landed:** moving `loadPortraits` into `BuildResourceList.cpp` fixes the
  ResourceTexture group in dynsym but costs 2 byte_exact because of `.rodata` string order.
* The remaining app-region dynsym work is therefore coupled to PLT order (RecordHandler) and
  `.rodata` order (ResourceTexture); it should not be landed without also fixing those.
