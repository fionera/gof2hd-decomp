# Section-order removal: `Item::extractItems` placement (w3745)

Worker: w3745. Branch: `wave87-w3745`, based on master `a86e3396`.
Original: `../_work/bins/android_2.0.16_libgof2hdaa.so` (stripped, armeabi-v7a,
NDK r18b clang 7.0.2, `-Oz`). All numbers below were produced in this worktree
with the commands in §7. This is a follow-up to
`_work/reconstructions/section_order_removal_w3745.md` (§7 identified
`Array<AbstractGun*>::Array()` as the then-current plain no-order frontier).

## 1. Baseline

`cmake-build-match` was configured with `cmake --preset default`. Baseline
(`cmake --build cmake-build-match --target check -j8`, then `lint`, `drift`):

```text
verify: byte 4497  linked 4497  stubs 0  missing 0  extra 0  avg 99.9
data:   exact 843 of 843 exported objects
sodiff: extra-beyond-allow 0  missing 0  allowed 0   imports: 207
RATCHET PASS
lint: CLEAN ✓ (no new soft violations; hard invariants 0)
drift: 0 field(s) across 0 class(es)
```

Gate ELF layout (`readelf -SW cmake-build-match/libgof2hdaa.so`):

```text
.text            0x1337c8 @ 0x69440
.ARM.exidx       0x006f70 @ 0x19cc08
.ARM.extab       0x026290 @ 0x1a3b78   (original is 0x26298; known -8)
.rodata          0x041b67 @ 0x1c9e10
.data            0x00296c @ 0x214000
```

Plain no-order link (`relink_noorder.py` + `verify.py` + `addr_match.py`, §7):

```text
compared 4524  avg 99.9%  byte_exact 3013  linked_exact 4497  fuzzy_perfect 4497
3215/4524 same; prefix 2639;
first (0xE049C, Item::extractItems, 0xE04D4)
```

So the frontier at this HEAD is exactly `Item::extractItems`: original
`0xe049c`, ours `0xe04d4` (+0x38 = the size of `Item::clone()`).

## 2. Target order vs the current `Item.o` order

`cmake-build-match/section_order.txt` (generated from the original dynsym
address order) is the target. The `Item` block is:

```text
... fabricate
    makeItem(int)
    combineItems
    extractItems
    combineDuplicates
    ArrayRemove<Item*>
    mixItems
    equals
    isWeapon
    makeItem(int, int)
    clone
    makeItem()
    checkCredits
    adjustPrice
    checkCargoSpace
    isUnsaleable
```

The current `Item.cpp.obj` section-header order is (baseline):

```text
... fabricate
    Array<Item*>::Array()
    ArrayAdd<Item*>
    makeItem(int)
    combineItems
    clone                 <-- emitted early
    ArraySetLength<Item*>
    extractItems
    combineDuplicates
    ArrayRemove<Item*>
    mixItems
    Array<Item*>::~Array()
    equals
    isWeapon
    makeItem(int, int)
    makeItem()
    checkCredits
    ...
```

Reproduce:

```sh
python3 - <<'PY'
import subprocess, re
out = subprocess.run(['readelf','-SW',
  'cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/Item.cpp.obj'],
  capture_output=True, text=True).stdout
names = [m.group(1)[6:] for m in
         (re.match(r'\s*\[\s*\d+\]\s+(\.text\.\S+)\s+PROGBITS', l)
          for l in out.splitlines()) if m]
dem = subprocess.run(['c++filt'], input='\n'.join(names),
                     capture_output=True, text=True).stdout.splitlines()
for i, d in enumerate(dem, 1): print(i, d)
PY
```

The weak copies (`Array<Item*>::Array`, `ArrayAdd<Item*>`,
`ArraySetLength<Item*>`, `ArrayRemove<Item*>`, `Array<Item*>::~Array`) are
emitted by our `Item.o` but are also emitted by `Generator.o` / `ListItem.o`,
which come earlier in `link_order.rsp`; gold keeps the first weak definition,
so they are already at their original addresses in the plain link
(`0x95b58`, `0x95ba8`, `0x95b74`, `0xa6b9e`). They are not the problem.

The only displaced section is `clone`: it is emitted after `combineItems`
instead of after `makeItem(int, int)`, which pushes `extractItems` and the
whole following block by `0x38`.

## 3. Why `clone` is emitted early

`Item::makeItem(int)` is referenced by `Item::fabricate` (line 242) before its
definition, so clang emits `makeItem(int)` immediately after `fabricate`
(pulled forward). Its body calls `clone()`, so `clone` is queued for deferred
emission. The queue is flushed after the next emitted top-level definition
following `makeItem`'s **source position**, not after `makeItem`'s emission
position. This is the same behaviour already documented for
`Level::getGasClouds`/`Array<KIPlayer*>` in
`section_order_removal_w3745.md` §2/§7.

