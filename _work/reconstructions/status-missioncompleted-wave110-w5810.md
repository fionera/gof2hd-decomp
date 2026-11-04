# `Status::missionCompleted` — remove the `.jcr` rank table, keep `.dynsym` order (wave 110, w5810)

Status: **source change landed**. Worktree `wt/w5810`, branch `wave110-w5810`,
base master `390f58052`. Original binary:
`/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so`
(armeabi-v7a, NDK r18b clang 7.0.2, `-Oz`).

Target: `_ZN6Status16missionCompletedEbbx`, original `0xabf38`, size **1432 (0x598)**.
Function score is unchanged at **87.0 % / 1432 B** (the body already matched; this
pass is structural).

## 0. Result

The 192-byte mission-type -> case-rank table moves out of its own `.jcr`
PROGBITS section (wave 108: 28 sections) into the **tail of `.rodata`**, by
defining it in a new translation unit `tools/mission_case_table.cpp` that the
link places after every vendored archive.  It is therefore the last `.rodata`
input and lands at `0x20b977`, the first byte past the original `.rodata` image
(inside the original's zero gap before `.fini_array`).  No existing `.rodata`
byte moves, so no literal-pool address changes.

The wave-109 attempt did the same thing but referenced the table **by symbol**
from `Status.cpp`; that cross-TU undefined reference made gold insert a symbol
into the middle of its table and reorder `.dynsym` (40 versioned libc imports
moved).  This pass reads the table through its fixed address instead, so no
symbol is added and `.dynsym`/`.dynstr`/`.hash`/`.rel.plt` stay byte-identical.

```
before (wave 108 HEAD):  28 sections, .jcr 0xc0, .rodata 0x41b67, file 0x216040
after  (this pass):      27 sections, no .jcr,   .rodata 0x41c27, file 0x216040 (exact)
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
lint: CLEAN ✓ (hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

`report.json` row for the target: `pct 87.0`, `osize 1432`, `usize 1432`,
`byte false`, `linked false` (unchanged from the wave-108 baseline).

## 2. What changed

### `src/game/mission/Status.cpp`

The local `static const unsigned char kMissionCase[192]` in `.jcr` is replaced by
a fixed-address read of the table emitted by the new TU:

```cpp
#define MISSION_CASE ((const unsigned char *) 0x20b977)
...
switch (MISSION_CASE[(unsigned char)m->getType()]) {
```

The table content, the case values and the body order are unchanged, so the
emitted code has the same shape as the wave-108 `.jcr` build (the `.jcr` table
also lived at `0x20b977`, so even the literal-pool word is identical).  The
`(unsigned char)` cast is required: without it clang widens the switch range and
the function shrinks to **1428 B / 71.6 %**.

### `tools/mission_case_table.cpp` (new)

Same 192 rank bytes, hidden visibility so the symbol stays out of `.dynsym`:

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
| `.rodata` | `0x41b67` | `0x41c27` | table appended at `0x41b67`; **prefix byte-identical** |
| `.ARM.extab` | `0x26298` | `0x26294` | 4 B short (same as wave-108 baseline) |
| `.dynsym` | `0x15ca0` | `0x15ca0` | **byte-identical** |
| `.dynstr` | `0x2e318` | `0x2e318` | **byte-identical** |
| `.hash` | `0x973c` | `0x973c` | **byte-identical** |
| `.gnu.version` | `0x2b94` | `0x2b94` | **byte-identical** |
| `.rel.dyn` | `0x7048` | `0x7048` | **byte-identical** |
| `.rel.plt` | `0x72d0` | `0x72d0` | **byte-identical** |
| `.plt` | `0xac4c` | `0xac4c` | **byte-identical** |
| `.fini_array` | `0x8` | `0x8` | **byte-identical** |
| `.data.rel.ro` | `0x2fe8` | `0x2fe8` | **byte-identical** |
| `.init_array` | `0xc` | `0xc` | **byte-identical** |
| `.dynamic` | `0x148` | `0x148` | **byte-identical** |
| `.got` | `0x430c` | `0x430c` | **byte-identical** |
| `.data` | `0x296c` | `0x296c` | **byte-identical** |
| `.bss` | `0x69ac` | `0x69ac` | same size/base |
| `.shstrtab` | `0x117` | `0x117` | **byte-identical** |
| file size | `0x216040` | `0x216040` | exact |

The only differing section bytes are `.text`, `.ARM.exidx`, `.ARM.extab`
(4 B short) and `.note.gnu.build-id`, as in the wave-108 baseline; `.rodata`
differs only by the 192-byte table appended at its end.

## 4. Measured variants

All rows are full link + official verifier, same worktree/toolchain.

| variant | sections | `.rodata` | `.dynsym` name order | byte_exact | target |
|---|---:|---|---:|---:|---|
| HEAD wave-108 `.jcr` table | 28 | exact | 0 diffs | 4500 | 87.0 % / 1432 B |
| wave-109: new TU last, `extern` symbol reference | 27 | +0xc0 appended | 40 entries moved (80 diff lines) | 4500 | 87.0 % / 1432 B |
| `#pragma clang section rodata=".rodata.zzz"` static table in `Status.o` | 27 | +0xc0 **inserted at `0x1fc3c4`** | 0 diffs | 4424 | 87.0 % / 1432 B |
| macro reading the zero gap, no table emitted | 27 | exact | 0 diffs | 4500 | 87.0 % / 1432 B |
| macro without `(unsigned char)` cast | 27 | exact | 0 diffs | 4500 | 71.6 % / 1428 B |
| **new TU last, hidden global table, fixed address (landed)** | **27** | **+0xc0 appended at `0x20b977`** | **0 diffs** | **4500** | **87.0 % / 1432 B** |

Why the symbol reference cannot be used: gold's `Symbol_table::set_dynsym_indexes`
iterates an `std::unordered_map`.  A symbol inserted while `Status.o` is processed
(`Status.o` is TU #112) rehashes/reorders the table, moving 40 versioned libc
imports (`memset@LIBC`, `isxdigit@LIBC`, `vasprintf@LIBC`, `pthread_*`, …).  A
symbol added by an object linked *after* every other object (the new TU) is added
at the end and leaves the order untouched.  A hidden **defined** global added
early in `Status.cpp` also perturbs the order, so the issue is insertion
position, not defined-vs-undefined.  `#pragma weak` on the reference does not
help (same 80 diff lines).  Hence the fixed address.

## 5. Proven vs hypothesis

Proven from the built ELF and `report.json`:

- `.jcr` is gone; the ELF has 27 sections and the file size is exact.
- The table is at `0x20b977`; `.rodata` prefix (`0x41b67` bytes) is byte-identical
  and the table is the only appended data.
- `.dynsym`, `.dynstr`, `.hash`, `.gnu.version`, `.rel.dyn`, `.rel.plt`, `.plt`,
  `.fini_array`, `.data.rel.ro`, `.init_array`, `.dynamic`, `.got`, `.data`,
  `.comment`, `.shstrtab` are byte-identical to the original.
- `check` is RATCHET PASS with byte 4500 / linked 4500 / imports 207;
  `lint` CLEAN; `drift` 0.
- The target function is unchanged at 87.0 % / 1432 B.

Hypothesis (not proven):

- The original has no rank table at all: its dispatch is the direct
  `switch (m->getType())` (`sub #150; cmp #39; tbh` 40 entries + `sub #8; cmp #6;
  tbh` 7 entries).  The table is purely a clang-7 MBP workaround: a direct switch
  emits ascending case-value order, while the original's block order is
  `167/174, 150..155,158,162,166,170,184, 156,185,157,160,163,164,165,168,169,171,172`.
  No table-free shape found in waves 100–110 is both size-exact (1432 B) and
  body-order-exact (the best exact-order table-free shape is 1472 B / 76.4 %,
  see `status-missioncompleted-wave108-w5601.md`).

## 6. Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w5810
cmake --preset default \
  -DGOF2_ORIGINAL_SO=/home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so \
  -DGOF2_FMOD_LIBDIR=/home/debian/GalaxyOnFire2/_work/bins/apk_libs
cmake --build cmake-build-match --target link_so -j6
cmake --build cmake-build-match --target check -j6     # RATCHET PASS
cmake --build cmake-build-match --target lint  -j6     # CLEAN
cmake --build cmake-build-match --target drift -j6     # 0 fields

# table address and .dynsym name order
python3 - <<'PY'
d=open('cmake-build-match/libgof2hdaa.so','rb').read()
print(hex(d.find(bytes([0,190,190,190,190,190,190,190,8,190,190,11,190,13,14,190]))))
PY
readelf -sW cmake-build-match/libgof2hdaa.so | awk '{print $8}' > /tmp/ours.txt
readelf -sW /home/debian/GalaxyOnFire2/_work/bins/android_2.0.16_libgof2hdaa.so | awk '{print $8}' > /tmp/orig.txt
diff /tmp/orig.txt /tmp/ours.txt | wc -l    # 0
```

Scratch (not committed): `/dev/shm/w5810/` — `Status.B.cpp` (landed source),
`Status.macro_hardcoded.cpp` (same), `Status.macro.cur.cpp` (macro-only variant),
`score1.py`, `bodyorder.py`, `orig_dis.txt`.
