# Removing `__builtin_assume` — three sites, all load-bearing (w3523)

Status: research note (2026-11-01). Every number below is from a command run against
`../_work/bins/android_2.0.16_libgof2hdaa.so` or a controlled rebuild of this tree.
Base commit `16e5c9a4` (branch `wave87-w3523`).

## TL;DR

There are exactly three `__builtin_assume` uses in `src/`:

| # | site | assume |
|---|---|---|
| 1 | `src/game/core/String.cpp:566` (`String::Compare(const char*)`) | `!reachedEnd \|\| d == -1` |
| 2 | `src/game/ui/TouchButton.cpp:544` (`TouchButton::draw`) | `canvasTop != 0` |
| 3 | `src/game/world/StarMap.cpp:876` (`StarMap::draw`) | `!(transitionIn && transitionOut)` |

All three are **load-bearing**: deleting them changes clang 7.0.2 codegen. Sites 1 and 2 are
byte-exact with the assume and lose byte-exactness without it (site 2 also changes the function
size by 4 B, which shifts `.text` and breaks ~1500 downstream functions). Site 3 is already a
fuzzy match (94.8%); deleting the assume is gate-neutral but drops it to 94.4%.

No plain-C++ replacement was found for any of the three. The assume in each case encodes a fact
that clang's optimizer uses (i32 arithmetic / pointer liveness / flag exclusivity) but that the
surrounding C++ does not otherwise express. The note records the exact search so a later pass
does not repeat it.

## Baseline (measured, not quoted)

The task brief said "baseline is byte 4494, linked 4496"; the actual current HEAD is **byte
4495**, linked 4496. Recorded honestly:

```
cmake --build cmake-build-match --target check -j8
```
```
verify: byte 4495  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```
```
cmake --build cmake-build-match --target drift -j8
```
```
=== DRIFT: 0 field(s) across 0 class(es) ===
```
Lint is clean (`goal_lint.py` run by `check`). Per-function baseline from
`cmake-build-match/verify/report.json`:

| function | size | pct | byte |
|---|---:|---:|---|
| `String::Compare(const char*)` | 68 | 100.0 | exact |
| `TouchButton::draw` | 1376 | 100.0 | exact |
| `StarMap::draw` | 2644 | 94.8 | not exact |

## Method: a fast single-TU oracle

A full `check` takes ~25 s; a single-TU compile takes ~0.2 s. For screening variants I compiled
the real TU with the exact flags from `cmake-build-match/compile_commands.json`, replacing only
`-o` and the source path, then disassembled the one function from the object:

```python
# /tmp/compile_one.py  (src, out, [real-source])
import json, subprocess, sys, shlex
src, out = sys.argv[1], sys.argv[2]
real = sys.argv[3] if len(sys.argv) > 3 else src
d = json.load(open('/home/debian/GalaxyOnFire2/wt/w3523/cmake-build-match/compile_commands.json'))
e = next(x for x in d if x['file'] == src)
cmd = shlex.split(e['command'])
i = cmd.index('-o'); cmd[i+1] = out
cmd[-1] = real
sys.exit(subprocess.run(cmd, cwd='/home/debian/GalaxyOnFire2/wt/w3523/cmake-build-match').returncode)
```
```python
# /tmp/fn_disasm.py  (obj, mangled)  -- extracts one function from `objdump -d`
```
The object disassembly of the baseline TU reproduces the original function instruction-for-
instruction (offsets relative to the function start); only literal-pool words and relocation
targets differ. Every candidate that looked exact at object level was then confirmed with a full
`cmake --build cmake-build-match --target check -j8`.

## Site 1 — `String::Compare(const char*)`

Original (68 B @ `0x72fc0`, `objdump -d --start-address=0x72fc0 --stop-address=0x73004`):

```
72fe8: 1ac0       subs  r0, r0, r3
72fea: f1bc 0f00  cmp.w ip, #0
72fee: bf18       it    ne
72ff0: f04f 30ff  movne.w r0, #4294967295
72ff4: b240       sxtb  r0, r0
72ff6: 4770       bx    lr
```