Controlled experiment (compile only, `Item.cpp` variants compiled with the
exact `compile_commands.json` command): move the `makeItem(int)` definition to
different source positions and read back the `.text.*` order.

| `makeItem(int)` source position | section order around the block | `clone` lands after |
|---|---|---|
| before `combineItems` (baseline) | `makeItem, combineItems, clone, ArraySetLength, extractItems` | `combineItems` |
| after `combineItems` | `makeItem, combineItems, ArraySetLength, clone, extractItems` | `ArraySetLength` |
| after `extractItems` | `makeItem, combineItems, ArraySetLength, extractItems, clone, combineDuplicates` | `extractItems` |
| after `combineDuplicates` | `..., combineDuplicates, ArrayRemove, clone, mixItems` | `ArrayRemove` |
| after `mixItems` | `..., mixItems, Array<Item*>::~Array, clone, equals` | `Array<Item*>::~Array` |
| after `equals` | `..., equals, clone, isWeapon` | `equals` |
| after `isWeapon` | `..., isWeapon, clone, makeItem(int,int)` | `isWeapon` |
| after `makeItem(int,int)` / after `clone` / after `makeItem()` / at end | `..., makeItem(int,int), clone, makeItem()` | its own definition (target) |

So `clone` is emitted immediately after the next emitted function following
`makeItem`'s source position. To get the target order, `makeItem(int)` must be
defined at or after `makeItem(int, int)` — i.e. after the last function before
`clone`.

## 4. Landed source change

Move `Item::makeItem(int)` from just before `combineItems()` to just after
`makeItem(int, int)` (immediately before `clone()`). This is the minimal move
that puts `makeItem(int)`'s source position at the last function before
`clone`; it groups the two clone-based factories.

```diff
@@ after Item::fabricate()
-Item *Item::makeItem(int amount) {
-    int savedPrice = price;
-    Item *item = clone();
-    item->price = savedPrice;
-    item->amount = amount;
-    return item;
-}
-
 ItemArray *Item::combineItems(ItemArray *items, ItemArray *stationItems) {
@@ after Item::makeItem(int amount, int price)
+Item *Item::makeItem(int amount) {
+    int savedPrice = price;
+    Item *item = clone();
+    item->price = savedPrice;
+    item->amount = amount;
+    return item;
+}
+
 Item *Item::clone() {
```

The two equivalent placements `after clone()` (before `makeItem()`) and
`before checkCredits()` were also tested and produce a **byte-identical gate
`.so`** (`cmp` identical), so the exact position within the
`makeItem(int,int) … makeItem()` group is not observable in the binary.

### 4.1 Resulting object order

```text
... fabricate
    Array<Item*>::Array()
    ArrayAdd<Item*>
    makeItem(int)
    combineItems
    ArraySetLength<Item*>
    extractItems
    combineDuplicates
    ArrayRemove<Item*>
    mixItems
    Array<Item*>::~Array()
    equals
    isWeapon
    makeItem(int, int)
    clone
    makeItem()
    checkCredits
    adjustPrice
    checkCargoSpace
    isUnsaleable
```

After gold's weak-symbol dedup this is exactly the original `Item` block:
every `Item::*` symbol in the plain no-order link is at its original address:

```text
0xe0338 makeItem(int)        0xe049c extractItems      0xe07c6 makeItem(int,int)
0xe034a combineItems         0xe0504 combineDuplicates 0xe07d8 clone
0xe05ca mixItems             0xe07a6 equals            0xe0810 makeItem()
0xe07ba isWeapon             0xe0824 checkCredits      0xe0848 adjustPrice
0xe088c checkCargoSpace      0xe08c4 isUnsaleable
```

## 5. Measured before/after

Every row is a full rebuild of this tree. `gate` is
`cmake --build cmake-build-match --target check -j8` (ordering-file link);
`plain` is `relink_noorder.py` (the exact gold line minus
`--section-ordering-file`) followed by `verify.py` and `addr_match.py`.

| state | gate byte/linked | plain byte | funcs at orig addr | prefix | first plain mismatch (orig vs ours) |
|---|---:|---:|---:|---:|---|
| baseline (`makeItem(int)` before `combineItems`) | **4497 / 4497** | **3013** | 3215 / 4524 | 2639 | `Item::extractItems` `0xe049c` vs `0xe04d4` |
| landed (`makeItem(int)` after `makeItem(int,int)`) | **4497 / 4497** | **3019** | **3223 / 4524** | **2657** | `PlayerStatic::update` `0xe0980` vs `0xe09bc` |

Gate is unchanged in every counted metric: `byte 4497 / linked 4497`,
`stubs 0 / missing 0 / extra 0`, `data 843/843`, `sodiff` 0 beyond allow,
`imports 207`, `RATCHET PASS`, lint `CLEAN ✓`, drift `0`, `.text`
`0x1337c8 @ 0x69440`.

