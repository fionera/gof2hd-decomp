# Section-order removal: `RecordHandler::readRecord` placement (w3745)

Status: research note + landed source move, 2026-11-02. Worktree `wt/w3745`,
branch `wave87-w3745`, based on master `69513966` (the tip after the
`StarMap` object-order pass). Every number below was measured in this worktree
against `../_work/bins/android_2.0.16_libgof2hdaa.so` with the tree's NDK r18b
clang 7.0.2 / gold 1.12.

This note follows `_work/reconstructions/starmap_object_order_w3745.md` §7,
which left the plain no-order frontier at `RecordHandler::readRecord`
(`0xcd25c` original, `0xce858` ours).

## TL;DR

* Baseline at this HEAD: **byte_exact 4497 / linked_exact 4497**, `.text`
  `0x1337c8 @ 0x69440`, lint `CLEAN ✓`, drift `0`. (The brief's 4496 is stale;
  the current baseline is 4497/4497, as recorded in the parent notes.)
* Plain no-order baseline: **byte_exact 2995 / linked 4497**, **3196 / 4524**
  exported functions at their original address, exact-address prefix **2408**.
  First mismatch: `RecordHandler::readRecord` original `0xcd25c`, ours
  `0xce858`.
* Cause: clang emits `readRecord` late in `RecordHandler.o` (after
  `readWanted`) even though its definition is after `recordStoreRead` in the
  `.cpp`. The original binary and the iOS 1.1.19 symbol dump both place
  `readRecord` immediately after `recordStoreRead` and before
  `loadResolutionValue`.
* Fix: move the `RecordHandler::readRecord` definition from after
  `recordStoreRead()` to immediately after `readAllRecords()` (i.e. immediately
  before `loadResolutionValue()`). This is a pure move: all 179
  `.text.*`/`.ARM.*`/`.rodata*`/`.data*`/`.bss*` content sections in the object
  are byte-identical; only the `.text.*` header order changes.
* Gate after the move: **byte 4497 / linked 4497**, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift `0`. Plain no-order after: **byte_exact 3000 /
  linked 4497**, **3202 / 4524** addresses, prefix **2414**.
* The next plain no-order mismatch is `readRecordAsByteArray`
  (`0xcdbfc` original, `0xcdd00` ours), not a `readRecord` symbol.

## 1. Baseline and commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8
```

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
=== DRIFT: 0 field(s) across 0 class(es) ===
```

Section fingerprint of the gate `.so`:

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x02628c @ 0x1a3b78   (original 0x26298; known -0xc delta)
.rodata          0x041b67 @ 0x1c9e10
.data.rel.ro     0x002fe8 @ 0x20cbb0
.data            0x00296c @ 0x214000
.bss             0x0069ac @ 0x216970
```

Plain no-order measurement (the exact gold line minus
`--section-ordering-file`):

```sh
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/rh/noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/rh/verify \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/rh/noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/rh/noorder.so
```

```text
compared 4524  avg 99.9%  byte_exact 2995  linked_exact 4497  fuzzy_perfect 4497
/tmp/w3745/rh/noorder.so: 3196/4524 same; prefix 2408;
  first (840284, '_ZN13RecordHandler10readRecordEi', 845912)
```

`840284 = 0xcd25c`, `845912 = 0xce858`.

## 2. Original order

`nm -nDS --defined-only` on the original, restricted to the RecordHandler
block:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nDS --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | awk '$1>="000cba00" && $1<="000cd400"'
```

