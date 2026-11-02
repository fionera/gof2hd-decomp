# `.ARM.exidx` / `.ARM.extab` raw records (w3625)

Status: research note + one tool fix (2026-11-02). Worktree `wt/w3625`, branch
`wave87-w3625`, based on master `b0a1ebf2`. Original binary:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (armeabi-v7a, NDK r18b clang 7.0.2,
`-Oz`, gold 1.12, image base 0x10000). Every number below is from a command run
against the original binary or from a controlled A/B relink of this tree.
Scratch files are under `/tmp`; the worktree is clean apart from the committed
change and this note.

## TL;DR

1. **Every `.ARM.exidx` function address is already exact.** After the marker fix
   below, all 3566 `word0` prel31 words are byte-identical; all 272 `CANTUNWIND`
   and all 856 inline-model `word1` words are byte-identical. The only differing
   exidx words are the 2329 `EXT` prel31 pointers into `.ARM.extab`, and those
   differ only because the extab records are not at the same addresses.
2. **The two kind-sequence differences were a stale marker witness.** The tree
   had an extra CANTUNWIND before `pConstToNonConst` and was missing the
   CANTUNWIND before `GetTouch`. The cause is that `NdkAutosave.cpp`'s empty
   `.text` marker is now owned by an object whose section list no longer contains
   `.text.ndk_autosave`; `MARKER_AFTER` keyed on that name matched the wrong
   object. Adding `.text.pConstToNonConst -> .text._Z8GetTouchi` makes the
   complete 3566-entry kind sequence exact. Gate unchanged (byte 4496 /
   linked 4496, `.text` 0x1337c8).
3. **`.ARM.extab` records are emitted in input-section order, not `.text` order.**
   The original's extab order is its input order; ours diverges because the
   `--section-ordering-file` reorders `.text` but not `.ARM.extab`. A controlled
   scratch relink that lists `.ARM.extab...` sections in the original's extab
   order (with unique names for the three `.text.startup` extab sections and the
   plain `.ARM.extab`) reproduces the original record order **2438/2438** and
   drops the differing-record count from 578 to 448. Gate unchanged
   (byte 4496 / linked 4496).
4. **All remaining extab record differences are accounted for.** After the order
   fix, 441 equal-length records differ only in their first 4-byte personality
   prel31 word (their records moved because seven earlier records change length);
   seven records have the known body-length deltas (net -8 B); one record
   (`PaintCanvas::DrawString`) has a genuine LSDA call-site difference (cleanup
   range 8 B in the original vs 14 B in ours). There is no remaining table
   structure, kind, or ordering difference.

## 1. Baseline

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
```

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
```

`lint` = `CLEAN ✓`, `drift` = `0 field(s) across 0 class(es)`, `diff` = symbol
parity (5370/5370, extra 0, missing 0).

Section sizes/bases (both files):

| section | original | baseline |
|---|---:|---:|
| `.text` | `0x1337c8` @ `0x69440` | `0x1337c8` @ `0x69440` |
| `.ARM.exidx` | `0x006f70` @ `0x19cc08` | `0x006f70` @ `0x19cc08` |
| `.ARM.extab` | `0x026298` @ `0x1a3b78` | `0x026290` @ `0x1a3b78` |
| `.rodata` | `0x041b67` @ `0x1c9e10` | `0x041b67` @ `0x1c9e10` |

## 2. Tooling

`_work/reconstructions/exidx_table.py` remains the per-entry kind/record-length
tool. This pass adds `_work/reconstructions/exidx_raw_audit.py`, which compares
the raw words and aligns extab records by the **function that owns them** (not by
address), so record-order and length deltas do not masquerade as content deltas:

```sh
python3 _work/reconstructions/exidx_raw_audit.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so \
  cmake-build-match/libgof2hdaa.so
```

The optional third argument is a gold `--Map` file; it adds the record-order
metric (the position of every referenced extab record in the original's extab
order vs ours).

## 3. `.ARM.exidx` raw words

Entry format (ARM EHABI): `word0 = prel31(function)`,
`word1 = 1` (CANTUNWIND) / `0x8xxxxxxx` (inline compact model) /
`prel31(.ARM.extab)`.

Baseline vs after the marker fix (below):

| metric | baseline | after marker fix |
|---|---:|---:|
| entries | 3566 | 3566 |
| CANT / INL / EXT | 272 / 856 / 2438 | 272 / 856 / 2438 |
| same `word0` | 3560 | **3566** |
| same function address | 3560 | **3566** |
| same `word1` | 1233 | 1237 |
| same kind | 3560 | **3566** |
| CANT `word1` differ | 0 | 0 |
| INL `word1` differ | 0 | 0 |
| EXT `word1` differ | 2327 / 2436 | 2329 / 2438 |
| raw `.ARM.exidx` differing bytes | 2735 / 28528 | 2709 / 28528 |

The baseline's six `word0` mismatches are not address differences: the extra
CANT shifts the exidx entry addresses by 8 in that region, so `prel31(fn)`
changes even though `fn` is identical. Aligning by function shows all 3566
function addresses are identical in both files.

