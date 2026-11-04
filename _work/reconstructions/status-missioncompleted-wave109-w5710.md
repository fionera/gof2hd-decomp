# `Status::missionCompleted` — remove the `.jcr` rank table (wave 109, w5710)

Status: **source change landed**. Worktree `wt/w5710`, branch `wave109-w5710`,
base master `7c34c4098`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Target: `_ZN6Status16missionCompletedEbbx`, original `0xabf38`, size
**1432 (0x598)**. Function score is unchanged at **87.0 % / 1432 B** (the body
already matched; this pass is structural).

## 0. Result

The 192-byte rank table is moved out of its own `.jcr` PROGBITS section into
`.rodata` **as the last `.rodata` input**, by defining it in a new translation
unit `tools/mission_case_table.cpp` that the link places after every vendored
archive.  Appending it at the end of `.rodata` shifts no existing `.rodata`
byte (and therefore no existing literal-pool address), so the 27-section ELF is
restored and `byte_exact` stays at 4500.

```
before (wave 108 HEAD):  28 sections, .jcr 0xc0, .shstrtab 0x11c, file 0x21606c
after  (this pass):      27 sections, no .jcr,   .shstrtab 0x117, file 0x216040  (exact)
```

## 1. Gate (before and after)

```
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target check -j6
```

```
verify: byte 4500  linked 4500  stubs 0  missing 0  extra 0  avg 99.92
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

`report.json` row for the target: `pct 87.0`, `osize 1432`, `usize 1432`,
`byte false`, `linked false` (unchanged from the wave-108 baseline).

## 2. What changed

### `src/game/mission/Status.cpp`

The local `static const unsigned char kMissionCase[192]` in `.jcr` is replaced
by a hidden external declaration:

```cpp
#pragma GCC visibility push(hidden)
extern const unsigned char kMissionCase[192];
#pragma GCC visibility pop
```

The `switch (kMissionCase[m->getType()])` body is untouched, so the emitted
code is byte-for-byte the same shape as the wave-108 `.jcr` build.

### `tools/mission_case_table.cpp` (new)

Same 192 rank bytes, same order, defined hidden:

```cpp
#pragma GCC visibility push(hidden)
extern const unsigned char kMissionCase[192] = { ... };
#pragma GCC visibility pop
```

### `CMakeLists.txt`

A new OBJECT library `mission_table` is compiled with the same toolchain and
linked **after** every vendored archive (`libgcc.a`), immediately before
`-l dl -l c` / `crtend_so.o`, so it is the last `.rodata` input.  The link
`DEPENDS` list is extended so a change to the table re-links.

## 3. Section deltas vs the original

| section | original | ours | note |
|---|---:|---:|---|
| section count | 27 (+NULL) | 27 (+NULL) | `.jcr` removed |
| `.rodata` | `0x41b67` | `0x41c27` | table appended; **prefix byte-identical** |
| `.ARM.extab` | `0x26298` | `0x26294` | 4 B short (same as wave-108 baseline) |
| `.shstrtab` | `0x117` | `0x117` | **byte-identical** |
| `.dynsym` | `0x15ca0` | `0x15ca0` | size exact; see caveat below |
| `.dynstr` | `0x2e318` | `0x2e318` | size exact |
| `.hash` | `0x973c` | `0x973c` | size exact |
| `.rel.dyn` | `0x7048` | `0x7048` | size exact |
| `.rel.plt` | `0x72d0` | `0x72d0` | size exact |
| `.fini_array` | `0x8` | `0x8` | **byte-identical** |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` | **byte-identical** |
| `.dynamic` | `0x148` | `0x148` | **byte-identical** |
| `.got` | `0x430c` | `0x430c` | **byte-identical** |
| `.data` | `0x296c` | `0x296c` | **byte-identical** |
| `.init_array` | `0xc` | `0xc` | **byte-identical** |
| file size | `0x216040` | `0x216040` | exact |

`.rodata` prefix (all `0x41b67` original bytes) is byte-identical; the 192-byte
table is appended at offset `0x41b67` and `.rodata` grows by exactly `0xc0`.

## 4. `.dynsym` caveat (measured, not yet fixed)

The symbol **set**, values and `st_shndx` are identical to the original
(name-aligned diff: 0 value / 0 shndx / 0 other), but the **order** differs:
`memset@LIBC` and a handful of other `@LIBC` imports are inserted at a
different point, so the raw `.dynsym` byte diff is 746 (and `.dynstr` 4417,
`.hash` 372, `.rel.dyn` 18, `.rel.plt` 102).

This is caused by the **cross-TU undefined reference** to `kMissionCase` in
`Status.o`: gold inserts the (hidden) undefined symbol while processing
`Status.o`, which perturbs the dynamic-symbol insertion order.  It is not the
extra object and not the symbol name (all of `aaaMissionCase`,
`zzzMissionCase`, `kMissionCase` give the same 746).

Measured alternatives (all 27 sections, all 1432 B, all 87.0 %):

| variant | byte_exact | `.rodata` | `.dynsym` bytes | notes |
|---|---:|---|---:|---|
| **table in `.rodata`, separate TU linked last (landed)** | **4500** | +0xc0 appended | 746 | best gate count |
| table in `.data.rel.ro`, separate TU | 4489 | exact | 1235 | `.data.rel.ro` +0xc0 shifts |
| table in `.data.rel.ro.zzz` in `Status.o`, no extra TU | 4498 | exact | 194 | `.data.rel.ro` +0xc0 shifts |
| table in `.data.rel.ro.zzz` in `Status.o` + unused TU | 4498 | +0xc0 | 194 | same shift |
| table local static in `Status.o` `.rodata` | 4422 | +0x180 | 0 | `.rodata` content shifts |
| table local static in `Status.o` `.jcr` (wave 108) | 4500 | exact | 661 | 28 sections; `.shstrtab` 0x11c |

So no form tried achieves both `.dynsym` byte-identity and
`byte_exact >= 4500`: the only forms that preserve the `.dynsym` order keep the
table in `Status.o`, which shifts `.rodata`/`.data.rel.ro` and costs
`byte_exact`.  The landed form keeps the gate's primary hard count at 4500.

## 5. Proven vs hypothesis

Proven from the built ELF and `report.json`:

- `.jcr` is gone; the ELF has 27 sections and `.shstrtab` is byte-identical.
- `.rodata` prefix is byte-identical; the table is appended at `0x41b67`.
- `.fini_array`, `.data.rel.ro`, `.dynamic`, `.got`, `.data`, `.init_array`
  are byte-identical; file size is exact.
- `check` is RATCHET PASS with byte 4500 / linked 4500 / imports 207;
  `lint` CLEAN; `drift` 0.
- The target function is unchanged at 87.0 % / 1432 B.

Hypothesis (not proven):

- The `.dynsym` reorder is gold's insertion-order behaviour when a hidden
  global is referenced before its definition; a table-free form would remove
  it, but no table-free form found in waves 100–109 keeps both the original
  body order and 1432 B (the direct switch emits ascending case-value order,
  and the original layout is not ascending).