```text
000cba2c 000000dc T _ZN13RecordHandlerC1Ev
000cba2c 000000dc T _ZN13RecordHandlerC2Ev
000cbb08 00000028 T _ZN13RecordHandlerD1Ev
000cbb08 00000028 T _ZN13RecordHandlerD2Ev
000cbb30 00000016 T _ZN13RecordHandler15notEnoughMemoryEv
000cbb48 00000058 T _ZN13RecordHandler21readAllPreviewRecordsEv
000cbba0 0000001c W _ZN5ArrayIP10GameRecordEC2Ev
000cbbbc 00000034 W _Z14ArraySetLengthIP10GameRecordEvjR5ArrayIT_E
000cbbf0 000000ec T _ZN13RecordHandler22recordStoreReadPreviewEi
000cbcdc 00000058 T _ZN13RecordHandler14readAllRecordsEv
000cbd34 00001528 T _ZN13RecordHandler15recordStoreReadEi
000cd25c 00000004 T _ZN13RecordHandler10readRecordEi
000cd260 000001f0 T _ZN13RecordHandler19loadResolutionValueEf
000cd450 00000360 T _ZN13RecordHandler11loadOptionsEv
000cd7b0 00000118 T _ZN13RecordHandler9checkHashEj
000cd8c8 00000234 T _ZN13RecordHandler11saveOptionsEv
000cdafc 00000100 T _ZN13RecordHandler16addHashToOptionsEv
000cdbfc 000000b4 T _ZN13RecordHandler21readRecordAsByteArrayEPPaib
000cdcb0 0000009c T _ZN13RecordHandler22writeByteArrayAsRecordEPaiib
000cdd4c 0000008c T _ZN13RecordHandler26readOptionsFileAsByteArrayEPPa
000cddd8 00000078 T _ZN13RecordHandler27writeByteArrayAsOptionsFileEPai
000cde50 0000001c W _ZN5ArrayIhEC2Ev
000cde6c 00000032 W _Z8ArrayAddIhEvPKT_jR5ArrayIS0_E
000cdea0 00000114 T _ZN13RecordHandler7addHashEi
000cdfb4 0000029c T _ZN13RecordHandler11readMissionEj
```

The iOS 1.1.19 layout dump (`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios`, `Classes/Game/RecordHandler.cpp`) has the same relative
order:

```text
_RecordHandler::recordStoreRead(int)
_RecordHandler::readRecord(int)
_RecordHandler::loadResolutionValue(float)
_RecordHandler::loadOptions()
_RecordHandler::saveOptions()
_RecordHandler::readRecordAsByteArray(signed char**, int, bool)
_RecordHandler::writeByteArrayAsRecord(signed char*, int, int, bool)
_RecordHandler::checkHash(unsigned int)
```

## 3. Object order before the fix

The current `RecordHandler.cpp.obj` `.text.*` order (parsed from the section
header table) has `readRecord` at object index 43, after `readWanted`:

```text
...
_ZN13RecordHandler14readAllRecordsEv
_ZN13RecordHandler15recordStoreReadEi
_ZN13RecordHandler19loadResolutionValueEf
_ZN13RecordHandler11loadOptionsEv
_ZN13RecordHandler9checkHashEj
_ZN13RecordHandler11saveOptionsEv
_ZN13RecordHandler16addHashToOptionsEv
_ZN13RecordHandler26readOptionsFileAsByteArrayEPPa
_ZN13RecordHandler27writeByteArrayAsOptionsFileEPai
_ZN13RecordHandler21readRecordAsByteArrayEPPaib
_ZN13RecordHandler22writeByteArrayAsRecordEPaiib
_ZN13RecordHandler7addHashEi
_ZN13RecordHandler11readMissionEj
...
_ZN13RecordHandler10readWantedEj
_ZN13RecordHandler10readRecordEi
_ZN5ArrayIhEC2Ev
_Z8ArrayAddIhEvPKT_jR5ArrayIS0_E
_ZN13RecordHandler16recordStoreWriteEi
...
```

The target rank in `cmake-build-match/section_order.txt` is:

```text
.text._ZN13RecordHandler15recordStoreReadEi
.text._ZN13RecordHandler10readRecordEi
.text._ZN13RecordHandler19loadResolutionValueEf
```

So the plain link puts `loadResolutionValue` at `0xcd25c` (where `readRecord`
belongs), and the real `readRecord` is emitted much later at `0xce858`.

## 4. The fix

Pure move in `src/game/mission/RecordHandler.cpp`: cut the 4-byte `readRecord`
definition from after `recordStoreRead()` and paste it immediately after
`readAllRecords()`, before `loadResolutionValue()`.

