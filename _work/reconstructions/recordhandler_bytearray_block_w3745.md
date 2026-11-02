# Section-order removal: `RecordHandler` byte-array block (w3745)

Status: research note + landed source move, 2026-11-02. Worktree `wt/w3745`,
branch `wave87-w3745`, based on master `669c5cf3` (the tip after the
`RecordHandler::readRecord` placement pass). Every number below was measured in
this worktree against `../_work/bins/android_2.0.16_libgof2hdaa.so` with the
tree's NDK r18b clang 7.0.2 / gold 1.12.

This note closes the next plain no-order frontier left by
`_work/reconstructions/recordhandler_readrecord_placement_w3745.md` §7: the
`readRecordAsByteArray` / `writeByteArrayAsRecord` vs
`readOptionsFileAsByteArray` / `writeByteArrayAsOptionsFile` block, plus the
`Array<unsigned char>` weak copies that sit between them and `addHash`.

## TL;DR

* Baseline at this HEAD: **byte_exact 4497 / linked_exact 4497**, `.text`
  `0x1337c8 @ 0x69440`, lint `CLEAN ✓`, drift `0`. (The brief's 4496 is stale;
  the current baseline is 4497/4497.)
* Plain no-order baseline: **byte_exact 3000 / linked 4497**,
  **3202 / 4524** exported functions at their original address, exact-address
  prefix **2414**. First mismatch: `RecordHandler::readRecordAsByteArray`
  original `0xcdbfc`, ours `0xcdd00`.
* Cause: two independent clang emission-order effects.
  1. `saveOptions()` calls `addHashToOptions()`, so `addHashToOptions` is
     emitted immediately after `saveOptions`; its body is scanned at its
     definition. With the definition before the record byte-array functions,
     its calls to the options byte-array functions are emitted before the
     record byte-array functions, swapping the two blocks.
  2. `checkHash()` is the first user of `Array<unsigned char>`. Its definition
     position controls where the weak `Array<unsigned char>` ctor /
     `ArrayAdd<unsigned char>` copies are emitted. In the baseline they land
     after `readWanted`, not between the options block and `addHash`.
* Fix: reorder definitions only (no body edits):
  `readRecordAsByteArray`, `writeByteArrayAsRecord`, `addHashToOptions`,
  `checkHash`, `readOptionsFileAsByteArray`, `writeByteArrayAsOptionsFile`,
  `addHash`, then `recordStoreRead`.
* Gate after: **byte 4497 / linked 4497**, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0. Plain no-order after: **byte_exact 3013 /
  linked 4497**, **3215 / 4524** addresses, prefix **2639**. The whole
  `RecordHandler` exported block through `readWanted` is address-exact; the
  next plain no-order mismatch is `Item::extractItems`
  (`0xe049c` original, `0xe04d4` ours), unchanged by this pass.
* The move is a pure section-order change: 180 content sections
  (`.text.*`, `.ARM.*`, `.rodata*`, `.data*`, `.bss*`) are byte-identical;
  only `.text.*` header order changes.
* Reordered-all no-order control still reproduces the gate exactly:
  **4497 / 4497**, **4524 / 4524** addresses, prefix 4524.

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
compared 4524  avg 99.9%  byte_exact 3000  linked_exact 4497  fuzzy_perfect 4497
/tmp/w3745/rh/noorder.so: 3202/4524 same; prefix 2414;
  first (842748, '_ZN13RecordHandler21readRecordAsByteArrayEPPaib', 843008)
```

`842748 = 0xcdbfc`, `843008 = 0xcdd00`.

## 2. Original order

`nm -nDS --defined-only` on the original, restricted to the block:

```sh
NM=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-nm
$NM -nDS --defined-only ../_work/bins/android_2.0.16_libgof2hdaa.so \
  | awk '$1>="000cd8c8" && $1<="000ce6c8"'