The 2329 remaining `word1` differences are all `EXT` pointers: the target extab
record is at a different address. `CANT` and inline-model words are fully exact.

## 4. The stale marker (kind sequence)

`_work/reconstructions/exidx_table.py --diff` before the fix:

```
functions with differing kind sequence:
  _Z8GetTouchi: orig [('CANT', None, 27), ('EXT', 'prel31', 27)]
                ours [('EXT', 'prel31', 27)]
  pConstToNonConst: orig [('INL', 0, 39)]
                    ours [('CANT', None, 39), ('INL', 0, 39)]
```

The tree's `MARKER_AFTER` in `tools/gen_section_order.py` keyed the NdkAutosave
sentinel on `.text.ndk_autosave`. After the TU-convergence work that section is
in `o081_NdkHooks.cpp.obj` (which also contains `.text.ndk23_Init`), while
`o082_NdkAutosave.cpp.obj` contains only `.text.pConstToNonConst`. The dict
lookup therefore matched `o081` on `.text.ndk23_Init` and never matched `o082`;
`o082`'s sentinel fell back to "after the owner's last section", i.e. after
`pConstToNonConst` at `0x94818`. The original's second CANT is at `0x9518c`
(after `GetTouch`).

Fix:

```diff
 MARKER_AFTER = {
     ".text.ndk23_Init": ".text.ndk_resetNativeItemInformationList",   # NdkHooks.cpp
     ".text.ndk_autosave": ".text._Z8GetTouchi",                       # NdkAutosave.cpp
+    ".text.pConstToNonConst": ".text._Z8GetTouchi",                   # NdkAutosave.cpp
 }
```

After the fix `exidx_table.py --diff` reports no kind-sequence differences, and
the audit reports `same word0 3566`, `same kind 3566`. Gate:

```
verify: byte 4496  linked 4496  stubs 0  missing 0  extra 0  avg 99.89
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓
drift: 0 field(s) across 0 class(es)
diff: original 5370 ours 5370 extra 0 missing 0
```

`.text` stays `0x1337c8`; `.ARM.exidx` stays `0x6f70`; `.ARM.extab` stays
`0x26290`.

## 5. `.ARM.extab` record order

### Observation

The original's extab records are in input-section order. The original was linked
without a section-ordering file, so its input order and `.text` order are the
same (except `.text.startup`, which gold's default sort moves to the front of
`.text` but not of `.ARM.extab`). Our link reorders `.text` with
`--section-ordering-file` but does not list `.ARM.extab`, so our extab order
follows the *link-input* order, which no longer matches the `.text` order.

Metric (audit script, using a `--Map` relink to map functions to `.text.*`
sections): only **450 / 2438** extab records are at the same position as the
original. The first divergence is at record 531, where gold groups the three
same-named `.ARM.extab.text.startup` sections together.

### Controlled experiment

Scratch relink (no tree change):

