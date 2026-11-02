# `.rel.plt` / `.rel.dyn` ordering (w3616)

Status: research note + landed source/TU-order changes (2026-11-02). Worktree
`wt/w3616`, branch `wave87-w3616`, based on master `2b54aefe`. Every number below is
from a command run against `../_work/bins/android_2.0.16_libgof2hdaa.so` or a
controlled rebuild of this tree. No baseline was updated.

## TL;DR

At the starting HEAD the two relocation sections were **pure permutations of the
original** (same offsets, types and `(offset,type,name)` multisets) but differed in
`r_info` symbol indices and, for `.rel.plt`, in six name-order entries. The remaining
differences were entirely downstream of:

1. `.dynsym` first-mention order for `RecordHandler::recordStoreWrite` /
   `recordStoreWritePreview` and the touch globals;
2. the libzip member order (`_zip_err_str`), which also controls the LIBC namepool
   key `V` and therefore the 79 versioned imports;
3. a `LevelScript::process` call-site order (`freeMissionLock`);
4. one `LevelScript::process` call-site order (`deleteHackingGame`), still open.

Landed changes:

* move `ndk_autosave` into `NdkHooks.cpp` after `checkFirstCreditPackBoughtWriteAction`
  (and move `ndk_checkPlaytime...` after it so clang emits `ndk_autosave` at its
  definition, not first reference);
* move `pConstToNonConst` to `NdkAutosave.cpp` (the original's next TU starts at
  `0x947f0`; this keeps the TU boundary and its `.ARM.exidx` marker exact);
* keep the touch globals in `NdkAutosave.cpp` and add a `gen_link_order.py` override
  placing `NdkAutosave.cpp` immediately after `NdkHooks.cpp`;
* move `freeMissionLock()` before `switchSunForSupernovaIntro()` in
  `LevelScript::process`;
* reorder the libzip archive members so `zip_err_str` follows
  `zip_add_dir`/`zip_source_free`/`zip_unchange_archive`.

Measured before/after (full `check` gate):

```
before: byte 4496  linked 4496  .text 0x1337c8  lint CLEAN  drift 0
after:  byte 4496  linked 4496  .text 0x1337c8  lint CLEAN  drift 0
```

The gate count is unchanged because no function's instruction stream changes; the
improvement is in the relocation/dynsym bytes:

| section | before diff | after diff |
|---|---:|---:|
| `.rel.dyn` | 52 | **0** |
| `.rel.plt` | 56 | **15** |
| `.dynsym` | 477 | **62** |
| `.dynstr` | 2591 | **345** |
| `.hash` | 252 | **42** |
| `.gnu.version` | 2 | **0** |
| `.text` | 119626 | **119607** |

`.plt`, `.got`, `.rodata`, `.data`, `.data.rel.ro` remain byte-identical; every
section size/base is unchanged (`.ARM.extab` remains the pre-existing -8 bytes).

## 1. Baseline and method

```
cmake --preset default
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

Starting HEAD (`2b54aefe`, clean):

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
```

Relocation/section diff script (reads both `.so` files directly):

```python
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
O = '../_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
def sec(p, n):
    e = Elf32(p); s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset']+s['size']]
for n in ('.rel.dyn', '.rel.plt', '.dynsym', '.dynstr', '.hash', '.gnu.version',
          '.plt', '.got', '.text', '.rodata', '.data', '.data.rel.ro',
          '.ARM.exidx', '.ARM.extab'):
    a, b = sec(O, n), sec(U, n)
    print('%-14s %d/%d diff %d' % (n, len(a), len(b),
          sum(x != y for x, y in zip(a, b)) + abs(len(a)-len(b))))
```

Relocation entry comparison (`readelf -rW` + `.dynsym` name lookup) shows, at the
starting HEAD:

```
R_ARM_JUMP_SLOT 3674/3674  same offset 3674  same info 3464  same name 3645
R_ARM_ABS32     1817/1817  same offset 1817  same info 1817  same name 1817
R_ARM_GLOB_DAT   614/614   same offset 614   same info 575   same name 614
```