Deleting the assume (identical source otherwise) gives the **same 68 B but not the same bytes**:
`sxtb` is scheduled *before* the `it ne`/`movne` select:

```
28: 1ac0       subs  r0, r0, r3
2a: f1bc 0f00  cmp.w ip, #0
2e: b240       sxtb  r0, r0
30: bf18       it    ne
32: f04f 30ff  movne.w r0, #4294967295
```

Full gate for the delete-only variant:

```
verify: byte 4494  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
  WARN  linked_exact 4496 -> 4495 (transient band)
  FAIL  byte_exact 4495 -> 4494
RATCHET FAIL
```

### Why: the assume forces i32 arithmetic

`clang++ -S -emit-llvm` on the real TU, `String::Compare(const char*)` tail:

with assume (baseline):
```llvm
%36 = zext i16 %29 to i32
%37 = zext i8 %28 to i32
%38 = sub nsw i32 %36, %37
%39 = select i1 %30, i32 -1, i32 %38
%40 = icmp eq i32 %39, -1
%41 = xor i1 %30, true
%42 = or i1 %40, %41
tail call void @llvm.assume(i1 %42)
%43 = shl i32 %39, 24
%44 = ashr exact i32 %43, 24          ; sxtb after the select
```
without assume:
```llvm
%36 = trunc i16 %29 to i8
%37 = sub i8 %36, %28
%38 = sext i8 %37 to i32
%39 = select i1 %30, i32 -1, i32 %38  ; sxtb folded into the operand
```
The assume's `icmp eq i32 %39, -1` is the only i32 use of the post-select value; it inhibits
instcombine's `trunc(sub)` -> `sub(trunc)` narrowing. Without it clang narrows the subtraction
to i8 and sinks the sign extension into the select operand.

### Search (all object-level; none exact)

Each form was substituted for the `int d = ...; if (reachedEnd) d = -1; assume; return
(signed char)d;` tail, compiled with the real TU flags, and compared to the baseline object
disassembly. `diff` = differing instruction lines (baseline is 0).

| form | diff |
|---|---:|
| delete assume only | 2 |
| `if (a[i] == 0) d = -1;` | 2 |
| `if (!a[i]) d = -1;` | 2 |
| `if (reachedEnd && a[i] == 0) d = -1;` | 2 |
| `d = a[i] == 0 ? -1 : d;` | 2 |
| `return (signed char)(reachedEnd ? -1 : d);` | 2 |
| `d = reachedEnd ? -1 : d;` | 2 |
| `if (reachedEnd) return -1; return (signed char)d;` | 12 |
| `if (reachedEnd \|\| d == -1) d = -1;` | 4 |
| `if (d == -1) return -1; return (signed char)d;` | 4 |
| `signed char result; ... result = (signed char)d; return result;` | 3 |
| `return (unsigned char)d;` | 3 (wrong value: `movne #255`) |
| `short`/`unsigned short`/`char`/`long`/`int8_t`/`int16_t`/`int32_t`/`int64_t d` | 2 each |
| `if (d == -1) { }`, `while (d == -1) break;`, `switch (d) { case -1: ... }`, `goto` | 2 each |
| `if (!(!reachedEnd \|\| d == -1)) __builtin_unreachable();` | 2 |
| `if (reachedEnd && d != -1) __builtin_unreachable();` | 2 |

Also tested: `(signed char)(d & 0xff)`, `(signed char)((d << 24) >> 24)`, explicit `(int)`
casts, `d -= ...`, `unsigned int d`, `signed char d`, `long d`, `volatile`-free temporaries,
and compiling the TU as Objective-C++ (`-x objective-c++`). All narrow to i8.

**Conclusion (site 1):** the assume is required. A plain-C++ form that keeps the i32 subtraction
was not found; every form that does not carry an `llvm.assume`-style i32 use is narrowed.

