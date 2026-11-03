# w4237: `.ARM.exidx` LSDA-record pass — recordStoreWrite + Hud::draw

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4237`, branch `wave92-w4237`,
base `b379d00d` (HEAD `34e76bbe`). Ground truth:
`../_work/bins/android_2.0.16_libgof2hdaa.so` (real address = Ghidra address −
0x10000). The changes are ordinary C++ in `RecordHandler.cpp` and `Hud.cpp`; no
linker/data tricks and no EH anchors.

## Result

`.ARM.exidx` and `.ARM.extab` stay **size- and base-exact** and every other
section is unchanged.  The raw `.ARM.exidx` content gap shrinks because two of
the five mismatched LSDA records now have the original length, so their extab
targets land at the original addresses:

| metric | before | after |
|---|---:|---:|
| `.ARM.exidx` differing bytes | 1110 | **839** |
| `.ARM.exidx` differing entries (`w1` prel31 targets) | 1056 | **822** |
| `.ARM.extab` differing bytes | 117742 | **108471** |
| `.text` / `.ARM.exidx` / `.ARM.extab` / `.rodata` sizes | exact | exact |
| `byte_exact` / `linked_exact` | 4498 / 4498 | **4498 / 4498** |
| size-exact / data / imports / lint / drift | 4524, 843, 207, CLEAN, 0 | same |

`w0` (function address) is identical for all 3566 entries; only `w1` (the
prel31 to `.ARM.extab`) differs, and only for records whose extab address is
shifted by a preceding record-length delta.

## The five records at the start of the pass

`_work/reconstructions/exidx_table.py` (with the `classInfoOffset` fix below)
reports the five records that differ; they net to zero:

| function | orig | base | delta | rows orig/base |
|---|---:|---:|---:|---:|
| `Generator::createMission` | 172 | 176 | +4 | 26 / 26 |
| `Level::createShip` | 336 | 328 | −8 | 57 / 54 |
| `RecordHandler::recordStoreWrite` | 656 | 640 | −16 | 107 / 104 |
| `LevelScript::process` | 348 | 352 | +4 | 46 / 46 |
| `Hud::draw` | 476 | 492 | +16 | 77 / 80 |
| **total** | | | **0** | |

Because the deltas cancel, the section size is exact, but every record between
a mismatched record and the next compensating one is at the wrong address, so
1056 of the 2438 extab entries had a different `w1`.

## Parser correction: `classInfoOffset` is ULEB128, not an encoded pointer

The committed `exidx_table.py` reads the LSDA type-table offset as
`readEncodedPointer(ttypeEncoding)`.  libc++abi's real parser
(`sources/cxx-stl/llvm-libc++abi/src/cxa_personality.cpp`, NDK r18b) is:

```c
uint8_t ttypeEncoding = *lsda++;
if (ttypeEncoding != DW_EH_PE_omit) {
    uintptr_t classInfoOffset = readULEB128(&lsda);   // <-- ULEB128
    classInfo = lsda + classInfoOffset;
}
uint8_t callSiteEncoding = *lsda++;
uint32_t callSiteTableLength = readULEB128(&lsda);
```

This matters for every record with `ttypeEncoding = 0x00` (a `catch(...)`
type table).  `Level::createShip` is such a record: with the old parser it
reported `ttype_enc=0x00`, a bogus 4-byte class-info pointer and 0 rows; with
the ULEB128 rule it parses as `ttype_enc=0x00`, `classInfoOffset` 1 byte,
`callSiteEncoding=0x01`, 299 table bytes / 54 rows.  `exidx_table.py` is
updated in this commit:

```diff
     ttype_enc = data[i]
     i += 1
-    _ci, i = _read_enc(data, i, ttype_enc)
+    # libc++abi reads classInfoOffset as a ULEB128 offset from the byte after
+    # it (cxa_personality.cpp), NOT with ttypeEncoding.
+    if ttype_enc != 0xff:
+        _cioff, i = _uleb(data, i)
     cs_enc = data[i]