`.rel.dyn` is a pure permutation: the `(offset,type,name)` multiset is exactly equal.
`.rel.plt` has 3674 identical offsets/types; 181 entries differ only in `r_info`
symbol index and 29 entries differ in name order (two groups: 8 `Level.cpp`, 21
`LevelScript.cpp`). This is the same decomposition as the earlier
`data_rel_dyn_plt_finish_w3456.md` / `rel_plt_level_levelscript_w3461.md` notes, but
at the current 4496 baseline.

## 2. Controlled experiments

All experiments were full rebuilds (`link_so` + `check`); no `--update-baseline`.
The four groups were also tested individually from HEAD:

| # | change | byte | linked | `.rel.dyn` | `.rel.plt` | `.dynsym` | `.dynstr` | `.hash` | `.gnu.version` | `.text` |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | HEAD | 4496 | 4496 | 52 | 56 | 477 | 2591 | 252 | 2 | 119626 |
| T | touch globals -> `NdkAutosave.cpp` | 4496 | 4496 | 45 | 43 | 375 | 2057 | 215 | 2 | 119626 |
| L | `freeMissionLock` before `switchSun...` | 4496 | 4496 | 52 | 52 | 477 | 2591 | 252 | 2 | 119607 |
| Z | libzip `zip_err_str` member order | 4496 | 4496 | 46 | 50 | 420 | 2408 | 222 | **0** | 119626 |
| T+L+Z | combined | 4496 | 4496 | 39 | 33 | 318 | 1874 | 185 | 0 | 119607 |
| M | merge `ndk_autosave` into `NdkHooks` (no anchor function) | **4040** | 4496 | 0 | 15 | 257 | 345 | 42 | 0 | 124325 |
| M+`pConstToNonConst` | **final** | **4496** | **4496** | **0** | **15** | **62** | **345** | **42** | **0** | **119607** |

Experiment M is the key: moving `ndk_autosave` into `NdkHooks.cpp` makes
`recordStoreWrite`/`recordStoreWritePreview` first-mentioned in the same object as
`saveOptions`, exactly as the original `.dynsym` (indices 1984-1988:
`saveOptions, recordStoreWrite, recordStoreWritePreview, C1, D1`). It also makes
`.rel.dyn` exact. But with no real function left in `NdkAutosave.cpp`, the link-order
generator moves that object to the end (it has no strong `.text` symbol), the touch
globals move to dynsym 1907-1910, and `.ARM.exidx` grows by 8 bytes, which shifts
`.rodata` and costs 456 `byte_exact`.

The anchor-function experiment fixes that without moving the touch globals:

| anchor moved to `NdkAutosave.cpp` | byte | `.ARM.exidx` | `.rel.dyn` | `.rel.plt` | `.dynsym` | `.text` | lint |
|---|---:|---:|---:|---:|---:|---:|---|
| `ndk_getCurrentApplicationModule` | 4496 | 0x6f78 | 0 | 15 | 62 | 119607 | CLEAN |
| `ndk_getLogoShown` | — | 0x6f78 | 22 | 24 | 258 | 119666 | — |
| `ndk_isInMainMenu` | — | 0x6f70 | 22 | 24 | 257 | 119661 | — |
| `pConstToNonConst` | **4496** | **0x6f70** | **0** | **15** | **62** | **119607** | **CLEAN** |
| `getStringUTFChars` | — | 0x6f70 | 6 | 23 | 170 | 119607 | — |
| `releaseStringUTFChars` | 4496 | 0x6f70 | 0 | 15 | 62 | 119607 | `operator_call` +2 |

