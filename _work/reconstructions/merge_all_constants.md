# `-fmerge-all-constants` — A/B and binary evidence (w3421)

Status: research note, 2026-11-01. Worktree `wt/w3421`, branch `wave87-w3421`, base
`d00e0030` (master). Every number below is from a command run in this worktree against
`../_work/bins/android_2.0.16_libgof2hdaa.so`.

## TL;DR

**`-fmerge-all-constants` is a complete no-op on this tree.** All 204 app object files are
byte-identical with and without it; the linked `.so` is byte-identical except the 20-byte
SHA-1 build-id, and that difference is caused only by the scratch build directory path being
embedded in DWARF `.debug_str` (gold hashes the pre-strip output for the build-id).

| gate | baseline | `-fmerge-all-constants` |
|---|---:|---:|
| `byte_exact` | **4182** | **4182** |
| `linked_exact` | **4495** | **4495** |
| compared | 4524 | 4524 |
| `stub_zero_size` / `missing` / `wrong_type` / `extra` | 0 / 0 / 0 / 0 | 0 / 0 / 0 / 0 |
| `avg_match` | 99.89 | 99.89 |
| exported data exact | 843 / 843 | 843 / 843 |
| sodiff extra / missing | 0 / 0 | 0 / 0 |
| undefined imports | 207 | 207 |
| lint | `CLEAN ✓` | `CLEAN ✓` |
| drift | 0 fields | 0 fields |
| ratchet | `RATCHET PASS` | `RATCHET PASS` |

**Why it is a no-op:** in clang 7.0.2 the flag is consulted in exactly one place,
`clang/lib/CodeGen/CGDecl.cpp:1200` (release 7.0.0), inside `EmitAutoVarAlloca`. It only
promotes a **function-local** variable to a static global when *all* of these hold:

* the type is an array or a record (`Ty->isArrayType() || Ty->isRecordType()`);
* it has an initializer and is `constexpr` or a POD with a constant initializer;
* `MergeAllConstants && !NRVO && !isByRef && CGM.isTypeConstant(Ty, true)`.

A namespace-scope `static const` array, a scalar local (`const int x = 5`), and a pointer
local are **not** affected. An AST scan of all 204 TUs finds **zero** function-local `const`
arrays with a constant initializer (and no real local `const` record either), so the flag has
nothing to act on. The tree's lookup tables are already namespace-scope `static const`, whose
codegen is identical to the flag-promoted form.

**Original-build evidence:** ndk-build r18b's `build/core` never passes
`-fmerge-all-constants` (grep below), and the original's local-array codegen is the default
copy-from-`.rodata`-to-stack form (concrete example below). The flag's promoted form is
indistinguishable from a `static const` array, so even if the original had used it, the tree's
`static const` tables already reproduce that codegen. **No flag/source change was committed;
this note is the only change.**

## 1. Baseline (current tree, no flag)

```
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8
```

```
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
```

`.so` size: 2,187,328 bytes; `.text` 0x1337c8 @ 0x69440, `.rodata` 0x41b67 @ 0x1c9e10,
`.ARM.exidx` 0x6f70, `.ARM.extab` 0x26298, `.data.rel.ro` 0x2fe8, `.data` 0x296c,
`.bss` 0x69ac, `.got` 0x430c, `.dynamic` 0x148 — all size- and base-exact.

## 2. Controlled A/B

The toolchain sets `CMAKE_CXX_FLAGS` / `CMAKE_C_FLAGS` with `FORCE`, so the flag was added
through a wrapper toolchain (no edit to the tracked toolchain file):

```bash
mkdir -p /tmp/w3421
cat > /tmp/w3421/toolchain_merge.cmake <<'X'
include("/home/debian/GalaxyOnFire2/wt/w3421/cmake/toolchain.cmake")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmerge-all-constants" CACHE STRING "" FORCE)
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fmerge-all-constants" CACHE STRING "" FORCE)
X
cmake -S . -B /tmp/w3421/build-merge -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=/tmp/w3421/toolchain_merge.cmake -DGOF2_MATCH_OPT=-Oz
cmake --build /tmp/w3421/build-merge --target link_so -j16
cmake --build /tmp/w3421/build-merge --target check   -j8
cmake --build /tmp/w3421/build-merge --target drift   -j8
cmake --build /tmp/w3421/build-merge --target lint    -j8
```

The scratch build passes `-fmerge-all-constants` to every app TU (verified in
`/tmp/w3421/build-merge/compile_commands.json`). Gate output:

```
verify: byte 4182  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
=== DRIFT: 0 field(s) across 0 class(es) ===
CLEAN ✓ (no new soft violations; hard invariants 0)
```

`readelf -SW`, `readelf -lW` and `readelf -dW` of the two `.so` files are **identical**
(section names, types, addresses, sizes, alignments, entsizes, program headers, dynamic tags).

