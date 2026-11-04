# Per-TU data / rodata / layout build flags (w5208)

Status: research note, 2026-11-04. Worktree `wt/w5208`, branch `wave104-w5208`,
base commit `8c5fd3646` ("Hud::draw pass 2: dock posY top*2 fix + extab-exact
BHTWPD order"). Scratch lives in `/dev/shm/w5208`; the canonical
`cmake-build-match` was never modified and `--update-baseline` was never run.

**Bottom line.** No app TU **observably** requires a non-global
data/rodata/layout flag. The single canonical flag set already reproduces
`.rodata`, `.data`, `.data.rel.ro`, `.bss` size, `.init_array` and
`.fini_array` byte-for-byte. Every one of the 14 tested flags is either
(a) completely object-identical on all 192 app TUs (unobservable), or
(b) observable on some TUs and, for every such TU, **link-neutral or worse**
— never better. No per-TU variant has `byte_exact > 4500` or
`linked_exact > 4500`; the best observable variants tie the baseline
(`4500/4500`) while changing data bytes or non-exact function bytes. There is
**no strict improvement**.

## 0. Baseline (measured before and after)

```sh
cd /home/debian/GalaxyOnFire2/wt/w5208
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```text
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`size-exact` 4524/4524, lint `CLEAN`, drift 0. Our stripped `.so` sha256
`809aab4e633964764d8550ebd89a78d7a86d9775bfaeae731a29ba623123487a`;
the original is
`d4c17be61af96272e240de3d4ebf1340c581f9a8bc455b9defad72776bed9483`
(2,187,328 bytes).

Baseline section measurements (direct byte comparison against the original at
the same base; this is the reference every variant below is compared with):

| section | size | base | differing bytes |
|---|---:|---:|---:|
| `.text` | `0x1337c8` | `0x69440` | **100,113** |
| `.ARM.exidx` | `0x6f70` | `0x19cc08` | **829** |
| `.ARM.extab` | `0x26298` | `0x1a3b78` | **108,295** |
| `.rodata` | `0x41b67` | `0x1c9e10` | **0** |
| `.data` | `0x296c` | `0x214000` | **0** |
| `.data.rel.ro` | `0x2fe8` | `0x20cbb0` | **0** |
| `.bss` | `0x69ac` (NOBITS) | — | size-exact |
| `.init_array` | `0xc` (3 words) | `0x20fb98` | **0** |
| `.fini_array` | `0x8` (2 words) | `0x20cba0` | **0** |
| `.dynsym` | `0x15ca0` | `0x1f0` | **0** |
| `.dynstr` | `0x2e318` | `0x15e90` | **0** |
| `.hash` | `0x973c` | `0x441a8` | **0** |
| `.rel.dyn` | `0x7048` | `0x504d4` | **0** |
| `.rel.plt` | `0x72d0` | `0x5751c` | **0** |
| `.note.gnu.build-id` | `0x24` | — | **20** |

`.init_array` words are `0x69441, 0x694c1, 0x694dd`; `.fini_array` words are
`0x69548, 0x69534` (identical to the original). The `.rodata` printable-string
pool is **2,779 strings / 203,624 bytes**, sha256
`832c9ce86c28b7832fc1a7fa3c615006ba0db08448b86a58705355273e2145d7`.

> Note: `docs/VALIDATION.md` / `docs/IDENTICALITY.md` still state `.text`
> 102,777 differing bytes and list only `.text`, `.ARM.exidx` and the build-id
> as differing. At this base the measured values are `.text` 100,113,
> `.ARM.exidx` 829, `.ARM.extab` 108,295 and build-id 20. The docs are stale
> relative to `8c5fd3646`; this pass does not change those numbers.

Baseline per-TU exact counts (raw function bytes, 190 app TUs + archive):

```text
app TUs 190   app functions 4282   app exact 4258
archive functions 242   archive exact 242
TUs not fully exact (exact/total):
  game/ui/MenuTouchWindow.cpp 38/41     game/world/Level.cpp 106/109
  game/core/Globals.cpp 39/41           game/menu/MGame.cpp 43/45
  game/menu/ModStation.cpp 38/40        game/ui/Layout.cpp 48/50
  game/menu/SpaceLounge.cpp 30/31       game/mission/Generator.cpp 21/22
  game/mission/Status.cpp 137/138       game/ship/PlayerEgo.cpp 173/174
  game/ui/HangarWindow.cpp 31/32        game/ui/Hud.cpp 50/51
  game/weapons/Gun.cpp 27/28            game/weapons/Radar.cpp 18/19
  game/weapons/RocketGun.cpp 10/11      game/world/LevelScript.cpp 16/17
```

## 1. Method

Two independent layers, both against the canonical build:

1. **Global matrix.** Each flag is appended to the canonical `CMAKE_CXX_FLAGS`
   and `CMAKE_C_FLAGS` for a whole-tree build. This measures the flag's
   observable effect and establishes which flags can matter at all.
2. **Per-TU matrix.** For every one of the 192 app TUs and every flag, the TU is
   recompiled with the canonical command plus the flag, and its object's
   `.rodata`/`.data`/`.bss`/`.init_array`/`.fini_array`/`.comment` contents and
   function bytes are compared with the canonical object. Only TUs whose object
   changed are relinked (the changed object replaces the canonical object on the
   exact gold link line); the resulting `.so` is compared section-by-section
   with the original and with the baseline, and the official verifier is run on
   every relinked variant. This is 192 × 14 = 2,688 object compiles and 198
   relinks/verifies (the observable pairs).

The global matrix was run with the repo's existing harness:

```sh
cat > /dev/shm/w5208/global/variants.json <<'EOF'
[
 {"name":"fdata_sections","flags":"-fdata-sections"},
 {"name":"fno_data_sections","flags":"-fno-data-sections"},
 {"name":"fmerge_all_constants","flags":"-fmerge-all-constants"},
 {"name":"fmerge_constants","flags":"-fmerge-constants"},
 {"name":"fno_merge_constants","flags":"-fno-merge-constants"},
 {"name":"fwritable_strings","flags":"-fwritable-strings"},
 {"name":"fno_zero_init_bss","flags":"-fno-zero-initialized-in-bss"},
 {"name":"fno_common","flags":"-fno-common"},
 {"name":"fcommon","flags":"-fcommon"},
 {"name":"fshort_wchar","flags":"-fshort-wchar"},
 {"name":"fno_short_wchar","flags":"-fno-short-wchar"},
 {"name":"fno_ident","flags":"-fno-ident"},
 {"name":"fno_use_init_array","flags":"-fno-use-init-array"},
 {"name":"fuse_init_array","flags":"-fuse-init-array"}
]
EOF
python3 tools/flag_fingerprint.py --mode variants \
  --variants-file /dev/shm/w5208/global/variants.json \
  --workdir /dev/shm/w5208/global --jobs 2 \
  --json /dev/shm/w5208/global/results.json
```

Official verifier confirmation for the observable and representative neutral
global variants:

```sh
OBJDUMP=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
python3 tools/verify/verify.py --build-dir /dev/shm/w5208/global/verify-<v> \
  --original /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /dev/shm/w5208/global/build-<v>/libgof2hdaa.so \
  --objdump $OBJDUMP --fail-on-wrong-type --top 0
```

Per-TU screen and relink (the harness is reproduced in §6):

```sh
python3 /dev/shm/w5208/pertu.py screen --jobs 6 --outdir /dev/shm/w5208/pertu
python3 /dev/shm/w5208/pertu.py relink --jobs 6 --outdir /dev/shm/w5208/pertu
python3 /dev/shm/w5208/verify_all.py          # official verify on all 198 relinks
```

The relink is the exact `ninja -t commands libgof2hdaa.so` gold line with only
the changed TU's object path substituted; the same `strip --strip-all` and
`tools/reorder_shstrtab.py` post-processing run. The canonical objects are
untouched.

## 2. Global flag matrix

`byte`/`linked` are official verifier counts. `.rodata`/`.data` are section
sizes; `diff` is differing bytes against the original (0 = byte-identical).
`pool` is the printable-string pool in `.rodata`. `new −` is the number of
previously exact functions that became non-exact. `TUs` is the number of app TUs
whose exact-function set changed.

| variant | flags | byte | linked | size | `.rodata` | `.data` | `.bss` | `.init_array` | `.fini_array` | rodata diff | data diff | pool | new − | TUs |
|---|---|---:|---:|---:|---:|---:|---:|---|---|---:|---:|---|---:|---:|
| baseline | — | **4500** | **4500** | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | 3 words | 2 words | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fdata_sections` | `-fdata-sections` | 4311 | **4500** | 4524/4524 | `0x41a07` | `0x295c` | `0x69ac` | same | same | size | size | 2779s/203624B | 189 | 87 |
| `fno_data_sections` | `-fno-data-sections` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fmerge_all_constants` | `-fmerge-all-constants` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fmerge_constants` | `-fmerge-constants` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fno_merge_constants` | `-fno-merge-constants` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fwritable_strings` | `-fwritable-strings` | 1425 | 4491 | 4515/4524 | `0x256e7` | `0x2310c` | `0x6aac` | same | same | size | size | 657s/88025B | 3075 | 190 |
| `fno_zero_init_bss` | `-fno-zero-initialized-in-bss` | 1522 | 4490 | 4516/4524 | `0x41b67` | `0x800c` | `0x132c` | same | same | 0 | size | 2779s/203624B | 2978 | 189 |
| `fno_common` | `-fno-common` | **BUILD FAIL** | — | — | — | — | — | — | — | — | — | — | — | — |
| `fcommon` | `-fcommon` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fshort_wchar` | `-fshort-wchar` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fno_short_wchar` | `-fno-short-wchar` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fno_ident` | `-fno-ident` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fno_use_init_array` | `-fno-use-init-array` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |
| `fuse_init_array` | `-fuse-init-array` | 4500 | 4500 | 4524/4524 | `0x41b67` | `0x296c` | `0x69ac` | same | same | 0 | 0 | 2779s/203624B | 0 | 0 |

The ten neutral global variants differ from the canonical `.so` **only** in the
20-byte `.note.gnu.build-id` (the build-id hashes the unstripped output, whose
debug info records the flag). Their `.text`, `.rodata`, `.data`, `.bss`,
`.init_array`, `.fini_array`, `.ARM.attributes`, `.dynsym`, `.dynstr`, `.hash`,
`.rel.dyn` and `.rel.plt` are byte-identical to the baseline. The measured
`.so` sha256 values are:

```text
baseline                 809aab4e633964764d8550ebd89a78d7a86d9775bfaeae731a29ba623123487a
fdata_sections           232bc492d5e93882d21f40f9fd631b0768e796bb94c89c1e2ab1010ba416902e
fno_data_sections        6882223efc3160504c041791fedabc71ad59e88dad8db1ac303a4c52e950d636
fmerge_all_constants     35b9149e00df9adf72ceaac69e97864ad1235380924391e09cb59304c609bfee
fmerge_constants         e6bcefc3ec72ce05e4f8f02111713d12dcea20cd94b4bb7427c8c6656728d88c
fno_merge_constants      ef36a8783c2c78b8f9e9ab33b8435b60f900f73d9f220a9bfc8208a3bd1bbeec
fwritable_strings        e6a5fb8000465f66d576e5a47e9e24a1ce53dc8cd27d833dcebb5bb8314f51d2
fno_zero_init_bss        444db7e4b3c5203bb2910ec21ac73f46944f8494b9f4a4a9d1b2411493ebee8c
fcommon                  57a2798b007e399344b441f7a7190b62db0260e2d112831ea72a432379b6edea
fshort_wchar             32d65cf5ec9b201433133100074d028fd7d355bf5a70f6a7d867a8482f9b50d4
fno_short_wchar          d5a92fb16382cb865610b8b64d3dab757e5862c2375e27e7f71d4487fb759b27
fno_ident                4507ad54d8c9c19b5114f7e8c5b8d73fa6e3540988f45075bc722922a7202856
fno_use_init_array       7770be8c4527b188ecf0fa09e15c628d172b02547d303f40b7e5858055303f17
fuse_init_array          21af7cf808f35d81d82cd0a0fec66c386f1c7b7c7a27e75d92f7b7e4460ba509
```

### 2.1 `-fdata-sections` (observable, worse)

`-fdata-sections` changes `.rodata` from `0x41b67` to `0x41a07` (−0x160) and
`.data` from `0x296c` to `0x295c` (−0x10); `.bss` size is unchanged. The string
multiset is the same (2,779 strings / 203,624 bytes) but the pool order/padding
differs. `byte_exact` drops 4500 → 4311 (`linked_exact` stays 4500), and 189
previously exact functions become non-exact. This is the same conclusion as
`definitive_build_recipe_w3697.md` §4, re-measured at this base.

### 2.2 `-fwritable-strings` (observable, much worse)

String literals move from `.rodata` to `.data`: `.rodata` shrinks to `0x256e7`
and `.data` grows to `0x2310c`; `.bss` grows by 0x100 to `0x6aac`. The `.rodata`
string pool drops to 657 strings / 88,025 bytes. `byte_exact` 1425,
`linked_exact` 4491, size-exact 4515/4524; 3,075 previously exact functions
become non-exact.

### 2.3 `-fno-zero-initialized-in-bss` (observable, worse)

Zero-initialized globals move from `.bss` to `.data`: `.data` grows to `0x800c`,
`.bss` shrinks to `0x132c`. `.rodata` is unchanged (0 differing bytes) and the
string pool is unchanged. `byte_exact` 1522, `linked_exact` 4490, size-exact
4516/4524; 2,978 previously exact functions become non-exact.

### 2.4 `-fno-common` (global build fails)

The C TUs `src/platform/android/BackButton.c` and
`src/platform/android/GOF2HD2012.c` both contain a tentative definition of
`int g_android_back_button_pressed;`. With `-fno-common` both become strong
definitions and gold fails:

```text
ld.gold: error: .../GOF2HD2012.c.obj: multiple definition of 'g_android_back_button_pressed'
ld.gold: .../BackButton.c.obj: previous definition here
```

Per-TU `-fno-common` on any one of the four C TUs links (the other tentative
definition remains common and is resolved to the strong one) but is worse
(§3). `-fcommon` is the clang 7 C default and is neutral.

### 2.5 The neutral flags

`-fno-data-sections` (the default spelling), `-fmerge-all-constants`,
`-fmerge-constants`, `-fno-merge-constants`, `-fcommon`, `-fshort-wchar`,
`-fno-short-wchar`, `-fno-ident`, `-fno-use-init-array` and `-fuse-init-array`
are neutral on this tree:

* `-fno-data-sections` is already the clang default; the canonical build is the
  no-`-fdata-sections` layout.
* `-fmerge-constants` is already enabled at `-Oz`; `-fmerge-all-constants` and
  `-fno-merge-constants` do not change any object.
* There is no `wchar_t` or `L"..."` in `src/` (`grep -rn wchar_t src` → 0), so
  both `-fshort-wchar` and `-fno-short-wchar` are trivially unobservable.
* `-fno-ident` does not remove clang 7.0.2's object `.comment` (verified on a
  tiny TU: the `.comment` section is byte-identical with and without it).
* `-fno-use-init-array` / `-fuse-init-array` do not change the `.init_array` /
  `.ctors` sections of `Engine.cpp` or `ParticleSettings.cpp`; the linked
  `.init_array` (3 words) and `.fini_array` (2 words) are identical in every
  variant.

## 3. Per-TU matrix

For each flag, the table below gives the number of app TUs whose **object**
changed, the number that relinked to a byte-identical `.so` (excluding the
build-id), and the best/worst official verifier counts among the relinked
variants. "Object-identical" TUs are unobservable: the flag changes nothing in
that TU's object, so it cannot be distinguished from the canonical flag set.

| flag | object-changed TUs | object-identical TUs | relinked | `.so` identical excl. build-id | best byte | best linked | worst byte | worst linked | improved? |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---|
| `-fdata-sections` | 68 | 124 | 68 | 16 | **4500** | **4500** | 4346 | 4500 | no |
| `-fwritable-strings` | 96 | 96 | 96 | 0 | 4499 | 4500 | 1429 | 4497 | no |
| `-fno-zero-initialized-in-bss` | 30 | 162 | 30 | 0 | **4500** | **4500** | 1394 | 4494 | no |
| `-fno-common` | 4 | 188 | 4 | 0 | 4487 | 4500 | 4482 | 4500 | no |
| other 10 flags | 0 | 192 each | 0 | — | 4500 | 4500 | 4500 | 4500 | no |

No relinked variant has `byte_exact > 4500` or `linked_exact > 4500`. The
maximum is the baseline 4500/4500; several variants tie it while changing data
bytes or non-exact function bytes, which is not an improvement.

### 3.1 `-fdata-sections`: 16 link-neutral TUs, 3 small-diff TUs, 49 worse

The 16 TUs whose single-TU `-fdata-sections` build is byte-identical to the
baseline except for the build-id (so the flag is unobservable for them):

```text
engine/core/ApplicationManager.cpp      engine/render/Engine.cpp
engine/render/LensFlare.cpp             engine/render/shaders/PostBWShader.cpp
game/core/Radio.cpp                     game/menu/MGame.cpp
game/menu/ModMainMenu.cpp               game/menu/SpaceLounge.cpp
game/ui/DialogueWindow.cpp              game/ui/Hud.cpp
game/ui/MissionsWindow.cpp              game/weapons/BeamGun.cpp
game/weapons/SpriteGun.cpp              game/world/LevelScript.cpp
game/world/StarMap.cpp                  platform/android/NdkHooks.cpp
```

Three more TUs keep `byte_exact` 4500 but change 2 bytes of `.data.rel.ro`,
2 bytes of `.dynsym` and 5 bytes of `.rel.dyn`: `FileInterfaceAndroid.cpp`,
`MTitle.cpp`, `ObjectGun.cpp`. The remaining 49 TUs lose exact functions
(byte 4346–4499). The worst are `AEFile.cpp` 4346, `BumpShaderParticle.cpp`
4350, `GreenShader.cpp` 4351. In every case `linked_exact` stays 4500, so the
loss is literal-pool/address material, not instruction-stream structure.

### 3.2 `-fno-zero-initialized-in-bss`: 29 worse, 1 code-neutral

`Radar.cpp` keeps `byte_exact` 4500 and `linked_exact` 4500 but changes 566
bytes of `.data` (the zero-init globals move out of `.bss`). The other 29 TUs
are worse; the worst are `VirtualInput.cpp` 1394 (`linked` 4494),
`Engine.cpp` 2121 (`linked` 4499), `ModStation.cpp` 2467.

### 3.3 `-fno-common`: all four C TUs worse

`BlowfishSubkeyFile.c` 4482, `GOF2HD2012.c` 4482, `main.c` 4482,
`BackButton.c` 4487; all keep `linked_exact` 4500 but change the `.bss` size
(the tentative definition becomes strong, so `.bss` shrinks and the symbol's
placement changes; `.data` is unchanged).

### 3.4 `-fwritable-strings`: all 96 TUs worse

Every relinked TU loses exact functions. The worst are
`FileInterfaceAndroid.cpp` 1429, `MenuTouchWindow.cpp` 2659,
`ListItemWindow.cpp` 2694, `HangarWindow.cpp` 2745. `AESoundRessource.cpp` is
the only one whose linked data sections stay identical (byte 4498, two
functions lost), because its strings are merged/deduplicated; it is still not
an improvement.

## 4. Proven / disproven / unobservable

### Proven by measurement

* The canonical build's `.rodata` (0 differing bytes), `.data` (0),
  `.data.rel.ro` (0), `.bss` size, `.init_array` (3 words) and `.fini_array`
  (2 words) are byte-identical to the original. The canonical layout is the
  no-`-fdata-sections` layout.
* `-fdata-sections` is observable and worse: `.rodata` `0x41a07`, `.data`
  `0x295c`, byte 4311, linked 4500.
* `-fwritable-strings` is observable and much worse: `.rodata` `0x256e7`,
  `.data` `0x2310c`, `.bss` `0x6aac`, byte 1425, linked 4491.
* `-fno-zero-initialized-in-bss` is observable and worse: `.data` `0x800c`,
  `.bss` `0x132c`, byte 1522, linked 4490.
* `-fno-common` fails the global link (duplicate
  `g_android_back_button_pressed` in `BackButton.c` and `GOF2HD2012.c`); per-TU
  it is worse (byte 4482–4487).
* The other ten flags are object-identical on all 192 app TUs and differ from
  the canonical `.so` only in the build-id. They are unobservable.
* No single-TU application of any of the 14 flags improves the gate. The best
  result is a tie at 4500/4500, and every tie either leaves the `.so` identical
  (build-id aside), changes data bytes, or changes non-exact function bytes.

### Disproven (for the observable TUs)

* `-fdata-sections` on any of the 68 object-changed TUs: 49 are strictly worse;
  16 are link-neutral; 3 change `.data.rel.ro`/`.dynsym`/`.rel.dyn` by
  2/2/5 bytes. None is required.
* `-fwritable-strings` on any of the 96 object-changed TUs: all worse.
* `-fno-zero-initialized-in-bss` on any of the 30 object-changed TUs: 29 worse,
  1 data-changing tie (`Radar.cpp`).
* `-fno-common` on any of the four C TUs: all worse.

### Unobservable / unknown

* The 124 TUs where `-fdata-sections` is object-identical, the 96 where
  `-fwritable-strings` is object-identical, the 162 where
  `-fno-zero-initialized-in-bss` is object-identical, the 188 where
  `-fno-common` is object-identical, and **all 192 TUs** for the other ten
  flags: a per-TU use of the flag on these TUs cannot be distinguished from the
  canonical set. This is the only surviving "a TU might have used a non-global
  flag" possibility, and it is not observable in the shipped `.so`.
* Combinations of two or more flags on the same TU were not exhaustively
  tested. Since every single observable flag on every TU is neutral-or-worse,
  a combination would have to cancel two independent layout changes to be
  neutral; no evidence for such a cancellation exists.
* The original's TU membership is known to diverge from ours (the invented
  `AbyssEngine.cpp` mega-TU and the documented merges). A flag applied to an
  original TU that our tree splits/merges differently could be unobservable in
  our per-TU test. The global result (canonical data sections exact) still
  bounds any such effect to zero in the linked output.
* The exact build-system file (`Android.mk` vs custom Makefile) is not
  recoverable; `-fno-data-sections` is an effective flag, not necessarily a
  literal command-line flag.

## 5. Confidence table

| claim | confidence | hard evidence |
|---|---|---|
| Canonical `.rodata`/`.data`/`.data.rel.ro`/`.bss`/`.init_array`/`.fini_array` match the original | **certain** | direct byte comparison: 0 differing bytes; `.bss` size `0x69ac`; init words `0x69441,0x694c1,0x694dd`; fini `0x69548,0x69534` |
| `-fdata-sections` was not used globally | **certain** | global build: `.rodata` `0x41a07` vs `0x41b67`, `.data` `0x295c` vs `0x296c`, byte 4311 vs 4500 |
| `-fwritable-strings` was not used globally | **certain** | `.rodata` `0x256e7`, `.data` `0x2310c`, byte 1425 |
| `-fno-zero-initialized-in-bss` was not used globally | **certain** | `.data` `0x800c`, `.bss` `0x132c`, byte 1522 |
| `-fno-common` was not used globally | **certain** | link failure: duplicate `g_android_back_button_pressed` |
| The ten neutral flags are unobservable | **certain** | object-identical on all 192 TUs; linked `.so` differs only in build-id |
| No single-TU flag application improves the gate | **certain** | 198 official verify runs: max byte 4500, max linked 4500; no `newly_exact` |
| No TU *requires* a non-global data flag | **high** | canonical data sections exact; every observable single-TU application is neutral-or-worse |
| A per-TU flag on an object-identical TU could have been used invisibly | **high (unobservable)** | object bytes identical; cannot be distinguished from the canonical command |
| Combinations of flags are neutral | **medium** | not exhaustively tested; single-flag results are all neutral-or-worse |
| Original TU membership matches ours | **known false** | `_work/original_layout/` documents the `AbyssEngine.cpp` mega-TU and other merges |

## 6. Harness (self-contained)

The per-TU screen/relink harness is `/dev/shm/w5208/pertu.py`. Its essential
logic:

```python
FLAGS = [('fdata_sections','-fdata-sections'), ('fno_data_sections','-fno-data-sections'),
         ('fmerge_all_constants','-fmerge-all-constants'), ('fmerge_constants','-fmerge-constants'),
         ('fno_merge_constants','-fno-merge-constants'), ('fwritable_strings','-fwritable-strings'),
         ('fno_zero_init_bss','-fno-zero-initialized-in-bss'), ('fno_common','-fno-common'),
         ('fcommon','-fcommon'), ('fshort_wchar','-fshort-wchar'),
         ('fno_short_wchar','-fno-short-wchar'), ('fno_ident','-fno-ident'),
         ('fno_use_init_array','-fno-use-init-array'), ('fuse_init_array','-fuse-init-array')]

# screen: compile each TU with `command + flag`, compare object data sections
#   (.rodata*, .data*, .bss*, .init_array/.ctors, .fini_array/.dtors, .comment)
#   and every STT_FUNC body against the canonical object.
# relink: for each changed object, substitute its path in the exact
#   `ninja -C build -t commands libgof2hdaa.so` gold line, run the same
#   strip + reorder_shstrtab.py, and measure sections/functions.
```

The exact per-TU results are in the appendix. The raw JSON is
`/dev/shm/w5208/pertu/screen.json`, `relink.json`, `verify_all.json` and
`/dev/shm/w5208/global/results.json`; the scratch builds are
`/dev/shm/w5208/global/build-*` and `/dev/shm/w5208/pertu/build`.

## 7. Strict improvement

**None.** The canonical build already has 0 differing bytes in every data
section. Every observable per-TU variant either ties 4500/4500 while changing
data or non-exact code bytes, or is worse. The minimal change to land is
therefore **no change**: keep the global flag set as-is. No `src/`, `tools/`,
baseline or allowlist change is required.

## Appendix A — per-TU relink results

`lost` = previously exact functions that became non-exact (overall);
`own lost/total` = how many of those belong to the flagged TU itself / the TU's
total exported functions. `.rodata`/`.data` are sizes and differing bytes
against the original (`size` = the section size changed, so no same-size diff
is defined). `linked` is the official verifier count.


### `fdata_sections` — 68 TUs whose object changed

| TU | byte | linked | lost | own lost/total | .rodata | .data | .bss |
|---|---:|---:|---:|---:|---:|---:|---:|
| `engine/file/AEFile.cpp` | 4346 | 4500 | 154 | 0/81 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `game/core/BumpShaderParticle.cpp` | 4350 | 4500 | 150 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/GreenShader.cpp` | 4351 | 4500 | 149 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/SimpleShader.cpp` | 4354 | 4500 | 146 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/TextureAlphaTestShader.cpp` | 4355 | 4500 | 145 | 1/8 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `game/core/BumpShaderV4.cpp` | 4360 | 4500 | 140 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/BumpShader.cpp` | 4362 | 4500 | 138 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `game/core/BumpShaderV3.cpp` | 4364 | 4500 | 136 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/GlowPPShader.cpp` | 4367 | 4500 | 133 | 1/11 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/GenericShader2.cpp` | 4371 | 4500 | 129 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/GenericShader.cpp` | 4372 | 4500 | 128 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/GenericShader1.cpp` | 4373 | 4500 | 127 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/VertexColorShader.cpp` | 4378 | 4500 | 122 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/NoTexShader.cpp` | 4379 | 4500 | 121 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/CubeMapping.cpp` | 4380 | 4500 | 120 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/TextureShader.cpp` | 4382 | 4500 | 118 | 1/8 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/MaskShader.cpp` | 4384 | 4500 | 116 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/render/shaders/SpecCubeMapping.cpp` | 4390 | 4500 | 110 | 1/6 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `game/ship/Player.cpp` | 4415 | 4500 | 85 | 3/115 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `game/world/Level.cpp` | 4424 | 4500 | 76 | 7/109 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `game/ui/MenuTouchWindow.cpp` | 4458 | 4500 | 42 | 1/41 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `engine/input/VirtualInput.cpp` | 4461 | 4500 | 39 | 16/34 | 0x41b67/0 | 0x295c/size | 0x69ac |
| `game/world/StarSystem.cpp` | 4461 | 4500 | 39 | 4/25 | 0x41b57/size | 0x296c/5 | 0x69ac |
| `game/core/Globals.cpp` | 4497 | 4500 | 3 | 3/41 | 0x41b67/6479 | 0x296c/0 | 0x69ac |
| `game/mission/Status.cpp` | 4498 | 4500 | 2 | 2/138 | 0x41b67/42 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/BloomShader.cpp` | 4499 | 4500 | 1 | 1/11 | 0x41b67/3135 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/BlurShader.cpp` | 4499 | 4500 | 1 | 1/10 | 0x41b67/1006 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/BumpMapping.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/1282 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/BumpRimCubeShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/4726 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/ColorMixAdd.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/775 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/CubeNormalMapping.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/2335 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/DNSShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/262 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/DrawFBOShader.cpp` | 4499 | 4500 | 1 | 1/9 | 0x41b67/392 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/EnergyShield.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/1515 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/GlowShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/425 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/NoTexVtxColorShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/467 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/PulseShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/548 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/SandboxShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/1809 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/SimpleRefractionShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/1498 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/SpecCubeAlphaMapping.cpp` | 4499 | 4500 | 1 | 1/10 | 0x41b67/2003 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/TexOnlyShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/430 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/TextureLightShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/3088 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/TextureVtxColorShader.cpp` | 4499 | 4500 | 1 | 1/8 | 0x41b67/2094 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/VertexColorAlphaTextureShader.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/1282 | 0x296c/0 | 0x69ac |
| `game/core/BumpRimCubeShader_new.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/4766 | 0x296c/0 | 0x69ac |
| `game/core/BumpShaderRefract.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/1458 | 0x296c/0 | 0x69ac |
| `game/core/BumpShaderV2.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/1880 | 0x296c/0 | 0x69ac |
| `game/core/TextureConference.cpp` | 4499 | 4500 | 1 | 1/6 | 0x41b67/730 | 0x296c/0 | 0x69ac |
| `game/menu/ModStation.cpp` | 4499 | 4500 | 1 | 1/40 | 0x41b67/35 | 0x296c/0 | 0x69ac |
| `engine/core/ApplicationManager.cpp` | 4500 | 4500 | 0 | 0/79 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `engine/file/FileInterfaceAndroid.cpp` | 4500 | 4500 | 0 | 0/38 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `engine/render/Engine.cpp` | 4500 | 4500 | 0 | 0/76 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `engine/render/LensFlare.cpp` | 4500 | 4500 | 0 | 0/6 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `engine/render/shaders/PostBWShader.cpp` | 4500 | 4500 | 0 | 0/9 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/core/Radio.cpp` | 4500 | 4500 | 0 | 0/11 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/menu/MGame.cpp` | 4500 | 4500 | 0 | 0/45 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/menu/MTitle.cpp` | 4500 | 4500 | 0 | 0/21 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/menu/ModMainMenu.cpp` | 4500 | 4500 | 0 | 0/21 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/menu/SpaceLounge.cpp` | 4500 | 4500 | 0 | 0/31 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/ui/DialogueWindow.cpp` | 4500 | 4500 | 0 | 0/27 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/ui/Hud.cpp` | 4500 | 4500 | 0 | 0/51 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/ui/MissionsWindow.cpp` | 4500 | 4500 | 0 | 0/13 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/weapons/BeamGun.cpp` | 4500 | 4500 | 0 | 0/11 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/weapons/ObjectGun.cpp` | 4500 | 4500 | 0 | 0/12 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/weapons/SpriteGun.cpp` | 4500 | 4500 | 0 | 0/8 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/world/LevelScript.cpp` | 4500 | 4500 | 0 | 0/17 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/world/StarMap.cpp` | 4500 | 4500 | 0 | 0/25 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `platform/android/NdkHooks.cpp` | 4500 | 4500 | 0 | 0/41 | 0x41b67/0 | 0x296c/0 | 0x69ac |

### `fno_zero_init_bss` — 30 TUs whose object changed

| TU | byte | linked | lost | own lost/total | .rodata | .data | .bss |
|---|---:|---:|---:|---:|---:|---:|---:|
| `engine/input/VirtualInput.cpp` | 1394 | 4494 | 3106 | 20/34 | 0x41b67/0 | 0x399c/size | 0x597c |
| `engine/render/Engine.cpp` | 2121 | 4499 | 2379 | 27/76 | 0x41b67/0 | 0x29dc/size | 0x693c |
| `game/menu/ModStation.cpp` | 2467 | 4497 | 2033 | 29/40 | 0x41b67/0 | 0x297c/size | 0x699c |
| `engine/core/GameText.cpp` | 4464 | 4500 | 36 | 1/12 | 0x41b67/0 | 0x299c/size | 0x698c |
| `engine/file/FileInterfaceAndroid.cpp` | 4464 | 4500 | 36 | 0/38 | 0x41b67/0 | 0x298c/size | 0x697c |
| `platform/android/GOF2HD2012.c` | 4464 | 4500 | 36 | 0/11 | 0x41b67/0 | 0x298c/size | 0x697c |
| `engine/render/ParticleSettings.cpp` | 4467 | 4500 | 33 | 0/6 | 0x41b67/0 | 0x657c/size | 0x2dac |
| `engine/core/NFC.cpp` | 4469 | 4500 | 31 | 0/28 | 0x41b67/0 | 0x296c/0 | 0x699c |
| `game/ship/PlayerEgo.cpp` | 4472 | 4500 | 28 | 0/174 | 0x41b67/0 | 0x297c/size | 0x699c |
| `game/core/Globals.cpp` | 4474 | 4500 | 26 | 0/41 | 0x41b67/0 | 0x30ac/size | 0x626c |
| `game/core/Radio.cpp` | 4474 | 4500 | 26 | 0/11 | 0x41b67/0 | 0x297c/size | 0x699c |
| `game/menu/SpaceLounge.cpp` | 4474 | 4500 | 26 | 1/31 | 0x41b67/0 | 0x297c/size | 0x699c |
| `game/ship/PlayerAsteroid.cpp` | 4474 | 4500 | 26 | 0/24 | 0x41b67/0 | 0x298c/size | 0x698c |
| `game/ui/DialogueWindow.cpp` | 4474 | 4500 | 26 | 1/27 | 0x41b67/0 | 0x297c/size | 0x699c |
| `game/world/Level.cpp` | 4475 | 4500 | 25 | 0/109 | 0x41b67/0 | 0x29ac/size | 0x696c |
| `platform/android/NdkHooks.cpp` | 4475 | 4500 | 25 | 3/41 | 0x41b67/0 | 0x29dc/size | 0x693c |
| `game/ship/TargetFollowCamera.cpp` | 4476 | 4500 | 24 | 0/42 | 0x41b67/0 | 0x297c/size | 0x699c |
| `engine/core/ApplicationManager.cpp` | 4479 | 4500 | 21 | 0/79 | 0x41b67/0 | 0x29cc/size | 0x694c |
| `engine/file/AEFile.cpp` | 4480 | 4500 | 20 | 0/81 | 0x41b67/0 | 0x297c/size | 0x699c |
| `engine/render/PaintCanvas.cpp` | 4480 | 4500 | 20 | 0/223 | 0x41b67/0 | 0x297c/size | 0x699c |
| `game/core/String.cpp` | 4480 | 4500 | 20 | 0/65 | 0x41b67/0 | 0x296c/1563 | 0x699c |
| `platform/android/main.c` | 4480 | 4500 | 20 | 0/40 | 0x41b67/0 | 0x297c/size | 0x699c |
| `game/mission/RecordHandler.cpp` | 4494 | 4500 | 6 | 0/44 | 0x41b67/0 | 0x296c/1487 | 0x69ac |
| `platform/android/NdkAutosave.cpp` | 4494 | 4500 | 6 | 0/4 | 0x41b67/0 | 0x296c/1572 | 0x69ac |
| `game/ship/PlayerFighter.cpp` | 4497 | 4500 | 3 | 0/40 | 0x41b67/0 | 0x296c/1246 | 0x69ac |
| `game/ui/StatusWindow.cpp` | 4497 | 4500 | 3 | 1/13 | 0x41b67/0 | 0x296c/240 | 0x69ac |
| `game/world/StarSystem.cpp` | 4497 | 4500 | 3 | 0/25 | 0x41b67/0 | 0x296c/482 | 0x69ac |
| `game/mission/Mission.cpp` | 4498 | 4500 | 2 | 0/55 | 0x41b67/0 | 0x296c/240 | 0x69ac |
| `game/ui/Hud.cpp` | 4498 | 4500 | 2 | 0/51 | 0x41b67/0 | 0x296c/254 | 0x69ac |
| `game/weapons/Radar.cpp` | 4500 | 4500 | 0 | 0/19 | 0x41b67/0 | 0x296c/566 | 0x69ac |

### `fno_common` — 4 TUs whose object changed

| TU | byte | linked | lost | own lost/total | .rodata | .data | .bss |
|---|---:|---:|---:|---:|---:|---:|---:|
| `engine/file/BlowfishSubkeyFile.c` | 4482 | 4500 | 18 | 0/5 | 0x41b67/0 | 0x296c/0 | 0x69a4 |
| `platform/android/GOF2HD2012.c` | 4482 | 4500 | 18 | 0/11 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `platform/android/main.c` | 4482 | 4500 | 18 | 0/40 | 0x41b67/0 | 0x296c/0 | 0x69a4 |
| `platform/android/BackButton.c` | 4487 | 4500 | 13 | 0/1 | 0x41b67/0 | 0x296c/0 | 0x69a8 |

### `fwritable_strings` — 96 TUs whose object changed

| TU | byte | linked | lost | own lost/total | .rodata | .data | .bss |
|---|---:|---:|---:|---:|---:|---:|---:|
| `engine/file/FileInterfaceAndroid.cpp` | 1429 | 4498 | 3071 | 19/38 | 0x41af7/size | 0x2a0c/size | 0x69ac |
| `game/ui/MenuTouchWindow.cpp` | 2659 | 4499 | 1841 | 21/41 | 0x41ae7/size | 0x2a3c/size | 0x69dc |
| `game/ui/ListItemWindow.cpp` | 2694 | 4499 | 1806 | 7/12 | 0x41b57/size | 0x29fc/size | 0x69dc |
| `game/ui/HangarWindow.cpp` | 2745 | 4497 | 1755 | 20/32 | 0x41b37/size | 0x29dc/size | 0x69cc |
| `engine/render/GameResource.cpp` | 2889 | 4500 | 1611 | 3/8 | 0x270c7/size | 0x1f46c/size | 0x69bc |
| `game/ui/DialogueWindow.cpp` | 2929 | 4499 | 1571 | 17/27 | 0x41b67/5012 | 0x299c/size | 0x69bc |
| `platform/android/main.c` | 4005 | 4499 | 495 | 34/40 | 0x41867/size | 0x2c8c/size | 0x69ac |
| `engine/core/NFC.cpp` | 4017 | 4500 | 483 | 23/28 | 0x41907/size | 0x2c1c/size | 0x69ac |
| `engine/input/VirtualInput.cpp` | 4040 | 4500 | 460 | 16/34 | 0x41b57/size | 0x297c/size | 0x69ac |
| `engine/file/BlowfishSubkeyFile.c` | 4043 | 4500 | 457 | 2/5 | 0x41b27/size | 0x29ac/size | 0x69ac |
| `engine/render/GLES1Compat.cpp` | 4072 | 4500 | 428 | 1/46 | 0x41b57/size | 0x296c/1680 | 0x69ac |
| `engine/render/PaintCanvas.cpp` | 4118 | 4500 | 382 | 5/223 | 0x41b37/size | 0x299c/size | 0x69ac |
| `engine/core/GameTextiPhone.cpp` | 4125 | 4500 | 375 | 4/6 | 0x41af7/size | 0x29fc/size | 0x69ac |
| `engine/core/GameText.cpp` | 4127 | 4500 | 373 | 3/12 | 0x41b27/size | 0x29ac/size | 0x69ac |
| `engine/core/ApplicationManager.cpp` | 4128 | 4500 | 372 | 4/79 | 0x41a47/size | 0x2aac/size | 0x69bc |
| `engine/file/ConfigReader.cpp` | 4134 | 4500 | 366 | 4/14 | 0x41b47/size | 0x299c/size | 0x69ac |
| `engine/render/Engine.cpp` | 4134 | 4500 | 366 | 3/76 | 0x41b57/size | 0x297c/size | 0x69ac |
| `engine/render/shaders/SpecCubeAlphaMapping.cpp` | 4139 | 4500 | 361 | 3/10 | 0x41af7/size | 0x2a9c/size | 0x69ac |
| `game/core/BumpShaderParticle.cpp` | 4141 | 4500 | 359 | 3/6 | 0x41b57/size | 0x2a8c/size | 0x69ac |
| `engine/render/shaders/GreenShader.cpp` | 4143 | 4500 | 357 | 3/6 | 0x41b57/size | 0x2a2c/size | 0x69ac |
| `game/core/BumpRimCubeShader_new.cpp` | 4145 | 4500 | 355 | 3/6 | 0x41af7/size | 0x2b7c/size | 0x69ac |
| `engine/render/shaders/SimpleShader.cpp` | 4148 | 4500 | 352 | 3/6 | 0x41b57/size | 0x299c/size | 0x69ac |
| `engine/render/shaders/TextureAlphaTestShader.cpp` | 4149 | 4500 | 351 | 4/8 | 0x41b57/size | 0x29ec/size | 0x69ac |
| `engine/render/shaders/BloomShader.cpp` | 4152 | 4500 | 348 | 4/11 | 0x41af7/size | 0x2a0c/size | 0x69ac |
| `engine/render/shaders/EnergyShield.cpp` | 4155 | 4500 | 345 | 3/6 | 0x41b37/size | 0x2a2c/size | 0x69ac |
| `engine/render/shaders/BlurShader.cpp` | 4157 | 4500 | 343 | 3/10 | 0x41b27/size | 0x29dc/size | 0x69ac |
| `game/core/BumpShaderCloak.cpp` | 4159 | 4500 | 341 | 3/6 | 0x41b47/size | 0x2afc/size | 0x69ac |
| `game/core/BumpShaderV4.cpp` | 4161 | 4500 | 339 | 3/6 | 0x41b57/size | 0x2a3c/size | 0x69ac |
| `engine/render/shaders/TexOnlyShader.cpp` | 4163 | 4500 | 337 | 3/6 | 0x41b57/size | 0x29ac/size | 0x69ac |
| `engine/render/shaders/BumpShader.cpp` | 4165 | 4500 | 335 | 3/6 | 0x41b57/size | 0x2aec/size | 0x69ac |
| `engine/render/shaders/TextureVtxColorShader.cpp` | 4167 | 4500 | 333 | 4/8 | 0x41b57/size | 0x2a2c/size | 0x69ac |
| `game/core/BumpShaderV3.cpp` | 4171 | 4500 | 329 | 3/6 | 0x41b57/size | 0x2aec/size | 0x69ac |
| `engine/render/shaders/SimpleRefractionShader.cpp` | 4173 | 4500 | 327 | 3/6 | 0x41b47/size | 0x2a2c/size | 0x69ac |
| `game/core/TextureConference.cpp` | 4175 | 4500 | 325 | 3/6 | 0x41b57/size | 0x29bc/size | 0x69ac |
| `engine/render/shaders/GlowPPShader.cpp` | 4177 | 4500 | 323 | 4/11 | 0x41af7/size | 0x2a1c/size | 0x69ac |
| `game/core/BumpShaderV2.cpp` | 4180 | 4500 | 320 | 3/6 | 0x41b57/size | 0x2a3c/size | 0x69ac |
| `engine/render/shaders/TextureLightShader.cpp` | 4182 | 4500 | 318 | 3/6 | 0x41b07/size | 0x2aac/size | 0x69ac |
| `engine/render/shaders/NoTexVtxColorShader.cpp` | 4184 | 4500 | 316 | 3/6 | 0x41b57/size | 0x29ac/size | 0x69ac |
| `engine/render/shaders/GenericShader2.cpp` | 4186 | 4500 | 314 | 3/6 | 0x41b57/size | 0x2a2c/size | 0x69ac |
| `engine/render/ShaderBaseStruct.cpp` | 4188 | 4500 | 312 | 1/10 | 0x41b57/size | 0x297c/size | 0x69ac |
| `engine/render/shaders/GenericShader.cpp` | 4189 | 4500 | 311 | 3/6 | 0x41b57/size | 0x2a5c/size | 0x69ac |
| `engine/render/shaders/GenericShader1.cpp` | 4191 | 4500 | 309 | 3/6 | 0x41b57/size | 0x2a2c/size | 0x69ac |
| `engine/render/shaders/PostBWShader.cpp` | 4193 | 4500 | 307 | 3/9 | 0x41b57/size | 0x29ac/size | 0x69ac |
| `engine/render/shaders/DNSShader.cpp` | 4195 | 4500 | 305 | 3/6 | 0x41b27/size | 0x2a9c/size | 0x69ac |
| `game/core/BumpShaderRefract.cpp` | 4197 | 4500 | 303 | 3/6 | 0x41b57/size | 0x2a0c/size | 0x69ac |
| `engine/render/shaders/BumpRimCubeShader.cpp` | 4199 | 4500 | 301 | 3/6 | 0x41b17/size | 0x2b7c/size | 0x69ac |
| `engine/render/shaders/VertexColorShader.cpp` | 4201 | 4500 | 299 | 3/6 | 0x41b57/size | 0x2a4c/size | 0x69ac |
| `engine/render/shaders/NoTexShader.cpp` | 4203 | 4500 | 297 | 3/6 | 0x41b57/size | 0x299c/size | 0x69ac |
| `engine/render/shaders/CubeMapping.cpp` | 4205 | 4500 | 295 | 3/6 | 0x41b57/size | 0x2a3c/size | 0x69ac |
| `game/core/String.cpp` | 4206 | 4500 | 294 | 5/65 | 0x41b67/111204 | 0x297c/size | 0x69ac |
| `engine/render/shaders/CubeNormalMapping.cpp` | 4207 | 4500 | 293 | 3/6 | 0x41b47/size | 0x2a6c/size | 0x69ac |
| `engine/render/shaders/TextureShader.cpp` | 4208 | 4500 | 292 | 4/8 | 0x41b57/size | 0x2a0c/size | 0x69ac |
| `engine/render/shaders/BumpMapping.cpp` | 4211 | 4500 | 289 | 3/6 | 0x41b47/size | 0x29ec/size | 0x69ac |
| `engine/render/shaders/MaskShader.cpp` | 4213 | 4500 | 287 | 3/6 | 0x41b47/size | 0x29cc/size | 0x69ac |
| `engine/render/shaders/SandboxShader.cpp` | 4215 | 4500 | 285 | 3/6 | 0x41b37/size | 0x29fc/size | 0x69ac |
| `engine/render/shaders/VertexColorAlphaTextureShader.cpp` | 4217 | 4500 | 283 | 3/6 | 0x41b47/size | 0x2a4c/size | 0x69ac |
| `engine/render/shaders/GlowShader.cpp` | 4219 | 4500 | 281 | 3/6 | 0x41b57/size | 0x29bc/size | 0x69ac |
| `engine/render/shaders/ColorMixAdd.cpp` | 4221 | 4500 | 279 | 3/6 | 0x41b57/size | 0x29cc/size | 0x69ac |
| `engine/render/shaders/PulseShader.cpp` | 4223 | 4500 | 277 | 3/6 | 0x41b57/size | 0x2a3c/size | 0x69ac |
| `engine/render/shaders/SpecCubeMapping.cpp` | 4225 | 4500 | 275 | 3/6 | 0x41b17/size | 0x2a7c/size | 0x69ac |
| `engine/audio/FModSound.cpp` | 4227 | 4500 | 273 | 3/46 | 0x41b27/size | 0x29ac/size | 0x69ac |
| `platform/android/NdkHooks.cpp` | 4228 | 4500 | 272 | 3/41 | 0x41b57/size | 0x298c/size | 0x69ac |
| `engine/file/AEFile.cpp` | 4229 | 4500 | 271 | 3/81 | 0x41b67/111289 | 0x296c/1655 | 0x69ac |
| `game/mission/RecordHandler.cpp` | 4231 | 4500 | 269 | 7/44 | 0x41b27/size | 0x299c/size | 0x69bc |
| `game/ui/Layout.cpp` | 4232 | 4500 | 268 | 8/50 | 0x41b57/size | 0x297c/size | 0x69bc |
| `game/core/GameInit.cpp` | 4235 | 4500 | 265 | 1/5 | 0x41b27/size | 0x299c/size | 0x69bc |
| `game/menu/ModStation.cpp` | 4240 | 4500 | 260 | 9/40 | 0x41a97/size | 0x2a9c/size | 0x69bc |
| `game/core/Globals.cpp` | 4244 | 4500 | 256 | 12/41 | 0x41af7/size | 0x2a7c/size | 0x69bc |
| `game/ui/WantedWindow.cpp` | 4244 | 4500 | 256 | 2/20 | 0x41b57/size | 0x299c/size | 0x69bc |
| `engine/file/FileRead.cpp` | 4258 | 4500 | 242 | 17/32 | 0x41917/size | 0x2dec/size | 0x69bc |
| `game/world/NewsTicker.cpp` | 4284 | 4500 | 216 | 3/14 | 0x41a57/size | 0x2b7c/size | 0x69bc |
| `game/menu/MGame.cpp` | 4288 | 4500 | 212 | 4/45 | 0x41b57/size | 0x29bc/size | 0x69ac |
| `engine/render/ParticleSettings.cpp` | 4291 | 4500 | 209 | 1/6 | 0x41957/size | 0x2b8c/size | 0x69ac |
| `game/world/Level.cpp` | 4391 | 4500 | 109 | 1/109 | 0x41b67/110916 | 0x296c/1377 | 0x69ac |
| `game/weapons/Radar.cpp` | 4422 | 4500 | 78 | 4/19 | 0x41b67/5289 | 0x297c/size | 0x69bc |
| `game/world/StarMap.cpp` | 4425 | 4500 | 75 | 4/25 | 0x41b67/155 | 0x297c/size | 0x69bc |
| `game/ui/ChoiceWindow.cpp` | 4427 | 4500 | 73 | 2/23 | 0x41b67/5310 | 0x297c/size | 0x69ac |
| `game/ui/Hud.cpp` | 4430 | 4500 | 70 | 9/51 | 0x41b67/5015 | 0x29dc/size | 0x69bc |
| `game/ui/StatusWindow.cpp` | 4431 | 4500 | 69 | 3/13 | 0x41b67/5209 | 0x298c/size | 0x69bc |
| `engine/render/shaders/DrawFBOShader.cpp` | 4434 | 4500 | 66 | 1/9 | 0x41b67/438 | 0x299c/size | 0x69ac |
| `game/menu/SpaceLounge.cpp` | 4438 | 4500 | 62 | 4/31 | 0x41b67/4949 | 0x2aac/size | 0x69bc |
| `game/mission/Mission.cpp` | 4480 | 4500 | 20 | 10/55 | 0x41b67/0 | 0x296c/0 | 0x69bc |
| `game/ui/TouchButton.cpp` | 4484 | 4500 | 16 | 7/44 | 0x41b67/0 | 0x296c/0 | 0x69bc |
| `game/core/AutoPilotList.cpp` | 4486 | 4500 | 14 | 3/10 | 0x41b67/0 | 0x296c/623 | 0x69bc |
| `game/ship/KIPlayer.cpp` | 4486 | 4500 | 14 | 2/61 | 0x41b67/0 | 0x296c/0 | 0x69bc |
| `game/ui/MissionsWindow.cpp` | 4486 | 4500 | 14 | 3/13 | 0x41b67/0 | 0x296c/512 | 0x69bc |
| `game/world/Station.cpp` | 4486 | 4500 | 14 | 2/42 | 0x41b67/0 | 0x296c/0 | 0x69bc |
| `game/ship/PlayerFixedObject.cpp` | 4487 | 4500 | 13 | 2/31 | 0x41b67/0 | 0x296c/0 | 0x69bc |
| `game/core/Radio.cpp` | 4488 | 4500 | 12 | 1/11 | 0x41b67/0 | 0x296c/0 | 0x69bc |
| `game/world/LevelScript.cpp` | 4489 | 4500 | 11 | 0/17 | 0x41b67/0 | 0x296c/482 | 0x69bc |
| `game/ui/ScrollTouchWindow.cpp` | 4491 | 4500 | 9 | 1/16 | 0x41b67/0 | 0x296c/0 | 0x69bc |
| `game/mission/Status.cpp` | 4496 | 4500 | 4 | 1/138 | 0x41b67/0 | 0x296c/1492 | 0x69ac |
| `engine/audio/AESoundRessource.cpp` | 4498 | 4500 | 2 | 2/47 | 0x41b67/0 | 0x296c/0 | 0x69ac |
| `game/menu/MiningGame.cpp` | 4499 | 4500 | 1 | 1/16 | 0x41b67/0 | 0x296c/566 | 0x69ac |
| `game/ship/PlayerAsteroid.cpp` | 4499 | 4500 | 1 | 1/24 | 0x41b67/0 | 0x296c/1310 | 0x69ac |
| `game/ui/ScrollTouchBox.cpp` | 4499 | 4500 | 1 | 1/17 | 0x41b67/0 | 0x296c/482 | 0x69ac |