```

```text
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
000ce250 000003f4 T _ZN13RecordHandler9readAgentEj
000ce644 00000034 W _Z14ArraySetLengthIP14PendingProductEvjR5ArrayIT_E
000ce678 0000001c W _ZN5ArrayIP6WantedEC2Ev
000ce694 00000034 W _Z14ArraySetLengthIP6WantedEvjR5ArrayIT_E
000ce6c8 000001e4 T _ZN13RecordHandler10readWantedEj
```

The iOS 1.1.19 layout dump (`_work/original_layout/original_layout.json`,
`gof2hd_1.1.19_ios`, `Classes/Game/RecordHandler.cpp`) corroborates the
relative order of the visible functions: `readRecordAsByteArray`,
`writeByteArrayAsRecord`, `checkHash`, `addHash` (it omits the
`addHashToOptions` / options-file helpers from that symbol list).

## 3. Object order before the fix

The current `RecordHandler.cpp.obj` `.text.*` order before the fix:

```text
...
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
_ZN5ArrayIhEC2Ev
_Z8ArrayAddIhEvPKT_jR5ArrayIS0_E
_ZN13RecordHandler16recordStoreWriteEi
```

Reproduce it with:

```sh
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/RecordHandler.cpp.obj \
  | awk '$2 ~ /^\.text\./ {print $2}' | sed 's/^\.text\.//'