## 3. Object-level proof (the flag changes nothing)

### 3.1 Same-directory A/B, all 204 TUs

Each TU was compiled twice from its exact `compile_commands.json` command line, in the same
working directory, once with and once without `-fmerge-all-constants`, and the resulting
objects compared with `cmp`:

```
TUs=204 identical=204 differing=0
```

Script: `/tmp/w3421/compile_ab_all.py` (reads `cmake-build-match/compile_commands.json`,
removes `-o`, appends the flag, compiles both variants, SHA-256 compares). Because the two
objects are produced in the same directory, even `.debug_str` matches — the flag changes
literally no byte of any object.

### 3.2 Full scratch build vs baseline build

All 204 objects from the two build directories differ only in `.debug_str` (the
`DW_AT_comp_dir` is the build directory path). After `objcopy --strip-debug`:

```
objects=204 differing_after_strip_debug=0
```

The two linked `.so` files differ in exactly 20 bytes, at file offsets 476..495, which is the
20-byte SHA-1 payload of `.note.gnu.build-id` (the note itself is 0x24 bytes at 0x1cc:
namesz/descsz/type/name = 16 bytes, desc = 20 bytes). Zeroing those 20 bytes makes the two
files `cmp`-identical:

```bash
python3 - <<'X'
for p in ('a.so','b.so'):
    d=bytearray(open(p,'rb').read()); d[476:496]=b'\0'*20; open(p,'wb').write(d)
X
cmp a.so b.so        # identical
```

The build-id differs only because gold hashes the pre-strip output, which includes
`.debug_str` with the different build-directory path. This is not a code/data difference.

## 4. What the flag actually does in clang 7.0.2

`clang/lib/Frontend/CompilerInvocation.cpp:628`:

```cpp
Opts.MergeAllConstants = Args.hasArg(OPT_fmerge_all_constants);
```

`clang/lib/CodeGen/CGDecl.cpp:1192-1207` (release 7.0.0), inside `EmitAutoVarAlloca`:

```cpp
      // If the variable's a const type, and it's neither an NRVO
      // candidate nor a __block variable and has no mutable members,
      // emit it as a global instead.
      if ((!getLangOpts().OpenCL || Ty.getAddressSpace() == LangAS::opencl_constant) &&
          (CGM.getCodeGenOpts().MergeAllConstants && !NRVO && !isByRef &&
           CGM.isTypeConstant(Ty, true))) {
        EmitStaticVarDecl(D, llvm::GlobalValue::InternalLinkage);
        ...
```

The enclosing branch requires `D.getInit() && (Ty->isArrayType() || Ty->isRecordType())`
and either `D.isConstexpr()` or a POD with a constant initializer. So the flag is **only**
about function-local `const` aggregates with constant initializers.

### 4.1 Measured codegen (NDK r18b clang 7.0.2, exact tree flags)

Test TU (`/tmp/w3421/t/test4.cpp`):

```cpp
extern "C" int use(const int *);
extern "C" int f(int i) { const int a[4] = {1,2,3,4}; return use(a) + a[i & 3]; }
extern "C" int g(int i) { const int a[4] = {1,2,3,4}; return use(a) + a[i & 3]; }
```

| variant | `.text` | `.rodata` | code shape |
|---|---:|---:|---|
| default (`-Oz`) | 0x90 | `.rodata.cst16` 0x10 | `alloca` + `vld1.64 {d16-d17},[rodata]` + `vst1.64` to stack; the two identical arrays are **merged into one** `private unnamed_addr` constant |
| `-fmerge-all-constants` | 0x40 | plain `.rodata` 0x20 | direct `add rX, pc` to `.rodata`, no stack copy; the two arrays are **not** merged (two `internal constant` copies) |

Namespace-scope `static const int a[4]` (`test5.cpp`) produces the same 0x40-byte/plain-
`.rodata` form as the flag variant, and is unchanged by the flag. That is exactly the form
the tree's lookup tables already use.

Also note: clang 7.0.2 does **not** accept `-fno-merge-constants` (it warns
`optimization flag '-fno-merge-constants' is not supported` and ignores it), so the
`-fno-merge-constants` row in `flag_matrix_2.md` was a no-op for that reason too.

## 5. Why the tree has no eligible variables

AST scan of all 204 TUs (`-Xclang -ast-dump -fsyntax-only`, exact compile commands; the
scanner walks the dump tree and keeps `VarDecl`s whose nearest ancestor is a function-like
decl, that are not `static`, have a `const` aggregate type, and have `cinit`/`init`):

```
local const arrays: 0
local const records: 147   # all scalars / references / libc++ size typedefs
```

