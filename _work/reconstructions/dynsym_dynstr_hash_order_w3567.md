# `.dynsym` / `.dynstr` / `.hash` ordering rule (w3567)

Status: research note (2026-11-01). Worktree `wt/w3567`, branch `wave87-w3567`, based on
current `master`. Every number below was measured against
`../_work/bins/android_2.0.16_libgof2hdaa.so` with the repo's NDK r18b gold 1.12
(`.cache/ndk/android-ndk-r18b/.../ld.gold`). No source change was landed: none of the
controlled experiments was a strict improvement.

## TL;DR

* **`.dynsym` order is gold's `Symbol_table` hash-map iteration order, not input order.**
  The key is `(name_key, version_key)` and the hash is `name_key ^ version_key`
  (`gold/symtab.h`). `name_key` is the 1-based insertion index in gold's name stringpool.
  The NDK gold is built with GCC 4.8, whose `Unordered_map` is the SGI
  `__gnu_cxx::hash_map`; iteration walks buckets `0..N-1` and prepends within a bucket.
  With `number_of_input_files()*1024` buckets (≈226k, far above any key) this is
  **ascending hash**, later insertion first within a bucket.
* For an **unversioned** symbol `hash = name_key`, so the order is exactly the
  **first-occurrence order of global symbol names over the link inputs**. Each object's
  global `.symtab` is lexicographic by mangled name (clang 7.0.2 MC), so a name moves only
  when the first object that *mentions* it changes.
* The full model, including the 79 `LIBC`-versioned imports (`V = 7149`), reproduces our
  entire `.dynsym` **5577 / 5577**.
* **`.dynstr` has no independent order.** It is emitted in `.dynsym` order, with the
  version/NEEDED/SONAME strings inserted at their namepool insertion points. Both binaries
  have the same structure: extras at dynsym index 2 (`LIBC\0libc.so\0libgof2hdaa.so\0`) and
  index 323 (`libm.so\0`), then the 8 remaining `DT_NEEDED` strings at the tail.
* **`.hash` has no independent order.** It is exactly the SysV hash table over the
  `.dynsym` name sequence: `nbucket = 4099`, `nchain = 5578`,
  `bucket = elf_hash(name) % 4099`, chain links in dynsym-index order. Recomputed exactly
  for both binaries (4099/4099 buckets, 5578/5578 chains).
* The remaining **10 defined-symbol gaps are 5 first-occurrence/TU issues**, not link-flag
  issues. They are listed in §6.1. Three controlled A/B rebuilds are in §6; all were
  byte-neutral or regressions, so only this note is committed.

## 1. Baseline

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`report.json`: `compared 4524`, `byte_exact 4496`, `linked_exact 4496`,
`stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`.
`.text` is `0x1337c8` at `0x69440`.

Section byte diff vs the original (same section sizes and bases):

| section | size | differing bytes |
|---|---:|---:|
| `.dynsym` | 89,248 | 1,768 |
| `.dynstr` | 189,208 | 6,676 |
| `.hash` | 38,716 | 563 |
| `.gnu.version` | 11,156 | 52 |
| `.plt` | 44,108 | **0** |
| `.got` | 17,164 | **0** |
| `.rodata` | 269,159 | **0** |
| `.data` | 10,604 | **0** |
| `.data.rel.ro` | 12,264 | **0** |

`.dynsym` has 5578 entries: 1 null + **5370 defined** + 207 undefined imports
(79 `LIBC`-versioned + 128 unversioned). The 5370 defined symbols are all unversioned.
`difflib.SequenceMatcher` over the 5370 defined names: same index **5263 / 5370**,
ratio **0.998138**, 10 gaps, 5360 matched.

## 2. The rule

Gold's symbol table (`binutils-2.27/gold/symtab.h`):

```cpp
typedef std::pair<Stringpool::Key, Stringpool::Key> Symbol_table_key;
struct Symbol_table_hash {
  size_t operator()(const Symbol_table_key& key) const { return key.first ^ key.second; }
};
typedef Unordered_map<Symbol_table_key, Symbol*, Symbol_table_hash, Symbol_table_eq>
    Symbol_table_type;
```