```

Two separate causes are visible:

1. `addHashToOptions` is emitted after `saveOptions` because `saveOptions`
   calls it. Its body is scanned at its definition, which is before the record
   byte-array functions; its calls to the options byte-array functions are
   therefore emitted first. The record byte-array definitions come later and
   are emitted only when reached.
2. `Array<unsigned char>` is first used in `checkHash`. In the baseline
   `checkHash` is defined after `recordStoreRead`, and the weak
   `Array<unsigned char>` copies land after `readWanted`, not before
   `addHash`.

### Controlled compile-only placement sweep

Each row compiles a temporary copy of `RecordHandler.cpp` with the exact
`compile_commands.json` command and parses the `.text.*` header order. It is a
compile-only experiment; no link or gate numbers are involved.

| variant | object order of the six byte-array/addHash functions | `Array<unsigned char>` position |
|---|---|---|
| baseline | `addHashToOptions`, options-read, options-write, record-read, record-write, `addHash` | after `readWanted` |
| move `addHashToOptions` after `writeByteArrayAsRecord` | `addHashToOptions`, record-read, record-write, options-read, options-write, `addHash` | after `readWanted` |
| move `checkHash` before `readOptionsFileAsByteArray` | `addHashToOptions`, options-read, options-write, record-read, record-write, `addHash` | between record-write and `addHash` |
| both moves | `addHashToOptions`, record-read, record-write, options-read, options-write, `addHash` | between options-write and `addHash` |

Only the combined move matches the original order in §2.

## 4. The fix

Pure source reorder in `src/game/mission/RecordHandler.cpp`; no body text
changes. The definitions now appear in this order:

```text
saveOptions
readRecordAsByteArray
writeByteArrayAsRecord
addHashToOptions
checkHash
readOptionsFileAsByteArray
writeByteArrayAsOptionsFile
addHash
recordStoreRead
...
```

`checkHash` is moved from after `recordStoreRead` to immediately before
`readOptionsFileAsByteArray`. The other six definitions are moved as a group
before `recordStoreRead`, with `readRecordAsByteArray` /
`writeByteArrayAsRecord` before `addHashToOptions`.

After the move the object order is:

```text
...
_ZN13RecordHandler11saveOptionsEv
_ZN13RecordHandler16addHashToOptionsEv
_ZN13RecordHandler21readRecordAsByteArrayEPPaib
_ZN13RecordHandler22writeByteArrayAsRecordEPaiib
_ZN13RecordHandler26readOptionsFileAsByteArrayEPPa
_ZN13RecordHandler27writeByteArrayAsOptionsFileEPai
_ZN5ArrayIhEC2Ev
_Z8ArrayAddIhEvPKT_jR5ArrayIS0_E
_ZN13RecordHandler7addHashEi
_ZN13RecordHandler11readMissionEj
...
```

This is exactly the original order in §2.

## 5. Measured A/B

Every row is a controlled rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering file on);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| variant | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline | **4497 / 4497** | **3000** | 3202 / 4524 | 2414 | `RecordHandler::readRecordAsByteArray` `0xcdbfc` vs `0xcdd00` |
| byte-array block reorder | **4497 / 4497** | **3013** | **3215 / 4524** | **2639** | `Item::extractItems` `0xe049c` vs `0xe04d4` |

Delta: **+13 byte_exact**, **+13 exact addresses**, **+225 exact-address
prefix**, gate unchanged. `linked_exact` stays 4497. The gate `.text` size,
all section sizes/bases, `.rodata`/`.data` exactness (843/843), sodiff
allowlist (0), imports (207), lint and drift are unchanged. No baseline update
was made.

The whole `RecordHandler` exported block through `readWanted` is now
address-exact in the plain link:

```text
000cdafc 00000100 T _ZN13RecordHandler16addHashToOptionsEv
000cdbfc 000000b4 T _ZN13RecordHandler21readRecordAsByteArrayEPPaib
000cdcb0 0000009c T _ZN13RecordHandler22writeByteArrayAsRecordEPaiib
000cdd4c 0000008c T _ZN13RecordHandler26readOptionsFileAsByteArrayEPPa
000cddd8 00000078 T _ZN13RecordHandler27writeByteArrayAsOptionsFileEPai
000cde50 0000001c W _ZN5ArrayIhEC2Ev
000cde6c 00000032 W _Z8ArrayAddIhEvPKT_jR5ArrayIS0_E
000cdea0 00000114 T _ZN13RecordHandler7addHashEi
000cdfb4 0000029c T _ZN13RecordHandler11readMissionEj
000ce250 000003f4 T _ZN13RecordHandler9readAgentEj
000ce644 00000034 W _Z14ArraySetLengthIP14PendingProductEvjR5ArrayIT_E
000ce678 0000001c W _ZN5ArrayIP6WantedEC2Ev
000ce694 00000034 W _Z14ArraySetLengthIP6WantedEvjR5ArrayIT_E
000ce6c8 000001e4 T _ZN13RecordHandler10readWantedEj
```

`nm -nDS --defined-only` on the plain no-order `.so` gives the same addresses
for every one of these symbols; no `RecordHandler` symbol differs. The next
mismatch, `Item::extractItems`, is outside this TU and has the same address in
the baseline and after-build (`0xe04d4`), so it is not caused by this pass.

## 6. Purity check

Compile the pre-move (`git show HEAD:...`) and post-move source with the exact
`compile_commands.json` command, then compare every content section:

```sh
git show HEAD:src/game/mission/RecordHandler.cpp > /tmp/w3745/rh/pre.cpp
cp src/game/mission/RecordHandler.cpp /tmp/w3745/rh/post.cpp
# compile both with the RecordHandler.cpp command from compile_commands.json
```

Comparison result:

```text
content sections pre 180 post 180
differing/missing 0 extra 0
text sections 60 60 order same? False
first order diff 16 .text._ZN13RecordHandler26readOptionsFileAsByteArrayEPPa .text._ZN13RecordHandler21readRecordAsByteArrayEPPaib
```

So the move changes only `.text.*` section-header order (and the section-index
operands in relocation sections); no function body, literal, table or
exception-table bytes change.

## 7. Reordered-all control

Permute every object's own `.text.*` sections into `section_order.txt` order,
then relink without the ordering file:

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

## 8. Proven vs hypothesis

**Proven by measurement in this worktree**

* Baseline is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0.
* The plain no-order baseline is `byte 3000 / linked 4497`, 3202/4524 exact
  addresses, prefix 2414; the first mismatch is
  `RecordHandler::readRecordAsByteArray` (`0xcdbfc` vs `0xcdd00`).
* The original binary places `addHashToOptions`, the record byte-array pair,
  the options byte-array pair, the `Array<unsigned char>` weak copies,
  `addHash`, `readMission`, `readAgent`, `readWanted` in that order.
* The baseline object emits the options pair before the record pair, and the
  `Array<unsigned char>` copies after `readWanted`.
* Moving `addHashToOptions` after `writeByteArrayAsRecord` swaps the record
  pair before the options pair; moving `checkHash` before
  `readOptionsFileAsByteArray` moves the `Array<unsigned char>` copies before
  `addHash`. Both moves together reproduce the original order.
* The move is gate-neutral (`4497/4497`, lint `CLEAN`, drift 0) and improves
  the plain no-order build to `3013 / 4497`, 3215/4524 addresses, prefix 2639.
* The move is a pure section-order change: 180 content sections are
  byte-identical; only `.text.*` header order differs.
* The reordered-all control is still `4497/4497`, 4524/4524.

**Hypothesis / open**

* The exact clang 7 deferred-instantiation flush rule is still not fully
  characterized. The placement sweep shows the definition position controls
  where the weak `Array<unsigned char>` copies land, but a simple "emit at
  first textual use" rule does not explain every block in `RecordHandler.o`.
* The next plain no-order frontier is `Item::extractItems`
  (`0xe049c` original, `0xe04d4` ours). It is outside `RecordHandler.cpp` and
  was already mismatched before this pass; it is the next source-order target.
* No claim is made that the ordering file can be dropped now; the plain link
  is still `3013`, and the weak-copy TU-membership blockers from
  `section_order_removal_w3696.md` remain.

## 9. Reproduction command summary

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

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

## Appendix A — compile-only order sweep helper

The sweep in §3 uses the exact `RecordHandler.cpp` command from
`cmake-build-match/compile_commands.json`, replacing only the source and output
paths. The helper below reproduces the four rows (run from the repo root):

```python
#!/usr/bin/env python3
# usage: sweep.py "name1,name2,..." ANCHOR_SIGNATURE TAG
import json, re, subprocess, pathlib
ROOT = pathlib.Path('/home/debian/GalaxyOnFire2/wt/w3745')
SRC = ROOT / 'src/game/mission/RecordHandler.cpp'
SIGS = {
 'addHashToOptions': 'void RecordHandler::addHashToOptions()',
 'readRecordAsByteArray': 'int RecordHandler::readRecordAsByteArray(',
 'writeByteArrayAsRecord': 'int RecordHandler::writeByteArrayAsRecord(',
 'readOptionsFileAsByteArray': 'int RecordHandler::readOptionsFileAsByteArray(',
 'writeByteArrayAsOptionsFile': 'int RecordHandler::writeByteArrayAsOptionsFile(',
 'addHash': 'void RecordHandler::addHash(',
 'checkHash': 'bool RecordHandler::checkHash(',
 'recordStoreRead': 'GameRecord *RecordHandler::recordStoreRead(',
}
def block(text, sig):
    m = re.search(r'^' + re.escape(sig) + r'[^\n]*\n(?:.*?\n)*?^\}\n', text, re.M)
    return m.group(0)