The 147 "records" are false positives: their types are libc++ size typedefs (`const _Size`
= `size_t`, 110; `const _RandomAccessIterator1`, 22; `const _RandomAccessIterator`, 11) or
references (`const LightColor &`, 4), not record objects. There is no function-local `const`
array in `src/` at all. The only local arrays with constant initializers are non-`const`
(`int coords[6] = {...}` etc.), which the flag does not touch.

Scanner: `/tmp/w3421/scan_ast4.py` (self-contained; reads `compile_commands.json`). The
result is consistent with §3: nothing for the flag to promote.

## 6. Vendored libraries

The original's `APP_CFLAGS` (if it had the flag) would also apply to the vendored
zlib/libzip/libcrypto TUs. Each was compiled twice with the exact flags from
`third_party/build_match_lib.cmake`, with and without `-fmerge-all-constants`:

| library | units | identical |
|---|---:|---:|
| zlib 1.2.3 subset | 11 | 11 / 11 |
| libzip 0.9.3 subset | 53 | 53 / 53 |
| libcrypto 1.1.0l `sha256.c` (GCC 4.9) | 1 | 1 / 1 |

So the vendored archives are also unaffected.

## 7. Original-build evidence

### 7.1 ndk-build r18b default flags do not include it

```
$ grep -rn "merge-all-constants\|merge-constants" \
      .cache/ndk/android-ndk-r18b/build/
(no output)
```

The original is an ndk-build r18b build (`build_system_evidence.md`); the flag is not part of
`build/core`, so it could only have come from the app's own `Android.mk` `APP_CFLAGS` /
`LOCAL_CFLAGS`, and no original build script survives.

### 7.2 The original's local-array codegen is the default copy-to-stack form

`Level::createCampaignMission` at `0xb4e36` (original):

```
b4e36: ldr  r0, [pc, #112]      ; literal at 0xb4ea8 = 0x148622
b4e38: add  r4, sp, #384        ; destination = stack
b4e3a: add  r0, pc              ; r0 = 0x1fd460 (.rodata)
b4e3c: vld1.64 {d16-d17}, [r0]!
b4e40: vldr d18, [r0]
b4e44: mov  r0, r4
b4e46: vst1.64 {d16-d17}, [r0]!
b4e4a: vstr d18, [r0]
...
b4e58: movs r2, #6
b4e5a: blx  Route::Route(int*, int)@plt
```

The 24 bytes at `0x1fd460` are `00 00 00 00 00 00 00 00 00 00 00 00 c0 cf 6a 00 c0 cf 6a 00
c0 cf 6a 00` = `{0, 0, 0, 7000000, 7000000, 7000000}` — exactly
`src/game/world/Level.cpp:3862`, `int coords[6] = {0, 0, 0, 7000000, 7000000, 7000000};`.
The original copies it to the stack because the local is non-`const`; the flag would not
change that. This is the same default codegen the tree produces (the function is byte-exact).

### 7.3 The promoted form is indistinguishable from `static const`

With the flag, a local `const` array is emitted as an `internal constant` in plain `.rodata`
and used directly. A namespace-scope `static const` array has the same codegen. Therefore,
even if the original's source had local `const` lookup tables and used the flag, the tree's
`static const` tables already reproduce the original's codegen — the flag is not needed to
match them.

## 8. Proven vs hypothesis

**Proven (measured):**

* `-fmerge-all-constants` changes no byte of any of the 204 app objects (same-directory A/B).
* The linked `.so` is identical except the build-id, and the build-id difference is explained
  by the different build-directory path in `.debug_str` (all objects identical after
  `--strip-debug`; `.so` identical after zeroing the 20-byte build-id desc).
* Gate is unchanged: byte 4182, linked 4495, size 4524, data 843/843, sodiff 0/0, imports
  207, lint clean, drift 0.
* The tree has zero function-local `const` arrays with constant initializers (AST scan).
* zlib/libzip/libcrypto objects are unchanged by the flag.
* ndk-build r18b's `build/core` does not pass the flag.
* The original's `Level::createCampaignMission` local array is copied from `.rodata` to the
  stack (default form), matching the tree's byte-exact reconstruction.

**Hypothesis / not decidable without the original source:**

* Whether the original's `Android.mk` explicitly passed `-fmerge-all-constants`. No build
  script survives; the binary cannot distinguish the flag from `static const` for the
  constructs it affects (both produce the same promoted codegen), and the tree has no
  eligible local `const` aggregates to test it on.
* Whether the original source had any function-local `const` aggregates at all. If it did,
  the flag would have promoted them; our `static const` reconstruction is codegen-equivalent,
  so the flag would still be unnecessary.

**Conclusion:** `-fmerge-all-constants` is neutral and unobservable on this tree; there is no
evidence the original used it. Do not add it. No flag/source change was made.

## Appendix A — same-directory A/B compiler (all TUs)