Plain no-order delta: **+6 byte_exact, +8 functions at their original
address, +18 exact-address prefix**. The whole `Item` block is now
address-exact.

### 5.1 Gate `.so` side effect (`.ARM.exidx` / `.ARM.extab`)

The gate `.text` is byte-identical to the baseline (`cmp` shows no difference
in `0x69440..0x19cc08`). The gate `.so` differs from the baseline in exactly:

| range | bytes | section | what |
|---|---:|---|---|
| `0x1dc..0x1ef` | 20 | `.note.gnu.build-id` | content hash |
| `0x1a06dc..0x1a070c` | 6 | `.ARM.exidx` | extab prel31 words for the `Item` entries |
| `0x1ae618..0x1ae67f` | 88 | `.ARM.extab` | extab bytecode order |

This is the expected consequence of moving `makeItem`'s section: the
per-function `.ARM.extab.text.*` sections follow the object's section order.
The new extab order matches the original's order (every `Item` extab target is
the original address minus the known 8-byte `.ARM.extab` shortfall); the old
order had `clone`'s extab entry at `extractItems`' slot. The raw diff count
against the original is unchanged for all three sections
(`.text` 20716, `.ARM.exidx` 115568, `.ARM.extab` 113824 bytes), because the
known 8-byte extab shortfall dominates. No counted gate metric moves.

## 6. Next frontier

The next plain no-order mismatch is `PlayerStatic::update`:

```text
original: 0xe0980 update (2 bytes), 0xe0982 getPosition, 0xe09b8 render
ours:     0xe0980 render (6 bytes), 0xe0986 getPosition, 0xe09bc update
```

`src/game/ship/PlayerStatic.cpp` defines `render` before `update`; the original
address order is `update, getPosition, render`. This is a separate TU and was
not changed in this pass.

## 7. Exact commands

```sh
cmake --preset default
cmake --build cmake-build-match --target check -j8
cmake --build cmake-build-match --target lint  -j8
cmake --build cmake-build-match --target drift -j8

# plain no-order link + measurements
python3 _work/reconstructions/w3696/relink_noorder.py /tmp/w3745/noorder.so
python3 tools/verify/verify.py \
  --build-dir /tmp/w3745/verify_noorder \
  --original ../_work/bins/android_2.0.16_libgof2hdaa.so \
  --ours /tmp/w3745/noorder.so \
  --objdump .cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-objdump \
  --fail-on-wrong-type --top 0
python3 _work/reconstructions/w3696/addr_match.py \
  ../_work/bins/android_2.0.16_libgof2hdaa.so /tmp/w3745/noorder.so

# object section order
readelf -SW cmake-build-match/CMakeFiles/gof2.dir/src/game/mission/Item.cpp.obj \
  | awk '$3 ~ /^\.text\./ {print $3}' | sed 's/^\.text\.//' | c++filt
```

The variant experiments in §3 used the exact `compile_commands.json` command
for `Item.cpp` with only `-o` and the input path replaced; no flag was
changed.

## 8. Proven vs hypothesis

**Proven by measurement**

* Baseline gate is `byte 4497 / linked 4497`, `.text 0x1337c8 @ 0x69440`,
  lint `CLEAN ✓`, drift 0; plain no-order is `3013 / 4497`, prefix 2639,
  3215/4524, first mismatch `Item::extractItems` `0xe049c` vs `0xe04d4`.
* The baseline `Item.o` emits `clone` after `combineItems` and before
  `extractItems`; the original emits it after `makeItem(int,int)`.
* Moving `makeItem(int)` to any source position at or after
  `makeItem(int,int)` (before `clone`, after `clone`, after `makeItem()`, or at
  the end) makes clang emit `clone` at its natural position and reproduces the
  original `Item` block exactly. The gate `.so` is byte-identical for the
  `before clone` and `after clone` placements.
* The landed move is gate-neutral (`4497 / 4497`, lint `CLEAN`, drift 0) and
  improves the plain no-order build to `3019 / 4497`, prefix 2657, 3223/4524.
  Every `Item::*` symbol is now at its original address in the plain link.
* The gate `.text` is byte-identical to the baseline; only the build-id and
  the `.ARM.exidx`/`.ARM.extab` bytes for the moved section change.

**Hypothesis / open**

* The exact clang 7 rule that flushes the deferred queue after the next
  top-level definition following the pulled-forward function's source position
  is characterised empirically (§3) but not from the compiler source.
* The remaining frontier is `PlayerStatic::update` / `render` source order
  (§6); it was not tested in this pass.
* The ordering file still cannot be removed: the plain no-order build is
  `3019`, not `4497`.
