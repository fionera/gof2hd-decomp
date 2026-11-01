# `.dynsym` app order round 2 (w3452)

Status: research note + one landed link-order fix (2026-11-01). Worktree `wt/w3452`, branch
`wave87-w3452`, based on master `cfbd140e`. Every number below is measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b gold 1.12
(`.cache/ndk/android-ndk-r18b/.../ld.gold`, `.comment` = `GCC 4.8`, statically linked libstdc++).

## TL;DR

* The `.dynsym` order is **fully explained** by a model that reproduces our own build exactly
  (5577/5577 positions, before and after the fix below):
  * gold's `Symbol_table::table_` iterates in **ascending bucket order** (SGI `__gnu_cxx::hash_map`
    behaviour), not the reverse-insertion order of `std::unordered_map`;
  * the key is `(name_key, version_key)` = **1-based Stringpool insertion indices**, and
    `hash = name_key ^ version_key`;
  * for an unversioned symbol `hash = name_key`, so dynsym order = **namepool insertion order**;
  * for a default-versioned symbol both `(name,0)` and `(name,V)` entries point at the same
    `Symbol`, and the first one finalised wins, so the output position is
    `min(name_key, name_key ^ V)`.
* The **app region's unversioned symbols are already in exact insertion order**: 5498/5498 with
  12 matching blocks. The only differences are four local reorderings (RecordHandler,
  `ResourceTexture` ctor, `zip_err_str`, SHA group).
* The remaining app-region divergence is **entirely the 79 LIBC-versioned imports**. Their
  positions are a XOR function of the namepool index and are therefore hypersensitive to any
  namepool difference.
* The original's LIBC namepool key is **V = 7149**. Ours was **7516** because our link line put
  libcrypto and libc++abi *before* the shared libraries. Reordering the link inputs to
  *shared libs → libcrypto → libm → libc++abi* reproduces V = 7149 and improves
  `.dynsym` same-index **1410/5578 → 5330/5578**, differing `.dynsym` bytes **24461 → 1914**,
  `.dynstr` **142066 → 6891**, `.hash` **7311 → 613** — with every other ELF section
  **byte-identical** and the gate unchanged.
* The 29 remaining PLT-slot differences are **all app-defined symbols** (first-reference order);
  none is a LIBC import. The versioned dynsym divergence does **not** affect the PLT order.

## 1. The model (gold source + measurement)

Gold's symbol table (`binutils-2.27/gold/symtab.h`):

```cpp
typedef std::pair<Stringpool::Key, Stringpool::Key> Symbol_table_key;
struct Symbol_table_hash {
  size_t operator()(const Symbol_table_key& key) const { return key.first ^ key.second; }
};
typedef Unordered_map<Symbol_table_key, Symbol*, Symbol_table_hash, Symbol_table_eq>
    Symbol_table_type;
```

`Stringpool::add_with_length` (`stringpool.cc`) assigns `Key k = key_to_offset_.size() + 1`,
i.e. the key is the **1-based insertion index** of the string (names and version strings share
one pool). `Symbol_table::sized_finalize` walks `table_` directly.

The NDK gold is built with GCC 4.8 (`readelf -p .comment`), and its `Unordered_map` resolves to
the SGI-style `__gnu_cxx::hash_map`: iteration walks buckets 0..N-1 and prepends within a
bucket. With `number_of_input_files()*1024` buckets (≈226k, far above any key), bucket = hash,
so iteration is **ascending hash**, and within a bucket the later insertion is emitted first.

`min(name_key, name_key ^ V)`: for a default-versioned symbol `add_from_object` inserts both
`(name_key, 0)` and `(name_key, V)`; both point at the same `Symbol`. `sized_finalize_symbol`
returns false for the second (`has_symtab_index()`), so the entry that comes first in iteration
order — the smaller hash — determines the output index.

### Validation

`/tmp/validate_model.py` (inlined in §8) reconstructs the namepool by replaying the exact
input-processing order from a `gold -y <every dynsym name>` trace, then predicts the dynsym
order. Result on our build:

```
V(LIBC) = 7516   names = 8394   model matches 5577 / 5577     # before the fix
V(LIBC) = 7149   names = 8394   model matches 5577 / 5577     # after  the fix
```

This is an exact model, not a correlation: every one of the 5577 non-null symbols lands at the
predicted index.

## 2. Decomposition of the app region

Our build has 5578 dynsym entries: 1 null + 5498 unversioned + 79 LIBC-versioned.

### 2.1 Unversioned symbols are already exact

`difflib.SequenceMatcher` over the unversioned subsequence (original vs ours) gives 12 matching
blocks and exactly four non-trivial differences, none of which is a LIBC import:

| # | original order | our order | witness |
|---|---|---|---|
| 1 | `RecordHandler::saveOptions`, `recordStoreWrite`, `recordStoreWritePreview`, ctor, dtor | `saveOptions`, ctor, dtor, … , `recordStoreWrite`, `recordStoreWritePreview` | `RecordHandler.cpp` `.symtab` order (source definition order) |
| 2 | `ResourceTexture(ResourceMesh)`, `ResourceTexture(String)`, `ResourceMaterial` | `ResourceTexture(ResourceMesh)`, `ResourceMaterial`, `ResourceTexture(String)` | `ResourceTexture.cpp` source order |
| 3 | `zip_add_dir`, `zip_source_free`, `_zip_err_type`, `_zip_nerr_str`, … , `_zip_err_str` | `_zip_err_str`, `_zip_err_type`, `_zip_nerr_str`, `zip_add_dir`, … | libzip member pull order |
| 4 | `SHA224_Init` group before `_edata` | same group after `zip_unchange_all`; `sha256_block_data_order_neon` off by one | libcrypto member pull order |

These are real insertion-order witnesses (first encounter = reference or definition, in link
order), not hash artefacts: unversioned hash = name_key, and the model reproduces the order
exactly.

### 2.2 The 79 versioned imports

All 79 are `LIBC`-versioned (`.gnu.version` index 2 or 3). Their observed positions in the
original are a XOR permutation of the namepool index: e.g. `islower` (name_key 7513) sits at
index 1543, `memset` (5244) at 852, `raise` (6518) at 647, while `strcmp` (152) sits at 148
because `152 ^ V` is large. The measured positions are listed in §8's `verlist` output.

## 3. Original's V and the link-order experiment

The original's V can be fitted from the observed positions. A brute-force over the tail link
order (permuting the groups after the 203 app objects) with the validated model gives:

| tail order | predicted same-index | V |
|---|---:|---:|
| current tree: `crypto, shared, cxxabi, libm, android, gcc, dl, c` | 1409 / 5577 | 7516 |
| best: `shared, crypto, libm, cxxabi, android, gcc, dl, c` | **5329 / 5577** | **7149** |

The best order is unique up to the order of the trailing `android/gcc/dl/c` groups (which do
not affect V or the score). It says the original link processed the **shared libraries before
libcrypto**, and **libm before libc++abi**. That reproduces the original's LIBC namepool key
V = 7149 exactly.

This is not a linker trick: it is the order of the input files on the gold command line. The
original was linked by ndk-build, whose default places `LOCAL_LDLIBS` (the shared libs) before
the static runtime archives, and `-lm` before `libc++abi.a`.

## 4. Landed change and A/B

`CMakeLists.txt`: move `${_link_openssl}` after the shared-library list and `-l m` before
`${_link_libcxxabi}`.

```diff
-            ${_link_openssl}
             -l log -l GLESv2 -l GLESv1_CM -l EGL -l android
             ${GOF2_FMOD_LIBDIR}/libfmodex.so ${GOF2_FMOD_LIBDIR}/libfmodevent.so
+            ${_link_openssl}
+            -l m
             ${_link_libcxxabi}
-            -l m
```

Measured with the full gate (`cmake --build cmake-build-match --target check -j8`):

| metric | before | after |
|---|---:|---:|
| `byte_exact` | 4254 | 4254 |
| `linked_exact` | 4496 | 4496 |
| stubs / missing / extra | 0 / 0 / 0 | 0 / 0 / 0 |
| avg | 99.89 | 99.89 |
| `data` exported objects exact | 843/843 | 843/843 |
| sodiff imports | 207 | 207 |
| lint / drift | CLEAN / 0 | CLEAN / 0 |
| `.dynsym` same index | 1410 / 5578 | **5330 / 5578** |
| `.dynsym` differing bytes | 24461 | **1914** |
| `.dynstr` differing bytes | 142066 | **6891** |
| `.hash` differing bytes | 7311 | **613** |
| `.text` / `.rodata` / `.data.rel.ro` / `.data` / `.bss` / `.plt` / `.got` / `.ARM.*` | exact | **byte-identical to before** |
| `DT_NEEDED` order | liblog…libc | unchanged |

The only `.dynamic` byte that changes is the `DT_NEEDED` string offset of `libm.so` (0x2a0f →
0x29d8), because `.dynstr` is emitted in dynsym order; the tag order and all values are
otherwise identical.

## 5. PLT order is a separate observable

The PLT order is **not** the dynsym order. It is the order gold first sees a JUMP_SLOT
relocation. After the fix the PLT symbol sequence is **3645/3674 identical**, with 29 differing
slots. Every one of the 29 is an app-defined symbol; **none** is LIBC-versioned. The two
clusters are:

* `FileRead::loadStaticCollision` / `loadStationCollision` are 6 slots too early in ours
  (first referenced by `Level.cpp`, within-object relocation order);
* a ~21-slot rotation around `Standing::setStanding`, `PlayerEgo::autoTurretIsEnabled`,
  `setAutoTurret`, `setDockingState`, `deleteHackingGame`, `StarSystem::switchSun`,
  `Level::switchSkybox`, `ParticleSystemManager::reset`, `PlayerStation::setVisible`, etc.
  (first-reference order across `Level.cpp`, `LevelScript.cpp`, `PlayerEgo.cpp`, `StarSystem.cpp`).

So the 79 versioned dynsym positions are a **byte-neutral** observable for `verify` (they do not
move PLT slots); the 29 PLT slots are the byte-relevant part and are a source/within-object
order problem, not a namepool problem.

## 6. Proven vs hypothesis

**Proven (command/measurement above):**

* The dynsym order model (ascending hash, `hash = name_key ^ version_key`,
  `min(name_key, name_key^V)` for default-versioned symbols) reproduces our build exactly,
  5577/5577, before and after the fix.
* Unversioned symbols are in namepool insertion order; the app region's unversioned order
  differs from the original only in the four local reorderings of §2.1.
* The original's LIBC namepool key is V = 7149; the previous link order gave V = 7516.
* Moving the shared libraries before libcrypto and libm before libc++abi changes V to 7149 and
  improves `.dynsym` same-index 1410 → 5330 with every other section byte-identical and the
  gate unchanged.
* The 29 PLT-slot differences are all app-defined symbols; no LIBC import is involved.

**Hypothesis / not yet proven:**

* That the original's link line is exactly `shared libs → libcrypto → libm → libc++abi`. The
  dynsym fit is consistent with it and it is the ndk-build default shape, but the command line
  itself is not recoverable from the binary.
* That the remaining 248 dynsym mismatches are entirely the four local insertion-order
  differences plus small hidden-name-count differences in the original's objects. The model
  says they must be namepool-index differences (the hash is exact), but the original's exact
  hidden-symbol set is not recoverable without its object files.
* That reordering the `RecordHandler.cpp` definitions and the other three local witnesses would
  remove the remaining unversioned deltas and the 29 PLT slots. Not attempted here; it is a
  source-order change with possible `.rodata` side effects, so it needs its own gated pass.

## 7. Reproduction commands

```sh
# baseline / gate
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# dynsym order
python3 - <<'PY'
import sys, difflib, struct
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
def load(p):
    e = Elf32(p); d = open(p, 'rb').read()
    vs = next(s for s in e.sections if s['type'] == 0x6fffffff)
    vers = [struct.unpack_from('<H', d, vs['offset'] + 2*i)[0] for i in range(vs['size']//2)]
    return [s['name'] for s, v in zip(e._symbols(SHT_DYNSYM), vers)]
o = load('../_work/bins/android_2.0.16_libgof2hdaa.so')
u = load('cmake-build-match/libgof2hdaa.so')
print('same index', sum(a == b for a, b in zip(o, u)), '/', len(o))
sm = difflib.SequenceMatcher(None, o, u, autojunk=False)
print('blocks', len(sm.get_matching_blocks()))
PY

# section byte diff (the .dynsym/.dynstr/.hash improvement)
python3 - <<'PY'
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
def sec(p, n):
    e = Elf32(p); s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset']+s['size']]
O = '../_work/bins/android_2.0.16_libgof2hdaa.so'
for n in ('.dynsym', '.dynstr', '.hash', '.text', '.rodata', '.plt', '.got'):
    a, b = sec(O, n), sec('cmake-build-match/libgof2hdaa.so', n)
    print(n, sum(x != y for x, y in zip(a, b)))
PY

# PLT order
OD=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump
$OD -d -j .plt ../_work/bins/android_2.0.16_libgof2hdaa.so | grep -o '<[^>]*@plt>:' | sed 's/[<>:]//g' > /tmp/plt_o.txt
$OD -d -j .plt cmake-build-match/libgof2hdaa.so          | grep -o '<[^>]*@plt>:' | sed 's/[<>:]//g' > /tmp/plt_u.txt
python3 -c "o=[l.strip() for l in open('/tmp/plt_o.txt')];u=[l.strip() for l in open('/tmp/plt_u.txt')];print(sum(a==b for a,b in zip(o,u)),'/',len(o))"
```

The model validation script (`/tmp/validate_model.py`) is:

```python
import sys, struct, re, os, subprocess
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM

def global_syms_symtab(path):            # all STB_GLOBAL .symtab names, file order
    e = Elf32(path); d = open(path, 'rb').read()
    sec = next((s for s in e.sections if s['type'] == 2), None)
    if sec is None: return []
    st = e.sections[sec['link']]; base = st['offset']; out = []
    for off in range(sec['offset'], sec['offset'] + sec['size'], 16):
        st_name, val, sz = struct.unpack_from('<III', d, off)
        if (d[off+12] >> 4) == 0: continue
        end = d.find(b'\0', base + st_name)
        out.append(d[base+st_name:end].decode('utf8', 'replace'))
    return out

def dyn_syms_with_ver(path):             # (name, versym, version-name) in .dynsym order
    e = Elf32(path); d = open(path, 'rb').read()
    syms = list(e._symbols(SHT_DYNSYM))
    vs = next((s for s in e.sections if s['type'] == 0x6fffffff), None)
    vers = [struct.unpack_from('<H', d, vs['offset'] + 2*i)[0] for i in range(vs['size']//2)]
    vnames = {}
    vd = next((s for s in e.sections if s['type'] == 0x6ffffffd), None)
    if vd:
        off = vd['offset']
        while off < vd['offset'] + vd['size']:
            ver, flags, ndx, cnt, hv, aux, nxt = struct.unpack_from('<HHHHIII', d, off)
            a = off + aux; nameoff, = struct.unpack_from('<I', d, a)
            st = e.sections[vd['link']]; b = st['offset']; end = d.find(b'\0', b + nameoff)
            vnames[ndx] = d[b+nameoff:end].decode()
            if nxt == 0: break
            off += nxt
    vr = next((s for s in e.sections if s['type'] == 0x6ffffffe), None)
    if vr:
        off = vr['offset']
        while off < vr['offset'] + vr['size']:
            ver, cnt, fileoff, aux, nxt = struct.unpack_from('<HHIII', d, off)
            a = off + aux
            while True:
                h2, fl, other, nameoff, nxt2 = struct.unpack_from('<IHHII', d, a)
                st = e.sections[vr['link']]; b = st['offset']; end = d.find(b'\0', b + nameoff)
                vnames[other] = d[b+nameoff:end].decode()
                if nxt2 == 0: break
                a += nxt2
            if nxt == 0: break
            off += nxt
    return [(s['name'], v & 0x7fff, vnames.get(v & 0x7fff)) for s, v in zip(syms, vers)]

# file order from `gold -y <every name>` trace (see trace_link.py in the note text)
lines = open('/tmp/trace.err').read().splitlines()
pat = re.compile(r'^(.*?): (reference to|definition of) (.*)$')
fileorder = []
for ln in lines:
    m = pat.match(ln)
    if m and (not fileorder or fileorder[-1] != m.group(1)):
        fileorder.append(m.group(1))

memdir = '/tmp/members'; os.makedirs(memdir, exist_ok=True)
def resolve(f):
    m = re.match(r'^(.*\.a)\((.*)\)$', f)
    if not m: return f
    arc, mem = m.group(1), m.group(2)
    out = os.path.join(memdir, os.path.basename(arc) + '__' + mem)
    if not os.path.exists(out):
        open(out, 'wb').write(subprocess.run(['ar', 'p', arc, mem], capture_output=True).stdout)
    return out

namekey = {}
for f in fileorder:
    rf = resolve(f)
    if rf.endswith('.so'):
        for n, v, vn in dyn_syms_with_ver(rf):
            if n and n not in namekey: namekey[n] = len(namekey) + 1
            if v > 1 and vn and vn not in namekey: namekey[vn] = len(namekey) + 1
    else:
        for n in global_syms_symtab(rf):
            if n and n not in namekey: namekey[n] = len(namekey) + 1

ours = dyn_syms_with_ver('cmake-build-match/libgof2hdaa.so')
V = namekey['LIBC']
arr = []
for n, v, vn in ours:
    if not n: continue
    k = namekey[n]
    h = k if v in (0, 1) else min(k, k ^ V)
    arr.append((h, -k, n))
arr.sort()
pred = [x[2] for x in arr]; act = [n for n, v, vn in ours if n]
print('V(LIBC) =', V, ' names =', len(namekey))
print('model matches', sum(a == b for a, b in zip(pred, act)), '/', len(act))
```

`trace_link.py` re-runs the exact `libgof2hdaa.so` gold command from
`cmake-build-match/build.ninja` with `-y <name>` for every dynsym name and writes the trace to
`/tmp/trace.err`; the trace is only a measurement aid (the output `.so` is unchanged).
