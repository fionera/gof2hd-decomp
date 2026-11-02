# `.dynsym` within-TU first-mention (w3617)

Status: research note (2026-11-02). Worktree `wt/w3617`, branch `wave87-w3617`, based on
current `master` (`6dabaad1`). Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with this tree's NDK r18b gold 1.12
(`GNU gold (GNU Binutils 2.27.0.20170315) 1.12`). No source change was landed: no controlled
experiment was a strict improvement.

## TL;DR

* **Within a clang 7.0.2 TU, the global `.symtab` is sorted lexicographically by mangled
  name.** All **195/195** app objects in `link_order.rsp` satisfy this, as do the vendored
  objects built with clang (`libzip` 54/54, `libz` 11/11, `libc++abi` 19/19,
  `libandroid_support` 66/66). The GCC-built `libcrypto` (1/3) and `libgcc` (634/1751) are
  the exceptions: their symtabs are source/definition order.
* Therefore **source order inside a TU does not affect `.dynsym`**: gold's `.dynsym` order is
  the first-mention order of global names over the link inputs, and a TU contributes its
  symbols as one lexicographically-sorted block. The original's `NdkHooks` block (74 defined
  symbols) and `FModSound` block (47) are exactly sorted, confirming this on the ground truth.
* The remaining **10 defined-symbol gaps are all cross-TU first-mention differences**, not
  within-TU order differences. The three app groups are:
  `RecordHandler::recordStoreWrite/Preview` (ours first-mentioned by `NdkAutosave.cpp`,
  original by `NdkHooks.cpp`), `curTouchSize/maxTouchSize/touches` (ours by `LODManager.cpp`,
  original by the object between `NdkHooks.cpp` and `LODManager.cpp`, i.e. `NdkAutosave.cpp`),
  and `ResourceTexture::ResourceTexture(String const&, float)` (ours by `ResourceMaterial.cpp`,
  original by the `GameResource`/`BuildResourceList.cpp` TU). The other four are archive
  member pull order (`libzip` `_zip_err_*`, `libcrypto` `sha256_block_data_order_neon`).
* **Within-TU source order does matter for GOT/PLT order**, because gold assigns those slots
  from the first relocation in object-section order. A controlled swap of two adjacent
  functions in `NdkHooks.cpp` left `.dynsym` byte-for-byte unchanged (same 5263 same-index,
  10 gaps) but moved `Globals::logoIsShown`'s GOT slot by 4 bytes and dropped
  `byte_exact` 4496 -> 4492. This is the real "within-TU first-mention" effect.
* All three app-region fixes were re-measured at this HEAD. Each fixes its `.dynsym` group
  but costs `byte_exact` through GOT/PLT/alignment: RecordHandler merge **-519**,
  touch globals move **-11**, `loadPortraits` move **-1380**. None is a strict improvement.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

`.text` is `0x1337c8` at `0x69440`; `report.json` is `compared 4524`, `byte_exact 4496`,
`linked_exact 4496`, `size 4524/4524`.

Section byte diff vs the original (same sizes and bases):

| section | size | differing bytes |
|---|---:|---:|
| `.text` | 1,259,464 | 119,592 |
| `.rodata` | 269,159 | **0** |
| `.data` | 10,604 | **0** |
| `.data.rel.ro` | 12,264 | **0** |
| `.got` | 17,164 | **0** |
| `.plt` | 44,108 | **0** |
| `.dynsym` | 89,248 | 477 |
| `.dynstr` | 189,208 | 2,591 |
| `.hash` | 38,716 | 252 |
| `.gnu.version` | 11,156 | 2 |
| `.rel.dyn` | 28,744 | 52 |
| `.rel.plt` | 29,392 | 56 |
| `.ARM.exidx` | 28,528 | 2,710 |
| `.ARM.extab` | 156,312 | 119,161 (size -8) |

## 2. The rule: two different "first mention" consumers

Gold's `Symbol_table` is an `Unordered_map<(name_key, version_key), Symbol*>` with
`number_of_input_files()*1024` buckets, so iteration is insertion order (see
`dynsym_dynstr_hash_order_w3567.md`). `add_from_relobj` walks each object's **global
`.symtab` in ELF order**. For an unversioned symbol `hash = name_key`, so the output order is
exactly the first time a name is mentioned in the input stream.

There are two independent consumers of "first":

1. **`.dynsym` order** — the object's global `.symtab` order. clang 7.0.2 MC emits that table
   sorted by mangled name, so source order is invisible.