```diff
@@ after RecordHandler::readAllRecords()
+GameRecord *RecordHandler::readRecord(int slot) {
+    return this->recordStoreRead(slot);
+}
+
 float RecordHandler::loadResolutionValue(float resolution) {
@@ after RecordHandler::recordStoreRead()
-GameRecord *RecordHandler::readRecord(int slot) {
-    return this->recordStoreRead(slot);
-}
-
 bool RecordHandler::checkHash(unsigned int fd) {
```

After the move the object order is:

```text
...
_ZN13RecordHandler14readAllRecordsEv
_ZN13RecordHandler15recordStoreReadEi
_ZN13RecordHandler10readRecordEi
_ZN13RecordHandler19loadResolutionValueEf
_ZN13RecordHandler11loadOptionsEv
_ZN13RecordHandler9checkHashEj
_ZN13RecordHandler11saveOptionsEv
_ZN13RecordHandler16addHashToOptionsEv
_ZN13RecordHandler26readOptionsFileAsByteArrayEPPa
_ZN13RecordHandler27writeByteArrayAsOptionsFileEPai
_ZN13RecordHandler21readRecordAsByteArrayEPPaib
_ZN13RecordHandler22writeByteArrayAsRecordEPaiib
_ZN13RecordHandler7addHashEi
...
```

`readRecord` now follows `recordStoreRead` and precedes
`loadResolutionValue`, exactly the original order. The next object-order
mismatch is the `readRecordAsByteArray` / `readOptionsFileAsByteArray` block
(see §7).

### Controlled placement sweep

To show this is not an arbitrary move, the same source was compiled with
`readRecord` inserted before/after each of the other 29 RecordHandler
definitions (58 variants) using the exact `compile_commands.json` command.
The `.text.*` section-header order was parsed from each object. Only the
insertion point immediately after `readAllRecords()` (equivalently,
immediately before `loadResolutionValue()`) puts `readRecord` at object index
10, between `recordStoreRead` and `loadResolutionValue`. All other insertion
points leave it at a different index. The script is reproducible from the
command in §8; it is a compile-only experiment, not a proposed landing.

### Purity check

Compile the pre-move source and the post-move source with the same command.
Comparing every content section whose name starts with `.text.`, `.ARM.`,
`.rodata`, `.data` or `.bss`:

```text
compared 179 sections; differing 0
order same? False
first order diff 10 .text._ZN13RecordHandler19loadResolutionValueEf .text._ZN13RecordHandler10readRecordEi
```

So the move changes only `.text.*` section-header order (and the section-index
operands in relocation sections); no function body, literal, table or
exception-table bytes change.

## 5. Measured A/B

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| variant | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline (`readRecord` after `recordStoreRead`) | **4497 / 4497** | **2995** | 3196 / 4524 | 2408 | `RecordHandler::readRecord` `0xcd25c` vs `0xce858` |
| `readRecord` before `loadResolutionValue` | **4497 / 4497** | **3000** | 3202 / 4524 | 2414 | `RecordHandler::readRecordAsByteArray` `0xcdbfc` vs `0xcdd00` |

Delta: **+5 byte_exact**, **+6 exact addresses**, **+6 exact-address prefix**,
gate unchanged. The gate `.text` size, `.rodata`/`.data` exactness (843/843),
sodiff allowlist (0), imports (207), lint, and drift are all unchanged. No
baseline update was made.

The plain-link RecordHandler block after the move:

```text
000cba2c 000000dc T _ZN13RecordHandlerC1Ev
...
000cbd34 00001528 T _ZN13RecordHandler15recordStoreReadEi
000cd25c 00000004 T _ZN13RecordHandler10readRecordEi
000cd260 000001f0 T _ZN13RecordHandler19loadResolutionValueEf
000cd450 00000360 T _ZN13RecordHandler11loadOptionsEv
000cd7b0 00000118 T _ZN13RecordHandler9checkHashEj
000cd8c8 00000234 T _ZN13RecordHandler11saveOptionsEv
000cdafc 00000100 T _ZN13RecordHandler16addHashToOptionsEv
000cdbfc 0000008c T _ZN13RecordHandler26readOptionsFileAsByteArrayEPPa
...
```