```python
# /tmp/w3421/compile_ab_all.py  (run from the repo root)
import json, shlex, subprocess, os, hashlib
from concurrent.futures import ThreadPoolExecutor
cc = json.load(open('cmake-build-match/compile_commands.json'))
def work(idx_e):
    idx, e = idx_e
    parts = shlex.split(e['command'])
    base, i = [], 0
    while i < len(parts):
        if parts[i] == '-o': i += 2; continue
        base.append(parts[i]); i += 1
    h = []
    for tag, extra in (('def', []), ('merge', ['-fmerge-all-constants'])):
        out = f"/tmp/w3421/aball/{idx}_{tag}.o"
        subprocess.run(base + extra + ['-o', out], check=True, capture_output=True)
        h.append(hashlib.sha256(open(out, 'rb').read()).hexdigest())
        os.unlink(out)
    return (e['file'].split('/src/')[-1], h[0] == h[1])
os.makedirs('/tmp/w3421/aball', exist_ok=True)
with ThreadPoolExecutor(max_workers=16) as ex:
    res = list(ex.map(work, enumerate(cc)))
bad = [r for r in res if not r[1]]
print(f"TUs={len(res)} identical={len(res)-len(bad)} differing={len(bad)}")
for b in bad: print("DIFF", b[0])
```

## Appendix B — AST scan for eligible local `const` aggregates

```python
# /tmp/w3421/scan_ast4.py  (run from the repo root)
import json, shlex, subprocess, re
cc = json.load(open('cmake-build-match/compile_commands.json'))
FUNC = re.compile(r'(FunctionDecl|CXXMethodDecl|CXXConstructorDecl|CXXDestructorDecl|'
                  r'CXXConversionDecl|FunctionTemplateDecl|CXXRecordDecl|LambdaExpr)')
VD = re.compile(r'(?<!Parm)VarDecl\b')
PRIM = re.compile(r'const (int|unsigned|char|short|long|float|double|bool|size_t|wchar_t|'
                  r'uint\d+_t|int\d+_t|uintptr_t|intptr_t|ptrdiff_t|clock_t|time_t)\b')
def depth_of(line):
    i = 0
    while i < len(line) - 1:
        if line[i:i+2] in ('| ', '|-', '`-', '  '): i += 2
        else: break
    return i // 2
arr, rec = [], []
for e in cc:
    parts = shlex.split(e['command'])
    out, i = [], 0
    while i < len(parts):
        if parts[i] == '-o': i += 2; continue
        out.append(parts[i]); i += 1
    out += ['-Xclang', '-ast-dump', '-fsyntax-only']
    r = subprocess.run(out, capture_output=True, text=True, timeout=300)
    stack = []
    for ln in r.stdout.splitlines():
        d = depth_of(ln); txt = ln[d*2:]
        if VD.search(txt):
            anc = None
            for dd, k in reversed(stack):
                if dd < d and k in ('func', 'record'):
                    anc = k; break
            if anc == 'func' and 'static' not in txt and \
               ('cinit' in txt or ' init' in txt or 'listinit' in txt):
                q = re.findall(r"'([^']*)'", txt)
                ty = q[-1] if q else ''
                if ty.startswith('const ') and '[' in ty:
                    arr.append((e['file'], txt.strip()))
                elif ty.startswith('const ') and '*' not in ty and not PRIM.match(ty) \
                     and '<' not in ty and '::' not in ty:
                    rec.append((e['file'], txt.strip()))
        while stack and stack[-1][0] >= d: stack.pop()
        kind = None
        if FUNC.search(txt):
            kind = 'func' if any(s in txt for s in ('Function', 'Method', 'Constructor',
                                                    'Destructor', 'Conversion', 'Lambda')) else 'record'
        stack.append((d, kind))
print("local const arrays:", len(arr))
for h in arr: print("  ", h[0].split('/src/')[-1], '::', h[1][:160])
print("local const records:", len(rec))
for h in rec[:50]: print("  ", h[0].split('/src/')[-1], '::', h[1][:160])
```

## Appendix C — codegen probes

`test4.cpp` (local `const` array):

```cpp
extern "C" int use(const int *);
extern "C" int f(int i) { const int a[4] = {1,2,3,4}; return use(a) + a[i & 3]; }
extern "C" int g(int i) { const int a[4] = {1,2,3,4}; return use(a) + a[i & 3]; }
```

`test5.cpp` (namespace-scope `static const` array):

```cpp
extern "C" int use(const int *);
static const int a[4] = {1,2,3,4};
static const int b[4] = {1,2,3,4};
extern "C" int f(int i) { return use(a) + a[i & 3]; }
extern "C" int g(int i) { return use(b) + b[i & 3]; }
```

Compile with the tree's exact flags (from `compile_commands.json`), once as-is and once with
`-fmerge-all-constants`, then compare `readelf -SW` and `objdump -d`.