`Stringpool::add_with_length` assigns `Key k = key_to_offset_.size() + 1`, i.e. the key is
the 1-based insertion index of the string (names and version strings share one pool).
`Symbol_table::sized_finalize` walks `table_` directly. The NDK gold's `.comment` is
`GCC 4.8`, so `Unordered_map` is the SGI `__gnu_cxx::hash_map`: iteration is ascending
bucket, later insertion first within a bucket. For a default-versioned symbol,
`add_from_object` inserts both `(name_key, 0)` and `(name_key, V)`; both point at the same
`Symbol`, and the earlier one in iteration order wins, so the output position is
`min(name_key, name_key ^ V)`.

Consequences:

1. **Unversioned symbols** (`hash = name_key`) are in namepool insertion order, which is
   the first time a global name is mentioned by an object in link order. Because each
   object's global `.symtab` is lexicographic by mangled name, moving a definition between
   TUs only matters if the new TU is the first mention.
2. **Versioned imports** are permuted by `key ^ V`; they are not in first-occurrence order.
   In our link `V = 7149` (the insertion index of `LIBC` when `libc.so` is processed).
3. `.dynstr` and `.hash` are derived from the resulting `.dynsym` sequence (§4, §5).

## 3. Evidence: unversioned symbols are first-occurrence order

The trace is the exact gold command from `cmake-build-match/build.ninja` re-run with
`-y <name>` for every dynsym name. The file order is the order of the first `-y` line for
each input; the symbol order inside each file is the object's global `.symtab` order.

```python
# /tmp/model.py -- unversioned defined symbols only
import sys, re, struct, difflib
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
U = 'cmake-build-match/libgof2hdaa.so'
e = Elf32(U); d = open(U, 'rb').read()
vs = next(s for s in e.sections if s['type'] == 0x6fffffff)
vers = [struct.unpack_from('<H', d, vs['offset'] + 2*i)[0] for i in range(vs['size']//2)]
syms = list(e._symbols(SHT_DYNSYM))
pat = re.compile(r'^(.*?): (reference to|definition of) (.*)$')
order, seen = [], set()
for ln in open('/tmp/trace_all.err'):
    m = pat.match(ln.rstrip('\n'))
    if not m: continue
    n = m.group(3)
    if n not in seen:
        seen.add(n); order.append(n)
u_unv = [s['name'] for s, v in zip(syms, vers) if s['shndx'] != 0 and v in (0, 1)]
pred = [n for n in order if n in set(u_unv)]
print('unversioned defined', len(u_unv), 'pred', len(pred))
print('same', sum(a == b for a, b in zip(pred, u_unv)), '/', len(u_unv))
```

Measured:

```
unversioned defined 5370 pred 5370
same 5370 / 5370
```

This is exact, not a correlation: every one of the 5370 defined symbols is at the
predicted index.

## 4. Evidence: full model including versioned imports