2. **GOT/PLT slot order** — gold assigns slots from the first relocation it scans, which is
   the object's **section order** (clang emission order). Source order *is* visible here.

### 2.1 Evidence: clang symtabs are lexicographic

```
python3 - <<'PY'
import sys
sys.path.insert(0, '_work/reconstructions/w3560')
from symtab import parse
objs = [l.strip() for l in open('cmake-build-match/link_order.rsp') if l.strip()]
lex = sum(1 for o in objs
          if (lambda g: g == sorted(g))([s['name'] for s in parse(o)
                                         if s['name'] and s['bind'] in (1, 2)]))
print('app objects lexicographic:', lex, '/', len(objs))
PY
```

Measured: `app objects lexicographic: 195 / 195`.

Archive members (same check over `ar p`):

| archive | members | lexicographic symtab | non-lex |
|---|---:|---:|---:|
| `libzip_gof2.a` | 54 | 54 | 0 |
| `libz_gof2.a` | 11 | 11 | 0 |
| `libc++abi.a` | 19 | 19 | 0 |
| `libandroid_support.a` | 66 | 66 | 0 |
| `libcrypto_gof2.a` | 3 | 1 | 2 (`armv4cpuid.o`, `sha256.o`) |
| `libgcc.a` | 1751 | 634 | 1117 |

Example non-lexicographic GCC object (`libcrypto` `armv4cpuid.o`):

```
OPENSSL_atomic_add, OPENSSL_cleanse, CRYPTO_memcmp, _armv7_neon_probe, ...
```

This is source/definition order, not ASCII order. So the "within-TU order is lexicographic"
rule is clang-specific; the vendored GCC objects are the exception, and their order is fixed
by their own source, not by our TU layout.

### 2.2 Evidence: the original's TU blocks are lexicographically sorted

The original's defined `.dynsym` `NdkHooks` block is indices `1817..1890` (74 symbols,
`ExitFunction` .. `zip_open`) and is **exactly sorted**; the `FModSound` block
(`1770..1816`, 47 symbols) is likewise sorted. This is the ground-truth witness that a single
object's contribution is one sorted block, and that the original's `NdkHooks.cpp` block
contains `recordStoreWrite`/`recordStoreWritePreview` (they sort between `saveOptions` and
`RecordHandler::C1`).

```
python3 - <<'PY'
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
o = [s['name'] for s in Elf32('../_work/bins/android_2.0.16_libgof2hdaa.so')._symbols(SHT_DYNSYM)
     if s['shndx'] != 0]
print('NdkHooks block sorted:', o[1817:1891] == sorted(o[1817:1891]))  # True
print('FModSound block sorted:', o[1770:1817] == sorted(o[1770:1817])) # True
PY
```

## 3. The 10 remaining defined-symbol gaps are cross-TU

`difflib.SequenceMatcher(autojunk=False)` over the 5370 defined names: **5263 same index,
5360 matched, 10 gaps**. First-mention object in our link (from `link_order.rsp`):

| # | symbol(s) | orig idx | ours idx | our first-mention TU | original first-mention TU |
|---:|---|---:|---:|---|---|
| 1 | `RecordHandler::recordStoreWrite(int)`, `recordStoreWritePreview(int)` | 1827–1828 | 1889–1890 | `NdkAutosave.cpp` (link pos 83) | `NdkHooks.cpp` (pos 82), same object as `saveOptions`/`C1` |
| 2 | `curTouchSize`, `maxTouchSize`, `touches` | 1891–1893 | 1907–1909 | `LODManager.cpp` (pos 84) | the object between `NdkHooks.cpp` and `LODManager.cpp` = `NdkAutosave.cpp` (pos 83) |
| 3 | `ResourceTexture::ResourceTexture(String const&, float)` | 3879 | 3881 | `ResourceMaterial.cpp` (pos 119) | `GameResource.cpp` = `BuildResourceList.cpp` (pos 117) |
| 4 | `_zip_err_str`, `_zip_err_type`, `_zip_nerr_str` | 5030–5036 | 5028–5030 | `libzip` `zip_err_str.o` (m016) | a later `libzip` member pull |
| 5 | `sha256_block_data_order_neon` | 5082 | 5071 | `libcrypto` `sha256-armv4.o` (m002) | `libcrypto` `sha256-armv4.o` pulled after `armv4cpuid.o` (m000) |

Groups 1–3 are app TUs; groups 4–5 are archive member pull order. **None is a within-TU
ordering difference** — the symbols are lexicographically placed inside their first-mention
object in both binaries. The only lever is *which object mentions the name first*.