`readRecord`, `loadResolutionValue`, `loadOptions`, `checkHash`,
`saveOptions` and `addHashToOptions` are now address-exact. The next mismatch
is the swapped `readRecordAsByteArray` / `readOptionsFileAsByteArray` block.

## 6. Reordered-all control

Re-measured the parent note's control after the move: permute every object's
own `.text.*` sections into `section_order.txt` order, then relink without the
ordering file.

```sh
mkdir -p /tmp/w3745/rh/reordered
python3 - <<'PY'
import os, subprocess
src = 'cmake-build-match/exidx_markers/link_order.rsp'
lines = [l.strip() for l in open(src) if l.strip()]
out = []
for i, p in enumerate(lines):
    dst = f'/tmp/w3745/rh/reordered/{i:03d}_{os.path.basename(p)}'
    subprocess.run(['python3',
                    '_work/reconstructions/w3696/reorder_elf_sections.py',
                    p, dst, 'cmake-build-match/section_order.txt'], check=True)
    out.append(dst)
open('/tmp/w3745/rh/reordered/link_order.rsp', 'w').write('\n'.join(out) + '\n')
PY
python3 _work/reconstructions/w3696/relink_noorder.py \
  /tmp/w3745/rh/reordered/noorder.so /tmp/w3745/rh/reordered/link_order.rsp
# verify.py + addr_match.py as in §1
```

```text
compared 4524  avg 99.9%  byte_exact 4497  linked_exact 4497  fuzzy_perfect 4497
4524/4524 same; prefix 4524; first None
```

So the per-object section permutation still reproduces the ordering-file gate
exactly after the move.

## 7. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0.
* The plain no-order baseline is `byte 2995 / linked 4497`, 3196/4524 exact
  addresses, prefix 2408; the first mismatch is `RecordHandler::readRecord`
  (`0xcd25c` vs `0xce858`).
* The original binary and the iOS 1.1.19 symbol dump both place `readRecord`
  immediately after `recordStoreRead` and before `loadResolutionValue`.
* The pre-move object emits `readRecord` after `readWanted`, at object index
  43; the target rank is between `recordStoreRead` and
  `loadResolutionValue`.
* Moving the `readRecord` definition to immediately after `readAllRecords()`
  (before `loadResolutionValue()`) is the unique tested insertion point that
  puts it at object index 10 in the target position.
* The move is gate-neutral (`4497/4497`, lint `CLEAN`, drift 0) and improves
  the plain no-order build to `3000 / 4497`, 3202/4524 addresses, prefix 2414.
* The move is a pure section-order change: 179 content sections are
  byte-identical; only `.text.*` header order differs.
* The reordered-all control is still `4497/4497`, 4524/4524.

**Hypothesis / open**

* The exact clang rule that decides where a deferred/forward-referenced
  function is emitted is still not fully characterized. The placement sweep
  shows the definition position controls it, but a simple "emit at first use"
  or "emit at definition" rule does not explain every block in
  `RecordHandler.o`.
* The next plain no-order frontier is the `RecordHandler` block
  `readRecordAsByteArray` / `writeByteArrayAsRecord` vs
  `readOptionsFileAsByteArray` / `writeByteArrayAsOptionsFile`. The original
  order is `readRecordAsByteArray`, `writeByteArrayAsRecord`,
  `readOptionsFileAsByteArray`, `writeByteArrayAsOptionsFile`; our object
  emits the two options-file functions first. This is a separate placement
  problem and was not changed here.
* No claim is made that the ordering file can be dropped now; the plain link
  is still `3000`, and the parent note's weak-copy TU-membership blockers
  remain.

## 8. Reproduction of the placement sweep

The sweep compiles a temporary copy of `RecordHandler.cpp` with the
`readRecord` block moved before/after each other RecordHandler definition,
using the exact `compile_commands.json` command, then parses the `.text.*`
section-header order. The key result is:

```text
baseline                                      readRecord idx 43
readAllRecords_after                          readRecord idx 10
loadResolutionValue_before                    readRecord idx 10
all other insertion points                    readRecord idx 2..59
```

The landed source is the `loadResolutionValue_before` variant (equivalently
`readAllRecords_after`).
