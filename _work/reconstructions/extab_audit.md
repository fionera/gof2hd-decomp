# w3391: close the last `+0xc` `.ARM.extab` delta (exact)

Scope: grow `.ARM.extab` by exactly `+0xc` (`0x2628c -> 0x26298`) with **no
change to any function's code size**, keeping `.text` (`0x1337c8`),
`.ARM.exidx` (`0x6f70`) and the `.rodata` base (`0x1c9e10`) exact. Worktree
`wt/w3391`, branch `wave87-w3391`, HEAD `ddc2fd8f` (master).

## Result

| section | original | before (HEAD `ddc2fd8f`) | after | delta after |
|---|---:|---:|---:|---:|
| `.text` | `0x1337c8` | `0x1337c8` | **`0x1337c8`** | **0** |
| `.ARM.exidx` | `0x06f70` | `0x06f70` | **`0x06f70`** | **0** |
| `.ARM.extab` | `0x26298` | `0x2628c` (−0xc) | **`0x26298`** | **0** |
| `.rodata` | `0x41b67` @ `0x1c9e10` | `0x41ba7` @ `0x1c9e10` | `0x41ba7` @ `0x1c9e10` | +0x40 (base exact) |
| `.bss` | `0x069ac` | `0x069ac` | `0x069ac` | **0** |
| `.rel.dyn` | `0x07048` | `0x07048` | `0x07048` | **0** |

Gate: **byte_exact 3921 → 3921** (unchanged), linked_exact 4494 (unchanged),
avg 99.89, `RATCHET PASS`, `drift` 0 fields, `sodiff` symbol parity ✓.
`HangarWindow::render` stays **9700 B (`0x25e4`)**.

`.ARM.extab` is now byte-length exact. `.rodata` still has its known `+0x40`
content delta (first divergence at offset `0x1193`, the `FMOD_GOF2` literal
order described in §4 of the w3367 section), so the 582 literal-pool functions
do not all become byte-exact yet; the section-size goal is nevertheless met.

## Method

The committed `_work/reconstructions/exidx_table.py` parser is used as the
starting point. Two corrections are needed for correct attribution and LSDA
location (the same corrections are documented on `wave87-w3376` in commit
`000186c3`):

1. **Thumb bit.** `.dynsym` FUNC values are odd (Thumb), while the exidx
   prel31 decodes to the even function address. Attribute with `v & ~1 <= fn`,
   not `v <= fn`. This is why the old table named the `-8` record
   `ArrayAdd<KIPlayer>`; the true owner is `Level::createShip` (the exidx
   entry at `0xc0214` is the even address of `createShip`, whose dynsym value
   is `0xc0215`).
2. **LSDA start.** A generic-model `.ARM.extab` record is
   `prel31 personality` + unwind-opcode words + LSDA. The opcode words are
   emitted MSB-to-LSB per 32-bit word (`UnwindOpcodeStreamer`), so the size
   byte is byte 3 of the first opcode word (`extab+7`); the LSDA starts at
   `extab + 4 + 4*(1 + byte[extab+7])`. Compact model 1/2 uses byte 2
   (`extab+2`) and starts at `extab + 4*(1 + byte[extab+2])`.

The per-function table below uses the corrected owners. The `__cxa_demangle`
row is the sum of the vendored libc++abi local helpers.

## Per-function `.ARM.extab` record lengths

Before (HEAD `ddc2fd8f`, total `-0xc`):

| function | original | ours | Δ |
|---|---:|---:|---:|
| `HangarWindow::render` | 948 | 912 | −36 |
| `RecordHandler::recordStoreWrite` | 656 | 640 | −16 |
| `Level::createShip` | 336 | 328 | −8 |
| `Hud::draw` | 476 | 492 | +16 |
| `StarMap::draw` | 116 | 128 | +12 |
| `__cxa_demangle` group | 2172 | 2184 | +12 |
| `LevelScript::process` | 348 | 352 | +4 |
| `Generator::createMission` | 172 | 176 | +4 |
| **total** | | | **−12 = −0xc** |

After this pass (total `0`):

| function | original | ours | Δ | rows (orig/ours) |
|---|---:|---:|---:|---:|
| `HangarWindow::render` | 948 | **924** | **−24** | 156 / 152 |
| `RecordHandler::recordStoreWrite` | 656 | 640 | −16 | 107 / 104 |
| `Level::createShip` | 336 | 328 | −8 | 57 / 54 |
| `Hud::draw` | 476 | 492 | +16 | 77 / 80 |
| `StarMap::draw` | 116 | 128 | +12 | 16 / 18 |
| `__cxa_demangle` group | 2172 | 2184 | +12 | — |
| `LevelScript::process` | 348 | 352 | +4 | 46 / 46 |
| `Generator::createMission` | 172 | 176 | +4 | 26 / 26 |
| **total** | | | **0** | |

The `+0xc` comes entirely from `HangarWindow::render` (`912 -> 924`, i.e.
`+4` from the first pad and `+8` from the second). Every other record is
unchanged.