Group 1 is the clearest proof: the original's `NdkHooks` block is the lexicographic union of
our `NdkHooks.obj` and `NdkAutosave.obj` symtabs. The original's `NdkHooks.cpp` must therefore
contain the `ndk_autosave` body (or at least its `RecordHandler` references); our split into a
separate `NdkAutosave.cpp` is what moves the two names to index 1889–1890.

## 4. Controlled A/B experiments

All builds are the full `check` target; "before" is the unmodified baseline
(byte 4496 / linked 4496, `.text 0x1337c8`). No `--update-baseline` was used.

| # | change | byte | linked | size | dynsym same / gaps | other observable | verdict |
|---|---|---:|---:|---:|---:|---|---|
| E1 | swap `ndk_getLogoShown` / `ndk_isInMainMenu` definitions in `NdkHooks.cpp` | **4492** | 4496 | 4524/4524 | **5263 / 10 (unchanged)** | `.dynsym` byte-identical; `Globals::logoIsShown` GOT slot +4; `ndk_getLogoShown`, `ndk_isInMainMenu`, 2 `ModMainMenu` fns lose byte | not landed |
| E2 | merge `NdkAutosave.cpp` into `NdkHooks.cpp` (delete the TU) | **3977** | 4496 | 4524/4524 | 5327 / **9** | RecordHandler group fixed; PLT slot for `recordStoreWrite` moves 1017 -> 1009 | not landed |
| E3 | move `touches`/`maxTouchSize`/`curTouchSize` definitions to `NdkAutosave.cpp` | **4485** | 4496 | 4524/4524 | 5282 / **9** | touch group fixed; `.bss` addresses and `.init_array` stay exact; `touches` GOT slot moves to `0x2101e4` (was `0x2101f0`); 11 fns lose byte | not landed |
| E4 | move `loadPortraits` to end of `BuildResourceList.cpp` | **3116** | 4496 | 4524/4524 | 5266 / **8** | ResourceTexture group fixed; `.rodata` string order changes (portrait strings move before low-resource strings) | not landed |
| E5 | merge `LowResourceTable.cpp` + `loadPortraits` into `BuildResourceList.cpp` (low-resource before portraits) | **3116** | 4496 | 4524/4524 | 5266 / **8** | ResourceTexture group fixed; `.rodata` stays **0 diff**; 2-byte TU-start alignment shift at `0x11b70e` moves `SystemPathFinder` +1651 functions by 2 bytes; `.data` 72 / `.data.rel.ro` 167 bytes differ | not landed |

### E1 — within-TU source order: `.dynsym` unchanged, GOT moved

Change: swap the two adjacent definitions

```cpp
int ndk_getLogoShown() { return Globals::logoIsShown; }
int ndk_isInMainMenu() { return Globals::isInMainMenu; }
```

The section-ordering file lists both functions by name, so their `.text` addresses are
unchanged (`0x947d0`, `0x947e0`). The object's global symtab is lexicographic, so `.dynsym` is
unchanged (5263 same index, 10 gaps). But the first relocation in the object now references
`Globals::logoIsShown` before `Globals::isInMainMenu`, so its GOT slot moves:

```
ndk_getLogoShown
  orig 16B @0x947d0  |  ours 16B @0x947d0
  ...
  .word 0x0017b99a   ~ .word 0x0017b99e   (GOT slot +4)
```

This is the clean separation: **source order inside a TU is invisible to `.dynsym` but visible
to GOT/PLT**.

### E2 — RecordHandler merge

Change: append the `ndk_autosave` body to `NdkHooks.cpp`, delete `NdkAutosave.cpp`.
The dynsym group is fixed (gaps 10 -> 9), but clang emits `ndk_autosave`'s section at its first
call site (`ndk_checkPlaytimeAndSpendOfferwallCredits`) rather than after
`checkFirstCreditPackBoughtWriteAction`, so the `recordStoreWrite`/`recordStoreWritePreview`
PLT slots move from 1017/1018 to 1009/1010 and 519 functions lose byte-exactness.

### E3 — touch globals move

Change: move the three definitions to `NdkAutosave.cpp`, leave `extern` declarations in
`LODManager.cpp`. The `.dynsym` group is fixed (gaps 10 -> 9), `.bss` addresses stay exact
(`0x217b90/94/98`) and `.init_array` stays byte-identical (`0x69441/0x694c1/0x694dd`). The
cost is the `touches` GOT slot: its self-reference in the static initializer is now scanned at
`NdkAutosave.cpp`'s position, so it takes slot `0x2101e4` instead of `0x2101f0`; 11 functions
that use the touch globals lose byte-exactness.