```

The same commit also applies the documented Thumb-bit attribution fix
(`(v & ~1) <= fn` instead of `v <= fn`), so the diff names the true owners
(`Level::createShip`, `LevelScript::process`, `Generator::createMission`)
instead of the preceding function.

## Fix 1 — `RecordHandler::recordStoreWrite` (640 → 656, 107 rows, table-identical)

This is the fix documented (but not landed) in
`extab_hangarwindow_sink_w4209.md`.  Four declarations move to their first-use
sites, which changes clang's cleanup call-site grouping without changing the
`0xf94`-byte body:

```diff
 int RecordHandler::recordStoreWrite(int slot) {
     String path = this->recordDir + String(slot);
+    Array<Item *> *stationItems;
     if (AEFile::FileExist(path) != 0) { ... }
     unsigned int fd;
     AEFile::OpenWrite(path, &fd);
+    HintFlags *hints = reinterpret_cast<HintFlags *>(Globals::hints);
     bool *visited = Globals::galaxy->getVisited();
@@
     Array<Station *> *stack = Globals::status->getStationStack();
-    Array<Item *> *stationItems;
     AEFile::Write((int32_t) 3, fd);
@@
-    HintFlags *hints = reinterpret_cast<HintFlags *>(Globals::hints);
     AEFile::Write(hints->flag[8], fd);
@@
-    OptionsBuffer *options = reinterpret_cast<OptionsBuffer *>(Globals::options);
-    AEFile::Write(options->difficulty, fd);
+    AEFile::Write(reinterpret_cast<OptionsBuffer *>(Globals::options)->difficulty, fd);
@@
-    Ship *wingman = reinterpret_cast<Ship *>(Globals::status->field_8c);
-    if (wingman != 0) {
+    if (reinterpret_cast<Ship *>(Globals::status->field_8c) != 0) {
```

(The full applied form also renames the later `items`/`mods` locals to
`wingCargo`/`voidItems`/`wingMods`/`shipMods`; see the diff in the commit.)
The function stays byte-identical (`0xf94`, `byte_exact=True`), and the LSDA is
now **656 B / 107 rows and row-for-row identical to the original** — verified
by diffing the parsed call-site tables:

```
$ python3 /dev/shm/w4237/lsda.py <orig> _ZN13RecordHandler16recordStoreWriteEi
$ python3 /dev/shm/w4237/lsda.py <ours> _ZN13RecordHandler16recordStoreWriteEi
# both: ttype=0xff n=638 rows=107; diff of the row lists is empty
```

This is a **+16 B** record growth, so it needs a −16 B compensation.

## Fix 2 — `Hud::draw` (492 → 476, 77 rows, length-exact)

`Hud::draw` is one of the 26 non-byte-exact bodies (88.8 %), so its code bytes
are free as long as the function size (9332 B) and the `.rel.plt`
first-reference order are preserved.  The extra 3 rows are in the jump-drive /
cloak progress-bar block (code offsets 7698..8108): ours splits
`SetColor`/`DrawImage2D`/`DrawRegion2D`, `GetTextWidth` and `DrawString` into
three cleanup regions (landing pads 9182 / 9050 / 9272), while the original has
one region `(7696, 412, 9280)`.

The landed source shape moves the `String text` declaration down to the fade
computation and folds `textW` into the `DrawString` argument:

```diff
             barOffset = (int) ((float) textH + (float) frameH * 2.5f);
-            String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
             int barW = Globals::Canvas->GetImage2DWidth(this->image_0x37c);
             int barH = Globals::Canvas->GetImage2DHeight(this->image_0x37c);
             this->field_0x468 += dt;
             float fadeIn = (float) this->field_0x468 / 1000.0f;
             float fade = 1.0f;
+            String text = *Globals::gameText->getText(this->field_0x279 ? 3205 : 3204) + String(" ");
             if (fadeIn < 1.0f)
                 fade = fadeIn;
@@
             unsigned int fontId = (unsigned int) (uintptr_t) Globals::font;
             int halfW = Globals::w >> 1;
-            int textW = Globals::Canvas->GetTextWidth(fontId, text);
             float textY = (float) top + (float) barH * 2.5f;
-            Globals::Canvas->DrawString(fontId, text, halfW - (textW >> 1), (int) textY, false);
+            Globals::Canvas->DrawString(fontId, text, halfW - (Globals::Canvas->GetTextWidth(fontId, text) >> 1), (int) textY, false);
```

Measured on the object (isolated `Hud.cpp` compile):

| variant | text | extab |
|---|---:|---:|
| base (declaration before `barW`, separate `textW`) | 9332 | 492 |
| declaration after `float fade = 1.0f;` only | 9332 | 480 |
| declaration after `float fade` + inline `textW` | **9332** | **476** |

The record is now **476 B / 77 rows** (orig 476 / 77).  The call-site table
content is still 1 byte different (`n=453` vs 454, absorbed by the 4-byte
record alignment), but the record length and therefore the extab address match.

This is the −16 B compensation for Fix 1; together the two changes keep
`.text 0x1337c8`, `.ARM.exidx 0x6f70`, `.ARM.extab 0x26298` and
`.rodata 0x41b67 @ 0x1c9e10` exact.

## Remaining records (net 0, not landed)

After the two fixes `exidx_table.py --diff` reports only three records:

| function | orig | ours | delta | why it is still open |
|---|---:|---:|---:|---|
| `Generator::createMission` | 172 | 176 | +4 | 26 rows both, but one row's landing pad is `1892` (2-byte ULEB) where the original has `0` (1 byte).  The affected basic block is the `race < 4 && type == 13` `nextInt(2)` block: the original places it out-of-line after the `case 15` code and jumps back to the `agentName` construction, so no cleanup is live there; our block is placed inline before `agentName` and carries a cleanup.  A block-placement difference, not a missing row. |
| `Level::createShip` | 336 | 328 | −8 | 57 vs 54 rows.  The original has no `try/catch` (`ttypeEncoding=0xff`, 57 cleanup rows); ours still has the artificial `try { setEmpData } catch (...) { __builtin_trap(); }` (`ttypeEncoding=0x00`, a 4-byte type table and 54 rows).  Removing the try/catch gives 53 rows / 316 B and grows the body by 12 B, so the real 57-row cleanup shape has to be reconstructed first. |
| `LevelScript::process` | 348 | 352 | +4 | 46 rows both, but the row starts are at code offsets 18640..21746 instead of 12162..14270, so seven starts cross the 16384 ULEB boundary (3 bytes instead of 2).  The function body is only 55 % matched; the code layout before that block is ~6.5 KB longer in ours. |

Because the three deltas net to zero, the section stays exact; fixing them
individually would move `.rodata` and must be landed as a set.  Fixing all
three makes `.ARM.exidx` byte-identical (the 822 remaining differing entries
are exactly the records in the three shifted ranges: 346 in
`createMission..createShip`, 77 in `createShip..recordStoreWrite`, 388 in
`recordStoreWrite..LevelScript::process`, plus 11 boundary records).

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w4237
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check   -j8   # RATCHET PASS, byte 4498
cmake --build cmake-build-match --target lint    -j8   # CLEAN
cmake --build cmake-build-match --target drift   -j8   # 0 fields

# section sizes
.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf \
  -SW cmake-build-match/libgof2hdaa.so | grep -E '\.text |\.ARM.exidx|\.ARM.extab|\.rodata '

# per-record lengths / rows
python3 _work/reconstructions/exidx_table.py --diff \
  ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```

## Files changed

- `src/game/mission/RecordHandler.cpp` — `recordStoreWrite` declarations moved
  to first use (656 B / 107-row table-identical LSDA; body unchanged).
- `src/game/ui/Hud.cpp` — `String text` declaration moved to the fade
  computation and `textW` folded into `DrawString` (476 B / 77-row LSDA;
  body size unchanged at 9332 B).
- `docs/IDENTICALITY.md` — `.ARM.exidx` / `.ARM.extab` remaining-gap text.
- `_work/reconstructions/exidx_table.py` — ULEB128 `classInfoOffset` and
  Thumb-bit attribution fixes.
- this note.