def move(text, names, anchor):
    bs = [block(text, SIGS[n]) for n in names]
    for b in bs:
        text = text.replace(b, '', 1)
    idx = text.index(SIGS[anchor])
    return text[:idx] + ''.join(bs) + text[idx:]
def compile_order(text, tag):
    d = pathlib.Path('/tmp/w3745/rh/sweep'); d.mkdir(parents=True, exist_ok=True)
    src, obj = d / (tag + '.cpp'), d / (tag + '.o')
    src.write_text(text)
    cmd = next(e['command'] for e in json.load(open(ROOT / 'cmake-build-match/compile_commands.json'))
               if e['file'].endswith('RecordHandler.cpp'))
    cmd = cmd.replace(str(SRC), str(src)).replace(
        'CMakeFiles/gof2.dir/src/game/mission/RecordHandler.cpp.obj', str(obj))
    subprocess.run(cmd, shell=True, check=True, cwd=ROOT / 'cmake-build-match')
    out = subprocess.check_output(['readelf', '-SW', str(obj)], text=True)
    return [m.group(1)[6:] for m in re.finditer(r'\]\s+(\.text\.\S+)\s+', out)]
if __name__ == '__main__':
    import sys
    names = sys.argv[1].split(',') if sys.argv[1] else []
    text = move(SRC.read_text(), names, sys.argv[2]) if names else SRC.read_text()
    for i, s in enumerate(compile_order(text, sys.argv[3]), 1):
        if any(x in s for x in ('RecordHandler', 'ArrayIh', 'ArrayAddIh')):
            print(i, s)
```

Rows:

```sh
python3 /tmp/sweep.py '' recordStoreRead baseline
python3 /tmp/sweep.py 'readRecordAsByteArray,writeByteArrayAsRecord,addHashToOptions,readOptionsFileAsByteArray,writeByteArrayAsOptionsFile,addHash' recordStoreRead v1
python3 /tmp/sweep.py 'checkHash' readOptionsFileAsByteArray v2
python3 /tmp/sweep.py 'readRecordAsByteArray,writeByteArrayAsRecord,addHashToOptions,checkHash,readOptionsFileAsByteArray,writeByteArrayAsOptionsFile,addHash' recordStoreRead v3
```

`v3` is the landed source order. The helper prints the same object-order
subsequence shown in §3/§4.
