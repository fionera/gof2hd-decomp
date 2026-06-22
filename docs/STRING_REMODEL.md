# Scoping: re-model `AbyssEngine::String` to the binary's native layout

## Why this is its own effort (and bigger than "1 wrong_type")

The last `verify` wrong_type is `String::~String` (the D0 deleting destructor). It cannot be fixed in
isolation: it is a symptom of `String` being modeled as a `std::u16string` wrapper instead of the
binary's native string. The same root cause depresses a large swath of the match:

- **String methods today: 289 comparisons, only 27 linked-exact, avg 38.5%, byte-exact 3.** They
  don't match because every body operates on `std::u16string s` instead of the original's manual
  UTF-16 buffer.
- **String D0** is absent (a non-`std::u16string` destructor structure the original ships).
- **String-using callers**: 51 files / 155 sites reach into `.s`/`->s` directly (std::u16string
  internals), so those functions also diverge from the original.

Re-modeling String to its native layout is the single highest-leverage byte-match move left: it
should pull most of those ~260 non-matching String comparisons toward linked-exact, resolve String
D0, and unblock the callers — not merely flip one gate entry.

## Ground truth (from Ghidra, `android_2.0.16`, image_base 0x10000)

Native layout — **12 bytes**:

```cpp
struct String {            // sizeof == 0x0c
    void*           vptr;   // +0x00  set in ctor to (vtable_data + 8); see "open question" below
    unsigned short* data;   // +0x04  heap UTF-16 buffer, NUL-terminated, operator new[]/delete[]
    int             length; // +0x08  code-unit count (excludes NUL)
};
```

Evidence:
- `String::String()` (0x72398): `vptr = *(int*)(lit)+8; data = 0; length = 0;`
- `String::~String` D1 (0x72594): `vptr = (lit)+8; if (data) operator delete[](data); data = 0;`
- `String::~String` D0 (0x725b8): `~String(this) /*D1*/; operator delete(this);` (standard deleting dtor)
- `String::Set(char const*)` (0x724b4): frees old `data`, `length = strlen`, `data = new ushort[...]`,
  widens each byte to 16-bit, NUL-terminates.
- `String::operator[](int)` (0x72378): bounds-check `param < length`, returns `data + param`, else a
  shared static NUL.

**Open question for Phase 0:** there is **no `_ZTV`/`_ZTI`/`_ZTS` symbol** for String, yet ctor/dtor
set `vptr = vtable_data + 8` (i.e. past offset-to-top + typeinfo to the first function slot) and no
String method ever *dispatches* through `vptr`. Two possibilities, decide before coding:
1. **Virtual destructor only** — String is polymorphic; the vtable `[0][typeinfo][D1][D0]` exists but
   its symbol was stripped. Modeling `virtual ~String()` makes clang plant the vptr + emit D0/D1/D2
   automatically. Risk: our build emits `_ZTVN11AbyssEngine6StringE` (data) which the stripped binary
   lacks → may show as one `extra` (vtables are data, likely filtered — confirm).
2. **Manual header pointer** — offset 0 is a non-functional tag/base-subobject pointer, and D0 comes
   from an explicit out-of-line dtor whose address is taken by a container's element-destructor table
   (the `delete (String*)` thunk at 0x6f678 + table @0x220b68). Then we model it as a real first
   field set explicitly.
   Resolve by reading the literal the ctor loads (find the vtable/struct it points at) and checking
   for a typeinfo word before the function slots.

## Native method set to re-implement (~50)

`String::` symbols at 0x72378–0x72e7c: ctors `String()`, `String(char const*,bool)`,
`String(unsigned short const*,bool)`, `String(String const&,bool)`, `String(char)`, `String(int)`,
`String(float)`, `String(long long)`; `~String` D0/D1/D2; `operator=`, `operator+=`(String/char/int/
float/long long), `operator[]` (mutable+const), `operator unsigned short*`/`unsigned short const*`;
`Set`(char const*/unsigned short const*/long long/float); `Compare`, `StrLen`(2), `Reverse`,
`ToUpperCase`, `ReplaceString`, `ReplaceChar`, `Split`, `SubString`, `IndexOf*`, `ValueOf`,
`GetAEChar`, `GetAEWChar`, `ConvertFromUTF8`, `getWCharFromUtf8`, and `AbyssEngine::operator+`.
The current `String.cpp` (626 lines) already encodes the *behavior* (CP-1252 case tables, Cyrillic
transliteration, Compare sentinels, Split/SubString semantics) — re-express each on the manual buffer
instead of `std::u16string`, decompiling the original at its address to match the body.