## Site 2 — `TouchButton::draw`

Original is 1376 B @ `0x1693d8` and byte-exact. Deleting the assume only:

* size 1380 B (+4);
* one register-allocation difference around the first `min(rightWidth, filled)`:

```
baseline:  28c: 4640  mov r0, r8        ; save savedSpacing
           28e: ... ldr r8, [r5,#160]
           2bc: 4680  mov r8, r0        ; restore

no assume: 28c: 4658  mov r0, fp        ; save savedColor
           28e: 46c3  mov fp, r8        ; fp = savedSpacing
           290: ... ldr r8, [r5,#160]
           2c0: 46d8  mov r8, fp
           2c2: 4683  mov fp, r0
```

Full gate:

```
verify: byte 2973  linked 4495  stubs 0  missing 0  extra 0  avg 99.89
  WARN  linked_exact 4496 -> 4495 (transient band)
  FAIL  byte_exact 4495 -> 2973
RATCHET FAIL
```

The 1522-function collapse is the +4 B size change: `.text` grows `0x1337c8` -> `0x1337e8`, so
every later function moves and loses address-exact PC-relative references. This makes site 2 the
most expensive of the three to get wrong.

### Search (all object-level; none exact)

`canvasTop` is otherwise dead (`PaintCanvas *canvasTop = Globals::Canvas;` at entry, used only by
the assume). Tested replacements at the assume site, and uses of `canvasTop` at each of the 36
`Globals::Canvas->` call sites in the function:

* `(void) canvasTop;`, `if (canvasTop) { }`, `if (canvasTop == 0) { }`, `{ }`, `do { } while (0)`;
* `if (canvasTop == 0) break;`, `if (canvasTop == 0) goto restoreDrawState;`, `if (canvasTop == 0) return;`;
* `if (canvasTop == 0) imageToDraw = 0;`, `if (canvasTop == 0) (void) canvasTop->GetColor();`,
  `if (canvasTop == 0) (void) canvasTop->initialized;`;
* `if (canvasTop != Globals::Canvas) { }`, `canvasTop = Globals::Canvas; if (canvasTop == 0) { }`;
* `canvasTop->` substituted for `Globals::Canvas->` at every single call site (36 variants), and
  at the tail `SetColor(savedColor)` / `FontSetSpacing(...)`;
* `if (!canvasTop) __builtin_unreachable();`, `if (canvasTop == 0) __builtin_unreachable();`,
  bare `__builtin_unreachable();`;
* moving the `canvasTop` declaration after `savedColor`, deleting `canvasTop` entirely, changing
  `savedColor`/`savedSpacing` types.

The best of these (`__builtin_unreachable` conditional forms, `if (canvasTop == 0) break;`, etc.)
is still the same 11-line register-allocation diff as the plain delete. Using `canvasTop` at any
call site changes more (5–255 diff lines). The assume is the only construct that keeps the
canvas-pointer load live without emitting an instruction.

**Conclusion (site 2):** the assume is required. It is a register-allocation/liveness anchor, not
a semantic guard: `canvasTop` is never dereferenced and the original does not cache the pointer
in a register (it reloads through `&Globals::Canvas` at every call).

## Site 3 — `StarMap::draw`

Original is 2644 B @ `0xc90f4`, 94.8% (not byte-exact). Deleting the assume keeps the size
(2644 B) and the gate (`byte 4495 / linked 4496`, `RATCHET PASS`) but drops the function:

| variant | pct | size | gate |
|---|---:|---:|---|
| baseline (with assume) | 94.8 | 2644 | 4495 / 4496 |
| delete assume, keep `bool in` | 90.6 | 2644 | 4495 / 4496 |
| delete assume + `bool in` -> `this->transitionIn` (`no_in_var`) | 94.4 | 2644 | 4495 / 4496 |
| `if (this->transitionIn \|\| this->transitionOut)` first guard | 90.7 | 2644 | 4495 / 4496 |
| `in != this->transitionOut` second guard | 90.6 | 2644 | 4495 / 4496 |
| `in \| this->transitionOut` | 90.6 | 2644 | 4495 / 4496 |
| `bool in = transitionIn && !transitionOut` | 94.4 (object diff 184 vs 180) | 2644 | — |