## Exact edits (`src/game/ui/HangarWindow.cpp`)

Both edits are semantically-null locals that clang optimises away. They change
only the EH call-site grouping: the original has separate landing pads for
consecutive potentially-throwing calls where our IR currently merges them.

```diff
@@ -574,6 +574,7 @@ void HangarWindow::render() {
                                     int right, left;
                                     if (tab == 1) {
                                         right = item->item->getStationAmount();
+                                        int pad1 = 0; (void) pad1;
                                         left = item->item->getAmount();
                                     } else {
@@ -732,6 +733,7 @@ void HangarWindow::render() {
                         buttonOffset = 0;
                     } else if (item->isSlot()) {
                         name = *Globals::gameText->getText(174);
+                        int pad2 = 0; (void) pad2;
                         if (tab == 4 && i == items->size() - 1) {
```

Measured effects, one at a time (all with `.text` `0x1337c8`, `.ARM.exidx`
`0x6f70`, `.rodata` base `0x1c9e10`, `HangarWindow::render` `0x25e4`):

| edit | `.ARM.extab` | render LSDA | render code |
|---|---:|---:|---:|
| HEAD | `0x2628c` | 912 | `0x25e4` |
| `pad1` only | `0x26290` (+4) | 916 | `0x25e4` |
| `pad1` + `pad2` | **`0x26298`** (+0xc) | 924 | `0x25e4` |

`pad1` splits the `getStationAmount()` / `getAmount()` pair; `pad2` splits the
`getText(174)` call from the `buttons[23]` block. Each adds one call-site row
(4 B of LSDA) without changing the emitted instruction stream.

## Pad A/B sweep (for the record)

All pads are `int padN = 0; (void) padN;` inserted immediately after the named
statement in `HangarWindow::render`, on top of `pad1`. Measured after a full
`link_so`:

| pad site | `.ARM.extab` | render code | verdict |
|---|---:|---:|---|
| `buttons[8]->setVisible` | `0x2629c` (+0xc) | `0x25e8` (+4) | code grows |
| `buttons[9]->setVisible` | `0x2629c` (+0xc) | `0x25e8` (+4) | code grows |
| `buttons[23]->setVisible` | `0x26298` (+8) | `0x25e8` (+4) | code grows |
| `buttons[1]->setVisible` | `0x2629c` (+0xc) | `0x25e4` (0) | overshoots |
| `buttons[6]->setVisible` | `0x2629c` (+0xc) | `0x25e8` (+4) | code grows |
| `buttons[5]->setVisible` | `0x2629c` (+0xc) | `0x25e8` (+4) | code grows |
| `buttons[10]->setVisible` | `0x2629c` (+0xc) | `0x25dc` (−8) | code shrinks |
| `buttons[7]->setVisible` | `0x26298` (+8) | `0x25e8` (+4) | code grows |
| `buttons[0]->setVisible` | `0x26298` (+8) | `0x25e8` (+4) | code grows |
| `buttons[11]->setVisible` | `0x26290` (+0 vs pad1) | `0x25e4` (0) | neutral |
| `getText(item->ship->getIndex()+913)` | `0x2629c` (+0xc) | `0x25e4` (0) | overshoots |
| **`getText(174)`** | **`0x26298` (+8)** | **`0x25e4` (0)** | **landed** |

The `getText(item->item->getIndex()+1274)` site was also tried but is not a
local pad: it changed the whole function body (`render` `0x25e4 -> 0x2634`)
and was discarded.

## Why `byte_exact` did not jump

The `.rodata` base was already `0x1c9e10` before this pass (the
`text+exidx+extab` sum was inside the 16-byte alignment window), so making
`.ARM.extab` exact does not move any section. `.rodata` still has its `+0x40`
content delta from the 12 per-TU literal/table differences; the first byte
divergence is still the `FMOD_GOF2` literal order. Consequently the expected
literal-pool jump does not happen until that content delta is fixed. The
section-size goal (`.ARM.extab` exact) is independent of that campaign.

## Reproduction

```sh
cd /home/debian/GalaxyOnFire2/wt/w3391
cmake --build cmake-build-match --target link_so -j8
cmake --build cmake-build-match --target check   -j8   # RATCHET PASS, byte 3921
cmake --build cmake-build-match --target drift   -j8   # 0 fields
cmake --build cmake-build-match --target diff    -j8   # SYMBOL PARITY

RE=.cache/ndk/android-ndk-r18b/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-readelf
$RE -SW cmake-build-match/libgof2hdaa.so | grep -E '\.text |\.ARM.exidx|\.ARM.extab|\.rodata '
# .text 0x1337c8, .ARM.exidx 0x6f70, .ARM.extab 0x26298, .rodata @0x1c9e10
```

Per-function table (with the Thumb-bit correction described above):

```sh
python3 _work/reconstructions/exidx_table.py --diff \
    ../_work/bins/android_2.0.16_libgof2hdaa.so cmake-build-match/libgof2hdaa.so
```