### E4/E5 — `loadPortraits`

Change: move the whole `loadPortraits` body to `BuildResourceList.cpp`. The
`ResourceTexture(String const&, float)` ctor is then first mentioned by the `GameResource` TU,
fixing the dynsym group (gaps 10 -> 8). Moving only `loadPortraits` changes the `.rodata`
string order (portrait strings move ahead of the low-resource strings) and costs 1380
byte-exact. Merging `LowResourceTable.cpp` in as well, with `loadLowTexturesAndMaterials`
before `loadPortraits`, restores `.rodata` to **0 diff**, but the merged TU's last function
(`ResourceTexture(String)`) ends at `0x11b70e` and the next TU (`SystemPathFinder`) starts
2-byte aligned at `0x11b70e` instead of the original's 4-byte-aligned `0x11b710`; that 2-byte
shift propagates through 1651 functions and still costs 1380 byte-exact.

## 5. Proven vs hypothesis

**Proven (measured above):**

* All 195 app objects' global symtabs are lexicographically sorted by mangled name; the same
  holds for the clang-built vendored archives. The GCC `libcrypto`/`libgcc` members are
  source-order exceptions.
* The original's `NdkHooks` (74) and `FModSound` (47) defined-symbol blocks are exactly
  lexicographically sorted, so a TU's `.dynsym` contribution is one sorted block.
* The 10 remaining defined-symbol gaps are all cross-TU first-mention differences; none is a
  within-TU ordering difference.
* Swapping two function definitions inside a TU leaves `.dynsym` byte-identical but moves a
  GOT slot and costs 4 byte_exact (E1). This separates the two "first mention" consumers.
* Every app-region fix (E2/E3/E4/E5) fixes its dynsym group but regresses `byte_exact`
  (-519 / -11 / -1380 / -1380); none is a strict improvement.

**Hypothesis / not yet proven:**

* The original's `NdkHooks.cpp` contains the `ndk_autosave` body, but clang 7.0.2 emits a
  function at its first call site, so a same-TU definition would be emitted before
  `checkFirstCreditPackBoughtWriteAction`, not after it. The original's `.text` order
  (`checkFirst` `0x94708`, `ndk_autosave` `0x94770`) therefore needs either a section-ordering
  file or a different source shape; this was not found.
* The original's `libcrypto` archive pull order is `sha256.o`, `armv4cpuid.o`,
  `sha256-armv4.o`; ours is `sha256.o`, `sha256-armv4.o`, `armv4cpuid.o`. The pull order is
  driven by gold's undefined-symbol resolution, not by the archive's member order
  (`armv4cpuid.o`, `sha256.o`, `sha256-armv4.o` in both), so a member reorder alone will not
  fix it.

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3617
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# defined-symbol alignment + gaps
python3 - <<'PY'
import sys, difflib
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
def defined(p):
    return [s['name'] for s in Elf32(p)._symbols(SHT_DYNSYM) if s['shndx'] != 0]
o = defined('../_work/bins/android_2.0.16_libgof2hdaa.so')
u = defined('cmake-build-match/libgof2hdaa.so')
sm = difflib.SequenceMatcher(None, o, u, autojunk=False)
print('same', sum(a == b for a, b in zip(o, u)), '/', len(o),
      'gaps', sum(1 for x in sm.get_opcodes() if x[0] != 'equal'))
PY

# lexicographic symtab check (app objects)
python3 - <<'PY'
import sys
sys.path.insert(0, '_work/reconstructions/w3560')
from symtab import parse
objs = [l.strip() for l in open('cmake-build-match/link_order.rsp') if l.strip()]
lex = sum(1 for o in objs
          if (lambda g: g == sorted(g))([s['name'] for s in parse(o)
                                         if s['name'] and s['bind'] in (1, 2)]))
print('app objects lexicographic:', lex, '/', len(objs))
PY

# first-mention object for a name
python3 - <<'PY'
import sys, os
sys.path.insert(0, '_work/reconstructions/w3560')
from symtab import parse
objs = ['cmake-build-match/crtbegin_so_renamed.o']
objs += [l.strip() for l in open('cmake-build-match/link_order.rsp') if l.strip()]
first = {}
for pos, o in enumerate(objs):
    for s in parse(o):
        n = s['name']
        if n and s['bind'] in (1, 2) and n not in first:
            first[n] = (pos, os.path.basename(o))
for n in ('_ZN13RecordHandler16recordStoreWriteEi', 'curTouchSize',
          '_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf'):
    print(n, first.get(n))
PY
```