1. Relink the current objects with `--Map` (only `-o` and `--Map` differ from
   the build's gold command; the final `strip` is dropped) to get
   `/tmp/ours.map`.
2. Parse the original `.ARM.exidx`/`.ARM.extab`, sort the `EXT` entries by extab
   target address, and map each function address to its `.text.*` section via
   the map. This yields the original's 2438-record order.
3. Rename the three `.ARM.extab.text.startup` sections in the marked object
   copies to unique names (`.ARM.extab.text.startup.o032`, `.o083`, `.o191`) so
   gold does not group them; list the plain `.ARM.extab` (libgcc) at its
   original position.
4. Append `.ARM.extab<text-section-name>` lines to a scratch copy of
   `section_order.txt`, in the original order, followed by `.ARM.extab*`.
5. Relink with the scratch ordering file and re-run the audit.

Measured:

| variant | byte | linked | `.text` | exidx | extab | kind diffs | exidx diff bytes | extab order |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| baseline | 4496 | 4496 | `0x1337c8` | `0x6f70` | `0x26290` | 2 | 2735 | 450 / 2438 |
| +marker fix | 4496 | 4496 | `0x1337c8` | `0x6f70` | `0x26290` | 0 | 2709 | 450 / 2438 |
| +extab order (scratch) | 4496 | 4496 | `0x1337c8` | `0x6f70` | `0x26290` | 0 | **1699** | **2438 / 2438** |

`verify` on the scratch `.so`:

```
compared 4524  avg 99.89%  byte_exact 4496  linked_exact 4496  fuzzy_perfect 4496
stub_zero_size 0  orig_zero_size 0  missing 0 (wrong_type 0  absent 0)  extra 0
```

All alloc-section sizes and bases are unchanged (`.text` `0x1337c8`,
`.ARM.exidx` `0x6f70`, `.ARM.extab` `0x26290`, `.rodata` `0x1c9e10`).

### Record-content result

| variant | records differ | equal-length | diff bytes | length-diff | first-diff histogram |
|---|---:|---:|---:|---:|---|
| baseline | 578 | 571 | 1259 | 7 | `{0: 571}` |
| +marker fix | 578 | 571 | 1259 | 7 | `{0: 571}` |
| +extab order | **448** | **441** | **1044** | **7** | `{0: 440, 50: 1}` |

After the order fix, every one of the 440 equal-length records that differs at
offset 0 differs **only** in the 4-byte personality-routine prel31 word: the
record bytes after offset 4 are identical. The record moved, so the prel31
encoding of the same personality target changed. The one record that differs
later is `PaintCanvas::DrawString` (offset 50):

```
orig  ... 0080 0208 b402 0088 0258 00000000
ours  ... 0080 020e b402 008e 0252 00000000
```

Decoded LSDA call-site table (last two entries):

```
orig: start 256 len 8  lp 308 action 0 ; start 264 len 88 lp 0 action 0
ours: start 256 len 14 lp 308 action 0 ; start 270 len 82 lp 0 action 0
```

i.e. a 6-byte difference in the cleanup range, a genuine body/codegen delta in
that function.

The seven length deltas (net `-8`, the known `.ARM.extab` size residue) are:

```
_Z8ArrayAddIP8KIPlayerEvT_R5ArrayIS2_E   336 -> 328  -8
_ZN11LevelScriptD2Ev                    348 -> 352  +4
_ZN12HangarWindow8render3DEv            948 -> 928 -20
_ZN13RecordHandler10readWantedEj        656 -> 640 -16
_ZN3Hud22drawChallengeModeScoreEi       476 -> 492 +16
_ZN7StarMap16drawOnScreenInfoEib        116 -> 128 +12
_ZN9Generator11createAgentEP7Station    172 -> 176  +4
                                        total       -8
```

## 6. Proven vs hypothesis

Proven by measurement in this tree:

* All 3566 exidx function addresses (word0) are identical; the baseline's six
  word0 mismatches are the extra-CANT entry-address shift, not address deltas.
* All 272 CANT and 856 inline-model word1 words are identical. The only exidx
  word1 differences are EXT prel31 pointers (2329/2438 after the marker fix).
* The two kind-sequence differences are caused by the stale
  `.text.ndk_autosave` marker witness; adding
  `.text.pConstToNonConst -> .text._Z8GetTouchi` makes the kind sequence exact
  with no gate change.
* The original's `.ARM.extab` order is its input order; our order is the
  link-input order. Listing `.ARM.extab...` sections in the original's extab
  order in the section-ordering file reproduces 2438/2438 record positions.
* After the order fix, 440/441 equal-length differing records differ only in
  the personality prel31 word; the remaining record is
  `PaintCanvas::DrawString`'s LSDA cleanup range. The seven length deltas are
  the known function-body deltas.

Hypothesis / still open:

* The seven length deltas and the `DrawString` cleanup range are body/codegen
  differences in those functions, not table-placement bugs. Fixing the bodies
  should make `.ARM.extab` byte-identical and, with the order fix, `.ARM.exidx`
  byte-identical.
* The extab ordering experiment is proven but **not yet integrated into the
  build tools**. Integrating it requires `gen_exidx_markers.py` to rename the
  three `.ARM.extab.text.startup` sections uniquely and `gen_section_order.py`
  to emit the extab order. The scratch procedure above is the reference.
* The original's TU/input order is not directly observable; the claim rests on
  the exact 2438/2438 sequence match and the controlled relink.

## 7. Reproduction commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint -j8
cmake --build cmake-build-match --target drift -j8
cmake --build cmake-build-match --target diff -j8

python3 _work/reconstructions/exidx_table.py --diff \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so

python3 _work/reconstructions/exidx_raw_audit.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

The scratch extab-order relink (reference; see §5):

```sh
# 1. relink with a map, no strip (only -o/--Map differ from build.ninja)
#    /tmp/relink_map2.sh <out.so> <out.map> <section_order.txt>
# 2. generate the original extab order and the scratch ordering file
python3 _work/reconstructions/extab_order_experiment.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/ours.map \
  cmake-build-match/section_order.txt /tmp/section_order_extab.txt
# 3. rename the three startup extab sections in the marked object copies
OBJCOPY=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objcopy
$OBJCOPY --rename-section .ARM.extab.text.startup=.ARM.extab.text.startup.o032 \
  cmake-build-match/exidx_markers/objects/o032_Engine.cpp.obj
$OBJCOPY --rename-section .ARM.extab.text.startup=.ARM.extab.text.startup.o083 \
  cmake-build-match/exidx_markers/objects/o083_LODManager.cpp.obj
$OBJCOPY --rename-section .ARM.extab.text.startup=.ARM.extab.text.startup.o191 \
  cmake-build-match/exidx_markers/objects/o191_ParticleSettings.cpp.obj
# 4. relink with the scratch ordering file and audit
/tmp/relink_map2.sh /tmp/extab2.so /tmp/extab2.map /tmp/section_order_extab.txt
python3 _work/reconstructions/exidx_raw_audit.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/extab2.so /tmp/ours.map
```

## 8. Files changed

```
tools/gen_section_order.py                              +1   (MARKER_AFTER witness)
_work/reconstructions/exidx_raw_audit.py                new  (raw audit tool)
_work/reconstructions/extab_order_experiment.py         new  (scratch order generator)
_work/reconstructions/exidx_extab_raw_records_w3625.md  new  (this note)
```