`pConstToNonConst` is the correct anchor: the original `.text` order has
`ndk_isInMainMenu` at `0x947e0` and `pConstToNonConst` at `0x947f0`, a 4-byte-aligned
TU boundary. Keeping `pConstToNonConst` as the first function of the touch-globals TU
reproduces the original `.ARM.exidx` marker at that boundary (the marker merges with
the real function's entry), so `.ARM.exidx` stays `0x6f70` and `.rodata` stays at
`0x1c9e10`. `releaseStringUTFChars` gives the same bytes but moves two
`operator delete` calls into `NdkAutosave.cpp`, tripping the per-file soft-lint
ratchet; `pConstToNonConst` has no such calls.

Dropping `NdkAutosave.cpp`'s empty `.text` marker instead of adding an anchor gives
`.ARM.exidx 0x6f68` (two entries short) and byte 4040, so the anchor is required.

## 3. What the final state contains

### 3.1 `.rel.dyn` is byte-identical

All 3593 entries match `(offset, type, name)` *and* `r_info` symbol index. The
`R_ARM_GLOB_DAT` block that used to differ (Globals, touch globals, LODManager,
ResourceMaterial, RecordHandler) is gone: the RecordHandler first-mention order now
matches, and the touch globals land at dynsym 2049-2051, exactly the original.

### 3.2 `.rel.plt` has 15 bytes left

Current entry comparison:

```
R_ARM_JUMP_SLOT 3674/3674
  same offset 3674
  same info   3665
  same name   3668
  same-name/info-diff 3
  name-order opcodes 2:
    insert 2831 ours ['_ZN9PlayerEgo17deleteHackingGameEv']
    delete 2836 orig ['_ZN9PlayerEgo17deleteHackingGameEv']
```

The 3 same-name index differences are the two `ResourceMaterial` ctors and the
`ResourceTexture(String const&, float)` ctor:

```
2527 ResourceMaterial::ResourceMaterial(unsigned short, unsigned short, BlendMode)
2529 ResourceMaterial::ResourceMaterial(unsigned short, BlendMode)
2535 ResourceTexture::ResourceTexture(String const&, float)
```

They are downstream of the two remaining `.dynsym` gaps (§3.3). The 6 name-order
entries are one symbol: `PlayerEgo::deleteHackingGame` is at PLT slot 2831 in ours
and 2836 in the original. In the original `LevelScript::process`, the first
`PlayerStation::setVisible` call is at `0x13da1e` and the `deleteHackingGame` call is
at `0x13ded4`; in ours they are at `0x13e49e` and `0x13d15a`, i.e. the source case
order is still reversed. This is a `LevelScript::process` reconstruction issue, not a
linker/dynsym issue.

### 3.3 `.dynsym` has two defined-symbol gaps

```
defined 5370/5370  same index 5355
delete orig 3879 ['_ZN11AbyssEngine15ResourceTextureC2ERKNS_6StringEf']
insert orig 3882 [] ours 3881 [...]
insert orig 5071 [] ours 5071 ['sha256_block_data_order_neon']
delete orig 5082 ['sha256_block_data_order_neon'] ours 5083 []
versioned 79/79 same index 79
```

* `ResourceTexture(String const&, float)` is an in-class weak ctor emitted by
  `ResourceMaterial.cpp` (our `loadPortraits`). The original emits it in the
  `GameResource` TU (`BuildResourceList.cpp`), which also contains `loadPortraits`.
  Moving `loadPortraits` to `BuildResourceList.cpp` was already measured at -1380
  `byte_exact` because it moves the portrait string literals; no codegen-neutral
  emission anchor was found.
* `sha256_block_data_order_neon` is in libcrypto's `sha256-armv4.o`. The original
  first mentions it after the linker-provided `_edata`/`_end`/`__bss_start` and
  libc++abi's `__cxa_begin_cleanup`/`__cxa_call_unexpected`; ours mentions it before
  them. Moving the whole libcrypto archive later would change the LIBC namepool key
  `V` (currently 7149) and break the 79 versioned imports, so this needs an
  archive-member-level fix, not a link-order swap.

The 79 versioned imports are now at the same indices (`V = 7149`), which is why
`.gnu.version` is byte-identical and `.rel.dyn`/`.rel.plt` no longer carry the old
versioned-index deltas.

## 4. Proven vs hypothesis

**Proven by the measurements above:**

* `.rel.dyn` is now byte-identical; `.rel.plt` is down to 15 differing bytes, all
  accounted for by 3 Resource* symbol indices and the `deleteHackingGame` slot.
* The RecordHandler first-mention block is fixed by putting `ndk_autosave` in
  `NdkHooks.cpp` next to `checkFirstCreditPackBoughtWriteAction`, and by defining
  `ndk_checkPlaytime...` after `ndk_autosave` so clang emits the body at
  `0x94770` (definition) rather than `0x942aa` (first reference).
* The touch-globals TU must remain between `NdkHooks.cpp` and `LODManager.cpp`;
  `gen_link_order.py` now enforces that position even though the object has no
  strong `.text` function of its own.
* `pConstToNonConst` at `0x947f0` is the original TU boundary: anchoring
  `NdkAutosave.cpp` there keeps `.ARM.exidx` at `0x6f70` and `.rodata` at
  `0x1c9e10`. `ndk_getCurrentApplicationModule` as the anchor gives `.ARM.exidx
  0x6f78`; dropping the marker gives `0x6f68`.
* The libzip member reorder makes `_zip_err_str` first-mentioned in the original
  position and restores `V = 7149`, making all 79 versioned imports exact.
* `freeMissionLock` before `switchSunForSupernovaIntro` fixes the corresponding
  4 `.rel.plt` bytes with no gate change.

**Hypothesis / still open:**

* The original `NdkHooks.cpp` TU contains `checkFirstCreditPackBoughtWriteAction`,
  `ndk_autosave` and `ndk_checkPlaytime...`; the touch globals and
  `pConstToNonConst` belong to the next TU. The `.text`/`.ARM.exidx` boundary and
  the `.dynsym` first-mention order are consistent with this, but the original
  source file names are not recoverable from the stripped binary.
* `deleteHackingGame` can be fixed only by reproducing the original
  `LevelScript::process` case/block order (the original first `PlayerStation::
  setVisible` call precedes `deleteHackingGame`; ours is the reverse).
* `ResourceTexture(String)` and `sha256_block_data_order_neon` need
  `GameResource`/`loadPortraits` emission and libcrypto archive-member work
  respectively; both are outside the relocation-order knob.

## 5. Reproduction

```sh
cmake --preset default
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check   -j8
cmake --build cmake-build-match --target lint    -j8
cmake --build cmake-build-match --target drift   -j8
```

Relocation section diff: the Python snippet in §1.

Relocation entry comparison:

```python
import subprocess, re, sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
def syms(p): return [s['name'] for s in Elf32(p)._symbols(SHT_DYNSYM)]
def rels(p):
    out = subprocess.check_output(['readelf', '-rW', p], text=True)
    rows = []
    for line in out.splitlines():
        m = re.match(r'^([0-9a-f]+)\s+([0-9a-f]+)\s+(R_ARM_\w+)\s+([0-9a-f]+)\s*(.*)$', line)
        if m:
            rows.append((int(m.group(1),16), int(m.group(2),16), m.group(3),
                         int(m.group(4),16), m.group(5).strip()))
    return rows
O = '../_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
so, su = syms(O), syms(U)
ro, ru = rels(O), rels(U)
for typ in ('R_ARM_JUMP_SLOT', 'R_ARM_ABS32', 'R_ARM_GLOB_DAT'):
    o = [r for r in ro if r[2] == typ]
    u = [r for r in ru if r[2] == typ]
    print(typ, len(o), len(u),
          'same offset', sum(a[0] == b[0] for a, b in zip(o, u)),
          'same info',   sum(a[1] == b[1] for a, b in zip(o, u)),
          'same name',   sum(so[a[1]>>8] == su[b[1]>>8] for a, b in zip(o, u)))
```

`.dynsym` gaps: `difflib.SequenceMatcher(None, orig_defined, ours_defined,
autojunk=False)` over the 5370 defined names; see
`_work/reconstructions/dynsym_dynstr_hash_order_w3567.md` for the parser.

PLT name sequence:

```sh
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .plt ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -o '<[^>]*@plt>:' | sed 's/[<>:]//g' > /tmp/plt_o.txt
$OD -d -j .plt cmake-build-match/libgof2hdaa.so        | grep -o '<[^>]*@plt>:' | sed 's/[<>:]//g' > /tmp/plt_u.txt
diff /tmp/plt_o.txt /tmp/plt_u.txt
```

The final diff is exactly one moved symbol (`PlayerEgo::deleteHackingGame`).
