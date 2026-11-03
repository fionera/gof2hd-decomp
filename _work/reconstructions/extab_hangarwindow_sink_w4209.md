# w4209: `.ARM.extab` size-exact — HangarWindow::render LSDA sink relocation

Scope: worktree `/home/debian/GalaxyOnFire2/wt/w4209`, branch `wave92-w4209`,
base `b379d00d`. Ground truth: `../_work/bins/android_2.0.16_libgof2hdaa.so`
(real address = Ghidra address − 0x10000). The change is one sink in
`src/game/ui/HangarWindow.cpp`; no linker/data tricks.

## Result

`.ARM.extab` is now **size- and base-exact**: `0x26298` (156,312 B) at
`0x1a3b78`, and `.rodata` stays at `0x1c9e10`. The full gate is unchanged:
`byte_exact 4498/4524`, `linked_exact 4498/4524`, `size-exact 4524/4524`,
`stub_zero_size 0`, `missing 0`, `wrong_type 0`, `extra 0`, `data_exact 843/843`,
`imports 207`, `.text 0x1337c8`, `.ARM.exidx 0x6f70`, lint `CLEAN ✓`, drift 0.

Before/after section table (ours vs original):

| section | orig | before | after |
|---|---:|---:|---:|
| `.text` | 0x1337c8 | 0x1337c8 | 0x1337c8 |
| `.ARM.exidx` | 0x6f70 | 0x6f70 | 0x6f70 |
| `.ARM.extab` | 0x26298 | **0x26290 (-8)** | **0x26298 (0)** |
| `.rodata` (base) | 0x1c9e10 | 0x1c9e10 | 0x1c9e10 |

The last 8 bytes were absorbed by `.rodata`'s 16-byte alignment before the
change (extab end `0x1c9e08` + 8 pad = `0x1c9e10`), which is why the old
gate could still report `.rodata` base-exact with a short extab.

## Baseline per-function LSDA deltas

`pairdiff.py` (parse `.ARM.exidx` prel31 targets, sort the distinct extab
records, length = next record − this record) on the base build:

```text
Generator::createMission(Agent*, Array<SolarSystem*>*)  orig 172  ours 176  +4
Level::createShip(int,int,int,Waypoint*,bool,bool)      orig 336  ours 328  -8
RecordHandler::recordStoreWrite(int)                    orig 656  ours 640  -16
LevelScript::process(int)                               orig 348  ours 352  +4
HangarWindow::render()                                  orig 948  ours 940  -8
Hud::draw(long long,long long,PlayerEgo*,bool,unsigned,unsigned) orig 476 ours 492 +16
                                                                    net -8
```

Every other extab record is byte-length-identical. The six deltas sum to the
section's −8 B.

## HangarWindow::render record anatomy

`HangarWindow::render` is one of the 26 non-byte-exact bodies (80.6 %), so
changing its code bytes is free as long as its size and the `.rel.plt`
first-reference order are preserved.

Original record: **948 B / 156 rows**, `ttype=0xff`.
Base ours: **940 B / 155 rows**, `ttype=0xff`.

The missing row is at the cleanup split around code offset 1406:

```text
orig  (1406, 4, 9418, 0)   (1416, 4, 9588, 0)
ours  (1406, 18, 9586, 0)
```

The original has two 4-byte cleanup regions where ours has one 18-byte region.
Matching the *table contents* would require matching the 80.6 % body; the
section-size gate only needs the record length, so a sink that splits a
cleanup region anywhere in the body is sufficient.

## First attempt (rejected): sink in the isShip branch

Inserting the sink immediately after

```cpp
Globals::Canvas->SetColor(0xffffffffu);
```

in the `item->isShip()` branch (source line 730) gives the right record size:

```text
extab 948 B / 156 rows, code 9700 B
```

but it **reorders `.rel.text._ZN12HangarWindow6renderEv`**. The compiler
moves the `isShip` block's relocations ahead of the `isItem` block's
relocations, so the first reference to
`Item::canBeInstalledMultipleTimes` moves from before
`BluePrint::getCurrentAmount` to after `ListItem::isMoveToCargoButton`.
Gold then shifts five PLT slots:

```text
base .rel.plt (matches original):
  0x2133fc Item::canBeInstalledMultipleTimes
  0x213400 BluePrint::getCurrentAmount
  0x213404 BluePrint::getTotalAmount
  0x213408 Item::isInList
  0x21340c ListItem::isMoveToCargoButton

pad-at-730 .rel.plt:
  0x2133fc BluePrint::getCurrentAmount
  0x213400 BluePrint::getTotalAmount
  0x213404 Item::isInList
  0x213408 ListItem::isMoveToCargoButton
  0x21340c Item::canBeInstalledMultipleTimes
```