## Blast radius

- **AEString.h** (237 lines) — replace the `std::u16string s` member + the `AESTRING_SHIM`/`ctor`/
  `dtor`/`dtor_del` scaffolding with the native 3-field layout + real ctors/dtor.
- **String.cpp** (626 lines) — re-implement the ~50 methods on `data`/`length`.
- **51 files / 155 direct `.s` / `->s` sites** — callers that currently poke std::u16string internals
  (`.s.size()`, `.s.append()`, `.s[i]`, `.s.empty()`, …). Each must move to String's own API
  (`length`, `operator[]`, `operator+=`, `Set`, `Compare`, …) or the new fields. This is the
  error-prone bulk of the work; many are inside String.cpp itself.
- **164 TUs include AEString.h** — recompile surface (cost, not breakage). Expect long, flaky verify
  cycles; lean on the per-symbol `verify-fn` and `--only '11AbyssEngine6String'` slices.

## Phased plan

- **Phase 0 — settle the model.** Resolve the vptr open question (virtual-dtor vs header pointer) by
  reading the ctor's literal + the pointed-at data. Lock the exact struct (field order/types/size
  0x0c), the empty-string sentinel (`operator[]` OOB target), and the allocation convention
  (`operator new[]` sizing, NUL handling) from Ghidra. Write them into this doc.
- **Phase 1 — layout + lifecycle.** Swap AEString.h to the native layout; implement
  ctors/`~String`(D0/D1/D2)/`operator=`/copy on the manual buffer. Build-gate; confirm D0/D1/D2 (and
  vtable, if polymorphic) emit and that `String()`, `~String`, `Set` match in `verify-fn`. Expect a
  large temporary breakage of the 155 `.s` sites — keep them compiling with a thin transitional
  accessor only if needed, but prefer fixing them in Phase 3.
- **Phase 2 — methods.** Re-implement the remaining ~50 methods against `data`/`length`, decompiling
  each original. Verify the String slice (`--only '11AbyssEngine6String'`) climbs from 27 toward
  ~250+ linked-exact; never let it regress between methods.
- **Phase 3 — callers.** Convert the 51 files / 155 direct `.s` sites to String's API. Build-gate
  each TU. Watch that String-using functions' linked-exact rises (they were diverging via the wrapper).
- **Phase 4 — close + guard.** Full `verify`: String D0 gone (wrong_type 0), `extra` not increased
  (handle the `_ZTV` case), linked-exact net up. Re-enable `--fail-on-wrong-type`. Delete the
  `dtor`/`dtor_del`/`AESTRING_SHIM` scaffolding and update `[[string-d0-needs-remodel]]`.

## Risks / watch-items

- **ABI of by-value String.** Many functions take/return `String` by value (12-byte struct). The
  native layout + copy ctor must reproduce the original's by-value passing exactly, or every such
  caller shifts. Validate early with a by-value-heavy function (e.g. `GetStringLength(String)`).
- **`std::u16string` conveniences vanish.** `append/substr/rbegin/clear` become manual buffer ops;
  easy to introduce off-by-one / NUL / realloc bugs vs the original's exact sizing (note the
  `operator new[](len*2)` vs `(len+1)*2` branches in `Set`).
- **vtable as `extra`.** If we go polymorphic, confirm `_ZTVN11AbyssEngine6StringE` is filtered by
  `scope_filter`/`verify` (data, not a .text function) so it doesn't become a new `extra`.
- **Flaky verify at 164-TU rebuild scale.** The our_syms backstop already stabilizes wrong_type;
  still, prefer targeted `--only`/`verify-fn` over full runs while iterating.
- **No shortcuts.** Real `String::` methods (no `*_cs` free shims), real ctors/dtors (no `self`-taking
  frees), `#include` owners, recover receivers from asm — same rules as the rest of the port.

## Effort

Large / multi-session. ~50 method re-implementations + ~155 call-site conversions + a foundational
layout change across 164 TUs. Best run as its own branch with a method-by-method + file-by-file wave
loop (the existing `master_step`/wave machinery fits), gated on the String `verify` slice climbing and
the global linked-exact never regressing. Payoff: String D0 resolved **and** ~260 String comparisons
plus their callers moving toward byte-exact — the largest single correctness gain remaining.