The model replays the file order from the trace, reads every object's global `.symtab`
(or every shared library's `.dynsym`), assigns `namekey[name] = len(namekey)+1` on first
mention, and assigns `V` when `libc.so` is processed. It then sorts by
`(hash, -namekey)` and compares to the actual `.dynsym`.

```python
# /tmp/validate_model.py (self-contained)
import sys, struct, re, os, subprocess
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM

def sections(data):
    e_shoff = struct.unpack_from('<I', data, 0x20)[0]
    e_shentsize = struct.unpack_from('<H', data, 0x2E)[0]
    e_shnum = struct.unpack_from('<H', data, 0x30)[0]
    out = []
    for i in range(e_shnum):
        o = e_shoff + i*e_shentsize
        name, typ, flags, addr, off, size, link, info, align, ent = \
            struct.unpack_from('<IIIIIIIIII', data, o)
        out.append(dict(typ=typ, off=off, size=size, link=link, info=info, ent=ent))
    return out

def symtab_globals(path):
    data = open(path, 'rb').read(); secs = sections(data)
    st = next(s for s in secs if s['typ'] == 2)
    strs = secs[st['link']]; strtab = data[strs['off']:strs['off']+strs['size']]
    def cstr(i):
        e = strtab.find(b'\0', i); return strtab[i:e].decode('utf-8', 'replace')
    out = []; ent = st['ent'] or 16
    for i in range(st['info'], st['size']//ent):
        o = st['off'] + i*ent
        nm, val, sz, info, other, shndx = struct.unpack_from('<IIIBBH', data, o)
        if (info >> 4) == 0: continue          # global only
        out.append(cstr(nm))
    return out

def dyn_syms_with_ver(path):
    e = Elf32(path); d = open(path, 'rb').read()
    syms = list(e._symbols(SHT_DYNSYM))
    vs = next((s for s in e.sections if s['type'] == 0x6fffffff), None)
    vers = [struct.unpack_from('<H', d, vs['offset']+2*i)[0]
            for i in range(vs['size']//2)] if vs else [0]*len(syms)
    vnames = {}
    vd = next((s for s in e.sections if s['type'] == 0x6ffffffd), None)
    if vd:
        off = vd['offset']
        while off < vd['offset'] + vd['size']:
            ver, flags, ndx, cnt, hv, aux, nxt = struct.unpack_from('<HHHHIII', d, off)
            a = off + aux; nameoff, = struct.unpack_from('<I', d, a)
            st = e.sections[vd['link']]; b = st['offset']; end = d.find(b'\0', b+nameoff)
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
                st = e.sections[vr['link']]; b = st['offset']; end = d.find(b'\0', b+nameoff)
                vnames[other] = d[b+nameoff:end].decode()
                if nxt2 == 0: break
                a += nxt2
            if nxt == 0: break
            off += nxt
    return [(s['name'], v & 0x7fff, vnames.get(v & 0x7fff)) for s, v in zip(syms, vers)]

pat = re.compile(r'^(.*?): (reference to|definition of) (.*)$')
fileorder = []
for ln in open('/tmp/trace_all.err'):
    m = pat.match(ln.rstrip('\n'))
    if m and (not fileorder or fileorder[-1] != m.group(1)):
        fileorder.append(m.group(1))

memdir = '/tmp/members'; os.makedirs(memdir, exist_ok=True)
def resolve(f):
    m = re.match(r'^(.*\.a)\((.*)\)$', f)
    if not m: return f
    arc, mem = m.group(1), m.group(2)
    out = os.path.join(memdir, os.path.basename(arc) + '__' + mem)
    if not os.path.exists(out):
        open(out, 'wb').write(subprocess.run(['ar', 'p', arc, mem],
                                             capture_output=True).stdout)
    return out

namekey = {}
for f in fileorder:
    rf = resolve(f)
    if rf.endswith('.so'):
        for n, v, vn in dyn_syms_with_ver(rf):
            if n and n not in namekey: namekey[n] = len(namekey) + 1
            if v > 1 and vn and vn not in namekey: namekey[vn] = len(namekey) + 1
    else:
        for n in symtab_globals(rf):
            if n and n not in namekey: namekey[n] = len(namekey) + 1

ours = dyn_syms_with_ver('cmake-build-match/libgof2hdaa.so')
V = namekey.get('LIBC')
arr = []
for n, v, vn in ours:
    if not n: continue
    k = namekey[n]
    h = k if v in (0, 1) else min(k, k ^ V)
    arr.append((h, -k, n))
arr.sort()
pred = [x[2] for x in arr]
act = [n for n, v, vn in ours if n]
print('V', V, 'names', len(namekey))
print('model matches', sum(a == b for a, b in zip(pred, act)), '/', len(act))
```

Measured:

```
fileorder 291
V 7149 names 8394
model matches 5577 / 5577
```

To generate the trace, re-run the exact gold command from `build.ninja` with
`-y <name>` for every dynsym name (the output `.so` is unchanged):

```python
# /tmp/trace_all.py
import sys, subprocess, shlex
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
names = [s['name'] for s in Elf32('cmake-build-match/libgof2hdaa.so')._symbols(SHT_DYNSYM)
         if s['name']]
uniq = list(dict.fromkeys(names))
cmd = subprocess.run(['ninja', '-t', 'commands', 'libgof2hdaa.so'],
                     cwd='cmake-build-match', capture_output=True, text=True
                     ).stdout.strip().splitlines()[-1]
gold = shlex.split(cmd.split(' && ')[1])
i = gold.index('-o'); gold[i+1] = '/tmp/trace_all.so'
gold[1:1] = sum((['-y', n] for n in uniq), [])
with open('/tmp/trace_all.err', 'w') as f:
    subprocess.run(gold, stderr=f, stdout=subprocess.DEVNULL)
```

## 5. Evidence: `.hash` and `.dynstr` are derived from `.dynsym`

### 5.1 `.hash`

`readelf -SW` gives `.hash` size `0x973c` for both binaries, so
`nbucket = 4099`, `nchain = 5578` (`(2 + 4099 + 5578) * 4 = 0x973c`). Recomputing the
SysV hash table from the `.dynsym` name sequence reproduces both files exactly:

```python
import struct
from elf import Elf32, SHT_DYNSYM

def elf_hash(name):
    h = 0
    for c in name.encode():
        h = (h << 4) + c
        g = h & 0xf0000000
        if g: h ^= g >> 24
        h &= ~g
    return h

for p in ('../_work/bins/android_2.0.16_libgof2hdaa.so',
          'cmake-build-match/libgof2hdaa.so'):
    e = Elf32(p); d = e.data
    sec = next(s for s in e.sections if s['name'] == '.hash')
    nb, nc = struct.unpack_from('<II', d, sec['offset'])
    buckets = list(struct.unpack_from('<%dI' % nb, d, sec['offset'] + 8))
    chain = list(struct.unpack_from('<%dI' % nc, d, sec['offset'] + 8 + 4*nb))
    names = [s['name'] for s in e._symbols(SHT_DYNSYM)]
    rb = [0]*nb; rc = [0]*nc
    for i, n in enumerate(names):
        if i == 0: continue
        b = elf_hash(n) % nb
        rc[i] = rb[b]; rb[b] = i
    print(p, 'nbucket', nb, 'nchain', nc,
          'buckets', sum(a == b for a, b in zip(buckets, rb)), '/', nb,
          'chain', sum(a == b for a, b in zip(chain, rc)), '/', nc)
```

Measured:

```
orig  nbucket 4099 nchain 5578 buckets 4099/4099 chain 5578/5578
ours  nbucket 4099 nchain 5578 buckets 4099/4099 chain 5578/5578
```

So `.hash` is a pure function of the `.dynsym` name sequence. A different `.dynsym`
order necessarily produces a different `.hash`; there is no separate ordering knob.

### 5.2 `.dynstr`

`.dynstr` is emitted in `.dynsym` order. The only non-symbol strings are inserted at their
namepool insertion points. Walking the dynsym entries and recording the bytes skipped
between consecutive `st_name` offsets gives, for **both** binaries:

```
extras: [(2,  b'LIBC\0libc.so\0libgof2hdaa.so\0'),
         (323, b'libm.so\0')]
tail:   b'liblog.so\0libGLESv2.so\0libGLESv1_CM.so\0libEGL.so\0libandroid.so\0'
        b'libfmodex.so\0libfmodevent.so\0libdl.so\0'
final offset 189107, section size 189208
```

The extras are the version string (`LIBC`), the SONAME, and the `DT_NEEDED` strings.
Because the symbol names are emitted in dynsym order, a different `.dynsym` order changes
every later `st_name` offset and therefore the whole `.dynstr` byte stream. This is why
`.dynstr` differs by 6,676 bytes at baseline even though the name set is identical.

## 6. Remaining defined-symbol gaps and controlled A/B experiments

### 6.1 The 10 remaining defined-symbol gaps

All 10 are unversioned. The 79 versioned imports are a separate XOR permutation (22 small
gaps; best-fit `V` over our namepool keys is 7140–7149, consistent with the original's
`V = 7149`). The defined gaps are 5 first-occurrence/TU issues:

| # | symbol(s) | original index | ours | cause |
|---:|---|---:|---:|---|
| 1 | `RecordHandler::recordStoreWrite(int)`, `recordStoreWritePreview(int)` | 1827–1828 | 1889–1890 | first mention is `NdkAutosave.cpp` in our tree; original first mentions them in the same object as `saveOptions` |
| 2 | `curTouchSize`, `maxTouchSize`, `touches` | 1891–1893 | 1907–1909 | first mention is `LODManager.cpp` in our tree; original first mentions them before the `LODManager` block |
| 3 | `ResourceTexture::ResourceTexture(String const&, float)` | 3879 | 3881 | weak inline ctor is emitted in `ResourceMaterial.cpp`; original emits it in the `GameResource`/`BuildResourceList` TU |
| 4 | `_zip_err_str`, `_zip_err_type`, `_zip_nerr_str` | 5030–5036 | 5028–5030 | libzip archive-member order |
| 5 | `sha256_block_data_order_neon` | 5082 | 5071 | libcrypto archive-member order |

### 6.2 Experiment A: move `checkFirstCreditPackBoughtWriteAction` to `NdkAutosave.cpp`

Change: remove the function from `src/platform/android/NdkHooks.cpp` and insert it before
`ndk_autosave()` in `src/platform/android/NdkAutosave.cpp` (add `Status.h`/`Globals.h`).

```
before: byte 4496  linked 4496  dynsym same-index 5263/5370  gaps 10
after:  byte 4496  linked 4496  dynsym same-index 5262/5370  gaps 11
```

The whole `saveOptions, recordStoreWrite, recordStoreWritePreview, C1, D1,
Status::getCurrentCampaignMission` block moved from defined index 1826 to 1885. This
**confirms the first-occurrence rule** (the only `saveOptions` reference moved out of
`NdkHooks.cpp`), but it is a dynsym regression, so it was reverted.

### 6.3 Experiment B: merge `NdkAutosave.cpp` into `NdkHooks.cpp`

Change: move `ndk_autosave()` into `NdkHooks.cpp` immediately after
`checkFirstCreditPackBoughtWriteAction()` and delete `NdkAutosave.cpp`.

```
before: byte 4496  linked 4496  dynsym gaps 10
after:  byte 3977  linked 4496  dynsym gaps 9 (RecordHandler gap fixed)
```

The RecordHandler block becomes exactly the original's
`saveOptions, recordStoreWrite, recordStoreWritePreview, C1, D1`. The byte regression is a
**PLT-order** effect: clang 7.0.2 emits an out-of-line function at
`min(definition, first-reference)`, so `ndk_autosave`'s body is emitted at its first call
inside `ndk_checkPlaytimeAndSpendOfferwallCredits` (before `checkFirst...`), and its
`C1/recordStoreWrite/recordStoreWritePreview/D1` relocations move the PLT slots from
1013–1017 to 1007–1010. The original has `ndk_autosave`'s body after `checkFirst...`
(0x94770 vs 0x94708), so a codegen-preserving merge needs the body emitted later. Reverted.

### 6.4 Experiment C: move `loadPortraits` to `BuildResourceList.cpp`

Change: move the whole `loadPortraits` definition from
`src/engine/render/ResourceMaterial.cpp` to the end of
`src/engine/render/BuildResourceList.cpp` (add `Globals.h`/`String.h`), leaving
`ResourceMaterial.cpp` with only its includes.

```
before: byte 4496  linked 4496  dynsym gaps 10  .rodata diff 0
after:  byte 3116  linked 4496  dynsym gaps 8 (ResourceTexture gap fixed)
        .rodata diff 9000  .text diff 475557
```

The ResourceTexture ctor moves to the `BuildResourceList` TU and its dynsym position
becomes correct, but the generated portrait string literals move with it and destroy the
`.rodata` order. Reverted.

## 7. Proven vs hypothesis

**Proven (measured above):**

* Unversioned `.dynsym` order is exactly the first-occurrence order of global names over
  the link inputs: 5370/5370 on our build.
* The full gold model (`hash = name_key ^ version_key`, ascending hash,
  `min(key, key^V)` for default-versioned symbols, `V = 7149`) reproduces our `.dynsym`
  5577/5577.
* `.hash` is exactly the SysV hash table over the `.dynsym` name sequence for both
  binaries (4099/4099 buckets, 5578/5578 chains).
* `.dynstr` is emitted in `.dynsym` order with version/NEEDED/SONAME strings at their
  namepool insertion points; both binaries have the same extras and tail.
* The 10 remaining defined-symbol gaps are 5 first-occurrence/TU issues; moving a TU
  changes the order exactly as the model predicts (Experiment A).
* The `RecordHandler` gap can be fixed in dynsym by merging `NdkAutosave.cpp` into
  `NdkHooks.cpp` (Experiment B), but the current clang emission order moves the PLT slots
  and costs 519 byte_exact.

**Hypothesis / not yet proven:**

* The original's exact object link order and TU membership are not recoverable from the
  stripped binary; the model only proves that *some* insertion order produces the observed
  `.dynsym`. The remaining 10 gaps are consistent with the known TU divergences
  (`NdkAutosave`, `LODManager`, `ResourceMaterial`, libzip/libcrypto archive members).
* The original's `V` is 7149 (fits the versioned order with our namepool keys within
  1–2 positions); the residual 22 versioned gaps are namepool-index differences, not a
  different hash function.