That costs two `byte_exact` functions (`HangarWindow::selectItem` and
`BluePrint::getCurrentAmount`, both linked-exact 100 % but byte-false) →
`byte_exact 4496`. A pure extab-size patch (append 8 zero bytes to the
`render` extab section in the object) does **not** reorder the PLT, so the
cause is the `isShip`-branch code motion, not the extab size itself.

## Landed shape: sink after the item-kind test, replace the old pad0 anchor

Insert one sink after the item-kind test at source line 873 and remove the
old `volatile int pad0 = count;` sink in the buy-credits loop (the existing
`pad0b` sink stays):

```diff
@@ -871,6 +871,7 @@ void HangarWindow::render() {
                         if (item->isItem() || item->isShip() || item->isBluePrint() || item->isPendingProduct()) {
+                            volatile int padS = 0; (void) padS;
                             (*this->buttons)[0]->setPosition(
@@ -961,8 +962,7 @@ void HangarWindow::render() {
                 {
-                    volatile int pad0 = count;
-                    // Second sink: pins the same 4-byte size/regalloc delta as pad0.
+                    // Sink: keeps the original 4-byte size/regalloc delta.
                     volatile int pad0b = count;
                     (*this->buttons)[12 + count]->setSplitText(price);
                 }
```

This is the only single-line sink placement found by a 372-line sweep of
`render` that simultaneously gives

```text
extab 948 B / 156 rows, code 9700 B, .rel.text.render symbol order == base
```

(`padS` alone gives 948/9704; removing `pad0` gives back the 4 bytes, so the
pair is size-neutral. The old `pad0`/`pad0b` pair was itself a size/regalloc
anchor; the pair is now `padS`/`pad0b`.)

Verification of the landed build:

```text
readelf -SW: .text 0x1337c8, .ARM.exidx 0x6f70, .ARM.extab 0x26298,
             .rodata 0x1c9e10 (all equal to the original)
pairdiff:    HangarWindow::render orig 948 / ours 948 (delta 0)
cmprows:     orig 948 B / 156 rows; ours 948 B / 156 rows
.rel.plt:    identical to the base build (3674 entries), which in the
             affected region is identical to the original
check:       byte 4498  linked 4498  stubs 0  missing 0  extra 0
             data 843/843  imports 207  RATCHET PASS
lint:        CLEAN ✓
drift:       0 field(s) across 0 class(es)
```

The table *contents* still differ (the body is 80.6 %), but the record length
and row count now match, which is what the section-size gate measures.

## Remaining extab deltas (net 0, section exact by cancellation)

After the change `pairdiff.py` reports:

```text
Generator::createMission   +4
Level::createShip          -8
RecordHandler::recordStoreWrite -16
LevelScript::process       +4
Hud::draw                  +16
                           ----
                            0
```

`HangarWindow::render` is gone from the list. The section is size-exact even
though five records still differ; closing them individually is future work.

## RecordHandler::recordStoreWrite (proven, not landed)

The recordStoreWrite LSDA can be made **656 B / 107 rows, table-identical to
the original** by moving four declarations to their first-use sites in
`src/game/mission/RecordHandler.cpp`:

```diff
-    Array<Item *> *stationItems;
     if (AEFile::FileExist(path) != 0) {
         AEFile::FileDelete(path);
     }
@@
-    HintFlags *hints = reinterpret_cast<HintFlags *>(Globals::hints);
     bool *visited = Globals::galaxy->getVisited();
@@
     Array<Station *> *stack = Globals::status->getStationStack();
+    Array<Item *> *stationItems;
@@
+    HintFlags *hints = reinterpret_cast<HintFlags *>(Globals::hints);
     AEFile::Write(hints->flag[8], fd);
@@
-    AEFile::Write(reinterpret_cast<OptionsBuffer *>(Globals::options)->difficulty, fd);
+    OptionsBuffer *options = reinterpret_cast<OptionsBuffer *>(Globals::options);
+    AEFile::Write(options->difficulty, fd);
@@
-    if (reinterpret_cast<Ship *>(Globals::status->field_8c) != 0) {
+    Ship *wingman = reinterpret_cast<Ship *>(Globals::status->field_8c);
+    if (wingman != 0) {
```

The function body stays `0xf94` bytes and byte-identical; only the cleanup
call-site ranges change. This fix is **+16 B** to the section, so landing it
alone would move `.rodata` to `0x1c9e20` and collapse `byte_exact`. It needs a
compensating −16 B (e.g. `Hud::draw`'s +16) and is therefore not part of this
commit. The exact-rsw source is preserved at
`/dev/shm/w4209/RecordHandler.exact_rsw.cpp`; the diff above is the whole
change.

## Files changed

- `src/game/ui/HangarWindow.cpp` — one sink added, one old sink removed.
- `docs/IDENTICALITY.md` — `.ARM.extab` section row and remaining-gaps text.
- this note.
