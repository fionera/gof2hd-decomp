# libc++abi `.text` placement: the -0x24 is a libzip TU-alignment deficit

Status: research note + fix (2026-11-01). Worktree `wt/w3460`, branch `wave87-w3460`,
based on master `bd1a1bb7`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2,
`-Oz`, image base 0x10000). Every number below is from a command run in this tree or
from the original binary.

## TL;DR

The `.data` / `.data.rel.ro` "libc++abi pointer" delta is **not** a libc++abi archive
difference. It is the cumulative result of the **libzip archive members being laid out
with 2-byte TU starts instead of the original's 4-byte TU starts**:

```
libzip block (0x18c550..sha256.o)   original 0x31e0   ours 0x31c4   -0x1c
  + 4 B end-padding before libcrypto armv4cpuid.o (alignment side effect)   -0x20
  + two 2-byte alignment pads inside libc++abi stdlib_exception.o           -0x24
```

At that point `demangling_terminate_handler` / `demangling_unexpected_handler` (the two
`.data` words) are exactly `-0x24`. The 0x24-byte `levelTextAnchor()` padding anchor in
`AbyssEngine.cpp` was compensating for this. Fixing the libzip TU alignment and removing
the anchor makes `.data` **byte-identical** and `.data.rel.ro` drop from 667 to 21
differing bytes.

Gate: **byte_exact 4265 -> 4408, linked_exact 4496 -> 4496** (`.text` size still exact).

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4265  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` = `CLEAN ✓`, `drift` = `0 field(s) across 0 class(es)`, `diff` = symbol parity
(5370/5370, extra 0, missing 0).

Raw section content at this baseline:

| section | differing bytes | what |
|---|---:|---|
| `.data` | **3** | 2 libc++abi handler pointers (`demangling_terminate_handler`, `demangling_unexpected_handler`), both `-0x24` |
| `.data.rel.ro` | **667** | 599 libc++abi `.text` pointers; 9 `Level::createScene` table pointers; 3 `.bss` pointers |

## 2. The -0x24 chain (measured against the original)

`nm -D -n` on both files gives the libc++abi block start and the demangler:

```
orig  __cxa_end_cleanup 0x191d20   __cxa_demangle 0x194030
ours  __cxa_end_cleanup 0x191d00   __cxa_demangle 0x19400c     (-0x20, -0x24)
```

Tracing the delta backwards with the unstripped relink + gold `--Map`:

| address (orig / ours) | symbol | delta | cause |
|---|---|---|---:|---|
| `0x18ca54` / `0x18ca52` | `zip_get_archive_comment` | +0x2 | first libzip TU start |
| `0x18cbd4` / `0x18cbd0` | `zip_unchange` | +0x4 | next libzip TU start |
| `0x18db24` / `0x18db18` | `_zip_error_clear` | +0xc | next libzip TU start |
| `0x18f618` / `0x18f5fc` | `_zip_error_strerror` | +0x1c | end of libzip block |
| `0x18fe40` / `0x18fe20` | `OPENSSL_atomic_add` | +0x20 | 4 B pad after `sha256.o` |
| `0x191d20` / `0x191d00` | `__cxa_end_cleanup` | +0x20 | libc++abi block start |
| `0x193c10` / `0x193bee` | `_ZNSt13bad_exceptionD1Ev` | +0x22 | 2 B pad before `stdlib_exception.o` |
| `0x193c18` / `0x193bf4` | `_ZNKSt9exception4whatEv` | +0x24 | 2 B pad inside `stdlib_exception.o` |
| `0x193f24` / `0x193f00` | `demangling_terminate_handler` | +0x24 | `.data` pointer |
| `0x194030` / `0x19400c` | `__cxa_demangle` | +0x24 | demangler start |

The two internal `+0x2` steps are pure alignment pads: in the original the
`stdlib_exception.o` block starts at `0x193c10` (16-aligned), so `_ZNSt9exceptionD0Ev`
(4 B at `0x193c12`) ends at `0x193c16` and the next function is padded to `0x193c18`;
with the block 0x20 bytes earlier the same function ends 4-aligned and no pad appears.

### 2.1 The libzip TU alignment

The original's libzip TUs are **one 4-byte-aligned block per `.c` file**, with the
functions inside at their natural 2-byte alignment. Example `zip_error.o`:

```
orig  _zip_error_clear 0x18db24  _zip_error_copy 0x18db2c  _zip_error_fini 0x18db36
      _zip_error_get   0x18db48  _zip_error_init 0x18db6a  _zip_error_set  0x18db74
ours  _zip_error_clear 0x18db18  _zip_error_copy 0x18db20  _zip_error_fini 0x18db2a
      _zip_error_get   0x18db3c  _zip_error_init 0x18db5e  _zip_error_set  0x18db68