Full `check` for the best no-assume variant (`no_in_var`):
```
verify: byte 4495  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
RATCHET PASS
```
with `_ZN7StarMap4drawEv 94.4 False False 2644 2644`.

The assume lets clang simplify the second guard `mode != 3 || in || transitionOut` using
`!(transitionIn && transitionOut)` (the two flags are mutually exclusive, so `in || out` is
`in != out`). Without the fact the compiler emits the more expensive `||` form. No plain-C++
formulation tested re-establishes the fact: `in = transitionIn && !transitionOut` does not
propagate to the second guard.

**Conclusion (site 3):** the assume is required for the 94.8% match. Deleting it is gate-neutral
(StarMap is not byte-exact either way) but is a 0.4% fuzzy regression, so it is not a strict
improvement.

## Verdict

| site | delete-only gate | strict improvement? |
|---|---|---|
| `String::Compare` | byte 4495 -> 4494, linked 4496 -> 4495, FAIL | no (regression) |
| `TouchButton::draw` | byte 4495 -> 2973, linked 4496 -> 4495, FAIL | no (regression) |
| `StarMap::draw` | byte 4495 / linked 4496, PASS, pct 94.8 -> 94.4 | no (fuzzy regression) |

No flag or source change is committed with this note. The three assumes stay, each with its
`// lint: arm_builtin` waiver. Removing them requires either (a) recovering the original source
fact that produced the same `llvm.assume` (most likely an assert/assume macro in the original
source), or (b) a different clang-7 codegen path that keeps i32 arithmetic / the pointer live /
the flag exclusivity without an intrinsic — neither was found.

## Proven vs hypothesis

**Proven (measured):**
- Baseline at `16e5c9a4` is byte 4495 / linked 4496 / avg 99.89 / data 843/843 / imports 207 /
  lint clean / drift 0.
- Each of the three assumes is required for the current match; the delete-only numbers above are
  from full `check` runs (sites 1 and 2) or full `verify` runs (site 3).
- Site 1's mechanism is the i32-vs-i8 narrowing shown in the optimized IR.
- Site 2's mechanism is a 4-byte codegen change that shifts `.text` by 0x20 and breaks 1522
  downstream byte-exact functions.
- Site 3's mechanism is the second-guard simplification; the best no-assume form is 94.4%.

**Hypothesis (not proven):**
- The original source likely contained an assume/assert-like fact at these three points (the
  original binary's instruction stream needs the same facts). We did not recover the original
  source text, so this is inference from codegen, not from a source witness.
- A future pass might find a plain-C++ form outside the search space above (e.g. via a different
  local lifetime or an original-source idiom); the search here is exhaustive over the forms
  listed, not over all C++.

## Reproduction

```bash
# baseline
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target drift -j8

# site 1 delete-only (edit String.cpp, then)
cmake --build cmake-build-match --target check -j8     # byte 4494 / linked 4495 / FAIL

# site 2 delete-only (edit TouchButton.cpp, then)
cmake --build cmake-build-match --target check -j8     # byte 2973 / linked 4495 / FAIL

# site 3 best no-assume (edit StarMap.cpp, then)
cmake --build cmake-build-match --target check -j8     # byte 4495 / linked 4496 / PASS, pct 94.4

# single-function side-by-side
python3 tools/verify/verify.py \
  --build-dir /tmp/verify_w3523 \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours cmake-build-match/libgof2hdaa.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --show _ZN11AbyssEngine6String7CompareEPKc
```
(`FN=... cmake --build ... --target verify-fn` expands `$ENV{FN}` empty in this tree; use
`verify.py --show` directly.)