* A codegen-preserving way to emit `ndk_autosave`'s body after `checkFirst...` while
  keeping its first call in `ndk_checkPlaytime...` would land the RecordHandler dynsym fix
  without the PLT regression. Not found in this pass.

## 8. Reproduction commands

```sh
cd /home/debian/GalaxyOnFire2/wt/w3567
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# dynsym alignment (defined symbols)
python3 - <<'PY'
import sys, difflib
sys.path.insert(0, 'tools/verify')
from elf import Elf32, SHT_DYNSYM
def defined(p):
    return [s['name'] for s in Elf32(p)._symbols(SHT_DYNSYM) if s['shndx'] != 0]
o = defined('../_work/bins/android_2.0.16_libgof2hdaa.so')
u = defined('cmake-build-match/libgof2hdaa.so')
sm = difflib.SequenceMatcher(None, o, u, autojunk=False)
print('same index', sum(a == b for a, b in zip(o, u)), '/', len(o))
print('ratio', sm.ratio(), 'blocks', len(sm.get_matching_blocks()),
      'matched', sum(b.size for b in sm.get_matching_blocks()))
PY

# section byte diff
python3 - <<'PY'
import sys
sys.path.insert(0, 'tools/verify')
from elf import Elf32
def sec(p, n):
    e = Elf32(p); s = next(x for x in e.sections if x['name'] == n)
    return e.data[s['offset']:s['offset']+s['size']]
O = '../_work/bins/android_2.0.16_libgof2hdaa.so'
U = 'cmake-build-match/libgof2hdaa.so'
for n in ('.dynsym', '.dynstr', '.hash', '.gnu.version', '.plt', '.got',
          '.rodata', '.data', '.data.rel.ro'):
    a, b = sec(O, n), sec(U, n)
    print('%-14s %d/%d diff %d' % (n, len(a), len(b),
          sum(x != y for x, y in zip(a, b)) + abs(len(a)-len(b))))
PY
```

The full model scripts are inlined in §3–§5. `dynsym_dump.py` is not needed; the
`Elf32._symbols(SHT_DYNSYM)` helper from `tools/verify/elf.py` is sufficient.