```

Internal offsets are identical (`0, 8, 0x12, 0x24, 0x46, 0x50`); only the TU start is
shifted (here 0xc bytes early, i.e. 2 bytes per preceding libzip TU). The same is true of every libzip TU (measured: 22 libzip symbols — 18
exported + 4 local — were 2-mod-4 in ours vs only the 4 local functions in the original).

Cause: `tools/gen_exidx_markers.py` renamed the archive members' empty `.text` sentinels
with `sh_addralign = 1`, while app objects use `sh_addralign = 4`. Gold honours the
zero-size marker's alignment, so the next TU's first function stayed 2-aligned. The
original's archive TU starts are 4-aligned (the same rule already used for app objects).

A 4-byte shift at the end of `sha256.o` is a second-order effect: once the libzip block is
0x1c longer, `sha256.o` starts at the original's `0x18f730`, `SHA224_Final` ends at
`0x18fe3c`, and the 4-byte pad to the next 4-aligned section (`armv4cpuid.o` at
`0x18fe40`) appears automatically.

## 3. Controlled A/B (full gate each time)

All variants built with `cmake --build cmake-build-match --target check -j8`; no
`--update-baseline` until the final variant. `check` numbers are byte/linked.

| # | change | byte | linked | `.text` | `.ARM.exidx` | `.ARM.extab` | `.rodata` base | `.data` diff | `.data.rel.ro` diff |
|---|---|---:|---:|---:|---:|---:|---:|---:|---:|
| 0 | baseline | 4265 | 4496 | 0x1337c8 | 0x6f70 | 0x26298 | 0x1c9e10 | 3 | 667 |
| 1 | archive marker align 4 only (anchor kept) | 3233 | 4496 | 0x1337ec | 0x6f70 | 0x26298 | 0x1c9e40 | 72* | 197* |
| 2 | + remove `levelTextAnchor` (markers still in tail) | 4025 | 4496 | 0x1337c8 | 0x6f68 | 0x2628c | 0x1c9e00 | 72* | — |
| 3 | + place `LOCAL_ONLY_AFTER` markers adjacent | **4408** | **4496** | **0x1337c8** | 0x6f78 | 0x2628c | **0x1c9e10** | **0** | **21** |

`*` variants 1/2 have the right libc++abi pointers but `.rodata` moved, so every `.data`
pointer into `.rodata` differs; the 72/197 counts are that shift, not libc++abi.

Variant 1 proves the libzip alignment is the -0x24 cause: `__cxa_end_cleanup` moves to
`0x191d20`, `__cxa_demangle` to `0x194030`, and the two `.data` handler pointers become
exact. It is not a win on its own because `.text` grows by 0x24 and the anchor is still
present.

Variant 2 removes the anchor (0x24 bytes) and restores `.text` to 0x1337c8, but exposes a
second, pre-existing deficit: the original has CANTUNWIND exidx entries at `0x192fd4` and
`0x193ec4` (the empty `.text` sentinels of `fallback_malloc.o` / `abort_message.o`), which
the old `tail` placement merged away. That left `.ARM.exidx` 8 B and `.ARM.extab` 0xc
short, moving `.rodata` to 0x1c9e00.

Variant 3 places those two markers next to their owners instead of in the trailing group,
restoring the exidx entries and the `.rodata` base. `.ARM.exidx` is 8 B over (0x6f78 vs
0x6f70) and `.ARM.extab` 0xc under (0x2628c vs 0x26298), but the 16-byte `.rodata`
alignment absorbs the net 4 B, so `.rodata` is base-exact and the byte count jumps to 4408.

## 4. The fix

Three changes:

1. `tools/gen_exidx_markers.py`: archive empty-`.text` markers use `align=4` (was 1).
2. `tools/gen_section_order.py`: the `LOCAL_ONLY_AFTER` markers are placed next to their
   owner like every other marker (removed the `tail` special case).
3. `src/engine/core/AbyssEngine.cpp` / `src/game/world/Level.cpp`: delete the 0x24-byte
   `levelTextAnchor()` padding anchor; `Level::createMission` gets a sixth `pad = N`
   store (4 B) in place of the removed `bl`, so its size stays the original 0x4314
   (measured: `nm -S` on the object reports `00004314 T Level::createMission()`).

Final gate:

```
verify: byte 4408  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` = `CLEAN ✓`, `drift` = `0 field(s) across 0 class(es)`, `diff` = symbol parity
(5370/5370).

Content after the fix:

* `.data`: **0 differing bytes** (the two libc++abi handler pointers are exact).
* `.data.rel.ro`: **21 differing bytes** — 18 B `Level::createScene` switch table at
  `0x20d9c0..0x20d9e1` (the known `+0x3cc` `.rodata` block) and 3 B `.bss` pointer order
  (`gPirateGreetingFaceB`, `CHAR_BRENT`, `g_dw_playerFace`).
* libc++abi `.text` start (`0x191d20`), `__cxa_demangle` (`0x194030`), and the demangler
  tail (`PostfixQualifiedType::printLeft` `0x19adf0`, `D0Ev` `0x19ae10`) all match the
  original.

## 5. Proven vs still hypothesis

**Proven by measurement:**

* The `-0x24` is the sum of the libzip block's `-0x1c`, the `sha256.o` end pad `-0x4`,
  and two `-0x2` alignment pads inside `stdlib_exception.o`.
* The libzip block is `-0x1c` because its TU starts are 2-aligned in our link and
  4-aligned in the original; the archive marker's `sh_addralign` is the mechanism.
* Fixing the marker alignment moves libc++abi to the original addresses and makes `.data`
  byte-identical (variant 1: pointers exact; variant 3: whole section exact).
* The old `levelTextAnchor` anchor was the 0x24-byte compensation for this deficit.
* The `LOCAL_ONLY_AFTER` tail placement was hiding two original CANTUNWIND entries.

**Still hypothesis / open:**

* The original's libzip objects are probably single-`.text`-per-TU objects (no
  `-ffunction-sections`); we only prove the *alignment* effect, and reproduce it with the
  marker. Compiling libzip with `-fno-function-sections` was tested and is **wrong**:
  clang then emits a 2-byte `b.n` instead of the original's 4-byte `b.w` PLT tail call in
  `zip_get_name`, shrinking the TU by 4 B (measured byte 3187 before the marker fix).
* `.ARM.exidx` is now 8 B over (3567 vs 3566 entries: CANTUNWIND 275 vs 272, inline
  854 vs 856, extab 2438) and `.ARM.extab` 0xc under. The `.rodata` base is exact, but
  the exidx/extab *content* is not; this is a separate campaign. The two duplicate
  entries at `0x192fd4` / `0x193ec4` (CANTUNWIND + inline) are now reproduced; the
  remaining delta is a handful of address-level entries elsewhere (prel31 walk, net
  +1 entry).
* The remaining 21 `.data.rel.ro` bytes are the known `Level::createScene` `.rodata`
  block and `.bss` order, not libc++abi.

## 6. Reproduction commands

```
# baseline / final gate
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8

# symbol addresses
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -D -n --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -E '__cxa_end_cleanup|__cxa_demangle'
$NM -D -n --defined-only cmake-build-match/libgof2hdaa.so | grep -E '__cxa_end_cleanup|__cxa_demangle'

# unstripped relink + gold map (shows every input section address)
ninja -C cmake-build-match -t commands libgof2hdaa.so | tail -1 > /tmp/link.txt
sed -e 's#-o .../libgof2hdaa.so#-o /tmp/ours.so --Map=/tmp/ours.map#' /tmp/link.txt | bash

# .ARM.exidx function addresses (prel31; bit 30 is the sign bit)
python3 - <<'PY'
import struct
data = open('../_work/bins/android_2.0.16_libgof2hdaa.so','rb').read()
shoff, = struct.unpack_from('<I', data, 0x20)
entsz, n, shstr = struct.unpack_from('<HHH', data, 0x2e)
h = [struct.unpack_from('<IIIIIIIIII', data, shoff+i*entsz) for i in range(n)]
stroff = h[shstr][4]
for name, typ, fl, addr, off, size, link, info, al, es in h:
    end = data.index(b'\0', stroff+name)
    if data[stroff+name:end] == b'.ARM.exidx':
        for o in range(off, off+size, 8):
            w0, w1 = struct.unpack_from('<II', data, o)
            place = addr + (o-off); v = w0 & 0x7fffffff
            if w0 & 0x40000000: v -= 0x80000000
            fn = (place + v) & 0xffffffff
            if 0x191d00 <= fn < 0x19ae60:
                print(hex(fn), hex(w1))
PY

# .data / .data.rel.ro content diff
python3 - <<'PY'
import struct
def sec(path, name):
    data = open(path,'rb').read()
    shoff, = struct.unpack_from('<I', data, 0x20)
    entsz, n, shstr = struct.unpack_from('<HHH', data, 0x2e)
    h = [struct.unpack_from('<IIIIIIIIII', data, shoff+i*entsz) for i in range(n)]
    stroff = h[shstr][4]
    for nm, typ, fl, addr, off, size, link, info, al, es in h:
        end = data.index(b'\0', stroff+nm)
        if data[stroff+nm:end].decode() == name:
            return data[off:off+size], addr
for name in ('.data', '.data.rel.ro'):
    o, oa = sec('../_work/bins/android_2.0.16_libgof2hdaa.so', name)
    c, ca = sec('cmake-build-match/libgof2hdaa.so', name)
    d = [i for i in range(min(len(o), len(c))) if o[i] != c[i]]
    print(name, len(d), hex(oa+d[0]) if d else None)
PY
```
